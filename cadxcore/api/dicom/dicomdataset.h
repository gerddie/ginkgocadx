#pragma once
/*
*  
*  $Id: dicomdataset.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
*
*/
#ifndef DICOMDATASET_H
#define DICOMDATASET_H

#include <string>
#include <sstream>
#include <list>
#include <map>
#include <ostream>
#include <api/api.h>

namespace GIL {
	namespace DICOM {

		typedef std::map<std::string, std::string> ListaTags;

		class EXTAPI DicomDataset {

		private:
			

		public:

			typedef std::list<DicomDataset> DatasetList;

			DatasetList secuencias;
			DatasetList items;
			ListaTags tags;

			std::string tagName;

			DicomDataset() ;

			~DicomDataset() ;

			//añade referencias a las imagenes a las que hace referencia
			void AddReference(const std::string& sopClassUID, const std::string& sopInstanceUID);
			
			//añade referencias a las imagenes originales
			void AddReferenceSource(const std::string& sopClassUID, const std::string& sopInstanceUID);

			void clear();

			DicomDataset* buscar_secuencia(const char* tag);

			DicomDataset* buscar_secuencia(const std::string & tag);

			/* Obtiene un tag y lo asigna en el parámetro de salida valor.
			Devuelve true si el tag existe y false si el tag no existe */
			bool getTag(const std::string& tag, std::string & valor) const;

			bool getTag(const char* tag, std::string & valor) const;

			const std::string& getTag(const char* tag) const;
			const std::string& getTag(const std::string& tag) const;

			template<class T>
			inline T getTagAs(const std::string& tag, const T& defaultValue)
			{
				T ret = defaultValue;
				std::stringstream is(getTag(tag));
				is >> ret;
				return ret;
			}

			void dump(std::ostream& out, int indentLevel = 0) const;

			friend std::ostream & operator<<(std::ostream& out, const DicomDataset& j)
			{
				j.dump(out);
				return out;
			}
			
			friend std::ostream & operator<<(std::ostream& out, const DicomDataset* j)
			{
				if (j == NULL) {
					out << "[ NULL ]" << std::endl;
				} else {
					out << *j;
				}
				return out;
			}

		};
	}
}

#endif
