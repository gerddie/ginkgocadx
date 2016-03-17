/*
*
*  $Id: document2dcm.cpp $
*  Ginkgo CADx Project
*
*  Code addapted from DCMTK
*
*
*  Copyright (C) 2001-2007, OFFIS
*
*  This software and supporting documentation were developed by
*
*    Kuratorium OFFIS e.V.
*    Healthcare Information and Communication Systems
*    Escherweg 2
*    D-26121 Oldenburg, Germany
*
*  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
*  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
*  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
*  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
*  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
*
*  Module:  dcmdata
*
*  Author:  Michael Onken
*
*  Purpose: Implements utility for converting standard image formats to DICOM
*
*  Last Update:      $Author: onken $
*  Update Date:      $Date: 2008-01-16 16:32:31 $
*  CVS/RCS Revision: $Revision: 1.2 $
*  Status:           $State: Exp $
*
*  CVS/RCS Log at end of file
*
*/

#include <sstream>
#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

#include <main/controllers/dcmtk/ginkgouid.h>
#include <dcmtk/config/osconfig.h>
#include "document2dcm.h"
#include <dcmtk/dcmdata/dcpxitem.h>

#include "dcmtk/dcmdata/dctk.h"
//#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/dcmdata/dcuid.h"     /* for dcmtk version name */
#include "dcmtk/dcmjpeg/djencode.h"  /* for dcmjpeg encoders */
#include "dcmtk/dcmjpeg/djrplol.h"   /* for DJ_RPLossless */
#include "dcmtk/dcmjpeg/djrploss.h"   /* for DJ_RPLossy */
#include "dcmtk/dcmjpeg/dipijpeg.h"  /* for dcmimage JPEG plugin */
#include "dcmtk/dcmimage/diregist.h"  /* include to support color images */

#include <main/controllers/controladorlog.h>

#include <ctime>
#include <cstdlib>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

Document2Dcm::Document2Dcm() : D2DCommon(), m_overrideKeys(NULL), m_templateFile(""),
        m_readStudyLevel(OFFalse), m_readSeriesLevel(OFFalse), m_studySeriesFile(),
        m_incInstNoFromFile(OFFalse), m_disableAttribChecks(OFFalse),
        m_insertLatin1(OFFalse), m_insertUTF8(OFTrue)
{
        ;
}

OFCondition Document2Dcm::convert(D2DSource *inputPlug,
                                  I2DOutputPlug *outPlug,
                                  DcmDataset*& resultDset,
                                  E_TransferSyntax& proposedTS)

{
        if (!inputPlug || !outPlug)
                return EC_IllegalParameter;

        OFCondition cond;
        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Starting conversion");

        // If specified, copy DICOM template file to export file
        if (m_templateFile.length() != 0) {
                DcmFileFormat dcmff;
                cond = dcmff.loadFile(m_templateFile.c_str());
                if (cond.bad())
                        return cond;
                // remove problematic attributes from dataset
                cleanupTemplate(dcmff.getDataset());
                // copy from input file
                resultDset = new DcmDataset(*(dcmff.getDataset()));
        } else // otherwise, start with an empty DICOM file
                resultDset = new DcmDataset();
        if (!resultDset)
                return EC_MemoryExhausted;

        // Read patient and study or series information if desired and write to export file
        if (m_readStudyLevel || m_readSeriesLevel) {
                cond = applyStudyOrSeriesFromFile(resultDset);
                if (cond.bad()) {
                        delete resultDset;
                        resultDset = NULL;
                        return cond;
                }
        }

        OFString value;

        // Increment instance number
        if (m_incInstNoFromFile) {
                cond = incrementInstanceNumber(resultDset);
                if (cond.bad()) {
                        delete resultDset;
                        resultDset = NULL;
                        return cond;
                }
                OFString s;
                if (cond.good()) {
                        cond = DcmDate::getCurrentDate(value);
                }
                if (cond.good()) {
                        cond = resultDset->putAndInsertOFStringArray(DCM_InstanceCreationDate, value);
                }
                if (cond.good()) {
                        cond = DcmTime::getCurrentTime(value);
                }
                if (cond.good()) {
                        cond = resultDset->putAndInsertOFStringArray(DCM_InstanceCreationTime, value);
                }
                if (cond.bad()) {
                        delete resultDset;
                        resultDset = NULL;
                        return cond;
                }
        } else {
                cond = resultDset->findAndGetOFString(DCM_InstanceCreationDate, value);
                if (cond.bad()) {
                        cond = DcmDate::getCurrentDate(value);
                        if (cond.good()) {
                                cond = resultDset->putAndInsertOFStringArray(DCM_InstanceCreationDate, value);
                        }
                }
                if (cond.bad()) {
                        delete resultDset;
                        resultDset = NULL;
                        return cond;
                }

                cond = resultDset->findAndGetOFString(DCM_InstanceCreationTime, value);

                if (cond.bad()) {
                        cond = DcmTime::getCurrentTime(value);
                        if (cond.good()) {
                                cond = resultDset->putAndInsertOFStringArray(DCM_InstanceCreationTime, value);
                        }
                }
                if (cond.bad()) {
                        delete resultDset;
                        resultDset = NULL;
                        return cond;
                }
        }

        // Insert Latin 1 as standard character set if desired
        if (m_insertLatin1)
                cond = insertLatin1(resultDset);
        if (cond.bad())
                return cond;

        if (m_insertUTF8)
                cond = insertUTF8(resultDset);
        if (cond.bad())
                return cond;

        // Generate and insert UIDs as necessary
        generateUIDs(resultDset);

        // Read and insert pixel data

        cond = inputPlug->readAndInsertSpecificTags(resultDset, proposedTS);
        if (cond.bad()) {
                delete resultDset;
                resultDset = NULL;
                return cond;
        }

        // Insert SOP Class specific attributes (and values)
        cond = outPlug->convert(*resultDset);
        if (cond.bad()) {
                delete resultDset;
                resultDset = NULL;
                return cond;
        }

        // At last, apply override keys on dataset
        applyOverrideKeys(resultDset);

        // Do some very basic attribute checking (e. g. existence (type 2) and values (type 1))
        if (!m_disableAttribChecks) {
                OFString err;
                err = isValid(*resultDset);
                err += inputPlug->isValid(*resultDset);
                err += outPlug->isValid(*resultDset);
                if (!err.empty()) {
                        delete resultDset;
                        resultDset = NULL;
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, err.c_str());
                }
        }

        return EC_Normal;
}

OFCondition Document2Dcm::updateOverrideKeys(DcmDataset* dset)
{
        if (dset == NULL) {
                return EC_IllegalParameter;
        }

        applyOverrideKeys(dset);

        return EC_Normal;
}

OFCondition Document2Dcm::insertLatin1(DcmDataset *outputDset)
{
        if (outputDset == NULL)
                return EC_IllegalParameter;
        return outputDset->putAndInsertOFStringArray(DCM_SpecificCharacterSet, "ISO_IR 100");
}

OFCondition Document2Dcm::insertUTF8(DcmDataset *outputDset)
{
        if (outputDset == NULL)
                return EC_IllegalParameter;
        return outputDset->putAndInsertOFStringArray(DCM_SpecificCharacterSet, "ISO_IR 192");
}

void Document2Dcm::cleanupTemplate(DcmDataset *targetDset)
{
        if (!targetDset)
                return;
        // Remove any existing image pixel module attribute
        targetDset->findAndDeleteElement(DcmTagKey(0x0028, 0x7FE0)); // Pixel Data Provider URL (JPIP)
        targetDset->findAndDeleteElement(DCM_PhotometricInterpretation);
        targetDset->findAndDeleteElement(DCM_SamplesPerPixel);
        targetDset->findAndDeleteElement(DCM_Rows);
        targetDset->findAndDeleteElement(DCM_Columns);
        targetDset->findAndDeleteElement(DCM_BitsAllocated);
        targetDset->findAndDeleteElement(DCM_BitsStored);
        targetDset->findAndDeleteElement(DCM_HighBit);
        targetDset->findAndDeleteElement(DCM_PixelRepresentation);
        targetDset->findAndDeleteElement(DCM_PixelData);
        targetDset->findAndDeleteElement(DCM_PlanarConfiguration);
        targetDset->findAndDeleteElement(DCM_PixelAspectRatio);
        targetDset->findAndDeleteElement(DCM_SmallestImagePixelValue);
        targetDset->findAndDeleteElement(DCM_LargestImagePixelValue);
        targetDset->findAndDeleteElement(DCM_RedPaletteColorLookupTableDescriptor);
        targetDset->findAndDeleteElement(DCM_GreenPaletteColorLookupTableDescriptor);
        targetDset->findAndDeleteElement(DCM_BluePaletteColorLookupTableDescriptor);
        targetDset->findAndDeleteElement(DCM_RedPaletteColorLookupTableData);
        targetDset->findAndDeleteElement(DCM_GreenPaletteColorLookupTableData);
        targetDset->findAndDeleteElement(DCM_BluePaletteColorLookupTableData);
        targetDset->findAndDeleteElement(DCM_ICCProfile);
        // Remove SOP Class / Instance information
        targetDset->findAndDeleteElement(DCM_SOPClassUID);
        targetDset->findAndDeleteElement(DCM_SOPInstanceUID);

}

OFCondition Document2Dcm::applyStudyOrSeriesFromFile(DcmDataset *targetDset)
{
        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Applying study and/or series information from file");
        if ((!m_readSeriesLevel && !m_readStudyLevel) || (m_studySeriesFile.length() == 0))
                return EC_IllegalCall;
        DcmFileFormat dcmff;
        OFString errMsg;
        OFCondition cond;

        // Open DICOM file to read patient/study/series information from
        cond = dcmff.loadFile(m_studySeriesFile.c_str());
        if (cond.bad()) {
                errMsg = "Error: Unable to open study / series file ";
                errMsg += m_studySeriesFile;
                return makeOFCondition(OFM_dcmdata, 18, OF_error, errMsg.c_str());
        }

        DcmDataset *srcDset = NULL;
        srcDset = dcmff.getDataset();
        if (srcDset == NULL)
                return EC_IllegalCall;

        // Patient level attributes (type 2 - if value cannot be read, insert empty value
        OFString value;
        srcDset->findAndGetOFString(DCM_PatientName, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_PatientName, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Patients Name to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_PatientID, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_PatientID, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Patient ID to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_PatientSex, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_PatientSex, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Patient's Sex to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_PatientBirthDate, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_PatientBirthDate, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Patient's Birth Date to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_SpecificCharacterSet, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_SpecificCharacterSet, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Specific Character Set to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_InstanceCreationDate, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_InstanceCreationDate, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Instance Creation Date to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_InstanceCreationTime, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_InstanceCreationTime, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Instance Creation Time to file");
        value.clear();

        // Study level attributes (type 2 except Study Instance UID)

        cond = srcDset->findAndGetOFString(DCM_StudyInstanceUID, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable to read Study Instance UID (type 1) from file");
        cond = targetDset->putAndInsertOFStringArray(DCM_StudyInstanceUID, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Study Instance UID to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_StudyDate, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_StudyDate, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Study Date to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_StudyTime, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_StudyTime, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Study Time to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_ReferringPhysicianName, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_ReferringPhysicianName, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Referring Physician's Name to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_StudyID, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_StudyID, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Study ID to file");
        value.clear();

        srcDset->findAndGetOFString(DCM_AccessionNumber, value);
        cond = targetDset->putAndInsertOFStringArray(DCM_AccessionNumber, value);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Accession Number to file");
        value.clear();

        // Series Level attributes (type 2 except Series Instance UID which is type 1)
        if (m_readSeriesLevel) {
                // General Series Module attributes
                cond = srcDset->findAndGetOFString(DCM_SeriesInstanceUID, value);
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable to read Series Instance UID (type 1) from file");
                cond = targetDset->putAndInsertOFStringArray(DCM_SeriesInstanceUID, value);
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Series Instance UID to file");
                value.clear();

                srcDset->findAndGetOFString(DCM_SeriesNumber, value);
                cond = targetDset->putAndInsertOFStringArray(DCM_SeriesNumber, value);
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Series Number to file");
                value.clear();

                // General Equipment Module attributes
                srcDset->findAndGetOFString(DCM_Manufacturer, value);
                cond = targetDset->putAndInsertOFStringArray(DCM_Manufacturer, value);
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Manufacturer to file");
                value.clear();
        }

        return EC_Normal;
}

OFCondition Document2Dcm::incrementInstanceNumber(DcmDataset *targetDset)
{
        // Read and increment Instance Number if desired
        if (m_incInstNoFromFile) {
                if (m_debug)
                        printMessage(m_logStream, "Document2Dcm: Trying to read and increment instance number");
                Sint32 instanceNumber;
                if (targetDset->findAndGetSint32(DCM_InstanceNumber, instanceNumber).good()) {
                        instanceNumber++;
                        std::ostringstream os;
                        os << OFstatic_cast(long, instanceNumber);
                        OFCondition cond = targetDset->putAndInsertOFStringArray(DCM_InstanceNumber, os.str().c_str());
                        if (cond.bad()) {
                                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Instance Number to dataset");
                        }
                } else {
                        OFCondition cond = targetDset->putAndInsertOFStringArray(DCM_InstanceNumber, "1");
                        if (cond.bad()) {
                                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Instance Number to dataset");
                        }
                }
        }
        return EC_Normal;
}

OFCondition Document2Dcm::generateUIDs(DcmDataset *dset)
{
        OFString value;
        OFCondition cond;

        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Generate and insert new UIDs if necessary");
        // Generate and write Series Instance UID if not already present
        if (!m_readSeriesLevel) {
                cond = dset->findAndGetOFString(DCM_SeriesInstanceUID, value);
                if (cond.bad() || (value.length() == 0)) {
                        cond = dset->putAndInsertOFStringArray(DCM_SeriesInstanceUID, GIL::DICOM::MakeUID(GIL::DICOM::GUID_SeriesRoot).c_str());
                        if (cond.bad())
                                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Series Instance UID to file");
                }
                value.clear();
        }

        // Generate and write Study Instance UID if not already present
        if (!m_readStudyLevel) {
                cond = dset->findAndGetOFString(DCM_StudyInstanceUID, value);
                if (cond.bad() || (value.length() == 0)) {
                        cond = dset->putAndInsertOFStringArray(DCM_StudyInstanceUID, GIL::DICOM::MakeUID(GIL::DICOM::GUID_StudyRoot).c_str());
                        if (cond.bad())
                                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Study Instance UID to file");
                }
                value.clear();

                cond = dset->findAndGetOFString(DCM_AccessionNumber, value);
                if (cond.bad() || (value.length() == 0)) {
                        std::stringstream is;

                        int random_integer;
                        const int lowest  = 0;
                        const int highest = 9999;
                        const int range   = (highest-lowest)+1;

                        const time_t timestamp = std::time(NULL);
                        std::srand((unsigned int)timestamp);

                        random_integer = lowest + int(range * rand()/(RAND_MAX + 1.0));

                        is << (unsigned long) timestamp;
                        is.width(4);
                        is.fill('0');
                        is << (unsigned int) random_integer;
                        cond = dset->putAndInsertString(DCM_AccessionNumber, is.str().c_str());
                        if (cond.bad()) {
                                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write Accession Number");
                        }
                }
                value.clear();
        }

        // Generate SOP Instance UID if not already present
        cond = dset->findAndGetOFString(DCM_SOPInstanceUID, value);
        if (cond.bad() || (value.length() == 0)) {
                cond = dset->putAndInsertOFStringArray(DCM_SOPInstanceUID, GIL::DICOM::MakeUID(GIL::DICOM::GUID_InstanceRoot).c_str());
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable write SOP Instance UID to file");
        }

        return EC_Normal;
}

void Document2Dcm::setISOLatin1(OFBool insertLatin1)
{
        m_insertLatin1 = insertLatin1;
}

void Document2Dcm::setUTF8(OFBool insertUTF8)
{
        m_insertUTF8 = insertUTF8;
}


OFString Document2Dcm::isValid(DcmDataset& dset) const
{
        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Checking validity of DICOM output dataset");
        OFString dummy, err;
        OFCondition cond;
        // General Patient module attributes
        err += checkAndInventType2Attrib(DCM_PatientName, &dset);
        err += checkAndInventType2Attrib(DCM_PatientSex, &dset);
        err += checkAndInventType2Attrib(DCM_PatientBirthDate, &dset);
        err += checkAndInventType2Attrib(DCM_PatientID, &dset);

        // General Study module attributes
        err += checkAndInventType1Attrib(DCM_StudyInstanceUID, &dset);
        err += checkAndInventType2Attrib(DCM_StudyDate, &dset);
        err += checkAndInventType2Attrib(DCM_StudyTime, &dset);
        err += checkAndInventType2Attrib(DCM_ReferringPhysicianName, &dset);
        err += checkAndInventType2Attrib(DCM_StudyID, &dset);
        err += checkAndInventType2Attrib(DCM_AccessionNumber, &dset);

        // General Series module attributes
        err += checkAndInventType1Attrib(DCM_SeriesInstanceUID, &dset);
        err += checkAndInventType2Attrib(DCM_SeriesNumber, &dset);
        err += checkAndInventType2Attrib(DCM_InstanceNumber, &dset);

        return err;
}

void Document2Dcm::setSeriesFrom(const OFString& file)
{
        m_readSeriesLevel = OFTrue;
        m_studySeriesFile = file;
}

void Document2Dcm::setStudyFrom(const OFString& file)
{
        m_readStudyLevel = OFTrue;
        m_studySeriesFile = file;
}

void Document2Dcm::setValidityChecking(OFBool doChecks,
                                       OFBool insertMissingType2,
                                       OFBool inventMissingType1)
{
        m_disableAttribChecks = !doChecks;
        m_inventMissingType2Attribs = insertMissingType2;
        m_inventMissingType1Attribs = inventMissingType1;
}

void Document2Dcm::setTemplateFile(const OFString& file)
{
        m_templateFile = file;
}

void Document2Dcm::setIncrementInstanceNumber(OFBool incInstNo)
{
        m_incInstNoFromFile = incInstNo;
}

void Document2Dcm::setOverrideKeys(DcmDataset* dset)
{
        if (m_overrideKeys != NULL) {
                delete m_overrideKeys;
        }
        m_overrideKeys = dset;
}

void Document2Dcm::applyOverrideKeys(DcmDataset *outputDset)

{
        if ((m_overrideKeys == NULL) || (outputDset == NULL)) {
                return; /* nothing to do */
        }
        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Applying override keys");
        /* copy the override keys */
        DcmDataset keys(*m_overrideKeys);

        /* put the override keys into dset replacing existing tags */
        unsigned long elemCount = keys.card();
        OFCondition cond;
        for (unsigned long i = 0; i < elemCount; i++) {
                DcmElement *elem = keys.remove((unsigned long) 0);
                const DcmTag& tag = elem->getTag();
                cond = outputDset->insert(elem, true, false);
                if (cond.bad()) {
                        std::ostringstream os;
                        os << "Error al insertar Tag: " << tag.toString().c_str() << " : " << cond.text();
                        LOG_ERROR("Dicomizacion", os.str());
                } else {
                        //std::cout << "Insertado  " << tag.toString().c_str() << std::endl;
                }
        }
}

Document2Dcm::~Document2Dcm()
{
        if (m_debug)
                printMessage(m_logStream, "Document2Dcm: Freeing memory");
        if (m_overrideKeys != NULL) {
                delete m_overrideKeys;
        }
        m_overrideKeys = NULL;
}


/*
* CVS/RCS Log:
* $Log: i2d.cc,v $
* Revision 1.2  2008-01-16 16:32:31  onken
* Fixed some empty or doubled log messages in libi2d files.
*
* Revision 1.1  2008-01-16 14:36:02  onken
* Moved library "i2dlib" from /dcmdata/libsrc/i2dlib to /dcmdata/libi2d
*
* Revision 1.1  2007/11/08 15:55:17  onken
* Initial checkin of img2dcm application and corresponding library i2dlib.
*
*
*/
