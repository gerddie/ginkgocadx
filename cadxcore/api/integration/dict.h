/*
*  
*  $Id: dict.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once

//GinkgoCADx Integration dictionary

#define GKDI_GINKGOCADX_MODULE_UID                   "ginkgocadx:module.uid"
#define GKDI_GINKGOCADX_MODULE_NAME                  "ginkgocadx:module.name"
#define GKDI_GINKGOCADX_MODULE_TID                   "ginkgocadx:module.tid"

#define GKDI_GINKGOCADX_CONF_RECEIVING_ORG_ID        "ginkgocadx.conf:receiving.organization.id"
#define GKDI_GINKGOCADX_CONF_PETITIONER_SYS_ID       "ginkgocadx.conf:petitioner.system.id"
#define GKDI_GINKGOCADX_CONF_FILLER_APP_ID           "ginkgocadx.conf:filler.application.id"
#define GKDI_GINKGOCADX_CONF_ORGANIZATION_NAME       "ginkgocadx.conf:organization.name"
#define GKDI_GINKGOCADX_CONF_ORGANIZATION_ID         "ginkgocadx.conf:organization.id"
#define GKDI_GINKGOCADX_CONF_GINKGO_APP_ID           "ginkgocadx.conf:gingocadx.application.id"

#define GKDI_MESSAGE_HL7_TIMESTAMP                   "message:timestamp.hl7"
#define GKDI_MESSAGE_EPOCH_TIMESTAMP                 "message:timestamp.epoch"
#define GKDI_MESSAGE_PLACER                          "message:placer.id" // Custom Placer ID
#define GKDI_MESSAGE_FILLER                          "message:filler.id" // Custom Filler ID

#define GKDI_GLOBAL_PACS_STORE_SID                   "global:pacs.store.sid"
#define GKDI_GLOBAL_PACS_RETRIEVE_SID                "global:pacs.retrieve.sid"
#define GKDI_GLOBAL_OBSERVATIONS                     "global:observations"
#define GKDI_GLOBAL_APPLICATION_CODE                 "global:application.code"
#define GKDI_GLOBAL_PACS_LOCAL_AET                   "global:pacs.local.aet"

#define GKDI_HCE_PETITION_ID                         "hce:petition.id"
#define GKDI_HCE_EPISODE_NUMBER                      "hce:episode.number"
#define GKDI_HCE_ID                                  "hce:id"

#define GKDI_JIMENA_AMBITO_PETICION                  "jimena:ambito.peticion"

#define GKDI_PATIENT_ID                              "patient:id"
#define GKDI_PATIENT_ID_ASSIGNING                    "patient:issuer.of.patient.id"
#define GKDI_PATIENT_NAME                            "patient:name"
#define GKDI_PATIENT_FAMILY_NAME                     "patient:family.name"
#define GKDI_PATIENT_SECOND_FAMILY_NAME              "patient:family.second.name"
#define GKDI_PATIENT_SEX                             "patient:sex"
#define GKDI_PATIENT_BIRTH_DATE                      "patient:birth.date"

#define GKDI_REFERRING_PHISICIAN_ID                  "referring.physician:id"
#define GKDI_REFERRING_PHISICIAN_NAME                "referring.physician:name"
#define GKDI_REFERRING_PHISICIAN_FAMILY_NAME         "referring.physician:family.name"
#define GKDI_REFERRING_PHISICIAN_SECOND_FAMILY_NAME  "referring.physician:family.second.name"
#define GKDI_REFERRING_PHISICIAN_INSTITUTION_ID      "referring.physician:institution.id"
#define GKDI_REFERRING_PHISICIAN_INSTITUTION_NAME    "referring.physician:institution.name"

#define GKID_STUDY_IDX                               "study:idx"
#define GKDI_STUDY_ACCESSION_NUMBER                  "study:accession.number"
#define GKDI_STUDY_INSTANCE_UID                      "study:instance.uid"
#define GKDI_STUDY_DESCRIPTION                       "study:description"

#define GKID_SERIE_IDX                               "serie:idx"
#define GKDI_SERIE_INSTANCE_UID                      "serie:instance.uid"
#define GKDI_SERIE_DESCRIPTION                       "serie:description"

#define GKID_IMAGE_IDX                               "image:idx"
#define GKDI_IMAGE_SOP_INSTANCE_UID                  "image:sop.instance.uid"
#define GKDI_IMAGE_SOP_CLASS_UID                     "image:sop.class.uid"
#define GKDI_IMAGE_DICOM_PATH                        "image:file.target.path"