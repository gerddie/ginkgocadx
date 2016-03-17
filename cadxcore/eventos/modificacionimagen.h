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

#include <api/ievento.h>
#include "eventosginkgo.h"

class vtkImageData;
class vtkGinkgoImageViewer;

namespace GNC
{
namespace GCS
{
namespace Events
{

class EXTAPI EventoModificacionImagen : public GNC::GCS::Events::IEvent
{
public:

        typedef enum {
                ZM_CenteredBestFit = 0,
                ZM_CenteredOriginalSize = 1,
        } ResetZoomMode;

        typedef enum {
                ImagenCargada,
                ImagenModificada,
                ImagenRecalibrada,
                ImagenDescargada,
                VisualizacionImagenModificada,
                SliceCambiado,
                MapaModificado,
                AnotacionesEstaticasModificadas
        } TipoModificacion;

        // El parametro render indica si en el preprocesado de este evento, se debería apilar o no un render
        EventoModificacionImagen(GNC::GCS::IVista* pVista = NULL, TipoModificacion tipo = ImagenCargada, int sliceAfectado = -1, bool apilarRender = true) : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ModificacionImagen, 0, 100, pVista)
        {
                m_Nombre = "ModificacionImagen";
                m_SliceAfectado = sliceAfectado;
                m_Tipo = tipo;
                m_ApilarRender = apilarRender;
                m_OldSpacing[0] = m_OldSpacing[1] = m_OldSpacing[2] = 0.0f;
                m_NewSpacing[0] = m_NewSpacing[1] = m_NewSpacing[2] = 0.0f;
                m_OldOrigin[0] = m_OldOrigin[1] = m_OldOrigin[2] = 0.0f;
                m_NewOrigin[0] = m_NewOrigin[1] = m_NewOrigin[2] = 0.0f;
                m_newWindow = 0.0f;
                m_newLevel = 0.0f;
                m_resetWL = false;
                m_resetBC = false;
                m_resetCurrentPoint = false;
                m_resetZoom = false;
                m_resetZoomMode = ZM_CenteredBestFit;
                m_autoCalculeWL = false;
                if (tipo == VisualizacionImagenModificada) {
                        int x = 1;
                        x++;
                }
        }

        ~EventoModificacionImagen()
        {
        }

        virtual void pushInfo(std::ostream& out) const
        {
                out << "Tipo = ";
                switch (m_Tipo) {
                case ImagenCargada:
                        out << "ImagenCargada";
                        break;
                case ImagenModificada:
                        out << "ImagenModificada";
                        break;
                case ImagenRecalibrada:
                        out << "ImagenRecalibrada";
                        break;
                case ImagenDescargada:
                        out << "ImagenDescargada";
                        break;
                case VisualizacionImagenModificada:
                        out << "VisualizacionImagenModificada";
                        break;
                case SliceCambiado:
                        out << "SliceCambiado";
                        break;
                case MapaModificado:
                        out << "MapaModificado";
                        break;
                case AnotacionesEstaticasModificadas:
                        out << "AnotacionesEstaticasModificadas";
                        break;
                }
                out << ", SliceAfectado = " << m_SliceAfectado;
                if (m_Tipo == ImagenRecalibrada) {
                        out << ", OldSpacing = [ " << m_OldSpacing[0] << ", " << m_OldSpacing[1] << ", " << m_OldSpacing[2] << " ]";
                        out << ", NewSpacing = [ " << m_NewSpacing[0] << ", " << m_NewSpacing[1] << ", " << m_NewSpacing[2] << " ]";
                        out << ", OldOrigin = [ " << m_OldOrigin[0] << ", " << m_OldOrigin[1] << ", " << m_OldOrigin[2] << " ]";
                        out << ", NewOrigin = [ " << m_NewOrigin[0] << ", " << m_NewOrigin[1] << ", " << m_NewOrigin[2] << " ]";
                }
        }

        inline void SetSlice(int slice)
        {
                m_SliceAfectado = slice;
        }

        inline int GetSliceAfectado() const
        {
                return m_SliceAfectado;
        }

        inline bool GetApilarRender() const
        {
                return m_ApilarRender;
        }

        inline TipoModificacion GetTipo()
        {
                return m_Tipo;
        }

        inline void GetOldSpacing(double spacing[3])
        {
                spacing[0] = m_OldSpacing[0];
                spacing[1] = m_OldSpacing[1];
                spacing[2] = m_OldSpacing[2];
        }

        inline double* GetOldSpacing()
        {
                return m_OldSpacing;
        }

        inline void SetOldSpacing(double spacing[3])
        {
                m_OldSpacing[0] = spacing[0];
                m_OldSpacing[1] = spacing[1];
                m_OldSpacing[2] = spacing[2];
        }

        inline void GetNewSpacing(double spacing[3])
        {
                spacing[0] = m_NewSpacing[0];
                spacing[1] = m_NewSpacing[1];
                spacing[2] = m_NewSpacing[2];
        }

        inline double* GetNewSpacing()
        {
                return m_NewSpacing;
        }

        inline void SetNewSpacing(double spacing[3])
        {
                m_NewSpacing[0] = spacing[0];
                m_NewSpacing[1] = spacing[1];
                m_NewSpacing[2] = spacing[2];
        }

        inline void GetOldOrigin(double origin[3])
        {
                origin[0] = m_OldOrigin[0];
                origin[1] = m_OldOrigin[1];
                origin[2] = m_OldOrigin[2];
        }

        inline double* GetOldOrigin()
        {
                return m_OldOrigin;
        }

        inline void SetOldOrigin(double origin[3])
        {
                m_OldOrigin[0] = origin[0];
                m_OldOrigin[1] = origin[1];
                m_OldOrigin[2] = origin[2];
        }

        inline void GetNewOrigin(double origin[3])
        {
                origin[0] = m_NewOrigin[0];
                origin[1] = m_NewOrigin[1];
                origin[2] = m_NewOrigin[2];
        }

        inline double* GetNewOrigin()
        {
                return m_NewOrigin;
        }

        inline void SetNewOrigin(double origin[3])
        {
                m_NewOrigin[0] = origin[0];
                m_NewOrigin[1] = origin[1];
                m_NewOrigin[2] = origin[2];
        }

        inline void GetProporcionSpacing(double proporcionSpacing[3])
        {
                proporcionSpacing[0] = m_NewSpacing[0]/m_OldSpacing[0];
                proporcionSpacing[1] = m_NewSpacing[1]/m_OldSpacing[1];
                proporcionSpacing[2] = m_NewSpacing[2]/m_OldSpacing[2];
        }

        inline void GetDiferenciaOrigin(double diferenciaOrigin[3])
        {
                diferenciaOrigin[0] = m_NewOrigin[0] - m_OldOrigin[0];
                diferenciaOrigin[1] = m_NewOrigin[1] - m_OldOrigin[1];
                diferenciaOrigin[2] = m_NewOrigin[2] - m_OldOrigin[2];
        }

        inline double GetWindow()
        {
                return m_newWindow;
        }

        inline void SetWindow(double window)
        {
                m_newWindow=window;
        }

        inline double GetLevel()
        {
                return m_newLevel;
        }

        inline void SetLevel(double level)
        {
                m_newLevel = level;
        }

        inline bool ResetWindowLevel()
        {
                return m_resetWL;
        }

        inline bool ResetCurrentPoint()
        {
                return m_resetCurrentPoint;
        }

        inline bool ResetZoom()
        {
                return m_resetZoom;
        }

        inline bool ResetBrightnessContrast()
        {
                return m_resetBC;
        }

        inline ResetZoomMode GetResetZoomMode()
        {
                return m_resetZoomMode;
        }

        inline bool AutoCalculeWindowLevel()
        {
                return m_autoCalculeWL;
        }

        inline void EnableResetWindowLevel()
        {
                m_resetWL = true;
        }

        inline void EnableResetBrightnessContrast()
        {
                m_resetBC = true;
        }

        inline void EnableAutoCalculeWindowLevel()
        {
                m_autoCalculeWL = true;
        }

        inline void EnableResetCurrentPoint()
        {
                m_resetCurrentPoint = true;
        }

        inline void EnableResetZoom(ResetZoomMode zoomMode = ZM_CenteredBestFit)
        {
                m_resetZoom = true;
                m_resetZoomMode = zoomMode;
        }

        inline void ResetearCalibracion()
        {
                m_NewSpacing[0] = m_OldSpacing[0];
                m_NewSpacing[1] = m_OldSpacing[1];
                m_NewSpacing[2] = m_OldSpacing[2];

                m_NewOrigin[0] = m_OldOrigin[0];
                m_NewOrigin[1] = m_OldOrigin[1];
                m_NewOrigin[2] = m_OldOrigin[2];

        }
protected:
        double           m_OldSpacing[3];
        double           m_NewSpacing[3];
        double           m_OldOrigin[3];
        double           m_NewOrigin[3];
        int              m_SliceAfectado;
        bool             m_ApilarRender;
        TipoModificacion m_Tipo;
        double			 m_newWindow;
        double			 m_newLevel;
        bool             m_resetWL;
        bool             m_resetBC;
        bool             m_resetCurrentPoint;
        bool             m_resetZoom;
        bool             m_autoCalculeWL;
        ResetZoomMode    m_resetZoomMode;
};
}
}
}
