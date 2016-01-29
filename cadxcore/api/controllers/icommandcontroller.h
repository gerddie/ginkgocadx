#pragma once
/*
 *  
 *  $Id: icommandscontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef ICOMMANDSCONTROLLER_H
#define ICOMMANDSCONTROLLER_H

#include <string>

#include <api/iexception.h>
#include <api/icommand.h>

namespace GNC {
	namespace GCS {

		class EXTAPI CommandControllerException : public GNC::GCS::IException {
		public:
			CommandControllerException(const std::string& msg, const std::string& componente = "Model", bool fatal = true) throw();
		};


		class EXTAPI ICommandController {

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
