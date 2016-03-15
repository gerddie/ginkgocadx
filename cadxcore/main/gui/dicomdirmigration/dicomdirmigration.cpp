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


#include "dicomdirmigration.h"
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/dirdlg.h>
#include <wx/msgdlg.h>
#include <wx/icon.h>
#include <wx/stdpaths.h>
#include <wx/ginkgostyle/ginkgostyle.h>

#include <resources/ginkgoresourcesmanager.h>
#include <api/globals.h>


GNC::GUI::DicomDirMigrationDialog::DicomDirMigrationDialog(wxWindow* pParent, const std::string& dicomDirPath, int version, bool oldVersion): GNC::GUI::DicomDirMigrationDialogBase(pParent),
	m_DicomDirPath(dicomDirPath),
	m_OriginalDicomDirPath(dicomDirPath),
	FromVersion(version),
	Status(GNC::GUI::DicomDirMigrationDialog::TS_CloseAll)
{
	if (oldVersion) {
		m_pLabelVersion->SetLabel(wxString::Format(_("Ginkgo CADx has detected a default DICOM Dir created with an old version of\nGinkgo CADx (BBDD v.%d). What do you want to do?"), (int) FromVersion));
	}
	else {
		m_pLabelVersion->SetLabel(wxString::Format(_("DICOM Dir has been created with a new version of Ginkgo CADx (BBDD v.%d).\nThis version is unabled to open it."), (int) FromVersion));
	}
	m_panelMigration->Show(oldVersion);
	m_panelMigrationSep->Show(oldVersion);
	
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::Logos::GetLogoGinkgo32x32());
	this->SetIcon(icono);
	m_pMainPanel->Layout();
}

GNC::GUI::DicomDirMigrationDialog::~DicomDirMigrationDialog()
{
}

GNC::GUI::DicomDirMigrationDialog::TStatus GNC::GUI::DicomDirMigrationDialog::GetStatus()
{
	return Status;
}


void GNC::GUI::DicomDirMigrationDialog::OnSelectOther(wxCommandEvent& )
{
	wxString path = SelectFolder();
	if (!path.empty()) {
		m_DicomDirPath = TOPATH(path);
		Status=TS_Change;
		Close();
	}	
}

std::string GNC::GUI::DicomDirMigrationDialog::GetDicomDirPath()
{
	return m_DicomDirPath;
}

std::string GNC::GUI::DicomDirMigrationDialog::GetOriginalDicomDirPath()
{
	return m_OriginalDicomDirPath;
}

bool GNC::GUI::DicomDirMigrationDialog::GetRemoveAfterMigration()
{
	return m_pRemoveFiles->IsChecked();
}

void GNC::GUI::DicomDirMigrationDialog::OnMigrate(wxCommandEvent& )
{
	wxString originalWx =  wxString(FROMPATH(m_OriginalDicomDirPath));
	wxString newDicomDirPath = originalWx + wxString(wxT("_old"));
	int index = 0;
	while (wxDirExists(newDicomDirPath)) {
		newDicomDirPath = originalWx + wxString::Format(wxT("_old%d"), (int) index);
		index++;
	}
	if (!wxRenameFile(originalWx, newDicomDirPath)) {
		wxMessageBox(_("Error renaming old dicomdir to <name>_old, ensure you have permissions to do it"),_("Info"),wxOK,this);
		return;
	}

	m_DicomDirPath = m_OriginalDicomDirPath;
	m_OriginalDicomDirPath = TOPATH(newDicomDirPath);

	Status=TS_Migrate;
	Close();
}

wxString GNC::GUI::DicomDirMigrationDialog::SelectFolder()
{
	wxDirDialog seleccionarDirectorio(this,_("Select the DICOM Dir folder"),FROMPATH(m_DicomDirPath),wxDD_DEFAULT_STYLE|wxDD_NEW_DIR_BUTTON);
	int response = seleccionarDirectorio.ShowModal();
	if (response == wxID_OK)
	{
		wxString path = seleccionarDirectorio.GetPath();
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
				return wxEmptyString;
			} else {
                bool success;
	#ifdef _WIN32
                success = wxMkdir(path,511);
	#else
                success = wxMkDir(path.ToUTF8(), 0770);
	#endif
                if(!success || !wxDirExists(path))
				{
					wxMessageBox(_("Failed to create directory, make sure the path is correct and you have permissions on parent directory"), _("Info"));
					return wxEmptyString;
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
						return wxEmptyString;
					}
				}
				ficheroTmp.Write(wxT("test"));
				ficheroTmp.Close();
			}
			wxRemoveFile(pathFicheroPrueba);
		}
		return path;
	}
	return wxEmptyString;
}
