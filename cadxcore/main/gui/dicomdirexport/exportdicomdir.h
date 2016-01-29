/*
 *  
 *  $Id: exportdicomdir.h $
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
#include <api/iwizard.h>
#include <api/ivista.h>
#include <api/controllers/ipacscontroller.h>
#include "dicomdirexportbase.h"

#include <string>
#include <vector>

class IModeloDicom;

namespace GNC {

	namespace GUI {

		class ExportDicomDir: public ExportDicomBase
		{
			public:
				typedef std::list<long> TIDList;
				ExportDicomDir(wxWindow* pParent, const TIDList& pks);
				~ExportDicomDir();

			protected:
				virtual void OnExportClick(wxCommandEvent&);
		protected:
			TIDList m_pks;

		};
	}
}
