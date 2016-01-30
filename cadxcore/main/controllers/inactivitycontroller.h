/*
 *  
 *  $Id: inactivitycontroller.h $
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
#include <api/controllers/ieventscontroller.h>

#include <string>
#include <map>

class wxAuiNotebook;

namespace GNC {

	namespace GCS {
		class TimerInactivity;
		class EXTAPI InactivityController {

//region "Patron singleton"
		public:
			static InactivityController* Instance();
			static void FreeInstance();

			static InactivityController* m_pInstance;
//endregion

//region "Interfaz de controlador de vistas"
		public:
			virtual void StopsMonitoring();
			virtual void RestartMonitoring();
			virtual void ResetsInactivity();
			virtual void DetachForm();
			virtual void KillNow();
		protected:			
			TimerInactivity*		  m_pTimerInactivity;
			InactivityController();
			virtual ~InactivityController();
		};
	}
}
