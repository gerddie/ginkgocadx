/*
 *  
 *  $Id: vista2d.h $
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
#include <api/ivista.h>
#include <export/contracts/islidercontract.h>
#include <export/contracts/iviewcontract.h>
#include <api/controllers/ieventscontroller.h>
#include <api/controllers/ipacscontroller.h>
#include <api/controllers/icontroladorcarga.h> // TODO: Promote to IVista

class GVistaCompleja;
class wxWindow;
class wxVtkPageView;
namespace GNKVisualizator{
	class VisualizatorStudy;
	namespace GUI{
		class PanelResultados;
		class ContenedorHerramientas;
	}

	class Vista2D : public GNC::GCS::IVista, public GNC::GCS::IViewContract
	{
	public:
		/* Constructor */
		Vista2D(const GNC::GCS::Ptr<GNKVisualizator::VisualizatorStudy>& estudio);

		//----------------------------------------------------------------------------------------------------
		//region Interfaz de Carga

		// Comienzo de carga. Metodo sincrono con la interfaz.
		virtual void OnCargaIniciada();

		// Paso 1: Cargar los ficheros en memoria. Metodo NO sincrono con la interfaz.
		virtual void CargarEstudio(GNC::GCS::IComando* pCmdInvocador);

		// Finalizacion de la carga. Metodo sincrono con la interfaz.
		virtual void OnCargaFinalizada();

		// Paso 2: Inicializacion del pipeline. Metodo sincrono con la interfaz.
		virtual void IniciarPipeline();

		// Parada del pipeline. Metodo sincrono con la interfaz Se invoca en el caso de que ocurra un error de carga.
		virtual void DetenerPipeline();

		//endregion

		//region Interfaz IVista
		virtual bool SoportaHerramienta(GNC::GCS::ITool* h);
		//endregion

		/* Destructor */
		~Vista2D();

		virtual bool SoportaGuardar();

		virtual bool SoportaExportar();

		std::string GetDICOMTagOriginal(std::string tagId);

		std::string GetDICOMTagDiagnostico(std::string tagId);

		virtual void Activar();

		virtual void Desactivar();

		//ListaRutas GetPathDiagnosticos();
		//ListaRutas GetDicomsActivos();

		virtual wxWindow* GetWindow();

		void ActualizarImagen();

		virtual void OnConfiguracionCambiada();

		virtual void ActivarRuta(long file_pk);

		GIL::DICOM::DicomDataset  m_tagsActualizados;

		void ComienzaDestruccion();

		GNC::GCS::Ptr<GNKVisualizator::VisualizatorStudy>& GetVisualizatorStudy()
		{
			return VisualizatorStudy;
		}

		virtual void ActivarVista() {}

		virtual void DesactivarVista() {}
		
		virtual GNC::GCS::IVista* GetView();

		GVistaCompleja*             GVista;
		GNC::GCS::Ptr<GNKVisualizator::VisualizatorStudy>  VisualizatorStudy;
	protected:
		bool					         m_IgnorarModificaciones;

		void GenerarTitulo();
	};
}

