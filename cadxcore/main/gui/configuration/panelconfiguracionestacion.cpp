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

#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <sstream>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/xml/xml.h>

#include <wx/ginkgostyle/ginkgostyle.h>

#include "panelconfiguracionestacion.h"

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/commandcontroller.h>
#include <main/entorno.h>
#include <api/controllers/icontroladorinternacionalizacion.h>
#include <api/internationalization/internationalization.h>

namespace GNC
{
namespace GUI
{

PanelConfiguracionEstacion::PanelConfiguracionEstacion(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionEstacionBase(pParent), IPasoConfiguracion(pDialogo)
{
        Recargar();
        Layout();
}

PanelConfiguracionEstacion::~PanelConfiguracionEstacion()
{

}

void PanelConfiguracionEstacion::Recargar()
{
        std::string strTmp;
        GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Estacion","NombreMedico",strTmp);
        m_pMedicoNombre->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Estacion","IdMedico",strTmp);
        m_pMedicoId->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","NombreServicio",strTmp);
        m_pServicioNombre->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","IdServicio",strTmp);
        m_pServicioId->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","CentroNombre",strTmp);
        m_pCentroNombre->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","CentroId",strTmp);
        m_pCentroId->SetValue(wxString::FromUTF8(strTmp.c_str()));

        GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","DefaultCharset",strTmp,"ISO_IR 100");
        int selection = m_pComboCharset->FindString(wxString::FromUTF8(strTmp.c_str()));
        m_pComboCharset->SetSelection((selection>0)?selection:0);
}

//region "Metodos de IPasoConfiguracion"
wxWindow* PanelConfiguracionEstacion::GetPanel()
{
        return this;
}

std::string PanelConfiguracionEstacion::GetTitle()
{
        return _Std("Workstation");
}

std::string PanelConfiguracionEstacion::GetCabecera()
{
        return _Std("Configuration parameters of the local station");
}

bool PanelConfiguracionEstacion::Validar()
{
        return true;
}

bool PanelConfiguracionEstacion::Guardar()
{
        GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Estacion","NombreMedico",std::string(m_pMedicoNombre->GetValue().ToUTF8()));
        GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Estacion","IdMedico",std::string(m_pMedicoId->GetValue().ToUTF8()));

        GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/Estacion","IdServicio",std::string(m_pServicioId->GetValue().ToUTF8()));
        GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/Estacion","NombreServicio",std::string(m_pServicioNombre->GetValue().ToUTF8()));
        GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/Estacion","CentroNombre",std::string(m_pCentroNombre->GetValue().ToUTF8()));
        GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/Estacion","CentroId",std::string(m_pCentroId->GetValue().ToUTF8()));
        GNC::GCS::ConfigurationController::Instance()->writeStringGeneral("/GinkgoCore/Estacion","DefaultCharset",std::string(m_pComboCharset->GetValue().ToUTF8()));

        return true;
}
//endregion


void PanelConfiguracionEstacion::OnTextoCambiado(wxCommandEvent &)
{
        if(IsShown()) {
                OnPropiedadCambiada();
        }
}
}
}
