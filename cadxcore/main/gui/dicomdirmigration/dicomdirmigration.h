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

#pragma once

#include "dicomdirmigrationbase.h"

namespace GNC
{
namespace GUI
{
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
