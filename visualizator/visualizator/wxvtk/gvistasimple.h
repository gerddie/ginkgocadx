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
#include <api/controllers/ieventscontroller.h>
#include <api/iwidgets.h>
#include <api/icontexto.h>
#include <api/observers/ieventsobserver.h>
#include "../estudios/overlay.h"

#include <vtkCommand.h>

namespace GNC {
	namespace GCS {
		class IEntorno;
	}
}

namespace GNKVisualizator {
	class Vista2D;
}

class GVtkObserver;
class IconoImagenCtrl;
class vtkAlgorithmOutput;
class vtkImageData;

class GVistaSimple: public VistaSimpleBase, public GNC::GCS::IImpresorOverlay, public GNC::GCS::IEventsObserver {
public:
   /* Constructor */
	GVistaSimple( GNKVisualizator::Vista2D* pIVista, wxWindow* pParent);
   /* Destructor */
	~GVistaSimple();

	virtual void OnSize( wxSizeEvent& event );
	virtual void OnPaint( wxPaintEvent& event );

	virtual void setEstudioReferido(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& parent);

	virtual void UnRefIVista();

	void RefrescarOverlays(const GNKVisualizator::TEstadoOverlay& estadoOverlays);
	void SetOverlays(GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& pListaOverlays,const GNKVisualizator::TEstadoOverlay& estadoOverlays);

	GNC::GCS::Ptr<GNC::GCS::IReferredStudyContext>& GetEstudio();

	vtkImageData* GetImage(){
		return ViewImage2D->GetInput();
	};

   //region "Eventos"
	void OnMenuVerClick(wxCommandEvent& event);
	void OnMenuCineClick(wxCommandEvent& event);

	void OnFocus(wxChildFocusEvent &event);

	void ResetTimeInstant();

	void OnChar( wxKeyEvent& event );

	void OnMouseWheel( wxMouseEvent& event );
	
	void OnTimeInstantChanged(wxScrollEvent& event);

	void GoToTSlice(int posicion, bool relativa =  true);

	void GoToSlice(int posicion, bool relativa =  true, bool propagar = true);

	void ActualizarTSlider();
	
   //endregion

	//region "Interfaz de impresion de overlays de widgets"
	virtual std::string ImprimirOverlay(GNC::GCS::Widgets::IWidget* w, vtkGinkgoImageViewer* pViewer);
	//endregion

	//region "Helpers"
	float ObtenerValoracion(const GNC::GCS::Vector& v);
	void ObtenerEstadisticos(GNC::GCS::Widgets::IWidget* w, GNC::GCS::Widgets::IWidget::Muestreador ptr, GNC::GCS::Vector boundingBox[2], double* area, float* media, float* varianza);
	//endregion

	virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;

protected:
	GNKVisualizator::Vista2D*                  IVista;
	GNC::GCS::Ptr<GNC::GCS::IReferredStudyContext> EstudioReferido;
	GVtkObserver*                              m_pObserverVTK;
	int													 m_tDimensions;

	GNC::GCS::Ptr<GNKVisualizator::TListaOverlays> m_pListaOverlays;
	friend class GVistaCompleja;
};
