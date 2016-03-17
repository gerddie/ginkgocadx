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

#include "ventanacontrolhl7.h"
#include <api/imodelohl7.h>

#include <main/controllers/controladorenviohl7.h>
#include <main/controllers/controladorbbddhl7.h>
#include <main/controllers/configurationcontroller.h>

#include <resources/ginkgoresourcesmanager.h>
#define LOGGER "VentanaControlHL7"
#include <main/controllers/controladorlog.h>

#include <main/entorno.h>
#include <wx/log.h>
#include <wx/msgdlg.h>
#include <set>

#define COL_FECHA           0
#define COL_DESTINO         1
#define COL_ESTADO          2
#define COL_MENSAJE_ERROR   3
#define COL_MENSAJE         4
#define COL_ID              5

namespace GNC
{
namespace GUI
{
/////////////////
class DialogoMostrarMensaje: public GNC::GUI::DialogoMostrarMensajeBase
{
public:
        DialogoMostrarMensaje(wxWindow* pParent, const wxString& fecha, const wxString& url, const wxString& estado, const wxString& mensaje, const bool editar):DialogoMostrarMensajeBase(pParent)
        {
                m_pFecha->SetValue(fecha);
                m_pDestino->SetValue(url);
                m_pEstado->SetValue(estado);
                m_pContenido->SetValue(mensaje);
                if(editar) {
                        m_pDestino->SetEditable(true);
                        m_pContenido->SetEditable(true);
                        m_pFecha->Disable();
                        m_pEstado->Show(false);
                        m_pChoiceEstado->Show(true);
                        m_pChoiceEstado->Select(m_pChoiceEstado->FindString(estado));
                } else {
                        m_CancelButton->Hide();
                }
        }
        ~DialogoMostrarMensaje()
        {
        }

        std::string GetMensaje()
        {
                std::string mensaje(m_pContenido->GetValue().ToUTF8());
                return mensaje;
        }

        std::string GetDestino()
        {
                std::string destino(m_pDestino->GetValue().ToUTF8());
                return destino;
        }

        GIL::HL7::MensajeHL7::TEstado GetEstado()
        {
                switch(m_pChoiceEstado->GetSelection()) {
                case 0:
                        return GIL::HL7::MensajeHL7::TE_Enviado;
                case 1:
                        return GIL::HL7::MensajeHL7::TE_Pendiente;
                case 2:
                        return GIL::HL7::MensajeHL7::TE_Error;
                case 3:
                        return GIL::HL7::MensajeHL7::TE_Warning;
                }
                return GIL::HL7::MensajeHL7::TE_Error;
        }

        virtual void OnKeyDown( wxKeyEvent& event )
        {
                if (event.GetKeyCode() == WXK_ESCAPE) {
                        Close();
                }
                event.Skip();
        }

};
///////////////////
class TimerControlHL7: public wxTimer
{
public:
        TimerControlHL7(VentanaControlHL7* pVentana)
        {
                m_pVentana = pVentana;
        }
        ~TimerControlHL7()
        {
                m_pVentana = NULL;
        }
        virtual void Notify()
        {
                m_pVentana->Refrescar();
        }
        VentanaControlHL7* m_pVentana;
};
///////////////////

VentanaControlHL7* VentanaControlHL7::m_pInstance = NULL;

VentanaControlHL7* VentanaControlHL7::Instance()
{
        if(m_pInstance == NULL) {
                m_pInstance = new VentanaControlHL7();
        }
        return m_pInstance;
}

void VentanaControlHL7::CerrarSiAbierta()
{
        if(m_pInstance != NULL) {
                m_pInstance->Close(true);
        }
}

void VentanaControlHL7::Refrescar()
{
        ActualizarEstadoControlador();
        RefrescarMensajes();
}


VentanaControlHL7::VentanaControlHL7():VentanaControlHL7Base(NULL)
{
        Freeze();
        wxIcon icono;
        icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
        this->SetIcon(icono);


        // column 0 of the view control:
        m_pListViewCtrl->AppendColumn( new wxDataViewColumn( _("Date"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), COL_FECHA, 138, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
        m_pListViewCtrl->AppendColumn( new wxDataViewColumn( _("Destination"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), COL_DESTINO, 70, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
        m_pListViewCtrl->AppendColumn( new wxDataViewColumn( _("Status"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), COL_ESTADO, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
        m_pListViewCtrl->AppendColumn( new wxDataViewColumn( _("Error message"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), COL_MENSAJE_ERROR, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
        m_pListViewCtrl->AppendColumn( new wxDataViewColumn( _("Date Time"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), COL_FECHA, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));


        m_pLabelEnvio->SetLabel(wxString::FromUTF8("Proceso de envío:"));

        m_pTimerRefresh = new TimerControlHL7(this);

        ActualizarEstadoControlador();
        RefrescarMensajes();

        m_pListViewCtrl->Connect(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler( VentanaControlHL7::OnMensajeMenu ), NULL, this );
        m_pListViewCtrl->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( VentanaControlHL7::OnMensajeDClick ), NULL, this );
        m_pListViewCtrl->Connect(wxEVT_CHAR, wxKeyEventHandler( VentanaControlHL7::OnKeyDown ), NULL, this );

        this->Connect( m_menuItemPurgarMensajes->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnPurgarMensajes ) );
        this->Connect( m_menuItemSalir->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnMenuSalir ) );
        this->Connect( m_menuItemActualizar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnActualizar ) );

        Show();
        Layout();
        SetFocus();
        Thaw();
}

VentanaControlHL7::~VentanaControlHL7()
{
        m_pInstance = NULL;
        if(m_pTimerRefresh != NULL) {
                delete m_pTimerRefresh;
                m_pTimerRefresh = NULL;
        }
}

void VentanaControlHL7::OnActivate(wxActivateEvent &event)
{
        if(event.GetActive()) {
                m_pTimerRefresh->Start(5000);
        } else {
                m_pTimerRefresh->Stop();
        }
}

void VentanaControlHL7::OnMensajeMenu(wxDataViewEvent& )
{
        //si hay un mensaje seleccionado..
        wxDataViewItemArray seleccionados;
        m_pListViewCtrl->GetSelections(seleccionados);
        if(seleccionados.size() > 0) {
                wxMenu* popup = new wxMenu();
                wxMenuItem* pMenuMostrarVentana = new wxMenuItem(popup,wxID_ANY,_("Show Message"));
                wxMenuItem* pMenuEditarVentana = new wxMenuItem(popup,wxID_ANY,_("Edit Message"));
                wxMenuItem* pMenuEliminarMensaje = new wxMenuItem(popup,wxID_ANY,_("Delete message"));
#ifdef __WXMSW__
                pMenuMostrarVentana->SetBitmaps( GinkgoResourcesManager::MenuIcons::GetIcoEditor());
                pMenuEliminarMensaje->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoEliminarWidget());
#else
                pMenuMostrarVentana->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEditor());
                pMenuEliminarMensaje->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEliminarWidget());
#endif
                popup->Append(pMenuMostrarVentana);
                popup->Append(pMenuEditarVentana);
                popup->Append(pMenuEliminarMensaje);
                popup->Connect(pMenuMostrarVentana->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnMostrarMensaje ), NULL, this  );
                popup->Connect(pMenuEditarVentana->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnEditarMensaje ), NULL, this  );
                popup->Connect(pMenuEliminarMensaje->GetId(),wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( VentanaControlHL7::OnEliminarMensaje ), NULL, this  );
                this->PopupMenu(popup);
                delete popup;
        }
}
void VentanaControlHL7::OnMensajeDClick(wxDataViewEvent& )
{
        MostrarVentanaMensaje();
}

void VentanaControlHL7::OnMostrarMensaje(wxCommandEvent &)
{
        MostrarVentanaMensaje();
}

void VentanaControlHL7::OnEditarMensaje(wxCommandEvent &)
{
        wxDataViewItem id = m_pListViewCtrl->GetSelection();
        if(id.IsOk()) {
                wxVariant value;
                m_pListModel->GetValue(value, id, COL_FECHA);
                wxString fecha = value;
                m_pListModel->GetValue(value, id, COL_DESTINO);
                wxString destino = value;
                m_pListModel->GetValue(value, id, COL_ESTADO);
                wxString estado = value;
                m_pListModel->GetValue(value, id, COL_MENSAJE);
                wxString mensaje = value;
                m_pListModel->GetValue(value, id, COL_ID);
                wxString str = value;
                long idMensaje;
                if(str.ToLong(&idMensaje)) {
                        DialogoMostrarMensaje mostrar(this,fecha,destino,estado,mensaje, true);
                        if(mostrar.ShowModal() == wxID_OK) {
                                try {
                                        GIL::HL7::ControladorBBDDHl7::Instance()->ActualizarMensaje(idMensaje, mostrar.GetDestino(), mostrar.GetMensaje(), mostrar.GetEstado());
                                        RefrescarMensajes();
                                } catch (GIL::HL7::HL7Exception& ex) {
                                        wxMessageBox(wxString::FromUTF8(ex.GetCause().c_str()), _("Error editing message"), wxICON_ERROR, this);
                                }
                        }
                }
        }
}

void VentanaControlHL7::OnListaKey(wxKeyEvent &event)
{
        int keycode = event.GetKeyCode();
        switch (keycode) {
        case WXK_DELETE:
        case WXK_CLEAR:
        case WXK_BACK:
                EliminarMensaje();
                event.Skip(false);
                break;
        case WXK_RETURN:
                MostrarVentanaMensaje();
                event.Skip(false);
                break;
        case WXK_ESCAPE:
                Close();
                event.Skip(false);
                break;
        case WXK_TAB:
                SetFocus();
                event.Skip(false);
                break;
        default:
                event.Skip(true);
        }
}

void VentanaControlHL7::OnEliminarMensaje(wxCommandEvent &)
{
        EliminarMensaje();
}


void VentanaControlHL7::OnPurgarMensajes(wxCommandEvent& )
{
        try {
                GIL::HL7::ControladorBBDDHl7::Instance()->PurgarMensajesEnviados();
        } catch (GIL::HL7::HL7Exception& ex) {
                wxMessageBox(wxString::FromUTF8(ex.GetCause().c_str()), _("Error purguing messages"), wxICON_ERROR, this);
        }
        RefrescarMensajes();
}

void VentanaControlHL7::OnActualizar(wxCommandEvent &)
{
        ActualizarEstadoControlador();
        RefrescarMensajes();
}

void VentanaControlHL7::OnArrancarPararClick(wxCommandEvent &)
{
        m_pBArrancarParar->Disable();
        if (m_pBArrancarParar->GetLabel() == _("Stop") ) {
                GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/HCE", "HL7MonitorEnabled", false);
                GIL::HL7::ControladorEnvioHl7::FreeInstance();
        } else {
                GNC::GCS::ConfigurationController::Instance()->writeBoolGeneral("/GinkgoCore/HCE", "HL7MonitorEnabled", true);
                GIL::HL7::ControladorEnvioHl7::Arrancar();
        }
        ActualizarEstadoControlador();
        m_pBArrancarParar->Enable();
}

void VentanaControlHL7::RefrescarMensajes()
{
        m_pTimerRefresh->Start(5000);
        wxWindowDisabler disabler;
        Freeze();
        try {
                //conservamos los ids seleccionados
                wxDataViewItemArray seleccionados;
                m_pListViewCtrl->GetSelections(seleccionados);
                std::set<int> listaIds;
                for(wxDataViewItemArray::iterator it =seleccionados.begin(); it!= seleccionados.end(); ++it) {
                        wxVariant value;
                        m_pListModel->GetValue(value, (*it), COL_ID);
                        long id = value.GetLong();
                        listaIds.insert((int)id);
                }
                //

                typedef std::list<GIL::HL7::MensajeHL7> TListaMensajes;
                TListaMensajes listaMensajes;
                GIL::HL7::ControladorBBDDHl7::Instance()->GetMensajesHL7(listaMensajes);
                //primero vaciamos el arbol
                m_pListViewCtrl->DeleteAllItems();
                wxVector<wxVariant> data;
                for(TListaMensajes::iterator it = listaMensajes.begin(); it != listaMensajes.end(); ++it) {
                        data.clear();
                        data.push_back(wxString::FromUTF8((*it).m_fecha.c_str()));
                        data.push_back(wxString::FromUTF8((*it).m_destino.c_str()));
                        switch((*it).m_estado) {
                        case GIL::HL7::MensajeHL7::TE_Enviado:
                                data.push_back(_("Sent"));
                                data.push_back(wxEmptyString);
                                break;
                        case GIL::HL7::MensajeHL7::TE_Pendiente:
                                data.push_back(_("Pending"));
                                data.push_back(wxEmptyString);
                                break;
                        case GIL::HL7::MensajeHL7::TE_Warning:
                                data.push_back(_("Warning"));
                                data.push_back(wxString::FromUTF8((*it).m_mensajeError.c_str()));
                                break;
                        case GIL::HL7::MensajeHL7::TE_Error:
                                data.push_back(_("Error"));
                                data.push_back(wxString::FromUTF8((*it).m_mensajeError.c_str()));
                                break;
                        default:
                                LOG_DEBUG("HL7", "VentanaControlHL7::RefrescarMensajes: unknown message state");
                        }
                        data.push_back(wxString::FromUTF8((*it).m_mensaje.c_str()));
                        data.push_back(wxString::Format(wxT("%d"), (*it).m_id));
                        m_pListModel->AppendItem(data);

                        if(listaIds.find((*it).m_id) != listaIds.end()) {
                                m_pListViewCtrl->SelectRow(m_pListModel->GetItemCount()-1);
                        }
                }
        } catch (GIL::HL7::HL7Exception& ex) {
                Thaw();
                wxMessageBox(wxString::FromUTF8(ex.GetCause().c_str()), _("Error getting message"), wxICON_ERROR, this);
                return;
        }
        Thaw();
}

void VentanaControlHL7::ActualizarEstadoControlador()
{
        m_pTimerRefresh->Start(5000);
        if(GIL::HL7::ControladorEnvioHl7::EstaArrancado()) {
                m_pEstado->SetLabel(_("Running"));
                m_pEstado->SetToolTip(_("The submission process is started and ready to send"));
                m_pEstado->SetForegroundColour(wxColour(0,174,0));
                m_pBArrancarParar->SetLabel(_("Stop"));
        } else {
                m_pEstado->SetLabel(_("Stopped"));
                m_pEstado->SetToolTip(_("Usually due to an error in sending a message"));
                m_pEstado->SetForegroundColour(wxColour(174,0,0));
                m_pBArrancarParar->SetLabel(_("Start"));
        }
        m_panelEstado->Layout();
}

void VentanaControlHL7::MostrarVentanaMensaje()
{
        wxDataViewItem id = m_pListViewCtrl->GetSelection();
        if(id.IsOk()) {
                wxVariant value;
                m_pListModel->GetValue(value, id, COL_FECHA);
                wxString fecha = value;
                m_pListModel->GetValue(value, id, COL_DESTINO);
                wxString destino = value;
                m_pListModel->GetValue(value, id, COL_ESTADO);
                wxString estado = value;
                m_pListModel->GetValue(value, id, COL_MENSAJE);
                wxString mensaje = value;
                DialogoMostrarMensaje* mostrar = new DialogoMostrarMensaje(this,fecha,destino,estado,mensaje, false);
                mostrar->Show();
        }
}

void VentanaControlHL7::EliminarMensaje()
{
        //pillamos los elementos seleccionados
        wxDataViewItemArray seleccionados;
        m_pListViewCtrl->GetSelections(seleccionados);
        std::list<int> listaIds;

        for(wxDataViewItemArray::iterator it =seleccionados.begin(); it!= seleccionados.end(); ++it) {
                wxVariant value;
                m_pListModel->GetValue(value, (*it), COL_ID);
                listaIds.push_back(value.GetLong());
        }

        if(listaIds.empty()) {
                return;
        }

        int answer = wxMessageBox(_("Would you like to remove these messages?"), _("Delete"), wxYES_NO, this);
        if (answer == wxYES) {
                for(std::list<int>::iterator it = listaIds.begin(); it != listaIds.end(); ++it) {
                        try {
                                GIL::HL7::ControladorBBDDHl7::Instance()->EliminarMensaje((*it));
                        } catch (GIL::HL7::HL7Exception& ex) {
                                wxMessageBox(wxString::FromUTF8(ex.GetCause().c_str()), _("Error deleting messages"), wxICON_ERROR, this);
                        }
                }
                RefrescarMensajes();
        }
}

void VentanaControlHL7::OnMenuSalir( wxCommandEvent& /*event*/ )
{
        Close();
}

void VentanaControlHL7::OnKeyDown( wxKeyEvent& event )
{
        switch (event.GetKeyCode()) {
        case WXK_DELETE:
        case WXK_CLEAR:
        case WXK_BACK:
                EliminarMensaje();
                event.Skip(false);
                break;
        case WXK_RETURN: {
                wxCommandEvent evt;
                OnArrancarPararClick(evt);
                event.Skip(false);
        }
        break;
        case WXK_ESCAPE:
                Close();
                event.Skip(false);
                break;
        case WXK_TAB:
                m_pListViewCtrl->SetFocus();
                event.Skip(false);
                break;
        default:
                event.Skip(true);
        }
}
}
}
