/*
 *  
 *  $Id: ipreviewpanel.h $
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
#include <vector>
#include <list>
#include <map>

#include <api/ievento.h>
#include <api/autoptr.h>
#include <eventos/eventosginkgo.h>

class wxWindow;
namespace GNC {
	namespace GCS {
		class IVista;
	}
	namespace GUI {
		//nodo seleccionable
		class ISeleccionablePreview {
		protected:
			bool m_seleccionado;
		public:			
			ISeleccionablePreview(){
				m_seleccionado = false;
			}
			~ISeleccionablePreview(){
			}
			bool EstaSeleccionado()
			{
				return m_seleccionado;
			}
			virtual void Seleccionar(bool seleccionar, bool force=false) = 0;
		};

		class INodoHistorial {
		public:
			typedef std::list<GNC::GUI::INodoHistorial*> TChildrenList;
			INodoHistorial(INodoHistorial* pPadre) 
			{
				m_pPadre = pPadre;
			}
			~INodoHistorial() 
			{
				m_pPadre = NULL;
			}
			INodoHistorial* GetParentNode()
			{
				return m_pPadre;
			}
			
			virtual void GetNodeSize(int & /*x*/, int &/*y*/) {};
			virtual wxWindow* GetWxWindow() = 0;

		protected:
			INodoHistorial* m_pPadre;
		};

		//eventos variados...
		//evento de seleccion
		namespace Events {
			class EventoSeleccionarHistorial: public GNC::GCS::Events::IEvent
			{
			public:
				EventoSeleccionarHistorial(GNC::GCS::IVista* pView):GNC::GCS::Events::IEvent(ginkgoEVT_Core_HistorialSeleccionar, 0, 100, pView)
				{
					m_Nombre = "SeleccionarHistorial";
					m_pSeleccionable=NULL;
				}

				EventoSeleccionarHistorial(GNC::GCS::IVista* pView, ISeleccionablePreview* pSeleccionable ):GNC::GCS::Events::IEvent(ginkgoEVT_Core_HistorialSeleccionar, 0 , 100, pView)
				{
					m_pSeleccionable = pSeleccionable;
				}

				~EventoSeleccionarHistorial()
				{
				}

				ISeleccionablePreview* GetSeleccionable()
				{
					return m_pSeleccionable;
				}

			protected:
				ISeleccionablePreview* m_pSeleccionable;
			};
			
			class EventoSetFocusHistorial: public GNC::GCS::Events::IEvent
			{
			public:

				EventoSetFocusHistorial(GNC::GCS::IVista* pView):GNC::GCS::Events::IEvent(ginkgoEVT_Core_SetFocus, 0, 100, pView)
				{
					m_Nombre = "SetFocusHistorial";
				}

				~EventoSetFocusHistorial()
				{
				}
			protected:
			};

		}//eventos
	}
}
