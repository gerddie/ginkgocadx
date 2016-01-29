/*
 *  
 *  $Id: iprogresscontroller.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <string>
#include <api/ilock.h>

namespace GNC {

	//-------------------------------------------------------------------------

	class EXTAPI IControladorProgreso {
	public:

		IControladorProgreso();

		virtual ~IControladorProgreso();

		/** Interface **/
		virtual void InsertarTarea(long taskId, const std::string& texto) = 0;

		/** Interface **/
		virtual void EliminarTarea(long taskId) = 0;

		/** Interface **/
		virtual void SetProgresoTarea(long taskId, float normalizedProgress, const std::string& texto) = 0;

	};

	//-------------------------------------------------------------------------

	class EXTAPI INotificadorProgreso : public GNC::GCS::ILockable {

	public:

		INotificadorProgreso();

		virtual ~INotificadorProgreso();

		/** Cuidado! hay que proteger estos accesos con un cerrojo **/
		float GetProgresoNormalizado();

		/** Cuidado! hay que proteger estos accesos con un cerrojo **/
		std::string GetTextoProgreso();

		/** Cuidado! hay que proteger estos accesos con un cerrojo **/
		void SetProgresoNormalizado(float progresoNormalizado);

		/** Cuidado! hay que proteger estos accesos con un cerrojo **/
		void SetTextoProgreso(const std::string& texto);

		void LockProgreso();

		void UnLockProgreso();

		/** Interface **/
		virtual bool NotificarProgreso(float progresoNormalizado, const std::string& texto) = 0;

	protected:
		std::string m_Texto;
		float m_ProgresoNormalizado;
	};
}
