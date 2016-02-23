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

//#define _GINKGO_TRACE
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/ivista.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/imodelointegracion.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/dicom/imodelodicom.h>
#include <main/entorno.h>
#include "comandoincluirhistorial.h"
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/gui/history3/historypanel3.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/pacscontroller.h>

#include "dialogopathssobreescribirbase.h"

#define IDC_INCLUIR       101
#define IDC_AVISAR		  102

class SubComandoAvisarModelos : public GNC::GCS::IComando {
public:
	SubComandoAvisarModelos() : IComando(NULL)
	{
		SetId(IDC_AVISAR);
	}

	virtual void Execute()
	{
	}

	virtual void Update()
	{
		GNC::GUI::HistoryPanel3::Instance()->RefreshSearch();
	}

	virtual void LiberarRecursos()
	{
	}

protected:
};

namespace GADAPI {

	//lee un directorio en profundidad y lo incluye en el dicomdir
	namespace ComandoIncluirHistorial {
		ComandoIncluirHistorialParams::ComandoIncluirHistorialParams(const std::string& path, bool recursivo, GNC::GCS::HistoryController::TAddAction addAction) :
				m_numberOfFiles(0),
				m_path(path),
				m_recursivo(recursivo),
				m_abrirDespuesDeCargar(false),
				m_informar(true),
				m_addAction(addAction)

		{ }

		ComandoIncluirHistorialParams::ComandoIncluirHistorialParams(const std::list<std::string>& listaPaths, bool abrirDespuesDeCargar, GNC::GCS::HistoryController::TAddAction addAction) :
				m_numberOfFiles(0),
				m_recursivo(true),
				m_abrirDespuesDeCargar(abrirDespuesDeCargar),
				m_informar(true),
				m_addAction(addAction)
		{
			if (listaPaths.size() == 1) {
				m_path = listaPaths.front();
			} else {
				m_listaPaths = listaPaths;
			}
		}

		ComandoIncluirHistorialParams::~ComandoIncluirHistorialParams()
		{
		}


		ComandoIncluirHistorial::ComandoIncluirHistorial(ComandoIncluirHistorialParams* pParams) : IComando(pParams)
		{
			GTRACE(">> ComandoCarga::ComandoCarga(): " << this);
			m_pIncluirParams = pParams;
			SetId(IDC_INCLUIR);
			GTRACE("<< ComandoCarga::ComandoCarga(): " << this);
		}

		void ComandoIncluirHistorial::Execute()
		{
			std::list<long>* insertedSeriesPtr = &(m_pIncluirParams->m_InsertedSeries);

			NotificarProgreso(0.0f,_Std("Exploring directory ..."));

			if(m_pIncluirParams->m_path != "") {
				GNC::GCS::HistoryController::StringList listaFicheros;
				wxString wxPath = FROMPATH(m_pIncluirParams->m_path);
				if(wxFileExists(wxPath)) {
					wxFileName filename(wxPath);
					if(filename.GetExt().Lower() == wxT("dcm") || GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(wxPath), false, true) ) {
						listaFicheros.push_back(m_pIncluirParams->m_path);

						GNC::GCS::HistoryController::Instance()->AddFiles(listaFicheros, m_pIncluirParams->m_ErrorList, insertedSeriesPtr, this, m_pIncluirParams->m_addAction);
						m_pIncluirParams->m_numberOfFiles += listaFicheros.size();
					} else if (GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(wxPath), true, false)) {
						//it's a DICOMDIR file...
						NotificarProgreso(0.05f,_Std("Reading DICOMDIR ..."));
						IModeloDicom dicomDirModel;
						if(GIL::DICOM::PACSController::Instance()->LeerDicomDir(m_pIncluirParams->m_path,&dicomDirModel) && dicomDirModel.ListaPacientes().size()!=0) {
							for (IModeloDicom::ListaPacientesType::const_iterator it = dicomDirModel.ListaPacientes().begin(); it != dicomDirModel.ListaPacientes().end(); ++it) {
								const IModeloPaciente& p = *it;
								for (IModeloPaciente::ListaEstudiosType::const_iterator it2 = p.ListaEstudios().begin(); it2 != p.ListaEstudios().end(); ++it2) {
									const IModeloEstudio& e = *it2;
									listaFicheros.clear();
									for (IModeloEstudio::ListaSeriesType::const_iterator it3 = e.ListaSeries().begin(); it3 != e.ListaSeries().end(); ++it3) {
										const IModeloSerie& s = *it3;
										for(IModeloSerie::ListaImagenesType::const_iterator it4 = s.ListaImagenes().begin(); it4 != s.ListaImagenes().end(); ++it4) {
											const IModeloImagen& i = *it4;
											if(i.GetPathImagen() != "") {
												listaFicheros.push_back(i.GetPathImagen());
											}
										}
									}
									GNC::GCS::HistoryController::Instance()->AddFiles(listaFicheros, m_pIncluirParams->m_ErrorList, insertedSeriesPtr, this, m_pIncluirParams->m_addAction);
									m_pIncluirParams->m_numberOfFiles += listaFicheros.size();
									SubComandoAvisarModelos * pCmd = new SubComandoAvisarModelos();
									GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files in the history .."),pCmd,NULL);
								}
							}
						}
					}
				} else if (wxDirExists(wxPath) && m_pIncluirParams->m_recursivo) {
					//caso especial en el que se va avisando poco a poco...
					TListaListas listaListas;
					LeerDirectorioRecursivo(wxPath,listaListas);
					for (TListaListas::iterator it = listaListas.begin(); it != listaListas.end(); ++it) {
						//se incluyen "por paquetes los ficheros"
						GNC::GCS::HistoryController::Instance()->AddFiles((*it), m_pIncluirParams->m_ErrorList, insertedSeriesPtr, this, m_pIncluirParams->m_addAction);
						m_pIncluirParams->m_numberOfFiles += (*it).size();
						SubComandoAvisarModelos * pCmd = new SubComandoAvisarModelos();
						GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files in the history .."),pCmd,NULL);
					}
				} else if(wxDirExists(wxPath)){
					//leer el directorio normal
					wxDir dir;
					if (dir.Open(wxPath)) {
						wxString wxPathFich;
						bool cont = dir.GetFirst(&wxPathFich,wxEmptyString, wxDIR_FILES | wxDIR_DIRS);
						while (cont) {
							wxPathFich = dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) + wxPathFich;
							wxFileName filename(wxPathFich);
							if(filename.GetExt().Lower() == wxT("dcm") || GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(wxPathFich), false, true) ) {
								std::string pathStd(TOPATH(wxPathFich));
								listaFicheros.push_back(pathStd);
							}
							cont = dir.GetNext(&wxPathFich);
						}
					}
					GNC::GCS::HistoryController::Instance()->AddFiles(listaFicheros, m_pIncluirParams->m_ErrorList, insertedSeriesPtr, this, m_pIncluirParams->m_addAction);
					m_pIncluirParams->m_numberOfFiles += listaFicheros.size();
					SubComandoAvisarModelos * pCmd = new SubComandoAvisarModelos();
					GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files in the history .."),pCmd,NULL);
				}
			} else {
				GNC::GCS::HistoryController::Instance()->AddFiles(m_pIncluirParams->m_listaPaths, m_pIncluirParams->m_ErrorList, insertedSeriesPtr, this, m_pIncluirParams->m_addAction);
				m_pIncluirParams->m_numberOfFiles += m_pIncluirParams->m_listaPaths.size();
			}
			//remove duplicated series ids
			std::set<long> setOfIds;
			for (std::list<long>::iterator it = m_pIncluirParams->m_InsertedSeries.begin(); it != m_pIncluirParams->m_InsertedSeries.end(); ++it) {
				setOfIds.insert((*it));
			}
			m_pIncluirParams->m_InsertedSeries.clear();
			m_pIncluirParams->m_InsertedSeries.insert(m_pIncluirParams->m_InsertedSeries.begin(), setOfIds.begin(), setOfIds.end());
		}

		void  ComandoIncluirHistorial::LeerDirectorioRecursivo(wxString& dirPath, TListaListas& listaListasPaths, int profundidadMaxima, const int profundidadActual)
		{
			{
				std::ostringstream ostr;
				ostr << _Std("Exploring directory ") << dirPath.ToUTF8();
				NotificarProgreso(0.0f, ostr.str());
				if (EstaAbortado())
				{
					return;
				}
			}
			TListaPaths listaCurrentPaths;
			wxArrayString listaDirectoriosCurrent;
			bool dicomEncontrado = false;
			if(wxDirExists(dirPath)) {
				//vaciar
				wxDir dir;
				if (dir.Open(dirPath)) {
					wxString fileName;
					//no se listan los ocultos
					bool cont = dir.GetFirst(&fileName,wxEmptyString,wxDIR_FILES | wxDIR_DIRS);
					while (cont) {
						fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) + fileName;
						if(wxDir::Exists(fileName) ){
							listaDirectoriosCurrent.push_back(fileName);
						}else if (wxFileExists(fileName)) {
							wxFileName filename(fileName);
							if(filename.GetExt().Lower() == wxT("dcm") || GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(fileName), false, true) ) {
								std::string pathStd(TOPATH(fileName));
								listaCurrentPaths.push_back(pathStd);
								dicomEncontrado = true;
							}
						}
						cont = dir.GetNext(&fileName);
					}
				}
			} else if (wxFileExists(dirPath)) {
				wxFileName filename(dirPath);
				if(filename.GetExt().Lower() == wxT("dcm") || GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(dirPath), false, true)) {
					std::string pathStd(TOPATH(dirPath));
					listaCurrentPaths.push_back(pathStd);
					dicomEncontrado = true;
				}
			}

			int siguienteProfundidad = profundidadActual;
			if(dicomEncontrado) {
				//se resetea la profundidad actual
				siguienteProfundidad = 0;
			} else {
				siguienteProfundidad ++;
			}
			if(siguienteProfundidad <= profundidadMaxima) {
				for(wxArrayString::iterator it =listaDirectoriosCurrent.begin(); it != listaDirectoriosCurrent.end(); ++it) {
					LeerDirectorioRecursivo((*it),listaListasPaths,profundidadMaxima,siguienteProfundidad);
				}
			}

			listaListasPaths.push_back(listaCurrentPaths);
		}

		void ComandoIncluirHistorial::Update()
		{
			wxString message = wxT("");
			message += _("Image acquisition finished.");
			message += wxT(" ");
			message += wxString::Format(_("There have been included %d images."), (int)(m_pIncluirParams->m_numberOfFiles));
			message += wxT("\n");
			if (m_pIncluirParams->m_ErrorList.size() > 0)
				message += wxString::Format(_("\nThere are %d errors (see log for more detailed description)"), (int)(m_pIncluirParams->m_ErrorList.size()));

			//log
			{
				std::ostringstream errorOstr;
				if (!m_pIncluirParams->m_ErrorList.empty()) {

					bool first = true;
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pIncluirParams->m_ErrorList.begin();it != m_pIncluirParams->m_ErrorList.end(); ++it)
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
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pIncluirParams->m_ErrorList.begin();it != m_pIncluirParams->m_ErrorList.end(); ++it)
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
					for (GNC::GCS::HistoryController::TAddErrorList::iterator it = m_pIncluirParams->m_ErrorList.begin();it != m_pIncluirParams->m_ErrorList.end(); ++it)
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
			if (m_pIncluirParams->m_abrirDespuesDeCargar){
				std::set<long> openedStudies;
				for (std::list<long>::iterator it = m_pIncluirParams->m_InsertedSeries.begin(); it != m_pIncluirParams->m_InsertedSeries.end() && openedStudies.size() < MAX_OPENED_STUDIES; ++it) {
					GNC::GCS::HistoryController::SeriesModel seriesModel = GNC::GCS::HistoryController::Instance()->GetSeriesModel((*it));
					GNC::GUI::HistoryPanel3::Instance()->OpenSeriesOrStudy(seriesModel.study_fk, false);
				}				
			}
		}
	}
}

