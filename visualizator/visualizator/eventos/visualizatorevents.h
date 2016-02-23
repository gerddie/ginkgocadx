/*
 *  Ginkgo CADx Project
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


#pragma once
#define ginkgoAmbitoAtencionPrimaria  2

#define ginkgoEVT_GNKVisualizator_EventoSincronizacion			 3001
#define ginkgoEVT_GNKVisualizator_EventoRefrescarListaOverlays	 3002
#define ginkgoEVT_GNKVisualizator_EventoMostrarOverlay			 3003
#define ginkgoEVT_GNKVisualizator_EventoZoom							 3004
#define ginkgoEVT_GNKVisualizator_ChangePositionEvent				 3005

#include "sincronizacion.h"
#include "overlays.h"
#include "zoom.h"
#include "changeposition.h"
