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
