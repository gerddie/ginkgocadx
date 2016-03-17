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
#include <api/icommand.h>
#include <api/autoptr.h>

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GNC
{
class ComandoComprobarActualizaciones;

class ComandoComprobarActualizacionesParams : public GNC::GCS::IComandoParams
{
public:
        ComandoComprobarActualizacionesParams(const std::string& url, bool informar = false);

        virtual ~ComandoComprobarActualizacionesParams();

        std::string                     m_URL;
        bool                            m_Informar;

};

class ComandoComprobarActualizaciones : public GNC::GCS::IComando
{
public:

        ComandoComprobarActualizaciones(ComandoComprobarActualizacionesParams* pParams);

        const std::string& GetError() const;
        const std::string& GetVersion() const;
        const std::string& GetDescription() const;
        int GetRevision() const;

protected:

        virtual void Execute();
        virtual void Update();

protected:

        ComandoComprobarActualizacionesParams* m_pComprobarActualizacionesParams;
        std::string                            m_Error;

private:
        std::string m_Version;
        int         m_Revision;
        std::string m_Description;
        std::string m_URL;
};
}
