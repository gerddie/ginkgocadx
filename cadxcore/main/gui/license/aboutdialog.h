/*
 *  
 *  $Id: aboutdialog.h $
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
#include "aboutdialogbase.h"
#include <string>


class AboutDialog : public AboutDialogBase
{
public:
	AboutDialog(wxWindow* pPadre);
	~AboutDialog();

	virtual void OnLicenciasOpenSource(wxCommandEvent &event);	

	virtual void OnSize(wxSizeEvent &event);

	virtual void OnTextURL(wxHtmlLinkEvent &event);

private:

//endregion

};
