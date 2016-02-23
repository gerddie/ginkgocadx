/*
 *  Ginkgo CADx Project
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
#include <api/ivista.h>
#include <visualizator/estudios/ecgstudy.h>
#include <api/controllers/ieventscontroller.h>
#include <api/controllers/ipacscontroller.h>
#include <api/controllers/icontroladorcarga.h> // TODO: Promover a IVista
#include <export/contracts/iviewcontract.h>

class wxWindow;
class wxVtkPageView;
namespace GNKVisualizator{
	class VisualizatorStudy;
	namespace GUI{
		class GWaveformView;
	}

	class WaveFormView : public GNC::GCS::IVista, public GNC::GCS::IViewContract
	{
	public:
		/* Constructor */
		WaveFormView(const GNC::GCS::Ptr<GNKVisualizator::ECGStudy>& estudio);

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

		/* Destructor */
		~WaveFormView();

		virtual bool SoportaGuardar();

		virtual bool SoportaExportar();

		virtual void Activar();
		virtual void ActivarRuta(long /*file_pk*/){}

		virtual wxWindow* GetWindow();

		GNC::GCS::Ptr<GNKVisualizator::ECGStudy>& GetECGStudy() {return VisualizatorStudy;}

		//iviewcontract
		virtual GNC::GCS::IVista* GetView();
	
	protected:
		GNKVisualizator::GUI::GWaveformView*             GVista;
		GNC::GCS::Ptr<GNKVisualizator::ECGStudy>  VisualizatorStudy;

		void GenerarTitulo();
	};
}

