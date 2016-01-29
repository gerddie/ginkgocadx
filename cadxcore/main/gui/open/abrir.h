/*
 *  
 *  $Id: abrir.h $
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
#include <string>
#include <list>

#include <main/controllers/pacscontroller.h>

namespace GNC {
	namespace GUI {
		class Abrir
		{

		public:
			Abrir();
			~Abrir();

			static void AbrirFichero(wxWindow* pParent, const std::list<std::string>* directLoads = NULL);
			static void AbrirDirectorio(wxWindow* pParent);

		protected:
		};
	}
}
