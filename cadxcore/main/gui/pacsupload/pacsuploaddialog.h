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
#include <list>
#include <map>
#include "pacsuploaddialogbase.h"

namespace GNC {
	namespace GUI {
		
		class PACSUploadDialog: public PACSUploadDialogBase
		{
		public:
			PACSUploadDialog(wxWindow* pParent, const std::list<long>& seriesId, const std::list<long>& selectedSeriesId = std::list<long>());
			~PACSUploadDialog();
			std::string GetSelectedServer();
			std::list<long> GetSelectedSeries();
		protected:
			std::map<int, long> m_positionsId;
		};

	}
}
