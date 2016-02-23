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
#include <map>
#include <list>

#include <api/api.h>
#include <api/iexception.h>
#include <api/dicom/dicomdataset.h>

namespace GIL {
	namespace DICOM {
		class IDICOMImg2DCM;
		class IDICOMManager;
	}
}

class DcmElement;

namespace GIL {
	namespace DICOM {

		class IDICOMImg2DCM;
		class IDICOMManager;

		class EXTAPI TipoTagPrivado {
		public:
			TipoTagPrivado();
			virtual ~TipoTagPrivado();
			virtual std::string ToString() = 0;
			virtual DcmElement* ToElement(unsigned int group, unsigned int element) = 0;
		};

		class EXTAPI TagPrivadoString: public TipoTagPrivado {
		public:
			TagPrivadoString(const std::string& valor);
			virtual ~TagPrivadoString();

			virtual std::string ToString();

			virtual DcmElement* ToElement(unsigned int group, unsigned int element);
		protected:
			std::string Valor;
		};


		class EXTAPI TagPrivadoUndefined: public TipoTagPrivado {
		public:
			TagPrivadoUndefined();

			TagPrivadoUndefined(unsigned int size);

			TagPrivadoUndefined(unsigned char* valor, unsigned int size);

			virtual ~TagPrivadoUndefined();

			TagPrivadoUndefined(const TagPrivadoUndefined& otro);

			TagPrivadoUndefined& operator = (const TagPrivadoUndefined& otro) ;

			void Copiar(void* ptrCopia, int size) ;

			virtual std::string ToString() ;

			unsigned char* GetValor();

			unsigned int GetSize();

			virtual DcmElement* ToElement(unsigned int group, unsigned int element);
		protected:
			unsigned char* Valor;
			unsigned int Size;
		};

		class EXTAPI TipoPrivateTags {
		public:
			typedef std::map<unsigned char, TipoTagPrivado* > ListaTags;
			std::string UIDModulo;

			TipoPrivateTags();

			TipoPrivateTags(const std::string& uidModulo);

			~TipoPrivateTags();

			ListaTags& GetListaTags();

			void Vaciar();

			void DeleteTag(unsigned char tag);

			bool GetTag(unsigned char tag, std::string& valor) ;

			TagPrivadoUndefined* GetTagUndefined(unsigned char tag);

			void SetTag(unsigned char tag, const std::string& valor);

			void SetTag(unsigned char tag, unsigned char* valor, int longitud) ;

			TagPrivadoUndefined* NewTagUndefined(unsigned char tag, int longitud);
		protected:			
			ListaTags Tags;

		};
		
		class EXTAPI TipoMetaInfo {
		public:
			typedef std::map<std::string, std::string> ListaTags;
			
			ListaTags tags;
			
			~TipoMetaInfo();
			
			/* Obtiene un tag y lo asigna en el parámetro de salida valor.
			 Devuelve true si el tag existe y false si el tag no existe */
			bool getTag(const std::string& tag, std::string & valor);
			
			std::string getTag(const std::string& tag);
			
			template<class T> T getTagAs(const std::string& tag, const T& defaultValue);
		};

		class EXTAPI I2DException : public GNC::GCS::IException {
		public:
			I2DException(const std::string& msg, const std::string& component="GIL/I2D", bool fatal = true) throw();
                        virtual ~I2DException() throw();
		};
	}
}
