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

#include <wx/xml/xml.h>
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include "wpunto.h"
#include "wtextbox.h"
#include <api/ievento.h>
#include <api/icontexto.h>
#include <api/ioverlay.h>
#include <api/iwidgetsrenderer.h>
#include "main/managers/widgetsmanager.h"
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>


//region "Implementacion de WPuntoBuilder"

GNC::GCS::Widgets::WPuntoBuilder::WPuntoBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
}

GNC::GCS::Widgets::WPuntoBuilder::~WPuntoBuilder()
{
}


void GNC::GCS::Widgets::WPuntoBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	GTRACE("GNC::GCS::Widgets::WPuntoBuilder::OnMouseEvents(wxMouseEvent&)");
	if (evento.ButtonDown(m_ButtonMask)) {
		GNC::GCS::Widgets::WPunto* punto = new GNC::GCS::Widgets::WPunto(m_pManager, evento.c->GetRenderer()->GetVID(), evento.iP, "Punto", m_GID);
		m_pManager->InsertarWidget(punto);
		punto->OnMouseEvents(evento);
		m_pManager->LanzarEventoCreacion(punto);
		m_pManager->Modificado();
		ConsumirEvento();
	}
}

void GNC::GCS::Widgets::WPuntoBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
	GTRACE("GNC::GCS::Widgets::WPuntoBuilder::OnKeyEvents(wxKeyEvent&)");
}


GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WPuntoBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_PUNTO;
}


void GNC::GCS::Widgets::WPuntoBuilder::Render(GNC::GCS::Contexto3D *)
{
	//nothing to do
}
//endregion


//region "Constructor y destructor"

GNC::GCS::Widgets::WPunto::WPunto(IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodo, const char* nombre, long gid, bool oculto) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid,TID_USER_ANNOTATION_WIDGET), 
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_pTextBox(NULL)
{
	m_Nodo = nodo;
	m_Nodo.m_Size = WRADIO_CRUZ;
	m_MouseDown = false;
	m_UID = (long)this;
	m_Oculto = oculto;
	initTextBox();
	Recalcular();
}

GNC::GCS::Widgets::WPunto::~WPunto()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}
//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WPunto::WPunto(IWidgetsManager* pManager, long vid, long gid, wxXmlNode* nodo) : GNC::GCS::Widgets::IWidget(pManager, vid, "Punto", gid) , 
	GNC::GCS::Widgets::IWidgetSerializable(nodo),
	m_pTextBox(NULL)
{
	if(nodo->GetName() != wxT("punto_widget")){
		std::cerr<< "Ha ocurrido un error al desserializar el widget puntos"<<std::endl;
	}

	wxString strTmp = nodo->GetAttribute(wxT("x"),wxT("0.0"));
	double doubleTmp;
	strTmp.ToDouble(&doubleTmp);
	m_Nodo.x = (float) doubleTmp;

	strTmp = nodo->GetAttribute(wxT("y"),wxT("0.0"));
	strTmp.ToDouble(&doubleTmp);
	m_Nodo.y = (float) doubleTmp;

	m_Nodo.m_Size = WRADIO_CIRCULO;
	
	initTextBox();
	Recalcular();

	m_MouseDown = false;
	m_UID = (long)this;
}

void GNC::GCS::Widgets::WPunto::initTextBox()
{
	if (m_pTextBox == NULL) {
		//calculate bottom right corner to set label...
		m_pTextBox = new WTextBox(m_pManager, m_VID, m_Nodo, WTextBox::TNM_Dynamic, "point", "");
		m_pManager->InsertarWidget(m_pTextBox);
	}
}

wxXmlNode* GNC::GCS::Widgets::WPunto::Serializar(const std::string& nombreMedico) {
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("punto_widget"));
	//almacenamos los dos nodos
	resultado->AddAttribute(new wxXmlProperty(wxT("x"),wxString::Format(wxT("%f"),m_Nodo.x)));
	resultado->AddAttribute(new wxXmlProperty(wxT("y"),wxString::Format(wxT("%f"),m_Nodo.y)));
	//serializar Metadatos
	SerializarMetadatos(resultado,nombreMedico);
	return resultado;
}
//endregion

void GNC::GCS::Widgets::WPunto::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
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
		if (m_Nodo.m_Seleccionado) {
			Vector delta = evento.iP - m_PosCursor;
			m_PosCursor = evento.iP;
			m_Nodo += delta;
			Modificar(true);

			NotificarCambios(true);
			ConsumirEvento();
			LanzarEventoModificacion();
		}
		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {

		if (EventoConsumido() && !evento.m_controlDown) {
			if (EstaSeleccionado()) {
				NotificarCambios(true);
			}
			Seleccionar(false);
			return;
		}

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		bool dentro = false;

		m_PosCursor= evento.iP;

		if (m_Nodo.Hits(evento.iP, factor)) { // click sobre el nodo
			dentro = true;
			if (evento.m_controlDown) { // Inversion de seleccion
				InvertirSeleccion();
			}
			else{
				Seleccionar(true);
			}
		}
		else { // Click fuera del nodo
			if (!evento.m_controlDown) {
				Seleccionar(false);
			}
			else {
				ConsumirEvento();
				if (EstaSeleccionado()) {
					m_MouseDown = true;
				}
			}
		}
		if (dentro) {
			m_MouseDown = true;
			m_PosAntigua = m_Nodo;
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
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) {
		if (EventoConsumido()) {
			return;
		}
		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		bool dentro = false;

		m_PosCursor= evento.iP;

		if (m_Nodo.Hits(evento.iP, factor)) {
			Iluminar(true);
			dentro = true;
		}
		else {
			Iluminar(false);
		}
		if (dentro) {
			ConsumirEvento();
		}
		return;
	}
}

void GNC::GCS::Widgets::WPunto::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WPunto::HitTest(float x, float y, const GNC::GCS::Vector& umbral)
{
	return m_Nodo.Hits(x, y, umbral);
}

bool GNC::GCS::Widgets::WPunto::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return m_Nodo.DentroDePoligono(vertices,numVertices);
}

void GNC::GCS::Widgets::WPunto::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	GNC::GCS::Vector escala = c->RelacionImagenPantalla();

	GNC::GCS::Vector radioCruz = escala * WRADIO_CRUZ;
	GNC::GCS::Vector radioCirculo = escala * WRADIO_CIRCULO;

	// Sombra
	GNC::GCS::Vector fsombra;
	if (m_MouseDown)
	{
		fsombra.Asignar(WIDGET_OFFSET_X_SOMBRA_SELECCIONADO, WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO);
	}
	else
	{
		fsombra.Asignar(WIDGET_OFFSET_X_SOMBRA, WIDGET_OFFSET_Y_SOMBRA);
	}

	fsombra *= escala;

	wColorSombra.Aplicar();

	// Circulo
	glBegin(GL_LINE_LOOP);
	float step = M_2PI / 30;
	for ( float angulo = 0; angulo < M_2PI; angulo += step)
	{
		glVertex3f(m_Nodo.x + (radioCirculo.x * cos (angulo)) + fsombra.x , m_Nodo.y + (radioCirculo.y * sin (angulo)) - fsombra.y, 0.0f);
	}
	glEnd();

	// cruz
	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x - radioCruz.x + fsombra.x, m_Nodo.y - fsombra.y, 0.0f);
	glVertex3f(m_Nodo.x + radioCruz.x + fsombra.x, m_Nodo.y - fsombra.y, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x + fsombra.x, m_Nodo.y - radioCruz.y - fsombra.y, 0.0f);
	glVertex3f(m_Nodo.x + fsombra.x, m_Nodo.y + radioCruz.y - fsombra.y, 0.0f);
	glEnd();

	// arrastre
	if (m_MouseDown) {
		glEnable(GL_LINE_STIPPLE);
		wColorLineaArrastre.Aplicar();
		glLineWidth(m_DraggingLineWidthProp);
		glLineStipple(3, 0xAAAA);

		glBegin(GL_LINE_STRIP);
		glVertex2d(m_PosAntigua.x, m_PosAntigua.y);
		glVertex2d(m_Nodo.x, m_Nodo.y);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
	}

	// Cuerpo
	glLineWidth(m_LineWidthProp);
	wAplicarColor(m_Nodo.m_Iluminado, m_MouseDown, m_Nodo.m_Seleccionado);

	// Circulo
	glBegin(GL_LINE_LOOP);
	for ( float angulo = 0; angulo < M_2PI; angulo += step)
	{
		glVertex3f(m_Nodo.x + (radioCirculo.x * cos (angulo)) , m_Nodo.y + (radioCirculo.y * sin (angulo)), 0.0f);
	}
	glEnd();

	// Cruz
	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x - radioCruz.x, m_Nodo.y, 0.0f);
	glVertex3f(m_Nodo.x + radioCruz.x, m_Nodo.y, 0.0f);
	glEnd();

	glBegin(GL_LINES);
	glVertex3f(m_Nodo.x, m_Nodo.y - radioCruz.y, 0.0f);
	glVertex3f(m_Nodo.x, m_Nodo.y + radioCruz.y, 0.0f);
	glEnd();

	//label;
	Recalcular();	
}

void GNC::GCS::Widgets::WPunto::Recalcular()
{
	//label...
	std::string text;
	if (m_pManager->GetRendererActivo() != NULL && m_pManager->GetRendererActivo()->m_pIOverlay != NULL) {
		text = m_pManager->GetRendererActivo()->m_pIOverlay->ImprimirOverlay(this, m_pManager->GetRendererActivo()->m_pImageViewer);
	}
	if (text.empty()) {
		m_pTextBox->Ocultar(true);
	} else {
		m_pTextBox->Ocultar(false);
		m_pTextBox->SetTexto(text);
	}
}

void GNC::GCS::Widgets::WPunto::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
	TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
	{
		std::ostringstream ostr;
		ostr <<  m_Nodo;
		primerMapa[_Std("Position (I)")] = ostr.str();
	}
	{
		std::stringstream out;
		out << PuntoToStringMundo(m_Nodo);
		primerMapa[_Std("Position (M)")] = out.str();
	}
}

void GNC::GCS::Widgets::WPunto::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();

	GNC::GCS::Vector Nodo = c->Reproyectar(m_Nodo) * c->factorReescalado;

	float radioCruz = WRADIO_CRUZ * escala.x;
	float radioCirculo = WRADIO_CIRCULO * escala.x;
/*
	// Sombra
	float fsombrax, fsombray;
	if (m_MouseDown)
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
	}
	else
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
	}
*/
/*	//sombra
	wColorSombra.AplicarCairo(c->cr);
 */
	cairo_set_line_width(c->cr, m_LineWidthProp);
/*
	//la cruz
	cairo_move_to(c->cr, Nodo.x - radioCruz + fsombrax, Nodo.y - fsombray);
	cairo_line_to(c->cr, Nodo.x + radioCruz + fsombrax, Nodo.y - fsombray);
	cairo_stroke(c->cr);

	cairo_move_to(c->cr, Nodo.x + fsombrax, Nodo.y - radioCruz - fsombray);
	cairo_line_to(c->cr, Nodo.x + fsombrax, Nodo.y + radioCruz - fsombray);
	cairo_stroke(c->cr);
*/
	/*
	//el circulo
	cairo_arc(c->cr, Nodo.x + fsombrax, Nodo.y - fsombray, radioCirculo, 0, M_2PI);
	cairo_stroke(c->cr);
*/
	//punto
	wColorNormal.AplicarCairo(c->cr);
	//la cruz
	cairo_move_to(c->cr, Nodo.x - radioCruz, Nodo.y);
	cairo_line_to(c->cr, Nodo.x + radioCruz, Nodo.y);
	cairo_stroke(c->cr);

	cairo_move_to(c->cr, Nodo.x, Nodo.y - radioCruz );
	cairo_line_to(c->cr, Nodo.x, Nodo.y + radioCruz );
	cairo_stroke(c->cr);

	//el circulo
	cairo_arc(c->cr, Nodo.x, Nodo.y, radioCirculo, 0, M_2PI);
	cairo_stroke(c->cr);
}
