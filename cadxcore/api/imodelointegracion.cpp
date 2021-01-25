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

#include "imodelointegracion.h"
#include <main/controllers/controladorlog.h>
#include <main/controllers/configurationcontroller.h>

std::string GIL::IMapOfVarsModel::empty;

namespace GIL
{
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
                LOG_WARN("IMapOfVarsModel", "you are trying to get a variable " << key << " that doesn't exists");
                return empty;
        } else {
                return (*it).second.GetValue();
        }
}

const std::string& IMapOfVarsModel::GetDescription(const std::string& key) const
{
        const_iterator it = find(key);
        if (it == end()) {
                LOG_WARN("IMapOfVarsModel", "you are trying to get a variable " << key << " that doesn't exists");
                return empty;
        } else {
                return (*it).second.GetDescription();
        }
}

bool IMapOfVarsModel::Contains(const std::string& key) const
{
        return find(key) != end();
}

bool IMapOfVarsModel::IsEmpty()
{
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
