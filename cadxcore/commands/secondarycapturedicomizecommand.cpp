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

#include "secondarycapturedicomizecommand.h"
#include <api/ivista.h>
#include <api/controllers/icontroladorlog.h>
#include <api/ientorno.h>
#include <api/dicom/dcmdictionary.h>
#include <export/tagsprivadoscomunes.h>
#include <main/controllers/dicommanager.h>
#include <main/controllers/controladoreventos.h>
#include <api/integration/dict.h>
#include <api/internationalization/internationalization.h>
#include <eventos/scgenerated.h>

#include <wx/datetime.h>

GADAPI::SecondaryCaptureDicomizeCommandParameters::SecondaryCaptureDicomizeCommandParameters(GNC::GCS::IVista* pView, const std::string& screenShot) : GADAPI::DicomizeCommandParams(GNC::GCS::Ptr<GNC::GUI::ImportationData>()),
        m_pView(pView),
        m_screenShot(screenShot)
{
        m_originalImagePath = m_pView->GetCurrentDICOM();
        m_openAfterDicomize = false;
}

GADAPI::SecondaryCaptureDicomizeCommand::SecondaryCaptureDicomizeCommand(::GADAPI::SecondaryCaptureDicomizeCommandParameters* pParams) : ::GADAPI::DicomizeCommand(pParams),
        m_pSecondaryParams(pParams)
{
}

void GADAPI::SecondaryCaptureDicomizeCommand::Update()
{
        DicomizeCommand::Update();

        //send an event to the view and add current series...
        GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::SCGeneratedEvent(m_pSecondaryParams->m_pView, m_pIntegracionParams->m_InsertedSeries));
}

bool GADAPI::SecondaryCaptureDicomizeCommand::ImportarFicherosOriginales()
{
        //fill in parameters...
        GIL::DICOM::DICOMManager manager;
        GIL::DICOM::DicomDataset base;
        manager.CargarFichero(m_pSecondaryParams->m_originalImagePath, base);

        GNC::GCS::Ptr<GNC::GUI::ImportationData>& ImportData = m_pSecondaryParams->m_pImportData;
        //patient module atributes
        ImportData->baseImages.tags[GKDCM_PatientName] = base.getTag(GKDCM_PatientName);
        ImportData->baseImages.tags[GKDCM_PatientID] = base.getTag(GKDCM_PatientID);
        ImportData->baseImages.tags[GKDCM_PatientBirthDate] = base.getTag(GKDCM_PatientBirthDate);
        ImportData->baseImages.tags[GKDCM_PatientSex] = base.getTag(GKDCM_PatientSex);
        ImportData->baseImages.tags[GKDCM_IssuerOfPatientID] = base.getTag(GKDCM_IssuerOfPatientID);
        //study module atributes
        ImportData->baseImages.tags[GKDCM_StudyInstanceUID] = base.getTag(GKDCM_StudyInstanceUID);
        ImportData->baseImages.tags[GKDCM_StudyDate] = base.getTag(GKDCM_StudyDate);
        ImportData->baseImages.tags[GKDCM_StudyTime] = base.getTag(GKDCM_StudyTime);
        ImportData->baseImages.tags[GKDCM_StudyDescription] = base.getTag(GKDCM_StudyDescription);
        ImportData->baseImages.tags[GKDCM_ReferringPhysicianName] = base.getTag(GKDCM_ReferringPhysicianName);
        ImportData->baseImages.tags[GKDCM_StudyID] = base.getTag(GKDCM_StudyID);
        ImportData->baseImages.tags[GKDCM_AccessionNumber] = base.getTag(GKDCM_AccessionNumber);
        //series module attributes
        ImportData->baseImages.tags[GKDCM_Modality] = "SC";
        ImportData->baseImages.tags[GKDCM_SeriesDescription] = _Std("Ginkgo CADx Secondary Capture");
        ImportData->baseImages.tags[GKDCM_Laterality] = base.getTag(GKDCM_Laterality);
        wxDateTime date = wxDateTime::Now();
        ImportData->baseImages.tags[GKDCM_InstanceCreationDate] = ImportData->baseImages.tags[GKDCM_SeriesDate] =  std::string(date.Format(wxT("%Y%m%d")).ToUTF8());
        ImportData->baseImages.tags[GKDCM_InstanceCreationTime] = ImportData->baseImages.tags[GKDCM_SeriesTime] =std::string(date.Format(wxT("%H%M%S")).ToUTF8());

        //sc module
        ImportData->baseImages.tags[GKDCM_ConversionType] = "DI";

        //fill in image file
        ImportData->m_SourceFiles.push_back(m_pSecondaryParams->m_screenShot);
        //call original operation!
        return GADAPI::DicomizeCommand::ImportarFicherosOriginales();
}

//on update add to view!
