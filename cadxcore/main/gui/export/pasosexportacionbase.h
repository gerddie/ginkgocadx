///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __PASOSEXPORTACIONBASE_H__
#define __PASOSEXPORTACIONBASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class TitledPanel;
class TitledPanelSeparatorV;

#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/choice.h>
#include <wx/checkbox.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/slider.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class ParametrosPrincipalesBase
///////////////////////////////////////////////////////////////////////////////
class ParametrosPrincipalesBase : public wxPanel 
{
	private:
	
	protected:
		TitledPanel* m_pPanelProfile;
		wxStaticText* m_staticText15;
		wxChoice* m_pFormat;
		wxCheckBox* m_pAllFiles;
		TitledPanelSeparatorV* m_pSep;
		TitledPanel* m_pPanelJpeg;
		wxStaticText* m_staticText6;
		wxSlider* m_pSliderCalidad;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoiceFormatoDestino( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ParametrosPrincipalesBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 765,350 ), long style = wxTAB_TRAVERSAL ); 
		~ParametrosPrincipalesBase();
	
};

#endif //__PASOSEXPORTACIONBASE_H__
