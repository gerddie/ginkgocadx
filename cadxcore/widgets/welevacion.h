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
#include <api/controllers/ieventscontroller.h>

#include <vtkSmartPointer.h>

class vtkImageData;
namespace GNC
{
namespace GCS
{
namespace Widgets
{

namespace Elevacion
{

//------------------------------------------------------------------

class RepresentacionElevacion
{
public:

        typedef struct Color {
                float r;
                float g;
                float b;
                float a;

                inline Color() :
                        r(0.0f),
                        g(0.0f),
                        b(0.0f),
                        a(0.0f)
                {
                }

                inline Color(float r, float g, float b, float a) :
                        r(r),
                        g(g),
                        b(b),
                        a(a)
                {
                }

                inline Color(const Color& o) :
                        r(o.r),
                        g(o.g),
                        b(o.b),
                        a(o.a)
                {
                }

                inline void asignar(float r, float g, float b, float a)
                {
                        this->r = r;
                        this->g = g;
                        this->b = b;
                        this->a = a;
                };

        } Color;

        //region "Constructor y destructor"

        RepresentacionElevacion( bool soportaAutoBorrado = false) :
                colorCircunferencia(0.0f, 0.5f, 0.0f, 1.0f),
                colorCircunferenciaIluminada(0.5f, 0.0f, 0.0f, 1.0f),
                m_RadioCircunferencia(15.0f),
                m_SoportaAutoBorrado(soportaAutoBorrado)
        {
        }

        ~RepresentacionElevacion()
        {

        }

        bool SoportaAutoBorrado()
        {
                return m_SoportaAutoBorrado;
        }

        //region "Propiedades radiometricas"

        Color colorCircunferencia;
        Color colorCircunferenciaIluminada;
        float m_RadioCircunferencia;
        bool m_SoportaAutoBorrado;

        //endregion
};

typedef std::list<GNC::GCS::Widgets::Elevacion::RepresentacionElevacion*> ListaRepresentaciones;

//---------------------------------------------------------------------
class IObservadorElevacion
{
public:

        virtual ~IObservadorElevacion() {}

        virtual void OnPuntoInsertado(float x, float y) = 0;

        virtual void OnTerminarInsertar() = 0;
};
}

//---------------------------------------------------------------------

class WElevacionBuilder : public GNC::GCS::Widgets::IWidgetBuilder
{
public:
        WElevacionBuilder(GNC::GCS::IWidgetsManager* pManager, GNC::GCS::Widgets::Elevacion::ListaRepresentaciones* pRepresentaciones, const GNC::GCS::TriggerButton& buttonMask, long gid);

        virtual ~WElevacionBuilder();

        virtual void SetRepresentacionActiva(int idRepresentacion);

        virtual void SetObservador (GNC::GCS::Widgets::Elevacion::IObservadorElevacion* pobservador);

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

        virtual void Render(GNC::GCS::Contexto3D* c);

        virtual GNC::GCS::Widgets::TipoCursor GetCursor();

        //region "Helpers"

        bool         m_Dentro;
        bool         m_ListaCreada;
        unsigned int m_LIndex;
        float        m_RadioCacheado; // Radio el que esta creada la display list

        virtual void BuildDisplayList();
        virtual void DestroyDisplayList();


protected:
        GNC::GCS::Nodo				 m_NodoMoviendose;
        typedef std::list<GNC::GCS::Nodo*> TipoListaNodos;
        typedef std::list<GNC::GCS::Nodo*>::iterator TipoIteradorListaNodos;
        TipoListaNodos   m_Nodos;
        GNC::GCS::Vector m_VerticesSeleccion[2];
        bool             m_MouseDown;
        int              m_RepresentacionActiva;
        Estado           m_Estado;
        GNC::GCS::Widgets::Elevacion::ListaRepresentaciones* m_pRepresentaciones;
        GNC::GCS::Widgets::Elevacion::RepresentacionElevacion* m_pRepActiva;
        GNC::GCS::Widgets::Elevacion::IObservadorElevacion* m_pObservador;

};
}
}
}
