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

#include <api/autoptr.h>
#include "previewpanelbase.h"
#include "ipreviewpanel.h"
#include "previewpanelmodel.h"

namespace GNC
{
namespace GUI
{
class FirstLevelPanel: public FirstLevelPanelBase, public INodoHistorial
{
public:
        FirstLevelPanel(GNC::GCS::IVista* pView, INodoHistorial* pPadre,  GNC::GUI::IPreviewNode* pNode);
        ~FirstLevelPanel();

        wxWindow* GetWxWindow();
        virtual void GetNodeSize(int &x, int &y);


protected:
        virtual void OnChar(wxKeyEvent &event);
        virtual void OnPaint(wxPaintEvent& event);
        virtual void OnSize(wxSizeEvent &event);
        virtual void OnMouseWheel(wxMouseEvent &event);
        virtual void OnEraseBackground( wxEraseEvent& event );

        GNC::GUI::IPreviewNode* m_pNode;
        GNC::GCS::IVista* m_pView;
};
}
}
