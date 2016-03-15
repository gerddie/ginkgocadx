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

#include <vector>

#include <wx/checkbox.h>
#include <wx/msgdlg.h>
#include <wx/msgout.h>
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <main/controllers/configurationcontroller.h>
#include <sstream>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/stdpaths.h>
#include <wx/xml/xml.h>

#include <wx/ginkgostyle/ginkgostyle.h>

#include "localdatabaseconfigurationpanel.h"

#include <main/controllers/controladorextensiones.h>
#include <main/controllers/controladoreventos.h>
#include <main/controllers/controladorvistas.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/autocleaningcontroller.h>
#include <main/gui/history3/historypanel3.h>
#include <main/entorno.h>
#include <api/controllers/icontroladorinternacionalizacion.h>
#include <api/internationalization/internationalization.h>

namespace GNC {
	namespace GUI {

		LocalDatabaseConfigurationPanel::LocalDatabaseConfigurationPanel(wxWindow* pParent,IDialogoConfiguracion* pDialogo): LocalDatabaseConfigurationPanelBase(pParent), IPasoConfiguracion(pDialogo)
		{	
			if (GNC::GCS::ControladorVistas::Instance()->GetVistas().size() > 0)
			{
				m_pDICOMDir->Enable(false);
				m_buttonSeleccionarDICOMDir->Enable(false);
				m_pstaticTextAdvertencia->Show(true);
			}
			else
			{
				m_pstaticTextAdvertencia->Show(false);
			}

			m_mapTimePeriods[1] = _Std("One day");
			m_mapTimePeriods[2] = _Std("Two days");
			m_mapTimePeriods[3] = _Std("Three days");
			m_mapTimePeriods[7] = _Std("One week");
			m_mapTimePeriods[14] = _Std("Two weeks");
			m_mapTimePeriods[30] = _Std("One month");
			m_mapTimePeriods[90] = _Std("Three months");
			m_mapTimePeriods[365] = _Std("One year");

			for (std::map<int, std::string>::const_iterator it = m_mapTimePeriods.begin(); it != m_mapTimePeriods.end(); ++it) {
				m_pChoiceDeleteOlder->AppendString(wxString::FromUTF8((*it).second.c_str()));
			}
			m_pChoiceDeleteOlder->SetSelection(6);

			Recargar();
			Layout();
		}

		LocalDatabaseConfigurationPanel::~LocalDatabaseConfigurationPanel()
		{
		}

		void LocalDatabaseConfigurationPanel::Recargar()
		{
			std::string dicomDir;
			
			if(!GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Estacion","DicomDir", dicomDir)) {
				m_pathDicomDir = FROMPATH(GNC::GCS::HistoryController::Instance()->GetGinkgoDicomDir());
			} else {
				m_pathDicomDir = wxString::FromUTF8(dicomDir.c_str());
			}
			m_pDICOMDir->SetValue(m_pathDicomDir);
			
			//autocleaning dicomdir
			int tmp;
			//older than...
			GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "AcquisitionOlderThan", tmp, -1);
			if (tmp > 0) {
				int i = 0;
				for (std::map<int, std::string>::const_iterator it = m_mapTimePeriods.begin(); it != m_mapTimePeriods.end(); ++it,i++) {
					if ((*it).first == tmp) {
						m_pChoiceDeleteOlder->SetSelection(i);
						break;
					}
				}
				m_pChoiceDeleteOlder->Enable(true);
				m_pCheckDeleteOlder->SetValue(true);
			} else {
				m_pChoiceDeleteOlder->Enable(false);
				m_pCheckDeleteOlder->SetValue(false);
			}
			//hdFreeSpace...
			GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "HDFreeSpace", tmp, -1);
			if (tmp > 0) {
				m_pSpinHDSpace->SetValue(tmp);
				m_pSpinHDSpace->Enable(true);
				m_pCheckHDFreeSpace->SetValue(true);
			} else {
				m_pSpinHDSpace->Enable(false);
				m_pCheckHDFreeSpace->SetValue(false);
			}
			//DicomDirSize...
			GNC::GCS::ConfigurationController::Instance()->readIntUser("/GinkgoCore/Station/CleanDicomDir", "DicomDirSize", tmp, -1);
			if (tmp > 0) {
				m_pSpinDicomDirSize->SetValue(tmp);
				m_pSpinDicomDirSize->Enable(true);
				m_pCheckDicomDirSize->SetValue(true);
			} else {
				m_pSpinDicomDirSize->Enable(false);
				m_pCheckDicomDirSize->SetValue(false);
			}
		}

		//region "Metodos de IPasoConfiguracion"
		wxWindow* LocalDatabaseConfigurationPanel::GetPanel()
		{
			return this;
		}

		std::string LocalDatabaseConfigurationPanel::GetTitle()
		{
			return _Std("Local database");
		}

		std::string LocalDatabaseConfigurationPanel::GetCabecera()
		{
			return _Std("Local DICOM Dir setup");
		}

		bool LocalDatabaseConfigurationPanel::Validar()
		{
			wxString path = m_pDICOMDir->GetValue();
			wxFileName fileName(path);
			if(!fileName.IsAbsolute()) {
				wxFileName pathOfExec(wxStandardPaths::Get().GetExecutablePath());
				fileName.MakeAbsolute(pathOfExec.GetPath());
				path = fileName.GetFullPath();
			}

			if(!wxDir::Exists(path)) {
				//se pregunta si desea crearlo
				int answer = wxMessageBox(_("The selected directory does not exist\nWould you like to create it?"),_("Info"),wxYES_NO,this);
				if(answer == wxNO) {
					return false;
				} else {
                    bool success;
		#ifdef _WIN32
                    success = wxMkdir(path,511);
		#else
                    success = (wxMkDir(path.ToUTF8(), 0770) == 0);
		#endif
                    if(!success || !wxDirExists(path))
					{
						wxMessageBox(_("Failed to create directory, make sure the path is correct and you have permissions on parent directory"), _("Info"));
						return false;
					}
				}
			} else {
				//se comprueba que puede escribir(escribimos y borramos un fichero chorra)
				wxString pathFicheroPrueba = path + wxFileName::GetPathSeparator() + wxString::Format(wxT("%d"), (int) rand());
				while(wxFileExists(pathFicheroPrueba)) {
					pathFicheroPrueba = path + wxFileName::GetPathSeparator() + wxString::Format(wxT("%d"), (int) rand());
				}
				{
					wxFile ficheroTmp;
					if(!ficheroTmp.Create(pathFicheroPrueba,false)){
						if(!wxFileExists(pathFicheroPrueba) ) {
							wxMessageBox(_("There was an error reading the directory, make sure you have write permissions on it"), _("Info"));
								return false;
						}
					}
					ficheroTmp.Write(wxT("prueba"));
					ficheroTmp.Close();
				}
				wxRemoveFile(pathFicheroPrueba);
			}

			return true;
		}

		bool LocalDatabaseConfigurationPanel::Guardar()
		{
			//se actualiza el fichero de configuracion
			wxFileName dirAnterior(m_pathDicomDir);
			wxFileName dirNuevo(m_pDICOMDir->GetValue());
			if(dirAnterior != dirNuevo)
			{
				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Estacion", "DicomDir",std::string(m_pDICOMDir->GetValue().ToUTF8()));
				//se vacia el dicomdir actual y se carga el nuevo...
				GNC::GCS::HistoryController::FreeInstance();
				if (!GNC::GCS::HistoryController::Instance()->IsOk()) {
					//revert changes....
					GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Estacion", "DicomDir", std::string(m_pathDicomDir.ToUTF8()));
					//se vacia el dicomdir actual y se carga el nuevo...
					GNC::GCS::HistoryController::FreeInstance();
				}
				GNC::GUI::HistoryPanel3::Instance()->ReloadHistory();
				
			}

			//dicomdir auto cleaning
			//older than...
			int tmp = -1;
			if (m_pChoiceDeleteOlder->IsEnabled()) {
				for (std::map<int, std::string>::const_iterator it = m_mapTimePeriods.begin(); it != m_mapTimePeriods.end(); ++it) {
					if (wxString::FromUTF8((*it).second.c_str()).CompareTo(m_pChoiceDeleteOlder->GetString(m_pChoiceDeleteOlder->GetSelection())) == 0) {
						tmp = (*it).first;
						break;
					}
				}
			}
			GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/Station/CleanDicomDir", "AcquisitionOlderThan", tmp);
			//hdFreeSpace...
			tmp = -1;
			if (m_pSpinHDSpace->IsEnabled()) {
				tmp = m_pSpinHDSpace->GetValue();
			}
			GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/Station/CleanDicomDir", "HDFreeSpace", tmp);
			//DicomDirSize...
			tmp = -1;
			if (m_pSpinDicomDirSize->IsEnabled()) {
				tmp = m_pSpinDicomDirSize->GetValue();
			}
			GNC::GCS::ConfigurationController::Instance()->writeIntUser("/GinkgoCore/Station/CleanDicomDir", "DicomDirSize", tmp);

			GNC::GCS::AutoCleaningController::Instance()->Reset();

			return true;
		}
		//endregion

		void LocalDatabaseConfigurationPanel::OnTextoCambiado(wxCommandEvent &)
		{
			if(IsShown()){
				OnPropiedadCambiada();
			}
		}

		void LocalDatabaseConfigurationPanel::OnSeleccionarDirectorio(wxCommandEvent &)
		{
			wxDirDialog seleccionarDirectorio(this,_("Select DICOMDIR"),m_pDICOMDir->GetValue(),wxDD_DEFAULT_STYLE|wxDD_NEW_DIR_BUTTON);
			int response = seleccionarDirectorio.ShowModal();
			if (response == wxID_OK)
			{
				m_pDICOMDir->SetValue(seleccionarDirectorio.GetPath());
			}
		}

		void LocalDatabaseConfigurationPanel::OnSize(wxSizeEvent & event){
			wxSize oldSize = m_pstaticTextAdvertencia->GetSize();
			Freeze();
			m_pstaticTextAdvertencia->Freeze();
			m_pstaticTextAdvertencia->SetLabel(_("Warning, to update the directory, you must close current opened series."));
			m_pstaticTextAdvertencia->Wrap( event.GetSize().x - 25 );
			m_pstaticTextAdvertencia->Thaw();
			Thaw();
			wxSize newSize = m_pstaticTextAdvertencia->GetSize();
			if (newSize != oldSize) {
				Layout();
				event.Skip(false);
			} else {
				event.Skip(true);
			}
		}

		
		void LocalDatabaseConfigurationPanel::OnCheckOlderStudies( wxCommandEvent& event )
		{
			m_pChoiceDeleteOlder->Enable(m_pCheckDeleteOlder->GetValue());
			OnPropiedadCambiada();
			event.Skip(true);
		}

		void LocalDatabaseConfigurationPanel::OnCheckDicomDirSize(wxCommandEvent& event)
		{
			m_pSpinDicomDirSize->Enable(m_pCheckDicomDirSize->GetValue());
			OnPropiedadCambiada();
			event.Skip(true);
		}

		void LocalDatabaseConfigurationPanel::OnCheckHDSpace(wxCommandEvent& event)
		{
			m_pSpinHDSpace->Enable(m_pCheckHDFreeSpace->GetValue());
			OnPropiedadCambiada();
			event.Skip(true);
		}

		void LocalDatabaseConfigurationPanel::OnSpinCtrl(wxSpinEvent& /*evt*/)
		{
			OnPropiedadCambiada();
		}

		void LocalDatabaseConfigurationPanel::OnOptionModified(wxCommandEvent& /*evt*/)
		{
			OnPropiedadCambiada();
		}

		void LocalDatabaseConfigurationPanel::OnResetToDefault(wxCommandEvent& /*evt*/)
		{
			m_pDICOMDir->SetValue(FROMPATH(GNC::GCS::HistoryController::Instance()->GetDefaultDicomDirPath(true)));
			m_pCheckDeleteOlder->SetValue(false);
			m_pCheckHDFreeSpace->SetValue(false);
			m_pCheckDicomDirSize->SetValue(false);
			m_pChoiceDeleteOlder->Enable(m_pCheckDeleteOlder->GetValue());
			m_pSpinDicomDirSize->Enable(m_pCheckDicomDirSize->GetValue());
			m_pSpinHDSpace->Enable(m_pCheckHDFreeSpace->GetValue());
			OnPropiedadCambiada();
		}
	}
}
