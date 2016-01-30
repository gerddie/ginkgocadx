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

#include <main/controllers/hangingprotocolcontroller.h>
#include <main/controllers/controladorextensiones.h>
#include <api/controllers/ipacscontroller.h>
#include <main/entorno.h>
#include <api/dicom/dcmdictionary.h>
#include <api/dicom/idicommanager.h>

#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>
#include <wx/tooltip.h>
#include <wx/pen.h>
#include <wx/pen.h>
#include <wx/sizer.h>
#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <wx/wxginkgotoolbar.h>
#include "hangingprotocolconfigurationpanel.h"
#include <resources/ginkgoresourcesmanager.h>

#define COL_LABEL_ID				0
#define COL_WINDOW_ID				1
#define COL_LEVEL_ID				2

#define COL_HP_NAME					0
#define COL_HP_ACTIVE				1
#define COL_HP_CLIENT_DATA			2

namespace GNC {
	namespace GUI {
		//////////////////////////////////////////
		class HangingButtonsBar : public wxAuiToolBar{
		public:
			HangingButtonsBar(wxWindow* pParent, const wxColour& color, const wxFont& fuente, int style): wxAuiToolBar (pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize, style)
			{
				SetToolBitmapSize(wxSize(16,16));

				wxGinkgoToolBarArt* pToolArt= new wxGinkgoToolBarArt();
				pToolArt->SetCustomBaseColor(color);
				pToolArt->SetFont(fuente);
				SetArtProvider(pToolArt);

				Realize();
			}

			~HangingButtonsBar()
			{
			}
		};
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class SelectOpeningMode: public SelectOpeningModeBase
		{
		public:
			SelectOpeningMode(wxWindow* pParent): SelectOpeningModeBase(pParent)
			{
				GNC::ControladorExtensiones::ListaModulos listaModulos = ControladorExtensiones::Instance()->Modulos();
				for(GNC::ControladorExtensiones::IteradorListaModulos itModulos=listaModulos.begin();itModulos!=listaModulos.end();++itModulos){
					for(GNC::GCS::IModuleController::IteradorListaModos itModos=(*itModulos).second->GetListaModos().begin();itModos!=(*itModulos).second->GetListaModos().end();++itModos){
						wxString wxStr = wxString::FromUTF8((*itModos)->GetDescripcion().c_str());
						m_pOpeningMode->AppendString(wxStr);
						m_modulos.push_back((*itModulos).second);
						m_modos.push_back((*itModos));
					}
				}
				if(m_pOpeningMode->GetCount() > 0) {
					m_pOpeningMode->Select(0);
				}				
				m_pPanelMessage->Layout();
				m_pBody->Layout();
			}

			~SelectOpeningMode()
			{
			}

			GNC::GCS::IModuleController* GetModuloSeleccionado(){
				return m_modulos[m_pOpeningMode->GetSelection()];
			}

			GNC::GCS::ModoControlador* GetModoSeleccionado(){
				return m_modos[m_pOpeningMode->GetSelection()];
			}
			typedef std::vector<GNC::GCS::IModuleController*> TipoVectorModulos;
			typedef std::vector<GNC::GCS::ModoControlador*> TipoVectorModos;
			TipoVectorModulos m_modulos;
			TipoVectorModos m_modos;
		};
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class AddTagDialog: public AddTagDialogBase
		{
		public:
			AddTagDialog(wxWindow* pParent): AddTagDialogBase(pParent)
			{
				listId.push_back(GKDCM_PatientID);
				listId.push_back(GKDCM_IssuerOfPatientID);
				listId.push_back(GKDCM_PatientBirthDate);
				listId.push_back(GKDCM_PatientSex);
				listId.push_back(GKDCM_PatientName);
				listId.push_back(GKDCM_StudyInstanceUID);
				listId.push_back(GKDCM_StudyID);
				listId.push_back(GKDCM_AccessionNumber);
				listId.push_back(GKDCM_ReferringPhysicianName);
				listId.push_back(GKDCM_StudyDescription);
				listId.push_back(GKDCM_ModalitiesInStudy);
				listId.push_back(GKDCM_SeriesInstanceUID);
				listId.push_back(GKDCM_SeriesNumber);
				listId.push_back(GKDCM_Modality);
				listId.push_back(GKDCM_BodyPartExamined);
				listId.push_back(GKDCM_Laterality);
				listId.push_back(GKDCM_ImageLaterality);
				listId.push_back(GKDCM_PatientOrientation);
				listId.push_back(GKDCM_InstitutionName);
				listId.push_back(GKDCM_StationName);
				listId.push_back(GKDCM_InstitutionalDepartmentName);
				listId.push_back(GKDCM_PerformingPhysicianName);
				listId.push_back(GKDCM_ProtocolName);
				listId.push_back(GKDCM_SeriesDescription);

				GIL::DICOM::IDICOMManager*	pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
				for (std::vector<std::string>::const_iterator it = listId.begin(); it != listId.end(); ++it)
				{
					std::string description = pDICOMManager->GetDescription((*it));
					m_pChoiceTag->AppendString(wxString::FromUTF8(description.c_str()));
				}
				m_pChoiceTag->Select(0);
				GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
			}
			~AddTagDialog()
			{
				listId.clear();
			}

			std::pair<std::string, std::string> getTag()
			{
				std::pair<std::string, std::string> value;
				value.first = listId[m_pChoiceTag->GetSelection()];
				value.second = m_pTextValue->GetValue().ToUTF8();
				return value;
			}
		protected:
			std::vector<std::string> listId;
		};
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class EditCriteriaPanel: public EditCriteriaPanelBase
		{
			enum {
				COL_DEESCRIPTION = 0,
				COL_VALUE,
				COL_UID
			};
		public:
			EditCriteriaPanel(wxWindow* pParent, bool buttonsRight = true): EditCriteriaPanelBase(pParent)
			{
				if (!buttonsRight) {
					m_pGeneralSizer->SetOrientation(wxVERTICAL);
					m_pButtonSizer->SetOrientation(wxHORIZONTAL);
					m_pButtonSizer->Insert(0, 0, 0, 1, wxEXPAND, 5 );
				}

				m_pModel = new wxDataViewListStore();
				m_pListCriteria->AssociateModel(m_pModel.get());

				m_pListCriteria->AppendColumn( new wxDataViewColumn( _("Dicom tag"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_HP_NAME, 200, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
				m_pListCriteria->AppendColumn( new wxDataViewColumn( _("Value"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_EDITABLE ), COL_HP_ACTIVE, 230, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));

				m_pListCriteria->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( EditCriteriaPanel::OnCriteriaChangeSelection ), NULL, this );
				Layout();
			}
			~EditCriteriaPanel()
			{
				m_pListCriteria->Disconnect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( EditCriteriaPanel::OnCriteriaChangeSelection ), NULL, this );
			}			

			virtual void loadMatcher(const GNC::GCS::Ptr<GNC::GCS::Matcher>& matcher)
			{
				m_pListCriteria->DeleteAllItems();
				if (matcher.IsValid()) {
					for (std::map<std::string, std::string>::const_iterator it = matcher->getMatchingCriteria().begin(); it != matcher->getMatchingCriteria().end(); ++it)
					{
						addTag((*it).first, (*it).second);
					}
				}
			}

			virtual void updateMatcher(GNC::GCS::Ptr<GNC::GCS::Matcher>& matcher)
			{
				if (matcher.IsValid()) {
					matcher->getMatchingCriteria().clear();
					wxVariant tag,value;
					for (unsigned i = 0; i < m_pModel->GetCount(); ++i) {
						m_pModel->GetValueByRow(tag, i, COL_UID);
						m_pModel->GetValueByRow(value, i, COL_VALUE);
						matcher->getMatchingCriteria()[std::string(tag.GetString().ToUTF8())] = value.GetString().ToUTF8();
					}
				}
			}

			virtual void addTag(const std::string& tagKey, const std::string& value) 
			{
				GIL::DICOM::IDICOMManager*	pDICOMManager = GNC::GCS::IEntorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
				std::string description = pDICOMManager->GetDescription(tagKey);
				wxVector<wxVariant> data;
				data.push_back(wxString::FromUTF8(description.c_str()));
				data.push_back(wxString::FromUTF8(value.c_str()));
				data.push_back(wxString::FromUTF8(tagKey.c_str()));
				m_pModel->AppendItem(data);
				GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
			}

		protected:
			virtual void OnAddClick(wxCommandEvent& ) 
			{
				AddTagDialog dlg(this);
				if (dlg.ShowModal() == wxID_OK) {
					std::pair<std::string, std::string> value = dlg.getTag();
					wxVariant val;
					for (unsigned  i = 0; i < m_pModel->GetItemCount(); ++i) {
						m_pModel->GetValueByRow(val, i, COL_UID);
						if (val.GetString() == wxString::FromUTF8(value.first.c_str()))
						{
							m_pModel->SetValueByRow(wxString::FromUTF8(value.second.c_str()), i, COL_VALUE);
							return;
						}
					}
					
					addTag(value.first, value.second);
				}
			}

			virtual void OnDeleteClick(wxCommandEvent& )
			{
				
				wxDataViewItem item = m_pListCriteria->GetSelection();
				if(item.IsOk()){
					m_pListCriteria->DeleteItem(m_pListCriteria->GetSelectedRow());
				}
				
			}

			virtual void OnCriteriaChangeSelection(wxDataViewEvent& event)
			{
				m_pBDelete->Enable(event.GetItem().IsOk());
			}

			
			wxObjectDataPtr<wxDataViewListStore> m_pModel;
		};

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class EditHangingProtocolDialog: public EditHangingProtocolDialogBase
		{
			class StudyWindow: public StudyWindowBase, public GNC::GCS::IHangingProtocolContract
			{
				class PreviewLayoutPanel: public wxPanel, public GNC::GCS::IHangingSeriesViewer
				{
				public:
					PreviewLayoutPanel(StudyWindow* pSelectionManager, wxWindow* pParent, const GNC::GCS::Ptr<GNC::GCS::Matcher>& seriesMatcher = NULL): wxPanel(pParent, wxID_ANY, wxDefaultPosition, wxSize(-1,-1)),
						isSelected(false),
						pStudyWindow(pSelectionManager)
					{
						wxPanel::SetBackgroundColour(wxBG_STYLE_CUSTOM);
						if (seriesMatcher.IsValid()) {
							this->seriesMatcher = seriesMatcher;
						} else {
							this->seriesMatcher = GNC::GCS::Ptr<GNC::GCS::Matcher>(new GNC::GCS::Matcher());
							this->seriesMatcher->getMatchingCriteria()[GKDCM_SeriesDescription] =  "";
							this->seriesMatcher->getMatchingCriteria()[GKDCM_Modality] =  "";
						}

						this->Connect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewLayoutPanel::OnLeftClick ) );
						this->Connect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( PreviewLayoutPanel::OnRightClick ) );
						this->Connect( wxEVT_PAINT, wxPaintEventHandler( PreviewLayoutPanel::OnPaint ) );
					}

					~PreviewLayoutPanel()
					{
						this->Disconnect( wxEVT_LEFT_UP, wxMouseEventHandler( PreviewLayoutPanel::OnLeftClick ) );
						this->Disconnect( wxEVT_RIGHT_DOWN, wxMouseEventHandler( PreviewLayoutPanel::OnRightClick ) );
						this->Disconnect( wxEVT_PAINT, wxPaintEventHandler( PreviewLayoutPanel::OnPaint ) );
					}

					void select(bool selected) 
					{
						if (selected != isSelected) {
							isSelected = selected;
							Refresh(true);
						}
					}

					GNC::GCS::Ptr<GNC::GCS::Matcher>& getMatcher()
					{
						return this->seriesMatcher;
					}

					void getSeriesBase(GIL::DICOM::DicomDataset& seriesBase)
					{
						for (std::map<std::string, std::string>::iterator it = seriesMatcher->getMatchingCriteria().begin(); it != seriesMatcher->getMatchingCriteria().end(); ++it)
						{
							seriesBase.tags[(*it).first] = (*it).second;
						}
					}
				protected:
					virtual void OnLeftClick(wxMouseEvent &)
					{
						if (pStudyWindow->hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL) {
							pStudyWindow->doSelect(this);
						}
					}

					virtual void OnRightClick(wxMouseEvent &)
					{
						if (pStudyWindow->hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL) {
							pStudyWindow->doSelect(this);
							wxMenu popupMenu;
							int id = 0;
							wxMenuItem* pItem = popupMenu.Append(id, _("Split horizontally..."));
	#ifdef __WXMSW__
							pItem->SetBitmaps(GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1Blue());
	#else
							pItem->SetBitmap(GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1Blue());
	#endif						
							popupMenu.Connect(id++,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PreviewLayoutPanel::OnSplitH ), NULL, this);
							pItem = popupMenu.Append(id, _("Split vertically..."));
	#ifdef __WXMSW__
							pItem->SetBitmaps(GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2Blue());
	#else
							pItem->SetBitmap(GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2Blue());
	#endif
							popupMenu.Connect(id++,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PreviewLayoutPanel::OnSplitV ), NULL, this);
							pItem = popupMenu.Append(id, _("Reset"));
	#ifdef __WXMSW__
							pItem->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoReset());
	#else
							pItem->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoReset());
	#endif
							popupMenu.Connect(id++,wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( PreviewLayoutPanel::OnResetLayout ), NULL, this);
							this->PopupMenu(&popupMenu);
						}
					}

					virtual void OnSplitH(wxCommandEvent& )
					{
						pStudyWindow->SplitActive(true);
					}

					virtual void OnSplitV(wxCommandEvent& )
					{
						pStudyWindow->SplitActive(false);
					}

					virtual void OnResetLayout(wxCommandEvent &)
					{
						pStudyWindow->RebuildLayout();
					}

					virtual void OnPaint(wxPaintEvent &/*event*/)
					{
						wxPaintDC dc(this);
						//no muestra la rallita de abajo
						//wxBufferedPaintDC dc(this);

						const int width = dc.GetSize().x;
						const int height = dc.GetSize().y;
						// Title background
						wxRect rTitle(0, 0, width, height);
						if (this->isSelected) {
							dc.GradientFillLinear(rTitle, wxColour(50, 180, 50), wxColour(100, 230, 100), wxWEST);
						} else {
							dc.GradientFillLinear(rTitle, wxColour(180, 180, 180), wxColour(230, 230, 230), wxWEST);
						}

						// Title border
						dc.SetBrush(*wxTRANSPARENT_BRUSH);
						if (this->isSelected) {
							dc.SetPen(wxPen(wxColour(30, 115, 30), 1, wxSOLID));
						} else {
							dc.SetPen(wxPen(wxColour(115, 115, 115), 1, wxSOLID));
						}
						dc.DrawRectangle(rTitle);
					}

					GNC::GCS::Ptr<GNC::GCS::Matcher> seriesMatcher;
					bool isSelected;
					StudyWindow* pStudyWindow;
				};//end PreviewPanel
				public:
					StudyWindow(wxWindow* pParent): StudyWindowBase(pParent), layout(new GNC::GCS::Layout())
					{
					}

					StudyWindow(wxWindow* pParent, const GNC::GCS::Ptr<GNC::GCS::Layout>& lo): StudyWindowBase(pParent),
						layout(lo)
					{
					}
					~StudyWindow()
					{
						listPreviewLayouts.clear();
					}

					void InitPanels(GNC::GCS::ModoControlador::T_HangingProtocolSupportLevel hsl)
					{
						this->hangingSupportLevel = hsl;

						editSeriesCriteriaPanel = new EditCriteriaPanel(m_pLayoutPreview, false);
						m_pSizerSeriesCriteria->Add(editSeriesCriteriaPanel, 1, wxEXPAND|wxALL, 5);
						m_pLayoutPreview->Layout();

						m_pSpinCols->Enable(hangingSupportLevel != GNC::GCS::ModoControlador::THSL_ONLY_1x1);
						m_pSpinRows->Enable(hangingSupportLevel != GNC::GCS::ModoControlador::THSL_ONLY_1x1);
						m_pSetupPositionButton->Enable(hangingSupportLevel != GNC::GCS::ModoControlador::THSL_ONLY_1x1);

						//button bar
						{
							wxColour barColor = wxColour(200,200,200);
							wxFont fontBar = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );
							HangingButtonsBar* m_pButtonsBar = new HangingButtonsBar(m_pLayoutPreview, barColor, fontBar, wxAUI_TB_DEFAULT_STYLE);
							int i = 1;
							m_pButtonsBar->AddTool(i, _("Split active horizontally"), GinkgoResourcesManager::LayoutBar::GetIcoLayout2x1Blue(), _("Split active horizontally"));
							m_pButtonsBar->EnableTool(i, hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL);
							m_pButtonsBar->Connect(i++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(StudyWindow::OnSplitH), NULL, this);
							m_pButtonsBar->AddTool(i, _("Split active vertically"), GinkgoResourcesManager::LayoutBar::GetIcoLayout1x2Blue(), _("Split active vertically"));
							m_pButtonsBar->EnableTool(i, hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL);
							m_pButtonsBar->Connect(i++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(StudyWindow::OnSplitV), NULL, this);
							m_pButtonsBar->AddTool(i, _("Reset"), GinkgoResourcesManager::MenuIcons::GetIcoReset(), _("Reset"));
							m_pButtonsBar->EnableTool(i, hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL);
							m_pButtonsBar->Connect(i++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(StudyWindow::OnResetLayout), NULL, this);
							m_pButtonsBar->Realize();
							m_pSizerLayoutTools->Insert(0,m_pButtonsBar, 0, wxEXPAND| wxALL, 0);
						} 

						if (layout.IsValid()) {
							this->m_pSpinRows->SetValue(layout->getRows());
							this->m_pSpinCols->SetValue(layout->getColumns());
						}
						//dolayout...
						doLayout();
					}

					/**
					IHangingProtocolContract
					*/
					virtual wxWindow* createSeriesSlot(const std::map<std::string, std::string>& /*properties*/, long /*pk = -1*/, const GNC::GCS::Ptr<GNC::GCS::Matcher>& matcher)
					{
						listPreviewLayouts.push_back(new PreviewLayoutPanel(this, m_pLayoutPreview, matcher));
						return listPreviewLayouts.back();
					}

					virtual const std::list<long>& getOpeningSeries()
					{
						return fooList;
					}
					//
					void doLayout()
					{
						m_pLayoutPreview->Freeze();
						selectedPreview = NULL;
						m_pPreviewSizer->Clear(true);
						listPreviewLayouts.clear();

						layout->doLayout(this, m_pPreviewSizer);

						if (!listPreviewLayouts.empty()) {
							for (std::list<PreviewLayoutPanel*>::const_iterator it = listPreviewLayouts.begin(); it != listPreviewLayouts.end(); ++it) {
								(*it)->select((*it) == selectedPreview);
							}
							doSelect(listPreviewLayouts.front());
						}

						m_pLayoutPreview->Layout();
						m_pLayoutPreview->Thaw();
						Layout();
					}
					
					void doSelect(PreviewLayoutPanel* pPanel) {
						if (selectedPreview == pPanel)
							return;
						m_pLayoutPreview->Freeze();
						if (selectedPreview != NULL) {
							selectedPreview->select(false);
							//store matcher...
							editSeriesCriteriaPanel->updateMatcher(selectedPreview->getMatcher());
						}
						selectedPreview = pPanel;
						selectedPreview->select(true);

						editSeriesCriteriaPanel->loadMatcher(selectedPreview->getMatcher());
						m_pLayoutPreview->Thaw();
					}

					void SplitActive(bool horizontally)
					{
						m_pLayoutPreview->Freeze();
						wxBoxSizer* pNewSizer;
						if (horizontally) {
							pNewSizer = new wxBoxSizer(wxVERTICAL);
						} else {
							pNewSizer = new wxBoxSizer(wxHORIZONTAL);
						}
						wxSizer* pSizerActive = (wxSizer*)selectedPreview->GetContainingSizer();
						wxSizerItem* pItem = pSizerActive->GetItem(selectedPreview);

						pItem->AssignSizer(pNewSizer);
						pNewSizer->Add(selectedPreview, 1, wxEXPAND|wxALL);
						std::map<std::string, std::string> prop;
						pNewSizer->Add(createSeriesSlot(prop, -1, NULL), 1, wxEXPAND|wxALL);
						m_pLayoutPreview->Layout();
						m_pLayoutPreview->Thaw();
					}			

					GNC::GCS::Ptr<GNC::GCS::Layout> GetLayout()
					{
						if (selectedPreview != NULL) {
							editSeriesCriteriaPanel->updateMatcher(selectedPreview->getMatcher());
						}

						if (hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL) {
							GNC::GCS::Ptr<GNC::GCS::Layout> pLayout = GNC::GCS::Layout::getLayoutFromSizer(m_pPreviewSizer);
							pLayout->copyWindowProperties(*layout);
							return pLayout;
						} else {
							GNC::GCS::Ptr<GNC::GCS::Layout> pLayout(new GNC::GCS::Layout());
							pLayout->setRows(m_pSpinRows->GetValue());
							pLayout->setColumns(m_pSpinCols->GetValue());
							GNC::GCS::Ptr<GNC::GCS::Matcher> pSeriesMatcher(new GNC::GCS::Matcher());
							editSeriesCriteriaPanel->updateMatcher(pSeriesMatcher);
							pLayout->setMatcher(pSeriesMatcher);
							pLayout->copyWindowProperties(*layout);
							return pLayout;
						}
					}
				protected:
					virtual void OnSpinRows( wxSpinEvent&  )
					{
						RebuildLayout();
					}

					virtual void OnSpinCols( wxSpinEvent&  )
					{
						RebuildLayout();
					}

					virtual void OnSplitH(wxCommandEvent &)
					{
						SplitActive(true);
					}

					virtual void OnSplitV(wxCommandEvent &)
					{
						SplitActive(false);
					}

					virtual void OnResetLayout(wxCommandEvent &) 
					{
						RebuildLayout();
					}

					virtual void RebuildLayout() 
					{
						//rebuild layout...
						layout->setColumns(m_pSpinCols->GetValue());
						layout->setRows(m_pSpinRows->GetValue());
						layout->getCellList().clear();
						doLayout();
					}

				
					//setup window position
					virtual void OnSetupWindowPositionClick(wxCommandEvent &) 
					{
						SetupWindowPositionBase dlg(this);
						layout->applyWindowProperties(&dlg);
						if (dlg.ShowModal() == wxID_OK) {
							layout->setWindowProperties(dlg.IsMaximized(), dlg.GetPosition().x, dlg.GetPosition().y, dlg.GetSize().x, dlg.GetSize().y);
						}
					}

					virtual void OnSetDefaultWindowPosition(wxCommandEvent &)
					{
						layout->setDefaultPosition();
					}


					GNC::GCS::Ptr<GNC::GCS::Layout> layout;
					EditCriteriaPanel* editSeriesCriteriaPanel;
					std::list<long> fooList;
					PreviewLayoutPanel* selectedPreview;
					std::list<PreviewLayoutPanel*> listPreviewLayouts;
					GNC::GCS::ModoControlador::T_HangingProtocolSupportLevel hangingSupportLevel;
				};//end StudyWindow

			
		public:
			EditHangingProtocolDialog(wxWindow* pParent): EditHangingProtocolDialogBase(pParent)
			{	
			}
			EditHangingProtocolDialog(wxWindow* pParent, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& hP): EditHangingProtocolDialogBase(pParent),
				hangingProtocol(new GNC::GCS::HangingProtocol(*hP))
			{
			}

			~EditHangingProtocolDialog()
			{
			}

			virtual int ShowModal()
			{
				if (!hangingProtocol.IsValid()) {
					SelectOpeningMode dlg(this->GetParent());
					if (dlg.ShowModal() == wxID_OK) {
						hangingProtocol = GNC::GCS::Ptr<GNC::GCS::HangingProtocol>(new GNC::GCS::HangingProtocol());
						hangingProtocol->setName(_Std("New hanging protocol"));
						hangingProtocol->setModuleSid(dlg.GetModuloSeleccionado()->GetUID());
						hangingProtocol->setOpeningMode(dlg.GetModoSeleccionado()->GetId());
						//basic criteria...
						GNC::GCS::Ptr<GNC::GCS::Matcher> studyMatcher(new GNC::GCS::Matcher());
						studyMatcher->getMatchingCriteria()[GKDCM_ModalitiesInStudy] =  "";
						studyMatcher->getMatchingCriteria()[GKDCM_StudyDescription] =  "";
						hangingProtocol->setStudyMatcher(studyMatcher);
						//add new layout...
						GNC::GCS::Ptr<GNC::GCS::Layout> layout(new GNC::GCS::Layout());
						hangingProtocol->addLayout(layout);
					} else {
						return wxID_CANCEL;
					}
				}
				InitPanels();
				return EditHangingProtocolDialogBase::ShowModal();
			}


			void InitPanels()
			{
				editStudyCriteriaPanel = new EditCriteriaPanel(m_pGeneralPanel);
				m_pSizerStudyCriteria->Add(editStudyCriteriaPanel, 1, wxEXPAND|wxALL, 5);

				//fillin hanging...
				m_textOpeningMode->SetValue(wxT("Unknown"));
				hangingSupportLevel = GNC::GCS::ModoControlador::THSL_ONLY_1x1;
				GNC::GCS::IModuleController* pModule = GNC::ControladorExtensiones::Instance()->ObtenerModulo(hangingProtocol->getModuleSid());
				if (pModule != NULL) {
					GNC::GCS::ModoControlador* pController = pModule->GetModo(hangingProtocol->getOpeningMode());
					if (pController != NULL) {
						m_textOpeningMode->SetValue(wxString::FromUTF8(pController->GetDescripcion().c_str()));
						hangingSupportLevel = pController->GetHangingSupportLevel();
					}
				}

				for (GNC::GCS::HangingProtocol::TListOfLayouts::const_iterator it = hangingProtocol->getLayouts().begin(); it != hangingProtocol->getLayouts().end(); ++it)
				{
					if ((*it).IsValid()) {
						StudyWindow* sw = new StudyWindow(m_pNoteBookWindows, (*it));
						m_pNoteBookWindows->InsertPage(m_pNoteBookWindows->GetPageCount() - 1, sw, wxString::Format(_("Window %d"), (int) m_pNoteBookWindows->GetPageCount()));
						sw->InitPanels(hangingSupportLevel);
					}
				}	
				m_pNoteBookWindows->SetSelection(0);

				m_textName->SetValue(wxString::FromUTF8(hangingProtocol->getName().c_str()));
				this->editStudyCriteriaPanel->loadMatcher(hangingProtocol->getStudyMatcher());	

				m_pNoteBookWindows->Layout();
				m_pGeneralPanel->Layout();
				m_pMainNoteBook->Layout();
				Layout();
			}

			GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& getHangingProtocol()
			{
				this->hangingProtocol->clearLayouts();
				this->hangingProtocol->setName(std::string(m_textName->GetValue().ToUTF8()));
				GNC::GCS::Ptr<GNC::GCS::Matcher> pStudyMatcher(new GNC::GCS::Matcher());
				editStudyCriteriaPanel->updateMatcher(pStudyMatcher);
				this->hangingProtocol->setStudyMatcher(pStudyMatcher);
				for (unsigned i = 0; i < m_pNoteBookWindows->GetPageCount(); ++i) {
					wxWindow* pWin = m_pNoteBookWindows->GetPage(i);
					StudyWindow* sw = dynamic_cast<StudyWindow*> (pWin);
					if (sw != NULL) {
						GNC::GCS::Ptr<GNC::GCS::Layout> pLayout = sw->GetLayout();
						if (pLayout.IsValid()) {
							this->hangingProtocol->addLayout(pLayout);
						}
					}
				}
				return this->hangingProtocol;
			}


		protected:

			virtual void OnPageChanging( wxAuiNotebookEvent& event ) 
			{
				if (event.GetSelection() == ((int)m_pNoteBookWindows->GetPageCount() -1)) {
					if (event.GetOldSelection() < 0) {
						//if comes from page closing
						m_pNoteBookWindows->SetSelection(m_pNoteBookWindows->GetPageCount() - 2);
					} else {
						if (hangingSupportLevel == GNC::GCS::ModoControlador::THSL_FULL) {
							StudyWindow* sw = new StudyWindow(m_pNoteBookWindows);
							m_pNoteBookWindows->InsertPage(m_pNoteBookWindows->GetPageCount() - 1,sw, wxString::Format(_("Window %d"), (int) m_pNoteBookWindows->GetPageCount()), true);
							sw->InitPanels(hangingSupportLevel);
							m_pNoteBookWindows->Layout();
						} else {
							wxMessageBox(_("This view doesn't support multiple windows"),_("Info"),wxOK | wxICON_INFORMATION, this);
						}
					}
					event.Veto();
				} else {
					event.Skip();
				}
			}

			virtual void OnPageClose( wxAuiNotebookEvent& event )
			{
				if (m_pNoteBookWindows->GetPageCount() <= 2) {
					event.Veto();
				} else {
					event.Skip();					
				}
			}


			GNC::GCS::ModoControlador::T_HangingProtocolSupportLevel hangingSupportLevel;
			EditCriteriaPanel* editStudyCriteriaPanel;	
			GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol;	
		};
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		class wxHangingClientData: public wxClientData
		{
		public:
			wxHangingClientData(const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& hp): hangingProtocol(hp)
			{
			}
			virtual ~wxHangingClientData()
			{
			}
			GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol;
		};
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		HangingProtocolConfigurationPanel::HangingProtocolConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo): HangingProtocolConfigurationPanelBase(pParent), IPasoConfiguracion(pDialogo)
		{
			wxColour barColor = wxColour(200,200,200);
			wxFont fontBar = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );

			m_pModel = new wxDataViewListStore();
			m_pListHanging->AssociateModel(m_pModel.get());

			m_pListHanging->AppendColumn( new wxDataViewColumn( _("Name"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_HP_NAME, 200, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));
			m_pListHanging->AppendColumn( new wxDataViewColumn( _("Active"), new wxDataViewTextRenderer( "string", wxDATAVIEW_CELL_INERT ), COL_HP_ACTIVE, 150, wxALIGN_LEFT,wxDATAVIEW_COL_SORTABLE | wxDATAVIEW_COL_RESIZABLE ));


			m_pListHanging->Connect(wxEVT_DATAVIEW_ITEM_ACTIVATED, wxDataViewEventHandler( HangingProtocolConfigurationPanel::OnHangingListDClick ), NULL, this );
			m_pListHanging->Connect(wxEVT_DATAVIEW_SELECTION_CHANGED, wxDataViewEventHandler( HangingProtocolConfigurationPanel::OnHangingListChangeSelection ), NULL, this );
			
			Recargar();
		}

		HangingProtocolConfigurationPanel::~HangingProtocolConfigurationPanel()
		{
			wxDataViewItemArray children;
			m_pModel->GetChildren(wxDataViewItem(NULL), children);
			wxVariant val;
			for (wxDataViewItemArray::iterator it = children.begin(); it != children.end(); ++it) 
			{
				m_pModel->GetValue(val, (*it), COL_HP_CLIENT_DATA);
				delete (wxHangingClientData*)(val.GetVoidPtr());
			}
		}

		void HangingProtocolConfigurationPanel::Recargar()
		{
			std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> > listOfProtocols = GNC::GCS::HangingProtocolController::Instance()->getHangingProtocols();

			bool primero =true;
			
			wxVector<wxVariant> data;
			for (std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> >::iterator it = listOfProtocols.begin(); it != listOfProtocols.end(); ++it) 
			{
				data.clear();
				data.push_back(wxString::FromUTF8((*it)->getName().c_str()));
				if ((*it)->isActive()) {
					data.push_back(_("Yes"));
				} else {
					data.push_back(_("No"));
				}
				
				data.push_back(new wxHangingClientData((*it)));
				m_pModel->AppendItem(data);
				if(primero) {
					m_pListHanging->SelectRow(0);
					m_pBEdit->Enable(true);
					m_pBDelete->Enable(true);
					m_pBActive->Enable(true);
					primero = false;
				}
			}			
		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* HangingProtocolConfigurationPanel::GetPanel()
		{
			return this;
		}

		std::string HangingProtocolConfigurationPanel::GetTitle()
		{
			return _Std("Hanging protocols");
		}

		std::string HangingProtocolConfigurationPanel::GetCabecera()
		{
			return _Std("Hanging protocols");
		}

		bool HangingProtocolConfigurationPanel::Validar()
		{
			bool ok = true;
			return ok;
		}

		bool HangingProtocolConfigurationPanel::Guardar()
		{
			//get hanging protocols...
			std::list<GNC::GCS::Ptr<GNC::GCS::HangingProtocol> > listOfProtocols;
			wxVariant value;
			for (unsigned int i = 0; i< m_pModel->GetItemCount(); i++)
			{
				m_pModel->GetValueByRow(value, i, COL_HP_CLIENT_DATA);
				listOfProtocols.push_back(((wxHangingClientData*)value.GetVoidPtr())->hangingProtocol);
			}

			GNC::GCS::HangingProtocolController::Instance()->setHangingProtocols(listOfProtocols);
			return true;
		}
		//endregion


		void HangingProtocolConfigurationPanel::OnHangingListDClick(wxDataViewEvent& )
		{
			Edit();
		}

		void HangingProtocolConfigurationPanel::OnHangingListChangeSelection(wxDataViewEvent& event)
		{
			if(event.GetItem().IsOk()) {
				m_pBEdit->Enable(true);
				m_pBDelete->Enable(true);
				m_pBActive->Enable(true);
			} else {
				m_pBEdit->Enable(false);
				m_pBDelete->Enable(false);
				m_pBActive->Enable(false);
			}
		}

		void HangingProtocolConfigurationPanel::OnSetActive(wxCommandEvent &)
		{
			wxDataViewItem selection = m_pListHanging->GetSelection();
			if (selection.IsOk()) {
				wxVariant value;
				m_pModel->GetValue(value, selection, COL_HP_CLIENT_DATA);
				GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& hp = ((wxHangingClientData*)value.GetVoidPtr())->hangingProtocol;
				hp->setActive(!hp->isActive());
				if (hp->isActive()) {
					m_pListHanging->SetValue( _("Yes"), m_pModel->GetRow(selection), COL_HP_ACTIVE);
				} else {
					m_pListHanging->SetValue( _("No"), m_pModel->GetRow(selection), COL_HP_ACTIVE);
				}
				OnPropiedadCambiada();
			}
		}

		void HangingProtocolConfigurationPanel::OnNuevoClick(wxCommandEvent &)
		{
			EditHangingProtocolDialog dlg(this);
			if (dlg.ShowModal() == wxID_OK) {
				GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hp = dlg.getHangingProtocol();
				wxVector<wxVariant> data;
				data.push_back(wxString::FromUTF8(hp->getName().c_str()));
				if (hp->isActive()) {
					data.push_back(_("Yes"));
				} else {
					data.push_back(_("No"));
				}
				
				data.push_back(new wxHangingClientData(hp));
				m_pModel->AppendItem(data);
				OnPropiedadCambiada();
			}
		}

		void HangingProtocolConfigurationPanel::OnEditarClick(wxCommandEvent &)
		{
			Edit();
		}

		void HangingProtocolConfigurationPanel::Edit()
		{
			wxDataViewItem selection = m_pListHanging->GetSelection();
			if (selection.IsOk()) {
				wxVariant value;
				m_pModel->GetValue(value, selection, COL_HP_CLIENT_DATA);
				EditHangingProtocolDialog dlg(this, ((wxHangingClientData*)value.GetVoidPtr())->hangingProtocol);
				if (dlg.ShowModal() == wxID_OK) {
					GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hp = dlg.getHangingProtocol();
					if (hp->isActive()) {
						m_pListHanging->SetValue( _("Yes"), m_pModel->GetRow(selection), COL_HP_ACTIVE);
					} else {
						m_pListHanging->SetValue( _("No"), m_pModel->GetRow(selection), COL_HP_ACTIVE);
					}
					m_pListHanging->SetValue( wxString::FromUTF8(hp->getName().c_str()), m_pModel->GetRow(selection), COL_HP_NAME);
					((wxHangingClientData*)value.GetVoidPtr())->hangingProtocol = dlg.getHangingProtocol();
					OnPropiedadCambiada();
				}
			}
		}

		void HangingProtocolConfigurationPanel::OnEliminarClick(wxCommandEvent &)
		{
			wxDataViewItem selection = m_pListHanging->GetSelection();
			if (selection.IsOk()) {
				wxVariant value;
				m_pModel->GetValue(value, selection, COL_HP_CLIENT_DATA);
				delete (wxHangingClientData*)value.GetVoidPtr();
				m_pModel->DeleteItem(m_pListHanging->GetSelectedRow());
				OnPropiedadCambiada();
			}
		}

		void HangingProtocolConfigurationPanel::OnPropiedadCambiada()
		{
			IPasoConfiguracion::OnPropiedadCambiada();
		}

		void HangingProtocolConfigurationPanel::OnTextoCambiado(wxCommandEvent & )
		{
		}

		void HangingProtocolConfigurationPanel::OnSize(wxSizeEvent & event){
			event.Skip(true);
		}
	}
}
