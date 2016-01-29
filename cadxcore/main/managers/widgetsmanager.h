/*
 *  
 *  $Id: widgetsmanager.h $
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


#include <wx/event.h>

#include <api/iwidgets.h>
#include <api/iwidgetsmanager.h>
#include <api/observers/iwidgetsobserver.h>
#include <api/observers/ieventsobserver.h>
#include <api/controllers/ieventscontroller.h>

//----------------------------------------------------------------------------------------------------
//Forward Declarations

namespace GNC {
	namespace GCS {
		class IWidgetsRenderer;
	}
}
class TimerAnimation;
class wxMenu;

//End Forward Declarations

namespace GNC {
	class WidgetsManager : public wxEvtHandler, public GNC::GCS::IWidgetsManager, public GNC::GCS::IEventsObserver {

	public:
		WidgetsManager(GNC::GCS::IVista* pVista);
		virtual ~WidgetsManager();

	//----------------------------------------------------------------------------------------------------
	//region "Interfaz especifica"

		virtual void RenderPrivate(GNC::GCS::Contexto3D* c);
		
		virtual void TickAnimation(double freq);

		virtual void EnableAnimation();

		virtual void DisableAnimation();

		virtual void Render();

		virtual void OffScreenRender(GNC::GCS::Contexto3D* c);

		virtual void Modificado();
		
		virtual void ModificadoPorWidget(GNC::GCS::Widgets::IWidget* w);

		virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton& evento);

		virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento);

		virtual void DefaultMouseEventProcess(GNC::GCS::Events::EventoRaton& evento);

		virtual void DefaultKeyEventProcess(GNC::GCS::Events::EventoTeclado& event);

		// Propaga a todos los widgets no ocultos un hit test de seleccion. Los widgets se seleccionan si validan un hittest con el bounding box especificado.
		virtual void PropagarHitTestSeleccion(GNC::GCS::Vector* vertices, int numVertices);

	//endregion

	//----------------------------------------------------------------------------------------------------
	//region "Interfaz de registro de Renderers"

	public:

		virtual void SetRendererActivo(GNC::GCS::IWidgetsRenderer* pRenderer);

		virtual void InsertarRenderer(GNC::GCS::IWidgetsRenderer* pRenderer);

		virtual void EliminarRenderer(GNC::GCS::IWidgetsRenderer* pRenderer);

		virtual void LiberarRecursos(GNC::GCS::IWidgetsRenderer* pRenderer);

	//endregion

	//----------------------------------------------------------------------------------------------------
	//region "Interfaz de registro de IWidget's"

		virtual void InsertarWidget(GNC::GCS::Widgets::IWidget* pWidget, bool propagarEvento=true);

		virtual void EliminarWidget(const GNC::GCS::Widgets::IWidget* pWidget, bool propagarEvento=true);

		/** Remove all widgets except anyone with this tid **/
		virtual void RemoveAllWidgetsExceptTID(long tid, bool propagateEvents=true);

		virtual void RemoveUserAnnotationWidgets(bool propagateEvents = true);

		virtual void EliminarWidget(GNC::GCS::IWidgetsManager::IteradorListaWidgets& it, bool propagarEvento=true);

		virtual void EliminarTodosLosWidgets(bool propagarEventos=true);

		/** Set visible on all widgets matching TID **/
		virtual void SetVisibleByTID(bool visible, long tid);

		virtual void OcultarTodosLosWidgets(bool ocultar, long GID);

		virtual void EliminarTodosLosWidgets(long GID);

		virtual void OcultarWidgetsDeSubVista(bool ocultar, long vid);

		virtual GNC::GCS::ListaWidgets& GetListaWidgets();

		virtual void GetListaWidgetsSubVista(long vid, GNC::GCS::ListaWidgets& lista);

	//endregion

	//----------------------------------------------------------------------------------------------------
	//endregion "Interfaz de Observadores"

		virtual void NotificarCreacion(GNC::GCS::Widgets::IWidget* pWidget);

		virtual void NotificarDestruccion(GNC::GCS::Widgets::IWidget* pWidget);

		virtual void NotificarModificacion(GNC::GCS::Widgets::IWidget* pWidget);

	//endregion

	//----------------------------------------------------------------------------------------------------
	//region "Interfaz annotation"

		virtual void SetAnnotator(const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& pAnnotador);
		virtual const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& GetAnnotator();
		virtual void ShowAnnotations(bool show);
		virtual bool IsShownAnnotations();

	//enderegion

	//----------------------------------------------------------------------------------------------------
	//region "Interfaz de acceso al sistema de eventos ginkgo"

		virtual void LanzarEventoCreacion(GNC::GCS::Widgets::IWidget* w);
		virtual void LanzarEventoModificacion(GNC::GCS::Widgets::IWidget* w);
		virtual void LanzarEventoDestruccion(GNC::GCS::Widgets::IWidget* w);

		virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

	//endregion

	//----------------------------------------------------------------------------------------------------
	//region "Gestion propia de WX"

		virtual void OnClickEnEliminar(wxCommandEvent& );

		virtual void OnClickEnPropiedades(wxCommandEvent& );

		virtual bool GetMenuContextual(wxMenu* menuContextual) const;

	//endregion

	//----------------------------------------------------------------------------------------------------
	//region "Cursores
		virtual void SetCursor(GNC::GCS::Widgets::TipoCursor cursor);
	//endregion

		bool m_mouseDerechoDown;
		bool m_mouseIzquierdoDown;
		bool m_middleDown;
		
	//----------------------------------------------------------------------------------------------------
	//region "Helpers"
	private:
		inline void ModificarRenderersDeWidget(GNC::GCS::Widgets::IWidget* w);
		TimerAnimation* m_pTimerAnimation;
		GNC::GCS::Ptr<GNC::GCS::IAnnotator> m_pAnnotator;
	//endregion
	};
}
