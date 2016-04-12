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


#include <wx/app.h>
#include <wx/xml/xml.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/mstream.h>
#include <wx/tokenzr.h>

#include <api/globals.h>
//#include <api/filename.h>
#include <api/ientorno.h>
#include <api/toolsystem/itoolsregistry.h>
#include <api/internationalization/internationalization.h>

#include <main/controllers/controladorlog.h>

#include <api/controllers/icontroladorcarga.h>
#include <api/imodelointegracion.h>

#include <api/dicom/idicommanager.h>
#include <api/dicom/idicomizador.h>
#include <main/entorno.h>
#include <main/controllers/historycontroller.h>

#include "ecgstudy.h"

#include <vtkPointData.h>
#include <vtkImageData.h>



namespace UIDS
{

const char* HemodynamicWaveformStorage = "1.2.840.10008.5.1.4.1.1.9.2.1";
const char* TwelveLeadECGWaveformStorage = "1.2.840.10008.5.1.4.1.1.9.1.1";
const char* GeneralECGWaveformStorage = "1.2.840.10008.5.1.4.1.1.9.1.2";
}

namespace TAGS
{
const char* MediaStorageSOPClassUID = "0002|0002";
const char* WaveformOriginality = "003a|0004";
const char* NumberOfWaveformChannels = "003a|0005";
const char* NumberOfWaveformSamples = "003a|0010";
const char* SamplingFrequency = "003a|001a";
const char* MultiplexGroupLabel = "003a|0020";
const char* ChannelDefinitionSequence = "003a|0200";
const char* WaveformChannelNumber = "003a|0202";
const char* ChannelLabel = "003a|0203";
const char* ChannelStatus = "003a|0205";
const char* ChannelSourceSequence = "003a|0208";
const char* ChannelSourceModifiersSequence = "003a|0209";
const char* SourceWaveformSequence = "003a|020a";
const char* ChannelDerivationDescription = "003a|020c";
const char* ChannelSensitivity = "003a|0210";
const char* ChannelSensitivityUnitsSequence = "003a|0211";
const char* ChannelSensitivityCorrectionFactor = "003a|0212";
const char* ChannelBaseline = "003a|0213";
const char* ChannelTimeSkew = "003a|0214";
const char* ChannelSampleSkew = "003a|0215";
const char* ChannelOffset = "003a|0218";
const char* WaveformBitsStored = "003a|021a";
const char* FilterLowFrequency = "003a|0220";
const char* FilterHighFrequency = "003a|0221";
const char* NotchFilterFrequency = "003a|0222";
const char* NotchFilterBandwidth = "003a|0223";
const char* WaveformDataDisplayScale = "003a|0230";
const char* WaveformDisplayBkgCIELabValue = "003a|0231";
const char* WaveformPresentationGroupSequence = "003a|0240";
const char* PresentationGroupNumber = "003a|0241";
const char* ChannelDisplaySequence = "003a|0242";
const char* ChannelRecommendDisplayCIELabValue = "003a|0244";
const char* ChannelPosition = "003a|0245";
const char* DisplayShadingFlag = "003a|0246";
const char* FractionalChannelDisplayScale = "003a|0247";
const char* AbsoluteChannelDisplayScale = "003a|0248";
const char* MultiplexAudioChannelsDescrCodeSeq = "003a|0300";
const char* ChannelIdentificationCode = "003a|0301";
const char* ChannelMode = "003a|0302";

const char* WaveFormSequence = "5400|0100";
const char* ChannelMinimumValue = "5400|0110";
const char* ChannelMaximumValue = "5400|0112";
const char* WaveformBitsAllocated = "5400|1004";
const char* WaveformSampleInterpretation = "5400|1006";
const char* WaveformPaddingValue = "5400|100a";
const char* WaveformData = "5400|1010";

const char* AnnotationSequence = "0040|b020";
const char* UnformattedTextValue = "0070|0006";
const char* AnnotationGroupNumber = "0040|a180";
const char* ReferencedChannels = "0040|a0b0";
const char* TemporalRangeType = "0040|a130";
const char* RefSamplePositions = "0040|a132";
const char* NumericValue = "0040|a30a";

const char* ConceptCodeSequence = "0040|a168";
const char* ConceptNameCodeSequence = "0040|a043";
const char* MeasurementsUnitsCodeSequence = "0040|08ea";
const char* CodingSchemeDesignator = "0008|0102";
const char* CodingValue ="0008|0100";
const char* CodingSchemeVersion = "0008|0103";
}

template<class T> void Get(const std::string& str, T& val)
{
        std::istringstream is(str);
        is >> val;
}

typedef std::vector<std::pair<std::string, std::string> > TVectorPairStrings;
typedef std::vector<std::string> TVectorStrings;
typedef std::map<int,TVectorStrings > TMapVectorStrings;

bool IsCodeSequence(GIL::DICOM::DicomDataset& base, const std::string& tag, std::string& codeValue, std::string codingSchemeDesignator, std::string codingSchemeVersion)
{
        GIL::DICOM::DicomDataset* seq = base.buscar_secuencia(tag);
        if (seq != NULL && seq->items.size() > 0) {
                if (seq->items.front().getTag(TAGS::CodingSchemeDesignator) == codingSchemeDesignator || seq->items.front().getTag(TAGS::CodingSchemeVersion) == codingSchemeVersion) {
                        if (!seq->items.front().getTag(TAGS::CodingValue, codeValue)) {
                                /* GW: I don't know what it means when the CodingValue tag is not found,
                                 * but one of the callers simply discards the value, and the other checks against
                                 * it, so if it is empty, nothing should happen, so just make sure that the
                                 * value is cleaned.
                                 */
                                codeValue="";
                                LOG_ERROR("EGCStudy", "IsCodeSequence: Tag 'CodingValue' but still returning true");
                        }
                        return true;
                }
        }
        return false;
}

int calcNrOfValues(const TMapVectorStrings& vals)
{
        int nr = 0;

        for (TMapVectorStrings::const_iterator it = vals.begin(); it != vals.end(); ++it) {
                for (TVectorStrings::const_iterator itV = (*it).second.begin(); itV != (*it).second.end(); ++itV) {
                        if ((*itV) != "") {
                                nr ++;
                        }
                }
        }

        return nr;
}

TMapVectorStrings GetValues(GIL::DICOM::DicomDataset* sequence, TVectorPairStrings& items, TVectorPairStrings& units, std::list<int>& /*rwc*/, bool special)
{
        TMapVectorStrings result;

        TMapVectorStrings sl;

        for (GIL::DICOM::DicomDataset::DatasetList::reverse_iterator itAnnotations = sequence->items.rbegin(); itAnnotations != sequence->items.rend(); ++itAnnotations) {
                GIL::DICOM::DicomDataset& base = (*itAnnotations);
                std::string itemName;

                if (IsCodeSequence(base, TAGS::ConceptNameCodeSequence, itemName, "SCPECG", "1.3")) {
                        unsigned int nr = 0;

                        for (; nr < items.size(); nr++) {
                                if (items[nr].first == itemName) {
                                        break;
                                }
                        }

                        if (nr == items.size())
                                continue;

                        std::string val;
                        int groupNr;
                        Get(base.getTag(TAGS::AnnotationGroupNumber),groupNr);

                        if (units[nr].second == "") {
                                if (units[nr].first == base.getTag(TAGS::TemporalRangeType))
                                        val = base.getTag(TAGS::RefSamplePositions);
                        } else if (IsCodeSequence(base, TAGS::MeasurementsUnitsCodeSequence, val, "UCUM", "1.4")) {
                                val = base.getTag(TAGS::NumericValue);
                        }

                        /*int[] tempRWC = ds.GetInts(Tags.RefWaveformChannels);

                        if ((val != null)
                        &&	(groupNr >= 0)
                        &&	(tempRWC.Length == rwc.Length))
                        {
                        	for (int k=0;k < rwc.Length;k++)
                        		if (tempRWC[k] != rwc[k])
                        			continue;

                        	string[] temp = null;

                        	if (sl.Contains(groupNr))
                        		temp = (string[]) sl[groupNr];
                        	else
                        	{
                        		temp = new string[items.GetLength(1)];

                        		sl.Add(groupNr, temp);
                        	}

                        	temp[nr] = val;
                        }*/
                        if (val != "" && groupNr>=0) {
                                if (sl.find(groupNr) == sl.end()) {
                                        sl[groupNr] = TVectorStrings(items.size());
                                }
                                sl[groupNr][nr] = val;
                        }
                }
        }//for

        if (sl.size() > 0) {
                if (special && sl.size() >= 2
                    &&	(items.size() < 64)) {
                        size_t temp1 = items.size();
                        unsigned long a = 0, b = 0;

                        TVectorStrings& temp2 = sl.begin()->second;
                        int i = 0;
                        for (TVectorStrings::iterator it = temp2.begin(); it != temp2.end(); ++it,++i) {
                                if ((*it) != "") {
                                        a |= (1UL << i);
                                }
                        }

                        TMapVectorStrings::iterator itSecond = sl.begin();
                        itSecond++;
                        TVectorStrings& temp3 = itSecond->second;
                        i = 0;
                        for (TVectorStrings::iterator it = temp3.begin(); it != temp3.end(); ++it,++i) {
                                if ((*it) != "") {
                                        b |= (1UL << i);
                                }
                        }

                        if ((a ^ b) == (a + b)) {
                                for (size_t i = 0; i < temp1; i++) {
                                        if (temp2[i] != "") {
                                                temp3[i] = temp2[i];
                                        }
                                }

                                sl.erase(sl.begin());
                        }
                }

                for (TMapVectorStrings::iterator it = sl.begin(); it != sl.end(); ++it) {
                        result[(*it).first] = TVectorStrings();

                        for (TVectorStrings::iterator itStr = (*it).second.begin(); itStr != (*it).second.end(); ++itStr)
                                result[(*it).first].push_back((*itStr));
                        std::ostringstream ostr;
                        ostr << (*it).first;
                        result [(*it).first].push_back(ostr.str());
                }
        }
        return result;
}


///////////////////////////////////////////////////////////////////////
void GNKVisualizator::ChannelInfo::SetLead(const std::string& codingSchemeDesignator, const std::string& codingValue)
{
        if (codingSchemeDesignator == "SCPECG") {
                wxStringTokenizer tkz(wxString::FromUTF8(codingValue.c_str()), wxT("-"));
                //code is the last token
                wxString code(wxT("0"));
                while (tkz.HasMoreTokens()) {
                        code = tkz.GetNextToken();
                }
                long lead = 0;
                if (code.ToLong(&lead) && lead > 0 && lead < 185) {
                        this->LeadType = (TLeadType)(lead);
                }
        } else if (codingSchemeDesignator == "MDC") {

                const char* mdc0[] = { "MDC_ECG_LEAD_CONFIG", "MDC_ECG_LEAD_I", "MDC_ECG_LEAD_II", "MDC_ECG_LEAD_V1", "MDC_ECG_LEAD_V2", "MDC_ECG_LEAD_V3", "MDC_ECG_LEAD_V4", "MDC_ECG_LEAD_V5", "MDC_ECG_LEAD_V6",
                                       "MDC_ECG_LEAD_V7", "MDC_ECG_LEAD_V2R", "MDC_ECG_LEAD_V3R", "MDC_ECG_LEAD_V4R", "MDC_ECG_LEAD_V5R", "MDC_ECG_LEAD_V6R", "MDC_ECG_LEAD_V7R", "MDC_ECG_LEAD_X", "MDC_ECG_LEAD_Y", "MDC_ECG_LEAD_Z",
                                       "MDC_ECG_LEAD_CC5", "MDC_ECG_LEAD_CM5", "MDC_ECG_LEAD_LA", "MDC_ECG_LEAD_RA", "MDC_ECG_LEAD_LL", "MDC_ECG_LEAD_fI", "MDC_ECG_LEAD_fE", "MDC_ECG_LEAD_fC", "MDC_ECG_LEAD_fA", " MDC_ECG_LEAD_fM",
                                       "MDC_ECG_LEAD_fF", "MDC_ECG_LEAD_fH", "MDC_ECG_LEAD_dI", "MDC_ECG_LEAD_dII", "MDC_ECG_LEAD_dV1", "MDC_ECG_LEAD_dV2", "MDC_ECG_LEAD_dV3", "MDC_ECG_LEAD_dV4", "MDC_ECG_LEAD_dV5", "MDC_ECG_LEAD_dV6"
                                     };
                for (int i = 0; i< 39; ++i) {
                        if (codingValue == mdc0[i]) {
                                this->LeadType = (TLeadType)i;
                                return;
                        }
                }
                const char* mdc61[] = {"MDC_ECG_LEAD_III", "MDC_ECG_LEAD_aVR", "MDC_ECG_LEAD_aVL", "MDC_ECG_LEAD_aVF", "MDC_ECG_LEAD_aVRneg", "MDC_ECG_LEAD_V8", "MDC_ECG_LEAD_V9", "MDC_ECG_LEAD_V8R", "MDC_ECG_LEAD_V9R",
                                       "MDC_ECG_LEAD_D", "MDC_ECG_LEAD_A", "MDC_ECG_LEAD_J", "MDC_ECG_LEAD_Defib", " MDC_ECG_LEAD_Extern", "MDC_ECG_LEAD_A1", "MDC_ECG_LEAD_A2", "MDC_ECG_LEAD_A3", "MDC_ECG_LEAD_A4"
                                      };
                for (int i = 0; i< 18; ++i) {
                        if (codingValue == mdc61[i]) {
                                this->LeadType = (TLeadType)(i+61);
                                return;
                        }
                }
                const char* mdc86[] = {"MDC_ECG_LEAD_C", "MDC_ECG_LEAD_V", "MDC_ECG_LEAD_VR", "MDC_ECG_LEAD_VL", "MDC_ECG_LEAD_VF", "MDC_ECG_LEAD_MCL", "MDC_ECG_LEAD_MCL1", "MDC_ECG_LEAD_MCL2", "MDC_ECG_LEAD_MCL3",
                                       "MDC_ECG_LEAD_MCL4", " MDC_ECG_LEAD_MCL5", "MDC_ECG_LEAD_MCL6", "MDC_ECG_LEAD_CC", "MDC_ECG_LEAD_CC1", "MDC_ECG_LEAD_CC2", "MDC_ECG_LEAD_CC3", "MDC_ECG_LEAD_CC4", "MDC_ECG_LEAD_CC6", "MDC_ECG_LEAD_CC7",
                                       "MDC_ECG_LEAD_CM", "MDC_ECG_LEAD_CM1", "MDC_ECG_LEAD_CM2", "MDC_ECG_LEAD_CM3", "MDC_ECG_LEAD_CM4", "MDC_ECG_LEAD_CM6", "MDC_ECG_LEAD_dIII", " MDC_ECG_LEAD_daVR", "MDC_ECG_LEAD_daVL", "MDC_ECG_LEAD_daVF"
                                      };
                for (int i = 0; i< 29; ++i) {
                        if (codingValue == mdc86[i]) {
                                this->LeadType = (TLeadType)(i+86);
                                return;
                        }
                }
                const char* mdc121[] = {"MDC_ECG_LEAD_CM7", "MDC_ECG_LEAD_CH5", "MDC_ECG_LEAD_CS5", "MDC_ECG_LEAD_CB5", "MDC_ECG_LEAD_CR5", "MDC_ECG_LEAD_ML", "MDC_ECG_LEAD_AB1", "MDC_ECG_LEAD_AB2", "MDC_ECG_LEAD_AB3",
                                        "MDC_ECG_LEAD_AB4", "MDC_ECG_LEAD_ES", "MDC_ECG_LEAD_AS", "MDC_ECG_LEAD_AI", "MDC_ECG_LEAD_S"
                                       };
                for (int i = 0; i< 14; ++i) {
                        if (codingValue == mdc121[i]) {
                                this->LeadType = (TLeadType)(i+121);
                                return;
                        }
                }
                const char* mdc147[] = {"MDC_ECG_LEAD_RL", "MDC_ECG_LEAD_CV5RL", "MDC_ECG_LEAD_CV6LL", "MDC_ECG_LEAD_CV6LU", "MDC_ECG_LEAD_V10"
                                       };
                for (int i = 0; i< 5; ++i) {
                        if (codingValue == mdc147[i]) {
                                this->LeadType = (TLeadType)(i+147);
                                return;
                        }
                }
        } else {
                LOG_WARN("EGCStudy", "Unsupported coding scheme designator: " << codingSchemeDesignator);
        }
}

std::string GNKVisualizator::ChannelInfo::GetTitle() const
{
        const char* leadStrings[] = {"Unknown", "I", "II", "V1", "V2", "V3", "V4", "V5", "V6", "V7", "V2R", "V3R", "V4R", "V5R", "V6R", "V7R", "X", "Y", "Z",
                                     "CC5", "CM5", "LA", "RA", "LL", "fI", "fE", "fC", "fA", "fM", "fF", "fH", "dI", "dII", "dV1", "dV2", "dV3", "dV4", "dV5", "dV6",
                                     "dV7", "dV2R", "dV3R", "dV4R", "dV5R", "dV6R", "dV7R", "dX", "dY", "dZ", "dCC5", "dCM5", "dLA", "dRA", "dLL", "dfI", "dfE",
                                     "dfC", "dfA", "dfM", "dfF", "dfH", "III", "aVR", "aVL", "aVF", "aVRneg", "V8", "V9", "V8R", "V9R", "D", "A", "J", "Defib",
                                     "Extern", "A1", "A2", "A3", "A4", "dV8", "dV9", "dV8R", "dV9R", "dD", "dA", "dJ", "Chest", "V", "VR", "VL", "VF", "MCL", "MCL1",
                                     "MCL2", "MCL3", "MCL4", "MCL5", "MCL6", "CC", "CC1", "CC2", "CC3", "CC4", "CC6", "CC7", "CM", "CM1", "CM2", "CM3", "CM4", "CM6",
                                     "dIII", "daVR", "daVL", "daVF", "daVRneg", "dChest", "dV", "dVR", "dVL", "dVF", "CM7", "CH5", "CS5", "CB5", "CR5", "ML", "AB1",
                                     "AB2", "AB3", "AB4", "ES", "AS", "AI", "S", "dDefib", "dExtern", "dA1", "dA2", "dA3", "dA4", "dMCL1", "dMCL2", "dMCL3",
                                     "dMCL4", "dMCL5", "dMCL6", "RL", "CV5RL", "CV6LL", "CV6LU", "V10", "dMCL", "dCC", "dCC1", "dCC2", "dCC3", "dCC4", "dCC6",
                                     "dCC7", " dCM", "dCM1", "dCM2", "dCM3", "dCM4", "dCM6", "dCM7", "dCH5", "dCS5", "dCB5", "dCR5", "dML", "dAB1", "dAB2", "dAB3",
                                     "dAB4", "dES", "dAS", "dAI", "dS", "dRL", "dCV5RL", "dCV6LL", "dCV6LU", "dV10"
                                    };
        return std::string(leadStrings[(int)LeadType]);
}

void GNKVisualizator::ChannelInfo::SetUnitsValue(const std::string& unitsvalue)
{
        UnitsValue = unitsvalue;
        if (UnitsValue == "mV") {
                Multiplier = 100.0;
        } else if (UnitsValue == "uV") {
                Multiplier = 0.1;
        } else if (UnitsValue == "MV" ) {
                Multiplier = 100000000000.0;
        } else if (UnitsValue == "kV") {
                Multiplier = 100000000.0;
        } else if (UnitsValue ==  "V") {
                Multiplier = 100000.0;
        } else if (UnitsValue == "dV") {
                Multiplier = 10000.0;
        } else if (UnitsValue == "cV") {
                Multiplier = 1000.0;
        } else {
                std::ostringstream ostr;
                ostr << _Std("We are working to support ") << UnitsValue;
                throw GNC::GCS::ControladorCargaException(ostr.str(), "ECGStudy");
        }
}


///////////////////////////////////////////////////////////////////////

GNKVisualizator::ECGStudy::ECGStudy()
{
        GTRACE(">> ECGStudy::ECGStudy()" << this);
        GTRACE("<< ECGStudy::ECGStudy()" << this);
}

GNKVisualizator::ECGStudy::~ECGStudy()
{
        GTRACE(">> ECGStudy::~ECGStudy()" << this);
        ECGFiles.clear();
        GTRACE("<< ECGStudy::~ECGStudy()" << this);
}


void GNKVisualizator::ECGStudy::InitializeContext(long pkSeries)
{
        GNC::GCS::IHistoryController::LightFileModelList fileModels;
        GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels(pkSeries, fileModels);

        std::list<std::string> rutas;
        for (GNC::GCS::IHistoryController::LightFileModelList::iterator itFile = fileModels.begin(); itFile != fileModels.end(); ++itFile) {
                rutas.push_back((*itFile).real_path);
        }

        GNC::GCS::IStudyContext::DoInitiallizeContext(rutas);

        for (int i = 0; i < (int)rutas.size(); ++i) {
                ECGFiles.push_back(NULL);
        }
}

void GNKVisualizator::ECGStudy::SetIndexOfActualMultiplexGroup(int index)
{
        LoadChannels();
        ECGFiles[ActiveFileIndex]->SetIndexOfActualMultiplexGroup(index);
}

GNKVisualizator::ECGStudy::TListChannelInfo& GNKVisualizator::ECGStudy::GetListOfChannels()
{
        LoadChannels();
        return ECGFiles[ActiveFileIndex]->GetListOfChannels();
}
bool GNKVisualizator::ECGStudy::Ok()
{
        LoadChannels();
        return ECGFiles[ActiveFileIndex]->Groups.size() > 0;
}

std::list<std::string> GNKVisualizator::ECGStudy::GetLabelOfMultiplexGroups()
{
        LoadChannels();
        return ECGFiles[ActiveFileIndex]->GetLabelOfMultiplexGroups();
}

const std::string GNKVisualizator::ECGStudy::GetGlobalMeasurement()
{
        LoadChannels();
        std::ostringstream ostr;
        if (ECGFiles[ActiveFileIndex]->globalMeasurements.IsValid()) {
                GNKVisualizator::GlobalMeasurements& mes = (*ECGFiles[ActiveFileIndex]->globalMeasurements);

                int ventRate = (mes.getVentRate() == MeasurementNoValue) ? 0 : (int) mes.getVentRate(),
                    PRint = (mes.getPRint() == MeasurementNoValue) ? 0 : (int) mes.measurment[0]->PRint(),
                    QRSdur = (mes.getQRSdur() == MeasurementNoValue) ? 0 : (int) mes.measurment[0]->QRSdur(),
                    QT = (mes.getQTdur() == MeasurementNoValue) ? 0 : (int) mes.measurment[0]->QTdur(),
                    QTc = (mes.getQTc() == MeasurementNoValue) ? 0 : (int) mes.getQTc();

                ostr << _Std("Vent rate:      ");
                ostr << ventRate;
                ostr << _Std(" BPM") << std::endl;

                ostr << _Std("PR int:      ");
                ostr << PRint;
                ostr << _Std(" ms") << std::endl;

                ostr << _Std("QRS dur:      ");
                ostr << QRSdur;
                ostr << _Std(" ms") << std::endl;

                ostr << _Std("QT\\QTc:      ");
                ostr << QT << "/" << QTc;
                ostr << _Std(" ms") << std::endl;

                ostr << _Std("P-R-T axes: ");
                if (mes.measurment[0]->Paxis != MeasurementNoAxisValue) {
                        ostr << mes.measurment[0]->Paxis;
                } else {
                        ostr << 999;
                }
                ostr << ' ';
                if (mes.measurment[0]->QRSaxis != MeasurementNoAxisValue) {
                        ostr << mes.measurment[0]->QRSaxis;
                } else {
                        ostr << 999;
                }
                ostr << ' ';
                if (mes.measurment[0]->Taxis != MeasurementNoAxisValue) {
                        ostr << mes.measurment[0]->Taxis;
                } else {
                        ostr << 999;
                }
        } else {
                ostr << _Std("Measurements not found");
        }
        return ostr.str();
}

const std::string& GNKVisualizator::ECGStudy::GetDiagnosis()
{
        LoadChannels();
        return ECGFiles[ActiveFileIndex]->GetDiagnosis();
}

std::string GNKVisualizator::ECGStudy::getMeasureText(bool isVertical, float measure)
{
        std::ostringstream ostr;
        ostr.setf(ios::floatfield, ios::fixed );
        ostr.precision(2);
        ostr.fill('0');

        TListChannelInfo& chanelInfo = GetListOfChannels();
        if (!chanelInfo.empty()) {
                //ChannelInfo& frontValue = chanelInfo.front();
                if (isVertical) {
                        ostr << (measure /10) << " mV";
                } else {
                        //siempre esta a 25 mm/s
                        ostr << (measure /(  25 )) << " s";
                }
        } else {
                LOG_ERROR("ECGStudy", "Error getting measure");
        }

        return ostr.str();
}

void GNKVisualizator::ECGStudy::LoadChannels()
{
        if (!ECGFiles[ActiveFileIndex].IsValid()) {

                GNC::GCS::Ptr<TECGFile> ecgFile(new TECGFile);
                ECGFiles[ActiveFileIndex] = ecgFile;

                GNC::GCS::Ptr<GIL::DICOM::DicomDataset> base(new GIL::DICOM::DicomDataset());
                GIL::DICOM::IDICOMManager* pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();

                GIL::DICOM::TipoMetaInfo metaInfo;

                pDICOMManager->CargarMetaInfo(GetPathActiveImage(), metaInfo);

                std::string sopClassUID = metaInfo.getTag(TAGS::MediaStorageSOPClassUID);

                std::vector<GIL::DICOM::TagPrivadoUndefined> tag;

                if (sopClassUID == UIDS::GeneralECGWaveformStorage ||
                    sopClassUID == UIDS::HemodynamicWaveformStorage ||
                    sopClassUID == UIDS::TwelveLeadECGWaveformStorage ) {

                        pDICOMManager->CargarFichero(GetPathActiveImage(), *base, false);
                        bool tag_found = pDICOMManager->FindTag(0x5400, 0x1010, tag);

                        GNC::GCS::IEntorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
                        if (!tag_found) {
                                LOG_WARN("ECGStudy","Got data set that should contain waveform data, but the tag (0x5400, 0x1010) is not available");
                                return;
                        }

                } else {
                        GNC::GCS::IEntorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
                        LOG_INFO("ECGStudy::LoadChannels", "No waveform found");
                        return;
                }

                GIL::DICOM::DicomDataset* waveformSeq = base->buscar_secuencia(TAGS::WaveFormSequence);

                if (!waveformSeq) {
                        LOG_INFO("ECGStudy::LoadChannels", "No waveform sequence in base");
                        return;
                }

                int numChannels = 0;
                int numSamples = 0;
                double samplingFreq = 0.0;
                short paddingValue = 0;
                std::string orig("unknown");
                std::string groupLabel("unknown");
                int bitsAllocated;
                std::string waveFormSampleInterpretation("unknown");

                int waveform = 0;
                for (auto itSequences = waveformSeq->items.rbegin(); itSequences != waveformSeq->items.rend(); ++itSequences, ++waveform) {
                        //multiplex group
                        GIL::DICOM::DicomDataset& wf = (*itSequences);
                        ecgFile->Groups.push_back(TMultiplexGroup());
                        TMultiplexGroup& group = ecgFile->Groups.back();

                        Get(wf.getTag(TAGS::NumberOfWaveformChannels), numChannels);
                        LOG_INFO("ECGStudy::LoadChannels", "numChannels=" << numChannels);

                        Get(wf.getTag(TAGS::NumberOfWaveformSamples), numSamples);
                        LOG_INFO("ECGStudy::LoadChannels", "numSamples=" << numSamples);

                        Get(wf.getTag(TAGS::SamplingFrequency), samplingFreq);
                        LOG_INFO("ECGStudy::LoadChannels", "samplingFreq=" << samplingFreq);

                        //FIXME mal no pilla bien el padding value porque es de tipo ox
                        Get(wf.getTag(TAGS::WaveformPaddingValue), paddingValue);
                        LOG_INFO("ECGStudy::LoadChannels", "paddingValue=" << paddingValue);

                        if (!wf.getTag(TAGS::WaveformOriginality, orig))
                                LOG_INFO("ECGStudy", "Tag " << TAGS::WaveformOriginality << "not found");
                        if (!wf.getTag(TAGS::MultiplexGroupLabel, groupLabel))
                                LOG_INFO("ECGStudy", "Tag " << TAGS::MultiplexGroupLabel << "not found");
                        Get(wf.getTag(TAGS::WaveformBitsAllocated), bitsAllocated);
                        if (!wf.getTag(TAGS::WaveformSampleInterpretation, waveFormSampleInterpretation))
                                LOG_INFO("ECGStudy", "Tag " << TAGS::WaveformSampleInterpretation << "not found");
                        if (bitsAllocated != 16 || waveFormSampleInterpretation != "SS") {
                                LOG_INFO("ECGStudy", "We are unnable to interpret bitsAllocated=" << bitsAllocated << " waveFormSampleInterpretation=" << waveFormSampleInterpretation);
                                throw GNC::GCS::ControladorCargaException(_Std("ECG format not supported"), "ECGStudy");
                        }

                        if (groupLabel != "") {
                                group.Label = groupLabel;
                        }

                        //LOG_INFO("ECGStudy::LoadChannels","waveform=" << wf);

                        GIL::DICOM::DicomDataset* channel_def_seq = wf.buscar_secuencia(TAGS::ChannelDefinitionSequence);

                        if (!channel_def_seq) {
                                LOG_INFO("ECGStudy::LoadChannels", "wf.buscar_secuencia(TAGS::ChannelDefinitionSequence) returned NULL");
                        }

                        if (channel_def_seq->items.size() == (unsigned int) numChannels) {

                                int i = 0;
                                for (auto  it = channel_def_seq->items.rbegin(); it != channel_def_seq->items.rend(); ++it, i++) {
                                        LOG_INFO("ECGStudy::LoadChannels", "Read channel_def_seq=" << i);


                                        group.Channels.push_back(GNKVisualizator::ChannelInfo());

                                        GIL::DICOM::DicomDataset& channel_def = (*it);

                                        //>> SQ Items (1): (003A,0208) - Channel Source Sequence
                                        //   > (0008|0104, LO, "Lead I")  # 0x6 - Code Meaning  OK
                                        //   > (003A|0210, DS, "0.00122")  # 0x8 - Channel sensitivity  OK


                                        GIL::DICOM::DicomDataset* channel_src_seq = channel_def.buscar_secuencia(TAGS::ChannelSourceSequence);

                                        //>> SQ Items (1): (003A,0211) - Channel sensitivity units
                                        //  > (0008|0100, SH, "mV")  # 0x2 - Code Value  OK
                                        //  > (0008|0104, LO, "millivolt")  # 0xA - Code Meaning  OK
                                        //  > (003A|0212, DS, "1")  # 0x2 - Sensitivity correction factor  OK
                                        //  > (003A|0213, DS, "0")  # 0x2 - Channel baseline  OK
                                        //  > (003A|0214, DS, "0")  # 0x2 - Channel Time skew  OK
                                        //  > (003A|021A, US, 0x0010)  # 0x2 - Bits per sample  OK
                                        //  > (003A|0220, DS, ".05")  # 0x4 - Filter low frequency  OK
                                        //  > (003A|0221, DS, "100")  # 0x4 - filter high frequency  OK
                                        GIL::DICOM::DicomDataset* channel_sen_seq = channel_def.buscar_secuencia("003a|0211");




                                        if ( channel_src_seq != NULL && channel_src_seq->items.size() > 0 &&
                                             channel_sen_seq != NULL && channel_sen_seq->items.size() > 0) {
                                                GIL::DICOM::DicomDataset& channel_src = channel_src_seq->items.front();
                                                GIL::DICOM::DicomDataset& channel_sen = channel_sen_seq->items.front();

                                                ChannelInfo& channelInfo = group.Channels[i];

                                                channelInfo.NumSamples = numSamples;
                                                channelInfo.SamplingFrecuency = samplingFreq;

                                                std::string codingValue = channel_src.getTag("0008|0100");
                                                std::string codingSchemeDesignator = channel_src.getTag("0008|0102");
                                                channelInfo.SetLead                       (codingSchemeDesignator, codingValue);

                                                channelInfo.SetUnitsValue                  (channel_sen.getTag                ("0008|0100"));
                                                channelInfo.SetUnitsMeaning                (channel_sen.getTag                ("0008|0104"));

                                                channelInfo.SetSensitivity                 (channel_def.getTagAs<double>      ("003a|0210", 1.0));
                                                channelInfo.SetSensitivityCorrectionFactor (channel_def.getTagAs<double>      ("003a|0212", 1.0));
                                                channelInfo.SetChannelBaseline             (channel_def.getTagAs<double>      ("003a|0213", 0.0));
                                                channelInfo.SetChannelTimeSkew             (channel_def.getTagAs<double>      ("003a|0214", 0.0));
                                                channelInfo.SetBitsPerSample               (channel_def.getTagAs<unsigned int>("003a|021a", 16));
                                                channelInfo.SetFilterLowFrequency          (channel_def.getTagAs<double>      ("003a|0220", 0.0));
                                                channelInfo.SetFilterHighFrequency         (channel_def.getTagAs<double>      ("003a|0221", 0.0));



                                        }else{
                                                LOG_ERROR("ECGStudy::LoadChannels", "waveform not found");
                                        }
                                }

                                GIL::DICOM::TagPrivadoUndefined& samples = tag[waveform];
                                auto nSamples = samples.GetSize() / 2;
                                LOG_INFO("ECGStudy::LoadChannels", "Get " << nSamples << "from waveform "  << waveform
                                         << " need " << numSamples * group.Channels.size());

                                if (nSamples >=  numSamples * group.Channels.size()) {
                                        short* data = (short*)samples.GetValor();
                                        unsigned int samplepos = 0;
                                        for (int i = 0; i < numSamples; i++) {
                                                for (TListChannelInfo::iterator it = group.Channels.begin(); it != group.Channels.end(); ++it, ++samplepos) {
                                                        it->Samples.push_back(*data++);
                                                }
                                        }
                                        LOG_INFO("ECGStudy::LoadChannels", "copied " << samplepos << " samples");
                                }else{
                                        LOG_ERROR("ECGStudy::LoadChannels", "Didn't get waveform samples as expected: expect "
                                                  << numSamples * group.Channels.size()<< " bytes, got " <<  nSamples );
                                }


                        }else {
                                LOG_INFO("ECGStudy::LoadChannels", "Not reading data!");
                        }

                }//for secuencias waveforms
                //annotation
                {
                        GIL::DICOM::DicomDataset* annotation_seq = base->buscar_secuencia(TAGS::AnnotationSequence);
                        //TODO read referenced channel to read information about each channel

                        if (annotation_seq != NULL) {
                                //for each annotation group one list of strings
                                typedef std::map<int, std::list<std::string> > TMapMultiplexedGroup;
                                typedef std::map<int, TMapMultiplexedGroup> TMapAnnotations;
                                TMapAnnotations annotationsMap;
                                for (GIL::DICOM::DicomDataset::DatasetList::reverse_iterator itAnnotations = annotation_seq->items.rbegin(); itAnnotations != annotation_seq->items.rend(); ++itAnnotations) {
                                        GIL::DICOM::DicomDataset& an = (*itAnnotations);
                                        std::string textValue = an.getTag(TAGS::UnformattedTextValue);

                                        if (textValue != "") {
                                                int group = 0, multiplexedGroup = 0;
                                                Get(an.getTag(TAGS::AnnotationGroupNumber), group);
                                                Get(an.getTag(TAGS::ReferencedChannels), multiplexedGroup);
                                                multiplexedGroup -= 1;
                                                annotationsMap[multiplexedGroup][group].push_back(textValue);
                                        }
                                }//for annotations


                                for (TMapAnnotations::iterator itAnnotations = annotationsMap.begin(); itAnnotations != annotationsMap.end(); ++itAnnotations) {
                                        for (int i = 0; i < (int)ecgFile->Groups.size(); ++i) {
                                                if ((*itAnnotations).second.find(i) != (*itAnnotations).second.end()) {
                                                        TMapMultiplexedGroup& annotationsMultiplexed = (*itAnnotations).second;
                                                        std::ostringstream ostr;
                                                        for (TMapMultiplexedGroup::iterator itGroups = annotationsMultiplexed.begin(); itGroups != annotationsMultiplexed.end(); ++itGroups) {
                                                                for (std::list<std::string>::iterator itStrings = (*itGroups).second.begin(); itStrings != (*itGroups).second.end(); ++itStrings) {
                                                                        ostr << (*itStrings) << std::endl;
                                                                }
                                                                ostr << std::endl;
                                                        }
                                                        ecgFile->Groups[i].Diagnosis = ostr.str();
                                                }
                                        }
                                }
                                /////////////////////////////////////////////////////////
                                ////////////////GLOBAL MEASUREMENTS//////////////////////
                                /////////////////////////////////////////////////////////
                                std::string s_AvgRRPPItemsCodes[] = {"5.10.2.1-3", "5.10.2.1-5", "5.10.2.5-5", "5.10.2.5-1"};
                                std::string s_AvgRRPPItemsMeaning[] = {_Std("RR Interval"), _Std("PP Interval"), _Std("QTc Interval"), _Std("Vent Rate")};
                                TVectorPairStrings s_AvgRRPPItems;
                                for (int i = 0; i < 4; ++i) {
                                        s_AvgRRPPItems.push_back(std::pair<std::string, std::string>(s_AvgRRPPItemsCodes[i], s_AvgRRPPItemsMeaning[i]));
                                }
                                std::string s_AvgRRPPUnitsCodes[] = {"ms", "ms", "ms", "/min"};
                                std::string s_AvgRRPPUnitsMeaning[] = {"milliseconds", "milliseconds", "milliseconds", "heartbeat per minute"};
                                TVectorPairStrings s_AvgRRPPUnits;
                                for (int i = 0; i < 4; ++i) {
                                        s_AvgRRPPUnits.push_back(std::pair<std::string, std::string>(s_AvgRRPPUnitsCodes[i], s_AvgRRPPUnitsMeaning[i]));
                                }
                                std::string s_MeasurementItemsCodes[] = {"5.10.3-1", "5.10.3-2", "5.10.3-3", "5.10.3-4", "5.10.3-5", "5.10.3-11", "5.10.3-13", "5.10.3-15"};
                                std::string s_MeasurementItemsMeaning[] = {"P onset", "P offset", "QRS onset", "QRS offset", "T offset", "P Axis", "QRS Axis", "T Axis"};
                                TVectorPairStrings s_MeasurementItems;
                                for (int i = 0; i < 8; ++i) {
                                        s_MeasurementItems.push_back(std::pair<std::string, std::string>(s_MeasurementItemsCodes[i], s_MeasurementItemsMeaning[i]));
                                }
                                std::string s_MeasurementUnitsCode[] = {"ms", "ms", "ms", "ms", "ms", "deg", "deg", "deg"};
                                std::string s_MeasurementUnitsValue[]= {"milliseconds", "milliseconds", "milliseconds", "milliseconds", "milliseconds", "degrees", "degrees", "degrees"};
                                TVectorPairStrings s_MeasurementUnits;
                                for (int i = 0; i < 8; ++i) {
                                        s_MeasurementUnits.push_back(std::pair<std::string, std::string>(s_MeasurementUnitsCode[i], s_MeasurementUnitsValue[i]));
                                }
                                std::string s_MeasurementUnitsPointsCode[] = {"POINT", "POINT", "POINT", "POINT", "POINT", "deg", "deg", "deg"};
                                std::string s_MeasurementUnitsPointsValue[]= {"", "", "", "", "", "degrees", "degrees", "degrees"};
                                TVectorPairStrings s_MeasurementUnitsPoints;
                                for (int i = 0; i < 8; ++i) {
                                        s_MeasurementUnitsPoints.push_back(std::pair<std::string, std::string>(s_MeasurementUnitsPointsCode[i], s_MeasurementUnitsPointsValue[i]));
                                }
                                std::list<int> s_MeasurementRWC;
                                s_MeasurementRWC.push_back(1);
                                s_MeasurementRWC.push_back(0);

                                TMapVectorStrings resultAvgRR_PP = GetValues(annotation_seq, s_AvgRRPPItems, s_AvgRRPPUnits, s_MeasurementRWC, false);
                                TMapVectorStrings resultMeasurments = GetValues(annotation_seq, s_MeasurementItems, s_MeasurementUnits, s_MeasurementRWC, true);

                                float factor = 1.0f;

                                if (!resultAvgRR_PP.empty()) {
                                        TMapVectorStrings temp1 = GetValues(annotation_seq, s_MeasurementItems, s_MeasurementUnitsPoints, s_MeasurementRWC, true);

                                        if ((!temp1.empty()) && ((!resultMeasurments.empty())	|| (resultMeasurments.size() < temp1.size()) || (calcNrOfValues(resultMeasurments) < calcNrOfValues(temp1)))) {
                                                if (!ecgFile->Groups.empty()) {
                                                        factor = 1000.0f / ecgFile->Groups[0].Channels[0].SamplingFrecuency;
                                                }

                                                resultMeasurments = temp1;
                                        }
                                }

                                if ( (!resultAvgRR_PP.empty()) && (!resultMeasurments.empty()) ) {
                                        ecgFile->globalMeasurements = GNC::GCS::Ptr<GlobalMeasurements>(new GlobalMeasurements());
                                        GNKVisualizator::GlobalMeasurements& mes = (*ecgFile->globalMeasurements);

                                        TVectorStrings& resultVector = resultAvgRR_PP.begin()->second;

                                        if (resultVector.size() >= 1) {
                                                if (resultVector[0] != "") {
                                                        Get(resultVector[0], mes.AvgRR);
                                                }

                                                if (resultVector[1] != "") {
                                                        Get(resultVector[1], mes.AvgPP);
                                                }

                                                if (resultVector[2] != "") {
                                                        unsigned short qtc;
                                                        Get(resultVector[2], qtc);
                                                        mes.setQTc(qtc);
                                                }

                                                if (resultVector[3] != "") {
                                                        unsigned short ventRate;
                                                        Get(resultVector[3], ventRate);
                                                        mes.setVentRate(ventRate);
                                                }
                                        }

                                        if ( (!resultMeasurments.empty()) && ( ((*resultMeasurments.begin()).second.size() -1) == 8) ) {
                                                for (TMapVectorStrings::iterator itMap = resultMeasurments.begin(); itMap != resultMeasurments.end(); ++itMap) {
                                                        GNC::GCS::Ptr<GNKVisualizator::GlobalMeasurement> measurement(new GNKVisualizator::GlobalMeasurement());

                                                        TVectorStrings& vectorStrings = (*itMap).second;
                                                        unsigned short tempUnsignedShort;
                                                        short tempShort;

                                                        Get(vectorStrings[0], tempUnsignedShort);
                                                        measurement->Ponset = (unsigned short)(tempUnsignedShort*factor);
                                                        Get(vectorStrings[1], tempUnsignedShort);
                                                        measurement->Poffset = (unsigned short)(tempUnsignedShort*factor);
                                                        Get(vectorStrings[2], tempUnsignedShort);
                                                        measurement->QRSonset = (unsigned short)(tempUnsignedShort*factor);
                                                        Get(vectorStrings[3], tempUnsignedShort);
                                                        measurement->QRSoffset = (unsigned short)(tempUnsignedShort*factor);
                                                        Get(vectorStrings[4], tempUnsignedShort);
                                                        measurement->Toffset = (unsigned short)(tempUnsignedShort*factor);
                                                        Get(vectorStrings[5], tempShort);
                                                        measurement->Paxis = tempShort;
                                                        Get(vectorStrings[6], tempShort);
                                                        measurement->QRSaxis = tempShort;
                                                        Get(vectorStrings[7], tempShort);
                                                        measurement->Taxis = tempShort;


                                                        mes.measurment.push_back(measurement);
                                                }
                                        }
                                }

                        }//annotation_seq != NULL
                }//annotation
        }else{
                LOG_DEBUG("ECGStudy::LoadChannels", "ECGFiles[ActiveFileIndex].IsValid() = true");
        }
}


