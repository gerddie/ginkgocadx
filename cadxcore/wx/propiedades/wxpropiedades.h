/*
 *  
 *  $Id: wxpropiedades.h $
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

