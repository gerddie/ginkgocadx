/*
 *  
 *  $Id: imodulo.h $
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
