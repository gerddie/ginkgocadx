 /*  
 *  $Id: globalmeasurements.cpp $
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
#include "globalmeasurements.h"

namespace GNKVisualizator {
		GlobalMeasurements::GlobalMeasurements() :
				QTcType(GlobalMeasurement::Unknown),
				_QTc(MeasurementNoValue),
				_VentRate(MeasurementNoValue),
				AvgRR(MeasurementNoValue),
				AvgPP(MeasurementNoValue)
		{
		}
}