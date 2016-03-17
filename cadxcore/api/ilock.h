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

#ifndef ILOCK_H
#define ILOCK_H
#include <api/api.h>
#include <list>
#include <string>
#include <atomic>

//----------------------------------------------------------------------------------------------------
//region Forward Declarations
class wxCriticalSection;

namespace GNC
{
namespace GCS
{
class ILocker;
}
}
//endregion


namespace GNC
{
namespace GCS
{

//----------------------------------------------------------------------------------------------------
//region Interfaz de soporte de cerrojos
//----------------------------------------------------------------------------------------------------
class EXTAPI ILockable
{
        //----------------------------------------------------------------------------------------------------
        //region Constructor y destructor
public:

        ILockable();
        ~ILockable();
        //endregion

        //----------------------------------------------------------------------------------------------------
        //region Interfaz de exclusion mutua
public:

        void Lock();

        void UnLock();

        bool IsLocked() const;
        //enregion

        //----------------------------------------------------------------------------------------------------
        //region Interfaz de subscripcion
private:
//				ILocker* m_pLocker; // ILocker que ha bloqueado el cerrojo.
        //endregion

        //----------------------------------------------------------------------------------------------------
        //region Atributos
private:
        wxCriticalSection* m_pCS;             // El cerrojo real.
        bool               m_IsLocked;
        //endregion

        friend class ILocker;
};
//endregion

//----------------------------------------------------------------------------------------------------
//region Helper de bloqueo automatico
//----------------------------------------------------------------------------------------------------
class EXTAPI ILocker
{
        //----------------------------------------------------------------------------------------------------
        //region Construccion y destruccion
public:

        ILocker( GNC::GCS::ILockable& pLockable );
        ILocker( GNC::GCS::ILockable* pLockable );
        ~ILocker();
        //endregion

        //----------------------------------------------------------------------------------------------------
        //region Atributos
private:

        ILockable* m_pLockable;
        //endregion

        friend class ILockable;
};
//endregion
}
}

#endif
