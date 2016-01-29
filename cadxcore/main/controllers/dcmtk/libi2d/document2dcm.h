/*
*  
*  $Id: document2dcm.h $
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
*
*/
#pragma once

#include "d2dcommon.h"
#include "d2dsource.h"
#include "i2doutpl.h"

class Document2Dcm: public D2DCommon{

public:

	/** Constructor, initializes command line options
	*  @return none
	*/
	Document2Dcm();

	/** Start the conversion. Needs a fully configured input plugin
	*  and a fully configured output plugin to operate. Returns
	*  a dataset with the resulting DICOM object.
	*  @param inputPlug - [in] The input plugin to read pixel data
	*  @param outPlug - [in] The output plugin for specific SOP class output
	*  @param resultDset - [out] The DICOM object resulting from the conversion
	*  @param proposedTS - [out] The proposed transfer syntax (needed e. g.
	*                            by JPEG input plugin)
	*  @return EC_Normal, if successfull, error otherwise
	*/
	OFCondition convert(D2DSource *inputPlug,
		I2DOutputPlug *outPlug,
		DcmDataset*& resultDset,
		E_TransferSyntax& proposedTS);

	OFCondition updateOverrideKeys(DcmDataset* dset);

	/** Sets a DICOM file that should serve as a template for the resulting
	* DICOM object. Only the dataset of the given file is imported.
	* @param file - [in] The filename of the template DICOM file
	* @return none
	*/
	void setTemplateFile(const OFString& file);

	/** Set file from which patient/study/series data should be imported from.
	*  @param file - [in] The DICOM file to read from
	*  @return none
	*/
	void setSeriesFrom(const OFString& file);

	/** Set file from which patient/study/series data should be imported from.
	*  @param file - [in] The DICOM file to read from
	*  @return none
	*/
	void setStudyFrom(const OFString& file);

	/** Enables/Disables incrementing instance number taken over from DICOM file.
	*  This can either be a the study/series file or the template file
	*  provided.
	*  @param incInstNo - [in] If true, the instance number will be incremented
	*  @return none
	*/
	void setIncrementInstanceNumber(OFBool incInstNo);

	/** Enables/disables autotmatic insertion of the value "ISO_IR100" as
	* a value for the Specific Character Set attribute. If disabled,
	* no value is inserted for Specifific Character Set but instead
	* must be provided by other means (see setTemplateFile(), setSeriesFrom(),
	* setStudyFrom(), setOverrideKeys()). The insertion is applied after
	* loading the (optional) template file; the value might be overwritten
	* by the value copied by setStudy/Series/OverrideKeys.
	* @param insertLatin1 - [in] Enable/disable insertion of Iso Latin 1
	* @return none;
	*/
	void setISOLatin1(OFBool insertLatin1);

	void setUTF8(OFBool insertUTF8);

	/** Specifies some attributes that should be inserted after conversion.
	*  They will override any identical attributes already existing in the
	*  converted result DICOM object. The override keys are applied at the very
	*  end of the conversion and do not undergoe any validity checking.
	*  @param dset - [in] The dataset with override attributes
	*  @return none
	*/
	void setOverrideKeys(DcmDataset* dset);

	DcmDataset* getOverrideKeys() {
		if (m_overrideKeys == NULL) {
			m_overrideKeys = new DcmDataset();
		}
		return m_overrideKeys;
	};

	/** Enable/Disable basic validity checks for output dataset
	*  @param doChecks - [in] OFTrue enables checking, OFFalse turns it off.
	*  @param insertMissingType2 - [in] If true (default), missing type 2
	*         attributes are inserted automatically
	*  @param inventMissingType1 - [in] If true , missing type 1
	*         attributes are inserted automatically with a predefined
	*         value (if possible). An existing empty type 1 attribute is
	*         assigned a value, too.
	*  @return none
	*/
	void setValidityChecking(OFBool doChecks,
		OFBool insertMissingType2 = OFTrue,
		OFBool inventMissingType1 = OFTrue);



	/** Destructor, frees plugin memory
	*  @return none
	*/
	~Document2Dcm();

protected:

	/** Cleans up template for future insertion of pixel data information, ie
	* generally removes attributes from image pixel module
	* @param targetDset - [out] The dataset that should be cleand up
	* @return none
	*/
	void cleanupTemplate(DcmDataset *targetDset);

	/** Parses patient/study/series level attributes from file as defined
	*  on command line and fills internal attributes accordingly.
	*  @param targetDset - [out] The dataset to write the attributes to
	*  @return EC_Normal, if successful, error otherwise
	*/
	OFCondition applyStudyOrSeriesFromFile(DcmDataset *targetDset);

	/** Increments instance number in given dataset
	*  @param targetDset - [in/out] Dataset to increment instance number in
	*  @return EC_Normal, if successful, error otherwise
	*/
	OFCondition incrementInstanceNumber(DcmDataset *targetDset);

	/** Generates and inserts missing UIDs, ie UIDs not taken over
	*  from --dataset-from --study from and --series-from options
	*  @param dset - [in] The dataset to write the UIDs to
	*  @return EC_Normal, if successful, error otherwise
	*/
	OFCondition generateUIDs(DcmDataset *dset);

	/** Do some completeness / validity checks. Should be called when
	*  dataset is completed and is about to be saved.
	*  @param dataset - [in] The dataset to check
	*  @return Error string if error occurs, empty string otherwise
	*/
	OFString isValid(DcmDataset& dataset) const;

	/** Copy override keys over existing keys in given dataset.
	*  @param outputDset - [out] dataset to which the override keys are copied
	*/
	void applyOverrideKeys(DcmDataset *outputDset);

	/** Inserts "ISO_IR100" in the attribute "Specific Character Set".
	*  Overwrites any existing value.
	*  @param outputDset - [out] The dataset to write to
	*  @return EC_Normal if insertion is successfull, error code otherwise
	*/
	OFCondition insertLatin1(DcmDataset *outputDset);

	OFCondition insertUTF8(DcmDataset *outputDset);

private:

	/* Attributes for writing DICOM dataset */

	/// These attributes are applied to the dataset after conversion
	/// (and are not checked by the isValid() function)
	DcmDataset *m_overrideKeys;

	/// If not empty, the DICOM file specified in this variable is used
	/// as a base for the DICOM image file to be created, ie. all attributes
	/// are taken over from this template file
	OFString m_templateFile;

	/// If true, patient and study data is read from file
	OFBool m_readStudyLevel;

	/// If true, patient, study and series data is read from file
	OFBool m_readSeriesLevel;

	/// File to read study and series from
	OFString m_studySeriesFile;

	/// If true, Instance Number ist read from file and incremented by 1
	OFBool m_incInstNoFromFile;

	///  If true, some simple attribute checks (missing type 2 attributes or
	///  missing type 1 values) are omitted
	OFBool m_disableAttribChecks;

	/// If enabled, ISO_IR100 (ISO Latin 1) is inserted as a standard value
	/// for attribute "Specific Character Set". Insertion takes place after
	/// loading (optional) template file.
	OFBool m_insertLatin1;

	OFBool m_insertUTF8;

};
