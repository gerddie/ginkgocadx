///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __paneltagsbase__
#define __paneltagsbase__

#include <wx/intl.h>

class FooterPanel;

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>
#ifdef __VISUALC__
#include <wx/link_additions.h>
#endif //__VISUALC__
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class PanelTagsBase
///////////////////////////////////////////////////////////////////////////////
class PanelTagsBase : public wxDialog
{
private:

protected:
        wxBoxSizer* m_pMainSizer;
        wxPropertyGrid* m_pTags;
        FooterPanel* m_panel1;
        wxButton* m_button1;

        // Virtual event handlers, overide them in your derived class
        virtual void OnClose( wxCloseEvent& event )
        {
                event.Skip();
        }
        virtual void OnKeyDown( wxKeyEvent& event )
        {
                event.Skip();
        }


public:

        PanelTagsBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("DICOM Inspector"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxMAXIMIZE_BOX|wxRESIZE_BORDER|wxSYSTEM_MENU );
        ~PanelTagsBase();

};

} // namespace GUI
} // namespace GNC

#endif //__paneltagsbase__
