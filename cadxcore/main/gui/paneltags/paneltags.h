/*
 *  
 *  $Id: paneltags.h $
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
#include "paneltagsbase.h"
#include <api/controllers/ipacscontroller.h>
#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

class PanelCustomPASI;

namespace GNC {
	namespace GUI {

		class PanelTags : public PanelTagsBase {
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
