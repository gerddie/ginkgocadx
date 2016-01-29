/*
*
*  $Id: historypanel3.cpp $
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

#include <api/globals.h>
#include <api/ilock.h>
#include <set>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <wx/aui/aui.h>

#include "historypanel3.h"
#include "wxhistoryimagethumbnail.h"
#include "historytablemodel.h"
#include "historytoolmenu.h"
#include "confirmdeletedialog.h"
#include "openwithdialog.h"

#include <resources/ginkgoresourcesmanager.h>
#include <main/entorno.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorpermisos.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorenviohl7.h>
#include <main/controllers/streaming/streamingloader.h>
#include <main/controllers/integrationcontroller.h>
#include <main/controllers/hangingprotocolcontroller.h>
#include <main/controllers/windowattributescontroller.h>

#include <commands/comandoincluirhistorial.h>
#include <commands/synchronizationcommand.h>

#include <main/gui/import/wxwizardimportacionginkgo.h>
#include <main/gui/acquisition/dialogoadquisicion.h>
#include <main/gui/progress/statusbarprogreso.h>

#include <api/dicom/idicommanager.h>
#include <export/tagsprivadoscomunes.h>

#include <commands/openremovableunit.h>
#include <commands/sendpacscommand.h>

#include <eventos/progresocomando.h>

#include <vtkImageData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <wxVTKRenderWindowInteractor.h>
#include <vtk/interactor/ginkgointeractorstyleimage2d.h>
#include <main/gui/droptargetmainwindow.h>

#include <main/controllers/anonusagestats.h>

#define SIZE_THUMBNAILS 76
#define HISTORY_WINDOW_ID "GinkgoCADxHistory"

		

//////////////////////////////////////////////////////////////////////////
namespace GNC {
	namespace GUI {
		class ReloadHistoryTimer:public wxTimer
		{
		public:
			ReloadHistoryTimer(HistoryPanel3* pHistory){
				m_pHistory = pHistory;
			}

			~ReloadHistoryTimer()
			{
				m_pHistory=NULL;
			}

			virtual void Notify()
			{
				m_pHistory->RefreshSearch(true);
			}

			HistoryPanel3* m_pHistory;
		};
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		class wxPatientData: public wxClientData {
		public:
			wxPatientData(const GNC::GCS::HistoryController::PatientModel& patientModel):wxClientData()
			{
				m_PatientModel = patientModel;
			}
			~wxPatientData() {
			}
			GNC::GCS::HistoryController::PatientModel m_PatientModel;
		};

		///////////////////////////////////////////////
		class wxPopUpMenuThumbnails: public wxMenu
		{
		public:
			wxPopUpMenuThumbnails(GNC::GUI::HistoryPanel3* historyPanel, long seriesPk) :wxMenu(), HistoryPanel(historyPanel), SeriesPk(seriesPk)
			{				
				if (seriesPk != -1) {
					wxMenuItem* pItem = Append(wxID_ANY, _("Open Series"));
					Connect(pItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxPopUpMenuThumbnails::OnOpenMenu), NULL, this);
					pItem = Append(wxID_ANY, _("Open Series with .."));
					Connect(pItem->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxPopUpMenuThumbnails::OnOpenWithMenu), NULL, this);
					AppendSeparator();
				}

				GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, this, GNC::GCS::IHistoryTool::TFamily_Open);
				if (GetMenuItemCount() > 0 && !GetMenuItems().back()->IsSeparator()) {
					AppendSeparator();
				}
				GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, this, GNC::GCS::IHistoryTool::TFamily_Q_R);
				if (GetMenuItemCount() > 0 && !GetMenuItems().back()->IsSeparator()) {
					AppendSeparator();
				}
				GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, this, GNC::GCS::IHistoryTool::TFamily_Dicomize);
			}
			~wxPopUpMenuThumbnails()
			{
				Disconnect(wxID_ANY,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( wxPopUpMenuThumbnails::OnOpenMenu),NULL,this);
				Disconnect(wxID_ANY,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( wxPopUpMenuThumbnails::OnOpenWithMenu),NULL,this);
			}

			void OnOpenMenu(wxCommandEvent& )
			{
				HistoryPanel->OpenSeriesOrStudy(SeriesPk, true,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
			}

			void OnOpenWithMenu(wxCommandEvent& )
			{
				HistoryPanel->OpenSeriesOrStudyWith(SeriesPk, true);
			}

			long SeriesPk;
			GNC::GUI::HistoryPanel3* HistoryPanel;
		};

	}
}

//---------------------------------------------------------------------------

GNC::GUI::HistoryPanel3* GNC::GUI::HistoryPanel3::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new HistoryPanel3(GNC::Entorno::Instance()->GetVentanaRaiz());
	}
	return m_pInstance;
}

void GNC::GUI::HistoryPanel3::FreeInstance()
{
	if(m_pInstance!=NULL){
		m_pInstance->Close();
		m_pInstance = NULL;
	}
}

GNC::GUI::HistoryPanel3* GNC::GUI::HistoryPanel3::m_pInstance = NULL;


GNC::GUI::HistoryPanel3::HistoryPanel3(wxWindow* pParent) : HistoryPanel3Base(pParent), Loader(new GNC::StreamingLoader()),
	m_RefreshOnIdle(false)
{
	//load attributes
	GNC::GCS::WindowAttributesController::LoadWindowAttributes(HISTORY_WINDOW_ID, this, false);
	//

	
	m_pTimerReload = new ReloadHistoryTimer(this);
	
	ViewImage2D->IImagePropertiesProxy = Loader;
	ViewImage2D->SetInputConnection(Loader->GetOutputPort());
	ViewImage2D->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION);
	GinkgoInteractorStyleImage2D* pInteractor = GinkgoInteractorStyleImage2D::SafeDownCast(ViewImage2D->GetInteractorStyle().GetPointer());
	if (pInteractor != NULL) {
		pInteractor->SetPreviewMode(true);
	}
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::History::GetIcoHistorialOpen());
	this->SetIcon(icono);

	m_pStatusBar = new GNC::GUI::StatusBarProgreso(this, m_pPanelPreviews, false);
	SetStatusBar(m_pStatusBar);

	wxBitmap bmp;

	//modalities
	{
		const int MaxModalityNumber=20; // Modificar la constante si se añaden o quitan códigos de modalidad
		std::string modalidades[MaxModalityNumber] = {"CR","CT","DR","DX","IO","MG","MR","NM","OT","PT","RF","RG","SC","OCT","US","XA","XC","ES","ECG","HD"};
		for(int i = 0; i<MaxModalityNumber; i++) {
			wxCheckBox* pCheckBox = new wxCheckBox( m_pAdvancedSearchPanel, wxID_ANY, wxString::FromUTF8(modalidades[i].c_str()), wxDefaultPosition, wxDefaultSize, 0 );
			m_pModalitySizer->Add( pCheckBox, 0, wxALL, 2 );
			m_modalitiesList.push_back(pCheckBox);
			pCheckBox->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( HistoryPanel3::OnSearchChange ), NULL, this );
		}
	}

	m_pHistoryTableView = new wxDataViewCtrl( m_pPanelResults, wxID_ANY, wxDefaultPosition, wxDefaultSize,  wxDV_MULTIPLE|wxDV_ROW_LINES);
	m_pTreeListSizer->Add(m_pHistoryTableView, 1, wxEXPAND);	
	
	m_pHistoryTableModel = new HistoryTableModel();
	m_pHistoryTableView->AssociateModel(m_pHistoryTableModel.get());

	//columns...
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(wxEmptyString, new wxDataViewBitmapRenderer( wxT("wxIcon"), wxDATAVIEW_CELL_INERT ),COL_ICON, 50, wxALIGN_CENTER));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Patient name"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),COL_PATIENT_NAME, 330, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Patient id"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),COL_PATIENT_ID, 100, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Age"), new wxDataViewTextRenderer( wxT("long"), wxDATAVIEW_CELL_INERT ),COL_AGE, 30, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Modality"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),COL_MODALITY, 60, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Date acquired"), new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT),COL_DATE_TIME_ACQUIRED, 120, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Description"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),COL_DESCRIPTION, 110, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Date added"), new wxDataViewTextRenderer(wxT("string"), wxDATAVIEW_CELL_INERT),COL_DATE_TIME_ADDED, 120, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));
	m_pHistoryTableView->AppendColumn(new wxDataViewColumn(_("Accession number"), new wxDataViewTextRenderer( wxT("string"), wxDATAVIEW_CELL_INERT ),COL_ACCNUMBER, 128, wxALIGN_LEFT, wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE));	
	//

	m_pToolBar = new HistoryToolMenu(this);
	m_pSizerToolBar->Insert(0, m_pToolBar, 0, wxEXPAND, 0);
	m_pToolBar->Show();

	m_pThumbnails->SetThumbnailImageSize(wxSize(SIZE_THUMBNAILS, SIZE_THUMBNAILS));
	m_pThumbnails->SetBackgroundColour(wxColour(100,100,100));
	m_pThumbnails->SetThumbnailMargin(0);
	m_pThumbnails->SetThumbnailTextHeight(0);

	m_pHistoryTableView->Connect(wxEVT_DATAVIEW_ITEM_CONTEXT_MENU, wxDataViewEventHandler( HistoryPanel3::OnTreeItemMenu ), NULL, this);
	m_pHistoryTableView->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( HistoryPanel3::OnTreeItemActivated ), NULL, this);
	m_pHistoryTableView->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( HistoryPanel3::OnTreeSelChanged ), NULL, this);
	m_pHistoryTableView->Connect(wxEVT_CHAR, wxKeyEventHandler( HistoryPanel3::OnTreeKeyDown), NULL, this);
	m_pThumbnails->Connect(wxEVT_COMMAND_THUMBNAIL_ITEM_SELECTED,wxThumbnailHandler(HistoryPanel3::OnThumbnailSelected),NULL,this);
	m_pThumbnails->Connect(wxEVT_COMMAND_THUMBNAIL_LEFT_DCLICK,wxThumbnailHandler(HistoryPanel3::OnThumbnailDClick),NULL,this);
	m_pThumbnails->Connect(wxEVT_COMMAND_THUMBNAIL_RIGHT_CLICK, wxThumbnailHandler(HistoryPanel3::OnThumbnailRClick), NULL, this);
	
	m_pPatientCombo->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( HistoryPanel3::OnSearchChange ), NULL, this );
	m_pPatientCombo->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( HistoryPanel3::OnSearchChange ), NULL, this );

	GNC::GCS::ControladorEventos::Instance()->Registrar(this, GNC::GCS::Events::EventoProgresoComando());
	m_pSearchPanel->Layout();
	Layout();
	
	SetDropTarget(new GNC::GUI::DropTargetMainWindow());
}

GNC::GUI::HistoryPanel3::~HistoryPanel3()
{
	delete m_pTimerReload;
	
	GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "anonymous_history");
	if (estado) {
		GNC::GCS::HistoryController::Instance()->EmptyHistory(true);
	}

	ViewImage2D->Detach();
	//necesario para que no haga un doble delete
	ViewInteractor2D->Delete();
	ViewInteractor2D->Reparent(NULL);

	//save window attributes
	GNC::GCS::WindowAttributesController::SaveWindowAttributes(HISTORY_WINDOW_ID, this);

}

bool GNC::GUI::HistoryPanel3::Show(bool show)
{
	if (show) {
		LOG_STAT(ANON_STAT_DLG_HISTORY);
	}
	return GNC::GUI::HistoryPanel3Base::Show(show);
}

void GNC::GUI::HistoryPanel3::ReloadHistory()
{

	ReloadCombos(false);
	RefreshSearch(true);
}

void GNC::GUI::HistoryPanel3::ReloadToolBar()
{
	m_pToolBar->Reload();
	//reload menus..
	while (m_pMenuBar->GetMenuCount() > 0)
		delete m_pMenuBar->Remove(0);
	wxMenu* pMenuAcquire = new wxMenu();
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuAcquire, GNC::GCS::IHistoryTool::TFamily_Open);
	if (pMenuAcquire->GetMenuItemCount() > 0 && !pMenuAcquire->GetMenuItems().back()->IsSeparator()) {
		pMenuAcquire->AppendSeparator();
	}
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuAcquire, GNC::GCS::IHistoryTool::TFamily_Q_R);
	if (pMenuAcquire->GetMenuItemCount() >0) {
		m_pMenuBar->Append( pMenuAcquire, _("Acquire") );
	} else {
		delete pMenuAcquire;
	}

	wxMenu* pMenuDicomize = new wxMenu();
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuDicomize, GNC::GCS::IHistoryTool::TFamily_Dicomize);
	if (pMenuDicomize->GetMenuItemCount() >0) {
		m_pMenuBar->Append( pMenuDicomize, _("Dicomize") );
	} else {
		delete pMenuDicomize;
	}

	wxMenu* pMenuTools = new wxMenu();
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuTools, GNC::GCS::IHistoryTool::TFamily_Delete);
	if (pMenuTools->GetMenuItemCount() > 0 && !pMenuTools->GetMenuItems().back()->IsSeparator()) {
		pMenuTools->AppendSeparator();
	}
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuTools, GNC::GCS::IHistoryTool::TFamily_ShowMetadata);
	if (pMenuTools->GetMenuItemCount() >0) {
		m_pMenuBar->Append( pMenuTools, _("Tools") );
	} else {
		delete pMenuTools;
	}

	wxMenu* pMenuExport = new wxMenu();
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuExport, GNC::GCS::IHistoryTool::TFamily_Send);
	if (pMenuExport->GetMenuItemCount() > 0 && !pMenuExport->GetMenuItems().back()->IsSeparator()) {
		pMenuExport->AppendSeparator();
	}
	GNC::HistoryToolsController::Instance()->AppendsToolInMenu(this, pMenuExport, GNC::GCS::IHistoryTool::TFamily_Export);
	if (pMenuExport->GetMenuItemCount() >0) {
		m_pMenuBar->Append( pMenuExport, _("Export") );
	} else {
		delete pMenuExport;
	}
}

void GNC::GUI::HistoryPanel3::ReloadCombos(bool checkPurgue)
{
	if (checkPurgue) {
		//si es preciso limpiar historial...
		GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "anonymous_history");
		if (estado) {
			GNC::GCS::HistoryController::Instance()->EmptyHistory(true);
		}
	}

	//combo box pacientes
	GNC::GCS::HistoryController::PatientModelList listaPacientes;
	GNC::GCS::HistoryController::Instance()->GetAllPatients(listaPacientes);

	m_pPatientCombo->Clear();

	//m_pPatientCombo->Append(_("All patients"), GinkgoResourcesManager::History::GetIcoAllPatients());
	m_pPatientCombo->Append(_("All patients"));

	RefreshPatientFilter();
	if (!m_patientFilter.IsEnabled()) {
		m_pPatientCombo->Select(0);
	}

/*	for (GNC::GCS::HistoryController::PatientModelList::iterator it = listaPacientes.begin(); it != listaPacientes.end(); ++it) {
		std::ostringstream ostr;
		ostr << (*it).name << " (" << (*it).id << ")";

		wxPatientData* pData = new wxPatientData((*it));
		int indice = -1;
		if ((*it).sex == 'M'){
			indice = m_pPatientCombo->Append(wxString::FromUTF8(ostr.str().c_str()), GinkgoResourcesManager::History::GetIcoHombre(), pData);
		}else if((*it).sex =='F'){
			indice = m_pPatientCombo->Append(wxString::FromUTF8(ostr.str().c_str()), GinkgoResourcesManager::History::GetIcoMujer(), pData);
		}else{
			indice = m_pPatientCombo->Append(wxString::FromUTF8(ostr.str().c_str()), GinkgoResourcesManager::History::GetIcoOtro(), pData);
		}

		if (m_patientFilter.IsEnabled() && (*it).id == m_patientFilter.GetPatientId()) {
			m_pPatientCombo->Select(indice);
		}
	}*/
	for (GNC::GCS::HistoryController::PatientModelList::iterator it = listaPacientes.begin(); it != listaPacientes.end(); ++it) {
		std::ostringstream ostr;
		ostr << (*it).name << " (" << (*it).id << ")";

		wxPatientData* pData = new wxPatientData((*it));
		int indice = -1;
		indice = m_pPatientCombo->Append(wxString::FromUTF8(ostr.str().c_str()), pData);

		if (m_patientFilter.IsEnabled() && (*it).id == m_patientFilter.GetPatientId()) {
			m_pPatientCombo->Select(indice);
		}
	}
	if (m_pPatientCombo->GetSelection() <0) {
		m_pPatientCombo->Select(0);
	}


	if (checkPurgue) {
		//load filter state
		std::string strTmp;
		if (GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/History", "PatientComboValue", strTmp)) {
			int pos = m_pPatientCombo->FindString(wxString::FromUTF8(strTmp.c_str()));
			if (pos >= 0) {
				m_pPatientCombo->Select(pos);
			}
		}
	}
}


void GNC::GUI::HistoryPanel3::RefreshSearch(bool force)
{
	if (force) {
		doRefresh(force);
	} else {
		m_RefreshOnIdle = true;
	}
}

void GNC::GUI::HistoryPanel3::GetSelectedSeriesPk(std::list<long>& listOfPks)
{
	wxDataViewItemArray selections;
	int count = m_pHistoryTableView->GetSelections(selections);

	std::set<long> setOfPks;	
	for (int i = 0; i < count; ++i) {
		if (selections[i].IsOk()) {
			HistoryNode* pNode = (HistoryNode*) selections[i].GetID();
			GNC::GUI::HistoryNode::SeriesModelList seriesModels;
			pNode->GetSeriesModel(seriesModels);
			for (GNC::GUI::HistoryNode::SeriesModelList::iterator it = seriesModels.begin(); it != seriesModels.end(); ++it) {
				setOfPks.insert((*it)->pk);
			}
		}
	}
	for (std::set<long>::iterator it = setOfPks.begin(); it != setOfPks.end(); ++it) {
		listOfPks.push_back((*it));
	}
}

void GNC::GUI::HistoryPanel3::OnHistoryPanelIdle( wxIdleEvent& /*event*/ )
{
	if (m_RefreshOnIdle) {
		doRefresh();
	}
}

void GNC::GUI::HistoryPanel3::doRefresh(bool force)
{
	RefreshPatientFilter();
	RefreshModalityFilter();
	RefreshDateFilter();

	////se eliminan todos los estudios que no están abiertos
	Freeze();
	//
	std::list<GNC::GCS::History::IHistoryFilter*> filterList;
	filterList.push_back(&m_modalityFilter);
	filterList.push_back(&m_patientFilter);
	filterList.push_back(&m_dateFilter);

	GNC::GCS::HistoryController::StudyModelList studyList;
	GNC::GCS::HistoryController::Instance()->GetStudies(filterList, studyList);
	{
		wxWindowDisabler disableAll;
		Freeze();
		ShowPreviewPanel(false);
		m_pThumbnails->Clear();
		m_pHistoryTableModel->ReloadTree(studyList, force);	
		int numberImages = 0;
		for (GNC::GCS::HistoryController::StudyModelList::iterator it = studyList.begin(); it != studyList.end(); ++it) {
			numberImages += (*it).num_instances;
		}
		m_pStatusBar->SetStatusText(_("Results") + wxString::Format(_(" (%d images)"), numberImages));
		Thaw();
	}

	Thaw();
	m_RefreshOnIdle = false;
}

void GNC::GUI::HistoryPanel3::RefreshPatientFilter()
{
	if (m_pPatientCombo->GetSelection() > 0) {
		wxPatientData* pPatient = static_cast<wxPatientData*>( m_pPatientCombo->GetClientObject(m_pPatientCombo->GetSelection()) );
		if (pPatient != NULL) {
			m_patientFilter.Enable(true);
			m_patientFilter.SetPatientId(pPatient->m_PatientModel.id);
			GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/History", "PatientComboValue", pPatient->m_PatientModel.id);
		} else {
			m_patientFilter.Enable(false);
		}
	} else {
		m_patientFilter.Enable(false);
	}
}
void GNC::GUI::HistoryPanel3::RefreshModalityFilter()
{
	std::list<std::string> modalities;
	for (TModalitiesVector::iterator it = m_modalitiesList.begin(); it != m_modalitiesList.end(); ++it) {
		if ((*it)->IsChecked()) {
			std::string modality((*it)->GetLabel().ToUTF8());
			modalities.push_back(modality);
		}
	}
	m_modalityFilter.Enable(!modalities.empty());
	m_modalityFilter.SetModalities(modalities);
}

/** Advanced Search button toggled **/
void GNC::GUI::HistoryPanel3::OnAdvancedSearchToggled( wxCommandEvent& event )
{
	Freeze();
	if (event.IsChecked()) {
		m_pAdvancedSearchPanel->Show();
	}
	else {
		m_pAdvancedSearchPanel->Hide();	
	}
	Layout();
	Thaw();
	
}

void GNC::GUI::HistoryPanel3::RefreshDateFilter()
{
	
	wxDateTime fromDate, toDate;
	if (m_pBetween->GetValue()){
		if(m_pTextControlFechaDesde->GetValue().IsValid()){
			fromDate =  m_pTextControlFechaDesde->GetValue();
		}
		if(m_pTextControlFechaHasta->GetValue().IsValid()){
			toDate =  m_pTextControlFechaHasta->GetValue();
		}
	} else if (m_pToday->GetValue() || m_pTodayAM->GetValue() || m_pTodayPM->GetValue()) {
		fromDate = toDate = wxDateTime::Now();
		if (m_pTodayAM->GetValue()) {
			fromDate.SetHour(0);
			fromDate.SetMinute(0);
			fromDate.SetSecond(0);
			toDate.SetHour(11);
			toDate.SetMinute(59);
			toDate.SetSecond(59);
		} else if (m_pTodayPM->GetValue()) {
			fromDate.SetHour(12);
			fromDate.SetMinute(0);
			fromDate.SetSecond(0);
			toDate.SetHour(23);
			toDate.SetMinute(59);
			toDate.SetSecond(59);
		} else  {
			fromDate.SetHour(0);
			fromDate.SetMinute(0);
			fromDate.SetSecond(0);
			toDate.SetHour(23);
			toDate.SetMinute(59);
			toDate.SetSecond(59);
		}
	} else if (m_pYesterday->GetValue()) {
		fromDate = wxDateTime::Now().Add(wxDateSpan(0,0,0,-1));
	} else if (m_pLastWeek->GetValue()) {
		fromDate = wxDateTime::Now().Add(wxDateSpan(0,0,-1,0));
	} else if (m_pLastMonth->GetValue()) {
		fromDate = wxDateTime::Now().Add(wxDateSpan(0,-1,0,0));
	} else if (m_pLastThreeMonths->GetValue()) {
		fromDate = wxDateTime::Now().Add(wxDateSpan(0,-3,0,0));
	}
	m_dateFilter.SetDates(fromDate, toDate);
	m_dateFilter.Enable(fromDate.IsValid() || toDate.IsValid());
}

void GNC::GUI::HistoryPanel3::OnClose(wxCloseEvent &event)
{
	event.Skip(false);
	Hide();
}

void GNC::GUI::HistoryPanel3::OnSearchChange( wxCommandEvent& )
{
	m_pTimerReload->Stop();
	m_pTimerReload->Start(500,true);
}

void GNC::GUI::HistoryPanel3::OnDateChanged( wxDateEvent& /*event*/ )
{
	m_pBetween->SetValue(true);
	m_pTimerReload->Stop();
	m_pTimerReload->Start(500,true);
}

void GNC::GUI::HistoryPanel3::ShowPreviewPanel(bool visible)
{
	if (ViewInteractor2D->IsShown() != visible) {
		ViewInteractor2D->Show(visible);
		m_pHideInteractorPanel->Show(!visible);
		m_pPanelPreviews->Layout();
	}
}

void GNC::GUI::HistoryPanel3::OnTreeItemActivated(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	if (item.IsOk()) {
		HistoryNode* pNode = (HistoryNode*) item.GetID();
		if (pNode->IsStudyModel()) {
			OpenSeriesOrStudy(pNode->GetStudyModel()->pk, false,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
		} else {
			OpenSeriesOrStudy(pNode->GetSeriesModel()->pk, true,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
		}
	}
}

void GNC::GUI::HistoryPanel3::OnTreeItemMenu(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	if(item.IsOk()){
		HistoryNode* pNode = (HistoryNode*) item.GetID();
		wxMenu menu;
		int idMenuItem = 1;

		wxDataViewItemArray dummyArr;
		if (m_pHistoryTableView->GetSelections(dummyArr) == 1) {
			if (pNode->IsStudyModel()) {
				menu.Append(idMenuItem, _("Open study"));
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnOpenMenu),NULL,this);
				menu.Append(idMenuItem, _("Open study with .."));
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnOpenWithMenu),NULL,this);
			} else {
				menu.Append(idMenuItem, _("Open Series"));
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnOpenMenu),NULL,this);
				menu.Append(idMenuItem, _("Open Series with .."));
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnOpenWithMenu),NULL,this);
			} // SERIES MENU
		} else {
			//more than one item selected...
		}

		if (dummyArr.size() > 0) {
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Open);
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Dicomize);
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Q_R);
			{
				//synchronize option
				bool enableSynchronize = (pNode->IsStudyModel()?pNode->GetStudyModel()->location:pNode->GetSeriesModel()->location) 
					== GNC::GCS::IHistoryController::TL_WadoLinked;
				wxMenuItem* pMenuSynchronize = new wxMenuItem(&menu, idMenuItem, wxString( _("Synchronize") ), _("Synchronize"), wxITEM_NORMAL );
				#ifdef __WXMSW__
				pMenuSynchronize->SetBitmaps(GinkgoResourcesManager::Acquisition::GetIcoDownload());
				#else
				pMenuSynchronize->SetBitmap(GinkgoResourcesManager::Acquisition::GetIcoDownload());
				#endif
				menu.Append(pMenuSynchronize);
				menu.Enable(idMenuItem, enableSynchronize);				
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnSynchronizeSelected),NULL,this);
			}
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			if (!GNC::GCS::HistoryController::Instance()->IsReadOnly()) {
				wxMenuItem* pMenuEliminar = new wxMenuItem(&menu, idMenuItem, wxString( _("&Delete from history") ), _("Delete"), wxITEM_NORMAL );
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnDelete),NULL,this);
				#ifdef __WXMSW__
				pMenuEliminar->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
				#else
				pMenuEliminar->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoEliminar());
				#endif
				menu.Append(pMenuEliminar);

				wxMenuItem* pMenuLimpiar = new wxMenuItem(&menu, idMenuItem, wxString( _("&Clear history") ), _("Delete"), wxITEM_NORMAL );
				menu.Connect(idMenuItem++,wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler( HistoryPanel3::OnClearHistory),NULL,this);
				#ifdef __WXMSW__
				pMenuLimpiar->SetBitmaps(GinkgoResourcesManager::History::GetIcoCleanAll());
				#else
				pMenuLimpiar->SetBitmap(GinkgoResourcesManager::History::GetIcoCleanAll());
				#endif
				menu.Append(pMenuLimpiar);
				GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Delete);
				if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
					menu.AppendSeparator();
				}
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_ShowMetadata);
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Send);
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Anonymize);
			if (menu.GetMenuItemCount() > 0 && !menu.GetMenuItems().back()->IsSeparator()) {
				menu.AppendSeparator();
			}
			GNC::HistoryToolsController::Instance()->AppendsToolInMenu(&menu, &menu, GNC::GCS::IHistoryTool::TFamily_Export);
		}

		if (menu.GetMenuItemCount() > 0) {
			this->PopupMenu(&menu);
		}

		event.Skip(false);
	} else {
		event.Skip(true);
	}
}

void GNC::GUI::HistoryPanel3::OnTreeSelChanged(wxDataViewEvent& event)
{
	wxDataViewItem item = event.GetItem();
	if(item.IsOk()){
		ShowPreviewPanel(false);
		HistoryNode* pNode = (HistoryNode*) item.GetID();
		if (pNode->IsStudyModel()) {
			LoadPreviewsFromStudy(pNode->GetStudyModel());
		} else {
			LoadPreviewsFromSeries(pNode->GetSeriesModel());
		}
	}

	event.Skip(true);
}

void GNC::GUI::HistoryPanel3::OnTreeKeyDown(wxKeyEvent& event)
{
	if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_BACK|| event.GetKeyCode() == WXK_NUMPAD_DELETE) {
		RemoveSelected();
		event.Skip(false);
	} else {
		event.Skip(true);
	}
}


void GNC::GUI::HistoryPanel3::OnThumbnailSelected(wxThumbnailEvent& /*evt*/)
{
	int selected = m_pThumbnails->GetSelection();
	if (selected >= 0) {
		try {
			ShowPreviewPanel(true);
			wxHistoryImageThumbnailItem* pItem = static_cast<wxHistoryImageThumbnailItem*>( m_pThumbnails->GetItem(selected) );
			long file_pk;
			pItem->GetFilename().ToLong(&file_pk);
			GNC::GCS::HistoryController::FileModel selectedFile = GNC::GCS::HistoryController::Instance()->GetFileModel(file_pk);
			if (m_currentPreviewedFile != selectedFile.real_path) 
			{
				m_currentPreviewedFile = selectedFile.real_path;
				Loader->SetInput(selectedFile.real_path);
				Loader->UpdateInformation();
				Loader->SetOutputOrigin(selectedFile.image_position);
				double spacing[3] = {1.0,1.0,1.0};
				sscanf( selectedFile.spacing.c_str(), "%lf\\%lf", &spacing[0], &spacing[1]);
				Loader->SetOutputSpacing(spacing);
				if (ViewImage2D->GetNumberOfComponents() == 1) {
					double window, level;
					if (Loader->GetDefaultWindowLevel(window,level) && window != 0.0) {
						ViewImage2D->SetWindow(window);
						ViewImage2D->SetLevel(level);
					} else {
						ViewImage2D->SetAutoDefaultWindowLevel();
					}
				}
				ViewImage2D->SetupPipeline(true);
				ViewImage2D->ResetZoom(false, 0);
				ViewImage2D->UpdateOrientation();
			}
			ViewInteractor2D->Render();
		}
		catch( GNC::GCS::VistaException& ex )
		{
			ShowPreviewPanel(false);
			std::string error = "Exception loading:" + ex.GetCause();
			LOG_ERROR("History3", error);
		}
		catch (GNC::GCS::ControladorCargaException& ex)
		{
			ShowPreviewPanel(false);
			std::string error = "Exception loading:" + ex.GetCause();
			LOG_ERROR("History3", error);			
		}
		catch (...)
		{
			ShowPreviewPanel(false);
			std::string error = "Unknown exception loading";
			LOG_ERROR("History3", error);
		}
	}	
}

void GNC::GUI::HistoryPanel3::OnThumbnailDClick(wxThumbnailEvent& /*evt*/)
{
	int selected = m_pThumbnails->GetSelection();
	if (selected >= 0) {
		wxHistoryImageThumbnailItem* pItem = static_cast<wxHistoryImageThumbnailItem*>(m_pThumbnails->GetItem(selected));
		if (pItem->GetType() == wxHistoryImageThumbnailItem::TI_Series) {
			//open series
			OpenSeriesOrStudy(pItem->GetSeriesPk(), true,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
		} else {
			//open image
			GNC::GCS::IVista* pView = GetViewFromSeries(pItem->GetSeriesPk());
			if (pView != NULL) {
				GNC::Entorno::Instance()->GetVentanaPrincipal()->ForzarCambioVista(pView);
				std::string pathOfFile(TOPATH(pItem->GetFilename()));
				pView->ActivarRuta(pItem->GetFilePk());				
			} else  {
				OpenSeriesOrStudy(pItem->GetSeriesPk(), true,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
			}
		}
	}
}	
void GNC::GUI::HistoryPanel3::OnThumbnailRClick(wxThumbnailEvent& evt)
{
	wxHistoryImageThumbnailItem* pItem = static_cast<wxHistoryImageThumbnailItem*>(m_pThumbnails->GetItem(evt.GetIndex()));
	if (pItem != NULL) {
		wxPopUpMenuThumbnails menu(this, pItem->GetSeriesPk());
		m_pThumbnails->PopupMenu(&menu);
	} else {
		wxPopUpMenuThumbnails menu(this, -1);
		m_pThumbnails->PopupMenu(&menu);
	}
}


void GNC::GUI::HistoryPanel3::OnOpenMenu(wxCommandEvent& /*event*/)
{
	wxDataViewItem item = m_pHistoryTableView->GetSelection();
	if (item.IsOk()) {
		HistoryNode* pNode = (HistoryNode*) item.GetID();
		if (pNode->IsStudyModel()) {
			OpenSeriesOrStudy(pNode->GetStudyModel()->pk, false,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
		} else {
			OpenSeriesOrStudy(pNode->GetSeriesModel()->pk, true,!GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "reutilize_study"));
		}
	}
}

void GNC::GUI::HistoryPanel3::OnOpenWithMenu(wxCommandEvent& /*event*/)
{
	wxDataViewItem item = m_pHistoryTableView->GetSelection();
	if (item.IsOk()) {
		HistoryNode* pNode = (HistoryNode*) item.GetID();
		if (pNode->IsStudyModel()) {
			OpenSeriesOrStudyWith(pNode->GetStudyModel()->pk, false);
		} else {
			OpenSeriesOrStudyWith(pNode->GetSeriesModel()->pk, true);
		}
	}
}

void GNC::GUI::HistoryPanel3::OnDelete(wxCommandEvent& /*event*/)
{
	RemoveSelected();
}

void GNC::GUI::HistoryPanel3::OnClearHistory(wxCommandEvent& /*event*/)
{
	ClearHistory();
}

void GNC::GUI::HistoryPanel3::OnSynchronizeSelected(wxCommandEvent& /*event*/)
{
	SynchronizeSelected();
}


GNC::GCS::IVista* GNC::GUI::HistoryPanel3::GetViewFromSeries(long pk_series)
{
	GNC::GCS::ControladorVistas::TipoListaVistas viewList = GNC::Entorno::Instance()->GetControladorVistas()->GetVistas();
	for (GNC::GCS::ControladorVistas::TipoListaVistas::iterator itViews = viewList.begin(); itViews != viewList.end(); ++itViews) {
		if ((*itViews)->GetEstudio().IsValid()) {
			for(std::vector<long>::const_iterator itUID = (*itViews)->GetEstudio()->OpenedSeries.begin(); itUID != (*itViews)->GetEstudio()->OpenedSeries.end(); ++itUID)
			{
				if (pk_series == (*itUID)) {
					return (*itViews);
				}
			}
		}
	}
	return NULL;
}

void GNC::GUI::HistoryPanel3::OpenSeriesOrStudyWith(long pk, bool isSeries)
{
	GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "max_tabs");
	if (estado) {
		long maxNumberOfTabs = estado.ObtenerValor<long>();
		if ((int)GNC::Entorno::Instance()->GetControladorVistas()->GetVistas().size() >= maxNumberOfTabs) {
				wxMessageBox(_("You have reached maximum number of opened tabs, close some tabs and try it again"), _("Info"),
					wxOK | wxICON_INFORMATION, GNC::Entorno::Instance()->GetVentanaRaiz());
				return;					
		}
	}
	//max

	GNC::GCS::IHistoryController::SeriesModelList seriesModelList;
	if (isSeries) {
		GNC::GCS::IHistoryController::SeriesModel seriesModel = GNC::GCS::HistoryController::Instance()->GetSeriesModel(pk);
		seriesModelList.push_back(seriesModel);
	} else {
		GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy(pk, seriesModelList);	
	}

	OpenWithDialog::ListaModalidades listaModalidades;
	for (GNC::GCS::IHistoryController::SeriesModelList::iterator itSeries = seriesModelList.begin(); itSeries != seriesModelList.end(); ++itSeries) {
		GNC::GCS::IHistoryController::SeriesModel& seriesModel = (*itSeries);
		for (GNC::GCS::HistoryController::StringList::iterator it = seriesModel.tsuids.begin(); it != seriesModel.tsuids.end(); ++it) {
			listaModalidades.push_back(std::pair<std::string,std::string>(seriesModel.series_modality, (*it)));
		}
	}

	OpenWithDialog dialogoAbrir(this, listaModalidades);
	int answer = dialogoAbrir.ShowModal();
	if (answer == wxID_OK) {
		OpenSeries(seriesModelList, dialogoAbrir.GetModuloSeleccionado(), dialogoAbrir.GetModoSeleccionado()->GetId());
	}
}

void GNC::GUI::HistoryPanel3::OpenSeriesOrStudy(long pk, bool isSeries, bool force)
{
	//show current...
	bool seriesIsAlreadyOpen = false;
	GNC::GCS::IHistoryController::SeriesModelList seriesModelList;
	{
		GNC::GCS::IVista* pView = NULL;
		if (isSeries) {
			pView = GetViewFromSeries(pk);
			GNC::GCS::IHistoryController::SeriesModel seriesModel = GNC::GCS::HistoryController::Instance()->GetSeriesModel(pk);
			seriesModelList.push_back(seriesModel);
		} else {
			GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy(pk, seriesModelList);			
			for (GNC::GCS::IHistoryController::SeriesModelList::const_iterator it = seriesModelList.begin(); it != seriesModelList.end(); ++it) {
				pView = GetViewFromSeries((*it).pk);
				if (pView != NULL) {
					break;
				}
			}
		}
		if (seriesModelList.empty()) {
			LOG_WARN("HistoryPanel", "trying to open a series or a study that has been deleted");
			return;
		}
		
		if (pView != NULL) {
			GNC::Entorno::Instance()->GetVentanaPrincipal()->ForzarCambioVista(pView);
			GNC::GCS::HistoryController::FileModel fileModel = GNC::GCS::HistoryController::Instance()->GetFrameOfReference(seriesModelList.front().pk);
			pView->ActivarRuta(fileModel.pk);
			seriesIsAlreadyOpen = true;
			if (!force) {
				return;
			}
		} 	
	}

	//max number of opened tabs
	GNC::GCS::Permisos::EstadoPermiso estado = GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "max_tabs");
	if (estado) {
		long maxNumberOfTabs = estado.ObtenerValor<long>();
		if ((int)GNC::Entorno::Instance()->GetControladorVistas()->GetVistas().size() >= maxNumberOfTabs) {
				wxMessageBox(_("You have reached maximum number of opened tabs, close some tabs and try it again"), _("Info"),
					wxOK | wxICON_INFORMATION, GNC::Entorno::Instance()->GetVentanaRaiz());
				return;					
		}
	}
	//max

	//select modulo and modo that opens all series of a kind...
	GNC::GCS::IModuleController* pModulo = NULL;
	GNC::GCS::ModoControlador* pModo = NULL;
	GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hp;

	//ma
	if (!isSeries) {
		//it's an study maybe hanging protocols.....
		hp = GNC::GCS::HangingProtocolController::Instance()->GetProtocolForStudy(pk);
		if (hp.IsValid()) 
		{
			pModulo = ControladorExtensiones::Instance()->ObtenerModulo(hp->getModuleSid());
			if (pModulo != NULL) {
				pModo = pModulo->GetModo(hp->getOpeningMode());
			}
		}
	}	

	//first try view that supports this importation uid
	GNC::ControladorExtensiones::ListaModulos listaModulos = ControladorExtensiones::Instance()->Modulos();
	for (GNC::GCS::IHistoryController::SeriesModelList::iterator itSeries = seriesModelList.begin(); itSeries != seriesModelList.end() && pModo == NULL && pModulo == NULL; ++itSeries) 
	{
		for(GNC::ControladorExtensiones::IteradorListaModulos itModulos = listaModulos.begin(); itModulos!= listaModulos.end() && pModo == NULL && pModulo == NULL; ++itModulos)
		{
			for(GNC::GCS::IModuleController::IteradorListaModos itModos = (*itModulos).second->GetListaModos().begin(); itModos != (*itModulos).second->GetListaModos().end(); ++itModos){
				bool supports = true;
				for (GNC::GCS::HistoryController::StringList::iterator itTsyntax = (*itSeries).tsuids.begin(); itTsyntax != (*itSeries).tsuids.end(); ++itTsyntax) {
					supports = supports && (*itModos)->SupportsImportationUID((*itSeries).uid_importer) && (*itModos)->SupportsModalityFile((*itSeries).series_modality,(*itTsyntax));				
				}
				if (supports) {
					pModo = (*itModos);
					pModulo = (*itModulos).second;
					break;
				}
			}
		}

		if(pModo== NULL || pModulo == NULL) {
			//second try by modality
			std::list<GNC::GCS::IModuleController*> listaModulos = ControladorExtensiones::Instance()->ModulosOrdenados();
			for(std::list<GNC::GCS::IModuleController*>::iterator itModulos=listaModulos.begin(); itModulos != listaModulos.end() && pModo == NULL && pModulo == NULL; ++itModulos){
				for(GNC::GCS::IModuleController::IteradorListaModos itModos = (*itModulos)->GetListaModos().begin(); itModos != (*itModulos)->GetListaModos().end(); ++itModos){
					bool supports = true;
					for (GNC::GCS::HistoryController::StringList::iterator itTsyntax = (*itSeries).tsuids.begin(); itTsyntax != (*itSeries).tsuids.end(); ++itTsyntax) {
						supports = supports && (*itModos)->SupportsModalityFile((*itSeries).series_modality,(*itTsyntax));				
					}
					if (supports) {
						pModo = (*itModos);
						pModulo = (*itModulos);
						break;
					}
				}
			}
		}
	}

	if(pModo != NULL && pModulo != NULL){
		//openinG!!!
		OpenSeries(seriesModelList, pModulo, pModo->GetId(), hp);
		
	}
	else{
		wxString strModalidades=wxT("");
		wxMessageBox(_("Unable to open modality ") + wxString::FromUTF8(seriesModelList.front().series_modality.c_str()) + _( " with transfer syntax ") << wxString::FromUTF8(seriesModelList.front().tsuids.front().c_str()), _("Info"),
				 wxOK | wxICON_INFORMATION, GNC::Entorno::Instance()->GetVentanaRaiz());
	}
}

void GNC::GUI::HistoryPanel3::OpenSeries(GNC::GCS::HistoryController::SeriesModelList& seriesModelList, GNC::GCS::IModuleController* pModulo, int idModo, GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol)
{
	try {
		if (pModulo->OpenView(idModo,seriesModelList, hangingProtocol) == NULL) {
			LOG_ERROR("GNC/AbrirEstudio", "OpenView has returned null");
			if (hangingProtocol.IsValid()) {
				wxMessageBox(_("Error loading study, check hanging protocol settings"), _("Error loading study"),wxICON_ERROR);
			} 
		}
	}
	catch (GNC::GCS::VistaException& ex)
	{
		LOG_ERROR("GNC/AbrirEstudio", (std::string)ex );
		wxMessageBox(wxString::FromUTF8( ((std::string)ex).c_str() ), _("Error loading study"),wxICON_ERROR);
	}
	catch (...)
	{
		LOG_ERROR("GNC/AbrirEstudio", "Error al abrir el estudio: Error interno");
		wxMessageBox(_( "Internal error" ), _("Error loading study"),wxICON_ERROR);
	}
}

void GNC::GUI::HistoryPanel3::ClearHistory()
{
	//if there is any view opened...
	GNC::GCS::ControladorVistas::TipoListaVistas viewList = GNC::Entorno::Instance()->GetControladorVistas()->GetVistas();
	for (GNC::GCS::ControladorVistas::TipoListaVistas::iterator itViews = viewList.begin(); itViews != viewList.end(); ++itViews) {
		if ((*itViews)->GetEstudio().IsValid() && (*itViews)->GetEstudio()->OpenedSeries.size() > 0) {
			wxMessageBox(_("Failed to empty the history, you have to close opened studies"), _("Info"),
				wxOK | wxICON_WARNING);
			return;
		}
	}

	bool eliminar = false;

	ConfirmDeleteDialog dlg(this, _("all files"));
	switch (dlg.ShowModal()) {
		case wxOK :
			eliminar = GNC::GCS::HistoryController::Instance()->EmptyHistory(true);
			break;
		default :
			break;
	}
	if(eliminar) {
		RefreshSearch(true);
		ReloadCombos(false);
	}
}

void GNC::GUI::HistoryPanel3::RemoveSelected()
{
	GNC::GCS::HistoryController::IdList pkList;
	GetSelectedSeriesPk(pkList);

	if (pkList.empty()) {
		wxMessageBox(_("Select at least one series"), _("Info"), wxOK | wxICON_WARNING);
		return;
	}
	std::set<long> setOfPks;
	setOfPks.insert(pkList.begin(), pkList.end());

	//if there is any view opened...
	GNC::GCS::ControladorVistas::TipoListaVistas viewList = GNC::Entorno::Instance()->GetControladorVistas()->GetVistas();
	for (GNC::GCS::ControladorVistas::TipoListaVistas::iterator itViews = viewList.begin(); itViews != viewList.end(); ++itViews) {
		if ((*itViews)->GetEstudio().IsValid()) {
			for (std::vector<long>::const_iterator itUIDs = (*itViews)->GetEstudio()->OpenedSeries.begin(); itUIDs != (*itViews)->GetEstudio()->OpenedSeries.end(); ++itUIDs)
			{
				if (setOfPks.find((*itUIDs)) != setOfPks.end()) {
					wxMessageBox(_("You can't remove opened series"), _("Info"),
					wxOK | wxICON_WARNING);
					return;
				}
			}
		}
	}

	bool eliminar = false;
	wxString msg = (setOfPks.size()>1)?_("those series"):_("this series");
	ConfirmDeleteDialog dlg(this, msg);
	switch (dlg.ShowModal()) {
		case wxOK :
			eliminar = true;
			break;
		default:
			break;
	}
	if(eliminar){
		GNC::GCS::HistoryController::Instance()->DeleteSeriesList(pkList);
		wxDataViewItemArray selections;
		m_pHistoryTableView->Freeze();
		m_pHistoryTableView->GetSelections(selections);
		m_pHistoryTableModel->DeleteItems(selections);
		m_pHistoryTableView->Thaw();

		RefreshSearch();
		m_pThumbnails->Clear();
		ShowPreviewPanel(false);
		ReloadCombos(false);
	}
}


void GNC::GUI::HistoryPanel3::SelectAll()
{
	m_pHistoryTableView->SelectAll();
}

void GNC::GUI::HistoryPanel3::DeselectAll()
{
	m_pHistoryTableView->UnselectAll();
	m_pThumbnails->Clear();
	ShowPreviewPanel(false);
}

void GNC::GUI::HistoryPanel3::SynchronizeSelected()
{
	GNC::GCS::HistoryController::IdList pkList;
	GetSelectedSeriesPk(pkList);
	
	if (pkList.empty()) {
		wxMessageBox(_("Select at least one series"), _("Info"), wxOK | wxICON_WARNING);
		return;
	}
		
	GADAPI::SynchronizationCommandParameters* pParams = new GADAPI::SynchronizationCommandParameters(pkList, NULL);
	GADAPI::SynchronizationCommand* pCmd = new GADAPI::SynchronizationCommand(pParams);
	GNC::GCS::ICommandController::Instance()->ProcessAsync("syncronizing...", pCmd, NULL);
}

void GNC::GUI::HistoryPanel3::LoadPreviewsFromStudy(const GNC::GCS::Ptr<GNC::GCS::IHistoryController::StudyModel>& pModel)
{
	wxWindowDisabler disableAll;
	m_pThumbnails->Freeze();
	while (m_pThumbnails->GetCount()>0) {
		m_pThumbnails->Delete(0);
	}

	GNC::GCS::HistoryController::SeriesModelList seriesList;
	GNC::GCS::HistoryController::Instance()->GetSeriesFromStudy(pModel->pk, seriesList);
	
	for (GNC::GCS::HistoryController::SeriesModelList::const_iterator it = seriesList.begin(); it != seriesList.end(); ++it) {
		GNC::GCS::HistoryController::FileModel frame = GNC::GCS::HistoryController::Instance()->GetFrameOfReference((*it).pk);
		wxString pathOfItem = wxString::Format(wxT("%ld"), (int) frame.pk);
		

		wxHistoryImageThumbnailItem* pItem = new wxHistoryImageThumbnailItem(pathOfItem, wxHistoryImageThumbnailItem::TI_Series, (*it).pk, frame.pk, pModel->location);
		m_pThumbnails->Insert(pItem);
	}
	m_pThumbnails->Thaw();
}

void GNC::GUI::HistoryPanel3::LoadPreviewsFromSeries(const GNC::GCS::Ptr<GNC::GCS::IHistoryController::SeriesModel>& pModel)
{
	wxWindowDisabler disableAll;
	m_pThumbnails->Freeze();
	while (m_pThumbnails->GetCount()>0) {
		m_pThumbnails->Delete(0);
	}

	GNC::GCS::HistoryController::LightFileModelList listOfFiles;
	GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels(pModel->pk, listOfFiles);

	for (GNC::GCS::HistoryController::LightFileModelList::const_iterator it = listOfFiles.begin(); it != listOfFiles.end(); ++it) {
		wxString pathOfItem = wxString::Format(wxT("%ld"), (int) (*it).pk);
		wxHistoryImageThumbnailItem* pItem = new wxHistoryImageThumbnailItem(pathOfItem, wxHistoryImageThumbnailItem::TI_Image, pModel->pk, (*it).pk, pModel->location);
		m_pThumbnails->Append(pItem);
	}
	m_pThumbnails->Thaw();
}

void GNC::GUI::HistoryPanel3::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	switch(evt->GetCodigoEvento()) {
		case ginkgoEVT_Core_ProgresoComando:
		{
			GNC::GCS::Events::EventoProgresoComando* pEvt = dynamic_cast<GNC::GCS::Events::EventoProgresoComando*> (evt);

			if (pEvt == NULL  || pEvt->GetComando() == NULL) {
				return;
			}
			switch (pEvt->GetTipo()) {
				case GNC::GCS::Events::EventoProgresoComando::TEP_Iniciado:
					{
						GNC::GCS::IPersistentCommand* pPersistent = dynamic_cast<GNC::GCS::IPersistentCommand*> (pEvt->GetComando());
						if (pPersistent != NULL) {
							RefreshSearch();
						}
						m_pStatusBar->InsertarTarea(pEvt->GetComando()->GetId(), pEvt->GetTexto());
					}
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Progreso:
					//
					m_pStatusBar->SetProgresoTarea(pEvt->GetComando()->GetId(), pEvt->GetProgresoNormalizado(), pEvt->GetTexto());
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Finalizado:
					{
						m_pStatusBar->EliminarTarea(pEvt->GetComando()->GetId());
						GNC::GCS::IPersistentCommand* pPersistent = dynamic_cast<GNC::GCS::IPersistentCommand*> (pEvt->GetComando());
						if (pPersistent != NULL) {
							RefreshSearch();
						}
					}
					break;
				case GNC::GCS::Events::EventoProgresoComando::TEP_Unknown:
					break;
			}
		}
		break;
	}
}

//endregion
