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


#include <vector>
#include <sstream>

#include <wx/dialog.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/imaglist.h>

#include <main/controllers/configurationcontroller.h>
#include <wx/msgdlg.h>
#include "opendialogsbase.h"

#include "opendialogs.h"
#include "dicomdirtablemodel.h"

#include <main/entorno.h>
#include <main/controllers/commandcontroller.h>
#include <commands/comandoincluirhistorial.h>
#include <api/dicom/imodelodicom.h>
#include <resources/ginkgoresourcesmanager.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <resources/ginkgoresourcesmanager.h>

#define ICONO_ESTUDIO 0
#define ICONO_SERIE 1
#define ICONO_HOMBRE 2
#define ICONO_MUJER 3
#define ICONO_OTRO 4
#define SIZE_ICONOS 16

#define COLUMNA_DCMDIR_FECHA 2
#define COLUMNA_DCMDIR_MODALIDAD 1
#define COLUMNA_DCMDIR_MEDICO 3
#define COLUMNA_DCMDIR_ACCNUMBER 4
#define COLUMNA_DCMDIR_UID 5
namespace GNC
{
namespace GUI
{
////////////////////////////////////////////////////
////////////DIALOGO DICOMDIR////////////////////////
////////////////////////////////////////////////////
DialogoDicomDir::DialogoDicomDir(wxWindow* pParent, const std::string& pathDicomDir):
        DialogoDicomDirBase(pParent),
        m_pDicomDirTableModel(new DicomDirTableModel())
{
        m_pResultsView ->AssociateModel(m_pDicomDirTableModel.get());
        m_pResultsView->AppendToggleColumn(GinkgoResourcesManager::MenuIcons::GetIcoAbrir(), DicomDirTableModel::COL_CHECK, wxDATAVIEW_CELL_ACTIVATABLE, 50, wxALIGN_CENTER);
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Patient name"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),DicomDirTableModel::COL_PATIENT_NAME, 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Patient id"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),DicomDirTableModel::COL_PATIENT_ID, 100, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Modality"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),DicomDirTableModel::COL_MODALITY, 60, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Date"), new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT),DicomDirTableModel::COL_DATE_TIME, 120, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Description"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),DicomDirTableModel::COL_DESCRIPTION, 110, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
        m_pResultsView->AppendColumn(new wxDataViewColumn(_("Accession number"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),DicomDirTableModel::COL_ACCNUMBER, 128, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));

        wxString pathWx = FROMPATH(pathDicomDir);
        if (wxFile::Exists(pathWx)) {
                if (!m_pDicomDirTableModel->LoadModel(pathDicomDir)) {
                        wxMessageBox(_("There was an error reading DICOMDIR file"), _("Info"),
                                     wxOK | wxICON_INFORMATION, this);
                }
        } else {
                wxMessageBox(_("Select a valid DICOMDIR file"), _("Info"),
                             wxOK | wxICON_INFORMATION, this);
        }
        m_pBody->Layout();
        Layout();
        m_pResultsView->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DialogoDicomDir::OnItemActivation ), NULL, this);
        m_pResultsView->Connect(wxEVT_DATAVIEW_COLUMN_HEADER_CLICK, wxDataViewEventHandler( DialogoDicomDir::OnColumnHeaderClick ), NULL, this);
        m_pResultsView->Connect(wxEVT_CHAR, wxKeyEventHandler(DialogoDicomDir::OnListChar), NULL, this);
}

DialogoDicomDir::~DialogoDicomDir()
{
}

void DialogoDicomDir::OnCopyClick(wxCommandEvent&)
{
        Acquire(true);
}

void DialogoDicomDir::OnLinkClick(wxCommandEvent&)
{
        Acquire(false);
}

void DialogoDicomDir::OnItemActivation(wxDataViewEvent& event)
{
        if (!event.GetItem().IsOk()) {
                return;
        }
        wxDataViewItem item = event.GetItem();

        wxVariant curValue;
        m_pDicomDirTableModel->GetValue(curValue, item, DicomDirTableModel::COL_CHECK);
        m_pDicomDirTableModel->SetValue(!curValue.GetBool(), item, DicomDirTableModel::COL_CHECK);
        m_pDicomDirTableModel->ItemChanged(item);
}

void DialogoDicomDir::OnColumnHeaderClick(wxDataViewEvent& event)
{
        //minus or equal is because in GTK we receive -1 as column...
        if (event.GetColumn() <= 0) {
                m_pDicomDirTableModel->ToggleAllSelections();
                event.Skip(false);
        } else {
                event.Skip(true);
        }
}

void DialogoDicomDir::OnListChar(wxKeyEvent&event)
{
        if (event.GetKeyCode() == WXK_SPACE) {
                wxDataViewItemArray selected;
                m_pResultsView->GetSelections(selected);
                wxVariant curValue;
                for (wxDataViewItemArray::iterator it = selected.begin(); it != selected.end(); ++it) {
                        m_pDicomDirTableModel->GetValue(curValue, *it, DicomDirTableModel::COL_CHECK);
                        m_pDicomDirTableModel->SetValue(!curValue.GetBool(), *it, DicomDirTableModel::COL_CHECK);
                }
                m_pDicomDirTableModel->ItemsChanged(selected);
                event.Skip(false);
        } else {
                event.Skip(true);
        }
}

void DialogoDicomDir::Acquire(bool copy)
{
        //adquirimos desde dicomdir
        std::list<std::string> listaPaths;
        m_pDicomDirTableModel->GetSelectedPaths(listaPaths);

        if(listaPaths.empty()) {
                wxMessageBox(_("Select dicom dir files you want to acquire"), _("Info"),
                             wxOK | wxICON_INFORMATION, this);
                return;
        } else {
                //se lanza el comando de incluir en el historial
                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = NULL;
                if (copy) {
                        pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(listaPaths, false, GNC::GCS::IHistoryController::TAA_COPY);
                } else {
                        pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(listaPaths, false, GNC::GCS::IHistoryController::TAA_LINK);
                }
                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
                GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Storing in the history..."),pCmd, NULL);

                Close();
        }
}
///////////////////////////////////////END OF DIALOGODICOMDIR
SelectDrive::SelectDrive(wxWindow* pParent, const TListOfDrives& drives) : SelectDriveBase(pParent)
{
        m_drives = drives;
        for (TListOfDrives::iterator it = m_drives.begin(); it != m_drives.end(); ++it) {
                m_pUnits->Append(wxString::FromUTF8((*it).label.c_str()));
        }
        if (m_pUnits->GetCount() > 0) {
                m_pUnits->Select(0);
        }
}
SelectDrive::~SelectDrive()
{
}

void SelectDrive::SetMessage(const wxString& label)
{
        m_pLabel->SetLabel(label);
        m_pPanelMessage->Layout();
        m_pBody->Layout();
}

std::string SelectDrive::GetSelectedDrive()
{
        TListOfDrives::iterator it = m_drives.begin();
        for (int i = 0; i < m_pUnits->GetSelection(); ++i, ++it);
        return (*it).id;
}

CopyOrLink::CopyOrLink(wxWindow* pParent) : CopyOrLinkBase(pParent, wxID_ANY, _("Copy or link"), wxDefaultPosition, wxDefaultSize, wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU)
{
        m_copy = true;
        m_pPanelMessage->Layout();
        m_pBody->Layout();
        CentreOnParent();
}

CopyOrLink::~CopyOrLink()
{
}

bool CopyOrLink::HasSelectedCopy()
{
        return m_copy;
}

void CopyOrLink::OnCopy(wxCommandEvent &/*event*/)
{
        m_copy = true;
        EndModal(wxID_OK);
}

void CopyOrLink::OnLink(wxCommandEvent &/*event*/)
{
        m_copy = false;
        EndModal(wxID_OK);
}
}
}
