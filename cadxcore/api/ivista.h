#pragma once
/*
 *  
 *  $Id: ivista.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IVIEW_H
#define IVIEW_H

#include <api/api.h>
#include <api/globals.h>
#include <api/ilock.h>
#include <string>
#include <vector>
#include <api/autoptr.h>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations

class wxWindow;
class vtkAlgorithmOutput;
class vtkImageData;

namespace GNC{
	namespace GCS {
		class IModuleController;
		class IStudyContext;
		class ITool;
		class IComando;
		class IToolController;
	}
}

namespace GIL {
	class IModeloIntegracion;
}
//endregion


namespace GNC {
	namespace GCS {

//----------------------------------------------------------------------------------------------------
//region Excepcion de tipo vista
		class EXTAPI VistaException : public GNC::GCS::IException {
		public:
			VistaException(const std::string& msg, const std::string& component="Modelo", bool fatal = true) throw();
		};
//endregion

//----------------------------------------------------------------------------------------------------
//region Interfaz de vistas

		class EXTAPI IVista : public GNC::GCS::ILockable {
		public:

			/* Constructor */
			IVista(const GNC::GCS::Ptr<GNC::GCS::IStudyContext>& estudio);

			/* Destructor */
			virtual ~IVista();

			virtual std::string& GetTitulo();

			virtual wxWindow* GetWindow() = 0;

			virtual bool SoportaHerramienta(GNC::GCS::ITool* pHerramienta);

			virtual bool SoportaGuardar() = 0;

			virtual bool SoportaExportar() = 0;

			/* Notificacion de cambio en la configuracion */
			virtual void OnConfiguracionCambiada();

			virtual void Activar();

			virtual void Desactivar();

			/* Obtiene el identificador unico del modulo al que pertenece */
			virtual const std::string& GetUIDModulo();

			/* Obtiene el modulo al que pertenece */
			virtual GNC::GCS::IModuleController* GetModulo();

			GNC::GCS::Ptr<GNC::GCS::IStudyContext>& GetEstudio();

			virtual bool EstaCargada();

			virtual bool EstaModificada();

			virtual void SetCargada(bool cargada = true);

			virtual bool Guardar();

			virtual const std::list<std::string> GetImagePaths() const;

			virtual void ActivarRuta(long file_pk) = 0;

			/**Gets dicom is currently shown**/
			virtual std::string GetCurrentDICOM();

			//----------------------------------------------------------------------------------------------------
			//region Interfaz de Carga

			// Comienzo de carga. Metodo sincrono con la interfaz.
			virtual void OnCargaIniciada() = 0;

			// Paso 1: Cargar el estudio. Metodo NO sincrono con la interfaz.
			virtual void CargarEstudio(GNC::GCS::IComando* pCmdInvocador) = 0;

			// Finalizacion de la carga. Metodo sincrono con la interfaz.
			virtual void OnCargaFinalizada() = 0;

			// Paso 2: Inicializacion del pipeline. Metodo sincrono con la interfaz.
			virtual void IniciarPipeline() = 0;

			// Parada del pipeline. Metodo sincrono con la interfaz Se invoca en el caso de que ocurra un error de carga.
			virtual void DetenerPipeline() = 0;

			virtual GNC::GCS::IToolController* GetToolController();

			//endregion

			virtual void OnFocus();

		private:
			// Estudio is a pointer to GNC::GCS::Ptr to avoid circular dependencies.
			GNC::GCS::Ptr<GNC::GCS::IStudyContext>* Estudio;

		protected:
			std::string                        m_Titulo;
			GNC::GCS::IToolController*         m_toolController;

			bool                               m_Activada;
			bool                               m_Cargada;
			bool                               m_Guardada;

		};
	}
}
#endif
