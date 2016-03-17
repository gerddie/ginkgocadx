///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __wxpropiedadesbase__
#define __wxpropiedadesbase__

#include <wx/intl.h>

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/panel.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class wxPropiedadesBase
///////////////////////////////////////////////////////////////////////////////
class wxPropiedadesBase : public wxDialog
{
private:

protected:
        wxNotebook* m_pNotebook;
        wxScrolledWindow* m_pGeneral;
        wxBoxSizer* m_pSizerGeneral;
        wxStaticBitmap* m_pIcono;
        wxStaticText* m_pNombreWidget;
        wxPanel* m_pPanelBotones;


        wxButton* m_pBCerrar;

        // Virtual event handlers, overide them in your derived class
        virtual void OnClose( wxCloseEvent& event )
        {
                event.Skip();
        }
        virtual void OnSize( wxSizeEvent& event )
        {
                event.Skip();
        }


public:

        wxPropiedadesBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Properties of ..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 385,441 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
        ~wxPropiedadesBase();

};

} // namespace GUI
} // namespace GNC

#endif //__wxpropiedadesbase__
