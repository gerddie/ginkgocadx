/*
 *  
 *  $Id: synchronizedialog.h $
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
