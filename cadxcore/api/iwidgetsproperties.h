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

#define _GMAX(a,b) ( a > b? a : b )
#define _GMIN(a,b) ( a < b? a : b )

namespace GNC {
	namespace GCS {
		namespace Widgets {
			
			//region "Cursores
			typedef enum TipoCursor {
				CUR_INVALIDO,
				CUR_TRANSPARENTE,
				CUR_LUPA,
				CUR_SELECCION,
				CUR_FLECHA,
				CUR_MANO_ABIERTA,
				CUR_MANO_CERRADA,
				CUR_CREAR_LINEA,
				CUR_CREAR_ANGULO_COBB,
				CUR_CREAR_ANGULO_NORMAL,
				CUR_CREAR_ANOTACION,
				CUR_CREAR_ELIPSE,
				CUR_CREAR_PUNTO,
				CUR_CREAR_POLIGONO,
				CUR_CREAR_RECTANGULO,
				CUR_WINDOW_LEVEL,
				CUR_ANCLA_SIMPLE,
				CUR_ANCLA_COMPUESTA,
				CUR_ROTATION
			} TipoCursor;
			
			class Color {
				
			private:
				float R;
				float G;
				float B;
				float A;
				
			public:
				
				inline Color() {
					R = 1.0f;
					G = 1.0f;
					B = 1.0f;
					A = 1.0f;
				}
				
				inline Color(const Color& c)
				{
					*this = c;
				}
				
				inline Color (float r, float g, float b, float a = 1.0f)
				{
					R = Clamp(r);
					G = Clamp(g);
					B = Clamp(b);
					A = Clamp(a);
				}
				
				inline Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a = 255)
				{
					R = Clamp((float)r / 255.0f);
					G = Clamp((float)g / 255.0f);
					B = Clamp((float)b / 255.0f);
					A = Clamp((float)a / 255.0f);
				}
				
				inline Color& operator=(const Color& o)
				{
					R = o.R;
					G = o.G;
					B = o.B;
					A = o.A;
					return *this;
				}
				
				inline bool operator==(const Color& o)
				{
					return (R == o.R && G == o.G && B == o.B && A == o.A);
				}
				
				inline bool operator!=(const Color& o)
				{
					return !( (*this) == o);
				}
				
				inline Color operator*(float val) const
				{
					Color c(*this);
					c.R = Clamp(c.R * val);
					c.G = Clamp(c.G * val);
					c.B = Clamp(c.B * val);
					c.A = Clamp(c.A * val);
					return c;
				}
				
				inline Color operator-(float val) const
				{
					Color c(*this);
					c.R = Clamp(c.R - val);
					c.G = Clamp(c.G - val);
					c.B = Clamp(c.B - val);
					return c;
				}
				
				inline Color operator-(const Color& o) const
				{
					Color c(*this);
					c.R = Clamp(c.R - o.R);
					c.G = Clamp(c.G - o.G);
					c.B = Clamp(c.B - o.B);
					return c;
				}
				
				inline Color operator/(float val) const
				{
					Color c(*this);
					c.R = Clamp(c.R / val);
					c.G = Clamp(c.G / val);
					c.B = Clamp(c.B / val);
					c.A = Clamp(c.A / val);
					return c;
				}
				
				inline Color Complementario() const
				{
					float metrica = Max() + Min();
					return (*this) - metrica;
				}
				
				inline Color Complementar()
				{
					return *this = Complementario();
				}
				
				inline Color Inverso() const
				{
					return Color(1.0f, 1.0f, 1.0f) - (*this);
				}
				
				inline float Clamp(float valor) const
				{
					return _GMAX(0.0f, _GMIN(1.0f, valor));
				}
				
				inline float Max() const
				{
					return _GMAX(R, _GMAX(G, B));
				}
				
				inline float Min() const
				{
					return _GMIN(R, _GMIN(G, B));
				}
				
				inline Color& Invertir() {
					return (*this = Inverso());
				}
				
				inline unsigned char R8() const
				{
					return (unsigned char) (this->R * 255.0f);
				}
				
				inline unsigned char G8() const
				{
					return (unsigned char) (G * 255.0f);
				}
				
				inline unsigned char B8() const
				{
					return (unsigned char) (B * 255.0f);
				}
				
				inline unsigned char A8() const
				{
					return (unsigned char) (A * 255.0f);
				}
				
				inline float RF() const
				{
					return R;
				}
				
				inline float GF() const
				{
					return G;
				}
				
				inline float BF() const
				{
					return B;
				}
				
				inline float AF() const
				{
					return A;
				}
				
				inline void SetRF(float r)
				{
					R = Clamp(r);
				}
				
				inline void SetGF(float g)
				{
					G = Clamp(g);
				}
				
				inline void SetBF(float b)
				{
					B = Clamp(b);
				}
				
				inline void SetAF(float a)
				{
					A = Clamp(a);
				}
				
				inline void SetR8(unsigned char r)
				{
					R = Clamp((float)r / 255.0f);
				}
				
				inline void SetG8(unsigned char g)
				{
					G = Clamp((float)g / 255.0f);
				}
				
				inline void SetB8(unsigned char b)
				{
					B = Clamp((float)b / 255.0f);
				}
				
				inline void SetA8(unsigned char a)
				{
					A = Clamp((float)a / 255.0f);
				}
				
				friend std::ostream& operator<<(std::ostream& out, const Color& c)
				{
					return out << "[ R = " << c.R << ", G = " << c.G << ", B = " << c.B << ", A = " << c.A << " ]";
				}
				
				friend std::ostream& operator<<(std::ostream& out, const Color* c)
				{
					if (c != NULL) {
						return out << *c;
					}
					else {
						return out << "[ NULL ]";
					}
				}
			};
		}
	}
}
