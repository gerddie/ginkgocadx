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
