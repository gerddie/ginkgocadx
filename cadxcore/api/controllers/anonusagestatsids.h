/*
 *  
 *  $Id: anonusagestatsids.h $
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

/**Ids use to send anonymous statistics about usage of the app*/

//viewers usage
#define ANON_STAT_VIEW_OS "VIEW_OS"
#define ANON_STAT_VIEW_ECG "VIEW_ECG"

//dicomizers usage
#define ANON_STAT_DICOMIZE "DICOMIZE_"

//file acquisition usage
#define ANON_STAT_OPEN_FILE "OPEN_FILE"
#define ANON_STAT_OPEN_FOLDER "OPEN_FOLDER"
#define ANON_STAT_OPEN_CD "OPEN_CD"
#define ANON_STAT_OPEN_CLOUD "OPEN_CLOUD"

//system usage
#define ANON_STAT_RUN_GINKGO "RUN_GINKGO"

//other options usage
#define ANON_STAT_DLG_CLOUD_SEARCH "DLG_CLOUD_SEARCH"
#define ANON_STAT_DLG_PACS_SEARCH "DLG_PACS_SEARCH"
#define ANON_STAT_DLG_PACS_UPLOAD "DLG_PACS_UPLOAD"
#define ANON_STAT_DLG_HISTORY "DLG_HISTORY"
#define ANON_STAT_DLG_EXPORT "DLG_EXPORT"
#define ANON_STAT_DLG_DICOM_DIR_EXPORT "DLG_DICOM_DIR_EXPORT"

//integration usage
#define ANON_STAT_INT_URL "INT_URL"
#define ANON_STAT_INT_DICOMIZE "INT_DICOMIZE"
#define ANON_STAT_INT_RETRIEVE "INT_RETRIEVE"
#define ANON_STAT_INT_OPEN "INT_OPEN"

//license acceptance
#define ANON_STAT_ACCEPT_LICENSE "ACCEPT_LICENSE"