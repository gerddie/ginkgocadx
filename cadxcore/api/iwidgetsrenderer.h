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
#include <api/api.h>
#include <api/ievento.h>
#include <api/math/geometry.h>

//----------------------------------------------------------------------------------------------------
// Forward declarations

namespace GNC
{
namespace GCS
{
class IImpresorOverlay;
class IWidgetsManager;

namespace Widgets
{
class IWidget;
class IWidgetBuilder;
}
}
}

class vtkGinkgoImageViewer;
class vtkWorldPointPicker;
class WidgetRepresentation;
class wxWindow;

// End Forward declarations

namespace GNC
{
namespace GCS
{

/* Clase que solicita al modelo de widgets que redibuje sus widgets */
class EXTAPI IWidgetsRenderer : public GNC::GCS::Events::IPublicadorEventos
{
public:
        typedef GNC::GCS::Contexto3D TContexto;

        TContexto* m_c;
        IImpresorOverlay* m_pIOverlay;

        IWidgetsRenderer();

        virtual ~IWidgetsRenderer();

        /* Metodo que indica al manager que es preciso hacer un render() */
        void Modificar(bool modificar)
        {
                m_Modificado = modificar;
        }

        inline bool EstaModificado()
        {
                return m_Modificado;
        }

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de impresión de overlays"

public:

        void SetIOverlay(IImpresorOverlay* pOverlay);


        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de configuracion de vistas/slices"

public:

        /* Propaga la notificación de activación de este renderer hacia el manager */
        void Activar();

        /* Establece el id de sub-vista activo. Los widgets se asocian a un VID. */
        void SetVID(long vid, bool render);

        /* Obtiene el id de sub-vista activo. Los widgets se asocian a un VID. */
        inline long GetVID(void)
        {
                return m_VID;
        }

protected:

        /* Propaga la notificación de activación de este renderer hacia las clases base */
        virtual void OnActivar();

        long m_VID;

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de configuracion del manager"

public:
        void SetManager(GNC::GCS::IWidgetsManager* pManager);

        void SetRepresentation(WidgetRepresentation* pWidget);

        virtual void WSetCurrent() = 0;

protected:
        GNC::GCS::IWidgetsManager*      m_pManager;

        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de transformacion de coordenadas"
public:

        /** Project display to world **/
        virtual void DisplayToWorld(double displayPos[3], double worldPos[4]) = 0;

        /** Unproject work to display **/
        virtual void WorldToDisplay(double worldPos[4], double displayPos[3]) = 0;

        /** Gets camera vectors (camPos, camDir, camUp) **/
        virtual void GetCamVectors(double camPos[4], double camDir[4], double camUp[4]) = 0;

        /** shots a ray from unproyected displayPos with camera direction and intersects with image plane **/
        virtual void RayPick(double displayPos[2], double worldPos[4]) = 0;

        /** shots a ray from campPos with camera direction and intersects with image plane **/
        virtual void RayPick(double worldPos[4]) = 0;

        //endregion

protected:

public:
        virtual void PostRender(bool inmediato = false) = 0;

public:
        vtkGinkgoImageViewer* m_pImageViewer;
        vtkWorldPointPicker*  m_pPicker;
        WidgetRepresentation* m_pRepresentation;
        std::string sid;
        bool                  m_Modificado;
};
}
}
