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
