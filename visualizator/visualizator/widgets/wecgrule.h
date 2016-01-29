/*
 *  
 *  $Id: wecgrule.h $
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

namespace GNC
{
	namespace GCS 
	{
		namespace Widgets
		{
			class WTextBox;
		}
	}
}

namespace GNKVisualizator {
	namespace Widgets {
		class WECGRule;

		class EXTAPI WECGRuleBuilder : public GNC::GCS::Widgets::IWidgetBuilder {
		public:

			typedef GNC::GCS::Vector TVector;
			typedef GNC::GCS::Nodo   TNodo;
			typedef GNC::GCS::Matriz TMatriz;

			WECGRuleBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask);

			~WECGRuleBuilder();

			virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

			virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

			virtual void Render(GNC::GCS::Contexto3D* c);

			virtual GNC::GCS::Widgets::TipoCursor GetCursor();

		protected:

			bool                           m_MouseDown;
			WECGRule*                      m_pReglaTemp;

		};

		class EXTAPI WECGRule : public GNC::GCS::Widgets::IWidget 
		{
		friend class WECGRuleBuilder;
		public:

			typedef GNC::GCS::Vector TVector;
			typedef GNC::GCS::Matriz TMatriz;

			WECGRule(GNC::GCS::IWidgetsManager* pManager, long vid, GNC::GCS::Nodo nodos[2], const char* nombre = "");
			~WECGRule();

			virtual bool SoportaAutoBorrado() const;

			virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

			virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

			virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

			virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

			virtual void Render(GNC::GCS::Contexto3D* c);

			virtual void OffscreenRender(GNC::GCS::Contexto3D* c);

			virtual float Distancia();

			bool isVertical();

			virtual bool GetMenuContextual(wxMenu* pMenu, wxWindow* pParent) ;

		protected:
			
			GNC::GCS::Nodo                   m_Vertices[2];
			GNC::GCS::Vector                 m_StartPointMov;
			GNC::GCS::Widgets::WTextBox*     m_pTextBox;
			bool                             m_MouseDown;

			float                            m_Distancia;			
		public:

			void Recalcular(GNC::GCS::IWidgetsRenderer* renderer);
			
		public:
			//endregion
			virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);
		};
	}
}
