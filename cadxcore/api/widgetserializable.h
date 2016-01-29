/*
 *  
 *  $Id: widgetserializable.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <api/api.h>
#include <ctime>
#include <map>
#include <string>

// Forward Declarations
class wxXmlNode;
// Forward Declarations

namespace GNC {
	namespace GCS {
		namespace Widgets {
			
			class EXTAPI IWidgetSerializable {
			public:
				IWidgetSerializable();				
				IWidgetSerializable(wxXmlNode* nodo);
				
				virtual ~IWidgetSerializable();
				
				virtual wxXmlNode* Serializar(const std::string& nombreMedico) = 0;

				std::map<std::string,std::string> GetPropiedades();
				
			protected:
				void SerializarMetadatos(wxXmlNode* nodo, const std::string& nombreMedico);
				
				void LeerMetadatos(wxXmlNode* nodo);
						
				void ActualizarTimestampModificacion();
			
			protected:
				time_t m_timestampCreacion;
				time_t m_timestampModificacion;
				std::string m_nombreMedico;
				bool m_modificado;
			};
		}
	}
}
