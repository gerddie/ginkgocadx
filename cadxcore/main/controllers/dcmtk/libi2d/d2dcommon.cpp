#include "d2dcommon.h"
/** Check whether given type 1 attribute is present and has a length > 0.
*  @param  key - [in] The attribute tag check
*  @param  targetDset - [out] targetDset
*  @param  defaultValue - [in] value to be inserted if attribute is missing
*                         (needs invent option for type 1 attributes enabled)
*  @return A string with an error message if attribute is not present
*          or has length of 0
*/
OFString D2DCommon::checkAndInventType1Attrib(const DcmTagKey& key,
															 DcmDataset* targetDset, 
															 const OFString& defaultValue) const
{
	OFString err;
	OFBool exists = targetDset->tagExists(key);
	if (!exists) {
		OFString err = "Document2Dcm: Missing type 1 attribute: ";
		err += DcmTag(key).getTagName();
		err += "\n";
		return err;
	}
	DcmElement *elem;
	OFCondition cond = targetDset->findAndGetElement(key, elem);
	if (cond.bad() || !elem || (elem->getLength() == 0)) {
		if (!m_inventMissingType1Attribs) {
			err += "Document2Dcm: Empty value for type 1 attribute: ";
			err += DcmTag(key).getTagName();
			err += "\n";
			return err;
		}
		//holds element to insert in item
		DcmElement *elem = NULL;
		DcmTag tag(key);
		OFBool wasError = OFFalse;
		//if dicom element could be created, insert in to item and modify to value
		if (newDicomElement(elem, tag).good()) {
			if (targetDset->insert(elem, OFTrue).good()) {
				if (elem->putString(defaultValue.c_str()).good()) {
					if (m_debug) {
						OFString msg = "Document2Dcm: Inserting missing type 1 attribute ";
						msg += tag.getTagName();
						msg += " with value ";
						msg += defaultValue;
						printMessage(m_logStream, msg);
						return err;
					}
				} else wasError = OFTrue;
			} else wasError = OFTrue;
		} else wasError = OFTrue;
		if (wasError) {
			err += "Unable to insert type 1 attribute ";
			err += tag.getTagName();
			err += " with value ";
			err += defaultValue;
			err += "\n";
		}
	}
	return err;
}


/** Inserts an empty element with the given tag into a dataset
*  @param  key - [in] The attribute tag to check
*  @param  targetDset - [out] targetDset
*  @return A string with an error message if attribute is not present
*          nor gets inserted automatically (see m_inventMissingType2Attribs)
*/
OFString D2DCommon::checkAndInventType2Attrib(const DcmTagKey& key,
															 DcmDataset* targetDset) const
{
	OFString err;
	OFBool exists = targetDset->tagExists(key);
	if (!exists) {
		if (m_inventMissingType2Attribs) {
			DcmTag tag(key);
			if (m_debug)
				printMessage(m_logStream, "Document2Dcm: Inserting missing type 2 attribute: ", tag.getTagName());
			targetDset->insertEmptyElement(tag);
		} else {
			err = "Document2Dcm: Missing type 2 attribute: ";
			err += DcmTag(key).getTagName();
			err += "\n";
			return err;
		}
	}
	return err;
}

