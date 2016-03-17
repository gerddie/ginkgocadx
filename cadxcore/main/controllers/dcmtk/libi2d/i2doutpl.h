/*
*
*  $Id: i2doutpl.h $
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
*  Purpose: Base class for converter from image file to DICOM
*
*  Last Update:      $Author: onken $
*  Update Date:      $Date: 2009-01-16 09:51:55 $
*  CVS/RCS Revision: $Revision: 1.4 $
*  Status:           $State: Exp $
*
*  CVS/RCS Log at end of file
*
*/

#ifndef I2DOUTPL_H
#define I2DOUTPL_H

#include "d2dcommon.h"

class I2DOutputPlug: public D2DCommon
{

public:

        /** Constructor, initializes member variables
        *  @return none
        */
        I2DOutputPlug() : D2DCommon(), m_doAttribChecking(OFTrue)
        {};

        /** Virtual function that returns a short name of the plugin.
        *  @return The name of the plugin
        */
        virtual OFString ident() =0;

        /** Virtual function that returns the Storage SOP class UID, the plugin writes.
        *  @param suppSOPs - [out] List containing supported output SOP classes
        *  @return String containing the Storage SOP class UID
        */
        virtual void supportedSOPClassUIDs(OFList<OFString> suppSOPs) =0;

        /** Outputs SOP class specific information into dataset
        * @param dataset - [in/out] Dataset to write to
        * @return EC_Normal if successful, error otherwise
        */
        virtual OFCondition convert(DcmDataset &dataset) const = 0;

        /** Do some completeness / validity checks. Should be called when
        *  dataset is completed and is about to be saved.
        *  @param dataset - [in] The dataset to check
        *  @return Error string if error occurs, empty string otherwise
        */
        virtual OFString isValid(DcmDataset& dataset) const = 0;

        /** Destructor
        *  @return none
        */
        virtual ~I2DOutputPlug() {};


        /** Enable/Disable basic validity checks for output dataset
        *  @param doChecks - [in] OFTrue enables checking, OFFalse turns it off.
        *  @param insertMissingType2 - [in] If true (default), missing type 2
        *         attributes are inserted automatically
        *  @param inventMissingType1 - [in] If true (default), missing type 1
        *         attributes are inserted automatically with a predefined
        *         value (if possible). An existing empty type 1 attribute is
        *         assigned a value, too.
        *  @return none
        */
        virtual void setValidityChecking(OFBool doChecks,
                                         OFBool insertMissingType2 = OFTrue,
                                         OFBool inventMissingType1 = OFTrue)
        {
                m_doAttribChecking = doChecks;
                m_inventMissingType2Attribs = insertMissingType2;
                m_inventMissingType1Attribs = inventMissingType1;
        };

protected:

        /// if enabled, some simple attribute checking is performed
        /// default: enabled (OFTrue)
        OFBool m_doAttribChecking;
};

#endif // #ifndef I2DOUTPL_H

/*
* CVS/RCS Log:
* $Log: i2doutpl.h,v $
* Revision 1.4  2009-01-16 09:51:55  onken
* Completed doxygen documentation for libi2d.
*
* Revision 1.3  2008-01-16 16:32:23  onken
* Fixed some empty or doubled log messages in libi2d files.
*
* Revision 1.2  2008-01-16 15:10:20  onken
* Moved library "i2dlib" from /dcmdata/libsrc/i2dlib to /dcmdata/libi2d
*
* Revision 1.2  2008-01-11 14:17:53  onken
* Added various options to i2dlib. Changed logging to use a configurable
* logstream. Added output plugin for the new Multiframe Secondary Capture SOP
* Classes. Added mode for JPEG plugin to copy exsiting APPn markers (except
* JFIF). Changed img2dcm default behaviour to invent type1/type2 attributes (no
* need for templates any more). Added some bug fixes.
*
* Revision 1.1  2007/11/08 15:58:55  onken
* Initial checkin of img2dcm application and corresponding library i2dlib.
*
*
*/

