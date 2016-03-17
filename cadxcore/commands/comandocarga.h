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

#pragma once
#include <api/icommand.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/autoptr.h>

namespace GNC
{
namespace GCS
{
class IVista;
}
}

namespace GADAPI
{
class ComandoCarga;

class ComandoCargaParams : public GNC::GCS::IComandoParams, public GNC::GCS::IObservadorVistas
{
public:
        ComandoCargaParams(GNC::GCS::IVista* pVista, const bool loadFirst);

        virtual ~ComandoCargaParams();

        virtual void OnVistaDestruida(GNC::GCS::IVista* pVista);

        GNC::GCS::IVista*               IVista;
        ComandoCarga*                   Padre;
        bool									  LoadFirst;

};

class ComandoCarga : public GNC::GCS::IComando
{
public:

        ComandoCarga(ComandoCargaParams* pParams);

protected:

        virtual void Execute();
        virtual void Update();

protected:

        ComandoCargaParams* CargaParams;
        std::string         Error;

};
}
