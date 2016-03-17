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
#include "paneltagsbase.h"
#include <api/controllers/ipacscontroller.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class PanelCustomPASI;

namespace GNC
{
namespace GUI
{

class PanelTags : public PanelTagsBase
{
public:
        PanelTags(wxWindow* pPadre, bool hideOnClose = true);
        virtual ~PanelTags();

        virtual bool IsVisiblePanelTags();
        virtual void MostrarOcultarPanelTags();
        virtual void SetView(GNC::GCS::IVista* pView);
        virtual bool SetSeriesPk(long pk);

private:
        virtual void OnClose(wxCloseEvent &event);
        virtual void OnKeyDown(wxKeyEvent &event);

        void CreateCategories();
        wxPGProperty* CreatePropertyGroup(const wxString& label);
        void InsertTagInGroup(wxPGProperty* prop, const std::string& key, wxPGPropArg idPadre);
        void SetInfoTags(GIL::DICOM::TipoMetaInfo& info, GIL::DICOM::IDICOMManager*	pDICOMManager);
        void SetTags(GIL::DICOM::DicomDataset& base, wxPGPropArg idPadre, GIL::DICOM::IDICOMManager*	pDICOMManager);

        wxPGProperty* metadataGroup;
        wxPGProperty* identificationGroup;
        wxPGProperty* patientGroup;
        wxPGProperty* acquisitionGroup;
        wxPGProperty* relationshipGroup;
        wxPGProperty* imagePresentationGroup;
        wxPGProperty* textGroup;
        wxPGProperty* otherGroup;
        std::string m_currentPath;
        bool m_hideOnClose;
};
}
}
