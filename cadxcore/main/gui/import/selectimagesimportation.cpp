/*
*  
*  $Id: selectimagesimportation.cpp $
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
#include <vector>
#include <sstream>

#include <wx/image.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/dnd.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/pen.h>
#include <wx/sizer.h>
#include <wx/arrstr.h>
#include <wx/thread.h>
#include <wx/menu.h>
#include <wx/wxginkgotoolbar.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include <api/ubication.h>
#include <api/math/geometry.h>
#include <resources/ginkgoresourcesmanager.h>

#include <api/autoptr.h>
#include <api/ubication.h>
#include <main/entorno.h>

#include "selectimagesimportation.h"
#include <resources/ginkgoresourcesmanager.h>
#include <main/controllers/controladorlog.h>
#include <api/internationalization/internationalization.h>

#include <vtkImageData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkPointData.h>
#include <vtk/vtkginkgoimageviewer.h>

#include <wx/mimetype.h>
#include <wx/clipbrd.h>
#include <wx/wxginkgotoolbar.h>

#define ID_BUTTON_HIDE 0
#define ID_BUTTON_ROTATE_LEFT 1
#define ID_BUTTON_ROTATE_RIGHT 2
#define ID_BUTTON_DELETE 3


namespace GNC {
	namespace GUI {
		
		//drag&drop////////////////////////////////////////////////////////
		class DropTargetSelectImages: public wxFileDropTarget
		{
		public:
			DropTargetSelectImages(SelectImagesImportation* pParent, const std::list<std::string>& fileSpecs) : 
				wxFileDropTarget(),
				m_pParent(pParent),
				m_fileSpecs(fileSpecs),
				m_enabled(true)
			{
			}

			~DropTargetSelectImages()
			{
			}

			void SetEnable(bool enabled) {
				m_enabled = enabled;
			}

			bool OnDropFiles(wxCoord /*x*/, wxCoord /*y*/, const wxArrayString& filenames)
			{
				if (!m_enabled)
					return false;
				wxArrayString images;
				for(wxArrayString::const_iterator it = filenames.begin(); it!= filenames.end(); ++it)
				{
					if(wxDirExists((*it))) {
						m_pParent->ScanPath((*it));
					} else if(wxFileExists((*it))){
						wxFileName filename((*it));
						wxString extensionFile = filename.GetExt().Lower();
						for (std::list<std::string>::iterator itSpecs = m_fileSpecs.begin(); itSpecs != m_fileSpecs.end(); ++itSpecs) {
							wxFileName ext(wxString::FromUTF8((*itSpecs).c_str()).Lower());
							if (extensionFile == ext.GetExt().Lower()) {
								images.push_back((*it));
								break;
							}
						}
					}
				}
				m_pParent->AddImages(images);
				return true;
			}

			SelectImagesImportation* m_pParent;
			std::list<std::string> m_fileSpecs;
			bool m_enabled;
		};
		/////////////////////////////////////////////////////TIMER DIRECTORY
		class TimerDirectory : public wxTimer {
		public:
			TimerDirectory(SelectImagesImportation* pSelectImages) :
				m_path(),
				m_monitorize(false),
				m_pSelectImages(pSelectImages)
			{
			}

			~TimerDirectory() 
			{
			}

			void SetPath(const std::string& path, bool monitorize)					
			{
				m_path = wxString::FromUTF8(path.c_str());
				m_monitorize = monitorize;
			}

			virtual void Notify()
			{
				if (m_monitorize) {
					if (m_pSelectImages->ScanPath(m_path)) {
						this->Start(1000, true);
					} else {
						wxMessageBox(_("Path of the location doesn't exists"), _("Info"), wxOK | wxICON_INFORMATION, m_pSelectImages);
					}
				} else {
					m_pSelectImages->AddImage(m_path);
				}
			}

		protected:
			wxString m_path;
			bool m_monitorize;
			SelectImagesImportation* m_pSelectImages;
		};


		////////////////////////////////////////////////////////

		SelectImagesImportation::SelectImagesImportation(wxWindow* pParent, const std::string& dirTemp, IWizard* pWizard, GNC::GCS::Ptr<GIL::IModeloIntegracion>& pModeloIntegracion, const std::list<std::string>& fileSpecs, const wxString& defaultFilter) : SelectImagesImportationBase(pParent), IPasoWizard(pWizard)
		{			
			SetScrollbars(10, 10, 50, 50, 0, 0, false);
			AdjustScrollbars();
			m_firstAttach = true;
			m_pwxImagePreview = new wxImage();

			m_fileSpecs = fileSpecs;
			if (defaultFilter.IsEmpty()) {
#if defined(_WIN32) || defined(__WXMAC__)
				m_pDefaultFilter =_("Images (*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff;*.pdf)|*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff;*.pdf|JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg|BMP Images (*.bmp)|*.bmp|PNG Images (*.png)|*.png|TIFF Images (*.tif;*.tiff)|*.tif;*.tiff|PDF documents (*.pdf)|*.pdf|All files(*.*)|*.*");
#else
				m_pDefaultFilter =_("Images (*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff;*.pdf)|*.jpg;*.jpeg;*.bmp;*.png;*.tif;*.tiff;*.pdf;*.JPG;*.JPEG;*.BMP;*.PNG;*.TIF;*.TIFF;*.PDF|JPEG Images (*.jpg;*.jpeg)|*.jpg;*.jpeg;*.JPG;*.JPEG|BMP Images (*.bmp)|*.bmp;*BMP|PNG Images (*.png)|*.png;*PNG|TIFF Images (*.tif;*.tiff)|*.tif;*.tiff;*TIF;*TIFF|PDF documents (*.pdf;*.PDF)|*.PDF;*.pdf|All files (*.*)|*.*");
#endif
				m_fileSpecs.push_back("*.jpg");
				m_fileSpecs.push_back("*.jpeg");
				m_fileSpecs.push_back("*.bmp");
				m_fileSpecs.push_back("*.png");
				m_fileSpecs.push_back("*.tif");
				m_fileSpecs.push_back("*.tiff");
				m_fileSpecs.push_back("*.pdf");
			} else {
				m_pDefaultFilter = defaultFilter;
			}

			m_pModeloIntegracion = pModeloIntegracion;
			Hide();
			m_dirTemp = dirTemp;

			wxColour barColor = wxColour(200,200,200);
			wxFont fontBar = wxFont( 10, 70, 90, wxFONTWEIGHT_BOLD, false, wxEmptyString );

			m_pButtonsBar = new wxGinkgoToolBar(m_panelStep0, barColor, fontBar);
			int idButton = 0;
			m_pButtonsBar->AddTool(idButton, _("Select files..."), GinkgoResourcesManager::MenuIcons::GetIcoImportar(), _("Select files from filesystem"));
			m_pButtonsBar->Connect(idButton++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SelectImagesImportation::OnSelectFiles), NULL, this);

			GNC::GCS::IEntorno::MapaUbicaciones& mapa = GNC::Entorno::Instance()->GetUbicaciones();

			for (GNC::GCS::IEntorno::MapaUbicaciones::iterator it = mapa.begin(); it != mapa.end(); ++it) {
				GIL::Ubicacion* u = (*it).second;

				m_pButtonsBar->AddTool(idButton, wxString::FromUTF8(u->Titulo.c_str()), GinkgoResourcesManager::MenuIcons::GetIcoImportar(), wxString::FromUTF8(u->Descripcion.c_str()), wxITEM_CHECK);
				m_pButtonsBar->Connect(idButton++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SelectImagesImportation::OnUbicacionClick), NULL, this);
			}
			m_pButtonsBar->AddSeparator();
			m_pButtonsBar->AddTool(idButton, _("Paste"), GinkgoResourcesManager::MenuIcons::GetIcoPaste(), _("Paste from clipboard"));
			m_pButtonsBar->Connect(idButton++, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler(SelectImagesImportation::OnPasteFromClipboard), NULL, this);

			m_pButtonsBar->Realize();
			m_pSizerButtons->Add(m_pButtonsBar, 1, wxEXPAND| wxALL, 0);

			wxGinkgoToolBar* pToolPreview = new wxGinkgoToolBar(m_pPreviewPanel, barColor, fontBar);
			pToolPreview->AddTool(ID_BUTTON_HIDE,_("Back"), GinkgoResourcesManager::Acquisition::GetIcoAdquireFile(), _("Back"));
			pToolPreview->Connect(ID_BUTTON_HIDE,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( SelectImagesImportation::OnHidePreview),NULL,this);

			pToolPreview->Realize();
			m_pPreviewPanel->GetSizer()->Insert(0,pToolPreview,0,wxEXPAND);
			m_pPreviewPanel->Layout();

			m_pImagenes->Connect(wxEVT_COMMAND_THUMBNAIL_LEFT_DCLICK,wxCommandEventHandler(SelectImagesImportation::OnThumbLeftDClickSelected),NULL,this);
			m_pImagenes->Connect(wxEVT_COMMAND_THUMBNAIL_RIGHT_CLICK,wxCommandEventHandler(SelectImagesImportation::OnThumbRightClickSelected),NULL,this);
			m_pImagenes->Connect(wxEVT_KEY_DOWN, wxKeyEventHandler(SelectImagesImportation::OnImagenesKeyDown),NULL,this);

			m_pTimerDirectory = new TimerDirectory(this);
			m_pDropTarget = new DropTargetSelectImages(this, m_fileSpecs);
			m_pImagenes->SetDropTarget(m_pDropTarget);

			m_panelStep0->Layout();
			Layout();
		}

		void SelectImagesImportation::SetDefaultImage(const wxImage& defaultImage)
		{
			m_pDefaultImage = defaultImage;
		}

		SelectImagesImportation::~SelectImagesImportation()
		{
			if (m_pTimerDirectory != NULL) {
				delete m_pTimerDirectory;
				m_pTimerDirectory = NULL;
			}
			GIL::Ubicacion* pUbicacion = GetUbicacionSelected();
			if (pUbicacion != NULL && pUbicacion->CleanAfter) {
				/*//remove content from ubicacion directory...
				*/
				wxArrayString paths=this->GetSelectedPaths();
				for (wxArrayString::iterator it = paths.begin(); it != paths.end(); ++it) {
					wxRemoveFile((*it));
				}
			}
			if (m_pwxImagePreview != NULL)
			{
				delete m_pwxImagePreview;
				m_pwxImagePreview = NULL;
			}
			m_pImagenes->Disconnect(wxEVT_COMMAND_THUMBNAIL_LEFT_DCLICK,wxCommandEventHandler(SelectImagesImportation::OnThumbLeftDClickSelected),NULL,this);
			m_pImagenes->Disconnect(wxEVT_KEY_DOWN, wxKeyEventHandler(SelectImagesImportation::OnImagenesKeyDown),NULL,this);
		}

		void SelectImagesImportation::OnImagenesKeyDown(wxKeyEvent &event)
		{
			if (event.GetKeyCode() == WXK_DELETE || event.GetKeyCode() == WXK_BACK) {
				DeleteSelectedThumbnails();
				event.Skip(false);
			}
			else if ( event.ControlDown()) {
				if (event.GetKeyCode() == 86 ) {
					wxCommandEvent ev;
					OnPasteFromClipboard(ev);
					event.Skip(false);
				}
				else {
					event.Skip(true);
				}
			}
			else {
				event.Skip(true);
			}
		}

		void SelectImagesImportation::OnDeleteSelectedThumbnails(wxCommandEvent &)
		{
			DeleteSelectedThumbnails();
		}

		void SelectImagesImportation::DeleteSelectedThumbnails()
		{
			bool deleteFromDisk = false;
			///if monitoring... ask
			GIL::Ubicacion* pUbicacion = GetUbicacionSelected();
			if (pUbicacion != NULL) {
				if (pUbicacion->Monitorize) {
					int answer = wxMessageBox(_("Are you sure to delete this file from disk?"),_("Delete"), wxYES_NO , this);
					if (answer == wxYES) {
						deleteFromDisk = true;
					} else {
						return;
					}
				}
			}
			while(m_pImagenes->GetSelection()!=-1){
				if (deleteFromDisk) {
					wxThumbnailItem* item = m_pImagenes->GetItem(m_pImagenes->GetSelection());
					if (item != NULL) {
						wxRemoveFile(item->GetFilename());
					}
				}
				m_pImagenes->Delete(m_pImagenes->GetSelection());
			}
		}

		void SelectImagesImportation::OnPreviewClick(wxCommandEvent &)
		{
			ShowPreview();
		}

		void SelectImagesImportation::OnThumbLeftDClickSelected(wxCommandEvent& event)
		{
			ShowPreview();
			event.Skip();
		}

		void SelectImagesImportation::OnThumbRightClickSelected(wxCommandEvent& event)
		{
			//menu...
			wxArrayInt selection = m_pImagenes->GetSelections();
			if (selection.size() == 0) {
				return;
			}

			wxMenu contextual;
			wxMenuItem* pRotateLeft = new wxMenuItem(&contextual, ID_BUTTON_DELETE, _("Delete selected items"));
			contextual.Connect(ID_BUTTON_DELETE, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( SelectImagesImportation::OnDeleteSelectedThumbnails),NULL,this);
			contextual.Append(pRotateLeft);
			m_pImagenes->PopupMenu(&contextual);
			event.Skip();
		}

		void SelectImagesImportation::ShowPreview()
		{
			// Si hacemos doble click ampliamos la imagen para una mejor visualizacion
			if (m_pImagenes->GetSelections().size() != 1) 
			{
				wxMessageBox(_("You must select an image"), _("Info"), wxOK | wxICON_INFORMATION, this);
				return;
			}

			wxBusyInfo info(_("Generating preview..."));
			wxThumbnailItem* item = m_pImagenes->GetItem(m_pImagenes->GetSelection());
			if (item == NULL)
				return;

			Freeze();
			//load preview...
			wxString pathTmp = item->GetFilename();
			if (m_pwxImagePreview->LoadFile(pathTmp)) 
			{
				m_pImagenes->Hide();
				m_pPreviewPanel->Show();
				m_pButtonsBar->Hide();
				m_pPreview->SetFocus();
				m_pButtonRemoveAll->Hide();
				m_pButtonPreview->Hide();



				vtkSmartPointer<vtkImageData> data = vtkSmartPointer<vtkImageData>::New();
				double spacing[3] = {1.0f,1.0f,1.0f};
				double origin[3] = {0.0f,0.0f,0.0f};

				data->SetDimensions(m_pwxImagePreview->GetWidth(), m_pwxImagePreview->GetHeight(), 1);
				//data->SetNumberOfScalarComponents(3);
				//data->SetScalarTypeToUnsignedChar();
				data->SetSpacing(spacing[0],spacing[1],spacing[2]);
				data->SetOrigin(origin[0],origin[1],origin[2]);

				vtkSmartPointer<vtkUnsignedCharArray> dataImagen = vtkSmartPointer<vtkUnsignedCharArray>::New();
				dataImagen->SetNumberOfComponents(3);
				unsigned int size = data->GetDimensions()[0] * data->GetDimensions()[1];
				dataImagen->SetArray(m_pwxImagePreview->GetData(),size,1);

				data->GetPointData()->SetScalars(dataImagen);

				m_pPreview->SetInput(data);
				m_pPreview->GetView()->SetInteractionStyle(vtkGinkgoImageViewer::ZOOM_WITH_SELECT_INTERACTION);


				m_panelStep0->Layout();
				m_panelStep0->Refresh();
			} else {
				wxFileName fileName(pathTmp);
				wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromExtension(fileName.GetExt());
				if (fileType != NULL) {
					wxExecute(fileType->GetOpenCommand(pathTmp));
					delete fileType;
				}
			}
			Thaw();
		}

		void SelectImagesImportation::OnHidePreview(wxCommandEvent& )
		{
			HidePreview();
		}

		void SelectImagesImportation::HidePreview()
		{
			if (!m_pImagenes->IsShown())
			{
				Freeze();
				m_pImagenes->Show();
				m_pPreviewPanel->Hide();
				m_pButtonRemoveAll->Show();
				m_pButtonPreview->Show();
				m_pButtonsBar->Show();

				m_panelStep0->Layout();
				m_panelStep0->Refresh();
				Thaw();
			}
		}

		void SelectImagesImportation::OnRemoveAll(wxCommandEvent &event){
			m_pImagenes->SelectAll();
			DeleteSelectedThumbnails();
			event.Skip();
		}

		void SelectImagesImportation::OnPasteFromClipboard(wxCommandEvent& ) {

			if (wxTheClipboard->Open()) {
				if (wxTheClipboard->IsSupported( wxDF_BITMAP ))
				{
					wxBitmapDataObject bmpData;
					wxTheClipboard->GetData( bmpData );
					wxBitmap bmp = bmpData.GetBitmap();

					wxFileName tempDir = FROMPATH(m_dirTemp);
					wxFileName tempFileName(wxT("clipboard_") + wxString::Format(wxT("%d"), (int) rand()));
					//if files will be cleaned at the end, we have to copy it in temporally folder
					wxString tempFile = tempDir.GetFullPath() + wxFileName::GetPathSeparator() + tempFileName.GetName() + wxT(".bmp");

					if (bmp.SaveFile(tempFile, wxBITMAP_TYPE_BMP)) {

						wxThumbnailItem* pItem = NULL;
						pItem = new wxMemoryImageThumbnailItem(tempFile, bmp.ConvertToImage());
						m_pImagenes->Insert( pItem, m_pImagenes->GetCount());

						{
							GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Importacion", "LastMethod", "clipboard");
							GNC::GCS::ConfigurationController::Instance()->Flush();
						}
					}
					else {
						LOG_ERROR("Importation", "Unable to save bitmap");
					}
					
				} else if(wxTheClipboard->IsSupported( wxDF_FILENAME )) {
					wxFileDataObject fileData;
					wxTheClipboard->GetData(fileData);
					wxArrayString images;
					for(wxArrayString::const_iterator it = fileData.GetFilenames().begin(); it!= fileData.GetFilenames().end(); ++it)
					{
						if(wxDirExists((*it))) {
							ScanPath((*it));
						} else if(wxFileExists((*it))){
							wxFileName filename((*it));
							wxString extensionFile = filename.GetExt().Lower();
							for (std::list<std::string>::iterator itSpecs = m_fileSpecs.begin(); itSpecs != m_fileSpecs.end(); ++itSpecs) {
								wxFileName ext(wxString::FromUTF8((*itSpecs).c_str()).Lower());
								if (extensionFile == ext.GetExt().Lower()) {
									images.push_back((*it));
									break;
								}
							}
						}
					}
					AddImages(images);
				}
				wxTheClipboard->Close();
			}

		}

		void SelectImagesImportation::OnSelectFiles(wxCommandEvent & )
		{
			UnCheckAll();
			m_pDropTarget->SetEnable(true);
			m_pTimerDirectory->Stop();
			std::string pathDefecto;
			GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Importacion", "PathDefecto", pathDefecto);
			wxString wxPathDefecto = wxString::FromUTF8(pathDefecto.c_str());
			if(wxFileExists(wxPathDefecto) || wxDirExists(wxPathDefecto)) {
				AddImage(wxPathDefecto);
			} else {
				AddImage(wxEmptyString);
			}
		}

		void SelectImagesImportation::AddImage(const wxString& path)
		{
			wxFileDialog seleccionarFichero (this, _("Select images"), path, wxT(""), m_pDefaultFilter, wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_MULTIPLE);
			int response = seleccionarFichero.ShowModal();
			if (response == wxID_OK) {
				Update();
				wxBusyInfo info(_("Creating Thumbnail..."));
				wxArrayString paths;
				seleccionarFichero.GetPaths(paths);

				{
					GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Importacion", "PathDefecto", std::string(wxPathOnly(seleccionarFichero.GetPath()).ToUTF8()));
					GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Importacion", "LastMethod", "file");
					GNC::GCS::ConfigurationController::Instance()->Flush();
				}

				for (wxArrayString::iterator it = paths.begin(); it != paths.end(); ++it) {
					wxString& fileName = (*it);
					if(m_pImagenes->FindItemForFilename(fileName)==-1){
						wxThumbnailItem* pItem = NULL;
						if (m_pDefaultImage.IsOk()) {
							pItem = new wxMemoryImageThumbnailItem(fileName, m_pDefaultImage);
						} else {
							pItem = new wxImageThumbnailItem(fileName);
						}						
						m_pImagenes->Insert( pItem, m_pImagenes->GetCount());
					}
				}
			}
		}

		void SelectImagesImportation::AddImages(const wxArrayString& files)
		{
			for (wxArrayString::const_iterator it = files.begin(); it != files.end(); ++it) {
				const wxString& fileName = (*it);
				if(m_pImagenes->FindItemForFilename(fileName)==-1){
					wxBusyInfo info(_("Creating Thumbnails..."));
					wxThumbnailItem* pItem = NULL;
					if (m_pDefaultImage.IsOk()) {
						pItem = new wxMemoryImageThumbnailItem(fileName, m_pDefaultImage);
					} else {
						pItem = new wxImageThumbnailItem(fileName);
					}						
					m_pImagenes->Insert( pItem, m_pImagenes->GetCount());
					Update();
				}
			}
		}

		bool SelectImagesImportation::ScanPath(const wxString& path)
		{
			//se leen los ficheros dicom del directorio
			if (!wxDirExists(path)) {
				return false;
			}

			wxDir dir;
			if (dir.Open(path)) {
				wxArrayString images;
				for (std::list<std::string>::iterator itSpecs = m_fileSpecs.begin(); itSpecs != m_fileSpecs.end(); ++itSpecs) {
					wxString wxPathFich;
					bool cont = dir.GetFirst(&wxPathFich, wxString::FromUTF8((*itSpecs).c_str()));
					while (cont) {
						wxPathFich=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +wxPathFich;	
						images.push_back(wxPathFich);							
						cont = dir.GetNext(&wxPathFich);
					}
				}
#if !defined(_WINDOWS)
				for (std::list<std::string>::iterator itSpecs = m_fileSpecs.begin(); itSpecs != m_fileSpecs.end(); ++itSpecs) {
					wxString wxPathFich;
					bool cont = dir.GetFirst(&wxPathFich, wxString::FromUTF8((*itSpecs).c_str()).Upper());
					while (cont) {
						wxPathFich=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +wxPathFich;	
						images.push_back(wxPathFich);							
						cont = dir.GetNext(&wxPathFich);
					}
				}
#endif
				AddImages(images);
			}
			return true;
		}

		wxArrayString SelectImagesImportation::GetSelectedPaths()
		{
			wxArrayString resultado;
			for(int i=0;i<m_pImagenes->GetCount();++i){
				wxThumbnailItem* item=dynamic_cast<wxThumbnailItem*>(m_pImagenes->GetItem(i));
				if(item!=NULL){
					resultado.push_back(item->GetFilename());
				}
			}
			return resultado;
		}

		void SelectImagesImportation::UnCheckAll()
		{
			for (unsigned int i = 1; i < m_pButtonsBar->GetToolCount(); i++) {
				//uncheck ubicaciones
				m_pButtonsBar->ForceToggleTool(i, false);
			}
			m_pButtonsBar->Refresh(true);
		}

		void SelectImagesImportation::OnUbicacionClick(wxCommandEvent& event)
		{
			int id = event.GetId();
			if (id == 0) {
				UnCheckAll();
				m_pTimerDirectory->Stop();
				m_pDropTarget->SetEnable(true);
			} else {
				GNC::GCS::IEntorno::MapaUbicaciones& mapa = GNC::Entorno::Instance()->GetUbicaciones();
				GNC::GCS::IEntorno::MapaUbicaciones::iterator it = mapa.begin();
				for (int i = 1; i != id && it != mapa.end(); i++, ++it);
				if ((*it).second->Monitorize && !m_pButtonsBar->GetToolToggled(id)) {
					//if uncheck a monitorize change to normal mode
					UnCheckAll();
					m_pTimerDirectory->Stop();
					m_pDropTarget->SetEnable(true);
				} else {
					SetUbicacion((*it).second, id);
				}
			}
		}

		void SelectImagesImportation::SetUbicacion(GIL::Ubicacion* ubicacion, int id)
		{
			UnCheckAll();
			m_pTimerDirectory->Stop();
			m_pButtonsBar->ForceToggleTool(id, true);
			if (!ubicacion->Monitorize) {
				m_pDropTarget->SetEnable(true);
				m_pTimerDirectory->SetPath(ubicacion->Ruta, false);
			} else {
				m_pDropTarget->SetEnable(false);
				m_pImagenes->Clear();
				m_pTimerDirectory->SetPath(ubicacion->Ruta,true);
			}
			if (ubicacion->CleanBefore) {
				wxDir dir;
				if (dir.Open(FROMPATH(ubicacion->Ruta))) {
					wxString fileName;
					bool cont = dir.GetFirst(&fileName);
					while (cont) {
						fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
						if(wxFile::Exists(fileName)){
							if (wxRemoveFile(fileName)) {
								cont = dir.GetNext(&fileName);
							} else {
								break;
							}
						}
					}
				}
			}
			m_pTimerDirectory->Start(100, true);
		}

		GIL::Ubicacion* SelectImagesImportation::GetUbicacionSelected()
		{
			GNC::GCS::IEntorno::MapaUbicaciones& mapa = GNC::Entorno::Instance()->GetUbicaciones();
			GNC::GCS::IEntorno::MapaUbicaciones::iterator it = mapa.begin();
			for (unsigned int i = 1; i < m_pButtonsBar->GetToolCount() && it != mapa.end(); i++, ++it) {
				if (m_pButtonsBar->GetToolToggled(i)) {
					return (*it).second;
				}
			}
			return NULL;
		}

		//region "Metodos heredados de Ipasowizard"

		bool SelectImagesImportation::Attach()
		{
			if (m_firstAttach) {
				//maybe an ubication, maybe a normal path...
				std::string lastLocation;
				GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Importacion", "LastLocation", lastLocation);
				GNC::GCS::IEntorno::MapaUbicaciones& mapa = GNC::Entorno::Instance()->GetUbicaciones();
				GNC::GCS::IEntorno::MapaUbicaciones::iterator it = mapa.begin();
				unsigned int id = 1;
				for (; id < m_pButtonsBar->GetToolCount() && it != mapa.end(); id++, ++it) {
					if ((*it).first == lastLocation) {
						break;
					}
				}
				if (it == mapa.end()) {
					//opens select file dialog...
					std::string method;
					GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Importacion", "LastMethod", method);
					if (method.empty() || method == "file") {
						wxCommandEvent openDialogEvent(wxEVT_COMMAND_TOOL_CLICKED, 0);
						m_pButtonsBar->GetEventHandler()->AddPendingEvent(openDialogEvent);
					}
				} else {
					SetUbicacion((*it).second, id);
				}
				m_firstAttach = false;
			}

			GIL::Ubicacion* pUbicacion = GetUbicacionSelected();
			if (pUbicacion != NULL && pUbicacion->Monitorize) {
				m_pTimerDirectory->Start(100,true);
			}

			return true;
		}

		bool SelectImagesImportation::Detach()
		{
			m_pTimerDirectory->Stop();
			return true;
		}

		std::string SelectImagesImportation::GetTitle(){
			return _Std("Image selection");
		}

		std::string SelectImagesImportation::GetSubTitle(){
			return _Std("You must select the images you want to import");
		}

		bool SelectImagesImportation::Siguiente(){
			return true;
		}

		bool SelectImagesImportation::Anterior(){
			return true;
		}

		bool SelectImagesImportation::Cancelar(){
			return true;
		}

		void SelectImagesImportation::OnCancelled()
		{
			m_pTimerDirectory->Stop();
			m_pImagenes->Clear();
		}

		bool SelectImagesImportation::Validar(){
			wxArrayString paths=this->GetSelectedPaths();
			if(paths.GetCount()==0){
				wxMessageBox(_("You must select at least one image"), _("Info"),
					wxOK | wxICON_INFORMATION, this);
				return false;
			}

			m_pListaFicheros->clear();
			GIL::Ubicacion* pUbicacion = GetUbicacionSelected();

			if (pUbicacion != NULL) {
				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Importacion", "LastLocation", pUbicacion->Titulo);
			} else {
				GNC::GCS::ConfigurationController::Instance()->deleteEntryUser("/GinkgoCore/Importacion", "LastLocation");
			}

			//file must exist
			for(wxArrayString::iterator it=paths.begin();it!=paths.end();++it){
				if(!wxFileName::FileExists((*it))){
					wxString str(_("The file"));
					wxMessageBox(str <<(*it) <<_(" doesn't exist"), _("Info"),
						wxOK | wxICON_INFORMATION, this);
					m_pListaFicheros->clear();
					//el fichero no existe
					return false;
				}

				wxString wxDirTmp = FROMPATH(m_dirTemp);
				wxFileName fileName((*it));
				//if files will be cleaned at the end, we have to copy it in temporally folder
				if (pUbicacion != NULL && pUbicacion->CleanAfter) {
					wxString nombreImagenTemporal = wxDirTmp + wxFileName::GetPathSeparator() + fileName.GetName() + wxString::Format(wxT("%d"), (int) rand()) + wxT(".jpg");
					while(wxFile::Exists(nombreImagenTemporal))
					{
						//si existe se busca uno que no exista
						nombreImagenTemporal= wxDirTmp + wxFileName::GetPathSeparator() + fileName.GetName() + wxString::Format(wxT("%d"), (int) rand()) + wxT(".jpg");
					}
					if (!wxCopyFile((*it), nombreImagenTemporal)) {
						LOG_ERROR("SelectImagesImportation", "Error copying images from " << (*it).ToUTF8() << " TO " << nombreImagenTemporal.ToUTF8());
					}
					std::string path(TOPATH(nombreImagenTemporal));
					m_pListaFicheros->push_back(path);					
				} else {
					std::string path(TOPATH((*it)));
					m_pListaFicheros->push_back(path);
				}
			}

			//
			return true;
		}
		//endregion
	}
}
