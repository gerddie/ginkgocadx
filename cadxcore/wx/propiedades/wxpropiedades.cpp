/*
 *  
 *  $Id: wxpropiedades.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#include "wxpropiedades.h"
#include <sstream>
#include <wx/statline.h>
#include <wx/textctrl.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/internationalization/internationalization.h>


namespace GNC {
	namespace GUI {
		wxPropiedades::wxPropiedades( wxWindow* parent, const std::string& nombre, const TListaMapasPropiedades& listaMapas):wxPropiedadesBase(parent)
		{
			m_pIcono->SetBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo48x48());

			{
				std::stringstream ostr;
				ostr << _Std("Properties of ") <<nombre;
				SetTitle(wxString::FromUTF8(ostr.str().c_str()));
				m_pNombreWidget->SetLabel(wxString::FromUTF8(nombre.c_str()));
			}

			for(TListaMapasPropiedades::const_iterator itLista = listaMapas.begin(); itLista!= listaMapas.end(); itLista++) {
				//agrego la linea
				wxStaticLine* staticline = new wxStaticLine( m_pGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
				m_pSizerGeneral->Add( staticline, 0, wxEXPAND | wxALL, 5 );

				//creo el sizer
				wxFlexGridSizer* pSizer;
				pSizer = new wxFlexGridSizer( 0, 2, 0, 0 );
				pSizer->SetFlexibleDirection( wxBOTH );
				pSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
				pSizer->AddGrowableCol(1, 1);
				m_pSizerGeneral->Add( pSizer, 0, wxEXPAND|wxALL, 5 );

				for(TMapaPropiedades::const_iterator itMapa = (*itLista).begin(); itMapa!=(*itLista).end(); itMapa++) {
					std::stringstream ostr;
					ostr << (*itMapa).first << ":";
					//agrego los ids
					pSizer->Add(new wxStaticText(m_pGeneral,wxID_ANY,wxString::FromUTF8(ostr.str().c_str())), 0, wxEXPAND | wxALL, 2);				    
					wxTextCtrl* pText = new wxTextCtrl(m_pGeneral, wxID_ANY, wxString::FromUTF8((*itMapa).second.c_str()), wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxNO_BORDER);
					pText->SetBackgroundColour(m_pGeneral->GetBackgroundColour());
					pSizer->Add(pText, 1, wxEXPAND|wxALL, 2);
				}

			}
			m_pGeneral->SetScrollRate(5,5);
			m_pGeneral->FitInside();
			m_pGeneral->Layout();
			Layout();
		}

		wxPropiedades::~wxPropiedades()
		{
		}

		void wxPropiedades::AddPage(const wxString& titulo, wxPanel* panel)
		{
			m_pNotebook->AddPage(panel,titulo,false);
		}
	}
}

