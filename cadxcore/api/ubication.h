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
