/*
 *  
 *  $Id: wflecha.cpp $
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
//#define _GINKGO_TRACE
#include <wx/xml/xml.h>

#include <cmath>
#include <sstream>

#include <api/globals.h>
#include "wflecha.h"
#include "wtextbox.h"
#include <api/westilo.h>
#include <api/ievento.h>
#include <api/ioverlay.h>
#include <api/iwidgetsrenderer.h>
#include <api/icontexto.h>
#include <main/managers/widgetsmanager.h>
#include <vtkgl.h>

#include <cairo/cairo.h>
#include "openglhelper.h"


//region "Implementacion de WFlechaBuilder"

GNC::GCS::Widgets::WFlechaBuilder::WFlechaBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask)
{
	m_MouseDown = false;
	m_pFlechaTemp = NULL;
}

GNC::GCS::Widgets::WFlechaBuilder::~WFlechaBuilder()
{
	if (m_pFlechaTemp != NULL) {
		delete m_pFlechaTemp;
		m_pFlechaTemp = NULL;
	}
}

void GNC::GCS::Widgets::WFlechaBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	GTRACE("GNC::GCS::Widgets::WFlechaBuilder::OnMouseEvents(wxMouseEvent&)");
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_pFlechaTemp != NULL) {
			m_pFlechaTemp->m_Vertices[1] = evento.iP;
			m_pFlechaTemp->Recalcular();
			if (std::abs(m_pFlechaTemp->Distancia()) > evento.c->RelacionImagenPantalla().NormaInfinito()) {
				m_pManager->InsertarWidget(m_pFlechaTemp);
				m_pManager->LanzarEventoCreacion(m_pFlechaTemp);
				ConsumirEvento();
			}
			else {
				delete m_pFlechaTemp;
			}
			m_pFlechaTemp = NULL;
			m_pManager->Modificado();
		}
	} else if (evento.ButtonDown(m_ButtonMask)) {
		m_MouseDown = true;
		TNodo nodos[2];
		nodos[0] = nodos[1] = evento.iP;
		if (m_pFlechaTemp != NULL) {
			delete m_pFlechaTemp;
		}
		m_pFlechaTemp = new GNC::GCS::Widgets::WFlecha(m_pManager, evento.c->GetRenderer()->GetVID(), nodos, evento.c, "Flecha");
		//std::cout << "linea (leftdwn): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
		m_pManager->Modificado();
		ConsumirEvento();
	} else if (evento.Dragging() && m_MouseDown) {
		if (m_pFlechaTemp != NULL) {
			m_pFlechaTemp->m_Vertices[1] = evento.iP;
			m_pFlechaTemp->Recalcular();
			//std::cout << "linea (moving): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
			m_pManager->Modificado();
			ConsumirEvento();
		}		
	}

}

void GNC::GCS::Widgets::WFlechaBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
	GTRACE("GNC::GCS::Widgets::WFlechaBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WFlechaBuilder::Render(GNC::GCS::Contexto3D* c)
{
	if (m_pFlechaTemp == NULL) {
		return;
	}
	m_pFlechaTemp->Render(c);
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WFlechaBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_LINEA;
}

//endregion

//region "Constructor y destructor"

GNC::GCS::Widgets::WFlecha::WFlecha(IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodos[2], GNC::GCS::Contexto3D* c, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET),
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_pTextBox(NULL)
{
	m_Vertices[0] = nodos[0];
	m_Vertices[1] = nodos[1];

	m_MouseDown = false;
	m_Oculto = false;

	initTextBox(c->rotacion);
	Recalcular();
}

GNC::GCS::Widgets::WFlecha::~WFlecha()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WFlecha::WFlecha(IWidgetsManager* pManager, long vid, wxXmlNode* nodo): GNC::GCS::Widgets::IWidget(pManager, vid, "Flecha"), 
	GNC::GCS::Widgets::IWidgetSerializable(nodo),
	m_pTextBox(NULL)
{
	if(nodo->GetName() != wxT("flecha_widget")){
		std::cerr<< "Ha ocurrido un error al desserializar el widget flecha"<<std::endl;
	}

	wxXmlNode *child = nodo->GetChildren();
	while (child) {
		if(child->GetName() == wxT("node0")){
			m_Vertices[0] = GNC::GCS::Nodo::Deserializar(child);
		} else if(child->GetName() == wxT("node1")){
			m_Vertices[1] = GNC::GCS::Nodo::Deserializar(child);
		}
		child = child->GetNext();
	}

	m_MouseDown = false;
	
	initTextBox();
	Recalcular();
}

void GNC::GCS::Widgets::WFlecha::initTextBox(double rotation)
{
	if (m_pTextBox == NULL) {
		//calculate bottom right corner to set label...
		GNC::GCS::Vector bb[2];
		m_Vertices[0].IniciarBoundingBox(bb);
		m_Vertices[1].ExtenderBoundingBox(bb);
		GNC::GCS::Vector pm = bb[0].PuntoMedio(bb[1]);
		bb[1].RotarSobreZ( pm, rotation);

		m_pTextBox = new WTextBox(m_pManager, m_VID, bb[1], WTextBox::TNM_Dynamic, "arrow", "");
		m_pManager->InsertarWidget(m_pTextBox);
	}
}

wxXmlNode* GNC::GCS::Widgets::WFlecha::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("flecha_widget"));
	//almacenamos los dos nodos
	wxXmlNode* nodo = m_Vertices[0].Serializar();
	nodo->SetName(wxT("node0"));
	resultado->AddChild(nodo);
	nodo = m_Vertices[1].Serializar();
	nodo->SetName(wxT("node1"));
	resultado->AddChild(nodo);
	//metadatos
	SerializarMetadatos(resultado,nombreMedico);
	return resultado;
}

//endregion

void GNC::GCS::Widgets::WFlecha::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (EstaOculto()){
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

		if (EstaSeleccionado() || m_Vertices[0].EstaSeleccionado() || m_Vertices[1].EstaSeleccionado()) {
			Vector delta = evento.iP - m_PosCursor;
			m_PosCursor = evento.iP;
			if (EstaSeleccionado() || m_Vertices[0].EstaSeleccionado()) {
				m_Vertices[0] += delta;
			}
			if (EstaSeleccionado() || m_Vertices[1].EstaSeleccionado()) {
				m_Vertices[1] += delta;
			}
			LanzarEventoModificacion();
			Recalcular();
			ConsumirEvento();
		}
		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {
		if (EventoConsumido() && !evento.m_controlDown) {
			SeleccionarNodo(m_Vertices[0], false);
			SeleccionarNodo(m_Vertices[1], false);
			Seleccionar(false);
			return;
		}

		bool dentro = false;


		m_StartPointMov = evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		if (m_Vertices[0].Hits(evento.iP, factor)) { // click sobre el nodo
			SeleccionarNodo(m_Vertices[0], true);
			SeleccionarNodo(m_Vertices[1], false);
			Seleccionar(false);

			dentro = true;
		}
		else { // Click fuera del nodo
			SeleccionarNodo(m_Vertices[0], false);
		}
		if (!dentro && m_Vertices[1].Hits(evento.iP, factor)) { // click sobre el nodo
			SeleccionarNodo(m_Vertices[0], false);
			SeleccionarNodo(m_Vertices[1], true);
			Seleccionar(false);

			dentro = true;
		}
		else { // Click fuera del nodo
			SeleccionarNodo(m_Vertices[1], false);
		}

		if (!dentro && HitTest(evento.iP.x, evento.iP.y, factor ) ) { // click sobre la recta
			if (evento.m_controlDown) { // Inversion de seleccion
				InvertirSeleccion();
			}
			else{
				Seleccionar(true);
			}

			dentro = true;
		}
		else { // Click fuera del nodo
			if (!evento.m_controlDown) {
				Seleccionar(false);
			}
			else {
				ConsumirEvento();
				if (EstaSeleccionado()) {
					m_MouseDown = true;
					m_CentroAntiguo = m_Centro;
					m_PosCursor = evento.iP;
				}
			}
		}

		if (dentro) {
			m_MouseDown = true;
			m_CentroAntiguo = m_Centro;
			m_PosCursor = evento.iP;
			ConsumirEvento();
		}
		return;
	}
	//------------------------------------------------------
	// LeftUP
	else if (evento.LeftUp()) {
		if (m_MouseDown) {
			m_MouseDown = false;
			ActualizarTimestampModificacion();
			ConsumirEvento();
		}
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving())
	{
		if (EventoConsumido()) {
			IluminarNodo(m_Vertices[0], false);
			IluminarNodo(m_Vertices[1], false);
			Iluminar(false);
			return;
		}
		bool dentro = false;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		m_PosCursor = evento.iP;

		if (m_Vertices[0].Hits(evento.iP, factor)) {

			IluminarNodo(m_Vertices[0], true);
			IluminarNodo(m_Vertices[1], false);
			Iluminar(false);

			dentro = true;
		}
		else {
			IluminarNodo(m_Vertices[0], false);
		}

		if (m_Vertices[1].Hits(evento.iP, factor)) {

			IluminarNodo(m_Vertices[0], false);
			IluminarNodo(m_Vertices[1], true);
			Iluminar(false);

			dentro = true;
		}
		else {
			IluminarNodo(m_Vertices[1], false);
		}

		if (!dentro && HitTest(evento.iP.x, evento.iP.y, factor) ) {
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

void GNC::GCS::Widgets::WFlecha::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WFlecha::HitTest(float x, float y, const GNC::GCS::Vector& rel)
{
	bool hits = false;

	GNC::GCS::Vector dpos(x,y);
	if ( m_Vertices[0].DistanciaEuclideaCuadrado(m_Vertices[1]) / 4.0f > dpos.DistanciaEuclideaCuadrado(m_Vertices[0].PuntoMedio(m_Vertices[1]))) {

		GNC::GCS::Vector vdir = m_Vertices[1] - m_Vertices[0];		
		GNC::GCS::Vector orto = (vdir / rel).VectorOrtogonal() * rel;

		GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(dpos, dpos + orto, m_Vertices[0], m_Vertices[1]);
		GNC::GCS::Vector diff = (inter - dpos).ValorAbsoluto();
		if ( (diff / rel).Norma2Cuadrado() < (m_Vertices[0].m_Size * m_Vertices[0].m_Size) ) {
			hits = true;
		}		
	}
	return hits;
}

bool GNC::GCS::Widgets::WFlecha::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return m_Vertices[0].DentroDePoligono2(vertices, numVertices) && m_Vertices[1].DentroDePoligono2(vertices, numVertices);
}

void GNC::GCS::Widgets::WFlecha::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	const GNC::GCS::Vector& escala = c->RelacionImagenPantalla();

	glLineWidth(m_LineWidthProp);

	if (m_Vertices[0].m_Iluminado) {
		wDibujarElipseDegradado(m_Vertices[0], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
	}

	if (m_Vertices[1].m_Iluminado) {
		wDibujarElipseDegradado(m_Vertices[1], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
	}

	float fsombrax, fsombray;

	TVector vOrtogonal = ((m_Vertices[0] - m_Vertices[1]) / escala).VectorOrtonormal() * 15.0f;

	if (m_Seleccionado && m_MouseDown)
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA_SELECCIONADO * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO * escala.y;
		vOrtogonal *= WIDGET_INCREMENTO_DETALLE_SELECCIONADO;
	}
	else
	{
		fsombrax = WIDGET_OFFSET_X_SOMBRA * escala.x;
		fsombray = WIDGET_OFFSET_Y_SOMBRA * escala.y;
	}

	float angulo = M_PI_4 / 2.0f;
	TVector f0 = m_Vertices[1] + vOrtogonal.RotacionSobreZ(M_PI_2 + angulo) * escala;
	TVector f1 = m_Vertices[1] + vOrtogonal.RotacionSobreZ(M_PI_2 - angulo) * escala;

	//------------------------------------------------------------------------------------------
	// Sombra del cuerpo de la flecha
	wColorSombra.Aplicar();
	glBegin(GL_LINES);
		glVertex2d(m_Vertices[0].x + fsombrax, m_Vertices[0].y - fsombray);
		glVertex2d(m_Vertices[1].x + fsombrax, m_Vertices[1].y - fsombray);
	glEnd();

	// Sombra de las aspas de la flecha
	glBegin(GL_LINE_STRIP);
		glVertex2d(f0.x + fsombrax, f0.y - fsombray);
		glVertex2d(m_Vertices[1].x + fsombrax, m_Vertices[1].y - fsombray);
		glVertex2d(f1.x + fsombrax, f1.y - fsombray);
	glEnd();

	// Sombra de la cola
	glPointSize(WIDGET_SIZE_PUNTO);
	glBegin(GL_POINTS);
		glVertex2d(m_Vertices[0].x + fsombrax, m_Vertices[0].y - fsombray);
	glEnd();

	// Dibujado de la flecha

	// Cuerpo
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
	glEnd();

	// Aspas
	wAplicarColor(m_Iluminado || m_Vertices[1].m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINE_STRIP);
		glVertex2d(f0.x, f0.y);
		glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
		glVertex2d(f1.x, f1.y);
	glEnd();

	// Cola
	wAplicarColor(m_Iluminado || m_Vertices[0].m_Iluminado, m_MouseDown, m_Seleccionado);
	if (m_Seleccionado && m_MouseDown)
		glPointSize(WIDGET_SIZE_PUNTO * WIDGET_INCREMENTO_DETALLE_SELECCIONADO);
	else
		glPointSize(WIDGET_SIZE_PUNTO);

	glBegin(GL_POINTS);
		glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
	glEnd();


	//------------------------------------------------------------------------------------------
	// Dibujado de la linea punteada de arrastre
	if (m_MouseDown) {
		glLineWidth(m_DraggingLineWidthProp);
		glEnable(GL_LINE_STIPPLE);
		wColorLineaArrastre.Aplicar();
		glLineStipple(3, 0xAAAA);

		GNC::GCS::Vector startPoint, endPoint;
		if (m_Vertices[0].m_Seleccionado)
		{
			startPoint = m_Vertices[0];
			endPoint = m_Vertices[0] - (m_PosCursor - m_StartPointMov);
		}
		else if (m_Vertices[1].m_Seleccionado)
		{
			startPoint = m_Vertices[1];
			endPoint = m_Vertices[1] - (m_PosCursor - m_StartPointMov);
		}
		else // Es el centro
		{
			startPoint = m_CentroAntiguo;
			endPoint = m_Centro;
		}

		glBegin(GL_LINE_STRIP);
		glVertex2d(startPoint.x, startPoint.y);
		glVertex2d(endPoint.x, endPoint.y);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
	}
}

void GNC::GCS::Widgets::WFlecha::Seleccionar(bool seleccionado)
{
	if(m_Seleccionado != seleccionado){
		m_Seleccionado = seleccionado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFlecha::Iluminar(bool iluminado)
{
	if(m_Iluminado != iluminado){
		m_Iluminado = iluminado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFlecha::IluminarNodo( GNC::GCS::Nodo& nodo, bool iluminado)
{
	if(nodo.m_Iluminado != iluminado){
		nodo.m_Iluminado = iluminado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFlecha::SeleccionarNodo( GNC::GCS::Nodo& nodo, bool seleccionado)
{
	if(nodo.m_Seleccionado != seleccionado){
		nodo.m_Seleccionado = seleccionado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFlecha::Ocultar(bool oculto){
	if(oculto != m_Oculto){
		m_Oculto = oculto;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WFlecha::Recalcular()
{
	//std::cout << "--- Recalculando... " << std::endl;
	m_Modificado = true;
	m_Diff = m_Vertices[1] - m_Vertices[0];
	m_Centro = m_Vertices[0].PuntoMedio(m_Vertices[1]);
	m_Pendiente = m_Diff.Pendiente();
	m_Angulo = m_Diff.AnguloSobreAbscisa();
	m_Distancia = m_Vertices[0].DistanciaEuclidea(m_Vertices[1]);
	//std::cout << *this << std::endl;

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

void GNC::GCS::Widgets::WFlecha::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();
	TVector Vertices[2] = { c->Reproyectar(m_Vertices[0]) * c->factorReescalado, c->Reproyectar(m_Vertices[1]) * c->factorReescalado};
		
	TVector vOrtogonal = (Vertices[0] - Vertices[1]).VectorOrtonormal() * escala * 10.0f;
	
	float angulo = M_PI_4 / 2.0f;
	TVector f0 = Vertices[1] + vOrtogonal.RotacionSobreZ(M_PI_2 + angulo);
	TVector f1 = Vertices[1] + vOrtogonal.RotacionSobreZ(M_PI_2 - angulo);

	// Dibujado de la flecha

	// Color del cuerpo
	wColorNormal.AplicarCairo(c->cr);

	// Punto de origen de la flecha
	cairo_arc(c->cr, Vertices[0].x, Vertices[0].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
	cairo_fill(c->cr);

	// Cuerpo
	cairo_move_to(c->cr, Vertices[0].x, Vertices[0].y);
	cairo_line_to(c->cr, Vertices[1].x, Vertices[1].y);
	cairo_stroke(c->cr);

	// Aspas
	cairo_move_to(c->cr, f0.x, f0.y);
	cairo_line_to(c->cr, Vertices[1].x, Vertices[1].y);
	cairo_line_to(c->cr, f1.x, f1.y);
	cairo_stroke(c->cr);
}
