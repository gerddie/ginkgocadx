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

#include <wx/menu.h>

#include "cornerannotationstool.h"
#include <api/iwidgetsmanager.h>
#include <export/contracts/iwidgetscontract.h>

#include <main/controllers/controladoreventos.h>
#include <eventos/render.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#if defined(ENABLE_CORNERANNOTATIONTOOL)

namespace GNC
{
namespace GUI
{
class wxCornerAnnotationsEventHandler: public wxDefaultEvtHandlerTool
{
public:
        wxCornerAnnotationsEventHandler(wxEvtHandler* pParent, GNC::CornerAnnotationsTool* pTool): wxDefaultEvtHandlerTool(pParent, pTool)
        {
                m_pCornerTool = pTool;
        }

        ~wxCornerAnnotationsEventHandler()
        {
        }

        virtual void OnUpdateUI(wxUpdateUIEvent &event)
        {
                wxDefaultEvtHandlerTool::OnUpdateUI(event);
                if(m_pCornerTool->IsVisible()) {
                        event.SetText(_("Hide corner annotations"));
                } else {
                        event.SetText(_("Show corner annotations"));
                }
        }
        GNC::CornerAnnotationsTool* m_pCornerTool;
};

}
}


GNC::GCS::ITool* GNC::CornerAnnotationsTool::NewTool()
{
        return new GNC::CornerAnnotationsTool();
}

GNC::CornerAnnotationsTool::CornerAnnotationsTool()
{
}
GNC::CornerAnnotationsTool::~CornerAnnotationsTool()
{
}

bool GNC::CornerAnnotationsTool::ExecuteAction()
{
        WidgetsContract->GetManager()->ShowAnnotations(!WidgetsContract->GetManager()->IsShownAnnotations());
        GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
        return true;
}

void GNC::CornerAnnotationsTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (AppendsInMenu()) {
                GNC::GUI::wxCornerAnnotationsEventHandler* evtHandler = new GNC::GUI::wxCornerAnnotationsEventHandler(pParent, this);
                pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
        }
}

bool GNC::CornerAnnotationsTool::IsVisible()
{
        return WidgetsContract->GetManager()->IsShownAnnotations();
}
#endif
