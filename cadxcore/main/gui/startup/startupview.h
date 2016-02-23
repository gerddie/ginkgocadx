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

#include <api/globals.h>
#include <api/ivista.h>

#include <list>
#include <vector>
#include <map>
#include <api/istudycontext.h>
#include <export/contracts/iviewcontract.h>

namespace GNC {
	namespace GCS {
		class IEntorno;
	}
}

class wxWindow;

namespace GNC {
	namespace GUI {
		class StartUpForm;

		class StartUpStudy : public GNC::GCS::IStudyContext
		{
		public:
			
			StartUpStudy(){}
			~StartUpStudy(){}

			virtual void CallbackCargarTagsImagen(const int /*indice*/, GIL::DICOM::IDICOMManager* /*pDicomManager*/) {}

		protected:

		};

		class StartUpView  : public GNC::GCS::IVista, public GNC::GCS::IViewContract {
		public:
			/* Constructor */
			StartUpView (const GNC::GCS::Ptr<GNC::GUI::StartUpStudy>& study);

			/* Destructor */
			~StartUpView ();

			//suports save?
			virtual bool SoportaGuardar();

			//supports exportation?
			virtual bool SoportaExportar();

			//----------------------------------------------------------------------------------------------------
			//region Loading interface

			// Starts loading. Synchronous with interface.
			virtual void OnCargaIniciada();

			// Step1: LoadStudy not synchronized with interface thread
			virtual void CargarEstudio(GNC::GCS::IComando* pCmdInvocador);

			//Ends loading. Synchronous with interface
			virtual void OnCargaFinalizada();

			// Step2: init pipeline synchronous with interface
			virtual void IniciarPipeline();

			// stops pipeline, it's called if there has been an error loading study
			virtual void DetenerPipeline();

			//endregion

			//call when this view is activated
			virtual void Activar();

			virtual void ActivarRuta(long /*file_pk*/){}

			//view contract
			virtual GNC::GCS::IVista* GetView();

			//return graphic form
			virtual wxWindow* GetWindow();

			virtual wxSizer* GetBottomSizer();

		protected:

			void BuildTitle();

			GNC::GUI::StartUpForm* m_pWindow;
			GNC::GCS::Ptr<GNC::GUI::StartUpStudy>     FooStudy;
		};
	}
}
