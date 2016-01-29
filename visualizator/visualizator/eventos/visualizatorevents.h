/*
 *  
 *  $Id: visualizatorevents.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
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
