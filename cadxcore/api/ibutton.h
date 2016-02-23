#pragma once
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
