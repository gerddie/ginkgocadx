/*
 *  Ginkgo CADx Project
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

#define MeasurementNoValue      29999 
#define MeasurementNoAxisValue 29999

namespace GNKVisualizator {
	class GlobalMeasurement
	{
	public:
		typedef enum TQTcCalcType
		{
			Unknown = -1,
			Bazett = 0,
			Hodges = 1,
			Fridericia = 2,
			Framingham = 3
		} TQTcCalcType;
		
		unsigned short Ponset;
		unsigned short Poffset;
		unsigned short QRSonset;
		unsigned short QRSoffset;
		unsigned short Toffset;
		short Paxis;
		short QRSaxis;
		short Taxis;

		GlobalMeasurement();

		unsigned short Pdur()
		{
			return (unsigned short) ((Poffset != MeasurementNoValue) && (Ponset != MeasurementNoValue) && (Ponset < Poffset) ? (Poffset - Ponset) : MeasurementNoValue);
		}

		unsigned short PRint()
		{
			return (unsigned short) ((QRSonset != MeasurementNoValue) && (Ponset != MeasurementNoValue) && (Pdur() != MeasurementNoValue) ? (QRSonset - Ponset) : MeasurementNoValue);
		}

		unsigned short QRSdur()
		{
			return (unsigned short) ((QRSoffset != MeasurementNoValue) && (QRSonset != MeasurementNoValue) ? (QRSoffset - QRSonset) : MeasurementNoValue);
		}

		unsigned short Tdur()
		{
			return (unsigned short) ((Toffset != MeasurementNoValue) && (Ponset != MeasurementNoValue) ? (Toffset - QRSoffset) : MeasurementNoValue);
		}

		unsigned short QTdur()
		{
			return (unsigned short) ((Toffset != MeasurementNoValue) && (QRSonset != MeasurementNoValue) ? (Toffset - QRSonset) : MeasurementNoValue);
		}

		 unsigned short calcQTc(unsigned short AvgRR, unsigned short HR, TQTcCalcType calcType);
	};
}
