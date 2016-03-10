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

#include <list>
#include <map>
#include <cstring>

#include "idicomconformance.h"
#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/ientorno.h>
#include <api/controllers/icontroladorlog.h>
#include <main/controllers/configurationcontroller.h>

#include <wx/xml/xml.h>
#include <wx/file.h>
#include <wx/sstream.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>
#include <wx/filename.h>

namespace GIL {
	namespace DICOM {
		static std::string emptyString;
		static TransferSyntaxList emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::TransferSyntaxMap::AddTransferSyntax(const std::string& tsRef, const std::string& tsUID)
{
	TransferSyntaxMapImpl& impl = *this;
	impl[tsRef] = tsUID;
}

const std::string& GIL::DICOM::TransferSyntaxMap::GetTransferSyntaxUID(const std::string& tsRef) const {
	const_iterator it = find(tsRef);
	if (it != end()) {
		return (*it).second;
	}
	else{
		return emptyString;
	}				
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::SOPClassMap::AddSOPClass(const std::string& scRef, const std::string& scUID) {
	SOPClassMapImpl::iterator it = find(scRef);

	if (it == end()) {
		SOPClassMapImpl& impl = *this;
		impl[scRef] = GIL::DICOM::SOPClassTuple(scUID);
	}
}

void GIL::DICOM::SOPClassMap::AddTransferSyntaxForSOP(const std::string& scRef, const std::string& tsRef) {
	SOPClassMapImpl::iterator it = find(scRef);

	if (it != end()) {
		const std::string& tsUID = GIL::DICOM::Conformance::GetTransferSyntaxes().GetTransferSyntaxUID(tsRef);
		if (tsUID.size() != 0) {
			SOPClassTuple& sop = (*it).second;
			sop.tsRefs.push_back(tsRef);
			sop.tsUIDs.push_back(tsUID);
		}
		else {
			LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported transfer syntax ") << tsRef << _Std(" to SOP Class ") << scRef <<_Std(" . Transfer Syntax does not exists in model: Add it first."));			
		}
	}
	else {
		LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported transfer syntax ") << tsRef << _Std(" to SOP Class ") << scRef <<_Std(" . SOP Class does not exists in model: Add it first."));
	}
}

const std::string& GIL::DICOM::SOPClassMap::GetSOPClassUID(const std::string& scRef) const {
	
	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.scUID;
	}
	return emptyString;
}

const GIL::DICOM::TransferSyntaxList& GIL::DICOM::SOPClassMap::GetSupportedTransferSyntaxRefs(const std::string& scRef) const {

	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.tsRefs;
	}
	else {
		return emptyTSList;
	}
}

const GIL::DICOM::TransferSyntaxList& GIL::DICOM::SOPClassMap::GetSupportedTransferSyntaxUIDs(const std::string& scRef) const {

	SOPClassMapImpl::const_iterator it = find(scRef);
	if (it != end()) {
		return (*it).second.tsUIDs;
	}
	else {
		return emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

void GIL::DICOM::ModalityMap::AddModality(const std::string& mRef, const std::string& descr) {
	ModalityMapImpl::iterator it = find(mRef);

	if (it == end()) {
		ModalityMapImpl& impl = *this;
		impl[mRef] = GIL::DICOM::ModalityTuple(descr);
	}
}

void GIL::DICOM::ModalityMap::AddSOPClassForModality(const std::string& mRef, const std::string& scRef) {
	ModalityMapImpl::iterator it = find(mRef);

	if (it != end()) {
		std::string scUID = GIL::DICOM::Conformance::GetStorageSOPClasses().GetSOPClassUID(scRef);		
		if (scUID.size() == 0) {
			scUID = GIL::DICOM::Conformance::GetScuSOPClasses().GetSOPClassUID(scRef);
			if (scUID.size() == 0) {
				scUID = GIL::DICOM::Conformance::GetScpSOPClasses().GetSOPClassUID(scRef);				
			}
		}
		if (scUID.size() == 0) {
			LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported SOP Class ") << scRef << _Std(" to Modality ") << mRef <<_Std(" . SOP Class does not exists in model: Add it first."));
		}
		else {
			ModalityTuple& modality = (*it).second;
			modality.scRefs.push_back(scRef);
			modality.scUIDs.push_back(scUID);
		}
	}
	else {
		LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Unable to add supported SOP Class ") << scRef << _Std(" to Modality ") << mRef <<_Std(" . Modality does not exists in model: Add it first."));
	}
}

const std::string& GIL::DICOM::ModalityMap::GetDescription(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.descr;
	}
	return emptyString;
}

/** Gets the SOP Classes names (refs) list supported by this Modality **/
const GIL::DICOM::SOPClassList& GIL::DICOM::ModalityMap::GetSupportedSOPClassRefs(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.scRefs;
	}
	else {
		return emptyTSList;
	}

}
			
/** Gets the SOP Classes oid list supported by this Modality **/
const GIL::DICOM::SOPClassList& GIL::DICOM::ModalityMap::GetSupportedSOPClassUIDs(const std::string& mRef) const {

	ModalityMapImpl::const_iterator it = find(mRef);
	if (it != end()) {
		return (*it).second.scUIDs;
	}
	else {
		return emptyTSList;
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------


GIL::DICOM::TransferSyntaxMap GIL::DICOM::Conformance::m_TransferSyntaxes;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_StorageSOPClasses;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_ScuSOPClasses;
GIL::DICOM::SOPClassMap GIL::DICOM::Conformance::m_ScpSOPClasses;
GIL::DICOM::ModalityMap GIL::DICOM::Conformance::m_Modalities;
bool GIL::DICOM::Conformance::m_Loaded = false;

#define CONFORMANCE_FILE wxT("conformance.xml")
#define CONFORMANCE_ROOT_TAG wxT("dicom-conformance")


void GIL::DICOM::Conformance::Load() {
	
	m_Loaded = true;
	
	wxFileName cpath = FROMPATH(GNC::GCS::IEntorno::Instance()->GetGinkgoResourcesDir()) + wxFileName::GetPathSeparator() + CONFORMANCE_FILE;

	wxXmlDocument xmldoc;
	xmldoc.Load(cpath.GetFullPath());
	if (!xmldoc.IsOk()) {
		LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Error loading DICOM Conformance file" ));
		m_Loaded = false;
		return;
	}
	wxXmlNode* pRoot = xmldoc.GetRoot();
	if (pRoot->GetName().CmpNoCase(CONFORMANCE_ROOT_TAG) != 0) {
		LOG_ERROR(_Std("GIL/DICOM/Conformance"), _Std("Invalid DICOM Conformance file" ));
		m_Loaded = false;
		return;
	}

	for (wxXmlNode* entry = pRoot->GetChildren(); entry != NULL; entry = entry->GetNext())
	{
		
		if (entry->GetName().CmpNoCase(wxT("transfer-syntaxes")) == 0) {
			// Loading Transfer Syntaxes
			wxString wxTS_RefId;
			wxString wxTS_UID;
			for (wxXmlNode* tsentry = entry->GetChildren(); tsentry != NULL; tsentry = tsentry->GetNext()) {
				
				if ((tsentry->GetName().CmpNoCase(wxT("transfer-syntax")) == 0) && tsentry->GetAttribute(wxT("ref-id"), &wxTS_RefId) && tsentry->GetAttribute(wxT("uid"), &wxTS_UID)) {
					m_TransferSyntaxes.AddTransferSyntax(std::string(wxTS_RefId.ToUTF8()), std::string(wxTS_UID.ToUTF8()));
				}
			}
		}
		
		else if (entry->GetName().CmpNoCase(wxT("storage-sop-classes")) == 0) {
			// Loading Storage SOP Classes
			wxString wxSC_RefId;
			wxString wxSC_UID;
			wxString wxTS_Ref;

			std::string sc_RefId;

			for (wxXmlNode* scentry = entry->GetChildren(); scentry != NULL; scentry = scentry->GetNext()) {
				
				if ( (scentry->GetName().CmpNoCase(wxT("sop-class")) == 0) && scentry->GetAttribute(wxT("ref-id"), &wxSC_RefId) && scentry->GetAttribute(wxT("uid"), &wxSC_UID)) {
					
					sc_RefId = std::string(wxSC_RefId.ToUTF8());
					m_StorageSOPClasses.AddSOPClass( sc_RefId, std::string(wxSC_UID.ToUTF8()) );

					for (wxXmlNode* tsentry = scentry->GetChildren(); tsentry != NULL; tsentry = tsentry->GetNext()) {

						if ((tsentry->GetName().CmpNoCase(wxT("allowed-transfer-syntax")) == 0) && tsentry->GetAttribute(wxT("ref"), &wxTS_Ref)) {
							m_StorageSOPClasses.AddTransferSyntaxForSOP( sc_RefId, std::string(wxTS_Ref.ToUTF8()) );
						}
					}
				}
			}
		}
		else if (entry->GetName().CmpNoCase(wxT("scu-sop-classes")) == 0) {
			// Loading SCU SOP Classes
			wxString wxSC_RefId;
			wxString wxSC_UID;
			wxString wxTS_Ref;

			std::string sc_RefId;

			for (wxXmlNode* scentry = entry->GetChildren(); scentry != NULL; scentry = scentry->GetNext()) {
				
				if ( (scentry->GetName().CmpNoCase(wxT("sop-class")) == 0) && scentry->GetAttribute(wxT("ref-id"), &wxSC_RefId) && scentry->GetAttribute(wxT("uid"), &wxSC_UID)) {
					
					sc_RefId = std::string(wxSC_RefId.ToUTF8());
					m_ScuSOPClasses.AddSOPClass( sc_RefId, std::string(wxSC_UID.ToUTF8()) );

					for (wxXmlNode* tsentry = scentry->GetChildren(); tsentry != NULL; tsentry = tsentry->GetNext()) {

						if ((tsentry->GetName().CmpNoCase(wxT("allowed-transfer-syntax")) == 0) && tsentry->GetAttribute(wxT("ref"), &wxTS_Ref)) {
							m_ScuSOPClasses.AddTransferSyntaxForSOP( sc_RefId, std::string(wxTS_Ref.ToUTF8()) );
						}
					}
				}
				else if (scentry->GetName().CmpNoCase(wxT("include-storage-sop-classes")) == 0) {
					for (SOPClassMap::iterator it = m_StorageSOPClasses.begin(); it != m_StorageSOPClasses.end(); ++it) {
						const std::string& storageSOPRef = (*it).first;
						SOPClassTuple& storageSOPTuple = (*it).second;
						m_ScuSOPClasses[storageSOPRef] = storageSOPTuple;
					}
				}
			}
		}
		else if (entry->GetName().CmpNoCase(wxT("scp-sop-classes")) == 0) {
			// Loading SCP SOP Classes
			wxString wxSC_RefId;
			wxString wxSC_UID;
			wxString wxTS_Ref;

			std::string sc_RefId;

			for (wxXmlNode* scentry = entry->GetChildren(); scentry != NULL; scentry = scentry->GetNext()) {
				
				if ( (scentry->GetName().CmpNoCase(wxT("sop-class")) == 0) && scentry->GetAttribute(wxT("ref-id"), &wxSC_RefId) && scentry->GetAttribute(wxT("uid"), &wxSC_UID)) {
					
					sc_RefId = std::string(wxSC_RefId.ToUTF8());
					m_ScpSOPClasses.AddSOPClass( sc_RefId, std::string(wxSC_UID.ToUTF8()) );

					for (wxXmlNode* tsentry = scentry->GetChildren(); tsentry != NULL; tsentry = tsentry->GetNext()) {

						if ((tsentry->GetName().CmpNoCase(wxT("allowed-transfer-syntax")) == 0) && tsentry->GetAttribute(wxT("ref"), &wxTS_Ref)) {
							m_ScpSOPClasses.AddTransferSyntaxForSOP( sc_RefId, std::string(wxTS_Ref.ToUTF8()) );
						}
					}
				}
				else if (scentry->GetName().CmpNoCase(wxT("include-storage-sop-classes")) == 0) {
					for (SOPClassMap::iterator it = m_StorageSOPClasses.begin(); it != m_StorageSOPClasses.end(); ++it) {
						const std::string& storageSOPRef = (*it).first;
						SOPClassTuple& storageSOPTuple = (*it).second;
						m_ScpSOPClasses[storageSOPRef] = storageSOPTuple;
					}
				}
			}
		}
		else if (entry->GetName().CmpNoCase(wxT("modalities-map")) == 0) {
			// Loading Modalities
			wxString wxM_RefId;
			wxString wxM_Descr;
			wxString wxSC_Ref;

			std::string m_RefId;

			for (wxXmlNode* mentry = entry->GetChildren(); mentry != NULL; mentry = mentry->GetNext()) {
				
				if ( (mentry->GetName().CmpNoCase(wxT("modality")) == 0) && mentry->GetAttribute(wxT("ref-id"), &wxM_RefId)) {
					mentry->GetAttribute(wxT("descr"), &wxM_Descr);
					
					m_RefId = std::string(wxM_RefId.ToUTF8());
					m_Modalities.AddModality( m_RefId, std::string(wxM_Descr.ToUTF8()) );

					for (wxXmlNode* scentry = mentry->GetChildren(); scentry != NULL; scentry = scentry->GetNext()) {

						if ((scentry->GetName().CmpNoCase(wxT("allowed-sop-class")) == 0) && scentry->GetAttribute(wxT("ref"), &wxSC_Ref)) {
							m_Modalities.AddSOPClassForModality( m_RefId, std::string(wxSC_Ref.ToUTF8()) );
						}
					}
				}
			}
		}
	}
	return;
}

const GIL::DICOM::TransferSyntaxMap& GIL::DICOM::Conformance::GetTransferSyntaxes() {
	if (!m_Loaded) {
		Load();
	}
	return m_TransferSyntaxes;
}

const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetStorageSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_StorageSOPClasses;
}

const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetScuSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_ScuSOPClasses;
}


const GIL::DICOM::SOPClassMap& GIL::DICOM::Conformance::GetScpSOPClasses() {
	if (!m_Loaded) {
		Load();
	}
	return m_ScpSOPClasses;
}

const GIL::DICOM::ModalityMap& GIL::DICOM::Conformance::GetModalities() {
	if (!m_Loaded) {
		Load();
	}
	return m_Modalities;
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------

GIL::DICOM::ArrayHelper::ArrayHelper()
{
	array = NULL;
	size = 0;
}

GIL::DICOM::ArrayHelper::ArrayHelper(const std::list<std::string>& list)
{	
	array = NULL;
	size = 0;
	copyFrom(list);
}

GIL::DICOM::ArrayHelper::ArrayHelper(const ArrayHelper& o)
{
    this->size = o.size;
    this->array = new const char* [this->size];
    for (unsigned int i = 0; i < this->size; i++) {
        this->array[i] = strdup(o.array[i]);
    }
}

void GIL::DICOM::ArrayHelper::swap(ArrayHelper& other)
{
    std::swap(array, other.array);
    std::swap(size, other.size);
}

GIL::DICOM::ArrayHelper::~ArrayHelper()
{
    this->free();
}

GIL::DICOM::ArrayHelper& GIL::DICOM::ArrayHelper::operator=(const GIL::DICOM::ArrayHelper& o)
{
    if (this != &o) {
        ArrayHelper help(o);
        swap(help);
    }
    return *this;
}

GIL::DICOM::ArrayHelper& GIL::DICOM::ArrayHelper::operator=(const std::list<std::string>& list)
{
	copyFrom(list);
	return *this;
}

void GIL::DICOM::ArrayHelper::copyFrom(const std::list<std::string>& list)
{
	free();
	this->size = list.size();
	this->array = new const char* [this->size];
	unsigned int i = 0;
    for (auto it = list.begin(); it != list.end(); ++it) {
        this->array[i++] = strdup(it->c_str());
	}
}
void GIL::DICOM::ArrayHelper::free()
{
	if (this->size != 0) {
		for (unsigned int i = 0; i < this->size; i++) {
			delete[] this->array[i];
		}
		this->size = 0;
	}
	if (this->array != NULL) {
		delete[] this->array;
		this->array = NULL;
	}					
}
