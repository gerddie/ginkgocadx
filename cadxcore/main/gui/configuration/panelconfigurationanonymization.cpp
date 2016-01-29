/*
 *  
 *  $Id: panelconfigurationanonymization.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include <vector>
#include <sstream>
#include <limits>

#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/valtext.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/mstream.h>
#include <wx/dirdlg.h>

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/xml/xml.h>
#include <wx/log.h>

#include <wx/tokenzr.h>

#include "panelconfigurationanonymization.h"

#include <sstream>
#include <api/imodelointegracion.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/anonymizationcontroller.h>
#include <main/entorno.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>

namespace GNC {
	namespace GUI {

		PanelConfigurationAnonymization::PanelConfigurationAnonymization(wxWindow* pParent, IDialogoConfiguracion* pDialogo): PanelConfigurationAnonymizationBase(pParent), IPasoConfiguracion(pDialogo)
		{
			GNC::GCS::AnonymizationController::TAnonymizationMap map = GNC::GCS::AnonymizationController::GetAnonymizationMap();
			/*
			m_pTagsDicom->SetColumnCount(2);
			m_pTagsDicom->MakeColumnEditable(1, true);
			m_pTagsDicom->SetColumnProportion(0, 3);
			m_gridPermisos->SetColumnProportion(1, 1);
			for(GNC::GCS::AnonymizationController::TAnonymizationMap::const_iterator it = map.begin(); it != map.end(); ++it)
			{
				if(!(*it).second.privado) {
					if ((*it).second.espacioNombres != ultimaCategoria) {
						ultimaCategoria = (*it).second.espacioNombres;
						cat = m_gridPermisos->Append(new wxPropertyCategory(wxString::FromUTF8((*it).second.descripcionNamespace.c_str()),wxString::FromUTF8(ultimaCategoria.c_str())));
					}
					std::string clave = (*it).second.espacioNombres + SEPARADOR + (*it).second.id;
					wxPGProperty* prop = cat->AppendChild(new wxBoolProperty(wxString::FromUTF8((*it).second.descripcion.c_str()), wxString::FromUTF8(clave.c_str()), (*it).second.activoPorDefecto ) );
					prop->SetCell(2, new wxPGCell( wxString::FromUTF8( ((*it).second.valorPorDefecto).c_str())) );
					prop->SetCell(3, new wxPGCell( _("Default value") ));
				}
			}
			*/
		}

		PanelConfigurationAnonymization::~PanelConfigurationAnonymization()
		{
			
		}

		void PanelConfigurationAnonymization::Recargar()
		{
			
		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* PanelConfigurationAnonymization::GetPanel()
		{
			return this;
		}

		std::string PanelConfigurationAnonymization::GetTitle()
		{
			return _Std("Anonymization");
		}

		std::string PanelConfigurationAnonymization::GetCabecera()
		{
			return _Std("Anonymization settings");
		}

		bool PanelConfigurationAnonymization::Validar()
		{
			bool ok = true;

			return ok;
		}

		bool PanelConfigurationAnonymization::Guardar()
		{
			
			return true;
		}
	//endregion
	}
}
