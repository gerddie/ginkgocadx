/*
 *  
 *  $Id: iconfiguracion.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <string>
#include <list>

class wxWindow;

class IDialogoConfiguracion {
public:
	IDialogoConfiguracion() {}
	virtual ~IDialogoConfiguracion() {}
	virtual void OnPropiedadCambiada() = 0;
};

class IPasoConfiguracion{
public:
	IPasoConfiguracion(IDialogoConfiguracion* pDialogo) {
		m_pDialogoConfiguracion = pDialogo;
	}

	virtual ~IPasoConfiguracion() {
		m_pDialogoConfiguracion = NULL;
	}
	
protected:
	IDialogoConfiguracion* m_pDialogoConfiguracion;
	
public:

	void OnPropiedadCambiada() {
		if(m_pDialogoConfiguracion != NULL) {
			m_pDialogoConfiguracion->OnPropiedadCambiada();
		}
	}

	virtual void Recargar() = 0;

	virtual wxWindow* GetPanel() = 0;

	virtual std::string GetTitle() = 0;

	virtual std::string GetCabecera() = 0;

	virtual bool Validar() = 0;

	virtual bool Guardar() = 0;
};
