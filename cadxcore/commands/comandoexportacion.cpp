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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

#include <sstream>

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <main/controllers/configurationcontroller.h>

#include <api/globals.h>
#include <api/ientorno.h>
#include <api/dicom/idicommanager.h>
#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorlog.h>

#include <api/controllers/ipacscontroller.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/commandcontroller.h>
#include <main/gui/history3/historypanel3.h>

#include <eventos/mensajes.h>

#include "comandoexportacion.h"

#include <itkImage.h>
#include <itkRGBPixel.h>
#include <itkGDCMImageIO.h>
#include <itkJPEGImageIO.h>
#include <itkPNGImageIO.h>
#include <itkVectorResampleImageFilter.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkImageFileWriter.h>
#include <api/istudycontext.h>

#include <api/dicom/dcmdictionary.h>
#include <commands/comandoincluirhistorial.h>



namespace GADAPI {

	ComandoExportacionParams::ComandoExportacionParams(GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion>& pDatosPersistentes)
	{
		m_pDatosPersistentes = pDatosPersistentes;
		m_hasError = false;
		if (pDatosPersistentes->m_target != GNC::GUI::TipoWizardExportacion::EXTERNAL_FOLDER) {
			m_tempDir = GNC::Entorno::Instance()->CreateGinkgoTempDir();
		}
	}

	ComandoExportacionParams::~ComandoExportacionParams()
	{
	}

	ComandoExportacion::ComandoExportacion(ComandoExportacionParams* pParams): GNC::GCS::IComando(pParams,"Exportacion")
	{
		m_pExportacionParams = pParams;
	}

	void ComandoExportacion::Execute()
	{
		if (!NotificarProgreso(0.0, _Std("Exporting files...")) )
			return;
		
		ExportarDICOM();		

		if (m_pExportacionParams->m_pDatosPersistentes->m_target == GNC::GUI::TipoWizardExportacion::OVERWRITE) {
			GNC::GCS::HistoryController::FileModelList fileModelList;
			for (std::list<long>::iterator it = m_pExportacionParams->m_pDatosPersistentes->ListOfSeriesPks.begin(); it != m_pExportacionParams->m_pDatosPersistentes->ListOfSeriesPks.end(); ++it)
			{
				GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*it), fileModelList);
			}
			GNC::GCS::HistoryController::Instance()->UpdateFiles(fileModelList);
		}
	}

	void ComandoExportacion::Update()
	{
		switch (m_pExportacionParams->m_pDatosPersistentes->m_target) {
			case GNC::GUI::TipoWizardExportacion::EXTERNAL_FOLDER:
			{
				if (m_pExportacionParams->m_hasError)
				{
					GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,m_pExportacionParams->m_Error, GNC::GCS::Events::EventoMensajes::StatusMessage,true, GNC::GCS::Events::EventoMensajes::Error));
				} else {
					GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Export has been finished successfully"), GNC::GCS::Events::EventoMensajes::StatusMessage,true, GNC::GCS::Events::EventoMensajes::Informacion));
				}
			}
			break;
			case GNC::GUI::TipoWizardExportacion::OVERWRITE:
			{
				GNC::GUI::HistoryPanel3::Instance()->ReloadHistory();
			}
			break;
			case GNC::GUI::TipoWizardExportacion::NEW_STUDY:
			{
				//add to history...
				GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(m_pExportacionParams->m_outputPaths, false, GNC::GCS::HistoryController::TAA_MOVE);
				GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
				GNC::GCS::ICommandController::Instance()->ProcessAsync("Including...", pCmd, NULL);
			} 
			break;
		}
	}

	//endregion

	void ComandoExportacion::ExportarDICOM()
	{
		GNC::GCS::Ptr<GNC::GUI::TipoWizardExportacion> pDatosPersistentes = m_pExportacionParams->m_pDatosPersistentes;
		bool correcto = true;

		std::list<std::string> resultado;
		std::list<std::string> listaPaths;

		if (!pDatosPersistentes->ListOfSeriesPks.empty()) {
			for (std::list<long>::iterator itUIDs = pDatosPersistentes->ListOfSeriesPks.begin(); itUIDs != pDatosPersistentes->ListOfSeriesPks.end(); ++itUIDs)
			{
				GNC::GCS::HistoryController::StringList pathsSerie = GNC::GCS::HistoryController::Instance()->GetSeriesSortedPaths((*itUIDs));
				listaPaths.insert(listaPaths.begin(), pathsSerie.begin(), pathsSerie.end());
			}
		} else if (!pDatosPersistentes->ListOfDICOMPaths.empty()) {
			listaPaths.insert(listaPaths.end(), pDatosPersistentes->ListOfDICOMPaths.begin(), pDatosPersistentes->ListOfDICOMPaths.end());
		}

		GIL::DICOM::IDICOMManager*	pDICOMManager;

		
		wxString destino;

		//update ids to get new study...
		std::map<std::string, std::string> mapOfUIDs;
		std::string stringTmp;
		GIL::DICOM::DicomDataset baseUIDs;
		//

		int contador = 1;
		for(std::list<std::string>::iterator it = listaPaths.begin(); it!= listaPaths.end(); ++it,++contador) {
			wxString cadena = wxString::Format(_("Exporting file %d of %d"), (int)contador, (int)listaPaths.size());
			if (!NotificarProgreso((float)contador/listaPaths.size(), std::string(cadena.ToUTF8())))
				return;

			GIL::DICOM::DicomDataset base;

			pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
			pDICOMManager->CargarFichero((*it), base);
			
			pDICOMManager->ActualizarJerarquia(pDatosPersistentes->m_base);

			if (pDatosPersistentes->m_target == GNC::GUI::TipoWizardExportacion::NEW_STUDY) {
				baseUIDs.tags.clear();

                // GW: The original code didn't test whether the tag was actually found, and
                // so the first fail would create an UID for an empty value, and
                // for later calls the old value would be re-used (which would obvioulsy be wrong).
                // so now we just don't set the baseUID tags if they are not available
                // this may now reveal another bug.

				//study instance uid
                if (base.getTag(GKDCM_StudyInstanceUID, stringTmp)) {
                    if (mapOfUIDs.find(stringTmp) == mapOfUIDs.end()) {
                        mapOfUIDs[stringTmp] = pDICOMManager->GetNewUID();
                    }
                    baseUIDs.tags[GKDCM_StudyInstanceUID] = mapOfUIDs[stringTmp];
                }else{
                    LOG_INFO("Commands", "tag " << GKDCM_StudyInstanceUID << " not available");
                }

				//series instance uid
                if (base.getTag(GKDCM_SeriesInstanceUID, stringTmp)) {
                    if (mapOfUIDs.find(stringTmp) == mapOfUIDs.end()) {
                        mapOfUIDs[stringTmp] = pDICOMManager->GetNewUID();
                    }
                    baseUIDs.tags[GKDCM_SeriesInstanceUID] = mapOfUIDs[stringTmp];
                }else{
                    LOG_INFO("Commands", "tag " << GKDCM_SeriesInstanceUID << " not available");
                }

                //sop instance
                if (base.getTag(GKDCM_SOPInstanceUID, stringTmp)) {
                    if (mapOfUIDs.find(stringTmp) == mapOfUIDs.end()) {
                        mapOfUIDs[stringTmp] = pDICOMManager->GetNewUID();
                    }
                    baseUIDs.tags[GKDCM_SOPInstanceUID] = mapOfUIDs[stringTmp];
                }else{
                    LOG_INFO("Commands", "tag " << GKDCM_SOPInstanceUID << " not available");
                }
				//update uids
				pDICOMManager->ActualizarJerarquia(baseUIDs);
			}

			//anonimizar tags Ginkgo
			if(!pDatosPersistentes->m_incluirTagsGinkgo){
				pDICOMManager->AnonimizarTagsPrivados();
			}


			if (pDatosPersistentes->m_target == GNC::GUI::TipoWizardExportacion::OVERWRITE) {
				destino = FROMPATH((*it));
			} else {
				destino = GetFichero(wxT("dcm"));
			}

			std::string destinoStd(TOPATH(destino));
			correcto = correcto && pDICOMManager->AlmacenarFichero(destinoStd);
			resultado.push_back(destinoStd);
			m_pExportacionParams->m_outputPaths.push_back(destinoStd);
			GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
		}

		if(!correcto) {
			m_pExportacionParams->m_Error = _Std("Error storing file, check the permissions over the directory.");
			m_pExportacionParams->m_hasError = true;
		}
	}


	wxString ComandoExportacion::GetFichero(const wxString& extension) {
		wxString dir;
		if (m_pExportacionParams->m_pDatosPersistentes->m_target == GNC::GUI::TipoWizardExportacion::EXTERNAL_FOLDER) {
			dir = FROMPATH(m_pExportacionParams->m_pDatosPersistentes->m_pathDestino);
		} else {
			dir = FROMPATH(m_pExportacionParams->m_tempDir);
		}

		//if pathDestino is a file.. then return
		if (!wxDirExists(dir))
		{
			return dir;
		} else {
			wxString destino(wxEmptyString);
			std::string nombre (wxDateTime::Now().Format(_("image_%m-%d-%Y_")).ToUTF8());
			std::string stdDir(TOPATH(dir));
			std::string stdExtension(TOPATH(extension));
			int indice=0;
			//yy-mm-dd_imagen_indice
			do {
				std::ostringstream ostr;
				ostr << stdDir << (char)wxFileName::GetPathSeparator() << nombre << indice++ << "." << stdExtension;
				destino = FROMPATH(ostr.str());
			} while(wxFile::Exists(destino));
			return destino;
		}
	}
};

#ifdef _MSC_VER
#pragma warning(pop)
#endif

