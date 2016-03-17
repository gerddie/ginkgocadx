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
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ivista.h>
#include <api/controllers/ipacscontroller.h>

namespace GIL
{
class IModeloIntegracion;
namespace DICOM
{
class IPACSController;
}
}

namespace GADAPI
{
class EXTAPI SendPACSCommand : public GNC::GCS::IComando
{
public:
        SendPACSCommand(const std::string& selectedServer, const std::string& hl7Message, const std::list<long> seriesId);

        SendPACSCommand( const SendPACSCommand& o);
        SendPACSCommand& operator = (const SendPACSCommand& o);
protected:
        void checkDependencies();
public:

        virtual ~SendPACSCommand();
        virtual std::string getClassUID();

        virtual std::string serialize();
        virtual void init(const std::string& serializedValue);

protected:
        virtual void Execute();
        virtual void Update();

public:
        virtual bool NotificarProgreso(float progresoNormalizado, const std::string &texto);

        void LiberarRecursos();

        std::string m_selectedServer;
        std::list<long> m_seriesId;
        std::string m_hl7Message;
        std::string m_error;
};
}
