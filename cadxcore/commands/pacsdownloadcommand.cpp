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

#include <sstream>
#include <map>

#include <api/api.h>
#include <api/globals.h>
#include <api/ientorno.h>

#include <api/controllers/icontroladorlog.h>
#include <main/controllers/commandcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <api/controllers/icontroladorpermisos.h>

#include <eventos/mensajes.h>

#include <main/controllers/dcmtk/dicomservers.h>

#include "pacsdownloadcommand.h"
#include "comandoincluirhistorial.h"
#include "linkhistorycommand.h"


#define IDC_PACS_BUSCAR                  61
#define IDC_PACS_DESCARGAR               62
#define IDC_PACS_SUBIR			         64

#define ID_REFRESCO_PROGRESO 1

#define PARALLEL

// Singleton de persistencia
namespace GADAPI
{

PACSDownloadCommandParams::PACSDownloadCommandParams(const std::string& serverSeleccionado, const GIL::DICOM::DicomDataset& base, bool link, bool silent) :
        m_link(link),
        m_silent(silent),
        m_base(base)
{
        m_base = base;
        m_pModelo = GNC::GCS::Ptr<IModeloDicom>(new IModeloDicom());

        if(serverSeleccionado == "") {
                //usaremos el primer servidor (por defecto)
                if(!DicomServerList::Instance()->Empty()) {
                        m_serverSeleccionado = DicomServerList::Instance()->GetDefaultServer()->ID;
                } else {
                        throw GIL::DICOM::PACSException(_Std("There is not any Remote PACS configured"));
                }
        } else {
                m_serverSeleccionado = serverSeleccionado;
        }

        m_error = "";
}

PACSDownloadCommandParams::~PACSDownloadCommandParams()
{
}

PACSDownloadCommand::PACSDownloadCommand(PACSDownloadCommandParams* pParams) : IComando(pParams)
{
        m_pPACSParams = pParams;
        SetId(IDC_PACS_DESCARGAR);
        EsperaA(IDC_PACS_DESCARGAR);
#if !defined(PARALLEL)
        EsperaA(IDC_PACS_BUSCAR);
        EsperaA(IDC_PACS_DESCARGAR);
        EsperaA(IDC_PACS_SUBIR);
#endif
}

void PACSDownloadCommand::Execute()
{
        std::string tarea= _Std("Downloading files ...");
        if (!NotificarProgreso(0.0f,tarea)) {
                return;
        }

        GIL::DICOM::IPACSController* pCI = NULL;

        try {
                pCI = GNC::GCS::IEntorno::Instance()->GetPACSController();
                pCI->GetConnection(this);
                pCI->QueryRetrieve(this, m_pPACSParams->m_serverSeleccionado, m_pPACSParams->m_pModelo, m_pPACSParams->m_base, this, m_pPACSParams->m_link);
        } catch (GinkgoNoServerFoundException& ) {
                m_pPACSParams->m_error= _Std("Server ID not found. ID = ") + m_pPACSParams->m_serverSeleccionado;
                m_pPACSParams->m_pModelo->Error = true;
        } catch (GIL::DICOM::PACSException& ex) {
                m_pPACSParams->m_error =  (const std::string)ex;
                m_pPACSParams->m_pModelo->Error = true;
        } catch (std::exception& ex) {
                m_pPACSParams->m_error= ex.what();
                m_pPACSParams->m_pModelo->Error = true;
        } catch (...) {
                m_pPACSParams->m_error= _Std("Internal Error");
                m_pPACSParams->m_pModelo->Error = true;
        }
        if (pCI) {
                pCI->ReleaseConnection(this);
        }
        GTRACE(m_pPACSParams->m_pModelo);
        NotificarProgreso(1.0f,tarea);
}

void PACSDownloadCommand::Update()
{
        if (EstaAbortado()) {
                return;
        }


        if(!m_pPACSParams->m_error.empty()) {
                LOG_ERROR("C-MOVE/C-GET", "Error Downloading study: " << m_pPACSParams->m_error);
                if (!m_pPACSParams->m_silent) {
                        GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Error downloading study: ") + "\n" + m_pPACSParams->m_error, GNC::GCS::Events::EventoMensajes::PopUpMessage,false, GNC::GCS::Events::EventoMensajes::Error));
                }
                return;
        } else {
                if (!m_pPACSParams->m_link) {
                        //move downloaded images to history
                        std::list<std::string> rutas;
                        std::string rutaImagen;
                        if (m_pPACSParams->m_pModelo.IsValid()) {
                                //listamos las imagenes que nos hemos bajado
                                for (IModeloDicom::ListaPacientesType::const_iterator it = m_pPACSParams->m_pModelo->ListaPacientes().begin(); it != m_pPACSParams->m_pModelo->ListaPacientes().end(); ++it) {
                                        const IModeloPaciente& p = *it;
                                        for (IModeloPaciente::ListaEstudiosType::const_iterator it2 = p.ListaEstudios().begin(); it2 != p.ListaEstudios().end(); ++it2) {
                                                const IModeloEstudio& e = *it2;
                                                for (IModeloEstudio::ListaSeriesType::const_iterator it3 = e.ListaSeries().begin(); it3 != e.ListaSeries().end(); ++it3) {
                                                        const IModeloSerie& s = *it3;
                                                        for (IModeloSerie::ListaImagenesType::const_iterator it4 = s.ListaImagenes().begin(); it4 != s.ListaImagenes().end(); ++it4) {
                                                                const IModeloImagen& im = *it4;
                                                                rutas.push_back(im.GetPathImagen());
                                                        }
                                                }
                                        }
                                }
                        }

                        //si ha descargado algo...
                        if(rutas.empty()) {
                                return;
                        }

                        //se lanza un evento para incluir las imagenes en el historial
                        GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(rutas, true, GNC::GCS::IHistoryController::TAA_MOVE);
                        //silent mode...
                        pParams->m_abrirDespuesDeCargar = !m_pPACSParams->m_silent;
                        GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial * pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
                        GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files..."), pCmd, NULL);
                } else {
                        //link images...
                        GADAPI::LinkHistoryCommand::LinkHistoryCommandParams* pParams = new GADAPI::LinkHistoryCommand::LinkHistoryCommandParams(m_pPACSParams->m_pModelo);
                        //silent mode...
                        pParams->m_abrirDespuesDeCargar = !m_pPACSParams->m_silent;
                        GADAPI::LinkHistoryCommand::LinkHistoryCommand * pCmd = new GADAPI::LinkHistoryCommand::LinkHistoryCommand(pParams);
                        GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Including files..."), pCmd, NULL);
                }
        }
}



bool PACSDownloadCommand::NotificarProgreso(float progresoNormalizado,const std::string &texto)
{
        if (EstaAbortado()) {
                return false;
        }
        return IComando::NotificarProgreso(progresoNormalizado, texto);
}

void PACSDownloadCommand::LiberarRecursos()
{
}

}
