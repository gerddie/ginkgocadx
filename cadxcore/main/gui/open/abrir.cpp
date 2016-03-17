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
#include <sstream>

#include <wx/window.h>
#include <wx/filename.h>
#include <wx/imaglist.h>
#include <wx/file.h>
#include <wx/dnd.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/menu.h>
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>

#include "abrir.h"
#include "opendialogs.h"


#include <main/controllers/configurationcontroller.h>
#include <main/entorno.h>
#include <main/controllers/controladorextensiones.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/pacscontroller.h>
#include <commands/comandoincluirhistorial.h>
#include <api/dicom/imodelodicom.h>
#include <resources/ginkgoresourcesmanager.h>

#include <main/controllers/anonusagestats.h>

namespace GNC
{
namespace GUI
{

Abrir::Abrir()
{
}

Abrir::~Abrir()
{

}

void Abrir::AbrirFichero(wxWindow* pParent, const std::list<std::string>* directLoads)
{
        LOG_STAT(ANON_STAT_OPEN_FILE)
        bool success = false;
        std::list<std::string> listaPaths;
        wxString guiDir;

        if (directLoads != NULL) {

                listaPaths = (*directLoads);

                success = !listaPaths.empty();

        } else {
                std::string pathDefectoStd;
                GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Adquisicion", "PathDefecto", pathDefectoStd);
                wxString defPathWx = wxString::FromUTF8(pathDefectoStd.c_str());
                if(!wxDirExists(defPathWx)) {
                        defPathWx = wxEmptyString;
                }
                //se muestra un dialogo de abrir fichero, si abre un fichero mira a ver si es dicomdir, si abre un directorio lo explora en profundidad
#if defined(_WINDOWS)
                wxFileDialog fileDialog(pParent,_("Open..."),defPathWx,wxEmptyString,wxT("*.*"), wxFD_OPEN | wxFD_MULTIPLE);
#else
                wxFileDialog fileDialog(pParent,_("Open..."),defPathWx,wxEmptyString,wxT("*"), wxFD_OPEN | wxFD_MULTIPLE);
#endif
                fileDialog.SetDirectory(defPathWx);
                success = (fileDialog.ShowModal() == wxID_OK);

                if (success) {

                        guiDir = fileDialog.GetDirectory();
                        wxArrayString ficherosWx;
                        fileDialog.GetFilenames(ficherosWx);

                        for(wxArrayString::iterator it = ficherosWx.begin(); it != ficherosWx.end(); ++it) {
                                wxString wxPath = fileDialog.GetDirectory() + wxFileName::GetPathSeparator() + (*it);
                                listaPaths.push_back(std::string(TOPATH(wxPath)));
                        }
                }
        }

        if (success) {

                //si alguno de los ficheros es un dicomdir se pasa a tratarlos de una forma especial, si son ficheros normales, se lanza el comando
                bool esDicomDir = false;
                if (listaPaths.size() == 1 && GIL::DICOM::PACSController::Instance()->EsDICOM(listaPaths.front(),true,false)) {
                        esDicomDir = true;
                }

                if (!esDicomDir) {
                        CopyOrLink colDialog(pParent);
                        if (colDialog.ShowModal() == wxID_OK) {
                                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = NULL;
                                if (colDialog.HasSelectedCopy()) {
                                        pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(listaPaths, false, GNC::GCS::IHistoryController::TAA_COPY);
                                } else {
                                        pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(listaPaths, false, GNC::GCS::IHistoryController::TAA_LINK);
                                }
                                GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
                                GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Storing in the history..."),pCmd, NULL);
                        }
                } else {
                        //dialogo de adquisicion a traves de dicomdir...
                        DialogoDicomDir dicomDirDialog(pParent,listaPaths.front());
                        dicomDirDialog.ShowModal();
                }

                if (!guiDir.IsEmpty()) {
                        //se actualiza el fichero de configuracion
                        GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Adquisicion", "PathDefecto", std::string(guiDir.ToUTF8()));
                        GNC::GCS::ConfigurationController::Instance()->Flush();
                }
        }
}

//abrir directorio
void Abrir::AbrirDirectorio(wxWindow* pParent)
{
        LOG_STAT(ANON_STAT_OPEN_FOLDER)
        std::string defPath;
        GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Adquisicion", "PathDefecto", defPath);
        wxString pathDefecto = wxString::FromUTF8(defPath.c_str());
        if(!wxDirExists(pathDefecto)) {
                pathDefecto = wxEmptyString;
        }
        wxDirDialog dd( pParent, _("Choose a directory"), pathDefecto, 0, wxDefaultPosition );
        int response = dd.ShowModal();
        if (response == wxID_OK) {
                std::string stdPath(TOPATH(dd.GetPath()));
                //se lanza el comando de incluir en el historial
                CopyOrLink colDialog(pParent);
                if (colDialog.ShowModal() == wxID_OK) {
                        GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = NULL;
                        if (colDialog.HasSelectedCopy()) {
                                pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(stdPath,true, GNC::GCS::IHistoryController::TAA_COPY);
                        } else {
                                pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(stdPath,true, GNC::GCS::IHistoryController::TAA_LINK);
                        }
                        GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
                        GNC::Entorno::Instance()->GetCommandController()->ProcessAsync(_Std("Storing in the history..."),pCmd, NULL);
                }
                //se actualiza el fichero de configuracion
                GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Adquisicion", "PathDefecto", std::string(dd.GetPath().ToUTF8()));
                GNC::GCS::ConfigurationController::Instance()->Flush();
        }
}
}

}
