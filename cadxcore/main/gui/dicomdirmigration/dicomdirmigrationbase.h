///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  8 2010)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dicomdirmigrationbase__
#define __dicomdirmigrationbase__

#include <wx/intl.h>

class BodyPanel;
class SubTitledPanel;
class SubTitledPanelSeparatorV;
class TitledPanel;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/checkbox.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

namespace GNC
{
namespace GUI
{

///////////////////////////////////////////////////////////////////////////////
/// Class DicomDirMigrationDialogBase
///////////////////////////////////////////////////////////////////////////////
class DicomDirMigrationDialogBase : public wxDialog
{
private:

protected:
        BodyPanel* m_pBody;
        TitledPanel* m_pMainPanel;
        wxBoxSizer* m_pSizerOld;
        wxStaticText* m_pLabelVersion;
        SubTitledPanelSeparatorV* m_pSep1;
        SubTitledPanel* m_panelMigration;
        wxStaticText* m_staticText31;
        wxCheckBox* m_pRemoveFiles;
        wxButton* m_button5;
        SubTitledPanelSeparatorV* m_panelMigrationSep;
        SubTitledPanel* m_panel7;
        wxStaticText* m_staticText3;
        wxButton* m_button4;

        // Virtual event handlers, overide them in your derived class
        virtual void OnMigrate( wxCommandEvent& event )
        {
                event.Skip();
        }
        virtual void OnSelectOther( wxCommandEvent& event )
        {
                event.Skip();
        }


public:

        DicomDirMigrationDialogBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("DICOM Dir configuration"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxCAPTION|wxCLOSE_BOX|wxRESIZE_BORDER|wxSTAY_ON_TOP|wxSYSTEM_MENU );
        ~DicomDirMigrationDialogBase();

};

} // namespace GUI
} // namespace GNC

#endif //__dicomdirmigrationbase__
