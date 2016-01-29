/*
 *  
 *  $Id: panelgrid.cpp $
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

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <wx/menu.h>
#include <wx/dcbuffer.h>
#include <wx/dcclient.h>

#include <api/globals.h>

#include <resources/ginkgoresourcesmanager.h>
#include <api/ivista.h>
#include <main/entorno.h>

#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/controladorvistas.h>

#include "detacheddialog.h"

namespace GNC {
	namespace GUI {
		DetachedDialog::DetachedDialog(wxWindow* pParent, VentanaPrincipal* pVentanaPrincipal): DetachedDialogBase(pParent)
		{
			wxIcon icono;
			icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
			this->SetIcon(icono);

			GNC::Entorno::Instance()->ViewsObservers.push_back(this);
			m_pWindow = NULL;
			m_pVentanaPrincipal = pVentanaPrincipal;
		}

		DetachedDialog::~DetachedDialog()
		{
			GNC::Entorno::Instance()->ViewsObservers.remove(this);
			m_pWindow = NULL;
			m_pVentanaPrincipal = NULL;
		}

		void DetachedDialog::AddPanel(wxWindow* pWin, const wxString& titulo)
		{
			m_pWindow = pWin;
			SetTitle(titulo);
			if(m_pWindow->GetParent() != this){
				m_pWindow->GetParent()->GetSizer()->Detach(m_pWindow);
				m_pWindow->Reparent(this);
			}
			m_pWindow->Show(true);
			GetSizer()->Add(m_pWindow,1,wxEXPAND);
		}

		void DetachedDialog::SetTitulo(const wxString& titulo)
		{
			SetTitle(titulo);
		}

		wxWindow* DetachedDialog::GetWindow()
		{
			return m_pWindow;
		}

		wxString DetachedDialog::GetTitulo()
		{
			return GetTitle();
		}

		void DetachedDialog::OnCloseDialogo(wxCloseEvent &event)
		{
			if(!m_pVentanaPrincipal->CerrarVista(m_pWindow)) {
				event.Veto();
			} else {
				event.Skip();
				GetSizer()->Detach(m_pWindow);
				GNC::GCS::ControladorVistas::Instance()->Destruir(m_pWindow);
				// It's necessary to force this to activate startup view (and fill in archive menu)
				if (m_pVentanaPrincipal->m_pNoteBook->GetPageCount() == 0) { 
					//se muestra el mosaico
					m_pVentanaPrincipal->ShowStartUp(true);
				}
			}
		}

		//esto es para que se pinte correctamente el recuadro amarillo cuando cambias de ventana
		void DetachedDialog::OnVistaActivada(GNC::GCS::IVista* pVista)
		{
			RefrescarVista(pVista);
		}

		void DetachedDialog::RefrescarVista(GNC::GCS::IVista* pVista)
		{
			if(pVista != NULL) {
				if(pVista->GetWindow()->GetParent() == m_pWindow)
				{
					m_pWindow->Refresh();
				}
			}
		}


		void DetachedDialog::OnBotonTitulo(wxCommandEvent &)
		{

		}
	}
}
