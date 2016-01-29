/*
 *  
 *  $Id: geometry_defs.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#include <cmath>

#define PRECISION_COMPONENTE_POR_DEFECTO double
#define PRECISION_REALES_POR_DEFECTO     double
//#define EXACT_COMPARION // use exact comparison with == or treat nearly distances (< epsilon) as equal.

#ifdef _WIN32
#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif
#endif
#ifndef M_PI_4
#define M_PI_4     M_PI * 0.25f
#endif
#ifndef M_2PI
#define M_2PI     6.28318530717958647692
#endif
