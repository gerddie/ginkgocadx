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
#include <widgets/openglhelper.h>
#include <api/westilo.h>

namespace GNC
{
namespace GCS
{
namespace Widgets
{

class WAngulo;
class WTextBox;

typedef enum ModoAngulo {
        MA_Normal,
        MA_Cobb
} ModoAngulo;

class WAnguloBuilder : public GNC::GCS::Widgets::IWidgetBuilder
{
public:

        typedef GNC::GCS::Vector TVector;
        typedef GNC::GCS::Nodo   TNodo;
        typedef GNC::GCS::Matriz TMatriz;

        WAnguloBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask);

        ~WAnguloBuilder();

        void SetModo(ModoAngulo modo);

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

        virtual void Render(GNC::GCS::Contexto3D* c);

        virtual GNC::GCS::Widgets::TipoCursor GetCursor();

protected:
        TVector          m_Start;
        bool             m_Changed;
        Estado           m_Estado;
        ModoAngulo       m_Modo;
        int              m_NodoActual;

        WAngulo*         m_pAnguloTemp;

};

class WAngulo : public GNC::GCS::Widgets::IWidget, public GNC::GCS::Widgets::IWidgetSerializable
{

public:
        GNC::GCS::Nodo m_Nodos[4];

        //region "Constructor y destructor"

public:

        typedef GNC::GCS::Vector TVector;
        typedef GNC::GCS::Matriz TMatriz;

        WAngulo(IWidgetsManager* pManager, long vid, ModoAngulo modo, const char* nombre = "");

        ~WAngulo();

        //endregion

        //region "Serializado y desserializado"
        WAngulo(IWidgetsManager* pManager, long vid, wxXmlNode* nodo);

        void SetCompleted(bool completed);

        wxXmlNode* Serializar(const std::string& nombreMedico);
        //endregion

        //region "Interfaz especifica"

        virtual bool SoportaAutoBorrado() const
        {
                return true;
        }

        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

        virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

        virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

        virtual void Render(GNC::GCS::Contexto3D* c);

        virtual void OffscreenRender(Contexto3D* c);

        void RenderNormal(GNC::GCS::Contexto3D* c);
        void RenderCobb(GNC::GCS::Contexto3D* c);

        //endregion

        //region "Estado interno"
private:
        bool            m_MouseDown;
        //endregion

        //region "Propiedades geometricas"
private:
        ModoAngulo         m_Modo;
        float              m_Angulo;

        WTextBox*          m_pTextBox;

        friend std::ostream& operator<<(std::ostream& out, const WAngulo& w)
        {
                out << "[ P0=" << w.m_Nodos[0] << ", P1=" << w.m_Nodos[1] <<
                    ", Angulo=" << w.m_Angulo << "=" << w.m_Angulo*180.0f/M_PI <<
                    ", CursorPoint=" << w.m_PosCursor << " ]";
                return out;
        }

public:

        void Recalcular(GNC::GCS::IWidgetsRenderer* renderer, bool building);

public:
        //endregion

        //endregion
        virtual void InsertarPropiedades(TListaMapasPropiedades &listaMapaPropiedades);
};
}
}
}
