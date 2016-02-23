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

#include "anonymizepanel.h"
#include <api/dicom/idicommanager.h>
#include <api/dicom/dcmdictionary.h>
#include <main/entorno.h>
#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/iconfigurationcontroller.h>
#include <api/controllers/ipacscontroller.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <commands/comandoexportacion.h>
#include <wx/ginkgostyle/ginkgostyle.h>
/**dcmtk dictionary...*/
#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>

#include <wx/msgdlg.h>

/**/
namespace GNC
{
	namespace GUI 
	{
		class AnonymizeAddTagDialog: public AnonymizeAddTagDialogBase
		{
		public:
			AnonymizeAddTagDialog(wxWindow* pParent): AnonymizeAddTagDialogBase(pParent)
			{
				DcmDataDictionary& globalDataDict = dcmDataDict.wrlock();
				
				wxArrayString listOfStrings;
				for (DcmHashDictIterator it = globalDataDict.normalBegin(); it != globalDataDict.normalEnd(); ++it) 
				{
					DcmDictEntry entry(*(*it));
					listOfStrings.push_back(wxString::FromUTF8(entry.getTagName()));
				}
				dcmDataDict.unlock();

				listOfStrings.Sort();
				m_pTag->Append(listOfStrings);

				m_pBody->Layout();
				Layout();
			}

			virtual void OnTagSelection(wxCommandEvent& /*evt*/)
			{
				if (m_pTag->GetSelection() != 0) {
					wxString tagName = m_pTag->GetStringSelection();
					const DcmDataDictionary& globalDataDict = dcmDataDict.rdlock();
					const DcmDictEntry* entry = globalDataDict.findEntry(tagName.ToUTF8());
					dcmDataDict.unlock();
					if (entry != NULL) {
						m_pTextGroup->SetValue(wxString::Format(wxT("%04x"),entry->getGroup()));
						m_pTextElement->SetValue(wxString::Format(wxT("%04x"),entry->getElement()));
					}
				}
			}

			std::string getKey()
			{
				wxString key;
				long group,element;
				if (m_pTextGroup->GetValue().ToLong(&group, 16) && m_pTextElement->GetValue().ToLong(&element, 16) && group >= 0 && group < 0xffff && element >= 0 && element < 0xffff) {
					key = wxString::Format(wxT("%04x|%04x"), group, element);
				}
				return std::string(key.ToUTF8());
			}

			std::string getValue()
			{
				return std::string(m_pTextValue->GetValue().ToUTF8());
			}

			virtual void OnOkClick(wxCommandEvent &evt)
			{
				long group,element;
				if (m_pTextGroup->GetValue().ToLong(&group, 16) && m_pTextElement->GetValue().ToLong(&element, 16) && group >= 0 && group < 0xffff && element >= 0 && element < 0xffff) {
					evt.Skip(true);
				} else {
					wxMessageBox(_("Element field or group field have invalid values"),_("Info"),wxOK | wxICON_INFORMATION, this);
					evt.Skip(false);
				}
			}
		};
	}
}

/**********************************************************/

GNC::GUI::AnonymizePanel::AnonymizePanel(wxWindow* pParent): GNC::GUI::AnonymizePanelBase(pParent)
{
}

GNC::GUI::AnonymizePanel::AnonymizePanel(wxWindow* pParent, const std::list<long>& seriesPk): GNC::GUI::AnonymizePanelBase(pParent)
{
	if (seriesPk.empty()) {
		LOG_ERROR("AnonymizePanel", "you have to select at least one series to anonymize");
	} else {
		InitSeries(seriesPk.front());
	}
}

GNC::GUI::AnonymizePanel::~AnonymizePanel()
{
	m_pTagsList->Disconnect(wxEVT_PG_CHANGED,wxPropertyGridEventHandler(AnonymizePanel::OnPropertyChanged),NULL,this);
	m_pTagsList->Disconnect(wxEVT_PG_DOUBLE_CLICK,wxPropertyGridEventHandler(AnonymizePanel::OnPropertyDobleClick),NULL,this);
}

void GNC::GUI::AnonymizePanel::InitSeries(long pkSeries)
{
	GNC::GCS::HistoryController::LightFileModelList fileModels;
	GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels(pkSeries, fileModels);

	GIL::DICOM::IDICOMManager*	pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
	pDICOMManager->CargarFichero(fileModels.front().real_path, m_base);
	SetTags(m_base, m_pTagsList->GetRoot(), pDICOMManager);
	GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);

	MapOfCheck[GKDCM_PatientName] = m_pCheckNombrePaciente;
	MapOfCheck[GKDCM_PatientID] = m_pCheckIdPaciente;
	MapOfCheck[GKDCM_InstitutionName] = m_pCheckInstituciones;
	MapOfCheck[GKDCM_ReferringPhysicianName] = m_pReferringPhysician;
	MapOfCheck[GKDCM_StudyDescription] = m_pCheckComentarios;
	MapOfCheck[GKDCM_SeriesDescription] = m_pCheckComentarios;
	MapOfCheck[GKDCM_ImageComments] = m_pCheckComentarios;
	
	GNC::GCS::IConfigurationController::TListGroups groups;
	GNC::GCS::IConfigurationController::Instance()->readGroupUser("/Core/AnonymizationSelection", groups);
	for (GNC::GCS::IConfigurationController::TListGroups::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		if ((*it).find("tag") != (*it).end() && (*it).find("value") != (*it).end()) {
			Anonymize((*it)["tag"], true, (*it)["value"], false);
		}
	}

	m_pTagsList->Connect(wxEVT_PG_CHANGED,wxPropertyGridEventHandler(AnonymizePanel::OnPropertyChanged),NULL,this);
	m_pTagsList->Connect(wxEVT_PG_DOUBLE_CLICK,wxPropertyGridEventHandler(AnonymizePanel::OnPropertyDobleClick),NULL,this);
}

void GNC::GUI::AnonymizePanel::SetTags(GIL::DICOM::DicomDataset & base,wxPGProperty* idPadre,GIL::DICOM::IDICOMManager*	pDICOMManager) 
{
	for(GIL::DICOM::ListaTags::iterator it = base.tags.begin(); it!= base.tags.end(); ++it){
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription((*it).first).c_str());
		wxStringProperty* prop = NULL;
		wxString valor;
		if((*it).second.size()>100) {
			valor=wxT("Datos");
		} else {
			valor = wxString::FromUTF8((*it).second.c_str());
		}

		if(helpString.size()>0){
			helpString = wxT("(") + wxString::FromUTF8((*it).first.c_str()) + wxT(") ") + helpString;
			prop = new wxStringProperty(helpString,
				wxPG_LABEL,valor);
		}else{
			prop = new wxStringProperty(wxString::FromUTF8((*it).first.c_str()),
				wxPG_LABEL,valor);
		}
		prop->SetHelpString(wxString::FromUTF8((*it).first.c_str()));
		m_pTagsList->AppendIn(idPadre, prop);
	}

	for(GIL::DICOM::DicomDataset::DatasetList::iterator it = base.secuencias.begin(); it!=base.secuencias.end(); ++it){
		wxStringProperty* prop = NULL;
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription((*it).tagName).c_str());
		if(helpString.size()>0){
			helpString = wxT("(") + wxString::FromUTF8((*it).tagName.c_str()) + wxT(") ") + helpString;
			prop = new wxStringProperty(helpString,
				wxPG_LABEL,wxEmptyString);
		}else{
			prop = new wxStringProperty(wxString::FromUTF8((*it).tagName.c_str()),
				wxPG_LABEL,wxEmptyString);
		}
		prop->SetHelpString(wxString::FromUTF8((*it).tagName.c_str()));
		if(m_pTagsList->GetProperty(prop->GetName()) != NULL) {
			////////////////////////////////////std::cout<<"hasdfasdf";
		}
		wxPGProperty* pIdSequencia = m_pTagsList->AppendIn(idPadre,prop);

		//recursion
		int i=0;
		for(GIL::DICOM::DicomDataset::DatasetList::iterator it2 = (*it).items.begin(); it2!= (*it).items.end(); ++it2, ++i) {
			wxPGProperty* pIdItem = m_pTagsList->AppendIn(pIdSequencia,new wxStringProperty(wxString::Format(wxT("Item %d"),i),wxPG_LABEL,wxEmptyString));
			SetTags((*it2),pIdItem,pDICOMManager);
		}
	}
}

void GNC::GUI::AnonymizePanel::OnCheckNombreApellidos( wxCommandEvent& /*event*/ )
{
	//se anonimiza 0010|0010 (nombre del paciente)
	std::string clave("0010|0010");
	Anonymize(clave,m_pCheckNombrePaciente->IsChecked());
}

void GNC::GUI::AnonymizePanel::OnCheckIdPaciente( wxCommandEvent& /*event*/ )
{
	//se anonimiza 0010|0020" (id del paciente)
	std::string clave("0010|0020");
	Anonymize(clave,m_pCheckIdPaciente->IsChecked());
}

void GNC::GUI::AnonymizePanel::OnCheckInstituciones( wxCommandEvent& /*event*/ )
{
	//se anonimiza (0008,0080) Institution Name
	std::string clave("0008|0080");
	Anonymize(clave,m_pCheckInstituciones->IsChecked());
}

void GNC::GUI::AnonymizePanel::OnCheckPhysician( wxCommandEvent& /*event*/ )
{
	//referring physician name
	std::string clave("0008|0090");
	Anonymize(clave,m_pReferringPhysician->IsChecked());
}

void GNC::GUI::AnonymizePanel::OnCheckComentarios( wxCommandEvent& /*event*/ )
{
	//descripcion del estudio, serie e imagenes
	//estudio
	{
		std::string clave("0008|1030");
		Anonymize(clave,m_pCheckComentarios->IsChecked());
	}
	//serie
	{
		std::string clave("0008|103e");
		Anonymize(clave,m_pCheckComentarios->IsChecked());
	}
	//imagen
	{
		std::string clave("0020|4000");
		Anonymize(clave,m_pCheckComentarios->IsChecked());
	}
}

void GNC::GUI::AnonymizePanel::OnAddNewTag( wxCommandEvent &)
{
	AnonymizeAddTagDialog dlg(this);
	if (dlg.ShowModal() == wxID_OK) {
		Anonymize(dlg.getKey(), true, dlg.getValue());
	}
}

void GNC::GUI::AnonymizePanel::Anonymize(const std::string& clave, bool anonimizar)
{
	wxString value = wxEmptyString;

	if(!anonimizar){
		std::string tmp;
		if(m_base.getTag(clave,tmp)){
			value = wxString::FromUTF8(tmp.c_str());
		}
	}

	Anonymize(clave, anonimizar, std::string(value.ToUTF8()));
}

void GNC::GUI::AnonymizePanel::Anonymize(const std::string& clave, bool anonimizar, const std::string& value, bool anonymizeIfNotExists )
{
	if (MapOfCheck.find(clave) != MapOfCheck.end()) {
		MapOfCheck[clave]->SetValue(anonimizar);
	}
	wxString valueWx = wxString::FromUTF8(value.c_str());
	for (wxPropertyGridIterator it = m_pTagsList->GetIterator(); ! it.AtEnd(); ++it) {
		wxPGProperty* prop = it.GetProperty();
		if(prop->GetHelpString() == wxString::FromUTF8(clave.c_str())){
			prop->SetValueFromString(valueWx);
			if(anonimizar){
				m_pTagsList->SetPropertyCell(prop,0,prop->GetLabel(),wxNullBitmap,*wxWHITE,*wxRED);
				m_pTagsList->SetPropertyCell(prop,1,prop->GetValue(),wxNullBitmap,*wxWHITE,*wxRED);
			}else{
				m_pTagsList->SetPropertyCell(prop,0,prop->GetLabel(), wxNullBitmap, *wxBLACK, *wxWHITE);
				m_pTagsList->SetPropertyCell(prop,1,prop->GetValue(), wxNullBitmap, *wxBLACK, *wxWHITE);
			}
			prop->ChangeFlag(wxPG_PROP_MODIFIED,anonimizar);
			m_pTagsList->RefreshProperty(prop);
			m_pTagsList->SelectProperty(prop, true);
			return;
		}
	}
	if (anonymizeIfNotExists) {
		//key not found... add it to property grid
		wxStringProperty* prop = NULL;
		GIL::DICOM::IDICOMManager*	pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription(clave).c_str());
		if(helpString.size()>0) {
			helpString = wxT("(") + wxString::FromUTF8(clave.c_str()) + wxT(") ") + helpString;
			prop = new wxStringProperty(helpString,
				wxPG_LABEL,wxEmptyString);
		}else{
			prop = new wxStringProperty(wxString::FromUTF8(clave.c_str()),
				wxPG_LABEL,wxEmptyString);
		}
		prop->SetHelpString(wxString::FromUTF8(clave.c_str()));
		GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);

		prop->SetValue(wxString::FromUTF8(value.c_str()));
		prop->ChangeFlag(wxPG_PROP_MODIFIED,anonimizar);
		if(anonimizar){
			m_pTagsList->SetPropertyCell(prop,0,prop->GetLabel(),wxNullBitmap,*wxWHITE,*wxRED);
			m_pTagsList->SetPropertyCell(prop,1,prop->GetValue(),wxNullBitmap,*wxWHITE,*wxRED);
		}
		m_pTagsList->AppendIn(m_pTagsList->GetRoot(), prop);
		m_pTagsList->SelectProperty(prop, true);
	}
}


void GNC::GUI::AnonymizePanel::RemoveFromTable(const std::string& clave)
{
	for (wxPropertyGridIterator it = m_pTagsList->GetIterator(); ! it.AtEnd(); ++it) {
		wxPGProperty* prop = it.GetProperty();
		if(prop->GetHelpString() == wxString::FromUTF8(clave.c_str())){
			m_pTagsList->DeleteProperty(prop);
			break;
		}
	}
}

void GNC::GUI::AnonymizePanel::GetTagsAnonymized(GIL::DICOM::DicomDataset & base)
{
	base.tags.clear();
	GNC::GCS::IConfigurationController::TListGroups configGroups;
	for (wxPropertyGridIterator it = m_pTagsList->GetIterator(); ! it.AtEnd(); ++it) {
		wxPGProperty* prop = it.GetProperty();
		if(prop->GetFlags() & wxPG_PROP_MODIFIED){
			std::string clave(prop->GetHelpString().ToUTF8());
			std::string valor(prop->GetValueAsString().ToUTF8());
			base.tags[clave] = valor;

			GNC::GCS::IConfigurationController::TMapValues group;
			group["tag"] = clave;
			group["value"] = valor;
			configGroups.push_back(group);
		}
	}
	GNC::GCS::IConfigurationController::Instance()->writeGroupUser("/Core/AnonymizationSelection", configGroups);
}

bool GNC::GUI::AnonymizePanel::AnonymizePrivateTags()
{
	return m_pCheckAtributosPrivadosGinkgo->IsChecked();
}

void GNC::GUI::AnonymizePanel::OnPropertyChanged(wxPropertyGridEvent& event)
{
	wxPGProperty* selected= event.GetProperty();
	m_pTagsList->SetPropertyCell(selected,0,selected->GetLabel(),wxNullBitmap,*wxWHITE,*wxRED);
	m_pTagsList->SetPropertyCell(selected,1,selected->GetValue(),wxNullBitmap,*wxWHITE,*wxRED);
}

void GNC::GUI::AnonymizePanel::OnPropertyDobleClick(wxPropertyGridEvent& event)
{
	std::string clave(event.GetProperty()->GetHelpString().ToUTF8());
	bool isModified = (event.GetProperty()->GetFlags() & wxPG_PROP_MODIFIED);
	Anonymize(clave,!isModified);
}


