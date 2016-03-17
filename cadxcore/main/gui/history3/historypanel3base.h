///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HISTORYPANEL3BASE_H__
#define __HISTORYPANEL3BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class TitledPanel;

#include <wx/string.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/tglbtn.h>
#include <wx/radiobut.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/thumbnailctrl/thumbnailctrl.h>
#undef __DEPRECATED
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <wxVTKRenderWindowInteractor.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <wx/splitter.h>
#include <wx/frame.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listbox.h>
#include <wx/checkbox.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class HistoryPanel3Base
///////////////////////////////////////////////////////////////////////////////
class HistoryPanel3Base : public wxFrame
{
private:

protected:
        wxBoxSizer* m_pSizerToolBar;
        wxPanel* m_pSearchPanel;
        wxStaticText* m_pPatientLabelStr;
        wxComboBox* m_pPatientCombo;
        wxToggleButton* m_pAdvancedSearchButton;
        wxPanel* m_pAdvancedSearchPanel;
        wxRadioButton* m_pAnyDate;
        wxRadioButton* m_pTodayAM;
        wxRadioButton* m_pTodayPM;
        wxRadioButton* m_pToday;
        wxRadioButton* m_pYesterday;
        wxRadioButton* m_pLastWeek;
        wxRadioButton* m_pLastMonth;
        wxRadioButton* m_pLastThreeMonths;
        wxRadioButton* m_pBetween;
        wxDatePickerCtrl* m_pTextControlFechaDesde;
        wxDatePickerCtrl* m_pTextControlFechaHasta;
        wxGridSizer* m_pModalitySizer;
        wxSplitterWindow* m_splitter1;
        wxPanel* m_pPanelResults;
        wxBoxSizer* m_pTreeListSizer;
        wxPanel* m_pPanelPreviews;
        wxThumbnailCtrl* m_pThumbnails;
        vtkSmartPointer<wxVTKRenderWindowInteractor> ViewInteractor2D;
        vtkSmartPointer<vtkGinkgoImageViewer> ViewImage2D;
        wxPanel* m_pHideInteractorPanel;

        // Virtual event handlers, overide them in your derived class
        virtual void OnClose( wxCloseEvent& event )
        {
                event.Skip();
        }
        virtual void OnHistoryPanelIdle( wxIdleEvent& event )
        {
                event.Skip();
        }
        virtual void OnAdvancedSearchToggled( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnKeyDownFormulario( wxKeyEvent& event )
        {
                event.Skip();
        }
        virtual void OnSearchChange( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnDateChanged( wxDateEvent& event )
        {
                event.Skip();
        }


public:
        wxMenuBar* m_pMenuBar;

        HistoryPanel3Base( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Medical history"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU|wxTAB_TRAVERSAL );

        ~HistoryPanel3Base();

        void m_splitter1OnIdle( wxIdleEvent& )
        {
                m_splitter1->SetSashPosition( 0 );
                m_splitter1->Disconnect( wxEVT_IDLE, wxIdleEventHandler( HistoryPanel3Base::m_splitter1OnIdle ), NULL, this );
        }

};

///////////////////////////////////////////////////////////////////////////////
/// Class ConfirmDeleteDialogBase
///////////////////////////////////////////////////////////////////////////////
class ConfirmDeleteDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_body;
        TitledPanel* m_pTitled;
        wxStaticText* m_pLabelEliminar;
        FooterPanel* m_pFooter;
        wxButton* m_pEliminar;
        wxButton* m_pCancelar;

        // Virtual event handlers, overide them in your derived class
        virtual void OnEliminarClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCancelarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        ConfirmDeleteDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Confirm removal"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~ConfirmDeleteDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class OpenWithDialogBase
///////////////////////////////////////////////////////////////////////////////
class OpenWithDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelTitulo;
        wxListBox* m_pListaVistas;
        wxCheckBox* m_pCheckUtilizarSiempre;
        FooterPanel* m_pFooter;
        wxButton* m_button3;
        wxButton* m_button4;

        // Virtual event handlers, overide them in your derived class
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        OpenWithDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Open With ..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~OpenWithDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class SynchronizeDialogBase
///////////////////////////////////////////////////////////////////////////////
class SynchronizeDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelMessage;
        wxStaticText* m_pLabel;
        FooterPanel* m_pFooter;
        wxButton* m_button4;
        wxButton* m_button5;

        // Virtual event handlers, overide them in your derived class
        virtual void OnSynchronize( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        SynchronizeDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Synchronize files"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~SynchronizeDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__HISTORYPANEL3BASE_H__
