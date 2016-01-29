/*
 *  
 *  $Id: statusbarprogreso.cpp $
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
#include <wx/app.h>
#include <wx/animate.h>
#include <wx/mstream.h>
#include <wx/gauge.h>
#include <wx/msgdlg.h>
#include <wx/statbmp.h>
#include <wx/thread.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include "statusbarprogreso.h"

#include <resources/ginkgoresourcesmanager.h>

#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorvistas.h>
#include <eventos/mensajes.h>
#include <iostream>

IMPLEMENT_DYNAMIC_CLASS(EventoMensajesUsuarioAsync, wxEvent)

//dialogo tareas

//


GNC::GUI::StatusBarProgreso::StatusBarProgreso( wxWindow* pPadre, wxWindow* pPadrePanel, bool listenMessages, wxWindowID id, long style) : 
	m_dirty (true),
	wxStatusBar(pPadre, id, style)
{
	m_pPanelTareas = new PanelTareasBase(pPadrePanel);
	m_pPanelTareas->Show(false);

	SetFieldsCount(Field_Max); //uno para el texto y el otro para lo q yo quiera
	static const int widths[Field_Max] = { -1, 32, 250, 150};
	SetStatusWidths(Field_Max, widths);

	//mensaje tarea
	m_pMensajeTarea = new wxStaticText(this,wxID_ANY,wxT(""), wxDefaultPosition, wxSize(250, -1), wxST_NO_AUTORESIZE);
	m_pMensajeTarea->Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler( StatusBarProgreso::OnMostrarOcultarDialogoProgreso),NULL,this);

	//botoncico de progreso
	m_pIconoProgreso = new wxAnimationCtrl(this,wxID_ANY,GinkgoResourcesManager::ProgressBar::GetAniLoading());
	m_pIconoProgreso->Hide();

	m_pProgresoTarea = new wxGauge(this, wxID_ANY, 100);
	m_pProgresoTarea->Hide();

	m_pIconoParado = new wxStaticBitmap(this,wxID_ANY,GinkgoResourcesManager::ProgressBar::GetIcoStopped());
	m_pIconoParado->Show(true);

	//escuchamos el evento de mensajes
	if (listenMessages) {
		GNC::GCS::Events::EventoMensajes evt(NULL);
		GNC::GCS::ControladorEventos::Instance()->Registrar(this,evt);
	}

	wxSizeEvent event(pPadre->GetSize());
	OnSize(event);

	//onsize
	this->Connect(wxEVT_IDLE, wxIdleEventHandler(StatusBarProgreso::OnIdle), NULL, this);
	this->Connect(wxEVT_SIZE,wxSizeEventHandler(StatusBarProgreso::OnSize),NULL,this);
	this->GetParent()->Connect(wxEVT_MOVE,wxMoveEventHandler(StatusBarProgreso::OnMove),NULL,this);
	this->Connect(wxEVT_MENSAJES_USUARIO_ASYNC, EventoMensajesUsuarioAsyncHandler(StatusBarProgreso::OnMensajeUsuario), NULL, this);
}

GNC::GUI::StatusBarProgreso::~StatusBarProgreso()
{
	m_pMensajeTarea->Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler( StatusBarProgreso::OnMostrarOcultarDialogoProgreso),NULL,this);
	
	this->Disconnect(wxEVT_IDLE, wxIdleEventHandler(StatusBarProgreso::OnIdle), NULL, this);
	this->Disconnect(wxEVT_MENSAJES_USUARIO_ASYNC, EventoMensajesUsuarioAsyncHandler(StatusBarProgreso::OnMensajeUsuario), NULL, this);
	this->GetParent()->Disconnect(wxEVT_MOVE,wxMoveEventHandler(StatusBarProgreso::OnMove),NULL,this);
	this->Disconnect(wxEVT_SIZE,wxSizeEventHandler(StatusBarProgreso::OnSize),NULL,this);

	m_pIconoProgreso->Destroy();
	m_pIconoProgreso = NULL;
	m_pIconoParado->Destroy();
	m_pIconoParado = NULL;
	m_pProgresoTarea->Destroy();
	m_pProgresoTarea = NULL;
}

void GNC::GUI::StatusBarProgreso::InsertarTarea(long taskId, const std::string& text) 
{
	MapaTareasType::iterator it = m_MapaTareas.find(taskId);
	if (it == m_MapaTareas.end()) {

		PanelTarea* pt = new PanelTarea(m_pPanelTareas->m_pTasks, taskId);
		wxString str = wxString::FromUTF8(text.c_str());
		pt->SetProgress(str, 0);
		m_MapaTareas[taskId] = pt;
		pt->Show();
		m_pPanelTareas->m_pSizerTareas->Add(pt, 0, wxEXPAND | wxDOWN, 5);
		int ySize = pt->GetSize().y;
		m_pPanelTareas->m_pTasks->Layout();
		m_pPanelTareas->m_pTasks->SetVirtualSize(-1, ySize * m_MapaTareas.size());
		m_pPanelTareas->m_pTasks->AdjustScrollbars();

		m_pPanelTareas->Layout();

		m_dirty = true;
	}
}

void GNC::GUI::StatusBarProgreso::EliminarTarea(long taskId) {
	PanelTarea* pt = NULL;
	MapaTareasType::iterator it = m_MapaTareas.find(taskId);
	if (it != m_MapaTareas.end()) {
		pt = (*it).second;
		m_MapaTareas.erase(it);

		if (pt != NULL) {
			m_pPanelTareas->m_pSizerTareas->Detach(pt);
			int ySize = pt->GetSize().y;
			pt->Destroy();
			m_pPanelTareas->m_pTasks->SetVirtualSize(-1, ySize * m_MapaTareas.size());
			m_pPanelTareas->m_pTasks->AdjustScrollbars();
			m_pPanelTareas->m_pTasks->Layout();
		}

		m_dirty = true;
	}

}

void GNC::GUI::StatusBarProgreso::SetProgresoTarea(long taskId, float normalizedProgress, const std::string& text) {

	MapaTareasType::iterator it = m_MapaTareas.find(taskId);

	if (it != m_MapaTareas.end()) {
		PanelTarea* pt = (*it).second;
		if (pt != NULL) {
			wxString str = wxString::FromUTF8(text.c_str());
			pt->SetProgress(str, normalizedProgress);
		}

		m_dirty = true;
	}
}

void GNC::GUI::StatusBarProgreso::OnIdle(wxIdleEvent &WXUNUSED(event))
{
	if (m_dirty) {
		if(m_MapaTareas.size() == 0){
			if(m_pIconoProgreso->IsPlaying()){
				m_pIconoProgreso->Stop();
			}
			m_pIconoProgreso->Hide();
			m_pProgresoTarea->Hide();
			m_pIconoParado->Show(true);

			m_pMensajeTarea->SetLabel(_("0 running tasks."));

			if (!m_pPanelTareas->m_pSinTareas->IsShown()) {
				m_pPanelTareas->m_pSinTareas->Show(true);
				m_pPanelTareas->Layout();
			}
		} else {
			if (m_pPanelTareas->m_pSinTareas->IsShown()) {
				m_pPanelTareas->m_pSinTareas->Show(false);
				m_pPanelTareas->m_pTasks->Layout();
				m_pPanelTareas->Layout();
			}
			m_pIconoParado->Hide();
			if(!m_pIconoProgreso->IsPlaying()) {
				m_pIconoProgreso->Show();
				m_pIconoProgreso->Play();
			}
			if(m_MapaTareas.size() == 1) {
				if (!(*m_MapaTareas.begin()).second->GetStatus().empty()) {
					m_pMensajeTarea->SetLabel((*m_MapaTareas.begin()).second->GetStatus());	
				}

				m_pProgresoTarea->SetValue(((*m_MapaTareas.begin()).second->GetProgress() * 100));
				if (!m_pProgresoTarea->IsShown()) {
					m_pProgresoTarea->Show();
				}
			} else {
				m_pMensajeTarea->SetLabel(wxString::Format(_("There are %d pending tasks"), (int) m_MapaTareas.size()));
				if (m_pProgresoTarea->IsShown()) {
					m_pProgresoTarea->Hide();
				}
			}
		}
		m_dirty = false;
	}
	//Layout();
}

void GNC::GUI::StatusBarProgreso::ShowProgressDialog(bool show)
{
	bool isShown = GNC::GCS::ControladorVistas::Instance()->EsVisible(m_pPanelTareas);
	if (!show && isShown) {
		GNC::GCS::ControladorVistas::Instance()->OcultarPanel(m_pPanelTareas);
	} else if (show && !isShown) {
		wxPoint esquinaInferiorDerecha = GetParent()->GetPosition();
		esquinaInferiorDerecha.x += GetParent()->GetSize().x;
		esquinaInferiorDerecha.y += GetParent()->GetSize().y;
		wxPoint posicionDialogo(0,0);
		wxSize mejorSizeDialogo(400,170);
		#if defined (_WINDOWS)
			posicionDialogo.x = esquinaInferiorDerecha.x - mejorSizeDialogo.x - 5;
			posicionDialogo.y = esquinaInferiorDerecha.y - mejorSizeDialogo.y - 5 - GetSize().y;
		# else
			posicionDialogo.x = esquinaInferiorDerecha.x - mejorSizeDialogo.x;
			posicionDialogo.y = esquinaInferiorDerecha.y - mejorSizeDialogo.y;
		#endif
		GNC::GCS::ControladorVistas::Instance()->MostrarPanelFlotante(m_pPanelTareas, posicionDialogo.x, posicionDialogo.y);
	}
}

void GNC::GUI::StatusBarProgreso::OnMostrarOcultarDialogoProgreso(wxMouseEvent& )
{
	ShowProgressDialog(!GNC::GCS::ControladorVistas::Instance()->EsVisible(m_pPanelTareas));
}


void GNC::GUI::StatusBarProgreso::OnSize(wxSizeEvent& event)
{
	wxRect rect;
	GetFieldRect(Field_GifProgreso, rect);
	wxSize size = m_pIconoProgreso->GetSize();

	m_pIconoProgreso->Move(rect.x + (rect.width - size.x) / 2,
					  rect.y + (rect.height - size.y) / 2);
	m_pIconoParado->Move(rect.x + (rect.width - size.x) / 2,
					  rect.y + (rect.height - size.y) / 2);

	GetFieldRect(Field_ProgresoTarea, rect);

	wxPoint esquinaInferiorDerecha = GetParent()->GetPosition();
	esquinaInferiorDerecha.x += GetParent()->GetSize().x;
	esquinaInferiorDerecha.y += GetParent()->GetSize().y;

	m_pProgresoTarea->SetSize(rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4);

	GetFieldRect(Field_TextoTarea, rect);
	#if !defined(__WXOSX__)
	m_pMensajeTarea->SetSize(rect.x + 2, rect.y + 2, rect.width - 4, rect.height - 4);
#else
	m_pMensajeTarea->SetSize(rect.x , rect.y, rect.width, rect.height);
#endif

	event.Skip();
}

void GNC::GUI::StatusBarProgreso::OnMove(wxMoveEvent& event)
{
	wxPoint esquinaInferiorDerecha = GetParent()->GetPosition();
	esquinaInferiorDerecha.x += GetParent()->GetSize().x;
	esquinaInferiorDerecha.y += GetParent()->GetSize().y;
	event.Skip();
}


void GNC::GUI::StatusBarProgreso::OnMensajeUsuario(EventoMensajesUsuarioAsync& event )
{
	switch (event.GetTipo()) {
		case EventoMensajesUsuarioAsync::StatusBar:
			SetStatusText(event.GetTexto());
			break;
		case EventoMensajesUsuarioAsync::PopUp:
			switch(event.GetGravedad()) {
				case EventoMensajesUsuarioAsync::Error:
					wxMessageBox(event.GetTexto(), _("Error"),wxICON_ERROR);
					break;
				case EventoMensajesUsuarioAsync::Aviso:
					wxMessageBox(event.GetTexto(), _("Warning"),wxICON_WARNING);
					break;
				case EventoMensajesUsuarioAsync::Informacion:
					wxMessageBox(event.GetTexto(), _("Info"),wxICON_INFORMATION);
					break;
				default:
					wxMessageBox(event.GetTexto(), _("Info"),wxICON_INFORMATION);
					break;
			}
			break;
		default:
			wxMessageBox(event.GetTexto(), _("Info"),wxICON_INFORMATION);
			break;
	}

}

void GNC::GUI::StatusBarProgreso::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	switch (evt->GetCodigoEvento()) {
		case ginkgoEVT_Core_MensajesUsuario:
			{
				GNC::GCS::Events::EventoMensajes* pEvt = dynamic_cast<GNC::GCS::Events::EventoMensajes*>(evt);
				if(pEvt!=NULL){
					switch(pEvt->GetTipo()) {
						case GNC::GCS::Events::EventoMensajes::StatusMessage :
							{
								switch (pEvt->GetGravedad()) {
									case GNC::GCS::Events::EventoMensajes::Error:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(), EventoMensajesUsuarioAsync::StatusBar, EventoMensajesUsuarioAsync::Error);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									case GNC::GCS::Events::EventoMensajes::Aviso:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(), EventoMensajesUsuarioAsync::StatusBar,EventoMensajesUsuarioAsync::Aviso);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									case GNC::GCS::Events::EventoMensajes::Informacion:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(), EventoMensajesUsuarioAsync::StatusBar, EventoMensajesUsuarioAsync::Informacion);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									default:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(), EventoMensajesUsuarioAsync::StatusBar, EventoMensajesUsuarioAsync::Informacion);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
								}
							}
						break;
						case GNC::GCS::Events::EventoMensajes::PopUpMessage :
							{
								switch (pEvt->GetGravedad()) {
									case GNC::GCS::Events::EventoMensajes::Error:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(),EventoMensajesUsuarioAsync::PopUp, EventoMensajesUsuarioAsync::Error);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									case GNC::GCS::Events::EventoMensajes::Aviso:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(),EventoMensajesUsuarioAsync::PopUp, EventoMensajesUsuarioAsync::Aviso);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									case GNC::GCS::Events::EventoMensajes::Informacion:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(),EventoMensajesUsuarioAsync::PopUp, EventoMensajesUsuarioAsync::Informacion);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
									default:
										{
											EventoMensajesUsuarioAsync event(pEvt->GetMensaje(),EventoMensajesUsuarioAsync::PopUp, EventoMensajesUsuarioAsync::Informacion);
											this->GetEventHandler()->AddPendingEvent(event);
										}
										break;
								}
							}
						break;
					}
				}
			}
			break;
		default:
			break;
	}
}

