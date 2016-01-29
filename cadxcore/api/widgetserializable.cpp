/*
 *  
 *  $Id: widgetserializable.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "widgetserializable.h"

#include <api/internationalization/internationalization.h>

#include <sstream>
#include <wx/string.h>
#include <wx/xml/xml.h>


inline const std::string GetFormattedTimet(time_t fecha)
	{
			struct tm tminfo;
			char fechaString[100];

			#ifdef _WIN32
				localtime_s(&tminfo,&fecha);
			#else
				localtime_r(&fecha,&tminfo);
			#endif

			strftime(fechaString,100,"%d/%m/%Y, %H:%M:%S",&tminfo);
			return fechaString;
	}



GNC::GCS::Widgets::IWidgetSerializable::IWidgetSerializable()
{
	time ( &m_timestampCreacion );
	ActualizarTimestampModificacion();
}

GNC::GCS::Widgets::IWidgetSerializable::IWidgetSerializable(wxXmlNode* nodo)
{
	LeerMetadatos(nodo);
}

GNC::GCS::Widgets::IWidgetSerializable::~IWidgetSerializable()
{
}

std::map<std::string,std::string> GNC::GCS::Widgets::IWidgetSerializable::GetPropiedades()
{
	std::map<std::string,std::string> propiedades;
	{
		std::ostringstream ostr;
		ostr << GetFormattedTimet(m_timestampCreacion);
		propiedades[_Std("Creation Date")] = ostr.str();
	}
	{
		std::ostringstream ostr;
		ostr << GetFormattedTimet(m_timestampModificacion);
		propiedades[_Std("Modification Date")] = ostr.str();
	}
	{
		propiedades[_Std("Physician")] = m_nombreMedico;
	}
	return propiedades;
}

void GNC::GCS::Widgets::IWidgetSerializable::SerializarMetadatos(wxXmlNode* nodo, const std::string& nombreMedico) {
	{
		std::ostringstream ostr;
		ostr << m_timestampCreacion;
		nodo->AddAttribute(wxT("creacion"),wxString::FromUTF8(ostr.str().c_str()));
	}
	{
		std::ostringstream ostr;
		ostr << m_timestampModificacion;
		nodo->AddAttribute(wxT("modificacion"),wxString::FromUTF8(ostr.str().c_str()));
	}
	if(m_modificado) {
		nodo->AddAttribute(wxT("medico"),wxString::FromUTF8(nombreMedico.c_str()));
		m_nombreMedico = nombreMedico;
	} else {
		nodo->AddAttribute(wxT("medico"),wxString::FromUTF8(m_nombreMedico.c_str()));
	}
	m_modificado = false;
}

void GNC::GCS::Widgets::IWidgetSerializable::LeerMetadatos(wxXmlNode* nodo)
{
	wxString valor;
	
	if(nodo->GetAttribute(wxT("creacion"),&valor)) {
		#if defined(__WXOSX__) || defined(__WXGTK__)
		if(!valor.ToLong(&m_timestampCreacion)) {
		#else
		if(!valor.ToLongLong(&m_timestampCreacion)) {
		#endif
			m_timestampCreacion = 0;
		}
	}
	else {
		m_timestampCreacion = 0;
	}
		
	if(nodo->GetAttribute(wxT("modificacion"),&valor)){
		#if defined(__WXOSX__) || defined(__WXGTK__)
		if(!valor.ToLong(&m_timestampModificacion)) {
		#else
		if(!valor.ToLongLong(&m_timestampModificacion)) {
		#endif
			time ( &m_timestampModificacion );
			m_timestampModificacion = 0;
		}
	}
	else {
		time ( &m_timestampModificacion );
	}
	
	wxString wxStr;
	if(nodo->GetAttribute(wxT("medico"),&wxStr)) {
		//si se carga del xml no esta modificado
		m_nombreMedico = wxStr.ToUTF8();
		m_modificado = false;
	} else {
		m_nombreMedico = "";
	}
}
		
void GNC::GCS::Widgets::IWidgetSerializable::ActualizarTimestampModificacion()
{
	time ( &m_timestampModificacion );
	m_modificado = true;
}
