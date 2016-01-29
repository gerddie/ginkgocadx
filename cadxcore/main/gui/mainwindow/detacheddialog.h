/*
 *  
 *  $Id: panelgrid.h $
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
