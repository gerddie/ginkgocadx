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


#include "videotool.h"
#include <wx/thread.h>
#include <export/contracts/islidercontract.h>
#include <api/globals.h>
#include <eventos/toolevents.h>
#include <api/controllers/ieventscontroller.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <resources/ginkgoresourcesmanager.h>
#include <vtkInteractorStyle.h>


#include <iostream>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

#define BUCLE_BUTTON 1211
#define SPEED_BUTTON 1212

namespace GNC
{
namespace GUI
{
class VideoToolTimer : public wxTimer
{
public:
        VideoToolTimer(GNC::VideoTool* pTool): VideoTool(pTool)
        {
        }
        ~VideoToolTimer()
        {
                VideoTool = NULL;
        }

        void StartTimer()
        {
                if (IsRunning()) {
                        Stop();
                }
                Start(VideoTool->GetPeriod(), true);
        }

        virtual void Notify()
        {
                if (VideoTool->GoToNextSlide()) {
                        Start(VideoTool->GetPeriod(), true);
                }
        }
protected:
        GNC::VideoTool* VideoTool;
};

class EventHandlerVideoTool: public wxEvtHandler
{
public:
        EventHandlerVideoTool(wxEvtHandler* pParent, GNC::VideoTool* pHerramienta):wxEvtHandler()
        {
                m_pParent = pParent;
                m_pHerramienta=pHerramienta;
                // Connect Events
                m_pParent->Connect(BUCLE_BUTTON, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerVideoTool::OnEnableBucle),NULL,this);
        }

        ~EventHandlerVideoTool()
        {
                m_pParent->Disconnect(BUCLE_BUTTON, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerVideoTool::OnEnableBucle),NULL,this);
        }

        virtual void OnEnableBucle(wxCommandEvent& event)
        {
                m_pHerramienta->SetBucle(!m_pHerramienta->IsBucleEnabled());
                event.Skip(false);
        }

        GNC::VideoTool* m_pHerramienta;
        wxEvtHandler* m_pParent;
};

class EventHandlerSetFPS: public wxEvtHandler
{
public:
        EventHandlerSetFPS(wxEvtHandler* pParent, GNC::VideoTool* pHerramienta, int fps, int id):wxEvtHandler()
        {
                m_pParent = pParent;
                m_pHerramienta=pHerramienta;
                m_fps = fps;
                m_id = id;
                // Connect Events
                m_pParent->Connect(m_id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerSetFPS::OnSetFPS),NULL,this);
        }

        ~EventHandlerSetFPS()
        {
                m_pParent->Disconnect(m_id, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerSetFPS::OnSetFPS),NULL,this);
        }

        virtual void OnSetFPS(wxCommandEvent& event)
        {
                m_pHerramienta->SetPeriod(m_fps);
                event.Skip(false);
        }

        GNC::VideoTool* m_pHerramienta;
        wxEvtHandler* m_pParent;
        int m_fps;
        int m_id;
};


}
}


GNC::VideoTool::VideoTool() : BlucleEnabled(true), Period(50)
{
        pTimer = new GNC::GUI::VideoToolTimer(this);
}

GNC::VideoTool::~VideoTool()
{
        delete pTimer;
        pTimer = NULL;
}


GNC::GCS::ITool* GNC::VideoTool::NewTool()
{
        return new GNC::VideoTool();
}

bool GNC::VideoTool::HasDropDownMenu()
{
        return true;
}

void GNC::VideoTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
        {
                wxBitmap iconBucle;
                wxString message;
                if (BlucleEnabled) {
                        iconBucle = GinkgoResourcesManager::CineBar::GetIcoNoBucle();
                        message = _("Disable loop");
                } else {
                        iconBucle = GinkgoResourcesManager::CineBar::GetIcoBucle();
                        message = _("Enable loop");
                }
                pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, BUCLE_BUTTON,message, iconBucle, new GNC::GUI::EventHandlerVideoTool(pParent, this)));
        }
        pMenu->AppendSeparator();
        wxBitmap icon;
        int periodos[8] = {50,125,250,500,1000,2000,5000,10000};
        for (int id = SPEED_BUTTON; id < SPEED_BUTTON + 8; id++) {
                int period = periodos[id-SPEED_BUTTON];
                GNC::GUI::wxMenuItemTool* pItem =new GNC::GUI::wxMenuItemTool(pMenu, id,wxString::Format(_("%.1f fps"), (1000.0/period) ), icon, new GNC::GUI::EventHandlerSetFPS(pParent, this, period, id),wxITEM_CHECK);
                pMenu->Append(pItem);
                pItem->Check(Period ==period);
        }
}

bool GNC::VideoTool::ExecuteAction()
{
        wxBitmap icon;
        if (pTimer->IsRunning()) {
                pTimer->Stop();
                icon = GinkgoResourcesManager::CineBar::GetIcoPlay();
        } else {
                if (SliderContract->GetSlice() == (SliderContract->GetNumberOfSlices()-1)) {
                        GoToNextSlide(true);
                }
                pTimer->StartTimer();
                icon = GinkgoResourcesManager::CineBar::GetIcoPause();
        }
        GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ChangeToolIconEvent(&icon, this, m_pParent));
        return true;
}

bool GNC::VideoTool::GoToNextSlide(bool force)
{
        bool continuePlaying = false;
        int maxSlice = SliderContract->GetNumberOfSlices();
        if (maxSlice > 1) {
                int current = SliderContract->GetSlice();
                if ((current + 1) < maxSlice) {
                        SliderContract->SetSlice(current+1);
                        continuePlaying = true;
                } else {
                        if (BlucleEnabled || force) {
                                SliderContract->SetSlice(0);
                                continuePlaying = true;
                        }
                }
        }
        if (!continuePlaying) {
                wxBitmap icon = GinkgoResourcesManager::CineBar::GetIcoPlay();
                GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ChangeToolIconEvent(&icon, this, m_pParent));
        }
        return continuePlaying;
}

bool GNC::VideoTool::IsBucleEnabled()
{
        return BlucleEnabled;
}
void GNC::VideoTool::SetBucle(bool bucle)
{
        BlucleEnabled = bucle;
}
int GNC::VideoTool::GetPeriod()
{
        return Period;
}
void GNC::VideoTool::SetPeriod(int period)
{
        if (period != this->Period) {
                this->Period = period;
                if (pTimer->IsRunning()) {
                        pTimer->Stop();
                        pTimer->StartTimer();
                }
        }
}
//endregion
