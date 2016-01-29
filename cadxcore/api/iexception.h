#pragma once
/*
 *  
 *  $Id: iexception.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef IEXCEPTION_H
#define IEXCEPTION_H

#include <string>
#include <exception>

#include <api/api.h>

namespace GNC {
	namespace GCS {
		class EXTAPI IException : public std::exception {
		public:
			IException(const std::string& msg, const std::string& component="GNC", bool fatal = true) throw();

			virtual ~IException() throw();

			const std::string& GetComponent() const;

			const std::string& GetCause() const;

			virtual const char* what() const throw();

			std::string GetFullCause() const;

			bool IsFatal()  const;

			operator std::string () const;

			inline const std::string str() const { return (std::string) *this; }
			
		protected:
			std::string cause;
			std::string component;
			bool fatal;
		};

		class EXTAPI IRequestingException : public IException {
		public:
			IRequestingException(const std::string& msg, const std::string& component="GNC", bool fatal = true) throw();
			virtual ~IRequestingException() throw();

			virtual bool shouldExit() = 0;
		};
		
	}
}
#endif