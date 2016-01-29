/*
 *  
 *  $Id: autocleaningcontroller.cpp $
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

#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>
#include <wx/thread.h>
#include <wx/timer.h>

#include "autocleaningcontroller.h"
#include <commands/autocleaningcommand.h>

#include <main/controllers/commandcontroller.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/configurationcontroller.h>

//this timer is to controlate user inactivity
namespace GNC {
	namespace GCS {
		class TimerAutocleaning: public wxTimer
		{
		public:
			//first shot 30s
			TimerAutocleaning():wxTimer()
			{
				//Start(30000, true);
				Start(1000, true);
			}
			~TimerAutocleaning()
			{
				GNC::CommandController::Instance()->AbortarComandosDeOwner(this);
			}

			virtual void Notify()
			{
				//next shot one each 10 minutes...
				Start(10*60*1000, true);
				int acquisitionOlder,hdSpace,dicomDirSize;
				GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "AcquisitionOlderThan", acquisitionOlder, -1);
				GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "HDFreeSpace", hdSpace, -1);
				GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "DicomDirSize", dicomDirSize, -1);
				if (acquisitionOlder != -1 || hdSpace != -1 || dicomDirSize != -1) {
					//launch autocleaning command...
					GADAPI::AutoCleaningCommandParameters* pParams = new GADAPI::AutoCleaningCommandParameters(acquisitionOlder, hdSpace, dicomDirSize);
					GADAPI::AutoCleaningCommand* pCmd = new GADAPI::AutoCleaningCommand(pParams);
					GNC::CommandController::Instance()->ProcessAsync("Autocleaning", pCmd, this);
				}
			}

			virtual void Reset() {
				if (IsRunning()) {
					Stop();
				}
				Start(1000, true);
			}
		};
	}
}
//


//region "Patron singleton"

GNC::GCS::AutoCleaningController* GNC::GCS::AutoCleaningController::m_pInstance = 0;

GNC::GCS::AutoCleaningController::AutoCleaningController()
{
	m_pTimerAutocleaning = new TimerAutocleaning;
}

GNC::GCS::AutoCleaningController::~AutoCleaningController()
{
	if (m_pTimerAutocleaning != NULL) {
		if (m_pTimerAutocleaning->IsRunning()) {
			m_pTimerAutocleaning->Stop();
		}
		delete m_pTimerAutocleaning;
		m_pTimerAutocleaning = NULL;
	}
}

GNC::GCS::AutoCleaningController* GNC::GCS::AutoCleaningController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GNC::GCS::AutoCleaningController();
	}
	return m_pInstance;
}

void GNC::GCS::AutoCleaningController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

void GNC::GCS::AutoCleaningController::Reset()
{
	if (m_pTimerAutocleaning != NULL) {
		m_pTimerAutocleaning->Reset();
	}
}

//endregion

