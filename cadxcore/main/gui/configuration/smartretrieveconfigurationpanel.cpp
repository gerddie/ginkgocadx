/*
 *
 *  $Id: panelconfiguracionpermisos.cpp $
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
#include <vector>
#include <sstream>
#include <limits>

#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <api/internationalization/internationalization.h>
#include <main/controllers/smartretrievecontroller.h>
#include <main/controllers/storedqueriescontroller.h>
#include <wx/valtext.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/mstream.h>

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/log.h>
#include <wx/busyinfo.h>
#include <wx/xml/xml.h>
#include <wx/sstream.h>
#include <wx/log.h>
#include <wx/menu.h>

#include <wx/tokenzr.h>

#include "smartretrieveconfigurationpanel.h"

#include <sstream>


#define POD_COL_NAME_IDX     0
#define POD_COL_NAME_WIDTH   340
#define POD_COL_ACTIVE_IDX   1
#define POD_COL_ACTIVE_WIDTH 80

namespace GNC {
	namespace GUI {
		////////////////-------------------------------------------------------------------------------------/////////////////////////////

		SmartRetrieveConfigurationPanel::SmartRetrieveConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo): SmartRetrieveConfigurationPanelBase(pParent), IPasoConfiguracion(pDialogo)
		{
			m_pModel = new wxDataViewListStore();
			m_pListQueries->AssociateModel(m_pModel.get());

			m_pListQueries->AppendTextColumn(_("Name"), wxDATAVIEW_CELL_INERT, POD_COL_NAME_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );
			m_pListQueries->AppendToggleColumn(_("Active"),wxDATAVIEW_CELL_ACTIVATABLE, POD_COL_ACTIVE_WIDTH, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE );

			Recargar();

			m_pListQueries->Connect(wxEVT_DATAVIEW_ITEM_EDITING_DONE, wxDataViewEventHandler( SmartRetrieveConfigurationPanel::OnItemEdited), NULL, this );
			m_pListQueries->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( SmartRetrieveConfigurationPanel::OnListItemActivated ), NULL, this);
		}

		SmartRetrieveConfigurationPanel::~SmartRetrieveConfigurationPanel()
		{
		}

		void SmartRetrieveConfigurationPanel::Recargar()
		{
			m_pCheckEnable->SetValue(GNC::GCS::SmartRetrieveController::Instance()->isEnabled());
			
			GNC::GCS::StoredQueryController::TStoredQueriesList listOfQueries = GNC::GCS::StoredQueryController::Instance()->getStoredQueries();
			for (GNC::GCS::StoredQueryController::TStoredQueriesList::iterator it = listOfQueries.begin(); it != listOfQueries.end(); ++it)
			{
				wxVector<wxVariant> data;
				data.push_back(wxString::FromUTF8((*it)->getName().c_str()));
				data.push_back((*it)->isActive());
				m_pListQueries->AppendItem(data);
				
			}

			//refresh status
			Refresh();
		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* SmartRetrieveConfigurationPanel::GetPanel()
		{
			return this;
		}

		std::string SmartRetrieveConfigurationPanel::GetTitle()
		{
			return _Std("Smart retrieve");
		}

		std::string SmartRetrieveConfigurationPanel::GetCabecera()
		{
			return _Std("Smart retrieve settings");
		}

		bool SmartRetrieveConfigurationPanel::Validar()
		{
			bool ok = true;
			return ok;
		}

		bool SmartRetrieveConfigurationPanel::Guardar()
		{

			GNC::GCS::SmartRetrieveController::Instance()->enable(m_pCheckEnable->GetValue());
			//todo set status of queries...
			GNC::GCS::StoredQueryController::TStoredQueriesList queries = GNC::GCS::StoredQueryController::Instance()->getStoredQueries();
			for (int i = 0; i < m_pModel->GetCount(); ++i) {
				wxVariant value;
				m_pModel->GetValueByRow(value, i, POD_COL_ACTIVE_IDX);
				bool active = value.GetBool();
				m_pModel->GetValueByRow(value, i, POD_COL_NAME_IDX);
				std::string name(value.GetString().ToUTF8());

				for (GNC::GCS::StoredQueryController::TStoredQueriesList::iterator it = queries.begin(); it != queries.end(); ++it)
				{
					if ((*it)->getName() == name) {
						(*it)->setActive(active);
						break;
					}
				} 
			}
			GNC::GCS::StoredQueryController::Instance()->clearQueries();
			GNC::GCS::StoredQueryController::Instance()->storeQueries(queries);
			GNC::GCS::SmartRetrieveController::Instance()->resetStatus();
			return true;
		}
		//endregion
		
		void SmartRetrieveConfigurationPanel::OnItemEdited(wxDataViewEvent& /*event*/)
		{
			OnPropiedadCambiada();
		}

		void SmartRetrieveConfigurationPanel::OnCheckEnableBackgroundQueries(wxCommandEvent &/*evt*/)
		{
			OnPropiedadCambiada();
		}

		void SmartRetrieveConfigurationPanel::OnListItemActivated(wxDataViewEvent& event)
		{
			if (event.GetItem().IsOk()) {
				 int row = m_pModel->GetRow(event.GetItem());
				 m_pListQueries->SetToggleValue(!m_pListQueries->GetToggleValue(row, POD_COL_ACTIVE_IDX), row, POD_COL_ACTIVE_IDX);
			}
			OnPropiedadCambiada();
		}
	}
}
