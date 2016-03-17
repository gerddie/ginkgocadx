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

#pragma once

#include <list>
#include <map>
#include <string>
#include <sstream>

#include <api/api.h>
#include <api/ibutton.h>
#include <api/math/geometry.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif


#include "ievento.h"
#include "iwidgetsproperties.h"

//region "Forward declarations"

namespace GNC
{
namespace GCS
{
class Contexto3D;
class IWidgetsManager;
class IWidgetsRenderer;

namespace Widgets
{
class IObservadorWidget;
class IWidget;
}
}
}

class vtkWorldPointPicker;
class WidgetRepresentation;
class wxXmlNode;
class wxMenu;
class wxWindow;

// Forward declarations

//endregion

#define DEFAULT_POINT_SIZE 10.0f
#define TID_USER_ANNOTATION_WIDGET 1

namespace GNC
{

namespace GCS
{

class EXTAPI Nodo : public Vector
{
public:
        float m_Size;         // Tamano del punto
        bool  m_Seleccionado; // Indicador de seleccion
        bool  m_Iluminado;    // Indicador de iluminacion (hover)

        inline Nodo()
        {
                m_Size = DEFAULT_POINT_SIZE;
                m_Seleccionado = false;
                m_Iluminado = false;
        }

        inline Nodo(const Vector::TComp& x, const Vector::TComp& y) : Vector(x, y)
        {
                m_Size = DEFAULT_POINT_SIZE;
                m_Seleccionado = false;
                m_Iluminado = false;
        }

        inline Nodo(const Nodo& otro):Vector(otro)
        {

                *this = otro;
        }

        inline Nodo(const Vector& otro):Nodo()
        {

                *this = otro;
        }

        inline ~Nodo()
        {

        }

        //region "Serializado y desserializado"

        static Nodo Deserializar(wxXmlNode* nodo);

        wxXmlNode* Serializar();


        //endregion

        /* Comprueba si x, y cae centro del bounding box definido por:
         * x0,y0 == [ p.x - umbral * m_Size, p.y - umbral * m_Size ]
         * x1,y1 == [ p.x + umbral * m_Size, p.y + umbral * m_Size ]
         * Se computa: | p.x - x | < (umbral * m_Size) && | p.y - y | < (umbral * m_Size)
         *
         * umbral es un factor de escala que transforma coordenadas_mundo a pixels_pantalla.
         * Por tanto, el bounding box siempre estará definido en "m_Size" pixels pantalla.
         */
        inline bool Hits (const GNC::GCS::Vector& vector,  const Vector& umbral) const
        {
                GNC::GCS::Vector radix = umbral * m_Size;
                GNC::GCS::Vector bb[2];
                (vector - radix).IniciarBoundingBox(bb);
                (vector + radix).ExtenderBoundingBox(bb);
                if (this->DentroDeBoundingBox(bb[0], bb[1]))
                        return true;
                else
                        return false;
        }

        inline bool Hits ( Vector::TComp x,  Vector::TComp y,  const Vector& umbral) const
        {
                return Hits(GNC::GCS::Vector(x, y), umbral);
        }

        inline Nodo& operator = (const Nodo& otro)
        {
                this->x = otro.x;
                this->y = otro.y;
                this->m_Size = otro.m_Size;
                this->m_Seleccionado = otro.m_Seleccionado;
                this->m_Iluminado = otro.m_Iluminado;
                return *this;
        }

        inline Nodo& operator = (const Vector& otro)
        {
                this->x = otro.x;
                this->y = otro.y;
                return *this;
        }

        inline void Seleccionar(bool seleccionar)
        {
                m_Seleccionado = seleccionar;
        }

        inline void InvertirSeleccion()
        {
                Seleccionar(!EstaSeleccionado());
        }

        inline void Iluminar(bool iluminar)
        {
                m_Iluminado = iluminar;
        }

        inline bool EstaSeleccionado() const
        {
                return m_Seleccionado;
        }

        inline bool EstaIluminado() const
        {
                return m_Iluminado;
        }

        inline friend std::ostream& operator<<(std::ostream& out, const Nodo& n)
        {
                out << "[ x = " << n.x << ", y = " << n.y << "  ]";
                return out;
        }

        inline friend std::ostream& operator<<(std::ostream& out, const Nodo* n)
        {
                if (n == NULL) {
                        out << "[ NULL ]";
                } else {
                        out << *n;
                }
                return out;
        }

        //region "Helper para deserializar"
protected:
        //endregion

};

namespace Widgets
{

// Clase que construye widgets
class EXTAPI IWidgetBuilder
{

public:

        typedef enum Estado {
                WBS_Ninguno,
                WBS_Creando,
                WBS_Moviendo,
                WBS_Seleccionando
        } Estado;

protected:
        GNC::GCS::IWidgetsManager*  m_pManager;
        long m_GID; // Group ID de los widgets construidos
        GNC::GCS::TriggerButton m_ButtonMask;

        // Default (common) properties
        float m_FontSizeProp;
        float m_MaxTextWidthProp;
        float m_LineWidthProp;
        float m_DraggingLineWidthProp;

public:
        //existen builders que no necesitan activarse, como la rejilla
        IWidgetBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid = 0, bool activar = true);
        virtual ~IWidgetBuilder();

        virtual GNC::GCS::TriggerButton GetButtonMask() const
        {
                return m_ButtonMask;
        }

        /* Obtiene el id de grupo de los widget construidos (tipo) */
        virtual long GetGID();

        virtual void Activar();

        virtual void Desactivar();

        virtual void Render(Contexto3D* c) = 0;

        virtual void LanzarEventoCreacion(IWidget* w);

        virtual void LanzarEventoModificacion(IWidget* w);

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&) = 0;

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&) = 0;

        virtual GNC::GCS::Widgets::TipoCursor GetCursor();

};

// Clase que representa un widget
class EXTAPI IWidget
{
public:
        typedef std::list<GNC::GCS::Widgets::IObservadorWidget*> ListaObservadores;
        typedef bool (IWidget::*Muestreador) ( const GNC::GCS::Vector& punto ) const;

        //region "Construccion y destruccion"
protected:
        GNC::GCS::IWidgetsManager*           m_pManager;
        GNC::GCS::IWidgetsRenderer*          m_pRenderer; // Renderer asociado a widgets toplevel

        std::string                m_Nombre;
        long                       m_VID; // Id de la sub-vista
        long                       m_UID; // ID de widget
        long                       m_GID; // ID de grupo
        long                       m_TID; // ID de tipo del widget
        bool                       m_Seleccionado;
        bool                       m_Iluminado;
        bool                       m_Modificado;
        bool                       m_ReservaRecursos;
        bool                       m_NotificarCambios;
        bool                       m_IgnorarEventos;

        GNC::GCS::Vector           m_PosCursor;

protected:
        bool                       m_Oculto;
        bool                       m_TopLevel; // Indica si el widget es top-level. Los widgets top level procesan los eventos los primeros y se renderizan siempre los ultimos.

public:
        IWidget(GNC::GCS::IWidgetsManager* pManager, long vid, const char* Nombre = "", long gid = 0, long tid = 0, long uid = 0);

        virtual ~IWidget();
        //endregion

        //region "Interfaz especifica"

        virtual void IgnorarEventos(bool ignorar)
        {
                if (ignorar == true) {
                        Iluminar(false);
                        Seleccionar(false);
                }
                if (ignorar != m_IgnorarEventos) {
                        Modificar(true);
                        m_IgnorarEventos = ignorar;
                }
        }

        virtual bool IgnoraEventos()
        {
                return m_IgnorarEventos;
        }

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&) = 0;

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&) = 0;

        virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral) = 0;

        virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices) = 0;

        inline void NotificarCambios(bool notificar)
        {
                m_NotificarCambios = notificar;
        }

        inline bool RequiereNotificarCambios() const
        {
                return m_NotificarCambios;
        }

        inline IWidgetsRenderer* GetTopLevelRenderer() const
        {
                return m_pRenderer;
        }

        virtual void TickAnimation(double /*freq*/) {}

        //region Interfaz de dibujado de overlays
        virtual bool EsMarcador() const;
        virtual bool DelimitaArea() const;

        virtual bool DelimitaTendencia() const;

        virtual bool ContieneFast(const GNC::GCS::Vector& ) const;

        virtual void GetBoundingBox(GNC::GCS::Vector& , GNC::GCS::Vector& ) const;

        virtual void GetPosicion(GNC::GCS::Vector& ) const;

        virtual void GetPuntoAnclaje(GNC::GCS::Vector& , const GNC::GCS::Vector& ) const;

        //region Propiedades exclusivas de widgets de area
        virtual float GetAreaMetrica() const;

        virtual float GetPerimetro() const;

        virtual void GetVertices(std::list<GNC::GCS::Vector>& ) const;
        //endregion

        //region Propiedades exclusivas de widgets de tendencia

        virtual void GetDistancia(GNC::GCS::Vector& ) const;

        virtual void GetRecorrido(std::list<GNC::GCS::Vector>& ) const;
        //endregion

        //endregion Interfaz de dibujado de overlays

        virtual void Render(Contexto3D* c) = 0;

        virtual void OffscreenRender(Contexto3D* /*c*/) {}

        virtual void Ocultar(bool ocultar)
        {
                if (m_Oculto != ocultar) {
                        m_Oculto = ocultar;
                        m_Modificado = true;
                }
        }

        virtual void Modificar(bool modificar)
        {
                m_Modificado = modificar;
        }

        virtual void Seleccionar(bool seleccionar)
        {
                if (m_Seleccionado != seleccionar) {
                        m_Seleccionado = seleccionar;
                        m_Modificado = true;
                }
        }

        virtual void InvertirSeleccion()
        {
                Seleccionar(!EstaSeleccionado());
        }

        virtual void Iluminar(bool iluminar)
        {
                if (m_Iluminado != iluminar) {
                        m_Iluminado = iluminar;
                        m_Modificado = true;
                }
        }

        virtual void SeleccionarNodo(GNC::GCS::Nodo& nodo, bool seleccionar)
        {
                if (nodo.m_Seleccionado != seleccionar) {
                        nodo.m_Seleccionado = seleccionar;
                        m_Modificado = true;
                }
        }

        virtual void IluminarNodo(GNC::GCS::Nodo& nodo, bool iluminar)
        {
                if (nodo.m_Iluminado != iluminar) {
                        nodo.m_Iluminado = iluminar;
                        m_Modificado = true;
                }
        }

        virtual bool ReservaRecursos() const
        {
                return m_ReservaRecursos;
        }

        virtual void LiberarRecursos(GNC::GCS::IWidgetsRenderer* pKey);

        /* Indica si el widget permite ser borrado automaticamente por el manager. Esto ocurre cuando el widget esta seleccionado y el manager recibe la tecla SUPR */
        virtual bool SoportaAutoBorrado() const
        {
                return false;
        }

        //endregion

        //region "Interfaz generica"
        /* Obtiene el id de tipo del widget */
        inline long GetVID() const
        {
                return m_VID;
        }

        /* Obtiene el id de tipo del widget */
        inline long GetTID() const
        {
                return m_TID;
        }

        /* Obtiene el id unico del widget */
        inline long GetUID() const
        {
                return m_UID;
        }

        /* Obtiene el id de grupo del widget (tipo) */
        inline  long GetGID() const
        {
                return m_GID;
        }

        virtual bool EstaOculto() const
        {
                return m_Oculto;
        }

        virtual bool EstaModificado() const
        {
                return m_Modificado;
        }

        virtual bool EstaSeleccionado() const
        {
                return m_Seleccionado;
        }

        virtual bool EstaIluminado() const
        {
                return m_Iluminado;
        }

        virtual bool EsTopLevel() const
        {
                return m_TopLevel;
        }

        // Establece si el widget es top-level. Los widgets top level procesan los eventos los primeros y se renderizan siempre los ultimos.
        inline void SetTopLevel(bool setTopLevel = true)
        {
                if (m_TopLevel != setTopLevel) {
                        m_TopLevel = setTopLevel;
                        m_Modificado = true;
                }
        }

        void LanzarEventoModificacion();

        void LanzarEventoDestruccion();

        inline friend std::ostream& operator<<(std::ostream& out, const IWidget& w)
        {
                out << "[ VID = " << w.m_VID << ", nombre = " << w.m_Nombre.c_str() << ", GID = " << w.m_GID << ", TID = " << w.m_TID << ", UID = " << w.m_UID << " ]";
                return out;
        }

        inline friend std::ostream& operator<<(std::ostream& out, const IWidget* w)
        {
                if (w == NULL) {
                        out << "[ NULL ]";
                } else {
                        out << *w;
                }
                return out;
        }

        //endregion
        //region menu contextual
        virtual bool GetMenuContextual(wxMenu*, wxWindow* );
        virtual void MostrarPropiedades(wxWindow* pParent);

        typedef std::map< std::string , std::string > TMapaPropiedades;
        typedef std::list< TMapaPropiedades > TListaMapasPropiedades;
        virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);

        //endregion

protected:
        std::string PuntoToStringMundo(const GNC::GCS::Vector& valor);
        ListaObservadores   m_Observadores;   // Lista de observadores

        // Default (common) properties
        float m_LineWidthProp;
        float m_FontSizeProp;
        float m_MaxTextWidthProp;
        float m_DraggingLineWidthProp;
};
}
}
}
