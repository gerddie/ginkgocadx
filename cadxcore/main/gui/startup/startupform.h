/*
 *  
 *  $Id: startupform.h $
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
#include "startupformbase.h"
#include "startupview.h"

namespace GNC {
	namespace GUI {
		class StartUpForm: public StartUpFormBase {
		public:
			StartUpForm(GNC::GCS::IVista* pView);
			~StartUpForm();

			virtual void OnUpgradeLink( wxHyperlinkEvent& event );
		protected:
			virtual void OnEraseBackground(wxEraseEvent& event);
			//virtual void OnPaint(wxPaintEvent &event);
			GNC::GCS::IVista* IView;
		};
	}
}