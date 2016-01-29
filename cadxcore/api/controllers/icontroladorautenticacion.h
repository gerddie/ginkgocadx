/*
 *  
 *  $Id: icontroladorautenticacion.h $
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
#include <string>
#include <api/api.h>
#include <api/iexception.h>

class IModeloDicom;
namespace GNC {
	namespace GCS {
		class IEntorno;
	}
	class IProxyNotificadorProgreso;
}

namespace GSEC {
	namespace Auth {

		class AuthException : GNC::GCS::IException {
		public:
			AuthException(const std::string& msg, const std::string& componente="GSEC/Auth", bool _fatal = true) : GNC::GCS::IException(msg, componente, _fatal)
			{
			}
			~AuthException() throw()
			{
			}
		};

		class EXTAPI IControladorAutenticacion {
		public:

			virtual bool Auth (const std::string& usuario, const std::string& password) = 0;

		protected:

			IControladorAutenticacion()
			{
			}

			~IControladorAutenticacion()
			{
			}
		};
	}
}
