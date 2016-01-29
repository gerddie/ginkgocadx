/*
 *  
 *  $Id: dialogologin.h $
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
#include <main/gui/mainwindow/ventanaprincipalbase.h>

namespace GNC {
	namespace GUI {
		class DialogoLogin : public DialogoLoginBase {

		public:

			DialogoLogin(wxWindow* pParent);
			~DialogoLogin();

			virtual void OnLogin( wxCommandEvent& event );

		};
	}
}
