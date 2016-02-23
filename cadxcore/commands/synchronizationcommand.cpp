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

#include <api/internationalization/internationalization.h>
#include <api/controllers/ipacscontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/hangingprotocolcontroller.h>
#include <eventos/mensajes.h>
#include <main/entorno.h>
#include <main/controllers/historycontroller.h>
#include <eventos/progresocomando.h>
#include "synchronizationcommand.h"
#include "comandoincluirhistorial.h"
#include <wx/filename.h>
#include <wx/thread.h>


#define IDC_SYCHRONIZATION_COMMAND                  121

// Singleton de persistencia
namespace GADAPI
{
	SynchronizationCommandParameters::SynchronizationCommandParameters(const std::list<long>& seriesPk, GNC::GCS::IHistoryTool* pTool) :
		m_SeriesPk(seriesPk),
		m_pTool(pTool),
		m_pCtrlMod(NULL),
		m_mode(-1),
		m_pHangingProtocol(NULL),
		m_pSemWait(new wxSemaphore())
	{	
	}

	SynchronizationCommandParameters::SynchronizationCommandParameters(const std::list<long>& seriesPk, GNC::GCS::IModuleController* pCtrlMod, int mode, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& pHangingProtocol):
		m_SeriesPk(seriesPk),
		m_pTool(NULL),
		m_pCtrlMod(pCtrlMod),
		m_mode(mode),
		m_pHangingProtocol(pHangingProtocol),
		m_pSemWait(new wxSemaphore())
	{
	}

	SynchronizationCommandParameters::~SynchronizationCommandParameters()
	{
		if (m_pSemWait != NULL) {
			delete m_pSemWait;
			m_pSemWait = NULL;
		}
	}

	SynchronizationCommand::SynchronizationCommand(SynchronizationCommandParameters* pParams) : IComando(pParams)
	{
		m_pSynchronizeParams = pParams;
		SetId(IDC_SYCHRONIZATION_COMMAND);
		GNC::Entorno::Instance()->GetControladorEventos()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
	}

	void SynchronizationCommand::Execute()
	{
		std::string tarea= _Std("Downloading files ...");
		if (!NotificarProgreso(0.0f,tarea)) {
		    return;
		}

		GIL::DICOM::IPACSController* pCI = NULL;
		
		std::string pathDownloadedFiles = GNC::Entorno::Instance()->CrearDirectorioTemporal();
		try {
			pCI = GNC::GCS::IEntorno::Instance()->GetPACSController();
			int fileNum = 1;
			for (std::list<long>::const_iterator it = m_pSynchronizeParams->m_SeriesPk.begin(); it != m_pSynchronizeParams->m_SeriesPk.end(); ++it)
			{
				GNC::GCS::IHistoryController::SeriesModel sm = GNC::GCS::IHistoryController::Instance()->GetSeriesModel((*it));
				if (sm.location == GNC::GCS::IHistoryController::TL_WadoLinked) {
					//get files and download it
					GNC::GCS::IHistoryController::LightFileModelList listOfFiles;
					GNC::GCS::IHistoryController::Instance()->GetSeriesSortedFileModels((*it), listOfFiles);
					for (GNC::GCS::IHistoryController::LightFileModelList::iterator itFiles = listOfFiles.begin(); itFiles != listOfFiles.end(); ++itFiles)
					{
						{
							if (!NotificarProgreso(0.05, std::string(wxString::Format(_("Downloading file %d"), (int)(fileNum++) ).ToUTF8()))) {
								return;
							}
						}
						GNC::GCS::IHistoryController::LightFileModel& fm = (*itFiles);
						if (fm.location == GNC::GCS::IHistoryController::TL_WadoLinked) {
							std::ostringstream ostr;
							ostr << pathDownloadedFiles;
							ostr << (char)wxFileName::GetPathSeparator();
							ostr << fm.pk;

							if (!pCI->DownloadFileFromURL(fm.real_path, ostr.str(), this)) {
								std::ostringstream ostrErr;
								ostrErr << _Std("Error downloading file");
								ostrErr << fm.real_path;
								throw GIL::DICOM::PACSException(ostrErr.str());
							}
						}
					}
				}
			}
		}
		catch (GIL::DICOM::PACSException& ex) {
			m_pSynchronizeParams->m_error =  (const std::string)ex;
		}
		catch (std::exception& ex) {
			m_pSynchronizeParams->m_error= ex.what();
		}
		catch (...)
		{
			m_pSynchronizeParams->m_error= _Std("Internal Error");
		}
		//including history....
		if(m_pSynchronizeParams->m_error.empty()){
			GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(pathDownloadedFiles, true, GNC::GCS::IHistoryController::TAA_MOVE);
			GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial * pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
			GNC::Entorno::Instance()->GetCommandController()->ProcessAsync("Processing...",pCmd,this);
			m_pSynchronizeParams->m_pSemWait->Wait();

		}

		NotificarProgreso(1.0f,tarea);
	}

	void SynchronizationCommand::Update()
	{
		if (EstaAbortado()) {
		    return;
		}
		if(!m_pSynchronizeParams->m_error.empty()){
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Error synchronizing study: ") + "\n" + m_pSynchronizeParams->m_error, GNC::GCS::Events::EventoMensajes::PopUpMessage,false, GNC::GCS::Events::EventoMensajes::Error));
			return;
		}
		else {
			//execute tool
			if (m_pSynchronizeParams->m_pTool != NULL) {
				m_pSynchronizeParams->m_pTool->Execute(m_pSynchronizeParams->m_SeriesPk);	
			} else if (m_pSynchronizeParams->m_pCtrlMod != NULL) {
				GNC::GCS::IHistoryController::SeriesModelList seriesModelList;
				for (std::list<long>::const_iterator it = m_pSynchronizeParams->m_SeriesPk.begin(); it != m_pSynchronizeParams->m_SeriesPk.end(); ++it)
				{
					seriesModelList.push_back(GNC::GCS::HistoryController::Instance()->GetSeriesModel((*it)));
				}
				m_pSynchronizeParams->m_pCtrlMod->OpenView(m_pSynchronizeParams->m_mode, seriesModelList, m_pSynchronizeParams->m_pHangingProtocol);
			}
		}
	}

	

	bool SynchronizationCommand::NotificarProgreso(float progresoNormalizado,const std::string &texto) {
		if (EstaAbortado())
		{
			return false;
		}
		return IComando::NotificarProgreso(progresoNormalizado, texto);
	}

	void SynchronizationCommand::LiberarRecursos()
	{
	}

	void SynchronizationCommand::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
	{
		GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
		GNC::GCS::IComando* pCmd = pEvt->GetComando();
		if (pCmd != NULL && pCmd->GetOwner() == this &&pEvt->GetTipo() == GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado)
		{
			m_pSynchronizeParams->m_pSemWait->Post();						
		}
	}

}
