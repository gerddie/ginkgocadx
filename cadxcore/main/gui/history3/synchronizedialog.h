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
#include "historypanel3base.h"
#include <utility>

//----------DIALOGO CONFIRMAR ELIMINAR...-------------
namespace GNC {
	namespace GCS {
		class IHistoryTool;
		class IModuleController;
		class HangingProtocol;
	}
	namespace GUI {
		class SynchronizeDialog: public SynchronizeDialogBase {
		public:
			SynchronizeDialog(wxWindow* parent, const std::list<long>& seriesPk, GNC::GCS::IHistoryTool* pTool);
			SynchronizeDialog(wxWindow* parent, const std::list<long>& seriesPk, GNC::GCS::IModuleController* pCtrlMod, int mode, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& pHangingProtocol);
			~SynchronizeDialog();
			virtual void OnSynchronize(wxCommandEvent &evt);

		protected:
			std::list<long> m_seriesPk;
			GNC::GCS::IHistoryTool* m_pTool;
			GNC::GCS::IModuleController* m_pCtrlMod;
			int m_mode;
			GNC::GCS::Ptr<GNC::GCS::HangingProtocol> m_pHangingProtocol;
			bool m_isToolMode;
			
		};
	}
}
