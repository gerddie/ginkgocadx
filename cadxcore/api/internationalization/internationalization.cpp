/*
 *  
 *  $Id: internationalization.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <string>

#include <api/internationalization/internationalization.h>

#include <wx/intl.h>

std::string EXTAPI _Std(const char* s) {
	wxString wxStr = wxString::FromUTF8(s);
	wxStr = wxGetTranslation(wxStr);
	return std::string(wxStr.ToUTF8());
}

