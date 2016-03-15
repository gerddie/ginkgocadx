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


#include "dicomprintassociation.h"
#include <wx/string.h>
#include <wx/intl.h>
#include <wx/filename.h>

#include <api/controllers/icontroladorlog.h>
#include <api/controllers/ipacscontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>

#include <main/controllers/dcmtk/ginkgouid.h>
#include <api/internationalization/internationalization.h>
#include <api/dicom/dcmdictionary.h>


#include "helpercompresion.h"


//Server: dcmprscp -c /usr/local/etc/dcmtk/dcmpstat.cfg -p KODAK -ll debug >log.txt 2>&1
//Client:
//	Step 1: dcmpsprt -c /usr/local/etc/dcmtk/dcmpstat.cfg -p KODAK /home/carlos/DICOM/INCISIX/INCISIX/IM-0001-0100.dcm
//	Step 2: dcmprscu -c /usr/local/etc/dcmtk/dcmpstat.cfg -p KODAK database/SP_4f7959ef7ffdbf85.dcm

/*
FILM SESSION SOP CLASS N-CREATE REQUEST ATTRIBUTES
Number of Copies (2000|0010) IS [1 .. 10]
Medium type (2000|0030) CS [BLUE FILM, CLEAR FILM or PAPER]
Film Destination (2000|0040) CS [MAGAZINE or PROCESSOR]

FILM BOX SOP CLASS N-CREATE REQUEST ATTRIBUTES
Image Display Format (2010|0010) CS [STANDARD\1,1]
Film Orientation (2010|0040) CS [PORTRAIT OR LANDSCAPE]
Film Size ID (2010|0050) CS [14INX17IN, 14INX14IN, 11INX14IN, 11INX11IN, 85INX11IN, 8INX10IN]
Maginfication Type (2010|0060) CS [REPLICATE, BILINEAR, CUBIC or NONE]
Border Density (2010,0100) CS [BLACK or WHITE]
*/

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
#include <dcmtk/dcmdata/dctk.h>
#include <dcmtk/dcmimgle/dcmimage.h>
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif


static void DIMSE_printNStatusString(std::ostream& dumpStream, int status)
{
  switch(status)
  {
    case STATUS_Success:
      dumpStream << "0x0000: Success";
      break;
    case STATUS_N_Cancel:
      dumpStream << "0xFE00: Cancel";
      break;
    case STATUS_N_AttributeListError:
      dumpStream << "0x0107: Attribute list error";
      break;
    case STATUS_N_SOPClassNotSupported:
      dumpStream << "0x0122: SOP class not supported";
      break;
    case STATUS_N_ClassInstanceConflict:
      dumpStream << "0x0119: Class/instance conflict";
      break;
    case STATUS_N_DuplicateSOPInstance:
      dumpStream << "0x0111: Duplicate SOP instance";
      break;
    case STATUS_N_DuplicateInvocation:
      dumpStream << "0x0210: Duplicate invocation";
      break;
    case STATUS_N_InvalidArgumentValue:
      dumpStream << "0x0115: Invalid argument value";
      break;
    case STATUS_N_InvalidAttributeValue:
      dumpStream << "0x0106: Invalid attribute value";
      break;
    case STATUS_N_InvalidObjectInstance:
      dumpStream << "0x0117: Invalid object instance";
      break;
    case STATUS_N_MissingAttribute:
      dumpStream << "0x0120: Missing attribute";
      break;
    case STATUS_N_MissingAttributeValue:
      dumpStream << "0x0121: Missing attribute value";
      break;
    case STATUS_N_MistypedArgument:
      dumpStream << "0x0212: Mistyped argument";
      break;
    case STATUS_N_NoSuchArgument:
      dumpStream << "0x0114: No such argument";
      break;
    case STATUS_N_NoSuchAttribute:
      dumpStream << "0x0105: No such attribute";
      break;
    case STATUS_N_NoSuchEventType:
      dumpStream << "0x0113: No such event type";
      break;
    case STATUS_N_NoSuchObjectInstance:
      dumpStream << "0x0112: No such object instance";
      break;
    case STATUS_N_NoSuchSOPClass:
      dumpStream << "0x0118: No such SOP class";
      break;
    case STATUS_N_ProcessingFailure:
      dumpStream << "0x0110: Processing failure";
      break;
    case STATUS_N_ResourceLimitation:
      dumpStream << "0x0213: Resource limitation";
      break;
    case STATUS_N_UnrecognizedOperation:
      dumpStream << "0x0211: Unrecognized operation";
      break;
    case STATUS_N_PRINT_BFS_Warn_MemoryAllocation:
      dumpStream << "0xB600: Basic film session warning - Memory allocation";
      break;
    case STATUS_N_PRINT_BFS_Warn_NoSessionPrinting:
      dumpStream << "0xB601: Basic film session warning - No session printing";
      break;
    case STATUS_N_PRINT_BFS_Warn_EmptyPage:
      dumpStream << "0xB602: Basic film session warning - Empty page";
      break;
    case STATUS_N_PRINT_BFB_Warn_EmptyPage:
      dumpStream << "0xB603: Basic film box warning - Empty page";
      break;
    case STATUS_N_PRINT_BFS_Fail_NoFilmBox:
      dumpStream << "0xC600: Basic film session failure - No film box";
      break;
    case STATUS_N_PRINT_BFS_Fail_PrintQueueFull:
      dumpStream << "0xC601: Basic film session failure - Print queue full";
      break;
    case STATUS_N_PRINT_BSB_Fail_PrintQueueFull:
      dumpStream << "0xC602: Basic film box failure - Print queue full";
      break;
    case STATUS_N_PRINT_BFS_BFB_Fail_ImageSize:
      dumpStream << "0xC603: Basic film session/box failure - Image size";
      break;
    case STATUS_N_PRINT_BFS_BFB_Fail_PositionCollision:
      dumpStream << "0xC604: Basic film session/box failure - Position collision";
      break;
    case STATUS_N_PRINT_IB_Fail_InsufficientMemory:
      dumpStream << "0xC605: Image box failure - Insufficient memory";
      break;
    case STATUS_N_PRINT_IB_Fail_MoreThanOneVOILUT:
      dumpStream << "0xC606: Image box failure - More than one VOI LUT";
      break;
    default: {
      char prev = dumpStream.fill('0');
      dumpStream << "0x" << std::hex << std::setw(4) << status
                 << ": Unknown Status Code" << std::resetiosflags(std::ios::showbase);
      dumpStream.fill(prev);
      break;
  }
  }
}

PrintAssociation::PrintAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificadorProgreso) : Association(ambitolog)
{
	
	if (connectionKey == NULL) {
		LOG_ERROR(ambitolog, "NULL Connection key");
	}
	this->Net = GIL::DICOM::DCMTK::Network::Instance(connectionKey);
	this->m_pNotificadorProgreso = pNotificadorProgreso;
	this->ambitolog = ambitolog;
	this->blockMode = DIMSE_BLOCKING;
	this->eventHandler = NULL;
	this->presId = 0;
}

PrintAssociation::~PrintAssociation() {
	
}

OFCondition PrintAssociation::Print(const GNC::GCS::Ptr<DicomServer>& server, const std::string& local_aet, const GIL::DICOM::DicomDataset& film, const GIL::DICOM::DicomDataset& layout, const GIL::DICOM::DicomDataset& job, const std::list<std::string>& images)
{
	OFCondition status = EC_Normal;
	
	//1. Open Association
	
	if (!Net->Initiallized()) {
		Net->InitializeNetwork(GetTimeout(), GetRole(), GetAcceptorPort());
	}
	
	Create(
			server->AET,
			server->HostName,
			server->Port,
			local_aet
	);
	
	SetNotificadorProgreso(m_pNotificadorProgreso);

	LOG_DEBUG(ambitolog, "Conectando: AET = " << server->AET << ", Host = " << server->HostName << ", Puerto = " << server->Port << ", Local AET = " << local_aet << ", PDU = " << server->PDU);
	OFCondition r = Connect(Net, server->PDU);
	
	OFCondition c = EC_Normal;
	
	Uint16 rstatus;

	if (r.good() == true) {
		
		// 2. N-GET (Printer)
		
		DcmDataset* response;
		DcmDataset attrs;
		DcmElement* e = NULL;
		DcmSequenceOfItems* seq = NULL;
		DcmItem* item = NULL;
		
		Uint16 *infoAttrList = NULL;
		
		c = getRQ( UID_PrinterSOPClass, UID_PrinterSOPInstance, infoAttrList, 0, rstatus, response);
		
		if (c.good()) {
			
			// 3. N-CREATE (Film Session)
			
			delete response;
			response = NULL;
			attrs.clear();
			
			std::string tmpStd;


			e = newDicomElement(DCM_NumberOfCopies);
			if (job.getTag(GKDCM_NumberOfCopies, tmpStd)) {
				e->putString(tmpStd.c_str());
			} else {
				e->putString("1");
			}
			attrs.insert(e);
			
			if (job.getTag(GKDCM_PrintPriority, tmpStd)) {
				e = newDicomElement(DCM_PrintPriority);
				if (!tmpStd.empty()) {
					e->putString(tmpStd.c_str());
				}
				attrs.insert(e);
			}
			
			if (job.getTag(GKDCM_MediumType, tmpStd)) {
				e = newDicomElement(DCM_MediumType);
				if (!tmpStd.empty()) {
					e->putString(tmpStd.c_str());
				}
			}
			attrs.insert(e);
			
			
			if (film.getTag(GKDCM_FilmDestination, tmpStd)) {
				e = newDicomElement(DCM_FilmDestination);
				if (!tmpStd.empty()) {
					e->putString(tmpStd.c_str());
				}
				attrs.insert(e);
			}
			
			
			e = newDicomElement(DCM_FilmSessionLabel);
			e->putString("GNK Print Session");
			attrs.insert(e);
			
			/*
			e = newDicomElement(DCM_MemoryAllocation);
			attrs.insert(e);
			*/
			
			e = newDicomElement(DCM_OwnerID);
			attrs.insert(e);

			//T_ASC_PresentationContextID pcidColorPrint = findAcceptedPC(UID_BasicColorPrintManagementMetaSOPClass);
			//T_ASC_PresentationContextID pcidGrayscalePrint = findAcceptedPC(UID_BasicGrayscalePrintManagementMetaSOPClass);
			//T_ASC_PresentationContextID pcidPrint = color ? pcidColorPrint : pcidGrayscalePrint;			
			
			std::string filmSessionInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_InstanceRoot);
			
			OFString ofFilmSessionInstanceUID(filmSessionInstanceUID.c_str());
			
			c = createRQ(UID_BasicFilmSessionSOPClass, ofFilmSessionInstanceUID, &attrs, rstatus, response);
			
			if (c.good()) {

				// 4. N-CREATE (Presentation LUT)
				// 5. N-CREATE (Film Box)
				
				delete response;
				response = NULL;
				attrs.clear();
				
				if (layout.getTag(GKDCM_ImageDisplayFormat, tmpStd)) {
					e = newDicomElement(DCM_ImageDisplayFormat);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}
				
				{
					seq = new DcmSequenceOfItems(DCM_ReferencedFilmSessionSequence);
					
					item = new DcmItem();
					
					e = newDicomElement(DCM_ReferencedSOPClassUID);
					e->putString(UID_BasicFilmSessionSOPClass);
					item->insert(e);
					
					e = newDicomElement(DCM_ReferencedSOPInstanceUID);
					e->putString(filmSessionInstanceUID.c_str());
					
					item->insert(e);
					seq->insert(item);
					
					attrs.insert(seq);
				}
				
				if (film.getTag(GKDCM_FilmOrientation, tmpStd)) {
					e = newDicomElement(DCM_FilmOrientation);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}
				
				
				if (film.getTag(GKDCM_FilmSizeID, tmpStd)) {
					e = newDicomElement(DCM_FilmSizeID);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}
				
				if (film.getTag(GKDCM_MagnificationType, tmpStd)) {
					e = newDicomElement(DCM_MagnificationType);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}

				if (film.getTag(GKDCM_Trim, tmpStd)) {
					e = newDicomElement(DCM_Trim);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}
				
				if (layout.getTag(GKDCM_BorderDensity, tmpStd)) {
					e = newDicomElement(DCM_BorderDensity);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}

				if (layout.getTag(GKDCM_EmptyImageDensity, tmpStd)) {
					e = newDicomElement(DCM_EmptyImageDensity);
					e->putString(tmpStd.c_str());
					if (!tmpStd.empty()) {
						e->putString(tmpStd.c_str());
					}
					attrs.insert(e);
				}
				
				OFString filmBoxSOPInstanceUID;
				c = createRQ(UID_BasicFilmBoxSOPClass, filmBoxSOPInstanceUID, &attrs, rstatus, response);
				
				if (c.good()) {
					
					
					DcmSequenceOfItems* referencedImageBoxSequence = NULL;
					if (response == NULL || response->findAndGetSequence(DCM_ReferencedImageBoxSequence, referencedImageBoxSequence, false, false).bad()) {
						return makeOFCondition(OFM_dcmnet, 16, OF_error, "Unable to read referenced image box");
					}
					
					if (referencedImageBoxSequence == NULL || referencedImageBoxSequence->getItem(0)->findAndGetElement(DCM_ReferencedSOPClassUID, e, false, false).bad()) {
						return makeOFCondition(OFM_dcmnet, 16, OF_error, "No Image Box SOP Class received");
					}
					std::string referencedImageBoxSOPClass;
					{
						char* sopClassUID = NULL;
						if (e->getString(sopClassUID).bad()) {
							return makeOFCondition(OFM_dcmnet, 16, OF_error, "No Image Box SOP Class received");
						}
						referencedImageBoxSOPClass = sopClassUID;
					}
					
					#if 0 // TODO Color
						if (referencedImageBoxSOPClass != UID_BasicColorImageBoxSOPClass) {
							return makeOFCondition(OFM_dcmnet, 16, OF_error, "Invalid Image Box SOP Class for color print");
						}
					#else
						if (referencedImageBoxSOPClass != UID_BasicGrayscaleImageBoxSOPClass) {
							return makeOFCondition(OFM_dcmnet, 16, OF_error, "Invalid Image Box SOP Class for grayscale print");
						}
					#endif
					
					if (referencedImageBoxSequence->getItem(0)->findAndGetElement(DCM_ReferencedSOPInstanceUID, e, false, false).bad()) {
						return makeOFCondition(OFM_dcmnet, 16, OF_error, "No Image Box SOP Instance UID received");
					}
					std::string referencedImageBoxSOPInstanceUID;
					{
						char * sopInstanceUID = NULL;
						if (e->getString(sopInstanceUID).bad()) {
							return makeOFCondition(OFM_dcmnet, 16, OF_error, "Invalid Image Box SOP Instance UID received");
						}
						referencedImageBoxSOPInstanceUID = sopInstanceUID;
						//const char* test1 = referencedImageBoxSOPInstanceUID.c_str();
						//const char* test2 = sopInstanceUID;
					}
					std::cout << "Ref SOP = " << referencedImageBoxSOPInstanceUID << std::endl;
					
					
					// 6. N-SET (Image Box)
					
					unsigned int pos = 1;
					for (std::list<std::string>::const_iterator it = images.begin(); it != images.end(); ++it, pos++) {
						
						delete response;
						response = NULL;
						attrs.clear();
						
						const std::string& filePath = (*it);
						
						DcmFileFormat ff;
						DcmDataset* ds = NULL;
						
						LOG_DEBUG(ambitolog, "loading slice: [" << filePath.c_str() << "]" );
						
						c = ff.loadFile(filePath.c_str(), EXS_Unknown, EGL_noChange, DCM_MaxReadLength, ERM_autoDetect);
						
						std::string outfname;
						{
							std::ostringstream os;
							os << GNC::Entorno::Instance()->GetGinkgoTempDir().c_str() << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE) << "print_slice_" << pos << ".dcm";
							outfname = os.str();
						}
						
						LOG_DEBUG(ambitolog, "tempfile: [" << outfname.c_str() << "]" );
						
						DcmXfer filexfer(ff.getDataset()->getOriginalXfer());
						
						if (filexfer.getXfer() == EXS_JPEG2000LosslessOnly || filexfer.getXfer() == EXS_JPEG2000)
						{
							if (!DecompressJPEG2000(filePath, outfname)) {
								
								return makeOFCondition(OFM_dcmdata, 16, OF_error, "Unable to decompress JPEG2000");
							}
							//LOG_WARN(ambitolog, "The file is being uncompressed from JPG2000. Some tags could be lost");
							ff.loadFile(outfname.c_str(), EXS_Unknown, EGL_noChange, DCM_MaxReadLength, ERM_autoDetect);
						}
						
						ff.getDataset()->chooseRepresentation(EXS_LittleEndianExplicit, NULL);
						
						// check if everything went well
						if (ff.getDataset()->canWriteXfer(EXS_LittleEndianExplicit))
						{
							ff.loadAllDataIntoMemory();
							unlink(outfname.c_str());
							c = ff.saveFile( outfname.c_str(), EXS_LittleEndianExplicit);
						}
						else {
							return makeOFCondition(OFM_dcmdata, 16, OF_error, "Unable to convert to LittleEndianExplicit");
						}
						
						if (c.good()) {
							ds = ff.getDataset();
							
							c = attrs.chooseRepresentation(ff.getDataset()->getOriginalXfer(), NULL);
							if (c.bad()) {
								return makeOFCondition(OFM_dcmdata, 16, OF_error, "Unable to choose LittleEndianExplicit representation");
							}
							
							
							e = newDicomElement(DCM_ImageBoxPosition);
							e->putUint16(pos);
							attrs.insert(e);
							
							#if 0 // TODO 
								seq = new DcmSequenceOfItems(DCM_BasicColorImageSequence);
							#else
								seq = new DcmSequenceOfItems(DCM_BasicGrayscaleImageSequence);
							#endif

							item = new DcmItem();
							
							if (ds->findAndGetElement(DCM_SamplesPerPixel, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_PhotometricInterpretation, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_Rows, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_Columns, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_PixelAspectRatio, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_BitsAllocated, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_BitsStored, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_HighBit, e, false, true).good()) {
								item->insert(e);
							}
							
							if (ds->findAndGetElement(DCM_PixelRepresentation, e, false, true).good()) {
								item->insert(e);
							}

							if (ds->findAndGetElement(DCM_PixelData, e, false, true).good()) {
								item->insert(e);
							}
							
							seq->insert(item);
					
							attrs.insert(seq);
							//c = attrs.chooseRepresentation(EXS_LittleEndianExplicit, NULL);
							//attrs.insertEmptyElement(DCM_TransferSyntaxUID, EXS_LittleEndianExplicit);
						
							c = setRQ(referencedImageBoxSOPClass.c_str(), referencedImageBoxSOPInstanceUID.c_str(), &attrs, rstatus, response);
							
							if (c.bad()) {
								return c;
							}
						}
						else {
							return c;
						}
						
					}
					// 7. N-ACTION (Film Box)
					delete response;
					response = NULL;
					attrs.clear();
					
					OFCondition resultStatus = actionRQ(UID_BasicFilmBoxSOPClass, filmBoxSOPInstanceUID.c_str(), 1, NULL, rstatus, response);
					
					if (resultStatus.good()) {
						// 8. N-DELETE (Film Box)
						c = deleteRQ(UID_BasicFilmBoxSOPClass, filmBoxSOPInstanceUID.c_str(), rstatus);
					}
					else {
						c = EC_Normal;
					}
					
					if (c.good()) {
					
						// 9. N-DELETE (Film Session)
						c = deleteRQ(UID_BasicFilmSessionSOPClass, filmSessionInstanceUID.c_str(), rstatus);
					}
					
					return resultStatus;
				}
				else {
					return c;
				}
				
			}
			else {
				return c;
			}
		}
	}
	else {
		LOG_ERROR(ambitolog, "Error al conectar:" << r.text());
		Drop();
		Destroy();
		throw GIL::DICOM::PACSException(r.text());
	}
	
	if (!c.good()) {
		LOG_ERROR(ambitolog, "Error al enviar objeto: " << c.text());
		Drop();
		Destroy();
		throw GIL::DICOM::PACSException(c.text());
	}
	
	return c;
}

void PrintAssociation::OnAddPresentationContext(T_ASC_Parameters* params) {
	OFCondition cond = EC_Normal;
	
	const char* transferSyntaxes[3];
	int transferSyntaxCount = 0;

	bool implicitOnly = false;
	bool negotiatePresentationLUT = false;
	bool negotiateAnnotationBox = false;
	
	if (implicitOnly)
	{
        // coverity[DEADCODE] GW: don't know why this code is here, maybe future possibilities?
		transferSyntaxes[0] = UID_LittleEndianImplicitTransferSyntax;
		transferSyntaxCount = 1;
	} else {
		/* gLocalByteOrder is defined in dcxfer.h */
		if (gLocalByteOrder == EBO_LittleEndian) {
			/* we are on a little endian machine */
			transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
			transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
		} else {
			/* we are on a big endian machine */
			transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
			transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		}
		transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;
		transferSyntaxCount = 3;
	}
	
	/* we always propose basic grayscale, presentation LUT and annotation box*/
    if (cond.good())
        cond = ASC_addPresentationContext(params, 1, UID_BasicGrayscalePrintManagementMetaSOPClass, transferSyntaxes, transferSyntaxCount);

	if (negotiatePresentationLUT)
	{
        // coverity[DEADCODE] GW: don't know why this code is here, maybe future possibilities?
        if (cond.good())
            cond = ASC_addPresentationContext(params, 3, UID_PresentationLUTSOPClass, transferSyntaxes, transferSyntaxCount);
	}
	
	if (negotiateAnnotationBox)
	{
        // coverity[DEADCODE] GW: don't know why this code is here, maybe future possibilities?
        if (cond.good())
            cond = ASC_addPresentationContext(params, 5, UID_BasicAnnotationBoxSOPClass, transferSyntaxes, transferSyntaxCount);
	}
}

void PrintAssociation::dumpNMessage(T_DIMSE_Message &msg, DcmItem *dataset, OFBool outgoing)
{
    OFString str;
    if (outgoing) {
        DIMSE_dumpMessage(str, msg, DIMSE_OUTGOING, dataset);
    } else {
        DIMSE_dumpMessage(str, msg, DIMSE_INCOMING, dataset);
    }
    LOG_DEBUG(ambitolog, str);
}

OFCondition PrintAssociation::sendNRequest(
    T_ASC_PresentationContextID presId,
    T_DIMSE_Message &request,
    DcmDataset *rqDataSet,
    T_DIMSE_Message &response,
    DcmDataset* &statusDetail,
    DcmDataset* &rspDataset)
{
    OFCondition cond = EC_Normal;
    T_DIMSE_Command expectedResponse;
    DIC_US expectedMessageID=0;
    if (assoc == NULL)
    {
      return DIMSE_ILLEGALASSOCIATION;
    }

    T_DIMSE_DataSetType datasetType = DIMSE_DATASET_NULL;
    if (rqDataSet && (rqDataSet->card() > 0)) datasetType = DIMSE_DATASET_PRESENT;
    
    switch(request.CommandField)
    {
      case DIMSE_N_GET_RQ:
        request.msg.NGetRQ.DataSetType = datasetType;
        expectedResponse = DIMSE_N_GET_RSP;
        expectedMessageID = request.msg.NGetRQ.MessageID;
        break;
      case DIMSE_N_SET_RQ:
        request.msg.NSetRQ.DataSetType = datasetType;
        expectedResponse = DIMSE_N_SET_RSP;
        expectedMessageID = request.msg.NSetRQ.MessageID;
        break;
      case DIMSE_N_ACTION_RQ:
        request.msg.NActionRQ.DataSetType = datasetType;
        expectedResponse = DIMSE_N_ACTION_RSP;
        expectedMessageID = request.msg.NActionRQ.MessageID;
        break;
      case DIMSE_N_CREATE_RQ:
        request.msg.NCreateRQ.DataSetType = datasetType;
        expectedResponse = DIMSE_N_CREATE_RSP;
        expectedMessageID = request.msg.NCreateRQ.MessageID;
        break;
      case DIMSE_N_DELETE_RQ:
        request.msg.NDeleteRQ.DataSetType = datasetType;
        expectedResponse = DIMSE_N_DELETE_RSP;
        expectedMessageID = request.msg.NDeleteRQ.MessageID;
        break;
      default:
        return DIMSE_BADCOMMANDTYPE;
        /* break; */
    }

    dumpNMessage(request, rqDataSet, OFTrue);
    cond = DIMSE_sendMessageUsingMemoryData(assoc, presId, &request, NULL, rqDataSet, NULL, NULL);
    if (cond.bad()) return cond;

    T_ASC_PresentationContextID thisPresId;
    T_DIMSE_Message eventReportRsp;
    DIC_US eventReportStatus;
    do
    {
        thisPresId = presId;
        statusDetail = NULL;
        cond = DIMSE_receiveCommand(assoc, blockMode, this->m_timeout, &thisPresId, &response, &statusDetail);
        if (cond.bad()) return cond;

        if (response.CommandField == DIMSE_N_EVENT_REPORT_RQ)
        {
          /* handle N-EVENT-REPORT-RQ */
          rspDataset = NULL;
          if (response.msg.NEventReportRQ.DataSetType == DIMSE_DATASET_PRESENT)
          {
            cond = DIMSE_receiveDataSetInMemory(assoc, blockMode, m_timeout, &thisPresId, &rspDataset, NULL, NULL);
            if (cond.bad()) return cond;
          }  
          dumpNMessage(response, rspDataset, OFFalse);
          // call event handler if registered
          eventReportStatus = STATUS_Success;
          if (eventHandler) eventReportStatus = eventHandler->handleEvent(response.msg.NEventReportRQ, rspDataset, statusDetail);
          if (rspDataset) delete rspDataset;
          rspDataset = NULL;
          if (statusDetail) delete statusDetail;
          statusDetail = NULL;
          
          // send back N-EVENT-REPORT-RSP */
          eventReportRsp.CommandField = DIMSE_N_EVENT_REPORT_RSP;
          eventReportRsp.msg.NEventReportRSP.MessageIDBeingRespondedTo = response.msg.NEventReportRQ.MessageID;
          eventReportRsp.msg.NEventReportRSP.EventTypeID = response.msg.NEventReportRQ.EventTypeID;
          eventReportRsp.msg.NEventReportRSP.DimseStatus = eventReportStatus;
          eventReportRsp.msg.NEventReportRSP.DataSetType = DIMSE_DATASET_NULL;
          eventReportRsp.msg.NEventReportRSP.opts = O_NEVENTREPORT_EVENTTYPEID;
          eventReportRsp.msg.NEventReportRSP.AffectedSOPClassUID[0] = 0;
          eventReportRsp.msg.NEventReportRSP.AffectedSOPInstanceUID[0] = 0;
          dumpNMessage(eventReportRsp, NULL, OFTrue);
          cond = DIMSE_sendMessageUsingMemoryData(assoc, thisPresId, &eventReportRsp, NULL, NULL, NULL, NULL);
          if (cond.bad()) return cond;
        } else {
          /* No N-EVENT-REPORT-RQ. Check if this message is what we expected */
          if (response.CommandField != expectedResponse)
          {
            char buf1[256];
            sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
            return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
          }
          T_DIMSE_DataSetType responseDataset = DIMSE_DATASET_NULL;
          DIC_US responseMessageID = 0;
          /** change request to response */     
          switch(expectedResponse)
          {
            case DIMSE_N_GET_RSP:
              responseDataset = response.msg.NGetRSP.DataSetType;
              responseMessageID = response.msg.NGetRSP.MessageIDBeingRespondedTo;
              break;
            case DIMSE_N_SET_RSP:
              responseDataset = response.msg.NSetRSP.DataSetType;
              responseMessageID = response.msg.NSetRSP.MessageIDBeingRespondedTo;
              break;
            case DIMSE_N_ACTION_RSP:
              responseDataset = response.msg.NActionRSP.DataSetType;
              responseMessageID = response.msg.NActionRSP.MessageIDBeingRespondedTo;
              break;
            case DIMSE_N_CREATE_RSP:
              responseDataset = response.msg.NCreateRSP.DataSetType;
              responseMessageID = response.msg.NCreateRSP.MessageIDBeingRespondedTo;
              break;
            case DIMSE_N_DELETE_RSP:
              responseDataset = response.msg.NDeleteRSP.DataSetType;
              responseMessageID = response.msg.NDeleteRSP.MessageIDBeingRespondedTo;
              break;
            default:
              {
                char buf1[256];
                sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
                return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
              }
              /* break; */
          }
          if (responseMessageID != expectedMessageID)
          {
            char buf1[256];
            sprintf(buf1, "DIMSE: Unexpected Response Command Field: 0x%x", (unsigned)response.CommandField);
            return makeDcmnetCondition(DIMSEC_UNEXPECTEDRESPONSE, OF_error, buf1);
          }
          rspDataset = NULL;
          if (responseDataset == DIMSE_DATASET_PRESENT)
          {
            cond = DIMSE_receiveDataSetInMemory(assoc, blockMode, m_timeout, &thisPresId, &rspDataset, NULL, NULL);
            if (cond.bad()) return cond;
          }
          dumpNMessage(response, rspDataset, OFFalse);
        }
    } while (response.CommandField == DIMSE_N_EVENT_REPORT_RQ);
    return EC_Normal;
}    

OFCondition PrintAssociation::createRQ(
    const char *sopclassUID, 
    OFString& sopinstanceUID, 
    DcmDataset *attributeListIn, 
    Uint16& status, 
    DcmDataset* &attributeListOut)
{
  if (assoc == NULL)
  {
    return DIMSE_ILLEGALASSOCIATION;
  }
  if (sopclassUID==NULL)
  {
    return DIMSE_NULLKEY;
  }

  T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
  if (presCtx == 0)
  {
    return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }  

  T_DIMSE_Message request;
  T_DIMSE_Message response;
  DcmDataset *statusDetail = NULL;

  // construct N-CREATE-RQ
  request.CommandField = DIMSE_N_CREATE_RQ;
  request.msg.NCreateRQ.MessageID = assoc->nextMsgID++;
  strncpy(request.msg.NCreateRQ.AffectedSOPClassUID, sopclassUID, DIC_UI_LEN);
  request.msg.NCreateRQ.AffectedSOPClassUID[DIC_UI_LEN] = 0;

  if (sopinstanceUID.size() > 0)
  {
    strncpy(request.msg.NCreateRQ.AffectedSOPInstanceUID, sopinstanceUID.c_str(), DIC_UI_LEN);
    request.msg.NCreateRQ.AffectedSOPInstanceUID[DIC_UI_LEN] = 0;
    request.msg.NCreateRQ.opts = O_NCREATE_AFFECTEDSOPINSTANCEUID;
  } else {
    request.msg.NCreateRQ.AffectedSOPInstanceUID[0] = 0;
    request.msg.NCreateRQ.opts = 0;
  }
  
  OFCondition cond = sendNRequest(presCtx, request, attributeListIn, response, statusDetail, attributeListOut);
  if (statusDetail) delete statusDetail;
  if (cond.good()) 
  {
    status = response.msg.NCreateRSP.DimseStatus;
	 if (status != 0) {
		 LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
		 std::ostringstream ostr;
		 ostr << "Error sending create rq status: "<< status << " details: ";
		 DIMSE_printNStatusString(ostr, status);
		 throw GIL::DICOM::PACSException(ostr.str(), "GIL::PrintAssociation");
	 }
    // if response contains SOP Instance UID, copy it.
    if (response.msg.NCreateRSP.opts & O_NCREATE_AFFECTEDSOPINSTANCEUID)
    {
      sopinstanceUID = response.msg.NCreateRSP.AffectedSOPInstanceUID;
    }
  }
  return cond;
}

OFCondition PrintAssociation::setRQ(
    const char *sopclassUID, 
    const char *sopinstanceUID, 
    DcmDataset *modificationList, 
    Uint16& status, 
    DcmDataset* &attributeListOut)
{
  if (assoc == NULL)
  {
    return DIMSE_ILLEGALASSOCIATION;
  }
  if ((sopclassUID==NULL)||(sopinstanceUID==NULL)||(modificationList==NULL))
  {
    return DIMSE_NULLKEY;
  }

  T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
  if (presCtx == 0)
  {
    return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }  

  T_DIMSE_Message request;
  T_DIMSE_Message response;
  DcmDataset *statusDetail = NULL;

  // construct N-SET-RQ
  request.CommandField = DIMSE_N_SET_RQ;
  request.msg.NSetRQ.MessageID = assoc->nextMsgID++;
  strncpy(request.msg.NSetRQ.RequestedSOPClassUID, sopclassUID, DIC_UI_LEN);
  request.msg.NSetRQ.RequestedSOPClassUID[DIC_UI_LEN] = 0;
  strncpy(request.msg.NSetRQ.RequestedSOPInstanceUID, sopinstanceUID, DIC_UI_LEN);
  request.msg.NSetRQ.RequestedSOPInstanceUID[DIC_UI_LEN] = 0;
   
  OFCondition cond = sendNRequest(presCtx, request, modificationList, response, statusDetail, attributeListOut);
  if (cond.good()) status = response.msg.NSetRSP.DimseStatus;
  if (status != 0) {
		 LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
		 std::ostringstream ostr;
		 ostr << "Error sending create rq status: "<< status << " details: ";
		 DIMSE_printNStatusString(ostr, status);
		 throw GIL::DICOM::PACSException(ostr.str(), "GIL::PrintAssociation");
  }
  if (statusDetail) delete statusDetail;
  return cond;   
}

OFCondition PrintAssociation::getRQ(
    const char *sopclassUID, 
    const char *sopinstanceUID, 
    const Uint16 *attributeIdentifierList,
    ssize_t numShorts,
    Uint16& status, 
    DcmDataset* &attributeListOut)
{
  if (assoc == NULL)
  {
    return DIMSE_ILLEGALASSOCIATION;
  }
  if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
  {
    return DIMSE_NULLKEY;
  }

  T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
  if (presCtx == 0)
  {
    return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }  

  T_DIMSE_Message request;
  T_DIMSE_Message response;
  DcmDataset *statusDetail = NULL;

  // construct N-GET-RQ
  request.CommandField = DIMSE_N_GET_RQ;
  request.msg.NGetRQ.MessageID = assoc->nextMsgID++;
  strncpy(request.msg.NGetRQ.RequestedSOPClassUID, sopclassUID, DIC_UI_LEN);
  request.msg.NGetRQ.RequestedSOPClassUID[DIC_UI_LEN] = 0;
  strncpy(request.msg.NGetRQ.RequestedSOPInstanceUID, sopinstanceUID, DIC_UI_LEN);
  request.msg.NGetRQ.RequestedSOPInstanceUID[DIC_UI_LEN] = 0;

  request.msg.NGetRQ.ListCount = 0;
  if (attributeIdentifierList) request.msg.NGetRQ.ListCount = (int)numShorts;
  request.msg.NGetRQ.AttributeIdentifierList = (DIC_US *)attributeIdentifierList;
   
  OFCondition cond = sendNRequest(presCtx, request, NULL, response, statusDetail, attributeListOut);
  if (cond.good()) status = response.msg.NGetRSP.DimseStatus;
  if (status != 0) {
	 LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
	 std::ostringstream ostr;
	 ostr << "Error sending create rq status: "<< status << " details: ";
	 DIMSE_printNStatusString(ostr, status);
	 throw GIL::DICOM::PACSException(ostr.str(), "GIL::PrintAssociation");
  }
  if (statusDetail) delete statusDetail;
  return cond;   
}


OFCondition PrintAssociation::actionRQ(
    const char *sopclassUID, 
    const char *sopinstanceUID, 
    Uint16 actionTypeID, 
    DcmDataset *actionInformation,
    Uint16& status, 
    DcmDataset* &actionReply)
{
  if (assoc == NULL)
  {
    return DIMSE_ILLEGALASSOCIATION;
  }
  if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
  {
    return DIMSE_NULLKEY;
  }

  T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
  if (presCtx == 0)
  {
    return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }  

  T_DIMSE_Message request;
  T_DIMSE_Message response;
  DcmDataset *statusDetail = NULL;

  // construct N-ACTION-RQ
  request.CommandField = DIMSE_N_ACTION_RQ;
  request.msg.NActionRQ.MessageID = assoc->nextMsgID++;
  strncpy(request.msg.NActionRQ.RequestedSOPClassUID, sopclassUID, DIC_UI_LEN);
  request.msg.NActionRQ.RequestedSOPClassUID[DIC_UI_LEN] = 0;
  strncpy(request.msg.NActionRQ.RequestedSOPInstanceUID, sopinstanceUID, DIC_UI_LEN);
  request.msg.NActionRQ.RequestedSOPInstanceUID[DIC_UI_LEN] = 0;
  request.msg.NActionRQ.ActionTypeID = (DIC_US)actionTypeID;
   
  OFCondition cond = sendNRequest(presCtx, request, actionInformation, response, statusDetail, actionReply);
  if (cond.good()) status = response.msg.NActionRSP.DimseStatus;
  if (status != 0) {
		LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
		std::ostringstream ostr;
		ostr << "Error sending create rq status: "<< status << " details: ";
		DIMSE_printNStatusString(ostr, status);
		throw GIL::DICOM::PACSException(ostr.str(), "GIL::PrintAssociation");
  }
  if (statusDetail) delete statusDetail;
  return cond;   
}

OFCondition PrintAssociation::deleteRQ(
    const char *sopclassUID, 
    const char *sopinstanceUID, 
    Uint16& status)
{
  if (assoc == NULL)
  {
    return DIMSE_ILLEGALASSOCIATION;
  }
  if ((sopclassUID==NULL)||(sopinstanceUID==NULL))
  {
    return DIMSE_NULLKEY;
  }

  T_ASC_PresentationContextID presCtx = findAcceptedPC(sopclassUID);
  if (presCtx == 0)
  {
    return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }  

  T_DIMSE_Message request;
  T_DIMSE_Message response;
  DcmDataset *statusDetail = NULL;
  DcmDataset *attributeListOut = NULL;

  // construct N-DELETE-RQ
  request.CommandField = DIMSE_N_DELETE_RQ;
  request.msg.NDeleteRQ.MessageID = assoc->nextMsgID++;
  strncpy(request.msg.NDeleteRQ.RequestedSOPClassUID, sopclassUID, DIC_UI_LEN);
  request.msg.NDeleteRQ.RequestedSOPClassUID[DIC_UI_LEN] = 0;
  strncpy(request.msg.NDeleteRQ.RequestedSOPInstanceUID, sopinstanceUID, DIC_UI_LEN);
  request.msg.NDeleteRQ.RequestedSOPInstanceUID[DIC_UI_LEN] = 0;
   
  OFCondition cond = sendNRequest(presCtx, request, NULL, response, statusDetail, attributeListOut);
  if (cond.good()) status = response.msg.NDeleteRSP.DimseStatus;
  if (status != 0) {
	 LOG_ERROR(ambitolog, "Error sending create rq status: "<< status);
	 std::ostringstream ostr;
	 ostr << "Error sending create rq status: "<< status << " details: ";
	 DIMSE_printNStatusString(ostr, status);
	 throw GIL::DICOM::PACSException(ostr.str(), "GIL::PrintAssociation");
  }
  if (statusDetail) delete statusDetail;
  if (attributeListOut) delete attributeListOut;  // should never happen
  return cond;   
}

OFCondition PrintAssociation::releaseAssociation()
{
  OFCondition result = EC_Normal;
  if (assoc)
  {
    result = ASC_releaseAssociation(assoc);
	 Drop();
	 Destroy();
    assoc = NULL;
  }
  return result;
}

OFCondition PrintAssociation::abortAssociation()
{
	
  OFCondition result = EC_Normal;
  if (assoc)
  {
    result = ASC_abortAssociation(assoc);
	 Drop();
	 Destroy();
	 assoc =  NULL;
  }
  return result;
}

T_ASC_PresentationContextID PrintAssociation::findAcceptedPC(const char *sopclassuid)
{
  if ((assoc==NULL)||(sopclassuid==NULL)) return 0;

  // if the SOP class is one of the Basic Grayscale Print Management Meta SOP Classes,
  // look for a presentation context for Basic Grayscale Print.
  OFString sopclass(sopclassuid);
  if ((sopclass == UID_BasicFilmSessionSOPClass) ||
      (sopclass == UID_BasicFilmBoxSOPClass) ||
      (sopclass == UID_BasicGrayscaleImageBoxSOPClass) ||
      (sopclass == UID_PrinterSOPClass)) sopclassuid = UID_BasicGrayscalePrintManagementMetaSOPClass;
  return ASC_findAcceptedPresentationContextID(assoc, sopclassuid);
}


OFCondition PrintAssociation::negotiateAssociation(
  OFBool,
  OFBool,
  OFBool)
{
	OFCondition cond = EC_Normal;
	/*
  if (assoc)
  {
    return makeDcmnetCondition(DIMSEC_ILLEGALASSOCIATION, OF_error, "association already in place");
  }
  
  
  const char* transferSyntaxes[3];
  int transferSyntaxCount = 0;

  if (implicitOnly)
  {
      transferSyntaxes[0] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxCount = 1;
  } else {
      // gLocalByteOrder is defined in dcxfer.h
      if (gLocalByteOrder == EBO_LittleEndian) {
          // we are on a little endian machine
          transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
          transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
      } else {
          // we are on a big endian machine
          transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
          transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
      }
      transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;
      transferSyntaxCount = 3;
  }
  
  

  
  
  // create association
  LOG_INFO( ambitolog, "Requesting Association");
    
  if (cond.good()) 
  {
    cond = ASC_requestAssociation(Net->GetDcmtkNet(), NULL, &assoc);

    if (cond == DUL_ASSOCIATIONREJECTED)
    {
      OFString temp_str;
      T_ASC_RejectParameters rej;
      ASC_getRejectParameters(params, &rej);
      LOG_INFO( ambitolog, "Association Rejected" << OFendl << ASC_printRejectParameters(temp_str, &rej));
    } else {
      if (cond.bad()) 
      {
        // if assoc is non-NULL, then params has already been moved into the
        // assoc structure. Make sure we only delete once!
        if (assoc) ASC_destroyAssociation(&assoc);
        else if (params) ASC_destroyAssociationParameters(&params);

        if (net) ASC_dropNetwork(&net);
        assoc = NULL;
        net = NULL;
        return cond;
      }
    }       
  }                     
                        
  if ((cond.good()) && (0 == ASC_findAcceptedPresentationContextID(assoc, UID_BasicGrayscalePrintManagementMetaSOPClass)))
  {                     
    LOG_INFO( ambitolog, "Peer does not support Basic Grayscale Print Management, aborting association.");
    abortAssociation();
    cond = DIMSE_NOVALIDPRESENTATIONCONTEXTID;
  }
  
  if (cond.good())
  {
    LOG_INFO( ambitolog, "Association accepted (Max Send PDV: " << assoc->sendPDVLength << ")");
  } else {
    // params is now an alias to assoc->params. Don't call ASC_destroyAssociationParameters.
    if (assoc) ASC_destroyAssociation(&assoc);
    if (net) ASC_dropNetwork(&net);
    assoc = NULL;
    net = NULL;
  }
  */

  return cond;
}

OFBool PrintAssociation::printerSupportsPresentationLUT()
{
  if ((assoc)&&(0 != ASC_findAcceptedPresentationContextID(assoc, UID_PresentationLUTSOPClass))) return OFTrue;
  return OFFalse;
}

OFBool PrintAssociation::printerSupportsAnnotationBox()
{
  if ((assoc)&&(0 != ASC_findAcceptedPresentationContextID(assoc, UID_BasicAnnotationBoxSOPClass))) return OFTrue;
  return OFFalse;
}



