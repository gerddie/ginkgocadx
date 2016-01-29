/*
 *  
 *  $Id: wrejilla.cpp $
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
#include <api/globals.h>
#include "wrejilla.h"
#include <api/icontexto.h>
#include <api/ievento.h>
#include <api/iwidgetsmanager.h>
#include <api/iwidgetsrenderer.h>

#include <cmath>
#include <cstring>

#include <vtkgl.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>

//region "Implementacion de WBuilder"

GNC::GCS::Widgets::WRejillaBuilder::WRejillaBuilder(GNC::GCS::IWidgetsManager* pManager, long gid, double escalaX, double escalaY) : GNC::GCS::Widgets::IWidgetBuilder(pManager, GNC::GCS::TriggerButton(), gid, false), IObservadorWidget(-1, gid)
{
	m_pRejillaActiva = NULL;
	m_EscalaX = escalaX;
	m_EscalaY = escalaY;

	m_pManager->InsertarObservador(this);

	// Comprobamos si existe alguna rejilla para esta vista, y de ser asi la asociamos.
	for( GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it != m_pManager->GetListaWidgets().end(); ++it) {
		GNC::GCS::Widgets::IWidget* w = *it;
		if (w->GetVID() == -1 && w->GetGID() == m_GID) {
			m_pRejillaActiva = dynamic_cast<GNC::GCS::Widgets::WRejilla*> (w);
			if (m_pRejillaActiva != NULL) {
				m_EscalaX = m_pRejillaActiva->GetEscalaX();
				m_EscalaY = m_pRejillaActiva->GetEscalaY();
				return;
			}
		}
	}

}

GNC::GCS::Widgets::WRejillaBuilder::~WRejillaBuilder()
{
	m_pManager->EliminarObservador(this);
}

void GNC::GCS::Widgets::WRejillaBuilder::OnWidgetDestruido(GNC::GCS::Widgets::IWidget* w) {
	if (m_pRejillaActiva == w) {
		m_pRejillaActiva = NULL;
	}
}

bool GNC::GCS::Widgets::WRejillaBuilder::IsRejillaVisible()
{
	if(m_pRejillaActiva == NULL) {
		return false;
	} else {
		return !m_pRejillaActiva->EstaOculto();
	}
}

void GNC::GCS::Widgets::WRejillaBuilder::ActivarRejilla(bool activar)
{
	if (m_pRejillaActiva != NULL) {
		m_pRejillaActiva->Ocultar(!activar);
	} else {
		//se crea y se muestra
		if(activar){
			m_pRejillaActiva = new WRejilla(m_pManager,m_EscalaX, m_EscalaY, -1, "rejilla", m_GID);
			m_pManager->InsertarWidget(m_pRejillaActiva, true);
			m_pRejillaActiva->Ocultar(false);
		}
	}
}

void GNC::GCS::Widgets::WRejillaBuilder::SetEscalaX(double escala)
{
	m_EscalaX = escala;
	if (m_pRejillaActiva != NULL) {
		m_pRejillaActiva->SetEscalaX(escala);
	}
}

void GNC::GCS::Widgets::WRejillaBuilder::SetEscalaY(double escala)
{
	m_EscalaY = escala;
	if (m_pRejillaActiva != NULL) {
		m_pRejillaActiva->SetEscalaY(escala);
	}
}

double GNC::GCS::Widgets::WRejillaBuilder::GetEscalaX()
{
	return m_EscalaX;
}

double GNC::GCS::Widgets::WRejillaBuilder::GetEscalaY()
{
	return m_EscalaY;
}

//region "Interfaz especifica"
void GNC::GCS::Widgets::WRejillaBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& )
{
	if (!m_pManager) {
		return;
	}
	GTRACE("GNC::GCS::Widgets::WRejillaBuilder::OnMouseEvents(wxMouseEvent&)");
}

void GNC::GCS::Widgets::WRejillaBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
	GTRACE("GNC::GCS::Widgets::WRejillaBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WRejillaBuilder::Render(GNC::GCS::Contexto3D* )
{
	return;
}

//endregion

//-----------------------------------------------------------------------------------------------

//region "Constructor y destructor"

GNC::GCS::Widgets::WRejilla::WRejilla(IWidgetsManager* pManager, double escalaX, double escalaY, long vid, const char* nombre, long gid) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
	m_TopLevel = true;
	m_Oculto = true;
	m_EscalaX = escalaX;
	m_EscalaY = escalaY;
}

GNC::GCS::Widgets::WRejilla::~WRejilla()
{

}

//endregion


//region "Interfaz generica"

void GNC::GCS::Widgets::WRejilla::OnMouseEvents(GNC::GCS::Events::EventoRaton& )
{
}

void GNC::GCS::Widgets::WRejilla::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
}

bool GNC::GCS::Widgets::WRejilla::HitTest(float , float , const GNC::GCS::Vector&)
{
	return false;
}

bool GNC::GCS::Widgets::WRejilla::HitTest(GNC::GCS::Vector* , int )
{
	return false;
}

void GNC::GCS::Widgets::WRejilla::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	//esto es necesario ya que la escala esta en coord mundo así lo pasamos a coord imagen
	double spacing[3] = {1.0,1.0,1.0};
	c->GetRenderer()->m_pImageViewer->GetSpacing(spacing);
	GNC::GCS::Vector escalaRejilla(m_EscalaX / spacing[0], m_EscalaY / spacing[1]);
	//

	glPushAttrib(GL_ALL_ATTRIB_BITS);

	float viewport[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // { x, y, ancho, alto }. Convenio de coordenadas: {x, y} == {bottom, left}, {ancho, alto} == {top, right}
	glGetFloatv(GL_VIEWPORT, viewport);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho( 0, viewport[2] , viewport[3] , 0, -1, 1 );

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();


	const GNC::GCS::Vector& relacionImagenPantalla = c->RelacionImagenPantalla();
	GNC::GCS::Vector viewPortPantalla(viewport[2], viewport[3]);
	GNC::GCS::Vector escalaRejillaPantalla = escalaRejilla / relacionImagenPantalla;

	escalaRejillaPantalla.TruncarAMaximo(1.0f);

	GNC::GCS::Vector numCortes = (viewPortPantalla / escalaRejillaPantalla).RedondearBaja();
	if ( ((int) numCortes.x &1 ) == 0) {
		numCortes.x++;
	}
	if ( ((int) numCortes.y &1 ) == 0) {
		numCortes.y++;
	}
	GNC::GCS::Vector rejillaPantalla = escalaRejillaPantalla * numCortes;

	GNC::GCS::Vector rejillaCentrada[2];
	rejillaCentrada[0] = rejillaPantalla.OrigenDeVectorCentradoA(viewPortPantalla);
	rejillaCentrada[1] = rejillaCentrada[0] + rejillaPantalla;

	// Test
	glLineWidth(0.5f);
	glColor4f(0.0f, 0.75f, 0.0f, 0.75f);

	int n;
	float p;

	p = rejillaCentrada[0].x;
	for ( n = 0; n <= (int) numCortes.x; p += escalaRejillaPantalla.x, ++n) {
		glBegin(GL_LINES);
			glVertex3f(p, 0.0f, 0.0f);
			glVertex3f(p, viewPortPantalla.y, 0.0f);
		glEnd();
	}

	p = rejillaCentrada[0].y;
	for (n = 0; n <= (int)numCortes.y; p += escalaRejillaPantalla.y, ++n) {
		glBegin(GL_LINES);
			glVertex3f(0.0f, p, 0.0f);
			glVertex3f(viewPortPantalla.x, p, 0.0f);
		glEnd();
	}

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glPopAttrib();

}

void GNC::GCS::Widgets::WRejilla::Seleccionar(bool )
{
}

void GNC::GCS::Widgets::WRejilla::Iluminar(bool )
{
}

void GNC::GCS::Widgets::WRejilla::Ocultar(bool oculto)
{
	if (m_Oculto != oculto) {
		m_Oculto = oculto;
		m_Modificado = true;
	}
}


//endregion

//region "Estado interno"

void GNC::GCS::Widgets::WRejilla::SetEscalaX(double escala)
{
	if(escala != m_EscalaX) {
		m_EscalaX = escala;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WRejilla::SetEscalaY(double escala)
{
	if(escala != m_EscalaY) {
		m_EscalaY = escala;
		m_Modificado = true;
	}
}

double GNC::GCS::Widgets::WRejilla::GetEscalaX()
{
	return m_EscalaX;
}

double GNC::GCS::Widgets::WRejilla::GetEscalaY()
{
	return m_EscalaY;
}

