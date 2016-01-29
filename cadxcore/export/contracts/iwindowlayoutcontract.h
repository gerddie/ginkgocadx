#pragma once
/*
*
*  $Id: iwindowlayoutcontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IWINDOWLAYOUTCONTRACT_H
#define IWINDOWLAYOUTCONTRACT_H
#include <list>

#include <api/api.h>
#include <api/icontract.h>

namespace GNC {
	namespace GCS {
		class EXTAPI IWindowLayoutContract : public IContract {
		public:

			IWindowLayoutContract();
			~IWindowLayoutContract();

			virtual void Setup(int minSlice, int maxSlice);

			virtual void AddRow();
			virtual void AddColumn();
			virtual void DelRow();
			virtual void DelColumn();
			virtual void SetWindowLayout(int filas, int columnas) = 0;
			virtual int GetColumns() = 0;
			virtual int GetRows() = 0;
			virtual void SplitActive(bool vertically);
			virtual bool SupportsSplit();

			int m_MinSlice;
			int m_MaxSlice;
		};
	}
}

#endif