/*
 *  
 *  $Id: coretools.cpp $
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

#include <ostream>

#include "coretools.h"
#include "angletool.h"
#include "pointertool.h"
#include "ruletool.h"
#include "toolzoom.h"
#include "toolpan.h"
#include "anonymizetool.h"
#include "cornerannotationstool.h"
#include "dicomizetool.h"
#include "detachwindowtool.h"
#include "metricgridtool.h"
#include "resettool.h"
#include "pixelsbluringtool.h"
#include "windowlayouttool.h"
#include "markertool.h"
#include "undotool.h"
#include "toolslider.h"
#include "notetool.h"
#include "toolwindowlevel.h"
#include "showtagstool.h"
#include "rotateandfliptool.h"
#include "colormaptool.h"
#include "showhistorytool.h"
#include "imageexporttool.h"
#include "queryretrievetool.h"
#include "trackballtool.h"
#include "camerarotationtool.h"
#include "videotool.h"
#include "secondarycapturetool.h"
#include "pacsuploadtool.h"
#include "startdicomizationtool.h"
#include "addfiletohistorytool.h"
#include "openginkgocloudlinktool.h"
#include "copyimagetoclipboardtool.h"
#include "simpleprinttool.h"
#include "brightnesscontrasttool.h"

#include <main/tools/historytools.h>
#include <main/controllers/historytoolscontroller.h>

void GNC::RegisterCoreTools() {
	
	#ifdef ENABLE_ANGLETOOL
		(new GNC::AngleTool())->RegisterTool();
	#endif
		/*
	#ifdef ENABLE_ANONYMIZETOOL
		(new GNC::AnonymizeTool())->RegisterTool();
	#endif
	*/
	#ifdef ENABLE_CALIBRATIONTOOL
		(new GNC::CalibrationTool())->RegisterTool();
	#endif
	#ifdef ENABLE_CORNERANNOTATIONTOOL
		(new GNC::CornerAnnotationsTool())->RegisterTool();
	#endif
	/*
	#ifdef ENABLE_DICOMEXPORTTOOL
		(new GNC::DicomExportTool())->RegisterTool();
	#endif
	*/
	/*
	#ifdef ENABLE_DICOMIZETOOL
		(new GNC::DicomizeTool())->RegisterTool();
	#endif
	*/
	#ifdef ENABLE_DETACHWINDOWTOOL
		(new GNC::DetachWindowTool())->RegisterTool();
	#endif
	#ifdef ENABLE_ELEVATIONTOOL
		(new GNC::ElevationTool())->RegisterTool();
	#endif
	#ifdef ENABLE_INTERPOLATETOOL
		(new GNC::InterpolateTool())->RegisterTool();
	#endif
	#ifdef ENABLE_MAGNIFIERTOOL
		(new GNC::MagnifierTool())->RegisterTool();
	#endif
	#ifdef ENABLE_MARKERTOOL
		(new GNC::MarkerTool())->RegisterTool();
	#endif
	#ifdef ENABLE_METRICGRIDTOOL
		(new GNC::MetricGridTool())->RegisterTool();
	#endif
	#ifdef ENABLE_NOTETOOL
		(new GNC::NoteTool())->RegisterTool();
	#endif
	#ifdef ENABLE_PANTOOL
		(new GNC::ToolPan())->RegisterTool();
	#endif
	#ifdef ENABLE_POINTERTOOL
		(new GNC::PointerTool())->RegisterTool();
	#endif
	#ifdef ENABLE_QUERYRETRIEVETOOL
		(new GNC::QueryRetrieveTool())->RegisterTool();
	#endif
	#ifdef ENABLE_RESETTOOL
		(new GNC::ResetTool())->RegisterTool();
	#endif	
	#ifdef ENABLE_RULETOOL
		(new GNC::RuleTool())->RegisterTool();
	#endif
	#ifdef ENABLE_PIXELSBLURING
		(new GNC::PixelsBluringTool())->RegisterTool();
	#endif
	#ifdef ENABLE_SHOWTAGSTOOL
		(new GNC::ShowTagsTool())->RegisterTool();
	#endif
	#ifdef ENABLE_SLIDERTOOL
		(new GNC::ToolSlider())->RegisterTool();
	#endif
	#ifdef ENABLE_TAGSPANELTOOL
		(new GNC::TagsPanelTool())->RegisterTool();
	#endif
	#ifdef ENABLE_UNDOTOOL
		(new GNC::UndoTool())->RegisterTool();
	#endif
	#ifdef ENABLE_WINDOWLAYOUTTOOL
		(new GNC::WindowLayoutTool())->RegisterTool();
	#endif
	#ifdef ENABLE_WINDOWLEVELTOOL
		(new GNC::ToolWindowLevel())->RegisterTool();
	#endif
	#ifdef ENABLE_ZOOMTOOL
		(new GNC::ToolZoom())->RegisterTool();
	#endif
	#ifdef ENABLE_BRIGHTNESSCONTRASTTOOL
		(new GNC::BrightnessContrastTool())->RegisterTool();
	#endif

		(new GNC::RotateAndFlipTool())->RegisterTool();
		(new GNC::ColorMapTool())->RegisterTool();
		(new GNC::ShowHistoryTool())->RegisterTool();
		(new GNC::ImageExportTool())->RegisterTool();
		(new GNC::TrackballTool())->RegisterTool();
		(new GNC::VideoTool())->RegisterTool();
		(new GNC::SecondaryCaptureTool())->RegisterTool();
		(new GNC::PACSUploadTool())->RegisterTool();
		(new GNC::StartDicomizationTool())->RegisterTool();
		(new GNC::AddFileToHistory())->RegisterTool();
		(new GNC::CopyImageToClipboardTool())->RegisterTool();
		(new GNC::CameraRotationTool())->RegisterTool();
		(new GNC::SimplePrintTool())->RegisterTool();

	//history
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::AnonymizeTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::ShowTagsHistoryTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::OpenFileTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::OpenFolderTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::OpenRemovableTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::DicomizeTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::QueryRetrieveHistoryTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::SendPACSTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::ExportDicomDirTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::OpenGinkgoCloudLinkTool());
	GNC::HistoryToolsController::Instance()->RegisterTool(new GNC::SearchGinkgoCloudHistoryTool());
}
