/*
 *  
 *  $Id: imodelodicom.h $
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
#include <string>
#include <list>
#include <exception>

#include <api/globals.h>
#include <api/iexception.h>
#include <api/internationalization/internationalization.h>


class IModeloDicom;
class IModeloEstudio;
class IModeloSerie;
class IModeloPaciente;

namespace GIL {
	namespace DICOM {
		class EXTAPI ModelException : public GNC::GCS::IException {
		public:
			ModelException(const std::string& msg, const std::string& componente="GIL/DICOM/Model", bool fatal = true) : GNC::GCS::IException(msg, componente, fatal)
			{
				this->cause = msg;
				this->component = componente;
				this->fatal = fatal;
			}
			~ModelException() throw()
			{
			}

			const std::string& GetComponent() const
			{
				return component;
			}

			const std::string& GetCause() const {
				return cause;
			}

			std::string GetFullCause() const {
				return *this;
			}

			bool IsFatal()  const {
				return fatal;
			}

			operator std::string () const {
				return std::string("Excepcion en componente ") + GetComponent() + std::string(": ")  + GetCause();
			}

			friend std::ostream& operator<<(std::ostream& out, const ModelException& ex)
			{
				out << (const std::string&) ex;
				return out;
			}

			friend std::ostream& operator<<(std::ostream& out, const ModelException* ex)
			{
				if (ex != NULL) {
					out << *ex;
				}
				else {
					out << "(NULL)";
				}
				return out;
			}

		protected:
			std::string cause;
			std::string component;
			bool fatal;
		};
	}
}
class IModeloImagen {
public:

	IModeloImagen()
	{
		m_pSeriePadre = NULL;
		m_pModeloPadre = NULL;
	}

	IModeloImagen(IModeloDicom* pModeloPadre, IModeloSerie* pSeriePadre,
		const std::string& UID, const std::string& instanceNumber, const std::string& pathImagen = "")
	{
		m_pSeriePadre = pSeriePadre;
		m_pModeloPadre = pModeloPadre;
		m_UID = UID;
		m_pathImagen = pathImagen;
		m_instanceNumber = instanceNumber;
	}

	IModeloImagen( const IModeloImagen& o) {
		m_pSeriePadre = o.m_pSeriePadre;
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o.m_UID;
		m_pathImagen = o.m_pathImagen;
		m_instanceNumber = o.m_instanceNumber;
	}

	IModeloImagen& operator = (const IModeloImagen& o) {
		m_pSeriePadre = o.m_pSeriePadre;
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o.m_UID;
		m_pathImagen = o.m_pathImagen;
		m_instanceNumber = o.m_instanceNumber;
		return *this;
	}

	~IModeloImagen()
	{
	}

	const std::string& GetUID() const
	{
		return m_UID;
	}

	const std::string& GetPathImagen() const
	{
		return m_pathImagen;
	}
	
	const std::string& GetInstanceNumber() const
	{
		return m_instanceNumber;
	}
protected:
	IModeloSerie* m_pSeriePadre;
	IModeloDicom* m_pModeloPadre;
	std::string   m_UID;
	std::string	  m_pathImagen;
	std::string	  m_instanceNumber;

	friend class IModeloDicom;

};

class IModeloSerie {

public:

	typedef std::list<IModeloImagen> ListaImagenesType;

	IModeloSerie()
	{
		m_pEstudioPadre = NULL;
		m_pModeloPadre = NULL;
	}

	IModeloSerie(const IModeloSerie& o)
	{
		m_pEstudioPadre = o.m_pEstudioPadre;
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o.m_UID;
		m_Tipo = o.m_Tipo;
		m_Numero = o.m_Numero;
		m_Fecha = o.m_Fecha;
		m_Hora = o.m_Hora;
		m_Descripcion = o.m_Descripcion;
		m_Doctor = o.m_Doctor;
	}

	IModeloSerie(IModeloDicom* pModeloPadre, IModeloEstudio* pEstudioPadre,
			const std::string& UID,
			const std::string& tipo,
			const std::string& fecha,
			const std::string& hora,
			const std::string& descripcion,
			const std::string& numero,
			const std::string& doctor)
	{
		m_pEstudioPadre = pEstudioPadre;
		m_pModeloPadre = pModeloPadre;
		m_UID = UID;
		m_Tipo = tipo;
		m_Numero = numero;
		m_Fecha = fecha;
		m_Hora = hora;
		m_Descripcion = descripcion;
		m_Doctor = doctor;
	}

	IModeloSerie& operator=(const IModeloSerie& o)
	{
		m_pEstudioPadre = o.m_pEstudioPadre;
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o.m_UID;
		m_Tipo = o.m_Tipo;
		m_Numero = o.m_Numero;
		m_Fecha = o.m_Fecha;
		m_Hora = o.m_Hora;
		m_Descripcion = o.m_Descripcion;
		m_Doctor = o.m_Doctor;
		return *this;
	}

	~IModeloSerie()
	{
	}

	const std::string& GetUID() const
	{
		return m_UID;
	}

	const std::string& GetTipo() const
	{
		return m_Tipo;
	}

	const std::string& GetNumero() const
	{
		return m_Numero;
	}

	const std::string& GetFecha() const
	{
		return m_Fecha;
	}

	const std::string& GetHora() const
	{
		return m_Hora;
	}

	const std::string& GetDescripcion() const
	{
		return m_Descripcion;
	}

	const std::string& GetDoctor() const
	{
		return m_Doctor;
	}

	const ListaImagenesType& ListaImagenes() const
	{
		return Imagenes;
	}

	const IModeloImagen& Imagen(int indice) const
	{
		int i=0;
		ListaImagenesType::const_iterator it;
		for(it = Imagenes.begin(); i < indice && it != Imagenes.end(); ++it, i++);

		if(it != Imagenes.end()){
			return (*it);
		} else {
			return Imagenes.front();
		}
	}

	const IModeloEstudio* GetStudy() const
	{
		return m_pEstudioPadre;
	}
protected:
	std::string m_UID;
	std::string m_Tipo;
	std::string m_Numero;
	std::string m_Fecha;
	std::string m_Hora;
	std::string m_Descripcion;
	std::string m_Doctor;

	IModeloEstudio*     m_pEstudioPadre;
	IModeloDicom*       m_pModeloPadre;

	ListaImagenesType Imagenes;

	friend class IModeloDicom;

};

class IModeloEstudio {

public:

	typedef std::list<IModeloSerie> ListaSeriesType;

	IModeloEstudio()
	{
		m_pModeloPadre = NULL;
		m_pPacientePadre = NULL;
	}

	IModeloEstudio( const IModeloEstudio& o )
	{
		m_pModeloPadre = o.m_pModeloPadre;
		m_pPacientePadre = o.m_pPacientePadre;
		m_UID = o.m_UID;
		m_AccNumber = o.m_AccNumber;
		m_Descripcion = o.m_Descripcion;
		m_Modalidad = o.m_Modalidad;
		m_Fecha = o.m_Fecha;
		m_Hora = o.m_Hora;
		m_Doctor = o.m_Doctor;
	}

	IModeloEstudio( IModeloDicom* pModeloPadre, IModeloPaciente* pPacientePadre,
			const std::string& UID,
			const std::string& AccNumber,
			const std::string& descripcion,
			const std::string& modalidad,
			const std::string& fecha,
			const std::string& hora,
			const std::string& doctor)
	{
		m_pModeloPadre = pModeloPadre;
		m_pPacientePadre = pPacientePadre;
		m_UID = UID;
		m_AccNumber = AccNumber;
		m_Descripcion = descripcion;
		m_Modalidad = modalidad;
		m_Fecha = fecha;
		m_Hora = hora;
		m_Doctor = doctor;
	}

	IModeloEstudio& operator=(const IModeloEstudio& o)
	{
		m_UID = o.m_UID;
		m_AccNumber = o.m_AccNumber;
		m_Descripcion = o.m_Descripcion;
		m_Modalidad = o.m_Modalidad;
		m_Fecha = o.m_Fecha;
		m_Hora = o.m_Hora;
		m_Doctor = o.m_Doctor;
		return *this;
	}


	~IModeloEstudio()
	{
	}

	const std::string& GetUID() const
	{
		return m_UID;
	}

	const std::string& GetAccNumber() const
	{
		return m_AccNumber;
	}

	const std::string& GetDescripcion() const
	{
		return m_Descripcion;
	}

	const std::string& GetModalidad() const
	{
		return m_Modalidad;
	}

	const std::string& GetFecha() const
	{
		return m_Fecha;
	}

	const std::string& GetHora() const
	{
		return m_Hora;
	}

	const std::string& GetDoctor() const
	{
		return m_Doctor;
	}

	const ListaSeriesType& ListaSeries() const
	{
		return Series;
	}

	const IModeloSerie& Serie(int indice) const
	{
		int i=0;
		ListaSeriesType::const_iterator it;
		for(it = Series.begin();i<indice && it != Series.end(); ++it, i++);

		if(it != Series.end()){
			return (*it);
		} else {
			return Series.front();
		}
	}

	const IModeloPaciente* GetPatient() const
	{
		return m_pPacientePadre;
	}

protected:

	std::string m_UID;
	std::string m_AccNumber;
	std::string m_Descripcion;
	std::string m_Modalidad;
	std::string m_Fecha;
	std::string m_Hora;
	std::string m_Comentarios;
	std::string m_Doctor;

	IModeloDicom* m_pModeloPadre;
	IModeloPaciente* m_pPacientePadre;

	ListaSeriesType    Series;

	friend class IModeloDicom;

};

class IModeloPaciente {
public:

	typedef std::list<IModeloEstudio> ListaEstudiosType;

	IModeloPaciente()
	{
		m_pModeloPadre = NULL;
	}

	IModeloPaciente( const IModeloPaciente& o )
	{
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o .m_UID;
		m_Nombre = o.m_Nombre;
		m_FechaNacimiento = o.m_FechaNacimiento;
		m_Sexo = o.m_Sexo;
	}

	IModeloPaciente( IModeloDicom* pModeloPadre,
			const std::string& UID,
			const std::string& nombre,
			const std::string& fechaNacimiento,
			const std::string& sexo)
	{
		m_pModeloPadre = pModeloPadre;
		m_UID = UID;
		m_Nombre = nombre;
		m_FechaNacimiento = fechaNacimiento;
		m_Sexo = sexo;
	}

	IModeloPaciente& operator=(const IModeloPaciente& o)
	{
		m_pModeloPadre = o.m_pModeloPadre;
		m_UID = o .m_UID;
		m_Nombre = o.m_Nombre;
		m_FechaNacimiento = o.m_FechaNacimiento;
		m_Sexo = o.m_Sexo;
		return *this;
	}

	~IModeloPaciente()
	{
	}

	const std::string& GetUID() const
	{
		return m_UID;
	}

	const std::string& GetNombre() const
	{
		return m_Nombre;
	}

	const std::string& GetFechaNacimiento() const
	{
		return m_FechaNacimiento;
	}

	const std::string& GetSexo() const
	{
		return m_Sexo;
	}

	const ListaEstudiosType& ListaEstudios() const
	{
		return Estudios;
	}

	const IModeloEstudio& Estudio(int indice) const
	{
		int i=0;
		ListaEstudiosType::const_iterator it;
		for(it = Estudios.begin();i<indice && it != Estudios.end(); ++it, i++);

		if(it != Estudios.end()){
			return (*it);
		} else {
			return Estudios.front();
		}
	}

protected:

	std::string m_UID;
	std::string m_Nombre;
	std::string m_FechaNacimiento;
	std::string m_Sexo;

	ListaEstudiosType Estudios;

	IModeloDicom* m_pModeloPadre;

	friend class IModeloDicom;

};

class IModeloDicom {
public:

	typedef std::list<IModeloPaciente> ListaPacientesType;

	typedef std::list<IModeloEstudio*> ListaPunterosEstudiosType;
	typedef std::list<IModeloSerie*>   ListaPunterosSeriesType;
	typedef std::list<IModeloImagen*>  ListaPunterosImagenesType;

	IModeloDicom()
	{
		Error = false;
	}

	~IModeloDicom()
	{
	}

	const ListaPacientesType& ListaPacientes() const
	{
		return Pacientes;
	}

	const IModeloPaciente& Paciente(int indice) const
	{
		int i=0;
		ListaPacientesType::const_iterator it;
		for(it = Pacientes.begin();i<indice && it != Pacientes.end(); ++it, i++);

		if(it != Pacientes.end()){
			return (*it);
		} else {
			return Pacientes.front();
		}
	}

	// region operaciones de busqueda externas

public:

	// Esta operacion es externa porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de paciente.
	void BuscarPaciente(const std::string& UID, IModeloPaciente const **  result)
	{
		IModeloPaciente* paciente = NULL;
		BuscarPacienteInternal(UID, &paciente);
		*result = paciente;
	}

	// Esta operacion es externa porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de estudio.
	void BuscarEstudio(const std::string& UID, IModeloEstudio const ** result)
	{
		IModeloEstudio* estudio = NULL;
		BuscarEstudioInternal(UID, &estudio);
		*result = estudio;
	}

	// Esta operacion es externa porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de serie.
	void BuscarSerie(const std::string& UID, IModeloSerie const ** result)
	{
		IModeloSerie* serie = NULL;
		BuscarSerieInternal(UID, &serie);
		*result = serie;
	}

	// Esta operacion es externa porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de imagen.
	void BuscarImagen(const std::string& UID, IModeloImagen const ** result)
	{
		IModeloImagen* imagen = NULL;
		BuscarImagenInternal(UID, &imagen);
		*result = imagen;
	}

	bool TieneImagenes()
	{
		return (ListaPunterosImagenes.size() > 0);
	}

	// endregion

	// region operaciones de busqueda internas

private:

	// Esta operacion es interna porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de paciente.
	void BuscarPacienteInternal(const std::string& UID, IModeloPaciente **  result)
	{
		for (ListaPacientesType::iterator it = Pacientes.begin(); *result == NULL && it != Pacientes.end(); ++it) {
			IModeloPaciente& pi = *it;
			if (pi.GetUID() == UID) {
				*result = &(*it);
			}
		}
	}

	// Esta operacion es interna porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de estudio.
	void BuscarEstudioInternal(const std::string& UID, IModeloEstudio **  result)
	{
		for (ListaPunterosEstudiosType::iterator it = ListaPunterosEstudios.begin(); *result == NULL && it != ListaPunterosEstudios.end(); ++it) {
			IModeloEstudio* pi = *it;
			if (pi->GetUID() == UID) {
				*result = (*it);
			}
		}
	}

	// Esta operacion es interna porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de serie.
	void BuscarSerieInternal(const std::string& UID, IModeloSerie **  result)
	{

		for (ListaPunterosSeriesType::iterator it = ListaPunterosSeries.begin(); *result == NULL && it != ListaPunterosSeries.end(); ++it) {
			IModeloSerie* pi = *it;
			if (pi->GetUID() == UID) {
				*result = (*it);
			}
		}
	}


	// Esta operacion es interna porque devuelve como resultado un puntero a constante, para no modificar/destruir el registro de imagen.
	void BuscarImagenInternal(const std::string& UID, IModeloImagen **  result)
	{
		for (ListaPunterosImagenesType::iterator it = ListaPunterosImagenes.begin(); *result == NULL && it != ListaPunterosImagenes.end(); ++it) {
			IModeloImagen* pi = *it;
			if (pi->GetUID() == UID) {
				*result = (*it);
			}
		}
	}

	// endregion

	// operaciones de inserccion

public:

	const IModeloPaciente& InsertarPaciente(const std::string& UID, const std::string& nombre, const std::string& fechaNacimiento, const std::string& sexo)
	{

		const IModeloPaciente* paciente = NULL;

		BuscarPaciente(UID, &paciente);

		if (paciente == NULL) {
			Pacientes.push_back(IModeloPaciente( this, UID, nombre, fechaNacimiento, sexo));
			paciente = &Pacientes.back();
		}

		return *paciente;
	}

	const IModeloEstudio& InsertarEstudio(const std::string& UIDPaciente, const std::string& UID, const std::string& AccNumber, const std::string& descripcion, const std::string& modalidad, const std::string& fecha, const std::string& hora, const std::string& doctor)
	{

		IModeloEstudio* estudio = NULL;

		BuscarEstudioInternal(UID, &estudio);

		if (estudio == NULL) {

			IModeloPaciente* paciente = NULL;

			BuscarPacienteInternal(UIDPaciente, &paciente);

			if (paciente != NULL) {

				paciente->Estudios.push_back(IModeloEstudio( this, paciente, UID, AccNumber, descripcion, modalidad, fecha, hora, doctor));
				estudio = & (paciente->Estudios.back());
				ListaPunterosEstudios.push_back(estudio);
			}
			else {
				// El estudio no esta asociado a un paciente existente.
				throw GIL::DICOM::ModelException(_Std("Logical error: Study not associated with existent patient in model"));
			}
		}

		return *estudio;
	}

	const IModeloSerie& InsertarSerie(const std::string& UIDEstudio, const std::string& UID, const std::string& tipo, const std::string& fecha, const std::string& hora, const std::string& descripcion, const std::string& numero, const std::string& doctor)
	{

		IModeloSerie* serie = NULL;

		BuscarSerieInternal(UID, &serie);

		if (serie == NULL) {

			IModeloEstudio* estudio = NULL;

			BuscarEstudioInternal(UIDEstudio, &estudio);

			if (estudio != NULL) {

				estudio->Series.push_back(IModeloSerie( this, estudio, UID, tipo, fecha, hora, descripcion, numero,doctor));
				serie = & (estudio->Series.back());
				ListaPunterosSeries.push_back(serie);
			}
			else {
				// La Serie no esta asociada a un estudio existente.
				throw GIL::DICOM::ModelException(_Std("Logical error: Serie not associated with existent study in model"));
			}
		}

		return *serie;

	}

	const IModeloImagen& InsertarImagen(const std::string& UIDSerie, const std::string& UID, const std::string& instanceNumber, const std::string& pathImagen = "")
	{

		IModeloImagen* imagen = NULL;

		BuscarImagenInternal(UID, &imagen);

		if (imagen == NULL) {

			IModeloSerie* serie = NULL;

			BuscarSerieInternal(UIDSerie, &serie);

			if (serie != NULL) {

				serie->Imagenes.push_back(IModeloImagen( this, serie, UID, instanceNumber, pathImagen));
				imagen = & (serie->Imagenes.back());
				ListaPunterosImagenes.push_back(imagen);
			}
			else {
				// La Imagen no esta asociada a una serie existente.
				throw GIL::DICOM::ModelException(_Std("Logical error: Image not associated with existent serie in model"));
			}
		}

		return *imagen;

	}

	friend std::ostream& operator<<(std::ostream& out, const IModeloDicom& d) {
		for (IModeloDicom::ListaPacientesType::const_iterator it = d.Pacientes.begin(); it != d.Pacientes.end(); ++it) {
			const IModeloPaciente& p = *it;
			out << "P: " << p.GetUID().c_str() << " [ " << p.GetNombre().c_str() << std::endl;
			for (IModeloPaciente::ListaEstudiosType::const_iterator ite = p.ListaEstudios().begin(); ite != p.ListaEstudios().end(); ite++) {
				const IModeloEstudio& e = *ite;
				out << "\tE: " << e.GetUID().c_str() << ", " << e.GetDescripcion().c_str() << std::endl;
				for (IModeloEstudio::ListaSeriesType::const_iterator its = e.ListaSeries().begin(); its != e.ListaSeries().end(); its++) {
					const IModeloSerie& s = *its;
					out << "\t\tS: " << s.GetUID().c_str() << ", " << s.GetTipo().c_str() << ", " << s.GetDescripcion().c_str() << std::endl;
					for (IModeloSerie::ListaImagenesType::const_iterator iti = s.ListaImagenes().begin(); iti != s.ListaImagenes().end(); iti++) {
						const IModeloImagen& i = *iti;
						out << "\t\t\t" << "I: " << i.GetUID().c_str() << std::endl;
					}
				}
			}
		}
		return out;
	}

	friend std::ostream& operator<<(std::ostream& out, IModeloDicom* const d) {
		if (d != NULL) {
			out << *d;
		}
		else {
			out << "[ NULL ]";
		}
		return out;
	}

	const ListaPunterosImagenesType Imagenes() const {
		return ListaPunterosImagenes;
	}

public:
	bool Error;

protected:
	ListaPacientesType Pacientes;

	ListaPunterosEstudiosType  ListaPunterosEstudios;
	ListaPunterosSeriesType    ListaPunterosSeries;
	ListaPunterosImagenesType  ListaPunterosImagenes;

};
