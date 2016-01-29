/*
 *  
 *  $Id: secondarycapturetool.h $
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

#include <export/tools/isimpleprinttool.h>
#include <api/api.h>

class wxPrintData;
namespace GNC {
	class EXTAPI SimplePrintTool : public GNC::GCS::ISimplePrintTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		SimplePrintTool();
		~SimplePrintTool();
		
		virtual bool ExecuteAction();
		
		virtual bool HasDropDownMenu();
		virtual void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

		void doPrint();
		void doPrintPreview();
		void doSetup();

	protected:
		wxPrintData* pData;
	};
}