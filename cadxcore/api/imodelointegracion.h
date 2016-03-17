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
#include <string>
#include <list>
#include <sstream>
#include <api/api.h>
#include <api/dicom/idicom.h>
#include <api/autoptr.h>

namespace GIL
{
class EXTAPI IModeloPACSServer
{
public:

        typedef enum IMPS_Method {
                IMPS_GET,
                IMPS_MOVE,
                IMPS_WADO
        } IMPS_Method;

        std::string sid;
        std::string AET;
        std::string hostname;
        std::string puerto;
        std::string pdu;
        std::string retrieveLevel;
        bool        tls;
        bool        verify;
        IMPS_Method metodo;
        std::string cert;
        std::string key;
        std::string user;
        std::string password;
        std::string wadoURI;
        std::string defaultCharset;

        IModeloPACSServer()
        {
                this->tls = false;
                this->verify = false;
                this->metodo = IMPS_GET;
                user=password = "";
        }

        IModeloPACSServer(const IModeloPACSServer& otro) = default;


        IModeloPACSServer(const std::string& sid, const std::string& AET, const std::string& hostname, const std::string& puerto, const std::string& pdu, IMPS_Method metodo, const std::string& retrieveLevel = "SERIES", bool tls = false, bool verify = false, const std::string& cert = "", const std::string& key = "", const std::string& defaultCharset = "ISO_IR 192")
        {
                this->sid      = sid;
                this->AET      = AET;
                this->hostname = hostname;
                this->puerto   = puerto;
                this->pdu      = pdu;
                this->tls      = tls;
                this->verify   = verify;
                this->metodo   = metodo;
                this->retrieveLevel = retrieveLevel;
                this->cert     = cert;
                this->key      = key;
                this->defaultCharset = defaultCharset;
        }

        IModeloPACSServer& operator=(const IModeloPACSServer& otro) = default;

};

class EXTAPI IVariableModel
{
public:
        std::string key;
        std::string value;
        std::string description;

        IVariableModel()
        {
        }

        IVariableModel(const std::string& key, const std::string& value, const std::string& description = "")
        {
                this->key					= key;
                this->value					= value;
                this->description       = description;
        }

        IVariableModel( const IVariableModel& o)
        {
                (*this) = o;
        }

        IVariableModel& operator=(const IVariableModel& otro)
        {
                this->key					= otro.key;
                this->value					= otro.value;
                this->description       = otro.description;
                return *this;
        }

        const std::string& GetKey() const
        {
                return this->key;
        }

        const std::string& GetValue() const
        {
                return this->value;
        }

        const std::string& GetDescription() const
        {
                return this->description;
        }

};

class EXTAPI IMapOfVarsModel: protected std::map<std::string, IVariableModel>
{
public:
        static std::string empty;
        typedef std::map<std::string, IVariableModel>::iterator TMapOfVarsModelIterator;
        IMapOfVarsModel() {}
        ~IMapOfVarsModel() {}

        bool InsertVariable (const std::string& key, const std::string& value, const std::string& description = "");
        const std::string& GetValue(const std::string& key) const;
        const std::string& GetDescription(const std::string& key) const;
        bool Contains(const std::string& key) const;
        bool IsEmpty();
        TMapOfVarsModelIterator Begin();
        TMapOfVarsModelIterator End();

};

class EXTAPI IImageModel
{
public:
        IImageModel() {}
        ~IImageModel() {}

        IImageModel( const IImageModel& o)
        {
                (*this) = o;
        }

        IImageModel& operator=(const IImageModel& otro)
        {
                this->MapOfVars					= otro.MapOfVars;
                return *this;
        }

        IMapOfVarsModel MapOfVars;
};

class EXTAPI ISeriesModel
{
public:
        typedef std::list<IImageModel> TListOfImages;
        ISeriesModel() {}
        ~ISeriesModel() {}

        ISeriesModel( const ISeriesModel& o)
        {
                (*this) = o;
        }

        ISeriesModel& operator=(const ISeriesModel& otro)
        {
                this->Images					= otro.Images;
                this->MapOfVars					= otro.MapOfVars;
                return *this;
        }

        TListOfImages	Images;
        IMapOfVarsModel MapOfVars;
};

class EXTAPI IStudyModel
{
public:
        typedef std::list<ISeriesModel> TListOfSeries;

        IStudyModel() {}
        ~IStudyModel() {}

        IStudyModel( const IStudyModel& o)
        {
                (*this) = o;
        }

        IStudyModel& operator=(const IStudyModel& otro)
        {
                this->Series					= otro.Series;
                this->MapOfVars					= otro.MapOfVars;
                return *this;
        }

        TListOfSeries	Series;
        IMapOfVarsModel MapOfVars;
};

class EXTAPI IModeloIntegracion
{
public:
        typedef std::list<std::string> ListOfPaths;
        typedef std::list<IStudyModel> TListOfStudies;

        typedef enum TipoAccion {
                TA_Ninguna,
                TA_Dicomizar,
                TA_Obtener,
                TA_Imprimir,
                TA_Open
        } TipoAccion;


        GIL::DICOM::DicomDataset TagsDICOMOverwrite;
        IMapOfVarsModel GlobalVariables;
        TListOfStudies  Studies;

        ListOfPaths			Files;

        std::string       idPlantilla;
        TipoAccion        accion;

        std::string       rawXmlData;
        std::string       messageTemplate;

        bool              atendido;

        IModeloIntegracion();

        IModeloIntegracion(const IModeloIntegracion& otro)
        {
                *this = otro;
        }

        ~IModeloIntegracion()
        {

        }

        IModeloIntegracion& operator=(const IModeloIntegracion& otro)
        {
                this->TagsDICOMOverwrite             = otro.TagsDICOMOverwrite;
                this->GlobalVariables           = otro.GlobalVariables;
                this->Studies                = otro.Studies;
                this->Files          = otro.Files;
                this->idPlantilla         = otro.idPlantilla;
                this->accion = otro.accion;
                this->rawXmlData   = otro.rawXmlData;
                this->atendido      = otro.atendido;
                this->messageTemplate = otro.messageTemplate;

                return *this;
        }
};

typedef std::list< GNC::GCS::Ptr<GIL::IModeloIntegracion> > IntegrationModelList;
}

