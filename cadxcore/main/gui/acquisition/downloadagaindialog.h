/*
 *
 *  $Id: InactivityCountdownMessage.h $
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

#include <wx/timer.h>
#include "dialogoadquisicionbase.h"

namespace GNC {
	namespace GUI {
		class DownloadAgainDialog: public DownloadAgainDialogBase
		{
		public:
			DownloadAgainDialog(wxWindow* pParent, std::string uid, bool isSeries);
			~DownloadAgainDialog();
			virtual void OnOpenClick(wxCommandEvent &evt);
		protected:
			std::string UID;
			bool IsSeries;
		};
	}
}
