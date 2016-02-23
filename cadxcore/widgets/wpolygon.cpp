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
#include <api/icontexto.h>
#include "wpolygon.h"
#include "wtextbox.h"
#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorpermisos.h>
#include <api/iwidgetsrenderer.h>
#include <api/ievento.h>
#include <api/ioverlay.h>
#include "main/managers/widgetsmanager.h"


#include <iterator>
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>

//region "Implementacion de WPoligonoBuilder"

GNC::GCS::Widgets::WPolygonBuilder::WPolygonBuilder( GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask)
{
}

GNC::GCS::Widgets::WPolygonBuilder::~WPolygonBuilder()
{
	m_Vertices.clear();
}

void GNC::GCS::Widgets::WPolygonBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}

	if (evento.ButtonDClick(m_ButtonMask) && m_Vertices.size() >= 3)  
	{
		WPolygon* m_pPoligonoCreado = new WPolygon(m_pManager, evento.c->GetRenderer()->GetVID(), m_Vertices, evento.c, "FreeForm");
		m_pManager->InsertarWidget(m_pPoligonoCreado);
		m_pManager->LanzarEventoCreacion(m_pPoligonoCreado);
		ConsumirEvento();

		m_Vertices.clear();
		m_pManager->Modificado();
	} 
	else if (evento.ButtonUp(m_ButtonMask)) 
	{
		float distanceWithLastPoint = ((evento.iP - m_PosCursor)/evento.c->RelacionImagenPantalla()).Norma2Cuadrado();
		if (m_Vertices.empty() || distanceWithLastPoint  > (WIDGET_SIZE_PUNTO*WIDGET_SIZE_PUNTO) )
		{
			m_PosCursor = evento.iP;
			m_Vertices.push_back(m_PosCursor);
			m_pManager->Modificado();
			ConsumirEvento();
		}
	} 
	else if( evento.Moving() ){
		if (m_Vertices.empty()) {
			m_PosCursor= evento.iP;
		}
	}
}

void GNC::GCS::Widgets::WPolygonBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) 
	{
		m_Vertices.clear();
		m_pManager->Modificado();
		evento.Skip(false);
	}
}

void GNC::GCS::Widgets::WPolygonBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{
	
	glLineWidth(m_LineWidthProp);
	wColorNormal.Aplicar();

	TPolygonVertexList::iterator prev = m_Vertices.begin();
	if (prev != m_Vertices.end()) {
		TPolygonVertexList::iterator curr = prev;
		++curr;
		for (; curr != m_Vertices.end(); ++curr) {
			glBegin(GL_LINES);
			glVertex2d((*prev).x, (*prev).y);
			glVertex2d((*curr).x, (*curr).y);
			glEnd();
			prev = curr;
		}
	}


	if (m_Vertices.size() > 2) {
		glEnable(GL_LINE_STIPPLE);
		glColor4f(0.7f, 0.7f, 0.0f, 1.0);
		glLineStipple(3, 0xAAAA);
		glBegin(GL_LINES);
		glVertex2d(m_Vertices.front().x, m_Vertices.front().y);
		glVertex2d(m_Vertices.back().x, m_Vertices.back().y);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}

	
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glPointSize(WIDGET_SIZE_PUNTO);
	glBegin(GL_POINTS);
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x, (*it).y);
	}
	glEnd();
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WPolygonBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_POLIGONO;
}

//endregion

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//region "Constructor y destructor"

GNC::GCS::Widgets::WPolygon::WPolygon(IWidgetsManager* pManager, unsigned int vid, TPolygonVertexList& vertices, GNC::GCS::Contexto3D* c, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET),
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_pTextBox(NULL)
{
	m_Vertices = vertices;

	initTextBox(c->rotacion);
	
	Recalcular();
	m_MouseDown = false;
	m_PosCursor = m_Centro;
}

GNC::GCS::Widgets::WPolygon::~WPolygon()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WPolygon::WPolygon(IWidgetsManager* pManager, long vid, wxXmlNode* nodo): GNC::GCS::Widgets::IWidget(pManager, vid, "FreeForm"),
	GNC::GCS::Widgets::IWidgetSerializable(nodo),
	m_AreaMetrica(0),
	m_pTextBox(NULL)
{
	if(nodo->GetName() != wxT("free_form")){
		std::cerr<< "Ha ocurrido un error al desserializar el widget free_form"<<std::endl;
	}
	
	wxXmlNode *child = nodo->GetChildren();
	GNC::GCS::Nodo nodoTmp;
	while (child) {
		nodoTmp = GNC::GCS::Nodo::Deserializar(child);
		GNC::GCS::Vector v(nodoTmp);
		m_Vertices.push_back(v);
		child = child->GetNext();
	}
	initTextBox();

	m_MouseDown = false;
	Recalcular();
	m_PosCursor = m_Centro;
}

void GNC::GCS::Widgets::WPolygon::initTextBox(double rotation)
{
	if (m_pTextBox == NULL) {
		//calculate bottom right corner to set label...
		GNC::GCS::Vector bb[2];
		for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
		{
			if (it == m_Vertices.begin()) {
				(*it).IniciarBoundingBox(bb);
			} else {
				(*it).ExtenderBoundingBox(bb);
			}
		}	
		GNC::GCS::Vector pm = bb[0].PuntoMedio(bb[1]);
		bb[1].RotarSobreZ( pm, rotation);

		m_pTextBox = new WTextBox(m_pManager, m_VID, bb[1], WTextBox::TNM_Dynamic, "polygonBox", "");
		m_pManager->InsertarWidget(m_pTextBox);
	}
}

wxXmlNode* GNC::GCS::Widgets::WPolygon::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("free_form"));
	

	int i=0;
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		wxXmlNode* nodo = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxString::Format(wxT("nodo%d"), (int) i));
		nodo->AddAttribute(new wxXmlProperty(wxT("x"),wxString::Format(wxT("%f"),(*it).x)));
		nodo->AddAttribute(new wxXmlProperty(wxT("y"),wxString::Format(wxT("%f"),(*it).y)));
		resultado->AddChild(nodo);
		i++;
	}
	//metadatos
	SerializarMetadatos(resultado,nombreMedico);
	return resultado;
}
//endregion

void GNC::GCS::Widgets::WPolygon::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if(EstaOculto()){
		return;
	}
	//------------------------------------------------------
	// Leaving
	if (evento.Leaving()){
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
		Vector delta = evento.iP - m_PosCursor;
		m_PosCursor = evento.iP;

		bool modified = false;
		if (EstaSeleccionado()) 
		{
			for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
				(*it) += delta;
			}

			modified = true;
		} 
		else 
		{
			for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
				if ((*it).EstaIluminado()) {
					(*it) += delta;
					modified = true;
				}
			}
		}
		if (modified) 
		{
			Recalcular();
			Modificar(true);
			LanzarEventoModificacion();
		}

		ConsumirEvento();

		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {
		for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
		{
			SeleccionarNodo((*it), false);
		}

		if (EventoConsumido() && !evento.m_controlDown) {
			Seleccionar(false);
			return;
		}

		bool dentro = false;

		////first of all find if hits any vertex...
		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
		{
			if ((*it).Hits(evento.iP, factor)) {
				SeleccionarNodo((*it), true);
				IluminarNodo((*it), true);
				Seleccionar(false);
				dentro = true;
				break;
			}
			else 
			{
				SeleccionarNodo((*it), false);
				IluminarNodo((*it), false);
			}
		}

		if (!dentro) {
			if (HitTest(evento.iP.x, evento.iP.y, evento.c->RelacionImagenPantalla())) {
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
		}

		if (dentro) {
			m_MouseDown = true;
			m_PosCursor= evento.iP;
			m_Modificado = true;
			ConsumirEvento();
		}

		return;
	}
	//------------------------------------------------------
	// LeftUP
	else if (evento.LeftUp()) {
		if (m_MouseDown) {
			ActualizarTimestampModificacion();
			m_MouseDown = false;
			ConsumirEvento();
		}
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) 
	{
		for (TPolygonVertexList::iterator it2 = m_Vertices.begin(); it2 != m_Vertices.end(); ++it2) 
		{
			IluminarNodo((*it2), false);
		}

		if (EventoConsumido()) {
			Iluminar(false);
			return;
		}

		bool dentro = false;
		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();
		m_PosCursor= evento.iP;

		for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
		{
			if ((*it).Hits(evento.iP, factor)) {
				IluminarNodo((*it), true);
				dentro = true;
				break;
			}
			else {
				IluminarNodo((*it), false);
			}
		}

		if (!dentro) {
			if (HitTest(evento.iP.x, evento.iP.y, evento.c->RelacionImagenPantalla())) {
				Iluminar(true);
				dentro = true;
			}
			else {
				Iluminar(false);
			}
		}
		if (dentro)
		{
			ConsumirEvento();
		}
	}

}

void GNC::GCS::Widgets::WPolygon::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WPolygon::HitTest(const GNC::GCS::Nodo& p0, const GNC::GCS::Nodo& p1, const GNC::GCS::Vector& cursor, const GNC::GCS::Vector& rel)
{
	bool hits = false;

	if ( p0.DistanciaEuclideaCuadrado(p1) / 4.0f > cursor.DistanciaEuclideaCuadrado(p0.PuntoMedio(p1))) {

		GNC::GCS::Vector vdir = p1 - p0;		
		GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

		GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(cursor, cursor + orto, p0, p1);
		GNC::GCS::Vector diff = (inter - cursor).ValorAbsoluto();
		if ( (diff / rel).Norma2Cuadrado() < (p0.m_Size * p0.m_Size) ) {
			hits = true;
		}		
	}
	return hits;
}

bool GNC::GCS::Widgets::WPolygon::HitTest(float x, float y, const GNC::GCS::Vector& u)
{
	//distance to each line....
	bool hits = false;
	GNC::GCS::Vector point(x, y);
	TPolygonVertexList::iterator prev;
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); !hits && it != m_Vertices.end(); ++it) 
	{
		//distance between two nodes
		if (it == m_Vertices.begin())
		{
			hits = HitTest(m_Vertices.front(), m_Vertices.back(), point, u);
		} 
		else
		{
			hits = HitTest((*prev), (*it), point, u);
		}
		prev = it;
	}
	return hits;
}

bool GNC::GCS::Widgets::WPolygon::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	bool hits = true;

	for (TPolygonVertexList::iterator it = m_Vertices.begin(); hits && it != m_Vertices.end(); ++it) {
		if (!(*it).DentroDePoligono2(vertices, numVertices) ) {
			hits = false;
		}
	}

	return hits;
}

void GNC::GCS::Widgets::WPolygon::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}


	GNC::GCS::Vector escala = c->RelacionImagenPantalla();
	glLineWidth(m_LineWidthProp);

	float fsombrax, fsombray;

	if (m_Seleccionado && m_MouseDown)
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
	}
	else
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
	}

	wColorSombra.Aplicar();
	glBegin(GL_LINE_LOOP);
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x + fsombrax, (*it).y - fsombray);
	}
	glEnd();

	glBegin(GL_POINTS);
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x + fsombrax, (*it).y - fsombray);
	}
	glEnd();

	// Dibujo highlights puntos activos
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
	{
		if ((*it).m_Iluminado) {
			wDibujarElipseDegradado((*it), wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
		}
	}

	// Dibujo el poligono
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINE_LOOP);
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x, (*it).y);
	}
	glEnd();
	m_Modificado = false;

	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glPointSize(WIDGET_SIZE_PUNTO);
	
	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
	{
		wAplicarColor(m_Iluminado || (*it).m_Iluminado, m_MouseDown, m_Seleccionado);
		glBegin(GL_POINTS);
		glVertex2d((*it).x, (*it).y);
		glEnd();
	}	
}

void GNC::GCS::Widgets::WPolygon::Seleccionar(bool seleccionado)
{
	if (m_Seleccionado != seleccionado) {
		m_Seleccionado = seleccionado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WPolygon::Iluminar(bool iluminado)
{
	if (m_Iluminado != iluminado) {
		m_Iluminado = iluminado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WPolygon::Ocultar(bool oculto){
	if(oculto != m_Oculto){
		m_Oculto = oculto;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WPolygon::Recalcular()
{
	m_Centro = GNC::GCS::Vector::Centroide(m_Vertices);
	m_Perimetro = GNC::GCS::Vector::PerimetroDePoligono(m_Vertices);

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

//endregion
void GNC::GCS::Widgets::WPolygon::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
	TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
	{
		std::ostringstream ostr;
		ostr <<  m_Centro;
		primerMapa[_Std("Center (I)")] = ostr.str();
	}

	{
		std::stringstream out;
		out << PuntoToStringMundo(m_Centro);
		primerMapa[_Std("Center (M)")] = out.str();
	}

	{
		std::ostringstream ostr;
		ostr <<  m_Vertices.size();
		primerMapa[_Std("Number of Vertex")] = ostr.str();
	}
}

void GNC::GCS::Widgets::WPolygon::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	if(m_Vertices.size() < 2) {
		return;
	}

	TPolygonVertexList Vertices;

	for (TPolygonVertexList::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
	{
		Vertices.push_back( c->Reproyectar(*it) * c->factorReescalado );
	}

	cairo_set_line_width(c->cr, m_LineWidthProp);

	// Color del cuerpo
	wColorNormal.AplicarCairo(c->cr);

	// Dibujado del cuerpo
	for (TPolygonVertexList::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
		if(it == Vertices.begin()) {
			cairo_move_to(c->cr, (*it).x, (*it).y);
		}
		cairo_line_to(c->cr, (*it).x, (*it).y);
	}
	cairo_line_to(c->cr, Vertices.front().x, Vertices.front().y);
	cairo_stroke(c->cr);
}

float GNC::GCS::Widgets::WPolygon::GetAreaMetrica() const
{
	if (GNC::GCS::Vector::EsPoligonoSimple(m_Vertices)) {
		return GNC::GCS::Vector::AreaDePoligono(m_Vertices);
	}
	else {
		return std::numeric_limits<float>::quiet_NaN();
	}
}

