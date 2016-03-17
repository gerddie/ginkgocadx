/*
 *  Ginkgo CADx Project
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
#include <map>
#include <string>
#include <api/imodelointegracion.h>
#include <api/observers/ieventsobserver.h>
#include <wx/string.h>
#include <wx/xml/xml.h>
namespace GNC
{
namespace GCS
{
class IModuleController;
}
}

namespace GIL
{
class EXTAPI IntegrationException : public GNC::GCS::IException
{
public:
        IntegrationException(const std::string& msg, const std::string& component="Modelo", bool fatal = true) throw() : GNC::GCS::IException(msg, component, fatal) {}
};

class EXTAPI IXMLIntegrationParser
{
public:
        virtual ~IXMLIntegrationParser();
        virtual std::list<std::string> GetKeys() = 0;
        virtual void ParseIntegrationXML(GIL::IntegrationModelList& modelos, wxXmlNode* pRoot) = 0;
};

class EXTAPI IntegrationController : public GNC::GCS::IEventsObserver
{
public:
        static IntegrationController* Instance();
        static void FreeInstance();

        bool Process(GIL::IntegrationModelList& models);

        // Event listener realization
        virtual std::string GetMessageFromIntegrationModel(GNC::GCS::Ptr<GIL::IModeloIntegracion>& integrationModel);
        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

        /* devuelve el uid de modulo a partir del pid indicado*/
        virtual std::string GetUID(const std::string& idPlantilla) const;
        virtual std::string GetUIDModulo(const std::string& idPlantilla) const;
        virtual std::string GetIdPlantilla(const std::string& uidModulo) const;

        //parsers...
        void RegisterParser(const std::string& xmlKey, const GNC::GCS::Ptr<IXMLIntegrationParser>& pParser);

        void UnRegisterParser(const std::string& xmlKey);

        /* Parsea la cadena XML y construye el modelo de integracion. Los parametros no encontrados se asignan por defecto segun lo especificado en la configuracion global */
        virtual void ParsearModeloIntegracion(GIL::IntegrationModelList& modelos, const std::string& xmlString, const wxString& relativePath = wxEmptyString) const;

        /* Parses ginkgocadx:// url */
        virtual std::string ParsesModelFromURL(const wxString& url) const;

protected:
        IntegrationController();
        ~IntegrationController();


protected:
        static IntegrationController* m_pInstance;
        typedef std::map<std::string, GNC::GCS::Ptr<IXMLIntegrationParser> > TMapIntegrationXMLParsers;
        TMapIntegrationXMLParsers m_mapParsers;


};
}
