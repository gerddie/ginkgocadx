/*
 *  
 *  $Id: helpertexto.h $
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
#include <api/api.h>
#include <api/math/geometry.h>
#include <api/iwidgets.h>

//region Forward declarations
struct _cairo_surface;
struct _cairo;

namespace GNC {
	namespace GCS {
		class Contexto3D;
	}
}
//endregion

namespace GNC
{
	namespace GCS
	{
		namespace Widgets {
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
