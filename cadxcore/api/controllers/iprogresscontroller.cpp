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
