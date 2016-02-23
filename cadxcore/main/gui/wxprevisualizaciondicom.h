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

#pragma once

#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif

#include <api/api.h>
#include <wx/window.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <vtkSmartPointer.h>

class GinkgoRenderWindowInteractor;
class GinkgoImageViewer;
class vtkImageData;
class wxVTKRenderWindowInteractor;
class vtkGinkgoImageViewer;


class EXTAPI wxPrevisualizacionDicom : public wxPanel {
public:
    wxPrevisualizacionDicom(wxWindow* parent, wxWindowID id = wxID_ANY);
    ~wxPrevisualizacionDicom();
	
	void SetInput(vtkSmartPointer<vtkImageData>& data);
	
	vtkGinkgoImageViewer* GetView() { return ViewImage2D; };
	wxVTKRenderWindowInteractor* GetInteractor() {return ViewInteractor;}

	virtual void OnMouseLeftDClick();
	
protected:
	/*
	 GinkgoRenderWindowInteractor* ViewInteractor;
	 GinkgoImageViewer*            ViewImage2D;
	 */
	wxVTKRenderWindowInteractor*  ViewInteractor;
	vtkGinkgoImageViewer*     ViewImage2D;
	wxBoxSizer*                   Sizer;
	
	void reset();
};
