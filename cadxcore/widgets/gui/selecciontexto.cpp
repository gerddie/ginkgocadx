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

#include <cmath>
#include <wx/icon.h>
#include "selecciontexto.h"
#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>
#include <wx/xml/xml.h>

GNC::GCS::Widgets::Dialogos::SeleccionTexto::SeleccionTexto(wxWindow* parent, TWidget* pWidget, TManager* pManager) : SeleccionTextoBase(parent)
{
        wxIcon icono;
        icono.CopyFromBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEditar());
        this->SetIcon(icono);

        m_pWidget = pWidget;
        m_pManager = pManager;
        m_pControlTexto->SetValue(wxString::FromUTF8(m_pWidget->m_Texto.c_str()));
        m_pControlTexto->SetSelection(-1,-1);
}

GNC::GCS::Widgets::Dialogos::SeleccionTexto::~SeleccionTexto()
{
        if (m_pWidget != NULL && m_pWidget->m_pWidgetSeleccion == this) {
                m_pWidget->m_pWidgetSeleccion= NULL;
        }
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnClickEnMenu(wxCommandEvent& /*event*/)
{
        Center();
        ShowModal();
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnDialogoKeyDown( wxKeyEvent& event )
{
        if (event.GetKeyCode() == WXK_ESCAPE) {
                Close();
        } else if (event.GetKeyCode() == WXK_RETURN) {
                Actualizar();
                Close();
        } else {
                event.Skip();
        }
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnControlTextoKeyDown( wxKeyEvent& event )
{
        if (event.GetKeyCode() == WXK_ESCAPE) {
                Close();
        } else {
                event.Skip();
        }
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnControlTextoTextoActualizado( wxCommandEvent& event )
{
        event.Skip();
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnBotonCancelarClick( wxCommandEvent& /*event*/ )
{
        Close();
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnBotonAceptarClick( wxCommandEvent& /*event*/ )
{
        Actualizar();
        Close();
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnActivar(wxActivateEvent &/*event*/)
{
        m_pControlTexto->SetFocus();
}

void GNC::GCS::Widgets::Dialogos::SeleccionTexto::Actualizar(bool actualizarWidget)
{


        if (actualizarWidget) {

                bool modifica = false;

                std::string textoControl(m_pControlTexto->GetValue().ToUTF8());

                if (m_pWidget->GetTexto() != textoControl) {
                        m_pWidget->SetTexto(textoControl);
                        modifica = true;
                }

                if (modifica) {
                        m_pWidget->Invalidar();
                        m_pWidget->PropagarModificacion();
                }
        }

}

