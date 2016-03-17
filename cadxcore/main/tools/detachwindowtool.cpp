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

#include "detachwindowtool.h"
#include <export/contracts/iviewcontract.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorlog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>

//----------------------------------------------------------------------
namespace GNC
{
namespace GUI
{
class wxDetachWindowEventHandler: public wxDefaultEvtHandlerTool
{
public:
        wxDetachWindowEventHandler(wxEvtHandler* pParent, GNC::DetachWindowTool* pTool): wxDefaultEvtHandlerTool(pParent, pTool)
        {
                m_pCornerTool = pTool;
        }

        ~wxDetachWindowEventHandler()
        {
        }

        virtual void OnUpdateUI(wxUpdateUIEvent &event)
        {
                wxDefaultEvtHandlerTool::OnUpdateUI(event);
                if(m_pCornerTool->IsDetached()) {
                        event.SetText(_("Attach View"));
                } else {
                        event.SetText(_("Detach view"));
                }
        }

        virtual void OnToolClick(wxCommandEvent &)
        {
                m_pCornerTool->doDetach();
        }

        GNC::DetachWindowTool* m_pCornerTool;
};

}
}

GNC::GCS::ITool* GNC::DetachWindowTool::NewTool()
{
        return new GNC::DetachWindowTool();
}

GNC::DetachWindowTool::DetachWindowTool()
{
}
GNC::DetachWindowTool::~DetachWindowTool()
{
}

bool GNC::DetachWindowTool::ExecuteAction()
{
        //if i put it here it will be called twice because of toolsystembar
        //GNC::Entorno::Instance()->GetVentanaPrincipal()->EncajarDesencajar(ViewContract->GetView());
        return false;
}

void GNC::DetachWindowTool::doDetach()
{
        GNC::Entorno::Instance()->GetVentanaPrincipal()->EncajarDesencajar(ViewContract->GetView());
}

void GNC::DetachWindowTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
        if (AppendsInMenu()) {
                GNC::GUI::wxDetachWindowEventHandler* evtHandler = new GNC::GUI::wxDetachWindowEventHandler(pParent, this);
                pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
        }
}

bool GNC::DetachWindowTool::IsDetached()
{
        //si el padre de la vistapadre de la vista es un dialogo esta desencajada si no no

        if(ViewContract->GetView() != NULL) {
                wxWindow* pVentana = ViewContract->GetView()->GetWindow();
                if(pVentana != NULL) {
                        wxWindow* pVentanaPadre = pVentana->GetParent();
                        if(pVentanaPadre != NULL) {
                                wxFrame* pdlg = dynamic_cast<wxFrame*>(pVentanaPadre);
                                if(pdlg != NULL) {
                                        return true;
                                } else {
                                        return false;
                                }
                        }
                } else {
                        LOG_WARN("Core/HerramientaDesencajar", "Error al obtener la ventana asociada a la vista para comprobar encaje. Incosistencia interna");
                }
        }
        return false;
}


