/*
 *  
 *  $Id: wmarcadoarea.h $
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
#include <widgets/observers/iobservadormarcadoarea.h>


//----------------------------------------------------------------------------------------------------
//region "Forward Declarations"
class vtkImageData;
namespace GNC {
	namespace GCS {
		namespace Widgets {
			
			typedef std::list<GNC::GCS::Nodo> VerticesPoligono;
			
			class WMarcadoArea;
		}
	}
}
//endregion


namespace GNC {
	namespace GCS {
		namespace Widgets {

			//====================================================================================================
			//= Builder de marcado
			//====================================================================================================

			class EXTAPI WMarcadoAreaBuilder : public GNC::GCS::Widgets::IWidgetBuilder, public GNC::GCS::Widgets::Notificadores::INotificadorMarcadoArea {
			
			public:
				typedef enum ModoMarcado {
					MP_Lazo,      // Poligono volatil. Existencia maxima: 1.
					MP_Normal   // Poligono persistente. Existencia maxima: Inf.
				} ModoMarcado;

				WMarcadoAreaBuilder(ModoMarcado modo, vtkImageData* pImageData, GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid, GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador = NULL);

				~WMarcadoAreaBuilder();

				void EliminarWidget(bool notificar = false);

				void SetRadioNodo(float radio)
				{
					m_RadioNodo = radio;
					m_DistanciaMinimaCuadrado = radio * 2;
					m_DistanciaMinimaCuadrado *= m_DistanciaMinimaCuadrado;

				}

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual GNC::GCS::Widgets::TipoCursor GetCursor();

			protected:

				GNC::GCS::Vector                          m_PosCursor;
				VerticesPoligono                          m_Vertices;
				GNC::GCS::Widgets::IWidgetBuilder::Estado m_Estado;

				int                                       m_NumNodos;
				bool                                      m_MouseDown;
				float                                     m_RadioNodo;
				float                                     m_DistanciaMinimaCuadrado;
				GNC::GCS::Widgets::WMarcadoArea*          m_pPoligonoCreado;
				vtkImageData*                             m_pImageData;
				ModoMarcado                               m_Modo;

			};
			
			//====================================================================================================
			//= Widget de marcado
			//====================================================================================================
			class EXTAPI WMarcadoArea : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::Notificadores::INotificadorMarcadoArea {

			//----------------------------------------------------------------------------------------------------
			//region "Constructor y destructor"
			public:
				WMarcadoArea(WMarcadoAreaBuilder::ModoMarcado modo, IWidgetsManager* pManager, unsigned int vid, VerticesPoligono& vertices, long gid, GNC::GCS::Widgets::Observadores::IObservadorMarcadoArea* pObservador = NULL, const char* nombre = "");

				~WMarcadoArea();

			//endregion

				virtual bool EsMarcador() const
				{
					return m_Modo == WMarcadoAreaBuilder::MP_Normal;
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
					if (m_Vertices.size() > 0) {

						VerticesPoligono::const_iterator it = m_Vertices.begin();
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

				virtual float GetAreaMetrica() const
				{
					return m_AreaMetrica;
				}

				virtual float GetPerimetro() const
				{
					return m_Perimetro;
				}

				virtual bool SoportaAutoBorrado() const {
					if (m_Modo == WMarcadoAreaBuilder::MP_Normal) {
						return true;
					}
					else {
						return false;
					}
				}

			//----------------------------------------------------------------------------------------------------
			//region "Interfaz especifica"

				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);
	
			//endregion
	
			//----------------------------------------------------------------------------------------------------
			//region "Estado interno"
			private:
				bool              m_MouseDown;
				GNC::GCS::Vector  m_PosCursor;
			//endregion

			//----------------------------------------------------------------------------------------------------
			//region "Propiedades geometricas"
			private:

				VerticesPoligono  m_Vertices;
				float                    m_RadioNodo;

				GNC::GCS::Vector m_Centro;
				float                    m_AreaMetrica;
				float                    m_Perimetro;


				friend std::ostream & operator<<(std::ostream& out, const WMarcadoArea& /*w*/)
				{
					return out;
				}

			public:
				virtual void GetVertices(std::list<GNC::GCS::Vector>& vertices) const {
					for (VerticesPoligono::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
						vertices.push_back(*it);
					}
				}

				void GetNodos(std::list<GNC::GCS::Nodo>& vertices) {
					for (VerticesPoligono::const_iterator it = m_Vertices.begin(); it != m_Vertices.end(); ++it) {
						vertices.push_back(*it);
					}
				}
				
				const VerticesPoligono& Vertices() const
				{
					return m_Vertices;
				}

			//endregion

			private:
				WMarcadoAreaBuilder::ModoMarcado m_Modo;
			
			
				
				
			//----------------------------------------------------------------------------------------------------
			//region "Helpers"
			public:
				
				virtual void Recalcular();
				
			//endregion
			};
		}
	}
}
