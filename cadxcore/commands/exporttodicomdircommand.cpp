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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4996)
#endif


#include <dcmtk/dcmdata/dcdicdir.h>
#include <dcmtk/dcmdata/dcdeftag.h>

#include <sstream>

#include <wx/filename.h>
#include <wx/file.h>
#include <wx/dir.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/dicommanager.h>
#include <main/controllers/controladorlog.h>
#include <main/entorno.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/dicom/idicommanager.h>

#include <main/entorno.h>
#include <main/controllers/controladoreventos.h>

#include <eventos/mensajes.h>

#include "exporttodicomdircommand.h"


#define IDC_EXPORT_TO_DICOMDIR           118

GADAPI::ExportToDicomDirCommandParams::ExportToDicomDirCommandParams(const TListOfPks& seriesList, const std::string& destinationPath, const GIL::DICOM::DicomDataset& anonymized, bool includeGinkgo)
{
        m_seriesList = seriesList;
        m_destinationPath = destinationPath;
        m_hasError = false;
        m_anonymized = anonymized;
        m_includeGinkgoTags = includeGinkgo;
}

GADAPI::ExportToDicomDirCommandParams::~ExportToDicomDirCommandParams()
{
}

GADAPI::ExportToDicomDirCommand::ExportToDicomDirCommand(ExportToDicomDirCommandParams* pParams): GNC::GCS::IComando(pParams,"Export Dicom Dir")
{
        SetId(IDC_EXPORT_TO_DICOMDIR);
        m_pExportacionParams = pParams;
}

inline std::string GetRightPath(const std::string& uid, const std::string& prefix, std::map<std::string, int>& mapOfUids, int& index)
{
        std::string pathOfUid;
        if (mapOfUids.find(uid) == mapOfUids.end()) {
                mapOfUids[uid] = index;
                std::ostringstream ostr;
                ostr << prefix << index++;
                pathOfUid = ostr.str();
        } else {
                std::ostringstream ostr;
                ostr << prefix << mapOfUids[uid];
                pathOfUid = ostr.str();
        }
        return pathOfUid;
}

inline bool CreateDir(const wxString& path)
{
        if (!wxDirExists(path)) {
                if (!wxFileName::Mkdir(path)) {
                        return false;
                }
        }
        return true;
}

inline bool CreatePathOfSeries(const GNC::GCS::IHistoryController::StudyModel& studyModel,
                               const GNC::GCS::IHistoryController::SeriesModel& seriesModel,
                               std::map<std::string, int>& mapOfPathPatients,
                               std::map<std::string, int>& mapOfPathStudies,
                               std::map<std::string, int>& mapOfPathSeries,
                               int& patientIndex,
                               int& studyIndex,
                               int& seriesIndex,
                               const std::string& destinationPath,
                               std::string& pathOfPatient,
                               std::string& pathOfStudy,
                               std::string& pathOfSeries,
                               wxString& fullPath)
{
        pathOfPatient = GetRightPath(studyModel.pat_id, "PT", mapOfPathPatients, patientIndex);
        pathOfStudy = GetRightPath(studyModel.study_iuid, "ST", mapOfPathStudies, studyIndex);
        pathOfSeries = GetRightPath(seriesModel.series_iuid, "SE", mapOfPathSeries, seriesIndex);

        fullPath = FROMPATH(destinationPath) + wxFileName::GetPathSeparator() + wxT("DICOM");
        if (!CreateDir(fullPath)) {
                return false;
        }
        fullPath += wxFileName::GetPathSeparator() + wxString::FromUTF8(pathOfPatient.c_str());
        if (!CreateDir(fullPath)) {
                return false;
        }
        fullPath +=	wxFileName::GetPathSeparator() + wxString::FromUTF8(pathOfStudy.c_str());
        if (!CreateDir(fullPath)) {
                return false;
        }
        fullPath += wxFileName::GetPathSeparator() + wxString::FromUTF8(pathOfSeries.c_str());
        if (!CreateDir(fullPath)) {
                return false;
        }
        return true;
}

inline void InsertTagRecord(const DcmTagKey& key, const std::string& value, DcmDirectoryRecord* record)
{
        DcmTag tag(key);
        DcmElement* e = newDicomElement(tag);
        e->putString(value.c_str());
        record->insert(e, true, false);
}

inline DcmDirectoryRecord* GetSeriesRecord(const GNC::GCS::IHistoryController::StudyModel& studyModel,
                const GNC::GCS::IHistoryController::SeriesModel& seriesModel,
                std::map<std::string, DcmDirectoryRecord*>& dcmMapOfPatients,
                std::map<std::string, DcmDirectoryRecord*>& dcmMapOfStudies,
                std::map<std::string, DcmDirectoryRecord*>& dcmMapOfSeries,
                DcmDirectoryRecord * root)
{
        DcmDirectoryRecord* PatientRecord;
        if (dcmMapOfPatients.find(studyModel.pat_id) == dcmMapOfPatients.end()) {
                PatientRecord = new DcmDirectoryRecord();
                dcmMapOfPatients[studyModel.pat_id] = PatientRecord;
                root->insertSub(PatientRecord);
                InsertTagRecord(DCM_DirectoryRecordType, "PATIENT", PatientRecord);
                InsertTagRecord(DCM_SpecificCharacterSet, "ISO_IR 192", PatientRecord);
                InsertTagRecord(DCM_PatientID, studyModel.pat_id, PatientRecord);
                InsertTagRecord(DCM_PatientName, studyModel.pat_name, PatientRecord);
                {
                        std::ostringstream ostr;
                        ostr << studyModel.pat_sex;
                        InsertTagRecord(DCM_PatientSex, ostr.str(), PatientRecord);
                }
                InsertTagRecord(DCM_PatientBirthDate, studyModel.pat_bithdate, PatientRecord);
        } else {
                PatientRecord = dcmMapOfPatients[studyModel.pat_id];
        }

        DcmDirectoryRecord* StudyRecord;
        if (dcmMapOfStudies.find(studyModel.study_iuid) == dcmMapOfStudies.end()) {
                StudyRecord = new DcmDirectoryRecord();
                dcmMapOfStudies[studyModel.study_iuid] = StudyRecord;
                PatientRecord->insertSub(StudyRecord);
                InsertTagRecord(DCM_DirectoryRecordType, "STUDY", StudyRecord);
                InsertTagRecord(DCM_SpecificCharacterSet, "ISO_IR 192", StudyRecord);
                InsertTagRecord(DCM_StudyInstanceUID, studyModel.study_iuid, StudyRecord);
                InsertTagRecord(DCM_StudyDescription, studyModel.study_desc, StudyRecord);
                wxDateTime date;
                date.ParseFormat(wxString::FromUTF8(studyModel.study_datetime.c_str()), wxT("%Y-%m-%dT%H:%M:%S"));
                if (date.IsValid()) {
                        std::string tmp(date.Format(wxT("%Y%m%d")).ToUTF8());
                        InsertTagRecord(DCM_StudyDate, tmp, StudyRecord);
                        tmp = date.Format(wxT("%H%M%S.%l")).ToUTF8();
                        InsertTagRecord(DCM_StudyTime, tmp, StudyRecord);
                }
                InsertTagRecord(DCM_AccessionNumber, studyModel.study_acc_no, StudyRecord);
                InsertTagRecord(DCM_StudyID, studyModel.study_id, StudyRecord);
        } else {
                StudyRecord = dcmMapOfStudies[studyModel.study_iuid];
        }

        DcmDirectoryRecord* SeriesRecord;
        if (dcmMapOfSeries.find(studyModel.study_iuid) == dcmMapOfSeries.end()) {
                SeriesRecord = new DcmDirectoryRecord();
                dcmMapOfSeries[seriesModel.series_iuid] = SeriesRecord;
                StudyRecord->insertSub(SeriesRecord);
                InsertTagRecord(DCM_DirectoryRecordType, "SERIES", SeriesRecord);
                InsertTagRecord(DCM_SpecificCharacterSet, "ISO_IR 192", SeriesRecord);
                InsertTagRecord(DCM_SeriesInstanceUID, seriesModel.series_iuid, SeriesRecord);
                InsertTagRecord(DCM_SeriesDescription, seriesModel.series_desc, SeriesRecord);
                wxDateTime date;
                date.ParseFormat(wxString::FromUTF8(seriesModel.series_datetime.c_str()), wxT("%Y-%m-%dT%H:%M:%S"));
                if (date.IsValid()) {
                        std::string tmp(date.Format(wxT("%Y%m%d")).ToUTF8());
                        InsertTagRecord(DCM_SeriesDate, tmp, SeriesRecord);
                        tmp = date.Format(wxT("%H%M%S.%l")).ToUTF8();
                        InsertTagRecord(DCM_SeriesTime,  tmp, SeriesRecord);
                }
                InsertTagRecord(DCM_Modality, seriesModel.series_modality, SeriesRecord);
                InsertTagRecord(DCM_SeriesNumber, seriesModel.series_no, SeriesRecord);
        } else {
                SeriesRecord = dcmMapOfSeries[seriesModel.series_iuid];
        }

        return SeriesRecord;
}

void GADAPI::ExportToDicomDirCommand::Execute()
{
        if (!NotificarProgreso(0.0, _Std("Exporting files...")) )
                return;

        std::string pathOfDicomDir;
        {
                std::ostringstream ostr;
                ostr << m_pExportacionParams->m_destinationPath;
                ostr << (char)wxFileName::GetPathSeparator();
                ostr << "DICOMDIR";
                pathOfDicomDir = ostr.str();
        }

        //clean dicomdir file and dicomdir directory if present...
        {
                std::ostringstream ostr;
                ostr << m_pExportacionParams->m_destinationPath;
                ostr << (char) wxFileName::GetPathSeparator();
                ostr << "DICOM";
                GNC::Entorno::Instance()->RemoveDirRecursive(ostr.str());
                wxRemoveFile(FROMPATH(pathOfDicomDir));
        }
        //

        std::map<std::string, int> mapOfPathPatients;
        std::map<std::string, int> mapOfPathStudies;
        std::map<std::string, int> mapOfPathSeries;
        std::map<std::string, int> mapOfPathImages;

        std::map<std::string, DcmDirectoryRecord*> dcmMapOfPatients;
        std::map<std::string, DcmDirectoryRecord*> dcmMapOfStudies;
        std::map<std::string, DcmDirectoryRecord*> dcmMapOfSeries;

        std::string pathOfPatient, pathOfStudy, pathOfSeries;

        int patientIndex=0, studyIndex=0, seriesIndex=0;

        int numberOfSeries = m_pExportacionParams->m_seriesList.size();
        int actualSerie = 1;

        DcmDicomDir dicomDir(pathOfDicomDir.c_str(), "GINKGOCADXCD");
        DcmDirectoryRecord *   root = &(dicomDir.getRootRecord());

        //FIRST OF ALL EXPORT WITHOUT ANONYMIZE
        if (m_pExportacionParams->m_seriesList.size() > 0 && m_pExportacionParams->m_anonymized.tags.empty() && m_pExportacionParams->m_includeGinkgoTags) {
                for (ExportToDicomDirCommandParams::TListOfPks::const_iterator itUids = m_pExportacionParams->m_seriesList.begin(); itUids != m_pExportacionParams->m_seriesList.end(); ++itUids) {
                        wxString cadena = wxString::Format(_("Exporting series %d of %d"), (int)(actualSerie++), (int)(numberOfSeries));
                        if (!NotificarProgreso((float)actualSerie/numberOfSeries, std::string(cadena.ToUTF8())))
                                return;
                        //find source paths and series model
                        GNC::GCS::IHistoryController::SeriesModel seriesModel = GNC::GCS::HistoryController::Instance()->GetSeriesModel((*itUids));
                        GNC::GCS::IHistoryController::StudyModel studyModel = GNC::GCS::HistoryController::Instance()->GetStudyModel(seriesModel.study_fk);
                        GNC::GCS::IHistoryController::FileModelList fileModels;
                        GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*itUids), fileModels);

                        if (fileModels.empty()) {
                                m_pExportacionParams->m_Error = _Std("Some of selected series has been deleted");
                                m_pExportacionParams->m_hasError = true;
                                return;
                        }

                        //get path of series
                        wxString fullPathWx;
                        if (!CreatePathOfSeries(studyModel, seriesModel, mapOfPathPatients, mapOfPathStudies, mapOfPathSeries, patientIndex, studyIndex, seriesIndex, m_pExportacionParams->m_destinationPath, pathOfPatient, pathOfStudy, pathOfSeries, fullPathWx) ) {
                                m_pExportacionParams->m_Error = _Std("There was an error creating directory");
                                m_pExportacionParams->m_hasError = true;
                                return;
                        }

                        //dicomdir structure
                        DcmDirectoryRecord* SeriesRecord = GetSeriesRecord(studyModel, seriesModel, dcmMapOfPatients, dcmMapOfStudies, dcmMapOfSeries, root);

                        //path is created, now we are going to copy images...
                        mapOfPathImages[seriesModel.series_iuid] = 0;
                        for (GNC::GCS::IHistoryController::FileModelList::const_iterator itDCMModels = fileModels.begin(); itDCMModels != fileModels.end(); ++itDCMModels) {
                                std::string pathOfImage;
                                {
                                        std::ostringstream ostr;
                                        ostr << "IM";
                                        ostr << mapOfPathImages[seriesModel.series_iuid]++;
                                        pathOfImage = ostr.str();
                                }

                                wxString pathOfImageWx = fullPathWx + wxFileName::GetPathSeparator() + wxString::FromUTF8(pathOfImage.c_str());
                                std::string absolutepathFile = (*itDCMModels).real_path;
                                if (!wxCopyFile(FROMPATH(absolutepathFile),  pathOfImageWx)) {
                                        LOG_ERROR("ExportToDicomDirCommand", "Error copying file " << absolutepathFile << " TO " << pathOfImageWx.ToUTF8());
                                        m_pExportacionParams->m_Error = _Std("There was an error writing file");
                                        m_pExportacionParams->m_hasError = true;
                                        return;
                                }
                                DcmDirectoryRecord* ImageRecord = new DcmDirectoryRecord();
                                SeriesRecord->insertSub(ImageRecord);
                                std::string fileId;
                                {
                                        std::ostringstream ostr;
                                        ostr << "DICOM" << "\\" << pathOfPatient << "\\" << pathOfStudy << "\\" << pathOfSeries << "\\" << pathOfImage;
                                        fileId = ostr.str();
                                }
                                InsertTagRecord(DCM_DirectoryRecordType, "IMAGE", ImageRecord);
                                InsertTagRecord(DCM_ReferencedSOPInstanceUIDInFile, (*itDCMModels).sopiuid, ImageRecord);
                                InsertTagRecord(DCM_ReferencedTransferSyntaxUIDInFile, (*itDCMModels).tsuid, ImageRecord);
                                InsertTagRecord(DCM_ReferencedSOPClassUIDInFile, (*itDCMModels).sopcuid, ImageRecord);
                                InsertTagRecord(DCM_SpecificCharacterSet, "ISO_IR 192", ImageRecord);
                                InsertTagRecord(DCM_ImageComments, (*itDCMModels).file_desc, ImageRecord);
                                std::string instanceNumber;
                                {
                                        std::ostringstream ostr;
                                        ostr << (*itDCMModels).instance_number;
                                        instanceNumber = ostr.str();
                                }
                                InsertTagRecord(DCM_InstanceNumber, instanceNumber, ImageRecord);
                                InsertTagRecord(DCM_ReferencedFileID, fileId, ImageRecord);
                        }
                }
        }///END EXPORT WITHOUT ANONYMIZE
        std::string m_TmpDir = GNC::Entorno::Instance()->CrearDirectorioTemporal();
        //export series anonymizing...
        if (m_pExportacionParams->m_seriesList.size() > 0 && (!m_pExportacionParams->m_anonymized.tags.empty() || !m_pExportacionParams->m_includeGinkgoTags)) {
                for (ExportToDicomDirCommandParams::TListOfPks::const_iterator itUids = m_pExportacionParams->m_seriesList.begin(); itUids != m_pExportacionParams->m_seriesList.end(); ++itUids) {
                        wxString cadena = wxString::Format(_("Exporting series %d of %d"), (int)(actualSerie++), (int)(numberOfSeries));
                        if (!NotificarProgreso((float)actualSerie/numberOfSeries, std::string(cadena.ToUTF8())))
                                return;
                        //find source paths and series model
                        GNC::GCS::IHistoryController::FileModelList fileModels;
                        GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*itUids), fileModels);

                        for (GNC::GCS::IHistoryController::FileModelList::const_iterator itFileModel = fileModels.begin(); itFileModel != fileModels.end(); ++itFileModel) {
                                GIL::DICOM::DICOMManager manager;
                                manager.CargarFichero((*itFileModel).real_path);
                                manager.ActualizarJerarquia(m_pExportacionParams->m_anonymized);
                                if (!m_pExportacionParams->m_includeGinkgoTags) {
                                        manager.AnonimizarTagsPrivados();
                                }
                                wxString targetFile = FROMPATH(m_TmpDir) + wxFileName::GetPathSeparator() + wxString::Format(wxT("%d"), (int)(rand()));
                                while (wxFileExists(targetFile)) {
                                        targetFile = FROMPATH(m_TmpDir) + wxFileName::GetPathSeparator() + wxString::Format(wxT("%d"), (int)(rand()));
                                }
                                std::string targetFileStd(TOPATH(targetFile));
                                if (!manager.AlmacenarFichero(targetFileStd)) {
                                        m_pExportacionParams->m_Error = _Std("There was an error anonymizing files");
                                        m_pExportacionParams->m_hasError = true;
                                        return;
                                }
                        }
                }
        }
        //read tmp directory and insert into dcmdir...
        {
                wxString tmpDirWx = FROMPATH(m_TmpDir);
                wxDir dir;
                if (dir.Open(tmpDirWx)) {
                        wxString fileName;
                        bool cont = dir.GetFirst(&fileName);
                        while (cont) {
                                fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
                                const std::string fileNameStd(TOPATH(fileName));
                                GNC::GCS::IHistoryController::DICOMFileModel dicomFile;
                                GNC::GCS::IHistoryController::TAddErrorList foo;
                                if (!GNC::GCS::HistoryController::Instance()->ReadFile(dicomFile, fileNameStd, foo)) {
                                        LOG_ERROR("ExportDICOMDir", "error reading " << fileNameStd);
                                        continue;
                                }

                                //create path
                                wxString fullPathWx;
                                if (!CreatePathOfSeries(dicomFile.study, dicomFile.series, mapOfPathPatients, mapOfPathStudies, mapOfPathSeries, patientIndex, studyIndex, seriesIndex, m_pExportacionParams->m_destinationPath, pathOfPatient, pathOfStudy, pathOfSeries, fullPathWx) ) {
                                        m_pExportacionParams->m_Error = _Std("There was an error creating directory");
                                        m_pExportacionParams->m_hasError = true;
                                        return;
                                }

                                //dicomdir structure
                                DcmDirectoryRecord* SeriesRecord = GetSeriesRecord(dicomFile.study, dicomFile.series, dcmMapOfPatients, dcmMapOfStudies, dcmMapOfSeries, root);

                                //path is created, now we are going to copy images...
                                if (mapOfPathImages.find(dicomFile.series.series_iuid) == mapOfPathImages.end()) {
                                        mapOfPathImages[dicomFile.series.series_iuid] = 0;
                                }
                                std::string pathOfImage;
                                {
                                        std::ostringstream ostr;
                                        ostr << "IM";
                                        ostr << mapOfPathImages[dicomFile.series.series_iuid]++;
                                        pathOfImage = ostr.str();
                                }

                                wxString pathOfImageWx = fullPathWx + wxFileName::GetPathSeparator() + wxString::FromUTF8(pathOfImage.c_str());
                                //moving tmp files...
                                if (!wxRenameFile(fileName, pathOfImageWx)) {
                                        m_pExportacionParams->m_Error = _Std("There was an error writing file");
                                        m_pExportacionParams->m_hasError = true;
                                        return;
                                }

                                DcmDirectoryRecord* ImageRecord = new DcmDirectoryRecord();
                                SeriesRecord->insertSub(ImageRecord);
                                std::string fileId;
                                {
                                        std::ostringstream ostr;
                                        ostr << "DICOM" << "\\" << pathOfPatient << "\\" << pathOfStudy << "\\" << pathOfSeries << "\\" << pathOfImage;
                                        fileId = ostr.str();
                                }
                                InsertTagRecord(DCM_ReferencedFileID, fileId, ImageRecord);
                                InsertTagRecord(DCM_DirectoryRecordType, "IMAGE", ImageRecord);
                                InsertTagRecord(DCM_ReferencedSOPInstanceUIDInFile, dicomFile.file.sopiuid, ImageRecord);
                                InsertTagRecord(DCM_ReferencedTransferSyntaxUIDInFile, dicomFile.file.tsuid, ImageRecord);
                                InsertTagRecord(DCM_ReferencedSOPClassUIDInFile, dicomFile.file.sopcuid, ImageRecord);
                                {
                                        std::ostringstream ostr;
                                        ostr << dicomFile.file.instance_number;
                                        InsertTagRecord(DCM_InstanceNumber, ostr.str(), ImageRecord);
                                }
                                InsertTagRecord(DCM_ImageComments, dicomFile.file.file_desc, ImageRecord);
                                InsertTagRecord(DCM_SpecificCharacterSet, "ISO_IR 192", ImageRecord);
                                cont = dir.GetNext(&fileName);
                        }
                }

        }
        OFCondition cond = dicomDir.write();
        std::cout << cond.text() << std::endl;
}



void GADAPI::ExportToDicomDirCommand::Update()
{
        if (m_pExportacionParams->m_hasError) {
                GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,m_pExportacionParams->m_Error, GNC::GCS::Events::EventoMensajes::StatusMessage,true, GNC::GCS::Events::EventoMensajes::Error));
        } else {
                GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL, _Std("Export has been finished successfully"), GNC::GCS::Events::EventoMensajes::StatusMessage,true, GNC::GCS::Events::EventoMensajes::Informacion));
        }
}

//endregion


#ifdef _MSC_VER
#pragma warning(pop)
#endif

