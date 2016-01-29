/*
*  
*  $Id: openremovableunit.h $
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
#include <api/icommand.h>

namespace GADAPI {
	class  OpenRemovableUnitCommandParams: public GNC::GCS::IComandoParams {
	public:
		typedef struct TRemovableUnit {
			std::string displayName;
			std::string path;
			bool hasDicomDir;
		}TRemovableUnit;

		OpenRemovableUnitCommandParams(wxWindow* pParent)
		{
			m_pParent = pParent;
		}
		virtual ~OpenRemovableUnitCommandParams()
		{
		}
	public:
		typedef std::list<TRemovableUnit> TListOfUnits;
		TListOfUnits units;		
		wxWindow* m_pParent;
	};

	
	class OpenRemovableUnitCommand : public GNC::GCS::IComando {
	public:
		OpenRemovableUnitCommand(OpenRemovableUnitCommandParams* pParams);

    protected:
		virtual void Execute();
		virtual void Update();
		bool OpenDicomDir();
		bool ScanRecursively();
		void SelectDrive(bool hasDicomDir);
		void LaunchOpenRecursively(const std::string& path);
		void LaunchOpenDicomDir(const std::string& path);
    protected:

		OpenRemovableUnitCommandParams* m_pOpenParams;
	};
}
