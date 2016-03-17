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


#include "ubication.h"

GIL::Ubicacion::Ubicacion() :
        Monitorize(false),
        CleanBefore(false),
        CleanAfter(false)
{
}

GIL::Ubicacion::Ubicacion(const std::string& titulo, const std::string& ruta, const std::string& descripcion, bool monitorize, bool cleanbefore, bool cleanafter)
{
        this->Titulo = titulo;
        Update(ruta, descripcion, monitorize, cleanbefore, cleanafter);

}

GIL::Ubicacion::Ubicacion(const GIL::Ubicacion& otro)
{
        *this = otro;
}

GIL::Ubicacion::~Ubicacion()
{

}

void GIL::Ubicacion::Update(const std::string& ruta, const std::string& desc, bool monitorize, bool cleanbefore, bool cleanafter)
{
        this->Ruta        = ruta;
        this->Descripcion = desc;
        this->Monitorize  = monitorize;
        this->CleanBefore = cleanbefore;
        this->CleanAfter  = cleanafter;
}

GIL::Ubicacion& GIL::Ubicacion::operator=(const GIL::Ubicacion& otro)
{
        this->Titulo      = otro.Titulo;
        this->Ruta        = otro.Ruta;
        this->Descripcion = otro.Descripcion;
        this->Monitorize	= otro.Monitorize;
        this->CleanBefore	= otro.CleanBefore;
        this->CleanAfter	= otro.CleanAfter;

        return *this;
}

