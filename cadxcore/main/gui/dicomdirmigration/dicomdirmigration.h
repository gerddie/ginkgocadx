/*
 *  
 *  $Id: dicomdirmigration.h $
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
#pragma once

#include "dicomdirmigrationbase.h"

namespace GNC {
	namespace GUI {
		class DicomDirMigrationDialog: public GNC::GUI::DicomDirMigrationDialogBase
		{
		public:
			typedef enum TStatus {
				TS_CloseAll,
				TS_Migrate,
				TS_Change
			} TStatus;
			DicomDirMigrationDialog(wxWindow* pParent, const std::string& dicomDirPath, int version, bool oldVersion);
			~DicomDirMigrationDialog();

			TStatus GetStatus();
			std::string GetDicomDirPath();
			std::string GetOriginalDicomDirPath();
			bool GetRemoveAfterMigration();

		protected:
			virtual void OnSelectOther(wxCommandEvent& event);
			virtual void OnMigrate(wxCommandEvent& event);
			wxString SelectFolder();
			std::string m_DicomDirPath;
			std::string m_OriginalDicomDirPath;
			int FromVersion;
			TStatus Status;			
		};
	}
}