/*
 *  
 *  $Id$
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
#include <api/controllers/imodulecontroller.h>


class DialogoLicenciasOpenSource : public DialogoLicenciasOpenSourceBase
{
public:
	DialogoLicenciasOpenSource(wxWindow* pPadre);
	~DialogoLicenciasOpenSource();

	virtual void OnListaLicencias(wxCommandEvent &event);

	void EscribeTitulo(wxString titulo);
	void EscribeLicencia(wxString& licencia);

	void EscribeLicenciaGinkgo();
	void EscribeLicenciaCairo();
	void EscribeLicenciaDCMTK();
	void EscribeLicenciaFamFam();
	void EscribeLicenciaFugue();
	void EscribeLicenciaITK();
	void EscribeLicencialibjpeg();
	void EscribeLicencialibpng();
	void EscribeLicenciaOpenssl();
	void EscribeLicenciaVTK();
	void EscribeLicenciawxWidgets();
	void EscribeLicenciazlib();
	void EscribeLicenciaSQLLite();
	void EscribeLicenciaCharLS();
	void WriteDarkGlassLicense();
	void WriteLibcurl();
	void WriteLibJsonCpp();
	void WriteLibAIRS();


	GNC::GCS::IModuleController::TExtraLicensesMap ExtraMap;
//endregion

};
