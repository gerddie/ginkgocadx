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

#include "pixelsbluringtool.h"
#if defined(ENABLE_PIXELSBLURING)
#include <wx/menu.h>
#include "pixelsbluringtool.h"
#include <api/iwidgetsmanager.h>
#include <export/contracts/ipixelsbluringcontract.h>

#include <main/controllers/controladoreventos.h>
#include <eventos/render.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkImageData.h>
#include <vtkLookupTable.h>
#include <vtkImageActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtk/vtkginkgoimageviewer.h>

namespace GNC {
	namespace GUI {
		class wxPixelsBluringEventHandler: public wxDefaultEvtHandlerTool
		{
		public:
			wxPixelsBluringEventHandler(wxEvtHandler* pParent, GNC::PixelsBluringTool* pTool): wxDefaultEvtHandlerTool(pParent, pTool)
			{
				m_pPixelsBluringTool = pTool;
			}

			~wxPixelsBluringEventHandler()
			{
			}

			virtual void OnUpdateUI(wxUpdateUIEvent &event)
			{
				if (m_pPixelsBluringTool->IsEnabled()) {
					wxDefaultEvtHandlerTool::OnUpdateUI(event);
					if(m_pPixelsBluringTool->IsSet()) {
						event.SetText(_("Unset pixels blurring"));
					} else {
						event.SetText(_("Set pixels blurring"));
					}
					event.Enable(true);
				} else {
					event.Enable(false);
				}
			}
			GNC::PixelsBluringTool* m_pPixelsBluringTool;
		};

	}
}


GNC::GCS::ITool* GNC::PixelsBluringTool::NewTool()
{
	return new GNC::PixelsBluringTool();
}

GNC::PixelsBluringTool::PixelsBluringTool()
{
}
GNC::PixelsBluringTool::~PixelsBluringTool()
{
}
		
bool GNC::PixelsBluringTool::ExecuteAction()
{
	if (PixelsBluringContract->Inicializado()) {
		GNC::GCS::IWidgetsContract::TViewerList list;
		PixelsBluringContract->GetAllViewers(list);
		bool enable = false;
		if (PixelsBluringContract->GetViewerActivo()->GetInterpolationMode() == VTK_LINEAR_INTERPOLATION) {
			enable = true;
		}
		for (GNC::GCS::IWidgetsContract::TViewerList::iterator it = list.begin(); it != list.end(); ++it) {
			vtkGinkgoImageViewer* pW = (*it);
			if (enable) {
				pW->SetInterpolationMode(VTK_NEAREST_INTERPOLATION);
			}
			else {
				pW->SetInterpolationMode(VTK_LINEAR_INTERPOLATION);
			}
		}
		GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoRender(PixelsBluringContract->GetManager()->GetVista()));
	}
	return true;
}

void GNC::PixelsBluringTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		GNC::GUI::wxPixelsBluringEventHandler* evtHandler = new GNC::GUI::wxPixelsBluringEventHandler(pParent, this);
		pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, ID, wxString::FromUTF8(Name.c_str()),Icon, evtHandler));
	}
}

bool GNC::PixelsBluringTool::IsSet()
{
	if (PixelsBluringContract->GetViewerActivo() != NULL) {
		return PixelsBluringContract->GetViewerActivo()->GetInterpolationMode() == VTK_LINEAR_INTERPOLATION;
	} else {
		return NULL;
	}
}

bool GNC::PixelsBluringTool::IsEnabled()
{
	return PixelsBluringContract->GetViewerActivo() != NULL;
}
#endif
