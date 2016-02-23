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

#include <wx/icon.h>
#include <wx/msgdlg.h>
#include <wx/ginkgostyle/ginkgostyle.h>
#include "calibracion.h"
#include <sstream>
#include <main/entorno.h>
#include <api/controllers/ieventscontroller.h>
#include <api/iwidgetsrenderer.h>
#include <vtk/vtkginkgoimageviewer.h>
#include <eventos/modificacionimagen.h>
#include <resources/ginkgoresourcesmanager.h>
#include <api/math/geometry3d.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <limits>

GNC::GCS::Widgets::Dialogos::Calibracion::Calibracion(wxWindow* parent, TWidget* pWidget, TManager* pManager) : CalibracionBase(parent)
{
	wxIcon icono;
	icono.CopyFromBitmap(GinkgoResourcesManager::MenuIcons::GetIcoCalibrarImagen());
	this->SetIcon(icono);

	m_pWidget = pWidget;
	m_pManager = pManager;

	float metricaDistancia = m_pWidget->m_Distancia;

	if (metricaDistancia > 1000.0f) {
		metricaDistancia = metricaDistancia / 1000.0f;
		m_unidades->SetSelection(3);
	}
	else if (metricaDistancia > 10.0f) {
		metricaDistancia = metricaDistancia / 10.0f;
		m_unidades->SetSelection(2);
	}
	else if (metricaDistancia < 1.0f) {
		metricaDistancia = metricaDistancia * 1000.0f;
		m_unidades->SetSelection(0);
	}
	else {
		m_unidades->SetSelection(1);
	}

	std::ostringstream os;

	os << metricaDistancia;

	m_ControlDistancia->SetValue(wxString::FromUTF8(os.str().c_str()));
}

GNC::GCS::Widgets::Dialogos::Calibracion::~Calibracion()
{
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnDialogoKeyDown( wxKeyEvent& event )
{
	if (event.GetKeyCode() == WXK_ESCAPE) {
		Close();
	}
	else if (event.GetKeyCode() == WXK_RETURN) {
		if (ValidarDouble(m_ControlDistancia->GetValue(),std::numeric_limits<GNC::GCS::Vector::TComp>::min(), 1000000)){
			Close();
			Calibrar();
		}
	}
	else {
		event.Skip();
	}
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnControlDistanciaKeyDown( wxKeyEvent& event )
{
	if (event.GetKeyCode() == WXK_ESCAPE) {
		Close();
	}
	if (event.GetKeyCode() == WXK_RETURN) {
		if (ValidarDouble(m_ControlDistancia->GetValue(),std::numeric_limits<GNC::GCS::Vector::TComp>::min(), 1000000)){
			Close();
			Calibrar();
		}
	}
	else {
		event.Skip();
	}
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnBotonCancelarClick( wxCommandEvent& /*event*/ )
{
	Close();
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnBotonAceptarClick( wxCommandEvent& /*event*/ )
{
	//validamos el campo
	if(ValidarDouble(m_ControlDistancia->GetValue(),std::numeric_limits<GNC::GCS::Vector::TComp>::min(), 1000000)){
		m_panelMedida->Hide();
		m_pPanelEspera->Show(true);
		Layout();
		if(Calibrar()){
			Close();
		} else {
			m_panelMedida->Show(true);
			m_pPanelEspera->Hide();
			Layout();
		}
	}
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnActivate(wxActivateEvent& /*event*/)
{
	m_panelMedida->SetFocus();
}

void GNC::GCS::Widgets::Dialogos::Calibracion::OnClickEnMenu(wxCommandEvent& /*event*/){
	Center();
	ShowModal();
}

bool GNC::GCS::Widgets::Dialogos::Calibracion::Calibrar()
{

	if (m_pWidget == NULL) {
		return false;
	}

	double oldSpacing[3];                    // Espaciado antiguo
	double newSpacing[3];                    // Nuevo espaciado

	GNC::GCS::Vector3D worldPosition0,worldPosition1, wDiff;

	double oldDist = m_pWidget->m_Distancia;  // Distancia antigua
	double newDist;                           // Nueva distancia

	std::string dist(m_ControlDistancia->GetValue().ToUTF8());
	std::istringstream is (dist);
	is >> newDist;

	if(m_unidades->GetSelection() == 0){
		newDist = newDist / 1000.0f;
	} else if(m_unidades->GetSelection() == 2){
		newDist = newDist * 10.0f;
	} else if(m_unidades->GetSelection() == 3){
		newDist = newDist * 1000.0f;
	}

	if(!ValidarDouble(newDist,(double)0.0000001f, (double)100000000.0f)) {
		return false;
	}

	newDist = std::abs(newDist);

	//proporcion...
	double proporcion = newDist / oldDist;

	m_pManager->GetRendererActivo()->m_pImageViewer->GetSpacing(oldSpacing);

	//const double raizDe3 = std::sqrt(3.0f);
	for (int i = 0; i< 3; ++i)
	{
		newSpacing[i] = proporcion * oldSpacing[i];
	}

	if(!ValidarDouble(newSpacing[0],std::numeric_limits<GNC::GCS::Vector::TComp>::min(), (double)1000000.0f)
		|| !ValidarDouble(newSpacing[1],std::numeric_limits<GNC::GCS::Vector::TComp>::min(), (double)1000000.0f)
		|| !ValidarDouble(newSpacing[2],std::numeric_limits<GNC::GCS::Vector::TComp>::min(), (double)1000000.0f)){
		return false;
	} else {
		//std::cout << "Nuevo espaciado: " << newSpacing[0] << ", " << newSpacing[1] << std::endl;

		GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(m_pManager->GetVista(),GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada,m_pWidget->GetVID());
		pEvt->SetOldSpacing(oldSpacing);
		pEvt->SetNewSpacing(newSpacing);

		double oldOrigin[3];                    // Origen antiguo
		m_pManager->GetRendererActivo()->m_pImageViewer->GetOrigin(oldOrigin);
		pEvt->SetOldOrigin(oldOrigin);
		pEvt->SetNewOrigin(oldOrigin);

		GNC::Entorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);
		return true;
	}
}

bool GNC::GCS::Widgets::Dialogos::Calibracion::ValidarDouble(wxString wxStr, double min, double max ){
	if(wxStr != wxEmptyString ){
		double valor;
		if(!wxStr.ToDouble(&valor)) {
			wxMessageBox(_("The calibration value must be a decimal number"), _("Error"), wxICON_ERROR);
			return false;
		} else {
			if(valor <= min ){
				wxMessageBox(_("Calibration value is too low"), _("Error"), wxICON_ERROR);
				return false;
			}
			if(valor >= max){
				wxMessageBox(wxT("Calibration value is too high"), wxT("Error"), wxICON_ERROR);
				return false;
			}
		}
		return true;
	}
	wxMessageBox(_("Enter the calibration value"), _("Error"), wxICON_ERROR);
	return false;
}

bool GNC::GCS::Widgets::Dialogos::Calibracion::ValidarDouble(double valor, double min, double max ){
	if(valor<=min ){
		wxMessageBox(_("The calibration value is too low"), _("Error"), wxICON_ERROR);
		return false;
	}
	if(valor >=max){
		wxMessageBox(_("The calibration value is too high"), _("Error"), wxICON_ERROR);
		return false;
	}
	return true;
}
