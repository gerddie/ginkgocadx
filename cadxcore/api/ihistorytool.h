#pragma once
/*
*
*  $Id: ihistorytool.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IHISTORYTOOL_H
#define IHISTORYTOOL_H
#include <string>
#include <list>

#include <api/api.h>
#include <wx/bitmap.h>

class wxWindow;
class wxGinkgoToolBar;
class wxEvtHandler;
class wxMenu;

namespace GNC {
	namespace GCS {

		class EventHandlerHistoryTool;

		class EXTAPI IHistoryTool
		{
		public:
			typedef unsigned int UID;
			typedef enum {
					TFamily_Open,
					TFamily_Dicomize,
					TFamily_Delete,
					TFamily_ShowMetadata,
					TFamily_Q_R,
					TFamily_Anonymize,
					TFamily_Export,
					TFamily_Other,
					TFamily_Send
				} TToolFamily;

			IHistoryTool(UID id, TToolFamily family, const std::string& shortDescription, const std::string& description, const wxBitmap& icon, const wxBitmap& bigIcon = wxBitmap());

			virtual ~IHistoryTool();

			virtual void AppendToolIn(wxGinkgoToolBar* pParent, bool big = false);
			virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

			virtual void Execute() = 0;
			virtual void Execute(const std::list<long>& seriesPk);

			virtual bool Enabled() { return true;}
			virtual bool AreSeriesFullyDownloaded(const std::list<long>& seriesPk);

			UID ID;
			TToolFamily Family;
			std::string ShortDescription;
			std::string Description;
			wxBitmap Icon;
			wxBitmap BigIcon;
			EventHandlerHistoryTool* EventHandler;
		};
	}
}
#endif
