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

#include <api/globals.h>
#include "wslider.h"
#include <main/managers/widgetsmanager.h>
#include <vtkgl.h>
#include <api/westilo.h>
#include <cmath>

//region "Implementacion de WSliderBuilder"
#include <api/icontexto.h>

GNC::GCS::Widgets::WSliderBuilder::WSliderBuilder(TWidgetsManager* pManager, TContract* pContract, const GNC::GCS::TriggerButton& buttonMask, long gid) : GNC::GCS::Widgets::IWidgetBuilder(pManager, buttonMask, gid)
{
	m_pContract = pContract;
	m_MouseDown = false;
	m_Estado = WBS_Ninguno;
	m_Dentro = true;
	m_InitialSlice = 0;
	m_Start = 0.0;
	m_Stop = 0.0;
	m_Changed = false;
}

GNC::GCS::Widgets::WSliderBuilder::~WSliderBuilder()
{
}

void GNC::GCS::Widgets::WSliderBuilder::OnMouseEvents(GNC::GCS::Events::EventoRaton& evento)
{
	if (!m_pManager) {
		return;
	}
	if (m_MouseDown && evento.ButtonUp(m_ButtonMask)) {
		m_MouseDown = false;
		if (m_Estado != WBS_Creando) {
			m_Estado = WBS_Ninguno;
			return;
		}
		m_Estado = WBS_Ninguno;

		if (m_Changed) {
			m_pManager->Modificado();
			ConsumirEvento();
		}
	}
	else if (evento.ButtonDown(m_ButtonMask)) {
		if (m_Estado != WBS_Ninguno) {
			return;
		}
		m_MouseDown = true;
		
		m_Changed = false;
		m_Start = evento.sY;
		m_Stop = m_Start;
		m_InitialSlice = m_pContract->GetSlice();		
		
		m_pManager->Modificado();
		
		m_Estado = WBS_Creando;
		evento.Skip(false);
	}
	else if (evento.Dragging() && m_MouseDown) {
		if (m_Estado != WBS_Creando) {
			return;
		}
		
		m_Stop = evento.sY;
		
		double delta = (m_Stop - m_Start) / 5.0;
		
		int newSlice = m_InitialSlice;
		int numberOfSlices = m_pContract->GetNumberOfSlices();
		if (delta != 0.0) {
			m_Changed = true;

			if (delta > 0.0)  {
				newSlice += std::floor(delta);
			}
			else {
				newSlice += std::ceil(delta);
			}			
			newSlice = std::max(0, std::min(newSlice,  numberOfSlices - 1));
		}
		if (m_pContract->GetSlice() != newSlice) {
			m_pContract->SetSlice(newSlice);
		} else if ((delta >= 1.0 || delta <= -1.0) && (newSlice == 0 || newSlice == numberOfSlices)) {
			m_Start = m_Stop;
			m_InitialSlice = newSlice;
		}
		evento.Skip(false);
	}
	else if( evento.Moving() ){
		GTRACE("Moviendose");
		m_NodoMoviendose = evento.iP;
		m_Estado = WBS_Ninguno;
	}

}

void GNC::GCS::Widgets::WSliderBuilder::OnKeyEvents(TEventoTeclado&)
{

}

void GNC::GCS::Widgets::WSliderBuilder::Render(GNC::GCS::Contexto3D* /*c*/)
{
}

GNC::GCS::Widgets::TipoCursor GNC::GCS::Widgets::WSliderBuilder::GetCursor()
{
	return GNC::GCS::Widgets::CUR_FLECHA;
}

//endregion
