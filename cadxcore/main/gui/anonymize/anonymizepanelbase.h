///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __ANONYMIZEPANELBASE_H__
#define __ANONYMIZEPANELBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class HeaderPanel;
class SubTitledPanel;
class SubTitledPanelSeparatorV;
class TitledPanel;

#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/propgrid/propgrid.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valgen.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class AnonymizePanelBase
///////////////////////////////////////////////////////////////////////////////
class AnonymizePanelBase : public wxPanel
{
private:

protected:
        SubTitledPanel* m_pPanelCampos;
        wxCheckBox* m_pCheckNombrePaciente;
        wxCheckBox* m_pCheckIdPaciente;
        wxCheckBox* m_pCheckInstituciones;
        wxCheckBox* m_pReferringPhysician;
        wxCheckBox* m_pCheckComentarios;
        wxCheckBox* m_pCheckAtributosPrivadosGinkgo;
        SubTitledPanelSeparatorV* m_panel5;
        SubTitledPanel* m_pTagsPanel;
        wxPropertyGrid* m_pTagsList;
        wxButton* m_button4;

        // Virtual event handlers, overide them in your derived class
        virtual void OnCheckNombreApellidos( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckIdPaciente( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckInstituciones( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckPhysician( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCheckComentarios( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnAddNewTag( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        AnonymizePanelBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxTAB_TRAVERSAL );
        ~AnonymizePanelBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AnonymizeDialogBase
///////////////////////////////////////////////////////////////////////////////
class AnonymizeDialogBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pBody;
        FooterPanel* m_pFooter;
        wxButton* m_pOverwrite;
        wxButton* m_pCreateNew;
        wxButton* m_pCancel;

        // Virtual event handlers, overide them in your derived class
        virtual void OnOverwriteClick( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnCreateNewClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:
        wxBoxSizer* m_pBodySizer;

        AnonymizeDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Anonymize"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~AnonymizeDialogBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AnonymizeAddTagDialogBase
///////////////////////////////////////////////////////////////////////////////
class AnonymizeAddTagDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pTitled;
        wxChoice* m_pTag;
        wxStaticText* m_staticText1;
        wxStaticText* m_staticText2;
        wxStaticText* m_staticText3;
        wxTextCtrl* m_pTextGroup;
        wxTextCtrl* m_pTextElement;
        wxTextCtrl* m_pTextValue;
        FooterPanel* m_pFooter;
        wxButton* m_button5;
        wxButton* m_button6;

        // Virtual event handlers, overide them in your derived class
        virtual void OnTagSelection( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnOkClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        AnonymizeAddTagDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add DICOM tag"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 550,280 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~AnonymizeAddTagDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__ANONYMIZEPANELBASE_H__
