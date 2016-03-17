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

#pragma once
#include <api/api.h>
#include <ctime>
#include <map>
#include <string>

// Forward Declarations
class wxXmlNode;
// Forward Declarations

namespace GNC
{
namespace GCS
{
namespace Widgets
{

class EXTAPI IWidgetSerializable
{
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
