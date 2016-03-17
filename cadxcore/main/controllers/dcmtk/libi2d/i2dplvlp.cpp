/*
 *
 *  $Id: i2dplvlp.cpp $
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
 *  Purpose: Implements conversion from image into DICOM Visible Light Photography IOD
 *
 *  Last Update:      $Author: onken $
 *  Update Date:      $Date: 2008-01-16 16:32:31 $
 *  CVS/RCS Revision: $Revision: 1.2 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

#include <dcmtk/config/osconfig.h>
#include "i2dplvlp.h"
#include <api/controllers/icontroladorpermisos.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

I2DOutputPlugVLP::I2DOutputPlugVLP()
{
        if (m_debug)
                printMessage(m_logStream, "I2DOutputPlugVLP: Output plugin for VLP initialized");
}

OFString I2DOutputPlugVLP::ident()
{
        return "Visible Light Photographic Image SOP Class";
}

void I2DOutputPlugVLP::supportedSOPClassUIDs(OFList<OFString> suppSOPs)
{
        OFString str = GNC::GCS::IControladorPermisos::Instance()->Get("core.importacion", "storage_sop_class").ObtenerValor<std::string>().c_str();
        suppSOPs.push_back(str);
}


OFCondition I2DOutputPlugVLP::convert(DcmDataset &dataset) const
{
        if (m_debug) {
                printMessage(m_logStream, "I2DOutputPlugVLP: Inserting VLP specific attributes");
        }
        OFCondition cond;
        OFString str = GNC::GCS::IControladorPermisos::Instance()->Get("core.importacion", "storage_sop_class").ObtenerValor<std::string>().c_str();
        cond = dataset.putAndInsertOFStringArray(DCM_SOPClassUID, str);
        if (cond.bad())
                return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable to insert SOP class into dataset");

        if (!dataset.tagExists(DCM_Modality)) {
                cond = dataset.putAndInsertOFStringArray(DCM_Modality, "XC");
                if (cond.bad())
                        return makeOFCondition(OFM_dcmdata, 18, OF_error, "Unable to insert Modality (XC) into dataset");
        }

        return EC_Normal;
}


OFString I2DOutputPlugVLP::isValid(DcmDataset& dataset) const
{
        OFString err;
        // Just return if checking was disabled
        if (!m_doAttribChecking)
                return err;

        if (m_debug)
                printMessage(m_logStream, "I2DOutputPlugVLP: Checking VLP specific attributes for validity");
        // Acquisition Context Module
        checkAndInventType2Attrib(DCM_AcquisitionContextSequence, &dataset);

        // General Equipment Module
        checkAndInventType2Attrib(DCM_Manufacturer, &dataset);

        // VL Image Module
        checkAndInventType1Attrib(DCM_ImageType, &dataset, "DERIVED\\SECONDARY");
        checkAndInventType2Attrib(DCM_LossyImageCompression, &dataset);

        return err;
}


I2DOutputPlugVLP::~I2DOutputPlugVLP()
{
}


/*
 * CVS/RCS Log:
 * $Log: i2dplvlp.cc,v $
 * Revision 1.2  2008-01-16 16:32:31  onken
 * Fixed some empty or doubled log messages in libi2d files.
 *
 * Revision 1.1  2008-01-16 14:42:00  onken
 * Moved library "i2dlib" from /dcmdata/libsrc/i2dlib to /dcmdata/libi2d
 *
 * Revision 1.1  2007/11/08 15:55:17  onken
 * Initial checkin of img2dcm application and corresponding library i2dlib.
 *
 *
 */
