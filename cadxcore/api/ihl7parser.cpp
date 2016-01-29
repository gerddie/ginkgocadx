/*
 *  
 *  $Id: ihl7parser.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "ihl7parser.h"
#include <wx/datetime.h>


void GIL::Hl7Parser::Messages::MessageInterpreter::GenerateTimeStamp()
{
	std::string timeStamp(wxDateTime::Now().Format(wxT("%Y%m%d%H%M%S")).ToUTF8());
	
	MSH.SetTimeStamp(timeStamp);	
}
