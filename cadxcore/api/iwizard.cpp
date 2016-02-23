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

#include <wx/string.h>
#include "iwizard.h"

	IWizard::~IWizard()
	{
	}

	IPasoWizard::IPasoWizard(IWizard * pWizard) :
		m_pListaFicheros(NULL),
		m_pWizard(pWizard)
	{
	}

	IPasoWizard::~IPasoWizard()
	{
	}

	bool IPasoWizard::Attach()
	{
		return true;
	}

	bool IPasoWizard::Detach()
	{
		return true;
	}

	std::string IPasoWizard::GetSubTitle()
	{
		return "";
	}

	bool IPasoWizard::IsSkipped(){
		return false;
	}

	bool IPasoWizard::AllowCancel() 
	{
		return true;
	}

	void IPasoWizard::OnCancelled() {}

	void IPasoWizard::AsignarListaFicheros(ListaFicheros &lista)
	{
		m_pListaFicheros=&lista;
	}

	void IPasoWizard::SiguientePaso(){
		m_pWizard->SiguientePaso();
	}

	void IPasoWizard::EnableSiguiente(bool enable){
		m_pWizard->EnableSiguiente(enable);
	}

	void IPasoWizard::EnableAnterior(bool enable){
		m_pWizard->EnableAnterior(enable);
	}

	void IPasoWizard::EnableCancelar(bool enable){
		m_pWizard->EnableCancelar(enable);
	}
	//helpers
	bool IPasoWizard::ValidarDouble(std::string strNumero, double min, double max, bool obligatorio, bool estricto){
		if(strNumero != ""){
			wxString wxStr = wxString::FromUTF8(strNumero.c_str());
			double valor;
			if(!wxStr.ToDouble(&valor)) {
				return false;
			} else {
				if (estricto) {
					if(valor <= min || valor >= max){
						return false;
					}
				}
				else {
					if(valor < min || valor > max){
						return false;
					}
				}
			}
			return true;
		} else {
			if(obligatorio){
				return false;
			} else {
				return true;
			}
		}
	}

	bool IPasoWizard::ValidarLong(std::string strNumero, long min, long max, bool obligatorio, bool estricto){
		if(strNumero != ""){
			wxString wxStr = wxString::FromUTF8(strNumero.c_str());
			long valor;
			if(!wxStr.ToLong(&valor)) {
				return false;
			} else {
				if (estricto) {
					if(valor<=min || valor >=max){
						return false;
					}
				}
				else {
					if(valor < min || valor > max){
						return false;
					}
				}
			}
			return true;
		} else {
			if(obligatorio){
				return false;
			} else {
				return true;
			}
		}
	}
