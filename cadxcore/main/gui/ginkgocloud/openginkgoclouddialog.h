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

		class OpenGinkgoCloudDialog: public GNC::GUI::OpenGinkgoCloudDialogBase
		{
		public:
			OpenGinkgoCloudDialog(wxWindow* pParent);
			~OpenGinkgoCloudDialog();

		protected:
			virtual void OnCancel(wxCommandEvent& evt);
			virtual void OnAccept(wxCommandEvent &evt);
			virtual void OnKeyUp(wxKeyEvent &evt);
		};
	}
}
