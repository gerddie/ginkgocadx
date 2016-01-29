/*
 *  
 *  $Id: controladoracciones.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/api.h>
#include <api/controllers/icontroladoracciones.h>
#include <api/controllers/icontroladorvistas.h>
#include <list>
#include <map>

class wxCriticalSection;
namespace GNC {
	namespace GCS {
		class EXTAPI ControladorAcciones: public IControladorAcciones, public IObservadorVistas
		{
		public:
			static ControladorAcciones* Instance();
			static void FreeInstance();

			virtual void PushAccion(GNC::GCS::IVista* pVista, GNC::GCS::Ptr<Accion> pAccion);
			virtual void Deshacer(GNC::GCS::IVista* pVista);
			virtual void Hacer(GNC::GCS::IVista* pVista);
			virtual bool PuedeHacer(GNC::GCS::IVista* pVista, std::string& nombreAccion);
			virtual bool PuedeDeshacer(GNC::GCS::IVista* pVista, std::string& nombreAccion);

		protected:
			static ControladorAcciones* m_pInstance;
			ControladorAcciones();
			~ControladorAcciones();

			void OnVistaDestruida(GNC::GCS::IVista* pVista);

			typedef std::list<GNC::GCS::Ptr<Accion> > TListaAcciones;
			typedef struct TPilaDeshacer {
				TListaAcciones m_listaAcciones;
				TListaAcciones::iterator m_iteradorAcciones;
				TPilaDeshacer()
				{
					m_iteradorAcciones = m_listaAcciones.end();
				}
				~TPilaDeshacer()
				{
					m_listaAcciones.clear();
				}
			} TPilaDeshacer;
			typedef std::map<IVista*, TPilaDeshacer*> TMapaVista;
			TMapaVista m_mapaAcciones;

			wxCriticalSection* m_pCriticalSection;


		};
	}
}
