/*
 *  
 *  $Id: wxwizardimportacionginkgo.cpp $
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
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include <wx/ginkgostyle/ginkgostyle.h>

#include "wxwizardimportacionginkgo.h"
#include "pasoperfilimportacion.h"

#include <api/imodelointegracion.h>
#include <main/entorno.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladorpermisos.h>
#include <api/internationalization/internationalization.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/gui/progress/statusbarprogreso.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/windowattributescontroller.h>

#include <main/controllers/anonusagestats.h>

#include <vector>
#include <sstream>

#define DICOMIZE_WIZARD_ID "DicomizeWizard"

namespace GNC {
	namespace GUI {
		wxWizardImportacionGinkgo::wxWizardImportacionGinkgo(wxWindow* pParent, GNC::GCS::Ptr<GIL::IModeloIntegracion> pModeloIntegracion, std::string uidModulo) : wxWizardGinkgoBase(pParent)
		{
			//load attributes
			GNC::GCS::WindowAttributesController::LoadWindowAttributes(DICOMIZE_WIZARD_ID, this, false);
			//
			wxWindowDisabler disabler;
			SetTitle(_("Image Import Wizard"));
			m_pModeloIntegracion = pModeloIntegracion;

			m_pControladorModulo = NULL;
			
			//si hay mas de un modulo con importador disponible habra que seleccionar el modulo...
			int soportanImportacion=0;

			GNC::ControladorExtensiones::ListaModulos listaModulos = ControladorExtensiones::Instance()->Modulos();
			for(GNC::ControladorExtensiones::IteradorListaModulos it=listaModulos.begin();it!=listaModulos.end();++it){
				GNC::GCS::IModuleController* modulo=(*it).second;
				if(uidModulo == ""){	//se miran los modulos registrados y todo eso
					if(modulo->SoportaImportacion()){
						m_pControladorModulo=modulo;
						++soportanImportacion;
					}
				} else {
					std::stringstream os;
					os<<modulo->GetUID();
					if(os.str() == uidModulo) {
						if(!modulo->SoportaImportacion()) {
							wxMessageBox(_("Requested module does not support importation"), _("Info"),
							 wxOK | wxICON_INFORMATION, this);
							std::string msg(_Std("Requested module does not support importation"));
							throw (GIL::DICOM::I2DException(msg));
						} else {
							//importamos con el modulo que nos dicen
							m_pControladorModulo=modulo;
							++soportanImportacion;
							break;
						}
					}
				}				
			}
			if(soportanImportacion == 0) {
				if(uidModulo == ""){
					wxMessageBox(_("There is not registered any module with importation support"), _("Info"),
								 wxOK | wxICON_INFORMATION, this);
					std::string msg(_Std("There aren't any module with importation support"));
					throw (GIL::DICOM::I2DException(msg));
				} else {
					wxMessageBox(_("Requested module is not registered"), _("Info"),
								 wxOK | wxICON_INFORMATION, this);
					std::string msg(_Std("The requested module isn't registered"));
					throw (GIL::DICOM::I2DException(msg));
				}
			}
			
			do {
				int random = rand();
				std::ostringstream osDir;
				osDir << GNC::Entorno::Instance()->GetGinkgoTempDir() << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE) << "_gnktmp_" << random;
				m_pathDirTemporal = wxString::FromUTF8(osDir.str().c_str());
			} while(wxDir::Exists(m_pathDirTemporal));
			
#ifdef _WIN32
			wxFileName::Mkdir(m_pathDirTemporal);
#else
			wxFileName::Mkdir(m_pathDirTemporal.c_str(), 0777);
#endif
			
			std::string dir_temp (TOPATH(m_pathDirTemporal));
			if (soportanImportacion == 1) {
				//guay porque el m_pControladorModulo ya tiene valor correcto
				//se pillan los pasos
				std::list<IPasoWizard*> pasosModulo;

				m_pControladorModulo->GetPasosImportacion(this,pasosModulo, m_pPanelPrincipal,dir_temp, m_pModeloIntegracion);
				{//anon statistics
					std::ostringstream ostrStat;
					ostrStat << ANON_STAT_DICOMIZE;
					ostrStat << m_pControladorModulo->GetUID();
					LOG_STAT(ostrStat.str())
				}

				if (m_pModeloIntegracion.IsValid()) {
					//////////TODOm_pModeloIntegracion->UIDModulo = m_pControladorModulo->GetUID();
				}

				for(std::list<IPasoWizard*>::iterator it=pasosModulo.begin();it!=pasosModulo.end();++it) {
					if (!(*it)->IsSkipped()) {
						m_ListaPasos.push_back(*it);
						(*it)->AsignarListaFicheros(m_ListaFicheros);
					}
				}
				//
			} else {
				//se enchufa el paso perfilimportacion que se encargara de pillar el perfil
				IPasoWizard* pPaso = new PasoPefilImportacion (m_pPanelPrincipal, this);
				pPaso->AsignarListaFicheros(m_ListaFicheros);
				m_ListaPasos.push_back(pPaso);
			}

			//free space
			wxLongLong total = 0;
			wxLongLong free = 0;
			
			wxLongLong KB = 1024;
			wxLongLong MB = KB*1024;
			wxLongLong GB = MB*1024;

			if (wxGetDiskSpace(FROMPATH(GNC::GCS::IEntorno::Instance()->GetGinkgoTempDir()), &total, &free) ) {
				std::ostringstream os;
				if (free > GB) {
					os << std::string((free / GB ).ToString().ToUTF8()) << " GB ";
				}
				else if (free > MB) {
					os << std::string((free / MB ).ToString().ToUTF8()) << " MB ";
				}
				else if (free > KB) {
					os << std::string((free / KB).ToString().ToUTF8()) << " KB ";
				}
				else {
					os << std::string((free).ToString().ToUTF8()) << " B ";
				}
				os <<  _Std("free of") << " ";

				if (total > GB) {
					os << std::string((total / GB ).ToString().ToUTF8()) << " GB ";
				}
				else if (total > MB) {
					os << std::string((total / MB ).ToString().ToUTF8()) << " MB ";
				}
				else if (total > KB) {
					os << std::string((total / KB).ToString().ToUTF8()) << " KB ";
				}
				else {
					os << std::string((total).ToString().ToUTF8()) << " B ";
				}
				os << _Std("in dicomization dir.");
				m_pBottomLeftLabel->SetLabel(wxString::FromUTF8(os.str().c_str()));
				if (free < GB) {
					m_pBottomLeftLabel->SetForegroundColour(*wxRED);
				}
			}
			else {
				m_pBottomLeftLabel->SetLabel(wxT(""));
			}
			//
			
			m_currentPaso=m_ListaPasos.begin();
			m_currentPasoIndex=1;
			CargarCurrent();
		}
		
		wxWizardImportacionGinkgo::~wxWizardImportacionGinkgo()
		{
			m_ListaPasos.clear();
			
			//save window attributes
			GNC::GCS::WindowAttributesController::SaveWindowAttributes(DICOMIZE_WIZARD_ID, this);
		}
		
		void  wxWizardImportacionGinkgo::BorrarArchivosTemporales()
		{
			BorrarArchivosTemporales(m_pathDirTemporal);
		}
		
		bool  wxWizardImportacionGinkgo::BorrarArchivosTemporales(wxString dirPath)
		{
			//borra en cascada
			if(!wxRmdir(dirPath)){
				//vaciar
				wxDir dir;
				if (dir.Open(dirPath)) {
					wxString fileName;
					bool cont = dir.GetFirst(&fileName);
					while (cont) {
						fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
						if(wxDir::Exists(fileName)){
							if (BorrarArchivosTemporales(fileName)) {
								cont = dir.GetFirst(&fileName);
							} else {
								cont = dir.GetNext(&fileName);
							}
						}else{
							if (wxRemoveFile(fileName)) {
								cont = dir.GetFirst(&fileName);
							} else {
								cont = dir.GetNext(&fileName);
							}
						}
					}
				}
			}
			if(wxDir::Exists(dirPath)){
				return wxRmdir(dirPath);
			}
			return true;
		}
		
		void wxWizardImportacionGinkgo::OnCancelarClick(wxCommandEvent&)
		{
			if ((*m_currentPaso)->AllowCancel()) {
				for(ListaPasos::iterator it=m_ListaPasos.begin();it!=m_ListaPasos.end();++it){
					(*it)->OnCancelled();
				}
				EndModal(wxCANCEL);
			}
		}
		
		void wxWizardImportacionGinkgo::OnSiguienteClick(wxCommandEvent&)
		{
			if ((*m_currentPaso)->Validar()) 
			{
				Freeze();
				
				wxWindow* pCurrent = dynamic_cast<wxWindow*> (*m_currentPaso);
				if (pCurrent != NULL && (*m_currentPaso)->Detach()) {
					pCurrent->Hide();
					m_pSizerPrincipal->Detach(pCurrent);
				
					PasoPefilImportacion* pasoPerfil = dynamic_cast<PasoPefilImportacion * > (*m_currentPaso);
					if(pasoPerfil != NULL){
						//cargamos los pasos del perfil seleccionado...
						m_pControladorModulo = pasoPerfil->GetControladorSeleccionado();
						std::string dir_temp(TOPATH(m_pathDirTemporal));
						
						m_ListaPasos.clear();

						std::list<IPasoWizard*> pasosModulo;
											
						m_pControladorModulo->GetPasosImportacion(this,pasosModulo,m_pPanelPrincipal,dir_temp,m_pModeloIntegracion);
						{//anon statistics
							std::ostringstream ostrStat;
							ostrStat << ANON_STAT_DICOMIZE;
							ostrStat << m_pControladorModulo->GetUID();
							LOG_STAT(ostrStat.str())
						}
						if (m_pModeloIntegracion.IsValid()) {
////////TODO				m_pModeloIntegracion->UIDModulo = m_pControladorModulo->GetUID();
						}
						
						for(std::list<IPasoWizard*>::iterator it=pasosModulo.begin();it!=pasosModulo.end();++it) {
							if (!(*it)->IsSkipped()) {
								m_ListaPasos.push_back(*it);
								(*it)->AsignarListaFicheros(m_ListaFicheros);
							}
						}
						
						m_currentPaso = m_ListaPasos.begin();
						m_currentPasoIndex=1;
					} else {
						do {
							++m_currentPasoIndex;
							++m_currentPaso;
						} while ((m_currentPaso != m_ListaPasos.end() && (*m_currentPaso)->IsSkipped()));
					}
					
					if(m_currentPaso==m_ListaPasos.end()){
						if (GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions","auto_close")) {
							GNC::GUI::StatusBarProgreso* statusBar = (GNC::GUI::StatusBarProgreso*)GNC::Entorno::Instance()->GetVentanaPrincipal()->GetStatusBar();
							statusBar->ShowProgressDialog(true);
						}
						EndModal(wxOK);
					}else{
						CargarCurrent();
					}
				}
				
				Thaw();
			}
		}
		
		void wxWizardImportacionGinkgo::OnAnteriorClick(wxCommandEvent &)
		{
			if(m_currentPaso!=m_ListaPasos.begin()){
				wxWindow* pCurrent = dynamic_cast<wxWindow*> (*m_currentPaso);
				if (pCurrent != NULL && (*m_currentPaso)->Detach()) {
					pCurrent->Hide();
					m_pSizerPrincipal->Detach(pCurrent);

					--m_currentPaso;
					--m_currentPasoIndex;
					CargarCurrent();
				}
			}
		}
		
		void wxWizardImportacionGinkgo::OnClose(wxCloseEvent &event)
		{
			if(m_pModeloIntegracion.IsValid())
			{
				int answer = wxMessageBox(_("Are you sure you want to cancel the import process?"),_("Cancel"), wxYES_NO ,this);
				if(answer == wxYES)
				{
					BorrarArchivosTemporales();					
					EndModal(wxCANCEL);
				} else {
					event.Skip(false);
				}
			} else {
				BorrarArchivosTemporales();				
				EndModal(wxCANCEL);
			}
		}
		
		
		//devuelve la ruta del directorio temporal
		std::list<std::string> wxWizardImportacionGinkgo::GetRuta()
		{
			std::list<std::string> salida;
			for(IPasoWizard::ListaFicheros::iterator it = m_ListaFicheros.begin(); it!= m_ListaFicheros.end(); ++it){
				salida.push_back((*it));
			}
			
			return salida;
		}
		
		
		void wxWizardImportacionGinkgo::CargarCurrent(){
			wxWindow* pCurrent = dynamic_cast<wxWindow*> (*m_currentPaso);
			if (pCurrent != NULL && (*m_currentPaso)->Attach()) 
			{
				pCurrent->Show(true);
				m_pSizerPrincipal->Add(pCurrent, 10, wxEXPAND);

				wxString label= wxString::Format(_("Step %d of %d: "), (int) m_currentPasoIndex, (int) m_ListaPasos.size());
				m_pHeader->SetTitle(label+wxString::FromUTF8((*m_currentPaso)->GetTitle().c_str()));
				m_pBSiguiente->Enable((*m_currentPaso)->Siguiente());
				m_pBAnterior->Enable((*m_currentPaso)->Anterior());
				m_pBCerrar->Enable((*m_currentPaso)->Cancelar());
				PasoPefilImportacion* pasoPerfil = dynamic_cast<PasoPefilImportacion * > (*m_currentPaso);
				if(*(m_currentPaso)==m_ListaPasos.back() && pasoPerfil == NULL){
					m_pBSiguiente->SetLabel(_("&End"));
				}else{
					m_pBSiguiente->SetLabel(_("&Next >"));
				}
			}
			
			m_pPanelPrincipal->Layout();
			Layout();
		}
	}
}
