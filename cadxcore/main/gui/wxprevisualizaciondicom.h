/*
 *  
 *  $Id: wxprevisualizaciondicom.h $
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
