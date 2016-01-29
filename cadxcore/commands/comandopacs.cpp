/*
*
*  $Id: comandopacs.cpp $
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

#include <sstream>
#include <map>

#include <api/api.h>
#include <api/globals.h>
#include <api/ientorno.h>

#include <api/controllers/icontroladorlog.h>
#include <main/controllers/commandcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <api/dicom/dcmuiddictionary.h>
#include <api/controllers/icontroladorpermisos.h>
#include <main/controllers/storedqueriescontroller.h>

#include <eventos/mensajes.h>

#include <main/controllers/dcmtk/dicomservers.h>

#include "comandopacs.h"
#include "comandoincluirhistorial.h"



#define IDC_PACS_BUSCAR                  61
#define IDC_PACS_DESCARGAR               62
#define IDC_PACS_SUBIR			         64

#define ID_REFRESCO_PROGRESO 1

#define PARALLEL

// Singleton de persistencia
namespace GADAPI
{
	ComandoPACSParams::ComandoPACSParams(const GNC::GCS::Ptr<GNC::GCS::StoredQuery>& query) :
		pQuery(query),
		m_apResults(new TSearchResultList())
	{
		if (pQuery->getPacsId().empty()) {
			if(!DicomServerList::Instance()->Empty()) {
				pQuery->setPacsId(DicomServerList::Instance()->GetDefaultServer()->ID);
				LOG_DEBUG("COMANDOPACS", "Unspecified pacs ID, selecting " << pQuery->getPacsId());
			} else {
				throw GIL::DICOM::PACSException(_Std("There is not any Remote PACS configured"));
			}
		}

		m_informar=true;
		m_error = "";
	}

	ComandoPACSParams::~ComandoPACSParams() {
	}


	ComandoPACS::ComandoPACS(ComandoPACSParams* pParams) : IComando(pParams)
	{
		m_pPACSParams = pParams;
		
		SetId(IDC_PACS_BUSCAR);
#if !defined(PARALLEL)
		EsperaA(IDC_PACS_BUSCAR);
		EsperaA(IDC_PACS_DESCARGAR);
		EsperaA(IDC_PACS_SUBIR);
#endif				
	}

	void ComandoPACS::Execute()
	{
		std::string tarea=_Std("Querying PACS ...");
		if (!NotificarProgreso(0.0f,tarea)) {
		    return;
		}
		GIL::DICOM::IPACSController* pCI = NULL;
		try {

			pCI = GNC::GCS::IEntorno::Instance()->GetPACSController();
			if (pCI == NULL) {
				throw GIL::DICOM::PACSException(_Std("Error accessing the controller subsystem integration: (GIL:: DICOM)."));
			}

			pCI->GetConnection(this);

			std::string mensaje = _Std("Starting Search ...");
			if ( !NotificarProgreso(0.0f, mensaje) ) {
			    return;
			}

			GIL::DICOM::DicomDataset query;
			m_pPACSParams->pQuery->buildQuery(query);
			pCI->Query(this, GKUID_FINDStudyRootQueryRetrieveInformationModel, query, *(m_pPACSParams->m_apResults), m_pPACSParams->pQuery->getPacsId(), this);
			
		} catch (GinkgoNoServerFoundException& ) {
			m_pPACSParams->m_error= _Std("Server ID not found. ID = ") + m_pPACSParams->pQuery->getPacsId();
		} catch (GIL::DICOM::PACSException& ex) {
			m_pPACSParams->m_error= _Std("Query error with PACS Id ") + m_pPACSParams->pQuery->getPacsId() + "\n" + (const std::string)ex;
		} catch (GIL::DICOM::ModelException& ex) {
			m_pPACSParams->m_error= _Std("Query error with PACS Id ") + m_pPACSParams->pQuery->getPacsId() + "\n" + (const std::string)ex;
		} catch (std::exception& ex) {
			m_pPACSParams->m_error= _Std("Query error with PACS Id ") + m_pPACSParams->pQuery->getPacsId() + "\n" + ex.what();
		} catch ( ... ) {
			m_pPACSParams->m_error= _Std("Query error with PACS Id ") + m_pPACSParams->pQuery->getPacsId() + "\n" + _Std("Internal error");
		}
		if (pCI) {
			pCI->ReleaseConnection(this);
		}
		if (!NotificarProgreso(1.0f,tarea)) {
		    return;
		}
	}

	void ComandoPACS::Update()
	{
		if (EstaAbortado()) {
		    return;
		}
				
		if(!m_pPACSParams->m_error.empty()){
			LOG_ERROR("C-FIND", "Error searching on PACS:\n"  << m_pPACSParams->m_error);
			if (m_pPACSParams->m_informar) {
				GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Failed to perform search: ") + "\n" + m_pPACSParams->m_error, GNC::GCS::Events::EventoMensajes::PopUpMessage,false, GNC::GCS::Events::EventoMensajes::Error));
			}
			m_pPACSParams->m_informar = false;
			
		}
	}


	bool ComandoPACS::NotificarProgreso(float progresoNormalizado,const std::string &texto) {
		if (EstaAbortado())
		{
			return false;
		}
		return IComando::NotificarProgreso(progresoNormalizado, texto);
	}

	void ComandoPACS::LiberarRecursos()
	{
	}

}
