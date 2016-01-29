/*
 *  
 *  $Id: thumbnailcontroller.h $
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
namespace GNC {
	namespace GCS {
		class EXTAPI WindowAttributesController
		{			
		protected:
			WindowAttributesController();
			~WindowAttributesController();

		public:
			static void LoadWindowAttributes(const std::string& windowId, wxWindow* pWindow, bool defaultMaximized = true);
			static void SaveWindowAttributes(const std::string& windowId, wxWindow* pWindow);
		protected:
			
		};
	}
}
