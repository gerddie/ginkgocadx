 /*
*  
*  $Id: panelconfiguracionpacs.cpp $
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
#include <vector>
#include <sstream>
#include <limits>

#include <main/controllers/dcmtk/dicomnetwork.h>
#include <main/controllers/dcmtk/dicomechoassociation.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/pacscontroller.h>
#include <main/controllers/controladorextensiones.h>


#include <wx/uri.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/valtext.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include <wx/file.h>
#include <wx/textfile.h>
#include <wx/dir.h>
#include <wx/filedlg.h>
#include <wx/log.h>
#include <wx/busyinfo.h>
#include <wx/xml/xml.h>

#include "panelconfiguracionpacs.h"

#include <sstream>

#include <main/entorno.h>
#include <resources/ginkgoresourcesmanager.h>

#define NAME_COLUMN_IDX             0
#define NAME_COLUMN_WIDTH           300
#define USE_BY_DEFAULT_COLUMN_IDX   1
#define USE_BY_DEFAULT_COLUMN_WIDTH 80

namespace GNC {
	namespace GUI {

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////DialogoEditarCertificado//////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class DialogoEditarCertificado: public DialogoEditarCertificadoBase
		{
		public:
			DialogoEditarCertificado(wxWindow* pParent, const std::string& cert, const std::string& privateKey, bool permitirDefecto): DialogoEditarCertificadoBase(pParent)
			{
				m_Ok = false;
				m_pTextoCertificado->SetValue(wxString::FromUTF8(cert.c_str()));
				m_pTextoPrivateKey->SetValue(wxString::FromUTF8(privateKey.c_str()));

				if (permitirDefecto) {
					m_pCheckDefault->SetValue(cert.size() == 0 && privateKey.size() == 0);
				} else {
					m_pCheckDefault->SetValue(false);
					m_pPanelDefault->Show(false);
					m_pSepDefault->Show(false);
					m_pBody->Layout();
					Layout();
					Refresh(true);
				}

				RefrescarEstado();
			}
			~DialogoEditarCertificado() {
			}

			virtual void OnAceptarClick(wxCommandEvent&)
			{
				if (Validar()) {
					m_Ok = true;
					EndModal(wxID_OK);
				}
			}

			void CargarFichero(bool publicKey) 
			{
				wxFileDialog seleccionarFichero (this, _("Select certificate"), wxT(""), wxT(""), _("PEM files(*.pem;*.key)|*.pem;*.key|All files (*.*)|*.*"),wxFD_FILE_MUST_EXIST|wxFD_OPEN);
				int response = seleccionarFichero.ShowModal();
				if (response == wxID_OK) {
					wxTextFile textFile(seleccionarFichero.GetPath());
					if (textFile.Open()) {
						wxString cadena;
						if (!textFile.Eof()) {
							cadena += textFile.GetNextLine();
							while(!textFile.Eof()) {
								cadena += wxT("\n") + textFile.GetNextLine();
							}
						}
						if (publicKey) {
							m_pTextoCertificado->SetValue(cadena);
						} else {
							m_pTextoPrivateKey->SetValue(cadena);
						}
						textFile.Close();
					}
				}				
			}

			virtual void OnLoadPrivateClick(wxCommandEvent& )
			{
				CargarFichero(false);
			}

			virtual void OnLoadPublicClick(wxCommandEvent& )
			{
				CargarFichero(true);
			}

			virtual void OnResetToDefault(wxCommandEvent& ) 
			{
				if (m_pPanelDefault->IsShown()) {
					m_pCheckDefault->SetValue(true);
				} else {
					//cargamos el certificado por defecto...
#ifdef GINKGO_PUBLIC_KEY
					m_pTextoCertificado->SetValue(wxString::FromUTF8(GINKGO_PUBLIC_KEY));
#else
					m_pTextoCertificado->SetValue(wxT(""));
#endif
#ifdef GINKGO_PRIVATE_KEY
					m_pTextoPrivateKey->SetValue(wxString::FromUTF8(GINKGO_PRIVATE_KEY));
#else
					m_pTextoPrivateKey->SetValue(wxT(""));
#endif
				}

				RefrescarEstado();
			}

			virtual void OnCheckDefault(wxCommandEvent&)
			{
				RefrescarEstado();
			}

			virtual void RefrescarEstado()
			{
				m_pPanelPublic->Enable(!m_pCheckDefault->IsChecked());
				m_pPanelPublic->Refresh();
				m_pPanelPrivate->Enable(!m_pCheckDefault->IsChecked());
				m_pPanelPrivate->Refresh();

				if (m_pCheckDefault->IsChecked()) {
					m_pTextoCertificado->SetValue(wxT(""));
					m_pTextoPrivateKey->SetValue(wxT(""));
				}
			}

			std::string GetPublicValue()
			{
				std::string cert(m_pTextoCertificado->GetValue().ToUTF8());
				return cert;
			}

			std::string GetPrivateValue()
			{
				std::string cert(m_pTextoPrivateKey->GetValue().ToUTF8());
				return cert;
			}

			bool GetUseDefaultCert()
			{
				return m_pCheckDefault->GetValue();
			}

			bool Validar()
			{
				bool ok = true;
				std::stringstream sstream;

				if (!m_pCheckDefault->IsChecked() && m_pCheckDefault->IsShown()) {
					if (m_pTextoCertificado->GetValue().size() == 0 || m_pTextoPrivateKey->GetValue().size() == 0)
					{
						ok = false;
						sstream << _Std("You have to set private and public key") ;
					}
					if (!ok) {
						wxMessageBox(wxString::FromUTF8(sstream.str().c_str()),_("Info"),wxOK | wxICON_INFORMATION, this);
					}
				}
				return ok;
			}

			bool m_Ok;	
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////DialogoLocalPACS//////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		class DialogoLocalPACS: public DialogoLocalPACSBase
		{
		public:

			DialogoLocalPACS(wxWindow* pParent): DialogoLocalPACSBase(pParent)
			{
				m_Ok = false;
			}

			~DialogoLocalPACS()
			{}

			void SetParametros(PanelConfiguracionPACS::TipoConfigLocal& pacsLocal)
			{
				m_pUseTLS->SetValue(pacsLocal.useTLS);

				m_pVerifyServer->SetValue(pacsLocal.verifyCredentials);

				m_certificado = pacsLocal.certificate;
				m_privateKey = pacsLocal.privateKey;
				if (m_certificado.size()>0 || m_privateKey.size()>0) {
					m_pBCert->SetLabel(_("Change ..."));
				} else {
					m_pBCert->SetLabel(_("Import ..."));
				}
			}

			void GetParametros(PanelConfiguracionPACS::TipoConfigLocal& pacsLocal)
			{
				pacsLocal.useTLS = m_pUseTLS->GetValue();

				pacsLocal.verifyCredentials = m_pVerifyServer->GetValue();

				pacsLocal.certificate = m_certificado;
				pacsLocal.privateKey = m_privateKey;
			}


			virtual void OnEditCertificate(wxCommandEvent&)
			{
				DialogoEditarCertificado dlg(this, m_certificado, m_privateKey, false);
				dlg.ShowModal();
				if (dlg.m_Ok) {
					m_certificado = dlg.GetPublicValue();
					m_privateKey = dlg.GetPrivateValue();
					if (m_certificado.size()>0 || m_privateKey.size()>0) {
						m_pBCert->SetLabel(_("Change ..."));
					} else {
						m_pBCert->SetLabel(_("Import ..."));
					}
				}
			}

			virtual void OnAceptarClick(wxCommandEvent&)
			{
				if (Validar()) {
					m_Ok = true;
					EndModal(wxID_OK);
				}
			}

			bool Validar()
			{
				bool ok = true;
				std::stringstream sstream;

				if (m_pUseTLS->GetValue() 
					&& (m_certificado.size() == 0 || m_privateKey.size() == 0))
				{
					ok = false;
					sstream << _Std("If you want to use SSL/TLS you have to set a valid certificate and a valid private key") ;
				}
				if (!ok) {
					wxMessageBox(wxString::FromUTF8(sstream.str().c_str()),_("Info"),wxOK | wxICON_INFORMATION, this);
				}
				return ok;
			}

			bool m_Ok;
			std::string m_certificado;
			std::string m_privateKey;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////DialogoServidorPACS///////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class DialogoServidorPACS: public DialogoServidorPACSBase
		{
		public:

			DialogoServidorPACS(wxWindow* pParent, PanelConfiguracionPACS::TipoConfigLocal& configLocal, const wxString& strPuertoLocal, const wxString& aetLocal, std::list<std::string> usedPACSIds) :
				DialogoServidorPACSBase(pParent),
				m_configLocal(configLocal),
				m_Ok(false),
				m_aetLocal(aetLocal.ToUTF8()),
				m_puertoLocal(11112),
				m_idsPacs(usedPACSIds)
			{
				wxTextValidator OnlyNum = wxTextValidator(wxFILTER_NUMERIC);
				m_pPuerto->SetValidator(OnlyNum);

				m_pPDU->SetValue(16384);

				long puertoLocal;
				if(strPuertoLocal.ToLong(&puertoLocal)){
					m_puertoLocal = puertoLocal;
				}

				if(!GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_tls")) {
					m_pPanelSeguridad->Show(false);
					m_notebook1->RemovePage(1);
				}

				//worklist settings...
#ifndef GINKGO_PRO
				m_pWorkListPanel->Show(false);
#endif
				//

				LoadMessageDefaultValues();
				RefreshHL7Panel();
			}

			~DialogoServidorPACS()
			{

			}

			void GetParametros(DicomServer& tipoPacs)
			{
				tipoPacs.ID = std::string(m_pId->GetValue().ToUTF8());
				tipoPacs.AET= std::string(m_pAET->GetValue().ToUTF8());
				tipoPacs.HostName = std::string(m_pHost->GetValue().ToUTF8());
				tipoPacs.Port = atoi(m_pPuerto->GetValue().ToUTF8());
				tipoPacs.PDU = m_pPDU->GetValue();

				switch (m_pRetrieveMethod->GetSelection()) {
				case 2:
					tipoPacs.retrieveMethod = DicomServer::WADO;
					break;
				case 1:
					tipoPacs.retrieveMethod = DicomServer::GET;
					break;
				case 0:
				default:
					tipoPacs.retrieveMethod = DicomServer::MOVE;
					break;
				}
				tipoPacs.wadoURI = std::string(m_pWadoURL->GetValue().ToUTF8());
				tipoPacs.retrieveSeries = m_pRetrieveMode->GetSelection() == 0;

				tipoPacs.reuseConnection = m_pReuseConnection->GetValue();

				if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_tls")) {
					tipoPacs.useTLS = m_pUseSSL->GetValue();

					tipoPacs.pacsUser = m_pUser->GetValue().ToUTF8();
					tipoPacs.pacsPass = m_pPassword->GetValue().ToUTF8();

					tipoPacs.verifyCredentials = m_pVerifyServer->GetValue();

					tipoPacs.certificate = m_certificado;
					tipoPacs.privateKey = m_privateKey;
				}
				tipoPacs.defaultCharset = m_pComboCharset->GetValue().ToUTF8();

				//wl
				tipoPacs.GetWlConfig().useDICOMMPPS = m_pCheckDICOMMPPS->IsChecked();
				if (m_pCheckHL7->IsChecked()) {
					tipoPacs.GetWlConfig().useAbortMessage = m_pCheckAbort->IsChecked();
					tipoPacs.GetWlConfig().useCompletedMessage = m_pCheckCompleted->IsChecked();
					tipoPacs.GetWlConfig().useInProgressMessage = m_pCheckInProgress->IsChecked();
				} else {
					tipoPacs.GetWlConfig().useAbortMessage = tipoPacs.GetWlConfig().useCompletedMessage = tipoPacs.GetWlConfig().useInProgressMessage = false;
				}
				tipoPacs.GetWlConfig().abortTemplate = m_pTextAbort->GetValue().ToUTF8();
				tipoPacs.GetWlConfig().completedTemplate = m_pTextCompleted->GetValue().ToUTF8();
				tipoPacs.GetWlConfig().inProgressTemplate = m_pTextProgress->GetValue().ToUTF8();
				//
			}

			void SetParametros(DicomServer& tipoPacs)
			{
				m_pId->SetValue(wxString::FromUTF8(tipoPacs.ID.c_str()));
				m_pAET->SetValue(wxString::FromUTF8(tipoPacs.AET.c_str()));
				m_pHost->SetValue(wxString::FromUTF8(tipoPacs.HostName.c_str()));
				m_pPuerto->SetValue(wxString::Format(wxT("%d"), (int) tipoPacs.Port));
				m_pPDU->SetValue(wxString::Format(wxT("%d"), (int) tipoPacs.PDU));

				m_pRetrieveMethod->SetSelection((int)tipoPacs.retrieveMethod);
				m_pRetrieveMode->SetSelection(tipoPacs.retrieveSeries?0:1);
				m_pWadoURL->SetValue(wxString::FromUTF8(tipoPacs.wadoURI.c_str()));

				m_pReuseConnection->SetValue(tipoPacs.reuseConnection);

				if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_tls")) {
					m_pUseSSL->SetValue(tipoPacs.useTLS);
					m_pUser->SetValue(wxString::FromUTF8(tipoPacs.pacsUser.c_str()));
					m_pPassword->SetValue(wxString::FromUTF8(tipoPacs.pacsPass.c_str()));

					m_pVerifyServer->SetValue(tipoPacs.verifyCredentials);

					m_certificado = tipoPacs.certificate;
					m_privateKey = tipoPacs.privateKey;
				}

				m_pComboCharset->SetValue(wxString::FromUTF8(tipoPacs.defaultCharset.c_str()));

				//wl
				m_pCheckDICOMMPPS->SetValue(tipoPacs.GetWlConfig().useDICOMMPPS);
				m_pCheckHL7->SetValue(tipoPacs.GetWlConfig().useAbortMessage || tipoPacs.GetWlConfig().useCompletedMessage || tipoPacs.GetWlConfig().useInProgressMessage);
				m_pCheckAbort->SetValue(tipoPacs.GetWlConfig().useAbortMessage);
				m_pCheckCompleted->SetValue(tipoPacs.GetWlConfig().useCompletedMessage);
				m_pCheckInProgress->SetValue(tipoPacs.GetWlConfig().useInProgressMessage);
				m_pTextAbort->SetValue(wxString::FromUTF8(tipoPacs.GetWlConfig().abortTemplate.c_str()));
				m_pTextCompleted->SetValue(wxString::FromUTF8(tipoPacs.GetWlConfig().completedTemplate.c_str()));
				m_pTextProgress->SetValue(wxString::FromUTF8(tipoPacs.GetWlConfig().inProgressTemplate.c_str()));
				//
				RefreshHL7Panel();
				wxCommandEvent evt;
				OnRetrieveMethodChoice(evt);
			}

			virtual void OnAdvancedConfiguration(wxCommandEvent&)
			{
				DialogoEditarCertificado dlg(this, m_certificado, m_privateKey, true);
				dlg.ShowModal();
				if (dlg.m_Ok) {
					if (dlg.GetUseDefaultCert()) {
						m_certificado = m_configLocal.certificate;
						m_privateKey = m_configLocal.privateKey;
					}
					else {
						m_certificado = dlg.GetPublicValue();
						m_privateKey = dlg.GetPrivateValue();
					}
				}
			}

			virtual void OnRetrieveMethodChoice(wxCommandEvent&)
			{
				bool enableWADO = m_pRetrieveMethod->GetSelection() == DicomServer::WADO;
				m_pWadoURL->Enable(enableWADO);
				m_pRetrieveMode->Enable(!enableWADO);
			}

			virtual void OnCancelClick(wxCommandEvent &){
				m_Ok=false;
				EndModal(wxID_CANCEL);
			}

			virtual void OnOkClick(wxCommandEvent &){
				if(Validar()) {
					m_Ok=true;
					EndModal(wxID_OK);
				}
			}

			bool Validar ()
			{
				bool ok = true;
				if(m_pId->GetValue() != wxEmptyString && m_pAET->GetValue() != wxEmptyString && m_pHost->GetValue() != wxEmptyString && m_pPuerto->GetValue() != wxEmptyString){
					std::stringstream sstream;
					sstream << _Std("There are this errors: ");

					std::string idPACS = std::string(m_pId->GetValue().ToUTF8());
					for (std::list<std::string>::iterator it = m_idsPacs.begin(); it != m_idsPacs.end(); ++it) {
						if (idPACS == (*it)) {
							sstream << _Std("\n- This PACS id is in use");
							ok = false;
							break;
						}
					}

					long numero;
					if(!m_pPuerto->GetValue().ToLong(&numero)){
						ok = false;
						sstream << _Std("\n- The port must be a positive short integer (16 bits)");
					} else{
						if(numero > 32767){//maxshort
							ok = false;
							sstream <<_Std("\n- The port is too large");
						}
					}

					if(m_pAET->GetLabel().size() >16) {
						ok = false;
						sstream <<_Std("\n- The maximum length of the AET is 16 characters");
					}

					if (m_pPDU->GetValue() != 0) {

						if(!m_pPDU->GetValue()){
							ok = false;
							sstream << _Std("\n- The PDU size should be greater than or equal to zero (default = 16384)");
						} else{
							if(m_pPDU->GetValue() > MAX_PDU_LENGTH){ // max, viene de esta constante ASC_MAXIMUMPDUSIZE de DCMTK
								ok = false;
								sstream << _Std("\n- The maximum value of the PDU is ") <<  MAX_PDU_LENGTH << "(" << MAX_PDU_LENGTH/1024 << "K)";
							}
							if(m_pPDU->GetValue() < MIN_PDU_LENGTH){ // min, viene de esta constante ASC_MINIMUMPDUSIZE de DCMTK
								ok = false;
								sstream << _Std("\n- The minimum value of the PDU is ") <<  MIN_PDU_LENGTH << "(" << MIN_PDU_LENGTH/1024 << "K)" ;
							}
						}
					}

					if (m_pUseSSL->GetValue() && ( (m_certificado.size() == 0 || m_privateKey.size() == 0) && (m_configLocal.certificate.size() == 0 || m_configLocal.privateKey.size() == 0 )  ) )
					{
						ok = false;
						sstream << _Std("\n- If you want to use SSL/TLS you have to set a valid certificate and private key") ;
					}

					if (m_pRetrieveMethod->GetSelection() == DicomServer::WADO) {
						if (m_pWadoURL->GetValue().IsEmpty()) {
							ok = false;
							sstream << _Std("\n- If you want to use WADO you have to fill in wado url") ;
						} else {
							wxURI uri(m_pWadoURL->GetValue());
							if (uri.GetScheme().IsEmpty()) {
								sstream << _Std("\n- WADO URL doesn't contains scheme (i.e. http://)") ;
								ok = false;
							} else if (uri.GetServer().IsEmpty()) {
								sstream << _Std("\n- WADO URL doesn't contains host") ;
								ok = false;
							}
						}
					}

					if(!ok)
						wxMessageBox(wxString::FromUTF8(sstream.str().c_str()),_("Info"),wxOK | wxICON_INFORMATION, this);
				}else{
					ok = false;
					wxMessageBox(_("You must fill in all fields to continue"),_("Info"),wxOK | wxICON_INFORMATION, this);
				}
				return ok;
			}

			virtual void OnProbarClick(wxCommandEvent &)
			{
				if(Validar()) {
					bool success = true;
					std::ostringstream errorMsg;
					std::string errorTitle;

					{
						long puerto;
						long pdu = m_pPDU->GetValue();

						wxBusyInfo info( _("Testing the connection to the node ...") );

						// 1: Check DICOM interface
						errorTitle = _Std("DICOM Error");

						if(m_pPuerto->GetValue().ToLong(&puerto)) {

							EchoAssociation as("C-ECHO");
							if (m_pUseSSL->GetValue()) {
								if (m_certificado.empty() || m_privateKey.empty()) {							
									as.SetTLS(m_configLocal.certificate, m_configLocal.privateKey, m_pVerifyServer->GetValue());
								}
								else {
									as.SetTLS(m_certificado, m_privateKey, m_pVerifyServer->GetValue());
								}
							}
							if (m_pUser->GetValue() != wxEmptyString) {
								as.SetUserPass(std::string(m_pUser->GetValue().ToUTF8()), std::string(m_pPassword->GetValue().ToUTF8()));
							}
							OFCondition cond;

							std::string aet(m_pAET->GetValue().ToUTF8());
							std::string host(m_pHost->GetValue().ToUTF8());

							as.Create(aet, host, puerto, m_aetLocal, UID_VerificationSOPClass);

							GIL::DICOM::DCMTK::Network* pNetwork = NULL;
							try {
								pNetwork = GIL::DICOM::DCMTK::Network::Instance(this);
							}
							catch(const std::exception& ex)
							{
								success = false;
								errorMsg << _Std("Unable to connect: Could not start network support.") << ":\n" << ex.what();
							}

							if(success) {
								as.SetTimeout(10);
								pNetwork->InitializeNetwork(as.GetTimeout());
								cond = pNetwork->ConnectAssociation(&as, pdu);
								if (cond.good()) {
									cond = as.SendEchoRequest();
									if (cond.bad())	{
										success = false;
										errorMsg << _Std("The service did not respond correctly to the C-ECHO") << ":\n" << cond.text();
									}	
								}
								else {
									success = false;
									errorMsg << _Std("The service did not respond correctly to the DICOM standard") << ":\n" << cond.text();
								}
								as.Drop(cond);
								GIL::DICOM::DCMTK::Network::FreeInstance(this);
								pNetwork = NULL;
							}
						}
						else {
							success = false;
							errorMsg << _Std("Invalid port number");
						}

						// 2: check if wado support is enabled.

						if (success && ( m_pRetrieveMethod->GetSelection() == (int)DicomServer::WADO ) ) {
							errorTitle = _Std("WADO Error");
							 std::string url(m_pWadoURL->GetValue().ToUTF8());
							 try {
								GNC::GCS::IEntorno::Instance()->GetPACSController()->TestWADOURL(url);
							 }
							 catch (const GIL::DICOM::PACSException& ex) {
								 success = false;
								 errorMsg << ex.GetCause();
							 }
						}
					}

					if (success) {
						std::string resp = _Std("Connection test successful");
						LOG_INFO("C-TEST", resp);
						wxMessageBox(wxString::FromUTF8(resp.c_str()), _("Info"), wxOK | wxICON_INFORMATION, this);
						
					}
					else {
						LOG_ERROR("C-TEST", errorTitle << ":" << errorMsg.str());
						wxMessageBox(wxString::FromUTF8(errorMsg.str().c_str()), wxString::FromUTF8(errorTitle.c_str()), wxICON_ERROR, this);
					}
				}

			}

			virtual void OnChecWL(wxCommandEvent &)
			{
				RefreshHL7Panel();
			}

			virtual void RefreshHL7Panel()
			{
				m_pPanelHL7->Enable(m_pCheckHL7->GetValue());
				m_pTextAbort->Enable(m_pCheckAbort->GetValue());
				m_pTextCompleted->Enable(m_pCheckCompleted->GetValue());
				m_pTextProgress->Enable(m_pCheckInProgress->GetValue());
				m_pPanelHL7->Refresh(true);
			}

			virtual void OnMessageDefaultValues(wxCommandEvent&)
			{
				LoadMessageDefaultValues();
			}

			virtual void LoadMessageDefaultValues()
			{
				GNC::GCS::Permisos::EstadoPermiso perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "inprogress");
				if (perm) {
					m_pTextProgress->SetValue(wxString::FromUTF8(perm.GetRawValue().c_str()));
				}
				perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "completed");
				if (perm) {
					m_pTextCompleted->SetValue(wxString::FromUTF8(perm.GetRawValue().c_str()));
				}
				perm = GNC::GCS::IControladorPermisos::Instance()->Get("gnkwl.messages", "abort");
				if (perm) {
					m_pTextAbort->SetValue(wxString::FromUTF8(perm.GetRawValue().c_str()));
				}
			}

			PanelConfiguracionPACS::TipoConfigLocal& m_configLocal;

			bool m_Ok;
			std::string m_aetLocal;
			long m_puertoLocal;
			std::string m_certificado;
			std::string m_privateKey;
			std::list<std::string> m_idsPacs;
			std::string m_defCharset;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////PanelConfiguracionPACS////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		PanelConfiguracionPACS::PanelConfiguracionPACS(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionPACSBase(pParent), IPasoConfiguracion(pDialogo)
		{
			m_pModel = new wxDataViewListStore();
			m_pListPacs->AssociateModel(m_pModel.get());
			m_pListPacs->AppendColumn( new wxDataViewColumn( _("Name"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), NAME_COLUMN_IDX, NAME_COLUMN_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListPacs->AppendColumn( new wxDataViewColumn( _("Use by default"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), USE_BY_DEFAULT_COLUMN_IDX, USE_BY_DEFAULT_COLUMN_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			Recargar();

			m_pHelpBitmap->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoHelp());

			if(!GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_tls")) {
				m_pBAdvanced->Show(false);
				m_pPACSLocal->Layout();
				this->Layout();
				this->Refresh(true);
			}

			m_pListPacs->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(PanelConfiguracionPACS::OnListaPACSChoice), NULL, this);
			m_pListPacs->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(PanelConfiguracionPACS::OnListaPACSDClick), NULL, this);
		}

		PanelConfiguracionPACS::~PanelConfiguracionPACS()
		{
			m_servidores.clear();
		}

		void PanelConfiguracionPACS::Recargar()
		{
			std::string strTmp;
			if (!GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/PACS/Local", "AET", strTmp, "GINKGO_%IP4")) {
				GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/PACS/Local", "AET", strTmp);
			}
			m_pAETLocal->SetValue(wxString::FromUTF8(strTmp.c_str()));
			m_pAETReal->SetLabel(wxString::FromUTF8( GNC::Entorno::Instance()->ParseAET(strTmp).substr(0, 16).c_str() ));

			GNC::GCS::Ptr<DicomServer> serverLocal = DicomServerList::Instance()->GetLocalServer();
			m_pPuertoLocal->SetValue(wxString::Format(wxT("%d"),serverLocal->GetPort()));

			wxTextValidator OnlyNum = wxTextValidator(wxFILTER_NUMERIC);
			m_pPuertoLocal->SetValidator(OnlyNum);

			m_configLocal.useTLS = serverLocal->GetUseTLS();

			m_configLocal.verifyCredentials = serverLocal->GetverifyCredentials();
			m_configLocal.privateKey = serverLocal->GetPrivateKey();
			m_configLocal.certificate = serverLocal->GetCertificate();

			wxVector<wxVariant> data;

			if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","remote_pacs")) {
				m_pPACSServers->Enable(true);

				m_pListPacs->DeleteAllItems();
				m_servidores.clear();
				
				DicomServerList::Instance()->Reload();
				DicomServerList::TServerList ListOfServers = DicomServerList::Instance()->GetServerList();
				
				wxVector<wxVariant> data;
				for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it)
				{
					DicomServerList::TServerList::value_type& tuple = (*it);
					data.clear();
					m_servidores.push_back(tuple);

					data.push_back(wxString::FromUTF8(tuple->ID.c_str()));
					if (tuple->isDefault) {
						data.push_back(_("Yes"));
					} else {
						data.push_back(_("No"));
					}
					m_pModel->AppendItem(data);
				}

				if(m_pModel->GetCount() > 0) {
					wxDataViewItem item = m_pModel->GetItem(0);
					if (item.IsOk()) {
						m_pListPacs->SelectRow(0);						
					}
				}
				wxDataViewEvent evt;
				OnListaPACSChoice(evt);
			}
			else {
				m_pPACSServers->Enable(false);
			}

		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* PanelConfiguracionPACS::GetPanel()
		{
			return this;
		}

		std::string PanelConfiguracionPACS::GetTitle()
		{
			return _Std("DICOM nodes");
		}

		std::string PanelConfiguracionPACS::GetCabecera()
		{
			return _Std("DICOM nodes settings");
		}

		bool PanelConfiguracionPACS::Validar()
		{
			bool ok = true;
			std::stringstream sstream;
			sstream << _Std("They have produced the following errors:");

			long numero;
			if(!m_pPuertoLocal->GetValue().ToLong(&numero) && m_pPuertoLocal->GetValue().size() > 0){
				ok = false;
				sstream << _Std("\n- The local port must be a positive short integer (16 bits)");
			} else{
				if(numero > 32767){//maxshort
					ok = false;
					sstream << _Std("\n- The local port is too large");
				}
			}

			if(m_pAETReal->GetLabel().size() >16) {
				ok = false;
				sstream << _Std("\n- Maximum length of local AET is 16 characters");
			}

			if(!ok)
				wxMessageBox(wxString::FromUTF8(sstream.str().c_str()),_("Info"),wxOK | wxICON_INFORMATION, this);
			return ok;
		}

		bool PanelConfiguracionPACS::Guardar()
		{
			//se actualiza el fichero de configuracion
			GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/PACS/Local","AET",std::string(m_pAETLocal->GetValue().ToUTF8()));
			GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/PACS/Local","Puerto",std::string(m_pPuertoLocal->GetValue().ToUTF8()));
			GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/PACS/Local","useTLS",m_configLocal.useTLS);
			GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/PACS/Local","verifyCredentials",m_configLocal.verifyCredentials);
			GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/PACS/Local","privateKey",m_configLocal.privateKey);
			GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/PACS/Local","certificate",m_configLocal.certificate);

			GIL::DICOM::PACSController::Instance()->StartServer();

			if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","remote_pacs")) {
				wxString clave;
				wxString valor;
				GNC::GCS::ConfigurationController::Instance()->deleteEntryGeneral("/GinkgoCore/PACS/Servidores");
				GNC::GCS::ConfigurationController::TListGroups pacsList;

				for(TipoListaPACS::iterator it = m_servidores.begin(); it!= m_servidores.end(); ++it){
					GNC::GCS::ConfigurationController::TMapValues pacs;
					pacs["Identificador"] = (*it)->ID;
					pacs["AET"] = (*it)->AET;
					pacs["Host"] = (*it)->HostName;
					pacs.insertInt("Puerto",(*it)->Port);
					pacs.insertInt("PDU",(*it)->PDU);
					pacs.insertBool("useTLS", (*it)->useTLS);
					pacs.insertBool("verifyCredentials", (*it)->verifyCredentials);
					pacs.insertInt("retrieveMethod", (*it)->retrieveMethod);
					pacs["wadoURI"] = (*it)->wadoURI;
					pacs.insertBool("reuseConnection", (*it)->reuseConnection);
					pacs.insertBool("retrieveSeries", (*it)->retrieveSeries);
					pacs["pacsUser"] = (*it)->pacsUser;
					pacs["pacsPass"] = (*it)->pacsPass;
					pacs["certificate"] = (*it)->certificate;
					pacs["privateKey"] = (*it)->privateKey;
					pacs.insertBool("isDefault", (*it)->isDefault);
					pacs["defaultCharset"] = (*it)->defaultCharset;
					//wl
					pacs.insertBool("useDICOMMPPS", (*it)->GetWlConfig().useDICOMMPPS);
					pacs.insertBool("useAbortMessage", (*it)->GetWlConfig().useAbortMessage);
					pacs.insertBool("useCompletedMessage", (*it)->GetWlConfig().useCompletedMessage);
					pacs.insertBool("useInProgressMessage", (*it)->GetWlConfig().useInProgressMessage);
					pacs["abortTemplate"] = (*it)->GetWlConfig().abortTemplate;
					pacs["completedTemplate"] = (*it)->GetWlConfig().completedTemplate;
					pacs["inProgressTemplate"] = (*it)->GetWlConfig().inProgressTemplate;
					//
					pacsList.push_back(pacs);
				}
				GNC::GCS::ConfigurationController::Instance()->writeGroupGeneral("/GinkgoCore/PACS/Servidores", pacsList, "Server");
				DicomServerList::Instance()->Reload();
			}
			else {
				DicomServerList::Instance()->Reload();
			}

			return true;
		}

		//endregion

		void PanelConfiguracionPACS::OnAdvancedLocal(wxCommandEvent &)
		{
			DialogoLocalPACS dlg(this);
			dlg.SetParametros(m_configLocal);
			dlg.ShowModal();
			if(dlg.m_Ok){
				dlg.GetParametros(m_configLocal);
				OnPropiedadCambiada();
			}
		}

		void PanelConfiguracionPACS::OnListaPACSChoice(wxDataViewEvent &){
			//se carga la descripcion

			bool enable = m_pListPacs->GetSelection().IsOk();
			m_pEditButton->Enable(enable);
			m_pDelButton->Enable(enable);
			m_pDefaultButton->Enable(enable);
		}

		void PanelConfiguracionPACS::OnListaPACSDClick(wxDataViewEvent &)
		{
			wxCommandEvent evt;
			OnEditarClick(evt);
		}

		void PanelConfiguracionPACS::OnNuevoClick(wxCommandEvent &){
			
			std::list<std::string> listIdsPacs;
			for (TipoListaPACS::iterator it = m_servidores.begin(); it != m_servidores.end(); ++it) {
				listIdsPacs.push_back((*it)->ID);
			}
			DialogoServidorPACS dialogo(this, m_configLocal, m_pPuertoLocal->GetValue(), wxString::FromUTF8( GNC::Entorno::Instance()->ParseAET( std::string(m_pAETLocal->GetValue().ToUTF8()) ).c_str() ), listIdsPacs );
			dialogo.ShowModal();
			if(dialogo.m_Ok){
				GNC::GCS::Ptr<DicomServer> tuple(new DicomServer());
				dialogo.GetParametros(*tuple);
				tuple->isDefault = true;
				if(m_pModel->GetCount() > 0) {
					wxDataViewItem item = m_pModel->GetItem(0);
					if (item.IsOk()) {
						tuple->isDefault = false;
					}
				}
				m_servidores.push_back(tuple);

				wxVector<wxVariant> data;

				data.push_back(wxString::FromUTF8(tuple->ID.c_str()));
				if (tuple->isDefault) {
					data.push_back(_("Yes"));
				} else {
					data.push_back(_("No"));
				}
				m_pModel->AppendItem(data);
				

				wxVariant val;
				bool notFound = true;
				for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
					wxDataViewItem item = m_pModel->GetItem(i);
					m_pModel->GetValueByRow(val, i, NAME_COLUMN_IDX);
					std::string stdVal(val.GetString().ToUTF8());
					if (stdVal == tuple->ID) {
						notFound = false;
						m_pListPacs->SelectRow(i);
					}
				}
				wxDataViewEvent evt;
				OnListaPACSChoice(evt);
				OnPropiedadCambiada();
			}
		}

		void PanelConfiguracionPACS::OnEditarClick(wxCommandEvent &){

			wxDataViewItem selection = m_pListPacs->GetSelection();

			if(selection.IsOk()){
				wxVariant val;
				m_pModel->GetValue(val, selection, NAME_COLUMN_IDX);
				std::string idSeleccionado(val.GetString().ToUTF8());

				std::list<std::string> listIdsPacs;
				for (TipoListaPACS::iterator it = m_servidores.begin(); it != m_servidores.end(); ++it) {
					if((*it)->ID != idSeleccionado){
						listIdsPacs.push_back((*it)->ID);
					}
				}

				for(TipoListaPACS::iterator it = m_servidores.begin(); it != m_servidores.end(); ++it){
					if((*it)->ID == idSeleccionado){
						DialogoServidorPACS dialogo(this, m_configLocal, m_pPuertoLocal->GetValue(), wxString::FromUTF8( GNC::Entorno::Instance()->ParseAET( std::string( m_pAETLocal->GetValue().ToUTF8() ) ).c_str() ), listIdsPacs);
						dialogo.SetParametros(*(*it));

						dialogo.ShowModal();
						if(dialogo.m_Ok){
							dialogo.GetParametros(*(*it));
							// Se refresca el valor
							val = wxString::FromUTF8((*it)->ID.c_str());
							m_pModel->SetValue(val, selection, NAME_COLUMN_IDX);
							m_pModel->ItemChanged(selection);

							wxVariant val2;
							bool notFound = true;
							for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
								m_pModel->GetValueByRow(val2, i, NAME_COLUMN_IDX);
								if (val == val2) {
									m_pListPacs->SelectRow(i);									
									break;
								}
							}
							
							wxDataViewEvent evt;
							OnListaPACSChoice(evt);
							OnPropiedadCambiada();
						}
					}
				}
			}
		}

		void PanelConfiguracionPACS::OnEliminarClick(wxCommandEvent &){

			wxDataViewItem selection =m_pListPacs->GetSelection();

			if(selection.IsOk()){
				wxVariant val;
				m_pModel->GetValue(val, selection, NAME_COLUMN_IDX);
				std::string idSeleccionado(val.GetString().ToUTF8());

				int answer = wxMessageBox(_("Confirm the removal of the node") + wxT(" ") + wxString::FromUTF8(idSeleccionado.c_str()), _("Data modified"), wxYES_NO | wxCANCEL, this);
				if(answer == wxYES){
					for(TipoListaPACS::iterator it = m_servidores.begin(); it != m_servidores.end(); ++it){
						if((*it)->ID == idSeleccionado){

							bool notFound = true;
							for (unsigned int i = 0; notFound && i < m_pModel->GetCount(); ++i) {
								wxDataViewItem item = m_pModel->GetItem(i);

								wxVariant val2;
								m_pModel->GetValue(val2, item, NAME_COLUMN_IDX);
								if (val == val2) {
									notFound = false;
									m_pModel->DeleteItem(i);
								}
							}
							m_servidores.erase(it);
							break;
						}
					}
					m_pListPacs->Refresh();
					wxDataViewEvent evt;
					OnListaPACSChoice(evt);
					OnPropiedadCambiada();
				}
			}
		}

		void PanelConfiguracionPACS::OnSetDefaultClick(wxCommandEvent &)
		{
			
			wxDataViewItem selection = m_pListPacs->GetSelection();
			if(selection.IsOk()){
				wxVariant val;
				m_pModel->GetValue(val, selection, NAME_COLUMN_IDX);
				std::string idSeleccionado(val.GetString().ToUTF8());

				for(TipoListaPACS::iterator it = m_servidores.begin(); it != m_servidores.end(); ++it){
					TipoListaPACS::value_type& item = (*it);
					if(item->ID == idSeleccionado){
						item->isDefault = true;
					} else {
						item->isDefault = false;
					}
				}

				wxDataViewItemArray items;
				
				for (unsigned int i = 0; i < m_pModel->GetCount(); ++i) {
					wxVariant val2;
					wxDataViewItem item = m_pModel->GetItem(i);
					m_pModel->GetValue(val2, item, NAME_COLUMN_IDX);
					
					if (val == val2) {
						m_pModel->SetValue(_("Yes"), item, USE_BY_DEFAULT_COLUMN_IDX);
					} else {
						m_pModel->SetValue(_("No"), item, USE_BY_DEFAULT_COLUMN_IDX);
					}
					items.Add(item);
				}
				m_pModel->ItemsChanged(items);

				wxDataViewEvent evt;
				OnListaPACSChoice(evt);
				OnPropiedadCambiada();
			}
			
		}

		void PanelConfiguracionPACS::OnTextoCambiado(wxCommandEvent & )
		{
			m_pAETReal->SetLabel(wxString::FromUTF8( GNC::Entorno::Instance()->ParseAET(std::string(m_pAETLocal->GetValue().ToUTF8())).c_str() ));
			OnPropiedadCambiada();
		}

		void PanelConfiguracionPACS::OnSize(wxSizeEvent & event){
			wxSize oldSize = m_staticTextExplicacion->GetSize();
			Freeze();
			m_staticTextExplicacion->Freeze();
			m_staticTextExplicacion->SetLabel(_("You must configure the local AET, which is used to communicate with the DICOM nodes. The specified port must be opened in the firewall, as it is essential for transfers."));
			m_staticTextExplicacion->Wrap( event.GetSize().x-25 );
			m_staticTextExplicacion->Thaw();
			//Layout();
			Thaw();
			wxSize newSize = m_staticTextExplicacion->GetSize();
			if (newSize != oldSize) {
				Layout();
				event.Skip(false);
			} else {
				event.Skip(true);
			}
		}
	}
}
