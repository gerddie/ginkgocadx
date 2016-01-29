 /*  
 *  $Id: globalmeasurement.h $
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