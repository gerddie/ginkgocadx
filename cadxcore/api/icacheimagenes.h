/*
 *  
 *  $Id: icacheimagenes.h $
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

#include <map>
#include <api/autoptr.h>
#include "ilock.h"

namespace GNC {
	namespace GCS {
		class IElementoCache {
		public:
			IElementoCache(){};
			virtual ~IElementoCache(){};

		};

		template <class TPixel>
		class IElementoCacheConcreto : public IElementoCache, public ILockable {
		public:
			IElementoCacheConcreto(int ancho, int alto, int numeroComponentes):IElementoCache()
			{
				m_ancho = ancho;
				m_alto = alto;
				m_numeroComponentes = numeroComponentes;
				m_PixelData = NULL;
			}

			virtual ~IElementoCacheConcreto() {
			}

			virtual TPixel* GetPixelData() {
				GNC::GCS::ILocker lock(this);
				if(m_PixelData == NULL) {
					Inicializar();
				}
				return m_PixelData;
			}

			virtual void Inicializar() = 0;

			virtual int GetAncho() {
				return m_ancho;
			}

			virtual int GetAlto() {
				return m_alto;
			}

			virtual int GetNumeroComponentes() {
				return m_numeroComponentes;
			}

		protected:
			TPixel* m_PixelData;
			int m_ancho;
			int m_alto;
			int m_numeroComponentes;
		};

		typedef std::map<std::string,GNC::GCS::Ptr<IElementoCache> > MapaImagenes;

		class ICacheImagenes: protected std::map<void*, MapaImagenes> {
		public:
			ICacheImagenes(){}
			~ICacheImagenes() {
				for(ICacheImagenes::iterator it = begin(); it!=end(); ++it) {
					(*it).second.clear();
				}
				clear();
			}

		};
	}
}
