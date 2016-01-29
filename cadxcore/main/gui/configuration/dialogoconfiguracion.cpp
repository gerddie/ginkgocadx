/*
 *  
 *  $Id: dialogoconfiguracion.cpp $
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
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/filedlg.h>


#include "dialogoconfiguracion.h"
#include "panelconfiguracionestacion.h"
#include "panelconfiguracionhce.h"
#include "panelconfiguracionpacs.h"
#include "panelconfiguracionextensiones.h"
#include "panelconfiguracionpermisos.h"
#include "panelconfiguracionseguridad.h"
#include "panelconfiguracionubicaciones.h"
#include "panelconfiguraciongeneral.h"
#include "panelconfigurationanonymization.h"
#include "hangingprotocolconfigurationpanel.h"
#include "smartretrieveconfigurationpanel.h"
#include "localdatabaseconfigurationpanel.h"
#include "defaultmodalitysettingsconfigurationpanel.h"

#include <main/entorno.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorpermisos.h>

#include <resources/ginkgoresourcesmanager.h>

#include <wx/ginkgostyle/ginkgostyle.h>

#include <vector>
#include <sstream>

namespace GNC {
	namespace GUI {

		DialogoConfiguracion::DialogoConfiguracion(wxWindow* pParent):DialogoConfiguracionBase(pParent)
		{
			wxTreeItemId raiz = m_pTreeNavegacion->AddRoot(wxT("root"));
			wxTreeItemId ginkgo = m_pTreeNavegacion->AppendItem(raiz,wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()));

			SetTitle(wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()) + wxT(" ") + _("settings"));

			//se añade el primer paso de ginkgo
			wxTreeItemId primerId;
			IPasoConfiguracion* pPaso = new PanelConfiguracionGeneral(m_pPanelConfig,this);;
			pPaso->GetPanel()->Hide();
			wxTreeItemId id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			primerId = id;

			pPaso = new PanelConfiguracionEstacion(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new LocalDatabaseConfigurationPanel(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new PanelConfiguracionHCE(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new PanelConfiguracionPACS(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new SmartRetrieveConfigurationPanel(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;
/*
			pPaso = new PanelConfiguracionExtensiones(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;
*/
			pPaso = new PanelConfiguracionPermisos(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new DefaultModalitySettingsConfigurationPanel(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			pPaso = new HangingProtocolConfigurationPanel(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

			if (GNC::GCS::IControladorPermisos::Instance()->Get("core.seguridad","setup_security"))
			{
				pPaso = new PanelConfiguracionSeguridad(m_pPanelConfig, this);
				pPaso->GetPanel()->Hide();
				id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
				m_MapaPasos[id.m_pItem] = pPaso;
			}

			pPaso = new PanelConfiguracionUbicaciones(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;

/*			pPaso = new PanelConfigurationAnonymization(m_pPanelConfig, this);
			pPaso->GetPanel()->Hide();
			id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8(pPaso->GetTitle().c_str()));
			m_MapaPasos[id.m_pItem] = pPaso;
*/
			m_currentPaso = NULL;
			m_pTreeNavegacion->SelectItem(primerId);

			std::list<IPasoConfiguracion*> extraConfig;
			for (GNC::ControladorExtensiones::ListaModulos::const_iterator it = GNC::ControladorExtensiones::Instance()->Modulos().begin(); it != GNC::ControladorExtensiones::Instance()->Modulos().end(); ++it) 
			{
				(*it).second->GetPasosConfiguracion(extraConfig, m_pPanelConfig, this);				
			}

			for (std::list<IPasoConfiguracion*>::iterator it = extraConfig.begin(); it != extraConfig.end(); ++it)
			{
				(*it)->GetPanel()->Hide();
				id = m_pTreeNavegacion->AppendItem(ginkgo,wxString::FromUTF8((*it)->GetTitle().c_str()));
				m_MapaPasos[id.m_pItem] = (*it);
			}

			CargarCurrent();
			m_pBAplicar->Enable(false);

			Layout();
		}

		DialogoConfiguracion::~DialogoConfiguracion()
		{

		}

		void DialogoConfiguracion::CargarCurrent()
		{
			wxTreeItemId id = m_pTreeNavegacion->GetSelection();
			if(id.IsOk()){
				if(m_MapaPasos.find(id.m_pItem) != m_MapaPasos.end()){
					IPasoConfiguracion* pCurrent = m_MapaPasos[id.m_pItem];
					if( pCurrent!=NULL && pCurrent!=m_currentPaso){
						Freeze();
						if(m_currentPaso!=NULL){
							if(!m_currentPaso->Validar()){
								if(m_pTreeNavegacion->GetSelection() != id){
									m_pTreeNavegacion->SelectItem(id);
								}
								Thaw();
								return;
							}
							m_currentPaso->GetPanel()->Hide();
							m_pSizerPaneles->Detach(m_currentPaso->GetPanel());
						}
						m_currentPaso = pCurrent;
						m_currentPaso->GetPanel()->Show();
						m_pSizerPaneles->Add(m_currentPaso->GetPanel(),10, wxEXPAND|wxALL, 10);
						m_pTitledPanelConfig->SetTitle(wxString::FromUTF8(m_currentPaso->GetCabecera().c_str()));

						m_pPanelConfig->Layout();
						m_pPanelConfig->SetScrollbars(20, 20, 50, 50, 0, 0, false);
						m_pPanelConfig->AdjustScrollbars();
						m_pTitledPanelConfig->Layout();
						Thaw();					
					}
				}
			}
		}

		void DialogoConfiguracion::OnPropiedadCambiada()
		{
			if(!m_pBAplicar->IsEnabled()) {
				m_pBAplicar->Enable();
			}
		}

		void DialogoConfiguracion::OnCancelarClick(wxCommandEvent &)
		{
			EndModal(wxID_CANCEL);
		}

		void DialogoConfiguracion::OnAceptarClick(wxCommandEvent &)
		{
			if(m_pBAplicar->IsEnabled()){
				bool validar=true;
				for(MapaPasos::iterator it = m_MapaPasos.begin(); it != m_MapaPasos.end(); ++it){
					validar = validar && (*it).second->Validar();
				}

				if(validar){
					for(MapaPasos::iterator it = m_MapaPasos.begin(); it != m_MapaPasos.end(); ++it){
						(*it).second->Guardar();
					}

					GNC::GCS::ConfigurationController::Instance()->Flush();

					GNC::GCS::ControladorVistas* pC = GNC::GCS::ControladorVistas::Instance();
					if (pC != NULL) {
						pC->PropagarConfiguracionCambiada();
					}
					m_pBAplicar->Disable();
					EndModal(wxID_OK);
				}
			} else {
				EndModal(wxID_OK);
			}
		}

		void DialogoConfiguracion::OnAplicarClick(wxCommandEvent &)
		{
			bool validar=true;
			for(MapaPasos::iterator it = m_MapaPasos.begin(); it != m_MapaPasos.end(); ++it){
				validar = validar && (*it).second->Validar();
			}

			if(validar){
				for(MapaPasos::iterator it = m_MapaPasos.begin(); it != m_MapaPasos.end(); ++it){
					(*it).second->Guardar();
				}

				GNC::GCS::ConfigurationController::Instance()->Flush();

				GNC::GCS::ControladorVistas* pC = GNC::GCS::ControladorVistas::Instance();
				if (pC != NULL) {
					pC->PropagarConfiguracionCambiada();
				}
				m_pBAplicar->Disable();
			}
		}

		void DialogoConfiguracion::OnCambioNodoNavegacion(wxTreeEvent &){
			CargarCurrent();
		}

		void DialogoConfiguracion::OnExportarClick(wxCommandEvent &)
		{
			wxFileDialog seleccionarFichero (this, _("Export Configuration"), wxT(""), wxT(""), _("Ini Files(*.ini)|*.ini"),wxFD_SAVE);
			int response = seleccionarFichero.ShowModal();
			if (response == wxID_OK) {
				if(wxFileExists(seleccionarFichero.GetPath())) {
					response = wxMessageBox(_("File exists\nWould you like to overwrite it?"), _("Existing file"), wxYES_NO | wxCANCEL,this);
					if(response != wxYES) {
						return;
					}
				}
				std::string stdPath(TOPATH(seleccionarFichero.GetPath()));
				if (GNC::GCS::ConfigurationController::Instance()->saveGlobalConfigurationFile(stdPath)) {
					wxMessageBox(_("Export successfully completed"),_("Info"),wxOK,this);
				} else {
					wxMessageBox(_("There was an error during exportation"),_("Info"),wxOK,this);
				}
			}
		}

		void DialogoConfiguracion::OnImportarClick(wxCommandEvent &)
		{
			wxFileDialog seleccionarFichero (this, _("Import Configuration"), wxT(""), wxT(""), _("Ini files(*.ini)|*.ini"),wxFD_FILE_MUST_EXIST|wxFD_OPEN);
			int response = seleccionarFichero.ShowModal();
			if (response == wxID_OK) {
				std::string stdPath(TOPATH(seleccionarFichero.GetPath()));
				if (GNC::GCS::ConfigurationController::Instance()->loadGlobalConfigurationFile(stdPath)) {
					GNC::GCS::ConfigurationController::Instance()->Flush();

					for(MapaPasos::iterator it = m_MapaPasos.begin(); it != m_MapaPasos.end(); ++it){
						(*it).second->Recargar();
					}

					GNC::GCS::ControladorVistas* pC = GNC::GCS::ControladorVistas::Instance();
					if (pC != NULL) {
						pC->PropagarConfiguracionCambiada();
					}
					m_pBAplicar->Disable();
					wxMessageBox(_("Import successfully completed, restart Ginkgo CADx to apply changes"),_("Info"),wxOK,this);
				} else {
					wxMessageBox(_("There was an error during importation, check permissions"),_("Info"),wxOK,this);
				}
			}
		}

	}

}
