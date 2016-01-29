/*
*  
*  $Id: resettool.cpp $
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

#include "copyimagetoclipboardtool.h"
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>
#include <main/entorno.h>
#include <main/controllers/commandcontroller.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtk/vtkginkgoimageviewer.h>

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkSphereSource.h>
#include <vtkWindowToImageFilter.h>
#include <vtkJPEGWriter.h>

#include <resources/ginkgoresourcesmanager.h>

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/dataobj.h>
#include <wx/clipbrd.h>

GNC::GCS::ITool* GNC::CopyImageToClipboardTool::NewTool()
{
	return new GNC::CopyImageToClipboardTool();
}

GNC::CopyImageToClipboardTool::CopyImageToClipboardTool()
{
}
GNC::CopyImageToClipboardTool::~CopyImageToClipboardTool()
{
}

bool GNC::CopyImageToClipboardTool::ExecuteAction()
{	
	std::string tempName = GNC::Entorno::Instance()->CreateGinkgoTempFile();
	{
		vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = 
			vtkSmartPointer<vtkWindowToImageFilter>::New();
		windowToImageFilter->SetInput(WidgetsContract->GetViewerActivo()->GetRenderWindow());
		windowToImageFilter->SetInputBufferTypeToRGB(); //also record the alpha (transparency) channel
		windowToImageFilter->Update();
		//copy image to wx...
		// put the pixels into a wxImage ...
		vtkSmartPointer<vtkImageData> timg = windowToImageFilter->GetOutput();
		unsigned char *pVtkPix = (unsigned char *)timg->GetScalarPointer();
		
		unsigned long imgsize = timg->GetDimensions()[0] * timg->GetDimensions()[1] * sizeof(unsigned char) * 3;
		unsigned char* data = new unsigned char[imgsize];
		int linesize = timg->GetDimensions()[0] *3;
		for (int y = 0; y < timg->GetDimensions()[1]; ++y) {
			int offsetWx = imgsize - (linesize * (y +1));
			int offsetVtk = y*linesize;
			for (int x = 0; x < linesize; ++x) {
				data[offsetWx+x] = pVtkPix[offsetVtk+x];
			}
		}

		wxImage image(timg->GetDimensions()[0], timg->GetDimensions()[1],data,false);
		// ... and send the image to the clipboard
		if (wxTheClipboard->Open())
		{
		   wxTheClipboard->SetData( new wxBitmapDataObject(wxBitmap(image)) );
		   wxTheClipboard->Close();
		}
		
	}

	return true;
}
