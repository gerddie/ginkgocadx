/*
 *  
 *  $Id: iwidgetsrenderer.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "iwidgetsrenderer.h"
#include "iwidgetsmanager.h"
#include "ioverlay.h"
#include "icontexto.h"


GNC::GCS::IWidgetsRenderer::IWidgetsRenderer()
{
	m_pManager = NULL;
	m_pImageViewer = NULL;
	m_pPicker = NULL;
	m_pRepresentation = NULL;
	m_c = NULL;
	m_pIOverlay = NULL;
	m_VID = 0;
	m_Modificado = false;
}

GNC::GCS::IWidgetsRenderer::~IWidgetsRenderer()
{
	if (m_pManager != NULL) {
		m_pManager->EliminarRenderer(this);
		EliminarListener(m_pManager);
	}
}

//----------------------------------------------------------------------------------------------------
//region "Interfaz de impresión de overlays"

void GNC::GCS::IWidgetsRenderer::SetIOverlay(IImpresorOverlay* pOverlay) {
	m_pIOverlay = pOverlay;
}

//endregion


//----------------------------------------------------------------------------------------------------
//region "Interfaz de configuracion de vistas/slices"

void GNC::GCS::IWidgetsRenderer::Activar()
{
	OnActivar();
	if (m_pManager != NULL) {
		m_pManager->SetRendererActivo(this);
	}
}

/* Establece el id de sub-vista activo. Los widgets se asocian a un VID */
void GNC::GCS::IWidgetsRenderer::SetVID(long vid, bool render)
{
	if (m_VID != vid) {
		//long oldVID = m_VID;
		m_VID = vid;
		if (render) {
			PostRender();
		}
	}
}

/* Propaga la notificación de activación de este renderer hacia las clases base */
void GNC::GCS::IWidgetsRenderer::OnActivar()
{
	
}

//endregion

//----------------------------------------------------------------------------------------------------
//region "Interfaz de configuracion del manager"

void GNC::GCS::IWidgetsRenderer::SetManager(IWidgetsManager* pManager)
{
	if (m_pManager != pManager) {
		IWidgetsManager* oldManager = m_pManager;

		m_pManager = pManager;

		if (oldManager != NULL) {
			oldManager->EliminarRenderer(this);
			EliminarListener(oldManager);
		}

		if (m_pManager != NULL) {
			m_pManager->InsertarRenderer(this);
			InsertarListener(m_pManager);
		}
	}
}

void GNC::GCS::IWidgetsRenderer::SetRepresentation(WidgetRepresentation* pWidget) {
	m_pRepresentation = pWidget;
}

//endregion
