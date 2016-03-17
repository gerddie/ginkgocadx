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

#include <api/api.h>
#include <api/globals.h>
#include <api/controllers/ieventscontroller.h>

#include <string>
#include <map>

class wxAuiNotebook;

namespace GNC
{

namespace GCS
{
class TimerInactivity;
class EXTAPI InactivityController
{

//region "Patron singleton"
public:
        static InactivityController* Instance();
        static void FreeInstance();

        static InactivityController* m_pInstance;
//endregion

//region "Interfaz de controlador de vistas"
public:
        virtual void StopsMonitoring();
        virtual void RestartMonitoring();
        virtual void ResetsInactivity();
        virtual void DetachForm();
        virtual void KillNow();
protected:
        TimerInactivity*		  m_pTimerInactivity;
        InactivityController();
        virtual ~InactivityController();
};
}
}
