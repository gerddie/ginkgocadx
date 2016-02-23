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
#include "wfreeform.h"
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

GNC::GCS::Widgets::WFreeFormBuilder::WFreeFormBuilder( GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask)
{
	m_NumNodos = 0;
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	SetRadioNodo(5.0f);
	m_pPoligonoCreado = NULL;
}

GNC::GCS::Widgets::WFreeFormBuilder::~WFreeFormBuilder()
{
	m_Vertices.clear();
}

void GNC::GCS::Widgets::WFreeFormBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}
		m_Estado = WBS_Ninguno;
		m_PosCursor= evento.iP;

		
		if(m_Vertices.size()>2){
			m_pPoligonoCreado = new WFreeForm(m_pManager, evento.c->GetRenderer()->GetVID(), m_Vertices, m_RadioNodo, evento.c, "FreeForm");
			m_pManager->InsertarWidget(m_pPoligonoCreado);
			m_pManager->LanzarEventoCreacion(m_pPoligonoCreado);
			ConsumirEvento();
		}
		m_Vertices.clear();
		m_pManager->Modificado();

	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}

		m_MouseDown = true;
		//aviso que han soltado
		m_pManager->Modificado();
		m_Estado = WBS_Creando;
		ConsumirEvento();

	}
	else if (evento.Dragging() && m_MouseDown) {

		if (m_Estado != WBS_Creando) {
			return;
		}

		if (!m_Vertices.empty() && ((evento.iP - m_PosCursor)/evento.c->RelacionImagenPantalla()).Norma2Cuadrado() < m_DistanciaMinimaCuadrado) {
			return;
		}
		m_PosCursor = evento.iP;
		m_Vertices.push_back(m_PosCursor);
		m_pManager->Modificado();
		ConsumirEvento();
	}
	else if( evento.Moving() ){
		m_PosCursor= evento.iP;
		m_Estado = WBS_Ninguno;
		ConsumirEvento();
	}
}

void GNC::GCS::Widgets::WFreeFormBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado& /*evento*/)
{
	GTRACE("GNC::GCS::Widgets::WPoligonoBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WFreeFormBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

	if (m_Estado != WBS_Creando) {
		return;
	}

	glLineWidth(m_LineWidthProp);
	wColorNormal.Aplicar();

	VerticesPoligono::iterator prev = m_Vertices.begin();
	if (prev != m_Vertices.end()) {
		VerticesPoligono::iterator curr = prev;
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
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WFreeFormBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_POLIGONO;
}

//endregion

//---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//region "Constructor y destructor"

GNC::GCS::Widgets::WFreeForm::WFreeForm(IWidgetsManager* pManager, unsigned int vid, VerticesPoligono& vertices, float radioNodo, GNC::GCS::Contexto3D* c, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET),
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_pTextBox(NULL)
{
	m_Vertices = vertices;

	m_RadioNodo = radioNodo;

	initTextBox(c->rotacion);
	
	Recalcular();
	m_MouseDown = false;
	m_PosCursor = m_Centro;
}

GNC::GCS::Widgets::WFreeForm::~WFreeForm()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WFreeForm::WFreeForm(IWidgetsManager* pManager, long vid, wxXmlNode* nodo, float radioNodo): GNC::GCS::Widgets::IWidget(pManager, vid, "FreeForm"), GNC::GCS::Widgets::IWidgetSerializable(nodo), m_AreaMetrica(0),
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

	m_RadioNodo = radioNodo;
	m_MouseDown = false;
	Recalcular();
	m_PosCursor = m_Centro;
}

wxXmlNode* GNC::GCS::Widgets::WFreeForm::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("free_form"));
	

	int i=0;
	for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
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

void GNC::GCS::Widgets::WFreeForm::initTextBox(double rotation)
{
	if (m_pTextBox == NULL) {
		//calculate bottom right corner to set label...
		GNC::GCS::Vector bb[2];
		for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) 
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

void GNC::GCS::Widgets::WFreeForm::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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

		if (EstaSeleccionado()) {
			for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
				(*it) += delta;
			}

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

		if (EventoConsumido() && !evento.m_controlDown) {
			Seleccionar(false);
			return;
		}

		bool dentro = false;

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
	else if (evento.Moving()) {

		if (EventoConsumido()) {
			Iluminar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor= evento.iP;

		if (HitTest(evento.iP.x, evento.iP.y, evento.c->RelacionImagenPantalla())) {
			Iluminar(true);
			dentro = true;
		}
		else {
			Iluminar(false);
		}
		if (dentro)
		{
			ConsumirEvento();
		}
	}

}

void GNC::GCS::Widgets::WFreeForm::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WFreeForm::HitTest(const GNC::GCS::Vector& p0, const GNC::GCS::Vector& p1, const GNC::GCS::Vector& cursor, const GNC::GCS::Vector& rel)
{
	bool hits = false;

	if ( p0.DistanciaEuclideaCuadrado(p1) / 4.0f > cursor.DistanciaEuclideaCuadrado(p0.PuntoMedio(p1))) {

		GNC::GCS::Vector vdir = p1 - p0;		
		GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

		GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(cursor, cursor + orto, p0, p1);
		GNC::GCS::Vector diff = (inter - cursor).ValorAbsoluto();
		if ( (diff / rel).Norma2Cuadrado() < (m_RadioNodo * m_RadioNodo) ) {
			hits = true;
		}		
	}
	return hits;
}

bool GNC::GCS::Widgets::WFreeForm::HitTest(float x, float y, const GNC::GCS::Vector& u)
{
	//distance to each line....
	bool hits = false;
	GNC::GCS::Vector point(x, y);
	VerticesPoligono::iterator prev;
	for (VerticesPoligono::iterator it = m_Vertices.begin(); !hits && it != m_Vertices.end(); ++it) 
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

bool GNC::GCS::Widgets::WFreeForm::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	bool hits = true;

	for (VerticesPoligono::iterator it = m_Vertices.begin(); hits && it != m_Vertices.end(); ++it) {
		if (!(*it).DentroDePoligono2(vertices, numVertices) ) {
			hits = false;
		}
	}

	return hits;
}

void GNC::GCS::Widgets::WFreeForm::Render(GNC::GCS::Contexto3D* c)
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
	for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x + fsombrax, (*it).y - fsombray);
	}
	glEnd();

	// Dibujo el poligono
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINE_LOOP);
	for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
		glVertex2d((*it).x, (*it).y);
	}
	glEnd();
	m_Modificado = false;
}

void GNC::GCS::Widgets::WFreeForm::Seleccionar(bool seleccionado)
{
	if (m_Seleccionado != seleccionado) {
		m_Seleccionado = seleccionado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFreeForm::Iluminar(bool iluminado)
{
	if (m_Iluminado != iluminado) {
		m_Iluminado = iluminado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFreeForm::Ocultar(bool oculto){
	if(oculto != m_Oculto){
		m_Oculto = oculto;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFreeForm::Recalcular()
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
void GNC::GCS::Widgets::WFreeForm::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
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

void GNC::GCS::Widgets::WFreeForm::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	if(m_Vertices.size() < 2) {
		return;
	}

	VerticesPoligono Vertices;

	for (VerticesPoligono::iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it)
	{
		Vertices.push_back( c->Reproyectar(*it) * c->factorReescalado );
	}

	cairo_set_line_width(c->cr, m_LineWidthProp);

	// Color del cuerpo
	wColorNormal.AplicarCairo(c->cr);

	// Dibujado del cuerpo
	for (VerticesPoligono::iterator it = Vertices.begin(); it != Vertices.end(); ++it) {
		if(it == Vertices.begin()) {
			cairo_move_to(c->cr, (*it).x, (*it).y);
		}
		cairo_line_to(c->cr, (*it).x, (*it).y);
	}
	cairo_line_to(c->cr, Vertices.front().x, Vertices.front().y);
	cairo_stroke(c->cr);
}

float GNC::GCS::Widgets::WFreeForm::GetAreaMetrica() const
{
	if (GNC::GCS::Vector::EsPoligonoSimple(m_Vertices)) {
		return GNC::GCS::Vector::AreaDePoligono(m_Vertices);
	}
	else {
		return std::numeric_limits<float>::quiet_NaN();
	}
}

