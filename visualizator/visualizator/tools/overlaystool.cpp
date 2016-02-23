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

#include <api/autoptr.h>

#include "overlaystool.h"
#include <visualizator/eventos/visualizatorevents.h>
#include <visualizator/export/contracts/ioverlayscontract.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <resources/visualizatorresourcesmanager.h>
#include <api/controllers/ieventscontroller.h>

#define MENU_OVERLAYS  4051


namespace GNKVisualizator {
	namespace GUI {
		class wxEventHandlerOverlays : public wxEvtHandler {
			public:
			wxEventHandlerOverlays(wxMenu* pSubMenu, wxEvtHandler* pParent, GNKVisualizator::OverlaysTool* pHerramienta):wxEvtHandler() {
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				m_pSubMenu = pSubMenu;
				m_pParent->Connect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerOverlays::OnUpdateSeleccionarOverlaysUI),NULL,this);
			}

			~wxEventHandlerOverlays() {
				m_pParent->Disconnect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerOverlays::OnUpdateSeleccionarOverlaysUI),NULL,this);
				m_pParent->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerOverlays::OnMostrarOverlay ),NULL,this);
				m_pParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnUpdateSeleccionarOverlaysUI(wxUpdateUIEvent& event) {
				//se agregan los overlays...
				while(m_pSubMenu->GetMenuItemCount() >0) {
					m_pSubMenu->Destroy(m_pSubMenu->FindItemByPosition(0));
				}

				GNKVisualizator::TListaOverlays* pLista = m_pHerramienta->GetListaOverlays();
				GNKVisualizator::TEstadoOverlay* estadoOverlays = m_pHerramienta->GetEstadoOverlays();
				if(pLista!= NULL && estadoOverlays!=NULL && pLista->size() > 0) {
					int idOverlay = MENU_OVERLAYS;
					for(GNKVisualizator::TListaOverlays::iterator itOverlays = pLista->begin(); itOverlays!=pLista->end(); ++itOverlays, ++idOverlay) {
						wxMenuItem* capa = new wxMenuItem( m_pSubMenu, idOverlay,wxString::FromUTF8((*itOverlays).nombre.c_str()),wxEmptyString,wxITEM_CHECK);
						m_pSubMenu->Append(capa);
						bool oculto = false;
						if(estadoOverlays->find((*itOverlays).indice) != estadoOverlays->end()) {
							oculto = (*estadoOverlays->find(itOverlays->indice)).second;
						}
						if(oculto){
							m_pSubMenu->Check(capa->GetId(),false);
						} else {
							m_pSubMenu->Check(capa->GetId(),true);
						}
						m_pParent->Connect(capa->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerOverlays::OnMostrarOverlay ),NULL,this);
					}			
					event.Show(true);
					event.Enable(true);
				} else {
					event.Show(false);
					event.Enable(false);
				}
			}

			void OnMostrarOverlay(wxCommandEvent& event)
			{
				int idOverlay = event.GetId() - MENU_OVERLAYS;
				m_pHerramienta->ToggleOverlay(idOverlay);
				event.Skip(false);
			} 


			GNKVisualizator::OverlaysTool* m_pHerramienta;
			wxEvtHandler* m_pParent;
			wxMenu* m_pSubMenu;
		};
	}
}


GNC::GCS::ITool* GNKVisualizator::OverlaysTool::NewTool()
{
	return new GNKVisualizator::OverlaysTool();
}

GNKVisualizator::OverlaysTool::OverlaysTool()
{
}
GNKVisualizator::OverlaysTool::~OverlaysTool()
{
}
	
bool GNKVisualizator::OverlaysTool::ExecuteAction()
{	
	return false;
}


void GNKVisualizator::OverlaysTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {	
		GNC::GUI::wxMenuTool* pMenuTool = new GNC::GUI::wxMenuTool(NULL);
		pMenuTool->m_pHandler =  new GNKVisualizator::GUI::wxEventHandlerOverlays(pMenuTool, pParent, this);
		pMenu->Append(ID, wxString::FromUTF8(Name.c_str()), pMenuTool);
	}
}


GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& GNKVisualizator::OverlaysTool::GetListaOverlays()
{
	return OverlaysContract->GetOverlayList();
}

GNKVisualizator::TEstadoOverlay* GNKVisualizator::OverlaysTool::GetEstadoOverlays()
{
	return &OverlaysContract->m_EstadoOverlays;
}

void GNKVisualizator::OverlaysTool::ToggleOverlay(int idOverlay)
{
	if(OverlaysContract->GetOverlayList().IsValid())
	{
		GNKVisualizator::TListaOverlays::iterator itOverlays = OverlaysContract->GetOverlayList()->begin();
		for(; idOverlay > 0 && itOverlays!=OverlaysContract->GetOverlayList()->end(); ++itOverlays, --idOverlay);
		if( itOverlays!= OverlaysContract->GetOverlayList()->end()) {
			if(OverlaysContract->m_EstadoOverlays.find((*itOverlays).indice) != OverlaysContract->m_EstadoOverlays.end()) {
				OverlaysContract->m_EstadoOverlays[itOverlays->indice] = !OverlaysContract->m_EstadoOverlays[itOverlays->indice];
			} else {
				OverlaysContract->m_EstadoOverlays[itOverlays->indice] = false;
			}
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNKVisualizator::Events::EventoMostrarOverlay(OverlaysContract->m_pView,(*itOverlays)));
		}
	}
}


