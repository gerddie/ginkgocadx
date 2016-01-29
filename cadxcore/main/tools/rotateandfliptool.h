/*
 *  
 *  $Id: resettool.h $
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
#ifndef ROTATEANDFLIPTOOL_H
#define ROTATEANDFLIPTOOL_H

#include <export/tools/irotateandfliptool.h>
#include <api/api.h>

namespace GNC {

	class EXTAPI RotateAndFlipTool : public GNC::GCS::IRotateAndFlipTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		RotateAndFlipTool();
		~RotateAndFlipTool();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

		virtual void Rotate(bool right);
		virtual void Flip(bool vertical);

		virtual void Clear();

	protected:

	};
}
#endif