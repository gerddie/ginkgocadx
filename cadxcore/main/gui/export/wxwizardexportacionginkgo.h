/*
 *  
 *  $Id: wxwizardexportacionginkgo.h $
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
#include <wx/wizardginkgo/wxwizardginkgobase.h>
#include <api/iwizard.h>
#include "datospersistentesexportacion.h"
#include <api/ientorno.h>
#include <string>
#include <vector>

namespace GNC {
	namespace GCS
	{
		class WidgetsContract;
	}
	namespace GUI {
		class wxWidzardExportacionGinkgo: public wxWizardGinkgoBase
		{

		public:
			wxWidzardExportacionGinkgo(wxWindow* pParent, GNC::GCS::IWidgetsContract* WidgetsContract);
			~wxWidzardExportacionGinkgo();

			virtual void OnCancelarClick(wxCommandEvent& event);
			virtual void OnSiguienteClick(wxCommandEvent& event);
			virtual void OnAnteriorClick(wxCommandEvent &event);

			void CargarCurrent();

			bool IsOK();


		protected:
			typedef std::list<IPasoWizard*> ListaPasos;
			ListaPasos m_ListaPasos;

			ListaPasos::size_type m_currentPasoIndex;
			bool            m_OK;

		};
	};
};
