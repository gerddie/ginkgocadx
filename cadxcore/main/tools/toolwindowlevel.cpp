/*
 *  
 *  $Id: toolwindowlevel.cpp $
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

#include "toolwindowlevel.h"
#if defined(ENABLE_WINDOWLEVELTOOL)

#include <api/iwidgetsmanager.h>
#include <api/internationalization/internationalization.h>

#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <widgets/wwindowlevel.h>

#include <api/controllers/ieventscontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <eventos/modificacionimagen.h>

#include <resources/ginkgoresourcesmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkImageData.h>
#include <vtkPointData.h>

#include <vtkImageData.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>
#include <vtkObject.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

#define RESETEAR_WINDOWLEVEL 1221
#define AUTO_CALCULE 1222
#define FIRST_WINDOW_LEVEL 1223

namespace GNC {
	namespace GUI {
		class EventHandlerWindowLevel: public wxEvtHandler {
		public:
			EventHandlerWindowLevel(wxEvtHandler* pParent, GNC::ToolWindowLevel* pHerramienta, GNC::GCS::IWindowLevelContract::ListaWindowLevels* pListaWindowLevels):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				m_pWindowLevelList = pListaWindowLevels;
				// Connect Events
				m_pParent->Connect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerWindowLevel::OnMenuWindowLevelClick),NULL,this);
			}

			~EventHandlerWindowLevel()
			{
				m_pParent->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerWindowLevel::OnMenuWindowLevelClick),NULL,this);
				m_pParent = NULL;
				m_pHerramienta = NULL;
			}

			
			virtual void OnMenuWindowLevelClick( wxCommandEvent& event ){
				if (event.GetId() == RESETEAR_WINDOWLEVEL) {
					m_pHerramienta->Resetear();
				} else if (event.GetId() == AUTO_CALCULE) {
					m_pHerramienta->AutoCalcule();
				} else {
					if (m_pWindowLevelList != NULL) {
						int index = event.GetId() - FIRST_WINDOW_LEVEL;
						GNC::GCS::IWindowLevelContract::ListaWindowLevels::iterator it = m_pWindowLevelList->begin();
						for (int i = 0;index != i && it != m_pWindowLevelList->end(); ++it, ++i);
						if (it != m_pWindowLevelList->end()) {
							m_pHerramienta->SetWindowLevel((*it).m_label);
						}
					}
				}
				event.Skip(false);
			}		

			GNC::ToolWindowLevel* m_pHerramienta;
			wxEvtHandler* m_pParent;
			GNC::GCS::IWindowLevelContract::ListaWindowLevels* m_pWindowLevelList;
		};
	}
}

//----------------------------------------------------------------------

GNC::ToolWindowLevel::ToolWindowLevel()
{
	m_pWindowLevelBuilder = NULL;
}

GNC::ToolWindowLevel::~ToolWindowLevel()
{
	if (m_pWindowLevelBuilder != NULL) {
		delete m_pWindowLevelBuilder;
		m_pWindowLevelBuilder = NULL;
	}
}

GNC::GCS::ITool* GNC::ToolWindowLevel::NewTool()
{
	return new GNC::ToolWindowLevel();
}



void GNC::ToolWindowLevel::Connect()
{
	if (m_pWindowLevelBuilder != NULL)
	{
		delete m_pWindowLevelBuilder;
		m_pWindowLevelBuilder = NULL;
	}

	if (WindowLevelContract->Inicializado()) {
		// Subscribimos los eventos de la ventana a un metodo especifico del panel de esta herramienta
		// Usamos de Id de grupo el valor del puntero de la herramienta, WindowLevelContract es unico y no cambia
		m_pWindowLevelBuilder = new GNC::GCS::Widgets::WWindowLevelBuilder(WindowLevelContract->GetManager(), GetTriggerButton(), (unsigned long)this);
		m_pWindowLevelBuilder->SetObservador(this);
		WindowLevelContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
		WindowLevelContract->GetManager()->SetCursor(m_pWindowLevelBuilder->GetCursor());
	}
}

void GNC::ToolWindowLevel::Disconnect()
{
	if (m_pWindowLevelBuilder != NULL)
	{
		delete m_pWindowLevelBuilder;
		m_pWindowLevelBuilder = NULL;
	}
	WindowLevelContract->GetViewerActivo()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);
	WindowLevelContract->GetManager()->SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
}


bool GNC::ToolWindowLevel::HasDropDownMenu()
{
	return true;
}


void GNC::ToolWindowLevel::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (!WindowLevelContract->GetWindowLevelList().empty())
	{
		double window = -1, level = -1;	
		if (WindowLevelContract->Inicializado()) {
			window = WindowLevelContract->GetCurrentWindow();
			level = WindowLevelContract->GetCurrentLevel();
		}	else {
			return;
		}

		bool inserted = false;
		//first file
		int i = FIRST_WINDOW_LEVEL;
		wxBitmap icon;
		for (GNC::GCS::IWindowLevelContract::ListaWindowLevels::iterator it = WindowLevelContract->GetWindowLevelList().begin(); it != WindowLevelContract->GetWindowLevelList().end(); ++it) 
		{
			if ((*it).m_type == TWindowLevelContract::WindowLevel::TWLT_FILE) 
			{
				wxMenuItem* pItem = new GNC::GUI::wxMenuItemTool(pMenu, i++,wxString::FromUTF8((*it).m_label.c_str()), icon, new GNC::GUI::EventHandlerWindowLevel(pParent, this, &WindowLevelContract->GetWindowLevelList()), wxITEM_CHECK);
				pMenu->Append(pItem);
				pMenu->Check(pItem->GetId(), (window == (*it).m_window && level == (*it).m_level));
				inserted = true;
			}
		}
		if (inserted) {
			pMenu->AppendSeparator();
		}
		inserted = false;
		for (GNC::GCS::IWindowLevelContract::ListaWindowLevels::iterator it = WindowLevelContract->GetWindowLevelList().begin(); it != WindowLevelContract->GetWindowLevelList().end(); ++it) 
		{
			if ((*it).m_type == TWindowLevelContract::WindowLevel::TWLT_PREDEFINED) {
				wxMenuItem* pItem = new GNC::GUI::wxMenuItemTool(pMenu, i++,wxString::FromUTF8((*it).m_label.c_str()), icon, new GNC::GUI::EventHandlerWindowLevel(pParent, this, &WindowLevelContract->GetWindowLevelList()), wxITEM_CHECK);
				pMenu->Append(pItem);
				pMenu->Check(pItem->GetId(), (window == (*it).m_window && level == (*it).m_level));
				inserted = true;
			}
		}
		if (inserted) {
			pMenu->AppendSeparator();
		}
	}

	wxBitmap icon;
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, RESETEAR_WINDOWLEVEL,_("Reset to default"), GinkgoResourcesManager::MenuIcons::GetIcoReset(), new GNC::GUI::EventHandlerWindowLevel(pParent, this, &WindowLevelContract->GetWindowLevelList())));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, AUTO_CALCULE,_("Auto calculate"), icon, new GNC::GUI::EventHandlerWindowLevel(pParent, this, &WindowLevelContract->GetWindowLevelList())));
}


//region "Realización de la interfaz IHerramienta"

void GNC::ToolWindowLevel::OnStart() {
	double window = WindowLevelContract->GetCurrentWindow();
	double level = WindowLevelContract->GetCurrentLevel();
	m_WindowLevelInicial = GNC::GCS::IWindowLevelContract::WindowLevel("",window, level, TWindowLevelContract::WindowLevel::TWLT_PREDEFINED);
}


void GNC::ToolWindowLevel::OnAjuste(TVector& start, TVector& stop) {
	vtkGinkgoImageViewer* pIV = WindowLevelContract->GetViewerActivo();

	int* size = pIV->GetRenderWindow()->GetSize();

	TVector delta = ((stop - start) * 2) / TVector(size[0], size[1]);

	if (std::abs(m_WindowLevelInicial.m_window) > 0.01) {
		delta.x *= m_WindowLevelInicial.m_window;
	}
	else {
		delta.x *= (m_WindowLevelInicial.m_window < 0 ? -0.01 : 0.01);
	}
	if (std::abs(m_WindowLevelInicial.m_level) > 0.01) {
		delta.y *= m_WindowLevelInicial.m_level;
	}
	else {
		delta.y *= (m_WindowLevelInicial.m_level < 0 ? -0.01 : 0.01);
	}

	// Abs so that direction does not flip

	if (m_WindowLevelInicial.m_window < 0.0) {
		delta.x *= -1.0f;
	}
	if (m_WindowLevelInicial.m_level < 0.0) {
		delta.y *= -1.0f;
	}

	// Compute new window level

	double newWindow = delta.x + m_WindowLevelInicial.m_window;
	double newLevel = m_WindowLevelInicial.m_level - delta.y;

	// Stay away from zero and really

	newWindow = std::max((double)0.01f, newWindow);
	if (std::abs(newLevel) < 0.01)
	{
		newLevel = 0.01*(newLevel < 0 ? -1 : 1);
	}

	double newClampedWindow = newWindow;
	double newClampedLevel = newLevel;
	ClampWindowLevel(newWindow, newLevel, newClampedWindow, newClampedLevel);

	WindowLevelContract->SetUserDefinedWindowLevel(newClampedWindow, newClampedLevel);
	SetWindowLevel(newClampedWindow, newClampedLevel);
}

void GNC::ToolWindowLevel::OnEnd() {
}

//endregion

//region "metodos propios de la herramienta window/level"

void GNC::ToolWindowLevel::ClampWindowLevel(double window, double level, double& clampedWindow, double& clampedLevel) {
	clampedWindow = window;
	clampedLevel = level;
}

void GNC::ToolWindowLevel::SetWindowLevel(const std::string& label) 
{	
	for (TWindowLevelContract::ListaWindowLevels::iterator it = WindowLevelContract->GetWindowLevelList().begin(); it != WindowLevelContract->GetWindowLevelList().end(); ++it) 
	{
		if ( (*it).m_label == label) {
			WindowLevelContract->SetLabelCurrent(label);
			SetWindowLevel((*it).m_window, (*it).m_level);
			return;
		}
	}	
}

void GNC::ToolWindowLevel::SetWindowLevel(double window, double level){
	GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(WindowLevelContract->GetManager()->GetVista(),GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada, -1);
	pEvt->SetWindow(window);
	pEvt->SetLevel(level);
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(pEvt);
}

void GNC::ToolWindowLevel::Resetear() {
	wxString wLStr, wWStr;
	vtkGinkgoImageViewer* pIV = WindowLevelContract->GetViewerActivo();
	if (pIV == NULL) {
		return;
	}

	GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(WindowLevelContract->GetManager()->GetVista(),GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada, -1);
	
	pEvt->EnableResetWindowLevel();
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(pEvt);
	
	double window = WindowLevelContract->GetCurrentWindow();
	double level = WindowLevelContract->GetCurrentLevel();

	WindowLevelContract->SetLabelCurrent("");
	SetWindowLevel(window, level);
}

void GNC::ToolWindowLevel::AutoCalcule()
{
	GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(WindowLevelContract->GetManager()->GetVista(),GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada, -1);
	pEvt->EnableAutoCalculeWindowLevel();
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(pEvt);
}

//endregion
#endif