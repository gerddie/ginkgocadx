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
//#define DUMP_IMAGENES
#include <api/globals.h>
#include "commandsynchronizewithgui.h"
#include <main/controllers/controladoreventos.h>

#define IDC_COMANDO_SYNCHRONIZE_WITH_GUI           116

// Singleton de persistencia
namespace GADAPI
{
CommandSynchronizeWithGUI::CommandSynchronizeWithGUI(ISynchronizedWithGUI* pParams, ISynchronizedData* pData, bool clearData):IComando(NULL)
{
        SetId(IDC_COMANDO_SYNCHRONIZE_WITH_GUI);
        m_pListener = pParams;
        m_pData = pData;
        m_clearData = clearData;
}

CommandSynchronizeWithGUI::~CommandSynchronizeWithGUI()
{
        if (m_clearData && m_pData != NULL) {
                delete m_pData;
        }
}

void CommandSynchronizeWithGUI::Execute()
{
}

void CommandSynchronizeWithGUI::Update()
{
        m_pListener->ExecuteSynchronized(m_pData);
}

}


