#include "ihistorycontroller.h"
#include <main/controllers/historycontroller.h>

GNC::GCS::IHistoryController::IHistoryController()
{
}

GNC::GCS::IHistoryController::~IHistoryController()
{
}

GNC::GCS::IHistoryController::StudyModel::StudyModel() :
	pk(-1),
	pat_sex('?'),
	num_series(-1),
	num_instances(-1),
	location(TL_LocalDatabase)
{
}

GNC::GCS::IHistoryController::SeriesModel::SeriesModel() :
	pk(-1),
	study_fk(-1),
	num_instances(-1),
	location(TL_LocalDatabase)
{
	pk = -1;
}

GNC::GCS::IHistoryController::FileModel::FileModel() :
	pk(-1),
	series_fk(-1),
	orientation('A'),
	slice_location(0.0),
	height(0),
	width(0),
	number_of_frames(0),
	instance_number(0),
	location(TL_LocalDatabase)
{
	image_position[0] = 0.0;
	image_position[1] = 0.0;
	image_position[2] = 0.0;
}

GNC::GCS::IHistoryController::FileModel::~FileModel() {	
}


GNC::GCS::IHistoryController::LightFileModel::LightFileModel() :
	pk(-1),
	series_fk(-1),
	location(TL_LocalDatabase)
{
}

GNC::GCS::IHistoryController::LightFileModel::~LightFileModel() {	
}

GNC::GCS::IHistoryController::TAddError::TAddError(TError error, const std::string& path)
{
	this->error = error;
	this->path = path;
}


GNC::GCS::IHistoryController* GNC::GCS::IHistoryController::Instance()
{
	return GNC::GCS::HistoryController::Instance();
}

void GNC::GCS::IHistoryController::FreeInstance()
{
	GNC::GCS::HistoryController::FreeInstance();
}

char GNC::GCS::IHistoryController::LocationToChar(GNC::GCS::IHistoryController::TLocation location)
{
	switch (location) {
		case TL_LocalLinked:
			return 'L';
		case TL_WadoLinked:
			return 'W';
		case TL_LocalDatabase:
		default:
			return 'B';
	}
}

GNC::GCS::IHistoryController::TLocation GNC::GCS::IHistoryController::CharToLocation(char loc)
{
	switch (loc) {
		case 'L':
			return TL_LocalLinked;
		case 'W':
			return TL_WadoLinked;
		case 'B':
		default:
			return TL_LocalDatabase;
	}
}

