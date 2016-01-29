/*
 *  
 *  $Id: confirmdeletedialog.h $
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
#include <wx/ginkgostyle/ginkgostyle.h>

//----------DIALOGO CONFIRMAR ELIMINAR...-------------
namespace GNC {
	namespace GUI {
		class ConfirmDeleteDialog:public ConfirmDeleteDialogBase {
		public:
			ConfirmDeleteDialog(wxWindow* parent, const wxString& label):ConfirmDeleteDialogBase(parent){
				m_pLabelEliminar->SetLabel(m_pLabelEliminar->GetLabel() + label + _(" from History?"));
				m_pTitled->Layout();
				m_body->Layout();
				Layout();
			};

			~ConfirmDeleteDialog(){
			};

			void OnEliminarClick(wxCommandEvent &) {
				EndModal(wxOK);
			}

			void OnCancelarClick(wxCommandEvent &) {
				EndModal(wxCANCEL);
			}

		protected:
		};
	};
};
