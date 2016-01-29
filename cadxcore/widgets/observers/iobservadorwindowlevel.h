/*
 *  
 *  $Id: iobservadorwindowlevel.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/math/geometry.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			namespace Observadores {
				class IObservadorWindowLevel {
				public:
					typedef GNC::GCS::Vector TVector;

					virtual ~IObservadorWindowLevel() {}

					virtual void OnStart() = 0;
					virtual void OnAjuste(TVector& start, TVector& stop) = 0;
					virtual void OnEnd() = 0;
				};
			};
		};
	};
};
