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
#include <api/iwidgetsrenderer.h>
#include "welipse.h"
#include "wtextbox.h"
#include <api/ievento.h>
#include <api/ioverlay.h>
#include "main/managers/widgetsmanager.h"
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>


//region "Implementacion de WElipseBuilder"

GNC::GCS::Widgets::WElipseBuilder::WElipseBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
}

GNC::GCS::Widgets::WElipseBuilder::~WElipseBuilder()
{
}

void GNC::GCS::Widgets::WElipseBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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

		m_Stop = evento.iP;

		GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);		
		GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
		m_BB[0] = m_Start;
		m_BB[1] = a;
		m_BB[2] = m_Stop;
		m_BB[3] = b;


		TVector diff = m_Stop - m_Start;
		if (std::abs(diff.x) > 0.001f && std::abs(diff.y) > 0.001f) {
			GNC::GCS::Vector bBox[2];
			bBox[0] = m_BB[0];
			bBox[1] = m_BB[2];
			GNC::GCS::Vector::OrdenarBoundingBox(bBox);

			TVector centro = m_BB[0].PuntoMedio(m_BB[2]);

			TVector m_RadioMayor = m_BB[0] - m_BB[1];
			TVector m_RadioMenor = m_BB[0] - m_BB[3];

			/*
			TVector centro = bBox[0].PuntoMedio(bBox[1]);

			TVector radio1(bBox[1].x - centro.x, 0.0f);
			TVector radio2(0.0f, bBox[1].y - centro.y);
			*/

			WElipse* w = new WElipse(centro, m_RadioMayor/2, m_RadioMenor/2, m_pManager, evento.c->GetRenderer()->GetVID(), "Elipse", m_GID, evento.c);
			m_pManager->InsertarWidget(w);
			m_pManager->LanzarEventoCreacion(w);
			ConsumirEvento();
		}

		m_pManager->Modificado();
		m_Estado = WBS_Ninguno;
	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}
		m_MouseDown = true;

		m_Start = evento.iP;
		m_Stop = m_Start;

		GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);		
		GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
		m_BB[0] = m_Start;
		m_BB[1] = a;
		m_BB[2] = m_Stop;
		m_BB[3] = b;

		m_pManager->Modificado();
		m_Estado = WBS_Creando;
		evento.Skip(false);
	}
	else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}

		m_Stop = evento.iP;

		GNC::GCS::Vector a = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[0]);		
		GNC::GCS::Vector b = m_Start.ProyeccionOrtogonalSobreRecta(m_Stop,  m_Stop - evento.c->ScreenDirectionCosines[1]);
		m_BB[0] = m_Start;
		m_BB[1] = a;
		m_BB[2] = m_Stop;
		m_BB[3] = b;

		m_pManager->Modificado();
		evento.Skip(false);
	}
	else if( evento.Moving() ){
		GTRACE("Moviendose");
		m_NodoMoviendose = evento.iP;
		m_Estado = WBS_Ninguno;
	}

}

void GNC::GCS::Widgets::WElipseBuilder::OnKeyEvents(TEventoTeclado&)
{
}

void GNC::GCS::Widgets::WElipseBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{

	if (m_Estado == WBS_Creando) {
		TVector centro = m_BB[0].PuntoMedio(m_BB[2]);

		TVector m_RadioMayor = m_BB[0] - m_BB[1];
		TVector m_RadioMenor = m_BB[0] - m_BB[3];

		double m_NormaRadioMayor = m_RadioMayor.Norma2()/2;
		double m_NormaRadioMenor = m_RadioMenor.Norma2()/2;

		double m_Angulo = m_RadioMayor.AnguloSobreAbscisa();
		
		float step = M_2PI / 100;

		glLineWidth(2.0f);
		glColor4f(0.0f, 0.5f, 0.0f, 0.75f);
		glBegin(GL_LINE_LOOP);

			for ( float angulo = 0; angulo < M_2PI; angulo += step)
			{
				GNC::GCS::Vector v (m_NormaRadioMayor * cos(angulo), m_NormaRadioMenor * sin(angulo));
				v = centro + v.RotacionSobreZ(m_Angulo);
				glVertex3f(v.x , v.y, 0.0f);
			}

		glEnd();
	}
	else {

	}
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WElipseBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_CREAR_ELIPSE;
}

//endregion

//region "Constructor y destructor"

GNC::GCS::Widgets::WElipse::WElipse(TVector centro, TVector radio1, TVector radio2, IWidgetsManager* pManager, long vid, const char* nombre, long gid, GNC::GCS::Contexto3D* c) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre, gid,TID_USER_ANNOTATION_WIDGET), 
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_pTextBox(NULL)
{
	m_Centro = centro;
	m_RadioMayor = radio1;
	m_RadioMenor = radio2;
	m_NodoRadioMayor = m_Centro + m_RadioMayor;
	m_NodoRadioMayorReflejado = m_NodoRadioMayor.ReflejoSobrePunto(m_Centro);
	m_NodoRadioMenor = m_Centro + m_RadioMenor;
	m_NodoRadioMenorReflejado = m_NodoRadioMenor.ReflejoSobrePunto(m_Centro);
	m_MouseDown = false;
	m_UID = (long)this;
	m_Debug = true;

	m_Nodos = new GNC::GCS::Vector[m_NumNodos];

	initTextBox(c->rotacion);
	Recalcular();
}

GNC::GCS::Widgets::WElipse::~WElipse()
{
	delete[] m_Nodos;
	m_pManager->EliminarWidget(m_pTextBox, false);
	m_pManager->Modificado();
}

//endregion

//region "Serializado y desserializado"
GNC::GCS::Widgets::WElipse::WElipse(IWidgetsManager* pManager, long vid, long gid, wxXmlNode* nodo): GNC::GCS::Widgets::IWidget(pManager, vid,"Elipse",gid), 
	GNC::GCS::Widgets::IWidgetSerializable(nodo),
	m_pTextBox(NULL)
{
	if(nodo->GetName() != wxT("elipse_widget")){
		std::cerr<< "Ha ocurrido un error al desserializar el widget elipse"<<std::endl;
	}
	wxXmlNode *child = nodo->GetChildren();
	GNC::GCS::Nodo nodoTmp;
	while (child) {
		if(child->GetName() == wxT("centro")){
			nodoTmp = GNC::GCS::Nodo::Deserializar(child);
			m_Centro.x = nodoTmp.x;
			m_Centro.y = nodoTmp.y;
		} else if(child->GetName() == wxT("radioMayor")){
			m_NodoRadioMayor = GNC::GCS::Nodo::Deserializar(child);
		} else if(child->GetName() == wxT("radioMenor")){
			m_NodoRadioMenor = GNC::GCS::Nodo::Deserializar(child);
		} else if(child->GetName() == wxT("rotacion")){
			wxString strTmp = child->GetAttribute(wxT("valor"),wxT("0.0"));
			double doubleTmp;
			strTmp.ToDouble(&doubleTmp);
			m_Angulo = (float) doubleTmp;
		}
		child = child->GetNext();
	}

	m_NodoRadioMayorReflejado = m_NodoRadioMayor.ReflejoSobrePunto(m_Centro);
	m_NodoRadioMenorReflejado = m_NodoRadioMenor.ReflejoSobrePunto(m_Centro);
		
	initTextBox();

	m_MouseDown = false;
	m_UID = (long)this;
	m_Debug = true;

	m_Nodos = new GNC::GCS::Vector[m_NumNodos];

	Recalcular();
}

void GNC::GCS::Widgets::WElipse::initTextBox(double rotation)
{
	if (m_pTextBox == NULL) {
		//calculate bottom right corner to set label...
		GNC::GCS::Vector bb[2];
		m_NodoRadioMayor.IniciarBoundingBox(bb);
		m_NodoRadioMayorReflejado.ExtenderBoundingBox(bb);
		m_NodoRadioMenor.ExtenderBoundingBox(bb);
		m_NodoRadioMenorReflejado.ExtenderBoundingBox(bb);

		GNC::GCS::Vector pm = bb[0].PuntoMedio(bb[1]);
		bb[1].RotarSobreZ( pm, rotation);

		m_pTextBox = new WTextBox(m_pManager, m_VID, bb[1], WTextBox::TNM_Dynamic, "ellipse", "");
		m_pManager->InsertarWidget(m_pTextBox);
	}
}

wxXmlNode* GNC::GCS::Widgets::WElipse::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("elipse_widget"));
	//almacenamos los dos nodos
	wxXmlNode* nodo = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("centro"));
	nodo->AddAttribute(new wxXmlProperty(wxT("x"),wxString::Format(wxT("%f"),m_Centro.x)));
	nodo->AddAttribute(new wxXmlProperty(wxT("y"),wxString::Format(wxT("%f"),m_Centro.y)));
	resultado->AddChild(nodo);

	nodo = m_NodoRadioMayor.Serializar();
	nodo->SetName(wxT("radioMayor"));
	resultado->AddChild(nodo);

	nodo = m_NodoRadioMenor.Serializar();
	nodo->SetName(wxT("radioMenor"));
	resultado->AddChild(nodo);

	nodo = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("rotacion"));
	nodo->AddAttribute(new wxXmlProperty(wxT("valor"),wxString::Format(wxT("%f"),m_Angulo)));
	resultado->AddChild(nodo);

	//metadatos
	SerializarMetadatos(resultado,nombreMedico);

	return resultado;
}
//endregion

void GNC::GCS::Widgets::WElipse::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
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
		Vector delta = evento.iP - m_PosCursor;
		m_PosCursor = evento.iP;

		//GNC::GCS::Vector escala = evento.c->RelacionImagenPantalla();

		if (m_NodoRadioMayor.EstaSeleccionado()) {
			m_NodoRadioMayor += delta;
			m_Centro = m_NodoRadioMayor.PuntoMedio(m_NodoRadioMayorReflejado);
			m_RadioMayor = m_NodoRadioMayor - m_Centro;
			m_RadioMenor = (m_RadioMayor).VectorOrtogonal().Normalizar() * m_NormaRadioMenor;
			m_NodoRadioMenor = m_Centro + m_RadioMenor;
			m_NodoRadioMenorReflejado = m_NodoRadioMenor.ReflejoSobrePunto(m_Centro);
			Modificar(true);
		}
		else if (m_NodoRadioMayorReflejado.EstaSeleccionado()) {
			m_NodoRadioMayorReflejado += delta;
			m_Centro = m_NodoRadioMayorReflejado.PuntoMedio(m_NodoRadioMayor);
			m_RadioMayor = m_NodoRadioMayor - m_Centro;
			m_RadioMenor = (m_RadioMayor).VectorOrtogonal().Normalizar() * m_NormaRadioMenor;
			m_NodoRadioMenor = m_Centro + m_RadioMenor;
			m_NodoRadioMenorReflejado = m_NodoRadioMenor.ReflejoSobrePunto(m_Centro);
			Modificar(true);
		}
		else if (m_NodoRadioMenor.EstaSeleccionado()) {
			m_NodoRadioMenor += delta;
			m_Centro = m_NodoRadioMenor.PuntoMedio(m_NodoRadioMenorReflejado);
			m_RadioMenor = m_NodoRadioMenor - m_Centro;
			m_RadioMayor = (m_RadioMenor).VectorOrtogonal().Normalizar() *  m_NormaRadioMayor;
			m_NodoRadioMayor = m_Centro + m_RadioMayor;
			m_NodoRadioMayorReflejado = m_NodoRadioMayor.ReflejoSobrePunto(m_Centro);
			Modificar(true);
		}
		else if (m_NodoRadioMenorReflejado.EstaSeleccionado()) {
			m_NodoRadioMenorReflejado += delta;
			m_Centro = m_NodoRadioMenorReflejado.PuntoMedio(m_NodoRadioMenor);
			m_RadioMenor = m_NodoRadioMenorReflejado - m_Centro;
			m_RadioMayor = (m_RadioMenor).VectorOrtogonal().Normalizar() * m_NormaRadioMayor;
			m_NodoRadioMayor = m_Centro + m_RadioMayor;
			m_NodoRadioMayorReflejado = m_NodoRadioMayor.ReflejoSobrePunto(m_Centro);
			Modificar(true);
		}
		else if (EstaSeleccionado())  {
			m_Centro += delta;
			m_NodoRadioMayor += delta;
			m_NodoRadioMayorReflejado += delta;
			m_NodoRadioMenor += delta;
			m_NodoRadioMenorReflejado += delta;
			Modificar(true);
		}

		if (m_Modificado) {
			Recalcular();
			LanzarEventoModificacion();
			ConsumirEvento();
		}
		return;
	}
	//------------------------------------------------------
	// LeftDown
	else if (evento.LeftDown()) {
		if (EventoConsumido() && !evento.m_controlDown) {
			SeleccionarNodo(m_NodoRadioMayor, false);
			SeleccionarNodo(m_NodoRadioMayorReflejado, false);
			SeleccionarNodo(m_NodoRadioMenor, false);
			SeleccionarNodo(m_NodoRadioMenorReflejado, false);
			Seleccionar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor = evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		if (!dentro && m_NodoRadioMayor.Hits(evento.iP, factor)) {
			SeleccionarNodo(m_NodoRadioMayor, true);
			Seleccionar(false);
			dentro = true;
		}
		else {
			SeleccionarNodo(m_NodoRadioMayor, false);
		}

		if (!dentro && m_NodoRadioMayorReflejado.Hits(evento.iP, factor)) {
			SeleccionarNodo(m_NodoRadioMayorReflejado, true);
			Seleccionar(false);
			dentro = true;
		}
		else {
			SeleccionarNodo(m_NodoRadioMayorReflejado, false);
		}

		if (!dentro && m_NodoRadioMenor.Hits(evento.iP, factor)) {
			SeleccionarNodo(m_NodoRadioMenor, true);
			Seleccionar(false);
			dentro = true;
		}
		else {
			SeleccionarNodo(m_NodoRadioMenor, false);
		}

		if (!dentro && m_NodoRadioMenorReflejado.Hits(evento.iP, factor)) {
			SeleccionarNodo(m_NodoRadioMenorReflejado, true);
			Seleccionar(false);
			dentro = true;
		}
		else {
			SeleccionarNodo(m_NodoRadioMenorReflejado, false);
		}

		if (!dentro) {
			if (HitTest(evento.iP.x, evento.iP.y,evento.c->RelacionImagenPantalla())) 
			{
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
						m_PosCursor = evento.iP;
					}
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
			ActualizarTimestampModificacion();
			ConsumirEvento();
		}
	}
	//------------------------------------------------------
	// Moving
	else if (evento.Moving()) {
		if (EventoConsumido()) {
			IluminarNodo(m_NodoRadioMayor, false);
			IluminarNodo(m_NodoRadioMayorReflejado, false);
			IluminarNodo(m_NodoRadioMenor, false);
			IluminarNodo(m_NodoRadioMenorReflejado, false);
			Iluminar(false);
			return;
		}

		bool dentro = false;

		m_PosCursor = evento.iP;

		const GNC::GCS::Vector& factor = evento.c->RelacionImagenPantalla();

		if (!dentro && m_NodoRadioMayor.Hits(evento.iP, factor)) {
			IluminarNodo(m_NodoRadioMayor, true);
			Iluminar(false);
			dentro = true;
		}
		else {
			IluminarNodo(m_NodoRadioMayor, false);
		}

		if (!dentro && m_NodoRadioMayorReflejado.Hits(evento.iP, factor)) {
			IluminarNodo(m_NodoRadioMayorReflejado, true);
			Iluminar(false);
			dentro = true;
		}
		else {
			IluminarNodo(m_NodoRadioMayorReflejado, false);
		}

		if (!dentro && m_NodoRadioMenor.Hits(evento.iP, factor)) {
			IluminarNodo(m_NodoRadioMenor, true);
			Iluminar(false);
			dentro = true;
		}
		else {
			IluminarNodo(m_NodoRadioMenor, false);
		}

		if (!dentro && m_NodoRadioMenorReflejado.Hits(evento.iP, factor)) {
			IluminarNodo(m_NodoRadioMenorReflejado, true);
			Iluminar(false);
			dentro = true;
		}
		else {
			IluminarNodo(m_NodoRadioMenorReflejado, false);
		}

		if (!dentro) {

			if (HitTest(evento.iP.x, evento.iP.y,evento.c->RelacionImagenPantalla())) {
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

		return;
	}
}

void GNC::GCS::Widgets::WElipse::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WElipse::HitTest(float x, float y, const GNC::GCS::Vector& threshold)
{
	bool dentro = true;
	TNodo nodo(x,y);
	float difference = 0.0f;
	if (m_NormaRadioMayor > m_NormaRadioMenor)
	{
		difference = (((nodo - m_focus[0]).Norma2() + (nodo - m_focus[1]).Norma2()) - 2 * m_NormaRadioMayor);
	} 
	else 
	{
		difference = (((nodo - m_focus[0]).Norma2() + (nodo - m_focus[1]).Norma2()) - 2 * m_NormaRadioMenor);
	}

	dentro  = std::abs(difference) < (m_NodoRadioMayor.m_Size * threshold).Norma2();

	return dentro;
}

bool GNC::GCS::Widgets::WElipse::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{

	bool dentro = true;

	if (m_NumNodos == 0) {
		dentro = false;
	}
	else {
		for (unsigned int i = 0; dentro && i < m_NumNodos; ++i) {
			dentro = m_Nodos[i].DentroDePoligono2(vertices, numVertices);
		}
	}

	return dentro;
}

void GNC::GCS::Widgets::WElipse::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	GNC::GCS::Vector escala = c->RelacionImagenPantalla();

	glLineWidth(m_LineWidthProp);

	// Sombras
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

	// Sombra del cuerpo
	wColorSombra.Aplicar();
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < m_NumNodos; ++i) {
		glVertex2d(m_Nodos[i].x + fsombrax , m_Nodos[i].y - fsombray);
	}
	glEnd();

	// Sombra de los puntos
	glPointSize(WIDGET_SIZE_PUNTO);

	if (m_Seleccionado && m_MouseDown)
		glPointSize(WIDGET_SIZE_PUNTO * WIDGET_INCREMENTO_DETALLE_SELECCIONADO);

	glBegin(GL_POINTS);
		glVertex2d(m_NodoRadioMayor.x + fsombrax, m_NodoRadioMayor.y - fsombray);
		glVertex2d(m_NodoRadioMayorReflejado.x + fsombrax, m_NodoRadioMayorReflejado.y - fsombray);
		glVertex2d(m_NodoRadioMenor.x + fsombrax, m_NodoRadioMenor.y - fsombray);
		glVertex2d(m_NodoRadioMenorReflejado.x + fsombrax, m_NodoRadioMenorReflejado.y - fsombray);
	glEnd();

	// Highlights
	bool nodoEnabled = false;

	if (m_NodoRadioMayor.m_Iluminado)
	{
		wDibujarElipseDegradado(m_NodoRadioMayor, wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
		nodoEnabled = true;
	}

	if (m_NodoRadioMayorReflejado.m_Iluminado)
	{
		wDibujarElipseDegradado(m_NodoRadioMayorReflejado, wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
		nodoEnabled = true;
	}

	if (m_NodoRadioMenor.m_Iluminado)
	{
		wDibujarElipseDegradado(m_NodoRadioMenor, wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
		nodoEnabled = true;
	}

	if (m_NodoRadioMenorReflejado.m_Iluminado)
	{
		wDibujarElipseDegradado(m_NodoRadioMenorReflejado, wRadioPuntoIluminado * escala, wColorPuntoIluminadoInterior, wColorPuntoIluminadoExterior);
		nodoEnabled = true;
	}

	// Dibujado
	wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_LINE_LOOP);
	for (unsigned int i = 0; i < m_NumNodos; ++i) {
		glVertex2d(m_Nodos[i].x, m_Nodos[i].y);
	}
	glEnd();

	// Lineas internas
	if (nodoEnabled)
	{
		glLineWidth(1.0f);
		glEnable(GL_LINE_STIPPLE);
		wColorAngulo.Aplicar();
		glLineStipple(3, 0xAAAA);

		glBegin(GL_LINES);
			glVertex2d(m_NodoRadioMayor.x, m_NodoRadioMayor.y);
			glVertex2d(m_NodoRadioMayorReflejado.x, m_NodoRadioMayorReflejado.y);
		glEnd();

		glBegin(GL_LINES);
			glVertex2d(m_NodoRadioMenor.x, m_NodoRadioMenor.y);
			glVertex2d(m_NodoRadioMenorReflejado.x, m_NodoRadioMenorReflejado.y);
		glEnd();

		glDisable(GL_LINE_STIPPLE);
	}


	// Nodos
	glPointSize(WIDGET_SIZE_PUNTO);

	wAplicarColor(m_Iluminado || m_NodoRadioMayor.m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_POINTS);
		glVertex2d(m_NodoRadioMayor.x, m_NodoRadioMayor.y);
	glEnd();

	wAplicarColor(m_Iluminado || m_NodoRadioMayorReflejado.m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_POINTS);
		glVertex2d(m_NodoRadioMayorReflejado.x, m_NodoRadioMayorReflejado.y);
	glEnd();

	wAplicarColor(m_Iluminado || m_NodoRadioMenor.m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_POINTS);
		glVertex2d(m_NodoRadioMenor.x, m_NodoRadioMenor.y);
	glEnd();

	wAplicarColor(m_Iluminado || m_NodoRadioMenorReflejado.m_Iluminado, m_MouseDown, m_Seleccionado);
	glBegin(GL_POINTS);
		glVertex2d(m_NodoRadioMenorReflejado.x, m_NodoRadioMenorReflejado.y);
	glEnd();
}

void GNC::GCS::Widgets::WElipse::Recalcular()
{
	m_Centro = m_NodoRadioMayor.PuntoMedio(m_NodoRadioMayorReflejado);
	m_RadioMayor = m_NodoRadioMayor - m_Centro;
	m_RadioMenor = m_NodoRadioMenor - m_Centro;

	m_NormaRadioMayor = m_RadioMayor.Norma2();
	m_NormaRadioMenor = m_RadioMenor.Norma2();
	
	//calculate focus points
	double fDistance = sqrt(std::abs(m_NormaRadioMayor*m_NormaRadioMayor - m_NormaRadioMenor*m_NormaRadioMenor));	
	//difference from the formula => the sum of distance from point to focus has to be = 2*bigger radious
	if (m_NormaRadioMayor > m_NormaRadioMenor)
	{
		m_focus[0] = m_Centro +  m_RadioMayor.Normalizado() * fDistance;
		m_focus[1]= m_Centro +  m_RadioMayor.Normalizado() * (-fDistance);
	} else {
		m_focus[0] = m_Centro +  m_RadioMenor.Normalizado() * fDistance;
		m_focus[1] = m_Centro +  m_RadioMenor.Normalizado() * (-fDistance);
	}

	m_Angulo = m_RadioMayor.AnguloSobreAbscisa();

	float step = M_2PI / m_NumNodos;
	float angulo = 0.0f;
	for ( unsigned int i = 0; i < m_NumNodos; ++i, angulo += step )
	{
		GNC::GCS::Vector v (m_NormaRadioMayor * cos(angulo), m_NormaRadioMenor * sin(angulo));
		m_Nodos[i] =  m_Centro + v.RotacionSobreZ(m_Angulo);
	}

	m_Modificado = true;

	const float suma = m_NormaRadioMayor + m_NormaRadioMenor;
	const float diff = std::max<float>(m_NormaRadioMayor, m_NormaRadioMenor) - std::min<float>(m_NormaRadioMayor, m_NormaRadioMenor);
	const float rel = diff / suma;
	m_Perimetro = M_PI * suma * (3.0f - std::sqrt (4.0f - (rel * rel)));
	m_AreaMetrica = M_PI * m_NormaRadioMayor * m_NormaRadioMenor;

	//std::cout << this << std::endl;

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

void GNC::GCS::Widgets::WElipse::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
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

	
	std::string strRadioMayor;
	std::string strRadioMenor;

	{
		std::ostringstream os;
		if (m_NormaRadioMayor > 1000.0f) {
			os << m_NormaRadioMayor / 1000.0f << " m.";
		}
		else if (m_NormaRadioMayor > 10.0f) {
			os << m_NormaRadioMayor / 10.0f << " cm.";
		}
		else {
			os << m_NormaRadioMayor << " mm.";
		}
		strRadioMayor = os.str();
	}
	{
		std::ostringstream os;
		if (m_NormaRadioMenor > 1000.0f) {
			os << m_NormaRadioMenor / 1000.0f << " m.";
		}
		else if (m_NormaRadioMenor > 10.0f) {
			os << m_NormaRadioMenor / 10.0f << " cm.";
		}
		else {
			os << m_NormaRadioMenor << " mm.";
		}
		strRadioMenor = os.str();
	}

	if (m_NormaRadioMayor > m_NormaRadioMenor) {
		primerMapa[_Std("Greater Radius")] = strRadioMayor;
		primerMapa[_Std("Lower Radius")] = strRadioMenor;
	}
	else {
		primerMapa[_Std("Greater Radius")] = strRadioMenor;
		primerMapa[_Std("Lower Radius")] = strRadioMayor;
	}
}

//endregion

void GNC::GCS::Widgets::WElipse::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();
	TVector Nodos[m_NumNodos];

	Recalcular();

	TVector NPuntos[4] =  { c->Reproyectar(m_NodoRadioMayor) * c->factorReescalado, c->Reproyectar(m_NodoRadioMayorReflejado) * c->factorReescalado, c->Reproyectar(m_NodoRadioMenor) * c->factorReescalado, c->Reproyectar(m_NodoRadioMenorReflejado) * c->factorReescalado };

	for (unsigned int i = 0; i < m_NumNodos; ++i) {
		Nodos[i] = c->Reproyectar(m_Nodos[i]) * c->factorReescalado;
	}

	cairo_set_line_width(c->cr, m_LineWidthProp);

	// Color del cuerpo
	wColorNormal.AplicarCairo(c->cr);

	// Sombra de los puntos
	for (int i = 0; i < 4; ++i) {
		cairo_arc(c->cr, NPuntos[i].x, NPuntos[i].y, (WIDGET_SIZE_PUNTO / 2.0f) * escala.x, 0, M_2PI);
		cairo_fill(c->cr);
	}

	// Dibujado del cuerpo
	for (unsigned int i = 0; i < m_NumNodos; ++i) {
		if(i == 0) {
			cairo_move_to(c->cr, Nodos[i].x, Nodos[i].y);
		}
		cairo_line_to(c->cr, Nodos[i].x, Nodos[i].y);
	}
	cairo_line_to(c->cr, Nodos[0].x, Nodos[1].y);
	cairo_stroke(c->cr);
}
