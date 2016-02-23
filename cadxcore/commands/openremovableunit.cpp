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

#include <wx/window.h>
#include <wx/volume.h>
#include <wx/msgdlg.h>
#include <api/globals.h>
#include <main/controllers/commandcontroller.h>
#include <api/internationalization/internationalization.h>
#include "openremovableunit.h"
#include <wx/file.h>
#include <wx/dir.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/pacscontroller.h>
#include <main/gui/open/opendialogs.h>
#include <main/gui/open/abrir.h>
#include <commands/comandoincluirhistorial.h>
#include <main/entorno.h>

#include <main/controllers/anonusagestats.h>

namespace GADAPI
{
	OpenRemovableUnitCommand::OpenRemovableUnitCommand(OpenRemovableUnitCommandParams* pParams) : IComando(pParams)
	{
		m_pOpenParams = pParams;
	}

	void OpenRemovableUnitCommand::Execute()
	{
		LOG_STAT(ANON_STAT_OPEN_CD)
		#ifdef _WIN32
		NotificarProgreso(0.15f, _Std("Exploring drives..."));
		wxArrayString volumes = wxFSVolume::GetVolumes(wxFS_VOL_MOUNTED|wxFS_VOL_REMOVABLE );
		NotificarProgreso(0.75f, _Std("Exploring drives..."));
		wxArrayString mountedVolumes;
		for (wxArrayString::iterator it = volumes.begin(); it != volumes.end(); ++it) {
			wxFSVolume fs((*it));
			if (fs.IsOk()) {
				OpenRemovableUnitCommandParams::TRemovableUnit unit;
				unit.path = TOPATH(fs.GetName());
				//first of all list files in path if it isn't mounted ... error
				wxDir dir;
				wxString tmp;
				if (dir.Open(fs.GetName()) && dir.GetFirst(&tmp)) {
					//second check if it includes dicomdir
					wxString dicomDirPath = fs.GetName() + wxT("DICOMDIR");
					unit.hasDicomDir = wxFileExists(dicomDirPath) && GIL::DICOM::PACSController::Instance()->EsDICOM(TOPATH(dicomDirPath),true,false);

					unit.displayName = fs.GetDisplayName().ToUTF8();
					m_pOpenParams->units.push_back(unit);
				}			
			}
		}		
		#endif
	}

	void OpenRemovableUnitCommand::Update()
	{
		if (EstaAbortado()) {
		    return;
		}
		if (m_pOpenParams->units.empty()) {
			//no units
			if (wxMessageBox(_("No DICOMDIR found in removable units.\nWould you like to scan other path recursively?"), _("Info"), wxOK|wxCANCEL, m_pOpenParams->m_pParent) == wxOK) {
				GNC::GUI::Abrir::AbrirDirectorio(m_pOpenParams->m_pParent);
			}
		} else {
			if (!OpenDicomDir()) {
				ScanRecursively();
			}
		}
	}
	bool OpenRemovableUnitCommand::OpenDicomDir()
	{
		int numberOfUnits = 0;
		std::string path;
		for (OpenRemovableUnitCommandParams::TListOfUnits::iterator it = m_pOpenParams->units.begin(); it != m_pOpenParams->units.end(); ++it) {
			if ((*it).hasDicomDir) {
				numberOfUnits++;
				path = (*it).path;
			}
		}
		if (numberOfUnits == 0) {
			return false;
		} else if (numberOfUnits == 1) {
			//dialogo de adquisicion a traves de dicomdir...
			LaunchOpenDicomDir(path);
		} else {
			//show dialog listing units..
			SelectDrive(true);
		}
		return true;
	}

	bool OpenRemovableUnitCommand::ScanRecursively()
	{
		if (m_pOpenParams->units.size() == 1) {
			wxString name = wxString::FromUTF8(m_pOpenParams->units.front().displayName.c_str());
			wxString message = wxString::Format(_("No DICOMDIR found in removable units, would you like to scan recursively %s drive"), name.c_str());
			if (wxMessageBox(message, _("Info"), wxOK|wxCANCEL, m_pOpenParams->m_pParent) == wxOK) 
			{
				LaunchOpenRecursively(m_pOpenParams->units.front().path);
			}
		} else {
			SelectDrive(false);
		}
		return true;
	}

	void OpenRemovableUnitCommand::SelectDrive(bool hasDicomDir)
	{
		GNC::GUI::SelectDrive::TListOfDrives listOfDrives;
		for (OpenRemovableUnitCommandParams::TListOfUnits::iterator it = m_pOpenParams->units.begin(); it != m_pOpenParams->units.end(); ++it) {
			if ((*it).hasDicomDir == hasDicomDir) {
				listOfDrives.push_back(GNC::GUI::SelectDrive::TDrive((*it).path, (*it).displayName));
			}
		}
		GNC::GUI::SelectDrive dialog(m_pOpenParams->m_pParent, listOfDrives);
		if (hasDicomDir) {
			dialog.SetMessage(_("Select drive you want to open"));
		} else {
			dialog.SetMessage(_("Ginkgo CADx has detected more than one removable units,\nbut none of them includes a DICOMDIR at root.\nSelect drive where you want to acquire recursively"));
		}
		if (dialog.ShowModal() == wxID_OK) {
			std::string path = dialog.GetSelectedDrive();
			if (hasDicomDir) {
				LaunchOpenDicomDir(path);
			} else {
				LaunchOpenRecursively(path);
			}
		}
	}

	void OpenRemovableUnitCommand::LaunchOpenRecursively(const std::string& path)
	{
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(path,true);
		GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
		GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Storing in the history..."),pCmd, NULL);
	}

	void OpenRemovableUnitCommand::LaunchOpenDicomDir(const std::string& path)
	{
		std::ostringstream ostr;
		ostr << path << "DICOMDIR";
		GNC::GUI::DialogoDicomDir dicomDirDialog(m_pOpenParams->m_pParent, ostr.str());
		dicomDirDialog.ShowModal();
	}
}
