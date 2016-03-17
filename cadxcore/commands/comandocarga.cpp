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
#include <wx/msgdlg.h>

#include <api/globals.h>
#include <api/ivista.h>
#include <api/istudycontext.h>

#include <api/controllers/icontroladorvistas.h>
#include <api/imodelointegracion.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/internationalization/internationalization.h>

#include <main/entorno.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/controladorcarga.h>
#include <main/controllers/controladorlog.h>


#include "comandocarga.h"

#define IDC_CARGA       91

GADAPI::ComandoCargaParams::ComandoCargaParams(GNC::GCS::IVista* pVista, const bool loadFirst)
{
        IVista = pVista;
        Padre = NULL;
        GNC::Entorno* pEntorno = GNC::Entorno::Instance();
        LoadFirst = loadFirst;
        pEntorno->ViewsObservers.push_back(this);
}

GADAPI::ComandoCargaParams::~ComandoCargaParams()
{
        GNC::Entorno* pEntorno = GNC::Entorno::Instance();
        pEntorno->ViewsObservers.remove(this);
}

void GADAPI::ComandoCargaParams::OnVistaDestruida(GNC::GCS::IVista* pVista)
{
        if (IVista == pVista) {
                Padre->Abort();
                if (IVista != NULL) {
                        GNC::GCS::ILocker lock(pVista);
                        IVista->DetenerPipeline();
                        IVista = NULL;
                }
        }
}

GADAPI::ComandoCarga::ComandoCarga(ComandoCargaParams* pParams) : IComando(pParams, "ComandoCarga")
{
        CargaParams = pParams;
        CargaParams->Padre = this;
        SetId(IDC_CARGA);
        EsperaA(IDC_CARGA);
        CargaParams->IVista->OnCargaIniciada();
}

void GADAPI::ComandoCarga::Execute()
{
        std::string tarea(_Std("Loading..."));
        GNC::GCS::ILocker lock(CargaParams->IVista);

        if (!EstaAbortado()) {
                try {
                        //se le da la oportunidad a la vista de que haga cosas asincronas con la interfaz, por ejemplo reordenar ficheros
                        CargaParams->IVista->CargarEstudio(this);
                        if (CargaParams->LoadFirst) {
                                GNC::GCS::Ptr<GNC::GCS::IStudyContext> estudio = CargaParams->IVista->GetEstudio();

                                if (!estudio->GetImagePaths().empty()) {
                                        estudio->Loader->SetInput(estudio->GetImagePaths().front());
                                } else {
                                        Error = _Std("Series path couldn't be found");
                                }
                        }
                } catch( GNC::GCS::VistaException& ex ) {
                        Error = _Std("Exception loading:") + " " + ex.GetCause();
                } catch (GNC::GCS::ControladorCargaException& ex) {
                        Error = _Std("Exception loading:") + " " + ex.GetCause();
                } catch (...) {
                        Error = _Std("Study malformed or unsupported");
                }
        }

        //GTRACE("Saliendo comando carga ");
}

void GADAPI::ComandoCarga::Update()
{

        bool detener = false;

        if (Error.size() == 0) {

                if (CargaParams->IVista == NULL) {
                        LOG_ERROR("ControladorCarga", "Vista destruida sin notificar al proceso de carga. Inconsistencia interna");
                        return;
                }

                try {
                        LOG_TRACE("ControladorCarga", "Se invoca el iniciar pipeline de la vista");
                        CargaParams->IVista->IniciarPipeline();

                        CargaParams->IVista->SetCargada(true);
                        GNC::GCS::ControladorVistas::Instance()->SetTitulo(CargaParams->IVista, CargaParams->IVista->GetTitulo());
                        CargaParams->IVista->OnCargaFinalizada();
                } catch (GNC::GCS::VistaException& ex) {
                        Error = _Std("Error loading study: ") + ex.GetCause();
                        detener = true;
                } catch (const std::bad_alloc&) {
                        Error = _Std("Error loading study insufficient memory");
                        detener = true;
                } catch (const std::exception& ex) {
                        Error = _Std("Error loading study: ") + std::string(ex.what());
                        detener = true;
                } catch (...) {
                        Error = _Std("Error loading study.");
                        detener = true;
                }
        } else {
                LOG_DEBUG("ControladorCarga",  _Std("Load canceled by user"));
                detener = true;
        }

        if (detener) {
                if (Error.size() > 0) {
                        LOG_ERROR("ControladorCarga", Error);
                        wxMessageBox(wxString::FromUTF8(Error.c_str()), _("Error loading study"), wxICON_ERROR);
                }
        }


}

