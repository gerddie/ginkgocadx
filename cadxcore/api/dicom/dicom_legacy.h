

#ifdef DCMTK_POST_20170228
inline DcmElement *newDicomElement(const DcmTagKey& key)
{
	return DcmItem::newDicomElement(key); 
}

inline OFCondition newDicomElement(DcmElement *&newElement,
                                       const DcmTagKey &tag)
{
	return DcmItem::newDicomElement(newElement, tag); 
}

#endif 
