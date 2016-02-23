/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
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
