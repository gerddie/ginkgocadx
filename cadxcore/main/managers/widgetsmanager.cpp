/*
 * This file is part of Ginkgo CADx
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


#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif

#include <wx/wx.h>
#include <wx/menu.h>
#include <wx/image.h>
#include <wx/timer.h>

#include <api/globals.h>
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include <api/iannotator.h>
#include <api/ivista.h>
#include <api/toolsystem/itool.h>
#include <api/toolsystem/itoolcontroller.h>

#include <eventos/modificacionwidget.h>

#include <api/westilo.h>
#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>
#include <widgets/wanotacionesquina.h>
#include <widgets/wanotacionposicion.h>
#include <widgets/wstaticrule.h>

#include "widgetsmanager.h"

#include <resources/ginkgoresourcesmanager.h>

#include <cairo/cairo.h>

#include <vtk/vtkginkgoimageviewer.h>

class TimerAnimation : public wxTimer
{
public:
        TimerAnimation(GNC::GCS::IWidgetsManager* pManager)
        {
                m_pManager = pManager;
                m_Frecuencia = 20.0f;
        }

        ~TimerAnimation() {}

        virtual void Notify()
        {
                m_pManager->TickAnimation(m_Frecuencia);
                //std::cout << "Tick()" << std::endl;
                m_pManager->Render();
        }

        void Enable()
        {
                if (IsRunning()) {
                        Stop();
                }
                Start((int)( 1000.0f * (1.0f/m_Frecuencia) ) );
        }

        void Disable()
        {
                if (IsRunning()) {
                        Stop();
                }
        }
protected:
        GNC::GCS::IWidgetsManager* m_pManager;
        double                     m_Frecuencia;
};

GNC::WidgetsManager::WidgetsManager(GNC::GCS::IVista* pVista):GNC::GCS::IWidgetsManager(pVista), m_pAnnotator(new GNC::GCS::IAnnotator(pVista))
{
        m_pRendererActivo = NULL;
        m_cursor = GNC::GCS::Widgets::CUR_FLECHA;
        m_mouseDerechoDown = false;
        m_mouseIzquierdoDown = false;
        m_middleDown = false;
        m_pTimerAnimation = new TimerAnimation(this);

        GNC::GCS::Widgets::WAnotador* pW = new GNC::GCS::Widgets::WAnotador(this,-1,"",-1);
        InsertarWidget(pW);
        GNC::GCS::Widgets::WAnotacionPosicion* pPosicion = new GNC::GCS::Widgets::WAnotacionPosicion(this,-1,"",-1);
        InsertarWidget(pPosicion);
        GNC::GCS::Widgets::WStaticRule* pStaticRule = new GNC::GCS::Widgets::WStaticRule(this,-1,"",-1);
        InsertarWidget(pStaticRule);
}

GNC::WidgetsManager::~WidgetsManager()
{
        if (m_pTimerAnimation != NULL) {
                delete m_pTimerAnimation;
                m_pTimerAnimation = NULL;
        }

        for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                pRenderer->WSetCurrent();
                for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                        GNC::GCS::Widgets::IWidget* w = (*it);
                        if(w->ReservaRecursos()) {
                                w->LiberarRecursos(pRenderer);
                        }
                }
        }

        GNC::GCS::ListaRenderers copia(m_Renderers); // Porque los renderers invocaran a EliminarRenderer de esta misma instancia;

        for (GNC::GCS::ListaRenderers::iterator it = copia.begin(); it != copia.end(); ++it) {
                GNC::GCS::IWidgetsRenderer* renderer = *it;

                renderer->SetManager(NULL);
        }

        m_Widgets.clear();

}

void GNC::WidgetsManager::TickAnimation(double freq)
{
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (!w->EstaOculto()) {
                        w->TickAnimation(freq);
                }
        }
}

void GNC::WidgetsManager::EnableAnimation()
{
        m_pTimerAnimation->Enable();
}

void GNC::WidgetsManager::DisableAnimation()
{
        m_pTimerAnimation->Disable();
}

inline void GNC::WidgetsManager::ModificarRenderersDeWidget(GNC::GCS::Widgets::IWidget* w)
{
        if (w != NULL && !w->EsTopLevel()) {
                for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                        GNC::GCS::IWidgetsRenderer* ren = *itr;
                        if (w->GetVID() != -1 && w->GetVID() == ren->GetVID()) {
                                ren->Modificar(true);
                        }
                }
        }
}

//----------------------------------------------------------------------------------------------------
//region "Interfaz especifica"

void GNC::WidgetsManager::RenderPrivate(GNC::GCS::Contexto3D* c)
{
        if (!m_Enabled) {
                return;
        }

        glPushAttrib(GL_ENABLE_BIT);
        bool widgetIluminado = false;
        // Primera pasada: widgets normales.
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (!w->EsTopLevel() && (w->GetVID() == c->GetRenderer()->GetVID() || w->GetVID() == -1) ) {
                        w->Render(c);
                        w->Modificar(false);
                }
                widgetIluminado = widgetIluminado || (!w->EstaOculto()&& w->EstaIluminado());
        }

        // Segunda pasada: widgets toplevel.
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (w->EsTopLevel()) {
                        w->Render(c);
                }
        }

        if (c->GetRenderer() == m_pRendererActivo) {
                for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); it != m_pBuilders.end(); ++it) {
                        (*it)->Render(c);
                }
        }

        //cursor
        if(widgetIluminado) {
                //encima de un widget selecc e iluminado => si esta pulsando el boton derecho moviendose, si no pues no
                if(m_mouseIzquierdoDown) {
                        SetCursor(GNC::GCS::Widgets::CUR_MANO_CERRADA);
                        //std::cout << "WIluminado: IZDown: Mano Cerrada " << std::endl;
                } else {
                        SetCursor(GNC::GCS::Widgets::CUR_MANO_ABIERTA);
                        //std::cout << "WIluminado !IZDown: Mano Abierta " << std::endl;
                }
        } else {
                bool cursorNotFound = true;
                if (m_mouseIzquierdoDown) {
                        //si no se lo pedimos al primer builder que encontremos
                        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); cursorNotFound && it != m_pBuilders.end(); ++it) {
                                if ((*it)->GetButtonMask().IsLeftEnabled()) {
                                        if((*it)->GetCursor() != GNC::GCS::Widgets::CUR_INVALIDO) {
                                                //std::cout << "!WIluminado: !DERDown: Builder cursor " << std::endl;
                                                SetCursor((*it)->GetCursor());
                                                cursorNotFound = false;
                                        }
                                }
                        }
                }
                if (m_middleDown) {
                        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); cursorNotFound && it != m_pBuilders.end(); ++it) {
                                if ((*it)->GetButtonMask().IsMiddleEnabled()) {
                                        if((*it)->GetCursor() != GNC::GCS::Widgets::CUR_INVALIDO) {
                                                //std::cout << "!WIluminado: !DERDown: Builder cursor " << std::endl;
                                                SetCursor((*it)->GetCursor());
                                                cursorNotFound = false;
                                        }
                                }
                        }
                }
                if (m_mouseDerechoDown) {
                        //builder que atienda al boton derecho
                        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); cursorNotFound && it != m_pBuilders.end(); ++it) {
                                if ((*it)->GetButtonMask().IsRightEnabled()) {
                                        if((*it)->GetCursor() != GNC::GCS::Widgets::CUR_INVALIDO) {
                                                //std::cout << "!WIluminado: !DERDown: Builder cursor " << std::endl;
                                                SetCursor((*it)->GetCursor());
                                                cursorNotFound = false;
                                        }
                                }
                        }
                }
                if (cursorNotFound) {
                        //try left...
                        //si no se lo pedimos al primer builder que encontremos
                        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); cursorNotFound && it != m_pBuilders.end(); ++it) {
                                if ((*it)->GetButtonMask().IsLeftEnabled()) {
                                        if((*it)->GetCursor() != GNC::GCS::Widgets::CUR_INVALIDO) {
                                                //std::cout << "!WIluminado: !DERDown: Builder cursor " << std::endl;
                                                SetCursor((*it)->GetCursor());
                                                cursorNotFound = false;
                                        }
                                }
                        }
                        if (cursorNotFound) {
                                SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
                        }
                }
        }

        c->GetRenderer()->Modificar(false);

        glPopAttrib();
}

void GNC::WidgetsManager::Render()
{
        if (!m_Enabled) {
                return;
        }

        bool seleccionEnActual = false;
        bool seleccionEnOtro = false;

        // Primera pasada: Widgets
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (!w->EsTopLevel() && !w->EstaOculto()) {
                        if (m_pRendererActivo) { // Si hay renderer activo, comprobamos las selecciones.
                                if (w->EstaSeleccionado()) {
                                        if (w->GetVID() == m_pRendererActivo->GetVID()) {
                                                seleccionEnActual = true;
                                        } else {
                                                seleccionEnOtro = true;
                                        }
                                }
                        }
                        if (w->EstaModificado()) {
                                w->Modificar(false);
                                ModificarRenderersDeWidget(w);
                        }
                }
        }

        if (seleccionEnActual && seleccionEnOtro) {
                // Segunda pasada opcional: Deseleccion de selecciones en renderers no activos
                for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                        GNC::GCS::Widgets::IWidget* w = *it;
                        if (!w->EsTopLevel() && !w->EstaOculto()) {
                                if (m_pRendererActivo) { // Si hay renderer activo, comprobamos las selecciones.
                                        if (w->EstaSeleccionado()) {
                                                if (w->GetVID() != m_pRendererActivo->GetVID()) {
                                                        w->Seleccionar(false);
                                                        w->Modificar(false);
                                                        ModificarRenderersDeWidget(w);
                                                }
                                        }
                                }
                        }
                }
        }

        for (GNC::GCS::ListaRenderers::iterator it = m_Renderers.begin(); it != m_Renderers.end(); ++it) {
                GNC::GCS::IWidgetsRenderer* ren = *it;
                if (ren->EstaModificado()) {
                        GTRACE("Renderer[" << ren->GetVID() << "] modificado. Rendering...");
                        //si el renderer es el activo, provoca un update inmediato, sino solo un refresh que hará un update cuando pueda
                        ren->PostRender(ren == m_pRendererActivo);
                        ren->Modificar(false);
                }
        }
}

void GNC::WidgetsManager::OffScreenRender(GNC::GCS::Contexto3D* c)
{
        if (!m_Enabled) {
                return;
        }

        // Primera pasada: Widgets normales
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (!w->EstaOculto() && !w->EsTopLevel() && w->GetVID() == c->OVID) {
                        w->OffscreenRender(c);
                }
        }

        // Primera pasada: Widgets top level
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (!w->EstaOculto() && w->EsTopLevel()) {
                        w->OffscreenRender(c);
                }
        }
}

void GNC::WidgetsManager::Modificado()
{
        if (m_pRendererActivo != NULL) {
                m_pRendererActivo->Modificar(true);
        }
}

void GNC::WidgetsManager::ModificadoPorWidget(GNC::GCS::Widgets::IWidget* w)
{
        this->ModificarRenderersDeWidget(w);
}

void GNC::WidgetsManager::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
        if (!m_Enabled) {
                return;
        }
        evento.Skip(true);

        if (!evento.c->IsOk()) {
                std::cerr << "Adverencia: Render omitido: singularidad en contexto" << std::endl;
        }

        //pillamos boton derecho e izquierdo down-up

        m_mouseIzquierdoDown = evento.LeftIsDown();
        m_middleDown = evento.MiddleIsDown();
        m_mouseDerechoDown = evento.RightIsDown();


        // Primera pasada: widgets toplevel.
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(!w->EstaOculto() && w->EsTopLevel() && !w->IgnoraEventos()) {
                        w->OnMouseEvents(evento);
                        if (w->EstaModificado()) {
                                Modificado();
                                if (w->RequiereNotificarCambios()) {
                                        w->NotificarCambios(false);
                                        NotificarModificacion(w);
                                }
                                w->Modificar(false);
                        }
                }
        }
        // Segunda pasada: widgets normales.
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(!w->EstaOculto() && !w->EsTopLevel() && !w->IgnoraEventos() && ( w->GetVID() == evento.c->GetRenderer()->GetVID())) {
                        w->OnMouseEvents(evento);
                        if (w->EstaModificado()) {
                                Modificado();
                                if (w->RequiereNotificarCambios()) {
                                        w->NotificarCambios(false);
                                        NotificarModificacion(w);
                                }
                                w->Modificar(false);
                        }
                        if (w->EstaIluminado() && ( evento.LeftUp() ||evento.LeftDown() )) { // Si el widget esta iluminado y este evento es up/down del boton izquierdo, forzamos render para cambiar el cursor.
                                Modificado();
                        }
                }
        }

        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); evento.GetSkipped() && it != m_pBuilders.end(); ++it) {
                (*it)->OnMouseEvents(evento);
                if (!evento.GetSkipped()) {
                        break;
                }
        }

        if (evento.GetSkipped()) {
                DefaultMouseEventProcess(evento);
                if (evento.GetSkipped()) {
                        if (this->m_pDefaultEventListenerDelegate != NULL) {
                                this->m_pDefaultEventListenerDelegate->OnMouseEvents(evento);
                        } else {
                                if (m_pView != NULL && evento.ButtonUp(GNC::GCS::Events::EventoRaton::EB_DERECHO)) {
                                        wxMenu menu;
                                        m_pView->GetToolController()->AppendsToolInMenu(this, &menu, GNC::GCS::ITool::TSeriesMenu);
                                        evento.c->pWindow->PopupMenu(&menu, wxPoint(evento.sX, evento.sY));
                                }
                        }
                }
        }
        if (m_pRendererActivo != NULL && m_pRendererActivo->EstaModificado()) {
                Render();
        }
}

void GNC::WidgetsManager::OnKeyEvents(GNC::GCS::Events::EventoTeclado& evento)
{
        if (!m_Enabled) {
                return;
        }
        evento.Skip(true);

        /* Paso de auto-borrado (si lo soporta el widget) */
        if (evento.GetKeyCode() == WXK_DELETE || evento.GetKeyCode() == WXK_BACK) {
                GNC::GCS::ListaWidgets lw;
                for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                        GNC::GCS::Widgets::IWidget* w = (*it);

                        if(!w->EstaOculto() && w->EstaSeleccionado() && w->SoportaAutoBorrado() && (w->GetVID() == -1 || w->GetVID() == evento.c->GetRenderer()->GetVID())) {
                                lw.push_back(w);

                        }
                }
                for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                        EliminarWidget(*it, true);
                        Modificado();
                }
        } else if (evento.GetKeyCode() == 'r' || evento.GetKeyCode() == 'R') {
                for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                        GNC::GCS::Widgets::IWidget* w = (*it);
                        w->Modificar(true);
                }
        }

        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); it != m_pBuilders.end(); ++it) {
                (*it)->OnKeyEvents(evento);
                if (!evento.GetSkipped()) {
                        break;
                }
        }

        // Primera pasada: widgets toplevel.
        for (IteradorListaWidgets it = m_Widgets.begin(); evento.GetSkipped() && it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(!w->EstaOculto() && w->EsTopLevel() && !w->IgnoraEventos() && (w->GetVID() == evento.c->GetRenderer()->GetVID())) {
                        w->OnKeyEvents(evento);
                        if (w->EstaModificado()) {
                                Modificado();
                                if (w->RequiereNotificarCambios()) {
                                        w->NotificarCambios(false);
                                        NotificarModificacion(w);
                                }
                        }
                }
        }
        // Segunda pasada: widgets normales.
        for (IteradorListaWidgets it = m_Widgets.begin(); evento.GetSkipped() && it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(!w->EstaOculto() && !w->EsTopLevel() && !w->IgnoraEventos() && (w->GetVID() == evento.c->GetRenderer()->GetVID())) {
                        w->OnKeyEvents(evento);
                        if (w->EstaModificado()) {
                                Modificado();
                                if (w->RequiereNotificarCambios()) {
                                        w->NotificarCambios(false);
                                        NotificarModificacion(w);
                                }
                        }
                }
        }

        if (evento.GetSkipped()) {
                DefaultKeyEventProcess(evento);
        }
        Render();
}

void GNC::WidgetsManager::DefaultMouseEventProcess(GNC::GCS::Events::EventoRaton& evento)
{
        if (!m_Enabled) {
                return;
        }

        /* Paso de mostrar menu si procede */
        if (evento.ButtonUp(GNC::GCS::Events::EventoRaton::EB_DERECHO)) {

                //std::cout << "DerUp! ¿Render?" << std::endl;

                SetCursor(GNC::GCS::Widgets::CUR_MANO_ABIERTA);

                GNC::GCS::ListaWidgets lw;

                for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                        GNC::GCS::Widgets::IWidget* w = (*it);
                        if(!w->EstaOculto() && w->EstaSeleccionado() && !w->IgnoraEventos() && (w->GetVID() == -1 || w->GetVID() == evento.c->GetRenderer()->GetVID())) {
                                lw.push_back(w);
                        }
                }

                if(lw.size()>0) {
                        //comprobamos si alguno de los seleccionados valida el hittest
                        for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                                GNC::GCS::Widgets::IWidget* w = (*it);
                                if(w->HitTest(evento.iP.x, evento.iP.y, evento.c->RelacionImagenPantalla())) {
                                        wxMenu menu;
                                        if(lw.size() == 1) {
                                                //se meten las opciones que pueda aportar el widget
                                                w->GetMenuContextual(&menu, evento.c->pWindow );
                                                if(menu.GetMenuItemCount()>0) {
                                                        menu.AppendSeparator();
                                                }
                                        }
                                        GetMenuContextual(&menu);
                                        //en este caso hay varios seleccionados pues se muestra el menu comun...
                                        evento.c->pWindow->PopupMenu(&menu,wxPoint(evento.sX, evento.sY));
                                        bool cursorNotFound = true;
                                        for (GNC::GCS::ListaBuilders::iterator it = m_pBuilders.begin(); cursorNotFound && it != m_pBuilders.end(); ++it) {
                                                if ((*it)->GetCursor() != GNC::GCS::Widgets::CUR_INVALIDO) {
                                                        SetCursor((*it)->GetCursor());
                                                        cursorNotFound = false;
                                                }
                                        }
                                        if (cursorNotFound) {
                                                //y si no pues si esta pulsando el derecho es que esta haciendo pan
                                                SetCursor(GNC::GCS::Widgets::CUR_FLECHA);
                                        }
                                        ConsumirEvento();
                                        break;
                                }
                        }
                }

                if(!(EventoConsumido())) {
                        if(lw.size()>0) {
                                //se deseleccionan los widgets seleccionados
                                for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                                        GNC::GCS::Widgets::IWidget* w = (*it);
                                        w->Seleccionar(false);
                                }
                        }
                        //hay que buscar si algun widget valida el hittest, en ese caso se selecciona y se muestra el menu
                        GNC::GCS::Widgets::IWidget* widgetMenu = NULL;
                        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                                GNC::GCS::Widgets::IWidget* w = (*it);
                                if(!w->EstaOculto() && !w->IgnoraEventos() && (w->GetVID() == -1 || w->GetVID() == evento.c->GetRenderer()->GetVID()) && w->HitTest(evento.iP.x, evento.iP.y, evento.c->RelacionImagenPantalla())) {
                                        widgetMenu = w;
                                        break;
                                }
                        }
                        if(widgetMenu != NULL) {
                                widgetMenu->Seleccionar(true);
                                wxMenu menu;
                                //se meten las opciones que pueda aportar el widget
                                widgetMenu->GetMenuContextual(&menu, evento.c->pWindow );
                                if(menu.GetMenuItemCount()>0) {
                                        menu.AppendSeparator();
                                }

                                GetMenuContextual(&menu);

                                //en este caso hay varios seleccionados pues se muestra el menu comun...
                                evento.c->pWindow->PopupMenu(&menu, wxPoint(evento.sX, evento.sY));
                                ConsumirEvento();
                        }
                }
        } else {
                evento.Skip(true);
        }
}

void GNC::WidgetsManager::DefaultKeyEventProcess(GNC::GCS::Events::EventoTeclado& evento)
{
        if (!m_Enabled) {
                return;
        }
        evento.Skip(true);
}

void GNC::WidgetsManager::PropagarHitTestSeleccion(GNC::GCS::Vector* vertices, int numVertices)
{
        if (!m_Enabled) {
                return;
        }
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(!w->EstaOculto() && !w->IgnoraEventos() && w->HitTest(vertices, numVertices)) {
                        w->Seleccionar(true);
                }
        }
}

//endregion

//----------------------------------------------------------------------------------------------------
//region "Interfaz de registro de Renderers"

void GNC::WidgetsManager::SetRendererActivo(GNC::GCS::IWidgetsRenderer* pRenderer)
{
        if (m_pRendererActivo != pRenderer) {
                if (m_pRendererActivo != NULL) {
                        m_pRendererActivo->Modificar(true);
                }
                m_pRendererActivo = pRenderer;
        }
}

void GNC::WidgetsManager::InsertarRenderer(GNC::GCS::IWidgetsRenderer* pRenderer)
{
        bool notfound = true;
        for (GNC::GCS::ListaRenderers::iterator it = m_Renderers.begin(); notfound && (it != m_Renderers.end()); ++it) {
                GNC::GCS::IWidgetsRenderer* renderer = *it;

                if (pRenderer == renderer) {
                        notfound = false;
                }
        }
        if (notfound == true) {
                m_Renderers.push_back(pRenderer);
        }

}

void GNC::WidgetsManager::EliminarRenderer(GNC::GCS::IWidgetsRenderer* pRenderer)
{
        bool notfound = true;
        GNC::GCS::ListaRenderers::iterator itpos = m_Renderers.end();
        for (GNC::GCS::ListaRenderers::iterator it = m_Renderers.begin(); notfound && (it != m_Renderers.end()); ++it) {
                GNC::GCS::IWidgetsRenderer* renderer = *it;

                if (pRenderer == renderer) {
                        if (renderer == m_pRendererActivo) {
                                m_pRendererActivo = NULL;
                        }
                        notfound = false;
                        itpos = it;
                }
        }
        if (itpos != m_Renderers.end()) {
                m_Renderers.erase(itpos);
        }
        if (m_pRendererActivo == NULL && m_Renderers.size() > 0) {
                m_pRendererActivo = m_Renderers.front();
        }
}



void GNC::WidgetsManager::LiberarRecursos(GNC::GCS::IWidgetsRenderer* pRenderer)
{
        pRenderer->WSetCurrent();
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                (*it)->LiberarRecursos(pRenderer);
        }
}

//endregion

//----------------------------------------------------------------------------------------------------
//region "Interfaz de registro de IWidget's"

void GNC::WidgetsManager::InsertarWidget(GNC::GCS::Widgets::IWidget* pWidget, bool propagarEvento)
{
        m_Widgets.push_back(pWidget);
        if (propagarEvento) {
                NotificarCreacion(pWidget);
        }
}

void GNC::WidgetsManager::EliminarWidget(const GNC::GCS::Widgets::IWidget* pWidget, bool propagarEvento)
{
        IteradorListaWidgets it;

        for (it = m_Widgets.begin(); it != m_Widgets.end() && (*it) != pWidget; ++it);

        if (it != m_Widgets.end()) {
                GNC::GCS::Widgets::IWidget* pRemovingWidget = (*it);
                if (pRemovingWidget != NULL) {
                        if (propagarEvento) {
                                NotificarDestruccion(pRemovingWidget);
                        }
                        if(pRemovingWidget->ReservaRecursos()) {
                                for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                        GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                        pRenderer->WSetCurrent();
                                        pRemovingWidget->LiberarRecursos(pRenderer);
                                }

                        }
                        delete pRemovingWidget;
                }
                m_Widgets.erase(it);
        }
}

void GNC::WidgetsManager::EliminarWidget(GNC::GCS::IWidgetsManager::IteradorListaWidgets& it, bool propagarEvento)
{
        if (it != m_Widgets.end()) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if (propagarEvento) {
                        NotificarDestruccion(w);
                }
                if(w->ReservaRecursos()) {
                        for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                pRenderer->WSetCurrent();
                                w->LiberarRecursos(pRenderer);
                        }
                }
                delete w;
                m_Widgets.erase(it);
        }
}

void GNC::WidgetsManager::RemoveAllWidgetsExceptTID(long tid, bool propagateEvents)
{
        GNC::GCS::ListaWidgets lw;
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if (w != NULL && w->GetTID() != tid) {
                        lw.push_back(w);
                }
        }
        for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if (propagateEvents) {
                        NotificarDestruccion(w);
                }
                if(w->ReservaRecursos()) {
                        for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                pRenderer->WSetCurrent();
                                w->LiberarRecursos(pRenderer);
                        }
                }
                m_Widgets.remove(w);
                delete w;
        }
}

void GNC::WidgetsManager::RemoveUserAnnotationWidgets(bool propagateEvents)
{
        GNC::GCS::ListaWidgets lw;
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if (w != NULL && w->GetTID() == TID_USER_ANNOTATION_WIDGET) {
                        lw.push_back(w);
                }
        }
        for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if (propagateEvents) {
                        NotificarDestruccion(w);
                }
                if(w->ReservaRecursos()) {
                        for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                pRenderer->WSetCurrent();
                                w->LiberarRecursos(pRenderer);
                        }
                }
                m_Widgets.remove(w);
                delete w;
        }
}

void GNC::WidgetsManager::EliminarTodosLosWidgets(bool propagarEventos)
{
        while (!m_Widgets.empty()) {
                GNC::GCS::Widgets::IWidget* w = m_Widgets.front();
                if (propagarEventos) {
                        NotificarDestruccion(w);
                }
                if(w->ReservaRecursos()) {
                        for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                pRenderer->WSetCurrent();
                                w->LiberarRecursos(pRenderer);
                        }
                }
                delete w;
                m_Widgets.erase(m_Widgets.begin());
        }
}

/** Set visible on all widgets matching TID **/
void GNC::WidgetsManager::SetVisibleByTID(bool visible, long tid)
{
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(w->GetTID() == tid) {
                        if(!visible && w->ReservaRecursos()) {
                                for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                        GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                        pRenderer->WSetCurrent();
                                        w->LiberarRecursos(pRenderer);
                                }
                        }
                        w->Ocultar(!visible);
                }
        }
}

void GNC::WidgetsManager::OcultarTodosLosWidgets(bool ocultar, long GID)
{

        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(w->GetGID() == GID) {
                        if(ocultar && w->ReservaRecursos()) {
                                for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                                        GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
                                        pRenderer->WSetCurrent();
                                        w->LiberarRecursos(pRenderer);
                                }
                        }
                        w->Ocultar(ocultar);
                }
        }
}

void GNC::WidgetsManager::EliminarTodosLosWidgets(long GID)
{
        GNC::GCS::ListaWidgets listaEliminados;
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);
                if(w->GetGID() == GID) {
                        listaEliminados.push_back(w);
                }
        }
        for(IteradorListaWidgets it = listaEliminados.begin(); it!= listaEliminados.end(); ++it) {
                EliminarWidget((*it));
        }
}

void GNC::WidgetsManager::OcultarWidgetsDeSubVista(bool /*ocultar*/, long /*vid*/)
{
        /*
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
        	GNC::GCS::Widgets::IWidget* w = *it;
        	if(w->GetVID() == vid){
        		if( w->EstaOculto() != ocultar ) {
        			if (ocultar && w->ReservaRecursos()) {
        				for (ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr)
        				{
        					GNC::GCS::IWidgetsRenderer* pRenderer = *itr;
        					pRenderer->WSetCurrent();
        					w->LiberarRecursos(pRenderer);
        				}
        			}
        			w->Ocultar(ocultar);
        			Modificado();
        		}
        	}
        }
         */
}

GNC::GCS::ListaWidgets& GNC::WidgetsManager::GetListaWidgets()
{
        return m_Widgets;
}

void GNC::WidgetsManager::GetListaWidgetsSubVista(long vid, GNC::GCS::ListaWidgets& lista)
{
        for(GNC::GCS::IWidgetsManager::IteradorListaWidgets it = m_Widgets.begin(); it!= m_Widgets.end(); ++it) {
                if((*it)->GetVID() == vid) {
                        lista.push_back((*it));
                }
        }
}

//endregion

//----------------------------------------------------------------------------------------------------
//endregion "Interfaz de Observadores"

void GNC::WidgetsManager::NotificarCreacion(GNC::GCS::Widgets::IWidget* pWidget)
{

        for (IteradorListaObservadores it = m_Observadores.begin(); it != m_Observadores.end(); ++it) {
                GNC::GCS::Widgets::IObservadorWidget* pObservador = *it;
                //TODO hasta que no se adapten los observadores de las herramientas no se pondra esta linea que es la buena
                //if ((pObservador->VID == -1 || pObservador->VID == m_pRendererActivo->GetVID())&& pObservador->GID == pWidget->GetGID()) {
                if (pObservador->GID == pWidget->GetGID()) {
                        //
                        pObservador->OnWidgetCreado(pWidget);
                }
        }
}

void GNC::WidgetsManager::NotificarDestruccion(GNC::GCS::Widgets::IWidget* pWidget)
{

        for (IteradorListaObservadores it = m_Observadores.begin(); it != m_Observadores.end(); ++it) {
                GNC::GCS::Widgets::IObservadorWidget* pObservador = *it;
                //TODO hasta que no se adapten los observadores de las herramientas no se pondra esta linea que es la buena
                //if ((pObservador->VID == -1 || pObservador->VID == m_pRendererActivo->GetVID())&& pObservador->GID == pWidget->GetGID()) {
                if (pObservador->GID == pWidget->GetGID()) {
                        //
                        pObservador->OnWidgetDestruido(pWidget);
                }
        }
}

void GNC::WidgetsManager::NotificarModificacion(GNC::GCS::Widgets::IWidget* pWidget)
{

        for (IteradorListaObservadores it = m_Observadores.begin(); it != m_Observadores.end(); ++it) {
                GNC::GCS::Widgets::IObservadorWidget* pObservador = *it;
                //TODO hasta que no se adapten los observadores de las herramientas no se pondra esta linea que es la buena
                //if ((pObservador->VID == -1 || pObservador->VID == m_pRendererActivo->GetVID())&& pObservador->GID == pWidget->GetGID()) {
                if (pObservador->GID == pWidget->GetGID()) {
                        //
                        pObservador->OnWidgetModificado(pWidget);
                }
        }
}

//endregion

//----------------------------------------------------------------------------------------------------
//region "Interfaz general"

void GNC::WidgetsManager::SetAnnotator(const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& pAnnotador)
{
        m_pAnnotator = pAnnotador;
        Modificado();
        Render();
}

const GNC::GCS::Ptr<GNC::GCS::IAnnotator>& GNC::WidgetsManager::GetAnnotator()
{
        return m_pAnnotator;
}

void GNC::WidgetsManager::ShowAnnotations(bool show)
{
        //buscamos los widgets anotadores y los establecemos el modo
        for( GNC::GCS::ListaWidgets::iterator it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (w->GetVID() == -1 && w->GetGID() == -1) {
                        GNC::GCS::Widgets::WAnotador* pAnotador = dynamic_cast<GNC::GCS::Widgets::WAnotador*> (w);
                        if (pAnotador != NULL) {
                                pAnotador->Ocultar(!show);
                        } else {
                                GNC::GCS::Widgets::WStaticRule* pStaticRule = dynamic_cast<GNC::GCS::Widgets::WStaticRule*> (w);
                                if (pStaticRule != NULL) {
                                        pStaticRule->Ocultar(!show);
                                }
                        }
                }
        }
}

bool GNC::WidgetsManager::IsShownAnnotations()
{
        for( GNC::GCS::ListaWidgets::iterator it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (w->GetVID() == -1 && w->GetGID() == -1) {
                        GNC::GCS::Widgets::WAnotador* pAnotador = dynamic_cast<GNC::GCS::Widgets::WAnotador*> (w);
                        if (pAnotador != NULL) {
                                return !pAnotador->EstaOculto();
                        }
                }
        }
        return false;
}
//enderegion

//----------------------------------------------------------------------------------------------------
//region "Interfaz de acceso al sistema de eventos ginkgo"

void GNC::WidgetsManager::LanzarEventoCreacion(GNC::GCS::Widgets::IWidget* w)
{
        GNC::Entorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionWidget(m_pView,GNC::GCS::Events::EventoModificacionWidget::WidgetCreado, w));
}

void GNC::WidgetsManager::LanzarEventoModificacion(GNC::GCS::Widgets::IWidget* w)
{
        GNC::Entorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionWidget(m_pView,GNC::GCS::Events::EventoModificacionWidget::WidgetModificado, w));
}

void GNC::WidgetsManager::LanzarEventoDestruccion(GNC::GCS::Widgets::IWidget* w)
{
        GNC::Entorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionWidget(m_pView,GNC::GCS::Events::EventoModificacionWidget::WidgetDestruido, w));
}

void GNC::WidgetsManager::ProcesarEvento(GNC::GCS::Events::IEvent* /*evt*/)
{

}

//endregion

void GNC::WidgetsManager::OnClickEnPropiedades(wxCommandEvent& )
{
        //eliminamos los widgets seleccionados que soporten autoborrar
        GNC::GCS::ListaWidgets lw;
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);

                if(!w->EstaOculto() && w->EstaSeleccionado()) {
                        lw.push_back(w);
                }
        }
        for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                //mostrar propiedades de los widgets
                (*it)->MostrarPropiedades(GNC::Entorno::Instance()->GetVentanaRaiz());
        }
}

void GNC::WidgetsManager::OnClickEnEliminar(wxCommandEvent& )
{
        //eliminamos los widgets seleccionados que soporten autoborrar
        GNC::GCS::ListaWidgets lw;
        for (IteradorListaWidgets it = m_Widgets.begin(); it != m_Widgets.end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = (*it);

                if(!w->EstaOculto() && w->EstaSeleccionado() && w->SoportaAutoBorrado() ) {
                        lw.push_back(w);

                }
        }
        for (IteradorListaWidgets it = lw.begin(); it != lw.end(); ++it) {
                EliminarWidget(*it, true);
                Modificado();
        }
}

bool GNC::WidgetsManager::GetMenuContextual(wxMenu* menuContextual) const
{
        if (!m_Enabled) {
                return false;
        }
        wxMenuItem* pMenuEliminar = new wxMenuItem(menuContextual, -1, _("Delete"), _("Delete Widget"), wxITEM_NORMAL );
        menuContextual->Connect(pMenuEliminar->GetId(),(int)wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(WidgetsManager::OnClickEnEliminar),NULL,(wxEvtHandler*)this);
#ifdef __WXMSW__
        pMenuEliminar->SetBitmaps( GinkgoResourcesManager::MenuIcons::GetIcoEliminarWidget());
#else
        pMenuEliminar->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEliminarWidget());
#endif
        menuContextual->Append(pMenuEliminar);

        menuContextual->AppendSeparator();

        wxMenuItem* pMenuPropiedades = new wxMenuItem(menuContextual, -1, _("Properties ..."), _("Widget Properties"), wxITEM_NORMAL );
        menuContextual->Connect(pMenuPropiedades->GetId(),(int)wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(WidgetsManager::OnClickEnPropiedades),NULL,(wxEvtHandler*)this);
        menuContextual->Append(pMenuPropiedades);
        return true;
}

void GNC::WidgetsManager::SetCursor(GNC::GCS::Widgets::TipoCursor cursor)
{
        if(m_cursor != cursor) {
                for (GNC::GCS::ListaRenderers::iterator itr = m_Renderers.begin(); itr != m_Renderers.end(); ++itr) {
                        switch (cursor) {
                        case GNC::GCS::Widgets::CUR_TRANSPARENTE:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorTransparente()));
                                break;
                        case GNC::GCS::Widgets::CUR_LUPA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorLupa()));
                                break;
                        case GNC::GCS::Widgets::CUR_SELECCION:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorSeleccionar()));
                                break;
                        case GNC::GCS::Widgets::CUR_FLECHA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(wxCURSOR_ARROW));
                                break;
                        case GNC::GCS::Widgets::CUR_MANO_ABIERTA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorManoAbierta()));
                                break;
                        case GNC::GCS::Widgets::CUR_MANO_CERRADA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorManoCerrada()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_LINEA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearLinea()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_ANGULO_COBB:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearAnguloCobb()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_ANGULO_NORMAL:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearAnguloNormal()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_ANOTACION:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearAnotacion()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_ELIPSE:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearElipse()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_PUNTO:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearPunto()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_POLIGONO:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearPoligono()));
                                break;
                        case GNC::GCS::Widgets::CUR_CREAR_RECTANGULO:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorCrearRectangulo()));
                                break;
                        case GNC::GCS::Widgets::CUR_WINDOW_LEVEL:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorWindowLevel()));
                                break;
                        case GNC::GCS::Widgets::CUR_ANCLA_SIMPLE:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorAnclaSimple()));
                                break;
                        case GNC::GCS::Widgets::CUR_ANCLA_COMPUESTA:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetCursorAnclaCompuesta()));
                                break;
                        case GNC::GCS::Widgets::CUR_ROTATION:
                                (*itr)->m_c->pWindow->SetCursor(wxCursor(GinkgoResourcesManager::Cursors::GetRotationCursor()));
                                break;
                        case GNC::GCS::Widgets::CUR_INVALIDO:
                                break;
                        }
                }
                m_cursor = cursor;
        }
}
