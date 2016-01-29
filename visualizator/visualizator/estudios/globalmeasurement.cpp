 /*  
 *  $Id: globalmeasurement.cpp $
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
#include <cmath>

namespace GNKVisualizator {


	GlobalMeasurement::GlobalMeasurement()
	{
		Ponset    = MeasurementNoValue;
		Poffset   = MeasurementNoValue;
		QRSonset  = MeasurementNoValue;
		QRSoffset = MeasurementNoValue;
		Toffset   = MeasurementNoValue;
		Paxis     = MeasurementNoAxisValue;
		QRSaxis   = MeasurementNoAxisValue;
		Taxis     = MeasurementNoAxisValue;
	}
	unsigned short GlobalMeasurement::calcQTc(unsigned short AvgRR, unsigned short HR, TQTcCalcType calcType)
	{

		if ((AvgRR == 0)
		||	(AvgRR == MeasurementNoValue)
		||	(QTdur() == MeasurementNoValue))
			return MeasurementNoValue;

		unsigned short ret = MeasurementNoValue;

		switch (calcType)
		{
			case Bazett:
				ret = (unsigned short) (QTdur() / std::sqrt(AvgRR * 0.001));
				break;
			case Fridericia:
				ret = (unsigned short) (QTdur() / std::pow(AvgRR * 0.001, 1.0/3.0));
				break;
			case Framingham:
				ret = (unsigned short) (QTdur() + (154 * (1 - (AvgRR * 0.001))));
				break;
			case Hodges:
				ret = (unsigned short) (QTdur() + (1.75 * (HR - 60)));
				break;
			default:break;
		}

		return ret;
	}
}