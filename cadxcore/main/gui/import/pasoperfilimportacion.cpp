/*
 * This file is part of Ginkgo CADx
 *
 * Copyright (c) 2015-2016 Gert Wollny
 * Copyright (c) 2008-2014 MetaEmotion S.L. All rights reserved.
 *
 * Ginkgo CADx is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as 
 * published by the Free Software Foundation; version 3. 
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with Ginkgo CADx; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "pasoperfilimportacion.h"
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <vector>
#include <sstream>
#include <wx/file.h>
#include <wx/dir.h>
#include <main/controllers/controladorextensiones.h>
#include <api/internationalization/internationalization.h>

#include <api/ientorno.h>
#include <wx/dcbuffer.h>
#include <wx/wxginkgotoolbar.h>

namespace GNC {
	namespace GUI {

		class ImportProfileToolBar : public wxGinkgoToolBar
		{
		public:
			ImportProfileToolBar(PasoPefilImportacion* pParent, const wxColor& color = wxColor(255, 255, 255)) : wxGinkgoToolBar(pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_TEXT),
				m_pParent(pParent),
				idTool(1)
			{
				SetToolBitmapSize(wxSize(64,64));
				wxGinkgoToolBarArt* pToolArt= new wxGinkgoToolBarArt(color);
				SetBackgroundColour(wxColour(255,255,255));
				SetArtProvider(pToolArt);
				
				this->Connect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ImportProfileToolBar::OnToolClick),NULL, this );
				this->Connect( wxID_ANY, wxEVT_LEFT_DCLICK, wxCommandEventHandler(ImportProfileToolBar::OnToolDClick),NULL, this );
			}

			~ImportProfileToolBar()
			{
				this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(ImportProfileToolBar::OnToolClick),NULL, this );
				this->Disconnect( wxID_ANY, wxEVT_LEFT_DCLICK, wxCommandEventHandler(ImportProfileToolBar::OnToolDClick),NULL, this );
			}

			void AddTool(const std::string& toolTip, const wxBitmap& icon) 
			{
				wxGinkgoToolBar::AddTool(idTool++, wxString::FromUTF8(toolTip.c_str()), icon, wxString::FromUTF8(toolTip.c_str()), wxITEM_CHECK);
			}

			void SelectTool(int defaultDicomizator)
			{
				for (int i = 1; i < idTool; ++i) {
					ForceToggleTool(i, i == defaultDicomizator);
				}
				if ((defaultDicomizator <1 || defaultDicomizator > idTool) && idTool > 1)
				{
					ForceToggleTool(1,true);
				}
			}

						
			void OnToolClick(wxCommandEvent& event)
			{
				for (int i = 1; i < idTool; ++i) {
					ForceToggleTool(i, i == event.GetId());
				}
				Refresh(true);
				event.Skip(false);
			}

						
			void OnToolDClick(wxCommandEvent& /*event*/)
			{
				m_pParent->SiguientePaso();
			}

			int GetIdSelected() 
			{
				for (int i = 1; i < idTool; ++i) {
					if (wxAuiToolBar::GetToolToggled(i)) {
						return i;
					}
				}
				return -1;
			}

			std::string GetNameSelected() 
			{
				for (int i = 1; i < idTool; ++i) {
					if (wxAuiToolBar::GetToolToggled(i)) {
						return std::string(GetToolLabel(i).ToUTF8());
					}
				}
				return "";
			}

			PasoPefilImportacion* m_pParent;
			int idTool;
		};

		PasoPefilImportacion::PasoPefilImportacion(wxWindow* pParent,IWizard* pWizard):PasoPerfilImportacionBase(pParent),
			IPasoWizard(pWizard),
			m_pToolBar(new ImportProfileToolBar(this))
		{
			m_pControladorModulo = NULL;

			GetSizer()->Add( 0, 0, 1, wxEXPAND, 5 );
			GetSizer()->Add(m_pToolBar, 0, wxALIGN_CENTER_HORIZONTAL);
			GetSizer()->Add( 0, 0, 1, wxEXPAND, 5 );
			

			GNC::ControladorExtensiones::ListaModulos listaModulos = ControladorExtensiones::Instance()->Modulos();
			for(GNC::ControladorExtensiones::IteradorListaModulos it=listaModulos.begin();it!=listaModulos.end();++it){
				GNC::GCS::IModuleController* modulo = (*it).second;
				if(modulo->SoportaImportacion()) {
					m_pToolBar->AddTool(modulo->GetImporterDescription(), modulo->GetImporterIcon());
				}
			}

			{
				int defaultDicomizator;
				GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Importacion", "DefaultDicomizator", defaultDicomizator);				
				m_pToolBar->SelectTool(defaultDicomizator);
			}

			m_pToolBar->Realize();
			Layout();
		}

		PasoPefilImportacion::~PasoPefilImportacion()
		{
			m_pControladorModulo = NULL;
		}

//region "Metodos heredados de Ipasowizard"

		std::string PasoPefilImportacion::GetTitle(){
			return _Std("Selecting Import Profile");
		}

		std::string PasoPefilImportacion::GetSubTitle(){
			return _Std("You must select the profile you want to use import");
		}

		bool PasoPefilImportacion::Siguiente(){
			return true;
		}

		bool PasoPefilImportacion::Anterior(){
			return false;
		}

		bool PasoPefilImportacion::Cancelar(){
			return true;
		}

		bool PasoPefilImportacion::Validar(){

			if(m_pToolBar->GetIdSelected()<=0){
				wxMessageBox(_("You must select the import profile"), _("Info"),
					 wxOK | wxICON_INFORMATION, this);
				return false;
			}

			std::string nombreModulo(m_pToolBar->GetNameSelected());

			GNC::ControladorExtensiones::ListaModulos listaModulos = ControladorExtensiones::Instance()->Modulos();
			for(GNC::ControladorExtensiones::IteradorListaModulos it=listaModulos.begin();it!=listaModulos.end();++it){
				GNC::GCS::IModuleController* modulo=(*it).second;
				if(modulo->GetImporterDescription() == nombreModulo){
					m_pControladorModulo = modulo;

					GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/Importacion", "DefaultDicomizator", m_pToolBar->GetIdSelected());				
					return true;
				}
			}
			//
			wxMessageBox(_("Unexpected error, select the module again"), _("Info"),
					 wxOK | wxICON_INFORMATION, this);
			return false;
		}
	//endregion
	}
}
