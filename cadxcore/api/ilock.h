#pragma once
/*
 *  
 *  $Id: ilock.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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

namespace GNC {
	namespace GCS {
		class ILocker;		
	}
}
//endregion


namespace GNC {
	namespace GCS {

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
