/*
 *  
 *  $Id: generaldata.h $
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
#include <api/iwizard.h>
#include <api/api.h>
#include <main/gui/import/pasosimportacionbase.h>
#include <string>
#include <vector>
#include <main/gui/import/importationdata.h>
#include <api/dicom/idicomizador.h>
#include <wx/ginkgostyle/ginkgostyle.h>

namespace GNC {
	namespace GCS {
		class IEntorno;
	}
}

namespace GNC {
	namespace GUI {

		class EXTAPI GeneralData: public PanelDatosGeneralesBase,public IPasoWizard
		{
			public:
				GeneralData(wxWindow* pParent,IWizard* pWizard,const GNC::GCS::Ptr<ImportationData>& importationData, bool seriesDescription);
				~GeneralData();

			//region "Metodos heredados de IPasoWizard"
				virtual bool Attach();

				virtual std::string GetTitle();

				virtual std::string GetSubTitle();

				virtual bool Siguiente();

				virtual bool Anterior();

				virtual bool Cancelar();

				virtual bool Validar();

				virtual bool Validar(bool silent);

				virtual bool IsSkipped();
			//endregion
			protected:
				virtual void GetPacienteNombre();
				virtual void GetPacienteIdentificador();
				virtual void GetPacienteFechaNacimiento();
				virtual void GetPacienteEdad();
				virtual void GetPacientePeso();
				virtual void GetPacienteAltura();
				virtual void GetPacienteSexo();

				void OnKillFocusEstudioTimeControl(wxFocusEvent &event);
				void EnableComponents(bool enable);

				virtual void GetEstudioFecha();
				virtual void GetEstudioTime();
				virtual void GetEstudioDescripcion();

				void OnKillFocusSerieTimeControl(wxFocusEvent &event);

				virtual void GetSerieFecha();
				virtual void GetSerieTime();
				virtual void GetAccno();
				virtual void GetSerieDescripcion();

				GNC::GCS::Ptr<ImportationData> m_pImportData;

				// Helpers: AutoEdad
				int CalcularEdad();
		};
	}
}
