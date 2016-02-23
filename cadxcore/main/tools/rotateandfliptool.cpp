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


#include "rotateandfliptool.h"
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <resources/ginkgoresourcesmanager.h>


#define ROTATE_LEFT  1861
#define ROTATE_RIGHT  1862
#define FLIP_H			1863
#define FLIP_V			1864
#define CLEAR_ROTATION_AND_FLIP	1865

namespace GNC {
	namespace GUI {
		class wxEventHandlerRotateAndFlip : public wxEvtHandler {
		public:
			wxEventHandlerRotateAndFlip(wxEvtHandler* pParent, GNC::RotateAndFlipTool* pHerramienta):wxEvtHandler() {
				m_pEvtParent = pParent;
				m_pHerramienta=pHerramienta;

				m_pEvtParent->Connect(ROTATE_LEFT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Connect(ROTATE_RIGHT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Connect(FLIP_H, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Connect(FLIP_V, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Connect(CLEAR_ROTATION_AND_FLIP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);

				m_pEvtParent->Connect(ROTATE_LEFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerRotateAndFlip::OnRotateLeft),NULL,this);
				m_pEvtParent->Connect(ROTATE_RIGHT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnRotateRight),NULL,this);
				m_pEvtParent->Connect(FLIP_H, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnFlipH),NULL,this);
				m_pEvtParent->Connect(FLIP_V, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnFlipV),NULL,this);
				m_pEvtParent->Connect(CLEAR_ROTATION_AND_FLIP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::ClearRotationAndFlip),NULL,this);
			}

			~wxEventHandlerRotateAndFlip() {
				m_pEvtParent->Disconnect(ROTATE_LEFT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Disconnect(ROTATE_RIGHT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Disconnect(FLIP_H, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Disconnect(FLIP_V, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);
				m_pEvtParent->Disconnect(CLEAR_ROTATION_AND_FLIP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerRotateAndFlip::OnUpdateRotateAndFlipUI),NULL,this);

				m_pEvtParent->Disconnect(ROTATE_LEFT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerRotateAndFlip::OnRotateLeft),NULL,this);
				m_pEvtParent->Disconnect(ROTATE_RIGHT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnRotateRight),NULL,this);
				m_pEvtParent->Disconnect(FLIP_H, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnFlipH),NULL,this);
				m_pEvtParent->Disconnect(FLIP_V, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::OnFlipV),NULL,this);
				m_pEvtParent->Disconnect(CLEAR_ROTATION_AND_FLIP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerRotateAndFlip::ClearRotationAndFlip),NULL,this);
				m_pEvtParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnRotateLeft( wxCommandEvent& event )
			{
				m_pHerramienta->Rotate(false);
				event.Skip(false);
			}

			void OnRotateRight( wxCommandEvent& event )
			{
				m_pHerramienta->Rotate(true);
				event.Skip(false);
			}

			void OnFlipH( wxCommandEvent& event )
			{
				m_pHerramienta->Flip(false);
				event.Skip(false);
			}

			void OnFlipV( wxCommandEvent& event )
			{
				m_pHerramienta->Flip(true);
				event.Skip(false);
			}

			void OnUpdateRotateAndFlipUI(wxUpdateUIEvent& event) {
				event.Enable(m_pHerramienta->IsEnabled());
			}

			void ClearRotationAndFlip( wxCommandEvent& event )
			{
				m_pHerramienta->Clear();
				event.Skip(false);
			}

			GNC::RotateAndFlipTool* m_pHerramienta;
			wxEvtHandler* m_pEvtParent;
		};
	}
}

GNC::GCS::ITool* GNC::RotateAndFlipTool::NewTool()
{
	return new GNC::RotateAndFlipTool();
}

GNC::RotateAndFlipTool::RotateAndFlipTool()
{
}
GNC::RotateAndFlipTool::~RotateAndFlipTool()
{
}
		
bool GNC::RotateAndFlipTool::ExecuteAction()
{	
	return false;
}

bool GNC::RotateAndFlipTool::HasDropDownMenu()
{
	return true;
}

void GNC::RotateAndFlipTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (pMenu->GetMenuItemCount() > 0 && !pMenu->GetMenuItems().back()->IsSeparator()) {
		pMenu->AppendSeparator();
	}
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, CLEAR_ROTATION_AND_FLIP, _("Undo rotate and flip"), GinkgoResourcesManager::MenuIcons::GetIcoReset(), new GNC::GUI::wxEventHandlerRotateAndFlip( pParent, this)));
	pMenu->AppendSeparator();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ROTATE_LEFT, _("Rotate 90") + wxString::FromUTF8("º ")+ _("counter-clockwise"), GinkgoResourcesManager::ToolIcons::GetIcoRotateLeft(), new GNC::GUI::wxEventHandlerRotateAndFlip( pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ROTATE_RIGHT, _("Rotate 90") + wxString::FromUTF8("º ")+ _("clockwise"), GinkgoResourcesManager::ToolIcons::GetIcoRotateRight(), new GNC::GUI::wxEventHandlerRotateAndFlip( pParent, this)));
	pMenu->AppendSeparator();
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, FLIP_H, _("Flip horizontal"), GinkgoResourcesManager::ToolIcons::GetIcoFlipHorizontal(), new GNC::GUI::wxEventHandlerRotateAndFlip( pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, FLIP_V, _("Flip vertical"), GinkgoResourcesManager::ToolIcons::GetIcoFlipVertical(), new GNC::GUI::wxEventHandlerRotateAndFlip( pParent, this)));		
}

void GNC::RotateAndFlipTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		AppendDropDownMenu(pParent, pMenu);
	}
}

void GNC::RotateAndFlipTool::Rotate(bool right)
{
	GNC::GCS::IWidgetsContract::TViewerList listOfViewers;
	WidgetsContract->GetAllViewers(listOfViewers);
	for (GNC::GCS::IWidgetsContract::TViewerList::iterator it =listOfViewers.begin(); it != listOfViewers.end(); ++it) {
		(*it)->RotateCamera(right);
	}
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(WidgetsContract->GetManager()->GetVista(), GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));	
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

void GNC::RotateAndFlipTool::Flip(bool vertical)
{
	GNC::GCS::IWidgetsContract::TViewerList listOfViewers;
	WidgetsContract->GetAllViewers(listOfViewers);
	for (GNC::GCS::IWidgetsContract::TViewerList::iterator it =listOfViewers.begin(); it != listOfViewers.end(); ++it) {
		(*it)->Flip(vertical);
	}
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(WidgetsContract->GetManager()->GetVista(), GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));	
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

void GNC::RotateAndFlipTool::Clear()
{
	GNC::GCS::IWidgetsContract::TViewerList listOfViewers;
	WidgetsContract->GetAllViewers(listOfViewers);
	for (GNC::GCS::IWidgetsContract::TViewerList::iterator it =listOfViewers.begin(); it != listOfViewers.end(); ++it) {
		(*it)->ClearRotationAndFlip(true);
	}
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(WidgetsContract->GetManager()->GetVista(), GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));	
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}
