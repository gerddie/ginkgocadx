/*
 *  
 *  $Id: pdf2dsource.h $
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
 *  Purpose: Base Class for plugins extracting pixel data from standard
 *           image files
 *
 *  Last Update:      $Author: onken $
 *  Update Date:      $Date: 2009-01-16 09:51:55 $
 *  CVS/RCS Revision: $Revision: 1.2 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */

#pragma once

#include <dcmtk/config/osconfig.h>
#include <dcmtk/dcmdata/dctk.h>
#include "d2dsource.h"

class PDF2DSource: public D2DSource
{

public:

  /** Constructor
   */
	PDF2DSource() : D2DSource()
	{
		m_isTemp=false;
	}

	virtual void setPDFFile(const OFString& file) {m_file = file;}

  /** Returns identifier for the image input format the plugin imports.
   *  @return A short identifier (e. g. "JPEG")
   */
  virtual OFString inputFormat() const;

  /** Reads pixel data and corresponding attributes like rows etc. from image
	 *  file and inserts them into dataset.
	 *  @param dset - [out] The dataset to export the pixel data attributes to
	 *  @param outputTS - [out] The proposed transfex syntax of the dataset
	 *  @return EC_Normal, if successful, error otherwise
	 */
  virtual OFCondition readAndInsertSpecificTags( DcmDataset* dset,
									   E_TransferSyntax& outputTS);

  /** Do some completeness / validity checks. Should be called when
	 *  dataset is completed and is about to be saved.
	 *  @param dataset - [in] The dataset to check
	 *  @return Error string if error occurs, empty string otherwise
	 */
	OFString isValid(DcmDataset& dset) const;

 
  /** Virtual Destructor
   */
	virtual ~PDF2DSource();

protected:
	OFString m_file;
	bool m_isTemp;
};

/*
 * CVS/RCS Log:
 * $Log: i2dimgs.h,v $
 * Revision 1.2  2009-01-16 09:51:55  onken
 * Completed doxygen documentation for libi2d.
 *
 * Revision 1.1  2008-01-16 15:08:38  onken
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

