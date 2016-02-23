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
