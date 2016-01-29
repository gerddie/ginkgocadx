/*
 *  
 *  $Id: controladorcarga.h $
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
#include <api/controllers/icontroladorcarga.h>

class wxCriticalSection;
namespace GNC {
	namespace GCS {

		class EXTAPI ControladorCarga : public IControladorCarga {
		public:
			virtual vtkSmartPointer<vtkImageData> CargarITKMultidimensional(IComando* cmd, ListaRutas& listaFicheros, int* orientacion, double* spacing = NULL);
			virtual vtkSmartPointer<vtkImageData> CargarITK(IComando* cmd, std::string& path, int* orientacion, double* spacing = NULL);
			
			virtual void CargaAsincrona(GNC::GCS::IVista* pVista, const bool loadFirst = true);

			static ControladorCarga *Instance();
			static void FreeInstance();

			//region Creacion y destruccion de componente de carga en streaming
			virtual GNC::GCS::IStreamingLoader* NewLoader();
			virtual void FreeLoader(GNC::GCS::IStreamingLoader** loader);
			//endregion
		
		protected:
			vtkSmartPointer<vtkImageData> CargarITKMultidimensionalRGB(IComando* cmd, ListaRutas& listaFicheros, double* spacing = NULL);
			vtkSmartPointer<vtkImageData> CargarITKMultidimensionalUnsignedShort(IComando* cmd, ListaRutas& listaFicheros, int* orientacion,double* spacing = NULL);

			ControladorCarga();
			~ControladorCarga();
			static ControladorCarga* m_psInstancia;
			static wxCriticalSection* m_pCriticalSection;
		};
	}
}
