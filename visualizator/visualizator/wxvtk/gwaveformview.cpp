/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "gwaveformview.h"
#include <main/controllers/configurationcontroller.h>
#include <main/entorno.h>
#include <eventos/modificacionimagen.h>
#include <eventos/modificacionwidget.h>
#include <eventos/render.h>
#include <wx/pen.h>
#include <wx/wxginkgotoolbar.h>
#include <api/iannotator.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include <visualizator/vistas/waveformview.h>
#include <visualizator/estudios/visualizatorstudy.h>
#include <main/controllers/commandcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <visualizator/widgets/wecg.h>
#include <vtk/widgets/widgetsactor.h>
#include <widgets/wanotacionesquina.h>

#include <resources/visualizatorresourcesmanager.h>
#include <export/tools/iherramientascore.h>
#include <visualizator/export/tools/ivisualizatortools.h>
#include <export/contracts/iwidgetscontract.h>
#include <export/contracts/irulecontract.h>
#include <api/toolsystem/itoolcontroller.h>
#include <api/toolsystem/itool.h>


#define ID_BUTTON_METADATA 0
#define CUSTOM_ANNOTATOR_TEXT_COLOR 0.1f, 0.1f, 0.1f, 1.0f

namespace GNKVisualizator 
{
	namespace GUI 
	{		
		class GWaveformViewDelegate: public GNC::GCS::IRuleContract
		{
		public:
			GWaveformViewDelegate(GWaveformView* gView):GView(gView)
			{
			}
			~GWaveformViewDelegate()
			{
			}
			
			GNC::GCS::IWidgetsManager* GetManager()
			{ 
				return GView->m_pManager; 
			}

			virtual bool SupportsCallibration()
			{
				return false;
			}
		protected:
			GWaveformView* GView;
		};
		////////////////////GWaveform annotator delegate
		class GWaveformAnnotatorDelegate: public GNC::GCS::IAnnotator
		{
		public:
			GWaveformAnnotatorDelegate(GWaveformView* gView, GNKVisualizator::WaveFormView* pView):GNC::GCS::IAnnotator(pView),
				GView(gView)
			{
			}
			~GWaveformAnnotatorDelegate()
			{
			}

			virtual std::string GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key)
			{
				if (key == "${SLICE.WL.FULLTEXT}") {
					return "";
				} else if (key == "${SLICE.IDX.FULLTEXT}") {
					return "";
				} else if (key == "${VIEW.COORDLABELS}") {
					std::ostringstream os;
					os << "25 mm/s, 10 mm/mV";
					return os.str();
				}
				return GNC::GCS::IAnnotator::GetAnnotationValue(c, key);
			}



		protected:
			GWaveformView* GView;
		};
				
		/* Constructor */
		GWaveformView::GWaveformView(GNKVisualizator::WaveFormView* pView) : GWaveformViewBase(pView->GetEstudio()->ParentWindow),
				Delegate(new GWaveformViewDelegate(this)),
				AnnotatorDelegate(new GWaveformAnnotatorDelegate(this, pView))
		{
            View = pView;
			WECG = NULL;

			ViewImage2D->SetBackgroundColor(0.0f,0.0f,0.0f);
			ViewInteractor2D->m_pImageViewer = ViewImage2D;
			View->GetEstudio()->SetViewer(ViewImage2D);
			ViewImage2D->SetInputConnection(View->GetEstudio()->renderConnection);
			ViewImage2D->SetShowAnnotations(false);
			ViewImage2D->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_INTERACTION);

			// Setup del manejador de widgets */
			m_pManager = GNC::GCS::IEntorno::Instance()->NewWidgetsManager(View);
			m_pManager->SetAnnotator(AnnotatorDelegate);
			ViewInteractor2D->SetManager(m_pManager);
			ViewInteractor2D->SetVID(0,false);
			m_pManager->SetRendererActivo(ViewInteractor2D);
			
			WidgetRepresentation* pWidgetsRepresentation = WidgetRepresentation::New();
			pWidgetsRepresentation->SetRenderer(ViewInteractor2D->FindPokedRenderer(0,0));
			ViewInteractor2D->FindPokedRenderer(0,0)->AddViewProp(pWidgetsRepresentation);
			ViewInteractor2D->SetRepresentation(pWidgetsRepresentation);
			pWidgetsRepresentation->SetWidgetsManager(m_pManager);
			pWidgetsRepresentation->SetWidgetsRenderer(ViewInteractor2D);
			pWidgetsRepresentation->Delete();
			
			//toolbar
			GNC::GCS::IToolController* pToolBar = View->GetToolController();
			//widgets...
			pToolBar->AddTool(HCore_Pointer, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Zoom, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Pan, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Angulo, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HGNKVisualizator_ECGRule, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Nota, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Marcado, (GNC::GCS::IWidgetsContract*) Delegate);
			//view menu...
			pToolBar->AddTool(HCore_CornerAnnotations, (GNC::GCS::IWidgetsContract*) Delegate);
			#if !defined(__WXGTK__)
			pToolBar->AddTool(HCore_DetachWindow, (GNC::GCS::IViewContract*) View);
			#endif
			pToolBar->AddTool(HCore_MetricGrid, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_Reset, (GNC::GCS::IWidgetsContract*) Delegate);

			pToolBar->AddTool(HCore_SecondaryCapture, (GNC::GCS::IWidgetsContract*) Delegate);
			pToolBar->AddTool(HCore_ImageExport, (GNC::GCS::IWidgetsContract*) Delegate);
			//tools
			pToolBar->AddTool(HCore_ShowDicomTags, (GNC::GCS::IViewContract*) View);
			pToolBar->AddTool(HCore_ShowHistory, (GNC::GCS::IViewContract*) View);
			pToolBar->AddTool(HCore_PACSUpload, (GNC::GCS::IViewContract*) View);
			pToolBar->AddTool(HCore_Deshacer, (GNC::GCS::IViewContract*) View);
			m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaVisualizacion), 0, wxEXPAND);
			m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaMedicion), 0, wxEXPAND);
			m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaAnotacion), 0, wxEXPAND);

			{
				GNC::GCS::IToolController::TIdList listids;
				listids.push_back(HCore_ShowHistory);
				wxWindow* pwxToolBar = pToolBar->CreateNewToolBar(this, listids, false);
				m_pSizerTools->Add(pwxToolBar, 1, wxEXPAND);
			}

			//events
			GNC::GCS::Events::EventoModificacionImagen ev2(View);
			GNC::GCS::Events::EventoModificacionWidget ev3(View);
			GNC::GCS::Events::EventoRender ev4(View);

			m_pButtonBar = new wxGinkgoToolBar(m_pManipulationPanel, wxColour(0x40, 0x40, 0x40));
			m_pButtonBar->AddTool(ID_BUTTON_METADATA,_("Show/Hide metadata"),GNKVisualizator::ResourcesManager::ToolIcons::GetIcoECGMetadata(),_("Show/Hide metadata"));
			m_pButtonBar->Connect(ID_BUTTON_METADATA,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GWaveformView::OnShowHideMetadata),NULL,this);

			{
				GNC::GCS::IToolController::TIdList toolsLeft;
				toolsLeft.push_back(HCore_CornerAnnotations);
				wxWindow* toolBarLeft = pToolBar->CreateNewToolBar(m_pManipulationPanel, toolsLeft);
				m_pSizerRight->Add(toolBarLeft,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);	
			}

			m_pButtonBar->Realize();
			m_pSizerRight->Add(m_pButtonBar,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);

			//connect key events..
			pToolBar->ConnectKeyEvents(this);
			
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev2);
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev3);
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev4);

			bool showMetadata = false;
		// if metadata is shown at start, zoom will be broken
		//	GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GNKVisualizator/ECGView", "ShowMetadata", showMetadata, true);
			ShowMetaData(showMetadata);
			
			this->Connect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GWaveformView::OnFocus),NULL,this);
			
			Layout();
		}

		GWaveformView::~GWaveformView()
		{
			this->Disconnect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GWaveformView::OnFocus),NULL,this);
			if (View != NULL) {
				GNC::GCS::IEntorno::Instance()->GetCommandController()->AbortarComandosDeOwner(View);
                View->GetToolController()->FinalizeToolController();
			}			

			m_pManager->EliminarTodosLosWidgets(false);
			
			ViewInteractor2D->SetRepresentation(NULL);

			GNC::GCS::IEntorno::Instance()->FreeWidgetsManager(m_pManager);
			m_pManager = NULL;
			
			ViewImage2D->Detach();
			ViewImage2D->Delete();
			ViewImage2D = NULL;

			ViewInteractor2D->Delete();
			ViewInteractor2D = NULL;

            if (View != NULL) {
                View->Lock();
                delete View;
            }
		}

		void GWaveformView::InitPipeline()
		{
			ViewImage2D->SetInputConnection(View->GetEstudio()->renderConnection);

			m_pComboGroups->Clear();

			std::list<std::string> listOfLabels = View->GetECGStudy()->GetLabelOfMultiplexGroups();
			for (std::list<std::string>::iterator it = listOfLabels.begin(); it != listOfLabels.end(); ++it) {
				m_pComboGroups->AppendString(wxString::FromUTF8((*it).c_str()));
			}
			m_pComboGroups->Select(0);

			if(ViewImage2D != NULL) {
				// FIXME: Comprobar si el pipeline esta conectado.
				ViewImage2D->SetupPipeline();
				
				WECG = new GNKVisualizator::Widgets::WECG(GetManager(), 0);
					GetManager()->InsertarWidget(WECG, false);
				
				if (!View->GetECGStudy()->Ok()) {
					wxMessageBox(_("Error loading study with ecg view"), _("Error"), wxOK, NULL);
				} else {
					WECG->Reload(View->GetECGStudy()->GetListOfChannels());
					WECG->AutoCallibrate();

					m_pTextDiagnosis->SetValue(wxString::FromUTF8(View->GetECGStudy()->GetDiagnosis().c_str()));
					m_pTextMeasurements->SetValue(wxString::FromUTF8(View->GetECGStudy()->GetGlobalMeasurement().c_str()));

					//change color to widget anotacionesesquina
					for( GNC::GCS::ListaWidgets::iterator it = m_pManager->GetListaWidgets().begin(); it != m_pManager->GetListaWidgets().end(); ++it) {
						GNC::GCS::Widgets::IWidget* w = *it;
						if (w->GetVID() == -1) {
							GNC::GCS::Widgets::WAnotador* wac = dynamic_cast<GNC::GCS::Widgets::WAnotador*> (w);
							if (wac != NULL) {
								wac->SetTextColor(GNC::GCS::GLHelper::TColor(CUSTOM_ANNOTATOR_TEXT_COLOR));
							}
						}
					}
				}		
			}
		}

		void GWaveformView::StopPipeline()
		{
			View->GetToolController()->SaveStatus("viewer.waveform");
			if (m_pManager != NULL) {
				m_pManager->OcultarTodosLosWidgets(true, 666);
				m_pManager->DisableAnimation();
				m_pManager->Render();
			}
		}

		void GWaveformView::OnFinishLoad()
		{
			View->GetToolController()->InitializeToolController();
			View->GetToolController()->LoadStatus("viewer.waveform");
			m_pManager->OcultarTodosLosWidgets(true, 666);
			m_pManager->DisableAnimation();
			m_pManager->Render();
		}
				
		GNC::GCS::IWidgetsManager* GWaveformView::GetManager()
		{ 
			return m_pManager; 
		}

		void GWaveformView::OnPaint(wxPaintEvent& event)
		{
			//std::cout << "GVistaCompleja::OnPaint()" << std::endl;
			GWaveformViewBase::OnPaint(event);
			if(GNC::GCS::IEntorno::Instance()->GetControladorVistas()->GetVistaActiva() == View)
			{
				wxPaintDC dc(this);
				wxColour colorLineaSeleccion(219, 219, 0, 255);
				dc.SetBrush(wxBrush(colorLineaSeleccion,wxTRANSPARENT));
				dc.SetPen(wxPen(colorLineaSeleccion, 3, wxSOLID));
				dc.DrawRectangle(wxRect(wxPoint(1, 1), wxPoint(dc.GetSize().x-1, dc.GetSize().y-1)));
			}
		}
	
		void GWaveformView::OnComboGroup(wxCommandEvent &)
		{
			View->GetECGStudy()->SetIndexOfActualMultiplexGroup(m_pComboGroups->GetSelection());
			WECG->Reload(View->GetECGStudy()->GetListOfChannels());
			WECG->AutoCallibrate();

			m_pTextDiagnosis->SetValue(wxString::FromUTF8(View->GetECGStudy()->GetDiagnosis().c_str()));
			m_pTextMeasurements->SetValue(wxString::FromUTF8(View->GetECGStudy()->GetGlobalMeasurement().c_str()));
		}

		void GWaveformView::OnShowHideMetadata(wxCommandEvent &)
		{
			ShowMetaData(!m_pPanelMetadata->IsShown());
			GNC::GCS::ConfigurationController::Instance()->writeBoolUser("/GNKVisualizator/ECGView", "ShowMetadata", m_pPanelMetadata->IsShown());
		}

		void GWaveformView::ShowMetaData(bool show)
		{
			if (show && ! m_pSplitterPanel->IsSplit()) {
				//show
				m_pSplitterPanel->SplitHorizontally(m_pPanelView, m_pPanelMetadata, -100);
			} else if (!show && m_pSplitterPanel->IsSplit()) {
				//hide
				m_pSplitterPanel->Unsplit(m_pPanelMetadata);
			}
		}

		void GWaveformView::OnShowHideCornerAnnotations(wxCommandEvent &)
		{
			m_pManager->ShowAnnotations(!m_pManager->IsShownAnnotations());			
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoRender(View));
		}


		void GWaveformView::OnFocus(wxChildFocusEvent &)
		{
			if (View != NULL) {
				View->OnFocus();
			}
		}

		void GWaveformView::PreProcesarEvento(GNC::GCS::Events::IEvent* evt, std::list<GNC::GCS::Events::IEvent*>& lista)
		{
			if (evt == NULL) {
				std::cerr << "Error: Evento nulo" << std::endl;
				return;
			}
			switch (evt->GetCodigoEvento()) {
				case ginkgoEVT_Core_ModificacionImagen:
					{
						GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
						if (pEvt == NULL) {
							std::cerr << "Error al interpretar evento como evento de modificación de imagen: Evento = " << evt << std::endl;
							return;
						}
						switch (pEvt->GetTipo()) {
							case GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
								{
									lista.push_back(new GNC::GCS::Events::EventoModificacionImagen(View,GNC::GCS::Events::EventoModificacionImagen::ImagenCargada, -1));
								}
								break;
							default:
								break;
						}
						if (pEvt->GetApilarRender()) {
							lista.push_back(new GNC::GCS::Events::EventoRender(View,pEvt->GetSliceAfectado()));
						}
					}
					break;
				default:
					break;
			}
		}

		void GWaveformView::ProcesarEvento(GNC::GCS::Events::IEvent *evt) 
		{
			switch (evt->GetCodigoEvento()) {
				case ginkgoEVT_Core_ModificacionImagen:
				{
					GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
					if (pEvt == NULL) {
						std::cerr << "Error al interpretar evento como evento de modificación de imagen: Evento = " << evt << std::endl;
						return;
					}
					switch (pEvt->GetTipo()) {
						case GNC::GCS::Events::EventoModificacionImagen::ImagenModificada:
						case GNC::GCS::Events::EventoModificacionImagen::ImagenCargada:
						case GNC::GCS::Events::EventoModificacionImagen::SliceCambiado:
								{
									ViewImage2D->Modified();
									GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(View,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
									if (View->GetECGStudy()->Ok()) {
										WECG->Reload(View->GetECGStudy()->GetListOfChannels());
									}
								}
								break;
						case GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
							{
								View->GetEstudio()->RecalibrateActiveImage(pEvt->GetNewSpacing(), pEvt->GetNewOrigin());
							}
						break;
						case GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada:
							{
								//para que pille bien el window/level
								if(pEvt->ResetZoom())
								{
									ViewImage2D->ResetZoom(false, pEvt->GetResetZoomMode());
								}
								GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(View,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
							}
						break;
						case GNC::GCS::Events::EventoModificacionImagen::ImagenDescargada:
							{
							}
						break;
						
						case GNC::GCS::Events::EventoModificacionImagen::MapaModificado:
						case GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas:
						break;
					}
				}//fin modificacion
				break;

				case ginkgoEVT_Core_Render:
					{
						ViewInteractor2D->Refresh(false);
					}
					break;
				default:
					break;
			}			
		}
	}
}
