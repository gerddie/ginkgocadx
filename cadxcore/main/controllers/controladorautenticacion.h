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

#include <api/api.h>
#include <api/controllers/icontroladorautenticacion.h>
#include <api/autoptr.h>


class wxSQLite3Database;
namespace GSEC
{
namespace Auth
{
typedef enum {
        TA_NO_LOGIN = 0,
        TA_USER_PASSWORD,
        TA_PASSWORD
} TipoAutenticacion;

class EXTAPI ControladorAutenticacion : public IControladorAutenticacion
{
public:

        static ControladorAutenticacion* Instance();
        static void FreeInstance();

        GNC::GCS::Ptr<wxSQLite3Database> IniciarSesionModificacionBBDDSeguridad();
        void GuardarCambiosBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion);
        void CancelarCambiosYCerrarBBDDSeguridad(GNC::GCS::Ptr<wxSQLite3Database> sesion);


        TipoAutenticacion GetTipoAutenticacion();
        void SetTipoAutenticacion(TipoAutenticacion tipo);

        virtual bool Auth(const std::string& user, const std::string& password);

        bool InsertarUsuario(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

        bool CambiarPassword(const std::string& user, const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

        bool SetPasswordGeneral(const std::string& password, GNC::GCS::Ptr<wxSQLite3Database> sesion = NULL);

        bool EliminarUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

        void GetAllUsers(std::list<std::string>& listaUsuarios, GNC::GCS::Ptr<wxSQLite3Database> sesion =NULL);

        bool ExisteUsuario(const std::string& user, GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

        bool IsPasswordGeneralSetted(GNC::GCS::Ptr<wxSQLite3Database> sesion=NULL);

        int GetMaxLongitudUsuario();

protected:
        ControladorAutenticacion();
        ~ControladorAutenticacion();

        static ControladorAutenticacion* m_pInstance;

        bool AutenticacionUsuarioPassword(const std::string& usuario, const std::string& password);
        bool AutenticacionPassword(const std::string& password);

        std::string GetHashPassword(const std::string& password);
        GNC::GCS::Ptr<wxSQLite3Database> GetNewConexionSeguridad();

};
}
}
