/*
*  
*  $Id: wrule.cpp $
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
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/icontexto.h>
#include <api/ivista.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <api/westilo.h>

#include <widgets/gui/calibracion.h>
#include <wx/menu.h>
#include <wx/xml/xml.h>
#include "wecgrule.h"
#include <widgets/wtextbox.h>

#include <main/managers/widgetsmanager.h>
#include <vtkgl.h>
#include <cmath>
#include <sstream>
#include <cairo/cairo.h>
#include <widgets/openglhelper.h>
#include <eventos/modificacionimagen.h>
#include <resources/ginkgoresourcesmanager.h>
#include <visualizator/estudios/ecgstudy.h>

#include <api/math/geometry3d.h>

//region "Implementacion de WRuleBuilder"

GNKVisualizator::Widgets::WECGRuleBuilder::WECGRuleBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask) :
	GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask),
	m_MouseDown(false),
	m_pReglaTemp(NULL)
{
}

GNKVisualizator::Widgets::WECGRuleBuilder::~WECGRuleBuilder()
{
	if (m_pReglaTemp != NULL) {
		delete m_pReglaTemp;
		m_pReglaTemp = NULL;
	}
}

void GNKVisualizator::Widgets::WECGRuleBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}

	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_pReglaTemp != NULL) {
			m_pReglaTemp->m_Vertices[1] = evento.iP;;
			m_pReglaTemp->Recalcular(evento.c->GetRenderer());
			//como mínimo 2 pixels imagen
			if (std::abs(m_pReglaTemp->Distancia()) > GNC::GCS::Vector::EpsilonDeReales() ) {
				ConsumirEvento();
				m_pManager->InsertarWidget(m_pReglaTemp);
				m_pManager->LanzarEventoCreacion(m_pReglaTemp);
			}
			else {
				delete m_pReglaTemp;
			}
			m_pReglaTemp = NULL;
			m_pManager->Modificado();
		}		
	} else if (evento.ButtonDown(m_ButtonMask)) {

		m_MouseDown = true;
		TNodo nodos[2];
		nodos[0] = nodos[1] = evento.iP;
		if (m_pReglaTemp != NULL) {
			delete m_pReglaTemp;
		}
		m_pReglaTemp = new GNKVisualizator::Widgets::WECGRule(m_pManager, evento.c->GetRenderer()->GetVID(), nodos, "Regla");
		//std::cout << "linea (leftdwn): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
		m_pManager->Modificado();
		ConsumirEvento();
	} else if (evento.Dragging() && m_MouseDown) {
		if (m_pReglaTemp != NULL) {
			m_pReglaTemp->m_Vertices[1] = evento.iP;
			m_pReglaTemp->Recalcular(evento.c->GetRenderer());
			//std::cout << "linea (moving): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
			m_pManager->Modificado();
		}
		ConsumirEvento();
	}

}

void GNKVisualizator::Widgets::WECGRuleBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
}

void GNKVisualizator::Widgets::WECGRuleBuilder::Render(GNC::GCS::Contexto3D* c)
{
	if (m_pReglaTemp == NULL) {
		return;
	}
	m_pReglaTemp->Render(c);
}

GNC::GCS::Widgets::TipoCursor GNKVisualizator::Widgets::WECGRuleBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_LINEA;
}

//region "Constructor y destructor"

GNKVisualizator::Widgets::WECGRule::WECGRule(GNC::GCS::IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodos[2], const char* nombre) :
        GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET),
        m_pTextBox(NULL), 
	m_MouseDown(false),
	m_Distancia(0)

{
	m_Vertices[0] = nodos[0];
	m_Vertices[1] = nodos[1];

	m_Oculto = false;

	m_pTextBox = new GNC::GCS::Widgets::WTextBox(m_pManager, m_VID, m_Vertices[1], GNC::GCS::Widgets::WTextBox::TNM_Dynamic, "ruleBox", "");
	m_pTextBox->SetTexto("0");
	m_pManager->InsertarWidget(m_pTextBox);
}

GNKVisualizator::Widgets::WECGRule::~WECGRule()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}

//endregion

bool GNKVisualizator::Widgets::WECGRule::SoportaAutoBorrado() const {
	return true;
}

void GNKVisualizator::Widgets::WECGRule::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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
		if (EstaSeleccionado() || m_Vertices[0].EstaSeleccionado() || m_Vertices[1].EstaSeleccionado()) {
			GNC::GCS::Vector delta = evento.iP - m_PosCursor;
			m_PosCursor = evento.iP;
			if (EstaSeleccionado()) {
				m_Vertices[0] += delta;
				m_Vertices[1] += delta;
			}
			else 
			{
				if (m_Vertices[1].EstaSeleccionado()) 
				{
					m_Vertices[1] = evento.iP;
				}
				else if (m_Vertices[0].EstaSeleccionado())
				{
					m_Vertices[0] = evento.iP;
				}
			}
			LanzarEventoModificacion();
			Recalcular(evento.c->GetRenderer());
			ConsumirEvento();
		}
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

		if (m_Vertices[0].Hits(evento.iP.x, evento.iP.y, factor)) { // click sobre el nodo
			SeleccionarNodo(m_Vertices[0], true);
			SeleccionarNodo(m_Vertices[1], false);
			Seleccionar(false);

			dentro = true;
		}
		else { // Click fuera del nodo
			SeleccionarNodo(m_Vertices[0], false);
		}
		if (!dentro && m_Vertices[1].Hits(evento.iP.x, evento.iP.y, factor)) { // click sobre el nodo
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
					m_PosCursor = evento.iP;
				}
			}
		}

		if (dentro) {
			m_MouseDown = true;
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
			ConsumirEvento();
		}
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) {

		if (EventoConsumido()) {
			IluminarNodo(m_Vertices[0], false);
			IluminarNodo(m_Vertices[1], false);
			Iluminar(false);
			return;
		}
		bool dentro = false;

		m_PosCursor = evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		if (m_Vertices[0].Hits(evento.iP.x, evento.iP.y, factor)) {

			IluminarNodo(m_Vertices[0], true);
			IluminarNodo(m_Vertices[1], false);
			Iluminar(false);

			dentro = true;
		}
		else {
			IluminarNodo(m_Vertices[0], false);
		}

		if (m_Vertices[1].Hits(evento.iP.x, evento.iP.y, factor) && !m_Vertices[0].EstaIluminado()) {

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

void GNKVisualizator::Widgets::WECGRule::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}


bool GNKVisualizator::Widgets::WECGRule::HitTest(float x, float y, const GNC::GCS::Vector& rel)
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

bool GNKVisualizator::Widgets::WECGRule::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return m_Vertices[0].DentroDePoligono2(vertices, numVertices) && m_Vertices[1].DentroDePoligono2(vertices, numVertices);
}

void GNKVisualizator::Widgets::WECGRule::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	GNC::GCS::Vector escala = c->RelacionImagenPantalla();


	glLineWidth(m_LineWidthProp);

	// Highlights sobre nodos activos
	if (m_Vertices[0].m_Iluminado) {
		wDibujarElipseDegradado(m_Vertices[0], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
	}

	if (m_Vertices[1].m_Iluminado) {
		wDibujarElipseDegradado(m_Vertices[1], wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
	}

	GNC::GCS::Vector fsombra;
	if (m_Seleccionado && m_MouseDown)
	{
		fsombra.Asignar(WIDGET_OFFSET_X_SOMBRA_SELECCIONADO, WIDGET_OFFSET_Y_SOMBRA_SELECCIONADO);
	}
	else
	{
		fsombra.Asignar(WIDGET_OFFSET_X_SOMBRA, WIDGET_OFFSET_Y_SOMBRA);
	}
	fsombra *= escala;

	//------------------------------------------------------------------------------------------
	// Sombra de la regla
	wColorSombra.Aplicar();
	glBegin(GL_LINES);
	glVertex2d(m_Vertices[0].x + fsombra.x, m_Vertices[0].y - fsombra.y);
	glVertex2d(m_Vertices[1].x + fsombra.x, m_Vertices[1].y - fsombra.y);
	glEnd();

	// Dibujado de la regla
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
	glVertex2d(m_Vertices[0].x, m_Vertices[0].y);
	glVertex2d(m_Vertices[1].x, m_Vertices[1].y);
	glEnd();


	TVector diff = m_Vertices[1] - m_Vertices[0];

	// Apply spacing deformation
	diff /= GNC::GCS::Vector(escala.x, escala.y);

	TVector puntoMedio = m_Vertices[0].PuntoMedio(m_Vertices[1]);
	TVector ortogonal = (diff).VectorOrtogonal().Normalizar() * escala * 6.0;

	if (m_MouseDown && m_Seleccionado) {
		ortogonal *= WIDGET_INCREMENTO_DETALLE_SELECCIONADO;
	}


	TVector f00 = m_Vertices[0] + ortogonal;
	TVector f01 = m_Vertices[0] - ortogonal;
	TVector f10 = puntoMedio + ortogonal;
	TVector f20 = m_Vertices[1] + ortogonal;
	TVector f21 = m_Vertices[1] - ortogonal;

	//------------------------------------------------------------------------------------------
	// Dibujado de las marcas de la regla.
	//-- Sombras
	wColorSombra.Aplicar();
	glBegin(GL_LINES);
	glVertex2d(f00.x + fsombra.x, f00.y - fsombra.y);
	glVertex2d(f01.x + fsombra.x, f01.y - fsombra.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(f10.x + fsombra.x, f10.y - fsombra.y);
	glVertex2d(puntoMedio.x + fsombra.x, puntoMedio.y - fsombra.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(f20.x + fsombra.x, f20.y - fsombra.y);
	glVertex2d(f21.x + fsombra.x, f21.y - fsombra.y);
	glEnd();

	// Marca derecha
	glBegin(GL_LINES);
	glVertex2d(f20.x + fsombra.x, f20.y - fsombra.y);
	glVertex2d(f21.x + fsombra.x, f21.y - fsombra.y);
	glEnd();

	//-- Marca izquierda
	wAplicarColor(m_Iluminado || m_Vertices[0].m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
	glVertex2d(f00.x, f00.y);
	glVertex2d(f01.x, f01.y);
	glEnd();

	//-- Marca central
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
	glVertex2d(f10.x, f10.y);
	glVertex2d(puntoMedio.x, puntoMedio.y);
	glEnd();

	// Marca derecha
	wAplicarColor(m_Iluminado || m_Vertices[1].m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
	glVertex2d(f20.x, f20.y);
	glVertex2d(f21.x, f21.y);
	glEnd();

#if 0 // DEBUG

	glLineWidth(WIDGET_GROSOR_LINEA);
	glColor4d(1.0, 0.0, 0.0, 1.0);
	GNC::GCS::Vector ddiff = m_Vertices[1] - m_Vertices[0];
	glBegin(GL_LINES);
	glVertex2d(0.0, 0.0);
	glVertex2d(ddiff.x, ddiff.y);
	glEnd();

	GNC::GCS::Vector vr = (ddiff / escala).VectorOrtonormal() * 10.0;

	glColor4d(0.0, 1.0, 1.0, 1.0);

	GNC::GCS::Vector inter = GNC::GCS::Vector::InterseccionEntreRectas(m_PosCursor, m_PosCursor + (vr * escala), m_Vertices[0], m_Vertices[1]);

	glBegin(GL_LINES);
	glVertex2d(m_PosCursor.x, m_PosCursor.y);
	glVertex2d(inter.x, inter.y);
	glEnd();

	glColor4d(0.0, 0.0, 1.0, 1.0);

	glBegin(GL_LINES);
	glVertex2d(0.0, 0.0);	
	glVertex2d(vr.x * escala.x, vr.y * escala.y);
	glEnd();

	glBegin(GL_LINES);
	glVertex2d(m_Vertices[0].x, m_Vertices[0].y);	
	glVertex2d(m_Vertices[0].x + vr.x * escala.x, m_Vertices[0].y + vr.y * escala.y);
	glEnd();

#endif

}

float GNKVisualizator::Widgets::WECGRule::Distancia() {
	return m_Distancia;
}

bool GNKVisualizator::Widgets::WECGRule::isVertical()
{
	GNC::GCS::Vector iDiff = m_Vertices[1] - m_Vertices[0];
	return std::abs(iDiff.AnguloSobreAbscisa()) > M_PI_4;;
}

bool GNKVisualizator::Widgets::WECGRule::GetMenuContextual(wxMenu* /*menuContextual*/, wxWindow* /*pParent*/) 
{	
	return false;
}

void ortogonalize(bool isVertical, const GNC::GCS::Vector& fixed, GNC::GCS::Vector& modified)
{
	if (isVertical) {
		modified = modified.ProyeccionOrtogonalSobreRecta(fixed, (fixed + GNC::GCS::Vector(0.0f, 1.0f)));

	} else {
		modified = modified.ProyeccionOrtogonalSobreRecta(fixed, (fixed + GNC::GCS::Vector(1.0f, 0.0f)));
	}
}

void GNKVisualizator::Widgets::WECGRule::Recalcular(GNC::GCS::IWidgetsRenderer* renderer)
{
	GNC::GCS::Vector3D worldPosition0, worldPosition1, wDiff;
	bool isVerticalRule = isVertical();

	//ortogonalize nodes...
	if (m_Vertices[0].EstaIluminado())
	{
		ortogonalize(isVerticalRule, m_Vertices[1], m_Vertices[0]);
	} 
	else
	{
		ortogonalize(isVerticalRule, m_Vertices[0], m_Vertices[1]);
	}
	//

	if (renderer != NULL && renderer->m_pImageViewer != NULL) {
		const double tindex = renderer->m_pImageViewer->GetTindex();
		double imagePosition0[3] = {m_Vertices[0].x, m_Vertices[0].y, tindex};
		double imagePosition1[3] = {m_Vertices[1].x, m_Vertices[1].y, tindex};
		renderer->m_pImageViewer->CoordenadasImagenACoordenadasMundo(imagePosition0, worldPosition0.v);
		renderer->m_pImageViewer->CoordenadasImagenACoordenadasMundo(imagePosition1, worldPosition1.v);
	}

	m_Modificado = true;
	wDiff = worldPosition1 - worldPosition0;
	m_Distancia = worldPosition0.DistanciaEuclidea(worldPosition1);

	std::stringstream os;
	os.setf(ios::floatfield, ios::fixed );
	os.precision(2);
	os.fill('0');
	float metricaDistancia = wDiff.Norma2();

	GNKVisualizator::ECGStudy* pStudy = NULL;
	if (m_pManager->GetVista() != NULL && m_pManager->GetVista()->GetEstudio().IsValid())
	{
		pStudy = dynamic_cast<GNKVisualizator::ECGStudy*>(m_pManager->GetVista()->GetEstudio().GetRawPointer());
	}
	if (pStudy != NULL) {
		os << pStudy->getMeasureText(isVerticalRule, metricaDistancia);
	} else {
		os << metricaDistancia << " px.";
	}

	std::string medida = os.str();

	m_pTextBox->SetTexto(medida);

	GNC::GCS::Vector textPosition(std::max(m_Vertices[0].x, m_Vertices[1].x), std::max(m_Vertices[0].y, m_Vertices[1].y));
	m_pTextBox->SetPosition(textPosition);
}


//region Interfaz de eventos ginkgo


//endregion
void GNKVisualizator::Widgets::WECGRule::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
	TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
	{
		primerMapa[_Std("Length")] = m_pTextBox->GetTexto();
	}
	{
		std::stringstream out;
		out << m_Vertices[0] << ",\n" << m_Vertices[1];
		primerMapa[_Std("Points (I)")] = out.str();
	}
	{
		std::stringstream out;
		out << PuntoToStringMundo(m_Vertices[0]) << "," << std::endl;
		out << PuntoToStringMundo(m_Vertices[1]) ;

		primerMapa[_Std("Points (M)")] = out.str();
	}
}

void GNKVisualizator::Widgets::WECGRule::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	GNC::GCS::Vector Diff = m_Vertices[1] - m_Vertices[0];
	GNC::GCS::Vector TamTexto;

	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();

	GNC::GCS::Vector vert[2] = {c->Reproyectar(m_Vertices[0]) * c->factorReescalado, c->Reproyectar(m_Vertices[1]) * c->factorReescalado };

	glLineWidth(m_LineWidthProp);

	// Dibujado de la regla
	wColorNormal.AplicarCairo(c->cr);
	cairo_move_to(c->cr, vert[0].x, vert[0].y);
	cairo_line_to(c->cr, vert[1].x, vert[1].y);
	cairo_stroke(c->cr);

	TVector diff = vert[1] - vert[0];

	TVector puntoMedio = vert[0].PuntoMedio(vert[1]);
	TVector ortogonal = (diff).VectorOrtonormal() * escala * 6.0f;

	TVector f00 = vert[0] + ortogonal;
	TVector f01 = vert[0] - ortogonal;
	TVector f10 = puntoMedio + ortogonal;
	TVector f20 = vert[1] + ortogonal;
	TVector f21 = vert[1] - ortogonal;

	//------------------------------------------------------------------------------------------
	// Dibujado de las marcas de la regla.

	//-- marcas normales
	wColorNormal.AplicarCairo(c->cr);
	//-- Marca izquierda
	cairo_move_to(c->cr, f00.x , f00.y );
	cairo_line_to(c->cr, f01.x , f01.y );
	cairo_stroke(c->cr);

	//-- Marca central
	cairo_move_to(c->cr, f10.x , f10.y );
	cairo_line_to(c->cr, puntoMedio.x , puntoMedio.y );
	cairo_stroke(c->cr);

	// Marca derecha
	cairo_move_to(c->cr, f20.x , f20.y );
	cairo_line_to(c->cr, f21.x , f21.y );
	cairo_stroke(c->cr);

	//------------------------------------------------------------------------------------------
	//// Dibujado del texto de la medida
	cairo_font_options_t* options;
	options = cairo_font_options_create ();

	cairo_select_font_face (c->cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(c->cr, std::max(m_FontSizeProp * escala.x, (double)8.0f));
	cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(c->cr, options);

	TamTexto = GNC::GCS::Widgets::HelperTexto::calcularBoundingBox(c, m_pTextBox->GetTexto(), false, m_MaxTextWidthProp * escala.x); // Tamaño de la region en pixels

	cairo_set_source_rgba (c->cr, 1.0f, 1.0f, 1.0f, 1.0f);
	cairo_save(c->cr);
	GNC::GCS::Vector puntoTexto( puntoMedio.x - (TamTexto.x / 2.0f), puntoMedio.y - TamTexto.y);
	puntoTexto.Redondear();
	cairo_translate(c->cr, puntoTexto.x, puntoTexto.y);
	GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, m_pTextBox->GetTexto(), false, m_MaxTextWidthProp * escala.x);
	cairo_restore(c->cr);

	cairo_font_options_destroy(options);

}

