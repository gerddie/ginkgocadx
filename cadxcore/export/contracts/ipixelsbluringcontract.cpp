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

#include "ipixelsbluringcontract.h"
#include <main/controllers/modalitysettingscontroller.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <vtkSystemIncludes.h>

GNC::GCS::IPixelsBluringContract::IPixelsBluringContract() : IWidgetsContract()
{
}

GNC::GCS::IPixelsBluringContract::~IPixelsBluringContract()
{
}

void GNC::GCS::IPixelsBluringContract::Setup(const std::string& modality)
{
        if (GetManager() != NULL && GetManager()->GetVista() != NULL) {
                GNC::GCS::Ptr<DefaultModalitySettings> ptr = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettings(modality);
                if (ptr.IsValid()) {
                        int interpolationMode = VTK_LINEAR_INTERPOLATION;
                        switch(ptr->getInterpolationMode()) {
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
