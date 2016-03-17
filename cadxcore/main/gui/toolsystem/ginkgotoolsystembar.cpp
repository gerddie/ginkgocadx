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

#include "ginkgotoolsystembar.h"
#include <api/toolsystem/itoolcontroller.h>
#include <api/toolsystem/iwidgettool.h>
#include <resources/ginkgoresourcesmanager.h>
#include <eventos/toolevents.h>
#include <api/controllers/ieventscontroller.h>
#include <wx/wxginkgotoolbar.h>
#include <wx/menu.h>
#include <wx/aui/aui.h>

#define MOUSE_MARK_BORDER_COLOR 128, 128, 128
#define MOUSE_MARK_OFFSETS 2, 8, 14
#define MOUSE_MARK_BRUSHES wxRED_BRUSH, wxGREEN_BRUSH, wxBLUE_BRUSH

class GinkgoToolSystemBarArt : public wxGinkgoToolBarArt
{
public:

        GinkgoToolSystemBarArt(GNC::GCS::IToolController* pControlador)
        {
                m_pToolBar = pControlador;
        }

        virtual ~GinkgoToolSystemBarArt() {}


        ////////////////////////////////////////////////////begin draw drowpdown////////////////////////////////////////
        //WXWIDGETS DOESN'T SUPPORT DRAW CHECKED + DROPDOWN
        // wxAuiBlendColour is used by wxAuiStepColour
        unsigned char wxAuiBlendColour(unsigned char fg, unsigned char bg, double alpha)
        {
                double result = bg + (alpha * (fg - bg));
                if (result < 0.0)
                        result = 0.0;
                if (result > 255)
                        result = 255;
                return (unsigned char)result;
        }

        // wxAuiStepColour() it a utility function that simply darkens
        // or lightens a color, based on the specified percentage
        // ialpha of 0 would be completely black, 100 completely white
        // an ialpha of 100 returns the same colour
        wxColor wxAuiStepColour(const wxColor& c, int ialpha)
        {
                if (ialpha == 100)
                        return c;

                unsigned char r = c.Red(),
                              g = c.Green(),
                              b = c.Blue();
                unsigned char bg;

                // ialpha is 0..200 where 0 is completely black
                // and 200 is completely white and 100 is the same
                // convert that to normal alpha 0.0 - 1.0
                ialpha = wxMin(ialpha, 200);
                ialpha = wxMax(ialpha, 0);
                double alpha = ((double)(ialpha - 100.0))/100.0;

                if (ialpha > 100) {
                        // blend with white
                        bg = 255;
                        alpha = 1.0 - alpha;  // 0 = transparent fg; 1 = opaque fg
                } else {
                        // blend with black
                        bg = 0;
                        alpha += 1.0;         // 0 = transparent fg; 1 = opaque fg
                }

                r = wxAuiBlendColour(r, bg, alpha);
                g = wxAuiBlendColour(g, bg, alpha);
                b = wxAuiBlendColour(b, bg, alpha);

                return wxColour(r, g, b);
        }

        ////////////////////////////////////////////////////end draw drowpdown////////////////////////////////////////

        void DrawExtraDecoration(
                wxDC& dc,
                wxWindow* /*wnd*/,
                const wxAuiToolBarItem& item,
                const wxRect& rect)
        {
                if (item.IsActive() && (item.GetState() & wxAUI_BUTTON_STATE_CHECKED) ) {
                        GNC::GCS::IWidgetTool* pLeftTool = m_pToolBar->GetLeftTool();
                        GNC::GCS::IWidgetTool* pRightTool = m_pToolBar->GetRightTool();
                        GNC::GCS::IWidgetTool* pMiddleTool = m_pToolBar->GetMiddleTool();

                        static const int offx[3] = { MOUSE_MARK_OFFSETS };

                        bool drawItem[3] = {false, false, false};

                        if ( (pLeftTool != NULL) && (pLeftTool->ID == (unsigned int)item.GetId()) && (pLeftTool->GetTriggerButton().IsLeftEnabled()) ) {
                                drawItem[0] = true;
                        }
                        if ( (pMiddleTool != NULL) && (pMiddleTool->ID == (unsigned int)item.GetId()) && (pMiddleTool->GetTriggerButton().IsMiddleEnabled()) ) {
                                drawItem[1] = true;
                        }
                        if ( (pRightTool != NULL) && (pRightTool->ID == (unsigned int)item.GetId()) && (pRightTool->GetTriggerButton().IsRightEnabled()) ) {
                                drawItem[2] = true;
                        }
                        if (drawItem[0] || drawItem[1] || drawItem[2]) {
                                dc.SetPen( wxPen( wxColor(MOUSE_MARK_BORDER_COLOR), 1 ) );
                                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                                dc.DrawRoundedRectangle(rect.x + 1, rect.height - 6, 20, 7, -0.25);

                                static const wxBrush* brush[3] = {MOUSE_MARK_BRUSHES};

                                for (int i = 0; i < 3; ++i) {
                                        if (drawItem[i]) {
                                                dc.SetBrush(*brush[i]);
                                        } else {
                                                dc.SetBrush(*wxTRANSPARENT_BRUSH);
                                        }
                                        dc.DrawRoundedRectangle(rect.x + offx[i], rect.height - 6, 6, 7, -0.25);
                                }
                        }
                }
        }

        virtual void DrawDropDownButton(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect)
        {
                wxGinkgoToolBarArt::DrawDropDownButton(dc, wnd, item, rect);
                DrawExtraDecoration(dc, wnd, item, rect);
        }

        virtual void DrawButton(
                wxDC& dc,
                wxWindow* wnd,
                const wxAuiToolBarItem& item,
                const wxRect& rect)
        {
                wxGinkgoToolBarArt::DrawButton(dc, wnd, item, rect);
                DrawExtraDecoration(dc, wnd, item, rect);
        }

private:
        GNC::GCS::IToolController* m_pToolBar;

};

GNC::GUI::GinkgoToolSystemBar::GinkgoToolSystemBar(wxWindow* pParent, GNC::GCS::IToolController* pToolBar, const GNC::GCS::ITool::TToolFamily& family) : wxGinkgoToolBar(pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize,wxAUI_TB_DEFAULT_STYLE )
{
        this->Connect(wxID_ANY,wxEVT_LEFT_UP, wxMouseEventHandler( GinkgoToolSystemBar::OnLeftUp) , NULL, this);
        this->Connect(wxID_ANY,wxEVT_RIGHT_UP, wxMouseEventHandler( GinkgoToolSystemBar::OnRightUp) , NULL, this);
        this->Connect(wxID_ANY,wxEVT_MIDDLE_UP, wxMouseEventHandler( GinkgoToolSystemBar::OnMiddleUp) , NULL, this);
        this->Connect(wxID_ANY, wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN,  wxAuiToolBarEventHandler(GinkgoToolSystemBar::OnDropDownTool) , NULL, this);
        pParent->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(GinkgoToolSystemBar::OnKeyEvent ), NULL, this);

        SetToolBitmapSize(wxSize(16,16));
        SetAutoLayout(true);
        m_pToolBar = pToolBar;
        GinkgoToolSystemBarArt* pToolArt = new GinkgoToolSystemBarArt(pToolBar);
        wxColor color = pToolArt->GetCustomBaseColor();
        switch(family) {
        case GNC::GCS::ITool::TFamiliaGlobal:
                color = wxColor(67,67,67);
                break;
        case GNC::GCS::ITool::TFamiliaVisualizacion:
                color = wxColor(255,255,186);
                break;
        case GNC::GCS::ITool::TFamiliaAnotacion:
                color = wxColor(255,186,186);
                break;
        case GNC::GCS::ITool::TFamiliaMedicion:
                color = wxColor(217,255,186);
                break;
        case GNC::GCS::ITool::TFamiliaVision:
                color = wxColor(186,230,255);
                break;
        case GNC::GCS::ITool::TFamiliaChroma:
                color = wxColor(230,181,242);
                break;
        case GNC::GCS::ITool::TMenuVer:
                break;
        case GNC::GCS::ITool::TSeriesMenu:
                break;
        case GNC::GCS::ITool::TMenuHerramientas:
                break;
        case GNC::GCS::ITool::TMenuExport:
                break;
        case GNC::GCS::ITool::TMenuEdicion:
                break;
        default:
                break;
        }
        pToolArt->SetCustomBaseColor(color);

        SetArtProvider(pToolArt);

        GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::ToolConnectedEvent());
        GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::ToolEnabledEvent());
        GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::ChangeToolIconEvent());
}

GNC::GUI::GinkgoToolSystemBar::~GinkgoToolSystemBar()
{
        this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxMouseEventHandler( GinkgoToolSystemBar::OnLeftUp), NULL, this);
        this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxMouseEventHandler( GinkgoToolSystemBar::OnRightUp), NULL, this);
        this->Disconnect(wxID_ANY, wxEVT_COMMAND_TOOL_CLICKED, wxMouseEventHandler( GinkgoToolSystemBar::OnMiddleUp), NULL, this);
        this->Disconnect(wxID_ANY, wxEVT_COMMAND_AUITOOLBAR_TOOL_DROPDOWN,  wxAuiToolBarEventHandler(GinkgoToolSystemBar::OnDropDownTool) , NULL, this);
        GetParent()->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler( GinkgoToolSystemBar::OnKeyEvent ), NULL, this);
        m_pToolBar=NULL;
}

wxRect GNC::GUI::GinkgoToolSystemBar::GetToolRect(int tool_id) const
{
        wxRect rect = wxAuiToolBar::GetToolRect(tool_id);
        rect.x -= 5;
        rect.y -= 5;
        rect.width += 5;
        rect.height += 5;
        return rect;
}

void GNC::GUI::GinkgoToolSystemBar::OnRightUp(wxMouseEvent& evt)
{
        wxAuiToolBarItem* hit_item;
        hit_item = FindToolByPosition(evt.GetX(), evt.GetY());
        if (hit_item != NULL && hit_item->IsActive()) {
                m_pToolBar->ActivateTool(hit_item->GetId(), GNC::GCS::TriggerButton().EnableRight(), this);
        }
        //don't remove this line, parent has to process the event!!
        wxGinkgoToolBar::OnRightUp(evt);
}

void GNC::GUI::GinkgoToolSystemBar::OnLeftUp(wxMouseEvent& evt)
{
        wxAuiToolBarItem* hit_item;
        hit_item = FindToolByPosition(evt.GetX(), evt.GetY());
        if (hit_item != NULL && hit_item->IsActive()) {
                GNC::GCS::ITool* pTool = m_pToolBar->GetTool(hit_item->GetId());
                if (pTool !=NULL && !pTool->AppendsInMenu()) {
                        //if appends in menu avoid double calling
                        m_pToolBar->ActivateTool(hit_item->GetId(), GNC::GCS::TriggerButton().EnableLeft(), this);
                }
        }
        //don't remove this line, parent has to process the event!!
        wxGinkgoToolBar::OnLeftUp(evt);
}

void GNC::GUI::GinkgoToolSystemBar::OnMiddleUp(wxMouseEvent& evt)
{
        wxAuiToolBarItem* hit_item;
        hit_item = FindToolByPosition(evt.GetX(), evt.GetY());
        if (hit_item != NULL && hit_item->IsActive()) {
                m_pToolBar->ActivateTool(hit_item->GetId(), GNC::GCS::TriggerButton().EnableMiddle(), this);
        }
        //don't remove this line, parent has to process the event!!
        wxGinkgoToolBar::OnMiddleUp(evt);
}

void GNC::GUI::GinkgoToolSystemBar::OnDropDownTool(wxAuiToolBarEvent& evt)
{
        if (evt.IsDropDownClicked()) {
                wxAuiToolBarItem* hit_item;
                hit_item = FindToolByPosition(evt.GetClickPoint().x, evt.GetClickPoint().y);
                if (hit_item != NULL && hit_item->IsActive()) {
                        //Activate...
                        wxAuiToolBar* tb = static_cast<wxAuiToolBar*>(evt.GetEventObject());

                        tb->SetToolSticky(evt.GetId(), true);
                        // create the popup menu
                        wxMenu menuPopup;

                        m_pToolBar->AppendDropDownMenu(hit_item->GetId(), &menuPopup, &menuPopup);

                        // line up our menu with the button
                        wxRect rect = tb->GetToolRect(evt.GetId());
                        wxPoint pt = tb->ClientToScreen(rect.GetBottomLeft());
                        pt = ScreenToClient(pt);

                        PopupMenu(&menuPopup, pt);

                        // make sure the button is "un-stuck"
                        tb->SetToolSticky(evt.GetId(), false);
                        //	m_pToolBar->ActivateTool(hit_item->GetId(), GNC::GCS::TriggerButton().EnableLeft(), this);
                }
        }
        evt.Skip(true);
}

void GNC::GUI::GinkgoToolSystemBar::OnKeyEvent(wxKeyEvent& event)
{
        GNC::GCS::Events::EventoTeclado evt(&event, NULL);
        evt.Skip(true);
        m_pToolBar->OnKeyAccelerator(evt, this);
        /*
        #if defined(__WXGTK__)
        if (m_pParent != NULL) {
        m_pParent->SetFocus();
        }
        #endif
        */
        event.Skip(evt.GetSkipped());
        event.ResumePropagation(3);
}

void GNC::GUI::GinkgoToolSystemBar::ProcesarEvento(GNC::GCS::Events::IEvent* evt)
{
        switch (evt->GetCodigoEvento()) {
        case ginkgoEVT_Core_ToolConnected: {
                GNC::GCS::Events::ToolConnectedEvent* pToolEvt = static_cast<GNC::GCS::Events::ToolConnectedEvent*>(evt);
                if (pToolEvt->GetToolBar() == m_pToolBar) {
                        this->ForceToggleTool(pToolEvt->GetTool()->ID, pToolEvt->IsConnected());
                        wxRect rect = GetToolRect(pToolEvt->GetTool()->ID);
                        Refresh(true, &rect);
                }
        }
        break;
        case ginkgoEVT_Core_ToolEnabled: {
                GNC::GCS::Events::ToolEnabledEvent* pToolEvt = static_cast<GNC::GCS::Events::ToolEnabledEvent*>(evt);
                if (pToolEvt->GetToolBar() == m_pToolBar) {
                        this->EnableTool(pToolEvt->GetTool()->ID, pToolEvt->IsEnabled());
                        wxRect rect = GetToolRect(pToolEvt->GetTool()->ID);
                        Refresh(true, &rect);
                }
        }
        break;
        case ginkgoEVT_Core_ToolIconChanged: {
                GNC::GCS::Events::ChangeToolIconEvent* pToolEvt = static_cast<GNC::GCS::Events::ChangeToolIconEvent*>(evt);
                if (pToolEvt->GetToolBar() == m_pToolBar) {
                        this->SetToolBitmap(pToolEvt->GetTool()->ID, *pToolEvt->GetBitmap());
                        wxRect rect = GetToolRect(pToolEvt->GetTool()->ID);
                        Refresh(true, &rect);
                }
        }
        break;
        }
}
