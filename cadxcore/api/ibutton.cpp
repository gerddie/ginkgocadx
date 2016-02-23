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

#include "ibutton.h"

GNC::GCS::TriggerButton::TriggerButton()
{
	ResetTriggerButtonMask();
}

GNC::GCS::TriggerButton::TriggerButton(const TriggerButton& o)
{
	*this = o;
}

/** Resets the activation button mask
**/
void GNC::GCS::TriggerButton::ResetTriggerButtonMask() {
	m_TriggerButtonMask = GNC::GCS::EMPTYBUTTONMASK;
}

/** Enable activation button mask
**/
void GNC::GCS::TriggerButton::EnableTriggerButton(int mask) {
	m_TriggerButtonMask |= mask;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::EnableLeft() {
	EnableTriggerButton(LBUTTONMASK);
	return *this;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::EnableRight() {
	EnableTriggerButton(RBUTTONMASK);
	return *this;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::EnableMiddle() {
	EnableTriggerButton(MBUTTONMASK);
	return *this;
}

/** Disable activation button mask
**/
void GNC::GCS::TriggerButton::DisableTriggerButton(int mask) {
	m_TriggerButtonMask &= ~mask;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::DisableLeft() {
	DisableTriggerButton(LBUTTONMASK);
	return *this;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::DisableRight() {
	DisableTriggerButton(RBUTTONMASK);
	return *this;
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::DisableMiddle() {
	DisableTriggerButton(MBUTTONMASK);
	return *this;
}

/** Checks activation button mask is applied
**/
bool GNC::GCS::TriggerButton::IsTriggerButtonEnabled(int mask) const {
	return (0 != (m_TriggerButtonMask & mask));
}

bool GNC::GCS::TriggerButton::IsLeftEnabled() const {
	return IsTriggerButtonEnabled(LBUTTONMASK);
}

bool GNC::GCS::TriggerButton::IsRightEnabled() const {
	return IsTriggerButtonEnabled(RBUTTONMASK);
}

bool GNC::GCS::TriggerButton::IsMiddleEnabled() const {
	return IsTriggerButtonEnabled(MBUTTONMASK);
}

GNC::GCS::TriggerButton& GNC::GCS::TriggerButton::operator=(const GNC::GCS::TriggerButton& o) {
	this->m_TriggerButtonMask = o.m_TriggerButtonMask;
	return *this;
}

bool GNC::GCS::TriggerButton::IsEmpty() const {
	return this->m_TriggerButtonMask == EMPTYBUTTONMASK;
}
