/*
 *  
 *  $Id: colormaptool.h 1032 2013-09-10 08:28:42Z carlos $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#ifndef COLORMAPTOOL_H
#define COLORMAPTOOL_H

#include <export/tools/icolormaptool.h>
#include <api/api.h>

namespace GNC {

	class ColorMapTool : public GNC::GCS::IColorMapTool
	{
		public:
		virtual GNC::GCS::ITool* NewTool();
		ColorMapTool();
		~ColorMapTool();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		
		virtual bool ExecuteAction();
		virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);
		
		//cuando hacen click en ella...
		virtual bool SoportaMapas();
		virtual void SeleccionarMapa(int id);

		int GetIdLookupTable();
		virtual void InvertColorMap();

	protected:

	};
}
#endif
