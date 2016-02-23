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
			virtual ~ControladorCarga();
			static ControladorCarga* m_psInstancia;
			static wxCriticalSection* m_pCriticalSection;
		};
	}
}
