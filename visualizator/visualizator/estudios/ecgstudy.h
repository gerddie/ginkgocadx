/*
 *  
 *  $Id: ecgstudy.h $
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

#include "overlay.h"
#include "globalmeasurements.h"
#include <api/istudycontext.h>
#include <api/internationalization/internationalization.h>

class wxWindow;
class wxSizer;
class wxXmlNode;
class wxXmlDocument;

namespace GNKVisualizator {
	//////////////ECGS
	class ChannelInfo
	{
	public:
		typedef enum TLeadType
		{
			Unknown = 0, I, II, V1, V2, V3, V4, V5, V6, V7, V2R, V3R, V4R, V5R, V6R, V7R, X, Y, Z,
			CC5, CM5, LA, RA, LL, fI, fE, fC, fA, fM, fF, fH, dI, dII, dV1, dV2, dV3, dV4, dV5, dV6,
			dV7, dV2R, dV3R, dV4R, dV5R, dV6R, dV7R, dX, dY, dZ, dCC5, dCM5, dLA, dRA, dLL, dfI, dfE,
			dfC, dfA, dfM, dfF, dfH, III, aVR, aVL, aVF, aVRneg, V8, V9, V8R, V9R, D, A, J, Defib, 
			Extern, A1, A2, A3, A4, dV8, dV9, dV8R, dV9R, dD, dA, dJ, Chest, V, VR, VL, VF, MCL, MCL1,
			MCL2, MCL3, MCL4, MCL5, MCL6, CC, CC1, CC2, CC3, CC4, CC6, CC7, CM, CM1, CM2, CM3, CM4, CM6,
			dIII, daVR, daVL, daVF, daVRneg, dChest, dV, dVR, dVL, dVF, CM7, CH5, CS5, CB5, CR5, ML, AB1,
			AB2, AB3, AB4, ES, AS, AI, S, dDefib, dExtern, dA1, dA2, dA3, dA4, dMCL1, dMCL2, dMCL3,
			dMCL4, dMCL5, dMCL6, RL, CV5RL, CV6LL, CV6LU, V10, dMCL, dCC, dCC1, dCC2, dCC3, dCC4, dCC6,
			dCC7,  dCM, dCM1, dCM2, dCM3, dCM4, dCM6, dCM7, dCH5, dCS5, dCB5, dCR5, dML, dAB1, dAB2, dAB3,
			dAB4, dES, dAS, dAI, dS, dRL, dCV5RL, dCV6LL, dCV6LU, dV10
		} TLeadType;

		ChannelInfo() :
				LeadType(Unknown),
				Sensitivity(1.0),
				SensitivityCorrectionFactor(1.0),
				Baseline(0.0),
				TimeSkew(0.0),
				BitsPerSample(16),
				FilterLowFrequency(0.0),
				FilterHighFrequency(0.0),
				NumSamples(0),
				SamplingFrecuency(0.0),
				Multiplier(1.0)
		{
			
		}

		void SetLead(const std::string& codingSchemeDesignator, const std::string& codingValue);
		std::string GetTitle() const;
			
		void SetUnitsValue(const std::string& unitsvalue);

		void SetUnitsMeaning(const std::string& unitsmeaning) {
			UnitsMeaning = unitsmeaning;			
		}

		void SetSensitivity (double sensitivity) { Sensitivity = sensitivity; }
		void SetSensitivityCorrectionFactor(double sensitivitycorrectionfactor) { SensitivityCorrectionFactor = sensitivitycorrectionfactor;}
		void SetChannelBaseline(double baseline) { Baseline = baseline;}
		void SetChannelTimeSkew(double timeskew) { TimeSkew = timeskew;}
		void SetBitsPerSample(unsigned int bitspersample) { BitsPerSample = bitspersample;}
		void SetFilterLowFrequency(double filterlowfrequency) { FilterLowFrequency = filterlowfrequency;}
		void SetFilterHighFrequency(double filterhighfrequency) { FilterHighFrequency = filterhighfrequency;}
		


		TLeadType	 LeadType;
		std::string  UnitsValue;
		std::string  UnitsMeaning;

		double       Sensitivity;
		double       SensitivityCorrectionFactor;
		double       Baseline;
		double       TimeSkew;
		unsigned int BitsPerSample;
		double       FilterLowFrequency;
		double       FilterHighFrequency;
		double		 NumSamples;
		double		 SamplingFrecuency;

		typedef std::vector<double> TVectorSamples;
		TVectorSamples Samples;

		double       Multiplier;
	};

	class ECGStudy : public GNC::GCS::IStudyContext
	{
	public:
		typedef std::vector<ChannelInfo> TListChannelInfo;
		class TMultiplexGroup {
		public:
			TMultiplexGroup() :
				Label(_Std("Unknown"))
			{
				
			}
			TListChannelInfo Channels;
			std::string Label;
			std::string Diagnosis;
		};
		typedef std::vector<TMultiplexGroup> TListMultiplexGroup;


		typedef struct TECGFile {
			TListMultiplexGroup Groups;
			GNC::GCS::Ptr<GlobalMeasurements> globalMeasurements;
			int IndexOfActualMultiplexGroup;
			TECGFile() {
				IndexOfActualMultiplexGroup = 0;
			}
			~TECGFile() {
				Groups.clear();
			}
			TListChannelInfo& GetListOfChannels() {
				return Groups[IndexOfActualMultiplexGroup].Channels;
			}
			std::list<std::string> GetLabelOfMultiplexGroups() {
				std::list<std::string> listOfLabels;
				for (TListMultiplexGroup::iterator it = Groups.begin(); it != Groups.end(); ++it) {
					listOfLabels.push_back((*it).Label);
				}
				return listOfLabels;
			}			

			void SetIndexOfActualMultiplexGroup(int index) {
				if (index >=0 && index < (int)Groups.size()) {
					IndexOfActualMultiplexGroup = index;
				}
			}
			const std::string& GetDiagnosis() {
				return Groups[IndexOfActualMultiplexGroup].Diagnosis;
			}
		} TECGFile;


		typedef std::vector<GNC::GCS::Ptr<TECGFile> >              TVectorECGFiles;
		
		ECGStudy();
		~ECGStudy();

		void InitializeContext(long pkSeries);

		//overlays...

		virtual void CallbackCargarTagsImagen(const int /*indice*/, GIL::DICOM::IDICOMManager* /*pDicomManager*/) {}

		void SetIndexOfActualMultiplexGroup(int index);
		TListChannelInfo& GetListOfChannels();
		bool Ok();
		std::list<std::string> GetLabelOfMultiplexGroups();
		const std::string GetGlobalMeasurement();

		const std::string& GetDiagnosis();
		
		std::string getMeasureText(bool isVertical, float measure);

		virtual void LoadChannels();
		

	protected:
		TVectorECGFiles	  ECGFiles;
	};
}
