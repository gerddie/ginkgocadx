/*
*  
*  $Id: comandoincluirhistorial.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
//#define _GINKGO_TRACE
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/uri.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/imodelointegracion.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/dicom/imodelodicom.h>
#include <main/entorno.h>
#include "linkhistorycommand.h"
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/gui/history3/historypanel3.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/pacscontroller.h>

#include "dialogopathssobreescribirbase.h"

#define IDC_INCLUIR       101

namespace GADAPI {

	//lee un directorio en profundidad y lo incluye en el dicomdir
	namespace LinkHistoryCommand {
		LinkHistoryCommandParams::LinkHistoryCommandParams(const GNC::GCS::Ptr<IModeloDicom>& pModelo) :
				m_pModelo(pModelo),
				m_abrirDespuesDeCargar(false),
				m_informar(true)

		{ }

		LinkHistoryCommandParams::~LinkHistoryCommandParams()
		{
		}


		LinkHistoryCommand::LinkHistoryCommand(LinkHistoryCommandParams* pParams) : IComando(pParams)
		{
			GTRACE(">> ComandoCarga::ComandoCarga(): " << this);
			m_pLinkParams = pParams;
			SetId(IDC_INCLUIR);
			GTRACE("<< ComandoCarga::ComandoCarga(): " << this);
		}

		void LinkHistoryCommand::Execute()
		{
			std::list<long>* insertedSeriesPtr = &(m_pLinkParams->m_InsertedSeries);

			NotificarProgreso(0.0f,_Std("Linking study..."));

			//first iteration insert downloaded slices into history as usual...
			if (m_pLinkParams->m_pModelo.IsValid()) {
				//listamos las imagenes que nos hemos bajado
				GNC::GCS::HistoryController::StringList urlList;
				for (IModeloDicom::ListaPacientesType::const_iterator it = m_pLinkParams->m_pModelo->ListaPacientes().begin(); it != m_pLinkParams->m_pModelo->ListaPacientes().end(); ++it) {
					const IModeloPaciente& p = *it;
					for (IModeloPaciente::ListaEstudiosType::const_iterator it2 = p.ListaEstudios().begin(); it2 != p.ListaEstudios().end(); ++it2) {
						const IModeloEstudio& e = *it2;
						for (IModeloEstudio::ListaSeriesType::const_iterator it3 = e.ListaSeries().begin(); it3 != e.ListaSeries().end(); ++it3) {
							const IModeloSerie& s = *it3;
							//first of all insert files...
							GNC::GCS::HistoryController::StringList fileList;
							GNC::GCS::IHistoryController::DICOMFileModelList fileModels;
							for (IModeloSerie::ListaImagenesType::const_iterator it4 = s.ListaImagenes().begin(); it4 != s.ListaImagenes().end(); ++it4) {
								const IModeloImagen& im = *it4;
								std::string path = im.GetPathImagen();
								wxFileName filename(FROMPATH(im.GetPathImagen()));
								if(filename.GetExt().Lower() == wxT("dcm") || GIL::DICOM::PACSController::Instance()->EsDICOM(im.GetPathImagen(), false, true) ) {
									fileList.push_back(im.GetPathImagen());
								} else if (!path.empty()) {
									GNC::GCS::IHistoryController::DICOMFileModel fileModel;
										fileModel.file.file_path = im.GetPathImagen();
										long in = 0;
										wxString::FromUTF8(im.GetInstanceNumber().c_str()).ToLong(&in);
										fileModel.file.instance_number = in;
										fileModel.file.sopiuid = im.GetUID();
										fileModels.push_back(fileModel);
								} else {
									LOG_ERROR("LINKCommand", "trying to link an instance without wado url: " << im.GetUID());
									m_pLinkParams->m_ErrorList.push_back(GNC::GCS::HistoryController::TAddError(GNC::GCS::IHistoryController::TAddError::TE_WrongFormat, im.GetUID()));
								} 
							}
							if (fileList.empty()) {
								//error no file in series
								return;
							}
							if (GNC::GCS::HistoryController::Instance()->AddFiles(fileList, m_pLinkParams->m_ErrorList, insertedSeriesPtr, this, GNC::GCS::HistoryController::TAA_MOVE)) {
								GNC::GCS::HistoryController::Instance()->LinkFilesToSeries(insertedSeriesPtr->back(), fileModels,this);
							}
						}
					}
				}
			}
		}

		void LinkHistoryCommand::Update()
		{
			wxString message = wxT("");
			message += _("Image acquisition finished.");
			message += wxT("\n");
			if (m_pLinkParams->m_ErrorList.size() > 0)
				message += wxString::Format(_("\nThere are %d errors (see log for more detailed description)"), (int)(m_pLinkParams->m_ErrorList.size()));

			//log
			{
				std::ostringstream errorOstr;
				if (!m_pLinkParams->m_ErrorList.empty()) {

					bool first = true;
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pLinkParams->m_ErrorList.begin();it != m_pLinkParams->m_ErrorList.end(); ++it)
					{
						if ((*it).error == GNC::GCS::HistoryController::TAddError::TE_FileNotExist) {
							if (first) {
								errorOstr<< "\t" << _Std("This files doesn't exist:") << std::endl;
								first = false;
							}
							errorOstr << "\t\t" << (*it).path << std::endl;
						}
					}
					first = true;
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pLinkParams->m_ErrorList.begin();it != m_pLinkParams->m_ErrorList.end(); ++it)
					{
						if ((*it).error == GNC::GCS::HistoryController::TAddError::TE_WrongFormat) {
							if (first) {
								errorOstr<< "\t" << _Std("This files doesn't have DICOM format:") << std::endl;
								first = false;
							}
							errorOstr << "\t\t" << (*it).path << std::endl;
						}
					}
					
					first = true;
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pLinkParams->m_ErrorList.begin();it != m_pLinkParams->m_ErrorList.end(); ++it)
					{
						if ((*it).error == GNC::GCS::HistoryController::TAddError::TE_DICOMDir) {
							if (first) {
								errorOstr << "\t" <<_Std("This files are Dicom Dirs:") << std::endl;
								first = false;
							}
							errorOstr << "\t\t" << (*it).path << std::endl;
						}
					}
				}
				
				std::string errorString = errorOstr.str();
				if (!errorString.empty()) {
					LOG_ERROR("ComandoIncluirHistorial", errorOstr.str());
				}
			}

			GNC::GUI::HistoryPanel3::Instance()->ReloadCombos(false);
			GNC::GUI::HistoryPanel3::Instance()->RefreshSearch();
			const int MAX_OPENED_STUDIES = 3;
			if (m_pLinkParams->m_abrirDespuesDeCargar){
				std::set<long> openedStudies;
				for (std::list<long>::iterator it = m_pLinkParams->m_InsertedSeries.begin(); it != m_pLinkParams->m_InsertedSeries.end() && openedStudies.size() < MAX_OPENED_STUDIES; ++it) {
					GNC::GCS::HistoryController::SeriesModel seriesModel = GNC::GCS::HistoryController::Instance()->GetSeriesModel((*it));
					GNC::GUI::HistoryPanel3::Instance()->OpenSeriesOrStudy(seriesModel.study_fk, false);
				}				
			}
		}
	}
}

