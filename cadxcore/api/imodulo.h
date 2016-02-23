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
namespace GNC {
	namespace GCS {
		class IModuleController;
	}
}

#ifdef __WXMSW__
	#ifdef MAKING_MODULES
		#define PRVEXT_API _declspec(dllexport)
		#define MODULO_API _declspec(dllexport) GNC::GCS::IModuleController* 
	#else // USING_MODULES
		#define PRVEXT_API _declspec(dllimport)
		#define MODULO_API _declspec(dllimport) GNC::GCS::IModuleController*
		//typedef GNC::GCS::IModuleController* (__cdecl* MODULO_ENTRY_POINT)(wxWindow* handle, GNC::GCS::IEntorno* pEntorno);
	#endif
#elif defined(__WXMAC__)
	#ifdef MAKING_MODULES
		#define PRVEXT_API __attribute__((visibility("default")))
		#define MODULO_API __attribute__((visibility("default"))) GNC::GCS::IModuleController*
	#else // USING_MODULES
		#define PRVEXT_API 
		#define MODULO_API GNC::GCS::IModuleController*
		typedef GNC::GCS::IModuleController* (*MODULO_ENTRY_POINT)(WXHWND handle);
	#endif
#else
	#ifdef MAKING_MODULES
		#define PRVEXT_API __attribute__((visibility("default")))
		#define MODULO_API __attribute__((visibility("default"))) GNC::GCS::IModuleController*
	#else // USING_MODULES
		#define PRVEXT_API 
		#define MODULO_API GNC::GCS::IModuleController*
		//typedef GNC::GCS::IModuleController* (*MODULO_ENTRY_POINT)(void* handle);
	#endif
#endif


#define MODULO_ENTRY_POINT_NAME "init"
