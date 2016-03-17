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
#include "anonymizepanelbase.h"

#include <api/autoptr.h>
#include <api/dicom/idicom.h>
#include <api/internationalization/internationalization.h>


namespace GNC
{
namespace GUI
{
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
