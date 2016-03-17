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
#include "enabled_tools.h"
#if defined(ENABLE_SHOWTAGSTOOL)
#ifndef SHOWTAGSTOOL_H
#define SHOWTAGSTOOL_H
#include <export/tools/ishowtagstool.h>
#include <api/api.h>
#include <api/observers/ieventsobserver.h>

namespace GNC
{
namespace GUI
{
class PanelTags;
}

class EXTAPI ShowTagsTool : public GNC::GCS::IShowTagsTool, public GNC::GCS::IEventsObserver
{
protected:

public:
        virtual GNC::GCS::ITool* NewTool();
        ShowTagsTool();
        ~ShowTagsTool();
        virtual bool ExecuteAction();
        virtual void AppendToolIn(wxEvtHandler* pParent, wxMenu* pMenu);

        virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);
        bool IsVisible();
protected:
        GNC::GUI::PanelTags*								m_pPanelTags;
};
}
#endif
#endif

/*
#include "enabled_tools.h"
#if defined(ENABLE_SHOWTAGSTOOL)
#ifndef SHOWTAGSTOOL_H
#define SHOWTAGSTOOL_H
#include <export/iherramientapaneltags.h>
#include <api/controllers/ieventscontroller.h>
#include <api/api.h>

#include <map>

class HerramientaOpcionesGUIBase;

namespace GNC {

	class EXTAPI HerramientaPanelTags : public GNC::GCS::IToolPanelTags, public GNC::GCS::IObservadorEventos {

	protected:

	public:
		HerramientaPanelTags();
		~HerramientaPanelTags();

		virtual bool AppendInMenu(wxWindow* pParent, wxMenu* pMenuParent);

		//region "Realizacion de la interfaz IHerramienta"

		// Setup de paneles
		virtual void CrearPaneles( wxPanel* pParent );
		//endregion

		//cuando hacen click en ella...
		bool IsVisible();
		virtual void SolicitarActivacion(const GNC::GCS::TriggerButton& buttonmask);

		virtual void ConectarContratoFalso(bool activar);
		virtual void SetVistaActiva(GNC::GCS::IVista* pVista);
		virtual void ConectarContratos(bool activar);

		virtual void SetVentanaTags(GNC::GCS::IPanelHerramientaTags* pWindow) ;

		virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt);

	protected:
		GNC::GCS::IPanelHerramientaTags*								m_pPanelTags;

	};
}
#endif
#endif*/
