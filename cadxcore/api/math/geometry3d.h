#pragma once
/*
 *  
 *  $Id: geometry3d.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef GEOMETRY3D_H
#define GEOMETRY3D_H

#if defined(max)
	#undef max
#endif
#if defined(min)
	#undef min
#endif

#include <list>
#include <iostream>
#include <cmath>
#include <limits>

#include <api/api.h>
#include <api/globals.h>
#include <api/math/geometry_defs.h>


#define GEOMETRY3D_NO_TEMPLATE

#if defined(GEOMETRY3D_NO_TEMPLATE)

namespace GNC {
	namespace GCS {

		class Matriz3x3;

		class Vector3D {
		public:
			typedef PRECISION_COMPONENTE_POR_DEFECTO TComp;
			typedef PRECISION_REALES_POR_DEFECTO     TReal;

			TComp v[4];         // Coordenadas

			TComp& x;           // Coordenada X
			TComp& y;           // Coordenada Y
			TComp& z;           // Coordenada Z
			TComp& t;           // Coordenada Z

		public:

			static const Vector3D NaN() {
				return Vector3D(NaNDeComponentes(), NaNDeComponentes(), NaNDeComponentes());
			}

			static const Vector3D Inf() {
				return Vector3D(InfinitoDeComponentes(), InfinitoDeComponentes(), InfinitoDeComponentes());
			}

			static const Vector3D Zero() {
				return Vector3D();
			}

			static const Vector3D Unidad() {
				return Vector3D(1.0, 1.0, 1.0);
			}

			//----------------------------------------------
			//-- Constructores y destructores
			//----------------------------------------------

			inline Vector3D() : x(v[0]), y(v[1]), z(v[2]), t(v[3]) {
				x = (TComp) 0.0f;
				y = (TComp) 0.0f;
				z = (TComp) 0.0f;
				t = (TComp) 1.0f;
			}

			/* Constructor copia transparente */
			inline Vector3D(const Vector3D& b) : x(v[0]), y(v[1]), z(v[2]), t(v[3]) {
				x = (TComp) b.x;
				y = (TComp) b.y;
				z = (TComp) b.z;
				t = (TComp) b.t;
			}

			template <class TipoComp1, class TipoComp2, class TipoComp3> inline Vector3D(const TipoComp1& valX, const TipoComp2& valY, const TipoComp3& valZ) : x(v[0]), y(v[1]), z(v[2]), t(v[3]) {
				x = (TComp) valX;
				y = (TComp) valY;
				z = (TComp) valZ;
				t = (TComp) 1.0;
			}

			template <class TipoComp> inline Vector3D(const TipoComp* const valor) : x(v[0]), y(v[1]), z(v[2]), t(v[3]) {
				x = (TComp) valor[0];
				y = (TComp) valor[1];
				z = (TComp) valor[2];
				t = (TComp) 1.0;
			}

			~Vector3D() {

			}

			//----------------------------------------------
			//-- Asignacion explicita
			//----------------------------------------------

			/* Asigna las coordenadas al vector */
			template <class TipoComp1, class TipoComp2, class TipoComp3> inline Vector3D& Asignar(const TipoComp1& valX, const TipoComp2& valY, const TipoComp3& valZ) {
				x = (TComp) valX;
				y = (TComp) valY;
				z = (TComp) valZ;
				t = (TComp) 1.0;
				return (*this);
			}

			/* Asigna el valor a todas las coordenadas del vector */
			template <class TipoComp> inline Vector3D& Asignar(const TipoComp& valor) {
				x = (TComp) valor;
				y = (TComp) valor;
				z = (TComp) valor;
				t = (TComp) 1.0;
				return *this;
			}

			//----------------------------------------------
			//-- Load and Store
			//----------------------------------------------

			/* Load from array (interpret ncomp components) */
			template <class TipoComp> inline void Load(int ncomp, TipoComp* mem) {
				for (int i = 0; i < 4; ++i) {
					if (i < ncomp) {
						v[i] = (TComp) mem[i];
					}
					else {
						if (v[i] == 3) {
							v[i] = (TComp)0.0;
						}
						else {
							v[i] = (TComp)1.0;
						}
					}
				}
			}

			/* Store to array (interpret ncomp components) */
			template <class TipoComp> inline void Store(int ncomp, TipoComp* mem) const {
				for (int i = 0; i < 4; ++i) {
					if (i < ncomp) {
						mem[i] = (TipoComp) v[i];
					}
				}
			}

			//----------------------------------------------
			//-- Normas Vectoriales, Distancias y Producto Interior
			//----------------------------------------------

			inline TComp Norma1() const {
				return std::abs(x) + std::abs(y) + std::abs(z);
			}

			/* Devuelve la norma 2 (euclidea) al cuadrado */
			inline TComp Norma2Cuadrado() const {
				return (TReal)x * (TReal)x + (TReal)y * (TReal)y + (TReal)z * (TReal)z;
			}

			/* Devuelve la norma 2 (euclidea) */
			inline TReal Norma2() const {
				return std::sqrt((TReal) (*this).Norma2Cuadrado());
			}

			inline TReal NormaEnesima(unsigned int n) const {
				return std::pow( std::pow((TReal)x, (TReal)n) + std::pow((TReal)y, (TReal)n) + std::pow((TReal)z, (TReal)n), (TReal)1.0f / (TReal)n);
			}

			inline TComp NormaInfinito() const {
				return std::max(x, std::max(y, z));
			}

			/* Devuelve la distancia euclidea a "b" */
			inline TReal DistanciaEuclidea(const Vector3D& b) const {
				const Vector3D& a = *this;
				return std::sqrt( (double) (b - a).Norma2Cuadrado() );
			}

			/* Devuelve el cuadrado de la distancia euclidea */
			inline TReal DistanciaEuclideaCuadrado(const Vector3D& b) const {
				const Vector3D& a = *this;
				return (b - a).Norma2Cuadrado();
			}

			/* Normaliza un vector teniendo en cuenta singularidades */
			inline Vector3D& Normalizar() {

				Vector3D& a = (*this);

				const TReal& norma = a.Norma2();

				if ( std::abs(norma) < EpsilonDeReales()) {
					a.Asignar( (TComp)0.0, (TComp)0.0f, (TComp)0.0);
				}
				else {
					a /= norma;
				}
				return a;
			}

			/* Devuelve el vector normalizado teniendo en cuenta singularidades */
			inline Vector3D Normalizado() const {
				return Vector3D(*this).Normalizar();
			}

			/* Devuelve el producto escalar por b */
			inline TReal ProductoEscalar(const Vector3D& b) const {
				const Vector3D& a = *this;
				return ((TReal)a.x * (TReal)b.x) + ((TReal)a.y * (TReal)b.y) + ((TReal)a.z * (TReal)b.z);
			}

			/* Devuelve el producto vectorial
				w =  u  x v = (u_y v_z - u_z v_y) i + (u_z v_x - u_x v_z) j + (u_x v_y - u_y v_x) k 
				|  i   j   k  |
				| u.x u.y u.z | = { (u.y*v.z - u.z*v.y), (u.z*v.x - u.x*v.z), (u.x*v.y - u.y*v.x) }
				| v.x v.y v.z |
			*/
			inline Vector3D ProductoVectorial(const Vector3D& v) const {
				const Vector3D& u = *this;
				return Vector3D(
					((TReal)u.y * (TReal)v.z) - ((TReal)u.z * (TReal)v.y),
					((TReal)u.z * (TReal)v.x) - ((TReal)u.x * (TReal)v.z),
					((TReal)u.x * (TReal)v.y) - ((TReal)u.y * (TReal)v.x)
					);
			}

			//----------------------------------------------
			//-- Angulos
			//----------------------------------------------

			/* Devuelve el angulo { entre [0, PI) } que forma con "b" */
			inline TReal Angulo(const Vector3D& b) const {
				const Vector3D& a = (*this);
				const TReal& angulo = std::acos( ((TReal) a.ProductoEscalar(b)) / (a.Norma2() * b.Norma2()) );
				return angulo;
			}


			//----------------------------------------------
			//-- Ajuste e interpolacion
			//----------------------------------------------



			//----------------------------------------------
			//-- Componentes
			//----------------------------------------------

			/* Devuelve la mayor de las componentes */
			inline const TComp& Mayor() const {
				return x > y ? (x > z ? x : z) : (y > z ? y : z);
			}

			/* Devuelve la menor de las componentes */
			inline const TComp& Menor() const {
				return x > y ? (y > z ? z : y) : (x > z ? z : x );
			}

			/* Returns the dominant component index (x==0, y==1, z==2) */
			inline int GetDominantIndex() const {
				return x > y ? (y > z ? 2 : 1) : (x > z ? 2 : 0 );
			}

			//----------------------------------------------
			//-- Constantes
			//----------------------------------------------

			static inline TComp InfinitoDeComponentes() {
				return std::numeric_limits<TComp>::infinity();
			}

			static inline TReal InfinitoDeReales() {
				return std::numeric_limits<TReal>::infinity();
			}

			static inline TComp NaNDeComponentes() {
				return std::numeric_limits<TComp>::quiet_NaN();
			}

			static inline TReal NaNDeReales() {
				return std::numeric_limits<TReal>::quiet_NaN();
			}

			static inline TComp EpsilonDeComponentes() {
				return std::numeric_limits<TComp>::epsilon();
			}

			static inline TReal EpsilonDeReales() {
				return std::numeric_limits<TReal>::epsilon();
			}

			//----------------------------------------------
			//-- Comprobaciones
			//----------------------------------------------

			inline bool EsNulo() const {
				#if defined(EXACT_COMPARISON)
				return x == 0.0 && y == 0.0 && z == 0.0;
				#else
				return ((*this) - Zero()).ValorAbsoluto().NearlyEqual(Zero());
				#endif
			}

			inline bool EsInvalido() const {
				return (x != x) || (y != y) || (z != z);
			}

			inline bool EsNaN() const {
				return (x != x) || (y != y) || (z != z);
			}

			//----------------------------------------------
			//-- Geometria
			//----------------------------------------------

			/** Devuelve el punto de interseccion entre una recta y un plano
			La recta
			**/

			static Vector3D InterseccionEntreRectaYPlano(const Vector3D& punto_recta, const Vector3D& vdir_recta, const Vector3D& centro_plano, const Vector3D& vnorm_plano)
			{
				TReal numerador = (centro_plano - punto_recta).ProductoEscalar(vnorm_plano);
				TReal denominador = vdir_recta.ProductoEscalar(vnorm_plano);
				TReal distancia = 0.0;
				if (std::abs(denominador) < EpsilonDeReales()) {
					if (std::abs(numerador) < EpsilonDeReales()) { // Colineales
						return punto_recta;
					}
					else {
						return Inf(); // Paralelos. Se cortan en el infinito
					}
				}
				else {
					distancia = numerador / denominador;
					return punto_recta + vdir_recta * distancia;
				}
			}

			/* Devuelve el punto medio entre dos vectores */
			inline Vector3D PuntoMedio(const Vector3D& b) const {
				const Vector3D& a = (*this);
				return (a + b) / 2.0f;
			}

			/* Devuelve el punto de la proyeccion ortogonal del punto sobre la recta definida por p0 y p1 */
			inline Vector3D ProyeccionOrtogonalSobreRecta(const Vector3D& p0, const Vector3D& p1) const {
				 typedef Vector3D V;
				 V vdir = p1 - p0;
				 V::TComp proj = ( (*this) - p0).ProductoEscalar(vdir);
				 return p0 + (vdir * proj);
			}

			/* Devuelve la distancia del punto a la recta definida por p0 y p1 */
			inline TReal DistanciaARecta(const Vector3D& p0, const Vector3D& p1) const {
				return (ProyeccionOrtogonalSobreRecta(p0, p1) - (*this)).Norma2();
			}

			/* Devuelve el cuadrado de la distancia del punto a la recta definida por p0 y p1 */
			inline TComp DistanciaARectaCuadrado(const Vector3D& p0, const Vector3D& p1) const {
				return (ProyeccionOrtogonalSobreRecta(p0, p1) - (*this)).Norma2Cuadrado();
			}

			//----------------------------------------------
			//-- Acotaciones
			//----------------------------------------------

			/** Return signs ( -1.0 if < -epsilon, 1.0 if > epsilon 0.0 otherwise) */
			inline Vector3D Sign() const {
				Vector3D signs;
				const TComp lower = -EpsilonDeComponentes();
				const TComp upper = EpsilonDeComponentes();
				for (int i = 0; i < 3; i++) {
					const TComp& val = v[i];
					TComp& sign = signs.v[i];
					if (val < lower) {
						sign = -1.0;
					}
					else if (val > upper) {
						sign = 1.0;
					}
					else {
						sign = 0.0;
					}
				}
				return signs;
			}

			/** Stabilize at 0.0 (ignore if desv from zero > eps) */
			inline Vector3D& Stabilize() {
				TComp trunc = EpsilonDeComponentes();
				for (int i = 0; i < 3; i++) {
					TComp& val = v[i];
					if (val > 0.0) {
						if (val < trunc) {
							val = 0.0;
						}
					}
					else if (val < 0.0) {
						if (std::abs(val) < trunc) {
							val = 0.0;
						}
					}
				}
				return *this;
			}

			/** Return a new vector stabilized at 0.0 (ignore if desv from zero > eps) */
			inline Vector3D Stabilized() const {
				return Vector3D(*this).Stabilize();
			}

			inline Vector3D ValorAbsoluto() {
				return Vector3D(std::abs(x), std::abs(y), std::abs(z));
			}

			inline Vector3D TruncadoAMaximo(const TComp& val) const
			{
				return Vector3D(*this).TruncarAMaximo(val);
			}

			inline Vector3D TruncadoAMinimo(const TComp& val) const
			{
				return Vector3D(*this).TruncarAMinimo(val);
			}

			inline Vector3D& TruncarAMaximo(const TComp& val)
			{
				x = std::max(x, val);
				y = std::max(y, val);
				z = std::max(z, val);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D& TruncarAMinimo(const TComp& val)
			{
				x = std::min(x, val);
				y = std::min(y, val);
				z = std::min(z, val);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D TruncadoAMaximo(TComp valx, TComp valy, TComp valz) const
			{
				return Vector3D(*this).TruncarAMaximo(valx, valy, valz);
			}

			inline Vector3D TruncadoAMinimo(TComp valx, TComp valy, TComp valz) const
			{
				return Vector3D(*this).TruncarAMinimo(valx, valy, valz);
			}

			inline Vector3D& TruncarAMaximo(TComp valx, TComp valy, TComp valz)
			{
				x = std::max(x, valx);
				y = std::max(y, valy);
				z = std::max(z, valz);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D& TruncarAMinimo(TComp valx, TComp valy, TComp valz)
			{
				x = std::min(x, valx);
				y = std::min(y, valy);
				z = std::min(z, valz);
				t = (TComp) 1.0;
				return *this;
			}

			inline TComp ComponenteMaxima() const
			{
				return std::max(x,std::max(y, z));
			}

			inline TComp ComponenteMinima() const
			{
				return std::min(x,std::min(y, z));
			}

			static inline Vector3D ComponentesMaximas(const Vector3D& a, const Vector3D& b)
			{
				return Vector3D(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
			}

			static inline Vector3D ComponentesMinimas(const Vector3D& a, const Vector3D& b)
			{
				return Vector3D(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
			}

			inline Vector3D& AsignarMaximos(const Vector3D& o)
			{
				x = std::max(x, o.x);
				y = std::max(y, o.y);
				z = std::max(x, o.z);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D& AsignarMinimos(const Vector3D& o)
			{
				x = std::min(x, o.x);
				y = std::min(y, o.y);
				z = std::min(z, o.z);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D Maximos(const Vector3D& o) const
			{
				return Vector3D(std::max(x, o.x), std::max(y, o.y), std::max(z, o.z));
			}

			inline const Vector3D Minimos(const Vector3D& o) const
			{
				return Vector3D(std::min(x, o.x), std::min(y, o.y), std::min(z, o.z));
			}

			inline bool TieneComponenteMayorQue(const Vector3D& o) const
			{
				return ( (x > o.x) || (y > o.y) || (z > o.z));
			}

			inline bool TieneComponenteMenorQue(const Vector3D& o) const
			{
				return ( (x < o.x) || (y < o.y)  || (z < o.z));
			}

			/* Devuelve verdadero si el vector es singular (distancia a (0,0) < epsilon =~ (0,0) ) */
			inline bool Singular() const {
				return std::abs(x) < EpsilonDeComponentes() || std::abs(y) < EpsilonDeComponentes() || std::abs(z) < EpsilonDeComponentes();
			}

			inline Vector3D RedondeoAlza() const {
				return Vector3D(*this).RedondearAlza();
			}

			inline Vector3D& RedondearAlza() {
				x = std::ceil(x);
				y = std::ceil(y);
				z = std::ceil(z);
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D RedondeoBaja() const {
				return Vector3D(*this).RedondearBaja();
			}

			inline Vector3D& RedondearBaja() {
				x = std::floor(x);
				y = std::floor(y);
				z = std::floor(z);
				t = (TComp) 1.0;
				return *this;
			}

			template <class TipoComp> static TipoComp ValorRedondeado(const TipoComp& val) {
				const TipoComp& d = std::floor(val);
				const TipoComp& u = std::ceil(val);
				if ( val - d < u - val) {
					return d;
				}
				else {
					return u;
				}
			}

			inline Vector3D Redondeado() const {
				return Vector3D(*this).Redondear();
			}

			inline Vector3D& Redondear() {
				x = ValorRedondeado(x);
				y = ValorRedondeado(y);
				z = ValorRedondeado(z);
				t = (TComp) 1.0;
				return *this;
			}

			// Calcula y devuelve el centroide de una coleccion de puntos.
			template <class TipoVector>
			static Vector3D Centroide(const TipoVector v[], const int& numVertices)
			{
				Vector3D vSum;

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
			static Vector3D Centroide(const std::list<TipoVector>& lista)
			{
				typename std::list<TipoVector>::const_iterator i = lista.begin();
				Vector3D vSum;

				if (i != lista.end()) {
					for ( i = lista.begin(); i != lista.end(); i++) {
						vSum += (*i);
					}
					vSum /= (TComp)lista.size();
				}
				return vSum;
			}


			// Intercambia los valores de p0 a p1 y viceversa
			template <class TipoComp, class TipoReal> static void IntercambiarValores(Vector3D& p0, Vector3D& p1) {
				const Vector3D temp(p0);
				p0 = p1;
				p1 = temp;
			}

			inline Vector3D& operator=(const Vector3D& otro) {
				v[0] = (TComp) otro.v[0];
				v[1] = (TComp) otro.v[1];
				v[2] = (TComp) otro.v[2];
				v[3] = (TComp) otro.v[3];
				return *this;
			}


			template <class TipoComp> inline Vector3D& operator=(const TipoComp* const vec) {
				v[0] = (TComp) vec[0];
				v[1] = (TComp) vec[1];
				v[2] = (TComp) vec[2];
				//v[3] = (TComp) vec[3];
				v[3] = 1.0;
				return *this;
			}

			inline bool operator==(const Vector3D& otro) const {				
				#if defined(EXACT_COMPARION)
				return ( x == (TComp) otro.x ) && ( y == (TComp) otro.y ) && ( z == (TComp) otro.z );
				#else
				return this->NearlyEqual(otro);
				#endif
			}

			inline bool NearlyEqual(const Vector3D& otro) const {
				return (*this - otro).ValorAbsoluto() < Vector3D(EpsilonDeComponentes(), EpsilonDeComponentes(), EpsilonDeComponentes());
			}

			inline bool NearlyNotEqual(const Vector3D& otro) const {
				return !NearlyEqual(otro);
			}

			inline bool operator!=(const Vector3D& otro) const {
				return !(*this == otro);
			}

			inline Vector3D operator*(const Vector3D& otro) const {
				return Vector3D(x * otro.x, y * otro.y, z * otro.z);
			}

			inline Vector3D& operator*=(const Vector3D& otro) {
				x *= otro.x;
				y *= otro.y;
				z *= otro.z;
				t = (TComp) 1.0;
				return *this;
			}

			template <class TipoComp> inline Vector3D operator*(const TipoComp& valor) const {
				return Vector3D(x * valor, y * valor, z * valor);
			}

			template <class TipoComp> inline Vector3D& operator*=(const TipoComp& valor) {
				x *= (TComp) valor;
				y *= (TComp) valor;
				z *= (TComp) valor;
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D operator/(const Vector3D& otro) const {
				return Vector3D(x / otro.x, y / otro.y, z / otro.z);
			}

			inline Vector3D& operator/=(const Vector3D v) {
				x /= v.x;
				y /= v.y;
				z /= v.z;
				t = (TComp) 1.0;
				return *this;
			}

			template <class TipoComp> inline Vector3D operator/(const TipoComp& v) const {
				return Vector3D(x / v, y / v, z / v);
			}

			template <class TipoComp> inline Vector3D& operator/=(const TipoComp& v) {
				x /= v;
				y /= v;
				z /= v;
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D operator-(const Vector3D& otro) const {
				return Vector3D(x - otro.x, y - otro.y, z - otro.z);
			}

			inline Vector3D& operator-=(const Vector3D& otro) {
				x -= otro.x;
				y -= otro.y;
				z -= otro.z;
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D operator-(const TComp& valor) const {
				return Vector3D(x - valor, y - valor, z - valor);
			}

			/** Unary minus **/
			inline Vector3D operator-() const {
				return Vector3D(-x, -y, -z);
			}

			inline Vector3D& operator-=(const TComp& valor) {
				x -= valor;
				y -= valor;
				z -= valor;
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D operator+(const Vector3D& otro) const {
				return Vector3D(x + otro.x, y + otro.y, z + otro.z);
			}

			inline Vector3D& operator+=(const Vector3D& otro) {
				x += otro.x;
				y += otro.y;
				z += otro.z;
				t = (TComp) 1.0;
				return *this;
			}

			inline Vector3D operator+(const TComp& valor) const {
				Vector3D t(*this);
				return t += valor;
			}

			inline Vector3D& operator+=(const TComp& valor) {
				x += (TComp) valor;
				y += (TComp) valor;
				z += (TComp) valor;
				t = (TComp) 1.0;
				return *this;
			}

			inline bool operator<(const Vector3D& otro) const {
				return ( (x < otro.x) && (y < otro.y) && (z < otro.z) );
			}
			inline bool operator<=(const Vector3D& otro) const {
				return ( (*this) < otro ) || this->NearlyEqual(otro);
			}
			inline bool operator>(const Vector3D& otro) const {
				return ( (x > otro.x) && (y > otro.y) && (z > otro.z) );
			}
			inline bool operator>=(const Vector3D& otro) const {
				return ( (*this) < otro ) || this->NearlyEqual(otro);
			}

			inline operator TComp *() {
				return v;
			}

			inline operator const TComp *() const {
				return v;
			}

			//----------------------------------------------
			//-- Utils
			//----------------------------------------------

			inline bool InsideOpenedBoundingBox(const Vector3D& p0, const Vector3D& p1) const {
				bool vx = false;
				bool vy = false;
				bool vz = false;
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

				if (p0.z < p1.z) {
					vz = (p0.z < z && z < p1.z);
				}
				else {
					vz = (p1.z < z && z < p0.z);
				}

				return vx && vy && vz;
			}

			inline bool InsideClosedBoundingBox(const Vector3D& p0, const Vector3D& p1) const {
				bool vx = false;
				bool vy = false;
				bool vz = false;
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

				if (p0.z < p1.z) {
					vz = (p0.z <= z && z <= p1.z);
				}
				else {
					vz = (p1.z <= z && z <= p0.z);
				}

				return vx && vy && vz;
			}

			inline void InitBoundingBox(Vector3D& p0, Vector3D& p1) const {
				p0 = p1 = *this;
			}

			inline void InitBoundingBox(Vector3D bBox[2]) const {
				InitBoundingBox(bBox[0], bBox[1]);
			}

			inline void ExtendBoundingBox(Vector3D& p0, Vector3D& p1) const {
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
				if (p0.z <= p1.z) {
					if (z < p0.z) {
						p0.z = z;
					}
					else if (z > p1.z) {
						p1.z = z;
					}
				}
				else {
					if (z < p1.z) {
						p1.z = z;
					}
					else if (z > p0.z) {
						p0.z = z;
					}
				}
			}

			inline void ExtendBoundingBox(Vector3D bBox[2]) const {
				ExtendBoundingBox(bBox[0], bBox[1]);
			}

			//----------------------------------------------
			//-- Accesores
			//----------------------------------------------

			inline TComp& operator [] (int i) {
				return v[i];
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Vector3D& c) {
				out << "[ x = " << c.x << ", y = " << c.y << ", z = " << c.z << " ]";
				return out;
			}

			inline friend std::ostream& operator<<(std::ostream& out, const Vector3D* const c) {
				if (c == NULL) {
					out << "[ NULL ]";
				}
				else {
					out << *c;
				}
				return out;
			}
		};

		template <class TipoComp> inline Vector3D operator+(const TipoComp val, const Vector3D& o) {
			return Vector3D( (Vector3D::TComp)val + o.x, (Vector3D::TComp)val + o.y, (Vector3D::TComp)val + o.z );
		}

		template <class TipoComp> inline Vector3D operator-(const TipoComp val, const Vector3D& o) {
			return Vector3D( (Vector3D::TComp)val - o.x, (Vector3D::TComp)val - o.y, (Vector3D::TComp)val - o.z );
		}

		template <class TipoComp> inline Vector3D operator*(const TipoComp val, const Vector3D& o) {
			return Vector3D( (Vector3D::TComp)val * o.x, (Vector3D::TComp)val * o.y, (Vector3D::TComp)val * o.z );
		}

		template <class TipoComp> inline Vector3D operator/(const TipoComp val, const Vector3D& o) {
			return Vector3D( (Vector3D::TComp)val / o.x, (Vector3D::TComp)val / o.y, (Vector3D::TComp)val / o.z );
		}

		class  Matriz3x3 {

		public:
			typedef PRECISION_COMPONENTE_POR_DEFECTO TComp;
			typedef PRECISION_REALES_POR_DEFECTO     TReal;

			TReal v[9];

			TReal& a00;
			TReal& a01;
			TReal& a02;
			TReal& a10;
			TReal& a11;
			TReal& a12;
			TReal& a20;
			TReal& a21;
			TReal& a22;

			inline  Matriz3x3() : a00(v[0]), a01(v[1]), a02(v[2]), a10(v[3]), a11(v[4]), a12(v[5]), a20(v[6]), a21(v[7]), a22(v[8])
			{
				for (int i = 0; i < 9; i++) {
					v[i] = 0.0;
				}

			}

			inline Matriz3x3(const Matriz3x3& otra) :  a00(v[0]), a01(v[1]), a02(v[2]), a10(v[3]), a11(v[4]), a12(v[5]), a20(v[6]), a21(v[7]), a22(v[8])
			{
				*this = otra;
			}

			inline Matriz3x3& operator = (const Matriz3x3& otra)
			{
				for (int i = 0; i < 9; i++) {
					v[i] = otra.v[i];
				}
				return *this;
			}

			inline friend std::ostream& operator<<(std::ostream& out, const  Matriz3x3& m) {
				out << "[ a00 = " << m.a00 << ", a01 = " << m.a01 << ", a02 = " << m.a02 << ";  " << std::endl;
				out << "[ a10 = " << m.a10 << ", a11 = " << m.a11 << ", a12 = " << m.a12 << ";  " << std::endl;
				out << "[ a20 = " << m.a20 << ", a21 = " << m.a21 << ", a22 = " << m.a22 << "]";
				return out;
			}

			inline friend std::ostream& operator<<(std::ostream& out, const  Matriz3x3* m) {
				if (m == NULL) {
					out << "[ NULL ]";
				}
				else {
					out << *m;
				}
				return out;
			}
		};
	}
}
#endif
#endif
