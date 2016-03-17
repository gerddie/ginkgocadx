/*
*
*  $Id: foooutpl.h $
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

#pragma once

#include "i2doutpl.h"

class FooOutputPlug: public I2DOutputPlug
{

public:

        /** Constructor, initializes member variables
        *  @return none
        */
        FooOutputPlug() : I2DOutputPlug() {};

        /** Virtual function that returns a short name of the plugin.
        *  @return The name of the plugin
        */
        virtual OFString ident()
        {
                return "Foo";
        }

        /** Virtual function that returns the Storage SOP class UID, the plugin writes.
        *  @param suppSOPs - [out] List containing supported output SOP classes
        *  @return String containing the Storage SOP class UID
        */
        virtual void supportedSOPClassUIDs(OFList<OFString> /*suppSOPs*/)
        {
        }

        /** Outputs SOP class specific information into dataset
        * @param dataset - [in/out] Dataset to write to
        * @return EC_Normal if successful, error otherwise
        */
        virtual OFCondition convert(DcmDataset &/*dataset*/) const
        {
                OFCondition cond;
                return cond;
        }

        /** Do some completeness / validity checks. Should be called when
        *  dataset is completed and is about to be saved.
        *  @param dataset - [in] The dataset to check
        *  @return Error string if error occurs, empty string otherwise
        */
        virtual OFString isValid(DcmDataset& /*dataset*/) const
        {
                return "";
        }

        /** Destructor
        *  @return none
        */
        virtual ~FooOutputPlug() {};
protected:
};
