#pragma once
/*
 *  
 *  $Id: iwidgettool.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IWIDGETTOOL_H
#define IWIDGETTOOL_H

#include <api/toolsystem/itool.h>

namespace GNC {

	namespace GCS {
		class EXTAPI IWidgetTool: public GNC::GCS::ITool {
		public:
			virtual ~IWidgetTool();

			//new tool
			IWidgetTool(const ITool::UID& uid, const ITool::TToolFamily& familyUID, const std::string& name, const wxBitmap& icon, const int subFamily = 0xFFFFFFFF, const int& priority = 0);

			virtual bool AppendsInMenu() {
				return false;
			}
			virtual bool ExecuteAction();

			virtual void Connect() = 0;
			virtual void Disconnect() = 0;

			virtual void AppendToolIn(wxGinkgoToolBar* pParent);

			TriggerButton& GetTriggerButton() {
				return m_TriggerButtonMask;
			}

			TriggerButton m_TriggerButtonMask;
			//

		};
	}
}
#endif