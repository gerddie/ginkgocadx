/*
 *  
 *  $Id: contenedorherramientas.h $
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
