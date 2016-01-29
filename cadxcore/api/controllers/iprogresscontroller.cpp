/*
*  
*  $Id: iprogresscontroller.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "iprogresscontroller.h"

//-------------------------------------------------------------------------

GNC::IControladorProgreso::IControladorProgreso()
{
}

GNC::IControladorProgreso::~IControladorProgreso()
{
}

//-------------------------------------------------------------------------

GNC::INotificadorProgreso::INotificadorProgreso()
{
	m_ProgresoNormalizado = 0;
}

GNC::INotificadorProgreso::~INotificadorProgreso()
{
}

/** Cuidado! hay que proteger estos accesos con un cerrojo **/
float GNC::INotificadorProgreso::GetProgresoNormalizado()
{
	return m_ProgresoNormalizado;
}

/** Cuidado! hay que proteger estos accesos con un cerrojo **/
std::string GNC::INotificadorProgreso::GetTextoProgreso()
{
	return m_Texto;
}

/** Cuidado! hay que proteger estos accesos con un cerrojo **/
void GNC::INotificadorProgreso::SetProgresoNormalizado(float progresoNormalizado)
{
	m_ProgresoNormalizado = progresoNormalizado;
}

/** Cuidado! hay que proteger estos accesos con un cerrojo **/
void GNC::INotificadorProgreso::SetTextoProgreso(const std::string& texto)
{
	m_Texto = texto;
}

void GNC::INotificadorProgreso::LockProgreso()
{
	this->Lock();
}

void GNC::INotificadorProgreso::UnLockProgreso()
{
	this->UnLock();
}
