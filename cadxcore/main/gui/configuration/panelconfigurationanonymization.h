/*
 *  
 *  $Id: panelconfigurationanonymization.h $
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
#include <api/iconfiguracion.h>
#include "panelesconfiguracionginkgobase.h"
#include <string>
#include <vector>

namespace Ubicaciones {
	class UbicacionesGridTable;
}

namespace GNC {
	namespace GUI {

		class PanelConfigurationAnonymization: public PanelConfigurationAnonymizationBase, public IPasoConfiguracion
		{
			public:
				PanelConfigurationAnonymization(wxWindow* pParent,IDialogoConfiguracion* pDialogo);
				~PanelConfigurationAnonymization();

				void Recargar();

				//region "Metodos de IPasoConfiguracion"
				wxWindow* GetPanel();

				std::string GetTitle();

				std::string GetCabecera();

				bool Validar();

				bool Guardar();
				//endregion

				//region "Helpers"
				void AutoSizeGrid();
				//endregion

			public:
		};
	}
}
