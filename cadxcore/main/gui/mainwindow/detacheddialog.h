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
#include <main/gui/mainwindow/ventanaprincipalbase.h>
#include <api/controllers/icontroladorvistas.h>

class VentanaPrincipal;

namespace GNC {
	namespace GUI {
		class DetachedDialog: public DetachedDialogBase, public GNC::GCS::IObservadorVistas
		{
		public:
			DetachedDialog(wxWindow* pParent, VentanaPrincipal* pVentanaPrincipal);
			~DetachedDialog();
			void AddPanel(wxWindow* pWin, const wxString& titulo);
			void SetTitulo(const wxString& titulo);
			wxWindow* GetWindow();
			wxString GetTitulo();

		protected:
			void OnBotonTitulo(wxCommandEvent &);
			virtual void OnCloseDialogo(wxCloseEvent &event);
			virtual void OnVistaActivada(GNC::GCS::IVista* pVista);
			virtual void RefrescarVista(GNC::GCS::IVista* pVista);

			wxWindow* m_pWindow;
			VentanaPrincipal* m_pVentanaPrincipal;
		};
	}
}
