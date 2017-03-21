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
#include <vector>
#include <api/controllers/ipacscontroller.h>
#include <dcmtk/dcmnet/dimse.h>
class DicomServer;

namespace GNC
{
class IProxyNotificadorProgreso;
}

namespace GIL
{
namespace DICOM
{


class DicomStoreAssociation
{
public:

        DicomStoreAssociation();
        ~DicomStoreAssociation();
        typedef std::list<std::string> ListaRutas;
        void Store(ListaRutas listaFicheros, const GNC::GCS::Ptr<DicomServer>& server, std::string nombreAETLocal,GNC::IProxyNotificadorProgreso* pNotificador, GIL::DICOM::TipoTransferSyntaxEnvio transferSyntax);
        void FreeMemory(bool fail_is_fatal);
        static void progressCallback(void * /*callbackData*/,T_DIMSE_StoreProgress *progress,T_DIMSE_C_StoreRQ * /*req*/);

        void SetTLS(const std::string& CliCert, const std::string& CliKey, bool validate)
        {
                m_TLS = true;
                m_CliCert = CliCert;
                m_CliKey = CliKey;
                m_Validate = validate;
        }

        void SetUserPass(const std::string& PacsUser, const std::string& PacsPass)
        {
                m_UseUserPass = true;
                m_pacsUser = PacsUser, m_pacsPass = PacsPass;
        }
        bool UseUserPass()
        {
                return m_UseUserPass;
        }
        const std::string& GetUser()
        {
                return m_pacsUser;
        }
        const std::string& GetPass()
        {
                return m_pacsPass;
        }

        bool IsSecure() const
        {
                return m_TLS;
        }

        const std::string& GetCliCert() const
        {
                return m_CliCert;
        }
        const std::string& GetCliKey() const
        {
                return m_CliKey;
        }
        bool GetValidate() const
        {
                return m_Validate;
        }

protected:
        bool m_TLS;
        std::string m_CliCert;
        std::string m_CliKey;
        bool m_UseUserPass;
        std::string m_pacsUser;
        std::string m_pacsPass;
        bool m_Validate;
        T_ASC_Association *assoc;
        T_ASC_Network *net;

};
}
}
