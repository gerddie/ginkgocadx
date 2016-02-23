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
#include <stddef.h>

namespace GIL {
	namespace DICOM {
		class DicomDataset;
		class TagPrivadoUndefined;
		class TipoMetaInfo;
		class TipoPrivateTags;
	}
}

namespace GIL {
	namespace DICOM {



		class IInspectCallBack {
		public:

			virtual ~IInspectCallBack(){}

			virtual void ObtenerJerarquiaInserccion(DicomDataset& /*lista*/)
			{
			}

			virtual void Inspect(std::string& clave, std::string& desc, std::string& valor) = 0;
			virtual void Done() = 0;
		};
			
		class IDICOMManager {
		public:

			virtual ~IDICOMManager() {}

			/*Almacena la jerarquia cargada en un fichero*/
			virtual bool AlmacenarFichero(const std::string& inputFile, IInspectCallBack* pICallback = NULL, bool debug = false)=0;
			
			/*Actualiza la jerarquia cargada*/
			virtual int ActualizarJerarquia(DicomDataset& base)=0;

			/*actualiza los tags privados*/
			virtual int ActualizarTagsPrivados(TipoPrivateTags& tags)=0;

			/*anonimiza los tags privados*/
			virtual void AnonimizarTagsPrivados() = 0;

			/*carga los tags privados*/
			virtual int CargarTagsPrivados(GIL::DICOM::TipoPrivateTags& tags)=0;
			
			/*carga la MetaInfo*/
			virtual int CargarMetaInfo(const std::string& inputFile, GIL::DICOM::TipoMetaInfo& tags)=0;
			
			/* Obtiene la jerarquia de campos DICOM del fichero */
			virtual bool CargarFichero(const std::string& inputFile, GIL::DICOM::DicomDataset& jerarquia, bool cargarSoloTagsInfo = true, IInspectCallBack* pICallback = NULL)=0;

			/*Carga el fichero y responde a primitivas get tag, util para pillar tags especificos sin cargar toda la jerarquia*/
			virtual bool CargarFichero(const std::string& inputFile, bool cargarSoloTagsInfo = true)=0;
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, std::string & valor)=0;
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario)=0;
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, float& valor) = 0;
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, int& valor) = 0;
			virtual bool GetTag(unsigned int grupo,unsigned int elemento, GIL::DICOM::DicomDataset& secuencia) = 0;
			
			virtual bool FindTag(unsigned int grupo,unsigned int elemento, TagPrivadoUndefined& tagBinario)=0;
			
			/* returns the oid name or empty string if not found */
			virtual std::string GetOIDName(const std::string &oid) = 0;
			
			/* devuelve la descripcion de este tag*/
			virtual std::string GetDescription(const std::string &clave) const = 0;

			virtual std::string GetNewUID() = 0;
		};
	}
}
