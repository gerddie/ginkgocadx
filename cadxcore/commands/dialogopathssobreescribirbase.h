///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dialogopathssobreescribirbase__
#define __dialogopathssobreescribirbase__

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
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class OverwriteDialogBase
///////////////////////////////////////////////////////////////////////////////
class OverwriteDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        wxStaticText* m_staticText1;
        FooterPanel* m_pFooter;

        wxButton* m_button1;
        wxButton* m_button2;

public:
        TitledPanel* m_pPanelOverwrite;
        wxStaticText* m_pTextMessage;
        wxListBox* m_pPathList;

        OverwriteDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Overwrite Paths"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~OverwriteDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__dialogopathssobreescribirbase__
