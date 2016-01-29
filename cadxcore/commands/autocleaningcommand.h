/*
*  
*  $Id: autocleaningcommand.h $
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
#include <api/globals.h>
#include <api/icommand.h>
#include <api/controllers/ihistorycontroller.h>

namespace GADAPI {
	class AutoCleaningCommandParameters : public GNC::GCS::IComandoParams 
	{
	public:
		//download only specifying 
		AutoCleaningCommandParameters(int olderThan, int hdSpace, int dicomDirSize);
		virtual ~AutoCleaningCommandParameters();

	public:
		int OlderThan;
		int HdSpace;
		int DicomDirSize;
		GNC::GCS::IHistoryController::SeriesModelList seriesToDelete;
	};



	class AutoCleaningCommand: public GNC::GCS::IComando {
	public:
		AutoCleaningCommand(AutoCleaningCommandParameters* pParams);

    protected:
		virtual void Execute();
		virtual void Update();

    public:


		AutoCleaningCommandParameters* m_pAutoCleaningParameters;
	};
}
