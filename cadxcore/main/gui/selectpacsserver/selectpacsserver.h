/*
 *  
 *  $Id: selectpacsserver.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <string>
#include "selectpacsserverbase.h"


namespace GNC {
	namespace GUI {
		
		class SelectPacsServer: public SelectPacsServerBase
		{
		public:
			SelectPacsServer(wxWindow* pParent);
			~SelectPacsServer();
			std::string GetSelectedServer();
		};

	}
}
