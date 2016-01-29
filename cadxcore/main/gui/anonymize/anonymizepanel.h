/*
 *  
 *  $Id: anonymizedialog.h $
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
#include "anonymizepanelbase.h"

#include <api/autoptr.h>
#include <api/dicom/idicom.h>
#include <api/internationalization/internationalization.h>


namespace GNC {
	namespace GUI {
		class AnonymizePanel: public AnonymizePanelBase
		{
		public:
			AnonymizePanel(wxWindow* pParent);
			AnonymizePanel(wxWindow* pParent, const std::list<long>& seriesPk);
			~AnonymizePanel();
			
			void GetTagsAnonymized(GIL::DICOM::DicomDataset & base);
			bool AnonymizePrivateTags();
		protected:
			void InitSeries(long pkSeries);
			void SetTags(GIL::DICOM::DicomDataset & base,wxPGProperty* idPadre,GIL::DICOM::IDICOMManager*	pDICOMManager);
			virtual void OnCheckNombreApellidos( wxCommandEvent& event );
			virtual void OnCheckIdPaciente( wxCommandEvent& event );
			virtual void OnCheckInstituciones( wxCommandEvent& event );
			virtual void OnCheckPhysician( wxCommandEvent& event );
			virtual void OnCheckComentarios( wxCommandEvent& event );
			virtual void OnAddNewTag( wxCommandEvent &evt);

			void Anonymize(const std::string& clave, bool anonimizar);
			void Anonymize(const std::string& clave, bool anonimizar, const std::string& value, bool anonymizeIfNotExists = true);
			void RemoveFromTable(const std::string& clave);

			void OnPropertyChanged(wxPropertyGridEvent& event);
			void OnPropertyDobleClick(wxPropertyGridEvent& event);

			GIL::DICOM::DicomDataset m_base;
			std::map <std::string, wxCheckBox*> MapOfCheck;
		};
	}
}
