/*
*  
*  $Id: toolevents.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#pragma once

#include <api/ievento.h>
#include "eventosginkgo.h"

class wxBitmap;
namespace GNC {
	namespace GCS {
		class ITool;
		class IToolController;
		namespace Events {
			class EXTAPI ToolConnectedEvent : public GNC::GCS::Events::IEvent
			{
			private:
				bool connected;
				GNC::GCS::ITool* pTool;
				GNC::GCS::IToolController* pToolBar;
			public:
				ToolConnectedEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolConnected, 0, 100, NULL)
				{
					connected = false;
					pTool = NULL;
					pToolBar = NULL;
				}

				ToolConnectedEvent(bool connected, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolConnected, 0, 100, NULL){
					this->connected = connected;
					this->pTool = pTool;
					this->pToolBar = pToolBar;
				}

				~ToolConnectedEvent()
				{
				}
				
				bool IsConnected() {
					return connected;
				}

				GNC::GCS::ITool* GetTool() {
					return pTool;
				}

				GNC::GCS::IToolController* GetToolBar() {
					return pToolBar;
				}
			};

			class EXTAPI ToolEnabledEvent : public GNC::GCS::Events::IEvent
			{
			private:
				bool enabled;
				GNC::GCS::ITool* pTool;
				GNC::GCS::IToolController* pToolBar;
			public:
				ToolEnabledEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolEnabled, 0, 100, NULL)
				{
					enabled = false;
					pTool = NULL;
					pToolBar = NULL;
				}

				ToolEnabledEvent(bool enabled, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolEnabled, 0, 100, NULL){
					this->enabled = enabled;
					this->pTool = pTool;
					this->pToolBar = pToolBar;
				}

				~ToolEnabledEvent()
				{
				}
				
				bool IsEnabled() {
					return enabled;
				}

				GNC::GCS::ITool* GetTool() {
					return pTool;
				}

				GNC::GCS::IToolController* GetToolBar() {
					return pToolBar;
				}
			};

			class EXTAPI ChangeToolIconEvent : public GNC::GCS::Events::IEvent
			{
			private:
				const wxBitmap* pBitmap;
				GNC::GCS::ITool* pTool;
				GNC::GCS::IToolController* pToolBar;
			public:
				ChangeToolIconEvent() : GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolIconChanged, 0, 100, NULL)
				{
					pBitmap = NULL;
					pTool = NULL;
					pToolBar = NULL;
				}

				ChangeToolIconEvent(const wxBitmap* bmp, GNC::GCS::ITool* pTool, GNC::GCS::IToolController* pToolBar): GNC::GCS::Events::IEvent(ginkgoEVT_Core_ToolIconChanged, 0, 100, NULL),pBitmap(bmp)
				{
					this->pTool = pTool;
					this->pToolBar = pToolBar;
				}

				~ChangeToolIconEvent()
				{
				}
				
				const wxBitmap* GetBitmap() {
					return pBitmap;
				}

				GNC::GCS::ITool* GetTool() {
					return pTool;
				}

				GNC::GCS::IToolController* GetToolBar() {
					return pToolBar;
				}
			};

		}
	}
}
