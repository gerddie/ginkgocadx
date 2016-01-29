#include <set>

#include "pacsuploaddialog.h"
#include <main/controllers/dcmtk/dicomservers.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/historycontroller.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include <main/controllers/anonusagestats.h>

#define SIZE_ICONOS 16

#define COLUMNA_DCMDIR_FECHA 2
#define COLUMNA_DCMDIR_MODALIDAD 1
#define COLUMNA_DCMDIR_MEDICO 3
#define COLUMNA_DCMDIR_ACCNUMBER 4
#define COLUMNA_DCMDIR_UID 5


namespace GNC {
	namespace GUI {
		PACSUploadDialog::PACSUploadDialog(wxWindow* pParent, const std::list<long>& seriesId, const std::list<long>& selectedSeriesId) : PACSUploadDialogBase(pParent)
		{
			LOG_STAT(ANON_STAT_DLG_PACS_UPLOAD)
			wxArrayString pacsList;
			DicomServerList::TServerList ListOfServers = DicomServerList::Instance()->GetServerList();
			for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it)
			{
				pacsList.push_back(wxString::FromAscii((*it)->ID.c_str()));
			}
			m_pServer->Append(pacsList);

			if(m_pServer->GetCount()>0){
				m_pServer->SetSelection(0);
				std::string pacsDef;
				GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/SubirPACS", "PACSDefecto", pacsDef);
				
				for (unsigned int i =0; i< m_pServer->GetCount(); ++i) {
					if (m_pServer->GetString(i) == wxString::FromUTF8(pacsDef.c_str())) {
						m_pServer->Select(i);
						break;
					}
				}
			} else {
				m_pServer->AppendString(_("There aren't any PACS configured"));
				m_pServer->SetSelection(0);
				m_pServer->Disable();
				m_pOkButton->Disable();
			}

			m_pPanelMessage->Layout();
			m_pBody->Layout();
			
			std::set<long> setOfSelected;
			setOfSelected.insert(selectedSeriesId.begin(), selectedSeriesId.end());
			
			for (std::list<long>::const_iterator itSeries = seriesId.begin(); itSeries != seriesId.end(); ++itSeries)
			{
				GNC::GCS::HistoryController::SeriesModel sm = GNC::GCS::HistoryController::Instance()->GetSeriesModel((*itSeries));
				std::ostringstream ostr;
				ostr << sm.series_desc << " - " << sm.series_modality << " (" << sm.num_instances << ") ";
				wxDateTime seriesDate;
				seriesDate.ParseFormat(wxString::FromUTF8( sm.series_datetime.c_str() ).GetData(), wxT("%Y-%m-%dT%H:%M:%S"), wxDefaultDateTime);
				{
					if(seriesDate.IsValid()){
						ostr << wxString(seriesDate.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1))).ToUTF8();
					}
					else {
						ostr << _Std("0000/00/00 00:00:00");
					}
				}
				int position = m_pSeriesList->Append(wxString::FromUTF8(ostr.str().c_str()));
				m_positionsId[position] = (*itSeries);
				if (setOfSelected.empty() || setOfSelected.find((*itSeries)) != setOfSelected.end()) {
					m_pSeriesList->Check(position);
				}
			}
		}
		PACSUploadDialog::~PACSUploadDialog()
		{
		}


		std::string PACSUploadDialog::GetSelectedServer()
		{
			int selection = m_pServer->GetSelection();
			if (selection >=0){
				std::string idServer(m_pServer->GetString(selection).ToUTF8());
				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/SubirPACS", "PACSDefecto", idServer);
				return idServer;
			} else {
				return "";
			}
		}

		std::list<long> PACSUploadDialog::GetSelectedSeries()
		{
			std::list<long> series;
			for (int i = 0; i < m_pSeriesList->GetCount(); ++i) {
				if (m_positionsId.find(i) != m_positionsId.end()) {
					series.push_back(m_positionsId[i]);
				}
			}
			return series;
		}
	}
}
