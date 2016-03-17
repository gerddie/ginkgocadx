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

#include <wx/timer.h>
#include "countdownmessagebase.h"

namespace GNC
{
namespace GUI
{
class InactivityCountdownMessage: public InactivityCountdownMessageBase, public wxTimer
{
public:
        InactivityCountdownMessage(wxWindow* pParent, const wxString& message);
        ~InactivityCountdownMessage();
        virtual bool Show(bool show=true);
protected:
        virtual void OnCancel(wxCommandEvent &event);
        virtual void OnCloseButton(wxCommandEvent &event);
        virtual void OnClose(wxCloseEvent &event);
        virtual void Notify();
        int m_last;
        wxString m_message;
};
}
}
