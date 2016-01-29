/*
 *  
 *  $Id: wrule.h $
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
#pragma once
#include <api/iwidgets.h>
#include <api/iwidgetsmanager.h>
#include <api/widgetserializable.h>
#include <api/observers/ieventsobserver.h>
#include <widgets/openglhelper.h>

class wxXmlNode;

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {

			class WTextBox;

			class WRule;

			class EXTAPI WRuleBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Nodo   TNodo;
				typedef GNC::GCS::Matriz TMatriz;

				WRuleBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, bool supportsCallibration);

				~WRuleBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:

				bool                           m_MouseDown;
				WRule*                         m_pReglaTemp;
				bool                           m_SupportsCallibration;

			};

			class EXTAPI WRule : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::IWidgetSerializable, GNC::GCS::IEventsObserver {

			public:
				GNC::GCS::Nodo                  m_Vertices[2];

//region "Constructor y destructor"

			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Matriz TMatriz;

				//static const unsigned int m_NumNodos = 100;

				GNC::GCS::Vector m_StartPointMov;

				WRule(IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodos[2], bool supportsCallibration, const char* nombre = "");

				~WRule();

//endregion

				//region "Serializado y desserializado"
				WRule(IWidgetsManager* pManager, long vid, wxXmlNode* nodo);

				wxXmlNode* Serializar(const std::string& nombreMedico);

				//endregion


				//region "Interfaz especifica"

				virtual bool SoportaAutoBorrado() const {
					return true;
				}

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void OffscreenRender(Contexto3D* c);

				virtual float Distancia() {
					return m_Distancia;
				}

				//endregion

				//region menu contextual
				virtual bool GetMenuContextual(wxMenu* pMenu, wxWindow* pParent) ;
				//endregion

				virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

				//region "Estado interno"
			private:
				bool            m_MouseDown;
				//endregion

				//region "Propiedades geometricas"
			private:
				GNC::GCS::Nodo   m_Centro;
				GNC::GCS::Vector m_CentroAntiguo;
				float            m_Angulo;
				float            m_Pendiente;

				WTextBox*        m_pTextBox;

				GNC::GCS::Vector m_Escala;

				friend std::ostream& operator<<(std::ostream& out, const WRule& w) {
					out << "[ P0=" << w.m_Vertices[0] << ", P1=" << w.m_Vertices[1] <<
					", Centro=" << w.m_Centro << ", Angulo=" << w.m_Angulo << "=" << w.m_Angulo*180.0f/M_PI <<
					", Pendiente=" << w.m_Pendiente << ", Longitud=" << w.m_Distancia << ", CursorPoint=" << w.m_PosCursor << " ]";
					return out;
				}

			public:
				float            m_Distancia;				

			protected:
				bool             m_SupportsCallibration;

			public:

				void Recalcular(GNC::GCS::IWidgetsRenderer* renderer, bool building);

			public:
				//endregion
				virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);
			};
		}
	}
}
