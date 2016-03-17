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

#include "iwidgetscontract.h"
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <api/istudycontext.h>

GNC::GCS::IWidgetsContract::IWidgetsContract()
{
}

GNC::GCS::IWidgetsContract::~IWidgetsContract()
{
}

void GNC::GCS::IWidgetsContract::GetAllViewers(TViewerList& viewerList)
{
        viewerList.push_back(GetViewerActivo());
}

bool GNC::GCS::IWidgetsContract::Inicializado()
{
        return ( (GetManager() != NULL) && (GetViewerActivo() != NULL) );
}

//Anotation section
//this method processes special keys like: ${VOLUME.IDX} ${SLICE.WIDTH}...
std::string GNC::GCS::IWidgetsContract::GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key)
{
        return GetTagValue(c, key);
}

//return dicom tag value from key
std::string GNC::GCS::IWidgetsContract::GetTagValue(GNC::GCS::Contexto3D* /*c*/, const std::string& key)
{
        GNC::GCS::IWidgetsManager* pManager = GetManager();
        if (pManager != NULL) {
                std::string value;
                if (pManager->GetVista()->GetEstudio()->GetTagActiveImage(key, value)) {
                        return value;
                }
        }
        return "";
}

vtkGinkgoImageViewer* GNC::GCS::IWidgetsContract::GetViewerActivo()
{
        GNC::GCS::IWidgetsManager* pManager = GetManager();
        if (pManager != NULL) {
                return pManager->GetRendererActivo()->m_pImageViewer;
        } else {
                return NULL;
        }
}
