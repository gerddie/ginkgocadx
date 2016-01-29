/*
 *  
 *  $Id: selectimagesimportation.h $
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
#include <api/api.h>
#include <api/iwizard.h>
#include <main/gui/import/pasosimportacionbase.h>
#include <api/controllers/ipacscontroller.h>
#include <api/ientorno.h>
#include <api/controllers/imodulecontroller.h>
#include <api/autoptr.h>
#include <string>
#include <vector>


class wxGinkgoToolBar;

namespace GIL
{
	class IModeloIntegracion;
}

namespace GNC {
	namespace GUI {
		class TimerDirectory;
		class DropTargetSelectImages;

		class EXTAPI SelectImagesImportation: public SelectImagesImportationBase, public IPasoWizard
		{
			public:
				SelectImagesImportation(wxWindow* pParent, const std::string& dirTemp, IWizard* pWizard, GNC::GCS::Ptr<GIL::IModeloIntegracion>& pModeloIntegracion, const std::list<std::string>& fileSpec = std::list<std::string>(), const wxString& defaultFilter = wxEmptyString);
				void SetDefaultImage(const wxImage& defaultImage);
				~SelectImagesImportation();
				
				virtual void AddImages(const wxArrayString& files);
				virtual bool ScanPath(const wxString& path);
			
			protected:
				bool                                    m_firstAttach;
				std::string                             m_dirTemp;
				GNC::GCS::Ptr<GIL::IModeloIntegracion>  m_pModeloIntegracion;
				wxGinkgoToolBar*                        m_pButtonsBar;
				wxString                                m_pDefaultFilter;
				wxImage                                 m_pDefaultImage;
				TimerDirectory*                         m_pTimerDirectory;
				DropTargetSelectImages*                 m_pDropTarget;
				std::list<std::string>                  m_fileSpecs;
				wxImage*                                m_pwxImagePreview;

				virtual wxArrayString GetSelectedPaths();
				virtual void OnPasteFromClipboard(wxCommandEvent& event );
				virtual void OnSelectFiles(wxCommandEvent &event);

		public:
				virtual void AddImage(const wxString& path);
		protected:
				virtual void OnPreviewClick(wxCommandEvent &event);
				virtual void OnThumbLeftDClickSelected(wxCommandEvent& event);
				virtual void OnThumbRightClickSelected(wxCommandEvent& event);				
				virtual void ShowPreview();
				virtual void OnHidePreview(wxCommandEvent& event);
				virtual void HidePreview();
				virtual void OnRemoveAll(wxCommandEvent &event);
				virtual void OnImagenesKeyDown(wxKeyEvent &event);
				virtual void OnDeleteSelectedThumbnails(wxCommandEvent &event);
				virtual void DeleteSelectedThumbnails();

				virtual void UnCheckAll();
				virtual void OnUbicacionClick(wxCommandEvent& event);
				virtual void SetUbicacion(GIL::Ubicacion* ubicacion, int id);
				GIL::Ubicacion* GetUbicacionSelected();

			//region "Metodos heredados de IPasoWizard"
				virtual bool Attach();

				virtual bool Detach();

				virtual std::string GetTitle();

				virtual std::string GetSubTitle();

				virtual bool Siguiente();

				virtual bool Anterior();

				virtual bool Cancelar();

				virtual void OnCancelled();

				virtual bool Validar();
			//endregion

		};
	};
};
