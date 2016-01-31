/*
 *  
 *  $Id: toolcontroller.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#include <map>
#include <set>

#include "toolcontroller.h"

#include <main/gui/toolsystem/ginkgotoolsystembar.h>
#include <api/toolsystem/iwidgettool.h>
#include <api/toolsystem/itoolsregistry.h>
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/iconfigurationcontroller.h>
#include <api/controllers/ieventscontroller.h>
#include <eventos/toolevents.h>
#include <api/icontract.h>
#include <api/ientorno.h>

#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/menu.h>
#include <wx/wxginkgotoolbar.h>

#define LEFT_BUTTON_CONF_KEY   "lbt"
#define MIDDLE_BUTTON_CONF_KEY "mbt"
#define RIGHT_BUTTON_CONF_KEY  "rbt"

namespace GNC {
	namespace GUI {
		const wxEventType wxEVT_ACTIVATE_TOOL_ASYNC = wxNewEventType();
		class wxActivateToolAsync : public wxEvent
		{

		public:
			wxActivateToolAsync(GNC::ToolController* pToolController = NULL, long idTool = -1, const GNC::GCS::TriggerButton& buttonMask = GNC::GCS::TriggerButton(), wxWindow* pWindow = NULL): ToolController(pToolController), 
				IdTool(idTool), 
				ButtonMask(buttonMask), 
				Window(pWindow)
			{
				SetEventType(wxEVT_ACTIVATE_TOOL_ASYNC);
			}

			wxActivateToolAsync(const wxActivateToolAsync& event):wxEvent(event),ToolController(event.ToolController), 
				IdTool(event.IdTool), 
				ButtonMask(event.ButtonMask),
				Window(event.Window)
			{
			}

			GNC::ToolController* GetToolController()
			{
				return ToolController;
			}

			long GetIdTool()
			{
				return IdTool;
			}

			wxWindow* GetWindow()
			{
				return Window;
			}

			GNC::GCS::TriggerButton GetButtonMask()
			{
				return ButtonMask;
			}

			virtual wxEvent *Clone() const
			{
				return new wxActivateToolAsync(*this);
			}

		protected:
			GNC::ToolController* ToolController;
			long IdTool;
			GNC::GCS::TriggerButton ButtonMask;
			wxWindow* Window;

		private:
			 DECLARE_DYNAMIC_CLASS_NO_ASSIGN(wxActivateToolAsync)
		};

		typedef void (wxEvtHandler::*wxActivateToolAsyncFunction)(wxActivateToolAsync&);

		#define wxActivateToolAsyncHandler(func) \
			 (wxObjectEventFunction)(wxEventFunction)wxStaticCastEvent(wxActivateToolAsyncFunction, &func)

		#define EVT_MENSAJES_USUARIO(func) \
			 wx__DECLARE_EVT0(wxEVT_CLOSE_SERIESVIEWER_ASYNC, wxActivateToolAsyncHandler(func))

		IMPLEMENT_DYNAMIC_CLASS(GNC::GUI::wxActivateToolAsync, wxEvent)

		class wxEventHandlerToolController: public wxEvtHandler
		{
		public:
			wxEventHandlerToolController(GNC::ToolController* pToolController): ToolController(pToolController),
				IdEventHandler(wxNewId())
			{
				GNC::GCS::IEntorno::Instance()->GetVentanaRaiz()->Connect(IdEventHandler, wxEVT_ACTIVATE_TOOL_ASYNC, wxActivateToolAsyncHandler(wxEventHandlerToolController::OnActivateTool), NULL, this);
			}
			~wxEventHandlerToolController()
			{
				if (GNC::GCS::IEntorno::Instance()->GetVentanaRaiz() != NULL) {
					GNC::GCS::IEntorno::Instance()->GetVentanaRaiz()->Disconnect(IdEventHandler, wxEVT_ACTIVATE_TOOL_ASYNC, wxActivateToolAsyncHandler(wxEventHandlerToolController::OnActivateTool), NULL, this);
				}
			}
			void OnActivateTool(wxActivateToolAsync& event )
			{
				if (ToolController == event.GetToolController()) {
					ToolController->DoActivateTool(event.GetIdTool(), event.GetButtonMask(), event.GetWindow());
				}
			}

			long GetId() 
			{
				return IdEventHandler;
			}

		protected:
			GNC::ToolController* ToolController;
			long IdEventHandler;
		};
	}
}

GNC::ToolController::ToolController(): EventHandler(new GNC::GUI::wxEventHandlerToolController(this)),
                                       m_leftTool(NULL),
                                       m_rightTool(NULL),
                                       m_middleTool(NULL)
{	
}

GNC::ToolController::~ToolController()
{
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		delete pTool;
	}
	m_tools.clear();
}

void GNC::ToolController::InitializeToolController()
{

	// By default, try to select:
	// * The first visualization tool available => to left button,
	// * The next visualiation tool available after left button assignation => to right button
	// * The next visualiation tool available after rifht button assignation => to middle button
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool* pWidgetTool = dynamic_cast<GNC::GCS::ITool*> ((*it).second);
		if (pWidgetTool != NULL && pWidgetTool->FamilyId == GNC::GCS::ITool::TFamiliaVisualizacion && pWidgetTool->IsReady())
		{
			if (m_leftTool == NULL)
			{
				GNC::GCS::TriggerButton buttonMask;
				buttonMask.EnableLeft();
				DoActivateTool(pWidgetTool->ID, buttonMask, NULL);
			}
			else if (m_rightTool == NULL)
			{
				GNC::GCS::TriggerButton buttonMask;
				buttonMask.EnableRight();
				DoActivateTool(pWidgetTool->ID, buttonMask, NULL);
			}
			else if (m_middleTool == NULL)
			{
				GNC::GCS::TriggerButton buttonMask;
				buttonMask.EnableMiddle();
				DoActivateTool(pWidgetTool->ID, buttonMask, NULL);
			}
			else
			{
				break;
			}
		}
	}
}

void GNC::ToolController::FinalizeToolController()
{
	if (m_leftTool != NULL) {
		m_leftTool->GetTriggerButton().DisableLeft();
		m_leftTool->Disconnect();
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_leftTool, this));
	}

	if (m_rightTool != NULL) {
		m_rightTool->GetTriggerButton().DisableRight();
		m_rightTool->Disconnect();
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_rightTool, this));
	}

	if (m_middleTool != NULL) {
		m_middleTool->GetTriggerButton().DisableMiddle();
		m_middleTool->Disconnect();
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_middleTool, this));
	}
}

void GNC::ToolController::ReconnectToolController()
{
	if (m_leftTool != NULL) {
		m_leftTool->Disconnect();
	}
	if (m_rightTool != NULL) {
		m_rightTool->Disconnect();
	}
	if (m_middleTool != NULL) {
		m_middleTool->Disconnect();
	}

	if(m_leftTool != NULL && m_leftTool->IsEnabled()) {
		m_leftTool->Connect();
	}
	if (m_rightTool != NULL && m_rightTool->IsEnabled() && m_rightTool != m_leftTool) {
		m_rightTool->Connect();
	}
	if (m_middleTool != NULL && m_middleTool->IsEnabled() && m_middleTool != m_leftTool && m_middleTool != m_rightTool) {
		m_middleTool->Connect();
	}
}

void GNC::ToolController::LoadStatus(const std::string& view, const std::string& modality) {
	
	if ( view.empty() || modality.empty() ) {
		return;
	}

	int selectedLeftToolId = -1;
	int selectedMiddleToolId = -1;
	int selectedRightToolId = -1;

	std::string scope(GetToolStatusScope(view, modality));

	GNC::GCS::IConfigurationController::Instance()->readIntUser(scope, LEFT_BUTTON_CONF_KEY, selectedLeftToolId, -1);
	GNC::GCS::IConfigurationController::Instance()->readIntUser(scope, MIDDLE_BUTTON_CONF_KEY, selectedMiddleToolId, -1);
	GNC::GCS::IConfigurationController::Instance()->readIntUser(scope, RIGHT_BUTTON_CONF_KEY, selectedRightToolId, -1);

	if (selectedLeftToolId != -1) {
		GNC::GCS::TriggerButton buttonMask;
		buttonMask.EnableLeft();
		DoActivateTool(selectedLeftToolId, buttonMask, NULL);
	}
	if (selectedMiddleToolId != -1) {
		GNC::GCS::TriggerButton buttonMask;
		buttonMask.EnableMiddle();
		DoActivateTool(selectedMiddleToolId, buttonMask, NULL);
	}
	if (selectedRightToolId != -1) {
		GNC::GCS::TriggerButton buttonMask;
		buttonMask.EnableRight();
		DoActivateTool(selectedRightToolId, buttonMask, NULL);
	}
}

void GNC::ToolController::SaveStatus(const std::string& view, const std::string& modality) {

	if ( view.empty() || modality.empty() ) {
		return;
	}

	std::string scope(GetToolStatusScope(view, modality));

	if (m_leftTool != NULL) {
		GNC::GCS::IConfigurationController::Instance()->writeIntUser(scope, LEFT_BUTTON_CONF_KEY, m_leftTool->ID);
	}
	if (m_middleTool != NULL) {
		GNC::GCS::IConfigurationController::Instance()->writeIntUser(scope, MIDDLE_BUTTON_CONF_KEY, m_middleTool->ID);
	}
	if (m_rightTool != NULL) {
		GNC::GCS::IConfigurationController::Instance()->writeIntUser(scope, RIGHT_BUTTON_CONF_KEY, m_rightTool->ID);
	}
}

void GNC::ToolController::AddTool(long idTool, GNC::GCS::IContract* pContract)
{
	GNC::GCS::ITool* pTool = GNC::GCS::IToolsRegistry::Instance()->NewTool(idTool);
	if (pTool != NULL && pTool->IsReady()) {
		GNC::GCS::IContractable* pContractableTool = dynamic_cast<GNC::GCS::IContractable*>(pTool);
		if (pContractableTool) {
			if (pContractableTool->IsValidContract(pContract)) {				
				pContractableTool->Register(pContract);
			}
			else {
				LOG_WARN("ToolControler", "The tool \"" << pTool->Name.c_str() << "\" is not compatible with the specified contract");
				return;
			}
		}
		pTool->SetToolBar(this);
		m_tools[idTool] = pTool;
	} else if (pTool != NULL) {
		delete pTool;
	}
}

void GNC::ToolController::RemoveTool(long /*idTool*/)
{

}

GNC::GCS::ITool* GNC::ToolController::GetTool(long idTool)
{
	GNC::GCS::ITool* pTool = NULL;
	if (m_tools.find(idTool) != m_tools.end()) {
		pTool = m_tools[idTool];
	}
	return pTool;
}

void GNC::ToolController::Enable(long idTool, bool enabled)
{
	GNC::GCS::ITool* pTool = GetTool(idTool);
	if (pTool != NULL && pTool->IsEnabled() != enabled) {
		pTool->Enable(enabled);
		ReconnectToolController();
		GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolEnabledEvent(enabled, pTool, this));
	}
}

wxWindow* GNC::ToolController::CreateNewToolBar(wxWindow* pParent, GNC::GCS::ITool::TToolFamily family, bool alignLeft) 
{
	GNC::GUI::GinkgoToolSystemBar* pToolController = new GNC::GUI::GinkgoToolSystemBar(pParent, this, family);
	if (!alignLeft) {
		pToolController->AddStretchSpacer(1);
	}
	AppendsToolsInToolBar(pToolController, family);
	pToolController->Realize();
	return pToolController;
}

wxWindow* GNC::ToolController::CreateNewToolBar(wxWindow* pParent, const GNC::GCS::IToolController::TIdList& toolsToInclude, bool alignLeft)
{
	GNC::GUI::GinkgoToolSystemBar* pToolController = new GNC::GUI::GinkgoToolSystemBar(pParent, this, GNC::GCS::ITool::TFamiliaGlobal);
	if (!alignLeft) {
		pToolController->AddStretchSpacer(1);
	}
	AppendsToolsInToolBar(pToolController, toolsToInclude);	
	pToolController->Realize();
	return pToolController;
}

wxWindow* GNC::ToolController::CreateNewToolBar(wxWindow* pParent, bool alignLeft)
{
	wxPanel* toolBarPanel = new wxPanel(pParent);
	wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	typedef std::map<GNC::GCS::ITool::TToolFamily, GNC::GUI::GinkgoToolSystemBar*> TFamiliesMap;
	TFamiliesMap mapOfFamilies;
	
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		if (mapOfFamilies.find(pTool->GetFamily()) == mapOfFamilies.end() && pTool->IsReady()) {
			mapOfFamilies[pTool->GetFamily()] = new GNC::GUI::GinkgoToolSystemBar(toolBarPanel, this, pTool->GetFamily()); 
			if (!alignLeft) {
				mapOfFamilies[pTool->GetFamily()]->AddStretchSpacer(1);
			}
		}
		pTool->AppendToolIn(mapOfFamilies[pTool->GetFamily()]);
	}
	for (TFamiliesMap::iterator it = mapOfFamilies.begin(); it != mapOfFamilies.end(); ++it) 
	{
		if ((*it).second->GetToolCount() == 0) {
			delete (*it).second;
		} else {
			(*it).second->Realize();
			mainSizer->Add((*it).second,0, wxEXPAND, 0);
		}
	}
	toolBarPanel->SetSizer(mainSizer);
	toolBarPanel->Layout();
	return toolBarPanel;
}

void GNC::ToolController::AppendsToolInMenu(wxEvtHandler* pParent, wxMenu* pMenu, GNC::GCS::ITool::TToolFamily family)
{
	std::map<int , std::list<GNC::GCS::ITool*> > mapSubFamilies;
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		if (pTool->GetFamily() == family && pTool->IsReady()) {
			if (mapSubFamilies.find(pTool->GetSubFamily()) == mapSubFamilies.end()) {
				mapSubFamilies[pTool->GetSubFamily()] = std::list<GNC::GCS::ITool*>();
			}
			mapSubFamilies[pTool->GetSubFamily()].push_back(pTool);
		}
	}
	for (std::map<int , std::list<GNC::GCS::ITool*> >::iterator itSubFamilies = mapSubFamilies.begin(); itSubFamilies != mapSubFamilies.end(); ++itSubFamilies)
	{
		if (!pMenu->GetMenuItems().empty() && !pMenu->GetMenuItems().back()->IsSeparator()) {
			pMenu->AppendSeparator();
		}
		for (std::list<GNC::GCS::ITool*>::iterator itTools = (*itSubFamilies).second.begin(); itTools != (*itSubFamilies).second.end(); ++itTools) 
		{
			GNC::GCS::ITool*& pTool = (*itTools);
			pTool->AppendToolIn(pParent, pMenu);
		}
		if (!pMenu->GetMenuItems().empty() && pMenu->GetMenuItems().back()->IsSeparator()) {
			pMenu->Delete(pMenu->GetMenuItems().back()->GetId());
		}
	}	
	if (!pMenu->GetMenuItems().empty() && pMenu->GetMenuItems().back()->IsSeparator()) {
		pMenu->Delete(pMenu->GetMenuItems().back()->GetId());
	}
}

void GNC::ToolController::AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, GNC::GCS::ITool::TToolFamily family)
{
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		if (pTool->GetFamily() == family && pTool->IsReady()) {
			pTool->AppendToolIn(pToolBar);
		}
	}
}

void GNC::ToolController::AppendsToolsInToolBar(wxGinkgoToolBar* pToolBar, const GNC::GCS::IToolController::TIdList& toolsToInclude)
{
	std::set<long> setOfIds;
	setOfIds.insert(toolsToInclude.begin(), toolsToInclude.end());

	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		if (pTool != NULL && setOfIds.find(pTool->ID) != setOfIds.end() && pTool->IsReady()) {
			pTool->AppendToolIn(pToolBar);
		}
	}
}

void GNC::ToolController::OnKeyAccelerator(GNC::GCS::Events::EventoTeclado& evento, wxWindow* pWindow)
{
	if (evento.ControlDown() || evento.GetKeyCode() == WXK_ESCAPE) {
		for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
		{
			GNC::GCS::ITool*& pTool = (*it).second;
			if (pTool->IsEnabled() && pTool->GetKeyCode() == evento.GetKeyCode()) {
				GNC::GCS::TriggerButton buttonMask;
				if (evento.ShiftDown() && evento.AltDown() ) {
					buttonMask.EnableMiddle();
				}
				if (evento.ShiftDown()) {
					buttonMask.EnableRight();
				}
				else {
					buttonMask.EnableLeft();
				}
				ActivateTool(pTool->ID, buttonMask, pWindow);
				evento.Skip(false);
				return;
			}
		}
	}
	evento.Skip(true);
}

void GNC::ToolController::ActivateTool(long idTool, const GNC::GCS::TriggerButton& buttonMask, wxWindow* pWindow)
{
	if (!wxIsMainThread())
	{
		//activation will be processed in next iteration of loop event to avoid that component that request activation has been removed in action execution
		GNC::GUI::wxActivateToolAsync evt(this, idTool, buttonMask, pWindow);
		evt.SetId(EventHandler->GetId());
		GNC::GCS::IEntorno::Instance()->GetVentanaRaiz()->GetEventHandler()->AddPendingEvent(evt);
	}
	else
	{
		DoActivateTool(idTool, buttonMask, pWindow);
	}
}

void GNC::ToolController::DoActivateTool(long idTool, const GNC::GCS::TriggerButton& buttonMask, wxWindow* pWindow)
{
	if (m_tools.find(idTool) == m_tools.end()) {
		return;
	}
	GNC::GCS::ITool* pTool = m_tools[idTool];
	if (!pTool->IsEnabled()) {
		return;
	}

	GNC::GCS::IWidgetTool* pWidgetTool = dynamic_cast<GNC::GCS::IWidgetTool*> (pTool);
	if (pWidgetTool == NULL) 
	{
		//regular tools...
		if (!pTool->ExecuteAction()) {
			if (pTool->HasDropDownMenu() && pWindow != NULL) {
				wxMenu DropDownMenu;
				pTool->AppendDropDownMenu(&DropDownMenu, &DropDownMenu);
				pWindow->PopupMenu(&DropDownMenu);
			}
		}
	} 
	else 
	{
		if (m_leftTool != NULL) {
			m_leftTool->GetTriggerButton().DisableLeft();
			m_leftTool->Disconnect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_leftTool, this));
		}
		if (m_rightTool != NULL) {
			m_rightTool->GetTriggerButton().DisableRight();
			m_rightTool->Disconnect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_rightTool, this));
		}
		if (m_middleTool != NULL) {
			m_middleTool->GetTriggerButton().DisableMiddle();
			m_middleTool->Disconnect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(false, m_middleTool, this));
		}

		if ( buttonMask.IsLeftEnabled() && !pWidgetTool->GetTriggerButton().IsLeftEnabled()) {
			m_leftTool = pWidgetTool;
		}
		if ( buttonMask.IsRightEnabled() && !pWidgetTool->GetTriggerButton().IsRightEnabled()) {
			m_rightTool = pWidgetTool;
		}
		if ( buttonMask.IsMiddleEnabled() && !pWidgetTool->GetTriggerButton().IsMiddleEnabled()) {
			m_middleTool = pWidgetTool;
		}

		if (m_leftTool != NULL) {
			m_leftTool->GetTriggerButton().EnableLeft();
		}
		if (m_rightTool != NULL) {
			m_rightTool->GetTriggerButton().EnableRight();
		}
		if (m_middleTool != NULL) {
			m_middleTool->GetTriggerButton().EnableMiddle();
		}

		if (m_rightTool != NULL && m_rightTool->IsEnabled() && m_rightTool != m_leftTool) {
			m_rightTool->Connect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(true, m_rightTool, this));
		}
		if (m_middleTool != NULL && m_middleTool->IsEnabled() && m_middleTool != m_leftTool && m_middleTool != m_rightTool) {
			m_middleTool->Connect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(true, m_middleTool, this));
		}
		if(m_leftTool != NULL && m_leftTool->IsEnabled()) {
			m_leftTool->Connect();
			GNC::GCS::IEventsController::Instance()->ProcesarEvento(new GNC::GCS::Events::ToolConnectedEvent(true, m_leftTool, this));
		}
	}
}

void GNC::ToolController::AppendDropDownMenu(long idTool, wxEvtHandler* pParent, wxMenu* pMenu)
{
	if (m_tools.find(idTool) == m_tools.end()) {
		return;
	}
	GNC::GCS::ITool* pTool = m_tools[idTool];
	if (pTool->IsReady()) {
		pTool->AppendDropDownMenu(pParent,pMenu);
	}
}

void GNC::ToolController::ConnectKeyEvents(wxWindow* pParent)
{
	for (TToolMap::iterator it = m_tools.begin(); it != m_tools.end(); ++it) 
	{
		GNC::GCS::ITool*& pTool = (*it).second;
		pTool->ConnectKeyEvents(pParent);
	}

}

GNC::GCS::IWidgetTool* GNC::ToolController::GetLeftTool()
{
	return m_leftTool;
}

GNC::GCS::IWidgetTool* GNC::ToolController::GetRightTool()
{
	return m_rightTool;
}

GNC::GCS::IWidgetTool* GNC::ToolController::GetMiddleTool()
{
	return m_middleTool;
}

std::string GNC::ToolController::GetToolStatusScope(const std::string& view, const std::string& modality) {
	std::ostringstream os;
	os << "/TooBarStatus/" << view << "/" << modality;
	return os.str();
}
