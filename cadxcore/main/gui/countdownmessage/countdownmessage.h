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
		class CountdownMessage: public CountdownMessageBase, public wxTimer
		{
		public:

			CountdownMessage(wxWindow* pParent, const wxString title, const wxString& message, int seconds = 30);
			~CountdownMessage();
			virtual int ShowModalCountdown();
			virtual bool Show(bool show=true);
		protected:
			virtual void Notify();
			int m_last;
			wxString m_message;
		};
	}
}
