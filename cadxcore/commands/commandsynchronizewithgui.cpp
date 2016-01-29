/*
*  
*  $Id: commandsynchronizewithgui.cpp $
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
//#define DUMP_IMAGENES
#include <api/globals.h>
#include "commandsynchronizewithgui.h"
#include <main/controllers/controladoreventos.h>

#define IDC_COMANDO_SYNCHRONIZE_WITH_GUI           116

// Singleton de persistencia
namespace GADAPI {
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


