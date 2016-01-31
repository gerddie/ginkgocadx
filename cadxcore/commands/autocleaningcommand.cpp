/*
*
*  $Id: autocleaningcommand.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include <api/globals.h>

#include "autocleaningcommand.h"
#include <api/internationalization/internationalization.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>

#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/utils.h>

/*
this command is composed of two commands the sequence is this:
1 - get a list of candidates to be deleted listA
2 - synchronize with gui and call history panel to drop those candidates
3 - if any delete has be performed resets autocleaning controller to launch command again...
*/

#define IDC_AUTOCLEANING_COMMAND   122

GADAPI::AutoCleaningCommandParameters::AutoCleaningCommandParameters(int olderThan, int hdSpace, int dicomDirSize):
	OlderThan(olderThan),
	HdSpace(hdSpace),
	DicomDirSize(dicomDirSize)
{	
}

GADAPI::AutoCleaningCommandParameters::~AutoCleaningCommandParameters()
{
}

GADAPI::AutoCleaningCommand::AutoCleaningCommand(AutoCleaningCommandParameters* pParams) : IComando(pParams)
{
	m_pAutoCleaningParameters = pParams;
	SetId(IDC_AUTOCLEANING_COMMAND);
	AbortaSi(IDC_AUTOCLEANING_COMMAND);
}

bool compareCreation (const GNC::GCS::IHistoryController::StudyModel& first, const GNC::GCS::IHistoryController::StudyModel& second)
{
	wxDateTime dtFirst, dtSecond;
	dtFirst.ParseFormat(wxString::FromUTF8(first.created_time.c_str()), wxT("%Y-%m-%dT%H:%M:%S.%l"));
	dtSecond.ParseFormat(wxString::FromUTF8(second.created_time.c_str()), wxT("%Y-%m-%dT%H:%M:%S.%l"));
	return dtFirst > dtSecond;
}

void GADAPI::AutoCleaningCommand::Execute()
{
	std::string tarea= _Std("Cleaning dicomdir...");
	if (!NotificarProgreso(0.0f,tarea)) {
		return;
	}

	//from more restrictive to less...
	if (m_pAutoCleaningParameters->OlderThan > 0) {
		//ask history studies older than..
		wxDateTime from, to;
		to = wxDateTime::Now();
		to.Add(wxDateSpan::Days(-1*m_pAutoCleaningParameters->OlderThan));

		GNC::GCS::History::AcquisitionDateFilter filter(from,to);
		filter.Enable(true);
		GNC::GCS::HistoryController::StudyModelList studyList;
		std::list<GNC::GCS::History::IHistoryFilter*> filterList;
		filterList.push_back(&filter);
		GNC::GCS::HistoryController::Instance()->GetStudies(filterList, studyList);
		int progress = 0;
		for (GNC::GCS::HistoryController::StudyModelList::iterator it = studyList.begin(); it != studyList.end(); ++it,progress++) {
			if (!NotificarProgreso((double)progress/studyList.size(),tarea)) {
				return;
			}
			GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy((*it).pk, m_pAutoCleaningParameters->seriesToDelete);
		}
	}

	//how much size we have to free
	wxDiskspaceSize_t spaceToFree = 0;
	wxDiskspaceSize_t KB = 1024;
	wxDiskspaceSize_t MB = KB*1024;
        //	wxDiskspaceSize_t GB = MB*1024;

	//if there is no series to delete try with next criteria
	if (m_pAutoCleaningParameters->HdSpace > 0 && m_pAutoCleaningParameters->seriesToDelete.empty()) {
		//guess how much size we have to free...
		wxDiskspaceSize_t total = 0;
		wxDiskspaceSize_t free = 0;		

		if (wxGetDiskSpace(FROMPATH(GNC::GCS::HistoryController::Instance()->GetGinkgoDicomDir()), &total, &free) ) {
			wxDiskspaceSize_t hdSpaceInBytes = wxDiskspaceSize_t(m_pAutoCleaningParameters->HdSpace) * MB;
			if (hdSpaceInBytes > free) {
				//we have to free bytes
				spaceToFree = hdSpaceInBytes - free;
			}
		}
	}

	//if there is no series to delete and spactofree is ok try with next criteria
	if (m_pAutoCleaningParameters->DicomDirSize > 0 && m_pAutoCleaningParameters->seriesToDelete.empty() && spaceToFree == 0) {
		wxString dicomDirPath(FROMPATH(GNC::GCS::HistoryController::Instance()->GetGinkgoDicomDir()));
		//know current dicomdir size...
		wxDiskspaceSize_t currentSize = 0;
		wxDir dir;
		if (dir.Open(dicomDirPath)) {
			wxString wxPathFich;
			bool cont = dir.GetFirst(&wxPathFich,wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
			while (cont) {
				wxPathFich = dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) + wxPathFich;
				if (wxFileName::IsFileReadable(wxPathFich)) {
					currentSize += wxFileName::GetSize(wxPathFich).GetValue();
				}
				cont = dir.GetNext(&wxPathFich);
			}
		}
		wxDiskspaceSize_t maxDicomDirSizeBytes = wxDiskspaceSize_t(m_pAutoCleaningParameters->DicomDirSize) * MB;
		if (currentSize > maxDicomDirSizeBytes) {
			spaceToFree = currentSize - maxDicomDirSizeBytes;
		}
	}

	if (spaceToFree > 0) {
		LOG_DEBUG("AutoCleaning", "We have to free  " << spaceToFree.ToDouble() << "bytes");
		//select all studies sort by acquisition date located in local database...
		GNC::GCS::HistoryController::StudyModelList studyList;
		GNC::GCS::History::StudyLocationFilter filter(GNC::GCS::HistoryController::TL_LocalDatabase);
		std::list<GNC::GCS::History::IHistoryFilter*> filterList;
		filterList.push_back(&filter);
		GNC::GCS::HistoryController::Instance()->GetStudies(filterList, studyList);
		//sort them by acquisition date..
		studyList.sort(compareCreation);

		int progress = 0;
		for (GNC::GCS::HistoryController::StudyModelList::iterator itStudy = studyList.begin();spaceToFree>0 && itStudy != studyList.end(); ++itStudy, ++progress) {
			if (!NotificarProgreso((double)progress/studyList.size(),tarea)) {
				return;
			}
			//calculate study size to delete it...
			GNC::GCS::HistoryController::SeriesModelList series;
			GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy((*itStudy).pk, series);
			for (GNC::GCS::HistoryController::SeriesModelList::iterator itSeries = series.begin(); itSeries != series.end(); ++itSeries) {
				GNC::GCS::HistoryController::LightFileModelList files;
				GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*itSeries).pk, files);
				for (GNC::GCS::HistoryController::LightFileModelList::iterator itFiles = files.begin(); itFiles != files.end(); ++itFiles) {
					if ((*itFiles).location == GNC::GCS::HistoryController::TL_LocalDatabase) {
						wxFileName fn(FROMPATH((*itFiles).real_path));
						spaceToFree -= fn.GetSize().GetValue();
					}
				}
				m_pAutoCleaningParameters->seriesToDelete.push_back((*itSeries));
			}			
		}
	}
}

void GADAPI::AutoCleaningCommand::Update()
{
	if (EstaAbortado()) {
		return;
	}
		
}

	

	
