/*
 *  
 *  $Id: wnota.cpp $
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
#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#include <list>
#include <vtkgl.h>
#include <cmath>
#include <cairo/cairo.h>

#include <wx/xml/xml.h>
#include <wx/menu.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/math/geometry.h>
#include <api/math/geometry3d.h>

#include "wnota.h"
#include <api/ievento.h>
#include <api/iwidgetsrenderer.h>
#include <api/icontexto.h>
#include <api/iwidgetsmanager.h>

#include <main/managers/widgetsmanager.h>

#include <widgets/gui/selecciontexto.h>

#include <resources/ginkgoresourcesmanager.h>
#include <api/westilo.h>

#include "openglhelper.h"

#include <vtkRenderer.h>
#include <vtkCamera.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {

			struct TextBoxTexture {
				std::string texto;
				TexturaCairo* m_textura;
				bool	correcta;	//indica si esta correctamente cargada
				TextBoxTexture()
				{
					m_textura = new GNC::GCS::TexturaCairo();
					m_textura->ActivarInterpolacion();
					texto = "";
					correcta = false;
				}
				~TextBoxTexture()
				{
					m_textura->Descargar();
					delete m_textura;
				}
			};

		}
	}
}

//====================================================================================================

GNC::GCS::Widgets::WTextBox::WTextBox(IWidgetsManager* pManager, long vid, GNC::GCS::Vector posicion, TNoteMode mode, const char* nombre, std::string texto) : IWidget(pManager, vid, nombre,0,TID_USER_ANNOTATION_WIDGET), 
	GNC::GCS::Widgets::IWidgetSerializable(),
	m_mode(mode),
	m_Enabled(true)
{
	m_Texto = texto;
	m_Vacio = false;
	m_MouseDown = false;
	m_ReservaRecursos = true;
	m_Posicion = posicion;
	m_pWidgetSeleccion = NULL;
}

GNC::GCS::Widgets::WTextBox::~WTextBox()
{
	LanzarEventoDestruccion();
	if (m_pWidgetSeleccion != NULL) {
		m_pWidgetSeleccion->Destroy();
		m_pWidgetSeleccion = NULL;
	}
}

//----------------------------------------------------------------------------------------------------
//region "Serializado y desserializado"
GNC::GCS::Widgets::WTextBox::WTextBox(IWidgetsManager* pManager, long vid, wxXmlNode* nodo):GNC::GCS::Widgets::IWidget(pManager, vid, "Nota de Texto"), 
	GNC::GCS::Widgets::IWidgetSerializable(nodo),
	m_mode(TNM_Static),
	m_Enabled(true)
{

	wxString strTmp = nodo->GetAttribute(wxT("x"),wxT("0.0"));
	double doubleTmp;
	strTmp.ToDouble(&doubleTmp);
	m_Posicion.x = (float) doubleTmp;

	strTmp = nodo->GetAttribute(wxT("y"),wxT("0.0"));
	strTmp.ToDouble(&doubleTmp);
	m_Posicion.y = (float) doubleTmp;

	m_Texto = std::string(nodo->GetAttribute(wxT("text"), wxT("")).ToUTF8());

	m_Vacio = false;
	m_MouseDown = false;
	m_ReservaRecursos = true;
	m_pWidgetSeleccion = NULL;
}

wxXmlNode* GNC::GCS::Widgets::WTextBox::Serializar(const std::string& nombreMedico)
{
	wxXmlNode* resultado = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, wxT("text_widget"));
	//almacenamos los dos nodos
	resultado->AddAttribute(new wxXmlProperty(wxT("x"), wxString::Format(wxT("%f"),m_Posicion.x)));
	resultado->AddAttribute(new wxXmlProperty(wxT("y"), wxString::Format(wxT("%f"),m_Posicion.y)));
	resultado->AddAttribute(new wxXmlProperty(wxT("text"), wxString::FromUTF8(m_Texto.c_str())));
	//metadatos
	SerializarMetadatos(resultado, nombreMedico);
	return resultado;
}
//endregion

void GNC::GCS::Widgets::WTextBox::DoEditText(wxWindow* pWindow) 
{
	if (m_pWidgetSeleccion != NULL) {
		m_pWidgetSeleccion->Destroy();
		m_pWidgetSeleccion = NULL;
	}
	if (m_mode == TNM_Static) {		
		m_pWidgetSeleccion = new GNC::GCS::Widgets::Dialogos::SeleccionTexto(pWindow, this, m_pManager);
		m_pWidgetSeleccion->Center();
		m_pWidgetSeleccion->ShowModal();
	}
}

GNC::GCS::Widgets::TextBoxTexture* GNC::GCS::Widgets::WTextBox::GetTextura(void* renderer)
{
	if(m_mapaRecursos.find(renderer) == m_mapaRecursos.end()) {
		TextBoxTexture* pTextura = new TextBoxTexture();
		m_mapaRecursos[renderer] = pTextura;
		Recalcular(renderer);
	}
	return m_mapaRecursos[renderer];
}

void GNC::GCS::Widgets::WTextBox::Invalidar()
{
	for (TMapaRecursos::iterator it = m_mapaRecursos.begin(); it != m_mapaRecursos.end(); ++it) {
		TextBoxTexture* tn = (*it).second;
		tn->correcta = false;
	}
}

//----------------------------------------------------------------------------------------------------

const std::string& GNC::GCS::Widgets::WTextBox::GetTexto() const {
	return m_Texto;
}

void GNC::GCS::Widgets::WTextBox::SetTexto(const std::string& texto) {
	if (m_Texto != texto) {
		m_Texto = texto;
		ActualizarTimestampModificacion();
		Modificar(true);
	}
}

void GNC::GCS::Widgets::WTextBox::SetPosition(const TVector& pos)
{
	if (m_Posicion != pos)
	{
		m_Posicion = pos;
		ActualizarTimestampModificacion();
		Modificar(true);
	}
}

void GNC::GCS::Widgets::WTextBox::PropagarModificacion() {
	m_pManager->LanzarEventoModificacion(this);
}


void GNC::GCS::Widgets::WTextBox::Enable(bool enabled)
{
	m_Enabled = enabled;
}

bool GNC::GCS::Widgets::WTextBox::Enabled()
{
	return m_Enabled;
}

bool GNC::GCS::Widgets::WTextBox::SoportaAutoBorrado() const 
{
	return m_mode == TNM_Static;
}

void GNC::GCS::Widgets::WTextBox::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if(EstaOculto() || !Enabled()){
		return;
	}
	//------------------------------------------------------
	// Leaving
	else if (evento.Leaving() ) {
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
	if (evento.Dragging() && m_MouseDown) {
		if (EstaSeleccionado()) {
			Vector delta = evento.iP - m_PosCursor;
			m_PosCursor = evento.iP;
			m_Posicion += delta;
			LanzarEventoModificacion();
			Modificar(true);
			ConsumirEvento();
		}
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

		if (m_PosCursor.DentroDePoligono2(m_Quad, 4)) {
			if (evento.m_controlDown) { // Inversion de seleccion
				InvertirSeleccion();
			}
			else{
				Seleccionar(true);
			}
			dentro = true;
		}
		else { // Click fuera del texto
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
			ActualizarTimestampModificacion();
			m_MouseDown = false;
			ConsumirEvento();
		}
		return;
	}
	else if (evento.ButtonDClick(GNC::GCS::Events::EventoRaton::EB_IZQUIERDO)) {
		if (!EventoConsumido() && m_mode == TNM_Static) {
			if (evento.iP.DentroDePoligono2(m_Quad,4)) {
				DoEditText(evento.c->pWindow);
				ConsumirEvento();
			}
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

		if (m_PosCursor.DentroDePoligono2(m_Quad,4)) {
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

void GNC::GCS::Widgets::WTextBox::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
	if (evento.GetKeyCode() == WXK_ESCAPE) {
		Seleccionar(false);
	}
}

bool GNC::GCS::Widgets::WTextBox::HitTest(float x, float y, const GNC::GCS::Vector& )
{
	return GNC::GCS::Vector(x,y).DentroDePoligono2(m_Quad, 4);
}

bool GNC::GCS::Widgets::WTextBox::HitTest(GNC::GCS::Vector* vertices, int numVertices)
{
	return m_Quad[0].DentroDePoligono2(vertices, numVertices) && m_Quad[1].DentroDePoligono2(vertices, numVertices) && m_Quad[2].DentroDePoligono2(vertices, numVertices) && m_Quad[3].DentroDePoligono2(vertices, numVertices);
}

void GNC::GCS::Widgets::WTextBox::Render(GNC::GCS::Contexto3D* c)
{
	if(m_Oculto){
		return;
	}

	TextBoxTexture* tn = GetTextura(c->GetRenderer());
	if(tn->texto != m_Texto || !tn->correcta) {
		Recalcular(c->GetRenderer());
	}
	GNC::GCS::TexturaCairo* tc = tn->m_textura;

	if (!tc->contextoCreado) {
		//std::cerr << "Error: Contexto no creado" << std::endl;
		return;
	}
	if (!tc->texturaCargada) {
		tc->Cargar();
	}
	if (tc->texturaModificada) {
		tc->Actualizar();
	}

	TVector texP0;

	// Compute display pos
	GNC::GCS::Vector3D planePos(m_Posicion.x, m_Posicion.y, (double)c->pRenderer->m_pImageViewer->GetTindex());
	GNC::GCS::Vector3D worldPos;
	GNC::GCS::Vector3D displayPos;

	c->pRenderer->m_pImageViewer->UnProyect2D(planePos.v, worldPos.v);
	c->pRenderer->WorldToDisplay(worldPos.v, displayPos.v);

	GNC::GCS::Vector pos = (GNC::GCS::Vector(displayPos.x, displayPos.y)).Redondear();


	TWidgetColor backColor(0.0f, 0.0f, 1.0f, 0.8f);
	backColor.Aplicar();
	wDibujarFondoTexto2D(c, *tc, pos);

	wDibujarTexturaTexto2D(c, *tc, pos);

	if (m_Seleccionado || m_Iluminado) {
		wAplicarColor(m_Iluminado, m_MouseDown, m_Seleccionado);
		wDibujarBordeTexto2D(c, *tc, pos);
	}

	const GNC::GCS::Vector& escala = c->RefRelacionImagenPantalla();

	// Center

	m_Quad[0] =  m_Posicion;
	
	m_Quad[2].x = m_Quad[0].x + (double)tc->ancho * escala.x;
	m_Quad[2].y = m_Quad[0].y + (double)tc->alto * escala.y;

	m_Quad[1].x = m_Quad[2].x;
	m_Quad[1].y = m_Quad[0].y;

	m_Quad[3].x = m_Quad[0].x;
	m_Quad[3].y = m_Quad[2].y;

	// Apply camera transformation

	for (int i = 0; i < 4; i++) {
		m_Quad[i].RotarSobreZ( m_Posicion, c->rotacion);
	}

	#if 0 // DEBUG
	wAplicarColor(true, false, false);
	glBegin(GL_LINE_LOOP);
	glVertex2d(m_Quad[0].x, m_Quad[0].y);
	glVertex2d(m_Quad[1].x, m_Quad[1].y);
	glVertex2d(m_Quad[2].x, m_Quad[2].y);
	glVertex2d(m_Quad[3].x, m_Quad[3].y);
	glEnd();
	#endif

}

bool GNC::GCS::Widgets::WTextBox::GetMenuContextual(wxMenu *menuContextual, wxWindow* pParent) 
{
	if (m_mode == TNM_Static) 
	{
		m_pWidgetSeleccion = new GNC::GCS::Widgets::Dialogos::SeleccionTexto(pParent, this, m_pManager);

		wxMenuItem* pMenuEditar = new wxMenuItem(menuContextual, 1, wxT("Editar texto"), wxT("Editar contenido de la nota"), wxITEM_NORMAL );
		menuContextual->Connect(1,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( GNC::GCS::Widgets::Dialogos::SeleccionTexto::OnClickEnMenu), NULL, m_pWidgetSeleccion);

	#ifdef __WXMSW__
		pMenuEditar->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoEditar());
	#else
		pMenuEditar->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEditar());
	#endif

		menuContextual->Append(pMenuEditar);
		return true;
	} 
	else 
	{
		return false;
	}
}


void GNC::GCS::Widgets::WTextBox::Recalcular(void* renderer)
{
	TextBoxTexture* tn = GetTextura(renderer);
	GNC::GCS::TexturaCairo* tc = tn->m_textura;
	tn->correcta = true;

	if (!tc->contextoCreado) {
		tc->Redimensionar(32, 32);
	}

	std::string texto = m_Texto;
	if (texto.empty()) {
		texto = _Std("Default text");
	}

	m_Texto = texto;

	tn->texto = texto;

	if (!tc->contextoCreado) {
		tc->Redimensionar(2, 2);
	}

	cairo_font_options_t* options;

	cairo_select_font_face (tc->cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(tc->cr, m_FontSizeProp);
	options = cairo_font_options_create ();
	cairo_get_font_options(tc->cr, options);
	cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(tc->cr, options);
	cairo_font_options_destroy(options);
	options = NULL;

	m_TamTexto = GNC::GCS::GLHelper::calcularBoundingBox((*tc), m_Texto, false, m_MaxTextWidthProp); // Tamaño de la region en pixels

	tc->Redimensionar(std::ceil(m_TamTexto.x), std::ceil(m_TamTexto.y));
	m_TamTexto.Asignar(tc->ancho, tc->alto);

	cairo_select_font_face (tc->cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(tc->cr, m_FontSizeProp);
	options = cairo_font_options_create ();
	cairo_get_font_options(tc->cr, options);
	cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(tc->cr, options);
	cairo_font_options_destroy(options);
	options = NULL;

	cairo_set_operator(tc->cr, CAIRO_OPERATOR_SOURCE);
	cairo_set_source_rgba(tc->cr, 0.0f, 0.0f, 0.0f, 0.0f);
	cairo_paint(tc->cr);

	GNC::GCS::GLHelper::dibujarTexto((*tc), m_Texto, false, m_MaxTextWidthProp);
}

void GNC::GCS::Widgets::WTextBox::LiberarRecursos(GNC::GCS::IWidgetsRenderer* renderer)
{
	TMapaRecursos::iterator it = m_mapaRecursos.find(renderer);
	if(it!=m_mapaRecursos.end()) {
		delete (*it).second;
		m_mapaRecursos.erase(it);
	}
}

void GNC::GCS::Widgets::WTextBox::Seleccionar(bool seleccionado)
{
	if (m_Seleccionado != seleccionado) {
		m_Seleccionado = seleccionado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WTextBox::Iluminar(bool iluminado)
{
	if (m_Iluminado != iluminado) {
		m_Iluminado = iluminado;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WTextBox::Ocultar(bool oculto)
{
	if (m_Oculto != oculto) {
		m_Oculto = oculto;
		m_Modificado = true;
	}
}

void GNC::GCS::Widgets::WTextBox::InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades)
{
	TMapaPropiedades& primerMapa = listaMapaPropiedades.front();
	{
		 primerMapa[_Std("Text")] = m_Texto;
	}
}

void GNC::GCS::Widgets::WTextBox::OffscreenRender(GNC::GCS::Contexto3D* c)
{
	const GNC::GCS::Vector& escala = c->RefRelacionMundoPantallaOffscreen();
	TVector Pos = c->Reproyectar(m_Posicion) * c->factorReescalado;
	cairo_font_options_t* options;
	options = cairo_font_options_create ();

	cairo_select_font_face (c->cr, WIDGETS_TEXT_FONT_FAMILY, CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_NORMAL);
	cairo_set_font_size(c->cr, std::max(m_FontSizeProp * escala.x, (double)8.0f));
	cairo_font_options_set_antialias (options, CAIRO_ANTIALIAS_NONE);
	cairo_set_font_options(c->cr, options);

	
	GNC::GCS::Vector puntoTexto(Pos.x, Pos.y);
	puntoTexto.Redondear();
	cairo_set_source_rgba (c->cr, 1.0f, 1.0f, 1.0f, 1.0f);
	cairo_save(c->cr);
	cairo_translate(c->cr, puntoTexto.x, puntoTexto.y);
	GNC::GCS::Widgets::HelperTexto::dibujarTexto(c, m_Texto, false, m_MaxTextWidthProp * escala.x);
	cairo_restore(c->cr);

	cairo_font_options_destroy(options);
}
