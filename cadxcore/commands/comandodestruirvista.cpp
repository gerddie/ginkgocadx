/*
*  
*  $Id: comandodestruirvista.cpp $
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

