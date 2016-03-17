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

#pragma once
/*
** String Constants
*/

/*
** Defined Transfer Syntax UIDs
*/

/// Implicit VR Little Endian: Default Transfer Syntax for DICOM
#define GKUID_LittleEndianImplicitTransferSyntax  "1.2.840.10008.1.2"
/// Explicit VR Little Endian
#define GKUID_LittleEndianExplicitTransferSyntax  "1.2.840.10008.1.2.1"
/// Explicit VR Big Endian
#define GKUID_BigEndianExplicitTransferSyntax     "1.2.840.10008.1.2.2"
/// Deflated Explicit VR Little Endian
#define GKUID_DeflatedExplicitVRLittleEndianTransferSyntax "1.2.840.10008.1.2.1.99"
/** JPEG Baseline (Process 1): Default Transfer Syntax
 * for Lossy JPEG 8 Bit Image Compression
 */
#define GKUID_JPEGProcess1TransferSyntax          "1.2.840.10008.1.2.4.50"
/** JPEG Extended (Process 2 & 4): Default Transfer Syntax
 *  for Lossy JPEG 12 Bit Image Compression (Process 4 only)
 */
#define GKUID_JPEGProcess2_4TransferSyntax        "1.2.840.10008.1.2.4.51"
/// JPEG Extended (Process 3 & 5) - RETIRED
#define GKUID_JPEGProcess3_5TransferSyntax        "1.2.840.10008.1.2.4.52"
/// JPEG Spectral Selection, Non-Hierarchical (Process 6 & 8) - RETIRED
#define GKUID_JPEGProcess6_8TransferSyntax        "1.2.840.10008.1.2.4.53"
/// JPEG Spectral Selection, Non-Hierarchical (Process 7 & 9) - RETIRED
#define GKUID_JPEGProcess7_9TransferSyntax        "1.2.840.10008.1.2.4.54"
/// JPEG Full Progression, Non-Hierarchical (Process 10 & 12) - RETIRED
#define GKUID_JPEGProcess10_12TransferSyntax      "1.2.840.10008.1.2.4.55"
/// JPEG Full Progression, Non-Hierarchical (Process 11 & 13) - RETIRED
#define GKUID_JPEGProcess11_13TransferSyntax      "1.2.840.10008.1.2.4.56"
/// JPEG Lossless, Non-Hierarchical (Process 14)
#define GKUID_JPEGProcess14TransferSyntax         "1.2.840.10008.1.2.4.57"
/// JPEG Lossless, Non-Hierarchical (Process 15) - RETIRED
#define GKUID_JPEGProcess15TransferSyntax         "1.2.840.10008.1.2.4.58"
/// JPEG Extended, Hierarchical (Process 16 & 18) - RETIRED
#define GKUID_JPEGProcess16_18TransferSyntax      "1.2.840.10008.1.2.4.59"
/// JPEG Extended, Hierarchical (Process 17 & 19) - RETIRED
#define GKUID_JPEGProcess17_19TransferSyntax      "1.2.840.10008.1.2.4.60"
/// JPEG Spectral Selection, Hierarchical (Process 20 & 22) - RETIRED
#define GKUID_JPEGProcess20_22TransferSyntax      "1.2.840.10008.1.2.4.61"
/// JPEG Spectral Selection, Hierarchical (Process 21 & 23) - RETIRED
#define GKUID_JPEGProcess21_23TransferSyntax      "1.2.840.10008.1.2.4.62"
/// JPEG Full Progression, Hierarchical (Process 24 & 26) - RETIRED
#define GKUID_JPEGProcess24_26TransferSyntax      "1.2.840.10008.1.2.4.63"
/// JPEG Full Progression, Hierarchical (Process 25 & 27) - RETIRED
#define GKUID_JPEGProcess25_27TransferSyntax      "1.2.840.10008.1.2.4.64"
/// JPEG Lossless, Hierarchical (Process 28) - RETIRED
#define GKUID_JPEGProcess28TransferSyntax         "1.2.840.10008.1.2.4.65"
/// JPEG Lossless, Hierarchical (Process 29) - RETIRED
#define GKUID_JPEGProcess29TransferSyntax         "1.2.840.10008.1.2.4.66"
/** JPEG Lossless, Non-Hierarchical, First-Order Prediction (Process 14
 *  [Selection Value 1]): Default Transfer Syntax for Lossless JPEG Image Compression
 */
#define GKUID_JPEGProcess14SV1TransferSyntax      "1.2.840.10008.1.2.4.70"
/// JPEG-LS Lossless Image Compression
#define GKUID_JPEGLSLosslessTransferSyntax        "1.2.840.10008.1.2.4.80"
/// JPEG-LS Lossy (Near-Lossless) Image Compression
#define GKUID_JPEGLSLossyTransferSyntax           "1.2.840.10008.1.2.4.81"
/// JPEG 2000 Image Compression (Lossless Only)
#define GKUID_JPEG2000LosslessOnlyTransferSyntax  "1.2.840.10008.1.2.4.90"
/// JPEG 2000 Image Compression (Lossless or Lossy)
#define GKUID_JPEG2000TransferSyntax              "1.2.840.10008.1.2.4.91"
/// JPEG 2000 Part 2 Multi-component Image Compression (Lossless Only)
#define GKUID_JPEG2000Part2MulticomponentImageCompressionLosslessOnlyTransferSyntax "1.2.840.10008.1.2.4.92"
/// JPEG 2000 Part 2 Multi-component Image Compression (Lossless or Lossy)
#define GKUID_JPEG2000Part2MulticomponentImageCompressionTransferSyntax "1.2.840.10008.1.2.4.93"
/// JPIP Referenced
#define GKUID_JPIPReferencedTransferSyntax        "1.2.840.10008.1.2.4.94"
/// JPIP Referenced Deflate
#define GKUID_JPIPReferencedDeflateTransferSyntax "1.2.840.10008.1.2.4.95"
/// MPEG2 Main Profile @ Main Level
#define GKUID_MPEG2MainProfileAtMainLevelTransferSyntax "1.2.840.10008.1.2.4.100"
/// MPEG2 Main Profile @ High Level
#define GKUID_MPEG2MainProfileAtHighLevelTransferSyntax "1.2.840.10008.1.2.4.101"
/// RLE Lossless
#define GKUID_RLELosslessTransferSyntax           "1.2.840.10008.1.2.5"

/** MIME encapsulation (Supplement 101) is only a pseudo transfer syntax used to
 *  refer to MIME encapsulated HL7 CDA documents from a DICOMDIR when stored
 *  on a DICOM storage medium. It is never used for network communication
 *  or encoding of DICOM objects.
 */
#define GKUID_RFC2557MIMEEncapsulationTransferSyntax "1.2.840.10008.1.2.6.1"

/** XML encoding (Supplement 114) is only a pseudo transfer syntax used to refer to
 *  encapsulated HL7 CDA documents from a DICOMDIR when stored on a DICOM storage
 *  medium. It is never used for network communication or encoding of DICOM objects.
 */
#define GKUID_XMLEncodingTransferSyntax "1.2.840.10008.1.2.6.2"

/*
** Defined SOP Class UIDs according to DICOM standard
*/

// Storage
#define GKUID_RETIRED_StoredPrintStorage                             "1.2.840.10008.5.1.1.27"
#define GKUID_RETIRED_HardcopyGrayscaleImageStorage                  "1.2.840.10008.5.1.1.29"
#define GKUID_RETIRED_HardcopyColorImageStorage                      "1.2.840.10008.5.1.1.30"
#define GKUID_ComputedRadiographyImageStorage                        "1.2.840.10008.5.1.4.1.1.1"
#define GKUID_DigitalXRayImageStorageForPresentation                 "1.2.840.10008.5.1.4.1.1.1.1"
#define GKUID_DigitalXRayImageStorageForProcessing                   "1.2.840.10008.5.1.4.1.1.1.1.1"
#define GKUID_DigitalMammographyXRayImageStorageForPresentation      "1.2.840.10008.5.1.4.1.1.1.2"
#define GKUID_DigitalMammographyXRayImageStorageForProcessing        "1.2.840.10008.5.1.4.1.1.1.2.1"
#define GKUID_DigitalIntraOralXRayImageStorageForPresentation        "1.2.840.10008.5.1.4.1.1.1.3"
#define GKUID_DigitalIntraOralXRayImageStorageForProcessing          "1.2.840.10008.5.1.4.1.1.1.3.1"
#define GKUID_CTImageStorage                                         "1.2.840.10008.5.1.4.1.1.2"
#define GKUID_EnhancedCTImageStorage                                 "1.2.840.10008.5.1.4.1.1.2.1"
#define GKUID_RETIRED_UltrasoundMultiframeImageStorage               "1.2.840.10008.5.1.4.1.1.3"
#define GKUID_UltrasoundMultiframeImageStorage                       "1.2.840.10008.5.1.4.1.1.3.1"
#define GKUID_MRImageStorage                                         "1.2.840.10008.5.1.4.1.1.4"
#define GKUID_EnhancedMRImageStorage                                 "1.2.840.10008.5.1.4.1.1.4.1"
#define GKUID_MRSpectroscopyStorage                                  "1.2.840.10008.5.1.4.1.1.4.2"
#define GKUID_EnhancedMRColorImageStorage                            "1.2.840.10008.5.1.4.1.1.4.3"
#define GKUID_RETIRED_NuclearMedicineImageStorage                    "1.2.840.10008.5.1.4.1.1.5"
#define GKUID_RETIRED_UltrasoundImageStorage                         "1.2.840.10008.5.1.4.1.1.6"
#define GKUID_UltrasoundImageStorage                                 "1.2.840.10008.5.1.4.1.1.6.1"
#define GKUID_EnhancedUSVolumeStorage                                "1.2.840.10008.5.1.4.1.1.6.2"
#define GKUID_SecondaryCaptureImageStorage                           "1.2.840.10008.5.1.4.1.1.7"
#define GKUID_MultiframeSingleBitSecondaryCaptureImageStorage        "1.2.840.10008.5.1.4.1.1.7.1"
#define GKUID_MultiframeGrayscaleByteSecondaryCaptureImageStorage    "1.2.840.10008.5.1.4.1.1.7.2"
#define GKUID_MultiframeGrayscaleWordSecondaryCaptureImageStorage    "1.2.840.10008.5.1.4.1.1.7.3"
#define GKUID_MultiframeTrueColorSecondaryCaptureImageStorage        "1.2.840.10008.5.1.4.1.1.7.4"
#define GKUID_RETIRED_StandaloneOverlayStorage                       "1.2.840.10008.5.1.4.1.1.8"
#define GKUID_RETIRED_StandaloneCurveStorage                         "1.2.840.10008.5.1.4.1.1.9"
#define GKUID_TwelveLeadECGWaveformStorage                           "1.2.840.10008.5.1.4.1.1.9.1.1"
#define GKUID_GeneralECGWaveformStorage                              "1.2.840.10008.5.1.4.1.1.9.1.2"
#define GKUID_AmbulatoryECGWaveformStorage                           "1.2.840.10008.5.1.4.1.1.9.1.3"
#define GKUID_HemodynamicWaveformStorage                             "1.2.840.10008.5.1.4.1.1.9.2.1"
#define GKUID_CardiacElectrophysiologyWaveformStorage                "1.2.840.10008.5.1.4.1.1.9.3.1"
#define GKUID_BasicVoiceAudioWaveformStorage                         "1.2.840.10008.5.1.4.1.1.9.4.1"
#define GKUID_GeneralAudioWaveformStorage                            "1.2.840.10008.5.1.4.1.1.9.4.2"
#define GKUID_ArterialPulseWaveformStorage                           "1.2.840.10008.5.1.4.1.1.9.5.1"
#define GKUID_RespiratoryWaveformStorage                             "1.2.840.10008.5.1.4.1.1.9.6.1"
#define GKUID_RETIRED_StandaloneModalityLUTStorage                   "1.2.840.10008.5.1.4.1.1.10"
#define GKUID_RETIRED_StandaloneVOILUTStorage                        "1.2.840.10008.5.1.4.1.1.11"
#define GKUID_GrayscaleSoftcopyPresentationStateStorage              "1.2.840.10008.5.1.4.1.1.11.1"
#define GKUID_ColorSoftcopyPresentationStateStorage                  "1.2.840.10008.5.1.4.1.1.11.2"
#define GKUID_PseudoColorSoftcopyPresentationStateStorage            "1.2.840.10008.5.1.4.1.1.11.3"
#define GKUID_BlendingSoftcopyPresentationStateStorage               "1.2.840.10008.5.1.4.1.1.11.4"
#define GKUID_XAXRFGrayscaleSoftcopyPresentationStateStorage         "1.2.840.10008.5.1.4.1.1.11.5"
#define GKUID_XRayAngiographicImageStorage                           "1.2.840.10008.5.1.4.1.1.12.1"
#define GKUID_EnhancedXAImageStorage                                 "1.2.840.10008.5.1.4.1.1.12.1.1"
#define GKUID_XRayRadiofluoroscopicImageStorage                      "1.2.840.10008.5.1.4.1.1.12.2"
#define GKUID_EnhancedXRFImageStorage                                "1.2.840.10008.5.1.4.1.1.12.2.1"
#define GKUID_XRay3DAngiographicImageStorage                         "1.2.840.10008.5.1.4.1.1.13.1.1"
#define GKUID_XRay3DCraniofacialImageStorage                         "1.2.840.10008.5.1.4.1.1.13.1.2"
#define GKUID_BreastTomosynthesisImageStorage                        "1.2.840.10008.5.1.4.1.1.13.1.3"
#define GKUID_RETIRED_XRayAngiographicBiPlaneImageStorage            "1.2.840.10008.5.1.4.1.1.12.3"
#define GKUID_NuclearMedicineImageStorage                            "1.2.840.10008.5.1.4.1.1.20"
#define GKUID_RawDataStorage                                         "1.2.840.10008.5.1.4.1.1.66"
#define GKUID_SpatialRegistrationStorage                             "1.2.840.10008.5.1.4.1.1.66.1"
#define GKUID_SpatialFiducialsStorage                                "1.2.840.10008.5.1.4.1.1.66.2"
#define GKUID_DeformableSpatialRegistrationStorage                   "1.2.840.10008.5.1.4.1.1.66.3"
#define GKUID_SegmentationStorage                                    "1.2.840.10008.5.1.4.1.1.66.4"
#define GKUID_SurfaceSegmentationStorage                             "1.2.840.10008.5.1.4.1.1.66.5"
#define GKUID_RealWorldValueMappingStorage                           "1.2.840.10008.5.1.4.1.1.67"
#define GKUID_RETIRED_VLImageStorage                                 "1.2.840.10008.5.1.4.1.1.77.1"
#define GKUID_VLEndoscopicImageStorage                               "1.2.840.10008.5.1.4.1.1.77.1.1"
#define GKUID_VideoEndoscopicImageStorage                            "1.2.840.10008.5.1.4.1.1.77.1.1.1"
#define GKUID_VLMicroscopicImageStorage                              "1.2.840.10008.5.1.4.1.1.77.1.2"
#define GKUID_VideoMicroscopicImageStorage                           "1.2.840.10008.5.1.4.1.1.77.1.2.1"
#define GKUID_VLSlideCoordinatesMicroscopicImageStorage              "1.2.840.10008.5.1.4.1.1.77.1.3"
#define GKUID_VLPhotographicImageStorage                             "1.2.840.10008.5.1.4.1.1.77.1.4"
#define GKUID_VideoPhotographicImageStorage                          "1.2.840.10008.5.1.4.1.1.77.1.4.1"
#define GKUID_OphthalmicPhotography8BitImageStorage                  "1.2.840.10008.5.1.4.1.1.77.1.5.1"
#define GKUID_OphthalmicPhotography16BitImageStorage                 "1.2.840.10008.5.1.4.1.1.77.1.5.2"
#define GKUID_StereometricRelationshipStorage                        "1.2.840.10008.5.1.4.1.1.77.1.5.3"
#define GKUID_OphthalmicTomographyImageStorage                       "1.2.840.10008.5.1.4.1.1.77.1.5.4"
#define GKUID_VLWholeSlideMicroscopyImageStorage                     "1.2.840.10008.5.1.4.1.1.77.1.6"
#define GKUID_RETIRED_VLMultiFrameImageStorage                       "1.2.840.10008.5.1.4.1.1.77.2"
#define GKUID_LensometryMeasurementsStorage                          "1.2.840.10008.5.1.4.1.1.78.1"
#define GKUID_AutorefractionMeasurementsStorage                      "1.2.840.10008.5.1.4.1.1.78.2"
#define GKUID_KeratometryMeasurementsStorage                         "1.2.840.10008.5.1.4.1.1.78.3"
#define GKUID_SubjectiveRefractionMeasurementsStorage                "1.2.840.10008.5.1.4.1.1.78.4"
#define GKUID_VisualAcuityMeasurementsStorage                        "1.2.840.10008.5.1.4.1.1.78.5"
#define GKUID_SpectaclePrescriptionReportStorage                     "1.2.840.10008.5.1.4.1.1.78.6"
#define GKUID_OphthalmicAxialMeasurementsStorage                     "1.2.840.10008.5.1.4.1.1.78.7"
#define GKUID_IntraocularLensCalculationsStorage                     "1.2.840.10008.5.1.4.1.1.78.8"
#define GKUID_MacularGridThicknessAndVolumeReportStorage             "1.2.840.10008.5.1.4.1.1.79.1"
#define GKUID_OphthalmicVisualFieldStaticPerimetryMeasurementsStorage "1.2.840.10008.5.1.4.1.1.80.1"
#define GKUID_BasicTextSRStorage                                     "1.2.840.10008.5.1.4.1.1.88.11"
#define GKUID_EnhancedSRStorage                                      "1.2.840.10008.5.1.4.1.1.88.22"
#define GKUID_ComprehensiveSRStorage                                 "1.2.840.10008.5.1.4.1.1.88.33"
#define GKUID_ProcedureLogStorage                                    "1.2.840.10008.5.1.4.1.1.88.40"
#define GKUID_MammographyCADSRStorage                                "1.2.840.10008.5.1.4.1.1.88.50"
#define GKUID_KeyObjectSelectionDocumentStorage                      "1.2.840.10008.5.1.4.1.1.88.59"
#define GKUID_ChestCADSRStorage                                      "1.2.840.10008.5.1.4.1.1.88.65"
#define GKUID_XRayRadiationDoseSRStorage                             "1.2.840.10008.5.1.4.1.1.88.67"
#define GKUID_ColonCADSRStorage                                      "1.2.840.10008.5.1.4.1.1.88.69"
#define GKUID_ImplantationPlanSRDocumentStorage                      "1.2.840.10008.5.1.4.1.1.88.70"
#define GKUID_EncapsulatedPDFStorage                                 "1.2.840.10008.5.1.4.1.1.104.1"
#define GKUID_EncapsulatedCDAStorage                                 "1.2.840.10008.5.1.4.1.1.104.2"
#define GKUID_PositronEmissionTomographyImageStorage                 "1.2.840.10008.5.1.4.1.1.128"
#define GKUID_RETIRED_StandalonePETCurveStorage                      "1.2.840.10008.5.1.4.1.1.129"
#define GKUID_EnhancedPETImageStorage                                "1.2.840.10008.5.1.4.1.1.130"
#define GKUID_BasicStructuredDisplayStorage                          "1.2.840.10008.5.1.4.1.1.131"
#define GKUID_RTImageStorage                                         "1.2.840.10008.5.1.4.1.1.481.1"
#define GKUID_RTDoseStorage                                          "1.2.840.10008.5.1.4.1.1.481.2"
#define GKUID_RTStructureSetStorage                                  "1.2.840.10008.5.1.4.1.1.481.3"
#define GKUID_RTBeamsTreatmentRecordStorage                          "1.2.840.10008.5.1.4.1.1.481.4"
#define GKUID_RTPlanStorage                                          "1.2.840.10008.5.1.4.1.1.481.5"
#define GKUID_RTBrachyTreatmentRecordStorage                         "1.2.840.10008.5.1.4.1.1.481.6"
#define GKUID_RTTreatmentSummaryRecordStorage                        "1.2.840.10008.5.1.4.1.1.481.7"
#define GKUID_RTIonPlanStorage                                       "1.2.840.10008.5.1.4.1.1.481.8"
#define GKUID_RTIonBeamsTreatmentRecordStorage                       "1.2.840.10008.5.1.4.1.1.481.9"
#define GKUID_GenericImplantTemplateStorage                          "1.2.840.10008.5.1.4.43.1"
#define GKUID_ImplantAssemblyTemplateStorage                         "1.2.840.10008.5.1.4.44.1"
#define GKUID_ImplantTemplateGroupStorage                            "1.2.840.10008.5.1.4.45.1"

// DICOMDIR; was GKUID_BasicDirectoryStorageSOPClass in DCMTK versions prior to 3.5.3
#define GKUID_MediaStorageDirectoryStorage                           "1.2.840.10008.1.3.10"

/* Hanging Protocols Storage is a special case because hanging protocols use a different
   information model, i.e. there is no patient, study or series in a hanging protocol IOD. */
#define GKUID_HangingProtocolStorage                                 "1.2.840.10008.5.1.4.38.1"

// Query/Retrieve
#define GKUID_FINDPatientRootQueryRetrieveInformationModel           "1.2.840.10008.5.1.4.1.2.1.1"
#define GKUID_MOVEPatientRootQueryRetrieveInformationModel           "1.2.840.10008.5.1.4.1.2.1.2"
#define GKUID_GETPatientRootQueryRetrieveInformationModel            "1.2.840.10008.5.1.4.1.2.1.3"
#define GKUID_FINDStudyRootQueryRetrieveInformationModel             "1.2.840.10008.5.1.4.1.2.2.1"
#define GKUID_MOVEStudyRootQueryRetrieveInformationModel             "1.2.840.10008.5.1.4.1.2.2.2"
#define GKUID_GETStudyRootQueryRetrieveInformationModel              "1.2.840.10008.5.1.4.1.2.2.3"
#define GKUID_RETIRED_FINDPatientStudyOnlyQueryRetrieveInformationModel "1.2.840.10008.5.1.4.1.2.3.1"
#define GKUID_RETIRED_MOVEPatientStudyOnlyQueryRetrieveInformationModel "1.2.840.10008.5.1.4.1.2.3.2"
#define GKUID_RETIRED_GETPatientStudyOnlyQueryRetrieveInformationModel  "1.2.840.10008.5.1.4.1.2.3.3"
#define GKUID_RETIRED_MOVECompositeInstanceRootRetrieve              "1.2.840.10008.5.1.4.1.2.4.2"
#define GKUID_RETIRED_GETCompositeInstanceRootRetrieve               "1.2.840.10008.5.1.4.1.2.4.3"
#define GKUID_RETIRED_GETCompositeInstanceRetrieveWithoutBulkData    "1.2.840.10008.5.1.4.1.2.5.3"

// Worklist
#define GKUID_FINDModalityWorklistInformationModel                   "1.2.840.10008.5.1.4.31"
#define GKUID_FINDGeneralPurposeWorklistInformationModel             "1.2.840.10008.5.1.4.32.1"

// General Purpose Worklist
#define GKUID_GeneralPurposeScheduledProcedureStepSOPClass           "1.2.840.10008.5.1.4.32.2"
#define GKUID_GeneralPurposePerformedProcedureStepSOPClass           "1.2.840.10008.5.1.4.32.3"
#define GKUID_GeneralPurposeWorklistManagementMetaSOPClass           "1.2.840.10008.5.1.4.32"

// MPPS
#define GKUID_ModalityPerformedProcedureStepSOPClass                 "1.2.840.10008.3.1.2.3.3"
#define GKUID_ModalityPerformedProcedureStepRetrieveSOPClass         "1.2.840.10008.3.1.2.3.4"
#define GKUID_ModalityPerformedProcedureStepNotificationSOPClass     "1.2.840.10008.3.1.2.3.5"

// Unified Worklist and Procedure Step
#define GKUID_UnifiedWorklistAndProcedureStepServiceClass            "1.2.840.10008.5.1.4.34.4"
#define GKUID_UnifiedProcedureStepPushSOPClass                       "1.2.840.10008.5.1.4.34.4.1"
#define GKUID_UnifiedProcedureStepWatchSOPClass                      "1.2.840.10008.5.1.4.34.4.2"
#define GKUID_UnifiedProcedureStepPullSOPClass                       "1.2.840.10008.5.1.4.34.4.3"
#define GKUID_UnifiedProcedureStepEventSOPClass                      "1.2.840.10008.5.1.4.34.4.4"
#define GKUID_UnifiedWorklistAndProcedureStepSOPInstance             "1.2.840.10008.5.1.4.34.5"

// Storage Commitment
#define GKUID_StorageCommitmentPushModelSOPClass                     "1.2.840.10008.1.20.1"
#define GKUID_StorageCommitmentPushModelSOPInstance                  "1.2.840.10008.1.20.1.1"
#define GKUID_RETIRED_StorageCommitmentPullModelSOPClass             "1.2.840.10008.1.20.2"
#define GKUID_RETIRED_StorageCommitmentPullModelSOPInstance          "1.2.840.10008.1.20.2.1"

// Hanging Protocols
#define GKUID_FINDHangingProtocolInformationModel                    "1.2.840.10008.5.1.4.38.2"
#define GKUID_MOVEHangingProtocolInformationModel                    "1.2.840.10008.5.1.4.38.3"

// Relevant Patient Information Query
#define GKUID_GeneralRelevantPatientInformationQuery                 "1.2.840.10008.5.1.4.37.1"
#define GKUID_BreastImagingRelevantPatientInformationQuery           "1.2.840.10008.5.1.4.37.2"
#define GKUID_CardiacRelevantPatientInformationQuery                 "1.2.840.10008.5.1.4.37.3"

// Color Palette Storage and Query/Retrieve
#define GKUID_ColorPaletteStorage                                    "1.2.840.10008.5.1.4.39.1"
#define GKUID_FINDColorPaletteInformationModel                       "1.2.840.10008.5.1.4.39.2"
#define GKUID_MOVEColorPaletteInformationModel                       "1.2.840.10008.5.1.4.39.3"
#define GKUID_GETColorPaletteInformationModel                        "1.2.840.10008.5.1.4.39.4"

// Implant Template Query/Retrieve
#define GKUID_FINDGenericImplantTemplateInformationModel             "1.2.840.10008.5.1.4.43.2"
#define GKUID_MOVEGenericImplantTemplateInformationModel             "1.2.840.10008.5.1.4.43.3"
#define GKUID_GETGenericImplantTemplateInformationModel              "1.2.840.10008.5.1.4.43.4"
#define GKUID_FINDImplantAssemblyTemplateInformationModel            "1.2.840.10008.5.1.4.44.2"
#define GKUID_MOVEImplantAssemblyTemplateInformationModel            "1.2.840.10008.5.1.4.44.3"
#define GKUID_GETImplantAssemblyTemplateInformationModel             "1.2.840.10008.5.1.4.44.4"
#define GKUID_FINDImplantTemplateGroupInformationModel               "1.2.840.10008.5.1.4.45.2"
#define GKUID_MOVEImplantTemplateGroupInformationModel               "1.2.840.10008.5.1.4.45.3"
#define GKUID_GETImplantTemplateGroupInformationModel                "1.2.840.10008.5.1.4.45.4"

// Print
#define GKUID_BasicFilmSessionSOPClass                               "1.2.840.10008.5.1.1.1"
#define GKUID_BasicFilmBoxSOPClass                                   "1.2.840.10008.5.1.1.2"
#define GKUID_BasicGrayscaleImageBoxSOPClass                         "1.2.840.10008.5.1.1.4"
#define GKUID_BasicColorImageBoxSOPClass                             "1.2.840.10008.5.1.1.4.1"
#define GKUID_RETIRED_ReferencedImageBoxSOPClass                     "1.2.840.10008.5.1.1.4.2"
#define GKUID_BasicGrayscalePrintManagementMetaSOPClass              "1.2.840.10008.5.1.1.9"
#define GKUID_RETIRED_ReferencedGrayscalePrintManagementMetaSOPClass "1.2.840.10008.5.1.1.9.1"
#define GKUID_PrintJobSOPClass                                       "1.2.840.10008.5.1.1.14"
#define GKUID_BasicAnnotationBoxSOPClass                             "1.2.840.10008.5.1.1.15"
#define GKUID_PrinterSOPClass                                        "1.2.840.10008.5.1.1.16"
#define GKUID_PrinterConfigurationRetrievalSOPClass                  "1.2.840.10008.5.1.1.16.376"
#define GKUID_PrinterSOPInstance                                     "1.2.840.10008.5.1.1.17"
#define GKUID_PrinterConfigurationRetrievalSOPInstance               "1.2.840.10008.5.1.1.17.376"
#define GKUID_BasicColorPrintManagementMetaSOPClass                  "1.2.840.10008.5.1.1.18"
#define GKUID_RETIRED_ReferencedColorPrintManagementMetaSOPClass     "1.2.840.10008.5.1.1.18.1"
#define GKUID_VOILUTBoxSOPClass                                      "1.2.840.10008.5.1.1.22"
#define GKUID_PresentationLUTSOPClass                                "1.2.840.10008.5.1.1.23"
#define GKUID_RETIRED_ImageOverlayBoxSOPClass                        "1.2.840.10008.5.1.1.24"
#define GKUID_RETIRED_BasicPrintImageOverlayBoxSOPClass              "1.2.840.10008.5.1.1.24.1"
#define GKUID_RETIRED_PrintQueueSOPInstance                          "1.2.840.10008.5.1.1.25"
#define GKUID_RETIRED_PrintQueueManagementSOPClass                   "1.2.840.10008.5.1.1.26"
#define GKUID_RETIRED_PullPrintRequestSOPClass                       "1.2.840.10008.5.1.1.31"
#define GKUID_RETIRED_PullStoredPrintManagementMetaSOPClass          "1.2.840.10008.5.1.1.32"

// Detached Management
#define GKUID_RETIRED_DetachedPatientManagementSOPClass              "1.2.840.10008.3.1.2.1.1"
#define GKUID_RETIRED_DetachedPatientManagementMetaSOPClass          "1.2.840.10008.3.1.2.1.4"
#define GKUID_RETIRED_DetachedVisitManagementSOPClass                "1.2.840.10008.3.1.2.2.1"
#define GKUID_RETIRED_DetachedStudyManagementSOPClass                "1.2.840.10008.3.1.2.3.1"
#define GKUID_RETIRED_DetachedResultsManagementSOPClass              "1.2.840.10008.3.1.2.5.1"
#define GKUID_RETIRED_DetachedResultsManagementMetaSOPClass          "1.2.840.10008.3.1.2.5.4"
#define GKUID_RETIRED_DetachedStudyManagementMetaSOPClass            "1.2.840.10008.3.1.2.5.5"
#define GKUID_RETIRED_DetachedInterpretationManagementSOPClass       "1.2.840.10008.3.1.2.6.1"

// Procedure Log
#define GKUID_ProceduralEventLoggingSOPClass                         "1.2.840.10008.1.40"
#define GKUID_ProceduralEventLoggingSOPInstance                      "1.2.840.10008.1.40.1"

// Substance Administration
#define GKUID_SubstanceAdministrationLoggingSOPClass                 "1.2.840.10008.1.42"
#define GKUID_SubstanceAdministrationLoggingSOPInstance              "1.2.840.10008.1.42.1"
#define GKUID_ProductCharacteristicsQuerySOPClass                    "1.2.840.10008.5.1.4.41"
#define GKUID_SubstanceApprovalQuerySOPClass                         "1.2.840.10008.5.1.4.42"

// Media Creation
#define GKUID_MediaCreationManagementSOPClass                        "1.2.840.10008.5.1.1.33"

// SOP Class Relationship Negotiation
#define GKUID_StorageServiceClass                                    "1.2.840.10008.4.2"

// Instance Availability Notification
#define GKUID_InstanceAvailabilityNotificationSOPClass               "1.2.840.10008.5.1.4.33"

// Application Hosting
#define GKUID_NativeDICOMModel                                       "1.2.840.10008.7.1.1"
#define GKUID_AbstractMultiDimensionalImageModel                     "1.2.840.10008.7.1.2"

// Other
#define GKUID_VerificationSOPClass                                   "1.2.840.10008.1.1"
#define GKUID_RETIRED_BasicStudyContentNotificationSOPClass          "1.2.840.10008.1.9"
#define GKUID_RETIRED_StudyComponentManagementSOPClass               "1.2.840.10008.3.1.2.3.2"

// Coding Schemes
#define GKUID_DICOMControlledTerminologyCodingScheme                 "1.2.840.10008.2.16.4"
#define GKUID_DICOMUIDRegistryCodingScheme                           "1.2.840.10008.2.6.1"

// Configuration Management LDAP UIDs
#define GKUID_LDAP_dicomDeviceName                                   "1.2.840.10008.15.0.3.1"
#define GKUID_LDAP_dicomDescription                                  "1.2.840.10008.15.0.3.2"
#define GKUID_LDAP_dicomManufacturer                                 "1.2.840.10008.15.0.3.3"
#define GKUID_LDAP_dicomManufacturerModelName                        "1.2.840.10008.15.0.3.4"
#define GKUID_LDAP_dicomSoftwareVersion                              "1.2.840.10008.15.0.3.5"
#define GKUID_LDAP_dicomVendorData                                   "1.2.840.10008.15.0.3.6"
#define GKUID_LDAP_dicomAETitle                                      "1.2.840.10008.15.0.3.7"
#define GKUID_LDAP_dicomNetworkConnectionReference                   "1.2.840.10008.15.0.3.8"
#define GKUID_LDAP_dicomApplicationCluster                           "1.2.840.10008.15.0.3.9"
#define GKUID_LDAP_dicomAssociationInitiator                         "1.2.840.10008.15.0.3.10"
#define GKUID_LDAP_dicomAssociationAcceptor                          "1.2.840.10008.15.0.3.11"
#define GKUID_LDAP_dicomHostname                                     "1.2.840.10008.15.0.3.12"
#define GKUID_LDAP_dicomPort                                         "1.2.840.10008.15.0.3.13"
#define GKUID_LDAP_dicomSOPClass                                     "1.2.840.10008.15.0.3.14"
#define GKUID_LDAP_dicomTransferRole                                 "1.2.840.10008.15.0.3.15"
#define GKUID_LDAP_dicomTransferSyntax                               "1.2.840.10008.15.0.3.16"
#define GKUID_LDAP_dicomPrimaryDeviceType                            "1.2.840.10008.15.0.3.17"
#define GKUID_LDAP_dicomRelatedDeviceReference                       "1.2.840.10008.15.0.3.18"
#define GKUID_LDAP_dicomPreferredCalledAETitle                       "1.2.840.10008.15.0.3.19"
#define GKUID_LDAP_dicomTLSCyphersuite                               "1.2.840.10008.15.0.3.20"
#define GKUID_LDAP_dicomAuthorizedNodeCertificateReference           "1.2.840.10008.15.0.3.21"
#define GKUID_LDAP_dicomThisNodeCertificateReference                 "1.2.840.10008.15.0.3.22"
#define GKUID_LDAP_dicomInstalled                                    "1.2.840.10008.15.0.3.23"
#define GKUID_LDAP_dicomStationName                                  "1.2.840.10008.15.0.3.24"
#define GKUID_LDAP_dicomDeviceSerialNumber                           "1.2.840.10008.15.0.3.25"
#define GKUID_LDAP_dicomInstitutionName                              "1.2.840.10008.15.0.3.26"
#define GKUID_LDAP_dicomInstitutionAddress                           "1.2.840.10008.15.0.3.27"
#define GKUID_LDAP_dicomInstitutionDepartmentName                    "1.2.840.10008.15.0.3.28"
#define GKUID_LDAP_dicomIssuerOfPatientID                            "1.2.840.10008.15.0.3.29"
#define GKUID_LDAP_dicomPreferredCallingAETitle                      "1.2.840.10008.15.0.3.30"
#define GKUID_LDAP_dicomSupportedCharacterSet                        "1.2.840.10008.15.0.3.31"
#define GKUID_LDAP_dicomConfigurationRoot                            "1.2.840.10008.15.0.4.1"
#define GKUID_LDAP_dicomDevicesRoot                                  "1.2.840.10008.15.0.4.2"
#define GKUID_LDAP_dicomUniqueAETitlesRegistryRoot                   "1.2.840.10008.15.0.4.3"
#define GKUID_LDAP_dicomDevice                                       "1.2.840.10008.15.0.4.4"
#define GKUID_LDAP_dicomNetworkAE                                    "1.2.840.10008.15.0.4.5"
#define GKUID_LDAP_dicomNetworkConnection                            "1.2.840.10008.15.0.4.6"
#define GKUID_LDAP_dicomUniqueAETitle                                "1.2.840.10008.15.0.4.7"
#define GKUID_LDAP_dicomTransferCapability                           "1.2.840.10008.15.0.4.8"

// UTC Synchronization Frame of Reference (CP 432)
#define GKUID_UniversalCoordinatedTimeSynchronizationFrameOfReference "1.2.840.10008.15.1.1"

// Well-known Frame of References
#define GKUID_TalairachBrainAtlasFrameOfReference                    "1.2.840.10008.1.4.1.1"
#define GKUID_SPM2T1FrameOfReference                                 "1.2.840.10008.1.4.1.2"
#define GKUID_SPM2T2FrameOfReference                                 "1.2.840.10008.1.4.1.3"
#define GKUID_SPM2PDFrameOfReference                                 "1.2.840.10008.1.4.1.4"
#define GKUID_SPM2EPIFrameOfReference                                "1.2.840.10008.1.4.1.5"
#define GKUID_SPM2FILT1FrameOfReference                              "1.2.840.10008.1.4.1.6"
#define GKUID_SPM2PETFrameOfReference                                "1.2.840.10008.1.4.1.7"
#define GKUID_SPM2TRANSMFrameOfReference                             "1.2.840.10008.1.4.1.8"
#define GKUID_SPM2SPECTFrameOfReference                              "1.2.840.10008.1.4.1.9"
#define GKUID_SPM2GRAYFrameOfReference                               "1.2.840.10008.1.4.1.10"
#define GKUID_SPM2WHITEFrameOfReference                              "1.2.840.10008.1.4.1.11"
#define GKUID_SPM2CSFFrameOfReference                                "1.2.840.10008.1.4.1.12"
#define GKUID_SPM2BRAINMASKFrameOfReference                          "1.2.840.10008.1.4.1.13"
#define GKUID_SPM2AVG305T1FrameOfReference                           "1.2.840.10008.1.4.1.14"
#define GKUID_SPM2AVG152T1FrameOfReference                           "1.2.840.10008.1.4.1.15"
#define GKUID_SPM2AVG152T2FrameOfReference                           "1.2.840.10008.1.4.1.16"
#define GKUID_SPM2AVG152PDFrameOfReference                           "1.2.840.10008.1.4.1.17"
#define GKUID_SPM2SINGLESUBJT1FrameOfReference                       "1.2.840.10008.1.4.1.18"
#define GKUID_ICBM452T1FrameOfReference                              "1.2.840.10008.1.4.2.1"
#define GKUID_ICBMSingleSubjectMRIFrameOfReference                   "1.2.840.10008.1.4.2.2"

// Well-known SOP Instances for Color Palettes
#define GKUID_HotIronColorPaletteSOPInstance                         "1.2.840.10008.1.5.1"
#define GKUID_PETColorPaletteSOPInstance                             "1.2.840.10008.1.5.2"
#define GKUID_HotMetalBlueColorPaletteSOPInstance                    "1.2.840.10008.1.5.3"
#define GKUID_PET20StepColorPaletteSOPInstance                       "1.2.840.10008.1.5.4"

// Private DCMTK UIDs

// Private SOP Class UID used to shutdown external network applications
#define GKUID_PrivateShutdownSOPClass                                "1.2.276.0.7230010.3.4.1915765545.18030.917282194.0"


/* DRAFT SUPPLEMENTS - EXPERIMENTAL USE ONLY */

/*
 * The following UIDs were defined in "frozen draft for trial implementation" versions
 * of various DICOM supplements and are or will be changed before final text.
 * Since it is likely that trial implementations exist, we leave the UIDs in the dictionary.
 */

/* Supplement 23 Frozen Draft (November 1997) */
#define GKUID_DRAFT_SRTextStorage                                    "1.2.840.10008.5.1.4.1.1.88.1"
#define GKUID_DRAFT_SRAudioStorage                                   "1.2.840.10008.5.1.4.1.1.88.2"
#define GKUID_DRAFT_SRDetailStorage                                  "1.2.840.10008.5.1.4.1.1.88.3"
#define GKUID_DRAFT_SRComprehensiveStorage                           "1.2.840.10008.5.1.4.1.1.88.4"

/* Supplement 30 Draft 08 for Demonstration (October 1997) */
#define GKUID_DRAFT_WaveformStorage                                  "1.2.840.10008.5.1.4.1.1.9.1"

/* Supplement 74 Frozen Draft (October 2007) */
#define GKUID_DRAFT_RTBeamsDeliveryInstructionStorage                "1.2.840.10008.5.1.4.34.1"
#define GKUID_DRAFT_RTConventionalMachineVerification                "1.2.840.10008.5.1.4.34.2"
#define GKUID_DRAFT_RTIonMachineVerification                         "1.2.840.10008.5.1.4.34.3"
