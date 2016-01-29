/*
*  
*  $Id: inotificadorcalibrado.h $
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

namespace GNC{
	namespace GUI{
		struct TPuntoCalibrado;
	}
}

namespace GADAPI {

	class INotificadorCalibrado{
	public:
		INotificadorCalibrado(){}
		virtual ~INotificadorCalibrado(){}

		virtual void SetPuntosCalibrado(std::list<GNC::GUI::TPuntoCalibrado>& puntos) = 0;
	};
}
