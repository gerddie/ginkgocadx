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


#include <iostream>
#include <sstream>

#include <wx/wx.h>
#include <wx/zstream.h>
#include <wx/wfstream.h>
#include <wx/aui/aui.h>
#include <wx/app.h>
#include <wx/propgrid/propgrid.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/mstream.h>
#include <wx/textfile.h>
#include <wx/statbmp.h>

#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/html/htmlwin.h>
#include <wx/gbsizer.h>
#include <wx/txtstrm.h>
#include <wx/display.h>
#include <wx/cmdline.h>
#include <wx/stdpaths.h>
#include <wx/mimetype.h>

#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/gui/mainwindow/detacheddialog.h>
#include <main/gui/login/dialogologin.h>
#include <main/gui/open/abrir.h>
#include <main/gui/license/aboutdialog.h>
#include <main/gui/license/acceptlicensedialog.h>
#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <main/gui/acquisition/dialogoadquisicion.h>
#include <main/gui/ginkgocloud/ginkgocloudsearchdialog.h>
#include <main/gui/history3/historypanel3.h>
#include <main/gui/progress/statusbarprogreso.h>
#include <main/gui/configuration/dialogoconfiguracion.h>
#include <main/gui/hl7controlwindow/ventanacontrolhl7.h>
#include <main/gui/logcontrolwindow/ventanacontrollogs.h>
#include <main/gui/startup/startupview.h>
#include <main/gui/droptargetmainwindow.h>

#include <api/globals.h>
#include <api/ivista.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/ipacscontroller.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/toolsystem/itoolcontroller.h>
#include <api/toolsystem/itoolsregistry.h>

#include <main/entorno.h>

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/integrationcontroller.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/controladorhl7.h>
#include <main/controllers/controladorenviohl7.h>
#include <main/controllers/controladorautenticacion.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/windowattributescontroller.h>

#include <commands/comandoactualizaciones.h>
#include <commands/comandoincluirhistorial.h>
#include <commands/openremovableunit.h>
#include <commands/commandsynchronizewithgui.h>

#include <eventos/updateavailable.h>

#include <resources/ginkgoresourcesmanager.h>

#define ID_CERRAR 1054
#define ID_CERRAR_TODAS 1055
#define ID_SALIR 1059
#define ID_ONLINE_SUPPORT 1060
#define ID_ACERCA_DE 1062
#define ID_CONFIGURACION 1063
#define ID_PANTALLA_COMPLETA 1070
#define ID_MOSAICO_RESTAURAR 1075
#define ID_CONTROLHL7 1076
#define ID_CONTROLLOGS 1077
#define ID_CHECK_UPDATES 1078
#define ID_ONLINE_MANUAL 1079
#define ID_EXTENSIONS_SUPPORT 1081
#define ID_OPEN_TEST_CARD 1082

#define MAIN_WINDOW_ID "GinkgoCADxMain"

//////////////////////////
#if defined(GINKGO_PRO)
int InitTest(const std::string& pathTests);
/**
launches cppunit tests
*/
class TestLauncher: public wxThread
{
public:
	TestLauncher(const std::string& pathTests) :m_path(pathTests)
	{
	}
	virtual void *Entry() 
	{
		LOG_INFO("INITWX", "Launching from external thread");
		InitTest(m_path);
		LOG_INFO("INITWX", "tests complete, launching closing command");

		class CloseCommand: public GADAPI::ISynchronizedWithGUI
		{
			virtual void ExecuteSynchronized(GADAPI::ISynchronizedData* )
			{
				LOG_INFO("INITWX", "Closing Ginkgo CADx...");
				GNC::Entorno::Instance()->GetVentanaPrincipal()->Close(true);
				wxExit();
				exit(0);
			}
		};
		GADAPI::CommandSynchronizeWithGUI* pCmd = new GADAPI::CommandSynchronizeWithGUI(new CloseCommand());
		GNC::CommandController::Instance()->ProcessAsync("", pCmd, NULL);
		LOG_INFO("INITWX", "closing command launched");
		
		return NULL;
	}
	std::string m_path;
};
#endif
///////////

class wxGinkgoAuiNoteBook: public wxAuiNotebook
{
public:
	wxGinkgoAuiNoteBook(): wxAuiNotebook()
	{ }

    wxGinkgoAuiNoteBook(wxWindow* parent,
                  wxWindowID id = wxID_ANY,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
				  long style = wxAUI_NB_DEFAULT_STYLE): wxAuiNotebook(parent, id, pos, size, style)
    { 
    }

    virtual ~wxGinkgoAuiNoteBook()
	{
	}
	//hack to expose this method
	virtual wxAuiTabCtrl* GetActiveTabCtrl()
	{
		return wxAuiNotebook::GetActiveTabCtrl();
	}
};

////


EventoProgreso::EventoProgreso(TipoProgreso tipo, long threadId)
{
	SetEventType(wxEVT_PROGRESO);
	m_Tipo = tipo;
	m_ThreadId = threadId;
}

EventoProgreso::EventoProgreso(const EventoProgreso& event) : wxEvent(event), m_Tipo(event.m_Tipo), m_ThreadId(event.m_ThreadId)
{
}

EventoProgreso::~EventoProgreso()
{
}

wxEvent* EventoProgreso::Clone() const
{
	return new EventoProgreso(*this);
}

IMPLEMENT_DYNAMIC_CLASS(EventoProgreso, wxEvent)

// Mapeo de eventos

BEGIN_EVENT_TABLE(VentanaPrincipal, VentanaPrincipalBase)
EVT_CLOSE(VentanaPrincipal::OnWindowClose)
EVT_MENU(ID_Inicio_Aplicacion, VentanaPrincipal::OnInicioAplicacion)
END_EVENT_TABLE()

VentanaPrincipal::VentanaPrincipal(wxWindow* pParent, const wxString& title) :
VentanaPrincipalBase(pParent, wxID_ANY, title, wxDefaultPosition, wxSize( 800,600 ), wxDEFAULT_FRAME_STYLE),
        m_mgr(m_pPanelCentral, wxAUI_MGR_ALLOW_FLOATING | wxAUI_MGR_TRANSPARENT_DRAG | wxAUI_MGR_VENETIAN_BLINDS_HINT ),
	StartupView(NULL)
{
	//load attributes
	GNC::GCS::WindowAttributesController::LoadWindowAttributes(MAIN_WINDOW_ID, this, false);
	//

    //SetDoubleBuffered(true);
	m_Iniciada = false;
	m_VersionChecked = false;
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
	this->SetIcon(icono);

	m_TratandoDeCerrar = false;
	m_SuperFreezeCount = 0;

	disabler = NULL;

	GNC::Entorno* pEntorno = GNC::Entorno::Instance();

	pEntorno->SetVentanaPrincipal(this);
	pEntorno->SetVentanaRaiz(m_pPanelCentral);

	SetAutoLayout(true);

	// Subscripción a la notificación de carga y descarga de extensiones
	pEntorno->ExtensionsObservers.push_back(this);

	// Subscripción a la notificación de activación y desactivación de vistas/estudios
	pEntorno->ViewsObservers.push_back(this);
	
	m_mgr.SetDockSizeConstraint(0.5f, 0.5f);

	//menu and tool bar
	m_pMenuBar = new wxMenuBar( wxMB_DOCKABLE );
	//

	GNC::GUI::StatusBarProgreso* pStatusBar = new GNC::GUI::StatusBarProgreso(this, m_pPanelCentral, true);
	SetStatusBar(pStatusBar);

	{
		wxString wxCaption = _("Tasks");
		wxAuiPaneInfo pi(wxAuiPaneInfo().Caption(wxCaption).Float().BestSize(400,170).MinSize(200,100).Resizable(true).CloseButton(true).Show(false));
		pi.DestroyOnClose(false);
		m_mgr.AddPane(pStatusBar->m_pPanelTareas, pi);
	}

	GNC::CommandController::Instance()->RegistrarProgreso(pStatusBar);
	Connect(wxEVT_PROGRESO, EventoProgresoFunctionHandler(VentanaPrincipal::OnComando), NULL, this);

	m_pNoteBook = new wxGinkgoAuiNoteBook(m_pPanelCentral, wxID_ANY,wxDefaultPosition,
                                              wxDefaultSize,
                                              wxAUI_NB_TAB_SPLIT |  wxAUI_NB_TAB_MOVE | wxAUI_NB_SCROLL_BUTTONS
                                              | wxAUI_NB_WINDOWLIST_BUTTON | wxAUI_NB_CLOSE_ON_ALL_TABS | wxEXPAND );
	m_pNoteBook->SetBackgroundColour(wxColour(0, 0, 0));
	m_pNoteBook->Connect( wxEVT_CHILD_FOCUS, wxChildFocusEventHandler( VentanaPrincipal::OnNotebookFocus ) );

	m_mgr.AddPane(m_pNoteBook, wxAuiPaneInfo().Name(wxT("vistas")).CenterPane().Layer(0).PaneBorder(true).Movable(false).Floatable(false).Dockable(false).LeftDockable(false).RightDockable(false).TopDockable(false).BottomDockable(false).CloseButton(false).Hide());
	//

	wxString wxCaption;
	m_mgr.Update();


	//hay que conectarlo al auimanager, porque el notebook delega el procesado de eventos al auimanager
	{
		m_mgr.Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookPageClose),NULL,this);
		m_mgr.Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGING, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookPageChanging), NULL, this);
		m_mgr.Connect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookPageChanged),NULL,this);
		m_mgr.Connect(wxEVT_COMMAND_AUINOTEBOOK_TAB_RIGHT_UP, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookMenuTab),NULL,this);
		m_mgr.Connect(wxEVT_COMMAND_AUINOTEBOOK_TAB_RIGHT_DOWN, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookCambiarTab),NULL,this);

		m_mgr.Connect(wxEVT_AUI_PANE_CLOSE, wxAuiManagerEventHandler(VentanaPrincipal::OnPanelClose),NULL,this);
	}

	SetDropTarget(new GNC::GUI::DropTargetMainWindow());

	Layout();

}

VentanaPrincipal::~VentanaPrincipal()
{
	m_pNoteBook->Disconnect( wxEVT_CHILD_FOCUS, wxChildFocusEventHandler( VentanaPrincipal::OnNotebookFocus ) );

	Disconnect(wxEVT_PROGRESO, EventoProgresoFunctionHandler(VentanaPrincipal::OnComando), NULL, this);
	GNC::CommandController::Instance()->RegistrarProgreso(NULL);
	GNC::GUI::DialogoAdquisicion::FreeInstance();
	GNC::GUI::GinkgoCloudSearchDialog::FreeInstance();
	GNC::GCS::ICommandController::Instance()->AbortarComandosDeOwner(NULL);
	//cierra la ventana si esta abierta
	GNC::GUI::VentanaControlHL7::CerrarSiAbierta();
	GNC::GUI::VentanaControlLogs::CerrarSiAbierta();

	{
		m_mgr.Disconnect(wxEVT_COMMAND_AUINOTEBOOK_PAGE_CLOSE, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookPageClose),NULL,this);
		m_mgr.Disconnect(m_pNoteBook->GetId(), wxEVT_COMMAND_AUINOTEBOOK_PAGE_CHANGED, wxAuiNotebookEventHandler(VentanaPrincipal::OnNotebookPageChanged),NULL,this);
	}


	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMenuCerrarTabClick ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( VentanaPrincipal::OnMenuCerrarTabUpdateUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMenuCerrarTodosTabsClick ) );
	this->Disconnect( wxID_ANY, wxEVT_UPDATE_UI, wxUpdateUIEventHandler( VentanaPrincipal::OnMenuCerrarTodosTabsUpdateUI ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnSalir ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnConfiguracion ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMensajesHL7 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnControlLogs ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnAcercaDe ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnPantallaCompleta ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMosaicoRestaurar ) );

	//delete history tools
	GNC::HistoryToolsController::Instance()->UnRegisterAll();

	//
	GNC::Entorno* pEntorno = GNC::Entorno::Instance();

	GNC::ControladorExtensiones::Instance()->DesRegistrarModulos();
	GNC::GCS::ControladorVistas::FreeInstance();
	m_mgr.UnInit();
	pEntorno->ExtensionsObservers.remove(this);
	pEntorno->ViewsObservers.remove(this);
	pEntorno->SetVentanaPrincipal(NULL);
	pEntorno->SetVentanaRaiz(NULL);

	//save attributes
	GNC::GCS::WindowAttributesController::SaveWindowAttributes(MAIN_WINDOW_ID, this);
}

//region "Eventos de comprobacion de actualizaciones"
void VentanaPrincipal::ComprobarActualizaciones()
{
	bool check;
	GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/Estacion", "CheckForUpdates", check, true);
	if (check) {
		m_VersionChecked = false;
		//this is the profile
		GNC::GCS::Permisos::EstadoPermiso updates = GNC::GCS::IControladorPermisos::Instance()->Get("core.update","check_updates");
		if (updates) {
			bool tryCheck = false;
			std::string lastDateStr;
			if (GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/LastUpdateDateTime", "LastDate", lastDateStr)) 
			{
				GNC::GCS::Permisos::EstadoPermiso period = GNC::GCS::IControladorPermisos::Instance()->Get("core.update","update_period");
				std::string periodStr = period.ObtenerValor<std::string>();
				long iPeriod;
				if (!wxString::FromUTF8(periodStr.c_str()).ToLong(&iPeriod)) {
					iPeriod = 2;
				}

				wxDateTime lastDate;

				lastDate.ParseFormat(wxString::FromUTF8(lastDateStr.c_str()), wxT("%d/%m/%Y %H:%M:%S"));
				if (!lastDate.IsValid()) {
					lastDate = wxDateTime::Now();
					lastDate.Subtract( wxDateSpan(0, 0, 0, iPeriod + 1 ) );
				}

				if (wxDateTime::Now().Subtract(lastDate).GetDays() > iPeriod) {
					tryCheck = true;
					GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Check Updates"),
						new GNC::ComandoComprobarActualizaciones(new GNC::ComandoComprobarActualizacionesParams(updates.ObtenerValor<std::string>(), false) ), this);
				}

			} else {
				//if there is no date in the ini file update will be checked
				GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Check Updates"),
					new GNC::ComandoComprobarActualizaciones(new GNC::ComandoComprobarActualizacionesParams(updates.ObtenerValor<std::string>(), false) ), this);
				tryCheck = true;
			}

			if (tryCheck) {
				//writes date
				GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/LastUpdateDateTime", "LastDate", std::string(wxDateTime::Now().Format(wxT("%d/%m/%Y %H:%M:%S")).ToUTF8()));
			}
		}
	}

}

void VentanaPrincipal::OnDoNotWarnAgainClicked( wxCommandEvent& /*event*/ )
{
	if (m_VersionChecked) {
		wxString valor;
                GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/SkipUpdate", m_VersionCheck, "1");
		GNC::GCS::ConfigurationController::Instance()->Flush();
	}

}

void VentanaPrincipal::OnVersionNotifierClose( wxCommandEvent& /*event*/ )
{
	wxWindowDisabler dis;
	SuperFreeze();
	m_pPanelActualizacion->Hide();
	//m_pPanelCentral->Layout();
	Layout();
	Refresh(true);
	SuperThaw();

}
//endregion



void VentanaPrincipal::ForzarCambioVista(GNC::GCS::IVista* pVistaCambiar)
{
	//se cambia la pestaña
	//bool cerrar = true;

	this->SetFocus();
	GNC::GCS::ControladorVistas* pcv = GNC::GCS::ControladorVistas::Instance();
	if (pcv == NULL) {
		std::cerr << "Error: No se pudo obtener el controlador de vistas" << std::endl;
		return ;
	}

	if (pcv->GetVistaActiva() != pVistaCambiar) {
		pcv->SolicitarActivarVista(pVistaCambiar);
	}

	for (GNC::GCS::ControladorVistas::MapaVentanas::iterator it = pcv->m_MapaVentanas.begin(); it != pcv->m_MapaVentanas.end(); ++it) {
		wxWindow* pVentana = (*it).first;
		GNC::GCS::IVista*   pVista = (*it).second;
		if (pVentana != NULL && pVista != NULL) {
			if (pVistaCambiar == pVista) {
				//Tenemos la ventana, la vista y el estudio
				//primero cambiamos de pestaña
				int selection = m_pNoteBook->GetPageIndex(pVentana);
				if (m_pNoteBook->IsShown() && selection >= 0) {
					if(selection >= 0 && selection != m_pNoteBook->GetSelection()){
						wxWindowDisabler dis;
						SuperFreeze();
						m_pNoteBook->SetSelection(selection);
						SuperThaw();
						return;
					}
				} else {
					//desencajado
					GNC::GUI::DetachedDialog* pDialogo = dynamic_cast<GNC::GUI::DetachedDialog*> (pVentana->GetParent());
					if(pDialogo != NULL) {
						if (pDialogo->IsIconized()) {
							pDialogo->Iconize(false);
						}
						pDialogo->Raise();
					}
				}
			}
		}
	}
}

void VentanaPrincipal::OnSalir(wxCommandEvent& WXUNUSED(event))
{
	SuperFreeze();
	Close();
	SuperThaw();
}
void VentanaPrincipal::OnAcercaDe(wxCommandEvent& WXUNUSED(event))
{
	AboutDialog(this).ShowModal();
}


void VentanaPrincipal::OnOnlineHelp(wxCommandEvent& /*event*/)
{
	GNC::GCS::Permisos::EstadoPermiso manual = GNC::GCS::IControladorPermisos::Instance()->Get("core.help","url_manual");
	wxString urlManual = wxString::FromUTF8(manual.ObtenerValor<std::string>().c_str());
	if (urlManual != wxEmptyString) {
		if (urlManual.StartsWith(wxT("http://"))) {
			wxLaunchDefaultBrowser(urlManual);
		} else {
			urlManual.Replace(wxT("\\"), wxString(wxFileName::GetPathSeparator()));
			wxFileName executable_path = wxStandardPaths::Get().GetExecutablePath();
			wxString pathDocument = executable_path.GetPath() + wxFileName::GetPathSeparator() + urlManual;
			wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromMimeType(wxT("application/pdf"));
			if (fileType == NULL) {
				wxMessageBox(_("There isn't any pdf reader registered in the system"), _("Info"), wxICON_WARNING|wxOK);
				return;
			}

			wxString openCmd = fileType->GetOpenCommand(pathDocument);
			if (!openCmd.IsEmpty()) {
				wxExecute(openCmd);
			} else {
				wxMessageBox(_("There isn't any pdf reader registered in the system"), _("Info"), wxICON_WARNING|wxOK);
			}
			
			delete fileType;
		}
	}
}

void VentanaPrincipal::OnOnlineSupport(wxCommandEvent& /*event*/)
{
	GNC::GCS::Permisos::EstadoPermiso updates = GNC::GCS::IControladorPermisos::Instance()->Get("core.help","url_support");
	wxString urlSupport = wxString::FromUTF8(updates.ObtenerValor<std::string>().c_str());
	if (urlSupport != wxEmptyString) {
		wxLaunchDefaultBrowser(urlSupport);
	}
}

void VentanaPrincipal::OnOpenTestCard(wxCommandEvent& /*event*/)
{
	wxFileName cpath = FROMPATH(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir()) + wxFileName::GetPathSeparator() + wxT("test_card.dcm");
	std::string stdpath(cpath.GetFullPath().ToUTF8());
	GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(stdpath, false, GNC::GCS::IHistoryController::TAA_LINK);
	pParams->m_abrirDespuesDeCargar = true;
	GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
	GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Included in the history ..."),pCmd, NULL);
}

/* Evento de lanzamiento de la comprobacion de actualizaciones */
void VentanaPrincipal::OnCheckForUpdates(wxCommandEvent& /*event*/)
{
	m_VersionChecked = false;
	GNC::GCS::Permisos::EstadoPermiso updates = GNC::GCS::IControladorPermisos::Instance()->Get("core.update","check_updates");
	if (updates.ObtenerValor<std::string>().empty()) {
		wxMessageBox(_("There is no URL to check updates on the current configuration."), _("Check update error"),wxICON_ERROR);
	}
	else {
		GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Check Updates"),
			new GNC::ComandoComprobarActualizaciones(new GNC::ComandoComprobarActualizacionesParams(updates.ObtenerValor<std::string>(), true) ), this);

		GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/LastUpdateDateTime", "LastDate", std::string(wxDateTime::Now().Format().ToUTF8()));
	}
}

/*startup page*/


class DialogoComandosEnEjecucion: public DialogoComandosEnEjecucionBase
{
public:
	DialogoComandosEnEjecucion(wxWindow* pParent, std::list<std::string>& listaNombres) : DialogoComandosEnEjecucionBase(pParent)
	{
		for (std::list<std::string>::iterator it = listaNombres.begin(); it!= listaNombres.end(); ++it) {
			m_pListacomandos->Append(wxString::FromUTF8((*it).c_str()));
		}
		Layout();
	}

	~DialogoComandosEnEjecucion()
	{
	}
};

void VentanaPrincipal::OnWindowClose(wxCloseEvent& event) {
#if defined (_WINDOWS)
	wxWindowDisabler* dis = new wxWindowDisabler();
#endif
	SuperFreeze();

	GNC::CommandController::ListaNombreComandos comandos =  GNC::CommandController::Instance()->GetComandosActivos();
	if(comandos.size()>0 && event.CanVeto()) {
		DialogoComandosEnEjecucion dlg(this,comandos);
#if defined (_WINDOWS)
		delete dis;
		dis = NULL;
#endif
		this->SuperThaw();
		int answer = dlg.ShowModal();
#if defined (_WINDOWS)
		dis = new wxWindowDisabler();
#endif
		this->SuperFreeze();
		if(answer != wxID_OK) {
			event.Veto(true);
#if defined (_WINDOWS)
			delete dis;
#endif
			this->SuperThaw();
			return;
		}
	}

	GNC::GCS::ICommandController::Instance()->AbortarComandosDeOwner(this);

	if(CerrarTodosLosTabsYPanelGrids()) {
		//cerramos todos las ventanas flotantes que podamos haber abierto...
		int numeroVistas = GNC::GCS::ControladorVistas::Instance()->GetVistas().size();
		while (numeroVistas == (int)GNC::GCS::ControladorVistas::Instance()->GetVistas().size() && GNC::GCS::ControladorVistas::Instance()->GetVistas().size()>0) {
			wxWindow* ventana = GNC::GCS::ControladorVistas::Instance()->GetVistas().front()->GetWindow();
			GNC::GUI::DetachedDialog* pDialogo = dynamic_cast<GNC::GUI::DetachedDialog*> (ventana->GetParent());
			if(pDialogo != NULL) {
				if(!pDialogo->Close() && event.CanVeto()){
					event.Veto(true);
					break;
				}
			}
			numeroVistas --;
		}
		if(GNC::GCS::ControladorVistas::Instance()->GetVistas().size() != 0 && event.CanVeto()) {
			event.Veto(true);
		} else {
			this->Destroy();
		}
	} else {
		//si quedan tabs o panelgrids
		if (event.CanVeto()) {
			event.Veto(true);
		}
	}
	SuperThaw();
	if (!event.GetVeto()) {
		GNC::Entorno::Instance()->SetVentanaPrincipal(NULL);
		GNC::Entorno::Instance()->SetVentanaRaiz(NULL);
	}
#if defined(_WINDOWS)
	delete dis;
#endif
}

#ifdef __WXMAC__
void VentanaPrincipal::MacOpenFile(const wxString &fileName) {
	try{
		std::list<std::string> ruta;
		ruta.push_back(std::string(fileName.ToUTF8()));
		//m_pHistorial->CargarDICOM(ruta,true);
	}
	catch(GIL::DICOM::I2DException& i){
		LOG_ERROR("Principal", "Error al cargar fichero: " << i.str());
	}
}
#endif


/* Evento de configuracion */
void VentanaPrincipal::OnConfiguracion(wxCommandEvent& /*event*/){

	GNC::GUI::DialogoConfiguracion config(this);
	config.ShowModal();
}

/*Evento de ventana de control de envio HL7*/
void VentanaPrincipal::OnMensajesHL7(wxCommandEvent& /*event*/)
{
	GNC::GUI::VentanaControlHL7* ventana = GNC::GUI::VentanaControlHL7::Instance();
	if(!ventana->IsActive()) {
		ventana->RequestUserAttention();
	}
}

/*Evento de ventana de control de registros (Logs)*/
void VentanaPrincipal::OnControlLogs(wxCommandEvent& /*event*/)
{
	GNC::GUI::VentanaControlLogs* ventana = GNC::GUI::VentanaControlLogs::Instance();
	if(!ventana->IsActive()) {
		ventana->RequestUserAttention();
	}
}

/* Evento de aplicacion de herramienta*/
void VentanaPrincipal::OnAplicarHerramienta(wxCommandEvent& /*event*/){
	/*
	GNC::GCS::IControladorHerramientas* cH = GNC::Entorno::Instance()->GetControladorHerramientas();
	GNC::GCS::ITool* herr = NULL;
	try {
		// Subscribimos la vista al contrato de herr
		herr= cH->ObtenerHerramienta(event.GetId());
		if(herr!=NULL){
			herr->SolicitarActivacion(GNC::GCS::TriggerButton().EnableLeft());
		}
	}
	catch (GNC::GCS::ControladorHerramientasException& ex) {
		std::cerr << "Error al obtener la herramienta puntero: No se pudo subscribir la herramienta: " << ex.GetCause() << std::endl;
	}
	event.Skip(false);
	*/
}

void VentanaPrincipal::OnUpdateUIHerramienta(wxUpdateUIEvent& /*event*/)
{
	/*
	GNC::GCS::IControladorHerramientas* cH = GNC::Entorno::Instance()->GetControladorHerramientas();
	GNC::GCS::ITool* herr = NULL;
	try {
		// Subscribimos la vista al contrato de herr
		herr = cH->ObtenerHerramienta(event.GetId());
		if(herr!=NULL){
			event.Enable(herr->Habilitada());
			event.Show(herr->Habilitada());
		}
		else{
			event.Enable(false);
			event.Show(false);
		}
	}
	catch (GNC::GCS::ControladorHerramientasException& ex) {
		std::cerr << "Error al obtener la herramienta puntero: No se pudo subscribir la herramienta: " << ex.str() << std::endl;
	}
	event.Skip(true);
	*/
}

/* Eventos de teclado */
void VentanaPrincipal::OnKeyDown(wxKeyEvent& event)
{
	GNC::GCS::IVista* pVista = GNC::GCS::ControladorVistas::Instance()->GetVistaActiva();
	if(pVista != NULL)
	{
		wxKeyEvent evt = event;
		wxWindow* win = pVista->GetWindow();
		if (win) {
			win->GetEventHandler()->AddPendingEvent(evt);
		}
		event.Skip(false);
	}
	else {
		event.Skip(true);
	}
}

void VentanaPrincipal::OnMenuCerrarTabClick(wxCommandEvent& )
{
	GNC::GCS::IVista* pVista = GNC::GCS::ControladorVistas::Instance()->GetVistaActiva();
	if(pVista != NULL)
	{
		wxWindow* pVentana = pVista->GetWindow();
		if(pVentana != NULL) {
			if(m_pNoteBook->GetPageIndex(pVentana) >= 0 && m_pNoteBook->GetSelection()!=-1)
			{
				wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BUTTON, m_pNoteBook->GetSelection());
				evt.SetInt(wxAUI_BUTTON_CLOSE);
				evt.SetId(5380+500);
				evt.SetEventObject(((wxGinkgoAuiNoteBook*)m_pNoteBook)->GetActiveTabCtrl());
				m_pNoteBook->GetEventHandler()->ProcessEvent(evt);
			}
		}
	}
}

void VentanaPrincipal::OnMenuCerrarTodoExceptoActiva(wxCommandEvent& )
{
	/*NO ESTA CORRECTO
	 unsigned int pestanias = m_pNoteBook->GetPageCount();
	 //la segunda condicion es para que si das a cancelar pues pare
	 unsigned int tabActual = pestanias - 1;
	 while(m_pNoteBook->GetPageCount() > 1 && pestanias == m_pNoteBook->GetPageCount()){
	 if(m_pNoteBook->GetPage(tabActual) == m_pNoteBook->GetActiveTabCtrl()){

	 } else {
	 wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BUTTON, m_pNoteBook->GetPage(tabActual)->GetId());
	 evt.SetInt(wxAUI_BUTTON_CLOSE);
	 evt.SetEventObject(m_pNoteBook->GetPage(tabActual));
	 m_pNoteBook->GetEventHandler()->ProcessEvent(evt);
	 pestanias --;
	 }
	 }	*/

}

void VentanaPrincipal::OnMenuCerrarTodosTabsClick(wxCommandEvent& )
{
	CerrarTodosLosTabsYPanelGrids();
}

//devuelve bool si ha cerrado todos
bool VentanaPrincipal::CerrarTodosLosTabsYPanelGrids() {
	bool todosCerrados = false;
	unsigned int pestanias = m_pNoteBook->GetPageCount();
	//la segunda condicion es para que si das a cancelar pues pare	
	while(m_pNoteBook->GetPageCount() > 0 && pestanias == m_pNoteBook->GetPageCount()){
		wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BUTTON, m_pNoteBook->GetSelection());
		evt.SetInt(wxAUI_BUTTON_CLOSE);
		evt.SetId(5380+500);
		evt.SetEventObject(((wxGinkgoAuiNoteBook*)m_pNoteBook)->GetActiveTabCtrl());
		m_pNoteBook->GetEventHandler()->ProcessEvent(evt);
		pestanias --;
	}

	if(m_pNoteBook->GetPageCount() == 0) {
		//se cierran los paneles tabs...
		todosCerrados = true;
	}

	// Se han eliminado todas las paginas
	//se muestra el mosaico
	ShowStartUp(todosCerrados);

	return todosCerrados;
}

bool VentanaPrincipal::ClosesAllUndocked()
{
	std::list<GNC::GUI::DetachedDialog*> listaDesencajados;
	GNC::GCS::ControladorVistas::TipoListaVistas viewsList = GNC::GCS::ControladorVistas::Instance()->GetVistas();
	for (GNC::GCS::ControladorVistas::TipoListaVistas::iterator it = viewsList.begin(); it != viewsList.end(); ++it) {
		GNC::GUI::DetachedDialog* pPanel = dynamic_cast<GNC::GUI::DetachedDialog*> ((*it)->GetWindow()->GetParent());
		if(pPanel!=NULL) {
			listaDesencajados.push_back(pPanel);
		}
	}

	for (std::list<GNC::GUI::DetachedDialog*>::iterator it = listaDesencajados.begin(); it != listaDesencajados.end(); ++it)
	{
		if (!(*it)->Close())
		{
			return false;
		}
	}
	return true;
}

void VentanaPrincipal::OnMenuPrimerPlanoPestaniaActiva(wxCommandEvent& )
{
	if(m_pNoteBook->GetPageCount() > 0) {
		PasarAPrimerPlano(m_pNoteBook->GetPage(m_pNoteBook->GetSelection()));
	}
}

void VentanaPrincipal::OnMenuCerrarTabUpdateUI(wxUpdateUIEvent& event)
{
	event.Enable(m_pNoteBook->GetPageCount() > 0);
}

void VentanaPrincipal::OnMenuCerrarTodosTabsUpdateUI(wxUpdateUIEvent& event)
{
	event.Enable(m_pNoteBook->GetPageCount() > 0);
}


/*Eventos de impresion*/


void VentanaPrincipal::OnPantallaCompleta(wxCommandEvent& )
{
	this->ShowFullScreen(!this->IsFullScreen(),wxFULLSCREEN_NOBORDER|wxFULLSCREEN_NOCAPTION);
}

wxWindow* VentanaPrincipal::GetRootWindow() {
	return m_pPanelCentral;
}

void VentanaPrincipal::EncajarDesencajar(GNC::GCS::IVista* pVista) {
	if(pVista != NULL)
	{
		wxWindow* pVentana = pVista->GetWindow();
		if(pVentana != NULL) {
			GNC::GCS::ControladorVistas::Instance()->SolicitarActivarVista(NULL);
			//la buscamos en el notebook
			if(m_pNoteBook->GetPageIndex(pVentana) >= 0){
				GNC::GUI::DetachedDialog* pDlg;
				{
#if defined (_WINDOWS)
					wxWindowDisabler dis;
#endif

					SuperFreeze();
					wxString caption = m_pNoteBook->GetPageText(m_pNoteBook->GetPageIndex(pVentana));
					wxString titulo = m_pNoteBook->GetPageText(m_pNoteBook->GetPageIndex(pVentana));

					m_pNoteBook->RemovePage(m_pNoteBook->GetPageIndex(pVentana));

					if (m_pNoteBook->GetPageCount() == 0) {
						ShowStartUp(true);
					}

					pDlg = new GNC::GUI::DetachedDialog(this, this);
					pDlg->AddPanel(pVentana, titulo);

					m_mgr.Update();
					pDlg->Show(true);
					SuperThaw();

				}
			} else { //esta desencajada => la encajamos
				GNC::GUI::DetachedDialog* pDlg = dynamic_cast<GNC::GUI::DetachedDialog*>(pVentana->GetParent());
				if(pDlg != NULL)
				{
#if defined (_WINDOWS)
					wxWindowDisabler dis;
#endif
					pDlg->Show(false);
					pDlg->GetSizer()->Detach(pVentana);
					SuperFreeze();
					//encajar!
					//encajar en el notebook
					ShowStartUp(false);
					pVentana->Reparent(m_pNoteBook);
					m_pNoteBook->AddPage(pVentana, pDlg->GetTitulo());
					pDlg->Destroy();
					m_pNoteBook->Refresh();
					m_mgr.GetPane(m_pNoteBook).Show(true);
					m_mgr.Update();
					SuperThaw();
				}
			}
			GNC::GCS::ControladorVistas::Instance()->SolicitarActivarVista(pVista);
		}
	}
}


void VentanaPrincipal::ShowStartUp(bool show) 
{
	bool update = false;
	if (StartupView == NULL) {
		return;
	}
	if (StartupView->GetWindow()->IsShown() != show) {
		m_mgr.GetPane(StartupView->GetWindow()).Show(show);
		update = true;
	}
	if (show) {
		GNC::GCS::ControladorVistas::Instance()->SolicitarActivarVista(StartupView);
	}
	if (m_pNoteBook->IsShown() == show) {
		m_mgr.GetPane(m_pNoteBook).Show(!show);
		update = true;
		m_pNoteBook->Refresh(false);
	}
	if (update) {
		m_mgr.Update();
	}
}

void VentanaPrincipal::OnMosaicoRestaurar(wxCommandEvent& )
{
	wxWindowDisabler disabler;
	SuperFreeze();
	ReplegarPestanias();
	m_mgr.Update();
	SuperThaw();
}

void VentanaPrincipal::ReplegarPestanias()
{
	//remember the tab now selected
	int nowSelected = m_pNoteBook->GetSelection();
	//select first tab as destination
	m_pNoteBook->SetSelection(0);
	//iterate all other tabs
	for( unsigned int idx = 0; idx < m_pNoteBook->GetPageCount(); idx++){
		//get win reference
		wxWindow* win = m_pNoteBook->GetPage(idx);
		//get tab title
		wxString title = m_pNoteBook->GetPageText(idx);
		//remove from notebook
		m_pNoteBook->RemovePage(idx);
		//re-add in the same position so it will tab
		m_pNoteBook->InsertPage(idx, win, title);
	}
	//restore orignial selected tab
	m_pNoteBook->SetSelection(nowSelected);
}

/* Añade una nueva pestaña al panel de estudios */
void VentanaPrincipal::InsertarVentana(wxWindow* pVentana)
{
	wxWindowDisabler disabler;
	SuperFreeze();
	ShowStartUp(false);

	m_pNoteBook->AddPage(pVentana, wxEmptyString, false);
	m_mgr.Update();
	SuperThaw();
}

/* Elimina un tab del panel de estudios */
void VentanaPrincipal::EliminarVentana(wxWindow *WXUNUSED(pVentana))
{
	std::cerr << "Error: Metodo no implementado VentanaPrincipal::EstudioRemoveTab(Tab* t)" << std::endl;
	throw new std::exception();
}

void VentanaPrincipal::OnInicioAplicacion(wxCommandEvent &) 
{	
	//parameters parsing...
	bool aceptedLicense = false;
	bool versionParameter = false;
	wxString pathOfFile = wxEmptyString;

	int argc = GNC::Entorno::Instance()->GetApp()->argc;
	wxChar** argv = GNC::Entorno::Instance()->GetApp()->argv;
	if (argc > 1) {
		for (int i = 1; i < argc; i++) {
			wxString param(argv[i]);

			if ( param.CmpNoCase(wxT("-version")) == 0 || param.CmpNoCase(wxT("--version")) == 0 || param.CmpNoCase(wxT("-v")) == 0 ) {
				versionParameter = true;
				continue;
			}
			if (param.CmpNoCase(wxT("--accepted")) == 0 || param.CmpNoCase(wxT("-a")) == 0) {
				aceptedLicense = true;
				continue;
			}
			#if defined(GINKGO_PRO)
			else if (param.CmpNoCase(wxT("-unittest")) == 0) {
				LOG_INFO("INITWX", "Running unitttests");
				if (argc <= i+1) {
					LOG_ERROR("INITWX", "no tests cases found for -unittest option. Usage -unittest <path_test_cases>");
				} else {
					i++;
					std::string pathTests = wxString(argv[i]).ToUTF8();
					TestLauncher* test = new TestLauncher(pathTests);
					test->Create();
					test->Run();
				}
				continue;
			}
			#endif
			if (!param.StartsWith("--")) {
				pathOfFile = param;
			}
		}
	}

	if (versionParameter) {
		std::cout << GNC::GCS::IEntorno::Instance()->GetGinkgoVersionString() << std::endl;
		GNC::GCS::IEntorno::FreeInstance();
		Close();
	}

	if (!aceptedLicense) 
	{
		//aceptacion de la licencia
		bool aceptada;
		GNC::GCS::ConfigurationController::Instance()->readBoolGeneral("/GinkgoCore/Licencia", "Aceptada", aceptada, false);
		if(!aceptada) {
			GNC::GUI::AcceptLicenseDialog dlgLicencia(NULL);
			wxYield();
			int answer = dlgLicencia.ShowModal();
			if(answer == wxID_OK) {
				GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/Licencia", "Aceptada", true);
				GNC::GCS::ConfigurationController::Instance()->Flush();
			} else {
				GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/Licencia", "Aceptada", false);
				GNC::GCS::ConfigurationController::Instance()->Flush();
				Close();
				return;
			}
		}
	}

	//si viene un parametro y es un directorio... abrir directamente
	if(!pathOfFile.empty())
	{
		Login();
		EjecutarParametro(pathOfFile, true);
	} else {
		Login();
	}
}

void VentanaPrincipal::EjecutarParametro(const wxString cadena, bool doLogin)
{
	std::string xmlString("");
	std::string errorMsg;

	bool error = false;
	
	if (cadena.Lower().StartsWith(wxT("ginkgocadx://"))) {
		try {
			xmlString = GIL::IntegrationController::Instance()->ParsesModelFromURL(cadena);
		} catch (GIL::IntegrationException& ex) {
			LOG_ERROR("GIL/WF", "Semantic/Sintactic error parsing integration url: " << ex.GetFullCause());
			wxMessageBox(_("Semantic/Sintactic error parsing integration URL (see log for details):\n") + wxString::FromUTF8(ex.GetCause().c_str()),_("Error"),wxICON_INFORMATION);
		}
	}
	else {

		wxFileName path = cadena;
		
	
		if (path.IsOk()) {
			LOG_DEBUG("Core", "Interpretando " << std::string(TOPATH(path.GetFullPath())).c_str());
			if (path.IsRelative()) {
				LOG_DEBUG("Core", "Convirtiendo ruta en absoluta");
				#if defined(__WXGTK__)
			
				char* invocation_dir = getenv("INVOCATION_DIR");					
				if (invocation_dir != NULL) {
					wxString invocationDir = wxString::FromUTF8(invocation_dir);
					if (!invocationDir.IsEmpty()) {
						path = invocationDir + wxFileName::GetPathSeparator() + path.GetFullPath();
					}
				}
				#else
				path.MakeAbsolute();
				#endif
				LOG_DEBUG("Core", "Ruta absoluta: " << std::string(TOPATH(path.GetFullPath())).c_str());
			}

			if(wxDirExists(path.GetFullPath()) && path.IsDirReadable()){
				if(doLogin) {
					Login();
				}
				std::list<std::string> rutas;
				rutas.push_back(std::string(TOPATH( path.GetFullPath() )));
				GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(rutas,true,GNC::GCS::IHistoryController::TAA_LINK);
				GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
				GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Included in the history ..."), pCmd, NULL);
				return ;
			} else if(path.FileExists() && path.IsFileReadable()){

				//estamos ante un fichero
				//se mira a ver si a partir del byte 128 pone DICM
				wxFile fichero(path.GetFullPath(), wxFile::read);

				bool esGzip = false;
				bool esDicom = false;
				if(fichero.IsOpened()){
					char buffer[128];
					if(fichero.Read(buffer,128) == 128){
						esGzip = ((unsigned char)buffer[0]==0x1f) && ((unsigned char)buffer[1]==0x8b);
						char numeroMagico[4];
						if(fichero.Read(numeroMagico,4) == 4){
							std::string str(numeroMagico,4);
							if(str == "DICM") {
								esDicom = true;
							}
						}
					}
				}
				if(esDicom) {
					if (doLogin) {
						Login();
					}
					//si no es un xml pues sera un dicom...
					std::list<std::string> rutas;
					rutas.push_back(std::string(TOPATH(path.GetFullPath()) ));
					GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(rutas,true,GNC::GCS::IHistoryController::TAA_LINK);
					GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
					GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Included in the history ..."),pCmd, NULL);			
				} else {				
					//se lee el modelo
					bool loaded = false;
					wxXmlDocument documento;
					if (esGzip) {
						wxFileInputStream* fileIn = new wxFileInputStream(path.GetFullPath());
						wxZlibInputStream zInput(fileIn);
						loaded = documento.Load(zInput);
					} else {
						loaded = documento.Load(path.GetFullPath());
					}
					if(loaded) {
						wxStringOutputStream out;
						documento.Save(out);
						xmlString=out.GetString().ToUTF8();
					} else {
						//dcmurl this is a file with a relative path...
						LOG_DEBUG("GIL/WF", "Try reading dcmurl file...");
						wxTextFile dcmURLFile;
						if (!dcmURLFile.Open(path.GetFullPath(), wxMBConvUTF8())) {
							for (wxString line = dcmURLFile.GetFirstLine(); !dcmURLFile.Eof(); line = dcmURLFile.GetNextLine()) {
								wxFileName fileName(line);
								if (fileName.IsRelative()) {
									fileName.MakeAbsolute(path.GetPath());
								}
								if (wxFileExists(fileName.GetFullPath())) {
									EjecutarParametro(fileName.GetFullPath());
								} else {
									LOG_ERROR("GIL/WF", "File doesn't exists");
									break;
								}
							}
						}
						LOG_ERROR("GIL/WF", "Error al interpretar el fichero de entrada: XML mal formado");
						wxMessageBox(_("Malformed XML"),_("Error"),wxICON_INFORMATION);
						if (doLogin) {
							Login();
						}
					}
				}
			}
			else {
				error = true;
			}
		}
		else {
			error = true;
		}
	}
	if (error) {
		LOG_ERROR("GIL/WF", "Error al interpretar los parámetros de la línea de comandos. (" << cadena.ToUTF8() << ") : " << errorMsg );
			wxMessageBox(
				_("Error in interpreting the parameters of the command line. (")
				+ cadena
				+ wxT(") : ")
				+ wxString::FromUTF8(errorMsg.c_str())
				,_("Error"), wxICON_INFORMATION);
			if (doLogin) {
				Login();
			}
			return;
	}


	if(xmlString != "") {
		try {
			GIL::IntegrationModelList listaModelos;	
			GIL::IntegrationController::Instance()->ParsearModeloIntegracion(listaModelos, xmlString, cadena);
			GIL::IntegrationController::Instance()->Process(listaModelos);
		}
		catch (GIL::IntegrationException& ex) {
			LOG_ERROR("GIL/WF", "Semantic/Sintactic error parsing integration XML: " << ex.GetFullCause());
			wxMessageBox(_("Semantic/Sintactic error parsing integration XML (see log for details):\n") + wxString::FromUTF8(ex.GetCause().c_str()),_("Error"),wxICON_INFORMATION);
		}
		catch (GIL::HL7::HL7XMLException& ex) {
			LOG_ERROR("GIL/WF", "Semantic/Sintactic error parsing integration XML: " << ex.GetFullCause());
			wxMessageBox(_("Semantic/Sintactic error parsing integration XML (see log for details):\n") + wxString::FromUTF8(ex.GetCause().c_str()),_("Error"),wxICON_INFORMATION);
		}
		catch (GIL::HL7::HL7Exception& ex1) {
			LOG_ERROR("GIL/WF", "Error parsing integration XML: " << ex1.GetCause());
			wxMessageBox(_("Error parsing integration XML:\n") + wxString::FromUTF8(ex1.GetCause().c_str()),_("Error"),wxICON_INFORMATION);
		}
		catch (GIL::DICOM::PACSException& ex3) {
			LOG_ERROR("GIL/WF", "PACS error: " << ex3.GetCause());
			wxMessageBox(_("PACS error:\n") + wxString::FromUTF8(ex3.GetCause().c_str()),_("Error"),wxICON_INFORMATION);
		}
		catch (std::exception& ex4) {
			LOG_ERROR("GIL/WF", "Error executing WorkFlow: " << ex4.what());
			wxMessageBox(_("Error executing WorkFlow\n") + wxString::FromUTF8(ex4.what()),_("Error"),wxICON_INFORMATION);
		}
		catch (...) {
			LOG_ERROR("GIL/WF", "Error executing WorkFlow: Internal error.");
			wxMessageBox(_("Error executing WorkFlow\nInternal Error"), _("Error"),wxICON_INFORMATION);
		}

		if (!m_Iniciada) {
			if (doLogin) {
				Login();
			}
		}
	}
	else {
		if (doLogin) {
			Login();
		}
	}
}

void VentanaPrincipal::OnSize(wxSizeEvent& WXUNUSED(event))
{
	Layout();
}


void VentanaPrincipal::RefrescarMenus()
{

	//se vacía la barra y se borran los menus que se crearon en la anterior pasada
    wxWindowDisabler dis;
    SuperFreeze();

	wxMenu* pMenuArchivo = new wxMenu();
	wxMenu* pMenuEdicion = new wxMenu();
	wxMenu* pMenuVer = new wxMenu();
	wxMenu* pMenuSeries = new wxMenu();
	wxMenu* pMenuHerramientas = new wxMenu();
	wxMenu* pMenuAyuda = new wxMenu();

	{
		//menu archivo
		wxMenuItem* m_pCerrar = new wxMenuItem( pMenuArchivo, ID_CERRAR, wxString( _("Close") ) + wxT("\tAlt+w"), _("Close current view"), wxITEM_NORMAL );
		wxMenuItem* m_pCerrarTodas = new wxMenuItem( pMenuArchivo, ID_CERRAR_TODAS, wxString( _("Close all") ) , _("Close all views"), wxITEM_NORMAL );
		wxMenuItem* m_pMenuSalir = new wxMenuItem( pMenuArchivo, ID_SALIR, wxString( _("&Exit") ) + wxT("\tAlt+s"), _("Exit application"), wxITEM_NORMAL );
#ifdef __WXMSW__
		m_pCerrar->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoCerrarTab());
		m_pMenuSalir->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoSalir());
	#else
		m_pCerrar->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoCerrarTab());
		m_pMenuSalir->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoSalir());
	#endif
		InsertToolsFromFamily(pMenuArchivo, GNC::GCS::ITool::TMenuArchivo);
		if (pMenuArchivo->GetMenuItemCount() > 0) 
		{
			pMenuArchivo->AppendSeparator();
		}
		pMenuArchivo->Append( m_pCerrar );
		pMenuArchivo->Append( m_pCerrarTodas );
		pMenuArchivo->AppendSeparator();		
		pMenuArchivo->Append( m_pMenuSalir );
		this->Connect( m_pCerrar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMenuCerrarTabClick ) );
		this->Connect( m_pCerrar->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( VentanaPrincipal::OnMenuCerrarTabUpdateUI ) );
		this->Connect( m_pCerrarTodas->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMenuCerrarTodosTabsClick ) );
		this->Connect( m_pCerrarTodas->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler( VentanaPrincipal::OnMenuCerrarTodosTabsUpdateUI ) );
		
		this->Connect( m_pMenuSalir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnSalir ) );

		//edit
		InsertToolsFromFamily(pMenuEdicion, GNC::GCS::ITool::TMenuEdicion);

		//view
		{
			wxMenuItem*  pPantallaCompleta = new wxMenuItem( pMenuVer, ID_PANTALLA_COMPLETA, wxString(_("Full screen"))+ wxT("\tF11"), _("Show full screen"), wxITEM_NORMAL );
			wxMenuItem* pMosaicoRestaurar = new wxMenuItem(pMenuVer, ID_MOSAICO_RESTAURAR, wxString( _("Tab grouping") ), _("Tab grouping"), wxITEM_NORMAL );

			#ifdef __WXMSW__
			pMosaicoRestaurar->SetBitmaps(GinkgoResourcesManager::MenuTabs::GetIcoReagrupar());
			pPantallaCompleta->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoPantallaCompleta());
			#else
			pMosaicoRestaurar->SetBitmap(GinkgoResourcesManager::MenuTabs::GetIcoReagrupar());
			pPantallaCompleta->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoPantallaCompleta());
			#endif
			pMenuVer->Append( pPantallaCompleta );
			pMenuVer->AppendSeparator();
			pMenuVer->Append(pMosaicoRestaurar);

			//image
			InsertToolsFromFamily(pMenuVer, GNC::GCS::ITool::TMenuVer);

			this->Connect( pPantallaCompleta->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnPantallaCompleta ) );
			this->Connect( pMosaicoRestaurar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMosaicoRestaurar ) );
		}

		//image
		InsertToolsFromFamily(pMenuSeries, GNC::GCS::ITool::TSeriesMenu);

		//tools
		InsertToolsFromFamily(pMenuHerramientas, GNC::GCS::ITool::TMenuHerramientas);

		//ayuda
		wxMenuItem*  pMenuGNKHelp = new wxMenuItem( pMenuAyuda, ID_ONLINE_MANUAL, wxString( _("Ginkgo CADx manual ...") ), _("Ginkgo CADx manual ..."), wxITEM_NORMAL );
		wxMenuItem*  m_pMenuOpenTestCard = new wxMenuItem( pMenuAyuda, ID_OPEN_TEST_CARD, wxString( _("Open test card") ), _("Open test card"), wxITEM_NORMAL );
		wxMenuItem*  m_pMenuCheckUpdates = new wxMenuItem( pMenuAyuda, ID_CHECK_UPDATES, wxString( _("Check for updates") ), _("Check for newest versions"), wxITEM_NORMAL );
		wxMenuItem*  pMenuOnlineSupport = new wxMenuItem( pMenuAyuda, ID_ONLINE_SUPPORT, wxString( _("Need support ...") ), _("Need support ..."), wxITEM_NORMAL );
		wxMenuItem*  m_pMenuAcercaDe = new wxMenuItem( pMenuAyuda, ID_ACERCA_DE,  _("About") + wxString(wxT(" ")) + wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()) + wxT(" ...") ,  _("About ") + wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()) + wxT(" ...") , wxITEM_NORMAL );

		#ifdef __WXMSW__
		pMenuGNKHelp->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoHelp());
		pMenuOnlineSupport->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoSupport());
		#else
		pMenuGNKHelp->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoHelp());
		pMenuOnlineSupport->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoSupport());
		#endif

		pMenuAyuda->Append( pMenuGNKHelp );
		pMenuAyuda->AppendSeparator();
		pMenuAyuda->Append( pMenuOnlineSupport );
		
		pMenuAyuda->AppendSeparator();		
		pMenuAyuda->Append( m_pMenuCheckUpdates );
		InsertToolsFromFamily(pMenuAyuda, GNC::GCS::ITool::TMenuHelp);
		pMenuAyuda->AppendSeparator();		
		pMenuAyuda->Append( m_pMenuOpenTestCard );
		pMenuAyuda->AppendSeparator();
		pMenuAyuda->Append( m_pMenuAcercaDe );

		this->Connect( pMenuGNKHelp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnOnlineHelp ) );
		this->Connect( m_pMenuOpenTestCard->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnOpenTestCard ) );
		this->Connect( m_pMenuCheckUpdates->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnCheckForUpdates ) );
		this->Connect( m_pMenuAcercaDe->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnAcercaDe ) );
		this->Connect( pMenuOnlineSupport->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnOnlineSupport ) );
	}

	//herramientas -> configuracion, envio hl7 y logs
	if(GNC::GCS::IControladorPermisos::Instance()->Get("core.menu","configuracion")) {
		if(pMenuHerramientas->GetMenuItemCount() >0)
			pMenuHerramientas->AppendSeparator();
		wxMenuItem*  pMenuConfiguracion = NULL;
		wxMenuItem*  pMenuHL7 = NULL;
		wxMenuItem*  pMenuLogs = NULL;
		//herramientas
		pMenuConfiguracion = new wxMenuItem( pMenuHerramientas, ID_CONFIGURACION, wxString(_("Settings")) + wxT("\tAlt+c"), _("Settings"), wxITEM_NORMAL );
		pMenuHL7 = new wxMenuItem( pMenuHerramientas, ID_CONTROLHL7, wxString(_("HL7 monitor")) + wxT(""), _("HL7 message control"), wxITEM_NORMAL );
		pMenuLogs = new wxMenuItem( pMenuHerramientas, ID_CONTROLLOGS, wxString(_("Log monitor")) + wxT("\tAlt+r"), _("Log monitored"), wxITEM_NORMAL );

#ifdef __WXMSW__
		pMenuConfiguracion->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoConfiguracion());
#else
		pMenuConfiguracion->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoConfiguracion());
#endif
		pMenuHerramientas->Append( pMenuLogs );
		pMenuHerramientas->Append( pMenuHL7 );
		pMenuHerramientas->AppendSeparator();
		pMenuHerramientas->Append( pMenuConfiguracion );
		
		this->Connect( pMenuConfiguracion->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnConfiguracion ) );
		this->Connect( pMenuHL7->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnMensajesHL7 ) );
		this->Connect( pMenuLogs->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaPrincipal::OnControlLogs ) );
	}
	//avoid hide bar => flickering!!
	m_pMenuBar->Freeze();
	m_pMenuBar->Append( pMenuArchivo, _("&File") );
	while (m_pMenuBar->GetMenuCount() > 1)
		delete m_pMenuBar->Remove(0);	
	m_pMenuBar->Append( pMenuEdicion, _("&Edit") );
	m_pMenuBar->Append( pMenuVer, _("View") );
	m_pMenuBar->Append( pMenuSeries, _("Series") );
	m_pMenuBar->Append( pMenuHerramientas, _("Tools") );
	m_pMenuBar->Append( pMenuAyuda, _("&Help") );
	
	m_pMenuBar->Thaw();
	this->SetMenuBar(m_pMenuBar);

#ifdef __WXMAC__
	wxMenuBar::MacSetCommonMenuBar(m_pMenuBar);
#endif
	SuperThaw();
}

void VentanaPrincipal::InsertToolsFromFamily(wxMenu* pParent, GNC::GCS::ITool::TToolFamily family)
{
	//general tools
	GNC::GCS::IToolsRegistry::Instance()->AppendsToolInMenu(this, pParent, family);

	GNC::GCS::IVista* pVistaActiva = GNC::GCS::ControladorVistas::Instance()->GetVistaActiva();
	if (pVistaActiva == NULL) {
		return;
	}

	GNC::GCS::IToolController* pToolController = pVistaActiva->GetToolController();
	pToolController->AppendsToolInMenu(this, pParent, family);
}

void VentanaPrincipal::CargarHistorial()
{
	GNC::GUI::HistoryPanel3::Instance();
	//if unittest... delete current history
	#if defined(GINKGO_PRO)
	int argc = GNC::Entorno::Instance()->GetApp()->argc;
	wxChar** argv = GNC::Entorno::Instance()->GetApp()->argv;
	for (int i = 1; i < argc; i++) {
		wxString param(argv[i]);
		if (param.CmpNoCase(wxT("-unittest")) == 0) {
			//drop file
			std::string dirPath = GNC::GCS::HistoryController::GetDefaultDicomDirPath();
			LOG_INFO("INITWX", "Removing default dicomdir:" << dirPath);
			GNC::GCS::IEntorno::Instance()->RemoveDirRecursive(dirPath);
			LOG_INFO("INITWX", "Default dicomdir removed successfully");
			break;
		}
	}
	#endif
	//

	if (!GNC::GCS::HistoryController::Instance()->IsOk()) {
		wxMessageBox(_("Error loading history, Ginkgo CADx will be closed"), _("Error"), wxICON_ERROR|wxOK, this);
		Close(true);
		return;
	}

	SuperFreeze();
	
	GNC::GUI::HistoryPanel3::Instance()->ReloadCombos(true);
	GNC::GUI::HistoryPanel3::Instance()->RefreshSearch(true);

	GNC::GCS::Ptr<GNC::GUI::StartUpStudy> study(new GNC::GUI::StartUpStudy());
	study->Module = NULL;
	study->ParentWindow = m_pPanelCentral;
	StartupView = new GNC::GUI::StartUpView(study);
	m_mgr.AddPane(StartupView->GetWindow(), wxAuiPaneInfo().Name(_("Mosaic")).CenterPane().Layer(0).PaneBorder(true).Movable(false).Floatable(false).Dockable(false).LeftDockable(false).RightDockable(false).TopDockable(false).BottomDockable(false).CloseButton(false));
	
	GNC::GCS::Ptr<GIL::IModeloIntegracion> ptr;
	GNC::Entorno::Instance()->GetControladorCarga()->CargaAsincrona(StartupView, false);
	m_mgr.Update();
	
	SuperThaw();
}

void VentanaPrincipal::OnComando(EventoProgreso& evento)
{
	switch (evento.GetTipo()) {
		case EventoProgreso::ComandoIniciado:
			GNC::CommandController::Instance()->OnComandoLanzado(evento.GetThreadId());
			break;
		case EventoProgreso::ComandoEjecutando:
			GNC::CommandController::Instance()->OnComandoProgreso(evento.GetThreadId());
			break;
		case EventoProgreso::ComandoFinalizado:
			GNC::CommandController::Instance()->OnComandoFinalizado(evento.GetThreadId());
			break;
		default:
			std::cerr << "Error, Tipo de notificacion de comando desconocida." << std::endl;
			break;
	}

}

void VentanaPrincipal::OnNotebookPageChanging(wxAuiNotebookEvent& /*event*/)
{
	/*
	int old_idx = event.GetOldSelection();
	int new_idx = event.GetSelection();

	if (old_idx != new_idx && old_idx != -1 && new_idx != -1) {

		wxWindowDisabler dis;
		SuperFreeze();

		m_pNoteBook->SetSelection(new_idx);

		SuperThaw();
	}

	event.Skip(false);
	*/
}


void VentanaPrincipal::OnNotebookPageChanged(wxAuiNotebookEvent& event)
{
	SuperFreeze();
	if (m_pNoteBook->GetPageCount() == 0) { // Se han eliminado todas las paginas
		ShowStartUp(true);
	}
	else
	{ // Se ha cambiado de pagina
		ShowStartUp(false);
		GNC::GCS::IVista* v  = GNC::GCS::ControladorVistas::Instance()->ObtenerVistaRegistrada(m_pNoteBook->GetPage(event.GetSelection()));
		if (v != NULL) {
			GNC::GCS::ControladorVistas::Instance()->SolicitarActivarVista(v);
		}
	}
	SuperThaw();

}

void VentanaPrincipal::OnPanelClose(wxAuiManagerEvent& event)
{
	wxWindowDisabler disabler;
	SuperFreeze();
	if(CerrarVista(event.GetPane()->window)) {
		GNC::GCS::ControladorVistas::Instance()->Destruir(event.GetPane()->window);
	}
	SuperThaw();
}


void VentanaPrincipal::OnNotebookPageClose(wxAuiNotebookEvent& event)
{
	wxWindowDisabler disabler;
	SuperFreeze();
	if(CerrarVista(m_pNoteBook->GetPage(event.GetSelection()))) {
		GNC::GCS::ControladorVistas::Instance()->Destruir(m_pNoteBook->GetPage(event.GetSelection()));
		if (m_pNoteBook->GetPageCount() == 1) { // Se han eliminado todas las paginas
			//se muestra el mosaico
			ShowStartUp(true);
		}
	} else {
		event.Veto();
	}
	SuperThaw();
}

void VentanaPrincipal::PasarAPrimerPlano(wxWindow* window)
{
	wxWindowDisabler disabler;
	SuperFreeze();
	ReplegarPestanias();
	int index = m_pNoteBook->GetPageIndex(window);
	if(index>0) {
		m_pNoteBook->SetSelection(index);
	}
	//este update es necesario porque pasar a primer plano viene cuando estan en pestanias y cuando estan en grid
	m_mgr.Update();
	SuperThaw();
}

void VentanaPrincipal::DestruirPanelVista(wxWindow* panel)
{
	wxAuiNotebook* padreNotebook = dynamic_cast<wxAuiNotebook*> (panel->GetParent());
	if(padreNotebook != NULL)
	{
		CerrarNotebookPage(panel);
	} else {
		GNC::GUI::DetachedDialog* pDialogo = dynamic_cast<GNC::GUI::DetachedDialog*> (panel->GetParent());
		if(pDialogo != NULL)
		{
			pDialogo->Close();
		}
	}
}

void VentanaPrincipal::CerrarNotebookPage(wxWindow* panel)
{
	int index = m_pNoteBook->GetPageIndex(panel);
	if (index < 0 ) {
		return;
	}

	SuperFreeze();
	if(CerrarVista(panel)) {
		m_pNoteBook->DeletePage(index);
		if (m_pNoteBook->GetPageCount() == 0) { // Se han eliminado todas las paginas
			//se muestra el mosaico
			ShowStartUp(true);
		}
		m_mgr.Update();
	}
	SuperThaw();
}

bool VentanaPrincipal::CerrarVista(GNC::GCS::IVista* pVista)
{
	if (pVista == NULL)
		return true;

	//it's important to avoid focus errors
	if (this->IsIconized()) {
		this->Restore();
	}
	this->Raise();

	wxWindow* pWindow = pVista->GetWindow();
	//if it's a tab...
	int idx = m_pNoteBook->GetPageIndex(pWindow);
	if(idx >= 0){
		unsigned int tabCount = m_pNoteBook->GetPageCount();
		//selects the page
		if (idx >= 0) {
			if(idx != m_pNoteBook->GetSelection()){
				wxWindowDisabler dis;
				SuperFreeze();
				m_pNoteBook->SetSelection(idx);
				SuperThaw();
			}
		}
		wxAuiNotebookEvent evt(wxEVT_COMMAND_AUINOTEBOOK_BUTTON, m_pNoteBook->GetSelection());
		evt.SetInt(wxAUI_BUTTON_CLOSE);
		evt.SetId(5380+500);
		evt.SetEventObject(((wxGinkgoAuiNoteBook*)m_pNoteBook)->GetActiveTabCtrl());
		m_pNoteBook->GetEventHandler()->ProcessEvent(evt);
		
		return ( tabCount > m_pNoteBook->GetPageCount() );
	} else {
		GNC::GUI::DetachedDialog* pDlg = dynamic_cast<GNC::GUI::DetachedDialog*>(pWindow->GetParent());
		if(pDlg != NULL)
		{
			bool closed = pDlg->Close();
			return closed;;
		} else {
			return false;
		}
	}

}

bool VentanaPrincipal::CerrarVista(wxWindow* window) {
	bool permitido = true;
	SuperFreeze();

	GNC::GCS::ControladorVistas* pcv = GNC::GCS::ControladorVistas::Instance();
	if (pcv == NULL) {
		std::cerr << "Error: No se pudo obtener el controlador de vistas" << std::endl;
		SuperThaw();
		return false;
	}
	GNC::GCS::IVista* v  = pcv->ObtenerVistaRegistrada(window);

	if (v != NULL) {
		if (v->SoportaGuardar() && v->EstaModificada()) {
			std::string titulo = GNC::GCS::ControladorVistas::Instance()->GetTitulo(v);
			wxString descr;
			std::ostringstream os;
			os << _Std("Would you like to save the changes in the study:") << titulo.c_str() << "?";
			PauseSuperFreeze();
			int answer = wxMessageBox(wxString::FromUTF8(os.str().c_str()), _("Unsaved data"), wxYES_NO | wxCANCEL, this);
			ContSuperFreeze();
			if (answer == wxCANCEL) {
				permitido = false;
			}
			else if (answer == wxYES) {
				if (!v->Guardar()) {

					wxMessageDialog dialog(NULL,_("Errors have occurred when saving the series.\nWould you like to continue closing? "),_("Error saving"),wxYES_NO|wxICON_WARNING);
					PauseSuperFreeze();
					permitido = dialog.ShowModal() == wxID_YES;
					ContSuperFreeze();
				}
			}
		}
	}

	SuperThaw();

	return permitido;
}

void VentanaPrincipal::OnNotebookCambiarTab(wxAuiNotebookEvent& event)
{
	wxWindowDisabler disabler;
	SuperFreeze();
	if(m_pNoteBook->GetSelection()!=-1){
		GNC::GCS::IVista* v  = GNC::GCS::ControladorVistas::Instance()->ObtenerVistaRegistrada(m_pNoteBook->GetPage(event.GetSelection()));
		GNC::GCS::ControladorVistas* pcv = GNC::GCS::ControladorVistas::Instance();
		if(v!=NULL && pcv != NULL){
			pcv->SolicitarActivarVista(v);
		}
	}
	SuperThaw();
	event.Skip(true);
}

void VentanaPrincipal::OnNotebookMenuTab(wxAuiNotebookEvent& )
{
	wxMenu*  menu = new wxMenu();
	//cerrar pestania activa
	wxMenuItem*  pPantallaCompleta = new wxMenuItem( menu, ID_PANTALLA_COMPLETA, wxString(_("Full Screen")) + wxT("\tF11"), _("View Full Screen"), wxITEM_NORMAL );

	wxMenu* subMenuPestanias = new wxMenu();
	wxMenuItem* pMenuMostrarPrimerPlano = new wxMenuItem(subMenuPestanias, wxID_ANY, _("Tab Regroup"), _("Tab Regroup"), wxITEM_NORMAL );
	wxMenuItem* pMenuCerrarActiva = new wxMenuItem(menu, ID_CERRAR, _("&Close"), _("Close"), wxITEM_NORMAL );
	wxMenuItem* pMenuCerrarTodas = new wxMenuItem(menu, ID_CERRAR_TODAS, _("&Close all") , _("Close all windows"), wxITEM_NORMAL );

	menu->Connect(pMenuMostrarPrimerPlano->GetId(),wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( VentanaPrincipal::OnMenuPrimerPlanoPestaniaActiva),NULL,this);

#ifdef __WXMSW__
	pMenuCerrarActiva->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoCerrarTab());
	pMenuMostrarPrimerPlano->SetBitmaps(GinkgoResourcesManager::MenuTabs::GetIcoReagrupar());
	pPantallaCompleta->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoPantallaCompleta());
#else
	pMenuCerrarActiva->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoCerrarTab());
	pMenuMostrarPrimerPlano->SetBitmap(GinkgoResourcesManager::MenuTabs::GetIcoReagrupar());
	pPantallaCompleta->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoPantallaCompleta());
#endif

	menu->Append( pPantallaCompleta );
	menu->AppendSeparator();
	subMenuPestanias->Append(pMenuMostrarPrimerPlano);
	subMenuPestanias->AppendSeparator();
	menu->AppendSubMenu(subMenuPestanias,_("Tabbed organization"));
	menu->AppendSeparator();
	menu->Append(pMenuCerrarActiva);
	menu->Append(pMenuCerrarTodas);
	m_pNoteBook->PopupMenu(menu);
	delete menu;
}

void VentanaPrincipal::OnNotebookFocus(wxChildFocusEvent&)
{

/*	if(m_pNoteBook->GetPageCount() > 0){
		GNC::GCS::IVista* v  = GNC::GCS::ControladorVistas::Instance()->ObtenerPrimeraVista(m_pNoteBook->GetPage(m_pNoteBook->GetSelection()));
		v->GetWindow()->SetFocus();
	}
	*/
}

//region "eventos ginkgo"
void VentanaPrincipal::ProcesarEvento(GNC::GCS::Events::IEvent *pEvt)
{
	switch (pEvt->GetCodigoEvento()) {
		case ginkgoEVT_Core_UpdateAvailable:
			{
				GNC::GCS::Events::EventoUpdateAvailable* pE = dynamic_cast<GNC::GCS::Events::EventoUpdateAvailable*>(pEvt);
				if (pE != NULL) {

					int curRevision = GNC::GCS::IEntorno::Instance()->GetGinkgoRevision();

					bool superior = false;

					if ( pE->GetRevision() > curRevision ) {
						superior = true;
					}
					
					m_VersionChecked = true;
					m_VersionNueva = pE->GetVersionString();

					if (!superior) {
						if (pE->Informar()) {
							wxMessageBox(_("There is not newest Ginkgo CADx versions at this moment."), _("Ginkgo CADx updated"), wxICON_INFORMATION);
						}
						return;
					}

					std::string valor;
					GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/SkipUpdate", m_VersionNueva, valor, "0");
					if (valor == "1" && !pE->Informar()) {
						return;
					}

					//wxWindowDisabler dis;
					//SuperFreeze();

					m_pPanelActualizacion->Show();
					//m_pPanelCentral->Layout();
					Layout();
					Refresh(true);

					//SuperThaw();
				}
			}
			break;
		default:
			break;
	}
}
//endregion






//------------------------------------------------------------------------------
//region "IExtensionsObserver realization"

void VentanaPrincipal::OnModuleLoaded(GNC::GCS::IModuleController* )
{
	//GTRACE("VentanaPrincipal::OnModuloCargado(" << pCM->GetNombre().c_str() << ")");
	RefrescarMenus();
	GNC::GUI::HistoryPanel3::Instance()->ReloadToolBar();
}

void VentanaPrincipal::OnModuleUnloaded(GNC::GCS::IModuleController* )
{
	//GTRACE("VentanaPrincipal::OnModuloDescargado(" << pCM->GetNombre().c_str() << ")");
}

void VentanaPrincipal::OnModuleActivated(GNC::GCS::IModuleController* /*pCM*/)
{
	/*
	if (pCM != NULL) {
		GTRACE("VentanaPrincipal::OnModuloActivado(" << pCM->GetNombre().c_str() << ")");
	}
	else {
		GTRACE("VentanaPrincipal::OnModuloActivado(" << pCM->GetNombre().c_str() << ")");
	}
	*/
}

void VentanaPrincipal::OnModuleDeactivated(GNC::GCS::IModuleController* )
{
	//GTRACE("VentanaPrincipal::OnModuloDeactivado(" << pCM->GetNombre().c_str() << ")");
}
//endregion



//------------------------------------------------------------------------------
//region "IExtensionsObserver realization"

void VentanaPrincipal::OnVistaActivada(GNC::GCS::IVista* pVista) {
	#if defined (_WINDOWS)
	wxWindowDisabler dis;
	#endif
	if (pVista != NULL) {
		ForzarCambioVista(pVista);	
	}
	RefrescarMenus();
}

//endregion

//region "helpers"
void VentanaPrincipal::SuperFreeze()
{
	if (m_SuperFreezeCount <= 0) {
		GTRACE("SuperFreeze Locked");
		if (m_SuperFreezeCount < 0) {
			m_SuperFreezeCount = 0;
			std::cerr << "Error: Estado de bloqueo inconsistente." << std::endl;
		}
		/*Freeze();
		m_pPanelActualizacion->Freeze();
		m_pPanelCentral->Freeze();
		m_pNoteBook->Freeze();
		m_pMenuBar->Freeze();
		*/
	}
	m_SuperFreezeCount++;
}

void VentanaPrincipal::SuperThaw()
{
	m_SuperFreezeCount--;

	if (m_SuperFreezeCount <= 0) {
		GTRACE("SuperFreeze Released");
		if (m_SuperFreezeCount < 0) {
			m_SuperFreezeCount = 0;
			std::cerr << "Error: Estado de bloqueo inconsistente." << std::endl;
		}

		/*m_pMenuBar->Thaw();
		m_pNoteBook->Thaw();
		m_pPanelCentral->Thaw();
		m_pPanelActualizacion->Thaw();
		Thaw();*/
	}
}

void VentanaPrincipal::PauseSuperFreeze()
{
	if (m_SuperFreezeCount > 0) {
		GTRACE("SuperFreeze Paused");

		/*m_pMenuBar->Thaw();
		m_pNoteBook->Thaw();
		m_pPanelCentral->Thaw();
		m_pPanelActualizacion->Thaw();
		Thaw();*/
	}
}

void VentanaPrincipal::ContSuperFreeze()
{
	if (m_SuperFreezeCount > 0) {
		GTRACE("SuperFreeze Resumed");
	/*	Freeze();
		m_pPanelCentral->Freeze();
		m_pPanelActualizacion->Freeze();
		m_pNoteBook->Freeze();
		m_pMenuBar->Freeze();*/
	}
}
//endregion

//region "login e inicializacion"
void VentanaPrincipal::Login()
{
	if (GSEC::Auth::ControladorAutenticacion::Instance()->GetTipoAutenticacion() != GSEC::Auth::TA_NO_LOGIN)
	{
		try {

			GNC::GUI::DialogoLogin dlg(NULL);
			if (dlg.ShowModal() == wxID_OK) {
				MostrarVentana();
			}
			else {
				Close();
			}
		}
		catch (...)
		{
			LOG_ERROR("Core/Auth", "Error interno al autenticar");
			wxMessageBox(_("Internal error during auth process"), _("Internal error"), wxICON_ERROR);
			Close();
		}
	}
	else {
		MostrarVentana();
	}
}

void VentanaPrincipal::MostrarVentana()
{
	if (m_Iniciada) {
		Show(true);
		return;
	}
	m_Iniciada = true;
	try {
		//HL7 Service init
		if (GNC::GCS::IControladorPermisos::Instance()->Get("core.hl7", "autostart")) {
			GIL::HL7::ControladorEnvioHl7::Arrancar();
		}
	}
	catch(...)
	{
		LOG_ERROR("Core/Init", "Error starting HL7 service");
	}
	
	wxWindowDisabler dis;

	GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoUpdateAvailable() );

	std::ostringstream os;
	os <<  GNC::Entorno::Instance()->GetApplicationName() << " " << GNC::Entorno::Instance()->GetGinkgoCopyRight();
	SetStatusText(wxString::FromUTF8(os.str().c_str()));
	//Center();
#if !defined(_GINKGO_DEBUG)
/////////////////ATENCION!!!! esto es porque sino en windows no va bien, se desvincula la status bar
	#ifdef _WIN32
			Show(true);
			Maximize(true);
	#else
			Show(true);
	#endif
#else
	Show(true);
#endif
	SetFocus();
        //	ComprobarActualizaciones();

}

//endregion

//region "Getters y Setters de elementos de la interfaz"

wxAuiNotebook* VentanaPrincipal::GetAUINoteBook()
{
	return m_pNoteBook;
}

//endregion
