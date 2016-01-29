#pragma once
/*
 *  
 *  $Id: geometry.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef GEOMETRY2D_H
#define GEOMETRY2D_H

#if defined(max)
#undef max
#endif
#if defined(min)
#undef min
#endif

#include <list>
#include <limits>
#include <iostream>

#include <api/api.h>
#include <api/globals.h>
#include <api/math/geometry_defs.h>

#define GEOMETRY2D_NO_TEMPLATE

#if defined(GEOMETRY2D_NO_TEMPLATE)

namespace GNC {
	namespace GCS {
		
		class Matriz;

		class Vector {
		public:
			
			typedef PRECISION_COMPONENTE_POR_DEFECTO TComp;
			typedef PRECISION_REALES_POR_DEFECTO     TReal;

			TComp& x;           // Coordenada mundo X
			TComp& y;           // Coordenada mundo Y 
			TComp& z;           // Coordenada mundo Z (Allways zero)
			TComp& w;           // Coordenada mundo W

			TComp v[4];

		public:

			static inline const Vector NaN()
			{
				return Vector(NaNDeComponentes(), NaNDeComponentes());
			}

			static inline const Vector Inf()
			{
				return Vector(InfinitoDeComponentes(), InfinitoDeComponentes());
			}

			static inline const Vector Zero()
			{
				return Vector();
			}

			static inline const Vector Unidad()
			{
				return Vector(1.0, 1.0);
			}

			//----------------------------------------------
			//-- Constructores y destructores
			//----------------------------------------------

			inline Vector() : x(v[0]), y(v[1]), z(v[2]), w(v[3])
			{
				x = (TComp)0.0;
				y = (TComp)0.0;
				z = (TComp)0.0;
				w = (TComp)1.0;
			}

			/* Constructor copia transparente */
			inline Vector(const Vector& b) : x(v[0]), y(v[1]), z(v[2]), w(v[3])
			{
				x = b.x;
				y = b.y;
				z = (TComp)0.0;
				w = (TComp)1.0;
			}

			template <class TipoComp1, class TipoComp2> 
			inline Vector(const TipoComp1& valX, const TipoComp2& valY)  : x(v[0]), y(v[1]), z(v[2]), w(v[3])
			{
				x = (TComp) valX;
				y = (TComp) valY;
				z = (TComp)0.0;
				w = (TComp)1.0;
			}

			template <class TipoComp> inline Vector(const TipoComp* const valor)  : x(v[0]), y(v[1]), z(v[2]), w(v[3])
			{
				x = valor[0];
				y = valor[1];
				z = (TComp)0.0;
				w = (TComp)1.0;
			}

			inline ~Vector()
			{
			}


			//----------------------------------------------
			//-- Asignacion explicita
			//----------------------------------------------

			/* Asigna las coordenadas al vector */
			template <class TipoComp1, class TipoComp2>
			inline Vector& Asignar(const TipoComp1& valX, const TipoComp2& valY)
			{
				x = (TComp) valX;
				y = (TComp) valY;
				return (*this);
			}

			/* Asigna el valor a todas las coordenadas del vector */
			template <class TipoComp> inline Vector& Asignar(const TipoComp& valor)
			{
				x = (TComp) valor;
				y = (TComp) valor;
				return *this;
			}

			/* Asigna el valor a todas las coordenadas del vector */
			template <class TipoComp> inline void Exportar(TipoComp* ptr)
			{
				ptr[0] = (TipoComp) x;
				ptr[1] = (TipoComp) y;
			}

			//----------------------------------------------
			//-- Normas Vectoriales, Distancias y Producto Interior
			//----------------------------------------------

			inline TComp Norma1() const {
				return std::abs(x) + std::abs(y);
			}

			/* Devuelve la norma 2 (euclidea) al cuadrado */
			inline TComp Norma2Cuadrado() const {
				return (TReal)x * (TReal)x + (TReal)y * (TReal)y;
			}

			/* Devuelve la norma 2 (euclidea) */
			inline TReal Norma2() const {
				return std::sqrt((TReal) (*this).Norma2Cuadrado());
			}

			inline TReal NormaEnesima(unsigned int n) const {
				return std::pow( std::pow((TReal)x, (TReal)n) + std::pow((TReal)y, (TReal)n), (TReal)1.0f / (TReal)n);
			}

			inline TComp NormaInfinito() const {
				return std::max<TReal>(x, y);
			}

			/* Devuelve la distancia euclidea a "b" */
			inline TReal DistanciaEuclidea(const Vector& b) const {
				const Vector& a = *this;
				return std::sqrt( (double) (b - a).Norma2Cuadrado() );
			}

			/* Devuelve el cuadrado de la distancia euclidea */
			inline TReal DistanciaEuclideaCuadrado(const Vector& b) const {
				const Vector& a = *this;
				return (b - a).Norma2Cuadrado();
			}

			/* Normaliza un vector teniendo en cuenta singularidades */
			inline Vector& Normalizar() {

				Vector& a = (*this);

				const TReal& norma = a.Norma2();

				if ( std::abs(norma) < EpsilonDeReales()) {
					a.Asignar( (TComp)0.0f, (TComp)0.0f);
				}
				else {
					a /= norma;
				}
				return a;
			}

			/* Devuelve el vector normalizado teniendo en cuenta singularidades */
			inline Vector Normalizado() const {
				return Vector(*this).Normalizar();
			}

			/* Devuelve el producto escalar por b */
			inline TReal ProductoEscalar(const Vector& b) const {
				const Vector& a = *this;
				return ((TReal)a.x * (TReal)b.x) + ((TReal)a.y * (TReal)b.y);
			}

			/* Devuelve la dimension z del producto vectorial */
			inline TReal ProductoVectorialZ(const Vector& b) const {
				const Vector& a = *this;
				return ((TReal)a.y * (TReal)b.x) - ((TReal)a.x * (TReal)b.y);
			}

			//----------------------------------------------
			//-- Angulos
			//----------------------------------------------

			/* Devuelve el angulo { entre [0, -PI) } que forma con "b" */
			inline TReal Angulo(const Vector& b) const {
				const Vector& a = (*this);
				const TReal& angulo = std::acos( ((TReal) a.ProductoEscalar(b)) / (a.Norma2() * b.Norma2()) );
				if ( (TReal) a.ProductoVectorialZ(b) < 0.0f) {
					return angulo;
				}
				else {
					return -1.0f * angulo;
				}
			}

			inline TReal Angulo() const {
				return std::atan2( (TReal) y, (TReal) x );
			}

			/* Devuelve el angulo entre pi/2 y -pi/2 tomando como origen el punto con abscisa mas baja */
			inline TReal AnguloSobreAbscisa() const {
				const TReal& angulo = Angulo();
				if (angulo > (TReal)M_PI_2) {
					return (TReal)-M_PI + angulo;
				}
				else if (angulo < (TReal)-M_PI_2) {
					return (TReal)M_PI + angulo;
				}
				else {
					return angulo;
				}
			}

			/* Devuelve el angulo rotado entre pi/2 y -pi/2 tomando como origen el punto con abscisa mas baja teniendo en cuenta una rotacion del eje z.*/
			inline TReal AnguloRotadoSobreAbscisa(const TReal& rotacion) const {
				const TReal& angulo = Angulo() - rotacion;
				if (angulo > (TReal)M_PI_2) {
					return (TReal)-M_PI + angulo;
				}
				else if (angulo < (TReal)-M_PI_2) {
					return (TReal)M_PI + angulo;
				}
				else {
					return angulo;
				}
			}

			/* Devuelve la pendiente del vector */
			inline TReal Pendiente() const {

				if (std::abs(x) < EpsilonDeComponentes()) {
					if (std::abs(y) < EpsilonDeComponentes()) {
						return NaNDeReales();
					}
					else {
						return (TReal)0.0f;
					}
				}
				else {
					return (TReal)y / (TReal)x;
				}
			}

			//----------------------------------------------
			//-- Ajuste e interpolacion
			//----------------------------------------------

			/* Sean "this" y rect, ambos rectangulos definidos con inicio en el origen de coordenadas,
			 calcula el rectangulo minimo con igual proporcion a "this" que contiene a rect */
			inline Vector AjusteOptimoProporcional(const Vector& r2) const {
				const Vector& r1 = *this;

				const TReal& m = std::abs(r1.Pendiente());

				if (m < EpsilonDeReales()) {
					return Vector( (TComp) 0.0f, (TComp) 0.0f);
				}

				const TReal& Yc = std::max<TReal>(std::abs(r2.y), std::abs(m * r2.x));

				return Vector(Yc / m, Yc);
			}

			/* Sean "this" y rect, ambos rectangulos definidos con inicio en el origen de coordenadas,
			 calcula el rectangulo minimo con igual proporcion a "this" contenido en rect */
			inline Vector AjusteInteriorProporcional(const Vector& r2) const {
				const Vector& r1 = *this;

				const TReal& m = std::abs(r1.Pendiente());

				if (m < EpsilonDeReales()) {
					return Vector( (TComp) 0.0f, (TComp) 0.0f);
				}

				const TReal& Yc = std::min<TReal>( (std::abs((TReal)r2.y)) , (std::abs( m * (TReal) r2.x)) );

				return Vector(Yc / m, Yc);
			}

			// Devuelve un vector que apunta al origen del desplazamiento de este vector para compartir centro con un vector determinado.
			inline Vector OrigenDeVectorCentradoA(const Vector& b) const
			{
				const Vector& a = (*this);
				return (b - a ) / 2.0f;
			}

			/* Sea rect(p0, p1) un rectangulo definido por las esquinas p0, p1, desinfla el rectangulo las unidades especificadas */
			static inline void DesinflarRectangulo(Vector& p0, Vector& p1, const TComp& tamano)
			{
				const Vector& puntoMedio = p0.PuntoMedio(p1);
				if (p0.x < p1.x) {
					p0.x = std::min(p0.x + tamano, puntoMedio.x);
					p1.x = std::max(p1.x - tamano, puntoMedio.x);
				}
				else {
					p0.x = std::min(p0.x - tamano, puntoMedio.x);
					p1.x = std::max(p1.x + tamano, puntoMedio.x);
				}

				if (p0.y < p1.y) {
					p0.y = std::min(p0.y + tamano, puntoMedio.y);
					p1.y = std::min(p1.y - tamano, puntoMedio.y);
				}
				else {
					p0.y = std::min(p0.y - tamano, puntoMedio.y);
					p1.y = std::min(p1.y + tamano, puntoMedio.y);
				}
			}

			/* Sea rect(p0, p1) un rectangulo definido por las esquinas p0, p1, infla el rectangulo las unidades especificadas */
			static inline void InflarRectangulo(Vector& p0, Vector& p1, const TComp& tamano) {
				if (p0.x < p1.x) {
					p0.x -= tamano;
					p1.x += tamano;
				}
				else {
					p0.x += tamano;
					p1.x -= tamano;
				}

				if (p0.y < p1.y) {
					p0.y += tamano;
					p1.y -= tamano;
				}
				else {
					p0.y -= tamano;
					p1.y += tamano;
				}
			}

			/* translada el vector de acuerdo con el nuevo spacing y con el nuevo origen */
			inline void ReEscalar(const TReal* oldSpacing, const TReal* oldOrigin, const TReal* newSpacing, const TReal* newOrigin) {
				const Vector& vOldSpacing = oldSpacing;
				const Vector vOldOrigin(oldOrigin[0],-oldOrigin[1]);
				const Vector& vNewSpacing = newSpacing;
				const Vector vNewOrigin(newOrigin[0],-newOrigin[1]);

				*this = ( ( (*this) - vOldOrigin) * ( vNewSpacing / vOldSpacing) ) + vNewOrigin;
			}

			inline Vector& ReflejarSobreAbscisas() {
				y = 0.0f - y;
				return *this;
			}

			inline Vector ReflejoSobreAbscisas() const {
				return Vector(x, 0.0f - y);
			}

			inline Vector& ReflejarSobreOrdenadas() {
				x = 0.0f - x;
				return *this;
			}

			inline Vector ReflejoSobreOrdenadas() const {
				return Vector(0.0f - x, y);
			}

			// Devuelve el reflejo del punto sobre el punto p (proyección sobre recta que atraviesa los dos puntos de manera que la distancia entre p y el punto actual se mantiene)
			inline Vector ReflejoSobrePunto(const Vector& p) const {
				return p + (p - *this);
			}

			// Refleja el punto sobre p (proyección sobre recta que atraviesa los dos puntos de manera que la distancia entre p y el punto actual se mantiene)
			inline Vector& ReflejarSobrePunto(const Vector& p) {
				*this = ReflejoSobrePunto(p);
				return *this;
			}

			//----------------------------------------------
			//-- Componentes
			//----------------------------------------------

			/* Devuelve la mayor de las componentes */
			inline const TComp& Mayor() const {
				return x>y?x:y;
			}

			/* Devuelve la menor de las componentes */
			inline const TComp& Menor() const {
				return x>y?y:x;
			}

			//----------------------------------------------
			//-- Constantes
			//----------------------------------------------

			static inline const TComp InfinitoDeComponentes() {
				return std::numeric_limits<TComp>::infinity();
			}

			static inline const TReal InfinitoDeReales() {
				return std::numeric_limits<TReal>::infinity();
			}

			static inline const TComp NaNDeComponentes() {
				return std::numeric_limits<TComp>::quiet_NaN();
			}

			static inline const TReal NaNDeReales() {
				return std::numeric_limits<TReal>::quiet_NaN();
			}

			static inline const TComp EpsilonDeComponentes() {
				return std::numeric_limits<TComp>::epsilon();
			}

			static inline const TReal EpsilonDeReales() {
				return std::numeric_limits<TReal>::epsilon();
			}

			//----------------------------------------------
			//-- Comprobaciones
			//----------------------------------------------

			inline bool EsNulo() const {
				#if defined(EXACT_COMPARISON)
				return x == 0.0f && y == 0.0f;
				#else
				return ((*this) - Zero()).ValorAbsoluto().NearlyEqual(Zero());
				#endif
			}

			inline bool EsInvalido() const {
				return (x != x) || (y != y);
			}

			inline bool EsNaN() const {
				return (x != x) || (y != y);
			}

			//----------------------------------------------
			//-- Geometria
			//----------------------------------------------

			// Calcula el vector ortogonal (perpendicular) con dirección y sentido segun la regla de la mano derecha.
			inline Vector VectorOrtogonal() const {
				if (std::abs(x) < EpsilonDeComponentes()) { // Vector sobre las ordenadas
					if (std::abs(y) < EpsilonDeComponentes()) { // Singularidad. Punto = 0, 0. Usamos un vector cualquiera
						return Vector(1.0f, 0.0f);
					}
					else if (y > 0.0f) {
						return Vector(1.0f, 0.0f);
					}
					else { // y < 0.0f
						return Vector(-1.0f, 0.0f);
					}
				}
				else {
					if ( std::abs(y) < EpsilonDeComponentes() ) { // Vector sobre las abscisas
						if( x > 0.0f) {
							return Vector(0.0f, -1.0f);
						}
						else {
							return Vector(0.0f, 1.0f);
						}
					}
					else if (y > 0.0f) {
						return Vector(1.0f, -x/y);
					}
					else { // y < 0.0f
						return Vector(-1.0f, x/y);
					}
				}
			}

			// Calcula el vector ortonormal (perpendicular y norma 1) con dirección y sentido segun la regla de la mano derecha.
			inline Vector VectorOrtonormal() const {
				if (std::abs(x) < EpsilonDeComponentes()) { // Vector sobre las ordenadas
					if (std::abs(y) < EpsilonDeComponentes()) { // Singularidad. Punto = 0, 0. Usamos un vector cualquiera
						return Vector(1.0f, 0.0f);
					}
					else if (y > 0.0f) {
						return Vector(1.0f, 0.0f);
					}
					else {
						return Vector(-1.0f, 0.0f);
					}
				}
				else {
					if (std::abs(y) < EpsilonDeComponentes()) { // Vector sobre las abscisas
						if( x > 0.0f ) {
							return Vector(0.0f, -1.0f);
						}
						else {
							return Vector(0.0f, 1.0f);
						}
					}
					else if (y > 0) {
						if (x > 0) {
							return Vector(1.0f, -x/y).Normalizar();
						}
						else {
							return Vector(1.0f, -x/y).Normalizar();
						}
					}
					else {
						if (x > 0) {
							return Vector(-1.0f, x/y).Normalizar();
						}
						else {
							return Vector(-1.0f, x/y).Normalizar();
						}
					}

				}
			}

			/* Devuelve el punto medio entre dos vectores */
			inline Vector PuntoMedio(const Vector& b) const {
				const Vector& a = (*this);
				return (a + b) / 2.0f;
			}

			/* Devuelve el punto de interseccion entre la recta r definida por r0,r1 y la recta s definida por s0,s1 */
			static inline Vector InterseccionEntreRectas(const Vector& r0, const Vector& r1, const Vector& s0, const Vector& s1) {

				TReal d1 = ((TReal)r0.x - (TReal)r1.x) * ((TReal)s0.y - (TReal)s1.y);
				TReal d2 = ((TReal)r0.y - (TReal)r1.y) * ((TReal)s0.x - (TReal)s1.x);

				if (std::abs(d1) < Vector::EpsilonDeReales()) {
					d1 = 0.0f;
				}
				if (std::abs(d2) < Vector::EpsilonDeReales()) {
					d2 = 0.0f;
				}
				const TReal& d = d1 - d2;
				if ( std::abs(d) < Vector::EpsilonDeReales()) {
					return Vector(NaNDeComponentes(), NaNDeComponentes());
				}
				else {
					const TComp& pre = (r0.x * r1.y - r0.y * r1.x);
					const TComp& post = (s0.x * s1.y - s0.y * s1.x);
					return ( ( (s0 - s1) * pre ) - (r0 - r1) * post) / d;
				}
			}

			static inline Vector InterseccionEntreRectaYElipse(const Vector&, const Vector& , const Vector& , const Vector& ) {
				return Vector();
			}

			static inline Vector VectorUnitarioTangenteAElipse(const Vector&, const Vector& , const TReal& ) {
				return Vector();
			}

			/* Devuelve el punto de la proyeccion ortogonal del punto sobre la recta definida por p0 y p1 */
			inline Vector ProyeccionOrtogonalSobreRecta(const Vector& p0, const Vector& p1) const {
				return InterseccionEntreRectas(*this, *this + (p1-p0).VectorOrtogonal(), p0, p1);
			}

			/* Devuelve la distancia del punto a la recta definida por p0 y p1 */
			inline TReal DistanciaARecta(const Vector& p0, const Vector& p1) const {
				return (ProyeccionOrtogonalSobreRecta(p0, p1) - (*this)).Norma2();
			}

			/* Devuelve el cuadrado de la distancia del punto a la segmento de p0 y p1 */
			inline TComp DistanciaASegmentoCuadrado(const Vector& p0, const Vector& p1) const {
				Vector vi = ProyeccionOrtogonalSobreRecta(p0, p1);
				Vector pm = p0.PuntoMedio(p1);
				if (vi.DistanciaEuclideaCuadrado(pm) > p0.DistanciaEuclideaCuadrado(pm)) {
					return std::min(this->DistanciaEuclideaCuadrado(p0), this->DistanciaEuclideaCuadrado(p1));
				}
				else {
					return this->DistanciaEuclideaCuadrado(vi);
				}
			}

			/* Devuelve el cuadrado de la distancia del punto a la segmento de p0 y p1 */
			inline TComp DistanciaASegmento(const Vector& p0, const Vector& p1) const {
				return std::sqrt(this->DistanciaASegmentoCuadrado(p0, p1));
			}

			/* Devuelve el cuadrado de la distancia del punto a la recta definida por p0 y p1 */
			inline TComp DistanciaARectaCuadrado(const Vector& p0, const Vector& p1) const {
				return (ProyeccionOrtogonalSobreRecta(p0, p1) - (*this)).Norma2Cuadrado();
			}

			//----------------------------------------------
			//-- Acotaciones
			//----------------------------------------------

			inline Vector ValorAbsoluto() {
				return Vector(std::abs(x), std::abs(y));
			}

			inline Vector TruncadoAMaximo(const TComp& val) const
			{
				return Vector(*this).TruncarAMaximo(val);
			}

			inline Vector TruncadoAMinimo(const TComp& val) const
			{
				return Vector(*this).TruncarAMinimo(val);
			}

			inline Vector& TruncarAMaximo(const TComp& val)
			{
				x = std::max<TReal>(x, val);
				y = std::max<TReal>(y, val);
				return *this;
			}

			inline Vector& TruncarAMinimo(const TComp& val)
			{
				x = std::min<TReal>(x, val);
				y = std::min<TReal>(y, val);
				return *this;
			}

			inline Vector TruncadoAMaximo(TComp valx, TComp valy) const
			{
				return Vector(*this).TruncarAMaximo(valx, valy);
			}

			inline Vector TruncadoAMinimo(TComp valx, TComp valy) const
			{
				return Vector(*this).TruncarAMinimo(valx, valy);
			}

			inline Vector& TruncarAMaximo(TComp valx, TComp valy)
			{
				x = std::max<TReal>(x, valx);
				y = std::max<TReal>(y, valy);
				return *this;
			}

			inline Vector& TruncarAMinimo(TComp valx, TComp valy)
			{
				x = std::min<TReal>(x, valx);
				y = std::min<TReal>(y, valy);
				return *this;
			}
			
			inline TComp ComponenteMaxima() const
			{
				return std::max<TReal>(x,y);
			}
			
			inline TComp ComponenteMinima() const
			{
				return std::min<TReal>(x,y);
			}
			
			static inline Vector ComponentesMaximas(const Vector& a, const Vector& b)
			{
				return Vector(std::max<TReal>(a.x, b.x), std::max<TReal>(a.y, b.y));
			}
			
			static inline Vector ComponentesMinimas(const Vector& a, const Vector& b)
			{
				return Vector(std::min<TReal>(a.x, b.x), std::min<TReal>(a.y, b.y));
			}
			
			inline Vector& AsignarMaximos(const Vector& o)
			{
				x = std::max<TReal>(x, o.x);
				y = std::max<TReal>(y, o.y);
				return *this;
			}
			
			inline Vector& AsignarMinimos(const Vector& o)
			{
				x = std::min<TReal>(x, o.x);
				y = std::min<TReal>(y, o.y);
				return *this;
			}
			
			inline Vector ObtenerMaximos(const Vector& o) const
			{
				return Vector(std::max<TReal>(x, o.x), std::max<TReal>(y, o.y));
			}
			
			inline Vector ObtenerMinimos(const Vector& o) const
			{
				return Vector(std::min<TReal>(x, o.x), std::min<TReal>(y, o.y));
			}
			
			inline bool TieneComponenteMayorQue(const Vector& o) const
			{
				return ( (x > o.x) || (y > o.y) );
			}
			
			inline bool TieneComponenteMenorQue(const Vector& o) const
			{
				return ( (x < o.x) || (y < o.y) );
			}

			/* Devuelve verdadero si el vector es singular (distancia a (0,0) < epsilon =~ (0,0) ) */
			inline bool Singular() const {
				return std::abs(x) < EpsilonDeComponentes() || std::abs(y) < EpsilonDeComponentes();
			}

			inline Vector RedondeoAlza() const {
				return Vector(*this).RedondearAlza();
			}

			inline Vector& RedondearAlza() {
				x = std::ceil(x);
				y = std::ceil(y);
				return *this;
			}

			inline Vector RedondeoBaja() const {
				return Vector(*this).RedondearBaja();
			}

			inline Vector& RedondearBaja() {
				x = std::floor(x);
				y = std::floor(y);
				return *this;
			}

			static TComp ValorRedondeado(const TComp& val) {
				const TComp& d = std::floor(val);
				const TComp& u = std::ceil(val);
				if ( val - d < u - val) {
					return d;
				}
				else {
					return u;
				}
			}

			inline Vector Redondeado() const {
				return Vector(*this).Redondear();
			}

			inline Vector& Redondear() {
				x = ValorRedondeado(x);
				y = ValorRedondeado(y);
				return *this;
			}

			/* Devuelve el vector de rotacion para un angulo dado (x = cos(angulo), y = sin(angulo)) */

			static inline Vector VectorDeRotacion(const TReal& angulo)
			{
				return Vector((TComp)std::cos(angulo), (TComp)std::sin(angulo));
			}

			/* Devuelve un nuevo vector rotado el vector sobre el eje de coordenadas Z */
			inline Vector RotacionSobreZ(const TReal& angulo) const;

			/* Rota el vector sobre el eje de coordenadas Z */
			inline Vector& RotarSobreZ(const TReal& angulo) {
				*this = RotacionSobreZ(angulo);
				return *this;
			}

			inline Vector RotacionSobreZ(const Vector& centro, const TReal& angulo) const;

			inline Vector& RotarSobreZ(const Vector& centro, const TReal& angulo) {
				*this = RotacionSobreZ(centro, angulo);
				return *this;
			}

			inline bool DentroDeElipse(const Vector& centro, const TReal& radioX, const TReal& radioY, const TReal& rotacion) const {
				Vector p(*this);
				p.RotarSobreZ(centro, -rotacion);

				const TComp& t1 = (p.x - centro.x) / radioX;
				const TComp& t2 = (p.y - centro.y) / radioY;

				return (t1 * t1) + (t2 * t2) <= (TComp)1.0f;
			}

			/* Devuelve true si el punto esta contenido dentro del poligono especificado.
			 Metodo del teorema de las curvas de Jordan para poligonos cerrados concavos o convexos.
			 Contempla los casos:
				* punto coincidente con un vértice.
			    * vertices consecutivos de poligono coincidentes.
			 */
			 template <class TipoVector>
			 inline bool DentroDePoligono(const TipoVector *v, int numVertices) const
			 {
				int cruces = 0;
				for ( int i = 0; i < numVertices; i++ ){
					if ( *this == v[i] ) {
						return true;
					}
					int j = (i == numVertices-1 ? 0 : i + 1);

					const TComp& xmin = std::min(v[i].x, v[j].x);
					const TComp& xmax = std::max(v[i].x, v[j].x);
					const TComp& ymin = std::min(v[i].y, v[j].y);
					const TComp& ymax = std::max(v[i].y, v[j].y);

					if (x <= xmax && y <= ymax && y >= ymin) {
						if (x <= xmin) {
							cruces++;
						}
						else {
							// Ecuacion de la linea
							const TComp& dx = v[j].x - v[i].x;

							if (std::abs(dx) > EpsilonDeComponentes()) {
								const Vector& pi = InterseccionEntreRectas (*this, *this + Vector(1.0f, 0.0f), v[i], v[j]);
								if ( pi.x > x){
									cruces++;
								}
							}
						}
					}
				}
				return (cruces & 1) != 0;
			}

			 /* Devuelve true si el punto esta contenido dentro del poligono especificado.
			 Metodo del teorema de las curvas de Jordan para poligonos cerrados concavos o convexos.
			 Contempla los casos:
				* punto coincidente con un vértice.
			    * vertices consecutivos de poligono coincidentes.
			 */
			 template <class TipoVector>
			 inline bool DentroDePoligono(const TipoVector& lista) const
			 {
				int cruces = 0;
				typename TipoVector::const_iterator i = lista.begin();
				typename TipoVector::const_iterator j = --lista.end();

				for ( typename TipoVector::size_type k = 0; k < lista.size(); ++k ){
					if ( *this == (*i) ) {
						return true;
					}
					j = i++;

					const TComp& xmin = std::min((*i).x, (*j).x);
					const TComp& xmax = std::max((*i).x, (*j).x);
					const TComp& ymin = std::min((*i).y, (*j).y);
					const TComp& ymax = std::max((*i).y, (*j).y);

					if (x <= xmax && y <= ymax && y >= ymin) {
						if (x <= xmin) {
							cruces++;
						}
						else {
							// Ecuacion de la linea
							const TComp& dx = (*j).x - (*i).x;

							if (std::abs(dx) > EpsilonDeComponentes()) {
								const Vector& pi = InterseccionEntreRectas (*this, *this + Vector(1.0f, 0.0f), (*i), (*j));
								if ( pi.x > x){
									cruces++;
								}
							}
						}
					}
				}
				return (cruces & 1) != 0;
			}

			/* Otra implementacion distinta que devuelve true si el punto esta contenido dentro del poligono especificado.
			 Metodo del teorema de las curvas de Jordan para poligonos cerrados concavos o convexos.
			 Contempla los casos:
				* punto coincidente con un vértice.
			    * vertices consecutivos de poligono coincidentes.
			 */
			template <class TipoVector>
			inline bool DentroDePoligono2(const TipoVector* const v, const int& numVertices) const
			{
				int i, j;
				bool t = false;
				for (i = 0, j = numVertices-1; i < numVertices; j = i++) {
					if ( ( (v[i].y > y) != (v[j].y > y) ) && (x < (v[j]. x - v[i].x) * (y - v[i].y) / (v[j].y - v[i].y) + v[i].x) )
						t = !t;
				}
				return t;
			}

			template <class TipoVector>
			inline bool DentroDePoligono2(const std::list<TipoVector>& lista) const
			{
				typename std::list<TipoVector>::const_iterator i = lista.begin();
				typename std::list<TipoVector>::const_iterator j = --lista.end();
				bool t = false;

				if (i != j) {
					for ( ; i != lista.end(); j = i++) {
						if ( ( ( (*i).y > y) != ((*j).y > y) ) && (x < ((*j). x - (*i).x) * (y - (*i).y) / ((*j).y - (*i).y) + (*i).x) ) {
							t = !t;
						}
					}
				}
				return t;
			}
			
			// Calcula el area de un poligono simple por el metodo de los determinantes
			template <class TipoVector>
			static TReal AreaDePoligono(const TipoVector* const v, const int& numVertices)
			{
				TReal area = 0.0f;
				const TReal fact = 0.5f;
				if (numVertices > 2) {
					for (int i = 1, j = 2; j < numVertices; i++, j++) {
						area += ( fact * (
											( (TReal)(v[0].x) * (TReal)(v[i].y) ) +
											( (TReal)(v[i].x) * (TReal)(v[j].y) ) +
											( (TReal)(v[j].x) * (TReal)(v[0].y) ) -
											( (TReal)(v[0].y) * (TReal)(v[i].x) ) -
											( (TReal)(v[i].y) * (TReal)(v[j].x) ) -
											( (TReal)(v[j].y) * (TReal)(v[0].x) )
										  )
								 );
							
					}
				}
				return std::abs(area);
			}
			
			// Calcula el area de un poligono simple por el metodo de los determinantes
			template <class TipoVector>
			static TReal AreaDePoligono(const std::list<TipoVector>& lista)
			{
				TReal area = 0.0f;
				const TReal fact = 0.5f;

				if (lista.size() > 2) {
					typename std::list<TipoVector>::const_iterator v0 = lista.begin();
					typename std::list<TipoVector>::const_iterator v1 = v0; v1++;
					typename std::list<TipoVector>::const_iterator v2 = v1; v2++;
					for (; v2 != lista.end(); v1++, v2++) {
						area += ( fact * (
										  ( (TReal)((*v0).x) * (TReal)((*v1).y) ) +
										  ( (TReal)((*v1).x) * (TReal)((*v2).y) ) +
										  ( (TReal)((*v2).x) * (TReal)((*v0).y) ) -
										  ( (TReal)((*v0).y) * (TReal)((*v1).x) ) -
										  ( (TReal)((*v1).y) * (TReal)((*v2).x) ) -
										  ( (TReal)((*v2).y) * (TReal)((*v0).x) )
										  )
								 );
						
					}
				}
				return std::abs(area);
			}
			
			// Determina si el poligono es simple (sus aristas no intersectan entre ellas)
			template <class TipoVector>
			static bool EsPoligonoSimple(const std::list<TipoVector>& lista)
			{
				bool simple = true;
				
				if (lista.size() > 3) {
					typename std::list<TipoVector>::const_iterator i = lista.begin();
					typename std::list<TipoVector>::const_iterator j = i; j++;

					typename std::list<TipoVector>::const_iterator k;
					typename std::list<TipoVector>::const_iterator l;

					TipoVector inter;
					TipoVector bbox[2][2];
					
					const unsigned long nl = lista.size() - 1;
					unsigned long ni = 0;

					for (ni = 0; simple && ni < nl; ni++, i++, j++) {
						(*i).IniciarBoundingBox(bbox[0][0],bbox[0][1]);
						(*j).ExtenderBoundingBox(bbox[0][0],bbox[0][1]);

						k = j;
						
						while ( simple && (++k) != lista.end()) {
							l = k;
							l++;
							
							if (l == lista.end()) {
								l = lista.begin();
								if (l == i) {
									break;
								}
							}
							
							(*k).IniciarBoundingBox(bbox[1][0], bbox[1][1]);
							(*l).ExtenderBoundingBox(bbox[1][0], bbox[1][1]);

							TipoVector::IntersectarBoundingBox(bbox[0], bbox[1], bbox[1]);
							
							if ( BoundingBoxValido(bbox[1]) ) {
								
								inter = TipoVector::InterseccionEntreRectas(*i, *j, *k, *l);

								if ( (inter.x + 1e-6) >= bbox[1][0].x && (inter.x - 1e-6) <= bbox[1][1].x && (inter.y + 1e-6) >= bbox[1][0].y && (inter.y - 1e-6) <= bbox[1][1].y ) {
									simple = false;
								}
							}
						}
					}
				}
				return simple;
			}
			
			// Determina si el poligono es simple (sus aristas no intersectan entre ellas)
			template <class TipoVector>
			static bool EsPoligonoSimple(const TipoVector* const v, const int numVertices)
			{
				bool simple = true;
				
				if (numVertices > 3) {
					int i = 0;
					int j = 1;

					int k = 0;
					int l = 0;

					TipoVector inter;
					TipoVector bbox[2][2];
					
					const unsigned long nl = numVertices - 1;
					unsigned long ni = 0;

					for (ni = 0; simple && ni < nl; ni++, i++, j++) {
						v[i].IniciarBoundingBox(bbox[0][0],bbox[0][1]);
						v[j].ExtenderBoundingBox(bbox[0][0],bbox[0][1]);

						k = j;
						
						while ( simple && (++k) != numVertices) {
							l = k + 1;
							
							if (l == numVertices) {
								l = 0;
								if (l == i) {
									break;
								}
							}
							
							v[k].IniciarBoundingBox(bbox[1][0], bbox[1][1]);
							v[l].ExtenderBoundingBox(bbox[1][0], bbox[1][1]);

							TipoVector::IntersectarBoundingBox(bbox[0], bbox[1], bbox[1]);
							
							if ( TipoVector::BoundingBoxValido(bbox[1]) ) {
								
								inter = TipoVector::InterseccionEntreRectas(v[i], v[j], v[k], v[l]);

								if ( (inter.x + 1e-6) >= bbox[1][0].x && (inter.x - 1e-6) <= bbox[1][1].x && (inter.y + 1e-6) >= bbox[1][0].y && (inter.y - 1e-6) <= bbox[1][1].y ) {
									simple = false;
								}
							}
						}
					}
				}
				return simple;
			}
			
			// Calcula y devuelve el centroide de una colección de puntos.
			template <class TipoVector>
			static Vector Centroide(const TipoVector v[], const int& numVertices)
			{
				Vector vSum;

				if (numVertices > 0) {
					for (int i = 0; i < numVertices; ++i) {
						vSum += v[i];
					}
					vSum /= (TComp)numVertices;
				}
				return vSum;
			}

			// Calcula y devuelve el centroide de una coleccion de puntos.
			template <class TipoVector>
			static Vector Centroide(const std::list<TipoVector>& lista)
			{
				typename std::list<TipoVector>::const_iterator i = lista.begin();
				Vector vSum;

				if (i != lista.end()) {
					for ( i = lista.begin(); i != lista.end(); i++) {
						vSum += (*i);
					}
					vSum /= (TComp)lista.size();
				}
				return vSum;
			}

			// Calcula y devuelve el perimetro del poligono
			template <class TipoVector>
			static TReal PerimetroDePoligono(const TipoVector v[], int numVertices)
			{
				int i, j;
				TReal perimetro = 0.0f;
				for (i = 0, j = numVertices-1; i < numVertices; j = i++) {
					perimetro += (v[i] - v[j]).Norma2();
				}
				return perimetro;
			}

			// Calcula y devuelve el perimetro del poligono
			template <class TipoVector>
			static TReal PerimetroDePoligono(const std::list<TipoVector>& lista)
			{
				TReal perimetro = 0.0f;
				if (lista.size() > 0)
				{
					typename std::list<TipoVector>::const_iterator i = lista.begin();
					typename std::list<TipoVector>::const_iterator j = --lista.end();


					for ( ; i != lista.end(); j = i++) {
						perimetro += ( (*j) - (*i) ).Norma2();
					}
				}
				return perimetro;
			}

			inline bool DentroDeBoundingBox(const Vector& p0, const Vector& p1) const {
				bool vx = false;
				bool vy = false;
				if (p0.x < p1.x) {
					vx = (p0.x < x && x < p1.x);
				}
				else {
					vx = (p1.x < x && x < p0.x);
				}

				if (p0.y < p1.y) {
					vy = (p0.y < y && y < p1.y);
				}
				else {
					vy = (p1.y < y && y < p0.y);
				}

				return vx && vy;
			}

			inline bool DentroDeBoundingBoxCerrado(const Vector& p0, const Vector& p1) const {
				bool vx = false;
				bool vy = false;
				if (p0.x < p1.x) {
					vx = (p0.x <= x && x <= p1.x);
				}
				else {
					vx = (p1.x <= x && x <= p0.x);
				}

				if (p0.y < p1.y) {
					vy = (p0.y <= y && y <= p1.y);
				}
				else {
					vy = (p1.y <= y && y <= p0.y);
				}

				return vx && vy;
			}

			// Inicia un bounding box que contiene al punto
			inline void IniciarBoundingBox(Vector bBox[2]) const {
				IniciarBoundingBox(bBox[0], bBox[1]);
			}

			// Inicia un bounding box que contiene al punto
			inline void IniciarBoundingBox(Vector& p0, Vector& p1) const {
				p0 = p1 = *this;
			}

			// Extiende el bounding box especificado como parámetro para contener al punto
			inline void ExtenderBoundingBox(Vector bBox[2]) const {
				ExtenderBoundingBox(bBox[0], bBox[1]);
			}

			// Extiende el bounding box especificado como parámetro para contener al punto
			inline void ExtenderBoundingBox(Vector& p0, Vector& p1) const {
				if (p0.x <= p1.x) {
					if (x < p0.x) {
						p0.x = x;
					}
					else if (x > p1.x) {
						p1.x = x;
					}
				}
				else {
					if (x < p1.x) {
						p1.x = x;
					}
					else if (x > p0.x) {
						p0.x = x;
					}
				}
				if (p0.y <= p1.y) {
					if (y < p0.y) {
						p0.y = y;
					}
					else if (y > p1.y) {
						p1.y = y;
					}
				}
				else {
					if (y < p1.y) {
						p1.y = y;
					}
					else if (y > p0.y) {
						p0.y = y;
					}
				}
			}

			// Ordena el bounding box de manera que p0.x <= p1.x y p0.y <= p1.y
			static void OrdenarBoundingBox(Vector bBox[2]) {
				Vector::OrdenarBoundingBox(bBox[0], bBox[1]);
			}

			// Ordena el bounding box de manera que p0.x <= p1.x y p0.y <= p1.y
			static void OrdenarBoundingBox(Vector& p0, Vector& p1) {
				const Vector temp (std::min(p0.x, p1.x), std::min(p0.y, p1.y));
				p1.Asignar (std::max(p0.x, p1.x), std::max(p0.y, p1.y));
				p0 = temp;
			}

			template <class TipoVector> static void IntersectarBoundingBox(const TipoVector* a, const TipoVector* b, TipoVector* res) {
				const TipoVector a0 (std::min<TComp>(a[0].x, a[1].x), std::min<TComp>(a[0].y, a[1].y));
				const TipoVector a1 (std::max<TComp>(a[0].x, a[1].x), std::max<TComp>(a[0].y, a[1].y));
				const TipoVector b0 (std::min<TComp>(b[0].x, b[1].x), std::min<TComp>(b[0].y, b[1].y));
				const TipoVector b1 (std::max<TComp>(b[0].x, b[1].x), std::max<TComp>(b[0].y, b[1].y));

				res[0].Asignar(std::max<TComp>(a[0].x, b[0].x), std::max<TComp>(a[0].y, b[0].y));
				res[1].Asignar(std::min<TComp>(a[1].x, b[1].x), std::min<TComp>(a[1].y, b[1].y));
				if (res[0].x > res[1].x || res[0].y > res[1].y) {
					res[0].Asignar(TipoVector::NaNDeComponentes(), TipoVector::NaNDeComponentes());
					res[1].Asignar(TipoVector::NaNDeComponentes(), TipoVector::NaNDeComponentes());
				}
			}

			template <class TipoVector> static bool BoundingBoxValido(const TipoVector* bb) {
				return ( !(bb[0].EsInvalido()) && !(bb[1].EsInvalido()) );
			}

			// Intercambia los valores de p0 a p1 y viceversa
			static void IntercambiarValores(Vector& p0, Vector& p1) {
				const Vector& temp = p0;
				p0 = p1;
				p1 = temp;
			}

			inline Vector& operator=(const Vector& otro) {
				x = (TComp) otro.x;
				y = (TComp) otro.y;
				return *this;
			}

			inline Vector& operator=(const TComp* const v) {
				x = (TComp) v[0];
				y = (TComp) v[1];
				return *this;
			}

			inline bool operator==(const Vector& otro) const {
				#if defined EXACT_COMPARION
				return ( x == (TComp) otro.x ) && ( y == (TComp) otro.y );
				#else
				return this->NearlyEqual(otro);
				#endif
				
			}

			inline bool NearlyEqual(const Vector& otro) const {
				return (*this - otro).ValorAbsoluto() < Vector(EpsilonDeComponentes(), EpsilonDeComponentes());
			}

			inline bool NearlyNotEqual(const Vector& otro) const {
				return !NearlyEqual(otro);
			}

			inline bool operator<(const Vector& otro) const {
				return ( (x < otro.x) && (y < otro.y));
			}
			inline bool operator<=(const Vector& otro) const {
				return (*this < otro) || this->NearlyEqual(otro);
			}
			inline bool operator>(const Vector& otro) const {
				return ( (x > otro.x) && (y > otro.y));
			}
			inline bool operator>=(const Vector& otro) const {
				return (*this > otro) || this->NearlyEqual(otro);
			}

			inline bool operator!=(const Vector& otro) const {
				return !(*this == otro);
			}

			inline Vector operator*(const Vector& otro) const {
				return Vector(*this) *= otro;
			}

			inline Vector& operator*=(const Vector& otro) {
				x *= otro.x;
				y *= otro.y;
				return *this;
			}

			inline Vector operator*(const TComp& valor) const {
				return Vector(*this) *= (TComp) valor;
			}

			inline Vector& operator*=(const TComp& valor) {
				x *= (TComp) valor;
				y *= (TComp) valor;
				return *this;
			}

			inline Vector operator/(const Vector& otro) const {
				return Vector(*this) /= otro;
			}

			inline Vector& operator/=(const Vector v) {
				x /= v.x;
				y /= v.y;
				return *this;
			}

			inline Vector operator/(const TComp& v) const {
				return Vector(*this) /= v;
			}

			inline Vector& operator/=(const TComp& v) {
				x /= v;
				y /= v;
				return *this;
			}

			inline Vector operator-(const Vector& otro) const {
				return Vector(*this) -= otro;
			}

			inline Vector& operator-=(const Vector& otro) {
				x -= otro.x;
				y -= otro.y;
				return *this;
			}

			inline Vector operator-(const TComp& valor) const {
				return Vector(*this) -= valor;
			}

			/** Unary minus **/
			inline Vector operator-() const {
				return Vector(-x, -y);
			}

			inline Vector& operator-=(const TComp& valor) {
				x -= valor;
				y -= valor;
				return *this;
			}

			inline Vector operator+(const Vector& otro) const {
				return Vector(*this) += otro;
			}

			inline Vector& operator+=(const Vector& otro) {
				x += otro.x;
				y += otro.y;
				return *this;
			}

			inline Vector operator+(const TComp& valor) const {
				return Vector(*this) += valor;
			}

			inline Vector& operator+=(const TComp& valor) {
				x += (TComp) valor;
				y += (TComp) valor;
				return *this;
			}

			inline operator TComp *() {
				return v;
			}

			inline operator const TComp *() const {
				return v;
			}

			//----------------------------------------------
			//-- Accesores
			//----------------------------------------------

			inline TComp& operator [] (int i) {
				return v[i];
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Vector& c) {
				out << "[ x = " << c.x << ", y = " << c.y << " ]";
				return out;
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Vector* const c) {
				if (c == NULL) {
					out << "[ NULL ]";
				}
				else {
					out << *c;
				}
				return out;
			}
		};

		inline GNC::GCS::Vector operator+(int val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val + o.x, (GNC::GCS::Vector::TComp)val + o.y );
		}

		inline GNC::GCS::Vector operator+(float val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val + o.x, (GNC::GCS::Vector::TComp)val + o.y );
		}

		inline GNC::GCS::Vector operator+(double val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val + o.x, (GNC::GCS::Vector::TComp)val + o.y );
		}

		inline GNC::GCS::Vector operator-(int val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val - o.x, (GNC::GCS::Vector::TComp)val - o.y );
		}

		inline GNC::GCS::Vector operator-(float val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val - o.x, (GNC::GCS::Vector::TComp)val - o.y );
		}

		inline GNC::GCS::Vector operator-(double val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val - o.x, (GNC::GCS::Vector::TComp)val - o.y );
		}

		inline GNC::GCS::Vector operator*(int val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val * o.x, (GNC::GCS::Vector::TComp)val * o.y );
		}

		inline GNC::GCS::Vector operator*(float val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val * o.x, (GNC::GCS::Vector::TComp)val * o.y );
		}

		inline GNC::GCS::Vector operator*(double val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val * o.x, (GNC::GCS::Vector::TComp)val * o.y );
		}

		inline GNC::GCS::Vector operator/(int val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val / o.x, (GNC::GCS::Vector::TComp)val / o.y );
		}

		inline GNC::GCS::Vector operator/(float val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val / o.x, (GNC::GCS::Vector::TComp)val / o.y );
		}

		inline GNC::GCS::Vector operator/(double val, const Vector& o) {
			return GNC::GCS::Vector( (GNC::GCS::Vector::TComp)val / o.x, (GNC::GCS::Vector::TComp)val / o.y );
		}

		class  Matriz {

		public:
			typedef PRECISION_COMPONENTE_POR_DEFECTO TComp;
			typedef PRECISION_REALES_POR_DEFECTO     TReal;
			typedef Vector TVector;
			typedef Matriz TMatriz;

			TReal a00;
			TReal a01;
			TReal a10;
			TReal a11;

			inline  Matriz() {
				a00 = a01 = a10 = a11 = (TReal) 0.0f;
			}

			inline Matriz(const TComp& a00, const TComp& a01, const TComp& a10, const TComp& a11) {
				this->a00 = (TReal) a00;
				this->a01 = (TReal) a01;
				this->a10 = (TReal) a10;
				this->a11 = (TReal) a11;
			}

			inline Matriz(const Matriz& otra) {
				a00 = otra.a00;
				a01 = otra.a01;
				a10 = otra.a10;
				a11 = otra.a11;
			}

			inline static Matriz MatrizRotacion(const TComp& angulo) {
				return Matriz(std::cos(angulo), -std::sin(angulo), std::sin(angulo), std::cos(angulo));
			}

			inline Matriz ProductoMatricial(const Matriz& otra) const {
				return TMatriz(
							  a00 * otra.a00 + a01 * otra.a10,
							  a00 * otra.a01 + a01 * otra.a11,
							  a10 * otra.a00 + a11 * otra.a10,
							  a10 * otra.a01 + a11 * otra.a11);
			}

			inline TVector ProductoMatricial(const TVector& otro) const {
				return Vector(
							   (TVector::TComp)( a00 * (TReal) otro.x + a01 * (TReal) otro.y ),
							   (TVector::TComp)( a10 * (TReal) otro.x + a11 * (TReal) otro.y ) );
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Matriz& m) {
				out << "[ a00 = " << m.a00 << ", a01 = " << m.a01 << "; " << std::endl;
				out << "  a10 = " << m.a10 << ", a11 = " << m.a11 << " ]";
				return out;
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Matriz* m) {
				if (m == NULL) {
					out << "[ NULL ]";
				}
				else {
					out << *m;
				}
				return out;
			}
		};

		/* Devuelve un nuevo vector rotado el vector sobre el eje de coordenadas Z */
		inline Vector Vector::RotacionSobreZ(const TReal& angulo) const
		{
			return  Matriz::MatrizRotacion(angulo).ProductoMatricial(*this);
		}

		inline Vector Vector::RotacionSobreZ(const Vector& centro, const TReal& angulo) const
		{
			return centro +  Matriz::MatrizRotacion(angulo).ProductoMatricial(*this - centro);
		}		
	}
}
#else

#endif
#endif
