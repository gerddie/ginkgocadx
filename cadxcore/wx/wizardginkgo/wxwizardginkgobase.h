/*
 *  
 *  $Id: wxwizardginkgobase.h $
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
#pragma once
#include "wxwizardginkgobasewx.h"
#include <api/iwizard.h>
#include <api/api.h>

class EXTAPI wxWizardGinkgoBase: public wxWizardGinkgoBaseWx, public IWizard{
public:
	wxWizardGinkgoBase( wxWindow* parent);
	~wxWizardGinkgoBase();

	virtual void SiguientePaso();

	virtual void EnableSiguiente(bool enable);

	virtual void EnableAnterior(bool enable);

	virtual void EnableCancelar(bool enable);

	void OnSize(wxSizeEvent&);

};

