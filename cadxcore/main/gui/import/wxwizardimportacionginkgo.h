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

#pragma once
#include <wx/wizardginkgo/wxwizardginkgobase.h>
#include <api/iwizard.h>
#include <api/dicom/idicomizador.h>
#include <api/controllers/ipacscontroller.h>
#include <string>
#include <vector>

namespace GIL
{
class IModeloIntegracion;
}

namespace GNC
{
namespace GUI
{
class wxWizardImportacionGinkgo: public wxWizardGinkgoBase
{

public:
        wxWizardImportacionGinkgo(wxWindow* pParent, GNC::GCS::Ptr<GIL::IModeloIntegracion> pModeloIntegracion, std::string uidModulo = "");
        ~wxWizardImportacionGinkgo();

        virtual bool  BorrarArchivosTemporales(wxString dir);

        virtual void  BorrarArchivosTemporales();

        virtual void OnCancelarClick(wxCommandEvent& event);
        virtual void OnSiguienteClick(wxCommandEvent& event);
        virtual void OnAnteriorClick(wxCommandEvent &event);
        virtual void OnClose(wxCloseEvent &event);


//region "Metodos de obtencion de resultados del dialogo"
        //devuelve la ruta del directorio temporal
        virtual std::list<std::string> GetRuta();

        virtual void CargarCurrent();
//endregion


protected:
        GNC::GCS::IModuleController*     m_pControladorModulo;
        typedef std::list<IPasoWizard*> ListaPasos;
        ListaPasos                        m_ListaPasos;
        ListaPasos::iterator              m_currentPaso;

        int m_currentPasoIndex;

        IPasoWizard::ListaFicheros        m_ListaFicheros;
        wxString                          m_pathDirTemporal;
        GNC::GCS::Ptr<GIL::IModeloIntegracion>   m_pModeloIntegracion;
};
}
}
