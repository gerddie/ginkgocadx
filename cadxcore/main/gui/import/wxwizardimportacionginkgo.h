/*
 *  
 *  $Id: wxwizardimportacionginkgo.h $
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
#pragma once
#include <wx/wizardginkgo/wxwizardginkgobase.h>
#include <api/iwizard.h>
#include <api/dicom/idicomizador.h>
#include <api/controllers/ipacscontroller.h>
#include <string>
#include <vector>

namespace GIL{
	class IModeloIntegracion;
}

namespace GNC {
	namespace GUI {
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
