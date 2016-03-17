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
#include <string>
#include <list>

class wxWindow;

class IDialogoConfiguracion
{
public:
        IDialogoConfiguracion() {}
        virtual ~IDialogoConfiguracion() {}
        virtual void OnPropiedadCambiada() = 0;
};

class IPasoConfiguracion
{
public:
        IPasoConfiguracion(IDialogoConfiguracion* pDialogo)
        {
                m_pDialogoConfiguracion = pDialogo;
        }

        virtual ~IPasoConfiguracion()
        {
                m_pDialogoConfiguracion = NULL;
        }

protected:
        IDialogoConfiguracion* m_pDialogoConfiguracion;

public:

        void OnPropiedadCambiada()
        {
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
