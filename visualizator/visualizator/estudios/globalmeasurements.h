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
#include "globalmeasurement.h"
#include <vector>
#include <api/autoptr.h>

namespace GNKVisualizator
{
struct Spike {
        unsigned short Time;
        unsigned short Amplitude;

        Spike() : Time(MeasurementNoValue), Amplitude(MeasurementNoValue)
        {
        }
};

///////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////
class GlobalMeasurements
{
public:


        GlobalMeasurement::TQTcCalcType QTcType;


        GlobalMeasurements();

private:
        unsigned short _QTc;
        unsigned short _VentRate;

public:
        unsigned short AvgRR;
        unsigned short AvgPP;
        std::vector<GNC::GCS::Ptr<GlobalMeasurement> > measurment;
        std::list<GNC::GCS::Ptr<Spike> > spike;


        unsigned short getVentRate()
        {
                if (_VentRate < MeasurementNoValue) {
                        return _VentRate;
                }

                return (unsigned short) ((AvgRR == 0) || (AvgRR == MeasurementNoValue) ? 0 : (60000 / AvgRR));
        }
        void setVentRate(unsigned short value)
        {
                _VentRate = value < MeasurementNoValue ? value : MeasurementNoValue;
        }

        unsigned short getPdur()
        {
                if (!measurment.empty() && measurment[0].IsValid()) {
                        return measurment[0]->Pdur();
                }

                return MeasurementNoValue;
        }

        unsigned short getPRint()
        {
                if (!measurment.empty() && measurment[0].IsValid()) {
                        return measurment[0]->PRint();
                }

                return MeasurementNoValue;
        }

        unsigned short getQRSdur()
        {
                if (!measurment.empty() && measurment[0].IsValid()) {
                        return measurment[0]->QRSdur();
                }

                return MeasurementNoValue;
        }

        unsigned short getQTdur()
        {
                if (!measurment.empty() && measurment[0].IsValid()) {
                        return measurment[0]->QTdur();
                }

                return MeasurementNoValue;
        }

        unsigned short  getQTc()
        {
                if (_QTc < MeasurementNoValue) {
                        return _QTc;
                }

                if (!measurment.empty() && measurment[0].IsValid() && (AvgRR != MeasurementNoValue)) {
                        return measurment[0]->calcQTc(AvgRR, getVentRate(),(GNKVisualizator::GlobalMeasurement::TQTcCalcType) QTcType);
                }

                return MeasurementNoValue;
        }
        void setQTc(unsigned short value)
        {
                _QTc = value;
        }


        GlobalMeasurement::TQTcCalcType getQTcType()
        {
                if (_QTc >= MeasurementNoValue) {
                        return (GlobalMeasurement::TQTcCalcType) (_QTc - MeasurementNoValue);
                }

                return GlobalMeasurement::Unknown;
        }
        void setQTcType(GlobalMeasurement::TQTcCalcType value)
        {
                if (value != GlobalMeasurement::Unknown)
                        _QTc = (unsigned short) (MeasurementNoValue + value);
                else if (_QTc >= MeasurementNoValue)
                        _QTc = 0;
        }
        /*
        public GlobalMeasurements Clone()
        {
        GlobalMeasurements ret = new GlobalMeasurements();

        ret._QTc = _QTc;
        ret._VentRate = VentRate;
        ret.AvgPP = AvgPP;
        ret.AvgRR = AvgRR;

        if (measurment != null)
        {
        	ret.measurment = new GlobalMeasurement[measurment.Length];
        	for (int i=0;i < measurment.Length;i++)
        	{
        		ret.measurment[i] = new GlobalMeasurement();

        		ret.measurment[i].Ponset = measurment[i].Ponset;
        		ret.measurment[i].Poffset = measurment[i].Poffset;
        		ret.measurment[i].QRSonset = measurment[i].QRSonset;
        		ret.measurment[i].QRSoffset = measurment[i].QRSoffset;
        		ret.measurment[i].Toffset = measurment[i].Toffset;

        		ret.measurment[i].Paxis = measurment[i].Paxis;
        		ret.measurment[i].QRSaxis = measurment[i].QRSaxis;
        		ret.measurment[i].Taxis = measurment[i].Taxis;
        	}
        }

        if (spike != null)
        {
        	ret.spike = new Spike[spike.Length];
        	for (int i=0;i < spike.Length;i++)
        	{
        		ret.spike[i] = new Spike();
        		ret.spike[i].Amplitude = spike[i].Amplitude;
        		ret.spike[i].Time = spike[i].Time;
        	}
        }

        return ret;
        }*/
};
}
