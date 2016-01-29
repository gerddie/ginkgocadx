/*
 *  
 *  $Id: parametrosprincipales.cpp $
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

#include <wx/file.h>
#include <wx/dir.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <main/controllers/configurationcontroller.h>
#include <wx/msgout.h>
#include <wx/filename.h>
#include <wx/propgrid/propgrid.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include "parametrosprincipales.h"

#include <api/ientorno.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <api/controllers/ipacscontroller.h>
#include <api/dicom/idicommanager.h>
#include <api/internationalization/internationalization.h>
#include <export/contracts/iwidgetscontract.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>

#include <main/controllers/commandcontroller.h>

#include <commands/comandoexportacion.h>
#include <api/istudycontext.h>

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
#include <vtkPNGWriter.h>
#include <vtkBMPWriter.h>


namespace GNC {
	namespace GUI {

		ParametrosPrincipales::ParametrosPrincipales(wxWindow* pParent,IWizard* pWizard, GNC::GCS::IWidgetsContract* widgetsContract):ParametrosPrincipalesBase(pParent),IPasoWizard(pWizard),
			WidgetsContract(widgetsContract)
		{			
			//pillamos los mapas
			bool valBoolTmp;
			GNC::GCS::ConfigurationController::Instance()->readBoolUser("/GinkgoCore/ImageExport", "Scope", valBoolTmp, false);
			m_pAllFiles->SetValue(valBoolTmp);

			int valTmp;
			GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/ImageExport", "Format", valTmp, 0);
			m_pFormat->SetSelection(valTmp);
			wxCommandEvent evt;
			OnChoiceFormatoDestino(evt);
			
			Layout();
		}

		ParametrosPrincipales::~ParametrosPrincipales()
		{			
		}


		void ParametrosPrincipales::OnChoiceFormatoDestino(wxCommandEvent &)
		{
			switch(m_pFormat->GetSelection()){
				case 0:
					m_pAllFiles->Enable(true);
					break;
				case 1:
					m_pAllFiles->Enable(false);
					break;
			}
			Layout();
		}


//region "Metodos heredados de Ipasowizard"
		std::string ParametrosPrincipales::GetTitle(){
			return _Std("DICOM Export");
		}

		std::string ParametrosPrincipales::GetSubTitle(){
			return _Std("Select the format and the images you want to export");
		}

		bool ParametrosPrincipales::Siguiente(){
			return true;
		}

		bool ParametrosPrincipales::Anterior(){
			return false;
		}

		bool ParametrosPrincipales::Cancelar(){
			return true;
		}

		bool ParametrosPrincipales::Validar()
		{
			GNC::GCS::ConfigurationController::Instance()->writeBoolUser("/GinkgoCore/ImageExport", "Scope", m_pAllFiles->GetValue());
			GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/ImageExport", "Format", m_pFormat->GetSelection());
			
			//select path...		
			std::string pathDef;
			GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Exportacion", "PathDefecto", pathDef);
			wxString wxPathDefecto = wxString::FromUTF8(pathDef.c_str());
			
			int response;
			wxString wxPath;
			if (!m_pAllFiles->IsEnabled() || !m_pAllFiles->IsChecked())
			{
				//select only a file to store
				wxString fileWildCard;
				if (m_pFormat->GetSelection() == 0)//dicom
				{
					fileWildCard = _("DICOM (*.dcm;)|*.dcm|All files(*.*)|*.*");
				}
				else
				{
					fileWildCard = _("JPEG (*.jpg;)|*.jpg|BMP (*.bmp)|*.bmp|PNG (*.png)|*.png|All files(*.*)|*.*");
				}

				wxFileDialog selectFile(this, _("Select file"), wxPathDefecto, wxEmptyString, fileWildCard, wxFD_SAVE);
				response = selectFile.ShowModal();
				wxPath = selectFile.GetPath();
			}
			else
			{
				//select a folder
				wxDirDialog seleccionarDirectorio(this,_("Select the directory where the files will be stored"),wxPathDefecto,wxDD_DEFAULT_STYLE|wxDD_NEW_DIR_BUTTON);
				response = seleccionarDirectorio.ShowModal();
				wxPath = seleccionarDirectorio.GetPath();
				if( !wxDirExists(wxPath) ){
					wxMessageDialog dialog(NULL,_("The selected directory does not exist\nWould you like to create?"),_("Info"),wxYES_NO|wxICON_INFORMATION);
					if ( dialog.ShowModal() == wxID_NO)
					{
						return false;;
					}
					if(!wxFileName::Mkdir(wxPath)){
						wxMessageBox(_("There was an error creating directory"), _("Info"),
							wxOK | wxICON_INFORMATION, this);
						return false;
					}
				}
			}
			if (response == wxID_OK)
			{
				//if it is an image file it has to be exported inmediatly... because it depends on the view
				if (m_pFormat->GetSelection() == 0) //DICOM
				{
					GNC::GCS::Ptr<TipoWizardExportacion> m_pDatosPersistentes(new TipoWizardExportacion());
					m_pDatosPersistentes->m_pathDestino = TOPATH(wxPath);

					//get paths to be exported from view...
					if (m_pAllFiles->IsChecked())
					{
						std::vector<long>& openedSeries = WidgetsContract->GetManager()->GetVista()->GetEstudio()->OpenedSeries;
						m_pDatosPersistentes->ListOfSeriesPks.insert(m_pDatosPersistentes->ListOfSeriesPks.begin(), openedSeries.begin(), openedSeries.end());
					} 
					else 
					{
						m_pDatosPersistentes->ListOfDICOMPaths.push_back(WidgetsContract->GetManager()->GetVista()->GetCurrentDICOM());
					}

					GADAPI::ComandoExportacionParams* pParams = new GADAPI::ComandoExportacionParams(m_pDatosPersistentes);
					GADAPI::ComandoExportacion* pCmd = new GADAPI::ComandoExportacion(pParams);
					GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Exporting images ..."), pCmd, NULL);
				}
				else
				{
					//export image!
					ExportImage(wxPath);
				}
				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Exportacion", "PathDefecto",std::string(wxPath.ToUTF8()));
				return true;
			} else {
				return false;
			}
		}

		void ParametrosPrincipales::ExportImage(const wxString& path)
		{
			vtkSmartPointer<vtkWindowToImageFilter> windowToImageFilter = 
			vtkSmartPointer<vtkWindowToImageFilter>::New();
			windowToImageFilter->SetInput(WidgetsContract->GetViewerActivo()->GetRenderWindow());
			windowToImageFilter->SetInputBufferTypeToRGB(); //also record the alpha (transparency) channel
			windowToImageFilter->Update();

			wxFileName fn(path);
			if (fn.GetExt() == wxT("bmp"))
			{
				vtkSmartPointer<vtkBMPWriter> writer = 
					vtkSmartPointer<vtkBMPWriter>::New();
				writer->SetFileName(TOPATH(path).c_str());
				writer->SetInputConnection(windowToImageFilter->GetOutputPort());
				writer->Write();
			}
			else if (fn.GetExt() == wxT("png"))
			{
				vtkSmartPointer<vtkPNGWriter> writer = 
					vtkSmartPointer<vtkPNGWriter>::New();
				writer->SetFileName(TOPATH(path).c_str());
				writer->SetInputConnection(windowToImageFilter->GetOutputPort());
				writer->Write();
			}
			else
			{
				vtkSmartPointer<vtkJPEGWriter> writer = 
					vtkSmartPointer<vtkJPEGWriter>::New();
				writer->SetFileName(TOPATH(path).c_str());
				writer->SetQuality(97);
				writer->ProgressiveOn();
				writer->SetInputConnection(windowToImageFilter->GetOutputPort());
				writer->Write();
			}
		}
	};

};
