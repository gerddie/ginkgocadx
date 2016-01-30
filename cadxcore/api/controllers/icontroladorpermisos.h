/*
 *  
 *  $Id: icontroladorpermisos.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <string>
#include <ostream>
#include <sstream>
#include <api/iexception.h>

namespace GNC {
	namespace GCS {
		class EXTAPI ControladorPermisosException : public GNC::GCS::IException {
			public:
				ControladorPermisosException(const std::string& msg, bool fatal = true) throw();
		};

		namespace Permisos {
			class EstadoPermiso {

			public:

				EstadoPermiso()
				{
					m_Activo = m_ActivoPorDefecto = false;
					m_PermisoValido = false;
				}

				EstadoPermiso(const EstadoPermiso* const o)
				{
					if (o != NULL) {
						*this = *o;
					}
					else {
						m_Activo = m_ActivoPorDefecto = true;
						m_PermisoValido = false;
					}
				}

				EstadoPermiso(const EstadoPermiso& o)
				{
					*this = o;
				}

				EstadoPermiso(const std::string& valorPorDefecto, bool activoPorDefecto) : m_ActivoPorDefecto(activoPorDefecto), m_ValorPorDefecto(valorPorDefecto)
				{
					m_PermisoValido = true;
					Reset();
				}

				EstadoPermiso(const std::string& valorPorDefecto, const std::string& valor, bool activoPorDefecto, bool activo) : m_Activo(activo), m_ActivoPorDefecto(activoPorDefecto), m_Valor(valor), m_ValorPorDefecto(valorPorDefecto)
				{
					m_PermisoValido = true;
				}

				void Reset() {
					m_Activo = m_ActivoPorDefecto;
					m_Valor  = m_ValorPorDefecto;
				}

				void Activar(bool activo)
				{
					m_Activo = activo;
				}

				bool Activo() const {
					return m_Activo;
				}

				bool TieneValorPorDefecto() const
				{
					return m_Valor == m_ValorPorDefecto && m_Activo == m_ActivoPorDefecto;
				}

				const std::string& GetRawValue() const {
					return m_Valor;
				}

				template <class T> T ObtenerValor() const {
					T v;
					if (!m_Activo) {
						std::istringstream is(m_ValorPorDefecto);
						is >> v;
					}
					else {
						std::istringstream is(m_Valor);
						is >> v;
					}
					
					return v;
				}

				template <class T> void AsignarValor(T valor) {
					std::ostringstream os;
					os << valor;
					m_Valor = os.str();
				}

				EstadoPermiso& operator = (const EstadoPermiso& o)
				{
					this->m_Valor = o.m_Valor;
					this->m_ValorPorDefecto = o.m_ValorPorDefecto;
					this->m_Activo = o.m_Activo;
					this->m_ActivoPorDefecto = o.m_ActivoPorDefecto;
					this->m_PermisoValido = o.m_PermisoValido;
					return *this;
				}

				bool ValidoYActivo() const
				{
					return m_PermisoValido && m_Activo;
				}

				operator bool () const {
					return ValidoYActivo();
				}

			private:
				bool        m_Activo;
				bool        m_ActivoPorDefecto;
				std::string m_Valor;
				std::string m_ValorPorDefecto;
				bool        m_PermisoValido;

			};
		}

		class EXTAPI IControladorPermisos
		{

		protected:
			IControladorPermisos();
			
			virtual ~IControladorPermisos();

		public:

			static IControladorPermisos* Instance();
			static void FreeInstance();

			/** Carga el fichero XML de permisos **/
			virtual void CargarXMLPermisos(const std::string& xmlPermisos, bool privados = false) = 0;

			/** Resetea el estado y valor de todos los permisos a sus valores por defecto **/
			virtual void ResetearValoresPorDefecto() = 0;

			/** Metodo recomendado para obtener permisos **/
			virtual GNC::GCS::Permisos::EstadoPermiso* ObtenerPermiso(const std::string& stdNamespace, const std::string& idSujeto) = 0;

			/** Metodo recomendado para comprobar permisos **/
			virtual GNC::GCS::Permisos::EstadoPermiso Get(const std::string& stdNamespace, const std::string& idSujeto) = 0;

			/** Anyade una nueva definicion de permiso al conjunto **/
			virtual void AddDefinicionPermiso(const std::string& espacioNombres, const std::string& descripcionNamespace, const std::string& id, const std::string& descripcion, bool privado, const std::string& valorPorDefecto, bool activoPorDefecto) = 0;
		};
	}
}
