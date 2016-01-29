/*
 *  
 *  $Id: selectexternalfiles.h $
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
#include "downloadcloudbase.h"

namespace GNC {
	namespace GUI {

		class FileLinkDialog: public GNC::GUI::FileLinkDialogBase
		{
		public:
			FileLinkDialog(wxWindow* pParent, std::string fileLink);
			~FileLinkDialog();

		protected:
			virtual void OnCopyClipboard(wxCommandEvent& evt);
			virtual void OnAccept(wxCommandEvent &evt);
			virtual void OnKeyUp(wxKeyEvent &evt);
			virtual void OnSendMail(wxCommandEvent& evt);
		};
	}
}
