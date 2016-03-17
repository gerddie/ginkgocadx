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
#include <api/api.h>
#include <api/math/geometry.h>
#include <api/iwidgets.h>

//region Forward declarations
struct _cairo_surface;
struct _cairo;

namespace GNC
{
namespace GCS
{
class Contexto3D;
}
}
//endregion

namespace GNC
{
namespace GCS
{
namespace Widgets
{
class EXTAPI HelperTexto
{
public:
        typedef GNC::GCS::Vector           TVector;
        typedef GNC::GCS::Widgets::Color   TColor;
        typedef GNC::GCS::Contexto3D       TContexto;

        typedef enum TAlineamiento {
                TA_Izquierda,
                TA_Derecha,
                TA_Centrado,
                TA_Justificado
        } TAlineamiento;

        /* Calcula el bounding box del texto sobre cairo para la fuente seleccionada. Realiza word-wrapping. */
        static TVector calcularBoundingBox(TContexto* c, const std::string& texto, bool withShadow, float anchoMaximo = -1, TAlineamiento alineamiento = TA_Izquierda);

        /* Dibuja el texto en el contexto con fuente seleccionada. Realiza word-wrapping. */
        static TVector dibujarTexto(TContexto* c, const std::string& texto, bool withShadow, float anchoMaximo = -1, TAlineamiento alineamiento = TA_Izquierda);

        /* Dibuja el texto en el contexto con fuente seleccionada y color especificado. Realiza word-wrapping. */
        static TVector dibujarTexto(TContexto* c, const std::string& texto, const TColor& col, bool withShadow, float anchoMaximo = -1, TAlineamiento alineamiento = TA_Izquierda);

        /* Realiza el procesamiento del texto. Calcular el Bounding Box y opcionalmente pintar */
        static TVector procesarTexto(_cairo* cr, const std::string& texto, const TColor* col, bool withShadow, float anchoMaximo = -1, TAlineamiento alineamiento = TA_Izquierda, bool dibujar = true);
};
}
}
}
