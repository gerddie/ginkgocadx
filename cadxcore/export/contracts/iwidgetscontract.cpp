/*
*
*  $Id: iwidgetscontract.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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
	if (pManager != NULL) 
	{
		return pManager->GetRendererActivo()->m_pImageViewer;
	}
	else
	{
		return NULL;
	}
}
