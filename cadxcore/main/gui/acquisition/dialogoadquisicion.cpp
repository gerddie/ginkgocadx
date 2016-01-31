/*
*
*  $Id: dialogoadquisicion.cpp $
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
//#define _GINKGO_TRACE
#if defined(_WINDOWS)
#if !defined (_SCL_SECURE_NO_WARNINGS)
#define _SCL_SECURE_NO_WARNINGS
#endif
#if !defined (_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#else
#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#endif

#include <api/globals.h>
#include <api/ilock.h>
#include <set>

#include <wx/wx.h>
#include <wx/animate.h>
#include <wx/odcombo.h>
#include <wx/imaglist.h>
#include <wx/dnd.h>
#include <wx/tokenzr.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/propiedades/wxpropiedades.h>
#include <api/dicom/dcmdictionary.h>

#include "downloadagaindialog.h"
#include "dialogoadquisicion.h"
#include "acquisitiontablemodel.h"

#include <resources/ginkgoresourcesmanager.h>
#include <main/entorno.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/storedqueriescontroller.h>
#include <eventos/progresocomando.h>
#include <commands/pacsdownloadcommand.h>

#include <main/controllers/anonusagestats.h>

#define SIZE_ICONOS 16

#define STORE_QUERY_TOOL_ID 0
#define DELETE_QUERY_TOOL_ID 1

#define ICONO_ESTUDIO 0
#define ICONO_SERIE 1
#define ICONO_HOMBRE 2
#define ICONO_MUJER 3
#define ICONO_OTRO 4
#define SIZE_ICONOS 16
/////////////////////////////////////////////////////////////////
namespace GNC {
	namespace GUI {
		class QueryNameDialog: public QueryNameDialogBase
		{
		public:
			QueryNameDialog(wxWindow* pWindow, const wxString& queryName): QueryNameDialogBase(pWindow)
			{
				m_pQueryName->SetFocus();
				m_pQueryName->SetValue(queryName);
			}
			~QueryNameDialog()
			{
			}

			std::string getQueryName()
			{
				return std::string(m_pQueryName->GetValue().ToUTF8());
			}
			
			virtual void OnOkClick(wxCommandEvent& /*evt*/)
			{
				std::string queryName = getQueryName();
				if (queryName.empty()) {
					wxMessageBox(_("Write query name"), _("Info"),
						wxOK | wxICON_INFORMATION, this);
				} else {
					GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery = GNC::GCS::StoredQueryController::Instance()->getQuery(queryName);
					if (pStoredQuery.IsValid()) {
						if (wxMessageBox(_("This query name is in use, do you want to overwrite it?"), _("Info"), wxYES_NO, this) == wxYES) {
							EndModal(wxID_OK);
						}
					} else {
						EndModal(wxID_OK);
					}
				}
			}
		};
	}
}
//---------------------------------------------------------------------------

GNC::GUI::DialogoAdquisicion* GNC::GUI::DialogoAdquisicion::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new DialogoAdquisicion(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
	}
	return m_pInstance;
}

void GNC::GUI::DialogoAdquisicion::FreeInstance()
{
	if(m_pInstance!=NULL){
		m_pInstance->Close();
		m_pInstance = NULL;
	}
}

GNC::GUI::DialogoAdquisicion* GNC::GUI::DialogoAdquisicion::m_pInstance = NULL;


GNC::GUI::DialogoAdquisicion::DialogoAdquisicion(wxWindow* pParent) : DialogoAdquisicionBase(pParent)
{
	m_SizeRestriction = false;
	m_SizeOK = false;

	//animation
	m_pAnimation = new wxAnimationCtrl(m_pSearchPanel,wxID_ANY,GinkgoResourcesManager::ProgressBar::GetAniLoading());
	m_pAnimation->Hide();
	m_pSizerAnimation->Add(m_pAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	if(GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.limits", "patient_scope")) {
		int tmpIndex = m_pFieldCombo->FindString(_("Id"));
		if (tmpIndex > 0) {
			m_pFieldCombo->Remove(tmpIndex,tmpIndex);
		}
		tmpIndex = m_pFieldCombo->FindString(_("Name"));
		if (tmpIndex > 0) {
			m_pFieldCombo->Remove(tmpIndex,tmpIndex);
		}
		m_pTextControlField->SetEditable(false);
		m_pTextControlField->Enable(false);
		m_pFieldCombo->Enable(false);
		m_pPACSListaServidores->Enable(false);

		m_pTextControlField->Show(false);
		m_pFieldCombo->Show(false);
		m_pPatientLabelStr->Show(false);
		m_pLabelRestricted->Show(true);
		m_pPanelPACS->Layout();
	}
	else {
		int tmpIndex = m_pFieldCombo->FindString(_("Name"));
		if (tmpIndex < 0) {
			m_pFieldCombo->Insert(_("Name"),0);
		}
		tmpIndex = m_pFieldCombo->FindString(_("Id"));
		if (tmpIndex < 0) {
			m_pFieldCombo->Insert(_("Id"),0);
		}
	}
	m_pFieldCombo->Select(0);

	//modalities
	{
		const int MaxModalityNumber=20; // Modificar la constante si se añaden o quitan códigos de modalidad
		std::string modalidades[MaxModalityNumber] = {"CR","CT","DR","DX","IO","MG","MR","NM","OT","PT","RF","RG","SC","OCT","US","XA","XC","ES","ECG","HD"};
		for(int i = 0; i<MaxModalityNumber; i++) {
			wxCheckBox* pCheckBox = new wxCheckBox( m_pPanelPACS, wxID_ANY, wxString::FromUTF8(modalidades[i].c_str()), wxDefaultPosition, wxDefaultSize, 0 );
			m_pModalitySizer->Add( pCheckBox, 0, wxALL, 2 );
			m_modalitiesList.push_back(pCheckBox);
			pCheckBox->Connect( wxEVT_KEY_UP, wxKeyEventHandler( DialogoAdquisicion::OnKeyDownFormulario ), NULL, this );
		}
		m_pPanelPACS->Layout();
	}

	m_pAcquisitionTableView = new wxDataViewCtrl( m_pPanelPACS, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxDV_SINGLE|wxDV_ROW_LINES);
	m_pDataViewSizer->Add(m_pAcquisitionTableView, 1, wxEXPAND);	
	
	m_pAcquisitionTableModel = new AcquisitionTableModel();
	m_pAcquisitionTableView->AssociateModel(m_pAcquisitionTableModel.get());

	//columns...
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(wxEmptyString, new wxDataViewBitmapRenderer( "wxIcon", wxDATAVIEW_CELL_INERT ),COL_ICON, 50, wxALIGN_CENTER));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Patient name"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_PATIENT_NAME, 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Patient id"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_PATIENT_ID, 100, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Modality"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_MODALITY, 70, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Date Time"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_DATETIME, 120, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Description"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_DESCRIPTION, 200, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pAcquisitionTableView->AppendColumn(new wxDataViewColumn(_("Accession Number"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ),COL_ACCNUMBER, 128, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	//

	m_pAcquisitionTableView->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( DialogoAdquisicion::OnTreeSelChanged ), NULL, this);
	m_pAcquisitionTableView->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( DialogoAdquisicion::OnTreeItemActivated ), NULL, this);
	m_pAcquisitionTableView->Connect(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler( DialogoAdquisicion::OnTreeItemMenu ), NULL, this);
	m_pAcquisitionTableView->Connect(wxEVT_DATAVIEW_ITEM_EXPANDED, wxDataViewEventHandler( DialogoAdquisicion::OnTreeItemExpanded ), NULL, this);

	//stored queries
	m_pStoredQueriesButtonBar->AddTool(STORE_QUERY_TOOL_ID, _("Store query"), GinkgoResourcesManager::MenuIcons::GetIcoGuardar());
	m_pStoredQueriesButtonBar->AddTool(DELETE_QUERY_TOOL_ID, _("Remove query"), GinkgoResourcesManager::MenuIcons::GetIcoCross());
	m_pStoredQueriesButtonBar->Realize();
	m_pPanelPACS->Layout();


	m_pStoredQueriesButtonBar->Connect(STORE_QUERY_TOOL_ID,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DialogoAdquisicion::OnStoreQuery),NULL,this);
	m_pStoredQueriesButtonBar->Connect(DELETE_QUERY_TOOL_ID,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( DialogoAdquisicion::OnDeleteQuery),NULL,this);
	Layout();
	GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
}

GNC::GUI::DialogoAdquisicion::~DialogoAdquisicion()
{
	GNC::Entorno::Instance()->GetCommandController()->AbortarComandosDeOwner(this);
	m_pAnimation->Destroy();
}

bool GNC::GUI::DialogoAdquisicion::Show(const std::string& idPatient, const std::string& idPACS)
{
	LOG_STAT(ANON_STAT_DLG_PACS_SEARCH)
	CenterOnParent();
	m_pTextControlField->Show(true);
	m_pFieldCombo->Show(true);
	m_pPatientLabelStr->Show(true);
	m_pLabelRestricted->Show(false);

	m_pTextControlField->Enable(true);
	m_pTextControlField->SetValue(wxString::FromUTF8(idPatient.c_str()));

	m_pPanelPACS->Layout();

	GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Adquisicion", "PACSDefecto", idPACS);

	//load pacs list and select pacs..
	bool retVal = Show(true);

	Search();

	return retVal;
}

bool GNC::GUI::DialogoAdquisicion::Show(bool show)
{
	//se recarga la lista de servidores pacs
	wxArrayString serverListString;
	std::string pacsDef;
	GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Adquisicion", "PACSDefecto", pacsDef);
	int defaultIndex = 0;
	int ind = 0;
	ListOfServers = DicomServerList::Instance()->GetServerList();
	for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it, ++ind)
	{
		std::ostringstream ostr;
		ostr << (*it)->ID << " (" << (*it)->AET << "@" << (*it)->HostName << ":" << (*it)->Port << ")";
		serverListString.push_back(wxString::FromUTF8(ostr.str().c_str()));
		if ((*it)->ID == pacsDef) {
			defaultIndex = ind;
		}
	}
	m_pPACSListaServidores->Clear();
	m_pPACSListaServidores->Append(serverListString);


	if(m_pPACSListaServidores->GetCount()>0){
		//se selecciona el pacs por defecto
		m_pPACSListaServidores->SetSelection(defaultIndex);
	} else {
		m_pPACSListaServidores->AppendString(_("There is no any PACS configured"));
		m_pPACSListaServidores->SetSelection(0);
		m_pPACSListaServidores->Disable();
	}

	if (IsIconized()) {
		Iconize(false);
	}

	RefreshComboQueries();

	if (IsShown())
	{	
		Raise();
	}
	return DialogoAdquisicionBase::Show(show);
}


void GNC::GUI::DialogoAdquisicion::RefreshComboQueries()
{
	m_pComboQueries->Clear();
	GNC::GCS::StoredQueryController::TStoredQueriesList listOfQueries = GNC::GCS::StoredQueryController::Instance()->getStoredQueries();
	for (GNC::GCS::StoredQueryController::TStoredQueriesList::iterator it = listOfQueries.begin(); it != listOfQueries.end(); ++it) {
		m_pComboQueries->AppendString(wxString::FromUTF8((*it)->getName().c_str()));
	}
	m_pComboQueries->Enable(!m_pComboQueries->IsListEmpty());
	//m_pStoredQueriesButtonBar->Enable(!m_pComboQueries->IsListEmpty());
	Refresh();
}


//---------------------------------------------------------------------------
//region Eventos de interfaz
void GNC::GUI::DialogoAdquisicion::OnPACSChanged(wxCommandEvent & )
{
	int selected = m_pPACSListaServidores->GetCurrentSelection();
	DicomServerList::TServerList::iterator pServer = ListOfServers.begin();
	for (int ind = 0;pServer != ListOfServers.end() && ind < selected; ++pServer,++ind);

	if (pServer == ListOfServers.end()) {
		return;
	}
	GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Adquisicion","PACSDefecto", (*pServer)->ID);
	{
		m_pAcquisitionTableModel->ClearResults();
		GNC::GUI::AcquisitionTableModel::T_ModelMode mode = (*pServer)->retrieveSeries?GNC::GUI::AcquisitionTableModel::MM_SERIES:GNC::GUI::AcquisitionTableModel::MM_STUDIES;
		m_pAcquisitionTableModel->SetModelMode(mode);
	}
}

void GNC::GUI::DialogoAdquisicion::OnKeyDownFormulario( wxKeyEvent& event )
{
	if(event.GetKeyCode() == WXK_RETURN || event.GetKeyCode() == WXK_NUMPAD_ENTER) {
		wxCommandEvent evt;
		if (m_pTextControlField->IsEnabled()) {
			OnBusquedaClick(evt);
		}
		event.Skip(false);
	}
	else {
		if(event.GetKeyCode() == WXK_ESCAPE) {
			this->Hide();
			event.Skip(false);
		}
	}
	event.Skip(true);
}


void GNC::GUI::DialogoAdquisicion::OnFechaDesdeDateChanged( wxDateEvent& /*event*/ )
{
	m_pBetween->SetValue(true);
}



void GNC::GUI::DialogoAdquisicion::OnFechaHastaDateChanged( wxDateEvent& /*event*/ )
{
	m_pBetween->SetValue(true);
}

void GNC::GUI::DialogoAdquisicion::OnCancelClick( wxCommandEvent&  )
{
	m_pAcquisitionTableModel->CancelCommand();
	ShowAnimation(false);
}

void GNC::GUI::DialogoAdquisicion::OnBusquedaClick( wxCommandEvent&  )
{
	Search();
}

GNC::GCS::Ptr<GNC::GCS::StoredQuery> GNC::GUI::DialogoAdquisicion::buildQuery()
{
	GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery(new GNC::GCS::StoredQuery());
	std::string pacsId = GetServerSeleccionado();
	if(pacsId=="")
		return pStoredQuery;

	
	pStoredQuery->setName("AcquisitionFormQuery");
	pStoredQuery->setPacsId(pacsId);
	
	//conditions...
	if (m_pBetween->GetValue()){
		std::ostringstream ostr; 
		if(m_pTextControlFechaDesde->GetValue().IsValid()){
			ostr << std::string(m_pTextControlFechaDesde->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
		}
		ostr << "-";
		if(m_pTextControlFechaHasta->GetValue().IsValid()){
			ostr << std::string(m_pTextControlFechaHasta->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
		}
		pStoredQuery->addCondition(GKDCM_StudyDate, ostr.str());
	} else if (m_pToday->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY);	
	} else if(m_pTodayAM->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY_MORNING);
	} else if(m_pTodayPM->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY_AFTERNOON);	
	} else if (m_pYesterday->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_YESTERDAY);
	} else if (m_pLastWeek->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_7_DAYS);
	} else if (m_pLastMonth->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_30_DAYS);
	} else if (m_pLastThreeMonths->GetValue()) {
		pStoredQuery->addSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_3_MONTHS);
	}

	if (!m_pTextControlField->GetValue().empty()) {
		if (m_pFieldCombo->GetValue() == _("Id")) {
			pStoredQuery->addCondition(GKDCM_PatientID, std::string(m_pTextControlField->GetValue().ToUTF8()));
		} else if (m_pFieldCombo->GetValue() == _("Name")) {
			pStoredQuery->addCondition(GKDCM_PatientName, std::string(m_pTextControlField->GetValue().ToUTF8()));
		} else if (m_pFieldCombo->GetValue() == _("Acc#")) {
			pStoredQuery->addCondition(GKDCM_AccessionNumber, std::string(m_pTextControlField->GetValue().ToUTF8()));
		} else if (m_pFieldCombo->GetValue() == _("Study UID")) {
			pStoredQuery->addCondition(GKDCM_StudyInstanceUID, std::string(m_pTextControlField->GetValue().ToUTF8()));
		}
	}

	std::string modalitiesFilter = GetModalities();
	if (!modalitiesFilter.empty()) {
		pStoredQuery->addCondition(GKDCM_ModalitiesInStudy, modalitiesFilter);
	}
	return pStoredQuery;
}

void GNC::GUI::DialogoAdquisicion::Search()
{
	GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery = buildQuery();
	if (pStoredQuery->getPacsId().empty()) {
		return;
	}

	if(pStoredQuery->isEmpty())
	{
		int answer = wxMessageBox(_("Search without parameters could take al long time\nWould you like to continue?"),_("Search"), wxYES_NO , this);
		if (answer == wxNO) {
			return;
		}
		m_pTextControlField->SetFocus();
	}


	m_pAcquisitionTableModel->DoSearch(pStoredQuery);
}

std::string GNC::GUI::DialogoAdquisicion::GetModalities()
{
	std::string modalities;

	for (TModalitiesVector::iterator it =m_modalitiesList.begin(); it !=m_modalitiesList.end(); ++it) {
		wxCheckBox* pCheck = dynamic_cast<wxCheckBox*>(*it);
		if(pCheck != NULL)
			if(pCheck->IsChecked()){
				if (modalities == ""){
					modalities = pCheck->GetLabel().ToUTF8();
				}
				else{
					modalities.append("\\");
					modalities.append(pCheck->GetLabel().ToUTF8());
				}
			}
	}
	return modalities;
}

void GNC::GUI::DialogoAdquisicion::OnLimpiarClick( wxCommandEvent& /*event*/ )
{
	m_pAcquisitionTableModel->ClearResults();
}

void GNC::GUI::DialogoAdquisicion::OnCloseClick( wxCommandEvent& /*event*/ )
{
	Hide();
}

void GNC::GUI::DialogoAdquisicion::OnDescargarClick( wxCommandEvent& /*event*/ )
{
	AddDescarga();
}

void GNC::GUI::DialogoAdquisicion::OnLinkClick( wxCommandEvent& /*event*/)
{
	AddDescarga(true);
}

// Eventos del Treelist
void GNC::GUI::DialogoAdquisicion::OnTreeSelChanged(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	m_pBDescargar->Enable(item.IsOk()); // El item es un estudio o una serie
	m_pBLink->Enable(item.IsOk() && IsLinkSupported());

	event.Skip(true);
}

void GNC::GUI::DialogoAdquisicion::OnTreeItemActivated(wxDataViewEvent& event)
{
	AddDescarga();
	event.Skip(true);
}

void GNC::GUI::DialogoAdquisicion::OnTreeItemMenu(wxDataViewEvent& event)
{
	//clase menu contextual
	class PopUpMenuDescargar: public wxMenu
	{
	public:
		PopUpMenuDescargar(DialogoAdquisicion* pDlg, bool allowDownload, bool series = false) : wxMenu()
		{
			m_pDlg = pDlg;
			if (allowDownload) {
				{
					wxString label;
					if(series) {
						label = _("Download series");
					} else {
						label = _("Download study");
					}
					wxMenuItem* descargar = new wxMenuItem( this,wxID_ANY,label);
#ifdef __WXMSW__
					descargar->SetBitmaps(GinkgoResourcesManager::Acquisition::GetIcoDownload());
#else
					descargar->SetBitmap(GinkgoResourcesManager::Acquisition::GetIcoDownload());
#endif
					Append(descargar);
					Connect(descargar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnDescargar ), NULL, this);
				}
				if (m_pDlg->IsLinkSupported()) {
					wxString label;
					if(series) {
						label = _("Link series");
					} else {
						label = _("Link study");
					}
					wxMenuItem* link = new wxMenuItem( this,wxID_ANY,label);
#ifdef __WXMSW__
					link->SetBitmaps(GinkgoResourcesManager::History::GetIcoLocalLinked());
#else
					link->SetBitmap(GinkgoResourcesManager::History::GetIcoLocalLinked());
#endif
					Append(link);
					Connect(link->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnLink ), NULL, this);
				}				

				this->AppendSeparator();
			}
			wxMenuItem* props = new wxMenuItem( this, wxID_ANY, _("Properties..."));
			Append(props);
			Connect(props->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnProperties ),NULL, this);

		}
		~PopUpMenuDescargar()
		{
			m_pDlg = NULL;
			Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnDescargar ), NULL, this);
			Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnProperties ), NULL, this);
		}

		void OnDescargar(wxCommandEvent& )
		{
			if (m_pDlg) {
				m_pDlg->AddDescarga();
			}
		}

		void OnLink(wxCommandEvent& )
		{
			if (m_pDlg) {
				m_pDlg->AddDescarga(true);
			}
		}

		void OnProperties(wxCommandEvent& )
		{
			if (m_pDlg) {
				m_pDlg->ShowProperties();
			}
		}

		DialogoAdquisicion* m_pDlg;

	};
	//
	wxDataViewItem item = event.GetItem();
	if(item.IsOk()){
		AcquisitionNode* pNode = (AcquisitionNode*) item.GetID();
		PopUpMenuDescargar* pMenu = new PopUpMenuDescargar(this, true, pNode->IsSeriesNode());
		m_pAcquisitionTableView->PopupMenu(pMenu);
		delete pMenu;
	}
}

void GNC::GUI::DialogoAdquisicion::OnTreeItemExpanded(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	if (item.IsOk()) {
		std::string server=GetServerSeleccionado();
		if(server=="")
			return;

		AcquisitionNode* pNode = (AcquisitionNode*) item.GetID();

		m_pAcquisitionTableModel->DoExpand(pNode, GetModalities(), server);		
	}
	event.Skip(true);
}


void GNC::GUI::DialogoAdquisicion::OnComboStoredQueriesClick(wxCommandEvent& /*evt*/)
{
	//
	std::string queryName(m_pComboQueries->GetValue().ToUTF8());
	GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery = GNC::GCS::StoredQueryController::Instance()->getQuery(queryName);
	if (pStoredQuery.IsValid()) {
		//setup server
		{
			int pacsSelection = 0;
			for (DicomServerList::TServerList::iterator it = ListOfServers.begin(); it != ListOfServers.end(); ++it,++pacsSelection)
			{
				if (pStoredQuery->getPacsId() == (*it)->ID) {
					m_pPACSListaServidores->SetSelection(pacsSelection);
					break;
				}
			}
		}
		//setup combo and search value...
		if (pStoredQuery->hasCondition(GKDCM_PatientID)) {
			m_pFieldCombo->SetSelection(0);
			m_pTextControlField->SetValue(wxString::FromUTF8(pStoredQuery->getCondition(GKDCM_PatientID)->getValue().c_str()));
		} else if (pStoredQuery->hasCondition(GKDCM_PatientName)) {
			m_pFieldCombo->SetSelection(1);
			m_pTextControlField->SetValue(wxString::FromUTF8(pStoredQuery->getCondition(GKDCM_PatientName)->getValue().c_str()));
		} else if (pStoredQuery->hasCondition(GKDCM_AccessionNumber)) {
			m_pFieldCombo->SetSelection(2);
			m_pTextControlField->SetValue(wxString::FromUTF8(pStoredQuery->getCondition(GKDCM_AccessionNumber)->getValue().c_str()));
		} else if (pStoredQuery->hasCondition(GKDCM_StudyInstanceUID)) {
			m_pFieldCombo->SetSelection(3);
			m_pTextControlField->SetValue(wxString::FromUTF8(pStoredQuery->getCondition(GKDCM_StudyInstanceUID)->getValue().c_str()));
		} else {
			m_pTextControlField->SetValue(wxEmptyString);
		}
		//setup date radio buttons...
		m_pTodayAM->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY_MORNING));
		m_pTodayPM->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY_AFTERNOON));
		m_pToday->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_TODAY));
		m_pYesterday->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_YESTERDAY));
		m_pLastWeek->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_7_DAYS));
		m_pLastMonth->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_30_DAYS));
		m_pLastThreeMonths->SetValue(pStoredQuery->containsSpecialCondition(GNC::GCS::StoredQuery::TSC_LAST_3_MONTHS));
		//setup modalities check...
		std::set<std::string> modalitiesChecked;
		if (pStoredQuery->hasCondition(GKDCM_ModalitiesInStudy)) {
			std::string modalitiesInStudy = pStoredQuery->getCondition(GKDCM_ModalitiesInStudy)->getValue().c_str();
			//split field through \ (what?) 
			//split with \ bar
			wxStringTokenizer tknz(wxString::FromUTF8(modalitiesInStudy.c_str()), wxT("\\"));
			//insert in the set the values
			while (tknz.HasMoreTokens()) {
				wxString modality = tknz.GetNextToken();
				modalitiesChecked.insert(std::string(modality.ToUTF8()));
			}		
		}
		//check fields contained in the check and uncheck other
		for (TModalitiesVector::iterator it =m_modalitiesList.begin(); it !=m_modalitiesList.end(); ++it) {
			wxCheckBox* pCheck = dynamic_cast<wxCheckBox*>(*it);
			if(pCheck != NULL) {
				pCheck->SetValue(modalitiesChecked.find(std::string(pCheck->GetLabel().ToUTF8())) != modalitiesChecked.end());
			}
		}

		//do search...
		m_pAcquisitionTableModel->DoSearch(pStoredQuery);
	}
}

void GNC::GUI::DialogoAdquisicion::OnStoreQuery(wxCommandEvent& /*evt*/)
{
	GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery = buildQuery();
	if (pStoredQuery->getPacsId().empty()) {
		return;
	} else if (!pStoredQuery->getCondition(GKDCM_ModalitiesInStudy).IsValid() && !pStoredQuery->getCondition(GKDCM_StudyDate).IsValid()) {
		wxMessageBox(_("You must specify at least a modality or a period of time"), _("Info"),
			wxOK | wxICON_INFORMATION, this);
		return;
	} else {
		QueryNameDialog qDlg(this, m_pComboQueries->GetValue());
		if (qDlg.ShowModal() == wxID_OK) {
			pStoredQuery->setName(qDlg.getQueryName());
			GNC::GCS::StoredQueryController::Instance()->storeQuery(pStoredQuery);
			RefreshComboQueries();
		}
	}
}

void GNC::GUI::DialogoAdquisicion::OnDeleteQuery(wxCommandEvent& /*evt*/)
{
	std::string queryName(m_pComboQueries->GetValue().ToUTF8());
	GNC::GCS::Ptr<GNC::GCS::StoredQuery> pStoredQuery = GNC::GCS::StoredQueryController::Instance()->getQuery(queryName);
	if (pStoredQuery.IsValid()) {
		if (wxMessageBox(_("This query will be removed permanently, do you confirm removal?"), _("Info"), wxYES_NO, this) == wxYES) {
			GNC::GCS::StoredQueryController::Instance()->removeQuery(queryName);
			RefreshComboQueries();
		}
	} else {
		wxMessageBox(_("Select a valid query"), _("Info"),
			wxOK | wxICON_INFORMATION, this);
	}
}

std::string GNC::GUI::DialogoAdquisicion::GetServerSeleccionado()
{
	int selected = m_pPACSListaServidores->GetCurrentSelection();
	DicomServerList::TServerList::iterator pServer = ListOfServers.begin();
	for (int ind = 0;pServer != ListOfServers.end() && ind < selected; ++pServer,++ind);

	if (pServer == ListOfServers.end()) {
		wxMessageBox(_("Select a valid PACS Server"), _("Info"),
			wxOK | wxICON_INFORMATION, this);
		return "";
	} else {
		return (*pServer)->ID;
	}
}

bool GNC::GUI::DialogoAdquisicion::IsLinkSupported()
{
	int selected = m_pPACSListaServidores->GetCurrentSelection();
	DicomServerList::TServerList::iterator pServer = ListOfServers.begin();
	for (int ind = 0;pServer != ListOfServers.end() && ind < selected; ++pServer,++ind);

	if (pServer == ListOfServers.end()) {
		return false;
	} else {
		return (*pServer)->retrieveMethod == DicomServer::WADO;
	}
}


// Obtiene el item seleccionado del TreeList de resultados, genera una descarga, la apila y la inicia.
void GNC::GUI::DialogoAdquisicion::AddDescarga(bool link)
{
	wxDataViewItem selection = m_pAcquisitionTableView->GetSelection();

	if(selection.IsOk()){
		AcquisitionNode* pNode = (AcquisitionNode*) selection.GetID();
		std::string modality = m_pAcquisitionTableModel->GetModality(pNode);
		if (pNode->IsStudyNode()) {
			
			if (!IsAllowedToDownload(modality)) {
				LOG_ERROR("GUI/Adquisition", _Std("Download of modality ") << modality << _Std(" not allowed"));
				wxMessageBox(_("You are not allowed to download this kind of modalities."),_("Modality download error"),wxICON_ERROR);
			} else {
				AddDownload(GetServerSeleccionado(), modality, false, pNode->uid, "", link);
			}
		} else {
			if (!modality.empty()) {
				if(! IsAllowedToDownload(modality)) {
					LOG_ERROR("GUI/Adquisition", _Std("Download of modality ") << modality << _Std(" not allowed"));
					wxMessageBox(_("You are not allowed to download this kind of modalities."),_("Modality download error"),wxICON_ERROR);
				}
				else {
					AddDownload(GetServerSeleccionado(), modality, true, pNode->GetParent()->uid, pNode->uid, link);
				}
			}
		}
	}
	else {
		return;
	}	
}

// Shows a dialog with the properties of selected treelist item
void GNC::GUI::DialogoAdquisicion::ShowProperties()
{
	wxDataViewItem selection = m_pAcquisitionTableView->GetSelection();

	if(selection.IsOk()){
		AcquisitionNode* pNode = (AcquisitionNode*) selection.GetID();
		GNC::GUI::wxPropiedades::TListaMapasPropiedades listaMapas;
		m_pAcquisitionTableModel->GetProperties(pNode, listaMapas);

		GNC::GUI::wxPropiedades* pProp = new GNC::GUI::wxPropiedades(this, _Std("search results"), listaMapas);
		//increments width
		wxSize size = pProp->GetSize();
		size.x = 500;
		pProp->SetSize(size);
		pProp->Show();
	}

}

bool GNC::GUI::DialogoAdquisicion::IsAllowedToDownload(const std::string& modality)
{
	if ((GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.download", "all"))) {
		return true;
	} else {
		//split with \ bar
		wxStringTokenizer tknz(wxString::FromUTF8(modality.c_str()), wxT("\\"));
		std::list<std::string> modalities;
		while (tknz.HasMoreTokens()) {
			modalities.push_back(std::string(tknz.GetNextToken().ToUTF8()));
		}
		bool value = true;
		for (std::list<std::string>::iterator it =modalities.begin(); it != modalities.end(); ++it) {
			value = value && GNC::GCS::IControladorPermisos::Instance()->Get("core.pacs.download", (*it));
		}
		return value;
	}
}


void GNC::GUI::DialogoAdquisicion::ShowAnimation(bool show)
{
	if (show != m_pAnimation->IsShown()) {
		m_pAnimation->Show(show);
		if (show) {
			m_pAnimation->Play();
		} else {
			m_pAnimation->Stop();
		}
		m_pTextControlField->ShowCancelButton(show);
		m_pSearchPanel->Layout();
	}
}



void GNC::GUI::DialogoAdquisicion::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
	if (pEvt == NULL  || pEvt->GetComando() == NULL) {
		return;
	}
	
	switch (pEvt->GetTipo()) {
	case GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado:
		{
			GNC::GCS::IComando* pCmd = pEvt->GetComando();
			if (pCmd->GetOwner() == m_pAcquisitionTableModel.get()) {
				ShowAnimation(true);
			}
		}
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Progreso:
		{
			GNC::GCS::IComando* pCmd = pEvt->GetComando();
			if (pCmd->GetOwner() == m_pAcquisitionTableModel.get()) {
				ShowAnimation(true);
			}
		}
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Unknown:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado:
		{
			GNC::GCS::IComando* pCmd = pEvt->GetComando();
			if (pCmd->GetOwner() == m_pAcquisitionTableModel.get()) {
				ShowAnimation(false);
			}
		}
		break;
	
	}
}

bool GNC::GUI::DialogoAdquisicion::AddDownload(const std::string& server, const std::string& modality, bool seriesMode, const std::string& studyUID, const std::string& seriesUID, bool link)
{
	if (server.empty() || (studyUID.empty() && seriesUID.empty())) {
		LOG_ERROR("GUI/Adquisition", "Empty mandatory values <serverId>, <studyInstanceUID | serieInstanceUID> while adding serie to download queue");
		return false;
	}

	GADAPI::PACSDownloadCommandParams * pParams;
	{
		GIL::DICOM::DicomDataset base;
		if (seriesMode) {
			if (!seriesUID.empty() && GNC::GCS::HistoryController::Instance()->ExistsSeriesWithUID(seriesUID)) 
			{
				DownloadAgainDialog dlg(this, seriesUID, true);
				int answer = dlg.ShowModal();
				if (answer != wxID_OK) {
					return false;
				}
			}
			base.tags["0008|0052"] = "SERIES";
			base.tags["0020|000e"] = seriesUID; //series instance uid
			if (!modality.empty()) {
				base.tags["0008|0060"] = modality; //modalitiy
			}
		} else {
			if (!studyUID.empty()) {
				if (GNC::GCS::HistoryController::Instance()->ExistsStudyWithUID(studyUID)) 
				{
					DownloadAgainDialog dlg(this, studyUID, false);
					int answer = dlg.ShowModal();
					if (answer != wxID_OK) {
						return false;
					}
				}
				base.tags["0020|000d"] = studyUID; //study instance uid
			
			}
			base.tags["0008|0052"] = "STUDY";
		}
		pParams = new GADAPI::PACSDownloadCommandParams(server, base, link);
	}

	if (seriesMode) {
		LOG_INFO("GUI/Adquisition", _Std("Downloading serie..."));
	}
	else {
		LOG_INFO("GUI/Adquisition", _Std("Downloading study..."));
	}
	GNC::GCS::IComando* pCommand = new GADAPI::PACSDownloadCommand(pParams);
	
	GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Downloading from PACS..."), pCommand, this);

	return true;

	//endregion
}
