/*
 *  
 *  $Id: showtagstool.cpp $
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
#include "showtagstool.h"
#include <export/contracts/iviewcontract.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <api/controllers/ieventscontroller.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <main/gui/paneltags/paneltags.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <eventos/modificacionimagen.h>

namespace GNC {
	namespace GUI {
		class EventHandlerShowTagsTool: public GNC::GUI::wxDefaultEvtHandlerTool {
		public:
			EventHandlerShowTagsTool(wxEvtHandler* pParent, GNC::ShowTagsTool* pTool): GNC::GUI::wxDefaultEvtHandlerTool(pParent, pTool)
			{
				m_pHerramienta = pTool;
				m_pParent = pParent;
			}
			~EventHandlerShowTagsTool()
			{
			}

			virtual void OnUpdateUI(wxUpdateUIEvent &event)
			{
				wxDefaultEvtHandlerTool::OnUpdateUI(event);
				if(m_pHerramienta->IsVisible()) {
					event.SetText(_("Hide DICOM inspector"));	
				} else {
					event.SetText(_("Show DICOM inspector"));	
				}
			}

			GNC::ShowTagsTool* m_pHerramienta;
			wxEvtHandler* m_pParent;			
		};
	}
}


GNC::GCS::ITool* GNC::ShowTagsTool::NewTool()
{
	return new GNC::ShowTagsTool();
}

GNC::ShowTagsTool::ShowTagsTool()
{
	m_pPanelTags = NULL;
}
GNC::ShowTagsTool::~ShowTagsTool()
{
	if (m_pPanelTags != NULL) {
		m_pPanelTags->Close(true);
		m_pPanelTags->Destroy();
	}
}
		
bool GNC::ShowTagsTool::ExecuteAction()
{
	if (m_pPanelTags == NULL) {
		m_pPanelTags = new GNC::GUI::PanelTags(GNC::Entorno::Instance()->GetVentanaRaiz());
		GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::EventoModificacionImagen(ViewContract->GetView()));
	}
	if (m_pPanelTags != NULL) {
		if (!IsVisible()) {
			if (ViewContract->GetView() != NULL) {
				m_pPanelTags->SetView(ViewContract->GetView());
			} else {
				m_pPanelTags->SetView(NULL);
			}
		}
		m_pPanelTags->MostrarOcultarPanelTags();
	}
	return true;
}


void GNC::ShowTagsTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		GNC::GUI::EventHandlerShowTagsTool* evtHandler = new GNC::GUI::EventHandlerShowTagsTool(pParent, this);
		pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
	}
}

bool GNC::ShowTagsTool::IsVisible()
{
	bool isVisible =false;

	if (m_pPanelTags != NULL) {
		isVisible = m_pPanelTags->IsVisiblePanelTags();
	}
			
	return isVisible;
}

void GNC::ShowTagsTool::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	if ( evt->GetCodigoEvento() == ginkgoEVT_Core_ModificacionImagen) {
		GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
		if (pEvt->GetTipo() == GNC::GCS::Events::EventoModificacionImagen::SliceCambiado 
			|| pEvt->GetTipo() == GNC::GCS::Events::EventoModificacionImagen::ImagenCargada
			|| pEvt->GetTipo() == GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas) {
			if (m_pPanelTags != NULL) {
				if (IsVisible()) {
					if (ViewContract->GetView() != NULL) {
						m_pPanelTags->SetView(ViewContract->GetView());
					} else {
						m_pPanelTags->SetView(NULL);
					}
				}
			}
		}
	}
}

