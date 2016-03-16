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

#include <api/globals.h>
#include <api/helpers/helpertexto.h>
#include <api/westilo.h>
#include <api/controllers/icontroladorlog.h>
#include "openglhelper.h"
#include <sstream>
#include <string>
#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkgl.h>
#include <cairo/cairo.h>

//--------------------------------------------------------------------------------------------

GNC::GCS::TexturaCairo::TexturaCairo() {
	idTextura         = (unsigned int)-1;
	contextoCreado    = false;
	texturaCargada    = false;
	texturaModificada = false;
	ancho             = 0;
	alto              = 0;
	anchoPOT          = 2;
	altoPOT           = 2;
	numCanales        = 0;
	rowStride         = 0;
	pPixelData        = NULL;
	cs                = NULL;
	cr                = NULL;
    opacidad          = 0.5f;
	m_Interpolar      = false;
	trel.Asignar(1.0f, 1.0f);
}

GNC::GCS::TexturaCairo::~TexturaCairo()
{
	Descargar();
	Destruir();
}

void GNC::GCS::TexturaCairo::Redimensionar(unsigned int anchoTextura, unsigned int altoTextura) {

	texturaModificada = true;
	if (!contextoCreado || anchoTextura != ancho || altoTextura != alto) {
		//std::cout << "Actualizando " << anchoTextura << "x" << altoTextura << std::endl;
		Destruir();

		GTRACE("Creando textura de: " << anchoTextura << "x" << altoTextura);

		ancho = anchoTextura;
		anchoPOT = NearestPOT(anchoTextura);
		alto = altoTextura;
		altoPOT = NearestPOT(altoTextura);

		GTRACE("Creando textura POT de: " << anchoPOT << "x" << altoPOT);

		trel.Asignar((double)anchoTextura / (double) anchoPOT, (double)altoTextura / (double) altoPOT);

		numCanales = 4;
		rowStride = anchoPOT * numCanales;
		pPixelData = (unsigned char*) calloc(rowStride * altoPOT, sizeof(unsigned char));
		cs = cairo_image_surface_create_for_data(pPixelData, CAIRO_FORMAT_ARGB32, anchoPOT, altoPOT, rowStride);
		if (cairo_surface_status(cs) != CAIRO_STATUS_SUCCESS) {
			std::cerr << "Error al crear surface" << std::endl;
			free(pPixelData);
			pPixelData = NULL;
			cairo_surface_destroy(cs);
			cs =  NULL;
		}
		else {
			GTRACE("Superficie creada " << cs);
			cr = cairo_create(cs);
			if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
				std::cerr << "Error al crear contexto" << std::endl;
				free(pPixelData);
				pPixelData = NULL;
				cairo_destroy(cr);
				cr = NULL;
				cairo_surface_destroy(cs);
				cs = NULL;
			}
			else {
				GTRACE("Contexto creado " << cr);
				contextoCreado = true;
			}
			//std::cout << "Contexto creado: " << ancho << "x" << alto << std::endl;
		}
	}
	else {
		//std::cout << "No necesita actualizar (" << ancho << "x" << alto << ")" << std::endl;
	}
}

void GNC::GCS::TexturaCairo::Destruir() {

	if (texturaCargada) {
		Descargar();
	}
	if (contextoCreado) {

		if(cairo_status(cr) == CAIRO_STATUS_SUCCESS) {
			GTRACE("Contexto destruido: " << cr);
			cairo_destroy(cr);
			cr = NULL;
		}
		if(cairo_surface_status(cs) == CAIRO_STATUS_SUCCESS) {
			GTRACE("Superficie destruida: " << cs);
			cairo_surface_destroy(cs);
			cs = NULL;
		}
		if (pPixelData != NULL) {
			free(pPixelData);
			pPixelData = NULL;
		}
		contextoCreado = false;
	}
}

void GNC::GCS::TexturaCairo::Cargar() {
	if (!contextoCreado) {
		std::cerr << "Error al cargar textura: Contexto no  creado" << std::endl;
		return;
	}
	if (texturaCargada) {
		return;
	}

	glEnable(GL_TEXTURE_2D);

	//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
	//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
	//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_DONT_CARE);

	glGenTextures(1, &idTextura);

	GTRACE("Textura cargada: " << ancho << "x" << alto << ", " << idTextura);

	glBindTexture(GL_TEXTURE_2D, idTextura);
	if (m_Interpolar) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, anchoPOT, altoPOT, 0, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	texturaCargada = true;
	texturaModificada = false;
	//std::cout << "Textura cargada: " << ancho << "x" << alto << std::endl;
}

void GNC::GCS::TexturaCairo::Descargar() {
	if (!contextoCreado) {
		return;
	}
	if (!texturaCargada) {
		return;
	}

	glEnable(GL_TEXTURE_2D);

	GTRACE("Textura descargada: " << ancho << "x" << alto << ", " << idTextura);

	glDeleteTextures(1, &idTextura);

	glDisable(GL_TEXTURE_2D);

	idTextura = (unsigned int)-1;

	texturaCargada = false;
	texturaModificada = false;
}

void GNC::GCS::TexturaCairo::Actualizar() {
	if (!texturaCargada) {
		Cargar();
		return;
	}
	if (!texturaModificada) {
		return;
	}

	glEnable(GL_TEXTURE_2D);

	if (m_Interpolar) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

	//glHint(GL_TEXTURE_COMPRESSION_HINT, GL_FASTEST);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, anchoPOT, altoPOT, GL_RGBA, GL_UNSIGNED_BYTE, pPixelData);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

	texturaModificada = false;
}

void GNC::GCS::TexturaCairo::Render(const GNC::GCS::Vector* const quad, bool flipHorizontal, bool flipVertical, double rotacionCamara) const
{
	/*
	 * Posicion del Quad
	 *  q3   q2
	 *  q0   q1
	 * */

	glEnable(GL_TEXTURE_2D);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glBegin(GL_QUADS);

	if (flipVertical) {
		if (flipHorizontal) { // Se actua como si no se hiciese flipH
			//std::cout << "Render flipV & flipH" << std::endl;
			
			if (rotacionCamara != 0.0 ){}
			
			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(trel.x,  0);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(trel.x, trel.y);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(0.0f, trel.y);
			glVertex2d(quad[1].x, quad[1].y);
		}
		else {
			//std::cout << "Render flipV & !flipH" << std::endl;
			
			if (rotacionCamara != 0.0 ){}

			// Se actua como si se hiciese flipH
			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(trel.x,  0);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(trel.x, trel.y);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(0.0f, trel.y);
			glVertex2d(quad[0].x, quad[0].y);

		}
	}
	else {
		if (flipHorizontal) {
			//std::cout << "Render !flipV & flipH" << std::endl;

			if (rotacionCamara != 0.0 ){}

			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(trel.x,  0);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(trel.x, trel.y);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(0.0f, trel.y);
			glVertex2d(quad[2].x, quad[2].y);
		}
		else {
			//std::cout << "Render !flipV & !flipH" << std::endl;

			if (rotacionCamara != 0.0 ){}

			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(trel.x,  0);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(trel.x, trel.y);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(0.0f, trel.y);
			glVertex2d(quad[3].x, quad[3].y);
		}
	}
	
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}


void GNC::GCS::TexturaCairo::Render2D(const GNC::GCS::Vector* const quad)
{

	/*
	 * Posicion del Quad
	 *  q3   q2
	 *  q0   q1
	 * */
	
	glEnable(GL_TEXTURE_2D);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	glBegin(GL_QUADS);

	glTexCoord2d(0.0f, 0.0f);
	glVertex2d(quad[0].x, quad[0].y);

	glTexCoord2d(1.0,  0);
	glVertex2d(quad[1].x, quad[1].y);

	glTexCoord2d(1.0, 1.0);
	glVertex2d(quad[2].x, quad[2].y);

	glTexCoord2d(0.0f, 1.0);
	glVertex2d(quad[3].x, quad[3].y);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);

}


void GNC::GCS::TexturaCairo::Render(GNC::GCS::Contexto3D* c, const GNC::GCS::Vector& pos, bool flipHorizontal, bool flipVertical, double rotacionCamara, bool ajustarAPixeslPantalla, bool aplicarEscala) const
{

	/*
	 * Posicion del Quad
	 *  q3   q2
	 *  q0   q1
	 * */
	
	GNC::GCS::Vector escala;
	if (aplicarEscala) {
		escala = c->RelacionImagenPantalla();
	} else {
		escala.x = 1;
		escala.y = 1;
	}

	GNC::GCS::Vector quad[4];
	
	GNC::GCS::Vector rasterPos = pos.Redondeado();
	 
	quad[0].x = rasterPos.x;
	quad[0].y = rasterPos.y;
	
	quad[2].x = quad[0].x + (double)anchoPOT * escala.x;
	quad[2].y = quad[0].y + (double)altoPOT * escala.y;
	
	quad[1].x = quad[2].x;
	quad[1].y = quad[0].y;
	
	quad[3].x = quad[0].x;
	quad[3].y = quad[2].y;
	
	glEnable(GL_TEXTURE_2D);
	
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, idTextura);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);

	if (ajustarAPixeslPantalla) 
	{
		AjustarAPixelsPantalla(c, quad, 4);
	}

	glBegin(GL_QUADS);
	if (flipVertical) {
		if (flipHorizontal) { // Se actua como si no se hiciese flipH
			//std::cout << "Render2 flipV & flipH" << std::endl;
			
			if (rotacionCamara != 0.0 ){}
			
			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(1.0,  0);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(0.0f, 1.0);
			glVertex2d(quad[1].x, quad[1].y);
		}
		else {
			//std::cout << "Render2 flipV & !flipH" << std::endl;
			
			if (rotacionCamara != 0.0 ){}

			// Se actua como si se hiciese flipH
			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(1.0,  0);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(0.0f, 1.0);
			glVertex2d(quad[0].x, quad[0].y);

		}
	}
	else {
		if (flipHorizontal) {
			//std::cout << "Render2 !flipV & flipH" << std::endl;

			if (rotacionCamara != 0.0 ){}

			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(1.0,  0);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(quad[3].x, quad[3].y);

			glTexCoord2d(0.0f, 1.0);
			glVertex2d(quad[2].x, quad[2].y);
		}
		else {
			//std::cout << "Render2 !flipV & !flipH" << std::endl;

			if (rotacionCamara != 0.0 ){}

			glTexCoord2d(0.0f, 0.0f);
			glVertex2d(quad[0].x, quad[0].y);

			glTexCoord2d(1.0,  0);
			glVertex2d(quad[1].x, quad[1].y);

			glTexCoord2d(1.0, 1.0);
			glVertex2d(quad[2].x, quad[2].y);

			glTexCoord2d(0.0f, 1.0);
			glVertex2d(quad[3].x, quad[3].y);
		}
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
/*
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glVertex2dv(quad[0]);
	glVertex2dv(quad[2]);
	glEnd();
*/
}

inline unsigned int GNC::GCS::TexturaCairo::NearestPOT(unsigned int size)
{

	unsigned int pot = 1;
	while (pot < size && pot < 0x80000000) {
		pot <<= 1;
	}

	return pot;
}

void GNC::GCS::TexturaCairo::SetFontNormal(const std::string& fontname, float tamFuente)
{
	cairo_font_options_t* options;
	if (cr != NULL) {
		cairo_select_font_face (cr, fontname.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
		cairo_set_font_size(cr, tamFuente);
		options = cairo_font_options_create ();
		cairo_get_font_options(cr, options);
		cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
		cairo_set_font_options(cr, options);
		cairo_font_options_destroy(options);
	}
	else {
		LOG_WARN("Core/RenderAPI", "Unable to set font. Context not created yet");
	}
}

void GNC::GCS::TexturaCairo::SetFontBold(const std::string& fontname, float tamFuente)
{
	cairo_font_options_t* options;
	if (cr != NULL) {
		cairo_select_font_face (cr, fontname.c_str(), CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
		cairo_set_font_size(cr, tamFuente);
		options = cairo_font_options_create ();
		cairo_get_font_options(cr, options);
		cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
		cairo_set_font_options(cr, options);
		cairo_font_options_destroy(options);
	}
	else {
		LOG_WARN("Core/RenderAPI","Unable to set font. Context not created yet");
	}
}

void GNC::GCS::TexturaCairo::SetSourceColor(float b, float g, float r, float a)
{
	cairo_set_source_rgba (cr, r, g, b, a);
}

//--------------------------------------------------------------------------------------------


GNC::GCS::GLHelper::TVector GNC::GCS::GLHelper::calcularBoundingBox(TTextura& tc, const std::string& texto, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
	if (!tc.contextoCreado) {
		tc.Redimensionar(2, 2);
	}
	return GNC::GCS::Widgets::HelperTexto::procesarTexto(tc.cr, texto, NULL, withShadow, anchoMaximo, alineamiento, false);
}

GNC::GCS::GLHelper::TVector GNC::GCS::GLHelper::dibujarTexto(TTextura& tc, const std::string& texto, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
	tc.texturaModificada = true;
	return GNC::GCS::Widgets::HelperTexto::procesarTexto(tc.cr, texto, NULL, withShadow, anchoMaximo, alineamiento, true);
}

GNC::GCS::GLHelper::TVector GNC::GCS::GLHelper::dibujarTexto(TTextura& tc, const std::string& texto, const TColor& c, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
	tc.texturaModificada = true;
	return GNC::GCS::Widgets::HelperTexto::procesarTexto(tc.cr, texto, &c, withShadow, anchoMaximo, alineamiento, true);
}

//--------------------------------------------------------------------------------------------
