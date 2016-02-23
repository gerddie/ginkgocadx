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

#include <api/globals.h>

#include "paneltags.h"

#include <api/controllers/ieventscontroller.h>

#include <api/dicom/idicommanager.h>
#include <api/istudycontext.h>

#include <main/controllers/pacscontroller.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorlog.h>

GNC::GUI::PanelTags::PanelTags(wxWindow* pPadre, bool hideOnClose ) : PanelTagsBase(pPadre)
{
	m_hideOnClose = hideOnClose;
	acquisitionGroup = NULL;
	imagePresentationGroup = NULL;
	metadataGroup = NULL;
	textGroup = NULL;
	otherGroup = NULL;
	patientGroup = NULL;
	identificationGroup = NULL;
	relationshipGroup = NULL;
	m_currentPath ="";
}

GNC::GUI::PanelTags::~PanelTags()
{
}

bool GNC::GUI::PanelTags::IsVisiblePanelTags()
{
	return IsShown();
}

void GNC::GUI::PanelTags::MostrarOcultarPanelTags()
{
	if (IsShown()) {
		Hide();
	} else {
		//aui bug...
		Show();
	}
}

void GNC::GUI::PanelTags::SetView(GNC::GCS::IVista* pView)
{
	if(pView == NULL || pView->GetCurrentDICOM() == "" || pView->GetCurrentDICOM() == m_currentPath)
	{		
		return;
	}
	CreateCategories();
	m_currentPath = pView->GetCurrentDICOM();

	GIL::DICOM::TipoMetaInfo info;
	GIL::DICOM::DicomDataset base;
	
	GIL::DICOM::IDICOMManager*	pDICOMManager = GIL::DICOM::PACSController::Instance()->CrearInstanciaDeDICOMManager();
	try {
		pDICOMManager->CargarFichero(m_currentPath, base, false);
		pDICOMManager->CargarMetaInfo(m_currentPath, info);
	} catch (...) {
		LOG_ERROR("PanelTags", "Error loading path " + m_currentPath);
	}
	
	SetInfoTags(info, pDICOMManager);
	
	SetTags(base, m_pTags->GetRoot(), pDICOMManager);
	GIL::DICOM::PACSController::Instance()->LiberarInstanciaDeDICOMManager(pDICOMManager);
}

bool GNC::GUI::PanelTags::SetSeriesPk(long pk)
{
	CreateCategories();
	
	GNC::GCS::HistoryController::StringList paths = GNC::GCS::HistoryController::Instance()->GetSeriesSortedPaths(pk);
	if (paths.empty()) {
		return false;
	}
	m_currentPath = paths.front();

	GIL::DICOM::TipoMetaInfo info;
	GIL::DICOM::DicomDataset base;
	
	GIL::DICOM::IDICOMManager*	pDICOMManager = GIL::DICOM::PACSController::Instance()->CrearInstanciaDeDICOMManager();
	try {
		pDICOMManager->CargarFichero(paths.front(), base, false);
		pDICOMManager->CargarMetaInfo(paths.front(), info);
	} catch (...) {
		LOG_ERROR("PanelTags", "Error loading path " + paths.front());
		return false;
	}
	
	SetInfoTags(info, pDICOMManager);
	
	SetTags(base, m_pTags->GetRoot(), pDICOMManager);
	GIL::DICOM::PACSController::Instance()->LiberarInstanciaDeDICOMManager(pDICOMManager);
	return true;
}

void GNC::GUI::PanelTags::CreateCategories()
{
	m_pTags->Clear();
	metadataGroup =CreatePropertyGroup(_("Metadata"));
	identificationGroup = CreatePropertyGroup(_("Identification information"));
	patientGroup = CreatePropertyGroup(_("Patient information"));
	acquisitionGroup = CreatePropertyGroup(_("Acquisition information"));
	relationshipGroup = CreatePropertyGroup(_("Relationship information"));
	imagePresentationGroup = CreatePropertyGroup(_("Image presentation information"));
	textGroup = CreatePropertyGroup(_("Text"));
	otherGroup = CreatePropertyGroup(_("Other"));
}

void GNC::GUI::PanelTags::SetInfoTags(GIL::DICOM::TipoMetaInfo& info, GIL::DICOM::IDICOMManager*	pDICOMManager) 
{
	for(GIL::DICOM::TipoMetaInfo::ListaTags::iterator it = info.tags.begin(); it!= info.tags.end(); ++it) {
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription((*it).first).c_str());
		
		wxStringProperty* prop = NULL;
		wxString valor;
		if((*it).second.size()>100) {
			valor = _("Data");
		} else {
			std::string oidname = pDICOMManager->GetOIDName((*it).second);
			if (oidname.size() > 0) {
				valor = wxString::FromUTF8(oidname.c_str()) + wxT("[") + wxString::FromUTF8((*it).second.c_str()) + wxT("]");
			}
			else {
				valor = wxString::FromUTF8((*it).second.c_str());
			}
		}
		
		wxString label;
		if(helpString.size()>0){
			label = wxT("(") + wxString::FromUTF8((*it).first.c_str()) + wxT(") ") + helpString;
		}else{
			label = wxT("(") + wxString::FromUTF8((*it).first.c_str()) + wxT(") ");
		}
		while(m_pTags->GetProperty(label) != NULL)
			label = label + wxT(" ");
		prop = new wxStringProperty(label,
									wxPG_LABEL,valor);
		
		prop->SetHelpString(wxString::FromUTF8((*it).first.c_str()));
					
		m_pTags->AppendIn(metadataGroup, prop);
		prop->SetFlagRecursively(wxPG_PROP_READONLY, true);
	}
}

void GNC::GUI::PanelTags::SetTags(GIL::DICOM::DicomDataset& base, wxPGPropArg idPadre, GIL::DICOM::IDICOMManager* pDICOMManager) 
{	
	for(GIL::DICOM::ListaTags::iterator it = base.tags.begin(); it!= base.tags.end(); ++it) {
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription((*it).first).c_str());
		wxStringProperty* prop = NULL;
		wxString valor;
		if((*it).second.size()>100) {
			valor = _("Data");
		} else {
			valor = wxString::FromUTF8((*it).second.c_str());
		}

		wxString label;
		if(helpString.size()>0){
			label = wxT("(") + wxString::FromUTF8((*it).first.c_str()) + wxT(") ") + helpString;
		}else{
			label = wxT("(") + wxString::FromUTF8((*it).first.c_str()) + wxT(") ");
		}
		while(m_pTags->GetProperty(label) != NULL)
			label = label + wxT(" ");
		prop = new wxStringProperty(label,
			wxPG_LABEL,valor);

		prop->SetHelpString(wxString::FromUTF8((*it).first.c_str()));

		InsertTagInGroup(prop, (*it).first, idPadre);
		prop->SetFlagRecursively(wxPG_PROP_READONLY, true);
		//m_pTags->SetDisableProperty(prop);
	}

	for(GIL::DICOM::DicomDataset::DatasetList::iterator it = base.secuencias.begin(); it!=base.secuencias.end(); ++it){
		wxStringProperty* prop = NULL;
		wxString helpString = wxString::FromUTF8(pDICOMManager->GetDescription((*it).tagName).c_str());

		wxString label;
		if(helpString.size()>0){
			label = wxT("(") + wxString::FromUTF8((*it).tagName.c_str()) + wxT(") ") + helpString;
		}else{
			label = wxT("(") + wxString::FromUTF8((*it).tagName.c_str()) + wxT(") ");
		}
		while(m_pTags->GetProperty(label) != NULL)
			label = label + wxT(" ");
		prop = new wxStringProperty(label,
			wxPG_LABEL,wxEmptyString);

		prop->SetHelpString(wxString::FromUTF8((*it).tagName.c_str()));

		InsertTagInGroup(prop, (*it).tagName, idPadre);
		prop->SetFlagRecursively(wxPG_PROP_READONLY, true);
		//m_pTags->DisableProperty(pIdSequencia);
		//recursion
		int i=0;
		for(GIL::DICOM::DicomDataset::DatasetList::iterator it2 = (*it).items.begin(); it2!= (*it).items.end(); ++it2, ++i) {
			m_pTags->AppendIn(prop,new wxStringProperty(wxString::Format(wxT("Item %d"), (int) i),wxPG_LABEL,wxEmptyString));
			
			SetTags((*it2), prop, pDICOMManager);
		}
	}
}

//endregion
void GNC::GUI::PanelTags::OnClose(wxCloseEvent &event)
{
	if (m_hideOnClose) {
		Hide();
		event.Skip(false);
	} else {
		event.Skip(true);
	}
}

void GNC::GUI::PanelTags::OnKeyDown(wxKeyEvent &event)
{
	if (event.GetKeyCode() == WXK_ESCAPE) {
		Close(true);
	}
}

wxPGProperty* GNC::GUI::PanelTags::CreatePropertyGroup(const wxString& label)
{
	wxPGProperty* group = new wxPropertyCategory(label, wxPG_LABEL);
	m_pTags->AppendIn(m_pTags->GetRoot(), group);
	return group;
}

void GNC::GUI::PanelTags::InsertTagInGroup(wxPGProperty* prop, const std::string& key, wxPGPropArg idPadre)
{
	if (idPadre.GetPtr() == m_pTags->GetRoot()) {
		//parse group
		unsigned int g = 0xffff;
		{				
			unsigned int e = 0xffff;
			sscanf(key.c_str(), "%x|%x", &g, &e);
		}

		switch (g) {
			case 0x0008:
				m_pTags->AppendIn(identificationGroup, prop);
				break;
			case 0x0010:
				m_pTags->AppendIn(patientGroup, prop);
				break;
			case 0x0018:
				m_pTags->AppendIn(acquisitionGroup, prop);
				break;
			case 0x0020:
				m_pTags->AppendIn(relationshipGroup, prop);
				break;
			case 0x0028:
				m_pTags->AppendIn(imagePresentationGroup, prop);
				break;
			case 0x4000:
				m_pTags->AppendIn(textGroup, prop);
				break;
			default:
				m_pTags->AppendIn(otherGroup, prop);
				break;
		}
	} else {
		m_pTags->AppendIn(idPadre, prop);
	}
}
