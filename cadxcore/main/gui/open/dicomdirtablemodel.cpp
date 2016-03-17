/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <map>
#include "dicomdirtablemodel.h"
#include <main/controllers/controladorlog.h>
#include <set>
#include <wx/imaglist.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/pacscontroller.h>

#define SIZE_ICONOS 16


/////////////////////////////
GNC::GUI::DicomDirTableModel::DicomDirTableModel()
{
}


GNC::GUI::DicomDirTableModel::~DicomDirTableModel()
{
}

unsigned int GNC::GUI::DicomDirTableModel::GetColumnCount() const
{
        return COL_DICOM_DIR_MAX;
}

wxString GNC::GUI::DicomDirTableModel::GetColumnType( unsigned int/* col*/ ) const
{
        return wxT("string");
}

void GNC::GUI::DicomDirTableModel::GetValue( wxVariant &variant, const wxDataViewItem &item, unsigned int col ) const
{
        if (!item.IsOk()) {
                return;
        }
        //if it's a study...
        const IModeloEstudio* pNode = (const IModeloEstudio*)(item.GetID());
        switch (col) {
        case COL_CHECK: {
                if (checkedStudies.find(pNode) == checkedStudies.end()) {
                        variant = true;
                } else {
                        variant = (*checkedStudies.find(pNode)).second;
                }
        }
        return;
        case COL_PATIENT_NAME:
                variant = wxString::FromUTF8(pNode->GetPatient()->GetNombre().c_str());
                break;
        case COL_PATIENT_ID:
                variant = wxString::FromUTF8(pNode->GetPatient()->GetUID().c_str());
                break;
        case COL_MODALITY: {
                if (pNode->GetModalidad().empty()) {
                        //get modalities from series..
                        std::ostringstream ostr;
                        std::set<std::string> distinctModalities;
                        for (IModeloEstudio::ListaSeriesType::const_iterator itSeries = pNode->ListaSeries().begin(); itSeries != pNode->ListaSeries().end(); ++itSeries) {
                                distinctModalities.insert((*itSeries).GetTipo());
                        }
                        for (std::set<std::string>::iterator it = distinctModalities.begin(); it != distinctModalities.end(); ++it) {
                                if(it != distinctModalities.begin()) {
                                        ostr << "/";
                                }
                                ostr << *it;
                        }
                        variant = wxString::FromUTF8(ostr.str().c_str());
                } else {
                        variant = wxString::FromUTF8(pNode->GetModalidad().c_str());
                }
        }
        break;
        case COL_DESCRIPTION:
                variant = wxString::FromUTF8(pNode->GetDescripcion().c_str());
                break;
        case COL_DATE_TIME: {
                std::ostringstream ostr;
                ostr << pNode->GetFecha() << pNode->GetHora();
                wxDateTime studyDateTime;
                studyDateTime.ParseFormat(wxString::FromUTF8( ostr.str().c_str() ).GetData(), wxT("%Y%m%d%H%M%S"), wxDefaultDateTime);
                if(studyDateTime.IsValid()) {
                        variant = wxString(studyDateTime.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
                } else {
                        variant = _("00/00/0000 00:00:00");
                }
        }
        break;
        case COL_ACCNUMBER:
                variant = wxString::FromUTF8(pNode->GetAccNumber().c_str());
                break;
        default:
                LOG_ERROR("DicomDirTableModel", "wrong column id " << col);
                break;
        }
}


bool GNC::GUI::DicomDirTableModel::SetValue( const wxVariant &variant,const wxDataViewItem &item, unsigned int col )
{
        //model cant be modified...
        if (!item.IsOk() || col != COL_CHECK) {
                return false;
        }
        const IModeloEstudio* pNode = (const IModeloEstudio*)(item.GetID());
        checkedStudies[pNode] = variant.GetBool();
        return true;
}

bool GNC::GUI::DicomDirTableModel::IsEnabled( const wxDataViewItem &/*item*/, unsigned int/* col*/ ) const
{
        return true;
}

wxDataViewItem GNC::GUI::DicomDirTableModel::GetParent( const wxDataViewItem &/*item*/ ) const
{
        return wxDataViewItem(0);
}

bool GNC::GUI::DicomDirTableModel::IsContainer( const wxDataViewItem &item ) const
{
        return !item.IsOk();
}

bool GNC::GUI::DicomDirTableModel::HasContainerColumns (const wxDataViewItem &/*item*/) const
{
        return true;
}

bool GNC::GUI::DicomDirTableModel::HasValue (const wxDataViewItem &item, unsigned /*col*/) const
{
        if (!item.IsOk()) {
                return false;
        }
        return true;
}


unsigned int GNC::GUI::DicomDirTableModel::GetChildren( const wxDataViewItem &parent, wxDataViewItemArray &array ) const
{
        if (!parent.IsOk()) {
                //root items
                //we will use pointers to IStudy as wxDataViewItems
                for (IModeloDicom::ListaPacientesType::const_iterator itPatient = dicomModel.ListaPacientes().begin(); itPatient != dicomModel.ListaPacientes().end(); ++itPatient) {
                        for (IModeloPaciente::ListaEstudiosType::const_iterator itStudy = (*itPatient).ListaEstudios().begin(); itStudy != (*itPatient).ListaEstudios().end(); ++itStudy) {
                                const IModeloEstudio& study = *itStudy;
                                array.Add( wxDataViewItem( (void*)&study ) );
                        }
                }
                return array.Count();
        } else {
                return 0;
        }
}

int GNC::GUI::DicomDirTableModel::Compare( const wxDataViewItem &item1, const wxDataViewItem &item2,
                unsigned int column, bool ascending ) const
{
        if (column == COL_DATE_TIME && item1.IsOk() && item2.IsOk()) {
                const IModeloEstudio* node1 = (const IModeloEstudio*) item1.GetID();
                const IModeloEstudio* node2 = (const IModeloEstudio*) item1.GetID();

                int res = node1->GetFecha().compare(node2->GetFecha());
                if (res == 0) {
                        res = node1->GetHora().compare(node2->GetHora());
                }
                if (!ascending) {
                        if (res == 1) {
                                res = -1;
                        } else if (res == -1) {
                                res = 1;
                        }
                }
                return res;
        }

        return wxDataViewModel::Compare( item1, item2, column, ascending );
}

void GNC::GUI::DicomDirTableModel::GetSelectedPaths(TListPaths& paths)
{
        for (IModeloDicom::ListaPacientesType::const_iterator itPatient = dicomModel.ListaPacientes().begin(); itPatient != dicomModel.ListaPacientes().end(); ++itPatient) {
                for (IModeloPaciente::ListaEstudiosType::const_iterator itStudy = (*itPatient).ListaEstudios().begin(); itStudy != (*itPatient).ListaEstudios().end(); ++itStudy) {
                        const IModeloEstudio& study = (*itStudy);
                        bool selected = true; //default value is true
                        if (checkedStudies.find(&study) != checkedStudies.end()) {
                                selected = (*checkedStudies.find(&study)).second;
                        }
                        if (selected) {
                                for (IModeloEstudio::ListaSeriesType::const_iterator itSeries = study.ListaSeries().begin(); itSeries != study.ListaSeries().end(); ++itSeries) {
                                        const IModeloSerie& series = (*itSeries);
                                        for (IModeloSerie::ListaImagenesType::const_iterator itImages = series.ListaImagenes().begin(); itImages != series.ListaImagenes().end(); ++itImages) {
                                                paths.push_back((*itImages).GetPathImagen());
                                        }
                                }
                        }
                }
        }

}

IModeloDicom& GNC::GUI::DicomDirTableModel::GetModel()
{
        return dicomModel;
}

void GNC::GUI::DicomDirTableModel::ToggleAllSelections()
{
        //we will set false or not first value
        bool newValue = false;
        if (!checkedStudies.empty()) {
                newValue = !checkedStudies.begin()->second;
        }
        wxDataViewItemArray itemsChanged;
        for (IModeloDicom::ListaPacientesType::const_iterator itPatient = dicomModel.ListaPacientes().begin(); itPatient != dicomModel.ListaPacientes().end(); ++itPatient) {
                for (IModeloPaciente::ListaEstudiosType::const_iterator itStudy = (*itPatient).ListaEstudios().begin(); itStudy != (*itPatient).ListaEstudios().end(); ++itStudy) {
                        const IModeloEstudio& study = (*itStudy);
                        checkedStudies[&study] = newValue;
                        itemsChanged.Add( wxDataViewItem( (void*) &(*itStudy)) );
                }
        }
        //all items has been modfied...
        ItemsChanged(itemsChanged);
}

bool GNC::GUI::DicomDirTableModel::LoadModel(const std::string& pathDicomDir)
{
        Cleared();
        if(!GIL::DICOM::PACSController::Instance()->LeerDicomDir(pathDicomDir,&dicomModel)) {
                return false;
        } else {
                //in gtk implementation we have to to this...
                wxDataViewItemArray childrenAdded;
                if (GetChildren(wxDataViewItem(NULL), childrenAdded) > 0) {
                        ItemsAdded(wxDataViewItem(NULL), childrenAdded);
                }
                return true;
        }
}

