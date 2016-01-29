/*
 *
 *  $Id: ihistorycontroller.h $
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
#include <list>
#include <string>

#include <api/api.h>

namespace GNC {
	namespace GCS {
		class IPersistentCommand;
		class EXTAPI IHistoryController{
		public:
			typedef struct EXTAPI PatientModel
			{
				std::string id;
				std::string id_issuer;
				std::string bithdate;
				char		sex;
				std::string name;
			} PatientModel;

			typedef std::list<std::string> StringList;

			typedef enum TLocation {
				TL_LocalDatabase,
				TL_LocalLinked,
				TL_WadoLinked
			} TLocation;

			typedef struct EXTAPI StudyModel
			{
				long        pk;
				std::string pat_id;
				std::string pat_id_issuer;
				std::string pat_bithdate;
				char        pat_sex;
				std::string pat_name;
				std::string study_iuid;
				std::string study_id;
				std::string	study_datetime; //timestamp
				std::string study_acc_no;
				std::string ref_physician;
				std::string study_desc;
				std::list<std::string> mods_in_study;
				std::list<std::string> cuids_in_study;
				int			num_series;
				int			num_instances;
				std::string			created_time;
				std::string			updated_time;
				TLocation		location;
				StudyModel();
			} StudyModel;

			typedef struct EXTAPI SeriesModel
			{
			public:
				long			pk;
				long			study_fk;
				std::string series_iuid;
				std::string	series_datetime;
				std::string	series_no;
				std::string	series_modality;
				std::string	body_part;
				std::string	laterality;
				std::string	series_desc;
				std::string institution;
				std::string station_name;
				std::string department;
				std::string perf_physician;
				std::string	uid_importer;
				std::string frame_of_reference;
				StringList  tsuids;
				int			num_instances;
				std::string	created_time;
				std::string updated_time;
				TLocation	location;
				SeriesModel();
			} SeriesModel;		

			typedef struct EXTAPI FileModel
			{
			public:
				long			pk;
				long			series_fk;
				std::string real_path;
				std::string file_path;
				std::string sopiuid;
				std::string tsuid;
				std::string	content_datetime;
				std::string sopcuid;
				std::string file_desc;
				char			orientation;
				double		image_position[3];
				double		slice_location;
				double      slice_thickness;
				int			height;
				int			width;
				int			number_of_frames;
				std::string direction_cosines;
				std::string spacing;
				long		instance_number;
				std::string	created_time;
				std::string	updated_time;
				TLocation	location;
				
				FileModel();
				~FileModel();

			} FileModel;

			/** only important attributes **/
			typedef struct EXTAPI LightFileModel
			{
			public:
				long			pk;
				long			series_fk;
				std::string real_path;
				std::string file_path;
				std::string sopiuid;
				std::string tsuid;
				std::string sopcuid;

				TLocation	location;				
				LightFileModel();
				~LightFileModel();
			} LightFileModel;

			typedef struct EXTAPI DICOMFileModel
			{
				StudyModel study;
				SeriesModel series;
				FileModel file;
			} DICOMFileModel;

			typedef std::list<PatientModel> PatientModelList;
			typedef std::list<StudyModel> StudyModelList;
			typedef std::list<SeriesModel> SeriesModelList;
			typedef std::list<FileModel> FileModelList;
			typedef std::list<LightFileModel> LightFileModelList;
			typedef std::list<DICOMFileModel> DICOMFileModelList;

			typedef struct EXTAPI TAddError {
				typedef enum TError { // FIXME WARNING: Redefinition
					TE_FileNotExist,
					TE_WrongFormat,
					TE_DICOMDir
				} TError;
				TError error;
				std::string path;

				TAddError(TError error, const std::string& path);
			} TAddError;
			typedef std::list<TAddError> TAddErrorList;

			typedef enum TAddAction {
				TAA_MOVE,
				TAA_COPY,
				TAA_LINK
			} TAddAction;

			static char LocationToChar(TLocation location);
			static TLocation CharToLocation(char loc);
			virtual SeriesModel GetSeriesModel(long series_pk) = 0;
			virtual void GetSeriesModelFromUIDs(const StringList& uids, SeriesModelList& series) = 0;
			virtual void GetSeriesModelFromUID(const std::string& uids, SeriesModelList& series) = 0;
			virtual void GetSeriesFromStudy(long study_pk, SeriesModelList& listOfSeries) = 0;
			virtual StudyModel GetStudyModel(long study_pk) = 0;
			virtual FileModel GetFrameOfReference(long series_pk) = 0;
			virtual FileModel GetFrameOfReference(const std::string& series_pk) = 0;
			virtual FileModel GetFileModel(long file_pk) = 0;
			virtual FileModel GetFileModelFromSopInstance(const std::string& sopInstanceUID, long study_pk = -1) = 0;
			virtual bool IsOk() = 0;

			virtual void GetAllPatients(GNC::GCS::IHistoryController::PatientModelList& patientList) = 0;
			
			virtual bool ExistsSeriesWithUID(const std::string& seriesUID) = 0;
			virtual int GetNumberOfFiles() = 0;
			virtual void GetSeriesSortedFileModels(long series_pk, FileModelList& fileList, bool onlyFirst = false) = 0;
			virtual void GetSeriesSortedFileModels(long series_pk, LightFileModelList& fileList, bool onlyFirst = false) = 0;
			virtual StringList GetSeriesSortedPathsByUID(const std::string& seriesUID) = 0;
			virtual StringList GetSeriesSortedPaths(const std::string& series_pk) = 0;
			virtual StringList GetSeriesSortedPaths(long series_pk) = 0;
			virtual StringList GetSeriesSopClasses(long series_pk) = 0;
			virtual char GetSeriesOrientation(long series_pk) = 0;

			virtual bool EmptyHistory(bool eliminarDelDisco) = 0;

			static IHistoryController* Instance();
			static void FreeInstance();

			protected:
			IHistoryController();
			~IHistoryController();

		};
	}
}
