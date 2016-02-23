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

#include "markertool.h"

#if defined(ENABLE_MARKERTOOL)
#include <wx/menu.h>
#include <api/iwidgetsmanager.h>
#include <api/internationalization/internationalization.h>
#include <export/contracts/iwidgetscontract.h>
#include <eventos/toolevents.h>
#include <api/controllers/ieventscontroller.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#include <api/iwidgetsmanager.h>
#include <widgets/wflecha.h>
#include <widgets/welipse.h>
#include <widgets/wpunto.h>
#include <widgets/wtrapezoide.h>
#include <widgets/wfreeform.h>
#include <widgets/wpolygon.h>
#include <resources/ginkgoresourcesmanager.h>


#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

#define USAR_FLECHA     1451
#define USAR_ELIPSE     1452
#define USAR_TRAPEZOIDE 1453
#define USAR_FREEFORM   1454
#define USAR_PUNTO      1455
#define USAR_POLYGON      1456

namespace GNC {
	namespace GUI {
		class EventHandlerMarkerTools: public wxEvtHandler {
		public:
			EventHandlerMarkerTools(wxEvtHandler* pParent, GNC::MarkerTool* pHerramienta):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				// Connect Events
				m_pParent->Connect(USAR_PUNTO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaPuntoClick),NULL,this);
				m_pParent->Connect(USAR_FLECHA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaFlechaClick),NULL,this);
				m_pParent->Connect(USAR_ELIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaElipseClick),NULL,this);
				m_pParent->Connect(USAR_TRAPEZOIDE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaTrapezoideClick),NULL,this);
				m_pParent->Connect(USAR_FREEFORM,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaFreeFormClick),NULL,this);
				m_pParent->Connect(USAR_POLYGON,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaPolygonClick),NULL,this);
			}

			~EventHandlerMarkerTools()
			{
				m_pParent->Disconnect(USAR_PUNTO, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaPuntoClick),NULL,this);
				m_pParent->Disconnect(USAR_FLECHA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaFlechaClick),NULL,this);
				m_pParent->Disconnect(USAR_ELIPSE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaElipseClick),NULL,this);
				m_pParent->Disconnect(USAR_TRAPEZOIDE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaTrapezoideClick),NULL,this);
				m_pParent->Disconnect(USAR_FREEFORM,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaFreeFormClick),NULL,this);
				m_pParent->Disconnect(USAR_POLYGON,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerMarkerTools::OnHerramientaPolygonClick),NULL,this);
				m_pParent = NULL;
				m_pHerramienta = NULL;
			}
			
			virtual void OnHerramientaPuntoClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_Punto);
				event.Skip(false);
			}

			virtual void OnHerramientaFlechaClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_Flecha);
				event.Skip(false);
			}

			virtual void OnHerramientaElipseClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_Elipse);
				event.Skip(false);
			}

			virtual void OnHerramientaTrapezoideClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_Trapezoide);
				event.Skip(false);
			}

			virtual void OnHerramientaFreeFormClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_FreeForm);
				event.Skip(false);
			}	

			virtual void OnHerramientaPolygonClick( wxCommandEvent& event ){
				m_pHerramienta->SetTipoMarcado(GNC::MarkerTool::TW_Polygon);
				event.Skip(false);
			}	

			GNC::MarkerTool* m_pHerramienta;
			wxEvtHandler* m_pParent;
		};
	}
}

GNC::MarkerTool::MarkerTool() {
	m_MarcadoActivo = TW_Punto;
	m_pBuilder = NULL;
}

GNC::MarkerTool::~MarkerTool() 
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}


//------------------------------------------------------------------------------
//region "ITool/IContract realization"

GNC::GCS::ITool* GNC::MarkerTool::NewTool()
{
	return new GNC::MarkerTool();
}


void GNC::MarkerTool::Connect()
{
	if (m_pBuilder != NULL)
	{
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
	SetTipoMarcado(m_MarcadoActivo);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	if (m_pBuilder != NULL) {
		WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
	}
	else {
		WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	}
}

void GNC::MarkerTool::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	if (m_pBuilder != NULL)
	{
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

bool GNC::MarkerTool::HasDropDownMenu()
{
	return true;
}

void GNC::MarkerTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_PUNTO,_("Point"), GinkgoResourcesManager::MarkingBar::GetIcoPunto(), new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_FLECHA,_("Arrow"), GinkgoResourcesManager::MarkingBar::GetIcoFlecha(),new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_ELIPSE,_("Ellipse"), GinkgoResourcesManager::MarkingBar::GetIcoElipse(), new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_TRAPEZOIDE,_("Trapezoid"), GinkgoResourcesManager::MarkingBar::GetIcoTrapezoide(), new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_POLYGON,_("Polygon"), GinkgoResourcesManager::MarkingBar::GetIcoPoligono(), new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu,USAR_FREEFORM,_("Free form"), GinkgoResourcesManager::MarkingBar::GetIcoFreeForm(), new GNC::GUI::EventHandlerMarkerTools(pParent, this)));
}


void GNC::MarkerTool::SetTipoMarcado(TipoMarcado tipo)
{
	wxBitmap icon;
	if (m_MarcadoActivo == tipo && m_pBuilder != NULL) {
		return;
	}
	else {
		if (m_pBuilder != NULL) {
			delete m_pBuilder;
			m_pBuilder = NULL;
		}
		m_MarcadoActivo = tipo;
		switch (m_MarcadoActivo) {
			case TW_Flecha:
				m_pBuilder = new GNC::GCS::Widgets::WFlechaBuilder(WidgetsContract->GetManager(), GetTriggerButton());
				icon = GinkgoResourcesManager::MarkingBar::GetIcoFlecha();
				break;
			case TW_Elipse:
				m_pBuilder = new GNC::GCS::Widgets::WElipseBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
				icon = GinkgoResourcesManager::MarkingBar::GetIcoElipse();
				break;
			case TW_Trapezoide:
				m_pBuilder = new GNC::GCS::Widgets::WTrapezoideBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
				icon = GinkgoResourcesManager::MarkingBar::GetIcoTrapezoide();
				break;
			case TW_FreeForm:
				m_pBuilder = new GNC::GCS::Widgets::WFreeFormBuilder(WidgetsContract->GetManager(), GetTriggerButton());
				icon = GinkgoResourcesManager::MarkingBar::GetIcoFreeForm();
				break;
			case TW_Punto:
				{
					m_pBuilder = new GNC::GCS::Widgets::WPuntoBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
					icon = GinkgoResourcesManager::MarkingBar::GetIcoPunto();
				}
				break;
			case TW_Polygon:
				{
					m_pBuilder = new GNC::GCS::Widgets::WPolygonBuilder(WidgetsContract->GetManager(), GetTriggerButton());
					icon = GinkgoResourcesManager::MarkingBar::GetIcoPoligono();
				}
				break;
			default:
				std::cerr << "Error, Elemento de marcado no soportado." << std::endl;
				break;
			}
			if (m_pBuilder != NULL) {
				WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
			}
			else {
				WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
			}
	}
	if (icon.IsOk()) {
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ChangeToolIconEvent(&icon, this, m_pParent));
	}
}

//endregion
#endif
