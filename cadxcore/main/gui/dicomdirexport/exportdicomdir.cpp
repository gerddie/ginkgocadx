/*
 *  
 *  $Id: exportdicomdir.cpp $
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

#include <wx/file.h>
#include <wx/filename.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>

#include "exportdicomdir.h"
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/commandcontroller.h>
#include <commands/exporttodicomdircommand.h>
#include <api/internationalization/internationalization.h>

#include <main/controllers/anonusagestats.h>

namespace GNC {
	namespace GUI {

		ExportDicomDir::ExportDicomDir(wxWindow* pParent, const TIDList& pks): ExportDicomBase(pParent)
		{
			LOG_STAT(ANON_STAT_DLG_DICOM_DIR_EXPORT)
			m_pks = pks;
			Layout();
		}
		

		ExportDicomDir::~ExportDicomDir()
		{
		}

		void ExportDicomDir::OnExportClick(wxCommandEvent&)
		{
			std::string pathDef;
			GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Exportacion", "PathDefecto", pathDef);
			wxString wxPathDefecto = wxString::FromUTF8(pathDef.c_str());
			wxDirDialog seleccionarDirectorio(this,_("Select the directory where the files will be stored"),wxPathDefecto,wxDD_DEFAULT_STYLE|wxDD_NEW_DIR_BUTTON);
			int response = seleccionarDirectorio.ShowModal();
			if (response == wxID_OK)
			{
				wxString wxPath = seleccionarDirectorio.GetPath();

				if( !wxDirExists(wxPath) ){
					wxMessageDialog dialog(NULL,_("The selected directory does not exist\nWould you like to create?"),_("Info"),wxYES_NO|wxICON_INFORMATION);
					if ( dialog.ShowModal() == wxID_NO)
					{
						return;
					}
					if(!wxFileName::Mkdir(wxPath)){
						wxMessageBox(_("There was an error creating directory"), _("Info"),
							wxOK | wxICON_INFORMATION, this);
						return;
					}
				} else {
					if (wxDirExists(wxPath + wxFileName::GetPathSeparator() + wxT("DICOM"))) {
						wxMessageBox(_("There are other DICOM directory in selected path"), _("Info"),
							wxOK | wxICON_INFORMATION, this);
						return;
					}
				}
				//anonimization
				GIL::DICOM::DicomDataset base;
				if (m_pCheckPatientName->IsChecked()) {
					base.tags["0010|0010"] = _Std("Unknown");
				}
				if (m_pCheckPatientId->IsChecked()) {
					base.tags["0010|0020"] = _Std("Unknown");
				}
				if (m_pCheckInstitutions->IsChecked()) {
					base.tags["0008|0080"] = _Std("Unknown");
				}
				if (m_pCheckReferringPhysician->IsChecked()) {
					base.tags["0008|0090"] = _Std("Unknown");
				}
				if (m_pCheckComments->IsChecked()) {
					base.tags["0008|1030"] = _Std("Unknown"); //study
					base.tags["0008|103e"] = _Std("Unknown"); //series
					base.tags["0020|4000"] = _Std("Unknown"); //image
				}

				std::string pathDicomDir(TOPATH(wxPath));
				GADAPI::ExportToDicomDirCommandParams* pParams = new GADAPI::ExportToDicomDirCommandParams(m_pks, pathDicomDir, base, !m_pCheckPrivateTags->IsChecked());
				GADAPI::ExportToDicomDirCommand* pCmd = new GADAPI::ExportToDicomDirCommand(pParams);
				GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Exporting images ..."), pCmd, NULL);

				GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Exportacion", "PathDefecto",std::string(wxPath.ToUTF8()));
				
				EndModal(wxID_OK);
			} 
		}
		//endregion
	}
}
