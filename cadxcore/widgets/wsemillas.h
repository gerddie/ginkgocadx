/*
 *  
 *  $Id: wsemillas.h 3535 2011-03-18 17:57:05Z carlos $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-10 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once

#include <vector>

#include <api/iwidgets.h>

class vtkImageData;

namespace GNC {
	namespace GCS {
		namespace Widgets {

			namespace Semillas {

				//------------------------------------------------------------------

				class RepresentacionSemilla {
				public:

					typedef struct Color {
						float r;
						float g;
						float b;
						float a;

						inline void asignar(float r, float g, float b, float a)
						{
							this->r = r;
							this->g = g;
							this->b = b;
							this->a = a;
						};

					} Color;

					//region "Constructor y destructor"

					RepresentacionSemilla(bool soportaAutoBorrado = false)
					{
						m_RadioCruz = 20.0f;
						m_RadioCircunferencia = 15.0f;
						m_SoportaAutoBorrado = soportaAutoBorrado;
					}

					~RepresentacionSemilla()
					{

					}

					void SetAutoBorrado(bool autoBorrado) {
						m_SoportaAutoBorrado = autoBorrado;
					}

					bool SoportaAutoBorrado() {
						return m_SoportaAutoBorrado;
					}

					//region "Propiedades radiometricas"

					Color colorCruz;
					Color colorCircunferencia;
					Color colorCruzIluminada;
					Color colorCircunferenciaIluminada;
					Color colorCruzSeleccionada;
					Color colorCircunferenciaSeleccionada;
					float m_RadioCircunferencia;
					float m_RadioCruz;
					bool m_SoportaAutoBorrado;

					//endregion
				};

				typedef std::vector<GNC::GCS::Widgets::Semillas::RepresentacionSemilla*> ListaRepresentaciones;


			}

			//---------------------------------------------------------------------

			class WSemillasBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
			public:
				WSemillasBuilder(GNC::GCS::IWidgetsManager* pManager, int numMaxSemillas, GNC::GCS::Widgets::Semillas::ListaRepresentaciones* pRepresentaciones, long gid, vtkImageData* pImageData = NULL);

				~WSemillasBuilder();

				virtual void SetRepresentacionActiva(int idRepresentacion);

				virtual void OnMouseEvents(GNC::GCS::Eventos::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Eventos::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();


			protected:
				int                         m_NumMaxSemillas;
				GNC::GCS::Vector m_VerticesSeleccion[2];
				GNC::GCS::Nodo   m_Nodo;
				bool                      m_MouseDown;
				int                         m_RepresentacionActiva;
				Estado                   m_Estado;
				GNC::GCS::Widgets::Semillas::ListaRepresentaciones* m_pRepresentaciones;
				GNC::GCS::Widgets::Semillas::RepresentacionSemilla* m_pRepActiva;
				vtkImageData*                                       m_pImageData;

			};

			//---------------------------------------------------------------------

			class WSemillas : public GNC::GCS::Widgets::IWidget {


			//region "Constructor y destructor"

			public:
				WSemillas(IWidgetsManager* pManager, long vid, GNC::GCS::Widgets::Semillas::RepresentacionSemilla* pRepSemilla, GNC::GCS::Nodo nodo, const char* nombre, long gid, long tid, bool oculto = false);

				~WSemillas();

				//endregion

				//region "Serializado y desserializado"
				WSemillas(IWidgetsManager* pManager, long vid, GNC::GCS::Widgets::Semillas::RepresentacionSemilla* pRepSemilla, long gid, wxXmlNode* nodo);

				wxXmlNode* Serializar();
				//endregion

				//region "Interfaz especifica"

				virtual void OnMouseEvents(GNC::GCS::Eventos::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Eventos::EventoTeclado&);

				virtual bool HitTest(float x, float y, float umbralCuadrado);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual bool SoportaAutoBorrado() const {
					return m_pRepresentacion != NULL && m_pRepresentacion->SoportaAutoBorrado();
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
					punto = m_Nodo + GNC::GCS::Vector(10.0f, -10.0f) * escala;
				}

				virtual void GetPosicion(GNC::GCS::Vector& posicion) const
				{
					posicion = m_Nodo;
				}

				virtual void Seleccionar(bool seleccionar)
				{
					SeleccionarNodo(m_Nodo, seleccionar);
				}

				virtual void Iluminar(bool iluminar)
				{
					if (iluminar != m_Nodo.EstaIluminado()) {
						IluminarNodo(m_Nodo, iluminar);
						m_Modificado = true;
					}
				}

				virtual void SeleccionarNodo(GNC::GCS::Nodo& nodo, bool seleccionar)
				{
					if(seleccionar != nodo.EstaSeleccionado()){
						nodo.Seleccionar(seleccionar);
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

				//region "Propiedades geometricas"
			private:
				GNC::GCS::Nodo   m_Nodo;
				GNC::GCS::Vector m_PosAntigua;

			public:
				friend std::ostream& operator<<(std::ostream& out, const WSemillas& w) {
					out << "[ P=" << w.m_Nodo << ", GID=" << w.m_GID << ", TID=" << w.m_TID << " UID=" << w.m_UID << " ]";
					return out;
				}

				friend std::ostream& operator<<(std::ostream& out, const WSemillas* w) {
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

				GNC::GCS::Widgets::Semillas::RepresentacionSemilla* m_pRepresentacion;

				//endregion
			};
		}
	}
}
