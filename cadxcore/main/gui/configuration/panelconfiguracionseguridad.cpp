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

#include <vector>
#include <sstream>
#include <limits>

#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/mstream.h>
#include <wx/imaglist.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/busyinfo.h>
#include <wx/log.h>

#include <wx/tokenzr.h>

#include "panelconfiguracionseguridad.h"

#include <sstream>
#include <resources/ginkgoresourcesmanager.h>

#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorautenticacion.h>
#include <main/entorno.h>
#include <api/internationalization/internationalization.h>

namespace GNC
{
namespace GUI
{
class UserDialog: public UserDialogBase
{
public:
        //nuevo usuario
        UserDialog(wxWindow* pParent, GNC::GCS::Ptr<wxSQLite3Database> pConexion): UserDialogBase(pParent)
        {
                SetTitle(_("New User"));
                m_pHeader->SetToolTip(_("New User"));
                m_pFormPanel->SetToolTip(_("New User"));
                m_pSesionControlador = pConexion;
                m_pTCUsuario->SetFocus();
        }

        //cambiar password
        UserDialog(wxWindow* pParent, const wxString& nombreUsuario, GNC::GCS::Ptr<wxSQLite3Database> pConexion): UserDialogBase(pParent)
        {
                m_pTCUsuario->Enable(false);
                m_pTCUsuario->SetValue(nombreUsuario);
                m_pSesionControlador = pConexion;
                m_pTCPassword0->SetFocus();
        }

        void HideUserName()
        {
                m_pTCUsuario->Show(false);
                m_pLabelUser->Show(false);
                m_pFormPanel->Layout();
        }

        std::string GetPassword()
        {
                return std::string(m_pTCPassword0->GetValue().ToUTF8());
        }

        std::string GetUser()
        {
                return std::string(m_pTCUsuario->GetValue().ToUTF8());
        }

        virtual void OnOkClick(wxCommandEvent& )
        {
                //TODO hacer comprobaciones mejores sobre el password...

                if (m_pTCUsuario->IsEnabled() && m_pTCUsuario->IsShown()) {
                        if (m_pTCUsuario->GetValue().size() < 4) {
                                wxMessageBox(_("User name has to contain at least 4 characters"),_("Info"), wxOK | wxICON_INFORMATION, this);
                                m_pTCUsuario->SelectAll();
                                return;
                        }

                        std::string user(m_pTCUsuario->GetValue().ToUTF8());
                        if (GSEC::Auth::ControladorAutenticacion::Instance()->ExisteUsuario(user, m_pSesionControlador)) {
                                wxMessageBox(_("User name repeated"),_("Info"), wxOK | wxICON_INFORMATION, this);
                                m_pTCUsuario->SelectAll();
                                return;
                        }
                }

                if (m_pTCPassword0->GetValue() != m_pTCPassword1->GetValue()) {
                        wxMessageBox(_("Password and confirm password can't be different."),_("Info"), wxOK | wxICON_INFORMATION, this);
                        m_pTCPassword1->SetValue(wxEmptyString);
                        m_pTCPassword0->SetFocus();
                        m_pTCPassword0->SelectAll();
                        return;
                }

                if (m_pTCPassword0->GetValue().size() < 6) {
                        wxMessageBox(_("Password has to contain at least 6 characters"),_("Info"), wxOK | wxICON_INFORMATION, this);
                        m_pTCPassword0->SetFocus();
                        m_pTCPassword0->SelectAll();
                        return;
                }

                EndModal(wxID_OK);
        }

        GNC::GCS::Ptr<wxSQLite3Database> m_pSesionControlador;

};



PanelConfiguracionSeguridad::PanelConfiguracionSeguridad(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionSeguridadBase(pParent), IPasoConfiguracion(pDialogo)
{
        m_pSesionControlador = GSEC::Auth::ControladorAutenticacion::Instance()->IniciarSesionModificacionBBDDSeguridad();

        m_pImageList = new wxImageList(16,16,true);
        m_pImageList->Add(GinkgoResourcesManager::MenuIcons::GetIcoUser());
        m_pListaUsuarios->SetImageList(m_pImageList,wxIMAGE_LIST_SMALL);

        //se corresponden con TipoAutenticacion de controlador autenticacion
        m_pSecType->Append(_("No login"));
        m_pSecType->Append(_("User/Password"));
        m_pSecType->Append(_("Password"));
        m_pSecType->Select(GSEC::Auth::ControladorAutenticacion::Instance()->GetTipoAutenticacion());

        Recargar();
}

PanelConfiguracionSeguridad::~PanelConfiguracionSeguridad()
{
        delete m_pImageList;
        m_pImageList = NULL;
        GSEC::Auth::ControladorAutenticacion::Instance()->CancelarCambiosYCerrarBBDDSeguridad(m_pSesionControlador);
}

void PanelConfiguracionSeguridad::Recargar()
{
        wxWindowDisabler dis;
        Freeze();

        m_pPanelUsuarios->Hide();
        m_pPanelPassword->Hide();

        int seleccion = m_pSecType->GetSelection();
        switch(seleccion) {
        case GSEC::Auth::TA_USER_PASSWORD: {
                //se muestra el panel user/password
                m_pPanelUsuarios->Show(true);

                //se rellenan los usuarios
                m_pListaUsuarios->ClearAll();

                std::list<std::string> listaUsuarios;
                GSEC::Auth::ControladorAutenticacion::Instance()->GetAllUsers(listaUsuarios,m_pSesionControlador);
                long index = 0;
                for (std::list<std::string>::iterator it = listaUsuarios.begin(); it != listaUsuarios.end(); ++it) {
                        m_pListaUsuarios->InsertItem(index++, wxString::FromUTF8((*it).c_str()),0);
                }
        }
        break;
        case GSEC::Auth::TA_PASSWORD: {
                //se muestra el panel
                m_pPanelPassword->Show(true);

                //se mira a ver si el password esta establecido
                if(GSEC::Auth::ControladorAutenticacion::Instance()->IsPasswordGeneralSetted(m_pSesionControlador)) {
                        m_pLabelPassword->SetLabel(_("Password is setted"));
                        m_pButtonSetPassword->SetLabel(_("Change password"));
                } else {
                        m_pLabelPassword->SetLabel(_("Password is not setted"));
                        m_pButtonSetPassword->SetLabel(_("Set password"));
                }
                m_pPanelPassword->Layout();
        }
        break;
        case GSEC::Auth::TA_NO_LOGIN:
        default: {
                //no se hace nada
        }
        }

        Layout();
        Thaw();
}

//region "Metodos de IPasoConfiguracion"
wxWindow* PanelConfiguracionSeguridad::GetPanel()
{
        return this;
}

std::string PanelConfiguracionSeguridad::GetTitle()
{
        return _Std("Security");
}

std::string PanelConfiguracionSeguridad::GetCabecera()
{
        return _Std("Security Setting");
}

bool PanelConfiguracionSeguridad::Validar()
{
        bool ok = true;

        int seleccion = m_pSecType->GetSelection();
        switch(seleccion) {
        case GSEC::Auth::TA_USER_PASSWORD: {
                std::list<std::string> listaUsuarios;
                GSEC::Auth::ControladorAutenticacion::Instance()->GetAllUsers(listaUsuarios,m_pSesionControlador);
                if(listaUsuarios.empty()) {
                        wxMessageBox(_("You must create at least one user"), _("Info"));
                        ok = false;
                }
        }
        break;
        case GSEC::Auth::TA_PASSWORD: {
                std::list<std::string> listaUsuarios;
                if(!GSEC::Auth::ControladorAutenticacion::Instance()->IsPasswordGeneralSetted(m_pSesionControlador)) {
                        wxMessageBox(_("You must set the main password"), _("Info"));
                        ok = false;
                }
        }
        break;
        case GSEC::Auth::TA_NO_LOGIN:
        default: {
                //no se hace nada
        }
        }
        return ok;
}

bool PanelConfiguracionSeguridad::Guardar()
{
        //guardar el tipo de seguridad utilizado
        GSEC::Auth::ControladorAutenticacion::Instance()->SetTipoAutenticacion((GSEC::Auth::TipoAutenticacion) m_pSecType->GetSelection());
        GSEC::Auth::ControladorAutenticacion::Instance()->GuardarCambiosBBDDSeguridad(m_pSesionControlador);
        return true;
}

//endregion

void PanelConfiguracionSeguridad::OnSecTypeChoice(wxCommandEvent &/*event*/)
{
        OnPropiedadCambiada();
        Recargar();
}

void PanelConfiguracionSeguridad::OnListaUsuariosItemSelected(wxListEvent &)
{
        m_pBDelete->Enable(true);
        m_pBCambiarPassword->Enable(true);
}

void PanelConfiguracionSeguridad::OnListaUsuariosItemDeSelected(wxListEvent &)
{
        m_pBDelete->Enable(false);
        m_pBCambiarPassword->Enable(false);
}

void PanelConfiguracionSeguridad::OnNuevoClick(wxCommandEvent &)
{
        OnPropiedadCambiada();
        UserDialog dlg(this,m_pSesionControlador);
        int returnCode = dlg.ShowModal();
        if (returnCode == wxID_OK) {
                //se inserta en la bbdd
                if (GSEC::Auth::ControladorAutenticacion::Instance()->InsertarUsuario(dlg.GetUser(), dlg.GetPassword(), m_pSesionControlador)) {
                        //se inserta el la lista
                        wxMessageBox(_("User inserted successfully"),_("Info"), wxOK | wxICON_INFORMATION, this);
                        m_pListaUsuarios->InsertItem(m_pListaUsuarios->GetItemCount(), wxString::FromUTF8(dlg.GetUser().c_str()),0);
                } else {
                        wxMessageBox(_("There was an error inserting user. To get more info open the LOG."),_("Info"), wxOK | wxICON_INFORMATION, this);
                }
        }
}

void PanelConfiguracionSeguridad::OnCambiarPasswordClick(wxCommandEvent &)
{
        OnPropiedadCambiada();
        if (m_pListaUsuarios->GetSelectedItemCount() == 0)
                return;

        //se busca el item seleccionado


        long item = m_pListaUsuarios->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

        if( item != -1) {
                wxString nombreUsuario = m_pListaUsuarios->GetItemText(item);

                UserDialog dlg(this, nombreUsuario, m_pSesionControlador);
                int returnCode = dlg.ShowModal();
                if (returnCode == wxID_OK) {
                        //se inserta en la bbdd
                        if (GSEC::Auth::ControladorAutenticacion::Instance()->CambiarPassword(dlg.GetUser(), dlg.GetPassword(),m_pSesionControlador)) {
                                //se inserta el la lista
                                wxMessageBox(_("Password changed successfully"),_("Info"), wxOK | wxICON_INFORMATION, this);
                        } else {
                                wxMessageBox(_("There was an error changing the password. To get more info open the LOG."),_("Info"), wxOK | wxICON_INFORMATION, this);
                        }
                }
        }
}

void PanelConfiguracionSeguridad::OnEliminarClick(wxCommandEvent &)
{
        if (m_pListaUsuarios->GetItemCount() > 1 ) {
                OnPropiedadCambiada();
                long item = m_pListaUsuarios->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);

                if( item != -1) {
                        std::string nombreUsuario(m_pListaUsuarios->GetItemText(item).ToUTF8());
                        if (GSEC::Auth::ControladorAutenticacion::Instance()->EliminarUsuario(nombreUsuario, m_pSesionControlador)) {
                                //se inserta el la lista
                                wxMessageBox(_("User deleted successfully"),_("Info"), wxOK | wxICON_INFORMATION, this);
                                m_pListaUsuarios->DeleteItem(item);
                        } else {
                                wxMessageBox(_("There was an error deleting user. To get more info open the LOG."),_("Info"), wxOK | wxICON_INFORMATION, this);
                        }
                }
        } else {
                wxMessageBox(_("Ginkgo CADx must have at least one user."),_("Info"), wxOK | wxICON_INFORMATION, this);
        }
}

void PanelConfiguracionSeguridad::OnSetMainPassword(wxCommandEvent &)
{
        OnPropiedadCambiada();
        UserDialog dlg(this, _("User"), m_pSesionControlador);
        dlg.HideUserName();
        int returnCode = dlg.ShowModal();
        if (returnCode == wxID_OK) {
                //se inserta en la bbdd
                if (GSEC::Auth::ControladorAutenticacion::Instance()->SetPasswordGeneral(dlg.GetPassword(), m_pSesionControlador)) {
                        //se inserta el la lista
                        wxMessageBox(_("Password changed successfully"),_("Info"), wxOK | wxICON_INFORMATION, this);
                        Recargar();
                } else {
                        wxMessageBox(_("There was an error changing the password. To get more info open the LOG."),_("Info"), wxOK | wxICON_INFORMATION, this);
                }
        }
}
}
}
