/*
 *  
 *  $Id: d2dsource.h $
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
#include "d2dcommon.h"

class D2DSource: public D2DCommon
{

public:

  /** Constructor
   */
  D2DSource() {}


  /** Returns identifier for the image input format the plugin imports.
   *  @return A short identifier (e. g. "JPEG")
   */
  virtual OFString inputFormat() const = 0;

  /** Reads pixel data and corresponding attributes like rows etc. from image
	 *  file and inserts them into dataset.
	 *  @param imageSource - [in] The input plugin that actually reads the pixel data
	 *  @param dset - [out] The dataset to export the pixel data attributes to
	 *  @param outputTS - [out] The proposed transfex syntax of the dataset
	 *  @return EC_Normal, if successful, error otherwise
	 */
	virtual OFCondition readAndInsertSpecificTags( DcmDataset* dset,
									   E_TransferSyntax& outputTS) = 0;

	
	/** Do some completeness / validity checks. Should be called when
	 *  dataset is completed and is about to be saved.
	 *  @param dataset - [in] The dataset to check
	 *  @return Error string if error occurs, empty string otherwise
	 */
	virtual OFString isValid(DcmDataset& dset) const = 0;

  /** Virtual Destructor
   */
  virtual ~D2DSource() {}

protected:
};

