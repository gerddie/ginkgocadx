/*
 *  
 *  $Id: ventanaprincipal.h $
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

#include <api/internal.h>
#include "ventanaprincipalbase.h"

#include <wx/event.h>
#include <wx/aui/aui.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include <api/globals.h>

#include <api/toolsystem/itool.h>
#include <api/observers/ieventsobserver.h>
#include <api/observers/iviewsobserver.h>
#include <api/observers/iextensionsobserver.h>

namespace GNC {
	namespace GUI {
		class PanelGrid;
	}
	namespace GCS {
		class IEntorno;
	}
}
class wxWindowDisabler;
class wxScrolledWindow;
class wxStaticBitmap;
class wxBitmapButton;

const wxEventType wxEVT_PROGRESO = wxNewEventType();

extern const wxEventType wxEVT_PROGRESO;

//DEFINE_EVENT_TYPE(wxEVT_PROGRESO)

/*BEGIN_DECLARE_EVENT_TYPES()
	DECLARE_EVENT_TYPE( wxEVT_PROGRESO, wxNewEventType() )
END_DECLARE_EVENT_TYPES()
*/
class EventoProgreso : public wxEvent
{
public:

	typedef enum
	{
		ComandoIniciado = 0,
		ComandoEjecutando,
		ComandoFinalizado,
		Invalido
	} TipoProgreso;

	EventoProgreso(TipoProgreso tipo = Invalido, long threadId = 0);

	EventoProgreso(const EventoProgreso& event);

	~EventoProgreso();

	long GetTipo() const
	{
		return m_Tipo;
	}

	long GetThreadId() const
	{
		return m_ThreadId;
	}

	virtual wxEvent *Clone() const;

protected:
	TipoProgreso m_Tipo;
	long         m_ThreadId;

private:
    DECLARE_DYNAMIC_CLASS_NO_ASSIGN(EventoProgreso)
};

typedef void (wxEvtHandler::*EventoProgresoFunction)(EventoProgreso&);

#define EventoProgresoFunctionHandler(func) \
    (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(EventoProgresoFunction, &func)

#define EVT_PROGRESO(func) \
    wx__DECLARE_EVT0(wxEVT_PROGRESO, EventoProgresoFunctionHandler(func))


class EXTAPI VentanaPrincipal : public VentanaPrincipalBase, public GNC::GCS::IExtensionsObserver, public GNC::GCS::IObservadorVistas, public GNC::GCS::IEventsObserver {
public:

	/* Constructor */
	VentanaPrincipal(wxWindow* pParent, const wxString& title);

	/* Destructor */
	~VentanaPrincipal();

	/*cambia de pestaña*/
	void ForzarCambioVista(GNC::GCS::IVista* pVista);

protected:

//region "Helpers"

public:
	void RefrescarMenus();
	void CargarHistorial();

//endregion

//region "Eventos de actualizaciones"

protected:
	std::string m_VersionNueva; // Cadena con la version nueva para mostrar en mensajes y notificaciones (Ej: "1.5.0 beta")
	std::string m_VersionCheck; // Cadena con la version nueva con secuencia build para fichar version y poder ignorarla. (Ej: "1.5.0.0", que podria ser 1.5.0 beta | "1.5.0.1" que podria ser 1.5.0 final)
	bool        m_VersionChecked;
public:
	void ComprobarActualizaciones();

	virtual void OnDoNotWarnAgainClicked( wxCommandEvent& event );
	virtual void OnVersionNotifierClose( wxCommandEvent& event );

//endregion

//region "Eventos de menu"
public:
	/* Evento de cerrar aplicacion (evitable) */
	void OnWindowClose(wxCloseEvent& event);

	/* Evento de salir de la aplicacion */
	virtual void OnSalir(wxCommandEvent& event);

	/* Evento de lanzamiento de ventana "Acerca De" */
	virtual void OnAcercaDe(wxCommandEvent& event);

	/* Evento de lanzamiento de ventana "Online Help" */
	virtual void OnOnlineHelp(wxCommandEvent& event);

	/* Evento de lanzamiento de ventana "Online support" */
	virtual void OnOnlineSupport(wxCommandEvent& event);

	/* Open test card */
	virtual void OnOpenTestCard(wxCommandEvent& event);
	
	/* Evento de lanzamiento de la comprobacion de actualizaciones */
	virtual void OnCheckForUpdates(wxCommandEvent& event);

	#ifdef __WXMAC__
	void MacOpenFile(const wxString &fileName);
	#endif

	/* Evento de configuracion */
	virtual void OnConfiguracion(wxCommandEvent& event);

	/*Evento de ventana de control de envio HL7*/
	virtual void OnMensajesHL7(wxCommandEvent& event);

	/*Evento de ventana de control de registros (Logs)*/
	virtual void OnControlLogs(wxCommandEvent& event);

	/* Evento de aplicacion de herramientas*/
	virtual void OnAplicarHerramienta(wxCommandEvent& event);
	virtual void OnUpdateUIHerramienta(wxUpdateUIEvent& event);
	virtual void InsertToolsFromFamily(wxMenu* pParent, GNC::GCS::ITool::TToolFamily family);

	/* Eventos de teclado */
	virtual void OnKeyDown(wxKeyEvent& event);

	/*menu sobre pestanias*/
	void OnMenuCerrarTabClick(wxCommandEvent& event);

	void OnMenuCerrarTodoExceptoActiva(wxCommandEvent& event);

	void OnMenuCerrarTodosTabsClick(wxCommandEvent& event);

	//devuelve bool si ha cerrado todos
	bool CerrarTodosLosTabsYPanelGrids();

	//closes all undocked windows
	bool ClosesAllUndocked();

	void OnMenuCerrarTabUpdateUI(wxUpdateUIEvent& event);

	void OnMenuCerrarTodosTabsUpdateUI(wxUpdateUIEvent& event);

	void OnMenuPrimerPlanoPestaniaActiva(wxCommandEvent& event);

	/*Eventos de vista*/
	void OnPantallaCompleta(wxCommandEvent& event);

	wxWindow* GetRootWindow();

	/*reorganizacion de pestañas*/

	//toggle entre enc
	void ShowStartUp(bool show);
	void EncajarDesencajar(GNC::GCS::IVista* pVista);
	void OnMosaicoRestaurar(wxCommandEvent& event);
	void ReplegarPestanias();

//endregion

public:
//region "Eventos de ventana"

	/*evento que se ejecuta cuando se activa la app sirve para parsear los parametros*/
	void OnInicioAplicacion(wxCommandEvent& event);
	void EjecutarParametro(const wxString cadena, bool doLogin = false);

	/* Cambio de tamaño de la ventana */
	void OnSize(wxSizeEvent& event);

//endregion

//region "Eventos del control de pestañas"
	void OnPanelClose(wxAuiManagerEvent& event);
public:
	bool CerrarVista(GNC::GCS::IVista* pVista);
	bool CerrarVista(wxWindow* window);
	void PasarAPrimerPlano(wxWindow* window);
protected:
	void OnNotebookPageChanging(wxAuiNotebookEvent& event);
	void OnNotebookPageChanged(wxAuiNotebookEvent& event);
	void OnNotebookPageClose(wxAuiNotebookEvent&   event);
	void OnNotebookCambiarTab(wxAuiNotebookEvent&  event);
	void OnNotebookMenuTab(wxAuiNotebookEvent&     event);
	void OnNotebookFocus(wxChildFocusEvent&        event);


public:
	void DestruirPanelVista(wxWindow* panel);

protected:
	void CerrarNotebookPage(wxWindow* panel);

//endregion

//region "eventos ginkgo"
protected:
	void ProcesarEvento(GNC::GCS::Events::IEvent *evt);
//endregion

//endregion


//region "Puntos de entrada al hilo principal de la interfaz para controlar comandos"

	void OnComando(EventoProgreso& event);

//endregion

//region "Punto de entrada al controlador de eventos"
	void OnEvento(wxCommandEvent& event);
//endregion

public:
	/* Inserta una ventana en el notebook o grid  */
	void InsertarVentana(wxWindow* pVista);

	/* Elimina una vista del notebook o grid */
	void EliminarVentana(wxWindow* pVentana);

//region "Getters y Setters de elementos de la interfaz"

public:
	wxAuiNotebook* GetAUINoteBook();

public:

//------------------------------------------------------------------------------
//region "IExtensionsObserver realization"

	/** The module is loaded **/
	virtual void OnModuleLoaded(GNC::GCS::IModuleController* pMC);
	/** The module is unloaded **/
	virtual void OnModuleUnloaded(GNC::GCS::IModuleController* pMC);

	/** The module is activated **/
	virtual void OnModuleActivated(GNC::GCS::IModuleController* pMC);
	/** The module is deactivated **/
	virtual void OnModuleDeactivated(GNC::GCS::IModuleController* pMC);

//endregion

//region "Realización de la interfaz IObservadorVistas"

	virtual void OnVistaActivada(GNC::GCS::IVista* pVista);

//endregion

	wxAuiManager          m_mgr;
	wxAuiNotebook*        m_pNoteBook;
	GNC::GCS::IVista*     StartupView;


//region "helpers"
	void SuperFreeze();
	void SuperThaw();
	void PauseSuperFreeze();
	void ContSuperFreeze();

protected:
	int m_SuperFreezeCount;
	wxWindowDisabler* disabler; // Implementación similar a freeze/thaw pero a nivel de eventos y no respecto a responsabilidad de widgets.

//endregion


//region "login e inicializacion"
public:
	void Login();
	void MostrarVentana();

//endregion

protected:
	wxMenu*                     m_pMenuModulo;

	// Variable que controla la coherencia de confirmación entre cerrado de la aplicacion y cerrado de tabs para
	// no informar varias veces de lo mismo.
	bool                  m_TratandoDeCerrar;

	wxMenuBar*            m_pMenuBar;

	DECLARE_EVENT_TABLE()
	//friend class GNC::GCS::ControladorVistas;
	bool                  m_Iniciada;
};

/* Acciones de Menus */
enum {
	ID_Inicio_Aplicacion = 99
};
