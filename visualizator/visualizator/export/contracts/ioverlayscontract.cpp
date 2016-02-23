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
