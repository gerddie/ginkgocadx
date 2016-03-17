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

#include <wx/wx.h>
#include <api/iwidgetsmanager.h>
#include "gvistasimple.h"
#include "gvistacompleja.h"
#include "../vistas/vista2d.h"

#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <main/controllers/configurationcontroller.h>

#include "../eventos/visualizatorevents.h"
#include <api/toolsystem/itoolsregistry.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/internationalization/internationalization.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <main/entorno.h>
#include <resources/visualizatorresourcesmanager.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkVISUManagement/vtkLookupTableManager.h>
#include <vtk/widgets/widgetsactor.h>
#include <vtkCamera.h>
#include <vtkPointData.h>
#include <vtkImageBlend.h>
#include <vtkImageMathematics.h>
#include <api/westilo.h>

#include <widgets/openglhelper.h>


class GVtkObserver: public vtkCommand
{
public:
        static GVtkObserver* New()
        {
                return new GVtkObserver;
        }

        void Execute(vtkObject * caller, unsigned long event, void * /*callData*/)
        {
                if (IVista == NULL) {
                        return;
                }
                vtkGinkgoImageViewer* view = vtkGinkgoImageViewer::SafeDownCast (caller);
                if( !view ) {
                        return;
                }

                if ( event==vtkGinkgoImageViewer::ViewImageZoomChangeEvent && !this->ZoomLock) {
                        double zoom = view->GetZoom();
                        LockZoom();
                        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new AtencionPrimaria::Events::EventoZoom(IVista,zoom));
                        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoRender(IVista));
                        UnLockZoom();
                }

                if ( event==vtkGinkgoImageViewer::ViewImagePositionChangeEvent && !this->PanLock) {
                        LockPan();
                        if (view->GetRenderer()) {
                                double pos[3], focal[3];
                                view->GetRelativePositionOfCamera(focal, pos);
                                GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new AtencionPrimaria::Events::ChangePositionEvent(IVista, pos, focal));
                                GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoRender(IVista));
                        }
                        UnLockPan();
                }
        }

        void Setup(GNC::GCS::IVista* pIVista)
        {
                IVista = pIVista;
        }

        void LockZoom ()
        {
                this->ZoomLock = true;
        }
        void UnLockZoom ()
        {
                this->ZoomLock = false;
        }

        void LockPan ()
        {
                this->PanLock = true;
        }
        void UnLockPan ()
        {
                this->PanLock = false;
        }

protected:
        GVtkObserver() : ZoomLock(false),  PanLock(false), IVista(NULL)
        {
        }

        ~GVtkObserver()
        {
                IVista = NULL;
        }

        bool ZoomLock;
        bool PanLock;
        GNC::GCS::IVista* IVista;
};


GVistaSimple::GVistaSimple(GNKVisualizator::Vista2D* pIVista, wxWindow* parent) : VistaSimpleBase(parent), EstudioReferido(GNC::GCS::IStudyContext::NewRef(pIVista->GetEstudio()))
{
        IVista = pIVista;
        m_tDimensions = 0;

        this->Connect( wxEVT_SIZE, wxSizeEventHandler( GVistaSimple::OnSize ) );
        //this->Connect( wxEVT_PAINT, wxPaintEventHandler( GVistaSimple::OnPaint ) );

        ViewImage2D->SetBackgroundColor(0.0f,0.0f,0.0f);
        ViewInteractor2D->m_pImageViewer = ViewImage2D;

        EstudioReferido->SetViewer(ViewImage2D);
        ViewImage2D->SetInputConnection(EstudioReferido->renderConnection);

        GNC::GCS::Events::EventoModificacionImagen evtModificacionImagen(IVista);
        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, evtModificacionImagen);

        m_pObserverVTK = GVtkObserver::New();
        m_pObserverVTK->Setup(IVista);

        ViewImage2D->AddObserver ( vtkGinkgoImageViewer::ViewImageZoomChangeEvent, m_pObserverVTK );
        ViewImage2D->AddObserver ( vtkGinkgoImageViewer::ViewImagePositionChangeEvent, m_pObserverVTK );

        AtencionPrimaria::Events::EventoZoom evtZoom(IVista);
        AtencionPrimaria::Events::ChangePositionEvent evtChange(IVista);
        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, evtZoom);
        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, evtChange);

        this->Connect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GVistaSimple::OnFocus),NULL,this);
        ViewInteractor2D->Connect(wxEVT_KEY_DOWN,wxKeyEventHandler(GVistaSimple::OnChar),NULL,this);
        this->Layout();
}

GVistaSimple::~GVistaSimple()
{
        if(m_pObserverVTK != NULL) {
                m_pObserverVTK->Delete();
                m_pObserverVTK = NULL;
        }
        this->Disconnect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GVistaSimple::OnFocus),NULL,this);
        ViewInteractor2D->Disconnect(wxEVT_KEY_DOWN,wxKeyEventHandler(GVistaSimple::OnChar),NULL,this);

        this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( GVistaSimple::OnSize ) );

        //abortamos comandos antes de cargarnos nada!
        //m_pEntorno->GetCommandController()->AbortarComandosDeOwner(IVista);

        ViewImage2D->Detach();
        //necesario para que no haga un doble delete
        EstudioReferido->SetViewer(NULL);
        //ViewInteractor2D->Reparent(NULL);
        ViewInteractor2D->Delete();

}

void GVistaSimple::OnSize( wxSizeEvent& /*event*/ )
{
        this->Layout();
}

void GVistaSimple::OnPaint( wxPaintEvent& /*event*/ )
{
        wxPaintDC dc(this);
        dc.SetBrush(wxBrush(wxColor(0,0,0)));
        dc.DrawRectangle(this->GetClientRect());
}

void GVistaSimple::setEstudioReferido(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& parent)
{
        EstudioReferido = GNC::GCS::IStudyContext::NewRef(parent);
        ViewImage2D->RemoveInputs();
        EstudioReferido->SetViewer(ViewImage2D);
        ViewImage2D->SetInputConnection(EstudioReferido->renderConnection);
}

void GVistaSimple::UnRefIVista()
{
        IVista = NULL;
        m_pObserverVTK->Setup(NULL);
}

void GVistaSimple::RefrescarOverlays(const GNKVisualizator::TEstadoOverlay& estadoOverlays)
{
        if(!m_pListaOverlays.IsValid()) {
                ViewImage2D->SetOverlay(NULL);
        } else {
                vtkImageMathematics* pBlend = NULL;

                if(m_pListaOverlays->size()>0) {
                        int i=0;
                        vtkImageMathematics* pUltimoBlend = NULL;
                        for(GNKVisualizator::TListaOverlays::iterator itOverlays = m_pListaOverlays->begin(); itOverlays!=m_pListaOverlays->end(); ++itOverlays) {
                                bool oculto = false;
                                if(estadoOverlays.find((*itOverlays).indice) != estadoOverlays.end()) {
                                        oculto = (*estadoOverlays.find((*itOverlays).indice)).second;
                                }

                                if(!oculto) {
                                        if(i==0) {
                                                pBlend = vtkImageMathematics::New();
                                                pBlend->SetOperationToMax();
                                                pBlend->SetInput1Data((*itOverlays).img);
                                                pBlend->SetInput2Data((*itOverlays).img);
                                                pUltimoBlend = pBlend;
                                        } else {
                                                vtkImageMathematics* otroBlend = vtkImageMathematics::New();
                                                otroBlend->SetOperationToMax();
                                                pUltimoBlend->SetInput2Data(otroBlend->GetOutput());
                                                otroBlend->SetInput1Data((*itOverlays).img);
                                                otroBlend->SetInput2Data((*itOverlays).img);
                                                pUltimoBlend = otroBlend;
                                                otroBlend->Delete();
                                        }
                                        i++;
                                }
                        }
                        if(pBlend != NULL) {
                                pBlend->Update();
                        }
                }
                if(pBlend != NULL) {
                        ViewImage2D->SetOverlay(pBlend->GetOutput());
                        pBlend->Delete();
                } else {
                        ViewImage2D->SetOverlay(NULL);
                }
        }
}

void GVistaSimple::SetOverlays(GNC::GCS::Ptr<GNKVisualizator::TListaOverlays>& pListaOverlays, const GNKVisualizator::TEstadoOverlay& estadoOverlays)
{
        m_pListaOverlays = pListaOverlays;
        RefrescarOverlays(estadoOverlays);
}

GNC::GCS::Ptr<GNC::GCS::IReferredStudyContext>& GVistaSimple::GetEstudio()
{
        return EstudioReferido;
}

void GVistaSimple::OnFocus(wxChildFocusEvent& /*event*/)
{
        if (IVista != NULL) {
                IVista->OnFocus();
        }
}

void GVistaSimple::ResetTimeInstant()
{
        ViewImage2D->SetTindex(0);
}

void GVistaSimple::OnChar( wxKeyEvent& event )
{
        event.ResumePropagation(3);
        event.Skip(true);
}

void GVistaSimple::OnMouseWheel( wxMouseEvent& event )
{
        event.ResumePropagation(2);
        event.Skip(true);
}

void GVistaSimple::OnTimeInstantChanged(wxScrollEvent& event)
{
        int pos = m_pSliderTimeInstant->GetValue();
        if (pos != ViewImage2D->GetTindex()) {
                GoToTSlice(pos,false);
        }
        event.Skip(true);
}

void GVistaSimple::GoToTSlice(int pos, bool relativa)
{
        if (m_tDimensions <= 1) {
                return;
        }
        int posicion;
        if(relativa) {
                posicion = ViewImage2D->GetTindex() + pos;
        } else {
                posicion = pos;
        }

        posicion = std::max<int>(0, std::min<int>(m_tDimensions - 1, posicion));

        if (ViewImage2D->GetTindex() != posicion) {
                m_pSliderTimeInstant->SetValue(posicion);

                ViewImage2D->SetTindex(posicion);
                ViewInteractor2D->Refresh(false);

                //refrescar anotaciones y compañia
                if (IVista != NULL) {
                        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
                }
        }
}

void GVistaSimple::GoToSlice(int pos, bool relativa, bool /*propagar*/)
{
        int posicion;
        if(relativa) {
                posicion = EstudioReferido->ActiveFileIndex + pos;
        } else {
                posicion = pos;
        }

        posicion = std::max<int>(0, std::min<int>(EstudioReferido->GetSliceNumber(), posicion));

        //se le indica al manager el vid
        this->ViewInteractor2D->SetVID(posicion, false);
        EstudioReferido->SetActiveIndex(posicion);

        ActualizarTSlider();
}

void GVistaSimple::ActualizarTSlider()
{
        ViewImage2D->SetTindex(0);

        m_tDimensions = EstudioReferido->GetTSizeActive();
        if (m_tDimensions == 1) {
                m_pSliderTimeInstant->SetRange(1, 2);
                if (m_pSliderTimeInstant->IsShown()) {
                        m_pSliderTimeInstant->Show(false);
                        this->Layout();
                        ViewInteractor2D->Refresh(true);
                }
        } else {
                m_pSliderTimeInstant->SetRange(0, m_tDimensions - 1);
                if (!m_pSliderTimeInstant->IsShown()) {
                        m_pSliderTimeInstant->Show(true);
                        this->Layout();
                        ViewInteractor2D->Refresh(true);
                }
        }
        m_pSliderTimeInstant->SetValue(0);
}

//endregion

inline bool my_isnan(double x)
{
        return x != x;
}

inline bool my_isnan(float x)
{
        return x != x;
}

//region "Interfaz de impresion de overlays de widgets"
std::string GVistaSimple::ImprimirOverlay(GNC::GCS::Widgets::IWidget* w, vtkGinkgoImageViewer* /*pViewer*/)
{
        if (!w->EsMarcador()) {
                return "";
        }

        GNC::GCS::TexturaCairo tc;

        double posMundo[4] = {0.0, 0.0, 0.0, 1.0};
        GNC::GCS::Vector            posimagen;
        GNC::GCS::Vector            puntoAnclaje;
        GNC::GCS::Vector            bBox[2];

        GNC::GCS::Vector            tamTexto;

        std::ostringstream          os;

        os.setf(ios::floatfield, ios::fixed );
        os.precision(2);
        os.fill('0');

        double spacing[3] = {0.0, 0.0, 0.0};

        bool hasSpacing = EstudioReferido->GetSpacingActive(spacing[0], spacing[1], spacing[2]);

        w->GetPosicion(posimagen);
        {
                double arrIm[3] = {posimagen.x, posimagen.y, 0.0};
                ViewImage2D->CoordenadasImagenACoordenadasMundo(arrIm, posMundo);
        }

        w->GetBoundingBox(bBox[0], bBox[1]);
        //el bounding box hay que cambiarlo a coordenadas pixel
        {
                ViewImage2D->CoordenadasImagenACoordenadasPixel(bBox[0]);
                ViewImage2D->CoordenadasImagenACoordenadasPixel(bBox[1]);
        }

        //para las rgb solo se imprime rgb para los puntos y poligonos
        if(EstudioReferido->Viewer->GetNumberOfComponents() == 3) {
                if (w->DelimitaArea()) {
                        float perimetro = w->GetPerimetro();
                        float area = w->GetAreaMetrica();

                        os << _Std("Area      : ");

                        if (my_isnan(area)) {
                                os << _Std("undetermined") << std::endl;
                        } else {

                                if (hasSpacing) {
                                        if (area > (double)1000000.0f) {
                                                os << area / (double)1000000.0f << " m²." << std::endl;
                                        } else if (area > (double)100.0f) {
                                                os << area / (double)100.0f << " cm²." << std::endl;
                                        } else if (area < 1.0f) {
                                                os << area * (double)1000000.0f << " µm²." << std::endl;
                                        } else {
                                                os << area << " mm²." << std::endl;
                                        }
                                } else {
                                        os << area << "px²." << std::endl;
                                }
                        }
                        os << _Std("Perimeter : ");
                        if (hasSpacing) {
                                if (perimetro > (double)1000.0f) {
                                        os << perimetro / (double)1000.0f << " m.";
                                } else if (perimetro > (double)10.0f) {
                                        os << perimetro / (double)10.0f << " cm.";
                                } else if (perimetro < 1.0f) {
                                        os << perimetro * (double)1000.0f << " µm." << std::endl;
                                } else {
                                        os << perimetro << " mm.";
                                }
                        } else {
                                os << perimetro << " px.";
                        }
                } else if(!w->DelimitaTendencia()) {
                        //se imprime r = tal g = tal y b=tal

                        vtkDataArray* scalars = EstudioReferido->Viewer->GetScalars();
                        if (scalars != NULL) {
                                double ip[3] = {posimagen.x, posimagen.y, 0.0};
                                int pp[3];
                                int dims[3];

                                EstudioReferido->GetDimensionsActiveImage(dims[0], dims[1], dims[2]);

                                ViewImage2D->CoordenadasImagenACoordenadasPixel(ip, pp);

                                if (pp[0] >= 0 && pp[0] < dims[0] && pp[1] >= 0 && pp[1] < dims[1]) {
                                        unsigned int off = (dims[0] * dims[1] * ViewImage2D->GetTindex()) + (pp[1] * dims[0] + pp[0]);
                                        double tuple[3];
                                        scalars->GetTuple(off, tuple);
                                        os << _Std("Position   : ") << posMundo[0] << ", " << posMundo[1] << ", " << posMundo[2] << std::endl;
                                        os << _Std("RGB (") << (int) tuple[0] << ", " << (int) tuple[1] << "," << (int) tuple[2] << ")";
                                } else {
                                        os.clear();
                                }
                        } else {
                                os.clear();
                        }
                } else {
                        os.clear();
                }
        } else if (EstudioReferido->Viewer->GetNumberOfComponents() == 1) {
                //para los de una componente se imprime todo teniendo en cuenta el valor del pixel en esa posicion
                if (w->DelimitaTendencia()) {
                        std::list<GNC::GCS::Vector> recorrido;
                        w->GetRecorrido(recorrido);

                        if (!recorrido.empty()) {
                                std::list<GNC::GCS::Vector>::iterator it;

                                float val0 = 0.0f;
                                float val1 = 0.0f;
                                float diff = 0.0f;
                                float prop = 0.0f;

                                posimagen = recorrido.front();

                                it = recorrido.begin();
                                ++it;
                                {
                                        double arrIm[3] = {posimagen.x, posimagen.y, 0.0};
                                        ViewImage2D->CoordenadasImagenACoordenadasMundo(arrIm, posMundo);
                                        os << _Std("Position   : ") << posMundo[0] << ", " << posMundo[1] << ", " << posMundo[2] << std::endl;
                                }

                                for (; it != recorrido.end(); ++it) {
                                        double arrIm[3] = {(*it).x, (*it).y, 0.0};
                                        double mundoTmp[4];
                                        ViewImage2D->CoordenadasImagenACoordenadasMundo(arrIm, mundoTmp);
                                        os << " -> " << mundoTmp[0] << ", " << mundoTmp[1] << ", " << mundoTmp[2];
                                }
                                os << std::endl;

                                it = recorrido.begin();
                                ++it;
                                // Obtener valoracion p0
                                val0 = ObtenerValoracion(posimagen);
                                os << _Std("Rating   : ") << val0;
                                for (; it != recorrido.end(); ++it) {
                                        val1 = ObtenerValoracion(*it);
                                        diff = val1 - val0;
                                        double* rango = ViewImage2D->GetInput()->GetScalarRange();
                                        prop = 100.0f * (diff/(rango[1]-rango[0]));
                                        os << " -> " << val1 << _Std("[DNorm =") << prop << "% ]";
                                }
                        }
                } else if (w->DelimitaArea()) {
                        //float valor = 0.0f;
                        float media = 0.0f;
                        float varianza = 0.0f;
                        float perimetro = w->GetPerimetro();
                        float area = 0.0f;

                        //valor = ObtenerValoracion(posmundo);
                        ObtenerEstadisticos(w, &GNC::GCS::Widgets::IWidget::ContieneFast, bBox, NULL, &media, &varianza);
                        // Se obtuvo el area en pixels absolutos. La despreciamos y recalculamos el area metrica (Ticket #245)
                        area = w->GetAreaMetrica();
#if 0
                        os << "Coor. Centroide: " << posmundo.x << ", " << posmundo.y << std::endl;
#endif
                        os << _Std("Centroid Pos:") << posMundo[0] << ", " << posMundo[1] << ", " << posMundo[2] << std::endl;
                        os << _Std("Area:");
                        if (my_isnan(area)) {
                                os << "indeterminada" << std::endl;
                        } else {
                                if (hasSpacing) {
                                        if (area > (double)1000000.0f) {
                                                os << area / (double)1000000.0f << " m²." << std::endl;
                                        } else if (area > (double)100.0f) {
                                                os << area / (double)100.0f << " cm²." << std::endl;
                                        } else if (area < 1.0f) {
                                                os << area * (double)1000000.0f << " µm²." << std::endl;
                                        } else {
                                                os << area << " mm²." << std::endl;

                                        }
                                } else {
                                        os << area << " px²." << std::endl;
                                }
                        }
                        os << _Std("Perimeter:");
                        if (hasSpacing) {
                                if (perimetro > (double)1000.0f) {
                                        os << perimetro / (double)1000.0f << " m." << std::endl;
                                } else if (perimetro > (double)10.0f) {
                                        os << perimetro / (double)10.0f << " cm." << std::endl;
                                } else if (perimetro < 1.0f) {
                                        os << perimetro * (double)1000.0f << " µm." << std::endl;
                                } else {
                                        os << perimetro << " mm." << std::endl;
                                }
                        } else {
                                os << perimetro << " px." << std::endl;
                        }
                        os << _Std("Rating: mean =") << media << _Std(", dev =") << varianza;
                } else {
                        float valor = 0.0f;
                        valor = ObtenerValoracion(posimagen);
                        os << _Std("Position   : ") << posMundo[0] << ", " << posMundo[1] << ", " << posMundo[2] << std::endl;
                        os << _Std("Rating   : ") << valor;
                }
        } else {
                os.clear();
        }

        return os.str();
}

//endregion


float GVistaSimple::ObtenerValoracion(const GNC::GCS::Vector& vImagen)
{
        int pp[3];
        double ip[3] = {vImagen.x, vImagen.y, 0.0};
        ViewImage2D->CoordenadasImagenACoordenadasPixel(ip, pp);

        int dims[3];
        ViewImage2D->GetDimensions(dims);

        if (pp[0]>=0 && pp[0] < dims[0] && pp[1]>=0 && pp[1] < dims[1]) {
                vtkSmartPointer<vtkImageData> pImg = ViewImage2D->GetDataObject();
                if (pImg == NULL)
                        return 0.0;

                void *scalar = pImg->GetScalarPointer((int)pp[0],(int)pp[1],ViewImage2D->GetTindex());
                double value = 0.0f;
                if( scalar ) {
                        switch (ViewImage2D->GetInput()->GetScalarType()) {
                        case VTK_DOUBLE:
                                value = (((double*)scalar)[0]);
                                break;
                        case VTK_FLOAT:
                                value = (double) (((float*)scalar)[0]);
                                break;
                        case VTK_UNSIGNED_LONG:
                                value = (double) (((unsigned long*)scalar)[0]);
                                break;
                        case VTK_LONG:
                                value = (double) (((long*)scalar)[0]);
                                break;
                        case VTK_UNSIGNED_INT:
                                value = (double) (((unsigned int*)scalar)[0]);
                                break;
                        case VTK_INT:
                                value = (double) (((int*)scalar)[0]);
                                break;
                        case VTK_UNSIGNED_SHORT:
                                value = (double) (((unsigned short*)scalar)[0]);
                                break;
                        case VTK_SHORT:
                                value = (double) (((short*)scalar)[0]);
                                break;
                        case VTK_UNSIGNED_CHAR:
                                value = (double) (((unsigned char*)scalar)[0]);
                                break;
                        case VTK_CHAR:
                                value = (double) (((char*)scalar)[0]);
                                break;
                        }
                }
                return value;
        }
        return 0.0;
}

void GVistaSimple::ObtenerEstadisticos(GNC::GCS::Widgets::IWidget* w, GNC::GCS::Widgets::IWidget::Muestreador ptr, GNC::GCS::Vector boundingBox[2], double* area, float* media, float* varianza)
{
        int x, y;

        int dims[3] = {0, 0, 0};

        vtkSmartPointer<vtkImageData> pDataObject = ViewImage2D->GetDataObject();
        if(pDataObject == NULL) {
                return;
        }

        void* scalars = pDataObject->GetScalarPointer(0,0,ViewImage2D->GetTindex());
        ViewImage2D->GetDimensions(dims);


        double numPixels = 0.0f;
        double sumas = 0.0f;
        double sumasCuadrado = 0.0f;

        double med = 0.0f;

        GNC::GCS::Vector v;
        if (area != NULL) {
                *area = 0;
        }

        const int st = pDataObject->GetScalarType();
        const unsigned long nc = pDataObject->GetNumberOfScalarComponents();

#ifdef _GINKGO_DEBUG

        //std::cout << "bbox: " << boundingBox[0] << ", " << boundingBox[1] << std::endl;

        //std::cout << "checking " << (double)boundingBox[0].x * spacing[0] + origin[0] << ", " << (double)-1.0f * (((double)boundingBox[0].y * spacing[1]) + origin[1]) << " - " << (double)boundingBox[1].x * spacing[0] + origin[0] << ", " << (double)-1.0f * (((double)boundingBox[1].y * spacing[1]) + origin[1]) << std::endl << std::endl;
#endif

        const unsigned long start = 0;
        const unsigned int stride = nc * dims[0];
        unsigned int size = dims[0] * dims[1] * nc;

        for (y = boundingBox[0].y; y <= boundingBox[1].y; ++y) {
                if (y >= 0 && y < dims[1]) {
                        const unsigned long off = start + (y*stride);
                        v.y = y;
                        for (x = boundingBox[0].x; x <= boundingBox[1].x; ++x) {
                                if (x >= 0 && x < dims[0]) {
                                        const unsigned long ox = x * nc;
                                        if(off+ox > size) {
                                                continue;
                                        }

                                        v.x = x;
                                        if ( (w->*ptr)(v) ) {

                                                double pixel = 0.0f;
                                                switch(st) {
                                                case VTK_DOUBLE:
                                                        pixel = ((double*)scalars)[off + ox];
                                                        break;
                                                case VTK_FLOAT:
                                                        pixel = (double) ( ((float*)scalars)[off + ox] );
                                                        break;
                                                case VTK_UNSIGNED_LONG:
                                                        pixel = (double) ( ((unsigned long*)scalars)[off + ox] );
                                                        break;
                                                case VTK_LONG:
                                                        pixel = (double) ( ((long*)scalars)[off + ox] );
                                                        break;
                                                case VTK_UNSIGNED_INT:
                                                        pixel = (double) ( ((unsigned int*)scalars)[off + ox] );
                                                        break;
                                                case VTK_INT:
                                                        pixel = (double) ( ((int*)scalars)[off + ox] );
                                                        break;
                                                case VTK_UNSIGNED_SHORT:
                                                        pixel = (double) ( ((unsigned short*)scalars)[off + ox] );
                                                        break;
                                                case VTK_SHORT:
                                                        pixel = (double) ( ((short*)scalars)[off + ox] );
                                                        break;
                                                case VTK_UNSIGNED_CHAR:
                                                        pixel = (double) ( ((unsigned char*)scalars)[off + ox] );
                                                        break;
                                                case VTK_CHAR:
                                                        pixel = (double) ( ((char*)scalars)[off + ox] );
                                                        break;
                                                }
                                                //std::cout << "Pixel[" << (int) numPixels << "] = " << pixel << std::endl;
                                                sumas += pixel;
                                                sumasCuadrado += (pixel * pixel);
                                                numPixels += 1.0f;
                                        }
                                }
                        }
                }
        }

        double spacing[3];
        ViewImage2D->GetSpacing(spacing);
        if (area != NULL) {
                *area = numPixels*spacing[0]*spacing[1];
        }

        if (numPixels > 0.0f) {
                med = sumas / numPixels;
        }

        if (media != NULL) {
                if (numPixels > 0.0f) {
                        *media = (float)med;
                } else {
                        *media = 0.0f;
                }
        }
        //std::cout << "NumPixels = " << numPixels << std::endl;
        //std::cout << "sumas = " << sumas << std::endl;
        //std::cout << "sumascuadrado = " << sumasCuadrado << std::endl;
        if (varianza != NULL) {
                if (numPixels > 0.0f) {
                        double val = (sumasCuadrado / numPixels)  - (med * med);
                        if (val > std::numeric_limits<double>::epsilon()) {
                                *varianza = (float) std::sqrt( val );
                        } else {
                                *varianza = 0.0f;
                        }
                } else {
                        *varianza = 0.0f;
                }
        }
}

void GVistaSimple::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
        if ( IVista == NULL) {
                return;
        }
        switch (evt->GetCodigoEvento()) {
        case ginkgoEVT_GNKVisualizator_EventoZoom: {
                AtencionPrimaria::Events::EventoZoom* pEvt = dynamic_cast<AtencionPrimaria::Events::EventoZoom*>(evt);
                if(pEvt!=NULL) {
                        m_pObserverVTK->LockZoom();
                        ViewImage2D->SetZoom(pEvt->GetZoom());
                        m_pObserverVTK->UnLockZoom();
                }
        }
        break;
        case ginkgoEVT_GNKVisualizator_ChangePositionEvent: {
                AtencionPrimaria::Events::ChangePositionEvent* pEvt = dynamic_cast<AtencionPrimaria::Events::ChangePositionEvent*>(evt);
                if(pEvt!=NULL) {
                        m_pObserverVTK->LockPan();
                        ViewImage2D->SetRelativePositionOfCamera(pEvt->GetFocal(), pEvt->GetPosition());
                        m_pObserverVTK->UnLockPan();
                }
        }
        break;
        case ginkgoEVT_Core_ModificacionImagen: {
                GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
                if (pEvt != NULL  && ( pEvt->GetSliceAfectado() == -1 || pEvt->GetSliceAfectado() == EstudioReferido->ActiveFileIndex) ) {
                        switch (pEvt->GetTipo()) {
                        case GNC::GCS::Events::EventoModificacionImagen::ImagenCargada: {
                                ActualizarTSlider();
                        }
                        break;
                        case GNC::GCS::Events::EventoModificacionImagen::SliceCambiado: {
                                ActualizarTSlider();
                        }
                        break;
                        case GNC::GCS::Events::EventoModificacionImagen::ImagenDescargada: {
                        }
                        break;

                        case GNC::GCS::Events::EventoModificacionImagen::ImagenModificada:
                        case GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
                        case GNC::GCS::Events::EventoModificacionImagen::MapaModificado:
                        case GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas:
                        default:
                                break;
                        }
                }
        }
        default:
                break;
        }
}


