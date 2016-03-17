///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __acceptlicensedialogbase__
#define __acceptlicensedialogbase__

#include <wx/intl.h>

class BodyPanel;
class FooterPanel;
class HeaderPanel;

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class AcceptLicenseDialogBase
///////////////////////////////////////////////////////////////////////////////
class AcceptLicenseDialogBase : public wxDialog
{
private:

protected:
        HeaderPanel* m_pHeader;
        BodyPanel* m_pBody;
        wxTextCtrl* m_pTextoLicencia;
        FooterPanel* m_pFooter;
        wxStaticText* m_staticText1;
        wxCheckBox* m_pCheckForUpdates;
        wxCheckBox* m_pCheckAnonUsageStats;

        wxButton* m_buttonAceptar;
        wxButton* m_buttonCancelar;

        // Virtual event handlers, overide them in your derived class
        virtual void OnAnyKeyDown( wxKeyEvent& event )
        {
                event.Skip();
        }
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }
        virtual void OnAceptarClick( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        AcceptLicenseDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("License agreement"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 800,600 ), long style = wxCLOSE_BOX|wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~AcceptLicenseDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__acceptlicensedialogbase__
