/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-13 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include "ioverlayscontract.h"

GNKVisualizator::IOverlaysContract::IOverlaysContract(){
	m_pView = NULL;
}
GNKVisualizator::IOverlaysContract::~IOverlaysContract() {
	m_EstadoOverlays.clear();
}

void GNKVisualizator::IOverlaysContract::Setup(GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& pListaOverlays, GNC::GCS::IVista* pView)
{
	m_pListaOverlays = pListaOverlays;
	m_pView = pView;
	m_EstadoOverlays.clear();
	if(m_pListaOverlays.IsValid()) {
		for(GNKVisualizator::TListaOverlays::iterator it = pListaOverlays->begin(); it!=pListaOverlays->end(); ++it) {
			m_EstadoOverlays[it->indice] = false;
		}
	}
}

GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& GNKVisualizator::IOverlaysContract::GetOverlayList()
{
	return m_pListaOverlays;
}