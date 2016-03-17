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
#include <wx/sstream.h>
#include <wx/msgdlg.h>
#ifndef _WIN32
#include <signal.h>
#endif
#include <wx/tokenzr.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/filename.h>


#include "parserxmlspanish.h"
#include <api/globals.h>
#include <api/imodelohl7.h>
#include <api/integration/dict.h>
#include <api/controllers/imodulecontroller.h>
#include <api/helpers/aetwildcards.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>
#include "dcmtk/dicomservers.h"

//creacion de la base de datos de mensajes
GIL::ParserXMLSpanish::ParserXMLSpanish()
{
}

GIL::ParserXMLSpanish::~ParserXMLSpanish()
{
}

//region "Interfaz generica"

std::list<std::string> GIL::ParserXMLSpanish::GetKeys()
{
        std::list<std::string> list;
        list.push_back("getcontextresult");
        list.push_back("plantilla");
        return list;
}

/* Parsea la cadena XML y construye el modelo de integracion. Los parametros no encontrados se asignan por defecto segun lo especificado en la configuracion global */
void GIL::ParserXMLSpanish::ParseIntegrationXML(GIL::IntegrationModelList& models, wxXmlNode* pRoot)
{
        MapaServers servers;
        // PreCargamos la lista de PACS de la configuracion
        DicomServerList::TServerList ListOfServers = DicomServerList::Instance()->GetServerList();
        for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it) {
                IModeloPACSServer pacs;
                pacs.sid = (*it)->ID;
                pacs.AET = (*it)->AET;
                pacs.hostname = (*it)->HostName;
                pacs.puerto = (*it)->Port;

                servers[pacs.sid] = pacs;
        }
        //
        if (pRoot->GetName().CmpNoCase(wxT("getcontextresult")) == 0) {
                wxString propVal;

                // Parseamos los PACS especificados por fichero
                for (wxXmlNode* nodo = pRoot->GetChildren(); nodo != NULL; nodo = nodo->GetNext()) {
                        if (nodo->GetName().CmpNoCase(wxT("pacs")) == 0) { // Configuracion de PACS
                                GIL::IModeloPACSServer pacsServer;

                                propVal = nodo->GetAttribute(wxT("sid"), wxEmptyString);
                                if( !propVal.empty() ) {
                                        pacsServer.sid = propVal.ToUTF8();
                                } else {
                                        throw IntegrationException(_Std("Omitted Identificator"), "CONF/pacs");
                                }

                                propVal = nodo->GetAttribute(wxT("aet"), wxEmptyString);
                                if( !propVal.empty() ) {
                                        pacsServer.AET = propVal.ToUTF8();
                                } else {
                                        throw IntegrationException(_Std("Omitted AET"), "CONF/pacs");
                                }

                                propVal = nodo->GetAttribute(wxT("hostname"), wxEmptyString);
                                if( !propVal.empty() ) {
                                        pacsServer.hostname = propVal.ToUTF8();
                                } else {
                                        throw IntegrationException(_Std("Hostname missing"), "CONF/pacs");
                                }

                                propVal = nodo->GetAttribute(wxT("puerto"), wxEmptyString);
                                if( !propVal.empty() ) {
                                        pacsServer.puerto = propVal.ToUTF8();
                                } else {
                                        throw IntegrationException(_Std("Omitted port"), "CONF/pacs");
                                }

                                propVal = nodo->GetAttribute(wxT("tls"), wxEmptyString);
                                if( propVal.CmpNoCase(wxT("yes")) == 0 || propVal.CmpNoCase(wxT("true")) == 0 || propVal.CmpNoCase(wxT("si")) == 0 || propVal.CmpNoCase(wxT("1")) == 0 ) {
                                        pacsServer.tls = true;
                                } else {
                                        pacsServer.tls = false;
                                }

                                propVal = nodo->GetAttribute(wxT("usuario-pacs"), wxEmptyString);
                                if( !propVal.empty()) {
                                        pacsServer.user = propVal.ToUTF8();
                                }

                                propVal = nodo->GetAttribute(wxT("password-pacs"), wxEmptyString);
                                if( !propVal.empty()) {
                                        pacsServer.password = propVal.ToUTF8();
                                }

                                propVal = nodo->GetAttribute(wxT("verify-credentials"), wxEmptyString);
                                if( propVal.CmpNoCase(wxT("yes")) == 0 || propVal.CmpNoCase(wxT("true")) == 0 || propVal.CmpNoCase(wxT("si")) == 0 || propVal.CmpNoCase(wxT("1")) == 0 ) {
                                        pacsServer.verify = true;
                                } else {
                                        pacsServer.verify = false;
                                }

                                propVal = nodo->GetAttribute(wxT("metodo"), wxEmptyString);
                                if( propVal.CmpNoCase(wxT("get")) == 0 ) {
                                        pacsServer.metodo = GIL::IModeloPACSServer::IMPS_GET;
                                } else {
                                        pacsServer.metodo = GIL::IModeloPACSServer::IMPS_MOVE;
                                }

                                propVal = nodo->GetAttribute(wxT("pdu"), wxEmptyString);
                                if( propVal.empty()) {
                                        pacsServer.pdu = "16384";
                                } else {
                                        pacsServer.pdu = propVal.ToUTF8();
                                }

                                propVal = nodo->GetAttribute(wxT("nivel-recuperacion"), wxEmptyString);
                                if( propVal.empty()) {
                                        pacsServer.retrieveLevel = "SERIES";
                                } else {
                                        pacsServer.retrieveLevel = propVal.ToUTF8();
                                }

                                for (wxXmlNode* subnodo = nodo->GetChildren(); subnodo != NULL; subnodo = subnodo->GetNext()) {
                                        if (subnodo->GetName().CmpNoCase(wxT("client-cert")) == 0) {
                                                pacsServer.cert = subnodo->GetNodeContent().ToUTF8();
                                        } else if (subnodo->GetName().CmpNoCase(wxT("client-key")) == 0) {
                                                pacsServer.key = subnodo->GetNodeContent().ToUTF8();
                                        }
                                }

                                servers[pacsServer.sid] = pacsServer;
                                try {
                                        GNC::GCS::Ptr<DicomServer> server = DicomServerList::Instance()->GetServer(pacsServer.sid);
                                        server->AET = pacsServer.AET;
                                        server->HostName = pacsServer.hostname;
                                        long puerto;
                                        long pdu;
                                        if(!wxString::FromUTF8(pacsServer.pdu.c_str()).ToLong(&pdu)) {
                                                throw IntegrationException(_Std("The pdu size is not a valid number"), "CONF/pacs");
                                        }
                                        server->PDU = pdu;
                                        if(wxString::FromUTF8(pacsServer.puerto.c_str()).ToLong(&puerto)) {
                                                server->Port = puerto;
                                        } else {
                                                throw IntegrationException(_Std("Port is not a valid number"), "CONF/pacs");
                                        }
                                        if (pacsServer.metodo == GIL::IModeloPACSServer::IMPS_GET) {
                                                server->retrieveMethod = DicomServer::GET;
                                        } else {
                                                server->retrieveMethod = DicomServer::MOVE;
                                        }
                                        {
                                                wxString strTmp = wxString::FromUTF8(pacsServer.retrieveLevel.c_str());
                                                server->retrieveSeries = strTmp.CmpNoCase(wxT("SERIES")) == 0;
                                        }
                                        server->useTLS = pacsServer.tls;
                                        server->verifyCredentials = pacsServer.verify;
                                        server->certificate = pacsServer.cert;
                                        server->privateKey  = pacsServer.key;
                                        server->pacsUser = pacsServer.user;
                                        server->pacsPass = pacsServer.password;

                                        DicomServerList::Instance()->SetDefaultServer(server->ID);

                                } catch (GinkgoNoServerFoundException& ) {
                                        long puerto;
                                        long pdu;
                                        if(!wxString::FromUTF8(pacsServer.pdu.c_str()).ToLong(&pdu)) {
                                                throw IntegrationException(_Std("The pdu size is not a valid number"), "CONF/pacs");
                                        }
                                        if(wxString::FromUTF8(pacsServer.puerto.c_str()).ToLong(&puerto)) {
                                                DicomServer::TRetrieveMethod retrieveMethod = DicomServer::MOVE;
                                                if ( pacsServer.metodo == GIL::IModeloPACSServer::IMPS_GET) {
                                                        retrieveMethod = DicomServer::GET;
                                                }
                                                DicomServerList::Instance()->AddServer( DicomServer(pacsServer.sid, pacsServer.AET, pacsServer.hostname, (int)puerto, 0, true, pdu, pacsServer.tls, pacsServer.user, pacsServer.password, retrieveMethod, "", wxString::FromUTF8(pacsServer.retrieveLevel.c_str()).Upper() == wxT("SERIES"), false, pacsServer.verify, pacsServer.cert, pacsServer.key, pacsServer.defaultCharset), true );
                                        } else {
                                                throw IntegrationException(_Std("The port is not a valid number"), "CONF/pacs");
                                        }
                                }
                        }
                }

                // Parseamos las plantillas
                for (wxXmlNode* nodo = pRoot->GetChildren(); nodo != NULL; nodo = nodo->GetNext()) {
                        if (nodo->GetName().CmpNoCase(wxT("plantilla")) == 0)  { // Plantilla
                                GIL::IModeloIntegracion* modelo = ParsePlantilla(nodo, servers);

                                models.push_back(modelo);
                        } //fin plantilla
                } //fin de parseo de plantillas
        } else if (pRoot->GetName().CmpNoCase(wxT("plantilla")) == 0) {
                GIL::IModeloIntegracion* modelo = ParsePlantilla(pRoot, servers);
                models.push_back(modelo);
        } else {
                throw IntegrationException(_Std("Setting not found"), "CONF");
        }
}

GIL::IModeloIntegracion* GIL::ParserXMLSpanish::ParsePlantilla(wxXmlNode* nodo, MapaServers& servers)
{
        std::string codigoAplicacion;
        //se extrae de la configuracion el codigo de aplicacion
        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","CodigoAplicacion", codigoAplicacion);

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

        propVal = nodo->GetAttribute(wxT("pid"), wxEmptyString);
        if( !propVal.empty() ) {
                modelo->idPlantilla = propVal.ToUTF8();
        } else {
                modelo->idPlantilla = "";
        }

        propVal = nodo->GetAttribute(wxT("accion"), wxEmptyString).Lower();
        if ( !propVal.empty() ) {

                if (propVal.CmpNoCase(wxT("obtener")) == 0) {
                        modelo->accion = GIL::IModeloIntegracion::TA_Obtener;
                } else if (propVal.CmpNoCase(wxT("dicomizar")) == 0 ) {
                        modelo->accion = GIL::IModeloIntegracion::TA_Dicomizar;
                } else if (propVal.CmpNoCase(wxT("imprimir")) == 0) {
                        modelo->accion = GIL::IModeloIntegracion::TA_Imprimir;
                } else {
                        throw IntegrationException(_Std("\"accion\" attribute invalid at \"plantilla\" scope" ), "CONF/template");
                }
        } else {
                throw IntegrationException(_Std("\"accion\" attribute expected at \"plantilla\" scope"), "CONF/template");
        }

        propVal = nodo->GetAttribute(wxT("id_peticion"), wxEmptyString);
        if( !propVal.empty() ) {
                modelo->GlobalVariables.InsertVariable(GKDI_HCE_PETITION_ID, std::string(propVal.ToUTF8()), "Id de peticion");
        }

        propVal = nodo->GetAttribute(wxT("ambito"), wxEmptyString);
        if( !propVal.empty() ) {
                modelo->GlobalVariables.InsertVariable(GKDI_JIMENA_AMBITO_PETICION, std::string(propVal.ToUTF8()), "Ambito de la peticion");
        }

        if( nodo->HasAttribute(wxT("pacs_retrieve_sid")) ) {
                propVal = nodo->GetAttribute(wxT("pacs_retrieve_sid"), wxEmptyString);
                if (!propVal.IsEmpty()) {
                        modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_RETRIEVE_SID, std::string(propVal.ToUTF8()), "String Id del pacs de obtencion");
                } else {
                        throw IntegrationException(_Std("\"pacs_retrieve_sid\" attribute empty at \"plantilla\" scope"), "CONF/template");
                }
        }

        if( nodo->HasAttribute(wxT("pacs_store_sid")) ) {
                propVal = nodo->GetAttribute(wxT("pacs_store_sid"), wxEmptyString);
                if (!propVal.IsEmpty()) {
                        modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_STORE_SID, std::string(propVal.ToUTF8()), "String Id del pacs de almacenamiento");
                } else {
                        throw IntegrationException(_Std("\"pacs_store_sid\" attribute empty at \"plantilla\" scope"),  "CONF/template");
                }
        }

        //recorremos los hijos de las plantillas
        for (wxXmlNode* hijo = nodo->GetChildren(); hijo != NULL; hijo = hijo->GetNext()) {

                if(hijo->GetName().CmpNoCase(wxT("dimse")) == 0) {
                        //parseamos los campos dimse
                        std::string uid;
                        propVal = hijo->GetAttribute(wxT("uid"), wxEmptyString);
                        if ( !propVal.empty() ) {
                                uid =  propVal.ToUTF8();
                        } else {
                                uid = "";
                                if(modelo->accion != GIL::IModeloIntegracion::TA_Dicomizar) {
                                        throw IntegrationException(_Std("\"uid\" attribute expected at \"dimse\" scope"),  "CONF/template");
                                }
                        }

                        propVal = hijo->GetAttribute(wxT("ambito"), wxEmptyString).Lower();
                        if ( !propVal.empty() ) {
                                if (propVal.CmpNoCase(wxT("study")) == 0) {
                                        modelo->TagsDICOMOverwrite.tags["0008|0052"] = "STUDY";
                                        //study instance uid
                                        modelo->TagsDICOMOverwrite.tags["0020|000d"] = uid;
                                } else if (propVal.CmpNoCase(wxT("series")) == 0) {
                                        modelo->TagsDICOMOverwrite.tags["0008|0052"] = "SERIES";
                                        //series instance uid
                                        modelo->TagsDICOMOverwrite.tags["0020|000e"] = uid;
                                } else if (propVal.CmpNoCase(wxT("image")) == 0) {
                                        modelo->TagsDICOMOverwrite.tags["0008|0052"] = "IMAGE";
                                        //sop instance
                                        modelo->TagsDICOMOverwrite.tags["0008|0018"] = uid;
                                        //sop instance uid
                                } else {
                                        throw IntegrationException(_Std("\"ambito\" attribute invalid at \"dimse\" scope"), "CONF/template");
                                }
                        } else {
                                modelo->TagsDICOMOverwrite.tags["0020|000d"] = "STUDY";
                                if(modelo->accion != GIL::IModeloIntegracion::TA_Dicomizar) {
                                        throw IntegrationException(_Std("\"ambito\" attribute expected at \"dimse\" scope"), "CONF/template");
                                }
                        }

                }

                //parseamos los campos d paciente
                if(hijo->GetName().CmpNoCase(wxT("paciente")) == 0) {
                        for (wxXmlNode* hijosPaciente = hijo->GetChildren(); hijosPaciente != NULL; hijosPaciente = hijosPaciente->GetNext()) {
                                if(hijosPaciente->GetName().CmpNoCase(wxT("nombre")) == 0) {
                                        if(hijosPaciente->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_NAME,  std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()), "Patient Name");
                                        }
                                } else if(hijosPaciente->GetName().CmpNoCase(wxT("apellido1")) == 0) {
                                        if(hijosPaciente->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_FAMILY_NAME,  std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()), "Patient Surname");
                                        }
                                } else if(hijosPaciente->GetName().CmpNoCase(wxT("apellido2")) == 0) {
                                        if(hijosPaciente->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_SECOND_FAMILY_NAME,  std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()), "Patient Second Surname");
                                        }
                                } else if(hijosPaciente->GetName().CmpNoCase(wxT("numero_episodio")) == 0) {
                                        if(hijosPaciente->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_HCE_EPISODE_NUMBER,  std::string(hijosPaciente->GetChildren()->GetContent().ToUTF8()), "Episode Number");
                                        }
                                } else if(hijosPaciente->GetName().CmpNoCase(wxT("id")) == 0) {
                                        std::string codigo(propVal.ToUTF8());
                                        std::string valor(propVal.ToUTF8());
                                        propVal = hijosPaciente->GetAttribute(wxT("codigo"), wxEmptyString);
                                        if ( !propVal.empty() ) {
                                                codigo =  propVal.ToUTF8();
                                        } else {
                                                throw IntegrationException(_Std("\"codigo\" attribute expected at \"paciente\" scope"), "CONF/template");
                                        }
                                        propVal = hijosPaciente->GetAttribute(wxT("valor"), wxEmptyString);
                                        if ( !propVal.empty() ) {
                                                valor =  propVal.ToUTF8();
                                                if (!modelo->GlobalVariables.Contains(GKDI_PATIENT_ID) || codigo=="PI") {
                                                        modelo->GlobalVariables.InsertVariable(GKDI_PATIENT_ID,  valor, "Patient Id");
                                                }
                                        }
                                }
                        }
                }//fin paciente

                //parseamos los campos d medico
                if(hijo->GetName().CmpNoCase(wxT("medico")) == 0) {
                        for (wxXmlNode* hijosMedico = hijo->GetChildren(); hijosMedico != NULL; hijosMedico = hijosMedico->GetNext()) {
                                if(hijosMedico->GetName().CmpNoCase(wxT("nombre")) == 0) {
                                        if(hijosMedico->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_NAME,  std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()), "Referring Physician Name");
                                        }
                                } else if(hijosMedico->GetName().CmpNoCase(wxT("apellido1")) == 0) {
                                        if(hijosMedico->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_FAMILY_NAME,  std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()), "Referring Physician Family Name");
                                        }
                                } else if(hijosMedico->GetName().CmpNoCase(wxT("apellido2")) == 0) {
                                        if(hijosMedico->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_SECOND_FAMILY_NAME,  std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()), "Referring Physician Second Family Name");
                                        }
                                } else if(hijosMedico->GetName().CmpNoCase(wxT("id")) == 0) {
                                        std::string codigo(propVal.ToUTF8());
                                        std::string valor(propVal.ToUTF8());
                                        propVal = hijosMedico->GetAttribute(wxT("codigo"), wxEmptyString);
                                        if ( !propVal.empty() ) {
                                                codigo =  propVal.ToUTF8();
                                        } else {
                                                throw IntegrationException(_Std("\"codigo\" attribute expected at \"medico\" scope"), "CONF/template");
                                        }
                                        propVal = hijosMedico->GetAttribute(wxT("valor"), wxEmptyString);
                                        if ( !propVal.empty() ) {
                                                valor =  propVal.ToUTF8();
                                        } else {
                                                throw IntegrationException(_Std("\"valor\" attribute expected at \"medico\" scope"), "CONF/template");
                                        }
                                        if (!modelo->GlobalVariables.Contains(GKDI_REFERRING_PHISICIAN_ID) || codigo=="NNESP") {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_ID,  valor, "Referring Physician Id");
                                        }
                                } else if(hijosMedico->GetName().CmpNoCase(wxT("centro")) == 0) {
                                        propVal = hijosMedico->GetAttribute(wxT("cid"), wxEmptyString);
                                        if ( !propVal.empty() ) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_INSTITUTION_ID, std::string(propVal.ToUTF8()));
                                        } else {
                                                throw IntegrationException(_Std("\"cid\" atribute expected at \"centro\" scope"), "CONF/template");
                                        }

                                        if(hijosMedico->GetChildren() != NULL) {
                                                modelo->GlobalVariables.InsertVariable(GKDI_REFERRING_PHISICIAN_INSTITUTION_NAME, std::string(hijosMedico->GetChildren()->GetContent().ToUTF8()));
                                        }
                                }
                        }
                }//fin medico

                if(hijo->GetName().CmpNoCase(wxT("hce")) == 0) {
                        propVal = hijo->GetAttribute(wxT("aid"), wxEmptyString);
                        if ( !propVal.empty() ) {
                                modelo->GlobalVariables.InsertVariable(GKDI_HCE_ID, std::string(propVal.ToUTF8()));
                        } else {
                                throw IntegrationException(_Std("\"aid\" attribute expected at \"hce\" scope"), "CONF/template");
                        }
                }

                if (hijo->GetName().CmpNoCase(wxT("metadato")) == 0) {
                        std::string codigo;
                        std::string clave;
                        std::string valor;
                        std::string descripcion;

                        propVal = hijo->GetAttribute(wxT("codigo"), wxEmptyString);
                        if ( !propVal.empty() ) {
                                codigo = propVal.Upper().ToUTF8();
                        } else {
                                throw IntegrationException(_Std("\"codigo\" attribute expected at \"metadato\" scope"), "CONF/template");
                        }

                        propVal = hijo->GetAttribute(wxT("clave"), wxEmptyString);
                        if ( !propVal.empty() ) {
                                clave = propVal.Lower().ToUTF8();
                        } else {
                                throw IntegrationException(_Std("\"clave\" attribute expected at \"metadato\" scope"), "CONF/template");
                        }

                        propVal = hijo->GetAttribute(wxT("valor"), wxEmptyString);

                        if ( !propVal.empty() ) {
                                valor = propVal.ToUTF8();
                        }

                        propVal = hijo->GetAttribute(wxT("descripcion"), wxEmptyString);
                        if ( !propVal.empty() ) {
                                descripcion = propVal.ToUTF8();
                        }

                        if (codigo == "DICOM") {
                                modelo->TagsDICOMOverwrite.tags[clave] = valor;
                        } else {
                                modelo->GlobalVariables.InsertVariable(clave, valor, descripcion);
                        }
                }

                if (hijo->GetName().CmpNoCase(wxT("observaciones")) == 0) {
                        if(hijo->GetChildren() != NULL) {
                                modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_OBSERVATIONS, std::string(hijo->GetChildren()->GetContent().ToUTF8()));
                        }
                }
        }//fin hijos plantilla
//insert tags dicom...
        if (modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar) {
                //solo se sobreescribe en dicomizacion
                if (!modelo->GlobalVariables.Contains(GKDI_HCE_PETITION_ID)) {
                        throw IntegrationException(_Std("\"id_peticion\" attribute expected at \"gnkworkflow\" scope"), "CONF/template");
                }
                if (!modelo->GlobalVariables.Contains(GKDI_JIMENA_AMBITO_PETICION)) {
                        throw IntegrationException(_Std("\"ambito\" attribute expected at \"plantilla\" scope"), "CONF/template");
                }
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
        } else if (modelo->accion == GIL::IModeloIntegracion::TA_Obtener) {
                if (modelo->TagsDICOMOverwrite.tags.empty()) {
                        throw IntegrationException(_Std("You must specify attributes you want to use in the query"), "CONF/template");
                }
        }
        if(!modelo->GlobalVariables.Contains(GKDI_HCE_ID)) {
                throw IntegrationException(_Std("id_application expected"), "CONF/template");
        }
        if (!modelo->GlobalVariables.Contains(GKDI_PATIENT_ID) && modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar) {
                throw IntegrationException(_Std("It has been read a \"patient\" tag without identifiers"), "CONF/template");
        }
        //se pone el codigo de aplicacion y el primer PACS de la configuracion si no nos ha venido  ninguno
        modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_APPLICATION_CODE, codigoAplicacion);
        if(servers.size() > 0) {
                GNC::GCS::Ptr<DicomServer> pServer = DicomServerList::Instance()->GetDefaultServer();
                if (!modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_STORE_SID) && pServer.IsValid()) {
                        modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_STORE_SID, pServer->ID);
                }
                if (!modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_RETRIEVE_SID) && pServer.IsValid()) {
                        modelo->GlobalVariables.InsertVariable(GKDI_GLOBAL_PACS_RETRIEVE_SID, pServer->ID);
                }
        }
        if ( (modelo->accion == GIL::IModeloIntegracion::TA_Dicomizar && !modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_STORE_SID)) ||
             (modelo->accion == GIL::IModeloIntegracion::TA_Obtener && !modelo->GlobalVariables.Contains(GKDI_GLOBAL_PACS_RETRIEVE_SID)) ) {
                throw IntegrationException(_Std("You must define a default PACS in the integration XML or in the settings menu"), "CONF/template");
        }
        return modelo;
}

