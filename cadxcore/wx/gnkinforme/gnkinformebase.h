/*
 *  
 *  $Id: gnkinformebase.h $
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
#include "gnkinformebasewx.h"
#include <api/controllers/ipacscontroller.h>
#include <string>
#include <map>
#include <list>

namespace GNC {
	namespace GUI {

		class EXTAPI GnkInformeBase: public GNKInformeBaseWx {
		public:
			typedef enum TEstado {
				NO_INICIALIZADO,
				NO_EXISTE,
				ALMACENADO,
				ALMACENADO_CON_MODIFICACIONES_PENDIENTES
			} TEstado;

			typedef struct TDatoDemografico {
				std::string clave;
				std::string valor;

				TDatoDemografico(const std::string& clave, const std::string& valor)
				{
					this->clave = clave;
					this->valor = valor;
				}
			} TDatoDemografico;
			typedef std::list< TDatoDemografico > TListaDatosDemograficos;
			typedef std::list< wxWindow* > TListaPanelesCustom;

			GnkInformeBase( wxWindow* parent);
			~GnkInformeBase();

			virtual void CargarInforme(const std::string& pathFicheroInforme);

			void SetTags(GNC::GCS::Ptr<GIL::DICOM::DicomDataset>& pBase, GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags> pTagsPrivadosComunes = NULL);

			void AddDatosDemograficos(const TListaDatosDemograficos& datosDemograficos);
			void SetPanelesCustom(const TListaPanelesCustom& listaPaneles);

		protected:
			virtual void OnConsolidarClick(wxCommandEvent& event);
			virtual void ConsolidarComun(GNC::GCS::Ptr<GIL::DICOM::TipoPrivateTags>& tagsPrivadosComunes, const std::string& medico);
			void OnComentariosText(wxCommandEvent &event);

			virtual void ActualizarEstado(TEstado estado);

			virtual void OnConsolidar() = 0;
			wxWindow* GetParentPanelesCustom();
			std::string m_pathInforme;
			std::string m_cadenaConsolidaciones;
			std::string m_comentarios;
			TEstado m_estado;
		};
	}
}

