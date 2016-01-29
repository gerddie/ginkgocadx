/*
 *  
 *  $Id: controladorhl7.cpp $
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
//#define _GINKGO_TRACE

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
#include <wx/filename.h>

#include "integrationcontroller.h"
#include "controladorhl7.h"
#include "parserxmlspanish.h"
#include "parserxmlenglish.h"

#include "controladorbbddhl7.h"
#include <api/globals.h>
#include <api/imodelohl7.h>
#include <api/controllers/imodulecontroller.h>
#include <api/helpers/aetwildcards.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>
#include "dcmtk/dicomservers.h"

//creacion de la base de datos de mensajes

GIL::HL7::ControladorHL7::ControladorHL7(TipoProtocolo protocolo)
{
	
	m_Protocolo = protocolo;
}

GIL::HL7::ControladorHL7::~ControladorHL7()
{

}


//region "Interfaz generica"
/* Envia el mensaje por MLLP */
void GIL::HL7::ControladorHL7::EnviarMensaje(const GIL::HL7::Message& msg, bool procesarACK) const
{
	std::string hostname, port;

	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","HostnameMI",hostname);
	GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/HCE","PuertoMI",port);
	std::ostringstream ostr;
	ostr << hostname << ":" << port;
	std::string url = ostr.str();

	if (hostname.size() == 0) {
		throw HL7Exception(_Std("Integration engine hostname not set"), "NET");
	}
	if (port.size() == 0) {
		throw HL7Exception(_Std("Integration engine port not set"), "NET");
	}

	std::string msgControlId("");
	switch(m_Protocolo) {
		case GIL::HL7::ControladorHL7::TP_MLLP:
			//se inserta en la BBDD de mensajes
			if(procesarACK){
				msgControlId = msg.GetSegment("MSH").GetField(10).GetComponent(1);
			}
			InsertarMensajeBBDD(msg,url,procesarACK,(int)TP_MLLP,msgControlId);
			//EnviarMensajeMLLP(msg, url, procesarACK);
			break;
		default:
			throw HL7Exception(_Std("Unsupported protocol"), "NET");
	}
}

//endregion
void GIL::HL7::ControladorHL7::InsertarMensajeBBDD(const GIL::HL7::Message& msg, const std::string& url,const bool procesarACK,const int protocolo,const std::string& msgControlId) const
{
	std::stringstream outputStream;
	outputStream << msg;
	std::string str = outputStream.str();

	try{
		GIL::HL7::MensajeHL7 mensaje(0,str,"",url,GIL::HL7::MensajeHL7::TE_Pendiente,"",protocolo,procesarACK,msgControlId);
		GIL::HL7::ControladorBBDDHl7::Instance()->InsertarMensaje(mensaje);
	}catch (GIL::HL7::HL7Exception& ) {
		throw HL7Exception(_Std("Error queueing message. Check your supplier"), "NET");
	}
}
