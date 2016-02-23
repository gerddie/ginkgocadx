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
