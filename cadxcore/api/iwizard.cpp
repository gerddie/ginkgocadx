/*
 *  
 *  $Id: iwizard.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
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
