/*
 *  
 *  $Id: anonymizedialog.h $
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
#include "anonymizepanelbase.h"

#include <api/autoptr.h>
#include <api/dicom/idicom.h>
#include <api/internationalization/internationalization.h>

#include <main/gui/export/datospersistentesexportacion.h>

namespace GNC {
	namespace GUI {
		class AnonymizePanel;
		class AnonymizeDialog: public AnonymizeDialogBase
		{
		public:
			AnonymizeDialog(wxWindow* pParent, const std::list<long>& seriesPk);
			~AnonymizeDialog();


		protected:
			
			virtual void OnOverwriteClick( wxCommandEvent& event );
			virtual void OnCreateNewClick( wxCommandEvent& event );

			void LaunchCommand(bool overwrite);

			GNC::GCS::Ptr<TipoWizardExportacion> m_pDatosPersistentes;
			GNC::GUI::AnonymizePanel* m_pAnonymizePanel;
		};
	}
}
