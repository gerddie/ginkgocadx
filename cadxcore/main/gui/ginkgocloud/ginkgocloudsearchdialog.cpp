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

#include <api/cloud.h>
#include <api/internationalization/internationalization.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include "ginkgocloudsearchdialog.h"
#include <main/controllers/historytoolscontroller.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <eventos/progresocomando.h>
#include <resources/ginkgoresourcesmanager.h>
#include "filelinkdialog.h"

#include <wx/menu.h>
#include <wx/checkbox.h>
#include <wx/animate.h>
#include <wx/imaglist.h>
#include <wx/msgdlg.h>

#include <main/controllers/anonusagestats.h>

#define SIZE_ICONOS 16

#define STUDY_ICON 0
#define SIZE_ICONOS 16

#define CLOUD_STUDY_UID "studyInstanceUID"
#define CLOUD_MD5 "md5File"
#define CLOUD_CREATOR_USER_NAME "creatorUsername"
#define CLOUD_LAST_MOD_USER_NAME "lastModificationUsername"
#define CLOUD_DESCRIPTION "description"
#define CLOUD_DATE "creationDate"
#define CLOUD_MODALITIES "modalities"
#define CLOUD_URL "urlDownload"

#define DESCRIPTION_COLUMN 0
#define MODALITY_COLUMN 1
#define CREATOR_USER_NAME_COLUMN 2
#define LAST_MODIFICATOR_USER_NAME_COLUMN 3
#define DATE_COLUMN 4
#define URL_COLUMN 5
#define BUCKET_ID_COLUMN 6


////////////////////////////////////////////////

//---------------------------------------------------------------------------

GNC::GUI::GinkgoCloudSearchDialog* GNC::GUI::GinkgoCloudSearchDialog::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new GinkgoCloudSearchDialog(GNC::HistoryToolsController::Instance()->GetHistoryPanel()->GetWxWindow());
	}
	return m_pInstance;
}

void GNC::GUI::GinkgoCloudSearchDialog::FreeInstance()
{
	if(m_pInstance!=NULL){
		m_pInstance->Close();
		m_pInstance = NULL;
	}
}

GNC::GUI::GinkgoCloudSearchDialog* GNC::GUI::GinkgoCloudSearchDialog::m_pInstance = NULL;


GNC::GUI::GinkgoCloudSearchDialog::GinkgoCloudSearchDialog(wxWindow* pParent) : GinkgoCloudSearchDialogBase(pParent)
{
	m_pCommand = NULL;

	//animation
	m_pAnimation = new wxAnimationCtrl(m_pSearchPanel,wxID_ANY,GinkgoResourcesManager::ProgressBar::GetAniLoading());
	m_pAnimation->Hide();
	m_pSizerAnimation->Add(m_pAnimation, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

	m_pFieldCombo->Select(0);

	//modalities
	{
		const int MaxModalityNumber=20; // Modificar la constante si se añaden o quitan códigos de modalidad
		std::string modalidades[MaxModalityNumber] = {"CR","CT","DR","DX","IO","MG","MR","NM","OT","PT","RF","RG","SC","OCT","US","XA","XC","ES","ECG","HD"};
		for(int i = 0; i<MaxModalityNumber; i++) {
			wxCheckBox* pCheckBox = new wxCheckBox( m_pPanelPACS, wxID_ANY, wxString::FromUTF8(modalidades[i].c_str()), wxDefaultPosition, wxDefaultSize, 0 );
			m_pModalitySizer->Add( pCheckBox, 0, wxALL, 2 );
			m_modalitiesList.push_back(pCheckBox);
			pCheckBox->Connect( wxEVT_KEY_UP, wxKeyEventHandler( GinkgoCloudSearchDialog::OnKeyDownFormulario ), NULL, this );
		}
		m_pPanelPACS->Layout();
	}

	//datagridview...
	m_pResultsModel = new wxDataViewListStore();
	m_pResultsView = new wxDataViewListCtrl(m_pPanelPACS, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxDV_SINGLE|wxDV_HORIZ_RULES|wxDV_ROW_LINES);
	m_pSizerDataGrid->Add(m_pResultsView, 1, wxEXPAND);
	m_pResultsView->AssociateModel(m_pResultsModel.get());

	// column 0 of the view control:
	m_pResultsView->AppendColumn( new wxDataViewColumn( _("Description"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), DESCRIPTION_COLUMN, 370, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
	m_pResultsView->AppendColumn( new wxDataViewColumn( _("Modalities"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), MODALITY_COLUMN, 70, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
	m_pResultsView->AppendColumn( new wxDataViewColumn( _("Creator"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), CREATOR_USER_NAME_COLUMN, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
	m_pResultsView->AppendColumn( new wxDataViewColumn( _("Last Modificator"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), LAST_MODIFICATOR_USER_NAME_COLUMN, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
	m_pResultsView->AppendColumn( new wxDataViewColumn( _("Date Time"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ), DATE_COLUMN, 128, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));

	m_pResultsView->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler(GinkgoCloudSearchDialog::OnDataSelChanged), NULL, this);
	m_pResultsView->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler(GinkgoCloudSearchDialog::OnDataItemActivated), NULL, this);
	m_pResultsView->Connect(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler(GinkgoCloudSearchDialog::OnDataItemMenu), NULL, this);

	m_pPanelPACS->Layout();
	Layout();
	GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
}

GNC::GUI::GinkgoCloudSearchDialog::~GinkgoCloudSearchDialog()
{
	GNC::CommandController::Instance()->AbortarComandosDeOwner(this);
	m_pAnimation->Destroy();
}

bool GNC::GUI::GinkgoCloudSearchDialog::Show(bool show)
{
	if (show) {
		LOG_STAT(ANON_STAT_DLG_CLOUD_SEARCH)
	}
	if (IsIconized()) {
		Iconize(false);
	}
	//se recarga la lista de servidores pacs
	if (IsShown())
	{		
		Raise();
	}
	return GinkgoCloudSearchDialogBase::Show(show);
}



//---------------------------------------------------------------------------
//region Eventos de interfaz
void GNC::GUI::GinkgoCloudSearchDialog::OnKeyDownFormulario( wxKeyEvent& event )
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


void GNC::GUI::GinkgoCloudSearchDialog::OnFechaDesdeDateChanged( wxDateEvent& /*event*/ )
{
	m_pBetween->SetValue(true);
}


void GNC::GUI::GinkgoCloudSearchDialog::OnFechaHastaDateChanged( wxDateEvent& /*event*/ )
{
	m_pBetween->SetValue(true);
}

void GNC::GUI::GinkgoCloudSearchDialog::OnCancelClick( wxCommandEvent&  )
{
	GNC::CommandController::Instance()->AbortarComandosDeOwnerAsincrono(this);
	m_pTextControlField->ShowCancelButton(false);
	ShowAnimation(false);
	LimpiarBusquedas();
}

void GNC::GUI::GinkgoCloudSearchDialog::OnBusquedaClick( wxCommandEvent&  )
{
	Search();
}

void GNC::GUI::GinkgoCloudSearchDialog::Search()
{
	//cancel previous command (if any)
	m_pTextControlField->ShowCancelButton(false);
	ShowAnimation(false);
	GNC::CommandController::Instance()->AbortarComandosDeOwnerAsincrono(this);
	LimpiarBusquedas();
	//

	std::string fechaDesde("");
	std::string fechaHasta("");
	if (m_pBetween->GetValue()){
		if(m_pTextControlFechaDesde->GetValue().IsValid()){
			fechaDesde =  std::string(m_pTextControlFechaDesde->GetValue().Format(wxT("%Y-%m-%d")).ToUTF8());
		}
		if(m_pTextControlFechaHasta->GetValue().IsValid()){
			fechaHasta =  std::string(m_pTextControlFechaHasta->GetValue().Format(wxT("%Y-%m-%d")).ToUTF8());
		}
	} else if (m_pToday->GetValue()) {
		fechaDesde = fechaHasta = wxDateTime::Now().Format(wxT("%Y-%m-%d")).ToUTF8();
	} else if (m_pYesterday->GetValue()) {
		fechaDesde = wxDateTime::Now().Add(wxDateSpan(0,0,0,-1)).Format(wxT("%Y-%m-%d")).ToUTF8();
	} else if (m_pLastWeek->GetValue()) {
		fechaDesde = wxDateTime::Now().Add(wxDateSpan(0,0,-1,0)).Format(wxT("%Y-%m-%d")).ToUTF8();
	} else if (m_pLastMonth->GetValue()) {
		fechaDesde = wxDateTime::Now().Add(wxDateSpan(0,-1,0,0)).Format(wxT("%Y-%m-%d")).ToUTF8();
	} else if (m_pLastThreeMonths->GetValue()) {
		fechaDesde = wxDateTime::Now().Add(wxDateSpan(0,-3,0,0)).Format(wxT("%Y-%m-%d")).ToUTF8();
	}

	std::string description, userName, modalities;

	modalities = GetModalities();

	if(fechaDesde == "" && fechaHasta == "" && modalities.empty() &&m_pTextControlField->GetValue().size() == 0)
	{
		int answer = wxMessageBox(_("Search without parameters could take al long time\nWould you like to continue?"),_("Search"), wxYES_NO , this);
		if (answer == wxNO) {
			return;
		}
	}	

	std::ostringstream ostrField;
	if (m_pTextControlField->GetValue().Contains(wxT("*"))) {
		wxString replaced = m_pTextControlField->GetValue();
		replaced.Replace(wxT("*"),wxT("%"));
		ostrField << replaced.ToUTF8();
	} else {
		ostrField << "%";
		ostrField << m_pTextControlField->GetValue().ToUTF8();
		ostrField << "%";
	}
	if (m_pFieldCombo->GetValue() == _("Description")) {
		description = ostrField.str();
	} else if (m_pFieldCombo->GetValue() == _("User name")) {
		userName = ostrField.str();;
	} 

	std::string bucketId;
			
	if(!GNC::GCS::ConfigurationController::Instance()->readStringUser("/GNKPro/GinkgoCloud","bucketId", bucketId) || bucketId.empty()) {
		bucketId = GINKGO_CLOUD_DEFAULT_BUCKET;
	}

	GNC::GCS::CloudFindCommandParams* pParams = new GNC::GCS::CloudFindCommandParams(bucketId, userName, description, GetModalities(), fechaDesde, fechaHasta);
	GNC::GCS::CloudCommand* pCmd = new GNC::GCS::CloudCommand(pParams);
	GNC::CommandController::Instance()->ProcessAsync(_Std("Exploring PACS..."),pCmd, this);
	m_pTextControlField->ShowCancelButton(true);
	ShowAnimation(true);
}

std::string GNC::GUI::GinkgoCloudSearchDialog::GetModalities()
{
	std::string modalities;
	bool all = true;

	for (TModalitiesVector::iterator it =m_modalitiesList.begin(); it !=m_modalitiesList.end(); ++it) {
		wxCheckBox* pCheck = dynamic_cast<wxCheckBox*>(*it);
		if(pCheck != NULL)
			if(pCheck->IsChecked()){
				all = false;
				if (modalities == ""){
					modalities = pCheck->GetLabel().ToUTF8();
				}
				else{
					modalities.append("//");
					modalities.append(pCheck->GetLabel().ToUTF8());
				}
			}
	}
	return modalities;
}

void GNC::GUI::GinkgoCloudSearchDialog::OnCloseClick( wxCommandEvent& /*event*/ )
{
	Hide();
}

void GNC::GUI::GinkgoCloudSearchDialog::OnDescargarClick( wxCommandEvent& /*event*/ )
{
	AddDescarga();
}

void GNC::GUI::GinkgoCloudSearchDialog::OnDataSelChanged(wxDataViewEvent& evt)
{
	wxDataViewItem item = evt.GetItem();
	if (item.IsOk()) {
		m_pBDescargar->Enable(true);
	} else {
		m_pBDescargar->Enable(false);
	}
	evt.Skip(true);
}

void GNC::GUI::GinkgoCloudSearchDialog::OnDataItemActivated(wxDataViewEvent& event)
{
	AddDescarga();
	event.Skip(true);
}

void GNC::GUI::GinkgoCloudSearchDialog::OnDataItemMenu(wxDataViewEvent& event)
{
	//clase menu contextual
	class PopUpMenuDescargar: public wxMenu
	{
	public:
		PopUpMenuDescargar(GinkgoCloudSearchDialog* pDlg) : wxMenu()
		{
			m_pDlg = pDlg;
			wxMenuItem* descargar = new wxMenuItem( this,wxID_ANY,_("Download study"));
#ifdef __WXMSW__
			descargar->SetBitmaps(GinkgoResourcesManager::Acquisition::GetIcoDownload());
#else
			descargar->SetBitmap(GinkgoResourcesManager::Acquisition::GetIcoDownload());
#endif
			Append(descargar);
			Connect(descargar->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnDownload ), NULL, this);
		
				
			wxMenuItem* link = new wxMenuItem( this, wxID_ANY, _("Show link..."));
			Append(link);
			Connect(link->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnShowLink ),NULL, this);

		}
		~PopUpMenuDescargar()
		{
			m_pDlg = NULL;
			Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnDownload ), NULL, this);
			Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PopUpMenuDescargar::OnShowLink ), NULL, this);
		}

		void OnDownload(wxCommandEvent& )
		{
			if (m_pDlg) {
				m_pDlg->AddDescarga();
			}
		}

		void OnShowLink(wxCommandEvent& )
		{
			if (m_pDlg) {
				m_pDlg->ShowStudyLink();
			}
		}


		GinkgoCloudSearchDialog* m_pDlg;

	};
	//
	wxDataViewItem item = event.GetItem();
	if(item.IsOk()){
		PopUpMenuDescargar pMenu(this);
		m_pResultsView->PopupMenu(&pMenu);
	}
}


// Obtiene el item seleccionado del TreeList de resultados, genera una descarga, la apila y la inicia.
void GNC::GUI::GinkgoCloudSearchDialog::AddDescarga()
{
	wxDataViewItem selection = m_pResultsView->GetSelection();
	if(selection.IsOk()){
		wxVariant url;
		m_pResultsModel->GetValue(url, selection, URL_COLUMN);
		GNC::GCS::CloudGetCommandParams* pParams = new GNC::GCS::CloudGetCommandParams(std::string(url.GetString().ToUTF8()), true);
		GNC::GCS::CloudCommand* pCmd = new GNC::GCS::CloudCommand(pParams);
		GNC::CommandController::Instance()->ProcessAsync("Download", pCmd, NULL);
	}
}

void GNC::GUI::GinkgoCloudSearchDialog::ShowStudyLink()
{
	wxDataViewItem selection = m_pResultsView->GetSelection();
	if(selection.IsOk()){
		wxVariant url;
		m_pResultsModel->GetValue(url, selection, URL_COLUMN);
		
		GNC::GUI::FileLinkDialog* dlg = new GNC::GUI::FileLinkDialog(this, std::string(url.GetString().ToUTF8()));
		dlg->Show();
	}
}

void GNC::GUI::GinkgoCloudSearchDialog::ShowAnimation(bool show)
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


void GNC::GUI::GinkgoCloudSearchDialog::LimpiarBusquedas() 
{
	m_pResultsModel->DeleteAllItems();
}

//endregion


//---------------------------------------------------------------------------
//region realizacion de la interfaz IComandoPACSNotificador
void GNC::GUI::GinkgoCloudSearchDialog::LoadResults(const std::string& bucketId, const GNC::GCS::CloudFindCommandParams::Results& results)
{
	m_pTextControlField->ShowCancelButton(false);
	ShowAnimation(false);

	wxVector<wxVariant> data;
	for (GNC::GCS::CloudFindCommandParams::Results::const_iterator itTuples = results.begin(); itTuples != results.end(); ++itTuples) {
		const GNC::GCS::CloudFindCommandParams::ResultTuple& tuple = (*itTuples);
		data.clear();
		if (tuple.find(CLOUD_DESCRIPTION) != tuple.end()) {
			data.push_back(wxString::FromUTF8(tuple.find(CLOUD_DESCRIPTION)->second.c_str()));
		} else {
			data.push_back(_("?"));
		}
		if (tuple.find(CLOUD_MODALITIES) != tuple.end()) {
			data.push_back(wxString::FromUTF8(tuple.find(CLOUD_MODALITIES)->second.c_str()));
		} else {
			data.push_back( _("?"));
		}
		if (tuple.find(CLOUD_CREATOR_USER_NAME) != tuple.end()) {
			data.push_back(wxString::FromUTF8(tuple.find(CLOUD_CREATOR_USER_NAME)->second.c_str()));
		} else {
			data.push_back(_("?"));
		}
		if (tuple.find(CLOUD_CREATOR_USER_NAME) != tuple.end()) {
			data.push_back(wxString::FromUTF8(tuple.find(CLOUD_LAST_MOD_USER_NAME)->second.c_str()));
		} else {
			data.push_back(_("?"));
		}

		if (tuple.find(CLOUD_DATE) != tuple.end()) {
			wxDateTime wxDate;
			wxString dateStr = wxString::FromUTF8(tuple.find(CLOUD_DATE)->second.c_str());
			wxDate.ParseFormat( dateStr, wxT("%Y-%m-%d %H:%M:%S"), wxDefaultDateTime);
		
			if(wxDate.IsValid()){
				dateStr = wxString(wxDate.Format(_("%m/%d/%Y %H:%M:%S"), wxDateTime::TimeZone(wxDateTime::GMT1)));
			}
			data.push_back( dateStr);
		}
		if (tuple.find(CLOUD_URL) != tuple.end()) {
			data.push_back( wxString::FromUTF8(tuple.find(CLOUD_URL)->second.c_str()));
		}
		data.push_back(wxString::FromUTF8(bucketId.c_str()));
		m_pResultsModel->AppendItem(data);
	}
	
}

void GNC::GUI::GinkgoCloudSearchDialog::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);
	if (pEvt == NULL  || pEvt->GetComando() == NULL) {
		return;
	}
	
	switch (pEvt->GetTipo()) {
	case GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Progreso:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Unknown:
		break;
	case GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado:
		GNC::GCS::IComando* pCmd = pEvt->GetComando();
		if (pCmd->GetOwner() == this && !pCmd->EstaAbortado()) {
			GNC::GCS::CloudFindCommandParams* pParams = dynamic_cast<GNC::GCS::CloudFindCommandParams*>(pCmd->GetParams());
			if (pParams != NULL) {
				//load results...
				LoadResults(pParams->m_bucketId, pParams->m_pResults);
			}
		}
		break;
	
	}
}
