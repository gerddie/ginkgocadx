/*
 *  $Id: singleinstance.h $
 *  Proyecto Ginkgo
 *
 *  Copyright 2008 MetaEmotion S.L. All rights reserved.
 *
 */
#pragma once
#include <wx/ipc.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/controladorlog.h>
// Connection class, for use by both communicating instances

class GinkgoDispatcher: public GNC::GCS::ILockable {
public:
	static GinkgoDispatcher* Instance()
	{
		if (m_pInstance == NULL) {
			m_pInstance = new GinkgoDispatcher();
		}
		return m_pInstance;
	}
	static void FreeInstance()
	{
		if (m_pInstance != NULL) {
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	void InsertarPeticion(const std::string& peticion)
	{
		{
			GNC::GCS::ILocker lock(this);
			m_listaPeticiones.push_back(peticion);
		}
		if (!m_timer.IsRunning()) {
			Procesar();
		}

	}

	void Procesar()
	{
		if (m_listaPeticiones.size() == 0)
			return;
		// Check that there are no modal dialogs active
		wxWindowList::compatibility_iterator node = wxTopLevelWindows.GetFirst();
		while (node)
		{
			wxDialog* dialog = wxDynamicCast(node->GetData(), wxDialog);
			if (dialog && dialog->IsModal())
			{
				LOG_INFO("GinkgoDispatcher", "GinkgoDispatcher, hay diálogos abiertos no se procesa el mensaje");
				m_timer.Start(1000, true);
				return ;
			}
			node = node->GetNext();
		}
		std::string peticion;
		//extraigo la peticion a procesar
		{
			GNC::GCS::ILocker lock(this);
			peticion = m_listaPeticiones.front();
			m_listaPeticiones.pop_front();
		}

		//la proceso...
		wxString fileName = FROMPATH(peticion);
		GNC::Entorno::Instance()->GetVentanaPrincipal()->EjecutarParametro(fileName, false);

		{
			GNC::GCS::ILocker lock(this);
			if (m_listaPeticiones.size() > 0)
			{
				m_timer.Start(1000, true);
			}
		}
	}

protected:
	class wxTimerServer: public wxTimer
	{
		public:
			wxTimerServer(GinkgoDispatcher* pServer)
			{
				m_pServer = pServer;
			}

			~wxTimerServer()
			{}

			virtual void Notify()
			{
				m_pServer->Procesar();
			}
			GinkgoDispatcher* m_pServer;
	};

	GinkgoDispatcher(): m_timer(this){}
	~GinkgoDispatcher() {}

	static GinkgoDispatcher* m_pInstance;

	wxTimerServer m_timer;
	std::list<std::string> m_listaPeticiones;
};
GinkgoDispatcher* GinkgoDispatcher::m_pInstance = NULL;

//conexion
class stConnection : public wxConnection
{
public:
	stConnection() {}

	~stConnection() {}

	virtual bool OnExec(const wxString& WXUNUSED(topic),
                      const wxString& data)
  {

	  wxString filename(data);
		LOG_INFO("Core", "Servidor, OnExecute "<< filename.ToUTF8());
		if (filename.IsEmpty())
		{
			// Just raise the main window
			wxTopLevelWindow* frame = GNC::Entorno::Instance()->GetVentanaPrincipal();
			if (frame) {
				if (frame->IsIconized()) {
					frame->Restore();
				}
				frame->Raise();
			}
		}
		else
		{
			GinkgoDispatcher::Instance()->InsertarPeticion(std::string(TOPATH(filename)));
		}
		return true;
  }
};

class stServer: public wxServer
{
public:
	stServer()
	{
		//std::cout << "== stServer::stServer ( " << this << " )" << std::endl;
	}

	~stServer()
	{
		GinkgoDispatcher::FreeInstance();
		//std::cout << "== stServer::~stServer ( " << this << " )" << std::endl;
	}

	wxConnectionBase *OnAcceptConnection(const wxString& topic)
	{
		const wxString name = wxString::Format(wxT("Ginkgo-%s"), wxGetUserId().c_str());
		if (topic == name)
		{
			LOG_INFO("Core", "Servidor, conexión aceptada");
			return new stConnection();
		}
		else {
			LOG_INFO("Core", "Se han intentado comunicar con el servidor usando un topic desconocido");
			return NULL;
		}
	}
};
// Client class, to be used by subsequent instances in OnInit
class stClient: public wxClient
{
public:
	stClient() {};
	wxConnectionBase *OnMakeConnection() { return new stConnection; }
};




