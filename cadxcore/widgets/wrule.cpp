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
#include <api/icontexto.h>
#include <api/ivista.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <api/westilo.h>

#include <widgets/gui/calibracion.h>
#include <wx/menu.h>
#include <wx/xml/xml.h>
#include "wrule.h"
#include "wtextbox.h"

#include <main/managers/widgetsmanager.h>
#include <vtkgl.h>
#include <cmath>
#include <sstream>
#include <cairo/cairo.h>
#include "openglhelper.h"
#include <eventos/modificacionimagen.h>
#include <resources/ginkgoresourcesmanager.h>

#include <api/math/geometry3d.h>

//region "Implementacion de WRuleBuilder"

GNC::GCS::Widgets::WRuleBuilder::WRuleBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, bool supportsCallibration) :
		GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask),
		m_MouseDown(false),
		m_pReglaTemp(NULL),
		m_SupportsCallibration(supportsCallibration)
{
	//GNC::GCS::Events::EventoModificacionImagen evt1(m_pManager->GetVista());
}

GNC::GCS::Widgets::WRuleBuilder::~WRuleBuilder()
{
	if (m_pReglaTemp != NULL) {
		delete m_pReglaTemp;
		m_pReglaTemp = NULL;
	}
}

void GNC::GCS::Widgets::WRuleBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	GTRACE("GNC::GCS::Widgets::WRuleBuilder::OnMouseEvents(wxMouseEvent&)");
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_pReglaTemp != NULL) {
			m_pReglaTemp->m_Vertices[1] = evento.iP;;
			m_pReglaTemp->Recalcular(evento.c->GetRenderer(), true);
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
		m_pReglaTemp = new GNC::GCS::Widgets::WRule(m_pManager, evento.c->GetRenderer()->GetVID(), nodos, m_SupportsCallibration, "Regla");
		//std::cout << "linea (leftdwn): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
		m_pManager->Modificado();
		ConsumirEvento();
	} else if (evento.Dragging() && m_MouseDown) {
		if (m_pReglaTemp != NULL) {
			m_pReglaTemp->m_Vertices[1] = evento.iP;
			m_pReglaTemp->Recalcular(evento.c->GetRenderer(), true);
			//std::cout << "linea (moving): " << m_Vertices[0].m_wX << ", " << m_Vertices[0].m_wY << " -> " << m_Vertices[1].m_wX << ", " << m_Vertices[1].m_wY << std::endl;
			m_pManager->Modificado();
		}
		ConsumirEvento();
	}

}

void GNC::GCS::Widgets::WRuleBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
	GTRACE("GNC::GCS::Widgets::WRuleBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNC::GCS::Widgets::WRuleBuilder::Render(GNC::GCS::Contexto3D* c)
{
	if (m_pReglaTemp == NULL) {
		return;
	}
	m_pReglaTemp->Render(c);
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WRuleBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_LINEA;
}

//region Interfaz de eventos ginkgo



//endregion


//endregion


//region "Constructor y destructor"

GNC::GCS::Widgets::WRule::WRule(IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodos[2], bool supportsCallibration, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET),
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_MouseDown(false),
	m_Angulo(0.0),
	m_Pendiente(0.0),
	m_Distancia(0),
	m_SupportsCallibration(supportsCallibration)
{
	m_Vertices[0] = nodos[0];
	m_Vertices[1] = nodos[1];
	m_Oculto = false;

	GNC::GCS::Events::EventoModificacionImagen evt1(m_pManager->GetVista());
	GNC::GCS::IEventsController::Instance()->Registrar(this, evt1);

	m_pTextBox = new GNC::GCS::Widgets::WTextBox(m_pManager, m_VID, m_Vertices[1], GNC::GCS::Widgets::WTextBox::TNM_Dynamic, "ruleBox", "");
	m_pTextBox->SetTexto("0");
	m_pManager->InsertarWidget(m_pTextBox);
}

GNC::GCS::Widgets::WRule::~WRule()
{
	m_pManager->EliminarWidget(m_pTextBox, false);
	LanzarEventoDestruccion();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WRule::WRule(IWidgetsManager* pManager, long vid, wxXmlNode* nodo):
        GNC::GCS::Widgets::IWidget(pManager, vid, "Regla"),
    GNC::GCS::Widgets::IWidgetSerializable(nodo),
    m_MouseDown(false),
    m_Angulo(0.0),
    m_Pendiente(0.0),
    m_Distancia(0),
    m_SupportsCallibration(false)
{
	if(nodo->GetName() != wxT("rule_widget")){
		std::cerr<< "Ha ocurrido un error al desserializar el widget regla"<<std::endl;
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

    GNC::GCS::Events::EventoModificacionImagen evt1(m_pManager->GetVista());
	GNC::GCS::IEventsController::Instance()->Registrar(this, evt1);

	m_pTextBox = new GNC::GCS::Widgets::WTextBox(m_pManager, m_VID, m_Vertices[1], GNC::GCS::Widgets::WTextBox::TNM_Dynamic, "ruleBox", "");
	m_pTextBox->SetTexto("0");
	m_pManager->InsertarWidget(m_pTextBox);
}

wxXmlNode* GNC::GCS::Widgets::WRule::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("rule_widget"));
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

void GNC::GCS::Widgets::WRule::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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
			Vector delta = evento.iP - m_PosCursor;
			m_PosCursor = evento.iP;
			if (EstaSeleccionado() || m_Vertices[0].EstaSeleccionado()) {
				m_Vertices[0] += delta;
			}
			if (EstaSeleccionado() || m_Vertices[1].EstaSeleccionado()) {
				m_Vertices[1] += delta;
			}
			LanzarEventoModificacion();
			Recalcular(evento.c->GetRenderer(), false);
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
					m_CentroAntiguo = m_Centro;
				}
			}
		}

		if (dentro) {
			m_MouseDown = true;
			m_PosCursor = evento.iP;
			m_CentroAntiguo = m_Centro;
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

		if (m_Vertices[1].Hits(evento.iP.x, evento.iP.y, factor)) {

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

void GNC::GCS::Widgets::WRule::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WRule::HitTest(float x, float y, const GNC::GCS::Vector& rel)
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

bool GNC::GCS::Widgets::WRule::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return m_Vertices[0].DentroDePoligono2(vertices, numVertices) && m_Vertices[1].DentroDePoligono2(vertices, numVertices);
}

void GNC::GCS::Widgets::WRule::Render(GNC::GCS::Contexto3D* c)
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

	// Right mark
	glBegin(GL_LINES);
		glVertex2d(f20.x + fsombra.x, f20.y - fsombra.y);
		glVertex2d(f21.x + fsombra.x, f21.y - fsombra.y);
	glEnd();

	//-- Left mark
	wAplicarColor(m_Iluminado || m_Vertices[0].m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
		glVertex2d(f00.x, f00.y);
		glVertex2d(f01.x, f01.y);
	glEnd();

	//-- Central mark
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
		glVertex2d(f10.x, f10.y);
		glVertex2d(puntoMedio.x, puntoMedio.y);
	glEnd();

	// Right mark
	wAplicarColor(m_Iluminado || m_Vertices[1].m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINES);
		glVertex2d(f20.x, f20.y);
		glVertex2d(f21.x, f21.y);
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

bool GNC::GCS::Widgets::WRule::GetMenuContextual(wxMenu* menuContextual, wxWindow* pParent) {

	if (m_SupportsCallibration) {
		GNC::GCS::Widgets::Dialogos::Calibracion* pCalibracion = new GNC::GCS::Widgets::Dialogos::Calibracion(pParent, this, m_pManager);
		wxMenuItem* pMenuCalibrar = new wxMenuItem(menuContextual, 1, _("Image Calibration"), _("Image Calibration"), wxITEM_NORMAL );
		menuContextual->Connect(1,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( GNC::GCS::Widgets::Dialogos::Calibracion::OnClickEnMenu), NULL, pCalibracion);

		#ifdef __WXMSW__
		pMenuCalibrar->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoCalibrarImagen());
		#else
		pMenuCalibrar->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoCalibrarImagen());
		#endif

		menuContextual->Append(pMenuCalibrar);
		return true;
	}
	else {
		return false;
	}
}

void GNC::GCS::Widgets::WRule::Recalcular(GNC::GCS::IWidgetsRenderer* renderer, bool building)
{
	GNC::GCS::Vector3D worldPosition0, worldPosition1, wDiff;
	GNC::GCS::Vector iDiff;
	

	if (renderer != NULL && renderer->m_pImageViewer != NULL) {
		const double tindex = renderer->m_pImageViewer->GetTindex();
		double imagePosition0[3] = {m_Vertices[0].x, m_Vertices[0].y, tindex};
		double imagePosition1[3] = {m_Vertices[1].x, m_Vertices[1].y, tindex};
		renderer->m_pImageViewer->CoordenadasImagenACoordenadasMundo(imagePosition0, worldPosition0.v);
		renderer->m_pImageViewer->CoordenadasImagenACoordenadasMundo(imagePosition1, worldPosition1.v);
	}

	//std::cout << "--- Recalculando... " << std::endl;
	m_Modificado = true;
	wDiff = worldPosition1 - worldPosition0;
	iDiff = m_Vertices[1] - m_Vertices[0];
	m_Centro = m_Vertices[0].PuntoMedio(m_Vertices[1]);
	m_Pendiente = iDiff.Pendiente();
	m_Angulo = iDiff.AnguloSobreAbscisa();
	m_Distancia = worldPosition0.DistanciaEuclidea(worldPosition1);
	//std::cout << *this << std::endl;

	std::stringstream os;
	os.setf(ios::floatfield, ios::fixed );
	os.precision(2);
	os.fill('0');
	float metricaDistancia = wDiff.Norma2();

	double spacing[3] = {0.0, 0.0, 0.0};
	bool hasSpacing = m_pManager->GetVista()->GetEstudio()->GetSpacingActive(spacing[0], spacing[1], spacing[2]);
	if (hasSpacing) {
		if (metricaDistancia > 1000.0f) {
			os << metricaDistancia / 1000.0f << " m.";
		}
		else if (metricaDistancia > 10.0f) {
			os << metricaDistancia / 10.0f << " cm.";
		}
		else if (metricaDistancia < 1.0f) {
				os << metricaDistancia * 1000.0f << " µm.";
			}
		else {
			os << metricaDistancia << " mm.";
		}
	} else {
		os << metricaDistancia << " px.";
	}

	std::string medida = os.str();

	m_pTextBox->SetTexto(medida);
	if (building) {
		m_pTextBox->SetPosition(m_Vertices[0].PuntoMedio(m_Vertices[1]));
	}
}

//region Interfaz de eventos ginkgo

//endregion
void GNC::GCS::Widgets::WRule::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
	TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
	{
		std::stringstream os;
		os.setf(ios::floatfield, ios::fixed );
		os.precision(2);
		os.fill('0');

		double spacing[3] = {0.0, 0.0, 0.0};
		bool hasSpacing = m_pManager->GetVista()->GetEstudio()->GetSpacingActive(spacing[0], spacing[1], spacing[2]);
		float metricaDistancia = m_Distancia;
		if (hasSpacing) {
			if (metricaDistancia > 1000.0f) {
				os << metricaDistancia / 1000.0f << " m.";
			}
			else if (metricaDistancia > 10.0f) {
				os << metricaDistancia / 10.0f << " cm.";
			}
			else if (metricaDistancia < 1.0f) {
					os << metricaDistancia * 1000.0f << " µm.";
				}
			else {
				os << metricaDistancia << " mm.";
			}
		} else {
			os << metricaDistancia << " px.";
		}
		
		std::string medida = os.str();
		primerMapa[_Std("Length")] = medida;
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

void GNC::GCS::Widgets::WRule::OffscreenRender(GNC::GCS::Contexto3D* c)
{
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

}

void GNC::GCS::Widgets::WRule::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	if (evt->GetCodigoEvento() == ginkgoEVT_Core_ModificacionImagen)
	{
		GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
		if (pEvt == NULL) {
			std::cerr << "Error al interpretar evento como evento de modificacion de imagen: Evento = " << evt << std::endl;
			return;
		}
		if(pEvt->GetTipo() == GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada)
		{
			Recalcular(m_pManager->GetRendererActivo(), false);
		}
	}
}

