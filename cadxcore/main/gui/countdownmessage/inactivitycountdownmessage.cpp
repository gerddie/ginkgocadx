/*
 *
 *  $Id: InactivityCountdownMessage.cpp $
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
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include "inactivitycountdownmessage.h"
#include <wx/ginkgostyle/ginkgostyle.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/controllers/inactivitycontroller.h>
#include <main/entorno.h>

namespace GNC {
	namespace GUI {
		InactivityCountdownMessage::InactivityCountdownMessage(wxWindow* pParent, const wxString& message): InactivityCountdownMessageBase(pParent), m_last(6)
		{
			m_message = message;
			m_pFormPanel->Layout();
			m_pBody->Layout();
			Layout();
		}

		InactivityCountdownMessage::~InactivityCountdownMessage()
		{
			GNC::GCS::InactivityController::Instance()->DetachForm();
		}

		bool InactivityCountdownMessage::Show(bool show)
		{
			if (show && !IsShown()) {
				m_last = 6;			
				Notify();
			} else {
				if (IsRunning()) {
					Stop();
				}
				GNC::GCS::InactivityController::Instance()->RestartMonitoring();
			}
			return InactivityCountdownMessageBase::Show(show);
		}

		void InactivityCountdownMessage::Notify()
		{
			if (m_last > 0) {
				Freeze();
				m_last--; 
				m_pLabelInactivity->SetLabel(wxString::Format(m_message, m_last));
				m_pFormPanel->Layout();
				m_pBody->Layout();
				Layout();
				this->Start(1000, true);
				Thaw();
			} else {
				GNC::Entorno::Instance()->GetVentanaPrincipal()->Close();
			}
		}

		void InactivityCountdownMessage::OnCancel(wxCommandEvent &)
		{
			GNC::GCS::InactivityController::Instance()->RestartMonitoring();
			if (IsRunning()) {
				Stop();
			}
			////////	
			Hide();
		}

		void InactivityCountdownMessage::OnCloseButton(wxCommandEvent &/*event*/)
		{
			GNC::Entorno::Instance()->GetVentanaPrincipal()->Close();
		}
		void InactivityCountdownMessage::OnClose(wxCloseEvent &event)
		{
			GNC::GCS::InactivityController::Instance()->RestartMonitoring();
			if (IsRunning()) {
				Stop();
			}
			event.Veto();
			Hide();
		}
	}
}
