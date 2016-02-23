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
#include <api/controllers/icontroladorvistas.h>
#include <api/imodelointegracion.h>
#include <api/controllers/icontroladorcarga.h>
#include <main/entorno.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/controladorlog.h>
#include "comandodestruirvista.h"

#define IDC_DESTRUIR       101

GADAPI::ComandoDestruirVistaParams::ComandoDestruirVistaParams(GNC::GCS::IVista* pVista)
{
	m_pVista = pVista;
}

GADAPI::ComandoDestruirVistaParams::~ComandoDestruirVistaParams()
{
}


GADAPI::ComandoDestruirVista::ComandoDestruirVista(ComandoDestruirVistaParams* pParams) : IComando(pParams, "ComandoDestruirVista")
{
	GTRACE(">> ComandoDestruirVista::ComandoDestruirVista(): " << this);
	m_pDestruirParams = pParams;
	GTRACE("<< ComandoDestruirVista::ComandoDestruirVista(): " << this);
}

void GADAPI::ComandoDestruirVista::Execute()
{

}

void GADAPI::ComandoDestruirVista::Update()
{
	if (!EstaAbortado()) {
		//este destruir no destruye bien la ventana, hay que invocar a ventana principal!!

		//GNC::GCS::ControladorVistas::Instance()->Destruir(m_pDestruirParams->m_pVista);
	}
}

