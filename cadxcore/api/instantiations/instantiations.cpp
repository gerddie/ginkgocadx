/*
 *  
 *  $Id: instantiations.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-12 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <string>
#include <list>
#include <map>
#include <api/math/geometry.h>
#include <api/math/geometry3d.h>

#include "instantiations.h"

#if defined(GEOMETRY2D_NO_TEMPLATE)

#else
namespace GNC {
	namespace GCS {

		//EXTAPI_TEMPLATE template GVector<PRECISION_COMPONENTE_POR_DEFECTO,PRECISION_REALES_POR_DEFECTO>;
		//EXTAPI_TEMPLATE template GMatriz<PRECISION_COMPONENTE_POR_DEFECTO,PRECISION_REALES_POR_DEFECTO>;
	}
}
#endif
