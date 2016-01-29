#pragma once
/*
*
*  $Id: iwindowlevelcontract.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef IWINDOWLEVELCONTRACT_H
#define IWINDOWLEVELCONTRACT_H
#include <list>
#include <vector>

#include <api/api.h>
#include <export/contracts/iwidgetscontract.h>

namespace GIL {
	namespace DICOM {
		class DicomDataset;
	}
}

namespace GNC {
	namespace GCS {
		class EXTAPI IWindowLevelContract : public IWidgetsContract {
		public:

			class EXTAPI WindowLevel{

			public:

				typedef enum TWL_TYPE {
					TWLT_FILE,
					TWLT_PREDEFINED,
					TWLT_USER_DEFINED
				} TWL_TYPE;
				
				WindowLevel();

				WindowLevel(const std::string& label, const double& window,const double& level, TWL_TYPE type);

				WindowLevel(const WindowLevel& otro);

				WindowLevel& operator=(const WindowLevel& otro);

				TWL_TYPE			m_type;
				std::string         m_label;
				double              m_window;
				double              m_level;
			};

			typedef std::vector<WindowLevel> ListaWindowLevels;

			public:

			IWindowLevelContract();

			~IWindowLevelContract();

			virtual bool HasCurrentWindowLevel() ;

			virtual void SetUserDefinedWindowLevel(double window, double level);

			virtual WindowLevel GetCurrentWindowLevel();

			virtual bool HasDefaultWindowLevel();

			virtual WindowLevel GetAndSetDefaultWindowLevel();

			virtual void RemoveUserDefined();

			virtual void Setup(const GIL::DICOM::DicomDataset& base);

			virtual ListaWindowLevels& GetWindowLevelList() = 0;
			virtual std::string GetLabelCurrent() = 0;
			virtual void SetLabelCurrent(const std::string& labelCurrent) = 0;
			virtual void DoAutoCalculateWindowLevel() = 0;
			virtual void DoResetWindowLevel() = 0;
			virtual void DoSetWindowLevel(double window, double level) = 0;
			virtual double GetCurrentWindow();
			virtual double GetCurrentLevel();
		};
	}
}

#endif
