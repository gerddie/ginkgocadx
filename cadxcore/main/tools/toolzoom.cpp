/*
 *  
 *  $Id: toolzoom.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include <ostream>
#include "toolzoom.h"
#if defined(ENABLE_ZOOMTOOL)
#include <export/contracts/iwidgetscontract.h>

#include <api/globals.h>
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>
#include <vtkInteractorStyle.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/controllers/ieventscontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyle.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>


#define ZOOM_1_1 1211
namespace GNC {
	namespace GUI {
		class EventHandlerZoomTool: public wxEvtHandler {
		public:
			EventHandlerZoomTool(wxEvtHandler* pParent, GNC::ToolZoom* pHerramienta):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				// Connect Events
				m_pParent->Connect(ZOOM_1_1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerZoomTool::OnZoom1_1Click),NULL,this);
			}

			~EventHandlerZoomTool()
			{
				m_pParent->Disconnect(ZOOM_1_1, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerZoomTool::OnZoom1_1Click),NULL,this);
			}
			
			virtual void OnZoom1_1Click(wxCommandEvent& event){
				m_pHerramienta->ZoomOneOne();
				event.Skip(false);
			}
			GNC::ToolZoom* m_pHerramienta;
			wxEvtHandler* m_pParent;
		};
	}
	class EXTAPI wxToolZoomKeyHandler: public wxEvtHandler
	{
		public:
			wxToolZoomKeyHandler(wxEvtHandler* pParent, GNC::ToolZoom* pTool): wxEvtHandler()
			{
				m_pParent = pParent;
				m_pTool = pTool;
				// Connect Events
				m_pParent->Connect(wxEVT_CHAR, wxKeyEventHandler( wxToolZoomKeyHandler::OnChar),NULL,this);
			}

			virtual ~wxToolZoomKeyHandler()
			{
				m_pParent->Disconnect(wxEVT_CHAR, wxKeyEventHandler( wxToolZoomKeyHandler::OnChar),NULL,this);
			}

			void DoZoom(int steps) 
			{
				if (m_pTool->WidgetsContract && m_pTool->WidgetsContract->GetViewerActivo()) {
					vtkSmartPointer<vtkRenderWindowInteractor> rwi = m_pTool->WidgetsContract->GetViewerActivo()->GetRenderWindowInteractor();
					vtkSmartPointer<GinkgoInteractorStyleImage2D> is = dynamic_cast<GinkgoInteractorStyleImage2D*> (rwi->GetInteractorStyle());
					if (is != NULL) {
						if (is->GetState() != VTKIS_ZOOM) {
							is->StartZoom();
						}
						double factor = 0.1* steps * is->GetMouseWheelMotionFactor();
						//this->Dolly(pow((double)1.1, factor));
						is->DoZoom(factor, true);
					}
				}

			}

			virtual void OnChar(wxKeyEvent& event)
			{
				switch(event.GetKeyCode()){
					case WXK_PAGEUP:
					{
						//zoom -
						DoZoom(3);
						event.Skip(false);
					}
					break;
					case WXK_PAGEDOWN:
					{
						//zoom +
						DoZoom(-3);
						event.Skip(false);
					}
					break;
					default:
						event.ResumePropagation(10);
						event.Skip(true);
				}
			}
		protected:
			wxEvtHandler* m_pParent;
			GNC::ToolZoom* m_pTool;
		};

}


GNC::ToolZoom::ToolZoom() 
{
	m_pBuilder = NULL;
	m_pHandler = NULL;
}

GNC::ToolZoom::~ToolZoom()
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}	
	if (m_pHandler != NULL) {
		delete m_pHandler;
	}
}


GNC::GCS::ITool* GNC::ToolZoom::NewTool()
{
	return new GNC::ToolZoom();
}

void GNC::ToolZoom::Connect()
{
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}

	m_pBuilder = new TBuilder(WidgetsContract->GetManager(), GetTriggerButton(), (unsigned long)this);
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(m_pBuilder->GetCursor());
}

void GNC::ToolZoom::Disconnect()
{
	WidgetsContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WidgetsContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
	// DesSubscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
	if (m_pBuilder != NULL) {
		delete m_pBuilder;
		m_pBuilder = NULL;
	}
}

bool GNC::ToolZoom::HasDropDownMenu()
{
	return true;
}

void GNC::ToolZoom::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ZOOM_1_1,_("Reset size to display resolution"), GinkgoResourcesManager::MagnifierBar::GetIcoZoomIn(), new GNC::GUI::EventHandlerZoomTool(pParent, this)));
}


void GNC::ToolZoom::ZoomOneOne()
{
	GNC::GCS::IWidgetsContract::TViewerList listOfViewers;
	WidgetsContract->GetAllViewers(listOfViewers);
	for (GNC::GCS::IWidgetsContract::TViewerList::iterator it = listOfViewers.begin(); it != listOfViewers.end(); ++it)
	{
		GNC::GCS::IWidgetsContract::TViewerList::reference element = (*it);
		if (element) {
			element->ResetZoom(false, (int)GNC::GCS::Events::EventoModificacionImagen::ZM_CenteredOriginalSize);
		}
	}
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

void GNC::ToolZoom::ConnectKeyEvents(wxWindow* pWindow)
{
	if (pWindow) {
		if (m_pHandler != NULL) {
			LOG_WARN("ToolZoom", "Registering key events twice");
			delete m_pHandler;
		}
		m_pHandler = new wxToolZoomKeyHandler(pWindow, this);
	}
}
#endif
