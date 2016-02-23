#pragma once
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
