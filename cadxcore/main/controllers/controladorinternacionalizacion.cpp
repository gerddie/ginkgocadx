/*
 *  
 *  $Id: controladorinternacionalizacion.cpp $
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
#include "controladorinternacionalizacion.h"
#include <wx/intl.h>
#include <main/controllers/configurationcontroller.h>
#include <main/entorno.h>


namespace GNC {
	namespace GCS {
		ControladorInternacionalizacion* ControladorInternacionalizacion::Instance()
		{
			if (m_pInstance == NULL) {
				m_pInstance = new ControladorInternacionalizacion();
			}
			return m_pInstance;
		}

		void ControladorInternacionalizacion::FreeInstance()
		{
			if(m_pInstance!=NULL){
				delete m_pInstance;
				m_pInstance = NULL;
			}
		}

		ControladorInternacionalizacion* ControladorInternacionalizacion::m_pInstance = NULL;

		ControladorInternacionalizacion::ControladorInternacionalizacion()
		{
			m_idiomaInterfazSesion = GetIdiomaInterfaz();
			//se inicializa el soporte de idioma, leyendolo de la configuracion...
			wxLocale *pLoc = wxGetLocale();
			wxString pathLang = FROMPATH(GNC::Entorno::Instance()->GetGinkgoLanguageDir());
			if (pLoc == NULL) {
				pLoc = new wxLocale();
			}			
			pLoc->AddCatalogLookupPathPrefix(pathLang);
			pLoc->Init(m_idiomaInterfazSesion);
			setlocale(LC_NUMERIC, "C");
		}
		ControladorInternacionalizacion::~ControladorInternacionalizacion()
		{
			wxLocale *pLoc = wxGetLocale();
			if(pLoc != NULL) {
				pLoc->DestroyLanguagesDB();
				delete pLoc;
			}
		}

		void ControladorInternacionalizacion::AddCatalog(const std::string& catalogName)
		{
			wxString strTmp = FROMPATH(catalogName);
			wxLocale *pLoc = wxGetLocale();
			if (pLoc != NULL) {
				pLoc->AddCatalog(strTmp);
			}
		}

		//se lee de la configuracion
		int ControladorInternacionalizacion::GetIdiomaInterfaz()
		{
			int language;
			GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station", "Language", language,-1);
			if (language == -1) 
			{
				//se pilla el de la maquina
				language = wxLocale::GetSystemLanguage();
			}
			return language;
		}
	}
}
