/*
*  
*  $Id: imodelointegracion.cpp $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#include "imodelointegracion.h"
#include <main/controllers/controladorlog.h>
#include <main/controllers/configurationcontroller.h>

std::string GIL::IMapOfVarsModel::empty;

namespace GIL {
	bool IMapOfVarsModel::InsertVariable(const std::string& key, const std::string& value, const std::string& description)
	{
		if (find(key) != end()) {
			LOG_DEBUG("IMapOfVarsModel", "you are trying to replace variable " << key << " with value " << value);
			(*this)[key] = IVariableModel(key, value, description);
			return false;
		} else {
			(*this)[key] = IVariableModel(key, value, description);
			return true;
		}
	}
	
	const std::string& IMapOfVarsModel::GetValue(const std::string& key) const
	{	
		const_iterator it = find(key);
		if (it == end()) {
			LOG_WARN("IMapOfVarsModel", "you are trying to get a variable " << key << " that doesn't esists");
			return empty;
		} else {
			return (*it).second.GetValue();
		}
	}
	
	const std::string& IMapOfVarsModel::GetDescription(const std::string& key) const
	{
		const_iterator it = find(key);
		if (it == end()) {
			LOG_WARN("IMapOfVarsModel", "you are trying to get a variable " << key << " that doesn't esists");
			return empty;
		} else {
			return (*it).second.GetDescription();
		}
	}
	
	bool IMapOfVarsModel::Contains(const std::string& key) const
	{
		return find(key) != end();
	}

	bool IMapOfVarsModel::IsEmpty() {
		return (*this).size() == 0;
	}

	IMapOfVarsModel::TMapOfVarsModelIterator IMapOfVarsModel::Begin()
	{
		return (*this).begin();
	}

	IMapOfVarsModel::TMapOfVarsModelIterator IMapOfVarsModel::End()
	{
		return (*this).end();
	}

	IModeloIntegracion::IModeloIntegracion()
	{
		accion      = TA_Ninguna;
		atendido = false;
	}
}