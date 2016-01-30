/*
 *  
 *  $Id: autocleaningcontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once

#include <api/api.h>
#include <api/globals.h>

#include <string>
#include <map>


namespace GNC {

	namespace GCS {
		class TimerAutocleaning;
		class EXTAPI AutoCleaningController {

//region "Patron singleton"
		public:
			static AutoCleaningController* Instance();
			static void FreeInstance();
			
			virtual void Reset();
//endregion

//region "Interfaz de controlador de vistas"
		protected:			
			TimerAutocleaning*		  m_pTimerAutocleaning;
			static AutoCleaningController* m_pInstance;
			AutoCleaningController();
			virtual ~AutoCleaningController();
		};
	}
}
