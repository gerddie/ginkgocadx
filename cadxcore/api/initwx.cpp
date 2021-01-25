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


#define SINGLE_INSTANCE
#if defined (_WIN32) && defined (_GINKGO_DEBUG)
#include <cairo/cairo.h>
#if defined _GINKGO_CHECK_LEAKS
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#endif

#include <cctype>
#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <fcntl.h>

#include <wx/image.h>
#include <wx/splash.h>
#include <wx/msgout.h>
#include <wx/msgdlg.h>
#include <wx/dcmemory.h>
#include <wx/bitmap.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/dir.h>
#include <wx/sysopt.h>
#include <wx/log.h>
#include <wx/datetime.h>
#include <wx/stdpaths.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/wxsqlite3/wxsqlite3.h>
#include <wx/socket.h>
#include <wx/dcclient.h>

#include <main/controllers/configurationcontroller.h>
#include <main/tools/coretools.h>
#include "singleinstance.h"
#include "initwx.h"
#include <curl/curl.h>

#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif

#include <vtkPolyDataMapper.h>

#include <itkObject.h>
#include <itkMultiThreader.h>
#if defined(_GINKGO_DEBUG) && !defined(OMITIR_VENTANA_ERRORES_VTK)
#include <vtkObject.h>
#endif

#if defined(_WIN32)
#define _WINSOCKAPI_
#include <windows.h>
#include <io.h>
#endif
#if defined(_WIN32) && defined(_GINKGO_DEBUG) && !defined(_WIN32_WINNT_WIN7)
//#include <vld.h>
#endif
#if !defined(_WIN32)
#include <sys/signal.h>
#endif

#include <main/license.h>
#include <api/globals.h>
#include <api/imodulo.h>
#include <api/integration/template.h>
#include <api/toolsystem/itoolsregistry.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/commandcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <api/dicom/idicomconformance.h>
#include <eventos/hidesplashwindow.h>

#include "main/entorno.h"

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/pacscontroller.h>
#include <main/controllers/controladorcarga.h>
#include <main/controllers/controladorhl7.h>
#include <main/controllers/controladorenviohl7.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/controladorbbddhl7.h>
#include <main/controllers/controladoracciones.h>
#include <main/controllers/integrationcontroller.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/controladorinternacionalizacion.h>
#include <main/controllers/controladorautenticacion.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/thumbnailcontroller.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/hangingprotocolcontroller.h>
#include <main/controllers/autocleaningcontroller.h>
#include <main/controllers/modalitysettingscontroller.h>
#include <main/controllers/anonusagestats.h>
#include <main/controllers/smartretrievecontroller.h>
#include <main/controllers/storedqueriescontroller.h>

#include <main/gui/mainwindow/ventanaprincipal.h>

#include <resources/ginkgoresourcesmanager.h>

#define _GINKGO_ENABLE_SPLASH

using namespace GIL;

#if defined(_WIN32)

void RedirectIOToConsole()
{

        CONSOLE_SCREEN_BUFFER_INFO coninfo;
        FILE *fp;

        // allocate a console for this app
        AllocConsole();

        // set the screen buffer to be big enough to let us scroll text
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
                                   &coninfo);
        coninfo.dwSize.Y = 9999;
        SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE),
                                   coninfo.dwSize);

        // redirect unbuffered STDOUT to the console
        long lStdHandle = (long) GetStdHandle(STD_OUTPUT_HANDLE);
        long hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen(hConHandle, "w");
        *stdout = *fp;
        //setvbuf(stdout, NULL, _IONBF, 0);

        // redirect unbuffered STDIN to the console
        lStdHandle = (long) GetStdHandle(STD_INPUT_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen(hConHandle, "r");
        *stdin = *fp;
        //setvbuf(stdin, NULL, _IONBF, 0);

        // redirect unbuffered STDERR to the console
        lStdHandle = (long) GetStdHandle(STD_ERROR_HANDLE);
        hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
        fp = _fdopen(hConHandle, "w");
        *stderr = *fp;
        //setvbuf(stderr, NULL, _IONBF, 0);

        // make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
        // point to console as well
        //ios::sync_with_stdio();
}

#endif

class GinkgoEventsDelegate: GNC::GCS::IEventsObserver
{
public:
        GinkgoEventsDelegate(Ginkgo* pApp)
        {
                m_pApp = pApp;
                GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::EventHideSplashWindow());
        }

        ~GinkgoEventsDelegate()
        {
        }

        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt)
        {
                GNC::GCS::Events::EventHideSplashWindow* pEvt = dynamic_cast<GNC::GCS::Events::EventHideSplashWindow*>(evt);
                if (pEvt != NULL) {
                        if (m_pApp->GetLogoWindow() != NULL) {
                                m_pApp->GetLogoWindow()->Hide();
                        }
                }
        }

        virtual int FilterEvent(wxEvent &event)
        {
                // Automatically logout when the user is Away From Keyboard (AFK) for too long
                if (wxEVT_MOTION == event.GetEventType() || wxEVT_CHAR == event.GetEventType()) {
                        // User has done something, so stop the AFK timer
                        GNC::GCS::InactivityController::Instance()->ResetsInactivity();
                }
                const int FILTER_EVENT_PROCESS_NORMALLY = -1;

                return FILTER_EVENT_PROCESS_NORMALLY;
        }

        Ginkgo* m_pApp;
};

//Punto de entrada
IMPLEMENT_APP_NO_MAIN(Ginkgo)
#if defined(_WIN32)
int WINAPI InitWX(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
        SetProcessDefaultLayout(0);
        return wxEntry(hInstance, hPrevInstance, lpCmdLine, nCmdShow);
}
#else
int InitWX(int argc, char** argv)
{
        return wxEntry(argc, argv);
}
#endif

wxSplashScreen* Ginkgo::GetLogoWindow()
{
        return m_pVentanaLogo;
}

/* Carga las extensiones de plugins Ginkgo (Bibliotecas externas .plugin) */
void Ginkgo::LoadPlugins()
{
        GNC::ControladorExtensiones::Instance();
}

void Ginkgo::OnSplashClose(wxCloseEvent& WXUNUSED(event))
{
        m_pVentanaLogo = NULL;
}

// Evento de incialización de la aplicación

bool Ginkgo::OnInit()
{
        m_pEventObserverDelegate = new GinkgoEventsDelegate(this);

        setlocale(LC_NUMERIC, "C");

#if defined(__WXOSX__)
        m_Inicializado = false;
#endif
        //std::cout << "OnInit()" << std::endl;
#if defined(_GINKGO_DEBUG) && defined(_WINDOWS) && defined (_GINKGO_CHECK_LEAKS)
        _CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
        _CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

#if (defined(_WIN32) && defined(_GINKGO_DEBUG)) || defined(FORCE_ENABLE_CONSOLE)
        RedirectIOToConsole();
#endif

        wxLog *logger = new wxLogStderr();
        wxLog::SetActiveTarget(logger);

        {
                // TODO Migrate to getopt if params continues growing
                for (int i = 1; i < argc; ++i) { // Find the first "not starting with --" argument
                        wxString arg = wxString(argv[i]);
                        if (arg.StartsWith("--config-file")) {
                                wxString::size_type eqPos = arg.find("=");
                                if (eqPos != wxString::npos) {
                                        wxString configFilePath = arg.substr(eqPos + 1, std::string::npos);

                                        if (!wxFile::Exists(configFilePath)) {
                                                LOG_ERROR("INIT", "Specified config file does not exists" << ": " << TOPATH(configFilePath));
                                                GNC::GCS::IControladorLog::FreeInstance();
                                                GNC::Entorno::FreeInstance();
                                                GNC::GCS::ConfigurationController::FreeInstance();
                                                return false;
                                        } else {
                                                GNC::GCS::ConfigurationController::SetPreferredConfigFile(TOPATH(configFilePath));
                                        }
                                }
                        }
                }
        }

        wxSocketBase::Initialize();


        wxSQLite3Database::InitializeSQLite();
        //initialize WSA on windows
#if defined(_WINDOWS)
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD( 2, 2 ), &wsaData) != 0) {
                LOG_ERROR("INIT", "Error initializing network");
        }
#endif
        //initialize curl...
        /* Must initialize libcurl before any threads are started */
        curl_global_init(CURL_GLOBAL_ALL);

        WriteHeaderLog();

        //esto es para que no casque
        m_pServer = NULL;
#if defined(SINGLE_INSTANCE)

        //check numero de instancias del programa
        const wxString inst_name = wxString::Format(wxT("Ginkgo-%s"), wxGetUserId().c_str());
        m_singleInstanceChecker.Create(inst_name);
#if defined(_WINDOWS)
        const wxString& name = inst_name;
#else
        const wxString name(wxT("40000"));
#endif
        //comprobación una instancia
        //If using a single instance, use IPC to
        // communicate with the other instance
        if (!m_singleInstanceChecker.IsAnotherRunning()) {
                // Create a new server
                m_pServer = new stServer;
                if ( !m_pServer->Create( name ) ) {
                        //NO ha podido crearse el servidor......
                        LOG_ERROR("Core", "Error creating IPC server");
                }
        } else {
                GNC::Entorno::Instance()->SetChildInstance(true);
                LOG_INFO("Core", "Ginkgo client instance initiallized");
                m_pServer = NULL;
                // OK, there IS another one running, so try to connect to it
                // and send it any filename before exiting.
                stClient* client = new stClient;
                // ignored under DDE, host name in TCP/IP based classes
                wxString hostName = wxT("localhost");
                // Create the connection
                wxConnectionBase* connection =
                        client->MakeConnection(hostName,
                                               name, inst_name);
                if (connection) {
                        if (argc > 1) {
                                wxString str;
                                for (int i = 1; i < argc && str.IsEmpty(); ++i) { // Find the first "not starting with --" argument
                                        wxString arg = wxString(argv[1]);
                                        if (!arg.StartsWith("--")) {
                                                str = arg;
                                        }
                                }

                                if (!str.IsEmpty()) {
                                        if (str.Lower().StartsWith(wxT("ginkgocadx://"))) {
                                                LOG_INFO("Core", "The client sends following URL: " << str.ToUTF8());
                                                // Ask the other instance to open a file or raise itself
                                                connection->Execute(str);
                                                connection->Disconnect();
                                                delete connection;
                                        } else {
                                                wxFileName fileName = wxFileName(argv[1]);
                                                if (fileName.IsRelative()) {
#if defined(__WXGTK__)
                                                        char* invocation_dir = getenv("INVOCATION_DIR");
                                                        if (invocation_dir != NULL) {
                                                                wxString invocationDir = wxString::FromUTF8(invocation_dir);
                                                                if (!invocationDir.IsEmpty()) {
                                                                        fileName = invocationDir + wxFileName::GetPathSeparator() + fileName.GetFullPath();
                                                                }
                                                        }
#else
                                                        fileName.MakeAbsolute();
#endif
                                                }
                                                if (fileName.IsOk() && ((fileName.FileExists() && fileName.IsFileReadable()) || (wxDirExists(fileName.GetFullPath()) && fileName.IsDirReadable()))) {
                                                        LOG_INFO("Core", "The client sends following file: " << fileName.GetFullPath().ToUTF8());
                                                        // Ask the other instance to open a file or raise itself
                                                        connection->Execute(fileName.GetFullPath());
                                                        connection->Disconnect();
                                                        delete connection;
                                                } else {
                                                        wxString err = wxString::Format(wxT("Error: The file %s does not exists or is not readable"), argv[1]);
                                                        LOG_ERROR("Core", TOPATH(err));
                                                        wxMessageBox(err, wxT("Ginkgo CADx"), wxICON_INFORMATION|wxOK);
                                                }
                                        }
                                }
                        } else {
                                // Ask the other instance to open a file or raise itself
                                connection->Execute(wxT(""));
                                connection->Disconnect();
                                delete connection;
                        }
                } else {
                        LOG_ERROR("Core", "The client could not communicate with server because the server had opened modal dialogs");
                        wxMessageBox(wxT("Sorry, the existing instance may be too busy too respond.\nPlease close any open dialogs and retry."),
                                     wxT("Ginkgo CADx"), wxICON_INFORMATION|wxOK);
                }
                delete client;

                //nos salimos limpiamente sin dejar leaks
                GNC::GCS::IControladorLog::FreeInstance();
                GNC::Entorno::FreeInstance();
                GNC::GCS::ConfigurationController::FreeInstance();
                return false;
        }
#endif

        wxPNGHandler* handlerPng=new wxPNGHandler();
        wxImage::AddHandler(handlerPng);
#if defined(_GINKGO_ENABLE_SPLASH)
        m_pVentanaLogo = NULL;
        wxBitmap bitmap = GinkgoResourcesManager::Logos::GetLogoSplash();
        wxSplashScreen splash(bitmap, wxSPLASH_CENTRE_ON_SCREEN | wxSPLASH_NO_TIMEOUT,
                              -1, NULL, -1, wxDefaultPosition, wxDefaultSize,
                              wxSTAY_ON_TOP|wxNO_BORDER);
        wxIcon icono;
        icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
        splash.SetIcon(icono);
        splash.Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Ginkgo::OnSplashClose));
        // Copia del bitmap original de la ventana splash para pintar sobre ella
        if (m_pBitmapOriginal.Create(bitmap.GetWidth(), bitmap.GetHeight(), bitmap.GetDepth())) {
                m_pVentanaLogo = &splash;
                wxMemoryDC srcDC(bitmap);
                wxMemoryDC dstDC(m_pBitmapOriginal);
                dstDC.Blit(wxPoint(0, 0), srcDC.GetSize(), &srcDC, wxPoint(0, 0), wxCOPY, false, wxPoint(0, 0));
        }
#endif
        DoProgress(wxString(_("Initializing environment ...")));

        //inicializamos la intrnacionalizacion del core
        {
                GNC::GCS::ControladorInternacionalizacion::Instance()->AddCatalog("cadxcore");
        }
        //permisos del core
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "reutilize_study", _Std("Open only one tab for each study"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "anonymous_history", _Std("Anonymous mode (history is cleaned)"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "max_tabs", _Std("Max number of tabs opened"), false, "2", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "inactivity_time", _Std("Max seconds of Ginkgo CADx application inactivity"), false, "600", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "auto_close", _Std("Close automatically Ginkgo CADx after dicomize"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "export", _Std("Export images"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "acquire", _Std("Acquire DICOM files"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.restrictions", _Std("Ginkgo CADx restrictions"), "import", _Std("Import files to DICOM format"), false, "", true);
#ifdef GINKGO_PRO
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.help", _Std("Online manual"), "url_manual", _Std("URL of manual"), true, _Std("doc\\manual_ginkgo_cadx_pro_en.pdf"), true);
#else
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.help", _Std("Online manual"), "url_manual", _Std("URL of manual"), true, "http://doc.ginkgo-cadx.com/", true);
#endif
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.help", _Std("Online support"), "url_support", _Std("URL of online support"), true, _Std("http://ginkgo-cadx.com/"), true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.update", _Std("Program Updates"), "check_updates", _Std("Warn about newest Ginkgo CADx updates"), false, "http://updates.ginkgo-cadx.com/version3.xml", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.update", _Std("Program Updates"), "update_period", _Std("Update period (days)"), false, "2", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.seguridad",_Std("Security"), "setup_security", _Std("Setup Security"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.startup", _Std("Show on startup"), "startupdefaultvalue", _Std("Show on startup default value"), true, "", true);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.menu",_Std("Tools"),"configuracion",_Std("Settings menu"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.menu",_Std("Tools"),"licencia",_Std("License menu"), true, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "find_rsp_max_results", _Std("Maximum number of C-Find responses allowed"), false, "400", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"),"pacs_upload",_Std("Upload images to a PACS server"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "pacs_acquisition", _Std("Acquire images from PACS"),false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "remote_pacs", _Std("Enable remote PACS configuration"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "pacs_tls", _Std("Enable TLS configuration"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "patient_scope", _Std("Restrict search over selected patient"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "study_size", _Std("Restrict study retrieval by maximum size (KB)"), false, "16384", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.limits", _Std("PACS Limits"), "force_lossless", _Std("Force lossless when upload"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "storage_sop_class", _Std("Specify this Storage SOP Class in dicomization"), false, "1.2.840.10008.5.1.4.1.1.77.1.4", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "input_accno", _Std("Show accession number input"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "quality", _Std("Set compression, 0 implies no compression (0..14)"), false, "1", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "codec", _Std("Recompression codec (0=JPEG Baseline(default), 1=JPEG Progressive, 2=JPEG Lossless)"), false, "0", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "mantener", _Std("Don't change image codec (if possible)"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.importacion", _Std("Import"), "skip_demographic", _Std("Skip demographic step if it's possible"), false, "", false);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.hl7", _Std("HL7 Service"), "autostart", _Std("Automatically start HL7 Sender service"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.integracion", _Std("XML dicomization"), "send_message", _Std("Send Hl7 message when importing"), false, "", false);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.integracion", _Std("XML dicomization"), "upload_pacs_xml", _Std("Send study to PACS when importing"), false, "", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.integracion",_Std("XML dicomization"), "integration_template",_Std("Default ORU template"),false, DEFAULT_INTEGRATION_MESSAGE_TEMPLATE, true);

        // Private permissions: Styles
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgetsOverlay.maxTextWidth", "Widgets overlays Maximum text width", true, "-1", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgetsOverlay.fontSize", "Widgets overlays font size", true, "18.0", true);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "cornerAnnotation.fontSize", "Corner annotations font size", true, "14.0", true);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgets.fontSize", "Widgets font size", true, "14.0", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgets.maxTextWidth", "Widgets max text width", true, "350.0", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgets.lineWidth", "Widgets line width", true, "1.0", true);
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "widgets.draggingLineWidth", "Widgets dragging line width", true, "1.0", true);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("style", _Std("Style"), "ecg.label.fontSize", "ECG labels size", true, "14.0", true);

        std::string modalidades[20] = {"CR","CT","DR","DX","IO","MG","MR","NM","OT","PT","RF","RG","SC","SR","US","XA","XC","ES","ECG","HD"};
        //permisos modalidades
        for(int i = 0; i < 20; ++i) {
                wxString texto = wxString::Format(_("Download %s images"),wxString::FromUTF8(modalidades[i].c_str()).c_str());
                GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.download",_Std("PACS"),modalidades[i],std::string(texto.ToUTF8()),false, "", true);
        }

        wxString texto = _("Download all modalities");
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.pacs.download",_Std("PACS"),"all",std::string(texto.ToUTF8()),false, "", true);

        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("core.opengl", _Std("OpenGL"), "enable_shaders", _Std("Enable shaders"), false, "", true);

        //ginkgocloud
        GNC::GCS::IControladorPermisos::Instance()->AddDefinicionPermiso("ginkgocloud",_Std("Ginkgo Cloud"), "download",_Std("Download from Ginkgo Cloud"),false, "", true);

        itk::MultiThreader::SetGlobalDefaultNumberOfThreads(4);
        itk::MultiThreader::SetGlobalMaximumNumberOfThreads(16);

        /*
        vtkPolyDataMapper* m = vtkPolyDataMapper::New();
        m->GlobalImmediateModeRenderingOn();
        m->Delete();
        */
#ifdef __WXMAC__
        wxApp::s_macAboutMenuItemId = 1062;
        wxApp::s_macPreferencesMenuItemId = 1063;
        wxApp::s_macHelpMenuTitleName = wxT("A&yuda");
        //wxSystemOptions::SetOption( wxMAC_TEXTCONTROL_USE_SPELL_CHECKER, 1 );
#endif


#if !defined(_GINKGO_DEBUG) || defined(OMITIR_VENTANA_ERRORES_VTK)
        vtkObject::GlobalWarningDisplayOff();
        itk::Object::GlobalWarningDisplayOff();
#endif


        wxThread::SetConcurrency(wxThread::GetCPUCount());

        //codecs...
        wxJPEGHandler* handler=new wxJPEGHandler();
        wxImage::AddHandler(handler);

        wxGIFHandler* handlerGif=new wxGIFHandler();
        wxImage::AddHandler(handlerGif);

        wxTIFFHandler* handleTiff = new wxTIFFHandler();
        wxImage::AddHandler(handleTiff);

        GNC::Entorno::Instance()->SetApp(this);

        {
                wxWindowDisabler dis;
                m_pVentanaPrincipal = new VentanaPrincipal(NULL, wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()));
                m_pVentanaPrincipal->Hide();
                SetTopWindow(m_pVentanaPrincipal);
        }

        DoProgress(wxString(_("Tools loading base ...")));

        DoProgress(wxString(_("Starting tools...")));

        GNC::RegisterCoreTools();

        DoProgress(wxString(_("Loading plugins ...")));

        try {
                LoadPlugins();
        } catch (GNC::GCS::IRequestingException& ex) {
                m_pVentanaLogo->Hide();
                if (ex.shouldExit()) {
                        exit(-1);
                }
        } catch (GNC::GCS::IException& ex) {
                m_pVentanaLogo->Hide();
                LOG_ERROR("Init", "Error loading: " << ex.GetCause());
                wxMessageBox(_("Unable to start GinkgoCADx:\n") + wxString::FromUTF8(ex.GetCause().c_str()),_("error"),wxICON_INFORMATION);
                exit(-1);
        }

        DoProgress(wxString(_("Loading resources ...")));
        GIL::DICOM::Conformance::Load();

        DoProgress(wxString(_("Starting services...")));
        try {
                GIL::DICOM::PACSController::Instance()->StartServer();
        } catch(GNC::GCS::IException& ex) {
                LOG_ERROR("Init", "Error starting PACS Service: " << ex.GetCause());
        }
        try {
                GNC::GCS::SmartRetrieveController::Instance()->resetStatus();
        } catch(...) {
                LOG_ERROR("Init", "Error starting smartRetrieve service");
        }

        DoProgress(wxString(_("Configuring environment ...")));

        try {
                GNC::GCS::ConfigurationController::TListGroups profiles;
                GNC::GCS::ConfigurationController::Instance()->readGroupGeneral("/GinkgoCore/Profiles", profiles);

                for (GNC::GCS::ConfigurationController::TListGroups::iterator it = profiles.begin(); it != profiles.end(); ++it) {
                        bool value;
                        (*it).readBoolValue("Apply",value, false);
                        if (value) {
                                std::string xml;
                                if ((*it).readStringValue("XML", xml)) {
                                        GNC::GCS::IControladorPermisos::Instance()->CargarXMLPermisos(xml, false);
                                        break;
                                }
                        }
                }
        } catch(GNC::GCS::ControladorPermisosException& ex) {
                m_pVentanaLogo->Hide();
                wxMessageBox(_("Failed to set permissions:\n") + wxString::FromUTF8(ex.GetCause().c_str()),_("error"),wxICON_INFORMATION);
        }

        //es necesario porque hay permisos que afectan a menus
        m_pVentanaPrincipal->RefrescarMenus();

        //se registran vistas y modos
        const GNC::ControladorExtensiones::ListaModulos& listaModulos = GNC::ControladorExtensiones::Instance()->Modulos();
        for(GNC::ControladorExtensiones::ListaModulos::const_iterator it = listaModulos.begin(); it != listaModulos.end(); ++it) {
                (*it).second->RegistrarVistas();
        }

        DoProgress(wxString(_("Updating ...")));

        m_pVentanaPrincipal->CargarHistorial();

        DoProgress(wxString(_("Interpreting parameters ...")));



        //parseo
        wxCommandEvent ce(wxEVT_COMMAND_MENU_SELECTED, ID_Inicio_Aplicacion);
        m_pVentanaPrincipal->GetEventHandler()->AddPendingEvent(ce);

#if defined (__WXMAC__)
        m_Inicializado = true;
        if (m_OpenFileName.size() > 0) {
                GinkgoDispatcher::Instance()->InsertarPeticion(m_OpenFileName);
        }
#endif
#if defined(_GINKGO_ENABLE_SPLASH)
        m_pVentanaLogo = NULL;
#endif
        //launch auto cleaning controller...
        GNC::GCS::AutoCleaningController::Instance();

        LOG_STAT(ANON_STAT_RUN_GINKGO)

        return TRUE;
}

#ifdef __WXMAC__
void Ginkgo::MacOpenFile(const wxString &fileName)
{
        if (m_Inicializado) {
                GinkgoDispatcher::Instance()->InsertarPeticion( std::string(TOPATH(fileName)) );
        } else {
                m_OpenFileName = std::string(TOPATH(fileName));
        }

}
#endif


int Ginkgo::OnExit()
{
        LOG_INFO("Core", "Cerrando Ginkgo ");
        //UnRegister events
        delete m_pEventObserverDelegate;
        m_pEventObserverDelegate = NULL;

        //UnRegister tools
        try {
                GNC::GCS::IToolsRegistry::Instance()->UnRegisterAll();
        } catch (GNC::GCS::ToolsRegistryException& ex) {
                LOG_ERROR("Core", _Std("Error unregistering tools: ") << ex.str())
        }

        GNC::GCS::StoredQueryController::FreeInstance();
        GNC::GCS::SmartRetrieveController::FreeInstance();
        GNC::GCS::AutoCleaningController::FreeInstance();
        GNC::GCS::AnonUsageStats::FreeInstance();
        GNC::GCS::ThumbnailController::FreeInstance();
        GNC::GCS::ModalitySettingsController::FreeInstance();
        GNC::GCS::HangingProtocolController::FreeInstance();
        GNC::GCS::ControladorVistas::FreeInstance();
        GIL::IntegrationController::FreeInstance();
        GIL::HL7::ControladorEnvioHl7::FreeInstance();
        GNC::GCS::ControladorAcciones::FreeInstance();
        GNC::GCS::InactivityController::FreeInstance();
        GIL::DICOM::PACSController::FreeInstance();
        GNC::GCS::ICommandController::FreeInstance();
        GNC::ControladorExtensiones::FreeInstance();
        GNC::GCS::ControladorCarga::FreeInstance();
        GNC::HistoryToolsController::FreeInstance();
        GNC::GCS::IToolsRegistry::FreeInstance();
        GNC::GCS::HistoryController::FreeInstance();
        GIL::HL7::ControladorBBDDHl7::FreeInstance();
        GNC::GCS::IControladorPermisos::FreeInstance();
        GNC::GCS::ControladorInternacionalizacion::FreeInstance();
        GSEC::Auth::ControladorAutenticacion::FreeInstance();
        DicomServerList::FreeInstance();
        wxSQLite3Database::ShutdownSQLite();
        GNC::GCS::IEventsController::FreeInstance();

        GNC::Entorno::FreeInstance();

        wxSocketBase::Shutdown();

#if defined(_WINDOWS)
        if (m_pServer != NULL) {
                delete m_pServer;
        }
#endif
        GNC::GCS::ConfigurationController::FreeInstance();

        GNC::GCS::IControladorLog::FreeInstance();

#if defined(_WINDOWS)
        WSACleanup();
#endif
        curl_global_cleanup();

#if defined(_WINDOWS) && defined(_DEBUG)
        cairo_debug_reset_static_data();
#endif

        return wxApp::OnExit();
}

// Modificación del estado en el banner

void Ginkgo::DoProgress(wxString str)
{
        if (m_pVentanaLogo == NULL) {
                return;
        }
        wxSplashScreenWindow* pSplashWin = m_pVentanaLogo->GetSplashWindow();

        wxMemoryDC srcDC(m_pBitmapOriginal);
        wxClientDC wdc(pSplashWin);

        wxColour colorTexto;

        colorTexto.Set(70,70,70);

        // Restauración del logo
        wdc.Blit(wxPoint(0, 0), srcDC.GetSize(), &srcDC, wxPoint(0, 0), wxCOPY, false, wxPoint(0, 0));

        // Dibujado del texto
        wdc.SetTextForeground(colorTexto);
        wdc.DrawText(str, 190, srcDC.GetSize().y - 37);
}

void Ginkgo::WriteHeaderLog()
{
        LOG_INFO("Core", _Std("Starting") << " " << GNC::Entorno::Instance()->GetApplicationName() << " " << GNC::GCS::IEntorno::Instance()->GetGinkgoVersionString());
}
//endregion
