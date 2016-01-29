/*
 *  
 *  $Id: metricgridtool.cpp $
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

#include "metricgridtool.h"
#if defined(ENABLE_METRICGRIDTOOL)
#include <widgets/wrejilla.h>
#include <wx/aui/aui.h>
#include <wx/spinctrl.h>
#include <wx/dialog.h>
#include <wx/statbox.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/menu.h>
#include <wx/choice.h>


#include <api/ievento.h>
#include <eventos/render.h>
#include <api/controllers/ieventscontroller.h>
#include <export/contracts/iwidgetscontract.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtk/vtkginkgoimageviewer.h>

#include <api/iwidgetsmanager.h>
#include <resources/ginkgoresourcesmanager.h>


#define ACTIVAR_REJILLA  1241
#define ESCALA_REJILLA 1242

namespace GNC {
	namespace GUI {
		class DialogoRejilla: public wxDialog {
		public:
			typedef enum TMetrica {
				TM_UM = 0,
				TM_MM,
				TM_CM,
				TM_M
			} TMetrica;

			DialogoRejilla(wxWindow* pParent, GNC::MetricGridTool* pHerramienta):
				wxDialog(pParent,wxID_ANY,_("Grid"),wxDefaultPosition,wxDefaultSize,wxDEFAULT_DIALOG_STYLE)
			{
				m_pHerramienta=pHerramienta;

				this->SetSizeHints( wxDefaultSize, wxDefaultSize );

				wxBoxSizer* bSizer5;
				bSizer5 = new wxBoxSizer( wxVERTICAL );

				wxStaticBoxSizer* sbSizer2;
				sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Grid Scale") ), wxVERTICAL );

				wxFlexGridSizer* fgSizer1;
				fgSizer1 = new wxFlexGridSizer( 2, 3, 0, 0 );
				fgSizer1->SetFlexibleDirection( wxBOTH );
				fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

				wxStaticText* m_staticText4 = new wxStaticText( this, wxID_ANY, _("Scale X"), wxDefaultPosition, wxDefaultSize, 0 );
				fgSizer1->Add(m_staticText4,0,wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL,5);

				m_pSpinEscalaX = new wxSpinCtrl( this, wxID_ANY, wxT("50"), wxDefaultPosition, wxSize(60,-1), wxSP_ARROW_KEYS, 1, 10000, 50 );
				m_pSpinEscalaX->SetToolTip(_("X axis scale"));
				fgSizer1->Add(m_pSpinEscalaX,0,wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL,5);

				std::string micrometros("µm.");
				wxString choices[] = {wxString::FromUTF8(micrometros.c_str()),wxT("mm."),wxT("cm."),wxT("m.")};
				int choice2NChoices = sizeof( choices ) / sizeof( wxString );
				m_pChoiceX = new wxChoice(this,wxID_ANY, wxDefaultPosition, wxDefaultSize, choice2NChoices, choices);
				fgSizer1->Add(m_pChoiceX,0,wxALL|wxALIGN_CENTER_VERTICAL,5);

				wxStaticText* m_staticText5 = new wxStaticText( this, wxID_ANY, _("Scale Y"), wxDefaultPosition, wxDefaultSize, 0 );
				fgSizer1->Add(m_staticText5,0,wxALL|wxEXPAND,5);

				m_pSpinEscalaY = new wxSpinCtrl( this, wxID_ANY, wxT("50"), wxDefaultPosition, wxSize(60,-1), wxSP_ARROW_KEYS, 1, 10000, 50 );
				m_pSpinEscalaY->SetToolTip(_("Y axis scale"));
				fgSizer1->Add(m_pSpinEscalaY,0,wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL,5);

				m_pChoiceY = new wxChoice(this,wxID_ANY, wxDefaultPosition, wxDefaultSize, choice2NChoices, choices);
				fgSizer1->Add(m_pChoiceY,0,wxALL|wxALIGN_CENTER_VERTICAL,5);

				sbSizer2->Add( fgSizer1, 1, wxEXPAND|wxALL, 5 );

				bSizer5->Add( sbSizer2, 1,wxALL|wxEXPAND, 5 );

				wxButton* m_button3 = new wxButton( this, wxID_OK, _("Ok"), wxDefaultPosition, wxDefaultSize, 0 );
				m_button3->SetDefault();
				bSizer5->Add( m_button3, 0, wxALL|wxALIGN_RIGHT, 5 );

				this->SetSizer( bSizer5 );
				this->Layout();
				bSizer5->Fit( this );

				this->Centre( wxBOTH );


				double escalaX = m_pHerramienta->GetScaleX();
				m_pChoiceX->Select((int)AjustaAMetricaApropiada(escalaX));
				m_pSpinEscalaX->SetValue(escalaX);
				double escalaY = m_pHerramienta->GetScaleY();
				m_pChoiceY->Select((int)AjustaAMetricaApropiada(escalaY));
				m_pSpinEscalaY->SetValue(escalaY);

				m_pSpinEscalaX->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DialogoRejilla::OnSpinCtrlX ), NULL, this );
				m_pSpinEscalaY->Connect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DialogoRejilla::OnSpinCtrlY ), NULL, this );
				m_pChoiceX->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoRejilla::OnEscalaX), NULL, this);
				m_pChoiceY->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoRejilla::OnEscalaY), NULL, this);
			}

			~DialogoRejilla()
			{
				m_pSpinEscalaX->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DialogoRejilla::OnSpinCtrlX ), NULL, this );
				m_pSpinEscalaX->Disconnect( wxEVT_COMMAND_SPINCTRL_UPDATED, wxSpinEventHandler( DialogoRejilla::OnSpinCtrlY ), NULL, this );
				m_pChoiceX->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoRejilla::OnEscalaX), NULL, this);
				m_pChoiceY->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( DialogoRejilla::OnEscalaY), NULL, this);
				m_pHerramienta = NULL;
			}

			TMetrica AjustaAMetricaApropiada(double &escala)
			{
				if (escala > 10000.0f) {
					escala = (int)(escala / 1000.0f);
					return TM_M;
				}
				else if (escala > 100.0f) {
					escala = (int)(escala / 10.0f);
					return TM_CM;
				}
				else if (escala < 10.0f) {
					escala = (int)(escala * 1000.0f);
					return TM_UM;
				}
				else {
					escala = (int)(escala);
					return TM_MM;
				}
			}

			double TransformToMM(double valor, TMetrica escala)
			{
				switch (escala) {
					case TM_M:
						return valor * 1000.0;
					case TM_CM:
						return valor*10.0;
					case TM_UM:
						return valor / 1000.0;
					case TM_MM:
						return valor;
				}
				return valor;
			}

			void OnSpinCtrlX( wxSpinEvent&  )
			{
				const double valor = TransformToMM(m_pSpinEscalaX->GetValue(), (TMetrica)m_pChoiceX->GetSelection());
				m_pHerramienta->SetScaleX(valor);
			}

			void OnEscalaX( wxCommandEvent &)
			{
				const double valor = TransformToMM(m_pSpinEscalaX->GetValue(), (TMetrica)m_pChoiceX->GetSelection());
				m_pHerramienta->SetScaleX(valor);
			}

			void OnSpinCtrlY( wxSpinEvent&  )
			{
				const double valor = TransformToMM(m_pSpinEscalaY->GetValue(), (TMetrica)m_pChoiceY->GetSelection());
				m_pHerramienta->SetScaleY(valor);
			}

			void OnEscalaY( wxCommandEvent &)
			{
				const double valor = TransformToMM(m_pSpinEscalaY->GetValue(), (TMetrica)m_pChoiceY->GetSelection());
				m_pHerramienta->SetScaleY(valor);
			}

			wxSpinCtrl* m_pSpinEscalaX;
			wxSpinCtrl* m_pSpinEscalaY;
			wxChoice* m_pChoiceX;
			wxChoice* m_pChoiceY;
			GNC::MetricGridTool* m_pHerramienta;
		};


		class MenuMetricGridTool : public wxMenu {
		public:
			MenuMetricGridTool(wxEvtHandler* pParent, GNC::MetricGridTool* pHerramienta):wxMenu() {
				m_pParent = pParent;
				m_pHerramienta=pHerramienta;
				wxMenuItem* pItemVer = new wxMenuItem(this,ACTIVAR_REJILLA,_("Show grid"),_("Show grid"),wxITEM_NORMAL);
				wxMenuItem* pItemEscala = new wxMenuItem(this,ESCALA_REJILLA,_("Configure Grid ..."),_("Configure Grid"),wxITEM_NORMAL);
				#ifdef __WXMSW__
					pItemVer->SetBitmaps(GinkgoResourcesManager::GridBar::GetIcoVerRejilla());
				#else
					pItemVer->SetBitmap(GinkgoResourcesManager::GridBar::GetIcoVerRejilla());
				#endif
				m_pParent->Connect(pItemVer->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuMetricGridTool::OnActivarRejillaClick),NULL,this);
				m_pParent->Connect(pItemVer->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIMostrar),NULL,this);
				m_pParent->Connect(pItemEscala->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuMetricGridTool::OnEscalaClick),NULL,this);
				m_pParent->Connect(pItemEscala->GetId(), wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIHerramienta),NULL,this);
				Append(pItemVer);
				Append(pItemEscala);

				m_pParent->Connect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIHerramienta),NULL,this);
			}

			~MenuMetricGridTool() {
				m_pParent->Disconnect(ACTIVAR_REJILLA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuMetricGridTool::OnActivarRejillaClick),NULL,this);
				m_pParent->Disconnect(ESCALA_REJILLA, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuMetricGridTool::OnEscalaClick),NULL,this);
				m_pParent->Disconnect(ACTIVAR_REJILLA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIMostrar),NULL,this);
				m_pParent->Disconnect(ESCALA_REJILLA, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIHerramienta),NULL,this);

				m_pParent->Disconnect(m_pHerramienta->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuMetricGridTool::OnUpdateUIHerramienta),NULL,this);

				m_pParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnActivarRejillaClick( wxCommandEvent& event )
			{
				m_pHerramienta->ToggleGrid();
				event.Skip(false);
			}

			void OnEscalaClick( wxCommandEvent& event )
			{
				DialogoRejilla dlg(NULL,m_pHerramienta);
				dlg.ShowModal();
				event.Skip(false);
			}

			void OnUpdateUIMostrar(wxUpdateUIEvent& event) {
				if(m_pHerramienta->IsVisible()) {
					event.SetText(_("Hide Grid"));
				} else {
					event.SetText(_("Show Grid"));
				}
			}

			void OnUpdateUIHerramienta(wxUpdateUIEvent& event) {
				event.Enable(m_pHerramienta->IsEnabled());
			}

			GNC::MetricGridTool* m_pHerramienta;
			wxEvtHandler* m_pParent;
		};
	}
}





GNC::GCS::ITool* GNC::MetricGridTool::NewTool()
{
	return new GNC::MetricGridTool();
}

GNC::MetricGridTool::MetricGridTool()
{
}
GNC::MetricGridTool::~MetricGridTool()
{
}

bool GNC::MetricGridTool::ExecuteAction()
{
	ToggleGrid();
	return true;
}

void GNC::MetricGridTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		wxMenu* pGridMenu = new GNC::GUI::MenuMetricGridTool(pParent,this);
		pMenu->Append(ID,wxString::FromUTF8(Name.c_str()),pGridMenu);		
		pMenu->Enable(ID,Enabled);
	}
}

bool GNC::MetricGridTool::IsVisible()
{
	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this);
	bool visible = pRejillaBuilder->IsRejillaVisible();
	delete pRejillaBuilder;
	return visible;
}

void GNC::MetricGridTool::ToggleGrid()
{
	double spacing[3] = {1.0,1.0,1.0};
	int dimensions[3] = {0,0,0};
	WidgetsContract->GetViewerActivo()->GetSpacing(spacing);
	WidgetsContract->GetViewerActivo()->GetDimensions(dimensions);

	double escalaXDefecto = spacing[0] * dimensions [0] / 10;
	double escalaYDefecto = spacing[1] * dimensions [1] / 10;

	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this, escalaXDefecto, escalaYDefecto);
	pRejillaBuilder->ActivarRejilla(!pRejillaBuilder->IsRejillaVisible());
	delete pRejillaBuilder;
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

void GNC::MetricGridTool::SetScaleX(double escala)
{
	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this);
	pRejillaBuilder->ActivarRejilla(true);
	pRejillaBuilder->SetEscalaX(escala);
	delete pRejillaBuilder;
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

void GNC::MetricGridTool::SetScaleY(double escala)
{
	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this);
	pRejillaBuilder->ActivarRejilla(true);
	pRejillaBuilder->SetEscalaY(escala);
	delete pRejillaBuilder;
	GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(WidgetsContract->GetManager()->GetVista()));
}

double GNC::MetricGridTool::GetScaleX()
{
	double scale = 50.0;
	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this);
	scale = pRejillaBuilder->GetEscalaX();
	delete pRejillaBuilder;
	return scale;
}

double GNC::MetricGridTool::GetScaleY()
{
	double scale = 50.0;
	GNC::GCS::Widgets::WRejillaBuilder* pRejillaBuilder = new GNC::GCS::Widgets::WRejillaBuilder(WidgetsContract->GetManager(), (unsigned long)this);
	scale = pRejillaBuilder->GetEscalaY();
	delete pRejillaBuilder;
	return scale;
}


#endif
