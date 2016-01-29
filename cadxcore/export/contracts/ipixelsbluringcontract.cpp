/*
*
*  $Id: iwindowlevelcontract.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "ipixelsbluringcontract.h"
#include <main/controllers/modalitysettingscontroller.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkSystemIncludes.h>

GNC::GCS::IPixelsBluringContract::IPixelsBluringContract() : IWidgetsContract() {
}

GNC::GCS::IPixelsBluringContract::~IPixelsBluringContract(){
}

void GNC::GCS::IPixelsBluringContract::Setup(const std::string& modality)
{
	if (GetManager() != NULL && GetManager()->GetVista() != NULL) {
		GNC::GCS::Ptr<DefaultModalitySettings> ptr = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettings(modality);
		if (ptr.IsValid()) {
			int interpolationMode = VTK_LINEAR_INTERPOLATION;
			switch(ptr->getInterpolationMode()) 
			{
			case GNC::GCS::DefaultModalitySettings::TIM_LINEAR:
				interpolationMode = VTK_LINEAR_INTERPOLATION;
				break;
			case GNC::GCS::DefaultModalitySettings::TIM_NEAREST_NEIGHBOUR:
				interpolationMode = VTK_NEAREST_INTERPOLATION;
				break;
			case GNC::GCS::DefaultModalitySettings::TIM_CUBIC:
			//	interpolationMode = VTK_CUBIC_INTERPOLATION;
				interpolationMode = 2;//en linux no encuentra cubic
				break;
			}
			GNC::GCS::IWidgetsContract::TViewerList list;
			GetAllViewers(list);
			for (GNC::GCS::IWidgetsContract::TViewerList::iterator it = list.begin(); it != list.end(); ++it) {
				vtkGinkgoImageViewer* pW = (*it);
				pW->SetInterpolationMode(interpolationMode);
			}
		}

	}
}
