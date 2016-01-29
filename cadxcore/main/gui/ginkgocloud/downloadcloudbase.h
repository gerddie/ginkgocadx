///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DOWNLOADCLOUDBASE_H__
#define __DOWNLOADCLOUDBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class HeaderPanel;
class TitledPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/combobox.h>
#include <wx/srchctrl.h>
#include <wx/radiobut.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/statbox.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
	namespace GUI
	{
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class OpenGinkgoCloudDialogBase
		///////////////////////////////////////////////////////////////////////////////
		class OpenGinkgoCloudDialogBase : public wxFrame 
		{
			private:
			
			protected:
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBody;
				TitledPanel* m_pPanelSuperior;
				wxStaticText* m_staticText6;
				wxTextCtrl* m_pFileLink;
				FooterPanel* m_pFooter;
				wxButton* m_buttonAceptar;
				wxButton* m_pButtonCancel;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnKeyUp( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnAccept( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnCancel( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				OpenGinkgoCloudDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Open Ginkgo Cloud"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,350 ), long style = wxCAPTION|wxCLOSE_BOX|wxFRAME_FLOAT_ON_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );
				
				~OpenGinkgoCloudDialogBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class GinkgoCloudSearchDialogBase
		///////////////////////////////////////////////////////////////////////////////
		class GinkgoCloudSearchDialogBase : public wxDialog 
		{
			private:
			
			protected:
				wxBoxSizer* m_pMainSizer;
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBodyPanel;
				TitledPanel* m_pPanelPACS;
				wxStaticText* m_pPatientLabelStr;
				wxComboBox* m_pFieldCombo;
				wxPanel* m_pSearchPanel;
				wxSearchCtrl* m_pTextControlField;
				wxStaticText* m_pLabelRestricted;
				wxBoxSizer* m_pSizerAnimation;
				wxRadioButton* m_pAnyDate;
				wxRadioButton* m_pToday;
				wxRadioButton* m_pYesterday;
				wxRadioButton* m_pLastWeek;
				wxRadioButton* m_pLastMonth;
				wxRadioButton* m_pLastThreeMonths;
				wxRadioButton* m_pBetween;
				wxDatePickerCtrl* m_pTextControlFechaDesde;
				wxDatePickerCtrl* m_pTextControlFechaHasta;
				wxGridSizer* m_pModalitySizer;
				wxBoxSizer* m_pSizerDataGrid;
				wxButton* m_pBDescargar;
				FooterPanel* m_pFooter;
				wxButton* m_pBotonClose;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
				virtual void OnKeyDownFormulario( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnComboField( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnCancelClick( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnBusquedaClick( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnFechaDesdeDateChanged( wxDateEvent& event ) { event.Skip(); }
				virtual void OnFechaHastaDateChanged( wxDateEvent& event ) { event.Skip(); }
				virtual void OnDescargarClick( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnCloseClick( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				GinkgoCloudSearchDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Ginkgo Cloud search"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 1024,575 ), long style = wxCAPTION|wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
				~GinkgoCloudSearchDialogBase();
			
		};
		
		///////////////////////////////////////////////////////////////////////////////
		/// Class FileLinkDialogBase
		///////////////////////////////////////////////////////////////////////////////
		class FileLinkDialogBase : public wxDialog 
		{
			private:
			
			protected:
				HeaderPanel* m_pHeader;
				BodyPanel* m_pBody;
				TitledPanel* m_pPanelSuperior;
				wxStaticText* m_staticText6;
				wxTextCtrl* m_pFileLink;
				wxStaticText* m_staticText7;
				FooterPanel* m_pFooter;
				wxButton* m_button7;
				wxButton* m_pCopyClipboard;
				wxButton* m_buttonAceptar;
				
				// Virtual event handlers, overide them in your derived class
				virtual void OnKeyUp( wxKeyEvent& event ) { event.Skip(); }
				virtual void OnSendMail( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnCopyClipboard( wxCommandEvent& event ) { event.Skip(); }
				virtual void OnAccept( wxCommandEvent& event ) { event.Skip(); }
				
			
			public:
				
				FileLinkDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 650,350 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
				~FileLinkDialogBase();
			
		};
		
	} // namespace GUI
} // namespace GNC

#endif //__DOWNLOADCLOUDBASE_H__
