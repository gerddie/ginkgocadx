/*
 *  
 *  $Id: wxwizardexportacionginkgo.cpp $
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
#include <sstream>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/mstream.h>
#include <wx/busyinfo.h>
#include <wx/image.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include <export/contracts/iwidgetscontract.h>
#include <api/iwidgetsmanager.h>


#include "parametrosprincipales.h"
#include "wxwizardexportacionginkgo.h"

#include <main/controllers/anonusagestats.h>

#include <vector>

#include <main/entorno.h>

namespace GNC {
	namespace GUI {
		wxWidzardExportacionGinkgo::wxWidzardExportacionGinkgo(wxWindow* pParent, GNC::GCS::IWidgetsContract* WidgetsContract) : wxWizardGinkgoBase(pParent)
		{
			LOG_STAT(ANON_STAT_DLG_EXPORT)
			SetTitle(_("Image Export Wizard"));
			m_OK = false;			

			//se pillan los pasos
			IPasoWizard* pPaso=NULL;

			pPaso = new ParametrosPrincipales(m_pPanelPrincipal, this, WidgetsContract);
			m_ListaPasos.push_back(pPaso);
			//

			m_pBAnterior->Show(false);

			m_currentPasoIndex=0;
			CargarCurrent();
		}

		wxWidzardExportacionGinkgo::~wxWidzardExportacionGinkgo()
		{
			m_ListaPasos.clear();
		}


		void wxWidzardExportacionGinkgo::OnCancelarClick(wxCommandEvent& )
		{
			EndModal(wxCANCEL);
		}

		void wxWidzardExportacionGinkgo::OnSiguienteClick(wxCommandEvent& )
		{
			ListaPasos::iterator it = m_ListaPasos.begin();
			
			for (ListaPasos::size_type i = 0; i < m_currentPasoIndex; i++, ++it);
			IPasoWizard* currentPaso = *it;
			if(currentPaso->Validar()){
				wxWindow* pCurrent = dynamic_cast<wxWindow*> (currentPaso);
				if (pCurrent != NULL && (currentPaso)->Detach()) {
					pCurrent->Hide();
					m_pSizerPrincipal->Detach(pCurrent);
						++m_currentPasoIndex;
						if(m_currentPasoIndex >= m_ListaPasos.size()) {
							EndModal(wxOK);
							return;
						} else {
							CargarCurrent();
						}
				}
			}
		}

		void wxWidzardExportacionGinkgo::OnAnteriorClick(wxCommandEvent &)
		{
			if(m_currentPasoIndex!=0){
				ListaPasos::iterator it = m_ListaPasos.begin();
			
				for (ListaPasos::size_type i = 0; i < m_currentPasoIndex; i++, ++it);
				//IPasoWizard* currentPaso = *it;

				wxWindow* pCurrent = dynamic_cast<wxWindow*> (*it);
				if (pCurrent != NULL && (*it)->Detach()) 
				{
					pCurrent->Hide();
					m_pSizerPrincipal->Detach(pCurrent);
					switch(m_currentPasoIndex){
						case 1:
							--m_currentPasoIndex;
							break;
						case 2:
							m_currentPasoIndex=0;
							break;
					}

					CargarCurrent();
				}
			}
		}

		bool wxWidzardExportacionGinkgo::IsOK()
		{
			return m_OK;
		}

		void wxWidzardExportacionGinkgo::CargarCurrent(){
			ListaPasos::iterator it = m_ListaPasos.begin();
			
			for (ListaPasos::size_type i = 0; i < m_currentPasoIndex; i++, ++it);
			IPasoWizard* currentPaso = *it;

			wxWindow* pCurrent = dynamic_cast<wxWindow*> (currentPaso);
			if (pCurrent != NULL && (currentPaso)->Attach()) 
			{
				pCurrent->Show(true);
				m_pSizerPrincipal->Add(pCurrent, 10, wxEXPAND);

				m_pHeader->SetTitle(wxString::FromUTF8(currentPaso->GetTitle().c_str()));
				m_pHeader->SetSubtitle(wxString::FromUTF8(currentPaso->GetSubTitle().c_str()));
				m_pBSiguiente->Enable(currentPaso->Siguiente());
				m_pBAnterior->Enable(currentPaso->Anterior());
				m_pBCerrar->Enable(currentPaso->Cancelar());
				if(m_currentPasoIndex==m_ListaPasos.size()-1){
					m_pBSiguiente->SetLabel(_("&Accept"));
				}else{
					m_pBSiguiente->SetLabel(_("&Next >"));
				}
			}
			m_pPanelPrincipal->Layout();
			Layout();
		}
	};
}
