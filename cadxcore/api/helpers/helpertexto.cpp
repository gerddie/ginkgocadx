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

//#define _GINKGO_TRACE
#include <list>

#include <api/globals.h>
#include <api/icontexto.h>
#include <cairo/cairo.h>
#include <api/westilo.h>
#include "helpertexto.h"

GNC::GCS::Widgets::HelperTexto::TVector GNC::GCS::Widgets::HelperTexto::calcularBoundingBox(TContexto* c, const std::string& texto, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
        return procesarTexto(c->cr, texto, NULL, withShadow, anchoMaximo, alineamiento, false);
}

GNC::GCS::Widgets::HelperTexto::TVector GNC::GCS::Widgets::HelperTexto::dibujarTexto(TContexto* c, const std::string& texto, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
        return procesarTexto(c->cr, texto, NULL, withShadow, anchoMaximo, alineamiento, true);
}

GNC::GCS::Widgets::HelperTexto::TVector GNC::GCS::Widgets::HelperTexto::dibujarTexto(TContexto* c, const std::string& texto, const TColor& col, bool withShadow, float anchoMaximo, TAlineamiento alineamiento)
{
        return procesarTexto(c->cr, texto, &col, withShadow, anchoMaximo, alineamiento, true);
}


class Palabra
{
public:
        Palabra()
        {
        }

        Palabra(const Palabra& o)
        {
                *this = o;
        }

        Palabra(const std::string& texto, GNC::GCS::Vector size)
        {
                this->texto = texto;
                this->psize = size;
        }

        Palabra& operator = (const Palabra& o)
        {
                this->texto = o.texto;
                this->psize = o.psize;
                return *this;
        }

        std::string texto;
        GNC::GCS::Vector psize;

};

class Linea : public std::list<Palabra>
{

public:
        typedef std::list<Linea> Base;


        Linea()
        {
        }

        Linea(GNC::GCS::Vector size)
        {
                lsize = size;
        }

        void Insertar(const Palabra& palabra)
        {
                if (size() == 0) {
                        lsize.AsignarMaximos(palabra.psize);
                } else {
                        lsize.x += palabra.psize.x;
                        lsize.y = std::max(lsize.y, palabra.psize.y);
                }
                push_back(palabra);
        }

        double NumPalabrasD() const
        {
                return (double) size();
        }

        unsigned long NumPalabras() const
        {
                return size();
        }

        GNC::GCS::Vector lsize;

};

class Parrafo : public std::list<Linea>
{
public:
        typedef std::list<Linea> Base;
        typedef GNC::GCS::Widgets::HelperTexto::TAlineamiento TAlineamiento;

        Parrafo(_cairo* cr, double anchoMaximo, const GNC::GCS::Widgets::Color* c,  bool withShadow, TAlineamiento alineamiento = GNC::GCS::Widgets::HelperTexto::TA_Izquierda, double margen_horizontal = 2.0f, double margen_vertical = 2.0f)
        {
                this->cr = cr;
                this->margen.Asignar(margen_horizontal, margen_vertical);
                this->anchoMaximo = anchoMaximo - margen.x - margen.x;
                this->alineamiento = alineamiento;
                this->color = c;
                this->drawShadow = withShadow;
                cairo_font_extents(cr, &font_extents);
                cairo_text_extents(cr, "i", &espacio_extents);
                InsertarNuevaLinea();
        }

        void InsertarPalabra(const std::string& palabra)
        {
                cairo_text_extents_t text_extents;

                cairo_text_extents(cr, palabra.c_str(), &text_extents);

                GNC::GCS::Vector psize (text_extents.x_advance - text_extents.x_bearing, text_extents.y_advance - text_extents.y_bearing);

                Palabra p(palabra, psize);

                Linea& la = LineaActual();
                if ( Cabe(p, la) ) {
                        la.Insertar(p);
                } else {
                        InsertarNuevaLinea();
                        la.Insertar(p);
                }
        }

        void InsertarNuevaLinea()
        {
                push_back(Linea(GNC::GCS::Vector(0, std::ceil(font_extents.height) + 3.0f)));
        }

        void InsertarTab()
        {
        }

        Linea& LineaActual()
        {
                return back();
        }

        inline double AnchoDeEspacios(const Linea& l)
        {
                return std::max(l.NumPalabrasD() - 1.0f, (double)0.0f) * espacio_extents.x_advance;
        }

        bool Cabe(Palabra& p, Linea& l) const
        {
                bool cabe = false;
                if (anchoMaximo > 0) {
                        // El ancho de la linea actual es el ancho de las palabras mas el ancho de los espacios entre sus palabras.
                        const double anchoLineaActual = l.lsize.x + ( std::max(l.NumPalabrasD(), (double)0.0f) * espacio_extents.x_advance);
                        if ( anchoMaximo > (anchoLineaActual + p.psize.x) ) {
                                cabe = true;
                        }
                } else {
                        cabe = true;
                }

                return cabe;
        }

        GNC::GCS::Vector Procesar(bool dibujar = true)
        {
                GNC::GCS::Vector bbox;
                for (Base::iterator it = begin(); it != end(); ++it) {
                        const Linea& l = (*it);

                        bbox.x = std::max(bbox.x, l.lsize.x + AnchoDeEspacios(l));
                        bbox.y += std::ceil(font_extents.height) + 2.0f;
                }
                bbox += (margen + margen);
                if (dibujar) {

                        // Posicion inicial
                        GNC::GCS::Vector pos = margen + GNC::GCS::Vector(0.0f, std::ceil(font_extents.height));

                        for (Base::iterator itl = begin(); itl != end(); ++itl) {
                                const Linea& l = (*itl);
                                DibujarLinea(l, bbox, pos);
                                pos.y += (std::ceil(font_extents.height) + 2.0f);
                        }
                }
                return bbox;
        }

        inline void DibujarLinea(const Linea& l, const GNC::GCS::Vector& bbox, const GNC::GCS::Vector& posInicial)
        {
                GNC::GCS::Vector pos(posInicial);

                switch(alineamiento) {
                default:
                case GNC::GCS::Widgets::HelperTexto::TA_Izquierda: {
                        for (Linea::const_iterator it = l.begin(); it != l.end(); ++it) {
                                const Palabra& p = (*it);
                                DibujarPalabra(p, pos);
                                pos.x += (p.psize.x + espacio_extents.x_advance);
                        }
                }
                break;
                case GNC::GCS::Widgets::HelperTexto::TA_Derecha: {
                        pos.x += bbox.x - (margen.x + margen.x + l.lsize.x + AnchoDeEspacios(l));
                        for (Linea::const_iterator it = l.begin(); it != l.end(); ++it) {
                                const Palabra& p = (*it);
                                DibujarPalabra(p, pos);
                                pos.x += (p.psize.x + espacio_extents.x_advance);
                        }
                }
                break;
                case GNC::GCS::Widgets::HelperTexto::TA_Centrado: {
                        pos.x += ( bbox.x - (margen.x + margen.x + l.lsize.x + AnchoDeEspacios(l)) ) / 2.0f;
                        for (Linea::const_iterator it = l.begin(); it != l.end(); ++it) {
                                const Palabra& p = (*it);
                                DibujarPalabra(p, pos);
                                pos.x += (p.psize.x + espacio_extents.x_advance);
                        }
                }
                break;
                case GNC::GCS::Widgets::HelperTexto::TA_Justificado: {
                        double resto = ( bbox.x - (margen.x + margen.x + l.lsize.x + AnchoDeEspacios(l)) );
                        resto = resto / std::max( (double)1.0f, l.NumPalabrasD() - 1.0f);
                        for (Linea::const_iterator it = l.begin(); it != l.end(); ++it) {
                                const Palabra& p = (*it);
                                DibujarPalabra(p, pos);
                                pos.x += (p.psize.x + espacio_extents.x_advance + resto);
                        }
                }
                break;
                }
        }

        inline void DibujarPalabra(const Palabra& p, const GNC::GCS::Vector& pos)
        {
                if (p.texto.size() > 0) {

                        if (drawShadow) {
                                cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);

                                cairo_move_to(cr, pos.x + 1.0f, pos.y + 1.0f);
                                cairo_set_source_rgba(cr, DEFAULT_TEXT_COLOR_INVERTED);
                                cairo_show_text(cr, p.texto.c_str());
                        } else {
                                cairo_set_operator(cr, CAIRO_OPERATOR_OVER);
                        }

                        cairo_move_to(cr, pos.x, pos.y);

                        if (color == NULL) {
                                cairo_set_source_rgba(cr, DEFAULT_TEXT_COLOR);
                        } else {
                                cairo_set_source_rgba(cr, color->RF(), color->GF(), color->BF(), color->AF());
                        }
                        cairo_show_text(cr, p.texto.c_str());

                }
        }

        _cairo*                   cr;
        double                    anchoMaximo;
        TAlineamiento             alineamiento;

        cairo_font_extents_t      font_extents;
        cairo_text_extents_t      espacio_extents;

        GNC::GCS::Vector          margen;
        const GNC::GCS::Widgets::Color* color;
        bool                      drawShadow;

};

GNC::GCS::Widgets::HelperTexto::TVector GNC::GCS::Widgets::HelperTexto::procesarTexto(_cairo* cr, const std::string& texto, const TColor* c, bool withShadow, float anchoMaximo, TAlineamiento alineamiento, bool dibujar)
{

        Parrafo par(cr, anchoMaximo, c, withShadow, alineamiento, 5.0f, 5.0f);

        // Parseo del texto caracter a caracter
        for (std::string::const_iterator it1 = texto.begin(); it1 != texto.end(); ) {
                std::string palabra;
                int wl = 0;
                std::string::const_iterator it2;

                for (it2 = it1; it2 != texto.end() && *it2 != ' ' && *it2 != '\t' && *it2 != '\r' && *it2 != '\n'; ++it2) {
                        wl++;
                }
                if (wl > 0) {
                        palabra = std::string(it1, it2);
                        it1 += wl;

                        par.InsertarPalabra(palabra);

                } else {
                        int skipped = 0;
                        if (*it2 == ' ') {
                                skipped = 1;
                        } else if (*it2 == '\t') {
                                par.InsertarTab();
                                skipped = 1;
                        } else if (*it2 == '\n') {
                                par.InsertarNuevaLinea();
                                skipped = 1;
                        } else if (*it2 == '\r') {
                                par.InsertarNuevaLinea();
                                if (it2+1 != texto.end() && *(it2+1) == '\n') {
                                        skipped = 2;
                                } else {
                                        skipped = 1;
                                }
                        }
                        it1 += skipped;
                }
        }

        return par.Procesar(dibujar);
}
