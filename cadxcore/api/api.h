#pragma once
/*
 *  
 *  $Id: api.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef API_H
#define API_H

	#if defined(_WINDOWS)
		#pragma warning (disable : 4231)
		#pragma warning (disable : 4275)
		#pragma warning (disable : 4251)
		#define DEPRECATED(reason) __declspec(deprecated(reason))
		
		#if defined(APIEXPORT)
			#if !defined(MONOLITIC)
				#define EXTAPI _declspec(dllexport)
				#define EXTAPI_TEMPLATE
			#else
				#define EXTAPI
				#define EXTAPI_TEMPLATE
			#endif
			#define NOINCLUDE
		#else
			#if !defined(MONOLITIC)
				#define EXTAPI _declspec(dllimport)
				#define EXTAPI_TEMPLATE extern
			#else
				#define EXTAPI
				#define EXTAPI_TEMPLATE
			#endif
			#define NOINCLUDE #error This file could not been included externally
		#endif
	#else
		#define DEPRECATED(reason) __attribute__((deprecated))
		#if defined(APIEXPORT)
			#define EXTAPI __attribute__((visibility("default")))
			#define EXTAPI_TEMPLATE
			#define NOINCLUDE
		#else
			#define EXTAPI
			#define EXTAPI_TEMPLATE extern
			#define NOINCLUDE #error This file could not been included externally
		#endif
	#endif

#endif