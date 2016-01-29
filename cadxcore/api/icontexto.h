/*
 *  
 *  $Id: icontexto.h $
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

// Forward Declarations

struct _cairo;
struct _cairo_surface;

class vtkGinkgoImageViewer;
namespace GNC {
	namespace GCS {
		class IWidgetsRenderer;
	}
}

class wxWindow;

// Forward Declarations

namespace GNC {
	namespace GCS {

		typedef enum TipoProyeccion {
			TP_Ortogonal,
			TP_Perspectiva
		} TipoProyeccion;

		class EXTAPI Contexto3D{
		public:

			/* Dimension en las coordenadas x del viewport */
			unsigned int ancho;
			/* Dimension en las coordenadas y del viewport */
			unsigned int alto;

			// Propiedades para Offscreen Rendering

			double           spacing[3];
			double           origin[3];
			unsigned char*   pixelData;
			_cairo*          cr;
			_cairo_surface*  cs;
			bool             offscreenCreado;
			long             OVID; // VID offscreen
			GNC::GCS::Vector factorReescalado; // Factor de reescalado de la imagen. Para corregir coordenadas mundo al redimensionar.

			/* Puntero a wxWindow del renderer para permitir enparentar wxWidgets (Ventanas, dialogos, etc...) */
			wxWindow*              pWindow;
			IWidgetsRenderer*      pRenderer;
			vtkGinkgoImageViewer*  pOffscrenViewer;


		protected:
			/* Relacion coordenada mundo => coordenada pantalla*/
			GNC::GCS::Vector       m_RelacionMundoPantallaOffscreen;

			GNC::GCS::Vector       m_RelacionImagenPantalla;

			

		public:

			/* Rotacion y posicion de la camara */
			double rotacion;
			bool   flipHorizontal;
			bool   flipVertical;
			
			GNC::GCS::Vector ScreenDirectionCosines[2]; /** Director unit vectors (image plane coordinates) for screen coordinates **/

			/* Tipo de Proyeccion (Para calcular la norma) */
			TipoProyeccion tipoProyeccion;

			Contexto3D(TipoProyeccion proyeccion = TP_Ortogonal);

			~Contexto3D();

			void Reset();

			void SetupImageScreenRel(double relX, double relY, double relZ);

			GNC::GCS::Vector& RefRelacionImagenPantalla();

			const GNC::GCS::Vector& RelacionImagenPantalla() const;
			
			GNC::GCS::Vector& RefRelacionMundoPantallaOffscreen();
			
			const GNC::GCS::Vector& RelacionMundoPantallaOffscreen() const;

			bool IsOk() const;

			IWidgetsRenderer* GetRenderer() const;

			void CrearOffscreen();

			void DestruirOffscreen();

			GNC::GCS::Vector Reproyectar(const GNC::GCS::Vector& v);

		};
	}
}
