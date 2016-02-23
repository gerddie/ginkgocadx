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

#include "colormaptool.h"
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <resources/ginkgoresourcesmanager.h>
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkVISUManagement/vtkLookupTableManager.h>
#include <vtk/vtkginkgoimageviewer.h>

#define INVERTIR_COLORES  4100
#define MENU_MAPAS_COLOR  4101

namespace GNC {
	namespace GUI {
		class wxEventHandlerColorMap : public wxEvtHandler {
			public:
				wxEventHandlerColorMap(wxEvtHandler* pParent, GNC::ColorMapTool* pHerramienta):wxEvtHandler() {
				m_pEvtParent = pParent;
				m_pHerramienta=pHerramienta;

				m_pEvtParent->Connect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerColorMap::OnUpdateColorMapUI),NULL,this);
				m_pEvtParent->Connect(INVERTIR_COLORES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerColorMap::OnInvertColorMapClick),NULL,this);
				m_pEvtParent->Connect(INVERTIR_COLORES, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerColorMap::OnUpdateColorMapUI),NULL,this);
			}

			~wxEventHandlerColorMap() {
				m_pEvtParent->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerColorMap::OnSeleccionarMapa ),NULL,this);
				m_pEvtParent->Disconnect(wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( wxEventHandlerColorMap::OnUpdateMenuMapaColorUI ),NULL,this);
				
				m_pEvtParent->Disconnect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerColorMap::OnUpdateColorMapUI),NULL,this);
				m_pEvtParent->Disconnect(INVERTIR_COLORES, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerColorMap::OnInvertColorMapClick),NULL,this);
				m_pEvtParent->Disconnect(INVERTIR_COLORES, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerColorMap::OnUpdateColorMapUI),NULL,this);
				m_pEvtParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnSeleccionarMapa( wxCommandEvent& event )
			{
				int idMapa = event.GetId() - MENU_MAPAS_COLOR;
				m_pHerramienta->SeleccionarMapa(idMapa);
				event.Skip(false);
			}


			void OnUpdateMenuMapaColorUI(wxUpdateUIEvent& event) 
			{
				if ((event.GetId() - MENU_MAPAS_COLOR) >= 0)  {
					int idMapaSeleccionado = m_pHerramienta->GetIdLookupTable();
					event.Check(event.GetId()-MENU_MAPAS_COLOR == idMapaSeleccionado);
				}
			}

			
			void OnInvertColorMapClick( wxCommandEvent& event )
			{
				m_pHerramienta->InvertColorMap();
				event.Skip(false);
			}

			void OnUpdateColorMapUI(wxUpdateUIEvent& event) {
				event.Enable(m_pHerramienta->SoportaMapas());
			}

			GNC::ColorMapTool* m_pHerramienta;
			wxEvtHandler* m_pEvtParent;
		};
	}
}



GNC::GCS::ITool* GNC::ColorMapTool::NewTool()
{
	return new GNC::ColorMapTool();
}

GNC::ColorMapTool::ColorMapTool()
{
}
GNC::ColorMapTool::~ColorMapTool()
{
}

bool GNC::ColorMapTool::HasDropDownMenu()
{
	return true;
}

void GNC::ColorMapTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	std::list<std::string> choices = vtkLookupTableManager::GetAvailableLookupTables();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, INVERTIR_COLORES,_("Invert color map"), GinkgoResourcesManager::ToolIcons::GetIcoInvert(), new GNC::GUI::wxEventHandlerColorMap( pParent, this)));
	pMenu->AppendSeparator();
	int idMapa = MENU_MAPAS_COLOR;
	wxBitmap icon;
	for(std::list<std::string>::iterator it = choices.begin(); it!= choices.end(); ++it,idMapa++){
		GNC::GUI::wxMenuItemTool* pItem =  new GNC::GUI::wxMenuItemTool(pMenu, idMapa,wxString::FromUTF8((*it).c_str()), icon, new GNC::GUI::wxEventHandlerColorMap(pParent, this), wxITEM_CHECK);
		pParent->Connect(pItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( GNC::GUI::wxEventHandlerColorMap::OnSeleccionarMapa ),NULL,pItem->m_pHandler);
		pParent->Connect(pItem->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(GNC::GUI::wxEventHandlerColorMap::OnUpdateMenuMapaColorUI),NULL,pItem->m_pHandler);
		pMenu->Append(pItem);
	}
}
		
bool GNC::ColorMapTool::ExecuteAction()
{	
	return false;
}


void GNC::ColorMapTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		wxMenu* pLayoutMenu = new wxMenu();
		AppendDropDownMenu(pParent, pLayoutMenu);
		pMenu->Append(ID,wxString::FromUTF8(Name.c_str()),pLayoutMenu);	
		pMenu->Enable(ID,Enabled);
	}
}


bool GNC::ColorMapTool::SoportaMapas()
{
	if (WidgetsContract->Inicializado()) {
		GNC::GCS::IWidgetsContract::TViewerList listViewers;
		WidgetsContract->GetAllViewers(listViewers);
		bool soporta =true;
		for (GNC::GCS::IWidgetsContract::TViewerList::iterator it = listViewers.begin(); it != listViewers.end(); ++it)
		{
			soporta = ( soporta && ((*it)->GetNumberOfComponents()==1));
		}
		return soporta;
	} else {
		return false;
	}
}

void GNC::ColorMapTool::SeleccionarMapa(int idMapa)
{
	if (WidgetsContract->Inicializado()) {
		if(idMapa>=0 && idMapa<(int)vtkLookupTableManager::GetAvailableLookupTables().size()) {
			vtkLookupTable* vtkTabla = vtkLookupTableManager::GetLookupTable(idMapa);
			if(vtkTabla != NULL)
			{
				GNC::GCS::IWidgetsContract::TViewerList listViewers;
				WidgetsContract->GetAllViewers(listViewers);
				
				for (GNC::GCS::IWidgetsContract::TViewerList::iterator it = listViewers.begin(); it != listViewers.end(); ++it)
				{
					(*it)->SetLookupTable(vtkTabla, idMapa);
				}
				GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
			}
		}
	}
}

int GNC::ColorMapTool::GetIdLookupTable()
{
	if (!SoportaMapas())
		return -1;
	int idMapa = -1;
	if (WidgetsContract->Inicializado()) {
		idMapa = WidgetsContract->GetViewerActivo()->GetIdLookupTable();
	}
	return idMapa;
}

void GNC::ColorMapTool::InvertColorMap()
{
	int idMapa = GetIdLookupTable();

	if (idMapa == vtkLookupTableManager::LUT_LINEAR) {
		idMapa = vtkLookupTableManager::LUT_LINEAR_INV;
	} else if (idMapa == vtkLookupTableManager::LUT_LINEAR_INV) {
		idMapa = vtkLookupTableManager::LUT_LINEAR;
	}
	SeleccionarMapa(idMapa);
}

