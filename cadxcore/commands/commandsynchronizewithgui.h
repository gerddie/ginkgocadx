/*
*  
*  $Id: commandsynchronizewithgui.h $
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
//#define _GINKGO_TRACE
#include <api/api.h>
#include <api/globals.h>
#include <api/icommand.h>
#include <api/ientorno.h>
#include <api/controllers/ieventscontroller.h>
#include <api/imodelointegracion.h>

namespace GADAPI {
	class EXTAPI ISynchronizedData {
	public:
		ISynchronizedData(){}
		virtual ~ISynchronizedData(){}
	};

	class EXTAPI ISynchronizedWithGUI
	{
	public:
		virtual ~ISynchronizedWithGUI() {}

		virtual void ExecuteSynchronized(GADAPI::ISynchronizedData* pData) = 0;
	};

	class EXTAPI CommandSynchronizeWithGUI : public GNC::GCS::IComando {
	public:
		CommandSynchronizeWithGUI(ISynchronizedWithGUI* pParam, ISynchronizedData * pData = NULL, bool clearData = false);
		virtual ~CommandSynchronizeWithGUI();

	 protected:
		virtual void Execute();
		virtual void Update();

	protected:
		ISynchronizedWithGUI* m_pListener;
		ISynchronizedData * m_pData;
		bool m_clearData;
	};
}
