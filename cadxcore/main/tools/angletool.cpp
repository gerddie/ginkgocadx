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

#include <wx/menu.h>

#include "angletool.h"
#if defined(ENABLE_ANGLETOOL)

#include <api/iwidgetsmanager.h>
#include <api/internationalization/internationalization.h>
#include <export/contracts/iwidgetscontract.h>
#include <eventos/toolevents.h>
#include <api/controllers/ieventscontroller.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#include <widgets/wangulo.h>
#include <resources/ginkgoresourcesmanager.h>

#include <iostream>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

#define TIPO_NORMAL 1211
#define TIPO_COBB   1212

namespace GNC {
	namespace GUI {
		class EventHandlerAngleTools: public wxEvtHandler {
		public:
			EventHandlerAngleTools(wxEvtHandler* pParent, GNC::AngleTool* pHerramienta):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				// Connect Events
				m_pParent->Connect(TIPO_NORMAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerAngleTools::OnTipoAnguloNormalClick),NULL,this);
				m_pParent->Connect(TIPO_COBB, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerAngleTools::OnTipoAnguloCobbClick),NULL,this);
			}

			~EventHandlerAngleTools()
			{
				m_pParent->Disconnect(TIPO_NORMAL, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerAngleTools::OnTipoAnguloNormalClick),NULL,this);
				m_pParent->Disconnect(TIPO_COBB, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerAngleTools::OnTipoAnguloCobbClick),NULL,this);
			}
			
			virtual void OnTipoAnguloNormalClick(wxCommandEvent& event){
				m_pHerramienta->SetTipo(GNC::AngleTool::TA_Normal);
				event.Skip(false);
			}

			virtual void OnTipoAnguloCobbClick(wxCommandEvent& event){
				m_pHerramienta->SetTipo(GNC::AngleTool::TA_Cobb);
				event.Skip(false);
			}		

			GNC::AngleTool* m_pHerramienta;
			wxEvtHandler* m_pParent;
		};
	}
}


GNC::AngleTool::AngleTool() {
	m_TipoAngulo = TA_Normal;
	m_pAnguloBuilder = NULL;
}

GNC::AngleTool::~AngleTool() 
{
	if (m_pAnguloBuilder != NULL) {
		delete m_pAnguloBuilder;
		m_pAnguloBuilder = NULL;
	}
}

//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNC::AngleTool::NewTool()
{
	return new GNC::AngleTool();
}


void GNC::AngleTool::Connect()
{
	if (m_pAnguloBuilder != NULL)
	{
		delete m_pAnguloBuilder;
		m_pAnguloBuilder = NULL;
	}
	m_pAnguloBuilder = new GNC::GCS::Widgets::WAnguloBuilder(WidgetsContract->GetManager(), GetTriggerButton());
	if (m_TipoAngulo == TA_Normal) {
		m_pAnguloBuilder->SetModo(GNC::GCS::Widgets::MA_Normal);
	}
	else {
		m_pAnguloBuilder->SetModo(GNC::GCS::Widgets::MA_Cobb);
	}
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pAnguloBuilder->GetCursor());
}

void GNC::AngleTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	if (m_pAnguloBuilder != NULL)
	{
		delete m_pAnguloBuilder;
		m_pAnguloBuilder = NULL;
	}
}

bool GNC::AngleTool::HasDropDownMenu()
{
	return true;
}

void GNC::AngleTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, TIPO_NORMAL,_("Normal Angle (3 points)"), GinkgoResourcesManager::AngleBar::GetIcoAnguloNormal(), new GNC::GUI::EventHandlerAngleTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, TIPO_COBB,_("Cobb Angle (2 straight lines)"), GinkgoResourcesManager::AngleBar::GetIcoAnguloCobb(), new GNC::GUI::EventHandlerAngleTools(pParent, this)));
}


void GNC::AngleTool::SetTipo(TipoAngulo tipo)
{
	if (m_TipoAngulo != tipo) {
		m_TipoAngulo = tipo;
	
		wxBitmap icon;
		if (m_TipoAngulo == TA_Normal) {
			if (m_pAnguloBuilder != NULL) {
				m_pAnguloBuilder->SetModo(GNC::GCS::Widgets::MA_Normal);
			}
			icon = GinkgoResourcesManager::AngleBar::GetIcoAnguloNormal();
		}
		else {
			if (m_pAnguloBuilder != NULL) {
				m_pAnguloBuilder->SetModo(GNC::GCS::Widgets::MA_Cobb);
			}
			icon = GinkgoResourcesManager::AngleBar::GetIcoAnguloCobb();
		}
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ChangeToolIconEvent(&icon, this, m_pParent));
	}
}

//endregion

#endif
