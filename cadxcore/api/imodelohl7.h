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

#include <list>
#include <map>
#include <sstream>

#include <api/api.h>
#include <api/globals.h>
#include <api/iexception.h>

#define RECORD_SEPARATOR "\x0D"

namespace GIL {

	namespace HL7 {

		std::string DefaultEscapeER7(const std::string& str);

		/* Este namespace contiene una implementación completa de mensajeria HL7 generica */

		class HL7Exception : public GNC::GCS::IException {
		public:
			HL7Exception(const std::string& msg, const std::string& component="HL7", bool fatal = true) throw() : GNC::GCS::IException(msg ,component, fatal) {}
		};

		class HL7XMLException : public GNC::GCS::IException {
		public:
			HL7XMLException(const std::string& msg, const std::string& component="XMLIntegration", bool fatal = true) throw() : GNC::GCS::IException(msg ,component, fatal) {}			
		};

		namespace Util {
			inline std::list<std::string> split(const std::string& str, const std::string& separador)
			{
				std::list<std::string> lista;
				std::string::size_type start = 0;
				std::string::size_type end = 0;

				while ((end = str.find (separador, start)) != std::string::npos) {
					lista.push_back (str.substr (start, end-start));
					start = end + separador.size();
				}
				lista.push_back (str.substr (start));
				return lista;
			}

			inline std::string tolower(const std::string& input_string)
			{
				std::string str = input_string;

				// iterate through all the characters.  If uppercase,
				// convert to lowercase.
				for (std::string::size_type i = 0 ; i < str.length() ; ++i)
					if (isupper(str[i]) )
						str[i] |= 32;

				return str;
			}
		}


		class Component {

		private:
			size_t       m_Num;
			std::string  m_Header;
			std::string  m_Value;
			bool         m_Null;

		public:
			Component()
			{
				m_Num = 0;
				m_Null = true;
			}

			Component(size_t num)
			{
				m_Num = num;
				m_Null = true;
			}

			Component(size_t num, const std::string& header) : m_Num(num), m_Header(header), m_Null(true)
			{
			}

			Component(size_t num, const std::string& header, const std::string& value) : m_Num(num), m_Header(header), m_Value(value), m_Null(false)
			{
			}

			Component(const Component& otro)
			{
				*this = otro;
			}

			Component& operator=(const Component& otro)
			{
				m_Num    = otro.m_Num;
				m_Header = otro.m_Header;
				m_Value  = otro.m_Value;
				m_Null  = otro.m_Null;
				return *this;
			}

			Component& operator=(const std::string& valor)
			{
				m_Value = valor;
				m_Null = false;
				return *this;
			}

			bool operator==(const std::string& valor)
			{
				return m_Value == valor;
			}

			bool operator==(const char* valor)
			{
				return m_Value == valor;
			}

			operator const std::string& ()
			{
				return m_Value;
			}

			size_t GetNum() const
			{
				return m_Num;
			}

			const std::string& GetName() const
			{
				return m_Header;
			}

			const std::string& GetValue() const
			{
				return m_Value;
			}

			bool IsNull() const
			{
				return m_Null;
			}

			friend std::ostream& operator<<(std::ostream& out, const Component& sf)
			{
				if (!sf.IsNull()) {
					if (sf.m_Value.size() == 0) {
						out << "\"\"";
					}
					else {
						out << sf.m_Value;
					}
				}
				return out;
			}

			friend std::ostream& operator<<(std::ostream& out, const Component* sf)
			{
				out << *sf;
				return out;
			}
		};

		class Field : public std::map<size_t, Component> {

		private:
			typedef std::map<size_t, Component> Base;

		protected:
			size_t m_Num;
			std::string  m_Header;
			std::string  m_CS;       // component separator
			std::string  m_ES;       // escape separator
			std::string  m_SCS;      // subcomponent separator

		public:

			Field()
			{
				m_Num = 0;
			}

			Field(size_t num, const std::string& header = "", const std::string& sfs ="^", const std::string& es = "\\", const std::string& cs = "&") : m_Num(num), m_Header(header), m_CS(sfs), m_ES(es), m_SCS(cs)
			{
			}

		Field(const Field& otro):std::map<size_t, Component>(otro)
			{
				*this = otro;
			}

			Field& operator=(const Field& otro)
			{
				m_Num    = otro.m_Num;
				m_Header = otro.m_Header;
				m_CS     = otro.m_CS;
				m_ES     = otro.m_ES;
				m_SCS    = otro.m_SCS;
				Base::operator = ((const Base&)otro);
				return *this;
			}

			Field& operator<<(const Component& sf)
			{
				Field::operator[] (sf.GetNum()) = sf;
				return *this;
			}

			Component GetComponent(size_t num) const
			{
				if (num == 0) {
					throw HL7Exception("Indice de componente invalido: 0", "MODEL");
				}

				const_iterator it;
				for (it = begin(); it != end(); ++it) {
					size_t numComponent = (*it).first;
					if (numComponent == num) {
						const Component& c = (*it).second;
						return Component(c);
					}
				}
				return Component(num, "");
			}

			const std::string& GetSFS() const
			{
				return m_CS;
			}

			void SetSFS(const std::string& sfs)
			{
				m_CS = sfs;
			}

			size_t GetNum() const
			{
				return m_Num;
			}

			const std::string& GetName() const
			{
				return m_Header;
			}

			Component& operator[](size_t indice)
			{
				if (indice == 0) {
					throw HL7Exception("Indice de sub-campo invalido: 0", "MODEL");
				}

				Base& t = *this;

				size_t numLast = size();

				if ( indice > numLast) {
					for (size_t i = numLast + 1; i <= indice; i++) {
						t[i] = Component(i);
					}
				}

				return t[indice];
			}

			friend std::ostream& operator<<(std::ostream& out, const Field& f)
			{
				bool first = true;
				for(Field::const_iterator it= f.begin(); it != f.end(); ++it)
				{
					if (first) {
						first = false;
						out << (*it).second;
					}
					else {
						out << f.m_CS << (*it).second;
					}
				}
				return out;
			}

			friend std::ostream& operator<<(std::ostream& out, const Field* f)
			{
				return out << *f;
			}

			const std::string EscapeER7(const std::string& str) const;
			const std::string UnEscapeER7(const std::string& str) const;
		};

		class Segment : protected std::map<size_t, std::list<Field> > {

		private:
			typedef std::map<size_t, std::list<Field> > Base;

		protected:
			std::string m_Header;
			std::string m_RS;
			std::string m_FS;
			std::string m_CS;
			std::string m_FRS;
			std::string m_ES;
			std::string m_SCS;

		public:

			Segment()
			{
			}

			Segment(const std::string& header, const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^", const std::string& frs = "~", const std::string& es = "\\", const std::string& cs = "&" ) : m_Header(header), m_RS(rs), m_FS(fs), m_CS(sfs), m_FRS(frs), m_ES(es), m_SCS(cs)
			{
			}

		Segment(const Segment& otro):std::map<size_t, std::list<Field> >(otro)
			{
				*this = otro;
			}

			Segment& operator=(const Segment& otro)
			{
				m_Header       = otro.m_Header;
				m_RS           = otro.m_RS;
				m_FS           = otro.m_FS;
				m_CS           = otro.m_CS;
				m_FRS          = otro.m_FRS;
				m_ES           = otro.m_ES;
				m_SCS          = otro.m_SCS;
				Base::operator = ((const Base&)otro);
				return *this;
			}

			Field& operator<<=(const Field& f)
			{
				size_t indice = f.GetNum();

				if (indice == 0) {
					throw HL7Exception("Indice de campo invalido: 0", "MODEL");
				}

				Base& t = *this;

				size_t numLast = size();

				if ( indice > numLast) {
					for (size_t i = numLast + 1; i < indice; i++) {
						t[i] = std::list<Field>();
						t[i].push_back(Field(i));
					}
				}

				t[indice].clear();
				t[indice].push_back(f);
				return t[indice].back();
			}

			Field& operator<<(const Field& f)
			{
				size_t indice = f.GetNum();

				if (indice == 0) {
					throw HL7Exception("Indice de campo invalido: 0", "MODEL");
				}

				Base& t = *this;

				size_t numLast = size();

				if ( indice > numLast) {
					for (size_t i = numLast + 1; i < indice; i++) {
						t[i] = std::list<Field>();
						t[i].push_back(Field(i));
					}
				}

				t[indice].push_back(f);
				return t[indice].back();
			}

			Field GetField(size_t num, size_t repnum = 1) const
			{
				if (num == 0) {
					throw HL7Exception("Indice de campo invalido: 0", "MODEL");
				}
				if (repnum == 0) {
					throw HL7Exception("Indice de repeticion de campo invalido: 0", "MODEL");
				}

				const_iterator it;
				size_type i;
				for (it = begin(), i = 1; it != end(); ++it, i++) {
					if (i == num) {
						const std::list<Field>& fl = (*it).second;
						std::list<Field>::const_iterator itr = fl.begin();
						if (itr != fl.end()) {
							return Field(*itr);
						}
						else {
							return Field(num, "", m_RS, m_FS, m_CS);
						}
					}
				}
				return Field(num, "", m_RS, m_FS, m_CS);
			}

			const std::string& GetSFS() const
			{
				return m_CS;
			}

			void SetSFS(const std::string& sfs)
			{
				m_CS = sfs;
			}

			const std::string& GetName() const
			{
				return m_Header;
			}

			Field& operator[](size_t indice)
			{
				if (indice == 0) {
					throw HL7Exception("Indice de campo invalido: 0", "MODEL");
				}

				Base& t = *this;

				size_t numLast = size();

				if ( indice > numLast) {
					for (size_t i = numLast + 1; i <= indice; i++) {
						t[i] = std::list<Field>();
						t[i].push_back(Field(i));
					}
				}
				return t[indice].front();
			}

			Field& RepeatedField(size_t indiceCampo, size_t indiceRepeticion)
			{
				if (indiceCampo == 0) {
					throw HL7Exception("Indice de campo invalido: 0", "MODEL");
				}
				if (indiceRepeticion == 0) {
					throw HL7Exception("Indice de repeticion de campo invalido: 0", "MODEL");
				}

				Base& t = *this;

				size_t numLast = size();

				if ( indiceCampo > numLast) {
					for (size_t i = numLast + 1; i <= indiceCampo; i++) {
						t[i] = std::list<Field>();
						t[i].push_back(Field(i));
					}
				}

				std::list<Field>& fields = t[indiceCampo];
				numLast = fields.size();

				if ( indiceRepeticion > numLast) {
					for (size_t i = numLast + 1; i <= indiceRepeticion; i++) {
						fields.push_back(Field(i));
					}
				}

				std::list<Field>::iterator it;
				size_t i;

				for (i = 1, it = fields.begin(); it != fields.end() && i != indiceRepeticion; ++it, i++);

				if ( it == fields.end() || i != indiceRepeticion) {
					throw HL7Exception("Error al localizar el indice de repeticion", "MODEL");
				}

				return *it;
			}

			friend std::ostream& operator<<(std::ostream& out, const Segment& s)
			{
				out << s.GetName();
				size_t i = 1;

				if (s.m_Header == "MSH") {
					bool firstField = true;
					for(Segment::const_iterator it = s.begin(); it != s.end(); ++it, i++)
					{
						if (firstField) {
							firstField = false;
						}
						else {
							bool fieldRepeated = false;
							for (std::list<Field>::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
								if (fieldRepeated) {
									out << "~" << (*it2);
								}
								else {
									out << s.m_FS <<  (*it2);
									fieldRepeated = true;
								}
							}
						}
					}
				}
				else {
					for(Segment::const_iterator it = s.begin(); it != s.end(); ++it, i++)
					{
						bool fieldRepeated = false;
						for (std::list<Field>::const_iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
							if (fieldRepeated) {
								out << "~" << (*it2);
							}
							else {
								out << s.m_FS <<  (*it2);
								fieldRepeated = true;
							}
						}
					}
				}
				out << s.m_RS;
				return out;
			}

			friend std::ostream& operator<<(std::ostream& out, const Segment* s)
			{
				return out << *s;
			}

			EXTAPI Field& ParseField (size_t fieldNum, const std::string& record, bool unscape = true);

			EXTAPI const std::string EscapeER7(const std::string& str) const;
			EXTAPI const std::string UnEscapeER7(const std::string& str) const;

		};

		class Message : public std::list<Segment> {

		private:

			typedef std::list<Segment> Base;
			std::string m_Header;  // message header
			std::string m_RS;      // record separator
			std::string m_FS;      // (|) Field separator
			std::string m_CS;      // (^) Component separator
			std::string m_FRS;     // (~) Field repeat separator
			std::string m_ES;      // (\) Escape separator
			std::string m_SCS;     // (&) SubComponent separator

		public:

			Message(const std::string& header = "", const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs ="^") : m_Header(header), m_RS(rs), m_FS(fs), m_CS(sfs)
			{
			}

		Message(const Message& otro):std::list<Segment>(otro)
			{
				*this = otro;
			}

			Message& operator=(const Message& otro)
			{
				m_FRS          = otro.m_FRS;
				m_ES           = otro.m_ES;
				m_SCS          = otro.m_SCS;
				m_Header       = otro.m_Header;
				m_RS           = otro.m_RS;
				m_FS           = otro.m_FS;
				m_CS           = otro.m_CS;
				Base::operator = ((const Base&)otro);
				return *this;
			}

			static Message FromER7(const std::string& msgStr)
			{
				Message msg;
				msg.LoadFromER7(msgStr);
				return msg;
			}

			void LoadFromXML(const std::string& /*msgStr*/)
			{
			}

			void LoadFromER7(const std::string& msgStr)
			{
				Clear();

				std::list<std::string> records;

				std::string::size_type l_loc = 0; // Last Location
				std::string::size_type c_loc = std::string::npos; // Current Location
				bool cont = true;

				while(cont) {
					c_loc = msgStr.find(RECORD_SEPARATOR, l_loc);
					if (c_loc == std::string::npos) {
						const std::string& record = msgStr.substr(l_loc);
						if (record.size() > 0) {
							records.push_back(record);
						}
						cont = false;
					}
					else {
						const std::string& record = msgStr.substr(l_loc, c_loc - l_loc);
						if (record.size() > 0) {
							records.push_back(record);
						}
						l_loc = c_loc + 1;
					}
				}

				if (records.empty())
				{
					throw HL7Exception(std::string("Error al interpretar la cabecera del mensaje no se han encontrado registros"));
				}
				std::list<std::string>::iterator itr = records.begin();
				std::string& mshrecord = *itr;

				Message& msg = *this;

				if (mshrecord.size() >= 8 && mshrecord[0] == 'M' && mshrecord[1] == 'S' && mshrecord[2] == 'H')
				{
					m_FS  = mshrecord[3];
					m_CS  = mshrecord[4];
					m_FRS = mshrecord[5];
					m_ES  = mshrecord[6];
					m_SCS = mshrecord[7];

					Segment& segment = (msg << Segment("MSH", m_RS, m_FS, m_CS, m_FRS, m_ES, m_SCS));
					segment << Field(1, "MSH.1") << Component(1, "", m_FS);
					segment << Field(2, "MSH.2") << Component(1, "", m_CS + m_FRS + m_ES + m_SCS);

					ParseER7Segment(segment, mshrecord, 9, 3);

				}
				else {
					throw HL7Exception(std::string("Error al interpretar la cabecera del mensaje: ") + mshrecord.substr(0,8));
				}

				for (itr++; itr != records.end(); ++itr)
				{

					std::string& record = *itr;
					//std::cout << "Parsing Record: [" << record << "]" << std::endl;

					Segment& segment = (msg << Segment(record.substr(0,record.find(m_FS, 0)), m_RS, m_FS, m_CS, m_FRS, m_ES, m_SCS));
					ParseER7Segment(segment, record, 4);
				}
			}

		private:

			void ParseER7Segment(Segment& segment, const std::string& record, std::string::size_type offsetFirstField=5, int numFirstField=1)
			{
				std::string::size_type l_loc = offsetFirstField;
				std::string::size_type c_loc = std::string::npos;

				std::string::size_type start = std::string::npos;
				std::string::size_type size = std::string::npos;
				bool cont = true;

				size_t fieldNum = numFirstField;

				//std::cout << "Record = [" << record << "]" << std::endl;

				if (offsetFirstField >= record.size()) {
					return;
				}

				while (cont) {

					c_loc = record.find(m_FS, l_loc);
					if (c_loc == std::string::npos) {
						start = l_loc;
						size  = std::string::npos;
						cont  = false;
					}
					else {
						start = l_loc;
						size  = c_loc - l_loc;
						l_loc = c_loc + 1;
					}

					const std::string& str = record.substr(start, size);

					//std::cout << "Field " << fieldNum << " = [" << str << "] ";

					// Repeticion de campos.

					std::string::size_type l_locfr = 0;
					std::string::size_type c_locfr = std::string::npos;

					std::string::size_type startfr = std::string::npos;
					std::string::size_type endfr = std::string::npos;
					bool contfr = true;

					while (contfr) {
						c_locfr = str.find(m_FRS, l_locfr);
						if (c_locfr == std::string::npos) {
							startfr = l_locfr;
							endfr = c_locfr;
							l_locfr = std::string::npos;
							contfr = false;
						}
						else {
							startfr = l_locfr;
							endfr = c_locfr - l_locfr;
							l_locfr = c_locfr + 1;
						}

						segment.ParseField(fieldNum, str.substr(startfr, endfr));

					}

					//std::cout << std::endl;

					fieldNum++;
				}
			}

		public:

			EXTAPI const std::string EscapeER7(const std::string& str) const;
			EXTAPI const std::string UnEscapeER7(const std::string& str) const;

			void Clear()
			{
				Base::clear();
			}

			Segment& operator<<(const Segment& s)
			{
				push_back(s);
				return back();
			}

			/* Devuelve el primer mensaje encontrado con la cabecera parametrizada. */
			Segment& operator[] (const std::string& key)
			{
				return Segments(key, 1);
			}

			/* Devuelve el num-avo mensaje encontrado con la cabecera parametrizada. */
			Segment& Segments(const std::string& key, size_t num)
			{
				if (num == 0) {
					throw HL7Exception("Indice de segmento invalido: 0", "MODEL");
				}

				iterator it;
				size_type i;
				for (it = begin(), i = num; it != end()  && i > 0; ++it) {
					Segment& s = *it;
					if (s.GetName() == key) {
						--i;
					}
					if (i == 0) {
						return s;
					}
				}
				return (*this) << Segment(key, m_RS, m_FS, m_CS);
			}

			Segment GetSegment(const std::string& key, size_t num = 1) const
			{
				if (num == 0) {
					throw HL7Exception("Indice de segmento invalido: 0", "MODEL");
				}

				const_iterator it;
				size_type i;
				for (it = begin(), i = num; it != end() && i > 0; ++it) {
					const Segment& s = *it;
					if (s.GetName() == key) {
						--i;
					}
					if (i == 0) {
						return Segment(s);
					}
				}
				return Segment(key, m_RS, m_FS, m_CS);
			}

			void SetHeader(const std::string& header)
			{
				m_Header = header;
			}

			const std::string& GetHeader()
			{
				return m_Header;
			}

			friend std::ostream& operator<<(std::ostream& out, const Message& m) {
				for(Message::const_iterator it = m.begin(); it != m.end(); ++it)
				{
					out << *it;
				}
				return out;
			}

			friend std::ostream& operator<<(std::ostream& out, const Message* m) {
				return out << *m;
			}

		public:
			typedef enum ErrorCode {
				EC_MessageAccepted          = 0,
				EC_SegmentSequenceError     = 100,
				EC_RequiredFieldMissing     = 101,
				EC_DataTypeError            = 102,
				EC_TableValueNotFound       = 103,
				EC_UnsuportedMessageType    = 200,
				EC_UnsuportedProcessingId   = 201,
				EC_UnsupportedEventCode     = 202,
				EC_UnsupportedVersionId     = 203,
				EC_UnknownKeyIdentifier     = 204,
				EC_DupplicateKeyIdentifier  = 205,
				EC_ApplicationRecordLocked  = 206,
				EC_ApplicationInternalError = 207
			} ErrorCode;

			static const std::string GetErrorCodeDescription(ErrorCode errorCode)
			{
				std::stringstream ss;
				ss << errorCode << ": ";

				switch (errorCode) {
					case EC_MessageAccepted:
						ss << "Mensaje aceptado";
						break;
					case EC_SegmentSequenceError:
						ss << "Error en la secuencia del segmento";
						break;
					case EC_RequiredFieldMissing:
						ss << "Campo requerido inexistente";
						break;
					case EC_DataTypeError:
						ss << "Error en el tipo de dato";
						break;
					case EC_TableValueNotFound:
						ss << "Valor de la tabla no encontrado";
						break;
					case EC_UnsuportedMessageType:
						ss << "Tipo de mensaje no soportado";
						break;
					case EC_UnsuportedProcessingId:
						ss << "Id de procesamiento no soportado";
						break;
					case EC_UnsupportedEventCode:
						ss << "Codigo de evento no soportado";
						break;
					case EC_UnsupportedVersionId:
						ss << "Id de version no soportado";
						break;
					case EC_UnknownKeyIdentifier:
						ss << "Identificador desconocido";
						break;
					case EC_DupplicateKeyIdentifier:
						ss << "Identificador clave duplicado";
						break;
					case EC_ApplicationRecordLocked:
						ss << "Registro de la aplicacion bloqueado";
						break;
					case EC_ApplicationInternalError:
						ss << "Error interno de la aplicacion";
						break;
					default:
						ss << "Codigo invalido";
						break;
				}
				return ss.str();
			}

			static const std::string GetErrorCodeAsString(ErrorCode errorCode)
			{
				std::stringstream ss;
				ss << errorCode;
				std::string errorCodeString = ss.str();

				switch (errorCode) {
					case EC_MessageAccepted:
					case EC_SegmentSequenceError:
					case EC_RequiredFieldMissing:
					case EC_DataTypeError:
					case EC_TableValueNotFound:
					case EC_UnsuportedMessageType:
					case EC_UnsuportedProcessingId:
					case EC_UnsupportedEventCode:
					case EC_UnsupportedVersionId:
					case EC_UnknownKeyIdentifier:
					case EC_DupplicateKeyIdentifier:
					case EC_ApplicationRecordLocked:
					case EC_ApplicationInternalError:
						return errorCodeString;
					default:
						throw HL7Exception(std::string("[Error Code] invalido: ") + errorCodeString, "MODEL/LOGIC");
				}
			}

			static ErrorCode GetErrorCodeFromErrorCodeAsString(const std::string& errorCodeString)
			{
				if (errorCodeString == "0") {
					return EC_MessageAccepted;
				}
				else if (errorCodeString == "100") {
					return EC_SegmentSequenceError;
				}
				else if (errorCodeString == "101") {
					return EC_RequiredFieldMissing;
				}
				else if (errorCodeString == "102") {
					return EC_DataTypeError;
				}
				else if (errorCodeString == "103") {
					return EC_TableValueNotFound;
				}
				else if (errorCodeString == "200") {
					return EC_UnsuportedMessageType;
				}
				else if (errorCodeString == "201") {
					return EC_UnsuportedProcessingId;
				}
				else if (errorCodeString == "202") {
					return EC_UnsupportedEventCode;
				}
				else if (errorCodeString == "203") {
					return EC_UnsupportedVersionId;
				}
				else if (errorCodeString == "204") {
					return EC_UnknownKeyIdentifier;
				}
				else if (errorCodeString == "205") {
					return EC_DupplicateKeyIdentifier;
				}
				else if (errorCodeString == "206") {
					return EC_ApplicationRecordLocked;
				}
				else if (errorCodeString == "207") {
					return EC_ApplicationInternalError;
				}
				else {
					throw HL7Exception(std::string("[Error Code] invalido: ") + errorCodeString, "MODEL/LOGIC");
				}
			}

			typedef enum ErrorSeverity {
				ES_Warning     = 0,
				ES_Information = 1,
				ES_Error       = 2
			} ErrorSeverity;

			static const char* GetErrorSeverityString(ErrorSeverity es)
			{
				switch (es) {
					case ES_Information:
						return "I";
						break;
					case ES_Error:
						return "E";
						break;
					case ES_Warning:
						return "W";
					default:
						{
							std::stringstream ss;
							ss << es;
							throw HL7Exception(std::string("[Error Severity] invalido: ") + ss.str(), "MODEL/LOGIC");
						}
				}
			}

			static ErrorSeverity GetErrorSeverityFromString(const std::string& ess)
			{
				if (ess == "I") {
					return ES_Information;
				}
				else if (ess == "E") {
					return ES_Error;
				}
				else if (ess == "W") {
					return ES_Warning;
				}
				else {
					throw HL7Exception(std::string("[Error Severity String] invalida: ") + ess, "MODEL/LOGIC");
				}
			}
		};
	}
}
