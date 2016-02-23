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

#if 0
	#if defined (_GINKGO_DEBUG)

		#include <iostream>
		#if defined(_WINDOWS)
		#define _WINSOCKAPI_ 
		#include <windows.h>
		#endif
		
		#define GDEBUG(expr) std::cout << "D: " << expr << "   @ " << __FILE__ << ":" << __LINE__ << std::endl;

	#else // !defined (_GINKGO_DEBUG)
		 #define GDEBUG(expr) /* nada */
	#endif

	#if defined (_GINKGO_TRACE)
		 #define GTRACE(expr) std::cout << "T: " << expr << "   @ " << __FILE__ << ":" << __LINE__ << std::endl;

	#else // !defined (_GINKGO_DEBUG)
		 #define GTRACE(expr) /* nada */
	#endif
#else
	#define GTRACE(expr) /* nada */
#endif

#if defined(__WXGTK__) || defined(__WXMAC__) || defined(__WXMSW__)

#if defined (_WIN32)
	#define TOPATH(stringwx) std::string((stringwx.To8BitData().data()==NULL)?"":stringwx.To8BitData())
	#define FROMPATH(stringstd) wxString::From8BitData(stringstd.c_str())
#else
	#define TOPATH(stringwx) std::string(stringwx.ToUTF8())
	#define FROMPATH(stringstd) wxString::FromUTF8(stringstd.c_str())
#endif

#endif
