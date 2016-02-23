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
#include <string>
#include <list>

#include <api/dicom/dicomdataset.h>
#include <api/api.h>
#include <api/controllers/ihistorycontroller.h>
#include <api/globals.h>
#include <wx/datetime.h>

class wxSQLite3Database;
class wxCriticalSection;

namespace GNC {
	class IProxyNotificadorProgreso;
	namespace GCS {
		
		namespace History {
			class EXTAPI IHistoryFilter {
			public:
				IHistoryFilter() {
					Enabled = false;
				}
				virtual ~IHistoryFilter(){}

				virtual std::string GetSQLFilter() = 0;
				void Enable(bool enabled) {
					this->Enabled = enabled;
				}
				bool IsEnabled() {
					return this->Enabled;
				}
			protected:
				bool Enabled;
			};

			class EXTAPI StudyModalityFilter: public IHistoryFilter
			{
			public:
				StudyModalityFilter(){}

				StudyModalityFilter(const std::list<std::string>& listOfModalities) {
					this->listOfModalities = listOfModalities;
				}
				StudyModalityFilter(const std::string& modality) {
					this->listOfModalities.push_back(modality);
				}

				void SetModalities(const std::list<std::string>& listOfModalities) {
					this->listOfModalities = listOfModalities;
				}

				const std::list<std::string>& GetModalities() {
					return listOfModalities;
				}

				virtual std::string GetSQLFilter();
			protected:

				std::list<std::string> listOfModalities;
			};

			class EXTAPI IHistoryDateFilter: public IHistoryFilter
			{
			public:
				IHistoryDateFilter() {}

				IHistoryDateFilter(const wxDateTime& from, const wxDateTime& to) {
					this->from = from;
					this->to = to;
				}

				virtual void SetDates(const wxDateTime& from, const wxDateTime& to) {
					this->from = from;
					this->to = to;
				}

				const wxDateTime& GetFromDate() {
					return from;
				}

				const wxDateTime& GetToDate() {
					return to;
				}
			protected:
				wxDateTime from;
				wxDateTime to;
			};

			class EXTAPI AcquisitionDateFilter: public IHistoryDateFilter
			{
			public:
				AcquisitionDateFilter() {}
				AcquisitionDateFilter(const wxDateTime& from, const wxDateTime& to): IHistoryDateFilter(from,to)
				{}

				virtual std::string GetSQLFilter();
			};

			class EXTAPI StudyPatientFilter: public IHistoryFilter
			{
			public:
				StudyPatientFilter() {}

				StudyPatientFilter(const std::string& pat_id) {
					this->pat_id = pat_id;
				}

				void SetPatientId(const std::string& pat_id) {
					this->pat_id = pat_id;
				}

				const std::string& GetPatientId() {
					return pat_id;
				}

				virtual std::string GetSQLFilter();
			protected:
				std::string pat_id;
			};

			class EXTAPI StudyLocationFilter: public IHistoryFilter
			{
			public:
				StudyLocationFilter(GNC::GCS::IHistoryController::TLocation location): Location(location) {}

				StudyLocationFilter() {
					;
				}

				virtual std::string GetSQLFilter();
			protected:
				GNC::GCS::IHistoryController::TLocation Location;
			};

		}
		class EXTAPI HistoryController: public IHistoryController{
		public:
			typedef std::list<long> IdList;
			typedef struct TaskModel {
				long pk;
				std::string uid;
				int status;
				std::string statusDescription;
				std::string data;
				std::string description;
				std::string	created_time;
				IdList seriesId;
				TaskModel() {
					pk = 0;
					status = 0;
				}
			} TaskModel;

	//region "Patron singleton"
		public:
			static HistoryController* Instance();
			static void FreeInstance();
			
			/* Constructor*/
			HistoryController();
			HistoryController(const std::string& pathOfDataBase);
		protected:
			void InitController(const std::string& pathOfDataBase);
		public:
			virtual ~HistoryController();

			bool IsOk();
			bool IsReadOnly();
			virtual std::string GetGinkgoDicomDir(bool barraAlFinal = false);
			void ReloadHistory(const std::string& pathDicomDir = "", bool tryTemp = true);			
			static std::string GetDefaultDicomDirPath(bool ignoreConfig = false);
		protected:
			//	//sincrono con la interfaz
			static HistoryController* m_pInstance;
	//endregion
		public:

			//region "metodos propios del historial"
			bool AddFiles(const StringList& listaPaths, TAddErrorList& errorList, std::list<long>* insertedSeries, GNC::IProxyNotificadorProgreso* pNotificador = NULL, TAddAction action = TAA_COPY);
			bool UpdateFiles(const GNC::GCS::HistoryController::FileModelList& files);
			bool LinkFilesToSeries(long seriesPk, GNC::GCS::HistoryController::DICOMFileModelList& files, GNC::IProxyNotificadorProgreso* pNotificador = NULL);
		protected:
			bool UpdateFileModel(FileModel& fileModel);
			void FindOrInsertStudy(DICOMFileModel& model);
			void FindOrInsertSeries(DICOMFileModel& model);
			bool FindOrInsertFile(DICOMFileModel& model);
			bool FindOrLink(DICOMFileModel& model);

			void UpdateStudy(DICOMFileModel& model);
			void UpdateSeries(DICOMFileModel& model);
			void UpdateFile(DICOMFileModel& model);

			std::string GetPathOfFile(GNC::GCS::IHistoryController::TLocation location, long pk, const std::string& file_path);
			std::string GetPathOfFile(long file_pk);
			std::string GetRelativePath(const std::string& absolutePath);
			std::string GetAbsolutePath(const std::string& relativePath);
			void UpdateSeriesSummaryInfo(long series_pk);
			void UpdateStudySummaryInfo(long study_pk);
		public:
			SeriesModel GetSeriesModel(long series_pk);
			void GetDatabaseDatasetFromSeries(long series_pk, GIL::DICOM::DicomDataset& dataset);
			void GetSeriesModelFromUIDs(const StringList& uids, SeriesModelList& series);
			void GetSeriesModelFromUID(const std::string& uids, SeriesModelList& series);
			void GetSeriesFromStudy(long study_pk, SeriesModelList& listOfSeries);
			StudyModel GetStudyModel(long study_pk);
			void GetStudyModelFromUID(const std::string& uids, StudyModelList& studies);
			/**
			gets dicom tags from study stored in database
			*/
			void GetDatabaseDatasetFromStudy(long study_pk, GIL::DICOM::DicomDataset& dataset);
			void GetStudies(GNC::GCS::History::IHistoryFilter* historyFilter, StudyModelList& listOfStudies);
			void GetStudies(std::list<GNC::GCS::History::IHistoryFilter*>& historyFilter, StudyModelList& listOfStudies);
			FileModel GetFrameOfReference(long series_pk);
			FileModel GetFrameOfReference(const std::string& series_pk);
			FileModel GetFileModel(long file_pk);
			FileModel GetFileModelFromSopInstance(const std::string& sopInstanceUID, long study_pk = -1);

			void GetAllPatients(GNC::GCS::HistoryController::PatientModelList& patientList);
			
			bool ExistsSeriesWithUID(const std::string& seriesUID);
			bool ExistsStudyWithUID(const std::string& studyUID);
			int GetNumberOfFiles();
			/**this method take a lot of time, if you don't need verbose information, use LightFileModelList...**/
			virtual void GetSeriesSortedFileModels(long series_pk, FileModelList& fileList, bool onlyFirst = false);
			virtual void GetSeriesSortedFileModels(long series_pk, LightFileModelList& fileList, bool onlyFirst = false);
			StringList GetSeriesSortedPathsByUID(const std::string& seriesUID);
			StringList GetSeriesSortedPaths(const std::string& series_pk);
			StringList GetSeriesSortedPaths(long series_pk);
			virtual StringList GetSeriesSopClasses(long series_pk);
			char GetSeriesOrientation(long series_pk);

			bool DeleteSeriesList(const IdList& pks);
			bool DeleteSopInstanceUIDs(const StringList& sopInstanceUIds);

			bool EmptyHistory(bool eliminarDelDisco);
		//	//thumbnails...
			void SetThumbnail(long file_pk, int ancho, int alto, const unsigned char* imagen);
			void GetThumbnail(long file_pk, int& ancho, int& alto, unsigned char*& imagen);
			void DeleteThumbnails(IdList& listPks);

			//deprecated persistent commands are deprecated
			virtual void SaveTask(GNC::GCS::IPersistentCommand* pTask);
			virtual void DeleteTasks(const std::list<TaskModel>& tasks);
			virtual bool IsPendingTasks();
			virtual void GetPendingTasks(std::list<TaskModel>& tasks);
			virtual void GetSeriesPendingTasks(const std::list<long>& seriesPk, std::list<TaskModel>& tasks);
			virtual void GetSeriesTasks(const std::list<long>& seriesPk, std::list<TaskModel>& tasks);
			virtual TaskModel GetTask(long idTask);
			virtual void GetAllTasks(std::list<TaskModel>& tasks);
		//	//
		//protected:
		//	//region "helpers"
		//	//devuelve false si !copia y no esta donde deberia
		public:
			bool ReadFile(DICOMFileModel& modelo,const std::string& pathAbsoluto, TAddErrorList& errorList, TAddAction action);
			bool ReadFile(DICOMFileModel& modelo,const std::string& pathAbsoluto, TAddErrorList& errorList);

		protected:
			void RemoveFilesFromDisk(StringList&listOfFiles);

			//endergion

			wxSQLite3Database* m_pConexion;
			wxSQLite3Database* m_pConexionThumbnails;
			wxCriticalSection* m_pCriticalSection;
			wxCriticalSection* m_pCriticalSectionThumbnails;
			bool m_readOnly;
			std::string m_GinkgoDicomDir;
		};
	}
}
