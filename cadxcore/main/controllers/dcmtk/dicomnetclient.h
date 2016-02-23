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
#include <api/controllers/icontroladorlog.h>
#include <api/iproxynotificadorprogreso.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/dcmtk/dicomservers.h>
#include "dicomnetwork.h"
#include "helpers.h"

#include <iostream>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif
#include <dcmtk/dcmdata/dcdatset.h>
#include <dcmtk/dcmdata/dcdeftag.h>
#include <api/dicom/imodelodicom.h>
#include <wx/strconv.h>
#include <wx/string.h>

namespace GNC{
	class IProxyNotificadorProgreso;
}

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif

typedef enum CallbackType {
	CT_MoveEstudio,
	CT_MoveSerie,
	CT_MoveImagen,
	CT_None
} CallbackType;

#ifndef WXUNUSED
#define WXUNUSED(name)
#endif

template<class T>
class NetClient : public T {
private:
	GIL::DICOM::DCMTK::Network* Net;
	std::string                 ambitolog;

protected:
	GNC::IProxyNotificadorProgreso* m_pNotificadorProgreso;
public:
	NetClient(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificadorProgreso = NULL) : T(ambitolog) {

		if (connectionKey == NULL) {
			LOG_ERROR(ambitolog, "NULL Connection key");
		}
		this->Net = GIL::DICOM::DCMTK::Network::Instance(connectionKey);
		this->m_pNotificadorProgreso = pNotificadorProgreso;
		this->ambitolog = ambitolog;
	};
	
	bool QueryServer(const GNC::GCS::Ptr<DicomServer>& server, const std::string& local_aet) {
		if (server == NULL) {
			return false;
		}
		if (!Net->Initiallized()) {
		
			Net->InitializeNetwork(T::GetTimeout(), T::GetRole(), T::GetAcceptorPort());
		}

		T::Create(
			server->AET,
			server->HostName,
			server->Port,
			local_aet
		);

		T::SetNotificadorProgreso(m_pNotificadorProgreso);

		LOG_DEBUG(ambitolog, "Connected: AET = " << server->AET << ", Host = " << server->HostName << ", Port = " << server->Port << ", Local AET = " << local_aet << ", PDU = " << server->PDU);
		OFCondition r = T::Connect(Net, server->PDU);

		OFCondition c = EC_Normal;

		if (r.good() == true) {
			c = T::SendObject();
		}
		else {
			LOG_ERROR(ambitolog, "Error connecting:" << r.text());
			T::Drop();
			T::Destroy();
			throw GIL::DICOM::PACSException(r.text());
		}

		if (!c.good()) {
			LOG_ERROR(ambitolog, "Error sending object: " << c.text());
			T::Drop();
			T::Destroy();
			throw GIL::DICOM::PACSException(c.text());
		}

		LOG_DEBUG(ambitolog, "Closing association");
		T::Drop();
		return true;
	}

	bool QueryServer(DcmDataset* query, const GNC::GCS::Ptr<DicomServer>& server, std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWrapper, const std::string& local_aet) {
		if (!server.IsValid()) {
			return false;
		}

		if (!Net->Initiallized()) {
		
			Net->InitializeNetwork(T::GetTimeout(), T::GetRole(), T::GetAcceptorPort());
		}

		T::Create(
			server->AET,
			server->HostName,
			server->Port,
			local_aet
		);

		T::SetNotificadorProgreso(m_pNotificadorProgreso);

		T::SetCallbackInfo(&resultsWrapper, server);

		LOG_INFO(ambitolog, "Connecting: AET = " << server->AET << ", Host = " << server->HostName << ", Puerto = " << server->Port << ", Local AET = " << local_aet << ", PDU = " << server->PDU);
		OFCondition r = T::Connect(Net, server->PDU);

		OFCondition c = EC_Normal;

		if (r.good() == true) {
			LOG_DEBUG(ambitolog, "Requesting object:" << std::endl << DumpDataset(query));
			c = T::SendObject(query);
		}
		else {
			LOG_DEBUG(ambitolog, "Error connecting:" << r.text());
			T::Drop();
			T::Destroy();
			LOG_INFO(ambitolog, "Disconnected");
			throw GIL::DICOM::PACSException(r.text());
		}

		if (!c.good()) {
			LOG_DEBUG(ambitolog, "Error requesting object: " << c.text());
			T::Drop();
			T::Destroy();
			LOG_INFO(ambitolog, "Disconnected");
			throw GIL::DICOM::PACSException(c.text());
		}

		LOG_INFO(ambitolog, "Disconnected");
		T::Drop();
		
		return r.good();
	}

	///DEPRECATED! IModeloDICOM has to be replaced by results wrapper
	bool QueryServer(DcmDataset* query, const GNC::GCS::Ptr<DicomServer>& server, IModeloDicom* pModelo, const std::string& local_aet, CallbackType callbackType) {
		if (pModelo == NULL && callbackType != CT_None)  {
			return false;
		}
		if (!server.IsValid()) {
			return false;
		}

		if (!Net->Initiallized()) {
		
			Net->InitializeNetwork(T::GetTimeout(), T::GetRole(), T::GetAcceptorPort());
		}

		T::Create(
			server->AET,
			server->HostName,
			server->Port,
			local_aet
		);

		T::SetNotificadorProgreso(m_pNotificadorProgreso);

		LOG_INFO(ambitolog, "Connecting: AET = " << server->AET << ", Host = " << server->HostName << ", Puerto = " << server->Port << ", Local AET = " << local_aet << ", PDU = " << server->PDU);
		OFCondition r = T::Connect(Net, server->PDU);

		OFCondition c = EC_Normal;

		if (r.good() == true) {
			LOG_DEBUG(ambitolog, "Requesting object:" << std::endl << DumpDataset(query));
			c = T::SendObject(query);
		}
		else {
			LOG_DEBUG(ambitolog, "Error connecting:" << r.text());
			T::Drop();
			T::Destroy();
			LOG_INFO(ambitolog, "Disconnected");
			throw GIL::DICOM::PACSException(r.text());
		}

		if (!c.good()) {
			LOG_DEBUG(ambitolog, "Error requesting object: " << c.text());
			T::Drop();
			T::Destroy();
			LOG_INFO(ambitolog, "Disconnected");
			throw GIL::DICOM::PACSException(c.text());
		}

		DcmStack* result = T::GetResultStack();
		if (r.good() && c.good() && result != NULL && result->card() > 0) {
			LOG_DEBUG(ambitolog, "Num results: " << result->card());

			for (unsigned long i = 0; i < result->card(); ++i) {
				DcmDataset* dset = new DcmDataset( *(static_cast<DcmDataset*>(result->elem(i))) );
				switch (callbackType) {
					case CT_MoveEstudio:
						OnMoverEstudio(dset, pModelo);
						break;
					case CT_MoveSerie:
						OnMoverSerie(dset, pModelo);
						break;
					case CT_MoveImagen:
						OnMoverImagen(dset, pModelo);
						break;
					case CT_None:
						break;
				}
				delete dset;
			}
		}
		else {
			if (r.bad()) {
				LOG_ERROR(ambitolog, "Connection error: " << r.text());
			}
			if (c.bad()) {
				LOG_ERROR(ambitolog, "Error requesting object: " << c.text());
			}
		}

		LOG_INFO(ambitolog, "Disconnected");
		T::Drop();
		
		return r.good();
	}

	wxCSConv GetConv(DcmDataset* dset, const std::string& defaultCharset)
	 {
		 OFString Charset;
		 wxCSConv wxConv(wxFONTENCODING_ISO8859_1);
		 if (dset->findAndGetOFString(DCM_SpecificCharacterSet, Charset).bad()) {
			 Charset = defaultCharset.c_str();
		 }
		 if (Charset == "ISO_IR 192" || Charset == "ISO_IR 6" || Charset == "ISO_IR 138") {
			 wxConv = wxCSConv(wxFONTENCODING_UTF8);
		 }
		 else if (Charset == "ISO_IR 100") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_1);
		 }
		 else if (Charset == "ISO_IR 101") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_2);
		 }
		 else if (Charset == "ISO_IR 109") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_3);
		 }
		 else if (Charset == "ISO_IR 110") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_4);
		 }
		 else if (Charset == "ISO_IR 148") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_9);
		 }
		 else if (Charset == "ISO_IR 144") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_5);
		 }
		 else if (Charset == "ISO_IR 127") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_6);
		 }
		 else if (Charset == "ISO_IR 126") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_7);
		 }
		 else if (Charset == "ISO_IR 138") {
			 wxConv = wxCSConv(wxFONTENCODING_ISO8859_8);
		 }
		 return wxConv;
	 }

	void OnBuscarPacientes(DcmDataset* query, DcmDataset* dset, const GNC::GCS::Ptr<DicomServer>& server, IModeloDicom* pModelo)
	{
		wxCSConv wxConv = GetConv(dset, server->GetDefaultCharset());

		OFString OFPacienteUID;
		if (dset->findAndGetOFString(DCM_PatientID, OFPacienteUID).bad()) {
			if (!dset->isEmpty()) {
				LOG_ERROR(ambitolog, "El PACS no devolvió el PatientID del paciente. Entrada ignorada.");
			}
			return;
		}

		std::string PacienteUID;
		if (dset->findAndGetOFString(DCM_PatientID, OFPacienteUID).good()) {
			PacienteUID = wxString(OFPacienteUID.c_str(), wxConv).ToUTF8();
		}

		OFString OFPacienteNombre;
		std::string PacienteNombre;
		if (dset->findAndGetOFString(DCM_PatientName, OFPacienteNombre).good()) {
			PacienteNombre = wxString(OFPacienteNombre.c_str(), wxConv).ToUTF8();
		}

		OFString OFPacienteFechaNacimiento;
		std::string PacienteFechaNacimiento;
		if (dset->findAndGetOFString(DCM_PatientBirthDate, OFPacienteFechaNacimiento).good()) {
			PacienteFechaNacimiento = wxString(OFPacienteFechaNacimiento.c_str(), wxConv).ToUTF8();
		}

		OFString OFPacienteSexo;
		std::string PacienteSexo;
		if (dset->findAndGetOFString(DCM_PatientSex, OFPacienteSexo).good()) {
			PacienteSexo = wxString(OFPacienteSexo.c_str(), wxConv).ToUTF8();
		}

		if (PacienteUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_PatientID, OFStr).good()) {
				PacienteUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}

		pModelo->InsertarPaciente(PacienteUID, PacienteNombre, PacienteFechaNacimiento, PacienteSexo);
	}

	void OnBuscarSeries(DcmDataset* query, DcmDataset* dset, const GNC::GCS::Ptr<DicomServer>& server, IModeloDicom *pModelo)
	{
		wxCSConv wxConv = GetConv(dset, server->GetDefaultCharset());

		OFString OFEstudioUID;
		OFString OFSerieUID;
		if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUID).bad() || dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSerieUID).bad()) {
			if (!dset->isEmpty()) {
				LOG_ERROR(ambitolog, "El PACS no devolvió el StudyInstanceUID o el SeriesInstanceUID de la serie. Entrada ignorada.");
			}
			return;
		}

		std::string EstudioUID;
		EstudioUID = wxString(OFEstudioUID.c_str(), wxConv).ToUTF8();

		std::string SerieUID;
		SerieUID = wxString(OFSerieUID.c_str(), wxConv).ToUTF8();

		OFString OFSerieTipo;
		std::string SerieTipo;
		if (dset->findAndGetOFString(DCM_Modality, OFSerieTipo).good()) {
			SerieTipo = wxString(OFSerieTipo.c_str(), wxConv).ToUTF8();
		}

		OFString OFSerieFecha;
		std::string SerieFecha;
		if (dset->findAndGetOFString(DCM_SeriesDate, OFSerieFecha).good()) {
			SerieFecha = wxString(OFSerieFecha.c_str(), wxConv).ToUTF8();
		}

		OFString OFSerieHora;
		std::string SerieHora;
		if (dset->findAndGetOFString(DCM_SeriesTime, OFSerieHora).good()) {
			SerieHora = wxString(OFSerieHora.c_str(), wxConv).ToUTF8();
		}

		OFString OFSerieDescripcion;
		std::string SerieDescripcion;
		if (dset->findAndGetOFString(DCM_SeriesDescription, OFSerieDescripcion).good()) {
			SerieDescripcion = wxString(OFSerieDescripcion.c_str(), wxConv).ToUTF8();
		}

		OFString OFDoctor;
		std::string SerieDoctor;
		if (dset->findAndGetOFString(DCM_ReferringPhysicianName, OFDoctor).good()) {
			SerieDoctor = wxString(OFDoctor.c_str(), wxConv).ToUTF8();
		}

		OFString OFNumber;
		std::string SerieNumero;
		if (dset->findAndGetOFString(DCM_NumberOfSeriesRelatedInstances, OFNumber).good()) {
			SerieNumero = wxString(OFNumber.c_str(), wxConv).ToUTF8();
		}

		if (EstudioUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_StudyInstanceUID, OFStr).good()) {
				EstudioUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}
		if (SerieUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_SeriesInstanceUID, OFStr).good()) {
				SerieUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}

		pModelo->InsertarSerie(EstudioUID, SerieUID, SerieTipo, SerieFecha, SerieHora, SerieDescripcion, SerieNumero,SerieDoctor);
	}

    void OnBuscarImagenes(DcmDataset* query, DcmDataset *dset, const GNC::GCS::Ptr<DicomServer>& server, IModeloDicom* pModelo) 
	 {
		wxCSConv wxConv = GetConv(dset, server->GetDefaultCharset());

    	OFString OFSerieUID;
    	OFString OFImagenUID;
		OFString OFInstanceNumber;
		if (dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSerieUID).bad() || dset->findAndGetOFString(DCM_SOPInstanceUID, OFImagenUID).bad()) {
			if (!dset->isEmpty()) {
				LOG_ERROR(ambitolog, "El PACS no devolvió el SeriesInstanceUID o el SOPInstanceUID de la imagen. Entrada ignorada.");
			}
			return;
		}
		if (OFInstanceNumber.empty()) {
			LOG_WARN(ambitolog, "InstanceNumber not found. Setting to default");
			OFInstanceNumber = "1";
		}

		std::string SerieUID;
		std::string ImagenUID;
		std::string InstanceNumber;

		SerieUID.assign(OFSerieUID.c_str());
		ImagenUID.assign(OFImagenUID.c_str());
		InstanceNumber.assign(OFInstanceNumber.c_str());

		if (SerieUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_SeriesInstanceUID, OFStr).good()) {
				SerieUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}
		if (ImagenUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_SOPInstanceUID, OFStr).good()) {
				ImagenUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}
		if (InstanceNumber.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_InstanceNumber, OFStr).good()) {
				InstanceNumber = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}

		pModelo->InsertarImagen(SerieUID, ImagenUID, InstanceNumber);

	}

    void OnBuscarEstudios(DcmDataset* query, DcmDataset *dset, const GNC::GCS::Ptr<DicomServer>& server, IModeloDicom *pModelo) {
		OFString OFEstudioUID;
		if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUID).bad()) {
			if (!dset->isEmpty()) {
				LOG_ERROR(ambitolog, "El PACS no devolvió el StudyInstanceUID del estudio. Entrada ignorada.");
			}
			return;
		}

		wxCSConv wxConv = GetConv(dset, server->GetDefaultCharset());

		OFString OFPacienteUID;
		std::string PacienteUID;
		if (dset->findAndGetOFString(DCM_PatientID, OFPacienteUID).good()) {
			PacienteUID = wxString(OFPacienteUID.c_str(), wxConv).ToUTF8();
		} else {
			if (query->findAndGetOFString(DCM_PatientID, OFPacienteUID).good()) {
				PacienteUID = wxString(OFPacienteUID.c_str(), wxConv).ToUTF8();
			}
		}

		OFString OFPacienteNombre;
		std::string PacienteNombre;
		if (dset->findAndGetOFString(DCM_PatientName, OFPacienteNombre).good()) {
			PacienteNombre = wxString(OFPacienteNombre.c_str(), wxConv).ToUTF8();
		}

		OFString OFPacienteFechaNacimiento;
		std::string PacienteFechaNacimiento;

		if (dset->findAndGetOFString(DCM_PatientBirthDate, OFPacienteFechaNacimiento).good()) {
			PacienteFechaNacimiento = wxString(OFPacienteFechaNacimiento.c_str(), wxConv).ToUTF8();
		}

		OFString OFPacienteSexo;
		std::string PacienteSexo;
		if (dset->findAndGetOFString(DCM_PatientSex, OFPacienteSexo).good()) {
			PacienteSexo = wxString(OFPacienteSexo.c_str(), wxConv).ToUTF8();
		}

		pModelo->InsertarPaciente(PacienteUID, PacienteNombre, PacienteFechaNacimiento, PacienteSexo);

		std::string EstudioUID;
		if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFEstudioUID).good()) {
			EstudioUID = wxString(OFEstudioUID.c_str(), wxConv).ToUTF8();
		}

		OFString OFEAccNumber;
		std::string AccNumber;
		if (dset->findAndGetOFString(DCM_AccessionNumber, OFEAccNumber).good()) {
			AccNumber = wxString(OFEAccNumber.c_str(), wxConv).ToUTF8();
		}

		OFString OFEstudioDescripcion;
		std::string EstudioDescripcion;
		if (dset->findAndGetOFString(DCM_StudyDescription, OFEstudioDescripcion).good()) {
			EstudioDescripcion = wxString(OFEstudioDescripcion.c_str(), wxConv).ToUTF8();
		}

		OFString OFEstudioModalidad;
		std::string EstudioModalidad;
		if (dset->findAndGetOFStringArray(DCM_ModalitiesInStudy, OFEstudioModalidad).good()) {
			EstudioModalidad = wxString(OFEstudioModalidad.c_str(), wxConv).ToUTF8();
		}

		OFString OFEstudioFecha;
		std::string EstudioFecha;
		if (dset->findAndGetOFString(DCM_StudyDate, OFEstudioFecha).good()) {
			EstudioFecha = wxString(OFEstudioFecha.c_str(), wxConv).ToUTF8();
		}

		OFString OFEstudioHora;
		std::string EstudioHora;
		if (dset->findAndGetOFString(DCM_StudyTime, OFEstudioHora).good()) {
			EstudioHora = wxString(OFEstudioHora.c_str(), wxConv).ToUTF8();
		}

		OFString OFDoctor;
		std::string EstudioDoctor;
		if (dset->findAndGetOFString(DCM_ReferringPhysicianName, OFDoctor).good()) {
			EstudioDoctor = wxString(OFDoctor.c_str(), wxConv).ToUTF8();
		}

		if (EstudioUID.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_StudyInstanceUID, OFStr).good()) {
				EstudioUID = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}
		if (AccNumber.empty()) {
			OFString OFStr;
			if (query->findAndGetOFString(DCM_AccessionNumber, OFStr).good()) {
				AccNumber = wxString(OFStr.c_str(), wxConv).ToUTF8();
			}
		}

		pModelo->InsertarEstudio(PacienteUID, EstudioUID, AccNumber, EstudioDescripcion, EstudioModalidad, EstudioFecha, EstudioHora, EstudioDoctor);

    }

	void OnMoverEstudio(DcmDataset *WXUNUSED(dset), IModeloDicom *WXUNUSED(pModelo)) {
		#ifdef _GINKGO_DEBUG
		std::cout << "move study" << std::endl;
		#endif
	}

    void OnMoverSerie(DcmDataset *WXUNUSED(dset), IModeloDicom *WXUNUSED(pModelo)) {
		#ifdef _GINKGO_DEBUG
		std::cout << "move serie" << std::endl;
		#endif
    }

	void OnMoverImagen(DcmDataset *WXUNUSED(dset), IModeloDicom *WXUNUSED(pModelo)) {
		#ifdef _GINKGO_DEBUG
		std::cout << "move image" << std::endl;
		#endif
    }

    OFBool writeToFile(const char* ofname, DcmDataset *dataset) {
		/* write out as a file format */

		DcmFileFormat fileformat(dataset); // copies dataset
		OFCondition ec = fileformat.error();
		if (ec.bad()) {
			LOG_ERROR(T::ambitolog, "Unable to read DICOM dataset: " << ec.text());
			return OFFalse;
		}

		ec = fileformat.saveFile(ofname, dataset->getOriginalXfer());
		if (ec.bad()) {
			LOG_ERROR(T::ambitolog, "Unable to write file (" << ofname << ": " << ec.text());
			return OFFalse;
		}
		return OFTrue;
    }
};
