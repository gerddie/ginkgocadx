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


#include <string>
#include <map>
#include <ctime>
#include <sstream>

#include <wx/xml/xml.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/sstream.h>
#include <wx/msgdlg.h>
#ifndef _WIN32
#include <signal.h>
#endif
#include <wx/tokenzr.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/filename.h>


#include "parserxmlenglish.h"
#include <api/globals.h>
#include <api/imodelohl7.h>
#include <api/controllers/imodulecontroller.h>
#include <api/integration/dict.h>
#include <api/helpers/aetwildcards.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>
#include "dcmtk/dicomservers.h"

//creacion de la base de datos de mensajes
GIL::ParserXMLEnglish::ParserXMLEnglish()
{
}

GIL::ParserXMLEnglish::~ParserXMLEnglish()
{
}

//region "Interfaz generica"

std::list<std::string> GIL::ParserXMLEnglish::GetKeys()
{
	std::list<std::string> list;
	list.push_back("gnkworkflows");
	list.push_back("gnkworkflow");
	return list;
}

/* Parsea la cadena XML y construye el modelo de integracion. Los parametros no encontrados se asignan por defecto segun lo especificado en la configuracion global */
void GIL::ParserXMLEnglish::ParseIntegrationXML(GIL::IntegrationModelList& models, wxXmlNode* pRoot) 
{
	MapaServers servers;
	std::string xpp; // XML Pretty Printed for extended information info.
	// PreCargamos la lista de PACS de la configuracion
	DicomServerList::TServerList ListOfServers = DicomServerList::Instance()->GetServerList();
	for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it)
	{
		IModeloPACSServer pacs;
		pacs.sid = (*it)->ID;
		pacs.AET = (*it)->AET;
		pacs.hostname = (*it)->HostName;
		pacs.puerto = (*it)->Port;

		servers[pacs.sid] = pacs;
	}
	//	

	if (pRoot->GetName().CmpNoCase(wxT("gnkworkflows")) == 0) {

		wxString propVal;

		// Parseamos los PACS especificados por fichero
		for (wxXmlNode* nodo = pRoot->GetChildren(); nodo != NULL; nodo = nodo->GetNext()) {
			if (nodo->GetName().CmpNoCase(wxT("pacs")) == 0) { // Configuracion de PACS
				GIL::IModeloPACSServer pacsServer;

				propVal = nodo->GetAttribute(wxT("sid"), wxEmptyString);
				if( !propVal.empty() ) {
					pacsServer.sid = propVal.ToUTF8();
				}
				else {
					throw IntegrationException(_Std("Omitted Identificator"), xpp, "CONF/pacs");
				}

				propVal = nodo->GetAttribute(wxT("aet"), wxEmptyString);
				if( !propVal.empty() ) {
					pacsServer.AET = propVal.ToUTF8();
				}
				else {
					throw IntegrationException(_Std("Omitted AET"), xpp, "CONF/pacs");
				}

				propVal = nodo->GetAttribute(wxT("hostname"), wxEmptyString);
				if( !propVal.empty() ) {
					pacsServer.hostname = propVal.ToUTF8();
				}
				else {
					throw IntegrationException(_Std("Hostname missing"), xpp, "CONF/pacs");
				}

				propVal = nodo->GetAttribute(wxT("port"), wxEmptyString);
				if( !propVal.empty() ) {
					pacsServer.puerto = propVal.ToUTF8();
				}
				else {
					throw IntegrationException(_Std("Omitted port"), xpp, "CONF/pacs");
				}
				
				propVal = nodo->GetAttribute(wxT("tls"), wxEmptyString);
				if( propVal.CmpNoCase(wxT("yes")) == 0 || propVal.CmpNoCase(wxT("true")) == 0 || propVal.CmpNoCase(wxT("si")) == 0 || propVal.CmpNoCase(wxT("1")) == 0 ) {
					pacsServer.tls = true;
				}
				else {
					pacsServer.tls = false;
				}

				propVal = nodo->GetAttribute(wxT("pacs-user"), wxEmptyString);
				if( !propVal.empty()) {
					pacsServer.user = propVal.ToUTF8();
				}

				propVal = nodo->GetAttribute(wxT("pacs-password"), wxEmptyString);
				if( !propVal.empty()) {
					pacsServer.password = propVal.ToUTF8();
				}

				propVal = nodo->GetAttribute(wxT("wado-url"), wxEmptyString);
				if( !propVal.empty()) {
					pacsServer.wadoURI = propVal.ToUTF8();
				}

				propVal = nodo->GetAttribute(wxT("verify-credentials"), wxEmptyString);
				if( propVal.CmpNoCase(wxT("yes")) == 0 || propVal.CmpNoCase(wxT("true")) == 0 || propVal.CmpNoCase(wxT("si")) == 0 || propVal.CmpNoCase(wxT("1")) == 0 ) {
					pacsServer.verify = true;
				}
				else {
					pacsServer.verify = false;
				}

				propVal = nodo->GetAttribute(wxT("retrieve-method"), wxEmptyString);
				if( propVal.CmpNoCase(wxT("get")) == 0 ) {
					pacsServer.metodo = GIL::IModeloPACSServer::IMPS_GET;
				}
				else if (propVal.CmpNoCase(wxT("wado")) == 0) {
					pacsServer.metodo = GIL::IModeloPACSServer::IMPS_WADO;
				} else {
					pacsServer.metodo = GIL::IModeloPACSServer::IMPS_MOVE;
				}
				
				propVal = nodo->GetAttribute(wxT("pdu"), wxEmptyString);
				if( propVal.empty()) {
					pacsServer.pdu = "16384";
				}
				else {
					pacsServer.pdu = propVal.ToUTF8();
				}

				propVal = nodo->GetAttribute(wxT("retrieve-level"), wxEmptyString);
				if( propVal.empty()) {
					pacsServer.retrieveLevel = "SERIES";
				}
				else {
					pacsServer.retrieveLevel = propVal.ToUTF8();
				}

				propVal = nodo->GetAttribute(wxT("charset"), wxEmptyString);
				if( !propVal.empty() ) {
					pacsServer.defaultCharset = propVal.ToUTF8();
				}
				else {
					pacsServer.defaultCharset = "ISO_IR 100";
				}

				for (wxXmlNode* subnodo = nodo->GetChildren(); subnodo != NULL; subnodo = subnodo->GetNext()) {
					if (subnodo->GetName().CmpNoCase(wxT("client-cert")) == 0) {
						pacsServer.cert = subnodo->GetNodeContent().ToUTF8();
					}
					else if (subnodo->GetName().CmpNoCase(wxT("client-key")) == 0) {
						pacsServer.key = subnodo->GetNodeContent().ToUTF8();
					}
				}

				servers[pacsServer.sid] = pacsServer;
				try{
					GNC::GCS::Ptr<DicomServer> server = DicomServerList::Instance()->GetServer(pacsServer.sid);
					server->AET = pacsServer.AET;
					server->HostName = pacsServer.hostname;
					long puerto;
					long pdu;
					if(!wxString::FromUTF8(pacsServer.pdu.c_str()).ToLong(&pdu)) {
						throw IntegrationException(_Std("The pdu size is not a valid number"), xpp, "CONF/pacs");
					}
					server->PDU = pdu;
					if(wxString::FromUTF8(pacsServer.puerto.c_str()).ToLong(&puerto)) {
						server->Port = puerto;
					} else {
						throw IntegrationException(_Std("Port is not a valid number"), xpp, "CONF/pacs");
					}
					if (pacsServer.metodo == GIL::IModeloPACSServer::IMPS_GET) {
						server->retrieveMethod = DicomServer::GET;
					}
					else if (pacsServer.metodo == GIL::IModeloPACSServer::IMPS_WADO) {
						server->retrieveMethod = DicomServer::WADO;
					} else {
						server->retrieveMethod = DicomServer::MOVE;
					}
					server->retrieveSeries = wxString::FromUTF8(pacsServer.retrieveLevel.c_str()).CmpNoCase(wxT("SERIES")) == 0;
					server->useTLS = pacsServer.tls;
					server->verifyCredentials = pacsServer.verify;
					server->certificate = pacsServer.cert;
					server->privateKey  = pacsServer.key;
					server->pacsUser = pacsServer.user;
					server->pacsPass = pacsServer.password;
					server->wadoURI = pacsServer.wadoURI;

					DicomServerList::Instance()->SetDefaultServer(server->ID);

				} catch (GinkgoNoServerFoundException& ){
					long puerto;
					long pdu;
					if(!wxString::FromUTF8(pacsServer.pdu.c_str()).ToLong(&pdu)) {
						throw IntegrationException(_Std("The pdu size is not a valid number"), xpp, "CONF/pacs");
					}
					if(wxString::FromUTF8(pacsServer.puerto.c_str()).ToLong(&puerto)) {
						DicomServer::TRetrieveMethod retrieveMethod = DicomServer::MOVE;
						if ( pacsServer.metodo == GIL::IModeloPACSServer::IMPS_GET) {
							retrieveMethod = DicomServer::GET;
						} else if (pacsServer.metodo == GIL::IModeloPACSServer::IMPS_WADO) {
							retrieveMethod = DicomServer::WADO;
						} 
						DicomServerList::Instance()->AddServer( DicomServer(pacsServer.sid, pacsServer.AET, pacsServer.hostname, (int)puerto, 0, true, pdu, pacsServer.tls, pacsServer.user, pacsServer.password, retrieveMethod, pacsServer.wadoURI, wxString::FromUTF8(pacsServer.retrieveLevel.c_str()).Upper() == wxT("SERIES"), false, pacsServer.verify, pacsServer.cert, pacsServer.key, pacsServer.defaultCharset), true );
					} else {
						throw IntegrationException(_Std("The port is not a valid number"), xpp, "CONF/pacs");
					}
				}
			}
		}

		// Parseamos las gnkworkflows
		for (wxXmlNode* nodo = pRoot->GetChildren(); nodo != NULL; nodo = nodo->GetNext()) {
			if (nodo->GetName().CmpNoCase(wxT("gnkworkflow") ) == 0)  { // gnkworkflow
				GIL::IModeloIntegracion* modelo = ParseGnkWorkflow(nodo, servers);
				models.push_back(modelo);
			} //fin gnkworkflow
		} //fin de parseo de gnkworkflows
	} else if (pRoot->GetName().CmpNoCase(wxT("gnkworkflow")) == 0) {
		GIL::IModeloIntegracion* modelo = ParseGnkWorkflow(pRoot, servers);
		models.push_back(modelo);
	} else {
		throw IntegrationException(_Std("Setting not found"), xpp, "CONF");
	}
}

GIL::IModeloIntegracion* GIL::ParserXMLEnglish::ParseGnkWorkflow(wxXmlNode* nodo, GIL::ParserXMLEnglish::MapaServers& servers)
{	
	std::string xpp; // XML Pretty Printed for extended information info.
	wxString propVal;
	GIL::IModeloIntegracion* modelo = new IModeloIntegracion();
	//se copia el raw data
	{
		wxXmlNode* pNodo = new wxXmlNode(*nodo);
		wxXmlDocument docTmp;
		docTmp.SetRoot(pNodo);
		wxStringOutputStream out;
		docTmp.Save(out,wxXML_NO_INDENTATION);
		modelo->rawXmlData = out.GetString().ToUTF8();
	}
	//

	propVal = nodo->GetAttribute(wxT("tid"), wxEmptyString);
	if( !propVal.empty() ) {
		modelo->idPlantilla = propVal.ToUTF8();
	} else {
		modelo->idPlantilla = "";
	}

	propVal = nodo->GetAttribute(wxT("action"), wxEmptyString).Lower();
	if ( !propVal.empty() ) {

		if (propVal.CmpNoCase(wxT("retrieve")) == 0){
			modelo->accion = GIL::IModeloIntegracion::TA_Obtener;
		}
		else if (propVal.CmpNoCase(wxT("dicomize")) == 0 ) {
			modelo->accion = GIL::IModeloIntegracion::TA_Dicomizar;
		}
		else if (propVal.CmpNoCase(wxT("print")) == 0) {
			modelo->accion = GIL::IModeloIntegracion::TA_Imprimir;
		}
		else if (propVal.CmpNoCase(wxT("open")) == 0) {
			modelo->accion = GIL::IModeloIntegracion::TA_Open;
		}
		else {
			throw IntegrationException(_Std("\"action\" attribute invalid at \"gnkworkflow\" scope" ), xpp, "CONF/template");
		}
	}
	else {
		throw IntegrationException(_Std("\"action\" attribute expected at \"gnkworkflow\" scope"), xpp, "CONF/template");
	}

	propVal = nodo->GetAttribute(wxT("petition-id"), wxEmptyString);
	if( !propVal.empty() ) {
		modelo->GlobalVariables.InsertVariable(GKDI_HCE_PETITION_ID, std::string(propVal.ToUTF8()));
	}

	propVal = nodo->GetAttribute(wxT("scope"), wxEmptyString);
	if( !propVal.empty() ) {
		modelo->GlobalVariables.InsertVariable(GKDI_JIMENA_AMBITO_PETICION,std::string(propVal.ToUTF8()));
	}

	if( nodo->HasAttribute(wxT("pacs-retrieve-sid")) ) {
		propVal = nodo->GetAttribute(wxT("pacs-retrieve-sid"), wxEmptyString);
		if (!propVal.IsEmpty()) {
			modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_RETRIEVE_SID, std::string(propVal.ToUTF8()));
		}	
		else {
			throw IntegrationException(_Std("\"pacs-retrieve-sid\" attribute empty at \"gnkworkflow\" scope"), xpp, "CONF/template");
		}		
	}

	if( nodo->HasAttribute(wxT("pacs-store-sid")) ) {
		propVal = nodo->GetAttribute(wxT("pacs-store-sid"), wxEmptyString);
		if (!propVal.IsEmpty()) {
			modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_STORE_SID, std::string(propVal.ToUTF8()));
		}	
		else {
			throw IntegrationException(_Std("\"pacs-store-sid\" attribute empty at \"gnkworkflow\" scope"), xpp, "CONF/template");
		}		
	}

	modelo->GlobalVariables.InsertVariable(GKDI_HCE_ID, "defaultHCE");

	//recorremos los hijos de las gnkworkflows
	for (wxXmlNode* hijo = nodo->GetChildren(); hijo != NULL; hijo = hijo->GetNext()) {
		//files...
		if (hijo->GetName().CmpNoCase(wxT("file")) == 0 && hijo->GetChildren() != NULL){
			modelo->Files.push_back(std::string(hijo->GetChildren()->GetContent().ToUTF8()));
		}
		//parseamos los campos dimse
		if(hijo->GetName().CmpNoCase(wxT("query-retrieve-level")) == 0){
			propVal = hijo->GetAttribute(wxT("value"), wxEmptyString);
			if ( !propVal.empty() ) {
				modelo->TagsDICOMOverwrite.tags["0008|0052"] = std::string(propVal.Upper().ToUTF8());
			}
			else {
				throw IntegrationException(_Std("\"value\" attribute expected at \"query-retrieve-level\" scope"), xpp, "CONF/template");
			}
		}

		//parseamos los campos d paciente
		if(hijo->GetName().CmpNoCase(wxT("patient")) == 0) {
			for (wxXmlNode* hijosPaciente = hijo->GetChildren(); hijosPaciente != NULL; hijosPaciente = hijosPaciente->GetNext()) {
				if(hijosPaciente->GetName().CmpNoCase(wxT("name")) == 0) {
					if(hijosPaciente->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_NAME, std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosPaciente->GetName().CmpNoCase(wxT("first-surname")) == 0) {
					if(hijosPaciente->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_FAMILY_NAME, std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosPaciente->GetName().CmpNoCase(wxT("second-surname")) == 0) {
					if(hijosPaciente->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_SECOND_FAMILY_NAME, std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosPaciente->GetName().CmpNoCase(wxT("episode-number")) == 0) {
					if(hijosPaciente->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_HCE_EPISODE_NUMBER, std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()));
					}
				}else if(hijosPaciente->GetName().CmpNoCase(wxT("id")) == 0) {
					std::string codigo(propVal.ToUTF8());
					std::string valor(propVal.ToUTF8());
					propVal = hijosPaciente->GetAttribute(wxT("code"), wxEmptyString);
					if ( !propVal.empty() ) {
						codigo =  propVal.ToUTF8();
					}
					else {
						throw IntegrationException(_Std("\"code\" attribute expected at \"patient\" scope"), xpp, "CONF/template");
					}
					propVal = hijosPaciente->GetAttribute(wxT("value"), wxEmptyString);
					if ( !propVal.empty() ) {
						valor =  propVal.ToUTF8();
						if (!modelo->GlobalVariables.Contains(GKDI_PATIENT_ID)) {
							modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_ID, valor);
						}
					}								
				}
			}
		}//fin paciente

		//parseamos los campos d medico
		if(hijo->GetName().CmpNoCase(wxT("physician")) == 0) {
			for (wxXmlNode* hijosMedico = hijo->GetChildren(); hijosMedico != NULL; hijosMedico = hijosMedico->GetNext()) {
				if(hijosMedico->GetName().CmpNoCase(wxT("name")) == 0) {
					if(hijosMedico->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_NAME, std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosMedico->GetName().CmpNoCase(wxT("first-surname")) == 0) {
					if(hijosMedico->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_FAMILY_NAME, std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosMedico->GetName().CmpNoCase(wxT("second-surname")) == 0) {
					if(hijosMedico->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_SECOND_FAMILY_NAME, std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()));
					}
				} else if(hijosMedico->GetName().CmpNoCase(wxT("id")) == 0) {
					std::string codigo(propVal.ToUTF8());
					std::string valor(propVal.ToUTF8());
					propVal = hijosMedico->GetAttribute(wxT("code"), wxEmptyString);
					if ( !propVal.empty() ) {
						codigo =  propVal.ToUTF8();
					}
					else {
						throw IntegrationException(_Std("\"code\" attribute expected at \"physician\" scope"), xpp, "CONF/template");
					}
					propVal = hijosMedico->GetAttribute(wxT("value"), wxEmptyString);
					if ( !propVal.empty() ) {
						valor =  propVal.ToUTF8();
					}
					else {
						throw IntegrationException(_Std("\"value\" attribute expected at \"physician\" scope"), xpp, "CONF/template");
					}
					if (!modelo->GlobalVariables.Contains(GKDI_REFERRING_PHISICIAN_ID)) {
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_ID, valor);
					}
				} else if(hijosMedico->GetName() == wxT("institution")) {
					propVal = hijosMedico->GetAttribute(wxT("iid"), wxEmptyString);
					if ( !propVal.empty() ) {
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_INSTITUTION_ID, std::string(propVal.ToUTF8()));
					}
					else {
						throw IntegrationException(_Std("\"iid\" atribute expected at \"institution\" scope"), xpp, "CONF/template");
					}

					if(hijosMedico->GetChildren() != NULL){
						modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_INSTITUTION_NAME, std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()));
					}
				}
			}
		}//fin medico

		if(hijo->GetName().CmpNoCase(wxT("hce")) == 0) {
			propVal = hijo->GetAttribute(wxT("aid"), wxEmptyString);
			if ( !propVal.empty() ) {
				modelo->GlobalVariables.InsertVariable(GKDI_HCE_ID, std::string(propVal.ToUTF8()));
			}
			else {
				LOG_DEBUG("ParserXMLEnglish","application id unespecified, using \"defaultHCE\"");
				modelo->GlobalVariables.InsertVariable(GKDI_HCE_ID, "defaultHCE");
			}
		} 

		if (hijo->GetName().CmpNoCase(wxT("metadata")) == 0) {
			std::string codigo;
			std::string clave;
			std::string valor;
			std::string descripcion;

			propVal = hijo->GetAttribute(wxT("code"), wxEmptyString);
			if ( !propVal.empty() ) {
				codigo = propVal.Upper().ToUTF8();
			}else {
				throw IntegrationException(_Std("\"code\" attribute expected at \"metadata\" scope"), xpp, "CONF/template");
			}

			propVal = hijo->GetAttribute(wxT("key"), wxEmptyString);
			if ( !propVal.empty() ) {
				clave = propVal.Lower().ToUTF8();
			}else {
				throw IntegrationException(_Std("\"key\" attribute expected at \"metadata\" scope"), xpp, "CONF/template");
			}

			propVal = hijo->GetAttribute(wxT("value"), wxEmptyString);
			if ( !propVal.empty() ) {
				valor = propVal.ToUTF8();
			}

			propVal = hijo->GetAttribute(wxT("description"), wxEmptyString);
			if ( !propVal.empty() ) {
				descripcion = propVal.ToUTF8();
			}

			if (codigo == "DICOM") {
				modelo->TagsDICOMOverwrite.tags[clave] = valor;
			} else {
				modelo->GlobalVariables.InsertVariable(clave, valor, descripcion);
			}
		}

		if (hijo->GetName().CmpNoCase(wxT("observations")) == 0) {
			if(hijo->GetChildren() != NULL) {
				modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_OBSERVATIONS, std::string(hijo->GetChildren()->GetContent().ToUTF8()));
			}
		}

		if (hijo->GetName().CmpNoCase(wxT("message_template")) == 0) {
			if(hijo->GetChildren() != NULL) {
				modelo->messageTemplate = std::string(hijo->GetChildren()->GetContent().ToUTF8());
			}
		}
	}//fin hijos gnkworkflow

	//insert tags dicom...
	if (modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar) {
		//only overwrite en dicomize no en obtain....
		if (modelo->GlobalVariables.Contains(GKDI_PATIENT_ID)) {
			modelo->TagsDICOMOverwrite.tags["0010|0020"] = modelo->GlobalVariables.GetValue(GKDI_PATIENT_ID);
		}
		if (modelo->GlobalVariables.Contains(GKDI_PATIENT_NAME)) {
			std::ostringstream ostr;
			ostr << modelo->GlobalVariables.GetValue(GKDI_PATIENT_FAMILY_NAME);
			if (modelo->GlobalVariables.Contains(GKDI_PATIENT_SECOND_FAMILY_NAME)) {
				ostr << " " << modelo->GlobalVariables.GetValue(GKDI_PATIENT_SECOND_FAMILY_NAME);
			}
			ostr << "^" << modelo->GlobalVariables.GetValue(GKDI_PATIENT_NAME);
			modelo->TagsDICOMOverwrite.tags["0010|0010"] = ostr.str();
		}
		if (modelo->GlobalVariables.Contains(GKDI_REFERRING_PHISICIAN_NAME)) {
			std::ostringstream ostr;
			ostr << modelo->GlobalVariables.GetValue(GKDI_REFERRING_PHISICIAN_FAMILY_NAME);
			if (modelo->GlobalVariables.Contains(GKDI_REFERRING_PHISICIAN_SECOND_FAMILY_NAME)) {
				ostr << " " << modelo->GlobalVariables.GetValue(GKDI_REFERRING_PHISICIAN_SECOND_FAMILY_NAME);
			}
			ostr << "^" << modelo->GlobalVariables.GetValue(GKDI_REFERRING_PHISICIAN_NAME);
			modelo->TagsDICOMOverwrite.tags["0008|0090"] = ostr.str();
		}
		if (modelo->GlobalVariables.Contains(GKDI_REFERRING_PHISICIAN_INSTITUTION_NAME)) {
			modelo->TagsDICOMOverwrite.tags["0008|0080"] = modelo->GlobalVariables.GetValue(GKDI_REFERRING_PHISICIAN_INSTITUTION_NAME);
		}
		if (!modelo->GlobalVariables.Contains(GKDI_HCE_PETITION_ID)) {
			LOG_DEBUG("ParserXMLEnglish", "\"petition-id\" attribute not specified");
		}
	} else if (modelo->accion == GIL::IModeloIntegracion::TA_Obtener) {
		if (modelo->TagsDICOMOverwrite.tags.empty() && modelo->Files.empty()) {
			throw IntegrationException(_Std("You must specify attributes you want to use in the query"), xpp, "CONF/template");
		}
	}
	if (!modelo->GlobalVariables.Contains(GKDI_PATIENT_ID) && modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar) {
		throw IntegrationException(_Std("It has been read a \"patient\" tag without identifiers"), xpp, "CONF/template");
	}
	//se pone el codigo de aplicacion y el primer PACS de la configuracion si no nos ha venido  ninguno
	std::string codigoAplicacion;
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","CodigoAplicacion", codigoAplicacion);
	modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_APPLICATION_CODE, codigoAplicacion);
	if(!servers.empty()) {
		GNC::GCS::Ptr<DicomServer> pServer = DicomServerList::Instance()->GetDefaultServer();
		if (!modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_STORE_SID) && pServer.IsValid()) {
			modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_STORE_SID, pServer->ID);
		}
		if (!modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_RETRIEVE_SID) && pServer.IsValid()) {
			modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_RETRIEVE_SID, pServer->ID);
		}
	}
	if ( (modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar && !modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_STORE_SID)) || 
		(modelo->accion == GIL::IModeloIntegracion::TA_Obtener && !modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_RETRIEVE_SID)) ) 
	{
		throw IntegrationException(_Std("You must define a default PACS in the integration XML or in the settings menu"), xpp, "CONF/template");
	}
	return modelo;
}
