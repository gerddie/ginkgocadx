/*
 *  Ginkgo CADx Project
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

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vector>
#include <map>
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

namespace GNKVisualizator{
	
	typedef std::map<int,bool> TEstadoOverlay;

	typedef struct TOverlay {
		std::string nombre;
		int indice;
		vtkSmartPointer<vtkImageData> img;

		TOverlay() :
			nombre("Overlay"),
			indice(0)
		{
		}

		~TOverlay() {
			this->img = NULL;
		}

		TOverlay(std::string nombre,int indice)
		{
			this->nombre = nombre;
			this->indice = indice;
		}

		TOverlay(const TOverlay& otro){
			this->nombre = otro.nombre;
			this->indice = otro.indice;
			this->img = otro.img;
		}

		TOverlay& operator = (const TOverlay& otro) 
		{
			this->nombre = otro.nombre;
			this->indice = otro.indice;
			this->img = otro.img;			
			return *this;
		}
	} TOverlay;
	
	typedef std::list<TOverlay> TListaOverlays;
};
