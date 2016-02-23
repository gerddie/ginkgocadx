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
