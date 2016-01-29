/*
*
*  $Id: iwindowlevelcontract.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iwindowlevelcontract.h"
#include <api/dicom/dicomdataset.h>
#include <api/internationalization/internationalization.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <main/controllers/modalitysettingscontroller.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <api/istudycontext.h>

GNC::GCS::IWindowLevelContract::WindowLevel::WindowLevel() {
	m_label = std::string("");
	m_window = 0;
	m_level = 0;
	m_type = TWLT_USER_DEFINED;
}


GNC::GCS::IWindowLevelContract::WindowLevel::WindowLevel(const std::string& label, const double& window,const double& level, TWL_TYPE type){
	m_label = label;
	m_window = window;
	m_level = level;
	m_type = type;
}

GNC::GCS::IWindowLevelContract::WindowLevel::WindowLevel(const WindowLevel& otro) {
	m_window = otro.m_window;
	m_level = otro.m_level;
	m_label = otro.m_label;
	m_type = otro.m_type;
}

GNC::GCS::IWindowLevelContract::WindowLevel& GNC::GCS::IWindowLevelContract::WindowLevel::operator=(const WindowLevel& otro) {
	m_label = otro.m_label;
	m_window = otro.m_window;
	m_level = otro.m_level;
	m_type = otro.m_type;
	return *this;
}

GNC::GCS::IWindowLevelContract::IWindowLevelContract() : IWidgetsContract() {
}

GNC::GCS::IWindowLevelContract::~IWindowLevelContract(){
}

bool GNC::GCS::IWindowLevelContract::HasCurrentWindowLevel()
{
	return !GetWindowLevelList().empty();
}

void GNC::GCS::IWindowLevelContract::SetUserDefinedWindowLevel(double window, double level) {
	if (GetWindowLevelList().size() == 0 || (GetWindowLevelList().size() > 0 && GetWindowLevelList().back().m_type != WindowLevel::TWLT_USER_DEFINED)) {
		GetWindowLevelList().push_back(WindowLevel("USER_DEFINED", window, level, WindowLevel::TWLT_USER_DEFINED));
	} else {
		GetWindowLevelList().back().m_window = window;
		GetWindowLevelList().back().m_level = level;
	}
	SetLabelCurrent(GetWindowLevelList().back().m_label);
}

GNC::GCS::IWindowLevelContract::WindowLevel GNC::GCS::IWindowLevelContract::GetCurrentWindowLevel()
{
	for (ListaWindowLevels::const_iterator it = GetWindowLevelList().begin(); it != GetWindowLevelList().end(); ++it)
	{
		if ((*it).m_label == GetLabelCurrent()) {
			return (*it);
		}
	}
	if (GetWindowLevelList().size() > 0) {
		SetLabelCurrent(GetWindowLevelList().front().m_label);
		return GetWindowLevelList().front();
	} else {
		WindowLevel wl("",0,0,WindowLevel::TWLT_USER_DEFINED);
		return wl;
	}
}

bool GNC::GCS::IWindowLevelContract::HasDefaultWindowLevel()
{
	bool resultado = false;
	for (ListaWindowLevels::const_iterator it = GetWindowLevelList().begin(); it != GetWindowLevelList().end(); ++it)
	{
		resultado = resultado || (*it).m_type != WindowLevel::TWLT_USER_DEFINED;
	}
	return resultado;
}

GNC::GCS::IWindowLevelContract::WindowLevel GNC::GCS::IWindowLevelContract::GetAndSetDefaultWindowLevel()
{
	for (ListaWindowLevels::const_iterator it = GetWindowLevelList().begin(); it != GetWindowLevelList().end(); ++it)
	{
		if ((*it).m_type != WindowLevel::TWLT_USER_DEFINED) {
			SetLabelCurrent((*it).m_label);
			return (*it);
		}
	}
	WindowLevel wl("",0,0,WindowLevel::TWLT_USER_DEFINED);
	return wl;
}

void GNC::GCS::IWindowLevelContract::RemoveUserDefined() {
	ListaWindowLevels listaTemp;
	for (ListaWindowLevels::iterator it = GetWindowLevelList().begin(); it !=  GetWindowLevelList().end(); ++it)
	{
		if ((*it).m_type != WindowLevel::TWLT_USER_DEFINED) {
			listaTemp.push_back((*it));
		}
	}
	GetWindowLevelList() = listaTemp;
}

void GNC::GCS::IWindowLevelContract::Setup(const GIL::DICOM::DicomDataset& base) 
{
	//se guardan los user defined
	ListaWindowLevels listaTemp;
	for (ListaWindowLevels::iterator it = GetWindowLevelList().begin(); it !=  GetWindowLevelList().end(); ++it)
	{
		if ((*it).m_type == WindowLevel::TWLT_USER_DEFINED) {
			listaTemp.push_back((*it));
		}
	}
	GetWindowLevelList().clear();
	//se pillan los window levels
	double level = 0.0f;
	double window = 0.0f;
	//it's necessary because if there isn't current window level you have to set in this order:
	//	if current is set current
	//	wl of the hanging protocol
	//	wl of the file
	std::string labelCurrent;
	std::string label("");

	std::string levelStr;
	std::string windowStr;
	std::string labelStr;
	std::string modality;

	base.getTag(std::string("0028|1050"),levelStr);
	base.getTag(std::string("0028|1051"),windowStr);
	base.getTag(std::string("0028|1055"),labelStr);
	base.getTag(std::string("0008|0060"),modality);

	bool wlValido = true;

	if ( levelStr.empty() || windowStr.empty() ){
		wlValido = false;
	}
	else {
		while(levelStr.size()>0){
			std::istringstream issl(levelStr);
			issl >> level;

			std::istringstream issw(windowStr);
			issw >> window;

			if (window == 0.0f) {
				wlValido = false;
			}

			if(labelStr.find_first_of('\\')!=std::string::npos){
				label = labelStr.substr(0,labelStr.find_first_of('\\'));
			} else if(labelStr.size()>0) {
				label = labelStr;
			} else {
				std::ostringstream ostr;
				ostr << "WindowLevel" << GetWindowLevelList().size();
				label = ostr.str();
			}

			if(wlValido){
				if (labelCurrent.empty()) {
					labelCurrent = label;
				}
				WindowLevel wl(label,window,level, WindowLevel::TWLT_FILE);
				GetWindowLevelList().push_back(wl);
			}


			if(levelStr.find_first_of('\\')!=std::string::npos){
				levelStr = levelStr.substr(levelStr.find_first_of('\\')+1,levelStr.size()-1);
				windowStr = windowStr.substr(windowStr.find_first_of('\\')+1,windowStr.size()-1);
				if(labelStr.find_first_of('\\')!=std::string::npos){
					labelStr = labelStr.substr(labelStr.find_first_of('\\')+1,labelStr.size()-1);
				}
			} else {
				break;
			}
		}
	}

	GNC::GCS::Ptr<DefaultModalitySettings> ptr = GNC::GCS::ModalitySettingsController::Instance()->getModalitySettings(modality);
	if (ptr.IsValid()) {
		for (GNC::GCS::DefaultModalitySettings::TListOfWLSettings::const_iterator it  = ptr->getWLSettings().begin(); it != ptr->getWLSettings().end(); ++it)
		{
			GetWindowLevelList().push_back(WindowLevel((*it)->getLabel(), (*it)->getWindow(), (*it)->getLevel(), WindowLevel::TWLT_PREDEFINED));
		}
		if (ptr->getInitialWindowLevelLabel().compare(GNC::GCS::DefaultModalitySettings::getDefaultWindowLevelLabel()) != 0) {
			if (ptr->getInitialWindowLevelLabel().compare(GNC::GCS::DefaultModalitySettings::getAutocalculateWindowLevelLabel()) == 0) {
				if (GetViewerActivo() != NULL) {
					DoAutoCalculateWindowLevel();
				}
			} else {
				labelCurrent = ptr->getInitialWindowLevelLabel();
			}
		}
	}
	
	
	//se meten si es necesario los user defined
	for (ListaWindowLevels::iterator it = listaTemp.begin(); it !=  listaTemp.end(); ++it)
	{
		GetWindowLevelList().push_back((*it));
	}
	//sets current...
	if (GetLabelCurrent().empty()) {
		SetLabelCurrent(labelCurrent);
	}
}

double GNC::GCS::IWindowLevelContract::GetCurrentWindow()
{
	if (GetViewerActivo() != NULL) {
		return GetViewerActivo()->GetWindow();
	} else {
		return 0;
	}
}

double GNC::GCS::IWindowLevelContract::GetCurrentLevel()
{
	if (GetViewerActivo() != NULL) {
		return GetViewerActivo()->GetLevel();
	} else {
		return 0;
	}
}
