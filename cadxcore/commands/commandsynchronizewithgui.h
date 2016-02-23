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
