/*
 *  
 *  $Id: toolwindowlevel.h $
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
#include "enabled_tools.h"
#if defined(ENABLE_WINDOWLEVELTOOL)
#ifndef WINDOWLEVELTOOL_H
#define WINDOWLEVELTOOL_H
#include <export/tools/itoolwindowlevel.h>
#include <api/controllers/ieventscontroller.h>
#include <api/api.h>
#include <widgets/observers/iobservadorwindowlevel.h>
#include <export/contracts/iwindowlevelcontract.h>

#include <map>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			class WWindowLevelBuilder;
		}
	}

	class EXTAPI ToolWindowLevel : public GNC::GCS::IToolWindowLevel, public GNC::GCS::Widgets::Observadores::IObservadorWindowLevel {

	public:
		typedef GNC::GCS::Widgets::WWindowLevelBuilder TBuilder;
		typedef GNC::GCS::IWindowLevelContract TWindowLevelContract;

		ToolWindowLevel();
		virtual ~ToolWindowLevel();

		//region "ITool/IContract realization"

		virtual GNC::GCS::ITool* NewTool();
		virtual void Connect();
		virtual void Disconnect();

		virtual bool HasDropDownMenu();
		void AppendDropDownMenu(wxEvtHandler* pParent, wxMenu* pMenu);
		//endregion

		//region "Realización de la interfaz IHerramienta"
		virtual void OnStart();
		virtual void OnAjuste(TVector& start, TVector& stop);
		virtual void OnEnd();
		//endregion

		//region "metodos propios de la herramienta window/level"
		virtual void ClampWindowLevel(double window, double level, double& clampedWindow, double& clampedLevel);

		void SetWindowLevel(const std::string& label);
		virtual void SetWindowLevel(double window, double level);
		void Resetear();
		void AutoCalcule();
		//endregion

	protected:
		TBuilder*  m_pWindowLevelBuilder;
		GNC::GCS::IWindowLevelContract::WindowLevel m_WindowLevelInicial;

	};
}
#endif
#endif