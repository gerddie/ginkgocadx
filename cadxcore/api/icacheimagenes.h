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

#include <map>
#include <api/autoptr.h>
#include "ilock.h"

namespace GNC
{
namespace GCS
{
class IElementoCache
{
public:
        IElementoCache() {};
        virtual ~IElementoCache() {};

};

template <class TPixel>
class IElementoCacheConcreto : public IElementoCache, public ILockable
{
public:
        IElementoCacheConcreto(int ancho, int alto, int numeroComponentes):IElementoCache()
        {
                m_ancho = ancho;
                m_alto = alto;
                m_numeroComponentes = numeroComponentes;
                m_PixelData = NULL;
        }

        virtual ~IElementoCacheConcreto()
        {
        }

        virtual TPixel* GetPixelData()
        {
                GNC::GCS::ILocker lock(this);
                if(m_PixelData == NULL) {
                        Inicializar();
                }
                return m_PixelData;
        }

        virtual void Inicializar() = 0;

        virtual int GetAncho()
        {
                return m_ancho;
        }

        virtual int GetAlto()
        {
                return m_alto;
        }

        virtual int GetNumeroComponentes()
        {
                return m_numeroComponentes;
        }

protected:
        TPixel* m_PixelData;
        int m_ancho;
        int m_alto;
        int m_numeroComponentes;
};

typedef std::map<std::string,GNC::GCS::Ptr<IElementoCache> > MapaImagenes;

class ICacheImagenes: protected std::map<void*, MapaImagenes>
{
public:
        ICacheImagenes() {}
        ~ICacheImagenes()
        {
                for(ICacheImagenes::iterator it = begin(); it!=end(); ++it) {
                        (*it).second.clear();
                }
                clear();
        }

};
}
}
