/*
*
*  $Id: ihistorytool.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/

#include "ihistorytool.h"
#include <api/controllers/ihistorycontroller.h>

#include <main/gui/toolsystem/wxmenuitemtool.h>

#include <wx/dc.h>
#include <wx/sizer.h>
#include <wx/event.h>
#include <wx/bitmap.h>
#include <wx/menu.h>
#include <wx/wxginkgotoolbar.h>

namespace GNC {
	namespace GCS {
		class EventHandlerHistoryTool: public wxEvtHandler {
		public:
			EventHandlerHistoryTool(wxEvtHandler* pParent, GNC::GCS::IHistoryTool* pTool):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pTool=pTool;
				m_pParent->Connect(m_pTool->ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerHistoryTool::OnClick),NULL,this);
			}
			~EventHandlerHistoryTool()
			{
				m_pParent->Disconnect(m_pTool->ID, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerHistoryTool::OnClick),NULL,this);
				m_pParent = NULL;
				m_pTool = NULL;
			}

			void OnClick( wxCommandEvent& event )
			{
				m_pTool->Execute();
				event.Skip(false);
			}

			GNC::GCS::IHistoryTool* m_pTool;
			wxEvtHandler* m_pParent;			
		};

		class HistoryToolMenuItem : public wxMenuItem {
		public:
			HistoryToolMenuItem(wxEvtHandler* pParent, const wxString& nombre, wxMenu* pMenuPadre, GNC::GCS::IHistoryTool* pTool):wxMenuItem(pMenuPadre,pTool->ID,nombre,nombre,wxITEM_NORMAL) {
				pEventHandler = new EventHandlerHistoryTool(pParent,pTool);
#ifdef __WXMSW__
				SetBitmaps(pTool->Icon);
#else
				SetBitmap(pTool->Icon);
#endif
			}

			~HistoryToolMenuItem() {
				if(pEventHandler != NULL) {
					delete pEventHandler;
					pEventHandler = NULL;
				}
			}

			EventHandlerHistoryTool* pEventHandler;
		};

	}
}


GNC::GCS::IHistoryTool::IHistoryTool(UID id, TToolFamily family, const std::string& shortDescription, const std::string& description, const wxBitmap& icon, const wxBitmap& bigIcon) :
		ID(id),
		Family(family),
		ShortDescription(shortDescription),
		Description(description),
		Icon(icon),
		BigIcon(bigIcon),
		EventHandler(NULL)
{
}

GNC::GCS::IHistoryTool::~IHistoryTool()
{
}

void GNC::GCS::IHistoryTool::AppendToolIn(wxGinkgoToolBar* pParent, bool big)
{
	if (big) {
		if ( BigIcon.IsOk() ) {
			pParent->AddTool(this->ID, wxString::FromUTF8(ShortDescription.c_str()), BigIcon, wxString::FromUTF8(Description.c_str()));
		}
	} else {
		pParent->AddTool(this->ID, wxString::FromUTF8(ShortDescription.c_str()), Icon, wxString::FromUTF8(Description.c_str()));
	}	
}

void GNC::GCS::IHistoryTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	pMenu->Append(new HistoryToolMenuItem(pParent,wxString::FromUTF8(Description.c_str()),pMenu,this));
}

bool GNC::GCS::IHistoryTool::AreSeriesFullyDownloaded(const std::list<long>& seriesPk)
{
	for (std::list<long>::const_iterator it = seriesPk.begin(); it != seriesPk.end(); ++it)
	{
		if (GNC::GCS::IHistoryController::Instance()->GetSeriesModel((*it)).location == GNC::GCS::IHistoryController::TL_WadoLinked) {
			return false;
		}
	}
	return true;
}

//ignore series by default...
void GNC::GCS::IHistoryTool::Execute(const std::list<long>& /*seriesPk*/)
{
	Execute();
}
