/*
 *  
 *  $Id: iwidgets.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include "iwidgets.h"
#include "iwidgetsmanager.h"
#include "icontexto.h"
#include "widgetserializable.h"

#include <api/controllers/icontroladorpermisos.h>

#include <wx/xml/xml.h>
#include <wx/menu.h>
#include <wx/propiedades/wxpropiedades.h>

#include <api/iwidgetsrenderer.h>
#include <api/internationalization/internationalization.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <sstream>

//region "Serializado y desserializado"

GNC::GCS::Nodo GNC::GCS::Nodo::Deserializar(wxXmlNode* nodo)
{
	Nodo n;
	wxString strTmp = nodo->GetAttribute(wxT("x"),wxT("0.0"));
	double doubleTmp;
	strTmp.ToDouble(&doubleTmp);
	n.x = (TComp) doubleTmp;

	strTmp = nodo->GetAttribute(wxT("y"),wxT("0.0"));
	strTmp.ToDouble(&doubleTmp);
	n.y = (TComp) doubleTmp;

	strTmp = nodo->GetAttribute(wxT("size"),wxT("10.0"));
	strTmp.ToDouble(&doubleTmp);
	n.m_Size = (float) doubleTmp;

	n.m_Seleccionado = false;
	n.m_Iluminado = false;
	return n;
}

wxXmlNode* GNC::GCS::Nodo::Serializar()
{
	wxXmlNode* resultado = new wxXmlNode(NULL,wxXML_ELEMENT_NODE,wxT("node"));
	resultado->AddAttribute(new wxXmlProperty(wxT("x"),wxString::Format(wxT("%f"),x)));
	resultado->AddAttribute(new wxXmlProperty(wxT("y"),wxString::Format(wxT("%f"),y)));
	resultado->AddAttribute(new wxXmlProperty(wxT("size"),wxString::Format(wxT("%f"),m_Size)));
	return resultado;
}

//endregion

GNC::GCS::Widgets::IWidgetBuilder::IWidgetBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid, bool activar)
{
	m_ButtonMask = buttonMask;
	m_GID = gid;
	m_pManager = pManager;
	if(activar) {
		Activar();
	}

	m_FontSizeProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.fontSize").ObtenerValor<float>();
	m_MaxTextWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.maxTextWidth").ObtenerValor<float>();
	m_LineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.lineWidth").ObtenerValor<float>();
	m_DraggingLineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.draggingLineWidth").ObtenerValor<float>();
}

GNC::GCS::Widgets::IWidgetBuilder::~IWidgetBuilder()
{
	m_pManager->UnRegisterBuilder(this);
}

/* Obtiene el id de grupo de los widget construidos (tipo) */
long GNC::GCS::Widgets::IWidgetBuilder::GetGID()
{
	return m_GID;
}

void GNC::GCS::Widgets::IWidgetBuilder::Activar()
{
	m_pManager->RegisterBuilder(this);
}

void GNC::GCS::Widgets::IWidgetBuilder::Desactivar()
{
	m_pManager->UnRegisterBuilder(this);
}

void GNC::GCS::Widgets::IWidgetBuilder::LanzarEventoCreacion(IWidget* w)
{
	m_pManager->LanzarEventoCreacion(w);
}

void GNC::GCS::Widgets::IWidgetBuilder::LanzarEventoModificacion(IWidget* w)
{
	m_pManager->LanzarEventoModificacion(w);
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::IWidgetBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_INVALIDO;
}



GNC::GCS::Widgets::IWidget::IWidget(GNC::GCS::IWidgetsManager* pManager, long vid, const char* Nombre, long gid, long tid, long uid)
{
	m_pManager            = pManager;
	m_pRenderer           = NULL;
	m_Nombre              = Nombre;
	m_VID                 = vid;
	m_TID                 = tid;
	m_GID                 = gid;
	m_UID                 = uid;
	m_Seleccionado        = false;
	m_Iluminado           = false;
	m_Modificado          = false;
	m_Oculto			  = false;
	m_ReservaRecursos     = false;
	m_TopLevel            = false;
	m_NotificarCambios    = false;
	m_IgnorarEventos      = false;


	m_FontSizeProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.fontSize").ObtenerValor<float>();
	m_MaxTextWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.maxTextWidth").ObtenerValor<float>();
	m_LineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.lineWidth").ObtenerValor<float>();
	m_DraggingLineWidthProp = GNC::GCS::IControladorPermisos::Instance()->Get("style", "widgets.draggingLineWidth").ObtenerValor<float>();
}

GNC::GCS::Widgets::IWidget::~IWidget()
{
}

//endregion

//region "Interfaz especifica"

//region Interfaz de dibujado de overlays

bool GNC::GCS::Widgets::IWidget::EsMarcador() const
{
	return false;
}

bool GNC::GCS::Widgets::IWidget::DelimitaArea() const
{
	return false;
}

bool GNC::GCS::Widgets::IWidget::DelimitaTendencia() const
{
	return false;
}

bool GNC::GCS::Widgets::IWidget::ContieneFast(const GNC::GCS::Vector& ) const
{
	return false;
}

void GNC::GCS::Widgets::IWidget::GetBoundingBox(GNC::GCS::Vector& , GNC::GCS::Vector& ) const
{

}

void GNC::GCS::Widgets::IWidget::GetPosicion(GNC::GCS::Vector& ) const
{

}

void GNC::GCS::Widgets::IWidget::GetPuntoAnclaje(GNC::GCS::Vector& , const GNC::GCS::Vector& ) const
{
}

//region Propiedades exclusivas de widgets de area

float GNC::GCS::Widgets::IWidget::GetAreaMetrica() const
{
	return 0.0f;
}

float GNC::GCS::Widgets::IWidget::GetPerimetro() const
{
	return 0.0f;
}

void GNC::GCS::Widgets::IWidget::GetVertices(std::list<GNC::GCS::Vector>& ) const
{
}
//endregion

//region Propiedades exclusivas de widgets de tendencia

void GNC::GCS::Widgets::IWidget::GetDistancia(GNC::GCS::Vector& ) const
{
}

void GNC::GCS::Widgets::IWidget::GetRecorrido(std::list<GNC::GCS::Vector>& ) const
{
}

//endregion

//endregion Interfaz de dibujado de overlays

void GNC::GCS::Widgets::IWidget::LiberarRecursos(GNC::GCS::IWidgetsRenderer* /*pKey*/)
{
}

//endregion

//region "Interfaz generica"

void GNC::GCS::Widgets::IWidget::LanzarEventoModificacion() {
	m_pManager->LanzarEventoModificacion(this);
}

void GNC::GCS::Widgets::IWidget::LanzarEventoDestruccion() {
	m_pManager->LanzarEventoDestruccion(this);
}

//endregion

//region menu contextual

bool GNC::GCS::Widgets::IWidget::GetMenuContextual(wxMenu*, wxWindow* ) {
	return false;
}

inline std::string FloatToString(float valor) {
	std::ostringstream ostr;
	ostr.setf(std::ios::floatfield, std::ios::fixed );
	ostr.precision(2);
	ostr << valor;
	return ostr.str();
}

inline std::string PuntoToString(GNC::GCS::Vector valor) {
	std::ostringstream ostr;
	ostr << valor;
	return ostr.str();
}

std::string GNC::GCS::Widgets::IWidget::PuntoToStringMundo(const GNC::GCS::Vector& valor) {
	double wp[4];
	std::stringstream out;
	out.precision(3);
	const double ip[3] = {valor.x, valor.y,
                              static_cast<double>(m_pManager->GetRendererActivo()->m_pImageViewer->GetTindex())};
	m_pManager->GetRendererActivo()->m_pImageViewer->CoordenadasImagenACoordenadasMundo(ip, wp);
	out << "[ x = " << wp[0] << ", y = " << wp[1] << ", z=" << wp[2] << "  ]";
	return out.str();
}

void GNC::GCS::Widgets::IWidget::MostrarPropiedades(wxWindow* pParent)
{
	GNC::GUI::wxPropiedades::TMapaPropiedades mapaPropiedades;
	mapaPropiedades[_Std("Name")] = m_Nombre;
	if(this->DelimitaArea()) {
		{
			std::ostringstream          os;
			os.setf(std::ios_base::floatfield, std::ios_base::fixed );
			os.precision(2);
			os.fill('0');
			float area = GetAreaMetrica();
			if (area > 1000000.0f) {
				os << area / 1000000.0f << " m².";
			}
			else if (area > 100.0f) {
				os << area / 100.0f << " cm².";
			}
			else {
				os << area << " mm².";
			}
			mapaPropiedades[_Std("Area")] = os.str();
		}
		{
			float perimetro = GetPerimetro();
			std::ostringstream          os;
			os.setf(std::ios_base::floatfield, std::ios_base::fixed );
			os.precision(2);
			os.fill('0');
			if (perimetro > 1000.0f) {
				os << perimetro / 1000.0f << " m.";
			}
			else if (perimetro > 10.0f) {
				os << perimetro / 10.0f << " cm.";
			}
			else {
				os << perimetro << " mm.";
			}
			mapaPropiedades[_Std("Perimeter")] = os.str();
		}
	}
	if(this->DelimitaTendencia()) {
		std::list<GNC::GCS::Vector> recorrido;
		GetRecorrido(recorrido);
		{
			std::ostringstream ostr;
			unsigned int i =0;
			for(std::list<GNC::GCS::Vector>::iterator it = recorrido.begin(); it!= recorrido.end(); ++it, i++) {
				ostr << PuntoToString((*it));
				if((i+1) != recorrido.size())
					ostr << "," << std::endl;
			}
			mapaPropiedades[_Std("Travel (I)")] = ostr.str();
		}
		{
			std::stringstream out;
			unsigned int i =0;
			for(std::list<GNC::GCS::Vector>::iterator it = recorrido.begin(); it!= recorrido.end(); ++it, i++) {
				out << PuntoToStringMundo(*it);
				if((i+1) != recorrido.size())
					out << "," << std::endl;
			}
			mapaPropiedades[_Std("Travel (M)")] = out.str();
		}
	}



	GNC::GUI::wxPropiedades::TListaMapasPropiedades listaMapas;
	listaMapas.push_back(mapaPropiedades);

	InsertarPropiedades(listaMapas);

	//si es serializable...
/*	GNC::GCS::Widgets::IWidgetSerializable* pSerializable = dynamic_cast<GNC::GCS::Widgets::IWidgetSerializable*>(this);
	if(pSerializable != NULL) {
		listaMapas.push_back(pSerializable->GetPropiedades());
	}*/
	GNC::GUI::wxPropiedades pProp(pParent,m_Nombre, listaMapas);
	pProp.ShowModal();

}

void GNC::GCS::Widgets::IWidget::InsertarPropiedades(TListaMapasPropiedades &/*listaMapaPropiedades*/)
{
}

//endregion
