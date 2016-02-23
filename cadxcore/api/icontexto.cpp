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


#include <cstdlib>

#include "icontexto.h"
#include <api/math/geometry3d.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <cairo/cairo.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

GNC::GCS::Contexto3D::Contexto3D(TipoProyeccion proyeccion)
{
	
	tipoProyeccion = proyeccion;
	pWindow = NULL;
	pRenderer = NULL;
	pixelData = NULL;
	cr = NULL;
	cs = NULL;
	pOffscrenViewer = NULL;
	offscreenCreado = false;
	OVID = 0;
	Reset();
	
}
GNC::GCS::Contexto3D::~Contexto3D()
{
	Reset();
}

void GNC::GCS::Contexto3D::Reset()
{
	
	ancho           = 0;
	flipHorizontal  = false;
	flipVertical    = false;
	alto            = 0;
	rotacion        = 0;
	spacing[0] = spacing[1] = spacing[2] = 1.0f;
	origin[0] = origin[1] = origin[2] = 0.0f;
	m_RelacionMundoPantallaOffscreen.Asignar(1.0f, 1.0f);
	m_RelacionImagenPantalla.Asignar(1.0f, 1.0f);
	ScreenDirectionCosines[0].Asignar(1.0, 0.0);
	ScreenDirectionCosines[1].Asignar(0.0, 1.0);

	OVID            = 0;
	if ( offscreenCreado) {
		DestruirOffscreen();
	}
}

void GNC::GCS::Contexto3D::SetupImageScreenRel(double relX, double relY, double /*relZ*/)
{
	//GNC::GCS::Vector3D vrel(relX, relY, relZ);
	//std::cout << "Rel : " << vrel << std::endl;
	m_RelacionImagenPantalla.Asignar(relX, relY);
}

GNC::GCS::Vector& GNC::GCS::Contexto3D::RefRelacionImagenPantalla() {
	return m_RelacionImagenPantalla;
}

const GNC::GCS::Vector& GNC::GCS::Contexto3D::RelacionImagenPantalla() const {
	return m_RelacionImagenPantalla;
}

GNC::GCS::Vector& GNC::GCS::Contexto3D::RefRelacionMundoPantallaOffscreen() {
	return m_RelacionMundoPantallaOffscreen;
}

const GNC::GCS::Vector& GNC::GCS::Contexto3D::RelacionMundoPantallaOffscreen() const {
	return m_RelacionMundoPantallaOffscreen;
}

bool GNC::GCS::Contexto3D::IsOk() const {
	return ancho > 0 && alto > 0 && pWindow != NULL && pRenderer != NULL;
}

GNC::GCS::IWidgetsRenderer* GNC::GCS::Contexto3D::GetRenderer() const {
	return pRenderer;
}


void GNC::GCS::Contexto3D::CrearOffscreen()
{
	
	if (offscreenCreado) {
		DestruirOffscreen();
	}

	if (ancho == 0 || alto == 0) {
		return;
	}
	

	const int numCanales = 4;
	int rowStride = ancho * numCanales;
	pixelData = (unsigned char*)std::calloc(rowStride, alto);

	if (pixelData == NULL) {
		return;
	}

	cs = cairo_image_surface_create_for_data(pixelData, CAIRO_FORMAT_ARGB32, ancho, alto, rowStride);
	if (cairo_surface_status(cs) != CAIRO_STATUS_SUCCESS) {
		std::cerr << "Error al crear surface" << std::endl;
		cairo_surface_destroy(cs);
		cs =  NULL;
		free(pixelData);
		pixelData = NULL;
		return;
	}
	
	else {
		cr = cairo_create(cs);
		if (cairo_status(cr) != CAIRO_STATUS_SUCCESS) {
			cairo_destroy(cr);
			cr = NULL;
			cairo_surface_destroy(cs);
			cs = NULL;
			free(pixelData);
			pixelData = NULL;
			return;
		}
	}
	
	//relacion mundo/pantalla
	m_RelacionMundoPantallaOffscreen = GNC::GCS::Vector( ancho, alto) * (1.0f/720.0f);
	
	factorReescalado = GNC::GCS::Vector::Unidad();
	offscreenCreado = true;
}

void GNC::GCS::Contexto3D::DestruirOffscreen()
{
	
	if (!offscreenCreado) {
		return;
	}
	if (cr != NULL) {
		cairo_destroy(cr);
		cr = NULL;
	}
	if (cs != NULL) {
		cairo_surface_destroy(cs);
		cs = NULL;
	}
	if (pixelData != NULL) {
		free(pixelData);
		pixelData = NULL;
	}

	offscreenCreado = false;
	
}

GNC::GCS::Vector GNC::GCS::Contexto3D::Reproyectar(const GNC::GCS::Vector& v)
{

	GNC::GCS::Vector vp = v;

	return vp;
}

