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

#include <wx/dc.h>
#include <wx/sizer.h>
#include <wx/event.h>
#include <wx/bitmap.h>
#include <wx/menu.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/aui/auibar.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/ieventscontroller.h>
#include <api/globals.h>
#include <resources/ginkgoresourcesmanager.h>

#include "previewpanel.h"
#include "leafpanel.h"
#include "firstlevelpanel.h"



#define ID_MENUS 1401
#define ID_EJECUTAR 1
#define ID_TITULO 0

#define MIN_WIDTH 100


namespace GNC {
	namespace GUI {
		PreviewPanel::PreviewPanel(GNC::GCS::IVista* pView, wxWindow* parent) : PreviewPanelBase(parent),
			INodoHistorial(NULL),
			m_pView(pView),
			m_dirty(true)
		{
			//titulo
			m_pPreviewTitle->GetButtonBar()->AddTool(ID_EJECUTAR,_("Hide"),GinkgoResourcesManager::History::GetIcoPlegar(),_("Hide"), wxITEM_NORMAL);
			m_pPreviewTitle->GetButtonBar()->Connect(ID_EJECUTAR,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PreviewPanel::OnHidePreviewsClick),NULL,this);
			m_pPreviewTitle->Realize();

			//se conecta el evento Ginkgo
			GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GUI::Events::EventoSetFocusHistorial(m_pView));
			GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GUI::Events::EventoSeleccionarHistorial(m_pView));


			m_pSelected = NULL;
			Layout();
		}

		PreviewPanel::~PreviewPanel() 
		{			
		}

		//interfaz modelo dicom
		void PreviewPanel::LoadModel(IPreviewNode* pNode )
		{
			m_pPreviewTitle->SetToolTip(wxString::FromUTF8(pNode->GetLabel().c_str()));
			m_pPanelVertical->SetToolTip(wxString::FromUTF8(pNode->GetLabel().c_str()));
			m_pPanelPreviews->GetSizer()->Clear(true);
			m_pSelected = NULL;

			int leafIndex = 0;
			for (IPreviewNode::TListNodes::iterator it = pNode->children.begin(); it != pNode->children.end(); ++it, ++leafIndex)
			{
				IPreviewNode* curNode = (*it);
				IPreviewLeaf* pLeaf = dynamic_cast<IPreviewLeaf*>(curNode);
				if (pLeaf != NULL) {
					LeafPanel* p = new LeafPanel(m_pView, this, pLeaf, leafIndex%2); 
					m_pPanelPreviews->GetSizer()->Add(p,0,wxBOTTOM|wxEXPAND,0);
				} else {
					FirstLevelPanel* pNewLevel = new FirstLevelPanel(m_pView, this, curNode);
					m_pPanelPreviews->GetSizer()->Add(pNewLevel,0,wxBOTTOM|wxEXPAND,0);
					for (IPreviewNode::TListNodes::iterator itFirstLevel = curNode->children.begin(); itFirstLevel != curNode->children.end(); ++itFirstLevel, ++leafIndex)
					{
						IPreviewNode* curNode2 = (*itFirstLevel);
						IPreviewLeaf* pLeaf = dynamic_cast<IPreviewLeaf*>(curNode2);
						if (pLeaf != NULL) {
							LeafPanel* p = new LeafPanel(m_pView, this, pLeaf, leafIndex%2); 
							m_pPanelPreviews->GetSizer()->Add(p,0,wxBOTTOM|wxEXPAND,0);
						} else {
							LOG_ERROR("FirstLevelPanel", "leaf panel isn't an IPreviewLeaf");
						}				
					} 
				}				
			}
			Show();
			GetParent()->Layout();
		}

		wxWindow* PreviewPanel::GetWxWindow()
		{
			return m_pPanelPreviews;
		}

		void PreviewPanel::OnChar(wxKeyEvent &event)
		{
			event.ResumePropagation(10);
			event.Skip(true);
		}
		
		void PreviewPanel::OnHidePreviewsClick(wxCommandEvent& )
		{
			FoldUp(true);
		}

		void PreviewPanel::OnShowPreviewsClick(wxMouseEvent& )
		{
			FoldUp(false);
		}

		void PreviewPanel::FoldUp(bool plegar)
		{
			m_pPanelPreviews->Show(!plegar);
			m_pPreviewTitle->Show(!plegar);
			m_pPanelVertical->Show(plegar);
			if (plegar) {
				SetSize(m_pPanelVertical->GetSize());
				SetMaxSize(m_pPanelVertical->GetMaxSize());
				SetMinSize(m_pPanelVertical->GetMinSize());
			} else {
				SetSize(m_pPanelPreviews->GetSize());
				SetMaxSize(m_pPanelPreviews->GetMaxSize());
				SetMinSize(m_pPanelPreviews->GetMinSize());
			}
			Layout();
			GetParent()->Layout();
		}

		void PreviewPanel::PreProcesarEvento(GNC::GCS::Events::IEvent * evt, std::list<GNC::GCS::Events::IEvent*>& lista)
		{
			if (evt == NULL) {
				std::cerr << "Error: null event" << std::endl;
				return;
			}
			switch (evt->GetCodigoEvento()) {
			case ginkgoEVT_Core_HistorialSeleccionar:
				{
					lista.push_back(new GNC::GUI::Events::EventoSetFocusHistorial(m_pView));
				}
				break;
			default:
				break;

			}
		}

		void PreviewPanel::ProcesarEvento(GNC::GCS::Events::IEvent *evt) 
		{
			switch(evt->GetCodigoEvento()) {
			case  ginkgoEVT_Core_HistorialSeleccionar:
				{
					GNC::GUI::Events::EventoSeleccionarHistorial* pEvtSelecc = dynamic_cast<GNC::GUI::Events::EventoSeleccionarHistorial*>(evt);
					if(pEvtSelecc != NULL) {
						GNC::GUI::ISeleccionablePreview* pSeleccionable = pEvtSelecc->GetSeleccionable();
						if(pSeleccionable != m_pSelected){
							if(m_pSelected!= NULL) {
								m_pSelected->Seleccionar(false);
							}
							if(pSeleccionable != NULL){
								pSeleccionable->Seleccionar(true);
							}
							m_pSelected = pSeleccionable;
						}
					}
				}
				break;
			case ginkgoEVT_Core_SetFocus:
				{
#if defined(_WIN32) || defined(__WXOSX__)
					m_pPanelPreviews->SetFocus();
#endif
				}
				break;
			}
		}

		void PreviewPanel::OnEraseBackground( wxEraseEvent& event )
		{
			event.Skip(false);
		}

	};
};
