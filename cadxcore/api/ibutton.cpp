/*
*  
*  $Id: ibutton.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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
