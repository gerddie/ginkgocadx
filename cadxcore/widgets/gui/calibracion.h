/*
 *  
 *  $Id: calibracion.h $
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
#include "calibracionbase.h"
#include <widgets/wrule.h>

namespace GNC {
	namespace GCS {
		namespace Widgets {
			namespace Dialogos {
				class Calibracion : public CalibracionBase {
				public:
					typedef GNC::GCS::Widgets::WRule TWidget;
					typedef GNC::GCS::IWidgetsManager TManager;

					Calibracion(wxWindow* parent, TWidget* pWidget, TManager* pManager);
					~Calibracion();

					virtual void OnClickEnMenu(wxCommandEvent& event);

				protected:
					virtual void OnDialogoKeyDown( wxKeyEvent& event );

					virtual void OnControlDistanciaKeyDown( wxKeyEvent& event );

					virtual void OnBotonCancelarClick( wxCommandEvent& event );

					virtual void OnBotonAceptarClick( wxCommandEvent& event );

					virtual void OnActivate(wxActivateEvent& event);

					bool Calibrar();

					bool ValidarDouble(wxString strNumero, double min, double max);
					bool ValidarDouble(double valor, double min, double max);

					TWidget*   m_pWidget;
					TManager*  m_pManager;
				};
			}
		}
	}
}
