#pragma once
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

#ifndef ICOMMANDSCONTROLLER_H
#define ICOMMANDSCONTROLLER_H

#include <string>

#include <api/iexception.h>
#include <api/icommand.h>

namespace GNC
{
namespace GCS
{

class EXTAPI CommandControllerException : public GNC::GCS::IException
{
public:
        CommandControllerException(const std::string& msg, const std::string& componente = "Model", bool fatal = true) throw();
};


class EXTAPI ICommandController
{

protected:
        ICommandController();
        virtual ~ICommandController();

public:

        static ICommandController* Instance();
        static void FreeInstance();

        virtual void ProcessSync(IComando*, bool autodelete, bool update = true, GNC::INotificadorProgreso* pNotifier = NULL) = 0;
        virtual void ProcessAsync(const std::string& descr, IComando*, void* owner) = 0;
        virtual void AbortarComando(long threadId, bool sincrono = true) = 0;
        virtual void AbortarComandosDeOwner(void* owner) = 0;
        virtual void AbortarComandosDeOwnerAsincrono(void* owner) = 0;
        virtual void AbortarComando(IComando* pComando, bool sincrono = true) = 0;

        virtual unsigned int GetNumActiveCommands() = 0;

};
}
}
#endif
