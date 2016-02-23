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

//#define _GINKGO_TRACE
#include "ilock.h"
#include <wx/thread.h>

//----------------------------------------------------------------------------------------------------
//region Interfaz de soporte de cerrojos

//----------------------------------------------------------------------------------------------------
//region Constructor y destructor

GNC::GCS::ILockable::ILockable()
{
	m_IsLocked = false;
	m_pCS = new wxCriticalSection();
}

GNC::GCS::ILockable::~ILockable()
{
	if (m_pCS != NULL) {
		delete m_pCS;
	}
	m_pCS = NULL;
}

//endregion

//----------------------------------------------------------------------------------------------------
//region Interfaz de exclusion mutua

void GNC::GCS::ILockable::Lock()
{
	//----------------------------------------------------------------------------------------------------
	// Entrada a la seccion Critica
	//----------------------------------------------------------------------------------------------------
	m_pCS->Enter();
	m_IsLocked = true;
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
}

void GNC::GCS::ILockable::UnLock()
{
	//----------------------------------------------------------------------------------------------------
	// Salida de la seccion Critica
	//----------------------------------------------------------------------------------------------------
	m_IsLocked = false;
	m_pCS->Leave();
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
}

bool GNC::GCS::ILockable::IsLocked() const
{
	return m_IsLocked;
}

//endregion


//----------------------------------------------------------------------------------------------------
//region Construccion y destruccion

GNC::GCS::ILocker::ILocker(GNC::GCS::ILockable& pLockable)
{
	m_pLockable = &pLockable;
	m_pLockable->Lock();

}

GNC::GCS::ILocker::ILocker(GNC::GCS::ILockable* pLockable)
{
	m_pLockable = pLockable;
	m_pLockable->Lock();

}

GNC::GCS::ILocker::~ILocker()
{
	if (m_pLockable != NULL) {
		m_pLockable->UnLock();
		m_pLockable = NULL;
	}
}

//endregion

//endregion
