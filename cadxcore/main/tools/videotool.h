/*
 *  
 *  $Id: toolslider.h $
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
#ifndef VIDEOTOOL_H
#define VIDEOTOOL_H
#include <export/tools/ivideotool.h>
#include <api/api.h>

namespace GNC {
	namespace GUI {
		class VideoToolTimer;
	}
	class EXTAPI VideoTool : public GNC::GCS::IVideoTool {
	public:
		VideoTool();
		~VideoTool();

		virtual GNC::GCS::ITool* NewTool();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);

		virtual bool ExecuteAction();
		virtual bool GoToNextSlide(bool force = false);
		virtual bool IsBucleEnabled();
		virtual void SetBucle(bool bucle);
		virtual int GetPeriod();
		virtual void SetPeriod(int period);

	protected:
		GNC::GUI::VideoToolTimer* pTimer;
		bool BlucleEnabled;
		int Period;
	};
}
#endif