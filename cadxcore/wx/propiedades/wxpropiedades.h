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
#include <string>
#include <map>
#include <list>
#include <api/api.h>
#include "wxpropiedadesbase.h"

namespace GNC {
	namespace GUI {
		class EXTAPI wxPropiedades: public wxPropiedadesBase {
		public:
			typedef std::map< std::string , std::string > TMapaPropiedades;
			typedef std::list< TMapaPropiedades > TListaMapasPropiedades;

			wxPropiedades( wxWindow* parent, const std::string& nombre, const TListaMapasPropiedades& listaMapas);
			~wxPropiedades();

			void AddPage(const wxString& titulo, wxPanel* panel);

		};
	}
}

