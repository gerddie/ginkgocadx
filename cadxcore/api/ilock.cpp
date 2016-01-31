/*
 *  
 *  $Id: ilock.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
