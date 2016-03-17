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
#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <vector>
#include <list>
#include "openglhelper.h"


namespace GNC
{
namespace GCS
{

namespace Widgets
{

class EstadoInterno;
class Anotaciones;

//----------------------------------------------------------------------------------------------------
//region Widget anotador
class EXTAPI WStaticRule : public GNC::GCS::Widgets::IWidget, public GNC::GCS::IEventsObserver
{
public:

        WStaticRule(IWidgetsManager* pManager, long vid, const char* nombre, long gid);

        ~WStaticRule();

        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz generica"
public:
        virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

        virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

        virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

        virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

        virtual void Render(GNC::GCS::Contexto3D* c);

        virtual void OffscreenRender(Contexto3D* c);

        virtual void Modificar(bool modificar);

        virtual void Seleccionar(bool seleccionado);

        virtual void Iluminar(bool iluminado);

        virtual void Ocultar(bool oculto);


        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz especifica"
public:

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de eventos ginkgo"
public:

        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Estado interno"
protected:
        GNC::GCS::GLHelper::TColor m_color;

        //endregion

};
//endregion
}
}
}
