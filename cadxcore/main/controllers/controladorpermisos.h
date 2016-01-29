/*
 *  
 *  $Id: controladorpermisos.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/api.h>
#include <api/controllers/icontroladorpermisos.h>
#include <map>
#include <string>

namespace GNC {
	class ControladorPermisos: public GNC::GCS::IControladorPermisos {
	public:

		typedef std::map<std::string, GNC::GCS::Permisos::EstadoPermiso> TipoMapaPermisos; //se indexan por id
		typedef TipoMapaPermisos::iterator TipoIteradorMapaPermisos;
		typedef std::map<std::string, TipoMapaPermisos*> TipoMapaPermisosNamespace; //se indexan por namespace
		typedef TipoMapaPermisosNamespace::iterator TipoIteradorMapaPermisosNamespace;
	
		typedef struct TipoDefinicionPermiso
		{
			std::string espacioNombres;
			std::string id;
			std::string descripcion;
			std::string descripcionNamespace;

			bool        activoPorDefecto;
			std::string valorPorDefecto;

			bool privado;

			TipoDefinicionPermiso ()
			{
				this->activoPorDefecto = true;
				this->privado = false;
			}

			TipoDefinicionPermiso (const std::string& espacioNombres, const std::string& descripcionNamespace, const std::string& id, const std::string& descripcion, bool privado, const std::string& valorPorDefecto, bool activoPorDefecto)
			{
				this->espacioNombres = espacioNombres;
				this->id = id;
				this->descripcion = descripcion;
				this->descripcionNamespace = descripcionNamespace;
				this->privado = privado;
				this->activoPorDefecto = activoPorDefecto;
				this->valorPorDefecto = valorPorDefecto;
			}

			GNC::GCS::Permisos::EstadoPermiso GenerarPermisoPorDefecto()
			{
				return GNC::GCS::Permisos::EstadoPermiso(valorPorDefecto, activoPorDefecto);
			}

		} TipoDefinicionPermiso;

		typedef std::map<std::string, TipoDefinicionPermiso> TipoMapaDefiniciones;

		static ControladorPermisos* Instance();
		static void FreeInstance();

		const TipoMapaDefiniciones& GetMapaDefiniciones();
		TipoMapaPermisosNamespace GetPermisosFromXML(const std::string& xmlPermisos);

	//region "Realizacion de la interfaz IControladorPermisos

		/** Carga el fichero XML de permisos **/
		virtual void CargarXMLPermisos(const std::string& xmlPermisos, bool privados = false);

		/** Resetea el estado y valor de todos los permisos a sus valores por defecto **/
		virtual void ResetearValoresPorDefecto();

		/** Metodo recomendado para obtener permisos **/
		virtual GNC::GCS::Permisos::EstadoPermiso* ObtenerPermiso(const std::string& stdNamespace, const std::string& idSujeto);

		/** Metodo recomendado para comprobar permisos **/
		virtual GNC::GCS::Permisos::EstadoPermiso Get(const std::string& stdNamespace, const std::string& idSujeto);

		/** Anyade una nueva definicion de permiso al conjunto **/
		virtual void AddDefinicionPermiso(const std::string& espacioNombres, const std::string& descripcionNamespace, const std::string& id, const std::string& descripcion, bool privado, const std::string& valorPorDefecto, bool activoPorDefecto);
	// endregion


	protected:
		ControladorPermisos();
		~ControladorPermisos();
		void LimpiarPermisos();
		TipoMapaPermisos* GetMapaPermisos(const std::string& stdNamespace);
		//bool GetPermitido(TipoMapaPermisos* pMapaPermisos, const std::string& idSujeto);

	protected:
		static ControladorPermisos* m_pInstance;

		TipoMapaPermisosNamespace   m_mapaPermisos;
		TipoMapaDefiniciones		m_mapaDefiniciones;

	};
}
