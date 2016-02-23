/*
 *  Ginkgo CADx Project
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

#pragma once
#include <api/iwidgets.h>
#include <api/widgetserializable.h>
#include <vector>

class vtkImageData;

namespace GNC {
	namespace GCS {
		namespace Widgets {

			typedef std::list<GNC::GCS::Nodo> TPolygonVertexList;

			class WPolygon;
			class WTextBox;
		}
	}
}

namespace GNC {
	namespace GCS {
		namespace Widgets {

			class WPolygonBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:

				WPolygonBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask);

				~WPolygonBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:

				GNC::GCS::Nodo                          m_PosCursor;
				TPolygonVertexList                          m_Vertices;

			};

			class WPolygon : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::IWidgetSerializable {

				//region "Constructor y destructor"
			public:
				WPolygon(IWidgetsManager* pManager, unsigned int vid, TPolygonVertexList& vertices, GNC::GCS::Contexto3D* c, const char* nombre = "");

				~WPolygon();

				//endregion
				//region "Serializado y desserializado"
				WPolygon(IWidgetsManager* pManager, long vid, wxXmlNode* nodo);

				void initTextBox(double rotation = 0.0);

				wxXmlNode* Serializar(const std::string& nombreMedico);
				//endregion

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
					return punto.DentroDePoligono2(m_Vertices);
				}

				virtual void GetBoundingBox(GNC::GCS::Vector& bb0, GNC::GCS::Vector& bb1) const
				{
					if (!m_Vertices.empty()) {

						TPolygonVertexList::const_iterator it = m_Vertices.begin();
						(*it).IniciarBoundingBox(bb0, bb1);
						for (; it != m_Vertices.end(); ++it) {
							(*it).ExtenderBoundingBox(bb0, bb1);
						}
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


				virtual float GetAreaMetrica() const;

				virtual float GetPerimetro() const
				{
					return m_Perimetro;
				}

				virtual bool SoportaAutoBorrado() const {
					return true;
				}


				//region "Interfaz especifica"

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(const GNC::GCS::Nodo& p0, const GNC::GCS::Nodo& p1, const GNC::GCS::Vector& cursor, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void OffscreenRender(Contexto3D* c);

				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);

				//endregion

				//region "Estado interno"
			private:
				bool                    m_MouseDown;
				GNC::GCS::Nodo m_PosCursor;
				//endregion

				//region "Propiedades geometricas"
			private:

				TPolygonVertexList  m_Vertices;

				GNC::GCS::Vector m_Centro;
				float                    m_AreaMetrica;
				float                    m_Perimetro;
				WTextBox*                m_pTextBox;

				friend std::ostream & operator<<(std::ostream& out, const WPolygon& /*w*/)
				{
					return out;
				}

				void Recalcular();

			public:
				virtual void GetVertices(std::list<GNC::GCS::Vector>& vertices) const {
					for (TPolygonVertexList::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
						vertices.push_back(*it);
					}
				}

				void GetNodos(std::list<GNC::GCS::Nodo>& vertices) {
					for (TPolygonVertexList::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
						vertices.push_back(*it);
					}
				}

				//endregion

				virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);

			private:
			};
		}
	}
}
