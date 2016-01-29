/*
 *  
 *  $Id$
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
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
