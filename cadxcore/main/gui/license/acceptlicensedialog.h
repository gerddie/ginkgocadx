/*
 *  
 *  $Id: acceptlicensedialog.h $
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
#include "acceptlicensedialogbase.h"

namespace GNC{
	namespace GUI {
		class AcceptLicenseDialog : public AcceptLicenseDialogBase
		{
		public:
			AcceptLicenseDialog(wxWindow* pPadre);
			~AcceptLicenseDialog();

			virtual void OnAceptarClick(wxCommandEvent &event);

			//endregion

		};
	};
};
