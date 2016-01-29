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

#include "simpleprinttool.h"
#include <export/contracts/iwidgetscontract.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/modificacionimagen.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <main/entorno.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/gui/toolsystem/wxmenuitemtool.h>

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
#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/print.h>
#include <wx/printdlg.h>

#define SIMPLE_SETUP  1876
#define SIMPLE_PRINT  1877
#define SIMPLE_PRINT_PREVIEW  1878

namespace GNC {
	namespace GUI {
		class wxEventHandlerSimplePrint : public wxEvtHandler {
		public:
			wxEventHandlerSimplePrint(wxEvtHandler* pParent, GNC::SimplePrintTool* pHerramienta):wxEvtHandler() {
				m_pEvtParent = pParent;
				m_pHerramienta=pHerramienta;

				m_pEvtParent->Connect(SIMPLE_SETUP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);
				m_pEvtParent->Connect(SIMPLE_PRINT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);				
				m_pEvtParent->Connect(SIMPLE_PRINT_PREVIEW, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);

				m_pEvtParent->Connect(SIMPLE_SETUP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerSimplePrint::OnSetup),NULL,this);
				m_pEvtParent->Connect(SIMPLE_PRINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerSimplePrint::OnPrint),NULL,this);
				m_pEvtParent->Connect(SIMPLE_PRINT_PREVIEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerSimplePrint::OnPrintPreview),NULL,this);
			}

			~wxEventHandlerSimplePrint() {
				m_pEvtParent->Disconnect(SIMPLE_SETUP, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);
				m_pEvtParent->Disconnect(SIMPLE_PRINT, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);	
				m_pEvtParent->Disconnect(SIMPLE_PRINT_PREVIEW, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(wxEventHandlerSimplePrint::OnUpdateUI),NULL,this);

				m_pEvtParent->Disconnect(SIMPLE_SETUP, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxEventHandlerSimplePrint::OnSetup),NULL,this);
				m_pEvtParent->Disconnect(SIMPLE_PRINT, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerSimplePrint::OnPrint),NULL,this);
				m_pEvtParent->Disconnect(SIMPLE_PRINT_PREVIEW, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(wxEventHandlerSimplePrint::OnPrintPreview),NULL,this);
				m_pEvtParent = NULL;
				m_pHerramienta = NULL;
			}

			void OnPrint( wxCommandEvent& event )
			{
				m_pHerramienta->doPrint();
				event.Skip(false);
			}

			void OnSetup( wxCommandEvent& event )
			{
				m_pHerramienta->doSetup();
				event.Skip(false);
			}

			void OnPrintPreview(wxCommandEvent& event) 
			{
				m_pHerramienta->doPrintPreview();
				event.Skip(false);
			}

			void OnUpdateUI(wxUpdateUIEvent& event) {
				event.Enable(m_pHerramienta->IsEnabled());
			}


			GNC::SimplePrintTool* m_pHerramienta;
			wxEvtHandler* m_pEvtParent;
		};
	}

	///////printout helper
	class ginkgoPrintOutImage: public wxPrintout
	{
	public:
		ginkgoPrintOutImage(const wxImage& im):wxPrintout(_("Image printing")),img(im)
		{
		}

		~ginkgoPrintOutImage()
		{
		}

		virtual bool HasPage(int page)
		{
			return page == 1;
		}

		virtual void GetPageInfo(int *minPage, int *maxPage, int *pageFrom, int *pageTo)
		{
			*minPage = *maxPage = *pageFrom = *pageTo= 1;
		}

		virtual bool OnPrintPage(int /*page*/)
		{
			wxDC *dc = GetDC();
				if (dc)
				{
					//limpioi
					dc->SetBackground(*wxWHITE_BRUSH);
					dc->Clear();
					if (img.IsOk()) {
						wxSize pageSize = this->GetLogicalPageRect().GetSize();
						wxSize imgSize = img.GetSize();
						
						bool resample = false;

						if (imgSize.x > pageSize.x || imgSize.y > pageSize.y) {
							//downscale
							if (imgSize.x > pageSize.x) {
								imgSize.y = ((double)pageSize.x/imgSize.x)*imgSize.y;
								imgSize.x = pageSize.x;
								resample = true;
							}
							if (imgSize.y > pageSize.y) {
								imgSize.x = ((double)pageSize.y/imgSize.y)*imgSize.x;
								imgSize.y = pageSize.y;
								resample = true;
							}
						} else {
							//upscale
							if (((float)imgSize.x/pageSize.x) > ((float)imgSize.y/pageSize.y)) {
								imgSize.y = ((double)pageSize.x/imgSize.x)*imgSize.y;
								imgSize.x = pageSize.x;
								resample = true;
							} else {
								imgSize.x = ((double)pageSize.y/imgSize.y)*imgSize.x;
								imgSize.y = pageSize.y;
								resample = true;
							}
						}

						if (resample) {
							img.Rescale(imgSize.x, imgSize.y, wxIMAGE_QUALITY_BILINEAR);
						}
						dc->DrawBitmap( img, 0, 0 );
					}
					
					return true;
				}
				else {
					return false;
				}
		}


		wxImage img;
	};
}


GNC::GCS::ITool* GNC::SimplePrintTool::NewTool()
{
	return new GNC::SimplePrintTool();
}

GNC::SimplePrintTool::SimplePrintTool()
{
	pData = new wxPrintData();
}
GNC::SimplePrintTool::~SimplePrintTool()
{
	delete pData;
}

bool GNC::SimplePrintTool::ExecuteAction()
{	
	return false;
}

bool GNC::SimplePrintTool::HasDropDownMenu()
{
	return true;
}

void GNC::SimplePrintTool::AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (pMenu->GetMenuItemCount() > 0 && !pMenu->GetMenuItems().back()->IsSeparator()) {
		pMenu->AppendSeparator();
	}
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, SIMPLE_SETUP, _("Page setup") + wxString(wxT("...")), wxNullBitmap, new GNC::GUI::wxEventHandlerSimplePrint( pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, SIMPLE_PRINT_PREVIEW, _("Print preview") + wxString(wxT("...")), wxNullBitmap, new GNC::GUI::wxEventHandlerSimplePrint( pParent, this)));
	pMenu->Append(new GNC::GUI::wxMenuItemTool(pMenu, SIMPLE_PRINT, _("Print") + wxString(wxT("...")), GinkgoResourcesManager::MenuIcons::GetIcoImpresora(), new GNC::GUI::wxEventHandlerSimplePrint( pParent, this)));
}

void GNC::SimplePrintTool::AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (AppendsInMenu()) {
		AppendDropDownMenu(pParent, pMenu);
	}
}

wxImage getImage(const vtkSmartPointer<vtkRenderWindow>& rendWin)
{
    if (rendWin == NULL) {
            return wxNullImage;
    }
    vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = 
            vtkSmartPointer<vtkWindowToImageFilter>::New();
    windowToImageFilter->SetInput(rendWin);

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

	return wxImage(timg->GetDimensions()[0], timg->GetDimensions()[1],data,false);
}

void GNC::SimplePrintTool::doPrintPreview()
{
	wxImage image = getImage(WidgetsContract->GetViewerActivo()->GetRenderWindow());
	if (image.IsOk()) {
		wxPrintPreview* myPrintPreview = new wxPrintPreview(new ginkgoPrintOutImage(image), new ginkgoPrintOutImage(image), pData); 
		wxPreviewFrame *frame = new wxPreviewFrame(myPrintPreview, WidgetsContract->GetManager()->GetVista()->GetWindow(),
			_("Print image"), wxDefaultPosition, wxSize(800, 600));
		frame->InitializeWithModality(wxPreviewFrame_WindowModal);
			frame->Show(true);
		frame->Show(true);
	}
}

void GNC::SimplePrintTool::doPrint()
{
	wxImage image = getImage(WidgetsContract->GetViewerActivo()->GetRenderWindow());
	if (image.IsOk()) {
		wxPrintDialogData dlgData(*pData);
		wxPrinter printer(&dlgData);
		ginkgoPrintOutImage prOut(image);
		printer.Print(WidgetsContract->GetManager()->GetVista()->GetWindow(), &prOut);
	}
}

void GNC::SimplePrintTool::doSetup()
{
	wxPageSetupDialogData dlgData(*pData);
	wxPageSetupDialog dlg(WidgetsContract->GetManager()->GetVista()->GetWindow(), &dlgData);
	dlg.ShowModal();
}

