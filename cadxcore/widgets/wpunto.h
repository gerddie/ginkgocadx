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

#include <vector>

#include <api/iwidgets.h>
#include <api/widgetserializable.h>

#define WRADIO_CIRCULO 10.0f
#define WRADIO_CRUZ 17.0f

class vtkImageData;

namespace GNC {
	namespace GCS {
		namespace Widgets {
			class WTextBox;
			class WPunto;

			//---------------------------------------------------------------------

			class WPuntoBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:
				WPuntoBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid);

				~WPuntoBuilder();

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);
				
				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

				virtual void Render(GNC::GCS::Contexto3D *);


			protected:
			};

			//---------------------------------------------------------------------

			class WPunto : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::IWidgetSerializable {

			//region "Propiedades geometricas"
			private:
				GNC::GCS::Nodo   m_Nodo;
				GNC::GCS::Vector m_PosAntigua;
				WTextBox*                m_pTextBox;


			//region "Constructor y destructor"

			public:
				WPunto(IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodo, const char* nombre, long gid, bool oculto = false);

				~WPunto();

				//endregion

				//region "Serializado y desserializado"
				WPunto(IWidgetsManager* pManager, long vid, long gid, wxXmlNode* nodo);
				void initTextBox();

				wxXmlNode* Serializar(const std::string& nombreMedico);
				//endregion

				//region "Interfaz especifica"

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				void Recalcular();

				virtual void OffscreenRender(Contexto3D* c);

				virtual bool SoportaAutoBorrado() const {
					return true;
				}

				virtual GNC::GCS::Nodo& GetNodo() {
					return m_Nodo;
				}

				virtual bool EsMarcador() const
				{
					return true;
				}

				virtual bool DelimitaArea() const
				{
					return false;
				}

				virtual void GetPuntoAnclaje(GNC::GCS::Vector& punto, const GNC::GCS::Vector& escala) const
				{
					punto = m_Nodo + GNC::GCS::Vector(WRADIO_CRUZ, -WRADIO_CRUZ) * escala;
				}

				virtual void GetPosicion(GNC::GCS::Vector& posicion) const
				{
					posicion = m_Nodo;
				}

				virtual void Seleccionar(bool seleccionar)
				{
					if (m_Nodo.EstaSeleccionado() != seleccionar) {
						m_Nodo.Seleccionar(seleccionar);
						m_Seleccionado = seleccionar;
						m_Modificado = true;
					}
				}

				virtual void Iluminar(bool iluminar)
				{
					if (m_Nodo.EstaIluminado() != iluminar) {
						m_Nodo.Iluminar(iluminar);
						m_Iluminado = iluminar;
						m_Modificado = true;
					}
				}

				virtual bool EstaSeleccionado() const
				{
					return m_Nodo.EstaSeleccionado();
				}

				virtual bool EstaIluminado() const
				{
					return m_Nodo.EstaIluminado();
				}

				//endregion

				//region "Estado interno"
			private:
				bool            m_MouseDown;
				//endregion

				void SeleccionarNodo(GNC::GCS::Nodo &nodo, bool estado)
				{
					if (nodo.m_Seleccionado != estado)
					{
						nodo.m_Seleccionado = estado;
						m_Modificado = true;
					}
				}

				void IluminarNodo(GNC::GCS::Nodo &nodo, bool estado)
				{
					if (nodo.m_Iluminado != estado)
					{
						nodo.m_Iluminado = estado;
						m_Modificado = true;
					}
				}

			public:
				friend std::ostream& operator<<(std::ostream& out, const WPunto& w) {
					out << "[ P=" << w.m_Nodo << ", GID=" << w.m_GID << " UID=" << w.m_UID << " ]";
					return out;
				}

				friend std::ostream& operator<<(std::ostream& out, const WPunto* w) {
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
				virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);
			};
		}
	}
}
