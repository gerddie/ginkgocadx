/*
 *  
 *  $Id: apitest.h 5003 2012-07-11 13:50:13Z tovar $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-10 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once

#if defined(_WINDOWS)
	#pragma warning (disable : 4231)
	#pragma warning (disable : 4275)
	#pragma warning (disable : 4251)

	#if defined(LIBTEST_EXPORTS)
		#define EXTTEST _declspec(dllimport)
	#else
		#define EXTTEST _declspec(dllexport)
	#endif
#else
	#if defined(LIBTEST_EXPORTS)
		#define EXTTEST
	#else
		#define EXTTEST __attribute__((visibility("default")))
	#endif
#endif