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
#include <api/math/geometry.h>
#include <api/iwidgets.h>
#include <api/helpers/helpertexto.h>
#include <api/api.h>
#include <vtkgl.h>

#ifndef GL_TEXTURE_COMPRESSION_HINT
#define GL_TEXTURE_COMPRESSION_HINT 0x84EF
#endif
#ifndef GL_COMPRESSED_RGBA
#define GL_COMPRESSED_RGBA 0x84EE
#endif
#ifndef GL_COMPRESSED_LUMINANCE
#define GL_COMPRESSED_LUMINANCE 0x84EA
#endif

struct _cairo_surface;
struct _cairo;

namespace GNC
{
	namespace GCS
	{

		class EXTAPI TexturaCairo {
		public:

			/* Dimension en las coordenadas x de la imagen */
			unsigned int ancho;
			/* Dimension en las coordenadas y de la imagen */
			unsigned int alto;
			/* Dimension en las coordenadas x de la imagen como potencia de 2 mas cercana */
			unsigned int anchoPOT;
			/* Dimension en las coordenadas y de la imagen como potencia de 2 mas cercana*/
			unsigned int altoPOT;
			/* Numero de canales de la imagen (R,G,B..) */
			unsigned int numCanales;
			/* Longitud de una fila */
			unsigned int rowStride;
			/* Imagen sobre la que escribe Cairo */
			unsigned char*         pPixelData;
			/* Superficie de Cairo */
			struct _cairo_surface* cs;
			/* Contexto de renderer Cairo*/
			struct _cairo*         cr;

			float opacidad;

			GNC::GCS::Vector trel; // Tamano de la textura en coordenadas normalizadas [0,1]x[0,1]

			bool contextoCreado;

			bool texturaCargada;

			bool texturaModificada;

			unsigned int idTextura;

			bool m_Interpolar;

			TexturaCairo();

			~TexturaCairo();

			void Redimensionar(unsigned int anchoTextura, unsigned int altoTextura);

			void Destruir();

			void Cargar();

			void Descargar();

			void Actualizar();

			inline void Modificar(bool modificada = true)
			{
				texturaModificada = modificada;
			}

			inline void ActivarInterpolacion()
			{
				m_Interpolar = true;
			}

			inline void DesactivarInterpolacion()
			{
				m_Interpolar = false;
			}
			
			inline bool EsValida()
			{
				return ( (cr != NULL) );
			}

			void Render(const GNC::GCS::Vector* const quad, bool flipHorizontal, bool flipVertical, double rotacionCamara) const;
			void Render2D(const GNC::GCS::Vector* const quad);
			void Render(GNC::GCS::Contexto3D* c, const GNC::GCS::Vector& pos, bool flipHorizontal, bool flipVertical, double rotacionCamara, bool ajustarAPixelsPantalla = false, bool aplicarEscala = false) const;

			inline unsigned int NearestPOT(unsigned int size);

			void SetFontNormal(const std::string& fontname, float tamFuente);
			void SetFontBold(const std::string& fontname, float tamFuente);

			void SetSourceColor(float b, float g, float r, float a);

		};

		class EXTAPI GLHelper
		{
		public:
			typedef GNC::GCS::TexturaCairo               TTextura;
			typedef GNC::GCS::Vector                     TVector;
			typedef GNC::GCS::Widgets::Color             TColor;
			typedef GNC::GCS::Widgets::HelperTexto::TAlineamiento TAlineamiento;

			/* Calcula el bounding box del texto sobre cairo para la fuente seleccionada. Realiza word-wrapping. */
			static TVector calcularBoundingBox(TTextura& tc, const std::string& text, bool withShadow, float anchoMaximo = -1.0f, TAlineamiento alineamiento = GNC::GCS::Widgets::HelperTexto::TA_Izquierda);

			/* Dibuja el texto en el contexto cairo con fuente seleccionada. Realiza word-wrapping. */
			static TVector dibujarTexto(TTextura& tc, const std::string& texto, bool withShadow, float anchoMaximo = -1.0f, TAlineamiento alineamiento = GNC::GCS::Widgets::HelperTexto::TA_Izquierda);

			/* Dibuja el texto en el contexto cairo con fuente seleccionada y color especificado. Realiza word-wrapping. */
			static TVector dibujarTexto(TTextura& tc, const std::string& texto, const TColor& c, bool withShadow, float anchoMaximo = -1.0f, TAlineamiento alineamiento = GNC::GCS::Widgets::HelperTexto::TA_Izquierda);
		};
	}
}
