#pragma once
/*
 *  
 *  $Id: ibutton.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IBUTTON_H
#define IBUTTON_H
#include <api/api.h>

namespace GNC {
	namespace GCS {

		const int EMPTYBUTTONMASK = 0x00;
		const int LBUTTONMASK = 0x01;
		const int RBUTTONMASK = 0x02;
		const int MBUTTONMASK = 0x04;

		class EXTAPI TriggerButton {

		public:

			TriggerButton();

			TriggerButton(const TriggerButton& o);

			/** Resets the activation button mask
			**/
			void ResetTriggerButtonMask();

			/** Enable activation button mask
			**/
			void EnableTriggerButton(int mask);

			TriggerButton& EnableLeft();

			TriggerButton& EnableRight();

			TriggerButton& EnableMiddle();

			/** Disable activation button mask
			**/
			void DisableTriggerButton(int mask);

			TriggerButton& DisableLeft();

			TriggerButton& DisableRight();

			TriggerButton& DisableMiddle();

			/** Checks activation button mask is applied
			**/
			bool IsTriggerButtonEnabled(int mask) const;

			bool IsLeftEnabled() const;

			bool IsRightEnabled() const;

			bool IsMiddleEnabled() const;

			TriggerButton& operator=(const TriggerButton& o);

			bool IsEmpty() const;

		protected:
			int m_TriggerButtonMask;
		};
	}
}
#endif