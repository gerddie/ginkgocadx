/*
 *  
 *  $Id: iextensionsobserver.h $
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

namespace GNC {
	namespace GCS {
		class IModuleController;
	}
}

namespace GNC {
	namespace GCS {

		class EXTAPI IExtensionsObserver {
		public:

			IExtensionsObserver();
			virtual ~IExtensionsObserver();

			/** The module is loaded **/
			virtual void OnModuleLoaded(GNC::GCS::IModuleController* ) = 0;
			/** The module is unloaded **/
			virtual void OnModuleUnloaded(GNC::GCS::IModuleController* ) = 0;

			/** The module is activated **/
			virtual void OnModuleActivated(GNC::GCS::IModuleController* );
			/** The module is deactivated **/
			virtual void OnModuleDeactivated(GNC::GCS::IModuleController* );
		};
	}
}
