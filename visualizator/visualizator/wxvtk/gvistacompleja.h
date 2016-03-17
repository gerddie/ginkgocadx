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


#include "visualizatorviews.h"
#include <visualizator/export/contracts/ioverlayscontract.h>
#include <api/observers/ieventsobserver.h>
#include <export/contracts/iwidgetscontract.h>
#include <export/contracts/islidercontract.h>
#include <export/contracts/irulecontract.h>
#include <export/contracts/iwindowlevelcontract.h>
#include <export/contracts/iwindowlayoutcontract.h>
#include "../estudios/overlay.h"

#include <wx/sizer.h>
#include <wx/dc.h>
#include <wx/bitmap.h>
#include <wx/aui/auibar.h>

#include <vtkCommand.h>

namespace GNKVisualizator
{
class VisualizatorStudy;
class Vista2D;
class SimpleViewExportDelegate;
}
namespace GNC
{
namespace GCS
{
class Layout;
}
}

class GVistaSimple;

class TimerPlay;
class BarraHerramientasBotones;

class vtkScalarsToColors;
class vtkAlgorithmOutput;
class GVistaComplejaDelegate;
class GVistaComplejaOverlaysDelegate;
class GVistaComplejaWindowLayoutDelegate;
class GVistaComplejaPixelsBluringDelegate;
class GVistaComplejaAnnotatorDelegate;

namespace GNKVisualizator
{
class SimpleViewExportDelegate;
namespace GUI
{
class PreviewPanelVisualizator;
class wxDropTargetPreviewPanelVisualizator;
}
}

class GVistaCompleja: public VistaComplejaBase, public GNC::GCS::IEventsObserver
{
        friend class GVistaComplejaDelegate;
        friend class GVistaComplejaOverlaysDelegate;
        friend class GVistaComplejaWindowLayoutDelegate;
        friend class GNKVisualizator::GUI::wxDropTargetPreviewPanelVisualizator;
        friend class GVistaComplejaPixelsBluringDelegate;
        friend class GVistaComplejaAnnotatorDelegate;
public:

        /* Constructor */
        GVistaCompleja( GNKVisualizator::Vista2D* pIVista);
        /* Destructor */
        ~GVistaCompleja();

        virtual void OnEraseBackground( wxEraseEvent& event )
        {
                event.Skip(false);
        }

        void AddVistaSimple();

        void LoadSeries(long seriesPk);

        void IniciarPipeline(const GNC::GCS::Ptr<GNC::GCS::Layout>& layout = NULL);

        void DetenerPipeline();

        virtual void OnCargaFinalizada();

        void SetOverlays(GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& pListaOverlays);

        bool GetManipulacionEnabled()
        {
                return m_pPanelManipulacion->IsEnabled();
        }

        //region "Eventos"
        virtual void OnPaint(wxPaintEvent& evt);

        void OnMenuVerClick(wxCommandEvent& event);
        void OnInvertColorMap(wxCommandEvent &event);

        void ActualizarMaxMinSlider();

        void OnFocus(wxChildFocusEvent &event);

        void OnSize(wxSizeEvent &event);

        void OnZSliderScroll (wxScrollEvent& event);

        void OnChar( wxKeyEvent& event );

        void OnMouseWheel( wxMouseEvent& event );

        void OnSincronizarClick(wxCommandEvent &event);

        void Sincronizar(bool sincronizar);

        void GoToSlice(int posicion, bool relativa =  true, bool propagar = true, bool render= true);
        int GetCurrentSlice() const;

        virtual void PreProcesarEvento(GNC::GCS::Events::IEvent*, std::list<GNC::GCS::Events::IEvent*>& );

        void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;

        typedef std::vector<GNKVisualizator::Vista2D*> TipoListaVista2D;
        void Sincronizar(const TipoListaVista2D& lista);
        void DesSincronizar();
        bool EsSincronizable();

        //endregion


        // Asigna los valores de los apuntadores a la la imagen y el slice actual
        void AsignarConexionesDeFiltrado(vtkImageData** pImagen, int* slice, vtkImageData** pChroma);
        void ActualizarImagen();

        GNC::GCS::IVista* GetView();
protected:
        GNKVisualizator::Vista2D*            IVista;
        BarraHerramientasBotones*            m_pBarraIzquierda;
        BarraHerramientasBotones*            m_pBarraDerecha;

        int                                  m_orientacion;
        TipoListaVista2D                     m_ListaSincronizacion;
        int m_posicion;
        typedef std::vector<GVistaSimple*>   TListaGVistasSimples;
        TListaGVistasSimples                 m_VistasSimples;
        ///widgets
        //TipoFicherosGNKVisualizator*              m_pFicherosAprimaria;


        //si se ha pintado
        bool                                 m_hasBeenRendered;
        GNC::GCS::IWidgetsManager*				 m_pManager;
        GNC::GCS::Ptr<GVistaComplejaDelegate> Delegate;
        GNC::GCS::Ptr<GVistaComplejaOverlaysDelegate> OverlaysDelegate;
        GNC::GCS::Ptr<GVistaComplejaWindowLayoutDelegate> WindowLayoutDelegate;
        GNC::GCS::Ptr<GVistaComplejaPixelsBluringDelegate> PixelsBluringDelegate;
        GNC::GCS::Ptr<GVistaComplejaAnnotatorDelegate> AnnotatorDelegate;
        GNKVisualizator::GUI::PreviewPanelVisualizator*   m_pPanelPreview;

        friend class GNKVisualizator::Vista2D;
};
