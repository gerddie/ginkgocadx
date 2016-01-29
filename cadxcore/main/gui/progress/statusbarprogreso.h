/*
 *  
 *  $Id: statusbarprogreso.h $
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
#pragma once
#include <wx/event.h>
#include <wx/timer.h>
#include <wx/statusbr.h>

#include "progresobase.h"
#include "paneltarea.h"
#include <api/controllers/iprogresscontroller.h>
#include <api/observers/ieventsobserver.h>
#include <map>
#include <api/api.h>


class wxAnimationCtrl;
class wxGauge;
class wxStaticBitmap;

const wxEventType wxEVT_MENSAJES_USUARIO_ASYNC = wxNewEventType();

extern const wxEventType wxEVT_MENSAJES_USUARIO_ASYNC;

/*
BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE( wxEVT_MENSAJES_USUARIO_ASYNC, wxNewEventType() )
END_DECLARE_EVENT_TYPES()
*/
class EventoMensajesUsuarioAsync : public wxEvent
{

public:
	typedef enum {
		StatusBar,
		PopUp
	} TTipoMensaje;

	typedef enum {
		Informacion,
		Aviso,
		Error
	} TipoGravedad;

	EventoMensajesUsuarioAsync(const std::string& texto = "", TTipoMensaje tipo = StatusBar, TipoGravedad gravedad = Informacion) : m_Texto(texto)
	{
		SetEventType(wxEVT_MENSAJES_USUARIO_ASYNC);
		m_tipoMensaje = tipo;
		m_gravedad = gravedad;
	}

	EventoMensajesUsuarioAsync(const EventoMensajesUsuarioAsync& event) : wxEvent(event), m_Texto(event.m_Texto), m_gravedad(event.m_gravedad) { m_tipoMensaje = event.m_tipoMensaje;}

	wxString GetTexto()
	{
		return wxString::FromUTF8(m_Texto.c_str());

	}
	TTipoMensaje GetTipo() {
		return m_tipoMensaje;
	}

	void SetGravedad(TipoGravedad gravedad) {
		m_gravedad = gravedad;
	}

	TipoGravedad GetGravedad() {
		return m_gravedad;
	}

	virtual wxEvent *Clone() const { return new EventoMensajesUsuarioAsync(*this); }

protected:
	std::string m_Texto;
	TTipoMensaje m_tipoMensaje;

	TipoGravedad m_gravedad;

private:
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(EventoMensajesUsuarioAsync)
};


typedef void (wxEvtHandler::*EventoMensajesUsuarioAsyncFunction)(EventoMensajesUsuarioAsync&);

#define EventoMensajesUsuarioAsyncHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(EventoMensajesUsuarioAsyncFunction, &func)

#define EVT_MENSAJES_USUARIO(func) \
    wx__DECLARE_EVT0(wxEVT_MENSAJES_USUARIO_ASYNC, EventoMensajesUsuarioAsyncHandler(func))

namespace GNC {
	namespace GUI {
		class EXTAPI StatusBarProgreso : public wxStatusBar, public GNC::IControladorProgreso, public GNC::GCS::IEventsObserver {
			enum
			 {
				  Field_Text,
				  Field_GifProgreso,
				  Field_TextoTarea,
				  Field_ProgresoTarea,
				  Field_Max
			 };
		public:
			typedef std::map<long, PanelTarea*> MapaTareasType;

			StatusBarProgreso( wxWindow* pPadre, wxWindow* pPadrePanel, bool listenMessages, wxWindowID id = wxID_ANY, long style = wxST_SIZEGRIP );

			~StatusBarProgreso();

			virtual void InsertarTarea(long taskId, const std::string& text);

			virtual void EliminarTarea(long taskId);

			virtual void SetProgresoTarea(long taskId, float normalizedProgress, const std::string& text);

			virtual void ShowProgressDialog(bool show);


		protected:
			void OnMostrarOcultarDialogoProgreso(wxMouseEvent& event);
			void OnSize(wxSizeEvent& event);
			void OnMove(wxMoveEvent& event);

			void OnMensajeUsuario(EventoMensajesUsuarioAsync& event );
			void OnIdle(wxIdleEvent &WXUNUSED(event));


		public:
			void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;

			MapaTareasType m_MapaTareas;
			PanelTareasBase*	     m_pPanelTareas;
			wxAnimationCtrl* m_pIconoProgreso;
			wxGauge* m_pProgresoTarea;
			wxStaticText* m_pMensajeTarea;
			wxStaticBitmap* m_pIconoParado;
			bool m_dirty;

		};
	}
}
