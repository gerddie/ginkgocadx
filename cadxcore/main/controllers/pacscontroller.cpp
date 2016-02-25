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


#include "dcmtk/dicomimg2dcm.h"
#include "dcmtk/dicomservers.h"
#include "dcmtk/dicomservice.h"
#include "dcmtk/dicomnetclient.h"
#include "dcmtk/dicomfindassociation.h"
#include "dcmtk/dicommoveassociation.h"
#include "dcmtk/dicomgetassociation.h"
#include "dcmtk/dicomstoreassociation.h"
#include "dcmtk/dicomprintassociation.h"
#include "dcmtk/dicomcustomassociation.h"
#include <dcmtk/dcmdata/dcrledrg.h>
#include <dcmtk/dcmjpeg/djdecode.h>
#include <dcmtk/dcmdata/dcdicdir.h>

#include <string>
#include <limits>
#include <openssl/md5.h>

#include "pacscontroller.h"
#include <api/ientorno.h>
#include <api/dicom/imodelodicom.h>
#include <api/dicom/dcmdictionary.h>
#include <main/entorno.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/tokenzr.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/dicommanager.h>
/* curl stuff */
#include <curl/curl.h>

#ifdef verify
#define MACRO_QUE_ESTORBA verify
#undef verify
#endif

#include <dcmtk/dcmdata/dcdeftag.h>
#include <dcmtk/dcmdata/dcdict.h>
#include <dcmtk/dcmdata/dcdicent.h>

#ifdef MACRO_QUE_ESTORBA
#define verify MACRO_QUE_ESTORBA
#endif


namespace GIL {
	namespace DICOM {
		wxCSConv GetConv(const std::string& Charset)
		{
			wxCSConv wxConv(wxFONTENCODING_ISO8859_1);
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

		//helper compare instances
		bool compare_instances (std::pair<long, std::string> first, std::pair<long, std::string> second) {
			return first.first < second.first;
		}

		PACSController* PACSController::m_pInstance = NULL;

		PACSController::PACSController()
		{
			m_pServiceInstance = NULL;
		}

		PACSController::~PACSController()
		{
			//std::cout << "PACSController destruyendose" << std::endl;
			//std::cout << "PACSController destruido" << std::endl;
		}


		PACSController* PACSController::Instance()
		{
			if (m_pInstance == NULL) {
				m_pInstance = new PACSController();
			}
			return m_pInstance;
		}

		void PACSController::FreeInstance()
		{
			if (m_pInstance != NULL) {
				m_pInstance->StopServer();
				delete m_pInstance;
				m_pInstance = NULL;
			}
			//se desregistran los codecs dcmtk!
			DJDecoderRegistration::cleanup();
			DcmRLEDecoderRegistration::cleanup();
			//purgar temp
			PurgarDirectorioTemporal();
		}

		void PACSController::buildDicomDir()
		{

		}

		bool PACSController::LeerDicomDir(const std::string& pathDicomDir, IModeloDicom* pModelo)
		{
			if(pModelo == NULL || !DICOMManager::EsDicom(pathDicomDir))
			{
				return false;
			}

			DcmDicomDir            dicomdir(pathDicomDir.c_str());
			DcmDirectoryRecord *   root = &(dicomdir.getRootRecord());
			DcmDirectoryRecord *   PatientRecord = NULL;
			DcmDirectoryRecord *   StudyRecord = NULL;
			DcmDirectoryRecord *   SeriesRecord = NULL;
			DcmDirectoryRecord *   FileRecord = NULL;
			OFString            tmpString;

			wxFileName fileNameDicomDir(FROMPATH(pathDicomDir));
			std::string basePath;
			basePath = TOPATH(fileNameDicomDir.GetPath());

			if(root != NULL)
			{
				while (((PatientRecord = root->nextSub(PatientRecord)) != NULL))
				{
					OFString uidPaciente, nombrePaciente, fechaNacimiento, sexo;
					PatientRecord->findAndGetOFString(DCM_PatientID,uidPaciente);
					PatientRecord->findAndGetOFString(DCM_PatientName,nombrePaciente);
					PatientRecord->findAndGetOFString(DCM_PatientBirthDate, fechaNacimiento);
					PatientRecord->findAndGetOFString(DCM_PatientSex, sexo);

					pModelo->InsertarPaciente(uidPaciente.c_str(),nombrePaciente.c_str(),fechaNacimiento.c_str(),sexo.c_str());
					while (((StudyRecord = PatientRecord->nextSub(StudyRecord)) != NULL))
					{
						OFString uidEstudio, descripcionEstudio, fechaEstudio, horaEstudio, doctor, accesionNumber;
						StudyRecord->findAndGetOFString(DCM_StudyInstanceUID, uidEstudio);
						StudyRecord->findAndGetOFString(DCM_StudyDescription, descripcionEstudio);
						StudyRecord->findAndGetOFString(DCM_StudyDate, fechaEstudio);
						StudyRecord->findAndGetOFString(DCM_StudyTime, horaEstudio);
						StudyRecord->findAndGetOFString(DCM_ReferringPhysicianName, doctor);
						StudyRecord->findAndGetOFString(DCM_AccessionNumber, accesionNumber);

						pModelo->InsertarEstudio(uidPaciente.c_str(),uidEstudio.c_str(),accesionNumber.c_str(), descripcionEstudio.c_str(),"",fechaEstudio.c_str(),horaEstudio.c_str(),doctor.c_str());
						while (((SeriesRecord = StudyRecord->nextSub(SeriesRecord)) != NULL))
						{
							OFString uidSerie,modalidadSerie,fechaSerie,horaSerie,descripcionSerie,numeroSerie,doctorSerie;
							SeriesRecord->findAndGetOFString(DCM_SeriesInstanceUID, uidSerie);
							SeriesRecord->findAndGetOFString(DCM_Modality, modalidadSerie);
							SeriesRecord->findAndGetOFString(DCM_SeriesDate, fechaSerie);
							SeriesRecord->findAndGetOFString(DCM_SeriesTime, horaSerie);
							SeriesRecord->findAndGetOFString(DCM_SeriesDescription, descripcionSerie);
							SeriesRecord->findAndGetOFString(DCM_SeriesNumber, numeroSerie);
							SeriesRecord->findAndGetOFString(DCM_ReferringPhysicianName, doctorSerie);

							pModelo->InsertarSerie(uidEstudio.c_str(),uidSerie.c_str(),modalidadSerie.c_str(),fechaSerie.c_str(),horaSerie.c_str(),descripcionSerie.c_str(),numeroSerie.c_str(),doctorSerie.c_str());
							while(((FileRecord = SeriesRecord->nextSub(FileRecord)) != NULL))
							{
								if(FileRecord->findAndGetOFStringArray(DCM_DirectoryRecordType,tmpString).good() && (tmpString == "IMAGE" || tmpString == "ENCAP DOC"))
								{
									if(FileRecord->findAndGetOFStringArray(DCM_ReferencedFileID,tmpString).good())
									{
										OFString uidImagen,instanceNumber;
										FileRecord->findAndGetOFString(DCM_ReferencedSOPInstanceUIDInFile, uidImagen);
										FileRecord->findAndGetOFString(DCM_InstanceNumber, instanceNumber);

										wxString currentPath = FROMPATH(basePath);

										wxString wxStr = FROMPATH(tmpString);
										wxString separador = wxFileName::GetPathSeparator();
										wxString barra = wxT('\\');
										wxStr.Replace(barra,separador);

										if (wxFileExists(currentPath + separador + wxStr)) 
										{
											currentPath+= separador + wxStr;
										} 
										else
										{										
											// recase: Get case insensitive path (if needed)
											wxStringTokenizer tokenizer(wxStr, separador);
											bool validPath = true; // When false, "recase" will do nothing more

											while (validPath && tokenizer.HasMoreTokens() )
											{
												wxString relPathPart = tokenizer.GetNextToken();

												if (tokenizer.HasMoreTokens()) { // Scanning subdirectories
													wxString entry;
													if (validPath && !wxDir::Exists(currentPath + separador + relPathPart)) {
														wxDir dir;
														bool cont = dir.Open(currentPath) && dir.GetFirst(&entry, wxEmptyString, wxDIR_DIRS | wxDIR_HIDDEN );
														bool found = false;
														while ( cont )
														{
															if (entry.CmpNoCase(relPathPart) == 0) {
																found = true;
																cont = false;
															}
															if (cont) {
																cont = dir.GetNext(&entry);
															}
														}
														if (!found) {
															currentPath += separador + relPathPart; // Assign case sentitive subdir to current path. (Will be non-existing)
															validPath = false;
														}
														else {
															currentPath += separador + entry; // Assign case sentitive subdir to current path
														}
													}
													else {
														currentPath += separador + relPathPart; // Assign case sentitive subdir to current path (Could be non-existing if validPath == false)
													}

												}
												else { // Scanning end file

													wxString entry;

													if (validPath && !wxFile::Access(currentPath + separador + relPathPart, wxFile::read)) {
														wxDir dir;
														bool cont = dir.Open(currentPath) && dir.GetFirst(&entry, wxEmptyString, wxDIR_FILES | wxDIR_HIDDEN );
														bool found = false;
														while ( cont )
														{
															if (entry.CmpNoCase(relPathPart) == 0) {
																cont = false;
																found = true;
															}
															if (cont) {
																cont = dir.GetNext(&entry);
															}
														}
														if (!found) {
															currentPath += separador + relPathPart; // Assign case sentitive filename to current path. (Will be non-existing)
															validPath = false;
														}
														else {
															currentPath += separador + entry; // Assign case sentitive filename to current path
														}
													}
													else {
														currentPath += separador + relPathPart; // Assign case sentitive filename to current path (Could be non-existing if validPath == false)
													}
												}

											}
										}//end recase

										pModelo->InsertarImagen(uidSerie.c_str(), uidImagen.c_str(), instanceNumber.c_str(), std::string(TOPATH(currentPath)));
									}
								}
							}
						}
					}
				}
				return true;
			} else {
				return false;
			}
		}

		bool PACSController::findAndGetTagFromFile(const std::string& ruta, unsigned short group, unsigned short element, std::string& value)
		{

			DcmFileFormat dcm;
			OFCondition   cond;
			OFString      ofval;
			bool          found = false;
			cond = dcm.loadFile(ruta.c_str());
			if (cond.good()) {
				cond = dcm.getDataset()->findAndGetOFString(DcmTagKey(group, element), ofval);
			}
			if (cond.good()) {
				value = ofval.c_str();
				found = true;
			}
			return found;
		}

		bool PACSController::EsDICOM(const std::string& path, bool accept_dicomdir, bool accept_dicomfile) const
		{
			bool valido = false;
			char magic[5] = { 'X', 'X', 'X', 'X', 0 };
			std::string emptystr;
			std::fstream dcmfile;

			dcmfile.open (path.c_str(), std::ios::in | std::ios::binary);

			if (!dcmfile.eof() && dcmfile.good()) {
				dcmfile.seekp (128, std::ios::beg);
			}
			if (!dcmfile.eof() && dcmfile.good() ) {
				dcmfile.read (magic, 4);
			}
			if (!dcmfile.eof() && dcmfile.good()) {
				if ( magic[0] == 'D' && magic[1] == 'I' && magic[2] == 'C' && magic[3] == 'M' ) {
					if (!accept_dicomdir || !accept_dicomfile) {
						// Leemos el tag 0004 0x1130 => Si existe es DICOMDIR
						bool has_dcmdirtag = false;
						GIL::DICOM::DicomDataset base;

						if (GIL::DICOM::PACSController::Instance()->findAndGetTagFromFile(path, 0x0004, 0x1200, emptystr))
						{
							has_dcmdirtag = true;
						}

						if (has_dcmdirtag) {
							if (accept_dicomdir) {
								valido = true;
							}
						}
						else {
							if (accept_dicomfile) {
								valido = true;
							}
						}
					}
					else {
						valido = true;
					}
				}
			}

			dcmfile.close();
			return valido;
		}

		bool PACSController::EsPDF(const std::string& path) const
		{
			bool valido = false;
			char magic[4] = { 'X', 'X', 'X', 'X' }; // %PDF
			std::fstream pdffile;

			pdffile.open (path.c_str(), std::ios::in | std::ios::binary);

			if (!pdffile.eof() && pdffile.good() ) {
				pdffile.read (magic, 4);
			}
			if (!pdffile.eof() && pdffile.good()) {
				if ( magic[0] == '%' && magic[1] == 'P' && magic[2] == 'D' && magic[3] == 'F' ) {
					valido = true;
				}
			}
			pdffile.close();

			return valido;
		}

		void PACSController::FillInQuery(const GIL::DICOM::DicomDataset& base, DcmDataset* query, const GNC::GCS::Ptr<DicomServer>& server)
		{			
			wxCSConv conv = GetConv(server->GetDefaultCharset());
			OFCondition cond;

			typedef GIL::DICOM::DicomDataset TJerarquia;
			//tags
			for (ListaTags::const_iterator it = base.tags.begin(); it != base.tags.end(); ++it) {
				DcmElement* e = GIL::DICOM::DICOMManager::CrearElementoConValor((*it).first.c_str());

				if (e != NULL) {
					const std::string& val = (*it).second;
					if (val.size() > 0) {
						e->putString( wxString( (*it).second.c_str(), wxConvUTF8).mb_str(conv) );
					}					
					cond = query->insert(e, true, false);

					if (cond.bad()) {
						LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << e->getTag().toString().c_str() << "): " << cond.text());
					}
				}
			}
			//sequences
			for (TJerarquia::DatasetList::const_iterator it2 = base.secuencias.begin(); it2 != base.secuencias.end(); ++it2) 
			{
				const TJerarquia& seq = (*it2);
				DcmElement* es = GIL::DICOM::DICOMManager::CrearElementoConValor(seq.tagName.c_str());
				if (es != NULL) {

					for (TJerarquia::DatasetList::const_iterator it3 = seq.items.begin(); it3 != seq.items.end(); ++it3) {
						const TJerarquia& item = (*it3);
						DcmItem* di = new DcmItem();

						for (ListaTags::const_iterator it4 = item.tags.begin(); it4 != item.tags.end(); ++it4) {
							DcmElement* ei = GIL::DICOM::DICOMManager::CrearElementoConValor((*it4).first.c_str());
							if (ei != NULL) {
								const std::string& val = (*it4).second;
								if (val.size() > 0) {
									ei->putString( wxString( (*it4).second.c_str(), wxConvUTF8).mb_str(conv) );
								}					
								cond = di->insert(ei, true, false);

								if (cond.bad()) {
									LOG_ERROR("PACSCONTROLLER", "No se pudo insertar el elemento: (" << ei->getTag().toString().c_str() << "): " << cond.text());
								}
							}

						}

						query->insertSequenceItem(es->getTag(), di);
					}					
				}
			}
		}

		bool PACSController::Print(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& film, const GIL::DICOM::DicomDataset& layout, const GIL::DICOM::DicomDataset& job, const std::list<std::string> files, GNC::IProxyNotificadorProgreso* pNotificador)
		{
			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);
			if (!server.IsValid()) {
				LOG_ERROR("C-MOVE/C-GET", "Invalid server");
				return false;
			}
			if (!server->GetReuseConnection()) {
				ReleaseConnection(connectionKey);
				GetConnection(connectionKey);
			}
			try {
				PrintAssociation f(connectionKey, "C-PRINT", pNotificador);
				OFCondition cond = f.Print(server, GNC::Entorno::Instance()->GetDicomLocalAET(), film, layout, job, files);
				if (cond.bad()) {
					throw GIL::DICOM::PACSException(cond.text(), "GIL/DICOM/Print");
				}
			}
			catch(std::exception& ex) {
				std::ostringstream os;
				os << "Internal error: " << ex.what();
				throw GIL::DICOM::PACSException(os.str(), "GIL/DICOM/Print");
			}

			return true;
		}


		bool PACSController::ObtenerEstudio(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base, IModeloDicom* pModelo, GNC::IProxyNotificadorProgreso* pNotificador, bool link)
		{
			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);
			if (!server.IsValid()) {
				LOG_ERROR("C-MOVE/C-GET", "Invalid server");
				return false;
			}

			wxCSConv conv = GetConv(server->GetDefaultCharset());

			if (!server->GetReuseConnection()) {
				ReleaseConnection(connectionKey);
				GetConnection(connectionKey);
			}

			if (pModelo == NULL) {
				return false;
			}

			DcmDataset query;
			DcmElement* e = NULL;

			e = newDicomElement(DCM_SpecificCharacterSet);
			e->putString(server->GetDefaultCharset().c_str());
			query.insert(e);

			e = newDicomElement(DCM_PatientID);
			if (query.insert(e).bad()) {
				delete e;
			}

			e = newDicomElement(DCM_PatientName);
			if (query.insert(e).bad()) {
				delete e;
			}

			e = newDicomElement(DCM_StudyDate);
			if (query.insert(e).bad()) {
				delete e;
			}

			e = newDicomElement(DCM_StudyTime);
			if (query.insert(e).bad()) {
				delete e;
			}

			e = newDicomElement(DCM_StudyID);
			if (query.insert(e).bad()) {
				delete e;
			}

			FillInQuery(base, &query, server);

			std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

			if (server->GetRetrieveSeries() || server->GetRetrieveMethod() == DicomServer::WADO) {
				//buscar + descargar series...
				//we have to know the series uids and obtain series by series
				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "SERIES", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				e = newDicomElement(DCM_SeriesInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_Modality);
				if (query.insert(e).bad()) {
					delete e;
				}

				std::list<std::string> listOfUIDS;
				std::list<std::string> listOfModalities;
				{
					NetClient<FindAssociation> f(connectionKey, "C-GET/FIND", pNotificador);

					std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

					if (server->useTLS) {
						f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
					}
					if (server->GetPACSUser() != "") {
						f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
					}

					f.QueryServer(&query, server, pModelo, localAET, CT_None);

					if (f.Stopped()){
						return false;
					}
					DcmStack* stack = f.GetResultStack();

					unsigned int numResults = 0;

					numResults = stack->card();


					for (unsigned int i = 0; i < numResults; i++) {

						if (stack->elem(i)->ident() == EVR_dataset) {
							DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
							if (dset) {
								OFString OFSSeriesInstanceUID;
								OFString OFSeriesModality;
								if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSSeriesInstanceUID).good() && dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() )
								{
									std::string seriesInstanceUID(OFSSeriesInstanceUID.c_str());
									std::string seriesModality(OFSeriesModality.c_str());

									listOfUIDS.push_back(seriesInstanceUID);
									listOfModalities.push_back(seriesModality);
								}
							}

						}
					}
				}

				for (std::list<std::string>::iterator itUIDS = listOfUIDS.begin(), itModalities = listOfModalities.begin();
					itUIDS != listOfUIDS.end(); ++itUIDS, ++itModalities) 
				{
					//it's mandatory to release connection here to start one connection for each series	
					ReleaseConnection(connectionKey);
					GetConnection(connectionKey);

					GIL::DICOM::DicomDataset baseAux = base;
					baseAux.tags["0020|000e"] = (*itUIDS);
					baseAux.tags["0008|0060"] = (*itModalities);
					ObtenerSerie(connectionKey, serverId, baseAux, pModelo, pNotificador, link);
				}

				query.clear();	
			} else if (server->GetRetrieveMethod() == DicomServer::MOVE) {
				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "STUDY", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				if (server) {
					LOG_INFO("C-MOVE", "Obteniendo estudio del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << "Method=C-MOVE");
				}

				NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);

				a.SetRole(Association::RT_Requestor);
				a.SetModelo(pModelo);

				if (server->useTLS) {
					a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
				}
				if (server->GetPACSUser() != "") {
					a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
				}
				a.QueryServer(&query, server, pModelo, localAET, CT_MoveSerie);

			}
			else {

				if (server) {
					LOG_INFO("C-GET", "Obteniendo estudio del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
				}

				//we have to know the series uids and obtain series by series
				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "SERIES", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				e = newDicomElement(DCM_SeriesInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_Modality);
				if (query.insert(e).bad()) {
					delete e;
				}

				NetClient<FindAssociation> f(connectionKey, "C-GET/FIND", pNotificador);

				std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

				if (server->useTLS) {
					f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
				}
				if (!server->GetPACSUser().empty()) {
					f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
				}

				f.QueryServer(&query, server, pModelo, localAET, CT_None);

				if (f.Stopped()){
					return false;
				}
				DcmStack* stack = f.GetResultStack();

				unsigned int numResults = 0;

				numResults = stack->card();

				for (unsigned int i = 0; i < numResults; i++) {

					if (stack->elem(i)->ident() == EVR_dataset) {
						DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
						if (dset) {
							OFString OFSStudyInstanceUID;
							OFString OFSSeriesInstanceUID;
							OFString OFSeriesModality;
							if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, OFSSeriesInstanceUID).good() && dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() )
							{
								GIL::DICOM::DicomDataset baseAux = base;

								if (dset->findAndGetOFString(DCM_StudyInstanceUID, OFSStudyInstanceUID).good()) {
									baseAux.tags["0020|000d"] = OFSStudyInstanceUID.c_str();
								}

								baseAux.tags["0020|000e"] = OFSSeriesInstanceUID.c_str();
								baseAux.tags["0008|0060"] = OFSeriesModality.c_str();

								ObtenerSerie(connectionKey, serverId, baseAux, pModelo, pNotificador, link);
							}
						}

					}
				}

				query.clear();

			}

			return true;

		}

		bool PACSController::ObtenerSerie(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base, IModeloDicom* pModelo, GNC::IProxyNotificadorProgreso* pNotificador, bool link)
		{
			if (pModelo == NULL) {
				return false;
			}
			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

			if (!server.IsValid()) {
				LOG_ERROR("C-MOVE/C-GET", "Invalid server");
				return false;
			}

			wxCSConv conv = GetConv(server->GetDefaultCharset());

			std::string modality;
			if (base.tags.find("0008|0060") != base.tags.end()) {
				modality = base.tags.find("0008|0060")->second;
			}

			DcmElement* e = NULL;
			DcmDataset query;

			unsigned int numResults = 0;

			if (server->GetRetrieveMethod() == DicomServer::GET && modality.empty()) { // We have to find series modality
				FillInQuery(base, &query, server);

				e = newDicomElement(DCM_SpecificCharacterSet);
				e->putString(server->GetDefaultCharset().c_str());
				query.insert(e);

				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "SERIES", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				e = newDicomElement(DCM_StudyInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_SeriesInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_Modality);
				if (query.insert(e).bad()) {
					delete e;
				}

				NetClient<FindAssociation> f(connectionKey, "C-GET/FIND", pNotificador);

				std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

				if (server->useTLS) {
					f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
				}
				if (server->GetPACSUser() != "") {
					f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
				}

				if (!f.QueryServer(&query, server, pModelo, localAET, CT_None)) 
				{
					return false;
				}

				if (f.Stopped()){
					return false;
				}
				DcmStack* stack = f.GetResultStack();

				OFString OFSeriesModality;

				numResults = stack->card();
				if (numResults >1) {
					LOG_WARN("C-GET", "Obteniendo serie del PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", there have a Series with more than one modality, we are going to process only first modality");
				}

				for (unsigned int i = 0; i < numResults; i++) {

					if (stack->elem(i)->ident() == EVR_dataset) {
						DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
						if (dset) {

							if ( dset->findAndGetOFString(DCM_Modality, OFSeriesModality).good() && OFSeriesModality.size() > 0 )
							{
								modality = OFSeriesModality.c_str();
								break;
							}
						}

					}
				}
				query.clear();
			}// end query modality

			//association to make finds...
			NetClient<FindAssociation> f(connectionKey, "C-GET/FIND", pNotificador);

			std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

			if (server->useTLS) {
				f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			}
			if (server->GetPACSUser() != "") {
				f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			}

			if (server->GetRetrieveMethod() == DicomServer::WADO) 
			{
				//we need to know StudyInstanceUID, SeriesInstanceUID and SopInstanceUID of all elements
				std::string seriesInstanceUID, studyInstanceUID;
				if (!base.getTag(GKDCM_SeriesInstanceUID, seriesInstanceUID)) {
					LOG_ERROR("PACSController", "to obtain a series you must specify seriesInstanceUID");
					return false;
				}
				if (!base.getTag(GKDCM_StudyInstanceUID, studyInstanceUID)) {
					FillInQuery(base, &query, server);

					e = newDicomElement(DCM_SpecificCharacterSet);
					e->putString(server->GetDefaultCharset().c_str());
					query.insert(e);

					e = newDicomElement(DCM_QueryRetrieveLevel);
					e->putString(wxString( "SERIES", wxConvUTF8).mb_str(conv));
					query.insert(e, true);

					e = newDicomElement(DCM_StudyInstanceUID);
					if (query.insert(e).bad()) {
						delete e;
					}

					e = newDicomElement(DCM_SeriesInstanceUID);
					if (query.insert(e).bad()) {
						delete e;
					}

					if (!f.QueryServer(&query, server, pModelo, localAET, CT_None)) 
					{
						return false;
					}

					if (f.Stopped()){
						return false;
					}
					DcmStack* stack = f.GetResultStack();

					OFString tempOf;

					for (unsigned int i = 0; i < stack->card(); i++) {

						if (stack->elem(i)->ident() == EVR_dataset) {
							DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
							if (dset) {

								if ( dset->findAndGetOFString(DCM_StudyInstanceUID, tempOf).good() && tempOf.size() > 0 )
								{
									studyInstanceUID = tempOf.c_str();
									break;
								}
							}

						}
					}
					query.clear();
				}
				if (studyInstanceUID.empty()) {
					LOG_ERROR("PACSController", "StudyInstanceUID not found");
					return false;
				}

				//now we have to get all sop instance uids...
				f.DeleteResultStack();
				e = newDicomElement(DCM_SpecificCharacterSet);
				e->putString(server->GetDefaultCharset().c_str());
				query.insert(e);

				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "IMAGE", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				e = newDicomElement(DCM_SeriesInstanceUID);
				e->putString(wxString( seriesInstanceUID.c_str(), wxConvUTF8).mb_str(conv));
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_SOPInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_InstanceNumber);
				if (query.insert(e).bad()) {
					delete e;
				}

				if (!f.QueryServer(&query, server, pModelo, localAET, CT_None)) 
				{
					return false;
				}

				if (f.Stopped()){
					return false;
				}
				DcmStack* stack = f.GetResultStack();

				OFString tempOf;
				std::list<std::pair<long, std::string> > instances;

				for (unsigned int i = 0; i < stack->card(); i++) {
					if (stack->elem(i)->ident() == EVR_dataset) {
						DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
						if (dset) {
							if ( dset->findAndGetOFString(DCM_SOPInstanceUID, tempOf).good() && tempOf.size() > 0 )
							{
								const std::string sopInstanceUID = tempOf.c_str();
								long instanceNumber;
								std::pair<long, std::string> item;
								item.second = sopInstanceUID;
								if ( dset->findAndGetLongInt(DCM_InstanceNumber, instanceNumber).good() ) {
									item.first = instanceNumber;
								} else {
									item.first = LONG_MAX;
								}
								instances.push_back(item);
							}
						}
					}
				}
				query.clear();
				
				//sort by instance number...
				instances.sort(compare_instances);
				std::list<std::string> sopInstanceUIDs;
				std::list<long> instanceNumbers;
				for (std::list<std::pair<long, std::string> >::const_iterator it = instances.begin(); it != instances.end(); ++it) {
					sopInstanceUIDs.push_back((*it).second);
					instanceNumbers.push_back((*it).first);
				}

				//now we have studyInstanceUID, seriesInstanceUID and sopInstancesUIDS.... download it!
				return DownloadWADOImages(serverId, studyInstanceUID, seriesInstanceUID, sopInstanceUIDs, instanceNumbers, pModelo, pNotificador, link);
			}//end wado
			else 
			{//get and move
				FillInQuery(base, &query, server);

				e = newDicomElement(DCM_SpecificCharacterSet);
				e->putString(server->GetDefaultCharset().c_str());
				query.insert(e);			

				e = newDicomElement(DCM_QueryRetrieveLevel);
				e->putString(wxString( "SERIES", wxConvUTF8).mb_str(conv));
				query.insert(e, true);

				e = newDicomElement(DCM_SeriesInstanceUID);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_Modality);
				if (query.insert(e).bad()) {
					delete e;
				}

				e = newDicomElement(DCM_SeriesNumber);
				if (query.insert(e).bad()) {
					delete e;
				}

				std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

				if (server->GetRetrieveMethod() == DicomServer::MOVE) {

					if (server) {
						LOG_INFO("C-MOVE", "Downloading series from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
					}

					if (server) {
						LOG_INFO("C-MOVE", "Downloading study from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
					}

					NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);
					a.SetRole(Association::RT_Requestor);
					a.SetModelo(pModelo);


					if (server->useTLS) {
						a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
					}
					if (server->GetPACSUser() != "") {
						a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
					}
					if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveSerie)) {
						return false;
					}
				}
				else {
					if (server) {
						LOG_INFO("C-GET", "Downloading series from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
					}

					NetClient<GetAssociation> a(connectionKey, "C-GET", pNotificador);
					a.SetWellKnownNumResults(numResults);
					a.SetStorageSOPClasses(GIL::DICOM::Conformance::GetModalities().GetSupportedSOPClassUIDs(modality));
					a.SetModelo(pModelo);
					
					if (server->useTLS) {
						a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
					}
					if (server->GetPACSUser() != "") {
						a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
					}
					if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveSerie)) {
						return false;
					}
				}
				query.clear();
			}
			return true;

		}

		bool PACSController::ObtenerImagen(void* connectionKey, const std::string& serverId, const GIL::DICOM::DicomDataset& base, IModeloDicom* pModelo,GNC::IProxyNotificadorProgreso* pNotificador)
		{
			if (pModelo == NULL) {
				return false;
			}
			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

			if (!server.IsValid()) {
				LOG_ERROR("C-MOVE/C-GET", "Invalid server");
				return false;
			}

			wxCSConv conv = GetConv(server->GetDefaultCharset());

			DcmDataset query;
			DcmElement* e = NULL;

			FillInQuery(base, &query, server);

			e = newDicomElement(DCM_SpecificCharacterSet);
			e->putString(server->GetDefaultCharset().c_str());
			query.insert(e);

			e = newDicomElement(DCM_QueryRetrieveLevel);
			e->putString(wxString( "IMAGE", wxConvUTF8).mb_str(conv));
			query.insert(e, true);

			e = newDicomElement(DCM_InstanceNumber);
			if (query.insert(e).bad()) {
				delete e;
			}

			std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

			if (server->GetRetrieveMethod() == DicomServer::MOVE) {

				if (server) {
					LOG_INFO("C-MOVE", "Downloading imagen from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-MOVE");
				}

				NetClient<MoveAssociation> a(connectionKey, "C-MOVE", pNotificador);
				a.SetModelo(pModelo);
				
				if (server->useTLS) {
					a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
				}
				if (server->GetPACSUser() != "") {
					a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
				}
				if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveImagen)) {
					return false;
				}

			}
			else if (server->GetRetrieveMethod() == DicomServer::GET) {

				if (server) {
					LOG_INFO("C-GET", "Downloading image from PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser << ", Method=C-GET");
				}

				NetClient<GetAssociation> a(connectionKey, "C-GET", pNotificador);
				a.SetModelo(pModelo);
				
				if (server->useTLS) {
					a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
				}
				if (server->GetPACSUser() != "") {
					a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
				}
				if (!a.QueryServer(&query, server, pModelo, localAET, CT_MoveImagen))  {
					return false;
				}
			} else if (server->GetRetrieveMethod() == DicomServer::WADO) {
				//we have to get seriesInstance and studyInstanceUID...
				std::string seriesInstanceUID, studyInstanceUID;
				if (!base.getTag(GKDCM_SeriesInstanceUID, seriesInstanceUID) || !base.getTag(GKDCM_StudyInstanceUID, studyInstanceUID)) {
					FillInQuery(base, &query, server);

					e = newDicomElement(DCM_SpecificCharacterSet);
					e->putString(server->GetDefaultCharset().c_str());
					query.insert(e);

					e = newDicomElement(DCM_QueryRetrieveLevel);
					e->putString(wxString( "IMAGE", wxConvUTF8).mb_str(conv));
					query.insert(e, true);

					e = newDicomElement(DCM_StudyInstanceUID);
					if (query.insert(e).bad()) {
						delete e;
					}
					e = newDicomElement(DCM_SeriesInstanceUID);
					if (query.insert(e).bad()) {
						delete e;
					}

					//association to make finds...
					NetClient<FindAssociation> f(connectionKey, "WADO/FIND", pNotificador);

					std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

					if (server->useTLS) {
						f.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
					}
					if (server->GetPACSUser() != "") {
						f.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
					}


					if (!f.QueryServer(&query, server, pModelo, localAET, CT_None)) 
					{
						return false;
					}

					if (f.Stopped()){
						return false;
					}
					DcmStack* stack = f.GetResultStack();

					OFString tempOf;

					for (unsigned int i = 0; i < stack->card(); i++) {

						if (stack->elem(i)->ident() == EVR_dataset) {
							DcmDataset* dset = dynamic_cast<DcmDataset*>(stack->elem(i));
							if (dset) {

								if ( dset->findAndGetOFString(DCM_StudyInstanceUID, tempOf).good() && tempOf.size() > 0 )
								{
									studyInstanceUID = tempOf.c_str();
								}
								if ( dset->findAndGetOFString(DCM_SeriesInstanceUID, tempOf).good() && tempOf.size() > 0 )
								{
									seriesInstanceUID = tempOf.c_str();
								}
							}
						}
					}
					query.clear();
				}
				if (studyInstanceUID.empty() || seriesInstanceUID.empty()) {
					LOG_ERROR("PACSController", "StudyInstanceUID or SeriesInstanceUID not found");
					return false;
				}
				std::list<std::string> sopInstanceUIDs;
				sopInstanceUIDs.push_back(base.getTag(GKDCM_SOPInstanceUID));
				std::list<long> instanceNumbers;
				instanceNumbers.push_back(0);
				//now we have studyInstanceUID, seriesInstanceUID and sopInstancesUIDS.... download it!
				return DownloadWADOImages(serverId, studyInstanceUID, seriesInstanceUID, sopInstanceUIDs, instanceNumbers, pModelo, pNotificador, false);
			}
			return true;
		}

		size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
			size_t written = fwrite(ptr, size, nmemb, stream);
			return written;
		}

		//sopinstanceuids are sorted by instance number!
		bool PACSController::DownloadWADOImages(const std::string& serverId, 
				const std::string& studyInstanceUID,
				const std::string& seriesInstanceUID,
				const std::list<std::string>& sopInstanceUID,
				const std::list<long>& instanceNumbers,
				IModeloDicom* pModelo,
				GNC::IProxyNotificadorProgreso* pNotificador,
				bool link)
		{
			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

			std::string pathOfSeries = GNC::Entorno::Instance()->CrearDirectorioTemporal();

			int curr = 1;
			std::list<long>::const_iterator itInstanceNumber = instanceNumbers.begin();
			for (std::list<std::string>::const_iterator itSopInstances = sopInstanceUID.begin(); itSopInstances != sopInstanceUID.end(); ++itSopInstances, curr++, itInstanceNumber++)
			{
				std::ostringstream progress;
				progress << _Std("Downloading file") << " " << curr << "/" << sopInstanceUID.size();
				if (!pNotificador->NotificarProgreso((float)(curr-1)/sopInstanceUID.size(), progress.str())) {
					return false;
				}
				std::ostringstream ostr;
				ostr << server->GetWADOURI();
				if (server->GetWADOURI().find("?") == std::string::npos) {
					ostr << "?";
				} else {
					ostr << "&";
				}
				ostr << "requestType=WADO&contentType=application/dicom&studyUID=" << studyInstanceUID;
				ostr << "&seriesUID=" << seriesInstanceUID;
				ostr << "&objectUID=" << (*itSopInstances);
				std::string url = ostr.str();

				LOG_DEBUG("DownloadWADOImages", "Downloading url: " << url)
				if (link && curr > 1) {
					//link it
					const IModeloSerie* pSeries;
					pModelo->BuscarSerie(seriesInstanceUID, &pSeries);
					std::ostringstream ostr;
					ostr << (*itInstanceNumber);
					pModelo->InsertarImagen(seriesInstanceUID,(*itSopInstances), ostr.str(),url);
				} else {
					//download first file of the series...
					std::ostringstream ofilePath;
					ofilePath << pathOfSeries << (char)wxFileName::GetPathSeparator();
					ofilePath << (*itSopInstances);
					std::string filePath = ofilePath.str();
					if (DownloadFileFromURL(url, filePath, pNotificador)) {
						DICOMManager dm;
						GIL::DICOM::DicomDataset base;
						dm.CargarFichero(filePath, base);
						const IModeloPaciente& paciente = pModelo->InsertarPaciente(base.getTag(GKDCM_PatientID), base.getTag(GKDCM_PatientName), base.getTag(GKDCM_PatientBirthDate), base.getTag(GKDCM_PatientSex));
						pModelo->InsertarEstudio(paciente.GetUID(), base.getTag(GKDCM_StudyInstanceUID), base.getTag(GKDCM_AccessionNumber), base.getTag(GKDCM_StudyDescription), base.getTag(GKDCM_ModalitiesInStudy), base.getTag(GKDCM_StudyDate), base.getTag(GKDCM_StudyTime), base.getTag(GKDCM_ReferringPhysicianName));
						pModelo->InsertarSerie(base.getTag(GKDCM_StudyInstanceUID), base.getTag(GKDCM_SeriesInstanceUID), base.getTag(GKDCM_SeriesType), base.getTag(GKDCM_SeriesDate), base.getTag(GKDCM_SeriesTime), base.getTag(GKDCM_SeriesDescription), base.getTag(GKDCM_SeriesNumber),base.getTag(GKDCM_ReferringPhysicianName));
						pModelo->InsertarImagen(base.getTag(GKDCM_SeriesInstanceUID),base.getTag(GKDCM_SOPInstanceUID), base.getTag(GKDCM_InstanceNumber), filePath);
					}
				}//end download first file of the series
			}
			return true;
		}

		int progress_func_download_wado(void* ptr, double TotalToDownload, double NowDownloaded, double , double )
		{
			GNC::IProxyNotificadorProgreso* pCmd = (GNC::IProxyNotificadorProgreso*)(ptr);
			if (pCmd != NULL && TotalToDownload != 0) {
				if (pCmd->NotificarProgreso((float)NowDownloaded/TotalToDownload, "")) {
					return 0;
				} else {
					return -1;
				}
			} else if (pCmd != NULL && NowDownloaded != 0) {
				wxString message = wxString::Format(wxT("Unknow file size downloaded %.2f Mb"), (float)NowDownloaded/(1024*1024));
				if (pCmd->NotificarProgreso((float)NowDownloaded/(10*1024*1024), std::string(message.ToUTF8()))) {
					return 0;
				} else {
					return -1;
				}
			} else {
				return 0;
			}
		}

		bool PACSController::DownloadFileFromURL(const std::string& url, const std::string& filePath, GNC::IProxyNotificadorProgreso* pNotifier)
		{
			LOG_DEBUG("PACSController", "Trying to download: " << url);
			CURL *http_handle;
			FILE *fp;

			http_handle = curl_easy_init();
			if (http_handle) {
				fp = fopen(filePath.c_str(), "wb");
				curl_easy_setopt(http_handle, CURLOPT_URL, url.c_str());					
				//proxy settings
				GNC::GCS::ProxySettings settings = GNC::GCS::ConfigurationController::Instance()->GetProxySettings();
				if (settings.useProxy) {
					curl_easy_setopt(http_handle, CURLOPT_PROXY, settings.hostName.c_str());
					curl_easy_setopt(http_handle, CURLOPT_PROXYPORT, settings.port);
					if (settings.requiresAuth) {
						curl_easy_setopt(http_handle, CURLOPT_PROXYUSERNAME, settings.userName.c_str());
						curl_easy_setopt(http_handle, CURLOPT_PROXYPASSWORD, settings.password.c_str());
					}
					curl_easy_setopt(http_handle, CURLOPT_NOPROXY, settings.exceptions.c_str());
				}
				//progress
				curl_easy_setopt(http_handle, CURLOPT_PROGRESSDATA , pNotifier);
				curl_easy_setopt(http_handle, CURLOPT_PROGRESSFUNCTION, progress_func_download_wado);
				curl_easy_setopt(http_handle, CURLOPT_NOPROGRESS, 0);
				//
				curl_easy_setopt(http_handle, CURLOPT_WRITEFUNCTION, write_data);
				curl_easy_setopt(http_handle, CURLOPT_WRITEDATA, fp);
				CURLcode res = curl_easy_perform(http_handle);
				fclose(fp);
				long http_code = 0;
				curl_easy_getinfo (http_handle, CURLINFO_RESPONSE_CODE, &http_code);
				if (http_code == 200 && res != CURLE_ABORTED_BY_CALLBACK)
				{
					//succeeed
					curl_easy_cleanup(http_handle);
					return true;
				} else {
					curl_easy_cleanup(http_handle);
					LOG_ERROR("PACSController", "Error downloading file " << url << " Curl code (" << res << ") " << curl_easy_strerror(res) << " http status: " << http_code );
					std::ostringstream ostrMsg;
					ostrMsg << _Std("Error downloading WADO file")  << " (HTTP Status: " << http_code << ") " << "URL :" << std::endl << url << std::endl << _Std("Check PACS configuration");
					throw GIL::DICOM::PACSException( ostrMsg.str());
				}
			} else {
				LOG_ERROR("PACSController", "Error initializing curl");
				throw GIL::DICOM::PACSException( "Error initialiting curl" );
			}
		}

		/** Perform query */
		bool PACSController::Query(
			void* connectionKey,
			const std::string& abstractSyntax,
			const GIL::DICOM::DicomDataset& queryWrapper,
			std::list< GNC::GCS::Ptr<GIL::DICOM::DicomDataset> >& resultsWrapper,				
			const std::string& serverId,
			GNC::IProxyNotificadorProgreso* pNotificador
			)
		{

			resultsWrapper.clear();

			DicomServerList* listaServidores = DicomServerList::Instance();
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);

			DcmDataset query;
			DcmElement* e = NULL;

			e = newDicomElement(DCM_SpecificCharacterSet);
			e->putString(server->GetDefaultCharset().c_str());
			query.insert(e);

			FillInQuery(queryWrapper, &query, server);

			std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();

			NetClient<FindAssociation> a(connectionKey, "C-FIND", pNotificador);

			a.SetAbstractSyntax(abstractSyntax);

			if (server->useTLS) {
				a.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			}
			if (server->GetPACSUser() != "") {
				a.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			}
			
			return a.QueryServer(&query, server, resultsWrapper, localAET);
		}


		bool PACSController::QueryRetrieve(
			void* connectionKey,
			const std::string& serverId,
			IModeloDicom* pModelo,
			const GIL::DICOM::DicomDataset& base,
			GNC::IProxyNotificadorProgreso* pNotificador,
			bool link)
		{
			std::string strTmp;
			if(base.getTag("0008|0052", strTmp)) {
				if (strTmp == "STUDY") {
					ObtenerEstudio(connectionKey, serverId, base, pModelo, pNotificador, link);
				} else if (strTmp == "SERIES") {
					std::string uidSeries;
					ObtenerSerie(connectionKey, serverId, base, pModelo, pNotificador, link);
				} else if (strTmp == "IMAGE")  {
					ObtenerImagen(connectionKey, serverId, base, pModelo, pNotificador);
				} else  {
					throw GIL::DICOM::PACSException(_Std("Unknown query/retrieve level"));
				}
			} else {
				throw GIL::DICOM::PACSException(_Std("Query retrieve level not specified"));
			}

			return true;
		}

		void PACSController::TestWADOURL(const std::string& url) {
			CURL *http_handle = NULL;
			std::ostringstream errorMsg;
			bool success = true;

			char curl_err[CURL_ERROR_SIZE];

			http_handle = curl_easy_init();
			if (http_handle) {
				curl_easy_setopt(http_handle, CURLOPT_URL, url.c_str());					
				//proxy settings
				GNC::GCS::ProxySettings settings = GNC::GCS::ConfigurationController::Instance()->GetProxySettings();
				if (settings.useProxy) {
					curl_easy_setopt(http_handle, CURLOPT_PROXY, settings.hostName.c_str());
					curl_easy_setopt(http_handle, CURLOPT_PROXYPORT, settings.port);
					if (settings.requiresAuth) {
						curl_easy_setopt(http_handle, CURLOPT_PROXYUSERNAME, settings.userName.c_str());
						curl_easy_setopt(http_handle, CURLOPT_PROXYPASSWORD, settings.password.c_str());
					}
					curl_easy_setopt(http_handle, CURLOPT_NOPROXY, settings.exceptions.c_str());
				}

				curl_easy_setopt(http_handle, CURLOPT_ERRORBUFFER, curl_err);
				long http_code = -1;

				CURLcode res = curl_easy_perform(http_handle);
				
				if (res != CURLE_OK) {
					errorMsg << _Std("Unable to perform HTTP request") << ": " << curl_err;
					success = false;
				}
				if (success && (CURLE_OK != curl_easy_getinfo (http_handle, CURLINFO_RESPONSE_CODE, &http_code)) ) {
					errorMsg << _Std("Invalid HTTP response")  << ": " << curl_err;
					success = false;
				}
				if (success) {
					if ( (http_code < 200 || http_code > 500) ) {
						errorMsg << _Std("Invalid HTTP code") << ":" << http_code;
						success = false;
					}
				}
				curl_easy_cleanup(http_handle);

			} else {
				errorMsg << _Std("Unable to init HTTP subsystem");
				success = false;
			}

			if (!success) {
				throw GIL::DICOM::PACSException(errorMsg.str());
			}
		}

		GIL::DICOM::ICustomAssociation* PACSController::CreateCustomAssociation(void* connectionKey, const std::string& ambitolog, GNC::IProxyNotificadorProgreso* pNotificador)
		{
			return new CustomAssociation(connectionKey, ambitolog, pNotificador);
		}

		void PACSController::DestroyCustomAssociation(GIL::DICOM::ICustomAssociation* assoc)
		{
			if (assoc != NULL) {
				delete assoc;
			}
		}

		void PACSController::StartServer()
		{
			StopServer();

			int localPort = 0;
			std::string localAET;
			{
				GNC::GCS::ConfigurationController::Instance()->readIntGeneral("/GinkgoCore/PACS/Local", "Puerto", localPort, 11112);
				GNC::GCS::ConfigurationController::Instance()->readStringGeneral("/GinkgoCore/PACS/Local", "AET", localAET, "GINKGO_%IP4");
			}

			this->m_pServiceInstance = new GIL::DICOM::Service(_Std("PACS-Service"));
			this->m_pServiceInstance->SetRole(GIL::DICOM::Service::RT_Acceptor);
			this->m_pServiceInstance->SetAcceptorPort(localPort);

			this->m_pServiceInstance->SetLocalAET(localAET);
			this->m_pServiceInstance->Start();

		}

		void PACSController::StopServer()
		{
			if (this->m_pServiceInstance != NULL) {
				unsigned long tid;
				do {
					tid = this->m_pServiceInstance->GetTid();
					GNC::GCS::ThreadController::Stop(tid);
				} while (this->m_pServiceInstance->GetTid() != tid);
				delete this->m_pServiceInstance;
				this->m_pServiceInstance = NULL;
			}
		}


		void PACSController::GetConnection(void* connectionKey)
		{
			GIL::DICOM::DCMTK::Network::Instance(connectionKey);
		}

		void PACSController::ReleaseConnection(void* connectionKey)
		{
			GIL::DICOM::DCMTK::Network::FreeInstance(connectionKey);
		}

		void PACSController::SubirArchivos(void* /*connectionKey*/, const std::string& serverId, const std::list<std::string> &pathsSubida, GNC::IProxyNotificadorProgreso* pNotificador, TipoTransferSyntaxEnvio transferSyntax)
		{

			GTRACE("PACSController::SubirArchivos( " << serverId.c_str() << ", [...]");
			DicomServerList* listaServidores = DicomServerList::Instance();
			std::string localAET = GNC::Entorno::Instance()->GetDicomLocalAET();
			GIL::DICOM::DicomStoreAssociation asociacion;
			GNC::GCS::Ptr<DicomServer> server = listaServidores->GetServer(serverId);
			if (server) {
				LOG_INFO("PACS-STORE", "Enviando al PACS " << serverId << ": " << server->AET << "@" << server->HostName << ":" << server->Port << " PDU=" << server->PDU << ", TLS=" << server->useTLS << ",  User = " << server->pacsUser);
			}
			if (server->useTLS) {				
				asociacion.SetTLS(server->GetCertificate(), server->GetPrivateKey(), server->GetverifyCredentials());
			}
			if (server->GetPACSUser() != "") {
				asociacion.SetUserPass(server->GetPACSUser(), server->GetPACSPass());
			}
			asociacion.Store(pathsSubida,server, localAET, pNotificador, transferSyntax);

		}

		std::string GetMD5(const std::string& cadena)
		{
			unsigned char salida[MD5_DIGEST_LENGTH];
			MD5((const unsigned char*)cadena.c_str(), cadena.size(),salida);

			std::ostringstream cadenaMD5;
			cadenaMD5.setf ( std::ios::hex, std::ios::basefield );
			for(int i = 0; i<MD5_DIGEST_LENGTH; i++)
			{
				cadenaMD5 << (int)salida[i];
			}
			return cadenaMD5.str();
		}

		bool PACSController::GetRutaSerie(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, std::string& ruta, bool crearDirectorios,bool temporal)
		{
			bool correcto = true;
			std::string dicomDirPath = GNC::GCS::HistoryController::Instance()->GetGinkgoDicomDir();
			//md5 de uidpaciente, estudio y serie
			std::stringstream concat;
			if(uidPaciente != "")
				concat << uidPaciente;
			else
				concat << _("Unknown");
			if(uidEstudio != "")
				concat << uidEstudio;
			else
				concat << _("Unknown");
			if(uidSerie != "")
				concat << uidSerie;
			else
				concat << _("Unknown");
			std::string cadena = concat.str();
			std::string resumen = GetMD5(cadena);

			wxString dirRaiz, dirSerie;
			dirRaiz = FROMPATH(dicomDirPath);
			if (!wxDir::Exists(dirRaiz) && crearDirectorios) {
				correcto = correcto && wxFileName::Mkdir(dirRaiz, 0700);
			}

			if(temporal) {
				dirRaiz = dirRaiz + wxFileName::GetPathSeparator(wxPATH_NATIVE) + wxT("TEMP");
				if (!wxDir::Exists(dirRaiz)) {
					correcto = correcto && wxFileName::Mkdir(dirRaiz, 0700);
				}
			}

			dirSerie= dirRaiz + wxFileName::GetPathSeparator(wxPATH_NATIVE) + FROMPATH(resumen);
			if (!wxDir::Exists(dirSerie) && crearDirectorios) {
				correcto = correcto && wxFileName::Mkdir(dirSerie, 0700);
			}

			ruta = TOPATH(dirSerie);
			return correcto;
		}

		bool PACSController::GetRutaImagen(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, std::string& ruta, bool crearDirectorios)
		{
			bool correcto = GetRutaSerie(uidPaciente,uidEstudio,uidSerie, ruta, crearDirectorios, false);
			std::ostringstream ostr;
			ostr << ruta << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE)<<GetMD5(uidImagen)<<".dcm";
			ruta = ostr.str();
			return correcto;
		}

		bool PACSController::GetRutaImagenTemp(const std::string& uidPaciente, const std::string& uidEstudio, const std::string& uidSerie, const std::string& uidImagen, std::string& ruta, bool crearDirectorios)
		{
			bool correcto = GetRutaSerie(uidPaciente,uidEstudio,uidSerie, ruta, crearDirectorios, true);
			std::ostringstream ostr;
			ostr << ruta << (char)wxFileName::GetPathSeparator(wxPATH_NATIVE)<<GetMD5(uidImagen)<<".dcm";
			ruta = ostr.str();
			return correcto;
		}

		void PACSController::DumpFileElements(const std::string& rutaFichero, IInspectCallBack* callback)
		{
			DcmFileFormat fileformat;
			if (rutaFichero.empty() || callback == NULL) {
				return;
			}
			OFCondition status = fileformat.loadFile(rutaFichero.c_str());
			if (status.good()) {

				DcmDataset* ds = fileformat.getDataset();

				DcmDataDictionary& globalDataDict = dcmDataDict.wrlock();
				DcmHashDictIterator iter(globalDataDict.normalBegin());
				DcmHashDictIterator end(globalDataDict.normalEnd());

				std::string keyStr;
				std::string descStr;
				std::string valStr;
				for (; iter != end; ++iter) {
					DcmTagKey tagkey = (*iter)->getKey();
					keyStr = tagkey.toString().c_str();
					descStr = (*iter)->getTagName();
					OFString val;
					OFCondition c = ds->findAndGetOFString(tagkey, val);
					if (c.good()) {
						valStr = val.c_str();
						callback->Inspect(keyStr, descStr, valStr);
					}
				}
				dcmDataDict.unlock();
			}
		}


		//region "Métodos de Dicomización"
		/*
		void PACSController::CrearInstanciaDeImportacion(void* ref) {
		m_ListaInstanciasDicomImg2DCM[ref] = new IDICOMImg2DCM();
		}

		void PACSController::LiberarInstanciaDeImportacion(void* ref) {
		ListaInstanciasImg2DCMType::iterator it = m_ListaInstanciasDicomImg2DCM.find(ref);
		if (it != m_ListaInstanciasDicomImg2DCM.end()) {
		delete (*it).second;
		m_ListaInstanciasDicomImg2DCM.erase(it);
		}
		}
		*/

		IDICOMImg2DCM* PACSController::CrearInstanciaDeImportacion() {
			return new DICOMImg2DCM();
		}

		void PACSController::LiberarInstanciaDeImportacion(IDICOMImg2DCM* instancia) {
			DICOMImg2DCM* i = dynamic_cast<DICOMImg2DCM*>(instancia);
			if (i != NULL) {
				delete i;
			}
		}
		//endregion

		//region "Métodos de carga/almacenamiento de DICOMS"
		IDICOMManager* PACSController::CrearInstanciaDeDICOMManager() {
			return new DICOMManager();
		}

		void PACSController::LiberarInstanciaDeDICOMManager(IDICOMManager* instancia) {
			DICOMManager* i = dynamic_cast<DICOMManager*>(instancia);
			if (i != NULL) {
				delete i;
			}
		}

		void  BorrarDirTemp(wxString dirPath)
		{
			//borra en cascada
			if(!wxRmdir(dirPath)){
				//vaciar
				wxDir dir;
				if (dir.Open(dirPath)) {
					wxString fileName;
					bool cont = dir.GetFirst(&fileName);
					while (cont) {
						fileName=dir.GetName()+ wxFileName::GetPathSeparator(wxPATH_NATIVE) +fileName;
						if(wxDir::Exists(fileName)){
							BorrarDirTemp(fileName);
						}else{
							wxRemoveFile(fileName);
						}
						cont = dir.GetNext(&fileName);
					}
				}
			}
			if(wxDir::Exists(dirPath)){
				wxRmdir(dirPath);
			}
		}

		void PACSController::PurgarDirectorioTemporal()
		{
			wxString dirTemp;
			dirTemp = FROMPATH(GNC::GCS::HistoryController::Instance()->GetGinkgoDicomDir());
			if (!wxDir::Exists(dirTemp)) {
				return;
			}

			dirTemp = dirTemp + wxFileName::GetPathSeparator(wxPATH_NATIVE) + wxT("TEMP");
			if (!wxDir::Exists(dirTemp)) {
				return;
			}

			//borrar en profundidad
			BorrarDirTemp(dirTemp);
		}
		//endregion
	};
};
