/*
 *  
 *  $Id: paneltarea.h $
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
#include "progresobase.h"
namespace GNC {
	namespace GUI {
		
		class PanelTarea : public PanelTareaBase {
		public:
			PanelTarea( wxWindow* pPadre,long pidTarea);
			~PanelTarea();

			void SetProgress(const wxString& text, float progress);
			wxString GetStatus();
			float GetProgress();
		protected:
			void OnTareaCancelarClick(wxCommandEvent &event);
			long m_PIDTarea;
		};
	}
}
