/*
 *  
 *  $Id: selectimagesimportationvisualizator.cpp $
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

#include <string>
#include <sstream>
#include <cmath>

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/valtext.h>
#include <wx/busyinfo.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <main/controllers/configurationcontroller.h>
#include <main/entorno.h>


#include <api/dicom/idicommanager.h>

#include <main/controllers/commandcontroller.h>

#include <commands/comandopacs.h>

#include <export/tagsprivadoscomunes.h>

#include <commands/dicomizecommand.h>
#include <visualizator/export/tagsprivados.h>

#include "selectimagesimportationvisualizator.h"

namespace GNKVisualizator {
	namespace GUI {

		SelectImagesImportationVisualizator::SelectImagesImportationVisualizator(wxWindow* pParent,IWizard* pWizard,const GNC::GCS::Ptr<GNC::GUI::ImportationData>& importationData,const std::string &dirTemporal): 
			GNC::GUI::SelectImagesImportation(pParent, dirTemporal, pWizard, importationData->m_pIntegrationModel)
		{
			Hide();
			m_dicomizado = false;
			m_pImportationData = importationData;

			m_dirTemporal=dirTemporal;
		}

		SelectImagesImportationVisualizator::~SelectImagesImportationVisualizator()
		{
		}

//region "Metodos heredados de Ipasowizard"
		bool SelectImagesImportationVisualizator::Validar(){
			bool correcto =  true;
			if(GNC::GUI::SelectImagesImportation::Validar())
			{
				if(!m_dicomizado) {
					m_dicomizado = Dicomizar();
					correcto = m_dicomizado;
				}
			} else {
				correcto = false;
			}
			return correcto;
		}

		bool SelectImagesImportationVisualizator::Dicomizar() 
		{
			for(ListaFicheros::const_iterator it=m_pListaFicheros->begin(); it!= m_pListaFicheros->end(); ++it) {
				std::string rutaFichero = (*it);
				m_pImportationData->m_SourceFiles.push_back(rutaFichero);
			}

			RellenarTagsComunes(m_pImportationData->baseImages);
			GADAPI::DicomizeCommandParams* pDicomParams = new GADAPI::DicomizeCommandParams(m_pImportationData,m_dirTemporal);
			GADAPI::DicomizeCommand* pDicomCmd = new GADAPI::DicomizeCommand(pDicomParams);
			GNC::GCS::IEntorno::Instance()->GetCommandController()->ProcessAsync(_Std("Performing tasks of integration ..."),pDicomCmd,NULL);
			//se limpia la lista porque ya avisaremos de los nuevos ficheros
			m_pListaFicheros->clear();
			//
			return true;
			
		}

		void SelectImagesImportationVisualizator::RellenarTagsComunes(GIL::DICOM::DicomDataset& base)
		{
			std::string tag;

			//institucion
			if (GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/Estacion","CentroNombre",tag)) {
				base.tags[std::string("0008|0080")] = tag;
			}

			//nombre del medico responsable de la institucion
			if (GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Estacion","NombreMedico",tag)) {
				base.tags[std::string("0008|0090")] = tag;
			}

			base.tags[std::string("0008|0070")] = EXT_PROVIDER;

			base.tags[std::string("0008|1090")] = EXT_NAME;

			//uid de aprimaria
			base.tags[std::string("0018|1030")] = EXT_IMPORTER_SID;
		}
	//endregion
	}
}
