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

#include "modalitysettingscontroller.h"
#include <limits> 
#include <iomanip>
#include <iostream>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/dicommanager.h>
#include <api/dicom/dicomdataset.h>
#include <api/dicom/dcmdictionary.h>
#include <api/ivista.h>
#include <api/internationalization/internationalization.h>
#include <main/gui/mainwindow/ventanaprincipal.h>
#include <main/entorno.h>

#include <wx/xml/xml.h>
#include <wx/mstream.h>
#include <wx/sstream.h>
#include <wx/regex.h>
#include <wx/sizer.h>
#include <wx/display.h>

#define MS_DOCUMENT_ROOT wxT("modality-settings")

#define PROPERTY_NODE wxT("property")
#define PROPERTY_KEY wxT("key")
#define PROPERTY_VALUE wxT("value")

#define DMS_ROOT wxT("def-mod-settings")
#define DMS_ATT_MODALITY wxT("modality")
#define DMS_ATT_INITIAL_WL wxT("initial-wl")
#define DMS_ATT_INTERPOLATION wxT("interpolation-mode")
#define DMS_ATT_INT_NEAREST wxT("nearest-neighbour")
#define DMS_ATT_LINEAR wxT("linear")
#define DMS_ATT_CUBIC wxT("cubic")

#define WLS_ROOT wxT("window-levels")
#define WL_ROOT wxT("window-level")
#define WL_ATT_WINDOW wxT("window")
#define WL_ATT_LEVEL wxT("level")
#define WL_ATT_LABEL wxT("label")

#define AS_ROOT wxT("annotation-settings")
#define AN_NODE wxT("annotation")
#define AN_ATT_POS wxT("position")
#define AN_ATT_VALUE wxT("value")

/////////////////////////////////////////////////////////////
GNC::GCS::AnnotationSettings::AnnotationSettings()
{
	//default annotation settings...
	{
		/*
		Paciente: ${0010|0020:?}                   // {PatientId}
		Id Paciente: ${0010|0010:?}                // {PatientName}
		*/
		std::ostringstream ostr;
		ostr << _Std("Patient") << ": ${0010|0010:?}${NEWLINE}" << _Std("Patient ID") << ": ${0010|0020:?}";
		annotation[TP_TopLeft] = ostr.str();
	}
	{
		/*
		${0010|0040:?} ${0010|0030} ${0010,1010} // {Sex} {BirthDate} {PatientAge}
		${0010|1030:?} Kg ${0010|1020:?} m.      // {PatientWeight} {PatientSize}
		${0008|1030}                             // {Study Description}
		${0008|0020} ${0008|0030}                 // {StudyDate} {StudyTime}
		${0008|103E}                             // {SeriesDescription}
		Modalidad: ${0008|0060}                  // {Modality}
		*/
		std::ostringstream ostr;
		ostr << "${0010|0040:?} ${0010|0030::date} ${0010,1010}${NEWLINE}${0010|1030:?} Kg ${0010|1020:?} m.${NEWLINE}${0008|1030}${NEWLINE}${0008|0020::date} ${0008|0030::time}${NEWLINE}${0008|103e}${NEWLINE}" << _Std("Modality") << ": ${0008|0060}";
		annotation[TP_TopRight] = ostr.str();
	}
	{
		/*
		Imagen: ${SLICE.HEIGHT} x ${SLICE.WIDTH}
		${SLICE.WL.FULLTEXT}
		Corte: ${SLICE.IDX} de ${SLICE.COUNT}
		Volumen: ${VOLUME.IDX}
		${ANATOMICAL.REGION}${SLICE.VOXEL.SIZE}
		*/
		std::ostringstream ostr;
		ostr << _Std("Dimensions") << ": ${SLICE.WIDTH} x ${SLICE.HEIGHT}${NEWLINE}${SLICE.WL.FULLTEXT}${NEWLINE}${SLICE.IDX.FULLTEXT}${NEWLINE}${SLICE.THICKNESS.FULLTEXT}${NEWLINE}${SLICE.LOCATION.FULLTEXT}${NEWLINE}${SLICE.LATERALITY}${NEWLINE}${SLICE.VOXEL.SIZE}";
		annotation[TP_BottomLeft] = ostr.str();
	}
	{
		/*
		${0008|0021} {0008|0031} // {SeriesDate} {Seriestime}
		${0070|0081}             // {ImageDescription}
		${VIEW.COORDLABELS}
		Hecho con Ginkgo CADx Pro
		*/
		wxString appName ( wxString::FromUTF8(GNC::GCS::IEntorno::Instance()->GetApplicationName().c_str()));
		wxString madewith( wxString::Format(_("Made with %s"), appName.c_str()));

		std::ostringstream ostr;
		ostr << "${0008|0021::date} ${0008|0031::time}${NEWLINE}${0070|0081}${NEWLINE}${VIEW.COORDLABELS}${NEWLINE}" << madewith.ToUTF8();
		annotation[TP_BottomRight] = ostr.str();
	}
}

GNC::GCS::AnnotationSettings::AnnotationSettings(wxXmlNode* wlNode)
{
	for (wxXmlNode* child = wlNode->GetChildren(); child != NULL; child = child->GetNext())
	{
		if (child->GetName().CmpNoCase(AN_NODE) == 0) {
			long position = 0;
			child->GetAttribute(AN_ATT_POS, wxT("0")).ToLong(&position);
			if (position < TP_NumPos)
			{
				annotation[position] = child->GetAttribute(AN_ATT_VALUE, wxEmptyString).ToUTF8();
			}
		} else {
			LOG_ERROR("HangingProtocol", "error deserializing WindowLevelSetting, unknown node " << child->GetName().ToUTF8());
		}
	}
}

GNC::GCS::AnnotationSettings::AnnotationSettings(const AnnotationSettings& other)
{
	for (int i = 0; i < TP_NumPos; ++i) {
		annotation[i] = other.annotation[i];
	}
}

GNC::GCS::AnnotationSettings::~AnnotationSettings()
{
}

wxXmlNode* GNC::GCS::AnnotationSettings::serialize()
{
	wxXmlNode* wlNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, AS_ROOT);
	for (int i = 0; i < TP_NumPos; ++i) {
		wxXmlNode* annotationNode = new wxXmlNode(wlNode, wxXML_ELEMENT_NODE, AN_NODE);
		annotationNode->AddAttribute(AN_ATT_POS, wxString::Format(wxT("%d"), (int) i));
		annotationNode->AddAttribute(AN_ATT_VALUE, wxString::FromUTF8(annotation[i].c_str()));
	}

	return wlNode;
}

std::string GNC::GCS::AnnotationSettings::getAnnotation(TAnnotationPosition pos) const
{
	return annotation[pos];
}

void GNC::GCS::AnnotationSettings::setAnnotation(TAnnotationPosition pos, const std::string& annotation)
{
	this->annotation[pos] = annotation;
}
/////////////////////////////////////////////////////////////
GNC::GCS::WindowLevelSetting::WindowLevelSetting(): window(0), level(127), label(_Std("Default"))
{
}

GNC::GCS::WindowLevelSetting::WindowLevelSetting(double window, double level, const std::string& label)
{
	this->window = window;
	this->level = level;
	this->label = label;
}

GNC::GCS::WindowLevelSetting::WindowLevelSetting(wxXmlNode* wlNode)
{
	wlNode->GetAttribute(WL_ATT_WINDOW, wxEmptyString).ToDouble(&this->window);
	wlNode->GetAttribute(WL_ATT_LEVEL, wxEmptyString).ToDouble(&this->level);
	this->label = wlNode->GetAttribute(WL_ATT_LABEL, wxEmptyString).ToUTF8();
}

GNC::GCS::WindowLevelSetting::WindowLevelSetting(const WindowLevelSetting& other)
{
	window = other.window;
	level = other.level;
	label = other.label;
}

GNC::GCS::WindowLevelSetting::~WindowLevelSetting()
{
}

wxXmlNode* GNC::GCS::WindowLevelSetting::serialize()
{
	wxXmlNode* wlNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, WL_ROOT);
	wlNode->AddAttribute(WL_ATT_WINDOW, wxString::Format(wxT("%lf"), this->window));
	wlNode->AddAttribute(WL_ATT_LEVEL, wxString::Format(wxT("%lf"), this->level));
	wlNode->AddAttribute(WL_ATT_LABEL, wxString::FromUTF8(this->label.c_str()));	

	return wlNode;
}

double GNC::GCS::WindowLevelSetting::getWindow() const
{
	return window;
}

double GNC::GCS::WindowLevelSetting::getLevel() const
{
	return level;
}

const std::string& GNC::GCS::WindowLevelSetting::getLabel() const
{
	return label;
}

std::string GNC::GCS::WindowLevelSetting::toString() const
{
	std::ostringstream ostr;
	ostr << std::setiosflags(std::ios::fixed) << std::setprecision(2);
	ostr << "(";
	ostr << window;
	ostr << "/";
	ostr << level;
	ostr << ")";
	ostr << label;
	return ostr.str();
}

void GNC::GCS::WindowLevelSetting::setWindow(double window)
{
	this->window = window;
}

void GNC::GCS::WindowLevelSetting::setLevel(double level)
{
	this->level = level;
}

void GNC::GCS::WindowLevelSetting::setLabel(const std::string& label)
{
	this->label = label;
}
/////////////////////////////////////////////////////
GNC::GCS::DefaultModalitySettings::DefaultModalitySettings(): annotationSettings(new AnnotationSettings())
{
	this->interpolationMode = TIM_LINEAR;
}

GNC::GCS::DefaultModalitySettings::DefaultModalitySettings(wxXmlNode* defaultSettingsNode): annotationSettings(new AnnotationSettings())
{
	this->modality = defaultSettingsNode->GetAttribute(DMS_ATT_MODALITY, wxEmptyString).ToUTF8();
	this->initialWindowLevelLabel = defaultSettingsNode->GetAttribute(DMS_ATT_INITIAL_WL, wxEmptyString).ToUTF8();
	wxString strInterpolation = defaultSettingsNode->GetAttribute(DMS_ATT_INTERPOLATION, wxEmptyString);
	if (strInterpolation.CompareTo(DMS_ATT_INT_NEAREST) == 0) {
		interpolationMode = TIM_NEAREST_NEIGHBOUR;
	} else if (strInterpolation.CompareTo(DMS_ATT_LINEAR) == 0) {
		interpolationMode = TIM_LINEAR;
	} else if (strInterpolation.CompareTo(DMS_ATT_CUBIC) == 0) {
		interpolationMode = TIM_CUBIC;
	}
	
	for (wxXmlNode* child = defaultSettingsNode->GetChildren(); child != NULL; child = child->GetNext())
	{
		if (child->GetName().CmpNoCase(WLS_ROOT) == 0) {
			//layouts...
			for (wxXmlNode* wlChild = child->GetChildren(); wlChild != NULL; wlChild = wlChild->GetNext()) {
				if (wlChild->GetName().CmpNoCase(WL_ROOT) == 0) {
					GNC::GCS::Ptr<WindowLevelSetting> pWL(new WindowLevelSetting(wlChild));
					this->addWindowLevel(pWL);
				}
			}			
		} else if (child->GetName().CmpNoCase(AS_ROOT) == 0) {
			//annotation settings
			GNC::GCS::Ptr<AnnotationSettings> pAnot(new AnnotationSettings(child));
			this->setAnnotationSettings(pAnot);
		} else {
			LOG_ERROR("HangingProtocol", "error deserializing DefaultModalitySettings, unknown node " << child->GetName().ToUTF8());
		}
	}
}

GNC::GCS::DefaultModalitySettings::DefaultModalitySettings(const DefaultModalitySettings& other)
{
	this->modality = other.modality;
	this->initialWindowLevelLabel = other.initialWindowLevelLabel;
	this->interpolationMode = other.interpolationMode;
	this->annotationSettings = GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>(new GNC::GCS::AnnotationSettings(*other.annotationSettings));
	for (GNC::GCS::DefaultModalitySettings::TListOfWLSettings::const_iterator it = other.listOfWLSettings.begin(); it != other.listOfWLSettings.end(); ++it) {
		if ((*it).IsValid()) {
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> settings(new GNC::GCS::WindowLevelSetting(*(*it)));
			this->listOfWLSettings.push_back(settings);
		}
	}
}

GNC::GCS::DefaultModalitySettings::~DefaultModalitySettings()
{
}

wxXmlNode* GNC::GCS::DefaultModalitySettings::serialize()
{
	wxXmlNode* defaultSettingsNode = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, DMS_ROOT);
	defaultSettingsNode->AddAttribute(DMS_ATT_MODALITY, wxString::FromUTF8(this->modality.c_str()));
	defaultSettingsNode->AddAttribute(DMS_ATT_INITIAL_WL, wxString::FromUTF8(this->initialWindowLevelLabel.c_str()));
	wxString strInterpolation;
	switch (interpolationMode) {
	case TIM_NEAREST_NEIGHBOUR:
		strInterpolation = DMS_ATT_INT_NEAREST;
		break;
	case TIM_LINEAR:
		strInterpolation = DMS_ATT_LINEAR;
		break;
	case TIM_CUBIC:
		strInterpolation = DMS_ATT_CUBIC;
		break;
	}
	defaultSettingsNode->AddAttribute(DMS_ATT_INTERPOLATION, strInterpolation );

	wxXmlNode* wlSettingsNode = new wxXmlNode(defaultSettingsNode, wxXML_ELEMENT_NODE, WLS_ROOT);
	for (GNC::GCS::DefaultModalitySettings::TListOfWLSettings::const_iterator it = listOfWLSettings.begin(); it != listOfWLSettings.end(); ++it) {
		if ((*it).IsValid()) {
			wlSettingsNode->AddChild((*it)->serialize());
		}
	} 

	defaultSettingsNode->AddChild(annotationSettings->serialize());
	
	return defaultSettingsNode;
}

std::string GNC::GCS::DefaultModalitySettings::getDefaultWindowLevelLabel()
{
	return _Std("Default");
}

std::string GNC::GCS::DefaultModalitySettings::getAutocalculateWindowLevelLabel()
{
	return _Std("Autocalculate");
}

const GNC::GCS::DefaultModalitySettings::TListOfWLSettings& GNC::GCS::DefaultModalitySettings::getWLSettings() const
{
	return listOfWLSettings;
}

const std::string& GNC::GCS::DefaultModalitySettings::getModality() const
{
	return modality;
}

const std::string& GNC::GCS::DefaultModalitySettings::getInitialWindowLevelLabel() const
{
	return initialWindowLevelLabel;
}

GNC::GCS::DefaultModalitySettings::TInterpolationMode GNC::GCS::DefaultModalitySettings::getInterpolationMode() const
{
	return interpolationMode;
}

const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& GNC::GCS::DefaultModalitySettings::getAnnotationSettings() const
{
	return annotationSettings;
}

void GNC::GCS::DefaultModalitySettings::setAnnotationSettings(const GNC::GCS::Ptr<GNC::GCS::AnnotationSettings>& pSettings)
{
	if (pSettings.IsValid()) {
		annotationSettings = pSettings;
	}
}

void GNC::GCS::DefaultModalitySettings::addWindowLevel(const GNC::GCS::Ptr<WindowLevelSetting>& wlSetting)
{
	if (wlSetting.IsValid()) {
		listOfWLSettings.push_back(wlSetting);
	}
}

void GNC::GCS::DefaultModalitySettings::clearWindowLevels()
{
	listOfWLSettings.clear();
}

void GNC::GCS::DefaultModalitySettings::setModality(const std::string& modality)
{
	this->modality = modality;
}

void GNC::GCS::DefaultModalitySettings::setInitialWindowLevelLabel(const std::string& label)
{
	this->initialWindowLevelLabel = label;
}

void GNC::GCS::DefaultModalitySettings::setInterpolationMode(TInterpolationMode mode)
{
	this->interpolationMode = mode;
}


/////////////////////////////////////////////////////


/**
Controller for hanging protocols
*/
GNC::GCS::ModalitySettingsController* GNC::GCS::ModalitySettingsController::Instance()
{
	if (m_pInstance == NULL) {
		m_pInstance = new ModalitySettingsController();
	}
	return m_pInstance;
}

void GNC::GCS::ModalitySettingsController::FreeInstance()
{
	if (m_pInstance != NULL) {
		delete m_pInstance;
		m_pInstance = NULL;
	}
}

GNC::GCS::ModalitySettingsController::ModalitySettingsController()
{
	LoadFromConfiguration();
}

GNC::GCS::ModalitySettingsController::~ModalitySettingsController()
{
}
GNC::GCS::ModalitySettingsController* GNC::GCS::ModalitySettingsController::m_pInstance = NULL;

void GNC::GCS::ModalitySettingsController::LoadDefaultModalitySettings()
{
	//basic modality settings....
	if (modalitySettings.find("CT") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("CT");
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(300,35,_Std("SPINAL")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(100,50,_Std("CRANIUM")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(1800,400,_Std("BONE")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(3600,1000,_Std("CRANIUM BONE")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(400,40,_Std("MEDIASTINUM")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(1500,-500,_Std("LUNG")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(2200,200,_Std("BREAST")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(2000,350,_Std("VERTEBRA")));
			modSettings->addWindowLevel(wlSettings);
		}
		addModalitySettings(modSettings);
	}
	if (modalitySettings.find("MR") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("MR");
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(1600,678,_Std("ANGIO")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(150,500,_Std("CT CRANIUM")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(750,400,_Std("T1 CRANIUM")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(563,33,_Std("T2 CRANIUM")));
			modSettings->addWindowLevel(wlSettings);
		}
		{
			GNC::GCS::Ptr<GNC::GCS::WindowLevelSetting> wlSettings(new GNC::GCS::WindowLevelSetting(2030,1245,_Std("KNEE")));
			modSettings->addWindowLevel(wlSettings);
		}
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("CR") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("CR");
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("US") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("US");
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("MG") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("MG");
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("NM") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("NM");
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("RF") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("RF");
		addModalitySettings(modSettings);
	}
	
	if (modalitySettings.find("SC") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("SC");
		addModalitySettings(modSettings);
	}
		
	if (modalitySettings.find("XA") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("XA");
		addModalitySettings(modSettings);
	}
		
	if (modalitySettings.find("OT") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("OT");
		addModalitySettings(modSettings);
	}
		
	if (modalitySettings.find("PT") == modalitySettings.end()) 
	{
		GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> modSettings(new GNC::GCS::DefaultModalitySettings());
		modSettings->setModality("PT");
		addModalitySettings(modSettings);
	}

}

const GNC::GCS::ModalitySettingsController::TMapOfModalitySettings& GNC::GCS::ModalitySettingsController::getModalitySettingsMap()
{
	return this->modalitySettings;
}

void GNC::GCS::ModalitySettingsController::addModalitySettings(const GNC::GCS::Ptr<DefaultModalitySettings>& settings)
{
	if (settings.IsValid()) {
		this->modalitySettings[settings->getModality()] = settings;
	}
}

void GNC::GCS::ModalitySettingsController::clearModalitySettings()
{
	this->modalitySettings.clear();
}

GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> GNC::GCS::ModalitySettingsController::getModalitySettings(const std::string& modality)
{
	GNC::GCS::Ptr<GNC::GCS::DefaultModalitySettings> ptr;
	if (modalitySettings.find(modality) != modalitySettings.end()) {
		ptr = modalitySettings[modality];
	}
	return ptr;
}

void GNC::GCS::ModalitySettingsController::StoreModalitySettingsConfiguration()
{
	//serialization...
	wxXmlDocument doc;
	wxXmlNode* root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE,MS_DOCUMENT_ROOT);
	doc.SetRoot(root);
	for (TMapOfModalitySettings::const_iterator it = this->modalitySettings.begin(); it != this->modalitySettings.end(); ++it)
	{
		root->AddChild((*it).second->serialize());
	}
	wxMemoryOutputStream out;
	doc.SetFileEncoding(wxT("UTF-8"));
	doc.Save(out,0);
	char* vectorWidgets = new char[out.GetLength()];
	out.CopyTo(vectorWidgets,out.GetLength());
	GNC::GCS::ConfigurationController::Instance()->writeStringUser("/GinkgoCore/ModalitySettings","DefaultModalitySettings", std::string(vectorWidgets, out.GetLength()));
	delete[] vectorWidgets;
}


void GNC::GCS::ModalitySettingsController::LoadFromConfiguration()
{
	this->modalitySettings.clear();
	LoadDefaultModalitySettings();
	//Deserialize...
	std::string serializedValue;
	GNC::GCS::ConfigurationController::Instance()->readStringUser("/GinkgoCore/ModalitySettings","DefaultModalitySettings", serializedValue);
	if (!serializedValue.empty()) {
		wxString wxSerializedValue = wxString::FromUTF8(serializedValue.c_str());
		wxStringInputStream istream(wxSerializedValue);
		wxXmlDocument doc;
		if (doc.Load(istream)) {
			wxXmlNode* root = doc.GetRoot();
			if (root != NULL && root->GetName().CmpNoCase(MS_DOCUMENT_ROOT) == 0) {
				for (wxXmlNode* modalitySettingsNode = root->GetChildren(); modalitySettingsNode != NULL; modalitySettingsNode = modalitySettingsNode->GetNext())
				{
					if (modalitySettingsNode->GetName().CmpNoCase(DMS_ROOT) == 0) {
						GNC::GCS::Ptr<DefaultModalitySettings> modalitySettings(new DefaultModalitySettings(modalitySettingsNode));
						addModalitySettings(modalitySettings);
					}
				}
			}
		} else {
			LOG_ERROR("ModalitySettingsController", "Error deserializing modality settings");
		}
	}
}


