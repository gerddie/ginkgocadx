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

#include <cmath>
#include <cstring>

#include <api/globals.h>
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/iwidgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <eventos/modificacionimagen.h>
#include <api/math/geometry3d.h>
#include <api/controllers/ieventscontroller.h>
#include <api/westilo.h>

#include "wlupa.h"


#include <vtkgl.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtkBMPWriter.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <wx/defs.h>

//region "Implementacion de WBuilder"

GNC::GCS::Widgets::WLupaBuilder::WLupaBuilder(GNC::GCS::IWidgetsManager* pManager, GNC::GCS::Widgets::ModoLupa modo, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid), IObservadorWidget(-1, gid)
{
	m_ModoActivo = modo;
	m_pLupaActiva = NULL;

	m_pManager->InsertarObservador(this);

	// Comprobamos si existe alguna lupa para esta vista, y de ser asi la asociamos.
	bool notfound = true;
	for( GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); notfound && it != m_pManager->GetListaWidgets().end(); ++it) {
		GNC::GCS::Widgets::IWidget* w = *it;
		if (w->GetVID() == -1 && w->GetGID() == m_GID) {
			m_pLupaActiva = dynamic_cast<GNC::GCS::Widgets::WLupa*> (w);
			if (m_pLupaActiva != NULL) {
				m_pLupaActiva->SetModo(m_ModoActivo);
				m_pLupaActiva->SetRendererActivo(m_pManager->GetRendererActivo());
				notfound = true;
			}
		}
	}
}

GNC::GCS::Widgets::WLupaBuilder::~WLupaBuilder()
{
	m_pManager->EliminarObservador(this);
}

void GNC::GCS::Widgets::WLupaBuilder::OnWidgetDestruido(GNC::GCS::Widgets::IWidget* w) {
	if (m_pLupaActiva == w) {
		m_pLupaActiva = NULL;
	}
}

void GNC::GCS::Widgets::WLupaBuilder::SetModo(GNC::GCS::Widgets::ModoLupa modo)
{
	m_ModoActivo = modo;
	if (m_pLupaActiva != NULL) {
		m_pLupaActiva->SetModo(m_ModoActivo);
	}
}

//region "Interfaz especifica"
void GNC::GCS::Widgets::WLupaBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}

	else if (evento.ButtonDown(m_ButtonMask)) {
		m_PosicionCursor= evento.iP;
		if (m_pLupaActiva == NULL) {
			const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();
			m_pLupaActiva = new WLupa(evento.c->GetRenderer(), GNC::GCS::Vector(factor.x, factor.y), m_PosicionCursor, m_ModoActivo, m_pManager, -1, "Lupa", m_GID);
			GNC::GCS::Events::EventoModificacionImagen evt2(m_pManager->GetVista());
			GNC::GCS::IEventsController::Instance()->Registrar(m_pLupaActiva, evt2);
			m_pManager->InsertarWidget(m_pLupaActiva, true);
			m_pLupaActiva->SetMouseDown(true);
			m_pLupaActiva->OnMouseEvents(evento);
			m_pLupaActiva->SetPosCursor(m_PosicionCursor);
			m_pLupaActiva->DesplazarA(m_PosicionCursor);

		}
		else {
			m_pLupaActiva->SetRendererActivo(evento.c->GetRenderer());
			m_pLupaActiva->DesplazarA(m_PosicionCursor);
			m_pLupaActiva->SetMouseDown(true);
			m_pLupaActiva->OnMouseEvents(evento);
		}

		ConsumirEvento();
	}

	GTRACE("GNC::GCS::Widgets::WLupaBuilder::OnMouseEvents(wxMouseEvent&)");
}

void GNC::GCS::Widgets::WLupaBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
	GTRACE("GNC::GCS::Widgets::WLupaBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WLupaBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{
	return;
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WLupaBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_LUPA;
}

//endregion

//-----------------------------------------------------------------------------------------------

//region "Constructor y destructor"

GNC::GCS::Widgets::WLupa::WLupa(GNC::GCS::IWidgetsRenderer* pRenderer, const GNC::GCS::Vector& relacionImagenPantalla, GNC::GCS::Vector& puntoAnclaje, ModoLupa modo, IWidgetsManager* pManager, long vid, const char* nombre, long gid) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid)
{
	m_TopLevel = true;

	m_ModoActivo = modo;
	m_MouseDown = false;
	m_pRenderer = pRenderer;

	m_RelacionImagenPantalla = relacionImagenPantalla;

	m_RadioLente          = 75.0f; // 75 pixels-pantalla
	m_GrosorLupa          = 10.0f; // 10 pixels-pantalla
	m_GrosorMango         = 30.0f; // 30 pixels-pantalla

	m_AnguloPosAsa        = (M_PI / 4.0f); // 314 grados ( 45 grados )
	m_AnguloAsa           = M_PI / 24.0f; // 7.5 grados

	m_NumVertices = 60;
	m_AnguloVertices = (float)M_2PI / (float)m_NumVertices;
	m_VCoordsInteriores = new GNC::GCS::Vector[m_NumVertices];
	m_VInteriores = new GNC::GCS::Vector[m_NumVertices];
	m_VExteriores = new GNC::GCS::Vector[m_NumVertices];


	m_PosicionCursor = puntoAnclaje;

	GNC::GCS::Vector vDir = GNC::GCS::Vector::VectorDeRotacion(m_AnguloPosAsa);
	float radio = (m_RadioLente + m_GrosorLupa);
	GNC::GCS::Vector offset = (vDir * radio) / relacionImagenPantalla.NormaInfinito() ;

	m_Centro.Asignar(-offset.x, -offset.y);

	Recalcular();

}

GNC::GCS::Widgets::WLupa::~WLupa()
{
	delete[] m_VCoordsInteriores;
	m_VCoordsInteriores = NULL;
	delete[] m_VInteriores;
	m_VInteriores = NULL;
	delete[] m_VExteriores;
	m_VExteriores = NULL;
}

//endregion

//region "Interfaz especifica"

void GNC::GCS::Widgets::WLupa::SetModo(GNC::GCS::Widgets::ModoLupa modo)
{
	if (m_ModoActivo != modo) {
		m_ModoActivo = modo;
		Modificar(true);
	}
}

void GNC::GCS::Widgets::WLupa::SetRendererActivo( GNC::GCS::IWidgetsRenderer* r)
{
	if (m_pRenderer != r) {
		if (m_pRenderer != NULL) {
			m_pRenderer->Modificar(true);
		}
		m_pRenderer = r;
	}
}


//endregion

//region "Interfaz generica"

void GNC::GCS::Widgets::WLupa::Modificar(bool modificar)
{
	m_Modificado = modificar;
	if (m_Modificado && m_pRenderer != NULL) {
		m_pRenderer->Modificar(true);
	}

}

void GNC::GCS::Widgets::WLupa::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if(m_Oculto || evento.c == NULL || evento.c->GetRenderer() != m_pRenderer){
		return;
	}

	//std::cerr << "GNC::GCS::Widgets::WLupa::OnMouseEvents(" << evento.c << ")" << std::endl;

	if(EstaOculto()){
		return;
	}
	//------------------------------------------------------
	// Leaving
	if (evento.Leaving() ) {
		Iluminar(false);
	}
	//------------------------------------------------------
	// Entering
	else if (evento.Entering() ) {
		if (m_MouseDown) {
			Iluminar(true);
		}
	}
	//------------------------------------------------------
	// Dragging
	else if (evento.Dragging() && m_MouseDown) {

		//if (EstaSeleccionado()) {
		Vector delta = evento.iP - m_PosCursor;
		m_PosCursor = evento.iP ;

			Desplazar(delta);
			ConsumirEvento();
		//}
		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {

		if (EventoConsumido() && !evento.m_controlDown) {
			Seleccionar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor= evento.iP;

		if (HitTest(evento.iP.x, evento.iP.y, GNC::GCS::Vector())) {
			dentro = true;
			if (evento.m_controlDown) { // Inversion de seleccion
				InvertirSeleccion();
			}
			else{
				Seleccionar(true);
			}
		}
		else {
			if (!evento.m_controlDown) {
				Seleccionar(false);
			}
			else {
				ConsumirEvento();
				if (EstaSeleccionado()) {
					m_MouseDown = true;
					m_PosCursor= evento.iP;
				}
			}
		}

		if (dentro) {
			m_MouseDown = true;
			m_PosCursor= evento.iP;
			ConsumirEvento();
		}

		return;
	}
	//------------------------------------------------------
	// LeftUP
	else if (evento.LeftUp()) {
		if (m_MouseDown) {
			m_MouseDown = false;
			ConsumirEvento();
		}
		return;
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) {

		if (EventoConsumido()) {
			Iluminar(false);
			return;
		}

		bool dentro = false;

		if (HitTest(evento.iP.x, evento.iP.y, GNC::GCS::Vector())) {
			Iluminar(true);
			dentro = true;
		}
		else {
			Iluminar(false);
		}

		if (dentro) {
			ConsumirEvento();
		}

	}
}

void GNC::GCS::Widgets::WLupa::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WLupa::HitTest(float x, float y, const GNC::GCS::Vector&)
{
	return GNC::GCS::Vector(x, y).DentroDePoligono2(m_VExteriores, m_NumVertices);
}

bool GNC::GCS::Widgets::WLupa::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	bool dentro = true;

	for (unsigned int i = 0; dentro && i < m_NumVertices; i++) {
		dentro = m_VExteriores[i].DentroDePoligono2(vertices, numVertices);
	}

	return dentro;
}

void GNC::GCS::Widgets::WLupa::Render(GNC::GCS::Contexto3D* c)
{

	if (c == NULL || c->GetRenderer() != m_pRenderer) {
		return;
	}

	//std::cerr << "GNC::GCS::Widgets::WLupa::Render(" << c->GetRenderer() << ")" << std::endl;

	const GNC::GCS::Vector& rel = c->RelacionImagenPantalla();

	if (rel != m_RelacionImagenPantalla) {
		m_RelacionImagenPantalla = rel;
		Recalcular();
	}

	const GNC::GCS::Vector& escala = m_RelacionImagenPantalla;

	//glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

	// Dibujado de sombra de la lupa
	GNC::GCS::Vector::TComp fsombrax = 6.0f * escala.x;
	GNC::GCS::Vector::TComp fsombray = 2.0f * escala.y;
	glBegin(GL_QUAD_STRIP);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glVertex2d( m_VInteriores[i].x + fsombrax,m_VInteriores[i].y - fsombray);
			glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
			glVertex2d( m_VExteriores[i].x + fsombrax,m_VExteriores[i].y -fsombray);
		}
		if (m_NumVertices > 0) {
			glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
			glVertex2d( m_VInteriores[0].x + fsombrax,m_VInteriores[0].y - fsombray);
			glColor4f(0.1f, 0.1f, 0.1f, 0.1f);
			glVertex2d( m_VExteriores[0].x + fsombrax,m_VExteriores[0].y - fsombray);
		}

	glEnd();


	// Dibujado del efecto 3D de la lupa
	glColor4f(0.1f, 0.1f, 0.1f, 1.0f);
	fsombrax = 3.0f * escala.x;
	fsombray = 2.0f * escala.y;
	glBegin(GL_QUAD_STRIP);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glVertex2d( m_VInteriores[i].x + fsombrax, m_VInteriores[i].y - fsombray);
			glVertex2d( m_VExteriores[i].x + fsombrax, m_VExteriores[i].y - fsombray);
		}
		if (m_NumVertices > 0) {
			glVertex2d( m_VInteriores[0].x + fsombrax, m_VInteriores[0].y - fsombray);
			glVertex2d( m_VExteriores[0].x + fsombrax, m_VExteriores[0].y - fsombray);
		}

	glEnd();

	glBegin(GL_LINE_LOOP);
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glVertex2d( m_VExteriores[i].x + fsombrax, m_VExteriores[i].y - fsombray);
		}
	glEnd();


	glDisable(GL_BLEND);

	glColor4f(0.0f, 0.0f, 0.0f, 0.0f);

	double worldPoint[3];
	worldPoint[0] = m_Centro.x;
	worldPoint[1] = m_Centro.y;
	worldPoint[2] = 0.0f;

	float factor = 0.0f;

	if (m_ModoActivo == ML_Aumentar) {
		factor = 0.5f;
	}
	else {
		factor = 2.0f;
	}

	// Dibujado del fondo solido de la lente
	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2d(m_Centro.x, m_Centro.y);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glVertex2d(m_VInteriores[i].x, m_VInteriores[i].y);
		}
		if (m_NumVertices > 0) {
			glVertex2d(m_VInteriores[0].x, m_VInteriores[0].y);
		}

	glEnd();


	long IdTextura = c->GetRenderer()->m_pImageViewer->GetImageTexture();


	if ( IdTextura != 0 && glIsTexture(IdTextura) ){
		int dimensiones[3] = {0, 0, 0};
		int dimensionesTextura[2] = {0, 0};
		double spacing[3] = {0.0, 0.0, 0.0};

		c->GetRenderer()->m_pImageViewer->GetDimensions(dimensiones);
		c->GetRenderer()->m_pImageViewer->GetSpacing(spacing);

		GNC::GCS::Vector dimsImagen(dimensiones);
		GNC::GCS::Vector spacingImagen(spacing);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, IdTextura);

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &(dimensionesTextura[0]) );
		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &(dimensionesTextura[1]) );

		glBindTexture(GL_TEXTURE_2D, 0);

		glDisable(GL_TEXTURE_2D);

		GNC::GCS::Vector dimsTextura(dimensionesTextura);

		GNC::GCS::Vector pi = GNC::GCS::Vector(worldPoint[0], worldPoint[1]);

		GNC::GCS::Vector aumento = GNC::GCS::Vector(factor * m_RadioLentePantalla, factor * m_RadioLentePantalla) / dimsImagen;

		GNC::GCS::Vector vCoordCentro = (pi / dimsTextura);

		//std::cout << "Centro lupa = " << pi << std::endl;
		//std::cout << "Coord centro lupa = " << vCoordCentro << std::endl;

		GNC::GCS::Vector3D rectDis[2];
		
		float angulo = 0.0f;

		for (unsigned int i = 0; i < m_NumVertices; i++, angulo += m_AnguloVertices) {
			m_VCoordsInteriores[i] = ( vCoordCentro + aumento * GNC::GCS::Vector(cos(angulo), sin(angulo)) ) ;
		}

		GNC::GCS::Vector3D v0(0.0, 0.0, 0.0);
		GNC::GCS::Vector3D v1(dimsImagen.x, dimsImagen.y, (double)c->GetRenderer()->m_pImageViewer->GetTindex());

		double rect[2][4] = { {0.0, 0.0, 0.0, 1.0 }, {0.0, 0.0, 0.0, 1.0} };

		c->GetRenderer()->m_pImageViewer->UnProyect2D(v0.v, rect[0]);
		c->GetRenderer()->m_pImageViewer->UnProyect2D(v1.v, rect[1]);

		c->GetRenderer()->WorldToDisplay(rect[0], rectDis[0].v);
		c->GetRenderer()->WorldToDisplay(rect[1], rectDis[1].v);

	#if 0 /* Codigo de depurado visual */
		/*
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex3dv(rectCam[0].v);
		glVertex3dv(rectCam[1].v);
		glEnd();
		*/

		
		glLineWidth(2.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
		glBegin(GL_LINES);
		glVertex3dv(v0.v);
		glVertex3dv(v1.v);
		glEnd();


		// Debug 2

		double viewport[4] = {0.0f, 0.0f, 0.0f, 0.0f}; // { x, y, ancho, alto }. Convenio de coordenadas: {x, y} == {bottom, left}, {ancho, alto} == {top, right}
		glGetDoublev(GL_VIEWPORT, viewport);

		glPushAttrib(GL_ALL_ATTRIB_BITS);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glOrtho( 0, viewport[2] , 0 , viewport[3], -1, 1 );

		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		glVertex3dv(rectDis[0].v);
		glVertex3dv(rectDis[1].v);
		glEnd();

		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		
		glMatrixMode(GL_MODELVIEW);
		
		glPopAttrib();

	#endif


	#if 1 /* Habilitar o deshabilitar scissor */
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
		glScissor(rectDis[0][0], rectDis[0][1], rectDis[1][0] - rectDis[0][0] - 1.0f, rectDis[1][1] - rectDis[0][1] - 1.0f);
		glEnable(GL_SCISSOR_TEST);
	#endif

		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE, GL_REPLACE);

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, IdTextura);

		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, vtkgl::CLAMP_TO_BORDER );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, vtkgl::CLAMP_TO_BORDER );

		// Dibujado de la lente

		glBegin(GL_TRIANGLE_FAN);

			glTexCoord2d(vCoordCentro.x, vCoordCentro.y);
			glVertex2d(m_Centro.x, m_Centro.y);

			angulo = 0.0f;
			for (unsigned int i = 0; i < m_NumVertices; i++, angulo += m_AnguloVertices) {
				glTexCoord2d(m_VCoordsInteriores[i].x , m_VCoordsInteriores[i].y);
				glVertex2d(m_VInteriores[i].x, m_VInteriores[i].y);
			}

			if (m_NumVertices > 0) {
				glTexCoord2d(m_VCoordsInteriores[0].x, m_VCoordsInteriores[0].y);
				glVertex2d(m_VInteriores[0].x, m_VInteriores[0].y);
			}

		glEnd();

		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);

		glDisable(GL_SCISSOR_TEST);

		
		glColor4f(0.0f, 0.5f, 0.0f, 1.0f);
		glLineWidth(1.0f);
		
	}

	glEnable(GL_BLEND);

	// Dibujado de la lupa transparente
	glColor4f(0.2f, 0.2f, 0.2f, 1.0f);
	glBegin(GL_QUAD_STRIP);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glVertex2d( m_VInteriores[i].x,m_VInteriores[i].y);
			glVertex2d( m_VExteriores[i].x,m_VExteriores[i].y);
		}
		if (m_NumVertices > 0) {
			glVertex2d( m_VInteriores[0].x,m_VInteriores[0].y);
			glVertex2d( m_VExteriores[0].x,m_VExteriores[0].y);
		}

	glEnd();

	// Dibujado de detalles

	glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
		for (unsigned int i = 0; i < m_NumVertices; i++) {
			glVertex2d( m_VInteriores[i].x,m_VInteriores[i].y);
		}
	glEnd();

	GNC::GCS::Vector rot = GNC::GCS::Vector::VectorDeRotacion(m_AnguloPosAsa);
	float radio = m_GrosorMangoPantalla;
	GNC::GCS::Vector prayas = rot * (m_RadioLentePantalla);

	for (float inc = 0.25f; inc <= 1.0f; inc += 0.25f) {
		GNC::GCS::Vector p = prayas + (rot * (m_GrosorLupaPantalla + (inc * radio)));
		GNC::GCS::Vector ort = p.VectorOrtonormal() * 10.0f * (1.0f - inc) * escala;
		GNC::GCS::Vector p0 = m_Centro + p - ort;
		GNC::GCS::Vector p1 = m_Centro + p + ort;
		glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
		glBegin(GL_LINES);
		glVertex2d(p0.x, p0.y);
		glVertex2d(p1.x, p1.y);
		glEnd();

		p = prayas + (rot * (escala + m_GrosorLupaPantalla + (inc * radio)));
		ort = p.VectorOrtonormal() * 10.0f * (1.0f - inc) * escala;
		p0 = m_Centro + p - ort;
		p1 = m_Centro + p + ort;
		glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
		glBegin(GL_LINES);
		glVertex2d(p0.x, p0.y);
		glVertex2d(p1.x, p1.y);
		glEnd();
	}

	// Dibujado del outline de iluminacion/seleccion

	glLineWidth(m_LineWidthProp);
	if (m_Seleccionado || m_Iluminado) {
		wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	}
	else {
		glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
	}

	glBegin(GL_LINE_LOOP);

	for (unsigned int i = 0; i < m_NumVertices; i++) {
		glVertex2d( m_VExteriores[i].x,m_VExteriores[i].y);
	}

	glEnd();

	glPointSize(5.0f);
	if (m_Seleccionado) {
		glColor4f(1.0f, 1.0f, 0.0f, 0.25f);
	}
	else if (m_Iluminado) {
		glColor4f(1.0f, 1.0f, 0.0f, 0.25f);
	}
	else {
		glColor4f(0.0f, 0.0f, 0.0f, 0.25f);
	}
	glBegin(GL_POINTS);
		glVertex2d(m_Centro.x, m_Centro.y);
	glEnd();

	glFlush();
}

//endregion

//region "Estado interno"

void GNC::GCS::Widgets::WLupa::Desplazar(const GNC::GCS::Vector& delta)
{
	m_Centro += delta;
	Recalcular();
	m_Modificado = true;
}

void GNC::GCS::Widgets::WLupa::DesplazarA(const GNC::GCS::Vector& posicion)
{
	Desplazar( (posicion - (
			GNC::GCS::Vector::VectorDeRotacion(m_AnguloPosAsa) * (m_RadioLentePantalla + m_GrosorLupaPantalla + (m_GrosorMangoPantalla * 0.5f)))
		) - m_Centro);

	m_PosicionCursor = m_Centro + ( GNC::GCS::Vector::VectorDeRotacion(m_AnguloPosAsa) * (m_RadioLentePantalla + m_GrosorLupaPantalla + (m_GrosorMangoPantalla * 0.5f)));
}

void GNC::GCS::Widgets::WLupa::Recalcular()
{
	m_RadioLentePantalla   = (m_RelacionImagenPantalla * m_RadioLente).NormaInfinito();
	m_GrosorLupaPantalla   = (m_RelacionImagenPantalla * m_GrosorLupa).NormaInfinito();
	m_GrosorMangoPantalla  = (m_RelacionImagenPantalla * m_GrosorMango).NormaInfinito();

	GNC::GCS::Vector modsxy(m_RadioLentePantalla, m_RadioLentePantalla);

	GNC::GCS::Vector rot;
	float angulo = 0.0f;

	for (unsigned int i = 0; i < m_NumVertices; i++, angulo += m_AnguloVertices) {
		rot =  GNC::GCS::Vector::VectorDeRotacion(angulo);
		m_VInteriores[i] = m_Centro + rot * m_RadioLentePantalla;
		m_VExteriores[i] = m_Centro + rot * (m_RadioLentePantalla + m_GrosorLupaPantalla);

		if (angulo < m_AnguloPosAsa - m_AnguloAsa) {
			m_VExteriores[i] = m_Centro + rot * (m_RadioLentePantalla + m_GrosorLupaPantalla);
		}
		/*
		else if (angulo < m_AnguloPosAsa - m_AnguloAsa) // Asa ascendiendo
		{
			m_VExteriores[i] = m_Centro + rot * (m_RadioLentePantalla + m_GrosorLupaPantalla + m_GrosorMangoPantalla);
		}
		*/
		else if (angulo < m_AnguloPosAsa + m_AnguloAsa) // Asa descendiendo
		{
			m_VExteriores[i] = m_Centro + rot * (m_RadioLentePantalla + m_GrosorLupaPantalla + m_GrosorMangoPantalla);
		}
		else {
			m_VExteriores[i] = m_Centro + rot * (m_RadioLentePantalla + m_GrosorLupaPantalla);
		}

	}

	m_Modificado = true;
}


//region Interfaz de eventos ginkgo

void GNC::GCS::Widgets::WLupa::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	if (evt == NULL) {
		std::cerr << "Error: Evento nulo" << std::endl;
		return;
	}
	switch (evt->GetCodigoEvento()) {

		case ginkgoEVT_Core_ModificacionImagen:
			{
				GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
				if (pEvt != NULL) {
					switch (pEvt->GetTipo()) {
						case GNC::GCS::Events::EventoModificacionImagen::ImagenCargada:
							Modificar(true);
							break;
						case GNC::GCS::Events::EventoModificacionImagen::ImagenModificada:
							Modificar(true);
							break;
						case GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada:
							Modificar(true);
							break;
						case  GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
							{
							}
							break;
						case GNC::GCS::Events::EventoModificacionImagen::ImagenDescargada:
							Modificar(true);
							break;
						case GNC::GCS::Events::EventoModificacionImagen::SliceCambiado:
							Modificar(true);
							break;
						case GNC::GCS::Events::EventoModificacionImagen::MapaModificado:
							Modificar(true);
							break;
						case GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas:
							break;
						default:
							break;
					}
				}
			}
			break;
	}
}

//endregion

