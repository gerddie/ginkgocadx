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
#pragma once
#include <string>
#include <list>
#include <api/api.h>

class EXTAPI IWizard {
public:
	virtual ~IWizard();

	virtual void SiguientePaso() = 0;
	virtual void EnableSiguiente(bool enable) = 0;

	virtual void EnableAnterior(bool enable) = 0;

	virtual void EnableCancelar(bool enable) = 0;
};

class EXTAPI IPasoWizard{
public:
	typedef std::list<std::string> ListaFicheros;
protected:

	ListaFicheros* m_pListaFicheros;
	IWizard* m_pWizard;

public:
	IPasoWizard(IWizard * pWizard);
	virtual ~IPasoWizard();

	virtual bool Attach();

	virtual bool Detach();

	virtual std::string GetTitle()=0;

	virtual std::string GetSubTitle();

	virtual bool Siguiente()=0;

	virtual bool Anterior()=0;

	virtual bool Cancelar()=0;

	virtual bool Validar()=0;

	virtual bool IsSkipped();

	virtual bool AllowCancel() ;

	virtual void OnCancelled();

	virtual void AsignarListaFicheros(ListaFicheros &lista);

	virtual void SiguientePaso();

	virtual void EnableSiguiente(bool enable);

	virtual void EnableAnterior(bool enable);

	virtual void EnableCancelar(bool enable);
	//helpers
	virtual bool ValidarDouble(std::string strNumero, double min, double max, bool obligatorio = false, bool estricto = true);

	virtual bool ValidarLong(std::string strNumero, long min, long max, bool obligatorio = false, bool estricto = true);
};
