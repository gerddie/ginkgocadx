///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version May  4 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __progresobase__
#define __progresobase__

#include <wx/intl.h>

class GinkgoGauge;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/scrolwin.h>
#include <wx/panel.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PanelTareasBase
///////////////////////////////////////////////////////////////////////////////
class PanelTareasBase : public wxPanel
{
private:

protected:

public:
        wxScrolledWindow* m_pTasks;
        wxBoxSizer* m_pSizerTareas;
        wxStaticText* m_pSinTareas;

        PanelTareasBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelTareasBase();

};

///////////////////////////////////////////////////////////////////////////////
/// Class PanelTareaBase
///////////////////////////////////////////////////////////////////////////////
class PanelTareaBase : public wxPanel
{
private:

protected:

public:
        GinkgoGauge* m_pProgress;

        PanelTareaBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxTAB_TRAVERSAL );
        ~PanelTareaBase();

};

#endif //__progresobase__
