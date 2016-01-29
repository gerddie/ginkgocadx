/*
 *  
 *  $Id: thumbnailcontroller.cpp $
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
#include <limits> 
#include <wx/toplevel.h>
#include <wx/display.h>
#include "configurationcontroller.h"
#include "windowattributescontroller.h"
#include "controladorlog.h"

#define ATTRIBUTES_SCOPE "/GinkgoCore/WindowAttributes"
#define WINDOW_ID_KEY "windowId"
#define X_KEY_POSITION "xPos"
#define Y_KEY_POSITION "yPos"
#define WIDTH_KEY_POSITION "width"
#define HEIGH_KEY_POSITION "heigh"
#define IS_MAXIMIZED_KEY "isMaximized"

namespace GNC {
	namespace GCS {
		WindowAttributesController::WindowAttributesController()
		{		
		}
		WindowAttributesController::~WindowAttributesController()
		{			
		}
		
		void WindowAttributesController::LoadWindowAttributes(const std::string& windowId, wxWindow* pWindow, bool defaultMaximized)
		{
			GNC::GCS::IConfigurationController::TListGroups groups;
			GNC::GCS::ConfigurationController::Instance()->readGroupUser(ATTRIBUTES_SCOPE, groups);

			int maxX = 0, maxY = 0, minX=std::numeric_limits<int>::max(), minY=std::numeric_limits<int>::max();
			int deviceCount = wxDisplay::GetCount();
			for (int i = 0; i < deviceCount; ++i) {
				wxDisplay dsply(i);
				maxX = std::max<int>(maxX, dsply.GetClientArea().x + dsply.GetCurrentMode().GetWidth());
				maxY = std::max<int>(maxY, dsply.GetClientArea().y + dsply.GetCurrentMode().GetHeight());
				minX = std::min<int>(minX, dsply.GetClientArea().x);
				minY = std::min<int>(minY, dsply.GetClientArea().y);
			}
			LOG_DEBUG ("WindowAttributes",  "Client area X: " << minX << ", " << maxX << "  Y: " << minY << ", " << maxY << std::endl);
			std::string windowIdTmp;
			for (GNC::GCS::IConfigurationController::TListGroups::iterator it = groups.begin(); it != groups.end(); ++it)
			{
				(*it).readStringValue(WINDOW_ID_KEY, windowIdTmp);
				if (windowId.compare(windowIdTmp) == 0) {
					wxPoint position;
					wxSize size;
					(*it).readIntValue(X_KEY_POSITION, position.x, -1);
					(*it).readIntValue(Y_KEY_POSITION, position.y, -1);
					if (position.x >= (minX - pWindow->GetMinSize().x) && position.x  < maxX 
						&& position.y >= (minY  - pWindow->GetMinSize().y) && position.y < maxY) {
						pWindow->SetPosition(position);
					}
					wxTopLevelWindow* pTopLevel = dynamic_cast<wxTopLevelWindow*>(pWindow);
					if (pTopLevel != NULL) {
						bool isMaximized = defaultMaximized;
						(*it).readBoolValue(IS_MAXIMIZED_KEY, isMaximized);
						pTopLevel->Maximize(isMaximized);
						if (!isMaximized) {
							(*it).readIntValue(WIDTH_KEY_POSITION, size.x, 0);
							(*it).readIntValue(HEIGH_KEY_POSITION, size.y, 0);
						}
					} else {
						(*it).readIntValue(WIDTH_KEY_POSITION, size.x, 0);
						(*it).readIntValue(HEIGH_KEY_POSITION, size.y, 0);
					}
					pWindow->SetSize(size);
				}
			}
		}

		void WindowAttributesController::SaveWindowAttributes(const std::string& windowId, wxWindow* pWindow)
		{
			GNC::GCS::IConfigurationController::TListGroups groups;
			GNC::GCS::ConfigurationController::Instance()->readGroupUser(ATTRIBUTES_SCOPE, groups);
			std::string windowIdTmp;
			for (GNC::GCS::IConfigurationController::TListGroups::iterator it = groups.begin(); it != groups.end(); ++it)
			{
				(*it).readStringValue(WINDOW_ID_KEY, windowIdTmp);
				if (windowId.compare(windowIdTmp) == 0) {
					groups.erase(it);
					break;
				}
			}
			GNC::GCS::ConfigurationController::TMapValues newGroup;
			newGroup[WINDOW_ID_KEY] = windowId;
			wxPoint position = pWindow->GetPosition();
			newGroup.insertInt(X_KEY_POSITION,position.x);
			newGroup.insertInt(Y_KEY_POSITION,position.y);
			wxSize size = pWindow->GetSize();
			newGroup.insertInt(WIDTH_KEY_POSITION,size.x);
			newGroup.insertInt(HEIGH_KEY_POSITION,size.y);
			wxTopLevelWindow* pTopLevel = dynamic_cast<wxTopLevelWindow*>(pWindow);
			if (pTopLevel != NULL) {
				newGroup.insertBool(IS_MAXIMIZED_KEY, pTopLevel->IsMaximized());
			}
			groups.push_back(newGroup);
			GNC::GCS::ConfigurationController::Instance()->writeGroupUser(ATTRIBUTES_SCOPE, groups);
		}
		
	}
}
