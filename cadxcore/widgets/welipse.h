/*
 *  
 *  $Id: welipse.h $
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
#include <api/widgetserializable.h>
#include <vector>

class vtkImageData;

namespace GNC {
	namespace GCS {
		namespace Widgets {
			class WTextBox;
			//---------------------------------------------------------------------

			class WElipseBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Events::EventoRaton   TEventoRaton;
				typedef GNC::GCS::Events::EventoTeclado TEventoTeclado;
				typedef GNC::GCS::IWidgetsManager        TWidgetsManager;
				typedef GNC::GCS::Widgets::IWidget       TWidget;

				WElipseBuilder(TWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WElipseBuilder();

				virtual void OnMouseEvents(TEventoRaton& event);
				virtual void OnKeyEvents(TEventoTeclado& event);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

				//region "Helpers"

				bool         m_Dentro;
				//endregion


			protected:
				TVector	                 m_NodoMoviendose;  // Cursor
				TVector                  m_Start;           // Origen del boundingBox
				TVector                  m_Stop;            // Destino del boundingBox

				TVector                  m_BB[4]; // Vertices del bounding box.

				bool                     m_MouseDown;
				Estado                   m_Estado;
			};

			//---------------------------------------------------------------------

			class WElipse : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::IWidgetSerializable {

			//region "Constructor y destructor"

			public:
				typedef GNC::GCS::Nodo   TNodo;
				typedef GNC::GCS::Vector TVector;
				typedef GNC::GCS::Matriz TMatriz;
				typedef GNC::GCS::Contexto3D TContexto;
				typedef GNC::GCS::Events::EventoRaton TEventoRaton;
				typedef GNC::GCS::Events::EventoTeclado TEventoTeclado;

				WElipse(TVector centro, TVector radio1, TVector radio2, IWidgetsManager* pManager, long vid, const char* nombre, long gid, GNC::GCS::Contexto3D* c);

				~WElipse();

				//endregion

				//region "Serializado y desserializado"
				WElipse(IWidgetsManager* pManager, long vid, long gid, wxXmlNode* nodo);
				
				void initTextBox(double rotation = 0.0);

				wxXmlNode* Serializar(const std::string& nombreMedico);
				//endregion

				//region "Interfaz especifica"

				virtual void OnMouseEvents(TEventoRaton&);

				virtual void OnKeyEvents(TEventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(TVector* vertices, int numVertices);

				virtual void Render(TContexto* c);

				virtual void OffscreenRender(Contexto3D* c);

				virtual bool EsMarcador() const
				{
					return true;
				}

				virtual bool DelimitaArea() const
				{
					return true;
				}

				virtual bool ContieneFast(const GNC::GCS::Vector& punto) const
				{
					return punto.DentroDeElipse(m_Centro, m_NormaRadioMayor, m_NormaRadioMenor, m_Angulo);
				}

				virtual void GetBoundingBox(GNC::GCS::Vector& bb0, GNC::GCS::Vector& bb1) const
				{
					if (m_NumNodos > 0) {
					}
					m_Nodos[0].IniciarBoundingBox(bb0, bb1);
					for (unsigned int i = 1; i < m_NumNodos; ++i) {
						m_Nodos[i].ExtenderBoundingBox(bb0, bb1);
					}

				}

				virtual void GetPosicion(GNC::GCS::Vector& posicion) const
				{
					posicion = m_Centro;
				}

				virtual void GetPuntoAnclaje(GNC::GCS::Vector& punto, const GNC::GCS::Vector& escala) const
				{
					punto = m_PosCursor + (GNC::GCS::Vector(10.0f, -10.0f) * escala);
				}

				virtual void GetCentro(GNC::GCS::Vector& centro) const
				{
					centro = m_Centro;
				}

				virtual float GetAreaMetrica() const
				{
					return m_AreaMetrica;
				}

				virtual float GetPerimetro() const
				{
					return m_Perimetro;
				}

				virtual bool SoportaAutoBorrado() const {
					return true;
				}
				//endregion

				//region "Estado interno"
			private:
				bool            m_MouseDown;
				//endregion

				//region "Propiedades geometricas"
			public:
				bool         m_Debug;
				static const unsigned int m_NumNodos = 100;

				TVector* m_Nodos;

				TVector m_PosCursor;

				TVector m_Centro;
				TVector m_RadioMayor;
				TVector m_RadioMenor;
				TNodo   m_NodoRadioMayor;
				TNodo   m_NodoRadioMenor;
				TNodo   m_NodoRadioMayorReflejado;
				TNodo   m_NodoRadioMenorReflejado;
				float   m_NormaRadioMayor;
				float   m_NormaRadioMenor;
				float   m_Angulo;
				TVector m_focus[2];
				TVector m_BB[4];
				
				float m_AreaMetrica;
				float m_Perimetro;				
				WTextBox*                m_pTextBox;

				friend std::ostream& operator<<(std::ostream& out, const WElipse& w) {
					out << "Elipse = [ centro = " << w.m_Centro << ", radioMayor = " << w.m_RadioMayor << ", radioMenor = " << w.m_RadioMenor << ", GID=" << w.m_GID << ", TID=" << w.m_TID << " UID=" << w.m_UID << " ]";
					return out;
				}

				friend std::ostream& operator<<(std::ostream& out, const WElipse* w) {
					if (w == NULL) {
						out << "[ NULL ]";
					}
					else {
						out << *w;
					}
					return out;
				}

			public:
				//endregion

				//region "Propiedades radiometricas"

				//endregion

			public:

				void Recalcular();

				virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);

			};
		}
	}
}
