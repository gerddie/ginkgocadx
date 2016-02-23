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

#include <wx/wx.h>
#include <main/controllers/log4cplus/guiappender.h>
#include <main/controllers/configurationcontroller.h>
#include <api/internationalization/internationalization.h>
#include <set>
#include <fstream>
#include <sstream>
#include <wx/strconv.h>
#include <wx/log.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>

#include "ventanacontrollogs.h"

#include <resources/ginkgoresourcesmanager.h>
#define LOGGER "VentanaControlLogs"
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>


#define COL_FECHA           0
#define COL_NIVEL           1
#define COL_MENSAJE			2

namespace GNC {
	namespace GUI {

		VentanaControlLogs* VentanaControlLogs::m_pInstance = NULL;

		VentanaControlLogs* VentanaControlLogs::Instance()
		{
			if(m_pInstance == NULL) {
				m_pInstance = new VentanaControlLogs();
			}
			return m_pInstance;
		}

		void VentanaControlLogs::CerrarSiAbierta()
		{
			if(m_pInstance != NULL)
			{
				m_pInstance->Close(true);
				m_pInstance = NULL;
			}
		}

		void VentanaControlLogs::Refrescar()
		{
		}

		//region Interfaz ILogger

		void VentanaControlLogs::Append(const GNC::GCS::Logging::LogEvent& le)
		{
			GNC::GCS::ILocker lock(this);
			//write event into pending events array and set flag to refresh in idle time
			wxCSConv conv(wxFONTENCODING_UTF8);
			wxString str(le.formattedmsg.c_str(), conv);
			if (str.IsEmpty() && !le.formattedmsg.empty()) { // Fallback: CONV TO ISO
				wxCSConv convLatin1(wxFONTENCODING_ISO8859_1);
				str = wxString(le.formattedmsg.c_str(), convLatin1);
			}
			pendingLines.push_back(str);
			m_needRefresh = true;
		}

		//endregion

		VentanaControlLogs::VentanaControlLogs() : VentanaControlLogsBase(NULL)
		{
			m_needRefresh = false;

			wxIcon icono;
			icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
			this->SetIcon(icono);

			// Setup del combobox de niveles

			GNC::GCS::IControladorLog::Instance()->GetLogLevelsMap(m_MapaLogLevels);

			int i = 0;

			for (GNC::GCS::IControladorLog::MapaLogLevels::iterator it = m_MapaLogLevels.begin(); it != m_MapaLogLevels.end(); ++it, i++)
			{
				m_pComboNivel->Insert(wxString::FromUTF8((*it).second.c_str()), 0);
			}

			// Setup del nivel actual

			wxString nivelActual = wxString::FromUTF8(m_MapaLogLevels[GNC::GCS::IControladorLog::Instance()->GetActiveLogLevel()].c_str());

			bool notfound = true;
			for (int i = 0; notfound && i < (int)m_pComboNivel->GetCount(); i++)
			{
				if (nivelActual == m_pComboNivel->GetString(i) ) {
					m_pComboNivel->Select(i);
					notfound = false;
				}
			}

			this->Connect( m_menuItemGuardar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlLogs::OnGuardarRegistros ) );
			this->Connect( m_menuItemLimpiar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlLogs::OnLimpiarClick ) );
            this->Connect( m_menuItemSalir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlLogs::OnSalirClick ) );

			Layout();

			gnkLog4cplus::GUIAppender::Instance()->Attach(this);

			Show();
		}

		VentanaControlLogs::~VentanaControlLogs()
		{
			m_pInstance = NULL;
			gnkLog4cplus::GUIAppender::Instance()->Attach(NULL);

		}

		void VentanaControlLogs::OnClose( wxCloseEvent& event )
		{
			gnkLog4cplus::GUIAppender::Instance()->Attach(NULL);
			event.Skip();
		}

		void VentanaControlLogs::OnInternalIdle()
		{
			if (m_needRefresh) {
				GNC::GCS::ILocker locker(this);
				//if text in the control is too big remove a half
				const int maxBufferSize = 100*1024;
				if (m_pRegistros->GetValue().size() > maxBufferSize) {
					//if it's bigger than 100K remove first 50K
					wxString value = m_pRegistros->GetValue();
					m_pRegistros->SetValue(value.substr(value.size() - maxBufferSize/2));
				}

				//write pending events
				Freeze();
				for (wxArrayString::iterator it = pendingLines.begin(); it != pendingLines.end(); ++it) {
					m_pRegistros->AppendText((*it));
				}
				//set stroll to the bottom
				m_pRegistros->ShowPosition(m_pRegistros->GetLastPosition());
				Thaw();
				pendingLines.clear();
				m_pRegistros->Refresh(true);
				m_needRefresh = false;
			}
		}

		void VentanaControlLogs::OnGuardarRegistros( wxCommandEvent& /*event*/ )
		{
			wxFileDialog seleccionarFichero (this, _("Save logs"), wxT(""), wxT(""), _("Log file (*.log) | *.log | Text file (*.txt) | *.txt"),wxFD_SAVE);
			int response = seleccionarFichero.ShowModal();
			if (response == wxID_OK) {
				if(wxFileExists(seleccionarFichero.GetPath())) {
					response = wxMessageBox(_("File exists\nWould you like to overwrite it?"),_("Overwrite"),wxYES_NO | wxCANCEL,this);
					if(response != wxYES) {
						return;
					}
				}

				std::ofstream fout;
				std::ostringstream sout;
				try {
					fout.open(TOPATH(seleccionarFichero.GetPath()).c_str());
				}
				catch (std::exception& ex)
				{
					sout << _Std("File couldn't be stored ") << seleccionarFichero.GetPath().ToUTF8() << " : " << ex.what();
					wxMessageBox(wxString::FromUTF8(sout.str().c_str()),_("Error"),wxICON_ERROR);
					return;
				}

				if (!fout)
				{
					sout << _Std("File couldn't be stored ") << seleccionarFichero.GetPath().ToUTF8();
					wxMessageBox(wxString::FromUTF8(sout.str().c_str()),_("Error"),wxICON_ERROR);
					return;
				}

				try {
					fout << m_pRegistros->GetValue().ToUTF8();
				}
				catch (std::exception& ex)
				{
					sout << _Std("File couldn't be stored ") << seleccionarFichero.GetPath().ToUTF8() << " : " << ex.what();
					wxMessageBox(wxString::FromUTF8(sout.str().c_str()),_("Error"),wxICON_ERROR);
					return;
				}

				try {
					fout.close();
				}
				catch (std::exception& ex)
				{
					sout << _Std("Could not write to file") << seleccionarFichero.GetPath().ToUTF8() << " : " << ex.what();
					wxMessageBox(wxString::FromUTF8(sout.str().c_str()),_("Error"),wxICON_ERROR);
				}
			}
		}

		void VentanaControlLogs::OnNivelChoice( wxCommandEvent& /*event*/ )
		{

			int nivel = GNC::GCS::IControladorLog::Instance()->GetLogLevelCode(std::string(m_pComboNivel->GetStringSelection().ToUTF8()));

			GNC::GCS::IControladorLog::Instance()->SetActiveLogLevel(nivel);
			GNC::GCS::ConfigurationController::Instance()->writeIntGeneral("/GinkgoCore/Log", "LogLevel", nivel);

		}

		void VentanaControlLogs::LimpiarRegistros()
		{
			m_pRegistros->Clear();
			gnkLog4cplus::GUIAppender::Instance()->clear();

		}

		void VentanaControlLogs::OnSalirClick( wxCommandEvent& /*event*/ )
		{
			Close();
		}

		void VentanaControlLogs::OnKeyDown( wxKeyEvent& event )
		{
			if (event.GetKeyCode() == WXK_ESCAPE) {
				Close();
				event.Skip(false);
			}
			else {
				event.Skip();
			}
		}
	}

}
