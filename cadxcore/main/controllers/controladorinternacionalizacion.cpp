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

#include "controladorinternacionalizacion.h"
#include <wx/intl.h>
#include <main/controllers/configurationcontroller.h>
#include <main/entorno.h>


namespace GNC
{
namespace GCS
{
ControladorInternacionalizacion* ControladorInternacionalizacion::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new ControladorInternacionalizacion();
        }
        return m_pInstance;
}

void ControladorInternacionalizacion::FreeInstance()
{
        if(m_pInstance!=NULL) {
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
        if (language == -1) {
                //se pilla el de la maquina
                language = wxLocale::GetSystemLanguage();
        }
        return language;
}
}
}
