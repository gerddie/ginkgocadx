#pragma once
/*
 *  
 *  $Id: ubication.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef UBICATION_H
#define UBICATION_H
#include <string>

#include <api/api.h>

namespace GIL {
	class EXTAPI Ubicacion
	{
	public:

		std::string       Titulo;						
		std::string       Ruta;
		std::string       Descripcion;
		bool              Monitorize;
		bool              CleanBefore;
		bool              CleanAfter;

		Ubicacion();

		Ubicacion(const std::string& titulo, const std::string& ruta, const std::string& descripcion, bool monitorize, bool cleanbefore, bool cleanafter);

		Ubicacion(const Ubicacion& otro);

		~Ubicacion();

		void Update(const std::string& ruta, const std::string& desc, bool monitorize, bool cleanbefore, bool cleanafter);

		Ubicacion& operator=(const Ubicacion& otro);
	};
}
#endif