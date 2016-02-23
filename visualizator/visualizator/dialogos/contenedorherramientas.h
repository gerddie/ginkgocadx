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
#include <wx/control.h>
#include <wx/sizer.h>
namespace GNKVisualizator{
	namespace GUI{
		class ContenedorHerramientas : public wxControl { // public wxScrolledWindow {
		public:
			ContenedorHerramientas(wxWindow* pPadre);
			virtual ~ContenedorHerramientas();
			
			void InsertarPanel(wxWindow* child);
			void EliminarPanel(wxWindow* child);
			void ActualizarVirtualSize();
			
		protected:
			
		private:

			void OnSize(wxSizeEvent& event);

			wxBoxSizer* m_pSizer;
		};
	}
}
