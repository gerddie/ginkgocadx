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


#include <cmath>
#include <cstring>
#include <ostream>
#include <list>

#include <api/globals.h>
#include <api/controllers/icontroladorlog.h>
#include <api/math/geometry3d.h>
#include <api/icontexto.h>
#include <api/ievento.h>
#include <api/iwidgetsmanager.h>
#include <api/iwidgetsrenderer.h>
#include <api/controllers/ieventscontroller.h>
#include <api/ientorno.h>
#include <api/dicom/imodelodicom.h>
#include <api/westilo.h>

#include <api/controllers/icontroladorpermisos.h>

#include <eventos/modificacionimagen.h>

#include <main/entorno.h>


#include "wecg.h"
#include <widgets/openglhelper.h>

#include <vtk/gl.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <widgets/openglhelper.h>
#include <cairo/cairo.h>

#if defined(_GINKGO_DEBUG)
#define DEBUG_PRECISION
#endif

// Propiedades visuales

#define COLOR_FONDO 0.9f, 0.9f, 0.9f, 1.0f

// Num de pixels pantalla minimo para que aparezca la rejilla
#define GRID_1MM_NEAR_THRESHOLD 0.7
#define GRID_5MM_FAR_THRESHOLD 0.7
#define GRAPH_FAR_THRESHOLD 0.7

//greater that this text wont be shown
#define TEXT_FAR_THRESHOLD 1.2

#define GRID_1MM_LINE_WIDTH 0.25f
#define GRID_1MM_LINE_COLOR 1.0f, 0.0f, 0.0f, 0.3f

#define GRID_5MM_LINE_WIDTH 1.0f
#define GRID_5MM_LINE_COLOR 1.0f, 0.0f, 0.0f, 0.5f
#define GRID_5MM_LINE_WIDTH_FAR 0.15f
#define GRID_5MM_LINE_COLOR_FAR 1.0f, 0.0f, 0.0f, 0.5f

#define GRAPH_LINE_WIDTH 1.5f
#define GRAPH_LINE_COLOR 0.0f, 0.0f, 0.0f, 0.9f

#define GRAPH_LINE_WIDTH_FAR 0.3f
#define GRAPH_LINE_COLOR_FAR 0.0f, 0.0f, 0.0f, 0.9f

#define LEAD_TEXT_COLOR 1.0f, 0.1f, 0.1f, 1.0f

#define REFERENCE_WIDTH 0.4f //0.4 seconds


static GNC::GCS::Vector WinMarginRel[2] = {
        GNC::GCS::Vector(0.0, 0.0), GNC::GCS::Vector(1.0, 1.0)
};

//region "Implementacion de WBuilder"

GNKVisualizator::Widgets::WECGBuilder::WECGBuilder(GNC::GCS::IWidgetsManager* pManager, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid, false), IObservadorWidget(-1, gid)
{
        m_pRejillaActiva = NULL;
        m_pManager->InsertarObservador(this);

        // Comprobamos si existe alguna rejilla para esta vista, y de ser asi la asociamos.
        bool found = false;
        for( GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); !found && it != m_pManager->GetListaWidgets().end(); ++it) {
                GNC::GCS::Widgets::IWidget* w = *it;
                if (w->GetVID() == -1 && w->GetGID() == m_GID) {
                        m_pRejillaActiva = dynamic_cast<GNKVisualizator::Widgets::WECG*> (w);
                        if (m_pRejillaActiva != NULL) {
                                return;
                        }
                }
        }

}

GNKVisualizator::Widgets::WECGBuilder::~WECGBuilder()
{
        m_pManager->EliminarObservador(this);
}

void GNKVisualizator::Widgets::WECGBuilder::AutoCallibrateImage()
{
        if (m_pRejillaActiva) {
                m_pRejillaActiva->AutoCallibrate();
        }

}

void GNKVisualizator::Widgets::WECGBuilder::OnWidgetDestruido(GNC::GCS::Widgets::IWidget* w)
{
        if (m_pRejillaActiva == w) {
                m_pRejillaActiva = NULL;
        }
}

//region "Interfaz especifica"
void GNKVisualizator::Widgets::WECGBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& )
{
        if (!m_pManager) {
                return;
        }
        GTRACE("GNKVisualizator::Widgets::WECGBuilder::OnMouseEvents(wxMouseEvent&)");
}

void GNKVisualizator::Widgets::WECGBuilder::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
        GTRACE("GNKVisualizator::Widgets::WECGBuilder::OnKeyEvents(wxKeyEvent&)");
}

void GNKVisualizator::Widgets::WECGBuilder::Render(GNC::GCS::Contexto3D* )
{
        return;
}

//endregion

GNKVisualizator::Widgets::RecursosECG::RecursosECG()
{
}
GNKVisualizator::Widgets::RecursosECG::~RecursosECG()
{
        if (!Texturas.empty()) {
                LOG_WARN("ECGResources", "Resource leaked");
        }
}

void GNKVisualizator::Widgets::RecursosECG::Alloc(size_t csize)
{
        if (csize == Texturas.size()) {
                return;
        }
        Dealloc();
        Texturas = TListaTexturas(csize);
        for (unsigned int i = 0; i < csize; i++) {
                Texturas[i] = new TexturasCanalECG();
        }
}

void GNKVisualizator::Widgets::RecursosECG::Dealloc()
{
        for (size_t i = 0; i < Texturas.size(); i++) {
                Texturas[i]->Destruir();
                delete Texturas[i];
        }
        Texturas.clear();
}

namespace GNKVisualizator
{
namespace Widgets
{

TexturasCanalECG::TexturasCanalECG()
{
        tl = new GNC::GCS::TexturaCairo();
        tl->Redimensionar(2, 2);
}

TexturasCanalECG::~TexturasCanalECG()
{
        delete tl;
}

void TexturasCanalECG::Destruir()
{
        tl->Destruir();
}

class Channel
{
public:

        Channel(const ChannelInfo& info, const GNC::GCS::Vector& pos, const GNC::GCS::Vector& size, GNC::GCS::Vector& rel):
                sx(0.0),
                sy(0.0),
                Pos(pos),
                Size(size),
                Rel(rel),
                Info(info)
        {
        }

        ~Channel()
        {
        }

        void Plot(GNC::GCS::Contexto3D* c, GNC::GCS::TexturaCairo* tl)
        {

                GNC::GCS::Vector sample;
                GNC::GCS::Vector textSize;


                if (!tl->EsValida() || TamTitle.EsNulo()) {
                        const std::string Title = Info.GetTitle();
                        tl->SetFontBold(WIDGETS_TEXT_FONT_FAMILY, GNC::GCS::IControladorPermisos::Instance()->Get("style", "ecg.label.fontSize").ObtenerValor<float>());
                        textSize = GNC::GCS::GLHelper::calcularBoundingBox((*tl), Title, false, 300.0f); // Tamaño de la region en pixels
                        tl->Redimensionar(std::ceil(textSize.x), std::ceil(textSize.y));
                        textSize.Asignar(tl->ancho, tl->alto);

                        tl->SetFontBold(WIDGETS_TEXT_FONT_FAMILY, GNC::GCS::IControladorPermisos::Instance()->Get("style", "ecg.label.fontSize").ObtenerValor<float>());
                        textSize = GNC::GCS::GLHelper::calcularBoundingBox((*tl), Title, false, 300.0f); // Tamaño de la region en pixels
                        GNC::GCS::GLHelper::TColor col(LEAD_TEXT_COLOR);
                        TamTitle = GNC::GCS::GLHelper::dibujarTexto(*tl, Title, col, false);
                }

                if (!tl->texturaCargada) {
                        tl->Cargar();
                }
                if (tl->texturaModificada) {
                        tl->Actualizar();
                }

                if (c->RefRelacionImagenPantalla().x < TEXT_FAR_THRESHOLD ) {
                        //compute display position...if it's too far labels doesn't show well
                        GNC::GCS::Vector planePos;
                        planePos = Pos;
                        GNC::GCS::Vector3D worldPos;
                        GNC::GCS::Vector3D displayPos;

                        c->pRenderer->m_pImageViewer->UnProyect2D(planePos.v, worldPos.v);
                        c->pRenderer->WorldToDisplay(worldPos.v, displayPos.v);

                        //ensure label is shown...
                        displayPos.x = std::max(0.0, displayPos.x);
                        displayPos.y = std::max(0.0, displayPos.y);

                        wDibujarTexturaTexto2D(c, *tl, displayPos.v);
                }


                if (c->RefRelacionImagenPantalla().x > GRAPH_FAR_THRESHOLD ) {
                        glColor4d(GRAPH_LINE_COLOR_FAR);
                        glLineWidth(GRAPH_LINE_WIDTH_FAR);
                } else {
                        glColor4d(GRAPH_LINE_COLOR);
                        glLineWidth(GRAPH_LINE_WIDTH);
                }

                //draw reference...
                int i = 0;
                {
                        //2 squares of 0.2 sec
                        float referenceWidth = Info.SamplingFrecuency * REFERENCE_WIDTH;
                        if (!Info.Samples.empty()) {
                                glBegin(GL_LINE_STRIP);
                                do {
                                        if ((i < referenceWidth/4) || (i >3*referenceWidth/4)) {
                                                sample = Pos + (GNC::GCS::Vector(i, 0) * Rel);
                                        } else {
                                                sample = Pos + (GNC::GCS::Vector(i, -100) * Rel);
                                                //sample += GNC::GCS::Vector(0, -10.0f);
                                        }
                                        glVertex3d( sample.x, sample.y, 0.0);
                                } while (++i < referenceWidth);
                                glEnd();
                        }
                }

                glBegin(GL_LINE_STRIP);
                for (GNKVisualizator::ChannelInfo::TVectorSamples::const_iterator it = Info.Samples.begin(); it != Info.Samples.end(); ++it, ++i) {
                        sample = Pos + (GNC::GCS::Vector(i, -(*it) * Info.Multiplier * Info.Sensitivity * Info.SensitivityCorrectionFactor) * Rel);
                        glVertex3d( sample.x, sample.y, 0.0);

                }
                glEnd();
        }

        double sx;
        double sy;

        GNC::GCS::Vector Pos;
        GNC::GCS::Vector Size;
        GNC::GCS::Vector Rel;


        GNC::GCS::Vector TamTitle;
        GNC::GCS::Vector TamUnits;

        const ChannelInfo Info;
};

class Data : public std::list<Channel>
{
public:
        Data() : NumSamples(0), NumChannels(0), SamplingFreq(0.0)
        {
        }

        ~Data()
        {
                clear();
        }

        void Setup(GNC::GCS::Vector ImageSize, GNC::GCS::Vector ImageSpacing, const GNKVisualizator::ECGStudy::TListChannelInfo& listInfoChannels)
        {
                clear();
                if (listInfoChannels.size() == 0) {
                        return;
                }
                NumChannels = listInfoChannels.size();
                SamplingFreq = listInfoChannels.front().SamplingFrecuency;
                //we have to add reference width (0.4 seconds)
                const float referenceWidth = SamplingFreq*REFERENCE_WIDTH;
                NumSamples = listInfoChannels.front().NumSamples + referenceWidth;

                GNC::GCS::Vector WinMargin[2] = {ImageSize * WinMarginRel[0], ImageSize * WinMarginRel[1]};
                GNC::GCS::Vector WinSize = WinMargin[1] - WinMargin[0];

                GNC::GCS::Vector CSize ( WinSize.x / (double) NumSamples, WinSize.y / (double) NumChannels);
                GNC::GCS::Vector COff(WinMargin[0].x, WinMargin[0].y  + (0.5 * CSize.y));
                GNC::GCS::Vector CRel = GNC::GCS::Vector( CSize.x , 0.1/ImageSpacing.y); //con el inverso del spacing se sacan los mV como es 10mm/mv pues 0.1mm entre el spacing

                GNC::GCS::Vector CPos = COff;

                for (GNKVisualizator::ECGStudy::TListChannelInfo::const_iterator it = listInfoChannels.begin(); it != listInfoChannels.end(); ++it) {
                        push_back(Channel((*it), CPos, CSize, CRel));
                        CPos.y += CSize.y;
                }
        }

        void Plot(GNC::GCS::Contexto3D* ct, GNKVisualizator::Widgets::RecursosECG* rec)
        {

                rec->Alloc(size());
                unsigned int i = 0;
                for (iterator it = begin(); it != end(); ++it, i++) {
                        Channel& c = *it;
                        c.Plot(ct, rec->Texturas[i]->tl);
                }
        }

        size_t NumSamples;
        size_t NumChannels;
        double SamplingFreq;

};

}
}
//-----------------------------------------------------------------------------------------------

//region "Constructor y destructor"

GNKVisualizator::Widgets::WECG::WECG(GNC::GCS::IWidgetsManager* pManager, long vid, const char* nombre) : GNC::GCS::Widgets::IWidget(pManager, vid, nombre)
{
        m_TopLevel        = false;
        m_Oculto          = false;
        m_ReservaRecursos = true;
        this->CData       = new Data();
}

GNKVisualizator::Widgets::WECG::~WECG()
{
        delete this->CData;
}

//endregion

template<class T> void Get(const std::string& str, T& val)
{
        std::istringstream is(str);
        is >> val;
}

void GNKVisualizator::Widgets::WECG::AutoCallibrate()
{
        int dims[3];
        m_pManager->GetRendererActivo()->m_pImageViewer->GetDimensions(dims);

        GNC::GCS::Vector dimsImagen = dims;

        if (CData->size() > 0) {
                double time_channels = CData->NumSamples / CData->SamplingFreq;
                GNC::GCS::Vector WinMarginSize = (WinMarginRel[1] - WinMarginRel[0]) * dimsImagen;

                double time_px = 25.0 * (time_channels / WinMarginSize.x); // 25 mm/s
                double amplitude_px = 10.0 * (2.0 * (double) CData->size()  / WinMarginSize.y); // 10 mm/mV => 20 mV por canal

                GNC::GCS::Vector3D oldSpacing;
                GNC::GCS::Vector3D newSpacing(time_px, amplitude_px, 1.0);
                GNC::GCS::Vector3D oldOrigin;

                m_pManager->GetRendererActivo()->m_pImageViewer->GetSpacing(oldSpacing.v);
                m_pManager->GetRendererActivo()->m_pImageViewer->GetOrigin(oldOrigin.v);

                if (oldSpacing != newSpacing) {

                        GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(m_pManager->GetVista(), GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada, GetVID());
                        pEvt->SetOldSpacing(oldSpacing.v);
                        pEvt->SetNewSpacing(newSpacing.v);
                        pEvt->SetOldOrigin(oldOrigin.v);
                        pEvt->SetNewOrigin(oldOrigin.v);

                        GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);
                }
        }
}


void GNKVisualizator::Widgets::WECG::Reload(const GNKVisualizator::ECGStudy::TListChannelInfo& listInfoChannels)
{
        if(listInfoChannels.size() > 0) {
                int dims[3];
                double spacing[3];
                m_pManager->GetRendererActivo()->m_pImageViewer->GetDimensions(dims);
                m_pManager->GetRendererActivo()->m_pImageViewer->GetSpacing(spacing);

                GNC::GCS::Vector imageDimensions = dims;
                GNC::GCS::Vector imageSpacing = spacing;


                CData->Setup(imageDimensions, imageSpacing, listInfoChannels);
        } else {
                return;
        }

        //base->dump(std::cout);



        //waveformSeq->dump(std::cout);


}


//region "Interfaz generica"

void GNKVisualizator::Widgets::WECG::OnMouseEvents(GNC::GCS::Events::EventoRaton& )
{
}

void GNKVisualizator::Widgets::WECG::OnKeyEvents(GNC::GCS::Events::EventoTeclado&)
{
}

bool GNKVisualizator::Widgets::WECG::HitTest(float , float , const GNC::GCS::Vector& /*umbral*/)
{
        return false;
}

bool GNKVisualizator::Widgets::WECG::HitTest(GNC::GCS::Vector* , int )
{
        return false;
}

void GNKVisualizator::Widgets::WECG::Render(GNC::GCS::Contexto3D* c)
{
        if(m_Oculto) {
                return;
        }

        if (CData->size() == 0) {
                return;
        }

        RecursosECG* rec = NULL;
        TMapaRecursos::iterator it = m_mapaRecursos.find(c->GetRenderer());
        if(it == m_mapaRecursos.end()) {
                rec = new RecursosECG();
                m_mapaRecursos[c->GetRenderer()] = rec;
        } else {
                rec = (*it).second;
        }


        {
                int dims[3];
                GNC::GCS::Vector3D spacing;
                m_pManager->GetRendererActivo()->m_pImageViewer->GetDimensions(dims);
                m_pManager->GetRendererActivo()->m_pImageViewer->GetSpacing(spacing.v);

                GNC::GCS::Vector dimsImagen = dims;
                GNC::GCS::Vector step;

                GNC::GCS::Vector win[2] = { dimsImagen * WinMarginRel[0], dimsImagen * WinMarginRel[1] };

                glClearColor(COLOR_FONDO);
                glClear(GL_COLOR_BUFFER_BIT);
                /*
                glBegin(GL_QUADS);
                glVertex2d(0.0, 0.0);
                glVertex2d(0.0, dimsImagen.y);
                glVertex2d(dimsImagen.x, dimsImagen.y);
                glVertex2d(dimsImagen.x, 0.0);
                glEnd();
                */
                step.Asignar(1.0 / spacing.x, 1.0 / spacing.y);

                if (c->RefRelacionImagenPantalla().x < GRID_1MM_NEAR_THRESHOLD) {

                        glLineWidth(GRID_1MM_LINE_WIDTH);
                        glColor4f(GRID_1MM_LINE_COLOR);
                        glBegin(GL_LINES);
                        for (double x = win[0].x; x < win[1].x; x += step.x) {
                                glVertex2d(x, win[0].y);
                                glVertex2d(x, win[1].y);
                        }
                        for (double y = win[0].y; y < win[1].y; y += step.y) {
                                glVertex2d(win[0].x, y);
                                glVertex2d(win[1].x, y);
                        }
                        glEnd();
                }

                step *= 5.0f;


                if (c->RefRelacionImagenPantalla().x > GRID_5MM_FAR_THRESHOLD) {
                        glLineWidth(GRID_5MM_LINE_WIDTH_FAR);
                        glColor4f(GRID_5MM_LINE_COLOR_FAR);
                } else {
                        glLineWidth(GRID_5MM_LINE_WIDTH);
                        glColor4f(GRID_5MM_LINE_COLOR);
                }

                glBegin(GL_LINES);
                for (double x = win[0].x; x <= win[1].x; x += step.x) {
                        glVertex2d(x, win[0].y);
                        glVertex2d(x, win[1].y);
                }
                for (double y = win[0].y; y <= win[1].y; y += step.y) {
                        glVertex2d(win[0].x, y);
                        glVertex2d(win[1].x, y);
                }
                glEnd();

                /*
                glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
                glLineWidth(2.0f);
                glBegin(GL_LINE_LOOP);
                glVertex2d(0.0, 0.0);
                glVertex2d(dims[0], 0.0);
                glVertex2d(dims[0], dims[1]);
                glVertex2d(0.0, dims[1]);
                glVertex2d(0.0, 0.0);
                glEnd();
                */
        }

        CData->Plot(c, rec);
}

void GNKVisualizator::Widgets::WECG::Seleccionar(bool )
{
}

void GNKVisualizator::Widgets::WECG::Iluminar(bool )
{
}

void GNKVisualizator::Widgets::WECG::Ocultar(bool oculto)
{
        if (m_Oculto != oculto) {
                m_Oculto = oculto;
                m_Modificado = true;
        }
}

void GNKVisualizator::Widgets::WECG::LiberarRecursos(GNC::GCS::IWidgetsRenderer* pKey)
{
        TMapaRecursos::iterator it = m_mapaRecursos.find(pKey);
        if(it != m_mapaRecursos.end()) {
                RecursosECG* rec = (*it).second;
                rec->Dealloc();
                delete rec;
                m_mapaRecursos.erase(it);
        }
}
//endregion

