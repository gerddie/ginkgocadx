/*
 *  
 *  $Id: icontroladorextensiones.h $
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
#include <map>

#include <api/api.h>
#include <api/iexception.h>

class PrivateExtension;

namespace GADVAPI {

	class EXTAPI IPrivateExtensionWrapper {

	public:

		virtual ~IPrivateExtensionWrapper() {}

		#if defined(MONOLITIC)
		virtual bool Load(PrivateExtension* pExt) = 0;
		#else
		virtual bool Load() = 0;
		#endif
		virtual bool UnLoad() = 0;
		virtual bool IsLoaded() const = 0;

		virtual bool IsActivated() const 
		{
			return true;
		}

		virtual const std::string& GetProvider() const = 0;
		virtual const std::string& GetDescription() const = 0;

		virtual const std::string& GetUpdateURL() const = 0;

		virtual const std::string& GetError() const = 0;

		virtual PrivateExtension* GetExtension() = 0;

	};

	class EXTAPI PrivateExtensionManager : private std::map<std::string, IPrivateExtensionWrapper*>
	{
	public:
		
		typedef std::map<std::string, IPrivateExtensionWrapper*> Base;
		typedef Base::iterator iterator;

		PrivateExtensionManager();
		~PrivateExtensionManager();

		IPrivateExtensionWrapper* GetExtension(const std::string& nombre);

		void Scan();

		void UnLoadAll();

		iterator begin();
		iterator end();
		
	};
}

namespace GNC {
	namespace GCS {

		class EXTAPI IExtensionException : public IException {
		public:			

		protected:
			IExtensionException(const std::string& msg, const std::string& component="GNC", bool fatal = true) throw();

			~IExtensionException() throw();
		};

		class EXTAPI IControladorExtensiones {
		public:
			virtual GADVAPI::PrivateExtensionManager& GetPrivateExtensionsManager() = 0;

		protected:
			IControladorExtensiones();
			~IControladorExtensiones();
		};
	}
}

