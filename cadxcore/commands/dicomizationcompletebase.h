///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dicomizationcompletebase__
#define __dicomizationcompletebase__

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
#include <wx/sizer.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class DicomizationCompleteBase
///////////////////////////////////////////////////////////////////////////////
class DicomizationCompleteBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pBodyPanel;
        TitledPanel* m_pPanelTitled;
        wxStaticText* m_staticText8;
        FooterPanel* m_panel12;

        wxButton* m_button5;

        // Virtual event handlers, overide them in your derived class
        virtual void OnYesClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:
        wxCheckBox* m_pCheckNotShow;

        DicomizationCompleteBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Dicomization complete"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~DicomizationCompleteBase();

};

} // namespace GUI
} // namespace GNC

#endif //__dicomizationcompletebase__
