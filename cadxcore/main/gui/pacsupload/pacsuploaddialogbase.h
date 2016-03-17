///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PACSUPLOADDIALOGBASE_H__
#define __PACSUPLOADDIALOGBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class BodyPanel;
class FooterPanel;
class TitledPanel;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/sizer.h>
#include <wx/checklst.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class PACSUploadDialogBase
///////////////////////////////////////////////////////////////////////////////
class PACSUploadDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pPanelMessage;
        wxStaticText* m_pLabel;
        wxChoice* m_pServer;
        wxCheckListBox* m_pSeriesList;
        FooterPanel* m_pFooter;
        wxButton* m_pOkButton;
        wxButton* m_button5;

public:

        PACSUploadDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Upload study"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 712,495 ), long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~PACSUploadDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__PACSUPLOADDIALOGBASE_H__
