/*
 *  
 *  $Id: wxwizardginkgobase.cpp $
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
#include "wxwizardginkgobase.h"
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/inactivitycontroller.h>

wxWizardGinkgoBase::wxWizardGinkgoBase( wxWindow* parent):
wxWizardGinkgoBaseWx(parent)
{
	//stops inactivity monitor
	GNC::GCS::InactivityController::Instance()->StopsMonitoring();
	//
}

wxWizardGinkgoBase::~wxWizardGinkgoBase()
{
	//starts inactivity monitor
	GNC::GCS::InactivityController::Instance()->RestartMonitoring();
	//
}

void wxWizardGinkgoBase::SiguientePaso(){
	wxCommandEvent evt;
	OnSiguienteClick(evt);
}

void wxWizardGinkgoBase::EnableSiguiente(bool enable){
	m_pBSiguiente->Enable(enable);
}

void wxWizardGinkgoBase::EnableAnterior(bool enable) {
	m_pBAnterior->Enable(enable);
}

void wxWizardGinkgoBase::EnableCancelar(bool enable) {
	m_pBCerrar->Enable(enable);
}

void wxWizardGinkgoBase::OnSize(wxSizeEvent& event)
{
	event.Skip(true);
}

