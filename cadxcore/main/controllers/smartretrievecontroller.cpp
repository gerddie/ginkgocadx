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


#include "smartretrievecontroller.h"

#include <api/dicom/dcmuiddictionary.h>
#include <api/dicom/dcmdictionary.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/storedqueriescontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/pacscontroller.h>
#include <commands/pacsdownloadcommand.h>
#include <main/entorno.h>
#include <api/ilock.h>
#include <api/threads/thread.h>
#include <api/observers/ieventsobserver.h>
#include <main/controllers/controladoreventos.h>
#include <eventos/progresocomando.h>
#include <wx/thread.h>
#include <set>

#define LOGGER "SmartRetrieve"

namespace GNC
{
namespace GCS
{
////Anon usage thread to upload messages... Read anon usage database and send messages to the web service
class SmartRetrieveService: public GNC::GCS::Thread, GNC::GCS::IEventsObserver, GNC::GCS::ILockable
{
public:
        SmartRetrieveService(int sPeriod, int maxDownloads): GNC::GCS::Thread(LOGGER),
                hasToStop(false),
                condition(sleepLocker),
                sleepingPeriod(sPeriod),
                maxDownloadsPerIteration(maxDownloads)
        {
                GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
        }

        virtual ~SmartRetrieveService()
        {
                //can't be synchronous => deathlock
                GNC::GCS::ControladorEventos::Instance()->DesRegistrar(this);
                GNC::Entorno::Instance()->GetCommandController()->AbortarComandosDeOwnerAsincrono(this);
        }

        void Destroy()
        {
        }

        void Stop()
        {
                hasToStop = true;
                condition.Broadcast();
        }

        virtual void* Task()
        {
                try {
                        LOG_DEBUG(LOGGER, "Waiting to to start SmartRetrieveService Task...");
                        //we have to lock the mutex before wait, else it will fail
                        //sleepLocker.TryLock();
                        //condition.WaitTimeout(ANON_USAGE_THREAD_START_TO);
                        while (!hasToStop) {
                                LOG_DEBUG(LOGGER, "Starting SmartRetrieveService Task...");
                                //first of all get active queries...
                                GNC::GCS::StoredQueryController::TStoredQueriesList listOfQueries = GNC::GCS::StoredQueryController::Instance()->getActiveQueries();
                                int numberOfDownloads = 0;
                                for (GNC::GCS::StoredQueryController::TStoredQueriesList::iterator it = listOfQueries.begin(); it != listOfQueries.end() && numberOfDownloads < maxDownloadsPerIteration; ++it) {
                                        GNC::GCS::Ptr<GNC::GCS::StoredQuery>& pStoredQuery = (*it);
                                        //perform query...
                                        GIL::DICOM::IPACSController* pCI = NULL;
                                        typedef std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> > TSearchResultList;
                                        TSearchResultList results;
                                        try {

                                                pCI = GNC::GCS::IEntorno::Instance()->GetPACSController();
                                                if (pCI == NULL) {
                                                        LOG_ERROR(LOGGER, "pCI is null");
                                                        continue;
                                                }
                                                pCI->GetConnection(this);
                                                GIL::DICOM::DicomDataset query;
                                                pStoredQuery->addCondition(GKDCM_QueryRetrieveLevel,"STUDY");
                                                pStoredQuery->addConditionIfNotExists(GKDCM_StudyInstanceUID);
                                                pStoredQuery->buildQuery(query);
                                                pCI->Query(this, GKUID_FINDStudyRootQueryRetrieveInformationModel, query, results, pStoredQuery->getPacsId(), NULL);
                                        } catch (GNC::GCS::IException& ex) {
                                                LOG_WARN(LOGGER, "There has been an exception searching in PACS " << ex.GetFullCause());
                                        } catch (...) {
                                                LOG_WARN(LOGGER, "There has been an exception searching in PACS");
                                        }
                                        if (pCI) {
                                                pCI->ReleaseConnection(this);
                                        }
                                        //we have results stored in "results" iterate throught studyInstanceUIDs to check if we have to download them...
                                        for (TSearchResultList::iterator it  = results.begin(); it != results.end(); ++it) {
                                                std::string studyInstanceUID = (*it)->getTag(GKDCM_StudyInstanceUID);
                                                if (!studyInstanceUID.empty() && !GNC::GCS::HistoryController::Instance()->ExistsStudyWithUID(studyInstanceUID)) {
                                                        LOG_DEBUG(LOGGER, "Trying to download " << studyInstanceUID);
                                                        //download study!!
                                                        GIL::DICOM::DicomDataset base;
                                                        base.tags[GKDCM_QueryRetrieveLevel] = "STUDY";
                                                        base.tags[GKDCM_StudyInstanceUID] = studyInstanceUID;
                                                        {
                                                                GNC::GCS::ILocker(this);
                                                                inProgressStudies.insert(studyInstanceUID);
                                                        }
                                                        GADAPI::PACSDownloadCommandParams* pParams = new GADAPI::PACSDownloadCommandParams(pStoredQuery->getPacsId(), base, false, true);
                                                        GNC::GCS::IComando* pCommand = new GADAPI::PACSDownloadCommand(pParams);
                                                        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Downloading from PACS..."), pCommand, this);
                                                        numberOfDownloads ++;
                                                        if (numberOfDownloads > maxDownloadsPerIteration) {
                                                                LOG_DEBUG(LOGGER, "max downloads per iteration limit raised " << numberOfDownloads);
                                                                break;
                                                        }
                                                }
                                        }
                                }
                                condition.WaitTimeout(sleepingPeriod * 1000);
                        }
                } catch (...) {
                        LOG_WARN(LOGGER, "Error in AnonUsageTask")
                }
                LOG_DEBUG(LOGGER, "Task finished...");
                sleepLocker.Unlock();
                return 0;
        }

        virtual void ProcesarEvento(GNC::GCS::Events::IEvent* evt)
        {
                //delete studies from inProgres...
                if (evt != NULL && evt->GetCodigoEvento() == ginkgoEVT_Core_ProgresoComando) {
                        GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*>(evt);
                        if (pEvt != NULL && pEvt->GetTipo() == GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado
                            && pEvt->GetComando() != NULL && pEvt->GetComando()->GetOwner() == this) {
                                GADAPI::PACSDownloadCommandParams* pParams = dynamic_cast<GADAPI::PACSDownloadCommandParams*>(pEvt->GetComando()->GetParams());
                                if (pParams != NULL) {
                                        GNC::GCS::ILocker locker(this);
                                        std::string studyUID = pParams->m_base.getTag(GKDCM_StudyInstanceUID);
                                        if (inProgressStudies.find(studyUID) != inProgressStudies.end()) {
                                                inProgressStudies.erase(inProgressStudies.find(studyUID));
                                                LOG_TRACE(LOGGER, "Study downloaded " << studyUID);
                                                if (inProgressStudies.empty() && pParams->m_error.empty()) {
                                                        LOG_TRACE(LOGGER, "waking up thread...");
                                                        condition.Broadcast();
                                                }
                                        }
                                }
                        }
                }
        }

        bool hasToStop;
        wxMutex sleepLocker;
        wxCondition condition;
        int sleepingPeriod;
        int maxDownloadsPerIteration;
        std::set<std::string> inProgressStudies;
};
}
}


//singleton
GNC::GCS::SmartRetrieveController* GNC::GCS::SmartRetrieveController::m_pInstance = NULL;

GNC::GCS::SmartRetrieveController * GNC::GCS::SmartRetrieveController::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new SmartRetrieveController();
        }
        return m_pInstance;
}
void GNC::GCS::SmartRetrieveController::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}


GNC::GCS::SmartRetrieveController::SmartRetrieveController():
        tidService(-1),
        pThread(NULL)

{
}

GNC::GCS::SmartRetrieveController::~SmartRetrieveController()
{
        stopThread();
}

void GNC::GCS::SmartRetrieveController::resetStatus()
{
        //read settings...
        if (isEnabled()) {
                stopThread();
                startThread();
                LOG_TRACE(LOGGER, "SmartRetrieveController is enabled");
        } else {
                //stop thread
                stopThread();
                //delete all messages...
                LOG_TRACE(LOGGER, "SmartRetrieveController has been disabled");
        }
}


bool GNC::GCS::SmartRetrieveController::isEnabled()
{
        bool isEnabled;
        GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GinkgoCore/SmartRetrieve", "Enabled", isEnabled, false);
        return isEnabled;
}

void GNC::GCS::SmartRetrieveController::enable(bool enable)
{
        GNC::GCS::ConfigurationController::Instance()->writeBoolUser("/GinkgoCore/SmartRetrieve", "Enabled", enable);
}

int GNC::GCS::SmartRetrieveController::getPeriod()
{
        int period;
        GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/SmartRetrieve", "Period", period, 120);
        return period;
}

void GNC::GCS::SmartRetrieveController::setPeriod(int period)
{
        GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/SmartRetrieve", "Period", period);
}

int GNC::GCS::SmartRetrieveController::getMaxDownloadPerIteration()
{
        int maxDownloads;
        GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/SmartRetrieve", "MaxDownloadsPerIteration", maxDownloads, 7);
        return maxDownloads;
}

void GNC::GCS::SmartRetrieveController::setMaxDownloadPerIteration(int maxDownloads)
{
        GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/MaxDownloadsPerIteration", "Period", maxDownloads);
}

void GNC::GCS::SmartRetrieveController::stopThread()
{
        GNC::GCS::ThreadController::Stop(tidService);
        if (pThread != NULL) {
                delete pThread;
                pThread = NULL;
        }
}

void GNC::GCS::SmartRetrieveController::startThread()
{
        if (pThread != NULL)  {
                stopThread();
        }
        pThread = new SmartRetrieveService(getPeriod(), getMaxDownloadPerIteration());
        tidService = GNC::GCS::ThreadController::Launch(pThread, false);
}
