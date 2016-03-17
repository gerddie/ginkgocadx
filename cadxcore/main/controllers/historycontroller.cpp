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

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>
#include <wx/statbmp.h>
#include <wx/bmpbuttn.h>

#include "historycontroller.h"
#include <api/iproxynotificadorprogreso.h>
#include <api/dicom/idicommanager.h>
#include <api/dicom/dcmdictionary.h>
#include <api/internationalization/internationalization.h>

#include "pacscontroller.h"
#include "controladoreventos.h"
#include "controladorpermisos.h"
#include <main/controllers/configurationcontroller.h>
#include <main/gui/dicomdirmigration/dicomdirmigration.h>
#include <eventos/hidesplashwindow.h>
#define LOGGER "HistoryController"
#include "controladorlog.h"
#include <sqlite/ginkgosqlite.h>
#include <sqlite/sqlite3.h>
#include <commands/comandoincluirhistorial.h>

#include <main/controllers/commandcontroller.h>
#include <commands/dbmigrationcommand.h>

#include <main/entorno.h>
#include <sstream>
#include <cmath>
//helpers//////////////////////////////////////////////////////////////////////////////
////helpers orientacion
char getMajorAxisFromPatientRelativeDirectionCosine(double x, double y, double z)
{
        // 0.5477 would be the square root of 1 (unit vector sum of squares) divided by 3 (oblique axes - a "double" oblique)
        // 0.7071 would be the square root of 1 (unit vector sum of squares) divided by 2 (oblique axes)
        /***/
        const double obliquityThresholdCosineValue = 0.8;


        char axis = '?';

        char orientationX = x < 0 ? 'R' : 'L';
        char orientationY = y < 0 ? 'A' : 'P';
        char orientationZ = z < 0 ? 'F' : 'H';

        double absX = std::abs(x);
        double absY = std::abs(y);
        double absZ = std::abs(z);

        // The tests here really don't need to check the other dimensions,
        // just the threshold, since the sum of the squares should be == 1.0
        // but just in case ...

        if (absX>obliquityThresholdCosineValue && absX>absY && absX>absZ) {
                axis=orientationX;
        } else if (absY>obliquityThresholdCosineValue && absY>absX && absY>absZ) {
                axis=orientationY;
        } else if (absZ>obliquityThresholdCosineValue && absZ>absX && absZ>absY) {
                axis=orientationZ;
        }
        return axis;
}

char makeImageOrientationLabelFromImageOrientationPatient(
        const double rowX,const double rowY,const double rowZ,
        const double colX,const double colY,const double colZ)
{
        char label = 'O';
        char rowAxis = getMajorAxisFromPatientRelativeDirectionCosine(rowX,rowY,rowZ);
        char colAxis = getMajorAxisFromPatientRelativeDirectionCosine(colX,colY,colZ);
        if (rowAxis != '?' && colAxis != '?') {
                if      ((rowAxis == 'R' || rowAxis == 'L') && (colAxis == 'A' || colAxis == 'P')) label='A';
                else if ((colAxis == 'R' || colAxis == 'L') && (rowAxis == 'A' || rowAxis == 'P')) label='A';

                else if ((rowAxis == 'R' || rowAxis == 'L') && (colAxis == 'H' || colAxis == 'F')) label='C';
                else if ((colAxis == 'R' || colAxis == 'L') && (rowAxis == 'H' || rowAxis == 'F')) label='C';

                else if ((rowAxis == 'A' || rowAxis == 'P') && (colAxis == 'H' || colAxis == 'F')) label='S';
                else if ((colAxis == 'A' || colAxis == 'P') && (rowAxis == 'H' || rowAxis == 'F')) label='S';
        }
        return label;
}

std::string parseDateTime(const std::string& tagDate, const std::string& tagTime)
{
        std::ostringstream ostr;

        wxDateTime fecha;
        fecha.ParseFormat(wxString::FromUTF8(tagDate.c_str()),wxT("%Y%m%d"));
        if (fecha.IsValid()) {
                ostr << fecha.Format(wxT("%Y-%m-%d")).ToUTF8();
        } else {
                return "";
        }
        wxDateTime hora;
        hora.ParseFormat(wxString::FromUTF8(tagTime.c_str()),wxT("%H%M%S"));
        if(hora.IsValid()) {
                ostr << "T";
                ostr << hora.Format(wxT("%H:%M:%S")).ToUTF8();
                const int posPunto = tagTime.find(".");
                if (posPunto >= 0) {
                        ostr << tagTime.substr(posPunto, tagTime.size()-1);
                }
        }
        return ostr.str();
}

inline std::string GetDateTimeFromWx(const wxString& dateStr)
{
        std::string result;
        wxDateTime date,time;

        const wxChar* pchDate = date.ParseFormat(dateStr, wxT("%Y-%m-%d"));

        if ( pchDate ) {
                // Skip spaces, as the ParseTime() function fails on spaces
                while ( wxIsspace(*pchDate) || (*pchDate)=='T' )
                        pchDate++;

                time.ParseFormat(pchDate, wxT("%H:%M:%S"));
                if (time.IsValid()) {
                        size_t posPoint = dateStr.Find('.', true);
                        if (posPoint != wxString::npos) {
                                long milli=0;
                                wxString tmp = dateStr.substr(posPoint+1);
                                if (tmp.ToLong(&milli)) {
                                        time.SetMillisecond(milli);
                                }
                        }
                        date.Set(date.GetDay(), date.GetMonth(), date.GetYear(), time.GetHour(), time.GetMinute(), time.GetSecond(), time.GetMillisecond());
                }
        }
        if (date.IsValid()) {
                result = date.Format(wxT("%Y-%m-%dT%H:%M:%S.%l")).ToUTF8();
        }
        return result;
}

inline std::list<std::string> StringToList(const wxString& field)
{
        wxStringTokenizer tknz(field, wxT("/"));
        std::list<std::string> result;
        while(tknz.HasMoreTokens()) {
                std::string element(tknz.GetNextToken().ToUTF8());
                result.push_back(element);
        }
        return result;
}

inline std::string ListToString(const std::list<std::string>& listOfString)
{
        std::ostringstream ostr;
        bool primero = true;
        for (std::list<std::string>::const_iterator it = listOfString.begin(); it != listOfString.end(); ++it) {
                if (!primero) {
                        ostr << "/";
                }
                ostr << (*it);
                primero = false;
        }
        return ostr.str();
}
////region "helpers"
/////////////////////////////////////////////////////////
//region "filters"

//region "filters"
/////////////////////////////////////////////////////////
std::string GNC::GCS::History::StudyModalityFilter::GetSQLFilter()
{
        if (listOfModalities.empty()) {
                return "";
        }
        std::ostringstream ostr;
        ostr <<"(";
        for (std::list<std::string>::iterator it = listOfModalities.begin(); it != listOfModalities.end(); ++it) {
                if (it != listOfModalities.begin()) {
                        ostr << " OR ";
                }
                ostr << "mods_in_study LIKE '%" << (*it) << "%'";
        }
        ostr <<")";
        return ostr.str();
}

std::string GNC::GCS::History::AcquisitionDateFilter::GetSQLFilter()
{
        wxSQLite3StatementBuffer bufSQL;
        std::ostringstream ostr;
        if (from.IsValid() && to.IsValid()) {
                std::string tmpFrom(from.Format(wxT("%Y-%m-%d %H:%M:%S")).ToUTF8());
                std::string tmpTo(to.Format(wxT("%Y-%m-%dT%H:%M:%S")).ToUTF8());
                ostr << bufSQL.Format(" Study.study_datetime BETWEEN '%q' AND '%q'", tmpFrom.c_str(), tmpTo.c_str());
        } else if (from.IsValid()) {
                std::string tmpStr(from.Format(wxT("%Y-%m-%d %H:%M:%S")).ToUTF8());
                ostr << bufSQL.Format(" Study.study_datetime >= '%q'",  tmpStr.c_str());
        } else	if (to.IsValid()) {
                std::string tmpStr(to.Format(wxT("%Y-%m-%d %H:%M:%S")).ToUTF8());
                ostr << bufSQL.Format(" Study.study_datetime <= '%q'", tmpStr.c_str());
        }
        return ostr.str();
}

std::string GNC::GCS::History::StudyPatientFilter::GetSQLFilter()
{
        wxSQLite3StatementBuffer bufSQL;
        std::ostringstream ostr;
        ostr << bufSQL.Format("Study.pat_id = '%q'", pat_id.c_str());
        return ostr.str();
}


std::string GNC::GCS::History::StudyLocationFilter::GetSQLFilter()
{
        wxSQLite3StatementBuffer bufSQL;
        std::ostringstream ostr;
        ostr << bufSQL.Format("Study.location = '%c'", GNC::GCS::HistoryController::LocationToChar(Location));
        return ostr.str();
}



//region "Patron singleton"
GNC::GCS::HistoryController* GNC::GCS::HistoryController::m_pInstance = 0;

GNC::GCS::HistoryController::HistoryController()
{
        m_readOnly = false;
        m_pConexion = NULL;
        m_pConexionThumbnails = NULL;
        m_pCriticalSection = new wxCriticalSection();
        m_pCriticalSectionThumbnails = new wxCriticalSection();
        ReloadHistory();
}

GNC::GCS::HistoryController::HistoryController(const std::string& pathOfDataBase)
{
        m_readOnly = false;
        m_GinkgoDicomDir = pathOfDataBase;
        m_pConexion = NULL;
        m_pConexionThumbnails = NULL;
        m_pCriticalSection = new wxCriticalSection();
        m_pCriticalSectionThumbnails = new wxCriticalSection();
        ReloadHistory(pathOfDataBase, false);
}

void GNC::GCS::HistoryController::ReloadHistory(const std::string& pathDicomDir, bool tryTemp)
{
        if (pathDicomDir.empty()) {
                m_GinkgoDicomDir = GetDefaultDicomDirPath();
        } else {
                m_GinkgoDicomDir = pathDicomDir;
        }

        {
                wxFileName pathOfDir(FROMPATH(m_GinkgoDicomDir));
                if (pathOfDir.IsRelative()) {
                        wxFileName pathOfExec(wxStandardPaths::Get().GetExecutablePath());
                        pathOfDir.MakeAbsolute(pathOfExec.GetPath());
                        m_GinkgoDicomDir = TOPATH(pathOfDir.GetFullPath());
                }
        }

        bool error = false;
        if(m_pConexion!=NULL) {
                m_pConexion->Close();
                delete m_pConexion;
                m_pConexion = NULL;
        }
        if(m_pConexionThumbnails!=NULL) {
                m_pConexionThumbnails->Close();
                delete m_pConexionThumbnails;
                m_pConexionThumbnails = NULL;
        }

        try {
                m_pConexion = new wxSQLite3Database();
                m_pConexionThumbnails = new wxSQLite3Database();
                //pillamos la bbdd
                std::string pathFicheroDB;
                std::string pathFicheroDBThumbnails;
                {
                        std::ostringstream ostr;
                        ostr << GetGinkgoDicomDir()  << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgodatabase_2.1.db";
                        pathFicheroDB = ostr.str();
                }
                {
                        std::ostringstream ostr;
                        ostr << GetGinkgoDicomDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgothumbnails.db";
                        pathFicheroDBThumbnails = ostr.str();
                }

                wxString wxDirectorio = FROMPATH(GetGinkgoDicomDir());
                if(!wxDirExists(wxDirectorio)) {
                        bool success;
#ifdef _WIN32
                        success = wxMkdir(wxDirectorio,511);
#else
                        success = (wxMkDir(wxDirectorio.ToUTF8(), 0770) == 0);
#endif
                        if (!success) {
                                LOG_ERROR(LOGGER, "unable to create directory '" << wxDirectorio.c_str() << "'");
                        }
                }

                //existe el historial??
                if(!wxFileExists(FROMPATH(pathFicheroDB))) {
                        //se crea el fichero de base de datos
                        if(CreateGinkgoDB(*m_pConexion,pathFicheroDB.c_str()) == 0) {
                                LOG_ERROR(LOGGER,_Std("Error creating history database ") << pathFicheroDB << _Std(" trying temp"));
                                error = true;
                                if (tryTemp) {
                                        error = false;
                                        LOG_ERROR(LOGGER, _Std("You do not have permissions to write to the dicom Dir, preferences will be stored in the temporary directory"));
                                        {
                                                std::ostringstream ostr2;
                                                ostr2 << GNC::Entorno::Instance()->GetGinkgoTempDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgodatabase_2.1.db"<<std::ends;
                                                pathFicheroDB = ostr2.str();
                                        }
                                        if (CreateGinkgoDB(*m_pConexion, pathFicheroDB.c_str()) == 0) {
                                                LOG_FATAL(LOGGER, "Fatal error creating history in temp dir " << pathFicheroDB);
                                                error = true;
                                        }
                                }
                        }
                }

                if(!m_pConexion->IsOpen()) {
                        if( !AbrirConexion(*m_pConexion,pathFicheroDB.c_str()) ) {
                                LOG_ERROR(LOGGER, _Std("Error creating dicomDir database ") << pathFicheroDB);
                                error = true;
                        } else {
                                wxFileName fileName(FROMPATH(pathFicheroDB));
                                m_readOnly = !fileName.IsFileWritable();

                                //check version
                                int version = 0;
                                try {
                                        wxSQLite3StatementBuffer bufSQL;
                                        bufSQL.Format("SELECT Value FROM MetadataBBDD WHERE Property ='Version'");
                                        wxSQLite3ResultSet result = m_pConexion->ExecuteQuery(bufSQL);
                                        if (result.NextRow()) {
                                                version = result.GetInt(0, -1);
                                        }
                                } catch (wxSQLite3Exception& ) {
                                }
                                if (version != GINKGO_BBDD_VERSION) {
                                        if (!wxThread::IsMain()) {
                                                error = true;
                                                LOG_ERROR("HistoryController", "Ginkgo BBDD version isn't valid and isn't GUI thread");
                                        } else {
                                                //close connection...
                                                if(m_pConexion!=NULL) {
                                                        m_pConexion->Close();
                                                        delete m_pConexion;
                                                        m_pConexion = NULL;
                                                }
                                                GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventHideSplashWindow());
                                                std::string oldDicomDirPath = GetGinkgoDicomDir();
                                                GNC::GUI::DicomDirMigrationDialog dlg(GNC::Entorno::Instance()->GetVentanaRaiz(), oldDicomDirPath, version, version < GINKGO_BBDD_VERSION);
                                                dlg.ShowModal();
                                                if (dlg.GetStatus() == GNC::GUI::DicomDirMigrationDialog::TS_CloseAll) {
                                                        error = true;
                                                } else {
                                                        GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/Estacion", "DicomDir",dlg.GetDicomDirPath());
                                                        ReloadHistory(dlg.GetDicomDirPath(), tryTemp);
                                                        if (dlg.GetStatus() == GNC::GUI::DicomDirMigrationDialog::TS_Migrate) {
                                                                ::GADAPI::DBMigrationCommandParams* pParams = new ::GADAPI::DBMigrationCommandParams(dlg.GetOriginalDicomDirPath(), dlg.GetDicomDirPath(), dlg.GetRemoveAfterMigration());
                                                                ::GADAPI::DBMigrationCommand* pCmd = new ::GADAPI::DBMigrationCommand(pParams);
                                                                GNC::GCS::ICommandController::Instance()->ProcessAsync("Migrating...", pCmd, NULL);
                                                        }
                                                        return;
                                                }
                                        }
                                }
                        }
                }

                /*		if (reloadHistory)
                		{

                			wxMessageBox(_("Version of Ginkgo CADx dicom dir is old, database will be migrated to new version, do not close Ginkgo CADx until migration is finished"), _("Warning"), wxOK|wxICON_WARNING);
                			m_pConexion->Close();
                			if (wxCopyFile(FROMPATH(pathFicheroDB), FROMPATH(pathFicheroDB)+wxT(".bkp"),true)) {
                				if (!wxRemoveFile(FROMPATH(pathFicheroDBThumbnails))) {
                					LOG_ERROR("BBDD", "Error deleting thumbnails")
                				}
                				if (wxRemoveFile(FROMPATH(pathFicheroDB))) {
                					if(CreateGinkgoDB(*m_pConexion,pathFicheroDB.c_str()) != 0) {
                						std::string historyPath = GetGinkgoDicomDir();
                						GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams* pParams = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorialParams(historyPath, true);
                						GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial* pCmd = new GADAPI::ComandoIncluirHistorial::ComandoIncluirHistorial(pParams);
                						GNC::GCS::ICommandController::Instance()->ProcessAsync("Migrating History", pCmd, NULL);
                					} else {
                						LOG_ERROR("BBDD", "Error creating ginkgo BBDD")
                					}
                				} else {
                					LOG_FATAL("BBDD", "Error copying database file "<<pathFicheroDB)
                				}
                			} else {
                				LOG_FATAL("BBDD", "Error copying database file "<<pathFicheroDB)
                			}
                		}*/

                //existe el d los thumbnails??
                if(!wxFileExists(FROMPATH(pathFicheroDBThumbnails))) {
                        //se crea el fichero de base de datos
                        if(CreateGinkgoDBThumbnails(*m_pConexionThumbnails,pathFicheroDBThumbnails.c_str()) == 0) {
                                LOG_ERROR(LOGGER, _Std("Error creating thumbnails database ") << pathFicheroDBThumbnails);
                                {
                                        std::ostringstream ostr2;
                                        ostr2 << GNC::Entorno::Instance()->GetGinkgoTempDir() << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE) << "ginkgothumbnails.db"<<std::ends;
                                        pathFicheroDBThumbnails = ostr2.str();
                                }
                                if(CreateGinkgoDBThumbnails(*m_pConexionThumbnails,pathFicheroDBThumbnails.c_str()) == 0) {
                                        LOG_FATAL(LOGGER, "Fatal error creating thumbnails in temp dir " << pathFicheroDB);
                                        error = true;
                                }
                        }
                }

                if(!m_pConexionThumbnails->IsOpen()) {
                        if( !AbrirConexion(*m_pConexionThumbnails,pathFicheroDBThumbnails.c_str()) ) {
                                LOG_ERROR(LOGGER, _Std("Error creating thumbnails database ") << pathFicheroDB);
                                error = true;
                        }
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "Error al crear el la bbdd " << ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error creating database:\n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
        if (error) {
                if(m_pConexion!=NULL) {
                        m_pConexion->Close();
                        delete m_pConexion;
                        m_pConexion = NULL;
                }
                if(m_pConexionThumbnails!=NULL) {
                        m_pConexionThumbnails->Close();
                        delete m_pConexionThumbnails;
                        m_pConexionThumbnails = NULL;
                }
        }
}

GNC::GCS::HistoryController::~HistoryController()
{
        if(m_pConexion!=NULL) {
                m_pConexion->Close();
                delete m_pConexion;
                m_pConexion = NULL;
        }
        if(m_pConexionThumbnails!=NULL) {
                m_pConexionThumbnails->Close();
                delete m_pConexionThumbnails;
                m_pConexionThumbnails = NULL;
        }
        if(m_pCriticalSection != NULL) {
                delete m_pCriticalSection;
                m_pCriticalSection = NULL;
        }
        if (m_pCriticalSectionThumbnails != NULL) {
                delete m_pCriticalSectionThumbnails;
                m_pCriticalSectionThumbnails = NULL;
        }
}

bool GNC::GCS::HistoryController::IsOk()
{
        return m_pConexionThumbnails != NULL && m_pConexion != NULL;
}

bool GNC::GCS::HistoryController::IsReadOnly()
{
        return m_readOnly;
}

std::string GNC::GCS::HistoryController::GetGinkgoDicomDir(bool barraAlFinal)
{
        if(barraAlFinal) {
                std::ostringstream out;
                out << m_GinkgoDicomDir << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE);
                return out.str();
        } else {
                return m_GinkgoDicomDir;
        }
}

std::string GNC::GCS::HistoryController::GetDefaultDicomDirPath(bool ignoreConfig)
{
        std::string value;
        std::string path;
        if(ignoreConfig || !GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/Estacion", "DicomDir", value)) {
                std::ostringstream out;
                if (GNC::GCS::IControladorPermisos::Instance()->Get("core.restrictions", "anonymous_history")) {
                        out << GNC::Entorno::Instance()->CreateGinkgoTempDir() << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE) << "DICOMDIR";
                } else {
                        out << GNC::Entorno::Instance()->GetGinkgoUserDir() << (char) wxFileName::GetPathSeparator(wxPATH_NATIVE) << "DICOMDIR";
                }
                path = out.str();
        } else {
                path = value;
        }
        return path;
}

GNC::GCS::HistoryController* GNC::GCS::HistoryController::Instance()
{
        if (m_pInstance == NULL) {
                m_pInstance = new GNC::GCS::HistoryController();
        }
        return m_pInstance;
}

void GNC::GCS::HistoryController::FreeInstance()
{
        if (m_pInstance != NULL) {
                delete m_pInstance;
                m_pInstance = NULL;
        }
}

//region "metodos propios del historial"
bool GNC::GCS::HistoryController::AddFiles(const StringList& listaPaths, TAddErrorList& errorList, std::list<long>* insertedSeries, GNC::IProxyNotificadorProgreso* pNotificador, TAddAction action)
{
        if (m_readOnly) {
                return false;
        }
        wxCriticalSectionLocker locker(*m_pCriticalSection);
        DICOMFileModelList listOfInsertedFiles;
        try {
                m_pConexion->Begin();

                std::set<long> seriesPks;
                std::set<long> studiesPks;
                int i = 0;
                for(StringList::const_iterator it=listaPaths.begin(); it!= listaPaths.end(); ++it,++i) {
                        if(pNotificador != NULL) {
                                std::ostringstream ostr;
                                ostr << _Std("Including image ") << (i+1) << _Std(" of ") << listaPaths.size();
                                if(!pNotificador->NotificarProgreso((float)i/listaPaths.size(),ostr.str())) {
                                        m_pConexion->Commit();
                                        return false;
                                }
                        }

                        if(!wxFileExists((FROMPATH((*it))))) {
                                errorList.push_back(TAddError(TAddError::TE_FileNotExist, (*it)));
                        } else {
                                try {
                                        DICOMFileModel fileModel;
                                        if (!ReadFile(fileModel,(*it), errorList, action)) {
                                                continue;
                                        }

                                        FindOrInsertStudy(fileModel);
                                        studiesPks.insert(fileModel.study.pk);
                                        FindOrInsertSeries(fileModel);
                                        seriesPks.insert(fileModel.series.pk);

                                        if (insertedSeries != NULL) {
                                                insertedSeries->push_back(fileModel.series.pk);
                                        }

                                        if (action == TAA_LINK) {
                                                fileModel.file.file_path = GetRelativePath((*it));
                                        }
                                        if (!FindOrInsertFile(fileModel)) {
                                                if (action == TAA_COPY || action == TAA_MOVE) {
                                                        fileModel.file.real_path = GetPathOfFile(fileModel.file.location, fileModel.file.pk, fileModel.file.file_path);
                                                        wxString pathDicom = FROMPATH(fileModel.file.real_path);

                                                        if(wxCopyFile(FROMPATH((*it)),pathDicom)) {
                                                                int success;
#ifdef _WIN32
                                                                success = _chmod(TOPATH(pathDicom).c_str(), _S_IWRITE);
#else
                                                                success = chmod(TOPATH(pathDicom).c_str(), 0644);
#endif
                                                                if (success < 0) {
                                                                        LOG_ERROR("Historial", "Error changing permissions on :"
                                                                                  << TOPATH(pathDicom).c_str() << "':" << strerror(errno));
                                                                }

                                                                if(action == TAA_MOVE) {
                                                                        wxRemoveFile(FROMPATH((*it)));
                                                                }
                                                        } else {
                                                                //no se ha podido copiar
                                                                LOG_ERROR("Historial", "Error copying file: " << (*it) << " TO "  << pathDicom.ToUTF8());
                                                                return false;
                                                        }
                                                }
                                        }
                                        listOfInsertedFiles.push_back(fileModel);

                                } catch (wxSQLite3Exception& ex) {
                                        LOG_ERROR("BBDD", "Exception adding file "<<ex.GetMessage().ToUTF8())
                                        m_pConexion->Rollback();
                                        errorList.push_back(TAddError(TAddError::TE_WrongFormat, std::string(ex.GetMessage().ToUTF8())));
                                        return false;
                                }
                        }
                }//for paths...

                //update series and study summary info (modalities in study, number of instances...)
                for (std::set<long>::iterator it = seriesPks.begin(); it != seriesPks.end(); ++it) {
                        UpdateSeriesSummaryInfo((*it));
                }
                for (std::set<long>::iterator it = studiesPks.begin(); it != studiesPks.end(); ++it) {
                        UpdateStudySummaryInfo((*it));
                }

                m_pConexion->Commit();
                return true;
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "Error al anadir el fichero a la base de datos "<<ex.GetMessage().ToUTF8())
                errorList.push_back(TAddError(TAddError::TE_WrongFormat, std::string(ex.GetMessage().ToUTF8())));
                m_pConexion->Rollback();
                return false;
        }
}

bool GNC::GCS::HistoryController::UpdateFiles(const GNC::GCS::HistoryController::FileModelList& files)
{
        if (m_readOnly) {
                return false;
        }
        wxCriticalSectionLocker locker(*m_pCriticalSection);

        try {
                m_pConexion->Begin();
                std::set<long> seriesPks;
                std::set<long> studiesPks;
                for (GNC::GCS::HistoryController::FileModelList::const_iterator itFile = files.begin(); itFile != files.end(); ++itFile) {
                        TAddErrorList errorList;
                        DICOMFileModel fileModel;
                        if (!ReadFile(fileModel,(*itFile).real_path,errorList)) {
                                continue;
                        }
                        fileModel.file.pk = (*itFile).pk;
                        fileModel.series.pk = fileModel.file.series_fk = (*itFile).series_fk;
                        SeriesModel s = GetSeriesModel((*itFile).series_fk);
                        fileModel.study.pk = fileModel.series.study_fk = s.study_fk;

                        UpdateStudy(fileModel);
                        UpdateSeries(fileModel);
                        UpdateFile(fileModel);

                        seriesPks.insert(fileModel.series.pk);
                        studiesPks.insert(fileModel.study.pk);
                }
                //update series and study summary info (modalities in study, number of instances...)
                for (std::set<long>::iterator it = seriesPks.begin(); it != seriesPks.end(); ++it) {
                        UpdateSeriesSummaryInfo((*it));
                }
                for (std::set<long>::iterator it = studiesPks.begin(); it != studiesPks.end(); ++it) {
                        UpdateStudySummaryInfo((*it));
                }

                m_pConexion->Commit();
                return true;
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "Error al actualizar el fichero de la base de datos "<<ex.GetMessage().ToUTF8())
                m_pConexion->Rollback();
                return false;
        }
}

bool GNC::GCS::HistoryController::LinkFilesToSeries(long seriesPk, GNC::GCS::HistoryController::DICOMFileModelList& files, GNC::IProxyNotificadorProgreso* pNotificador )
{
        if (m_readOnly) {
                return false;
        }
        wxCriticalSectionLocker locker(*m_pCriticalSection);
        DICOMFileModelList listOfInsertedFiles;
        try {
                m_pConexion->Begin();

                SeriesModel sm = GetSeriesModel(seriesPk);

                int i = 0;
                for(GNC::GCS::HistoryController::DICOMFileModelList::iterator it=files.begin(); it!= files.end(); ++it, ++i) {
                        if(pNotificador != NULL) {
                                std::ostringstream ostr;
                                ostr << _Std("Linking image ") << (i+1) << _Std(" of ") << files.size();
                                if(!pNotificador->NotificarProgreso((float)i/files.size(),ostr.str())) {
                                        m_pConexion->Commit();
                                        return false;
                                }
                        }

                        DICOMFileModel& fileModel = (*it);
                        fileModel.file.series_fk = seriesPk;
                        FindOrLink(fileModel);
                }//for paths...

                //update series and study summary info (modalities in study, number of instances...)
                UpdateSeriesSummaryInfo(sm.pk);

                UpdateStudySummaryInfo(sm.study_fk);

                m_pConexion->Commit();
                return true;
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "Error linking file in database controller "<<ex.GetMessage().ToUTF8())
                m_pConexion->Rollback();
                return false;
        }
}

void GNC::GCS::HistoryController::FindOrInsertStudy(DICOMFileModel& model)
{
        std::ostringstream ostrSelect;
        wxSQLite3StatementBuffer bufSQL;
        ostrSelect << bufSQL.Format("SELECT pk from Study where pat_id='%q'",model.study.pat_id.c_str());
        ostrSelect <<  bufSQL.Format(" AND pat_id_issuer='%q'",model.study.pat_id_issuer.c_str());
        ostrSelect <<  bufSQL.Format(" AND pat_name='%q'",model.study.pat_name.c_str());
        ostrSelect <<  bufSQL.Format(" AND study_iuid='%q'",model.study.study_iuid.c_str());
        ostrSelect <<  bufSQL.Format(" AND study_id='%q'",model.study.study_id.c_str());
        ostrSelect <<  bufSQL.Format(" AND accession_no='%q'",model.study.study_acc_no.c_str());
        if (model.study.pat_sex == '?') {
                ostrSelect << bufSQL.Format(" AND ( pat_sex = '?' OR pat_sex IS NULL )", model.study.pat_sex);
        } else {
                ostrSelect << bufSQL.Format(" AND pat_sex = '%c'", model.study.pat_sex);
        }
        if (model.study.pat_bithdate.empty()) {
                ostrSelect << " AND pat_birthdate IS NULL";
        } else {
                ostrSelect << bufSQL.Format(" AND pat_birthdate = '%q'",model.study.pat_bithdate.c_str());
        }

        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrSelect.str().c_str());
        if (resultados.NextRow()) {
                model.study.pk = resultados.GetInt(wxT("pk"));
                model.series.study_fk = model.study.pk;
        } else {
                std::ostringstream ostrInsert;
                ostrInsert << bufSQL.Format("INSERT INTO Study "
                                            "(pat_id,pat_id_issuer,pat_name,mods_in_study,"
                                            "study_iuid, study_id, accession_no, ref_physician, study_desc,"
                                            "cuids_in_study,pat_sex,num_series,"
                                            "num_instances, created_time, updated_time,pat_birthdate,study_datetime) VALUES ("
                                            "'%q','%q','%q','%q',"
                                            "'%q','%q','%q','%q','%q',"
                                            "'%q','%c',1,1,datetime(),datetime(),",
                                            model.study.pat_id.c_str(), model.study.pat_id_issuer.c_str(), model.study.pat_name.c_str(),model.series.series_modality.c_str(),
                                            model.study.study_iuid.c_str(), model.study.study_id.c_str(),model.study.study_acc_no.c_str(), model.study.ref_physician.c_str(), model.study.study_desc.c_str(),
                                            model.file.sopcuid.c_str(),model.study.pat_sex);
                if (model.study.pat_bithdate.empty()) {
                        ostrInsert << "NULL,";
                } else {
                        ostrInsert << "'" << model.study.pat_bithdate << "' ,";
                }
                if (model.study.study_datetime.empty()) {
                        ostrInsert << "NULL)";
                } else {
                        ostrInsert << "'" << model.study.study_datetime << "' )";
                }
                if(m_pConexion->ExecuteUpdate(ostrInsert.str().c_str())>0) {
                        model.study.pk = (long)m_pConexion->GetLastRowId().GetValue();
                        model.series.study_fk = model.study.pk;
                }
        }
}

void GNC::GCS::HistoryController::FindOrInsertSeries(DICOMFileModel& model)
{
        wxSQLite3StatementBuffer bufSQL;
        std::stringstream ostrSelect;
        ostrSelect << bufSQL.Format("Select pk from Series where study_fk='%ld' AND series_iuid='%q' "
                                    "AND series_no='%q' AND modality='%q' ",
                                    model.series.study_fk, model.series.series_iuid.c_str(), model.series.series_no.c_str(), model.series.series_modality.c_str(),
                                    model.series.uid_importer.c_str(), model.series.body_part.c_str(), model.series.laterality.c_str());
        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrSelect.str().c_str());
        if (resultados.NextRow()) {
                model.series.pk = resultados.GetInt(wxT("pk"));
                model.file.series_fk = model.series.pk;
        } else {
                std::stringstream ostrInsert;
                ostrInsert << bufSQL.Format("INSERT INTO Series ("
                                            "study_fk,series_iuid,series_no,modality,"
                                            "uid_importer, body_part, laterality, series_desc,"
                                            "institution,station_name,department,perf_physician,"
                                            "frame_of_reference_uid, tuids_in_series, num_instances, created_time, updated_time,series_datetime) VALUES ("
                                            "'%ld','%q','%q','%q',"
                                            "'%q','%q','%q','%q',"
                                            "'%q','%q','%q','%q',"
                                            "'%q','%q',1,datetime(),datetime(),",
                                            model.series.study_fk, model.series.series_iuid.c_str(), model.series.series_no.c_str(), model.series.series_modality.c_str(),
                                            model.series.uid_importer.c_str(), model.series.body_part.c_str(), model.series.laterality.c_str(), model.series.series_desc.c_str(),
                                            model.series.institution.c_str(), model.series.station_name.c_str(), model.series.department.c_str(), model.series.perf_physician.c_str(),
                                            model.series.frame_of_reference.c_str(), model.file.tsuid.c_str());
                if (model.series.series_datetime.empty()) {
                        ostrInsert << "NULL)";
                } else {
                        ostrInsert << "'" << model.series.series_datetime << "')";
                }
                if(m_pConexion->ExecuteUpdate(ostrInsert.str().c_str())>0) {
                        model.series.pk = (long)m_pConexion->GetLastRowId().GetValue();
                        model.file.series_fk = model.series.pk;
                }
        }
}

//true if file exists, false if it has been overwritten or if it's new
bool GNC::GCS::HistoryController::FindOrInsertFile(DICOMFileModel& model)
{
        wxSQLite3StatementBuffer bufSQL;
        std::stringstream ostr;
        ostr << bufSQL.Format("Select pk, location from File where series_fk='%ld' "
                              "AND file_sopiuid='%q'",
                              model.file.series_fk,
                              model.file.sopiuid.c_str());
        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
        TLocation location;
        long pk = -1;
        if (resultados.NextRow()) {
                //file exists...
                pk = resultados.GetInt(wxT("pk"));
                location = CharToLocation(*resultados.GetAsString(wxT("location")).ToUTF8());
        }

        if (pk == -1) {
                //file doesn't exists so insert it
                std::stringstream ostrInsert;
                ostrInsert << bufSQL.Format("INSERT INTO File ("
                                            "series_fk, file_path, file_sopiuid, file_tsuid,"
                                            "sopcuid, orientation, image_positionx, image_positiony,"
                                            "image_positionz, slice_location, slice_thickness, height, width, number_of_frames,"
                                            "instance_number, file_desc, direction_cosines, spacing, location, created_time, updated_time, content_datetime) VALUES ("
                                            "'%ld','%q','%q','%q',"
                                            "'%q','%c','%f','%f',"
                                            "'%f','%f', '%f', '%d','%d','%d',"
                                            "'%d', '%q', '%q', '%q', '%c',datetime(),datetime(),",
                                            model.file.series_fk, model.file.file_path.c_str(), model.file.sopiuid.c_str(), model.file.tsuid.c_str(),
                                            model.file.sopcuid.c_str(), model.file.orientation, model.file.image_position[0], model.file.image_position[1],
                                            model.file.image_position[2], model.file.slice_location, model.file.slice_thickness, model.file.height, model.file.width, model.file.number_of_frames,
                                            model.file.instance_number, model.file.file_desc.c_str(), model.file.direction_cosines.c_str(), model.file.spacing.c_str(), LocationToChar(model.file.location));
                if (model.file.content_datetime.empty()) {
                        ostrInsert << "NULL)";
                } else {
                        ostrInsert << "'" << model.file.content_datetime << "')";
                }
                if(m_pConexion->ExecuteUpdate(ostrInsert.str().c_str())>0) {
                        model.file.pk = (long)m_pConexion->GetLastRowId().GetValue();
                }
                return false;
        } else if (location != TL_LocalDatabase) {
                //update file...
                model.file.pk = pk;
                UpdateFile(model);
                return false;
        } else {
                //nothing to do file is in local database
                model.file.pk = pk;
                return true;
        }
}

bool GNC::GCS::HistoryController::FindOrLink(DICOMFileModel& model)
{
        wxSQLite3StatementBuffer bufSQL;
        model.file.location = TL_WadoLinked;
        std::stringstream ostr;
        ostr << bufSQL.Format("Select pk, location from File where series_fk='%ld' "
                              "AND file_sopiuid='%q'",
                              model.file.series_fk,
                              model.file.sopiuid.c_str());
        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
        if (resultados.NextRow()) {
                //file exists and is fully local then return true...
                TLocation location = CharToLocation(*resultados.GetAsString(wxT("location")).ToUTF8());
                if (location != TL_WadoLinked) {
                        return true;
                } else {
                        model.file.pk = resultados.GetInt(wxT("pk"));
                        //file is now linked so overwrite it!
                        std::stringstream ostrUpdate;
                        wxSQLite3StatementBuffer bufSQL;
                        ostrUpdate << bufSQL.Format("Update File SET "
                                                    "series_fk = '%ld',file_sopiuid='%q',file_tsuid = '%q',"
                                                    "sopcuid = '%q', orientation = '%c', image_positionx = '%f', image_positiony = '%f',"
                                                    "image_positionz = '%f',slice_location = '%f',height = '%d',width = '%d',number_of_frames='%d',"
                                                    "instance_number = '%d', file_desc='%q', direction_cosines = '%q', spacing='%q', location='%c', updated_time = datetime(), content_datetime = ",
                                                    model.file.series_fk, model.file.sopiuid.c_str(), model.file.tsuid.c_str(),
                                                    model.file.sopcuid.c_str(), model.file.orientation, model.file.image_position[0], model.file.image_position[1],
                                                    model.file.image_position[2], model.file.slice_location, model.file.height, model.file.width,model.file.number_of_frames,
                                                    model.file.instance_number, model.file.file_desc.c_str(), model.file.direction_cosines.c_str(), model.file.spacing.c_str(),
                                                    LocationToChar(model.file.location));
                        if (model.file.content_datetime.empty()) {
                                ostrUpdate << "NULL";
                        } else {
                                ostrUpdate << "'" << model.file.content_datetime << "'";
                        }
                        ostrUpdate << bufSQL.Format(" WHERE File.pk = '%ld'", model.file.pk);
                        m_pConexion->ExecuteUpdate(ostrUpdate.str().c_str());
                        return true;
                }
        }
        std::stringstream ostrInsert;
        ostrInsert << bufSQL.Format("INSERT INTO File ("
                                    "series_fk, file_path, file_sopiuid, file_tsuid,"
                                    "sopcuid, orientation, image_positionx, image_positiony,"
                                    "image_positionz, slice_location, slice_thickness, height, width, number_of_frames,"
                                    "instance_number, file_desc, direction_cosines, spacing, location, created_time, updated_time, content_datetime) VALUES ("
                                    "'%ld','%q','%q','%q',"
                                    "'%q','%c','%f','%f',"
                                    "'%f','%f', '%f', '%d','%d','%d',"
                                    "'%d', '%q', '%q', '%q', '%c', datetime(),datetime(),",
                                    model.file.series_fk, model.file.file_path.c_str(), model.file.sopiuid.c_str(), model.file.tsuid.c_str(),
                                    model.file.sopcuid.c_str(), model.file.orientation, model.file.image_position[0], model.file.image_position[1],
                                    model.file.image_position[2], model.file.slice_location, model.file.slice_thickness, model.file.height, model.file.width, model.file.number_of_frames,
                                    model.file.instance_number, model.file.file_desc.c_str(), model.file.direction_cosines.c_str(), model.file.spacing.c_str(), LocationToChar(model.file.location));
        if (model.file.content_datetime.empty()) {
                ostrInsert << "NULL)";
        } else {
                ostrInsert << "'" << model.file.content_datetime << "')";
        }
        if(m_pConexion->ExecuteUpdate(ostrInsert.str().c_str())>0) {
                model.file.pk = (long)m_pConexion->GetLastRowId().GetValue();
        }
        return false;
}

void GNC::GCS::HistoryController::UpdateStudy(DICOMFileModel& model)
{
        std::stringstream ostrUpdate;
        wxSQLite3StatementBuffer bufSQL;
        ostrUpdate << bufSQL.Format("Update Study  SET "
                                    "pat_id='%q',pat_id_issuer='%q',pat_name='%q',"
                                    "study_iuid='%q', study_id='%q', accession_no='%q', ref_physician='%q', study_desc='%q',"
                                    "pat_sex='%c', location='%c', updated_time=datetime(),pat_birthdate=",
                                    model.study.pat_id.c_str(), model.study.pat_id_issuer.c_str(), model.study.pat_name.c_str(),
                                    model.study.study_iuid.c_str(), model.study.study_id.c_str(),model.study.study_acc_no.c_str(), model.study.ref_physician.c_str(), model.study.study_desc.c_str(),
                                    model.study.pat_sex, LocationToChar(model.study.location));

        if (model.study.pat_bithdate.empty()) {
                ostrUpdate << "NULL";
        } else {
                ostrUpdate << "'" << model.study.pat_bithdate << "'";
        }
        if (model.study.study_datetime.empty()) {
                ostrUpdate << ",study_datetime=NULL";
        } else {
                ostrUpdate << ",study_datetime= '" << model.study.study_datetime << "'";
        }
        ostrUpdate << bufSQL.Format(" WHERE Study.pk = '%ld'", model.study.pk);
        m_pConexion->ExecuteUpdate(ostrUpdate.str().c_str());
}

void GNC::GCS::HistoryController::UpdateSeries(GNC::GCS::HistoryController::DICOMFileModel& model)
{
        std::stringstream ostrUpdate;
        wxSQLite3StatementBuffer bufSQL;
        ostrUpdate << bufSQL.Format("Update Series SET "
                                    "study_fk = '%ld', series_iuid='%q',series_no='%q',modality='%q',"
                                    "uid_importer='%q', body_part='%q', laterality='%q', series_desc='%q',"
                                    "institution='%q',station_name='%q',department='%q',perf_physician='%q',"
                                    "frame_of_reference_uid='%q', location='%c', updated_time = datetime(),series_datetime=",
                                    model.series.study_fk, model.series.series_iuid.c_str(), model.series.series_no.c_str(), model.series.series_modality.c_str(),
                                    model.series.uid_importer.c_str(), model.series.body_part.c_str(), model.series.laterality.c_str(), model.series.series_desc.c_str(),
                                    model.series.institution.c_str(), model.series.station_name.c_str(), model.series.department.c_str(), model.series.perf_physician.c_str(),
                                    model.series.frame_of_reference.c_str(), LocationToChar(model.series.location));
        if (model.series.series_datetime.empty()) {
                ostrUpdate << "NULL";
        } else {
                ostrUpdate << "'" << model.series.series_datetime << "'";
        }
        ostrUpdate << bufSQL.Format(" WHERE Series.pk = '%ld'", model.series.pk);
        m_pConexion->ExecuteUpdate(ostrUpdate.str().c_str());
}

void GNC::GCS::HistoryController::UpdateFile(GNC::GCS::HistoryController::DICOMFileModel& model)
{
        std::stringstream ostrUpdate;
        wxSQLite3StatementBuffer bufSQL;
        ostrUpdate << bufSQL.Format("Update File SET "
                                    "series_fk = '%ld', file_path = '%q',file_sopiuid='%q',file_tsuid = '%q',"
                                    "sopcuid = '%q', orientation = '%c', image_positionx = '%f', image_positiony = '%f',"
                                    "image_positionz = '%f',slice_location = '%f',height = '%d',width = '%d',number_of_frames='%d',"
                                    "instance_number = '%d', file_desc='%q', direction_cosines = '%q', spacing='%q', slice_thickness='%f', location='%c', updated_time = datetime(), content_datetime = ",
                                    model.file.series_fk, model.file.file_path.c_str(), model.file.sopiuid.c_str(), model.file.tsuid.c_str(),
                                    model.file.sopcuid.c_str(), model.file.orientation, model.file.image_position[0], model.file.image_position[1],
                                    model.file.image_position[2], model.file.slice_location, model.file.height, model.file.width,model.file.number_of_frames,
                                    model.file.instance_number, model.file.file_desc.c_str(), model.file.direction_cosines.c_str(), model.file.spacing.c_str(),
                                    model.file.slice_thickness, LocationToChar(model.file.location));
        if (model.file.content_datetime.empty()) {
                ostrUpdate << "NULL";
        } else {
                ostrUpdate << "'" << model.file.content_datetime << "'";
        }
        ostrUpdate << bufSQL.Format(" WHERE File.pk = '%ld'", model.file.pk);
        m_pConexion->ExecuteUpdate(ostrUpdate.str().c_str());
}

std::string GNC::GCS::HistoryController::GetPathOfFile(GNC::GCS::IHistoryController::TLocation location, long pk, const std::string& file_path)
{
        switch (location) {
        case TL_LocalDatabase: {
                wxString pathDicom = FROMPATH(m_GinkgoDicomDir);
                pathDicom += wxFileName::GetPathSeparator();
                pathDicom += wxT("DicomDir");
                wxString rootDir(pathDicom);
                if (!wxDir::Exists(rootDir)) {
                        wxFileName::Mkdir(rootDir, 0700);
                }
                pathDicom += wxFileName::GetPathSeparator();
                pathDicom += wxString::Format(wxT("%d"), (int) pk);
                return std::string(TOPATH(pathDicom));
        }
        break;
        case TL_LocalLinked:
                return GetAbsolutePath(file_path);
        case TL_WadoLinked:
        default:
                return file_path;
        }
}


std::string GNC::GCS::HistoryController::GetPathOfFile(long file_pk)
{
        wxString pathDicom = FROMPATH(m_GinkgoDicomDir);
        pathDicom += wxFileName::GetPathSeparator();
        pathDicom += wxT("DicomDir");
        wxString rootDir(pathDicom);
        if (!wxDir::Exists(rootDir)) {
                wxFileName::Mkdir(rootDir, 0700);
        }
        pathDicom += wxFileName::GetPathSeparator();
        pathDicom += wxString::Format(wxT("%d"), (int) file_pk);
        return std::string(TOPATH(pathDicom));
}

std::string GNC::GCS::HistoryController::GetRelativePath(const std::string& absolutePath)
{
        wxFileName fileName(FROMPATH(absolutePath));
        if (fileName.MakeRelativeTo(FROMPATH(GetGinkgoDicomDir()))) {
                return std::string(TOPATH(fileName.GetFullPath()));
        } else {
                return absolutePath;
        }
}

std::string GNC::GCS::HistoryController::GetAbsolutePath(const std::string& relativePath)
{
        wxFileName fileName(FROMPATH(relativePath));
        if (fileName.IsRelative() && fileName.MakeAbsolute(FROMPATH(GetGinkgoDicomDir(true)))) {
                return std::string(TOPATH(fileName.GetFullPath()));
        } else {
                return relativePath;
        }
}

void GNC::GCS::HistoryController::UpdateSeriesSummaryInfo(long series_pk)
{
        wxSQLite3StatementBuffer bufSQL;
        bufSQL.Format("SELECT File.file_tsuid, count(*) as numberOfFiles "
                      "FROM File "
                      "WHERE series_fk = '%d' "
                      "GROUP BY FILE.file_tsuid", series_pk);
        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
        std::list<std::string> tSyntaxUIDs;
        long number_instances = 0;
        while (resultados.NextRow()) {
                std::string path(resultados.GetAsString(wxT("file_tsuid")).ToUTF8());
                if (!path.empty()) {
                        tSyntaxUIDs.push_back(path);
                }
                number_instances += resultados.GetInt64(wxT("numberOfFiles")).GetValue();
        }
        //order is W,L,B => if there is a wado is wado and if there is a linked is linked
        bufSQL.Format("SELECT distinct(location) "
                      "FROM File "
                      "WHERE series_fk = '%d' "
                      "Order by location DESC", series_pk);
        resultados = m_pConexion->ExecuteQuery(bufSQL);
        TLocation seriesLocation = TL_LocalDatabase;
        if (resultados.NextRow()) {
                seriesLocation = CharToLocation(*resultados.GetAsString(0).ToUTF8());
        }
        //
        bufSQL.Format("Update Series SET num_instances = '%d', tuids_in_series='%q', location='%c' WHERE pk = '%d'",
                      number_instances, ListToString(tSyntaxUIDs).c_str(), LocationToChar(seriesLocation), series_pk);
        m_pConexion->ExecuteUpdate(bufSQL);
}

void GNC::GCS::HistoryController::UpdateStudySummaryInfo(long study_pk)
{
        wxSQLite3StatementBuffer bufSQL;
        bufSQL.Format("SELECT sopcuid, count(*) as numberOfFiles "
                      "FROM File, Series "
                      "WHERE File.series_fk = Series.pk AND Series.study_fk = '%d'"
                      "GROUP BY FILE.sopcuid", study_pk);
        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
        std::list<std::string> sopclasses;
        long number_instances = 0;
        while (resultados.NextRow()) {
                std::string sop(resultados.GetAsString(wxT("sopcuid")).ToUTF8());
                if (!sop.empty()) {
                        sopclasses.push_back(sop);
                }
                number_instances += resultados.GetInt64(wxT("numberOfFiles")).GetValue();
        }
        bufSQL.Format("SELECT Series.modality, count(*) as numberOfSeries "
                      "FROM Series "
                      "WHERE Series.study_fk = '%d' "
                      "GROUP BY Series.modality", study_pk);
        resultados = m_pConexion->ExecuteQuery(bufSQL);
        std::list<std::string> modalities;
        long number_series = 0;
        while (resultados.NextRow()) {
                std::string mod(resultados.GetAsString(wxT("modality")).ToUTF8());
                modalities.push_back(mod);
                number_series += resultados.GetInt64(wxT("numberOfSeries")).GetValue();
        }
        //order is W,L,B => if there is a wado is wado and if there is a linked is linked
        bufSQL.Format("SELECT distinct(location) "
                      "FROM Series "
                      "WHERE study_fk = '%d' "
                      "Order by location DESC", study_pk);
        resultados = m_pConexion->ExecuteQuery(bufSQL);
        TLocation studyLocation = TL_LocalDatabase;
        if (resultados.NextRow()) {
                studyLocation = CharToLocation(*resultados.GetAsString(0).ToUTF8());
        }
        //
        bufSQL.Format("Update Study SET num_instances = '%d', num_series='%d', cuids_in_study='%q', mods_in_study = '%q', location='%c' WHERE pk = '%d'",
                      number_instances, number_series, ListToString(sopclasses).c_str(), ListToString(modalities).c_str(), LocationToChar(studyLocation), study_pk);
        m_pConexion->ExecuteUpdate(bufSQL);
}

inline void FillInSeriesModel(wxSQLite3ResultSet& resultados, GNC::GCS::HistoryController::SeriesModel& seriesModel)
{
        if (seriesModel.pk <= 0) {
                seriesModel.pk = resultados.GetInt64(wxT("pk")).GetValue();
                seriesModel.study_fk = resultados.GetInt64(wxT("study_fk")).GetValue();
                seriesModel.series_iuid = resultados.GetAsString(wxT("series_iuid")).ToUTF8();
                seriesModel.series_datetime = GetDateTimeFromWx(resultados.GetAsString(wxT("series_datetime")));
                seriesModel.series_no = resultados.GetAsString(wxT("series_no")).ToUTF8();
                seriesModel.series_modality = resultados.GetAsString(wxT("modality")).ToUTF8();
                seriesModel.uid_importer = resultados.GetAsString(wxT("uid_importer")).ToUTF8();
                seriesModel.body_part = resultados.GetAsString(wxT("body_part")).ToUTF8();
                seriesModel.laterality = resultados.GetAsString(wxT("laterality")).ToUTF8();
                seriesModel.series_desc = resultados.GetAsString(wxT("series_desc")).ToUTF8();
                seriesModel.institution = resultados.GetAsString(wxT("institution")).ToUTF8();
                seriesModel.station_name = resultados.GetAsString(wxT("station_name")).ToUTF8();
                seriesModel.department = resultados.GetAsString(wxT("department")).ToUTF8();
                seriesModel.perf_physician = resultados.GetAsString(wxT("perf_physician")).ToUTF8();
                seriesModel.num_instances = resultados.GetInt(wxT("num_instances"));
                seriesModel.frame_of_reference = resultados.GetAsString(wxT("frame_of_reference_uid")).ToUTF8();
                seriesModel.tsuids = StringToList(resultados.GetAsString(wxT("tuids_in_series")));
                seriesModel.created_time = GetDateTimeFromWx(resultados.GetAsString(wxT("created_time")));
                seriesModel.updated_time = GetDateTimeFromWx(resultados.GetAsString(wxT("updated_time")));
                seriesModel.location = GNC::GCS::IHistoryController::CharToLocation(*resultados.GetString(wxT("location")).ToUTF8());
        }
}

GNC::GCS::HistoryController::SeriesModel GNC::GCS::HistoryController::GetSeriesModel(long series_pk)
{
        SeriesModel seriesModel;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT Series.pk, Series.study_fk, Series.series_iuid, Series.series_datetime, Series.series_no,Series.modality,"
                              "Series.uid_importer, Series.body_part, Series.laterality, Series.series_desc,"
                              "Series.institution,Series.station_name,Series.department,Series.perf_physician,"
                              "Series.num_instances, Series.frame_of_reference_uid, Series.tuids_in_series, Series.created_time, Series.updated_time, Series.location "
                              "FROM Series "
                              "WHERE pk = '%ld'", series_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                while (resultados.NextRow()) {
                        FillInSeriesModel(resultados, seriesModel);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "SeriesModel "<<ex.GetMessage().ToUTF8());
                seriesModel.pk = series_pk;
        }
        return seriesModel;
}

void GNC::GCS::HistoryController::GetDatabaseDatasetFromSeries(long series_pk, GIL::DICOM::DicomDataset& dataset)
{
        GNC::GCS::HistoryController::SeriesModel seriesModel = GetSeriesModel(series_pk);
        dataset.tags[GKDCM_SeriesInstanceUID] = seriesModel.series_iuid;
        dataset.tags[GKDCM_SeriesNumber] = seriesModel.series_no;
        dataset.tags[GKDCM_Modality] = seriesModel.series_modality;
        dataset.tags[GKDCM_BodyPartExamined] = seriesModel.body_part;
        dataset.tags[GKDCM_Laterality] = seriesModel.laterality;
        dataset.tags[GKDCM_InstitutionName] = seriesModel.institution;
        dataset.tags[GKDCM_StationName] = seriesModel.station_name;
        dataset.tags[GKDCM_InstitutionalDepartmentName] = seriesModel.department;
        dataset.tags[GKDCM_PerformingPhysicianName] = seriesModel.perf_physician;
        dataset.tags[GKDCM_ProtocolName] = seriesModel.uid_importer;
        dataset.tags[GKDCM_FrameOfReferenceUID] = seriesModel.frame_of_reference;
        dataset.tags[GKDCM_SeriesDescription] = seriesModel.series_desc;
}


void GNC::GCS::HistoryController::GetSeriesModelFromUIDs(const GNC::GCS::HistoryController::StringList& uids, GNC::GCS::HistoryController::SeriesModelList& series)
{
        if (uids.empty()) {
                return;
        }

        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << bufSQL.Format("SELECT Series.pk, Series.study_fk, Series.series_iuid, Series.series_datetime, Series.series_no,Series.modality,"
                                      "Series.uid_importer, Series.body_part, Series.laterality, Series.series_desc,"
                                      "Series.institution,Series.station_name,Series.department,Series.perf_physician,"
                                      "Series.num_instances, Series.frame_of_reference_uid, Series.tuids_in_series, Series.location, Series.created_time, Series.updated_time "
                                      "FROM Series "
                                      "WHERE series_iuid IN (");
                for (GNC::GCS::HistoryController::StringList::const_iterator itUid = uids.begin(); itUid != uids.end(); ++itUid) {
                        if (itUid != uids.begin()) {
                                ostr << ",";
                        }
                        ostr << bufSQL.Format("'%q'",(*itUid).c_str());
                }
                ostr << ")";
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
                std::map<long, SeriesModel> mapOfSeries;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInSeriesModel(resultados, mapOfSeries[pk]);
                }
                for (std::map<long, SeriesModel>::const_iterator it = mapOfSeries.begin(); it != mapOfSeries.end(); ++it) {
                        series.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "SeriesModel "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::GetSeriesModelFromUID(const std::string& uids, GNC::GCS::HistoryController::SeriesModelList& series)
{
        StringList list;
        list.push_back(uids);
        GetSeriesModelFromUIDs(list, series);
        return;
}

void GNC::GCS::HistoryController::GetSeriesFromStudy(long study_pk, GNC::GCS::HistoryController::SeriesModelList& listOfSeries)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT Series.pk, Series.study_fk, Series.series_iuid, Series.series_datetime, Series.series_no,Series.modality,"
                              "Series.uid_importer, Series.body_part, Series.laterality, Series.series_desc,"
                              "Series.institution,Series.station_name,Series.department,Series.perf_physician,"
                              "Series.num_instances, Series.frame_of_reference_uid, Series.location, Series.tuids_in_series, Series.created_time, Series.updated_time "
                              "FROM Series "
                              "WHERE study_fk = '%d'",study_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                std::map<long, SeriesModel> mapOfSeries;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInSeriesModel(resultados, mapOfSeries[pk]);
                }
                for (std::map<long, SeriesModel>::const_iterator it = mapOfSeries.begin(); it != mapOfSeries.end(); ++it) {
                        listOfSeries.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "SeriesModel "<<ex.GetMessage().ToUTF8());
        }
}

inline void FillInStudyModel(wxSQLite3ResultSet& resultados, GNC::GCS::HistoryController::StudyModel& studyModel)
{
        studyModel.pk = resultados.GetInt64(wxT("pk")).GetValue();
        studyModel.pat_id = resultados.GetAsString(wxT("pat_id")).ToUTF8();
        studyModel.pat_id_issuer = resultados.GetAsString(wxT("pat_id_issuer")).ToUTF8();
        studyModel.pat_name = resultados.GetAsString(wxT("pat_name")).ToUTF8();
        studyModel.pat_bithdate = GetDateTimeFromWx(resultados.GetAsString(wxT("pat_birthdate")));
        studyModel.pat_sex = resultados.GetAsString(wxT("pat_sex")).GetChar(0);
        studyModel.study_iuid = resultados.GetAsString(wxT("study_iuid")).ToUTF8();
        studyModel.study_id = resultados.GetAsString(wxT("study_id")).ToUTF8();
        studyModel.study_datetime = GetDateTimeFromWx(resultados.GetAsString(wxT("study_datetime")));
        wxString tmp = resultados.GetAsString(wxT("study_datetime"));
        studyModel.study_acc_no = resultados.GetAsString(wxT("accession_no")).ToUTF8();
        studyModel.ref_physician = resultados.GetAsString(wxT("ref_physician")).ToUTF8();
        studyModel.study_desc = resultados.GetAsString(wxT("study_desc")).ToUTF8();
        studyModel.mods_in_study = StringToList(resultados.GetAsString(wxT("mods_in_study")));
        studyModel.cuids_in_study = StringToList(resultados.GetAsString(wxT("cuids_in_study")));
        studyModel.num_series = resultados.GetInt64(wxT("num_series")).GetValue();
        studyModel.num_instances = resultados.GetInt64(wxT("num_instances")).GetValue();
        studyModel.created_time = GetDateTimeFromWx(resultados.GetAsString(wxT("created_time")));
        studyModel.updated_time = GetDateTimeFromWx(resultados.GetAsString(wxT("updated_time")));
        studyModel.location = GNC::GCS::IHistoryController::CharToLocation(*resultados.GetString(wxT("location")).ToUTF8());
}

GNC::GCS::HistoryController::StudyModel GNC::GCS::HistoryController::GetStudyModel(long study_pk)
{
        StudyModel studyModel;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT Study.pk, Study.pat_id, Study.pat_id_issuer, Study.pat_name, Study.pat_birthdate,Study.pat_sex,"
                              "Study.study_iuid, Study.study_id, Study.study_datetime, Study.accession_no,"
                              "Study.ref_physician,Study.study_desc,Study.mods_in_study,Study.cuids_in_study,"
                              "Study.num_series, Study.num_instances, Study.location, Study.created_time, Study.updated_time "
                              "FROM Study "
                              "WHERE pk = '%ld'",study_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                if (resultados.NextRow()) {
                        FillInStudyModel(resultados, studyModel);
                }
                studyModel.pk = study_pk;
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "StudyModel "<<ex.GetMessage().ToUTF8());
        }
        return studyModel;
}


void GNC::GCS::HistoryController::GetStudyModelFromUID(const std::string& uid, GNC::GCS::HistoryController::StudyModelList& studies)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT Study.pk, Study.pat_id, Study.pat_id_issuer, Study.pat_name, Study.pat_birthdate,Study.pat_sex,"
                              "Study.study_iuid, Study.study_id, Study.study_datetime, Study.accession_no,"
                              "Study.ref_physician,Study.study_desc,Study.mods_in_study,Study.cuids_in_study,"
                              "Study.num_series, Study.num_instances, Study.location, Study.created_time, Study.updated_time "
                              "FROM Study "
                              "WHERE study_iuid = '%q'", uid.c_str());
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                std::map<long, StudyModel> mapOfStudies;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInStudyModel(resultados, mapOfStudies[pk]);
                }
                for (std::map<long, StudyModel>::const_iterator it = mapOfStudies.begin(); it != mapOfStudies.end(); ++it) {
                        studies.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "SeriesModel "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::GetDatabaseDatasetFromStudy(long study_pk, GIL::DICOM::DicomDataset& dataset)
{
        GNC::GCS::HistoryController::StudyModel studyModel = GetStudyModel(study_pk);
        dataset.tags[GKDCM_PatientID] = studyModel.pat_id;
        dataset.tags[GKDCM_IssuerOfPatientID] = studyModel.pat_id_issuer;
        dataset.tags[GKDCM_PatientBirthDate] = studyModel.pat_bithdate;
        dataset.tags[GKDCM_PatientSex] = studyModel.pat_sex;
        dataset.tags[GKDCM_PatientName] = studyModel.pat_name;
        dataset.tags[GKDCM_StudyInstanceUID] = studyModel.study_iuid;
        dataset.tags[GKDCM_StudyID] = studyModel.study_id;
        dataset.tags[GKDCM_AccessionNumber] = studyModel.study_acc_no;
        dataset.tags[GKDCM_ReferringPhysicianName] = studyModel.ref_physician;
        dataset.tags[GKDCM_StudyDescription] = studyModel.study_desc;
        std::ostringstream ostr;
        for (std::list<std::string>::const_iterator it = studyModel.mods_in_study.begin(); it != studyModel.mods_in_study.end(); ++it) {
                if (it != studyModel.mods_in_study.begin()) {
                        ostr << "/";
                }
                ostr << (*it);
        }
        dataset.tags[GKDCM_ModalitiesInStudy] = ostr.str();
}

void GNC::GCS::HistoryController::GetStudies(GNC::GCS::History::IHistoryFilter* historyFilter, StudyModelList& listOfStudies)
{
        std::list<GNC::GCS::History::IHistoryFilter*> filterList;
        if (historyFilter != NULL) {
                filterList.push_back(historyFilter);
        }
        GetStudies(filterList,listOfStudies);
}

void GNC::GCS::HistoryController::GetStudies(std::list<GNC::GCS::History::IHistoryFilter*>& historyFilter, StudyModelList& listOfStudies)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << bufSQL.Format("SELECT Study.pk, Study.pat_id, Study.pat_id_issuer, Study.pat_name, Study.pat_birthdate,Study.pat_sex,"
                                      "Study.study_iuid, Study.study_id, Study.study_datetime, Study.accession_no,"
                                      "Study.ref_physician,Study.study_desc,Study.mods_in_study,Study.cuids_in_study,"
                                      "Study.num_series, Study.num_instances, Study.location, Study.created_time, Study.updated_time "
                                      "FROM Study ");
                bool first = true;
                for (std::list<GNC::GCS::History::IHistoryFilter*>::iterator it = historyFilter.begin(); it != historyFilter.end(); ++it) {
                        if ((*it)->IsEnabled()) {
                                std::string str = (*it)->GetSQLFilter();
                                if (!str.empty()) {
                                        if (!first) {
                                                ostr << " AND ";
                                        } else {
                                                ostr << "WHERE ";
                                                first = false;
                                        }
                                        ostr << str;
                                }
                        }
                }
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
                while (resultados.NextRow()) {
                        StudyModel studyModel;
                        FillInStudyModel(resultados, studyModel);
                        listOfStudies.push_back(studyModel);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "StudyModel "<<ex.GetMessage().ToUTF8());
        }
}


inline void FillInFileModel(wxSQLite3ResultSet& resultados, GNC::GCS::HistoryController::FileModel& fileModel)
{
        fileModel.pk = resultados.GetInt64(wxT("pk")).GetValue();
        fileModel.series_fk = resultados.GetInt64(wxT("series_fk")).GetValue();
        {
                int columnIndex = resultados.FindColumnIndex(wxT("file_path"));
                const char* filePath = (const char*) sqlite3_column_text((sqlite3_stmt*) resultados.GetStmt(), columnIndex);
                fileModel.file_path = std::string(filePath);
        }
        fileModel.sopiuid = resultados.GetAsString(wxT("file_sopiuid")).ToUTF8();
        fileModel.tsuid = resultados.GetAsString(wxT("file_tsuid")).ToUTF8();
        fileModel.content_datetime = GetDateTimeFromWx(resultados.GetAsString(wxT("content_datetime")));
        fileModel.sopcuid = resultados.GetAsString(wxT("sopcuid")).ToUTF8();
        fileModel.orientation = resultados.GetAsString(wxT("orientation")).GetChar(0);
        fileModel.image_position[0] = resultados.GetDouble(wxT("image_positionx"));
        fileModel.image_position[1] = resultados.GetDouble(wxT("image_positiony"));
        fileModel.image_position[2] = resultados.GetDouble(wxT("image_positionz"));
        fileModel.slice_location = resultados.GetDouble(wxT("slice_location"));
        fileModel.slice_thickness = resultados.GetDouble(wxT("slice_thickness"));
        fileModel.height = resultados.GetInt(wxT("height"));
        fileModel.width = resultados.GetInt(wxT("width"));
        fileModel.number_of_frames = resultados.GetInt(wxT("number_of_frames"));
        fileModel.instance_number = resultados.GetInt(wxT("instance_number"));
        fileModel.file_desc = resultados.GetAsString(wxT("file_desc")).ToUTF8();
        fileModel.direction_cosines = resultados.GetAsString(wxT("direction_cosines")).ToUTF8();
        fileModel.spacing = resultados.GetAsString(wxT("spacing")).ToUTF8();
        fileModel.created_time = GetDateTimeFromWx(resultados.GetAsString(wxT("created_time")));
        fileModel.updated_time = GetDateTimeFromWx(resultados.GetAsString(wxT("updated_time")));
        fileModel.location = GNC::GCS::IHistoryController::CharToLocation(*resultados.GetString(wxT("location")).ToUTF8());
}

GNC::GCS::HistoryController::FileModel GNC::GCS::HistoryController::GetFrameOfReference(const std::string& series_pk)
{
        std::istringstream istr(series_pk);
        long pk;
        istr >> pk;
        return GetFrameOfReference(pk);
}

GNC::GCS::HistoryController::FileModel GNC::GCS::HistoryController::GetFrameOfReference(long series_pk)
{
        FileModel fileModel;
        wxSQLite3StatementBuffer bufSQL;
        bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.content_datetime,"
                      "File.sopcuid, File.orientation, File.image_positionx, File.image_positiony,"
                      "File.image_positionz,File.slice_location, File.slice_thickness, File.height,File.width, File.number_of_frames,"
                      "File.instance_number, File.file_desc, File.direction_cosines, File.spacing, File.location, File.created_time, File.updated_time "
                      "FROM File, Series "
                      "WHERE File.series_fk = Series.pk AND File.file_sopiuid=Series.frame_of_reference_uid AND Series.pk='%ld'", series_pk);

        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
        if (resultados.NextRow()) {
                FillInFileModel(resultados, fileModel);
                fileModel.real_path = GetPathOfFile(fileModel.location, fileModel.pk, fileModel.file_path);
        } else {
                FileModelList listOfFiles;
                GetSeriesSortedFileModels(series_pk, listOfFiles, true);
                if (!listOfFiles.empty()) {
                        return listOfFiles.front();
                }
        }

        return fileModel;
}

GNC::GCS::HistoryController::FileModel GNC::GCS::HistoryController::GetFileModel(long file_pk)
{
        FileModel fileModel;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.content_datetime,"
                              "File.sopcuid, File.orientation, File.image_positionx, File.image_positiony,"
                              "File.image_positionz,File.slice_location, File.slice_thickness, File.height,File.width, File.number_of_frames,"
                              "File.instance_number, File.file_desc, File.direction_cosines, File.spacing, File.location, File.created_time, File.updated_time "
                              "FROM File "
                              "WHERE pk = '%ld'", file_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                if (resultados.NextRow()) {
                        FillInFileModel(resultados, fileModel);
                        fileModel.real_path = GetPathOfFile(fileModel.location, fileModel.pk, fileModel.file_path);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "FileModel "<<ex.GetMessage().ToUTF8());
        }
        return fileModel;
}

GNC::GCS::HistoryController::FileModel GNC::GCS::HistoryController::GetFileModelFromSopInstance(const std::string& sopInstanceUID, long study_pk)
{
        FileModel fileModel;
        try {
                wxSQLite3StatementBuffer bufSQL;
                if (study_pk == -1) {
                        bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.content_datetime,"
                                      "File.sopcuid, File.orientation, File.image_positionx, File.image_positiony,"
                                      "File.image_positionz,File.slice_location, File.slice_thickness, File.height,File.width, File.number_of_frames,"
                                      "File.instance_number, File.file_desc, File.direction_cosines, File.spacing, File.location, File.created_time, File.updated_time "
                                      "FROM File "
                                      "WHERE File.file_sopiuid = '%q'", sopInstanceUID.c_str());
                } else {
                        bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.content_datetime,"
                                      "File.sopcuid, File.orientation, File.image_positionx, File.image_positiony,"
                                      "File.image_positionz,File.slice_location, File.slice_thickness, File.height,File.width, File.number_of_frames,"
                                      "File.instance_number, File.file_desc, File.direction_cosines, File.spacing, File.location, File.created_time, File.updated_time "
                                      "FROM File, Series "
                                      "WHERE File.file_sopiuid = '%q' AND File.series_fk= Series.pk AND series.study_fk='%ld'", sopInstanceUID.c_str(), study_pk);
                }
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                if (resultados.NextRow()) {
                        FillInFileModel(resultados, fileModel);
                        fileModel.real_path = GetPathOfFile(fileModel.location, fileModel.pk, fileModel.file_path);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "FileModel "<<ex.GetMessage().ToUTF8());
        }
        return fileModel;
}

void GNC::GCS::HistoryController::GetAllPatients(GNC::GCS::HistoryController::PatientModelList& patientList)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT DISTINCT pat_id, pat_id_issuer, pat_name, pat_birthdate,pat_sex "
                              "FROM Study");
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                while (resultados.NextRow()) {
                        PatientModel patient;
                        patient.id = resultados.GetAsString(wxT("pat_id")).ToUTF8();
                        patient.id_issuer = resultados.GetAsString(wxT("pat_id_issuer")).ToUTF8();
                        patient.name = resultados.GetAsString(wxT("pat_name")).ToUTF8();
                        patient.bithdate = GetDateTimeFromWx(resultados.GetAsString(wxT("pat_birthdate")));
                        patient.sex = resultados.GetAsString(wxT("pat_sex")).GetChar(0);
                        patientList.push_back(patient);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "StudyModel "<<ex.GetMessage().ToUTF8());
        }
}

bool GNC::GCS::HistoryController::ExistsSeriesWithUID(const std::string& seriesUID)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("Select count(*) from Series where series_iuid = '%q'",seriesUID.c_str());
                if(m_pConexion->ExecuteScalar(bufSQL)>0) {
                        return true;
                } else {
                        return false;
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "ExistsSeriesByUID "<<ex.GetMessage().ToUTF8())
                return false;
        }
}

bool GNC::GCS::HistoryController::ExistsStudyWithUID(const std::string& studyUID)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("Select count(*) from Study where study_iuid = '%q'",studyUID.c_str());
                if(m_pConexion->ExecuteScalar(bufSQL)>0) {
                        return true;
                } else {
                        return false;
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "ExistsStudyWithUID "<<ex.GetMessage().ToUTF8())
                return false;
        }
}

inline std::string GetFileOrderBySentence(char orientacion)
{
        std::ostringstream ostr;
        ostr << "ORDER BY ";
        ostr << "File.instance_number ASC, File.content_datetime ASC";
        switch (orientacion) {
        case 'A': {
                ostr << ", File.image_positionz ASC";
        }
        break;
        case 'C': {
                ostr << ", File.image_positiony ASC";
        }
        break;
        case 'S': {
                ostr << ", File.image_positionx ASC";
        }
        break;
        case 'O': {
                ostr << ", File.slice_location ASC";
        }
        break;
        default:
                ostr << ", File.slice_location ASC";
                break;
        }
        return ostr.str();
}

int GNC::GCS::HistoryController::GetNumberOfFiles()
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT count(*)"
                              "FROM File");
                return m_pConexion->ExecuteScalar(bufSQL);
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetNumberOfFiles "<<ex.GetMessage().ToUTF8())
        }
        return 0;
}

void GNC::GCS::HistoryController::GetSeriesSortedFileModels(long series_pk, GNC::GCS::HistoryController::FileModelList& fileList, bool onlyFirst)
{
        try {
                std::ostringstream sentencia;
                wxSQLite3StatementBuffer bufSQL;
                sentencia << bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.content_datetime,"
                                           "File.sopcuid, File.orientation, File.image_positionx, File.image_positiony,"
                                           "File.image_positionz,File.slice_location, File.slice_thickness, File.height,File.width, File.number_of_frames,"
                                           "File.instance_number, File.file_desc, File.direction_cosines, File.spacing, File.location, File.created_time, File.updated_time "
                                           "FROM File WHERE "
                                           "File.series_fk='%ld' "
                                           ,series_pk);
                sentencia << GetFileOrderBySentence(GetSeriesOrientation(series_pk));

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(sentencia.str().c_str());
                while (resultados.NextRow()) {
                        FileModel fileModel;
                        FillInFileModel(resultados, fileModel);
                        fileModel.real_path = GetPathOfFile(fileModel.location, fileModel.pk, fileModel.file_path);
                        fileList.push_back(fileModel);
                        if (onlyFirst) {
                                return;
                        }
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetPathsSerieOrdenados "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error reading history: \n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
}


inline void FillInLightFileModel(wxSQLite3ResultSet& resultados, GNC::GCS::HistoryController::LightFileModel& fileModel)
{
        fileModel.pk = resultados.GetInt64(wxT("pk")).GetValue();
        fileModel.series_fk = resultados.GetInt64(wxT("series_fk")).GetValue();
        {
                int columnIndex = resultados.FindColumnIndex(wxT("file_path"));
                const char* filePath = (const char*) sqlite3_column_text((sqlite3_stmt*) resultados.GetStmt(), columnIndex);
                fileModel.file_path = std::string(filePath);
        }
        fileModel.sopiuid = resultados.GetAsString(wxT("file_sopiuid")).ToUTF8();
        fileModel.tsuid = resultados.GetAsString(wxT("file_tsuid")).ToUTF8();
        fileModel.sopcuid = resultados.GetAsString(wxT("sopcuid")).ToUTF8();
        fileModel.location = GNC::GCS::IHistoryController::CharToLocation(*resultados.GetString(wxT("location")).ToUTF8());
}


void GNC::GCS::HistoryController::GetSeriesSortedFileModels(long series_pk, GNC::GCS::HistoryController::LightFileModelList& fileList, bool onlyFirst)
{
        try {
                std::ostringstream sentencia;
                wxSQLite3StatementBuffer bufSQL;
                sentencia << bufSQL.Format("SELECT File.pk, File.series_fk, File.file_path, File.file_sopiuid, File.file_tsuid, File.sopcuid, File.location "
                                           "FROM File WHERE "
                                           "File.series_fk='%ld' "
                                           ,series_pk);
                sentencia << GetFileOrderBySentence(GetSeriesOrientation(series_pk));

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(sentencia.str().c_str());
                while (resultados.NextRow()) {
                        LightFileModel fileModel;
                        FillInLightFileModel(resultados, fileModel);
                        fileModel.real_path = GetPathOfFile(fileModel.location, fileModel.pk, fileModel.file_path);
                        fileList.push_back(fileModel);
                        if (onlyFirst) {
                                return;
                        }
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetPathsSerieOrdenados "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error reading history: \n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
}

GNC::GCS::HistoryController::StringList GNC::GCS::HistoryController::GetSeriesSortedPathsByUID(const std::string& seriesUID)
{
        StringList list;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("Select series_iuid from Series where series_iuid = '%q'",seriesUID.c_str());
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                if (resultados.NextRow()) {
                        std::string pk(resultados.GetAsString(wxT("series_iuid")).ToUTF8());
                        list = GetSeriesSortedPaths(pk);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "ExistsSeriesByUID "<<ex.GetMessage().ToUTF8())
        }
        return list;
}

GNC::GCS::HistoryController::StringList GNC::GCS::HistoryController::GetSeriesSortedPaths(const std::string& series_pk)
{
        std::istringstream istr(series_pk);
        long id;
        istr >> id;
        return GetSeriesSortedPaths(id);
}

GNC::GCS::HistoryController::StringList GNC::GCS::HistoryController::GetSeriesSortedPaths(long series_pk)
{
        GNC::GCS::HistoryController::StringList result;
        try {
                std::ostringstream sentencia;
                wxSQLite3StatementBuffer bufSQL;
                sentencia << bufSQL.Format("SELECT pk, file_path  "
                                           "FROM File WHERE "
                                           "File.series_fk='%ld' "
                                           ,series_pk);
                sentencia << GetFileOrderBySentence(GetSeriesOrientation(series_pk));
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(sentencia.str().c_str());
                while (resultados.NextRow()) {
                        std::string filePath(resultados.GetAsString(wxT("file_path")).ToUTF8());
                        if (filePath.empty()) {
                                filePath = GetPathOfFile(resultados.GetInt64(wxT("pk")).GetValue());
                        } else {
                                filePath = GetAbsolutePath(filePath);
                        }

                        result.push_back( filePath );
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetPathsSerieOrdenados "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error reading history: \n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
        return result;
}

GNC::GCS::HistoryController::StringList GNC::GCS::HistoryController::GetSeriesSopClasses(long series_pk)
{
        GNC::GCS::HistoryController::StringList result;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT distinct sopcuid  "
                              "FROM File WHERE "
                              "File.series_fk='%ld' "
                              ,series_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                while (resultados.NextRow()) {
                        std::string sopCuid(resultados.GetAsString(wxT("sopcuid")).ToUTF8());
                        result.push_back( sopCuid );
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetSeriesSopClasses "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error reading history: \n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
        return result;
}

char GNC::GCS::HistoryController::GetSeriesOrientation(long series_pk)
{
        char orientacion = '?';
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT DISTINCT File.orientation "
                              "FROM File WHERE "
                              "File.series_fk='%ld'"
                              ,series_pk);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                if (resultados.NextRow()) {
                        std::string tmp (resultados.GetAsString(wxT("orientation")).ToUTF8());
                        if(tmp.size() == 1) {
                                orientacion = tmp[0];
                        }
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "GetPathsSerieOrdenados "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Error reading history: \n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
        }
        return orientacion;
}


bool GNC::GCS::HistoryController::DeleteSeriesList(const IdList& pks)
{
        bool result = true;
        if (pks.size() == 0) {
                return true;
        }
        try {
                std::list<std::string> listOfPaths;
                std::list<long> listOfThumbnails;
                wxSQLite3StatementBuffer bufSQL;
                {
                        std::ostringstream ostrSelect;
                        ostrSelect << "SELECT DISTINCT File.pk, File.file_path FROM File "
                                   "WHERE ";

                        for (IdList::const_iterator it = pks.begin(); it != pks.end(); ++it) {
                                if (it != pks.begin()) {
                                        ostrSelect << " OR";
                                }
                                ostrSelect << bufSQL.Format(" File.series_fk = '%ld'", (*it));
                        }

                        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrSelect.str().c_str());
                        while (resultados.NextRow()) {
                                wxString filePath = resultados.GetAsString(wxT("file_path"));
                                long pk = resultados.GetInt64(wxT("pk")).GetValue();
                                listOfThumbnails.push_back(pk);
                                if (filePath.empty()) {
                                        std::string filePath = GetPathOfFile(pk);
                                        listOfPaths.push_back(filePath);
                                }
                        }
                }
                RemoveFilesFromDisk(listOfPaths);
                DeleteThumbnails(	listOfThumbnails );

                {
                        std::ostringstream ostrDelete;
                        ostrDelete << "Delete from Series where ";
                        for (IdList::const_iterator it = pks.begin(); it != pks.end(); ++it) {
                                if (it != pks.begin()) {
                                        ostrDelete << " OR";
                                }
                                ostrDelete << bufSQL.Format(" Series.pk = '%ld'", (*it));
                        }
                        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrDelete.str().c_str());
                }

        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "EliminarSerie "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Failed to delete the number of database:\n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
                result = false;
        }
        return result;
}

bool GNC::GCS::HistoryController::DeleteSopInstanceUIDs(const StringList& sopInstanceUIds)
{
        bool result = true;
        if (sopInstanceUIds.size() == 0) {
                return true;
        }
        try {
                std::list<std::string> listOfPaths;
                std::list<long> listOfThumbnails;
                wxSQLite3StatementBuffer bufSQL;
                {
                        std::ostringstream ostrSelect;
                        ostrSelect << "SELECT DISTINCT File.pk, File.file_path FROM File "
                                   "WHERE ";

                        for (StringList::const_iterator it = sopInstanceUIds.begin(); it != sopInstanceUIds.end(); ++it) {
                                if (it != sopInstanceUIds.begin()) {
                                        ostrSelect << " OR";
                                }
                                ostrSelect << bufSQL.Format(" File.file_sopiuid = '%q'", (*it).c_str());
                        }

                        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrSelect.str().c_str());
                        while (resultados.NextRow()) {
                                wxString filePath = resultados.GetAsString(wxT("file_path"));
                                long pk = resultados.GetInt64(wxT("pk")).GetValue();
                                listOfThumbnails.push_back(pk);
                                if (filePath.empty()) {
                                        std::string filePath = GetPathOfFile(pk);
                                        listOfPaths.push_back(filePath);
                                }
                        }
                }
                RemoveFilesFromDisk(listOfPaths);
                DeleteThumbnails(listOfThumbnails);

                {
                        std::ostringstream ostrDelete;
                        ostrDelete << "Delete from File where ";
                        for (StringList::const_iterator it = sopInstanceUIds.begin(); it != sopInstanceUIds.end(); ++it) {
                                if (it != sopInstanceUIds.begin()) {
                                        ostrDelete << " OR";
                                }
                                ostrDelete << bufSQL.Format(" File.file_sopiuid = '%q'", (*it).c_str());
                        }
                        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostrDelete.str().c_str());
                }

        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "DeleteSopInstanceUIDs "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Failed to delete the number of database:\n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
                result = false;
        }
        return result;
}

bool GNC::GCS::HistoryController::EmptyHistory(bool eliminarDelDisco)
{
        wxCriticalSectionLocker locker(*m_pCriticalSection);
        wxCriticalSectionLocker locker1(*m_pCriticalSectionThumbnails);
        try {
                wxSQLite3StatementBuffer bufSQL;
                if(eliminarDelDisco) {
                        bufSQL.Format("SELECT DISTINCT file_path, pk FROM File");
                        wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                        std::list<std::string> listaPaths;
                        while (resultados.NextRow()) {
                                std::string file_path(TOPATH(resultados.GetAsString(0)));
                                if (file_path.empty()) {
                                        long pk = resultados.GetInt64(1).GetValue();
                                        listaPaths.push_back(GetPathOfFile(pk));
                                }
                        }
                        RemoveFilesFromDisk(listaPaths);
                }
                bufSQL.Format("DELETE FROM File");
                m_pConexion->ExecuteUpdate(bufSQL);
                bufSQL.Format("DELETE FROM Series");
                m_pConexion->ExecuteUpdate(bufSQL);
                bufSQL.Format("DELETE FROM Study");
                m_pConexion->ExecuteUpdate(bufSQL);
                bufSQL.Format("DELETE FROM Thumbnails");
                m_pConexionThumbnails->ExecuteUpdate(bufSQL);
                return true;
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "VaciarHistorial "<<ex.GetMessage().ToUTF8())
                wxMessageBox(_("Failed to empty the history:\n")+ex.GetMessage(), _("Info"),
                             wxOK | wxICON_WARNING);
                return false;
        }
}
//
bool GNC::GCS::HistoryController::ReadFile(DICOMFileModel& modelo,const std::string& pathAbsoluto, TAddErrorList& errorList, TAddAction action)
{
        switch (action) {
        case TAA_MOVE:
        case TAA_COPY:
                modelo.study.location = modelo.series.location = modelo.file.location = TL_LocalDatabase;
                break;
        case TAA_LINK:
                modelo.study.location = modelo.series.location = modelo.file.location = TL_LocalLinked;
                break;
        }
        return ReadFile(modelo, pathAbsoluto, errorList);
}

bool GNC::GCS::HistoryController::ReadFile(DICOMFileModel& modeloDCM, const std::string& pathAbsoluto, TAddErrorList& errorList)
{
        GIL::DICOM::IDICOMManager*	pDICOMManager= GNC::Entorno::Instance()->GetPACSController()->CrearInstanciaDeDICOMManager();
        GIL::DICOM::DicomDataset base;
        if(!pDICOMManager->CargarFichero(pathAbsoluto, base)) {
                //error en la carga dcmtk no es un dicom
                GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);
                errorList.push_back(TAddError(TAddError::TE_WrongFormat, pathAbsoluto));
                return false;
        }

        std::string tag, tag1;
        //si es dicomdir => return false
        // Leemos el tag 0004 0x1130 => Si existe es DICOMDIR
        if (base.getTag(GKDCM_FileSetID, tag)) {
                errorList.push_back(TAddError(TAddError::TE_DICOMDir, pathAbsoluto));
                return false;
        }

        //study
        if (base.getTag(GKDCM_PatientName, tag)) {
                modeloDCM.study.pat_name = tag;
        }

        if (base.getTag(GKDCM_PatientID, tag)) {
                modeloDCM.study.pat_id = tag;
        }

        if (base.getTag(GKDCM_IssuerOfPatientID, tag)) {
                modeloDCM.study.pat_id_issuer = tag;
        }

        if (base.getTag(GKDCM_PatientBirthDate, tag)) {
                modeloDCM.study.pat_bithdate = parseDateTime(tag, "");
        }

        if(base.getTag(GKDCM_PatientSex, tag)) {
                if(tag.compare("M")==0) {
                        modeloDCM.study.pat_sex ='M';
                } else if(tag.compare("F")==0) {
                        modeloDCM.study.pat_sex ='F';
                } else if (!tag.empty()) {
                        modeloDCM.study.pat_sex='O';
                } else {
                        modeloDCM.study.pat_sex = '?';
                }
        }

        if (base.getTag(GKDCM_StudyInstanceUID, tag)) {
                modeloDCM.study.study_iuid = tag;
        }

        if (base.getTag(GKDCM_StudyID, tag)) {
                modeloDCM.study.study_id = tag;
        }

        if (base.getTag(GKDCM_StudyDate, tag) && base.getTag(GKDCM_StudyTime, tag1)) {
                modeloDCM.study.study_datetime = parseDateTime(tag, tag1);
        }

        if (base.getTag(GKDCM_AccessionNumber, tag)) {
                modeloDCM.study.study_acc_no = tag;
        }

        if (base.getTag(GKDCM_ReferringPhysicianName, tag)) {
                modeloDCM.study.ref_physician = tag;
        }

        if (base.getTag(GKDCM_StudyDescription, tag)) {
                modeloDCM.study.study_desc = tag;
        }

        if (base.getTag(GKDCM_StudyDescription, tag)) {
                modeloDCM.study.study_desc = tag;
        }

        //series
        if (base.getTag(GKDCM_SeriesInstanceUID, tag)) {
                modeloDCM.series.series_iuid = tag;
        }

        tag1 = "";
        tag = "";
        if (base.getTag(GKDCM_SeriesDate, tag) && base.getTag(GKDCM_SeriesTime, tag1)) {
                modeloDCM.series.series_datetime = parseDateTime(tag, tag1);
        }

        if (base.getTag(GKDCM_SeriesNumber, tag)) {
                modeloDCM.series.series_no = tag;
        }

        if (base.getTag(GKDCM_Modality, tag)) {
                modeloDCM.series.series_modality = tag;
        }

        if (base.getTag(GKDCM_ProtocolName, tag)) {
                modeloDCM.series.uid_importer = tag;
        }

        if (base.getTag(GKDCM_BodyPartExamined, tag)) {
                modeloDCM.series.body_part = tag;
        }

        if (base.getTag(GKDCM_Laterality, tag)) {
                modeloDCM.series.laterality = tag;
        }

        if (base.getTag(GKDCM_SeriesDescription, tag)) {
                modeloDCM.series.series_desc = tag;
        }

        if (base.getTag(GKDCM_InstitutionName, tag)) {
                modeloDCM.series.institution = tag;
        }

        if (base.getTag(GKDCM_StationName, tag)) {
                modeloDCM.series.station_name = tag;
        }

        if (base.getTag(GKDCM_InstitutionalDepartmentName, tag)) {
                modeloDCM.series.department = tag;
        }

        if (base.getTag(GKDCM_PerformingPhysicianName, tag)) {
                modeloDCM.series.perf_physician = tag;
        }

        if (base.getTag(GKDCM_FrameOfReferenceUID, tag)) {
                modeloDCM.series.frame_of_reference = tag;
        }

        //file
        if (base.getTag(GKDCM_SOPInstanceUID, tag)) {
                modeloDCM.file.sopiuid = tag;
        }

        //transfer syntax
        {
                GIL::DICOM::TipoMetaInfo metaTags;
                pDICOMManager->CargarMetaInfo(pathAbsoluto, metaTags);

                if(metaTags.getTag(GKDCM_TransferSyntaxUID,tag)) {
                        modeloDCM.file.tsuid = tag;
                }
        }
        if(base.getTag(GKDCM_SOPClassUID,tag)) {
                modeloDCM.file.sopcuid = tag;
        }

        tag1 = "";
        tag = "";
        if (base.getTag(GKDCM_AcquisitionDate, tag) && base.getTag(GKDCM_AcquisitionTime, tag1)) {
                modeloDCM.file.content_datetime = parseDateTime(tag, tag1);
        }

        if (base.getTag(GKDCM_PixelSpacing, tag)) {
                modeloDCM.file.spacing = tag;
        } else if (base.getTag(GKDCM_ImagerPixelSpacing, tag)) {
                modeloDCM.file.spacing = tag;
        }

        std::string tagOrientation;
        //GKDCM_ImageOrientationPatient its longer than info tags...
        if (!pDICOMManager->GetTag(0x0020,0x0037, tagOrientation)) {
                //For ACR-NEMA GKDCM_ACR_NEMA_ImageOrientation
                pDICOMManager->GetTag(0x0020,0x0037, tagOrientation);
        }
        if (!tagOrientation.empty()) {
                modeloDCM.file.direction_cosines = tagOrientation;
                double row[3] = {0.0,0.0,0.0};
                double col[3] = {0.0,0.0,0.0};
                char c;
                bool ok = true;
                std::istringstream istr(tagOrientation);
                for (int i = 0; i < 3; ++i) {
                        if (ok && !istr.eof()) {
                                istr >> row[i];
                                if (!istr.eof()) {
                                        istr >> c; //la barra
                                }
                        } else {
                                ok = false;
                        }
                }
                for (int i = 0; i < 3; ++i) {
                        if (ok && !istr.eof()) {
                                istr >> col[i];
                                if (!istr.eof()) {
                                        istr >> c; //la barra
                                }
                        } else {
                                ok = false;
                        }
                }
                if (ok) {
                        modeloDCM.file.orientation = makeImageOrientationLabelFromImageOrientationPatient(row[0], row[1], row[2], col[0], col[1], col[2]);
                } else {
                        modeloDCM.file.orientation = 'A';
                }
        } else {
                modeloDCM.file.orientation = 'A';
        }

        //patient position
        modeloDCM.file.image_position[0] = 0.0;
        modeloDCM.file.image_position[1] = 0.0;
        modeloDCM.file.image_position[2] = 0.0;
        if (base.getTag(GKDCM_ImagePositionPatient, tag) ) {
                std::istringstream iiPos(tag);

                char c;
                bool ok = true;
                std::istringstream istr(tag);
                for (int i = 0; i < 3; ++i) {
                        if (ok && !istr.eof()) {
                                istr >> modeloDCM.file.image_position[i];
                                if (!istr.eof()) {
                                        istr >> c; //la barra
                                }
                        } else {
                                ok = false;
                        }
                }
                if(!ok) {
                        modeloDCM.file.image_position[0] = 0.0;
                        modeloDCM.file.image_position[1] = 0.0;
                        modeloDCM.file.image_position[2] = 0.0;
                }
        }

        if (base.getTag(GKDCM_SliceLocation, tag)) { //slice location
                std::istringstream istr(tag);
                istr >>	modeloDCM.file.slice_location;
        }

        if (base.getTag(GKDCM_SliceThickness, tag)) { //slice thickness
                std::istringstream istr(tag);
                istr >>	modeloDCM.file.slice_thickness;
        }

        if (base.getTag(GKDCM_Rows, tag) ) {
                std::istringstream istr(tag);
                istr >>	modeloDCM.file.height;
        }

        if (base.getTag(GKDCM_Columns, tag) ) {
                std::istringstream istr(tag);
                istr >>	modeloDCM.file.width;
        }

        if (base.getTag(GKDCM_NumberOfFrames, tag)) {
                std::istringstream istr(tag);
                istr >> modeloDCM.file.number_of_frames;
        } else {
                modeloDCM.file.number_of_frames = 1;
        }

        if (base.getTag(GKDCM_InstanceNumber, tag) ) {
                std::istringstream istr(tag);
                istr >>	modeloDCM.file.instance_number;
        }

        if (base.getTag(GKDCM_ImageComments, tag)) {
                modeloDCM.file.file_desc = tag;
        }

        GNC::Entorno::Instance()->GetPACSController()->LiberarInstanciaDeDICOMManager(pDICOMManager);


        return true;
}



void GNC::GCS::HistoryController::RemoveFilesFromDisk(GNC::GCS::HistoryController::StringList& listOfFiles)
{
        for (std::list<std::string>::iterator it = listOfFiles.begin(); it!= listOfFiles.end(); ++it) {
                wxString wxPath = FROMPATH((*it));
                if(wxFileExists(wxPath)) {
                        wxRemoveFile(wxPath);
                }
        }
}

void GNC::GCS::HistoryController::SetThumbnail(long file_pk, int ancho, int alto, const unsigned char* imagen)
{
        wxCriticalSectionLocker locker(*m_pCriticalSectionThumbnails);
        int size = ancho * alto * 3;
        if(size > 0) {
                wxSQLite3StatementBuffer bufSQL;
                try {
                        bufSQL.Format("INSERT OR REPLACE INTO Thumbnails (file_pk,width,height,thumb) VALUES ('%ld','%d','%d',?);",file_pk,ancho,alto);
                        wxSQLite3Statement stmt = m_pConexionThumbnails->PrepareStatement(bufSQL);
                        if(stmt.IsOk()) {
                                stmt.Bind(1, imagen, size);
                                stmt.ExecuteUpdate();
                        } else {
                                LOG_WARN("Historial", "Error al insertar en la base de datos");
                        }
                } catch (wxSQLite3Exception& ex) {
                        LOG_ERROR("Historial", "SetThumbnail " << ex.GetMessage().ToUTF8() << " file_pk = "<< file_pk);
                }
        }
}

void GNC::GCS::HistoryController::GetThumbnail(long file_pk, int& ancho, int& alto, unsigned char*& imagen)
{
        wxSQLite3StatementBuffer bufSQL;
        try {
                bufSQL.Format("Select width, height, thumb FROM Thumbnails WHERE file_pk = '%ld';",file_pk);
                wxSQLite3ResultSet resultados = m_pConexionThumbnails->ExecuteQuery(bufSQL);

                if (resultados.NextRow()) {
                        ancho = resultados.GetInt(wxT("width"));
                        alto = resultados.GetInt(wxT("height"));
                        int size = ancho*alto*3;
                        int blobLen;
                        const unsigned char* pbin = resultados.GetBlob(wxT("thumb"), blobLen);
                        if(blobLen != size) {
                                ancho = alto = 0;
                                imagen = NULL;
                        } else {
                                imagen = (unsigned char*)malloc(size*sizeof(unsigned char));
                                memcpy(imagen, pbin, size);
                        }
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("Historial", "GetThumbnail "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::DeleteThumbnails(GNC::GCS::HistoryController::IdList& listPks)
{
        std::ostringstream ostrDelete;
        wxSQLite3StatementBuffer bufSQL;
        ostrDelete << "Delete from Thumbnails where file_pk in (";
        for (IdList::const_iterator it = listPks.begin(); it != listPks.end(); ++it) {
                if (it != listPks.begin()) {
                        ostrDelete << ",";
                }
                ostrDelete << bufSQL.Format(" '%ld'", (*it));
        }
        ostrDelete << ")";
        wxSQLite3ResultSet resultados = m_pConexionThumbnails->ExecuteQuery(ostrDelete.str().c_str());
}
//
////endergion


void GNC::GCS::HistoryController::SaveTask(GNC::GCS::IPersistentCommand* pTask)
{
        if (pTask->getIdBBDD() > 0) {
                //update
                try {
                        wxSQLite3StatementBuffer bufSQL;
                        bufSQL.Format("UPDATE Tasks SET status='%d', error_description='%q' "
                                      "WHERE pk='%d';", (int)pTask->getStatus(), pTask->getErrorDescription().c_str(), pTask->getIdBBDD());
                        m_pConexion->ExecuteUpdate(bufSQL);
                } catch (wxSQLite3Exception& ex) {
                        LOG_ERROR("BBDD", "Exception updating task "<<ex.GetMessage().ToUTF8())
                }
        } else {
                //insert
                try {
                        wxSQLite3StatementBuffer bufSQL;
                        bufSQL.Format("INSERT INTO Tasks (uid,data,description) VALUES ('%q','%q','%q');",
                                      pTask->getClassUID().c_str(), pTask->serialize().c_str(), pTask->getDescription().c_str());
                        //begin transaction
                        m_pConexion->Begin();
                        if (m_pConexion->ExecuteUpdate(bufSQL) > 0) {
                                long idTask = (long)m_pConexion->GetLastRowId().GetValue();
                                pTask->setIdBBDD(idTask);
                                const std::list<long>& seriesId = pTask->getSeriesId();
                                for (std::list<long>::const_iterator it = seriesId.begin(); it != seriesId.end(); ++it) {
                                        bufSQL.Format("Insert INTO TasksSeries(task_fk, series_fk) VALUES ('%d','%d');",
                                                      idTask, (*it));
                                        m_pConexion->ExecuteUpdate(bufSQL);
                                }
                        }
                        m_pConexion->Commit();
                } catch (wxSQLite3Exception& ex) {
                        LOG_ERROR("BBDD", "Exception adding task "<<ex.GetMessage().ToUTF8())
                        m_pConexion->Rollback();
                }
        }
}

void GNC::GCS::HistoryController::DeleteTasks(const std::list<TaskModel>& tasks)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << "DELETE FROM Tasks WHERE pk IN (";
                for (std::list<TaskModel>::const_iterator it = tasks.begin(); it != tasks.end(); ++it) {
                        if (it != tasks.begin()) {
                                ostr << ",";
                        }
                        ostr << bufSQL.Format(" '%ld'", (*it).pk);
                }
                ostr << ");";
                m_pConexion->ExecuteUpdate(ostr.str().c_str());
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "Exception deleting task "<<ex.GetMessage().ToUTF8())
        }
}

bool GNC::GCS::HistoryController::IsPendingTasks()
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("Select count(*) from Tasks where status = '%d' OR status = '%d'",GNC::GCS::IPersistentCommand::Pending,GNC::GCS::IPersistentCommand::Error);
                if(m_pConexion->ExecuteScalar(bufSQL)>0) {
                        return true;
                } else {
                        return false;
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "ExistsSeriesByUID "<<ex.GetMessage().ToUTF8())
                return false;
        }
}

inline void FillInTaskModel(wxSQLite3ResultSet& resultados, GNC::GCS::HistoryController::TaskModel& taskModel)
{
        if (taskModel.pk <= 0) {
                taskModel.pk = resultados.GetInt64(wxT("pk")).GetValue();
                taskModel.uid = resultados.GetAsString(wxT("uid")).ToUTF8();
                taskModel.status = resultados.GetInt64(wxT("status")).GetValue();
                taskModel.statusDescription = resultados.GetAsString(wxT("error_description")).ToUTF8();
                taskModel.data = resultados.GetAsString(wxT("data")).ToUTF8();
                taskModel.description = resultados.GetAsString(wxT("description")).ToUTF8();
                taskModel.created_time = GetDateTimeFromWx(resultados.GetAsString(wxT("created_time")));
        }
        taskModel.seriesId.push_back(resultados.GetInt(wxT("series_fk")));
}


void GNC::GCS::HistoryController::GetPendingTasks(std::list<GNC::GCS::HistoryController::TaskModel>& tasks)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT Tasks.pk, Tasks.uid, Tasks.status, Tasks.error_description, Tasks.data, TasksSeries.series_fk, "
                              "Tasks.created_time, Tasks.description "
                              "FROM Tasks LEFT OUTER JOIN TasksSeries ON  pk=task_fk "
                              "where status in ('%d', '%d')",GNC::GCS::IPersistentCommand::Pending,GNC::GCS::IPersistentCommand::Error);
                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                std::map<long, TaskModel> mapOfTasks;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInTaskModel(resultados, mapOfTasks[pk]);
                }
                for (std::map<long, TaskModel>::const_iterator it = mapOfTasks.begin(); it != mapOfTasks.end(); ++it) {
                        tasks.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "TaskModel "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::GetSeriesPendingTasks(const std::list<long>& seriesPk, std::list<TaskModel>& tasks)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << bufSQL.Format("SELECT distinct Tasks.pk, Tasks.uid, Tasks.status, Tasks.error_description, Tasks.data, TasksSeries.series_fk, "
                                      "Tasks.created_time, Tasks.description "
                                      "FROM Tasks LEFT OUTER JOIN TasksSeries ON  pk=task_fk "
                                      "where  status in ('%d', '%d') AND series_fk IN (", GNC::GCS::IPersistentCommand::Pending,GNC::GCS::IPersistentCommand::Error);
                for (std::list<long>::const_iterator it = seriesPk.begin(); it != seriesPk.end(); ++it) {
                        if (it != seriesPk.begin()) {
                                ostr << ",";
                        }
                        ostr << bufSQL.Format(" '%ld'", (*it));
                }
                ostr << ");";

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
                std::map<long, TaskModel> mapOfTasks;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInTaskModel(resultados, mapOfTasks[pk]);
                }
                for (std::map<long, TaskModel>::const_iterator it = mapOfTasks.begin(); it != mapOfTasks.end(); ++it) {
                        tasks.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "TaskModel "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::GetSeriesTasks(const std::list<long>& seriesPk, std::list<TaskModel>& tasks)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << "SELECT distinct Tasks.pk, Tasks.uid, Tasks.status, Tasks.error_description, Tasks.data, TasksSeries.series_fk, "
                     "Tasks.created_time, Tasks.description "
                     "FROM Tasks LEFT OUTER JOIN TasksSeries ON  pk=task_fk "
                     "where series_fk IN (";
                for (std::list<long>::const_iterator it = seriesPk.begin(); it != seriesPk.end(); ++it) {
                        if (it != seriesPk.begin()) {
                                ostr << ",";
                        }
                        ostr << bufSQL.Format(" '%ld'", (*it));
                }
                ostr << ")";
                ostr << " ORDER BY created_time ASC";

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
                std::map<long, TaskModel> mapOfTasks;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInTaskModel(resultados, mapOfTasks[pk]);
                }
                for (std::map<long, TaskModel>::const_iterator it = mapOfTasks.begin(); it != mapOfTasks.end(); ++it) {
                        tasks.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "TaskModel "<<ex.GetMessage().ToUTF8());
        }
}

void GNC::GCS::HistoryController::GetAllTasks(std::list<TaskModel>& tasks)
{
        try {
                wxSQLite3StatementBuffer bufSQL;
                std::ostringstream ostr;
                ostr << "SELECT distinct Tasks.pk, Tasks.uid, Tasks.status, Tasks.error_description, Tasks.data, TasksSeries.series_fk, "
                     "Tasks.created_time, Tasks.description "
                     "FROM Tasks LEFT OUTER JOIN TasksSeries ON  pk=task_fk "
                     "ORDER BY created_time ASC";

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(ostr.str().c_str());
                std::map<long, TaskModel> mapOfTasks;
                while (resultados.NextRow()) {
                        long pk = resultados.GetInt64(wxT("pk")).GetValue();
                        FillInTaskModel(resultados, mapOfTasks[pk]);
                }
                for (std::map<long, TaskModel>::const_iterator it = mapOfTasks.begin(); it != mapOfTasks.end(); ++it) {
                        tasks.push_back((*it).second);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "TaskModel "<<ex.GetMessage().ToUTF8());
        }
}

GNC::GCS::HistoryController::TaskModel GNC::GCS::HistoryController::GetTask(long idTask)
{
        GNC::GCS::HistoryController::TaskModel taskModel;
        try {
                wxSQLite3StatementBuffer bufSQL;
                bufSQL.Format("SELECT distinct Tasks.pk, Tasks.uid, Tasks.status, Tasks.error_description, Tasks.data, TasksSeries.series_fk, "
                              "Tasks.created_time, Tasks.description "
                              "FROM Tasks LEFT OUTER JOIN TasksSeries ON  pk=task_fk "
                              "where pk ='%d'", idTask);

                wxSQLite3ResultSet resultados = m_pConexion->ExecuteQuery(bufSQL);
                while (resultados.NextRow()) {
                        FillInTaskModel(resultados, taskModel);
                }
        } catch (wxSQLite3Exception& ex) {
                LOG_ERROR("BBDD", "TaskModel "<<ex.GetMessage().ToUTF8());
        }
        return taskModel;
}
