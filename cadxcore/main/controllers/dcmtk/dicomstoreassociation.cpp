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

#define LOGGER "C-STORE"

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

// For sime reason dcmtk-3.6.1-20150924 has an hiccup in
// dcmth/oflog/  logger.h and logmacro.h 
// since they are not used we define the include guards here
#define DCMTK_LOG4CPLUS_TRACELOGGER_H
#define DCMTK_LOG4CPLUS_LOGGING_MACROS_HEADER_


#include "dicomstoreassociation.h"
#include <api/dicom/imodelodicom.h>

#include <api/iproxynotificadorprogreso.h>
#include <main/controllers/controladorpermisos.h>
#include <main/entorno.h>
#include <iostream>

#include <main/controllers/dcmtk/dicomservers.h>

#include <dcmtk/ofstd/ofstring.h>
#include <dcmtk/dcmnet/dimse.h>
#include <dcmtk/dcmnet/diutil.h>
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcmetinf.h>
#include <dcmtk/dcmdata/dcfilefo.h>
//#include <dcmtk/dcmdata/dcdebug.h>
#include <dcmtk/dcmdata/dcuid.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdeftag.h>
//#include "cmdlnarg.h"
#include <dcmtk/ofstd/ofconapp.h>
#include <dcmtk/dcmdata/dcuid.h>     /* for dcmtk version name */
#include <dcmtk/dcmnet/dicom.h>     /* for DICOM_APPLICATION_REQUESTOR */
#include <dcmtk/dcmdata/dcostrmz.h>  /* for dcmZlibCompressionLevel */
#include <dcmtk/dcmnet/dcasccfg.h>  /* for class DcmAssociationConfiguration */
#include <dcmtk/dcmnet/dcasccff.h>  /* for class DcmAssociationConfigurationFile */

#include <dcmtk/dcmjpeg/djencode.h>  /* for dcmjpeg encoders */
#include <dcmtk/dcmjpeg/djdecode.h>  /* for dcmjpeg encoders */
#include <dcmtk/dcmdata/dcrledrg.h>  /* for DcmRLEDecoderRegistration */
#include <dcmtk/dcmdata/dcrleerg.h>  /* for DcmRLEEncoderRegistration */
#include <dcmtk/dcmjpeg/djrploss.h>
#include <dcmtk/dcmjpeg/djrplol.h>
#include <dcmtk/dcmdata/dcpixel.h>
#include <dcmtk/dcmdata/dcrlerp.h>

#include "tls/tls.h"
#include "tls/gtlslayer.h"
#include <dcmtk/dcmtls/tlstrans.h>

#include "helpercompresion.h"

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

#include <api/controllers/icontroladorlog.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/dcmtk/ginkgouid.h>
#include "helpers.h"

#include <wx/filename.h>
#include <sstream>

/////////////////////-------------------------DEFINES--------------------/////////////////

namespace GIL {
	namespace DICOM {

		/* default application titles */
#define APPLICATIONTITLE        "GINKGO"
#define PEERAPPLICATIONTITLE    "DCM4CHEE"

		static OFBool opt_verbose = OFTrue;
		static OFBool opt_showPresentationContexts = OFTrue;
		static OFBool opt_debug = OFTrue;
		static OFBool opt_abortAssociation = OFFalse;
		static OFCmdUnsignedInt opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
		//static OFCmdUnsignedInt opt_maxSendPDULength = 0;
		static E_TransferSyntax opt_networkTransferSyntax = EXS_LittleEndianExplicit;

		//static OFBool opt_haltOnUnsuccessfulStore = OFTrue;
		static OFBool opt_haltOnUnsuccessfulStore = OFFalse;
		static OFBool unsuccessfulStoreEncountered = OFFalse;
		static std::string errorEncontrado;
		int lastStatusCode = STATUS_Success;

		static OFBool opt_proposeOnlyRequiredPresentationContexts = OFFalse;
		static OFBool opt_combineProposedTransferSyntaxes = OFFalse;

		static OFCmdUnsignedInt opt_repeatCount = 1;
		static OFCmdUnsignedInt opt_inventPatientCount = 25;
		static OFCmdUnsignedInt opt_inventStudyCount = 50;
		static OFCmdUnsignedInt opt_inventSeriesCount = 100;
		static OFBool opt_inventSOPInstanceInformation = OFFalse;
		static OFBool opt_correctUIDPadding = OFFalse;
		static OFString patientNamePrefix("OFFIS^TEST_PN_");   // PatientName is PN (maximum 16 chars)
		static OFString patientIDPrefix("PID_"); // PatientID is LO (maximum 64 chars)
		static OFString studyIDPrefix("SID_");   // StudyID is SH (maximum 16 chars)
		static OFString accessionNumberPrefix;  // AccessionNumber is SH (maximum 16 chars)
		//static OFBool opt_secureConnection = OFFalse; /* default: no secure connection */
		//static const char *opt_configFile = NULL;
		//static const char *opt_profileName = NULL;
		T_DIMSE_BlockingMode opt_blockMode = DIMSE_NONBLOCKING;
		int opt_dimse_timeout = 0;
		int opt_acse_timeout = 30;
		int opt_Quality = 100;

		//cancelacion

		typedef struct _StoreCallbackInfo {
			T_ASC_Association* assoc;
		} GetCallbackInfo;

		//progreso
		static GNC::IProxyNotificadorProgreso* m_pNotificadorProgreso = NULL;
		static std::string m_mensaje("");
		typedef struct TMedida{
			time_t m_instante;
			int bytesDescargados;
			float oldTasa;
			TMedida() {
				m_instante = time(NULL);
				bytesDescargados = 0;
				oldTasa = 0.0f;
			}
		} TMedida;
		static TMedida m_medida;

		float TasaTransferencia(int bytesDescargados)
		{
			time_t nuevoInstante = time(NULL);
			const double tiempo = difftime(nuevoInstante,m_medida.m_instante);
			if(tiempo>0.15f){
				if(bytesDescargados > m_medida.bytesDescargados) {
					const double bytesRecibidosPeriodo = bytesDescargados - m_medida.bytesDescargados;
					m_medida.oldTasa = ( ((float)bytesRecibidosPeriodo/1024.0f)/tiempo );
					m_medida.bytesDescargados = bytesDescargados;
					m_medida.m_instante = nuevoInstante;
				}
			}
			return m_medida.oldTasa;
		}

		void ResetearMedida(bool clearTasa = false) {
			m_medida.bytesDescargados =-1;
			m_medida.m_instante = time(NULL);
			if(clearTasa) {
				m_medida.oldTasa=0.0f;
			}
		}

		static void errmsg(const std::string& mensaje, bool parar)
		{
			LOG_ERROR(LOGGER,mensaje);
			if(parar) {
				throw GIL::DICOM::PACSException(mensaje);
			}
		}

		/*
		 static void
		 errmsg(const OFString& message, bool parar)
		 {
		 std::string mensaje(message.c_str());
		 errmsg(mensaje,parar);
		 }
		 */

		static OFBool isaListMember(OFList<OFString>& lst, OFString& s)
		{
			OFListIterator(OFString) cur = lst.begin();
			OFListIterator(OFString) end = lst.end();

			OFBool found = OFFalse;

			while (cur != end && !found) {

				found = (s == *cur);

				++cur;
			}

			return found;
		}

		static OFCondition addPresentationContext(T_ASC_Parameters *params,
							   int presentationContextId, const OFString& abstractSyntax,
							   const OFString& transferSyntax,
							   T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
		{
			const char* c_p = transferSyntax.c_str();
			OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
														  abstractSyntax.c_str(), &c_p, 1, proposedRole);
			return cond;
		}

		static OFCondition addPresentationContext(T_ASC_Parameters *params,
							   int presentationContextId, const OFString& abstractSyntax,
							   const OFList<OFString>& transferSyntaxList,
							   T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
		{
			// create an array of supported/possible transfer syntaxes
			const char** transferSyntaxes = new const char*[transferSyntaxList.size()];
			int transferSyntaxCount = 0;
			OFListConstIterator(OFString) s_cur = transferSyntaxList.begin();
			OFListConstIterator(OFString) s_end = transferSyntaxList.end();
			while (s_cur != s_end) {
				transferSyntaxes[transferSyntaxCount++] = (*s_cur).c_str();
				++s_cur;
			}

			OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
														  abstractSyntax.c_str(), transferSyntaxes, transferSyntaxCount, proposedRole);

			delete[] transferSyntaxes;
			return cond;
		}

		static OFCondition addStoragePresentationContexts(T_ASC_Parameters *params, OFList<OFString>& sopClasses)
		{
			/*
			 * Each SOP Class will be proposed in two presentation contexts (unless
			 * the opt_combineProposedTransferSyntaxes global variable is true).
			 * The command line specified a preferred transfer syntax to use.
			 * This prefered transfer syntax will be proposed in one
			 * presentation context and a set of alternative (fallback) transfer
			 * syntaxes will be proposed in a different presentation context.
			 *
			 * Generally, we prefer to use Explicitly encoded transfer syntaxes
			 * and if running on a Little Endian machine we prefer
			 * LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
			 * Some SCP implementations will just select the first transfer
			 * syntax they support (this is not part of the standard) so
			 * organise the proposed transfer syntaxes to take advantage
			 * of such behaviour.
			 */

			// Which transfer syntax was preferred on the command line
			OFString preferredTransferSyntax;
			if (opt_networkTransferSyntax == EXS_Unknown) {
				/* gLocalByteOrder is defined in dcxfer.h */
				if (gLocalByteOrder == EBO_LittleEndian) {
					/* we are on a little endian machine */
					preferredTransferSyntax = UID_LittleEndianExplicitTransferSyntax;
				} else {
					/* we are on a big endian machine */
					preferredTransferSyntax = UID_BigEndianExplicitTransferSyntax;
				}
			} else {
				DcmXfer xfer(opt_networkTransferSyntax);
				preferredTransferSyntax = xfer.getXferID();
			}

			OFListIterator(OFString) s_cur;
			OFListIterator(OFString) s_end;


			OFList<OFString> fallbackSyntaxes;
			fallbackSyntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
			fallbackSyntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
			fallbackSyntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
			// Remove the preferred syntax from the fallback list
			fallbackSyntaxes.remove(preferredTransferSyntax);
			// If little endian implicit is preferred then we don't need any fallback syntaxes
			// because it is the default transfer syntax and all applications must support it.
			if (opt_networkTransferSyntax == EXS_LittleEndianImplicit) {
				fallbackSyntaxes.clear();
			}

			// created a list of transfer syntaxes combined from the preferred and fallback syntaxes
			OFList<OFString> combinedSyntaxes;
			s_cur = fallbackSyntaxes.begin();
			s_end = fallbackSyntaxes.end();
			combinedSyntaxes.push_back(preferredTransferSyntax);
			while (s_cur != s_end)
			{
				if (!isaListMember(combinedSyntaxes, *s_cur)) combinedSyntaxes.push_back(*s_cur);
				++s_cur;
			}

			if (!opt_proposeOnlyRequiredPresentationContexts) {
				// add the (short list of) known storage sop classes to the list
				// the array of Storage SOP Class UIDs comes from dcuid.h
				
				//FIXME: ÑAPA HECHA PARA QUE PUEDA SUBIR VIDEOS(EL -1 Y EL PUSH DE DESPUES)
				for (int i=0; i<numberOfDcmShortSCUStorageSOPClassUIDs-1; i++) {
					sopClasses.push_back(dcmShortSCUStorageSOPClassUIDs[i]);
				}
				//	UID_VideoPhotographicImageStorage
				sopClasses.push_back("1.2.840.10008.5.1.4.1.1.77.1.4.1");
			}

			// thin out the sop classes to remove any duplicates.
			OFList<OFString> sops;
			s_cur = sopClasses.begin();
			s_end = sopClasses.end();
			while (s_cur != s_end) {
				if (!isaListMember(sops, *s_cur)) {
					sops.push_back(*s_cur);
				}
				++s_cur;
			}

			// add a presentations context for each sop class / transfer syntax pair
			OFCondition cond = EC_Normal;
			int pid = 1; // presentation context id
			s_cur = sops.begin();
			s_end = sops.end();
			while (s_cur != s_end && cond.good()) {

				if (pid > 255) {
					LOG_WARN("C-GET", "Too many PresentationContexts setted");
					return cond;
				}

				if (opt_combineProposedTransferSyntaxes) {
					cond = addPresentationContext(params, pid, *s_cur, combinedSyntaxes);
					pid += 2;   /* only odd presentation context id's */
				} else {

					// sop class with preferred transfer syntax
					cond = addPresentationContext(params, pid, *s_cur, preferredTransferSyntax);
					pid += 2;   /* only odd presentation context id's */

					if (fallbackSyntaxes.size() > 0) {
						if (pid > 255) {
							//errmsg("Demasiados contextos de presentacion");
							return ASC_BADPRESENTATIONCONTEXTID;
						}

						// sop class with fallback transfer syntax
						cond = addPresentationContext(params, pid, *s_cur, fallbackSyntaxes);
						pid += 2;       /* only odd presentation context id's */
					}
				}
				++s_cur;
			}

			return cond;
		}

		static OFString intToString(int i)
		{
			char numbuf[32];
			sprintf(numbuf, "%d", i);
			return numbuf;
		}

		static int secondsSince1970()
		{
			time_t t = time(NULL);
			return (int)t;
		}

		static OFBool updateStringAttributeValue(DcmItem* dataset, const DcmTagKey& key, OFString& value)
		{
			DcmStack stack;
			DcmTag tag(key);

			OFCondition cond = EC_Normal;
			cond = dataset->search(key, stack, ESM_fromHere, OFFalse);
			if (cond != EC_Normal) {
				LOG_DEBUG(LOGGER,"error: updateStringAttributeValue: cannot find: " << tag.getTagName() << " " << key << ": "<< cond.text());
				return OFFalse;
			}

			DcmElement* elem = (DcmElement*) stack.top();

			DcmVR vr(elem->ident());
			if (elem->getLength() > vr.getMaxValueLength()) {
				LOG_DEBUG(LOGGER,"error: updateStringAttributeValue: INTERNAL ERROR: " << tag.getTagName()<< " " << key << ": value too large (max "<< vr.getMaxValueLength() << ") for " << vr.getVRName() << " value: " << value );
				return OFFalse;
			}

			cond = elem->putOFStringArray(value);
			if (cond != EC_Normal) {
				LOG_DEBUG(LOGGER, "error: updateStringAttributeValue: cannot put string in attribute: " << tag.getTagName()<< " " << key << ": "<< cond.text());
				return OFFalse;
			}

			return OFTrue;
		}

		static void replaceSOPInstanceInformation(DcmDataset* dataset)
		{
			static OFCmdUnsignedInt patientCounter = 0;
			static OFCmdUnsignedInt studyCounter = 0;
			static OFCmdUnsignedInt seriesCounter = 0;
			static OFCmdUnsignedInt imageCounter = 0;
			static OFString seriesInstanceUID;
			static OFString seriesNumber;
			static OFString studyInstanceUID;
			static OFString studyID;
			static OFString accessionNumber;
			static OFString patientID;
			static OFString patientName;

			if (seriesInstanceUID.length() == 0) seriesInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_SeriesRoot, (int)seriesCounter).c_str();
			if (seriesNumber.length() == 0) seriesNumber = intToString((int)seriesCounter);
			if (studyInstanceUID.length() == 0) studyInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_StudyRoot, (int)studyCounter).c_str();
			if (studyID.length() == 0) studyID = studyIDPrefix + intToString((int)secondsSince1970()) + intToString((int)studyCounter);
			if (accessionNumber.length() == 0) accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
			if (patientID.length() == 0) patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
			if (patientName.length() == 0) patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);

			if (imageCounter >= opt_inventSeriesCount) {
				imageCounter = 0;
				seriesCounter++;
				seriesInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_SeriesRoot, (int)seriesCounter).c_str();
				seriesNumber = intToString((int)seriesCounter);
			}
			if (seriesCounter >= opt_inventStudyCount) {
				seriesCounter = 0;
				studyCounter++;				
				studyInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_StudyRoot, (int)studyCounter).c_str();
				studyID = studyIDPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
				accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
			}
			if (studyCounter >= opt_inventPatientCount) {
				// we create as many patients as necessary */
				studyCounter = 0;
				patientCounter++;
				patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
				patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
			}

			OFString sopInstanceUID = GIL::DICOM::MakeUID(GIL::DICOM::GUID_InstanceRoot, (int)imageCounter).c_str();
			OFString imageNumber = intToString((int)imageCounter);

			if (opt_debug) {
				LOG_DEBUG(LOGGER, "Inventing Identifying Information (" <<
						  "pa" << patientCounter << ", st" << studyCounter <<
						  ", se" << seriesCounter << ", im" << imageCounter << "): " << std::endl
						  << "  PatientName=" << patientName << std::endl
						  << "  PatientID=" << patientID << std::endl
						  << "  StudyInstanceUID=" << studyInstanceUID << std::endl
						  << "  StudyID=" << studyID << std::endl
						  << "  SeriesInstanceUID=" << seriesInstanceUID << std::endl
						  << "  SeriesNumber=" << seriesNumber << std::endl
						  << "  SOPInstanceUID=" << sopInstanceUID << std::endl
						  << "  ImageNumber=" << imageNumber << std::endl);
			}

			updateStringAttributeValue(dataset, DCM_PatientName, patientName);
			updateStringAttributeValue(dataset, DCM_PatientID, patientID);
			updateStringAttributeValue(dataset, DCM_StudyInstanceUID, studyInstanceUID);
			updateStringAttributeValue(dataset, DCM_StudyID, studyID);
			updateStringAttributeValue(dataset, DCM_SeriesInstanceUID, seriesInstanceUID);
			updateStringAttributeValue(dataset, DCM_SeriesNumber, seriesNumber);
			updateStringAttributeValue(dataset, DCM_SOPInstanceUID, sopInstanceUID);
			updateStringAttributeValue(dataset, DCM_InstanceNumber, imageNumber);

			imageCounter++;
		}

		// Conversión de transferencias
        // coverity[PASS_BY_VALUE] (GW: assuming this was done intentional)
        static OFBool decompressFile(DcmFileFormat fileformat, const char * fname, const char *outfname)
		{
			OFBool status = true;
			OFCondition cond;
			DcmXfer filexfer(fileformat.getDataset()->getOriginalXfer());

			if (filexfer.getXfer() == EXS_JPEG2000LosslessOnly || filexfer.getXfer() == EXS_JPEG2000)
			{
				status = DecompressJPEG2000(fname, outfname);
				LOG_WARN(LOGGER,"The file is being uncompressed from JPG2000. Some tags could be lost");
			}
			else {

				DcmDataset *dataset = fileformat.getDataset();

				// decompress data set if compressed
				dataset->chooseRepresentation(EXS_LittleEndianExplicit, NULL);

				// check if everything went well
				if (dataset->canWriteXfer(EXS_LittleEndianExplicit))
				{
					fileformat.loadAllDataIntoMemory();
					unlink( outfname);
					cond = fileformat.saveFile( outfname, EXS_LittleEndianExplicit);
					status =  (cond.good()) ? true : false;
				}
				else
					status = false;
			}


			return status;
		}

        // coverity[PASS_BY_VALUE] (GW: assuming this was done intentional)
        static OFBool compressFile(DcmFileFormat fileformat, const char * fname, const char *outfname)
		{
			OFCondition cond;
			OFBool status = true;
			DcmXfer filexfer(fileformat.getDataset()->getOriginalXfer());


			if (opt_networkTransferSyntax == EXS_JPEG2000)
			{
				status = CompressJPEG2000(fname, outfname);
				LOG_WARN(LOGGER,"Se ha comprimido a JPEG2000, podrían perderse algunos tags");
			}
			else if  (opt_networkTransferSyntax == EXS_JPEG2000LosslessOnly)
			{
				status = CompressJPEG2000(fname, outfname);
				LOG_WARN(LOGGER,"Se ha comprimido a JPEG2000, podrían perderse algunos tags");
			}
			else
			{
				DcmDataset *dataset = fileformat.getDataset();
				DcmItem *metaInfo = fileformat.getMetaInfo();
				DcmRepresentationParameter *params;
				DJ_RPLossy lossyParams(opt_Quality);
				DcmRLERepresentationParameter rleParams;
				DJ_RPLossless losslessParams; // codec parameters, we use the defaults
				if (opt_networkTransferSyntax == EXS_JPEGProcess14SV1)
					params = &losslessParams;
				else if (opt_networkTransferSyntax == EXS_JPEGProcess2_4)
					params = &lossyParams;
				else if (opt_networkTransferSyntax == EXS_RLELossless)
					params = &rleParams;
				else
					params = &losslessParams;


				// this causes the lossless JPEG version of the dataset to be created
				dataset->chooseRepresentation(opt_networkTransferSyntax, params);

				// check if everything went well
				if (dataset->canWriteXfer(opt_networkTransferSyntax))
				{
					// force the meta-header UIDs to be re-generated when storing the file
					// since the UIDs in the data set may have changed
					delete metaInfo->remove(DCM_MediaStorageSOPClassUID);
					delete metaInfo->remove(DCM_MediaStorageSOPInstanceUID);

					// store in lossless JPEG format

					fileformat.loadAllDataIntoMemory();

					unlink( outfname);

					cond = fileformat.saveFile( outfname, opt_networkTransferSyntax);
					status =  (cond.good()) ? true : false;
				}
				else {
					status = false;
				}
			}

			return status;
		}


		static long seed = 0;


		static OFCondition storeSCU(T_ASC_Association * assoc, const char *fname)
		/*
		 * This function will read all the information from the given file,
		 * figure out a corresponding presentation context which will be used
		 * to transmit the information over the network to the SCP, and it
		 * will finally initiate the transmission of all data to the SCP.
		 *
		 * Parameters:
		 *   assoc - [in] The association (network connection to another DICOM application).
		 *   fname - [in] Name of the file which shall be processed.
		 */
		{
			DIC_US msgId = assoc->nextMsgID++;
			T_ASC_PresentationContextID presId;
			T_DIMSE_C_StoreRQ req;
			T_DIMSE_C_StoreRSP rsp;
			DIC_UI sopClass;
			DIC_UI sopInstance;
			std::ostringstream outfnameStringStream;
			_StoreCallbackInfo storeCallbackData;

			outfnameStringStream << GNC::Entorno::Instance()->GetGinkgoTempDir().c_str() << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE) << seed++ << ".dcm";

			std::string outfname = outfnameStringStream.str();

			LOG_DEBUG(LOGGER, "tempfile: [" << outfname.c_str() << "]" );

			//sprintf( outfname, "%s%ld.dcm",  "D:\\temp\\", seed++);

			unsuccessfulStoreEncountered = OFTrue; // assumption

			if (opt_debug) {
				LOG_DEBUG(LOGGER, "--------------------------\nSending file: "<< fname);
			}

			/* read information from file. After the call to DcmFileFormat::loadFile(...) the information */
			/* which is encapsulated in the file will be available through the DcmFileFormat object. */
			/* In detail, it will be available through calls to DcmFileFormat::getMetaInfo() (for */
			/* meta header information) and DcmFileFormat::getDataset() (for data set information). */
			DcmFileFormat dcmff;
			OFCondition cond = dcmff.loadFile(fname);

			/* NO LO PONEMOS A FUEGO COMPRIMIMOS Y DESCOMPRIMIMOS
			 opt_networkTransferSyntax = dcmff.getDataset()->getOriginalXfer();
			 */

			/* figure out if an error occured while the file was read*/
			if (cond.bad()) {
				LOG_DEBUG(LOGGER, "Bad DICOM file " << fname);
				return cond;
			}

			/* if required, invent new SOP instance information for the current data set (user option) */
			if (opt_inventSOPInstanceInformation) {
				replaceSOPInstanceInformation(dcmff.getDataset());
			}

			/* figure out which SOP class and SOP instance is encapsulated in the file */
			if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(),
													 sopClass, sopInstance, opt_correctUIDPadding)) {
				LOG_DEBUG(LOGGER, "No SOP Class & Instance UIDs in file " << fname);
				return DIMSE_BADDATA;
			}

			/* figure out which of the accepted presentation contexts should be used */
			DcmXfer filexfer(dcmff.getDataset()->getOriginalXfer());

			/* special case: if the file uses an unencapsulated transfer syntax (uncompressed
			 * or deflated explicit VR) and we prefer deflated explicit VR, then try
			 * to find a presentation context for deflated explicit VR first.
			 */
			if (filexfer.isNotEncapsulated() &&
				opt_networkTransferSyntax == EXS_DeflatedLittleEndianExplicit)
			{
				filexfer = EXS_DeflatedLittleEndianExplicit;
			}


			/************* do on the fly conversion here*********************/

			//printf("on the fly conversion\n");
			//we have a valid presentation ID,.Chaeck and see if file is consistent with it
			DcmXfer preferredXfer(opt_networkTransferSyntax);

			OFBool status = false;
			presId = ASC_findAcceptedPresentationContextID(assoc, sopClass, preferredXfer.getXferID());
			T_ASC_PresentationContext pc;
			ASC_findAcceptedPresentationContext(assoc->params, presId, &pc);
			DcmXfer proposedTransfer(pc.acceptedTransferSyntax);

			status = true;

			if (presId != 0)
			{
				if (filexfer.isNotEncapsulated() && proposedTransfer.isNotEncapsulated()) {
					// do nothing
					status = true;
				}
				else if (filexfer.isEncapsulated() && proposedTransfer.isNotEncapsulated()) {
					status = decompressFile(dcmff, fname, outfname.c_str());
				}
				else if (filexfer.isNotEncapsulated() && proposedTransfer.isEncapsulated()) {
					status = compressFile(dcmff, fname, outfname.c_str());
				}
				else if (filexfer.getXfer() != opt_networkTransferSyntax)
				{
					// The file is already compressed, we will re-compress the file.....
					//E_TransferSyntax fileTS = filexfer.getXfer();

					if( (filexfer.getXfer() == EXS_JPEG2000LosslessOnly && preferredXfer.getXfer() == EXS_JPEG2000) ||
					   (filexfer.getXfer() == EXS_JPEG2000 && preferredXfer.getXfer() == EXS_JPEG2000LosslessOnly))
					{
					}
					else {
						LOG_WARN(LOGGER,"Recompressing files that are already compressed, you should optimize your ts parameters to avoid this: presentation for syntax:" << dcmFindNameOfUID(filexfer.getXferID()) << " -> " << dcmFindNameOfUID(preferredXfer.getXferID()));
					}
					status = compressFile(dcmff, fname, outfname.c_str());
				}
			}
			else
				status = false;


			if (status)
			{
				cond = 	dcmff.loadFile( outfname.c_str());
				filexfer = dcmff.getDataset()->getOriginalXfer();

				/* figure out which SOP class and SOP instance is encapsulated in the file */
				if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(),
														 sopClass, sopInstance, opt_correctUIDPadding)) {
					LOG_DEBUG(LOGGER, "No SOP Class & Instance UIDs in file " << outfname);
					return DIMSE_BADDATA;
				}

				fname = outfname.c_str();
			} else {
				const char *modalityName = dcmSOPClassUIDToModality(sopClass);
				if (!modalityName) modalityName = dcmFindNameOfUID(sopClass);
				if (!modalityName) modalityName = "unknown SOP class";
				LOG_DEBUG(LOGGER, "No presentation context for:  " << modalityName << "  "<< sopClass);
				std::ostringstream ostr;
				ostr << _Std("Ginkgo CADx is unable to compress from ") << filexfer.getXferName() << _Std(" to ") << proposedTransfer.getXferName();
				errmsg(ostr.str(),true);
				return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
			}

			if (filexfer.getXfer() != EXS_Unknown)
				presId = ASC_findAcceptedPresentationContextID(assoc, sopClass, filexfer.getXferID());
			else
				presId = ASC_findAcceptedPresentationContextID(assoc, sopClass);

			if (presId == 0) {
				const char *modalityName = dcmSOPClassUIDToModality(sopClass);
				if (!modalityName) modalityName = dcmFindNameOfUID(sopClass);
				if (!modalityName) modalityName = "unknown SOP class";
				LOG_DEBUG(LOGGER, "No presentation context for:  " << modalityName << "  "<< sopClass);
				return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
			}

			/* if required, dump general information concerning transfer syntaxes */
			if (opt_verbose) {
				DcmXfer fileTransfer(dcmff.getDataset()->getOriginalXfer());
				T_ASC_PresentationContext pc;
				ASC_findAcceptedPresentationContext(assoc->params, presId, &pc);
				DcmXfer netTransfer(pc.acceptedTransferSyntax);
				LOG_DEBUG(LOGGER,"Transfer " << dcmFindNameOfUID(fileTransfer.getXferID()) << " -> " << dcmFindNameOfUID(netTransfer.getXferID()));
			}

			/* prepare the transmission of data */
			bzero((char*)&req, sizeof(req));
			req.MessageID = msgId;
			strcpy(req.AffectedSOPClassUID, sopClass);
			strcpy(req.AffectedSOPInstanceUID, sopInstance);
			req.DataSetType = DIMSE_DATASET_PRESENT;
			req.Priority = DIMSE_PRIORITY_LOW;

			/* if required, dump some more general information */
			if (opt_debug) {
				LOG_DEBUG(LOGGER,"Store SCU RQ: MsgID (" <<msgId <<") "<< dcmSOPClassUIDToModality(sopClass));
			}

			/* finally conduct transmission of data */
			{
				storeCallbackData.assoc = assoc;
				DcmDataset *statusDetail = NULL;
				cond = DIMSE_storeUser(assoc, presId, &req,
									   NULL, dcmff.getDataset(), GIL::DICOM::DicomStoreAssociation::progressCallback, &storeCallbackData,
									   opt_blockMode, opt_dimse_timeout,
									   &rsp, &statusDetail, NULL, OFStandard::getFileSize(fname));

				/*
				 * If store command completed normally, with a status
				 * of success or some warning then the image was accepted.
				 */
				if (cond == EC_Normal && (rsp.DimseStatus == STATUS_Success || DICOM_WARNING_STATUS(rsp.DimseStatus))) {
					unsuccessfulStoreEncountered = OFFalse;
					/* dump status detail information if there is some */
					if (statusDetail != NULL) {
						LOG_DEBUG(LOGGER, "DIMSE_storeUser(): Estado: " << std::endl << DumpDataset(statusDetail));
					}
				} else {
					//error!!!
					std::stringstream os;
					os<< "Error al enviar el archivo";
					if(statusDetail!=NULL)
						statusDetail->print(os);
					os << ": " << cond.text();
					errorEncontrado = os.str();
					LOG_ERROR(LOGGER, errorEncontrado);
				}
				delete statusDetail;
				statusDetail = NULL;
			}

			/* remember the response's status for later transmissions of data */
			lastStatusCode = rsp.DimseStatus;

			/* dump some more general information */
			if (cond == EC_Normal)
			{
				if (opt_debug) {
					LOG_DEBUG(LOGGER,"Fichero Subido Correctamente " << fname);
				}
			}
			else
			{
				LOG_DEBUG(LOGGER,"Store failed: " << fname);
				DimseCondition::dump(cond);
			}

			if( status)
			{
				// We created a temporary file. Delete it now.

				unlink( outfname.c_str());
			}

			/* return */
			return cond;
		}


		static OFCondition cstore(T_ASC_Association * assoc, const OFString& fname)
		/*
		 * This function will process the given file as often as is specified by opt_repeatCount.
		 * "Process" in this case means "read file, send C-STORE-RQ, receive C-STORE-RSP".
		 *
		 * Parameters:
		 *   assoc - [in] The association (network connection to another DICOM application).
		 *   fname - [in] Name of the file which shall be processed.
		 */
		{
			OFCondition cond = EC_Normal;

			/* opt_repeatCount specifies how many times a certain file shall be processed */
			int n = (int)opt_repeatCount;

			/* as long as no error occured and the counter does not equal 0 */
			while ((cond.good()) && n-- && !(opt_haltOnUnsuccessfulStore && unsuccessfulStoreEncountered))
			{
				/* process file (read file, send C-STORE-RQ, receive C-STORE-RSP) */
				cond = storeSCU(assoc, fname.c_str());
			}

			// we don't want to return an error code if --no-halt was specified.
			if (! opt_haltOnUnsuccessfulStore)
			{
				cond = EC_Normal;
			}

			/* return result value */
			return cond;
		}

		DicomStoreAssociation::DicomStoreAssociation() :
                        m_TLS(false),
                        m_UseUserPass(false),
                        m_Validate(false),
			assoc(NULL),
			net(NULL)
		{
			
		}

		DicomStoreAssociation::~DicomStoreAssociation()
		{
			//no limpiamos no sea que los haya registrado otro que no seamos nosotros!!!
			FreeMemory();
		}

		void DicomStoreAssociation::Store(ListaRutas listaFicheros, const GNC::GCS::Ptr<DicomServer>& server, std::string nombreAETLocal,GNC::IProxyNotificadorProgreso* pNotificador, GIL::DICOM::TipoTransferSyntaxEnvio transferSyntax)
		{
			if (listaFicheros.size() == 0)
				return;

			m_pNotificadorProgreso = pNotificador;


			std::string nombreAETPar = server->AET;
			opt_maxReceivePDULength = server->PDU;
			std::string host = server->HostName;
			unsigned int puerto = server->Port;
			//TIMEOUT 60 SEGUNDOS
			unsigned int dicomtimeout = 60;
			bool _shouldAbort=false;
			DcmFileFormat fileformat;
			errorEncontrado ="";

			E_TransferSyntax transferSyntaxFichero;

			{
				DcmFileFormat dcmff;
				OFCondition cond;
				cond = dcmff.loadFile(listaFicheros.front().c_str(),EXS_Unknown,EGL_noChange,DCM_TagInfoLength);
				//dcmff.loadAllDataIntoMemory();
				if (cond.bad()) {
					errmsg(std::string("no se ha podido leer el dicom"),true);
				} else {
					DcmElement* elemento;
					cond = dcmff.getDataset()->findAndGetElement(DcmTagKey(0x0010,0x0020),elemento);
					if(cond.bad()) {
						std::stringstream strStream;
						strStream << "Es necesario que este establecido el nombre y el ID del paciente";
						errmsg(strStream.str(),true);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
					cond = dcmff.getDataset()->findAndGetElement(DcmTagKey(0x0010,0x0010),elemento);
					if(cond.bad()) {
						std::stringstream strStream;
						strStream << "Es necesario que este establecido el nombre y el ID del paciente";
						errmsg(strStream.str(),true);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
				}
				transferSyntaxFichero = dcmff.getDataset()->getOriginalXfer();
			}


			//se pilla el nombre del estudio y dlpaciente... ahora se comienza
			OFCondition cond;
			const char *opt_peer = NULL;
			OFCmdUnsignedInt opt_port = 104;
			const char *opt_peerTitle = PEERAPPLICATIONTITLE;
			const char *opt_ourTitle = APPLICATIONTITLE;

			if(nombreAETPar.size()>0){
				opt_peerTitle = nombreAETPar.c_str();
			}

			if(nombreAETLocal.size()>0){
				opt_ourTitle = nombreAETLocal.c_str();
			}

			OFList<OFString> fileNameList;       // list of files to transfer to SCP
			OFList<OFString> sopClassUIDList;    // the list of sop classes

			T_ASC_Parameters *params;
			DIC_NODENAME localHost;
			DIC_NODENAME peerHost;
			DcmAssociationConfiguration asccfg; // handler for association configuration profiles

			opt_peer = host.c_str();
			opt_port = puerto;

			//verbose option set to true for now
			opt_showPresentationContexts = OFFalse;

			//debug code off for now
			opt_debug = GNC::GCS::IControladorLog::Instance()->GetActiveLogLevel() == GNC::GCS::IControladorLog::DebugLog;

			switch (transferSyntax) {
				case SendExplicitLittleEndian:
					opt_networkTransferSyntax = EXS_LittleEndianExplicit;
					break;
				case SendJPEG2000Lossless:
					opt_networkTransferSyntax = EXS_JPEG2000LosslessOnly;
					opt_Quality = 0;
					break;
				case SendJPEG2000Lossy10:
					opt_networkTransferSyntax = EXS_JPEG2000;
					opt_Quality = 1;
					break;
				case SendJPEG2000Lossy20:
					opt_networkTransferSyntax = EXS_JPEG2000;
					opt_Quality = 2;
					break;
				case SendJPEG2000Lossy50:
					opt_networkTransferSyntax = EXS_JPEG2000;
					opt_Quality = 3;
					break;
				case SendJPEGLossless:
					opt_networkTransferSyntax = EXS_JPEGProcess14SV1;
					break;
				case SendJPEGLossy9:
					opt_networkTransferSyntax = EXS_JPEGProcess2_4;
					opt_Quality = 90;
					break;
				case SendJPEGLossy8:
					opt_networkTransferSyntax = EXS_JPEGProcess2_4;
					opt_Quality = 80;
					break;
				case SendJPEGLossy7:
					opt_networkTransferSyntax = EXS_JPEGProcess2_4;
					opt_Quality = 70;
					break;
				case SendImplicitLittleEndian:
					opt_networkTransferSyntax = EXS_LittleEndianImplicit;
					break;
					/*	case SendRLE:
					 opt_networkTransferSyntax = EXS_RLELossless;
					 break;
					 case SendExplicitBigEndian:
					 opt_networkTransferSyntax = EXS_BigEndianExplicit;
					 break;
					 case SendBZip:
					 opt_networkTransferSyntax = EXS_DeflatedLittleEndianExplicit;
					 break;*/
				case SendDefault:
					opt_networkTransferSyntax = transferSyntaxFichero;
					//FIXME esta ñapa es necesaria porque el pacs de IRE no acepta baseline
					if (GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","force_lossless")) {
						if (opt_networkTransferSyntax == EXS_JPEGProcess1)
						{
							opt_networkTransferSyntax = EXS_JPEGProcess14SV1;
						}
					}
					//
					break;
			}

			opt_proposeOnlyRequiredPresentationContexts = OFFalse;

			dcmEnableUnknownVRGeneration.set(OFTrue);
			dcmEnableUnlimitedTextVRGeneration.set(OFTrue);

			//	enable-new-vr
			dcmEnableUnknownVRGeneration.set(OFTrue);
			dcmEnableUnlimitedTextVRGeneration.set(OFTrue);

			//Timeout
			OFCmdSignedInt opt_timeout = dicomtimeout;
			dcmConnectionTimeout.set((Sint32) opt_timeout);

			//acse-timeout
			opt_acse_timeout = OFstatic_cast(int, opt_timeout);

			//dimse-timeout
			opt_dimse_timeout = OFstatic_cast(int, opt_timeout);
			opt_blockMode = DIMSE_NONBLOCKING;


			int paramCount = listaFicheros.size();
			const char *currentFilename = NULL;
			char sopClassUID[128];
			char sopInstanceUID[128];
			OFBool ignoreName;

			ListaRutas::iterator it = listaFicheros.begin();

			for (int i=0; i < paramCount; i++, ++it)
			{
				ignoreName = OFFalse;
				currentFilename = (*it).c_str();

				if (access(currentFilename, R_OK) < 0)
				{
					std::stringstream strStream;
					strStream << "No se puede acceder al fichero: " << currentFilename;
					errmsg(strStream.str(), opt_haltOnUnsuccessfulStore);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
				else
				{
					if (!DU_findSOPClassAndInstanceInFile(currentFilename, sopClassUID, sopInstanceUID))
					{
						ignoreName = OFTrue;
						std::stringstream strStream;
						strStream << "SOP class (o instance) no establecido en fichero: " << currentFilename;
						errmsg(strStream.str(), opt_haltOnUnsuccessfulStore);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
					else if (!dcmIsaStorageSOPClassUID(sopClassUID))
					{
						ignoreName = OFTrue;
						std::stringstream strStream;
						strStream << "Storage sop class desconocido en fichero: " << currentFilename << ": " << sopClassUID;
						errmsg(strStream.str(), opt_haltOnUnsuccessfulStore);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
					else
					{
						bool found = false;
						for (OFIterator<OFString> itsc =sopClassUIDList.begin(); itsc != sopClassUIDList.end(); ++itsc) {
							if ((*itsc).compare(sopClassUID) == 0) {
								found = true;
								break;
							}
						}
						if (!found) {
							sopClassUIDList.push_back(sopClassUID);
						}
					}
					if (!ignoreName) fileNameList.push_back(currentFilename);
				}
			}

			/* make sure data dictionary is loaded */
			if (!dcmDataDict.isDictionaryLoaded()) {
				LOG_WARN(LOGGER, "Warning: no data dictionary loaded, check environment variable");
			}

			/* initialize network, i.e. create an instance of T_ASC_Network*. */

			cond = ASC_initializeNetwork(NET_REQUESTOR, puerto, opt_acse_timeout, &net);
			if (cond.bad())
			{
				DimseCondition::dump(cond);
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) ASC_initializeNetwork; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				LOG_ERROR("C-STORE", strStream.str().c_str());
				errmsg(strStream.str(),true);
			}

			GTLSTransportLayer *tLayer = NULL;


			if (IsSecure())
			{
				tLayer = new GTLSTransportLayer(DICOM_APPLICATION_REQUESTOR, NULL);
				if (tLayer == NULL)
				{
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "Unable to create TLS transport layer";
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
				tLayer->setCertificateFromString(GetCliCert());
				tLayer->setPrivateKeyFromString(GetCliKey());

				if (! tLayer->checkPrivateKeyMatchesCertificate())
				{
					std::stringstream strStream;
					strStream << "private key and certificate do not match";
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}

				tLayer->addSystemTrustedCertificates();

				if (GetValidate()) {
					tLayer->setCertificateVerification(DCV_requireCertificate);
				}
				else {
					tLayer->setCertificateVerification(DCV_ignoreCertificate);
				}
				/*
				 if (opt_dhparam && ! (tLayer->setTempDHParameters(opt_dhparam)))
				 {
				 LOG_WARN(assoc->ambitolog, "unable to load temporary DH parameters. Ignoring");
				 }
				 */

				cond = ASC_setTransportLayer(net, tLayer, 0);
				if (cond.bad())
				{
					LOG_ERROR("C-STORE", "Error al insertar capa de transporte segura: " << cond.text());
				}
			}

			/* initialize asscociation parameters, i.e. create an instance of T_ASC_Parameters*. */
			cond = ASC_createAssociationParameters(&params, opt_maxReceivePDULength);
			DimseCondition::dump(cond);
			if (cond.bad())
			{
				DimseCondition::dump(cond);
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) ASC_createAssociationParameters; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			}

			if (UseUserPass()) {
				cond = ASC_setIdentRQUserPassword(params, GetUser().c_str(), GetPass().c_str());
				DimseCondition::dump(cond);
			}
			
			if (cond.bad()) {
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) ASC_createAssociationParameters; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			 }

			/* sets this application's title and the called application's title in the params */
			/* structure. The default values to be set here are "STORESCU" and "ANY-SCP". */
			ASC_setAPTitles(params, opt_ourTitle, opt_peerTitle, NULL);

			/* Set the transport layer type (type of network connection) in the params */
			/* strucutre. The default is an insecure connection; where OpenSSL is  */
			/* available the user is able to request an encrypted,secure connection. */
			cond = ASC_setTransportLayerType(params, IsSecure());
			if (cond.bad()) {
				DimseCondition::dump(cond);
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) ASC_setTransportLayerType; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			}

			/* Figure out the presentation addresses and copy the */
			/* corresponding values into the association parameters.*/
			gethostname(localHost, sizeof(localHost) - 1);
			sprintf(peerHost, "%s:%d", opt_peer, (int)opt_port);
			//NSLog(@"peer host: %s", peerHost);
			cond = ASC_setPresentationAddresses(params, localHost, peerHost);
			if (cond.bad()) {
				DimseCondition::dump(cond);
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) ASC_setPresentationAddresses; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			}

			/* Set the presentation contexts which will be negotiated */
			/* when the network connection will be established */
			cond = addStoragePresentationContexts(params, sopClassUIDList);
			if (cond.bad()) {
				DimseCondition::dump(cond);
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) addStoragePresentationContexts; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			}


			/* dump presentation contexts if required */
			if(opt_debug){
				std::stringstream strStream;
				strStream << "Request Parameters:\n";
				ASC_dumpParameters(params, strStream);
				LOG_DEBUG(LOGGER,strStream.str());
			}

			/* create association, i.e. try to establish a network connection to another */
			/* DICOM application. This call creates an instance of T_ASC_Association*. */
			LOG_DEBUG(LOGGER,"Requesting Association");
			cond = ASC_requestAssociation(net, params, &assoc);
			if (cond.bad()) {
				if (cond == DUL_ASSOCIATIONREJECTED) {
					T_ASC_RejectParameters rej;
					ASC_getRejectParameters(params, &rej);
					ASC_printRejectParameters(stderr, &rej);
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) ASC_requestAssociation[DUL_ASSOCIATIONREJECTED]; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				} else {
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) ASC_requestAssociation[Unknown]; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
			}

			/* dump the connection parameters if in debug mode*/
			if(opt_debug)
			{
				std::stringstream strStream;
				strStream << "Connection Parameters:\n";
				ASC_dumpConnectionParameters(assoc, strStream);
				LOG_DEBUG(LOGGER,strStream.str());
			}

			/* dump the presentation contexts which have been accepted/refused */
			if(opt_debug){
				std::stringstream strStream;
				strStream << "Association Parameters Negotiated:\n";
				ASC_dumpParameters(params, strStream);
				LOG_DEBUG(LOGGER,strStream.str());
			}

			/* count the presentation contexts which have been accepted by the SCP */
			/* If there are none, finish the execution */
			if (ASC_countAcceptedPresentationContexts(params) == 0) {
				std::stringstream strStream;
				strStream << "DICOM Network Failure (storescu) Contextos de presentacion no aceptados";
				errmsg(strStream.str(),true);
				LOG_WARN("C-STORE", strStream.str().c_str());
			}

			/* dump general information concerning the establishment of the network connection if required */
			if (opt_debug) {
				std::stringstream strStream;
				strStream << "Association Accepted (Max Send PDV: "<< assoc->sendPDVLength;
				LOG_DEBUG(LOGGER,strStream.str());
			}

			/* do the real work, i.e. for all files which were specified in the */
			/* command line, transmit the encapsulated DICOM objects to the SCP. */
			cond = EC_Normal;
			OFListIterator(OFString) iter = fileNameList.begin();
			OFListIterator(OFString) enditer = fileNameList.end();

			int _numberSent=0;
			//int _numberOfFiles =0;
			//int _numberErrors =0;
			OFString mensaje;

			ResetearMedida(true);
			while ((iter != enditer) && (cond == EC_Normal) && !_shouldAbort) // compare with EC_Normal since DUL_PEERREQUESTEDRELEASE is also good()
			{
				wxString msg = wxString::Format(_("Uploading file %d of %d"), (int)(_numberSent+1), (int)(listaFicheros.size()) );
				m_mensaje = std::string(msg.ToUTF8());
				pNotificador->NotificarProgreso(0.0f, m_mensaje);

				cond = cstore(assoc, *iter);
				++iter;
				if (!unsuccessfulStoreEncountered)
					_numberSent++;
				else {
					//error cerramos las conexiones...
					break;
				}

				//TODO CONFIGURACION MAXIMO NUMERO DFICHEROS Q SEPUEDE SUBIR, MAXIMO NUMERO DE ERRORES...
			}


			/* tear down association, i.e. terminate network connection to SCP */
			if (cond == EC_Normal)
			{
				if (opt_abortAssociation)
				{
					cond = ASC_abortAssociation(assoc);
					if (cond.bad())
					{
						LOG_DEBUG(LOGGER,"Abortando Asociacion: \n");
						DimseCondition::dump(cond);
						std::stringstream strStream;
						strStream << "DICOM Network Failure (storescu) Association Abort Failed; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
						errmsg(strStream.str(),true);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
				} else
				{
					/* release association */
					LOG_DEBUG(LOGGER,"Abortando Asociacion: \n");
					cond = ASC_releaseAssociation(assoc);
					if (cond.bad())
					{
						ASC_dropAssociation(assoc);
						DimseCondition::dump(cond);
						std::stringstream strStream;
						strStream << "DICOM Network Failure (storescu) Association Abort Failed; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
						errmsg(strStream.str(),true);
						LOG_ERROR("C-STORE", strStream.str().c_str());
					}
				}
			}
			else if (cond == DUL_PEERREQUESTEDRELEASE)
			{
				LOG_DEBUG(LOGGER,"Abortando Asociacion [PEERREQUESTEDRELEASE]");

				cond = ASC_abortAssociation(assoc);
				if (cond.bad())
				{
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) Association Abort Failed; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
			}
			else if (cond == DUL_PEERABORTEDASSOCIATION)
			{
				std::stringstream strStream;
				strStream << "Fallo en la red DICOM (storescu) El servidor ha cerrado la conexion ";
				errmsg(strStream.str(),true);
				LOG_ERROR("C-STORE", strStream.str().c_str());
			}
			else
			{
				DimseCondition::dump(cond);
				LOG_DEBUG(LOGGER,"Abortando Asociacion");

				cond = ASC_abortAssociation(assoc);
				if (cond.bad())
				{
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) Protocol Error: SCU Failed; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
			}

			if ( unsuccessfulStoreEncountered)
			{
				throw GIL::DICOM::PACSException(errorEncontrado);
			}
		}

		void DicomStoreAssociation::FreeMemory()
		{
			/* destroy the association, i.e. free memory of T_ASC_Association* structure. This */
			/* call is the counterpart of ASC_requestAssociation(...) which was called above. */
			if (assoc != NULL) {
				OFCondition cond = ASC_destroyAssociation(&assoc);
				if (cond.bad())
				{
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) Protocol Error: ASC_destroyAssociation; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
			}
			/* drop the network, i.e. free memory of T_ASC_Network* structure. This call */
			/* is the counterpart of ASC_initializeNetwork(...) which was called above. */

			if (net!= NULL) {
				OFCondition cond = ASC_dropNetwork(&net);
				if (cond.bad())
				{
					DimseCondition::dump(cond);
					std::stringstream strStream;
					strStream << "DICOM Network Failure (storescu) Protocol Error: ASC_dropNetwork; Modulo: "<< cond.module() << "; Code:"<< cond.code() << "; " << cond.text();
					errmsg(strStream.str(),true);
					LOG_ERROR("C-STORE", strStream.str().c_str());
				}
			}
		}
	}
}

void GIL::DICOM::DicomStoreAssociation::progressCallback(void * callbackData,
													T_DIMSE_StoreProgress *progress,
													T_DIMSE_C_StoreRQ * /*req*/)
{
	_StoreCallbackInfo *cbdata = static_cast<_StoreCallbackInfo*>(callbackData);

	if (progress->state == DIMSE_StoreProgressing) {
		//si ha pasado medio segundo...
		std::stringstream ostr;
		ostr << m_mensaje;
		ostr.setf(std::ios::floatfield, std::ios::fixed );
		ostr.precision(2);
		ostr << " a " << TasaTransferencia(progress->progressBytes) << " kb/s";
		std::string msg(ostr.str());

		if(!m_pNotificadorProgreso->NotificarProgreso((float)progress->progressBytes/progress->totalBytes,msg))
		{
			ASC_abortAssociation(cbdata->assoc);
		}
	}
	if (progress->state == DIMSE_StoreEnd) {
		ResetearMedida();
	}
}

