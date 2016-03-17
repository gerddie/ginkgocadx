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

#include "startupform.h"
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/entorno.h>
#include <export/tools/iherramientascore.h>
#include <main/gui/history3/historypanel3.h>
#include <main/controllers/historytoolscontroller.h>
#include <resources/ginkgoresourcesmanager.h>

#include <wx/xml/xml.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/dcbuffer.h>
#include <wx/aui/auibar.h>
#include <wx/wxginkgotoolbar.h>

#define ID_FILE_DROP_DOWN	1781
#define ID_SHOW_HISTORY	1782

namespace GNC
{
namespace GUI
{
//////////////////////////////////////////////////////////////////
struct RSSEntry {
        std::string content;
        std::string updated;
        std::string link;
};
typedef std::list<RSSEntry> TListOfEntries;
//////////////////////////////////////////////////////////////////
///////////////////TOOL BAR///////////////////////////////////////
class StartupFormToolbar : public wxGinkgoToolBar
{
public:
        StartupFormToolbar(wxWindow* pParent, const wxColor& color = wxColor(255, 255, 255)): wxGinkgoToolBar(pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_TEXT)
        {
                SetToolBitmapSize(wxSize(64,64));
                wxGinkgoToolBarArt* pToolArt= new wxGinkgoToolBarArt(color);
                SetBackgroundColour(wxColour(255,255,255));
                SetArtProvider(pToolArt);

                AddTool(ID_SHOW_HISTORY, _("History"), GinkgoResourcesManager::BigIcons::GetIcoBigBook());

                if(GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","acquire") && GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits","pacs_acquisition")) {
                        AddTool(ID_FILE_DROP_DOWN, _("Acquire"), GinkgoResourcesManager::BigIcons::GetIcoBigAcquire());
                        ForceSetToolDropDown(ID_FILE_DROP_DOWN, true);
                        this->Connect(ID_FILE_DROP_DOWN,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(StartupFormToolbar::OnDropDownAcquire),NULL, this );
                }

                GNC::HistoryToolsController::Instance()->AppendsBigToolsInToolBar(this);

                this->Connect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(StartupFormToolbar::OnToolClick),NULL, this );
        }

        ~StartupFormToolbar()
        {
                this->Disconnect(ID_FILE_DROP_DOWN,wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN, wxAuiToolBarEventHandler(StartupFormToolbar::OnDropDownAcquire),NULL, this );
                this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(StartupFormToolbar::OnToolClick),NULL, this );
        }


        void OnDropDownAcquire(wxAuiToolBarEvent& event)
        {
                wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(event.GetEventObject());

                tb->SetToolSticky(event.GetId(), true);

                // create the popup menu
                wxMenu menuPopup;

                GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menuPopup, &menuPopup, GNC::GCS::IHistoryTool::TFamily_Open);

                // line up our menu with the button
                wxRect rect = tb->GetToolRect(event.GetId());
                wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
                pt = ScreenToClient(pt);

                PopupMenu(&menuPopup, pt);

                // make sure the button is "un-stuck"
                tb->SetToolSticky(event.GetId(), false);
        }

        void OnToolClick(wxCommandEvent& event)
        {
                switch (event.GetId()) {
                case ID_SHOW_HISTORY: {
                        if (GNC::GUI::HistoryPanel3::Instance()->IsIconized()) {
                                GNC::GUI::HistoryPanel3::Instance()->Restore();
                        }
                        if (!GNC::GUI::HistoryPanel3::Instance()->IsShown()) {
                                GNC::GUI::HistoryPanel3::Instance()->Show(true);
                        } else {
                                GNC::GUI::HistoryPanel3::Instance()->Raise();
                        }
                }
                break;
                case ID_FILE_DROP_DOWN: {
                }
                break;
                default: {
                        GNC::GCS::IHistoryTool* pTool = GNC::HistoryToolsController::Instance()->GetTool(event.GetId());
                        if (pTool != NULL) {
                                pTool->Execute();
                        }
                }
                break;
                }
        }
};
//////////////////////////////////////////////////////////////////
class StartUpTitle: public wxPanel
{
public:
        StartUpTitle(wxWindow* pParent) : wxPanel(pParent)
        {
                m_bigFont = wxFont( 18, 70, 90, 90, false, wxEmptyString );

                wxWindow::SetBackgroundStyle(wxBG_STYLE_CUSTOM);

                wxClientDC dc(this);
                dc.SetFont(m_bigFont);
                int height,width;
                dc.GetTextExtent(wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str()) , &width, &height, NULL, NULL, NULL);

                SetSize(width + 20 + 20, height + 20);
                SetMinSize(wxSize(width + 20 + 20, height + 20));

                std::ostringstream ostrVersion;
                ostrVersion << GNC::Entorno::Instance()->GetApplicationName() << " v";
                ostrVersion << GNC::Entorno::Instance()->GetGinkgoVersion();

                if (sizeof(void*) == 4) {
                        ostrVersion << " 32 bits";
                } else if (sizeof(void*) == 8) {
                        ostrVersion << " 64 bits";
                }

                if(GNC::GCS::IControladorPermisos::Instance()->Get("core.license","licenseinfo")) {
                        ostrVersion << " ("<< GNC::GCS::IControladorPermisos::Instance()->Get("core.license","licenseinfo").GetRawValue() << ")";
                }
                m_Version = wxString::FromUTF8(ostrVersion.str().c_str());

                this->Connect( wxEVT_PAINT, wxPaintEventHandler( StartUpTitle::OnPaint ), NULL, this );
                this->Connect( wxEVT_SIZE, wxSizeEventHandler( StartUpTitle::OnSize ), NULL, this );
        }
        ~StartUpTitle()
        {
                this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( StartUpTitle::OnPaint ), NULL, this );
                this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( StartUpTitle::OnSize ), NULL, this );
        }
protected:
        virtual void OnPaint(wxPaintEvent &event)
        {
                wxPaintDC dc(this);
                PrepareDC(dc);

                int width = dc.GetSize().x;
                int height = dc.GetSize().y;
                wxRect cr(0,0, width, height);

                dc.GradientFillLinear(cr, wxColour(255,255,255), wxColour(200,200,200), wxNORTH);

                dc.SetFont(m_bigFont);
                wxCoord tx = 0, ty = 0;
                wxString str = wxString::FromUTF8(GNC::Entorno::Instance()->GetApplicationName().c_str());
                dc.GetTextExtent(str, &tx, &ty, NULL, NULL, NULL);
                dc.DrawText(str, 20,20);

                //draw version text...
                dc.SetFont(GetFont());
                dc.GetTextExtent(m_Version, &tx, &ty, NULL, NULL, NULL);
                dc.DrawText(m_Version, width - tx - 20, height - ty);

                event.Skip(false);
        }

        void OnSize(wxSizeEvent & event)
        {
                this->Refresh(false);
                event.Skip(false);
        }

        wxFont m_bigFont;
        wxString m_Version;
};
//////////////////////////////////////////////////////////////////
StartUpForm::StartUpForm(GNC::GCS::IVista* pView): StartUpFormBase(pView->GetEstudio()->ParentWindow)
{
        StartUpTitle* pFooter = new StartUpTitle( this);
        GetSizer()->Insert(0, pFooter, 0, wxEXPAND);

        IView = pView;
        wxAuiToolBar * pToolbar = new StartupFormToolbar(this);
        m_pSizerToolBar->Add( 0, 0, 1, wxEXPAND, 5 );
        m_pSizerToolBar->Add(pToolbar, 0, wxALIGN_CENTER_VERTICAL);
        m_pSizerToolBar->Add( 0, 0, 1, wxEXPAND, 5 );
        pToolbar->Realize();

        Layout();
}
StartUpForm::~StartUpForm()
{
        //this is the common way to destroy view without leaks
        IView->Lock();
        delete IView;
}

void StartUpForm::OnUpgradeLink(wxHyperlinkEvent& /*event*/)
{

}

void StartUpForm::OnEraseBackground(wxEraseEvent &event)
{
        event.Skip(true);
}

/*void StartUpForm::OnPaint(wxPaintEvent &event)
{
	wxPaintDC dc(this);
	PrepareDC(dc);

	int width = dc.GetSize().x;
	int height = dc.GetSize().y/2;
	wxRect cr(0,height, width, height);

	dc.GradientFillLinear(cr, wxColour(255,255,255), wxColour(200,200,200), wxSOUTH);

	dc.SetFont(m_bigFont);
	wxCoord tx = 0, tyBig = 0, ty = 0;
	wxString str = wxT("Ginkgo CADx");
	dc.GetTextExtent(str, &tx, &tyBig, NULL, NULL, NULL);
	dc.DrawText(str, 20,20);

	//draw version text...
	dc.SetFont(GetFont());
	dc.GetTextExtent(m_Version, &tx, &ty, NULL, NULL, NULL);
	dc.DrawText(m_Version, width - tx - 20, 20 + tyBig - ty);
	StartUpFormBase::OnPaint(event);
}*/
}
}
