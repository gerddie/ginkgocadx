/*
 *  
 *  $Id: smartretrievecontroller.h $
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
#include <string>
#include <api/autoptr.h>

namespace GNC {	
	namespace GCS {
		class SmartRetrieveService;

		class SmartRetrieveController
		{
		public:
			static SmartRetrieveController *Instance();
			static void FreeInstance();
			
			//stop and reload settings...
			void resetStatus();

			bool isEnabled();
			void enable(bool enable);
			int getPeriod();
			void setPeriod(int period);
			int getMaxDownloadPerIteration();
			void setMaxDownloadPerIteration(int maxDownloads);

		protected:
			SmartRetrieveController();
			~SmartRetrieveController();

			void stopThread();
			void startThread();
			
			int tidService;
			SmartRetrieveService* pThread;
			//singleton
			static SmartRetrieveController* m_pInstance;
		};
	}
}


