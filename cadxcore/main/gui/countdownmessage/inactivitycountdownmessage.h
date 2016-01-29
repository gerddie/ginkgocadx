/*
 *
 *  $Id: InactivityCountdownMessage.h $
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

#include <wx/timer.h>
#include "countdownmessagebase.h"

namespace GNC {
	namespace GUI {
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
