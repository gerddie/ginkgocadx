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
#include <api/internationalization/internationalization.h>
#include <wx/valtext.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/busyinfo.h>
#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/image.h>
#include "panelconfiguracionextensiones.h"

#include <prvext/prvext.h>

#include <main/entorno.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorpermisos.h>
#include <resources/ginkgoresourcesmanager.h>

#define COLUMNA_EXTENSION         0
#define COLUMNA_ACTIVATED         1
#define COLUMNA_VERSION           2
#define COLUMNA_FECHA_COMPILACION 3
#define COLUMNA_PROVEEDOR         4
#define COLUMNA_DESCRIPCION       5


inline wxBitmap _wxConvertMemoryToBitmap(const unsigned char* data, int length, wxBitmapType type = wxBITMAP_TYPE_ANY )
{
        wxMemoryInputStream stream( data, length );
        return wxBitmap( wxImage( stream, type, -1), -1 );;
}


namespace GNC
{
namespace GUI
{
class ExtensionPanel: public ExtensionPanelBase
{
public:
        ExtensionPanel(wxWindow* pParent, GADVAPI::IPrivateExtensionWrapper* pExtension, PanelConfiguracionExtensiones* pConfigPanel): ExtensionPanelBase(pParent)
        {
                m_pExtension = pExtension;
                m_pConfigPanel = pConfigPanel;
                m_pThumbnail->SetBackgroundColour(this->GetBackgroundColour());
                m_pLabelTitle->SetLabel(wxString::FromUTF8(pExtension->GetDescription().c_str()));

                if (!pExtension->GetExtension()->GetIcon().IsOk()) {
                        //default icon
                        m_pThumbnail->SetBitmap(GinkgoResourcesManager::Logos::GetExtensionsLogo());
                } else {
                        //load icon
                        m_pThumbnail->SetBitmap(pExtension->GetExtension()->GetIcon());
                }

                Layout();
        }

        ~ExtensionPanel()
        {
        }


        GADVAPI::IPrivateExtensionWrapper* m_pExtension;
        PanelConfiguracionExtensiones* m_pConfigPanel;
};



PanelConfiguracionExtensiones::PanelConfiguracionExtensiones(wxWindow* pParent,IDialogoConfiguracion* pDialogo): PanelConfiguracionExtensionesBase(pParent), IPasoConfiguracion(pDialogo)
{
        m_hasBeenChanged = false;
        Recargar();
}

PanelConfiguracionExtensiones::~PanelConfiguracionExtensiones()
{
}

void PanelConfiguracionExtensiones::Recargar()
{
        GADVAPI::PrivateExtensionManager& mgr = GNC::ControladorExtensiones::Instance()->GetPrivateExtensionsManager();
        //mgr.Reload();

        m_pScrollPanelExtensions->DestroyChildren();
        for (GADVAPI::PrivateExtensionManager::iterator it = mgr.begin(); it != mgr.end(); ++it) {
                ExtensionPanel* pPanel = new ExtensionPanel(m_pScrollPanelExtensions, (*it).second, this);
                m_pScrollPanelExtensions->GetSizer()->Add(pPanel, 0, wxEXPAND| wxALL, 0);
        }
        m_pScrollPanelExtensions->Layout();
}

//region "Metodos de IPasoConfiguracion"
wxWindow* PanelConfiguracionExtensiones::GetPanel()
{
        return this;
}

std::string PanelConfiguracionExtensiones::GetTitle()
{
        return _Std("Extensions");
}

std::string PanelConfiguracionExtensiones::GetCabecera()
{
        return _Std("Extensions settings");
}

bool PanelConfiguracionExtensiones::Validar()
{
        if(m_hasBeenChanged) {
                wxMessageBox(_("Ginkgo CADx must restart for the extensions changes to take effect"),_("Info"));
        }
        return true;
}

bool PanelConfiguracionExtensiones::Guardar()
{
        return true;
}

//endregion

void PanelConfiguracionExtensiones::OnPropiedadCambiada()
{
        m_hasBeenChanged = true;
        IPasoConfiguracion::OnPropiedadCambiada();
}
}
}
