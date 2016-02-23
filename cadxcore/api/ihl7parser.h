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
#include <api/api.h>
#include "imodelohl7.h"
#include <ostream>
#include <iomanip>
#include <sstream>
#include <ctime>

namespace GIL {
	namespace Hl7Parser {

	namespace Segmentos {

				class MSH : public GIL::HL7::Segment {

				protected:

				public:

					MSH(const std::string& messageType, const std::string& messageTriggerEvent, const std::string& messageHeader, const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("MSH",rs,fs,sfs)
					{
						GIL::HL7::Segment& msh = *this;

						msh <<= GIL::HL7::Field(1, "MSH.1") << GIL::HL7::Component(1, "", m_FS);
						msh <<= GIL::HL7::Field(2, "MSH.2") << GIL::HL7::Component(1, "", m_CS + "~\\&");

						msh <<= GIL::HL7::Field(3, "MSH.3") << GIL::HL7::Component(1, "HD.1"); // Sending Application
						msh <<= GIL::HL7::Field(4, "MSH.4") << GIL::HL7::Component(1, "HD.1"); // Sending Facility
						msh <<= GIL::HL7::Field(5, "MSH.5") << GIL::HL7::Component(1, "HD.1"); // Receiving Application
						msh <<= GIL::HL7::Field(6, "MSH.6") << GIL::HL7::Component(1, "HD.1"); // Receiving Facility

						msh <<= GIL::HL7:: Field(7, "MSH.7") << GIL::HL7::Component(1, "TS.1"); // TimeStamp: YYYYMMDDhhmmss

						msh <<= GIL::HL7::Field(9, "MSH.9") << GIL::HL7::Component(1, "MSG.1", messageType) << GIL::HL7::Component(2, "MSG.2", messageTriggerEvent) << GIL::HL7::Component(3, "MSG.3", messageHeader); // Message Type

						msh <<= GIL::HL7::Field(10, "MSH.10") << GIL::HL7::Component(1, ""); // Message Control ID
						msh <<= GIL::HL7::Field(11, "MSH.11") << GIL::HL7::Component(1, "PT.1", "P"); // Processing ID
						msh <<= GIL::HL7::Field(12, "MSH.12") << GIL::HL7::Component(1, "VID.1", "2.5"); // Version ID
						msh <<= GIL::HL7::Field(15, "MSH.15") << GIL::HL7::Component(1, "", "AL"); // Accept Acknowledgment type
						msh <<= GIL::HL7::Field(16, "MSH.16") << GIL::HL7::Component(1, "", "NE"); // Application Acknowledgement type
					}

					MSH(const GIL::HL7::Segment& segment)
					{
						*this = segment;
					}

					MSH& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& msh = *this;
						msh = segment;
						return *this;
					}

					void SetSendingApplication(const std::string& sendingApplication)
					{
						GIL::HL7::Segment& msh = *this;
						msh[3][1] = sendingApplication;
					}

					const std::string& GetSendingApplication()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[3][1];
					}

					void SetSendingFacility(const std::string& sendingFacility)
					{
						GIL::HL7::Segment& msh = *this;
						msh[4][1] = sendingFacility;
					}

					const std::string& GetSendingFacility()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[4][1];
					}

					void SetReceivingApplication(const std::string& receivingApplication)
					{
						GIL::HL7::Segment& msh = *this;
						msh[5][1] = receivingApplication;
					}

					const std::string& GetReceivingApplication()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[5][1];
					}

					void SetReceivingFacility(const std::string& receivingFacility)
					{
						GIL::HL7::Segment& msh = *this;
						msh[6][1] = receivingFacility;
					}

					const std::string& GetReceivingFacility()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[6][1];
					}

					void SetTimeStamp(const std::string& messageTimeStamp)
					{
						GIL::HL7::Segment& msh = *this;
						msh[7][1] = messageTimeStamp;
					}

					const std::string& GetTimeStamp()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[7][1];
					}

					void SetMessageType(const std::string& messageType)
					{
						GIL::HL7::Segment& msh = *this;
						msh[9][1] = messageType;
					}

					const std::string& GetMessageType()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[9][1];
					}

					void SetMessageTriggerEvent(const std::string& triggerEvent)
					{
						GIL::HL7::Segment& msh = *this;
						msh[9][2] = triggerEvent;
					}

					const std::string& GetMessageTriggerEvent()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[9][2];
					}

					void SetMessageHeader(const std::string& messageHeader)
					{
						GIL::HL7::Segment& msh = *this;
						msh[9][3] = messageHeader;
					}

					const std::string& GetMessageHeader()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[9][3];
					}

					void SetMessageControlId(const std::string& messageControlId)
					{
						GIL::HL7::Segment& msh = *this;
						msh[10][1] = messageControlId;
					}

					const std::string& GetMessageControlId()
					{
						GIL::HL7::Segment& msh = *this;
						return msh[10][1];
					}
				};

				class MSA : public GIL::HL7::Segment {

				public:

					typedef enum ACK_Code {
						AC_Success,
						AC_Error,
						AC_Rejected,
						AC_Unknown
					} ACK_Code;

					MSA(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("MSA", rs, fs, sfs)
					{
						GIL::HL7::Segment& msa = *this;

						msa <<= GIL::HL7::Field(1, "MSA.1") << GIL::HL7::Component(1, "");
						msa <<= GIL::HL7::Field(2, "MSA.2") << GIL::HL7::Component(1, "");
					}

					MSA& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& msa = *this;
						msa = segment;
						return *this;
					}

					void SetReferralMessageControlId(const std::string& refMsgControlId)
					{
						GIL::HL7::Segment& msa = *this;
						msa[2][1] = refMsgControlId;
					}

					const std::string& GetReferralMessageControlId()
					{
						GIL::HL7::Segment& msa = *this;
						return msa[2][1];
					}

					void SetAckCode(ACK_Code code)
					{
						GIL::HL7::Segment& msa = *this;
						switch(code) {
							case AC_Success:
								msa[1][1] = "CA";
								break;
							case AC_Rejected:
								msa[1][1] = "CR";
								break;
							case AC_Error:
							case AC_Unknown:
							default:
								msa[1][1] = "CE";
								break;
						}

					}

					ACK_Code GetAckCode()
					{
						GIL::HL7::Segment& msa = *this;
						if (msa[1][1] == "CA" || msa[1][1] == "AA")
						{
							return AC_Success;
						}
						else if (msa[1][1] == "CR" || msa[1][1] == "AR")
						{
							return AC_Rejected;
						}
						else if (msa[1][1] == "CE" || msa[1][1] == "AE")
						{
							return AC_Error;
						}
						else {
							return AC_Unknown;
						}
					}

					void SetAckCodeString(const std::string& ackCodeString)
					{
						GIL::HL7::Segment& msa = *this;
						msa[1][1] = ackCodeString;
					}

					const std::string& GetAckCodeString()
					{
						GIL::HL7::Segment& msa = *this;
						return msa[1][1];
					}

					bool IsSuccess() {
						return GetAckCode() == AC_Success;
					}

					bool IsRejected() {
						return GetAckCode() == AC_Rejected;
					}

					bool IsErrored() {
						return GetAckCode() == AC_Error;
					}

					bool IsUnknown() {
						return GetAckCode() == AC_Unknown;
					}

				};

				class ERR : public GIL::HL7::Segment {

				public:
					ERR(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("ERR", rs, fs, sfs)
					{
						//GIL::HL7::Segment& msh = *this;
					}

					ERR& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& err = *this;
						err = segment;
						return *this;
					}

					void SetErrorCode(GIL::HL7::Message::ErrorCode errorCode)
					{
						GIL::HL7::Segment& err = *this;
						err[2][1] = GIL::HL7::Message::GetErrorCodeAsString(errorCode);
					}

					GIL::HL7::Message::ErrorCode GetErrorCode()
					{
						GIL::HL7::Segment& err = *this;
						return GIL::HL7::Message::GetErrorCodeFromErrorCodeAsString(err[2][1]);
					}

					const std::string GetErrorCodeDescription()
					{
						return GIL::HL7::Message::GetErrorCodeDescription(GetErrorCode());
					}

					const std::string GetErrorMessage()
					{
						GIL::HL7::Segment& err = *this;
						return err[6][1];
					}

					void SetErrorSeverity(GIL::HL7::Message::ErrorSeverity errorSeverity)
					{
						GIL::HL7::Segment& err = *this;
						err[3][1] = GIL::HL7::Message::GetErrorSeverityString(errorSeverity);
					}

					GIL::HL7::Message::ErrorSeverity GetErrorSeverity()
					{
						GIL::HL7::Segment& err = *this;
						return GIL::HL7::Message::GetErrorSeverityFromString(err[3][1]);
					}
				};

				class PID : public GIL::HL7::Segment {
				public:
					PID(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("PID", rs, fs, sfs)
					{
						GIL::HL7::Segment& pid = *this;
						pid <<= GIL::HL7::Field(1, "PID.1") << GIL::HL7::Component(1, "SI.1");
						pid <<= GIL::HL7::Field(3, "PID.3");
						pid <<= GIL::HL7::Field(4, "PID.4");
						pid <<= GIL::HL7::Field(5, "PID.5");
						pid <<= GIL::HL7::Field(6, "PID.6");

					}

					PID& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& pid = *this;
						pid = segment;
						return *this;
					}

					void SetSetId(const std::string& setId) {
						GIL::HL7::Segment& pid = *this;
						pid[1][1] = setId;
					}

					const std::string& GetSetId() {
						GIL::HL7::Segment& pid = *this;
						return pid[1][1];
					}

					void ClearPatientIdentification()
					{
						GIL::HL7::Segment& pid = *this;
						pid <<= GIL::HL7::Field(3, "PID.3");
					}

					/* Numero de Identificacion Fiscal */
					void SetPatientNIF( const std::string& nif)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", nif);
						pid[3] << GIL::HL7::Component(4, "CX.4", "MI");
						pid[3] << GIL::HL7::Component(5, "CX.5", "NNESP");
						pid[3] << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
					}

					void AddPatientNIF( const std::string& nif) {
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", nif);
						pid_3 << GIL::HL7::Component(4, "CX.4", "MI");
						pid_3 << GIL::HL7::Component(5, "CX.5", "NNESP");
						pid_3 << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
						pid << pid_3;
					}

					/* Numero de Seguridad Social */
					void SetPatientSS(const std::string& numss)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", numss);
						pid[3] << GIL::HL7::Component(4, "CX.4", "SS");
						pid[3] << GIL::HL7::Component(5, "CX.5", "SS");
						pid[3] << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
					}

					/* Numero de Seguridad Social */
					void AddPatientSS(const std::string& numss)
					{
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", numss);
						pid_3 << GIL::HL7::Component(4, "CX.4", "SS");
						pid_3 << GIL::HL7::Component(5, "CX.5", "SS");
						pid_3 << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
						pid << pid_3;
					}

					/* CIP del Servicio Nacional de Salud*/
					void SetPatientCIP(const std::string& cip)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", cip);
						pid[3] << GIL::HL7::Component(4, "CX.4", "MS");
						pid[3] << GIL::HL7::Component(5, "CX.5", "HC");
						pid[3] << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
					}

					/* CIP del Servicio Nacional de Salud*/
					void AddPatientCIP(const std::string& cip)
					{
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", cip);
						pid_3 << GIL::HL7::Component(4, "CX.4", "MS");
						pid_3 << GIL::HL7::Component(5, "CX.5", "HC");
						pid_3 << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
						pid << pid_3;
					}

					/* CIP Hl7Parser */
					void SetPatientHl7ParserCIP(const std::string& cip)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", cip);
						pid[3] << GIL::HL7::Component(4, "CX.4", "CACL");
						pid[3] << GIL::HL7::Component(5, "CX.5", "JHN");
						pid[3] << GIL::HL7::Component(9, "CX.9", "CL&&ISO3166-2");
					}

					/* CIP Hl7Parser */
					void AddPatientHl7ParserCIP(const std::string& cip)
					{
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", cip);
						pid_3 << GIL::HL7::Component(4, "CX.4", "CACL");
						pid_3 << GIL::HL7::Component(5, "CX.5", "JHN");
						pid_3 << GIL::HL7::Component(9, "CX.9", "CL&&ISO3166-2");
						pid << pid_3;
					}

					/* Numero de pasaporte */
					void SetPatientPassportNum(const std::string& num)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", num);
						pid[3] << GIL::HL7::Component(4, "CX.4", "MI");
						pid[3] << GIL::HL7::Component(5, "CX.5", "PPN");
						pid[3] << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
					}

					/* Numero de pasaporte */
					void AddPatientPassportNum(const std::string& num)
					{
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", num);
						pid_3 << GIL::HL7::Component(4, "CX.4", "MI");
						pid_3 << GIL::HL7::Component(5, "CX.5", "PPN");
						pid_3 << GIL::HL7::Component(9, "CX.9", "ESP&&ISO3166");
						pid << pid_3;
					}

					/* Numero de Historia Clinica (Dependiente del centro) */
					void SetPatientNHC(const std::string& nhc, const std::string& idCentro)
					{
						ClearPatientIdentification();
						GIL::HL7::Segment& pid = *this;
						pid[3] << GIL::HL7::Component(1, "CX.1", nhc);
						pid[3] << GIL::HL7::Component(4, "CX.4", "HIS");
						pid[3] << GIL::HL7::Component(5, "CX.5", "PI");
						pid[3] << GIL::HL7::Component(9, "CX.9", idCentro + std::string("&&99CENTROSACYL"));
					}

					/* Numero de Historia Clinica (Dependiente del centro) */
					void AddPatientNHC(const std::string& nhc, const std::string& idCentro)
					{
						GIL::HL7::Segment& pid = *this;
						GIL::HL7::Field pid_3(3, "PID.3");
						pid_3 << GIL::HL7::Component(1, "CX.1", nhc);
						pid_3 << GIL::HL7::Component(4, "CX.4", "HIS");
						pid_3 << GIL::HL7::Component(5, "CX.5", "PI");
						pid_3 << GIL::HL7::Component(9, "CX.9", idCentro + std::string("&&99CENTROSACYL"));
						pid << pid_3;
					}

					void SetPatientName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
					{
						GIL::HL7::Segment& pid = *this;
						pid[5] << GIL::HL7::Component(1, "XPN.1", apellido1);
						pid[5] << GIL::HL7::Component(2, "XPN.2", nombre);
						pid[6] << GIL::HL7::Component(1, "XPN.1", apellido2);
					}
				};

				class PV1 : public GIL::HL7::Segment {
				public:
					PV1(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("PV1", rs, fs, sfs)
					{
						GIL::HL7::Segment& pv1 = *this;
						pv1 <<= GIL::HL7::Field(1, "PV1.1") << GIL::HL7::Component(1, "SI.1");
						pv1 <<= GIL::HL7::Field(19, "PV1.19") << GIL::HL7::Component(1, "CX.1") << GIL::HL7::Component(4, "CX.4") << GIL::HL7::Component(5, "CX.5") << GIL::HL7::Component(9, "CX.9");
					}

					PV1& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& pv1 = *this;
						pv1 = segment;
						return *this;
					}

					void SetSetId(const std::string& id)
					{
						GIL::HL7::Segment& pv1 = *this;
						pv1[1] << GIL::HL7::Component(1, "SI.1", id);
					}

					void SetPatientClass(const std::string& patientClass)
					{
						GIL::HL7::Segment& pv1 = *this;
						pv1[2] << GIL::HL7::Component(1, "IS.1", patientClass);
					}

					void SetEpisodeNumber(const std::string& episodeNumber, const std::string& ambitoPeticion, const std::string& idCentro)
					{
						GIL::HL7::Segment& pv1 = *this;
						if (!episodeNumber.empty()) {
							pv1[19] << GIL::HL7::Component(1, "CX.1", episodeNumber);
						}
						pv1[19] << GIL::HL7::Component(4, "CX.4", ambitoPeticion);
						pv1[19] << GIL::HL7::Component(5, "CX.5", "VN");
						pv1[19] << GIL::HL7::Component(9, "CX.9", idCentro + std::string("&&99CENTROHl7Parser"));
					}

				};

				class ORC : public GIL::HL7::Segment {
				public:
					ORC(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("ORC", rs, fs, sfs)
					{
						GIL::HL7::Segment& orc = *this;
						orc <<= GIL::HL7::Field(1, "ORC.1") << GIL::HL7::Component(1, "ID.1");
						orc <<= GIL::HL7::Field(2, "ORC.2") << GIL::HL7::Component(1, "EI.1");
						orc <<= GIL::HL7::Field(2, "ORC.3") << GIL::HL7::Component(1, "EI.1");
						orc <<= GIL::HL7::Field(4, "ORC.4") << GIL::HL7::Component(1, "EI.1");
						orc <<= GIL::HL7::Field(5, "ORC.5") << GIL::HL7::Component(1, "ID.1");
						orc <<= GIL::HL7::Field(12, "ORC.12") << GIL::HL7::Component(1, "XCN.1") << GIL::HL7::Component(2, "XCN.2") << GIL::HL7::Component(3, "XCN.3") << GIL::HL7::Component(4, "XCN.4") << GIL::HL7::Component(9, "XCN.9") << GIL::HL7::Component(13, "XCN.13") << GIL::HL7::Component(22, "XCN.22");
						orc <<= GIL::HL7::Field(17, "ORC.17") << GIL::HL7::Component(1, "CE.1") << GIL::HL7::Component(2, "CE.2") << GIL::HL7::Component(3, "CE.3");
					}

					ORC& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& orc = *this;
						orc = segment;
						return *this;
					}

					void SetControlOrder(const std::string& controlOrder)
					{
						GIL::HL7::Segment& orc = *this;
						orc[1][1] = controlOrder;
					}

					void SetPlacerOrderNumber(const std::string& placerOrder)
					{
						GIL::HL7::Segment& orc = *this;
						orc[2][1] = placerOrder;
						orc[4][1] = placerOrder;
					}

					void SetFillerOrderNumber(const std::string& fillerOrder)
					{
						GIL::HL7::Segment& orc = *this;
						orc[3][1] = fillerOrder;
					}

					void SetStatus(const std::string& status)
					{
						GIL::HL7::Segment& orc = *this;
						orc[5][1] = status;
					}

					void SetOrderingProviderName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
					{
						GIL::HL7::Segment& orc = *this;
						orc[12][2] = apellido1;
						orc[12][3] = nombre;
						orc[12][4] = apellido2;
					}

					void SetOrderingProviderNIF(const std::string& nif)
					{
						GIL::HL7::Segment& orc = *this;
						orc[12][1] = nif;
						orc[12][9] = "MI";
						orc[12][13] = "NNESP";
						orc[12][22] ="ESP&&ISO3166";
					}

					void SetOrderingProviderCollegNumber(const std::string& num)
					{
						GIL::HL7::Segment& orc = *this;
						orc[12][1] = num;
						orc[12][9] = "MD";
						orc[12][13] = "COM";
						orc[12][22] ="ESP&&ISO3166";
					}

					void SetOrderingFacility(const std::string& id, const std::string& nombre)
					{
						GIL::HL7::Segment& orc = *this;
						orc[21][1] = nombre;
						orc[21][10] = id;
					}

				};

				class OBR : public GIL::HL7::Segment {
				public:
					typedef enum Laterality_Code {
						LAT_R,
						LAT_L
					} ACK_Code;

					OBR(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("OBR", rs, fs, sfs)
					{
						GIL::HL7::Segment& obr = *this;
						obr <<= GIL::HL7::Field(1, "OBR.1") << GIL::HL7::Component(1, "SI.1");
						obr <<= GIL::HL7::Field(2, "OBR.2") << GIL::HL7::Component(1, "ST.1");
						obr <<= GIL::HL7::Field(2, "OBR.3") << GIL::HL7::Component(1, "ST.1");
						obr <<= GIL::HL7::Field(4, "OBR.4") << GIL::HL7::Component(1, "ST.1") << GIL::HL7::Component(2, "ST.1") << GIL::HL7::Component(3, "ID.1");
						obr <<= GIL::HL7::Field(26, "OBR.26") << GIL::HL7::Component(1, "ST.1");
						obr <<= GIL::HL7::Field(29, "OBR.26") << GIL::HL7::Component(1, "ST.1");
					}

					OBR& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& obr = *this;
						obr = segment;
						return *this;
					}

					void SetSetId(const std::string& id)
					{
						GIL::HL7::Segment& obr = *this;
						obr[1][1] = id;
					}

					void SetPlacerOrderNumber(const std::string& placeOrderUID)
					{
						GIL::HL7::Segment& obr = *this;
						obr[2][1] = placeOrderUID;
					}

					void SetStudyUID(const std::string& StudyUID)
					{
						GIL::HL7::Segment& obr = *this;
						obr[2][1] = StudyUID;
					}

					void SetFillerOrderNumber(const std::string& fillerOrder)
					{
						GIL::HL7::Segment& obr = *this;
						obr[3][1] = fillerOrder;
					}

					void SetFillerOrderNumber(const std::string& fillerOrderId, const std::string& fillerOrderNamespaceId)
					{
						GIL::HL7::Segment& obr = *this;
						obr[3][1] = fillerOrderId;
						obr[3][2] = fillerOrderNamespaceId;
					}

					void SetIdentifier(const std::string& identifier)
					{
						GIL::HL7::Segment& obr = *this;
						obr.ParseField(4, identifier);
					}

					void SetIdentifier(const std::string& codPrueba, const std::string& tipoPrueba, const std::string& sistemaCodificacion)
					{
						GIL::HL7::Segment& obr = *this;
						obr[4][1] = codPrueba;
						obr[4][2] = tipoPrueba;
						obr[4][3] = sistemaCodificacion;
					}

					void SetPlacerField1(const std::string& value)
					{
						GIL::HL7::Segment& obr = *this;
						obr[18][1] = value;
					}

					void SetParentResultIdentifier(const std::string& parentObservationIdentifier, const std::string& parentObservationSubIdentifier, const std::string& parentObservationValueDescriptor)
					{
						GIL::HL7::Segment& obr = *this;
						obr[26][1] = parentObservationIdentifier;
						obr[26][2] = parentObservationSubIdentifier;
						obr[26][3] = parentObservationValueDescriptor;
					}

					void SetParentID(const std::string& placerOrderUId, const std::string& fillerOderUId)
					{
						GIL::HL7::Segment& obr = *this;
						obr[29][1] = placerOrderUId;
						obr[29][2] = fillerOderUId;
					}

					void SetLaterality(Laterality_Code laterality)
					{
						GIL::HL7::Segment& obr = *this;
						switch(laterality) {
							case LAT_L:
								{
									obr[46][1] = "L";
									obr[46][2] = "Izquierdo";
									obr[46][3] = "HL70495";
								}
								break;
							case LAT_R:
								{
									obr[46][1] = "R";
									obr[46][2] = "Derecho";
									obr[46][3] = "HL70495";
								}
								break;
						}
					}
				};

				class OBX : public GIL::HL7::Segment {
				public:
					OBX(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("OBX", rs, fs, sfs)
					{
						GIL::HL7::Segment& obx = *this;
						obx <<= GIL::HL7::Field(1, "OBX.1")   << GIL::HL7::Component(1, "SI.1");
						obx <<= GIL::HL7::Field(2, "OBX.2")   << GIL::HL7::Component(1, "ID.1");
						obx <<= GIL::HL7::Field(3, "OBX.3")   << GIL::HL7::Component(1, "ST.1")  << GIL::HL7::Component(2, "ST.1")  << GIL::HL7::Component(3, "ID.1");
						obx <<= GIL::HL7::Field(5, "OBX.5")   << GIL::HL7::Component(1, "") << GIL::HL7::Component(2, "") << GIL::HL7::Component(3, "");
						obx <<= GIL::HL7::Field(11, "OBX.11") << GIL::HL7::Component(1, "ID.1");
						obx <<= GIL::HL7::Field(16, "OBX.16") << GIL::HL7::Component(1, "XCN.1") << GIL::HL7::Component(2, "XCN.2") << GIL::HL7::Component(3, "XCN.3") << GIL::HL7::Component(4, "XCN.4") << GIL::HL7::Component(9, "XCN.9") << GIL::HL7::Component(13, "XCN.13") << GIL::HL7::Component(22, "XCN.22");

					}

					OBX& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& obx = *this;
						obx = segment;
						return *this;
					}

					void SetSetId(const std::string& id)
					{
						GIL::HL7::Segment& obx = *this;
						obx[1][1] = id;
					}

					void SetValueType(const std::string& valueType)
					{
						GIL::HL7::Segment& obx = *this;
						obx[2][1] = valueType;
					}

					void SetIdentifier(const std::string& codigoPrueba, const std::string& descPrueba, const std::string& sistemaCodificacion)
					{
						GIL::HL7::Segment& obx = *this;
						if (codigoPrueba != "") {
							obx[3][1] = codigoPrueba;
						} 
						obx[3][2] = descPrueba;
						if (sistemaCodificacion != "") {
							obx[3][3] = sistemaCodificacion;
						} 
					}

					void SetSubIdentifier(const std::string& value, bool unscape = true)
					{
						GIL::HL7::Segment& obx = *this;
						obx.ParseField(4, value, unscape);
					}

					void SetValue(const std::string& value, bool unscape = true)
					{
						GIL::HL7::Segment& obx = *this;
						obx.ParseField(5, value, unscape);
					}

					void SetWado(const std::string& value)
					{
						GIL::HL7::Segment& obx = *this;
						obx[5][1] = value;
						obx[5][3] = "image";
						obx[5][4] = "dicom";
					}

					void SetValueMetadata(const std::string& idAplicacion, const std::string& dataType, const std::string& dataSubType)
					{
						GIL::HL7::Segment& obx = *this;
						obx[5][2] = idAplicacion;
						obx[5][3] = dataType;
						obx[5][4] = dataSubType;
					}

					void SetObservationResultStatus(const std::string& status)
					{
						GIL::HL7::Segment& obx = *this;
						obx[11][1] = status;
					}

					void SetResponsiveObserverName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
					{
						GIL::HL7::Segment& obx = *this;
						obx[16][2] = apellido1;
						obx[16][3] = nombre;
						obx[16][4] = apellido2;
					}

					void SetResponsiveObserverNIF(const std::string& nif)
					{
						GIL::HL7::Segment& obx = *this;
						obx[16][1]  = nif;
						obx[16][9]  = "MI";
						obx[16][13] = "NNESP";
						obx[16][22] ="ESP&&ISO3166";
					}

					void AddObservationMethod(const std::string& codigo, const std::string& desc, const std::string& namespaceid)
					{
						GIL::HL7::Segment& obx = *this;
						obx << GIL::HL7::Field(17, "OBX.17") << GIL::HL7::Component(1, "ST.1", codigo) << GIL::HL7::Component(2, "ST.2", desc) << GIL::HL7::Component(3, "ST.3", namespaceid);						
					}

					void SetObservationMethod(const std::string& codigo, const std::string& desc, const std::string& namespaceid)
					{
						GIL::HL7::Segment& obx = *this;
						obx[17][1] = codigo;
						obx[17][2] = desc;
						obx[17][3] = namespaceid;
					}
				};

				class TQ1 : public GIL::HL7::Segment {
				public:
					TQ1(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : GIL::HL7::Segment("TQ1", rs, fs, sfs)
					{
						GIL::HL7::Segment& tq1 = *this;
						tq1 <<= GIL::HL7::Field(1, "TQ.1") << GIL::HL7::Component(1, "SI.1");
						tq1 <<= GIL::HL7::Field(9, "TQ.9") << GIL::HL7::Component(1, "ST.1");
					}

					TQ1& operator=(const GIL::HL7::Segment& segment)
					{
						GIL::HL7::Segment& tq1 = *this;
						tq1 = segment;
						return *this;
					}

					void SetSetId(const std::string& id)
					{
						GIL::HL7::Segment& tq1 = *this;
						tq1[1][1] = id;
					}

					void SetUrgency(const std::string& urgency)
					{
						GIL::HL7::Segment& tq1 = *this;
						tq1[9][1] = urgency;
					}
				};

			}

			namespace Messages {

				class MessageInterpreter {

				public:

					MessageInterpreter(const std::string& messageType, const std::string& messageTriggerEvent, const std::string& messageHeader, const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : MSH( messageType, messageTriggerEvent, messageHeader, rs, fs, sfs)
					{
					}

					MessageInterpreter(const GIL::HL7::Message& msg) : MSH(msg.GetSegment("MSH", 1))
					{
					}

					virtual ~MessageInterpreter()
					{
					}

					virtual void GenerateTimeStamp();

					void SetSendingApplication(const std::string& sendingApplication)
					{
						MSH.SetSendingApplication(sendingApplication);
					}

					const std::string& GetSendingApplication()
					{
						return MSH.GetSendingApplication();
					}

					void SetSendingFacility(const std::string& sendingFacility)
					{
						MSH.SetSendingFacility(sendingFacility);
					}

					const std::string& GetSendingFacility()
					{
						return MSH.GetSendingFacility();
					}

					void SetReceivingApplication(const std::string& receivingApplication)
					{
						MSH.SetReceivingApplication(receivingApplication);
					}

					const std::string& GetReceivingApplicacion()
					{
						return MSH.GetReceivingApplication();
					}

					void SetReceivingFacility(const std::string& receivingFacility)
					{
						MSH.SetReceivingFacility(receivingFacility);
					}

					const std::string& GetReceivingFacility()
					{
						return MSH.GetReceivingFacility();
					}

					void SetTimeStamp(const std::string& messageTimeStamp)
					{
						MSH.SetTimeStamp(messageTimeStamp);
					}

					const std::string& GetTimeStamp()
					{
						return MSH.GetTimeStamp();
					}

					void SetMessageType(const std::string& messageType)
					{
						MSH.SetMessageType(messageType);
					}

					const std::string& GetMessageType()
					{
						return MSH.GetMessageType();
					}

					void SetMessageTriggerEvent(const std::string& triggerEvent)
					{
						MSH.SetMessageTriggerEvent(triggerEvent);
					}

					const std::string& GetMessageTriggerEvent()
					{
						return MSH.GetMessageTriggerEvent();
					}

					void SetMessageHeader(const std::string& messageHeader)
					{
						MSH.SetMessageHeader(messageHeader);
					}

					const std::string& GetMessageHeader()
					{
						return MSH.GetMessageHeader();
					}

					void SetMessageControlId(const std::string& messageControlId)
					{
						MSH.SetMessageControlId(messageControlId);
					}

					const std::string& GetMessageControlId()
					{
						return MSH.GetMessageControlId();
					}

					void Build(GIL::HL7::Message& msg)
					{
						GenerateTimeStamp();
						msg.SetHeader(MSH.GetMessageHeader());
						msg["MSH"] = MSH;

						BuildSpecific(msg);
					}

				protected:

					virtual void BuildSpecific(GIL::HL7::Message& msg) = 0;

				public:

					GIL::Hl7Parser::Segmentos::MSH MSH;

				protected:
					std::string messageType;
					std::string messageTriggerEvent;

				};

				class ORU_R01Interpreter : public MessageInterpreter {

				public:

					class PatientResult {

					public:

						class Patient {

						public:

							class PatientIdentification {

							public:

								PatientIdentification()
								{
								}

								void SetSetId(unsigned int id)
								{
									std::stringstream ss;
									ss << id;
									PID.SetSetId(ss.str());
								}

								void SetPatientNIF( const std::string& nif)
								{
									PID.SetPatientNIF(nif);
								}

								void AddPatientNIF( const std::string& nif)
								{
									PID.AddPatientNIF(nif);
								}

								/* Numero de Seguridad Social */
								void SetPatientSS(const std::string& numss)
								{
									PID.SetPatientSS(numss);
								}

								/* Numero de Seguridad Social */
								void AddPatientSS(const std::string& numss)
								{
									PID.AddPatientSS(numss);
								}

								/* CIP del Servicio Nacional de Salud*/
								void SetPatientCIP(const std::string& cip)
								{
									PID.SetPatientCIP(cip);
								}

								/* CIP del Servicio Nacional de Salud*/
								void AddPatientCIP(const std::string& cip)
								{
									PID.AddPatientCIP(cip);
								}

								/* CIP Hl7Parser */
								void SetPatientHl7ParserCIP(const std::string& cip)
								{
									PID.SetPatientHl7ParserCIP(cip);
								}

								/* CIP Hl7Parser */
								void AddPatientHl7ParserCIP(const std::string& cip)
								{
									PID.AddPatientHl7ParserCIP(cip);
								}

								/* Numero de pasaporte */
								void AddPatientPassportNum(const std::string& num)
								{
									PID.AddPatientPassportNum(num);
								}

								/* Numero de pasaporte */
								void SetPatientPassportNum(const std::string& num)
								{
									PID.SetPatientPassportNum(num);
								}

								/* Numero de Historia Clinica (Dependiente del centro) */
								void SetPatientNHC(const std::string& nhc, const std::string& idCentro)
								{
									PID.SetPatientNHC(nhc, idCentro);
								}

								/* Numero de Historia Clinica (Dependiente del centro) */
								void AddPatientNHC(const std::string& nhc, const std::string& idCentro)
								{
									PID.AddPatientNHC(nhc, idCentro);
								}

								void SetPatientName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
								{
									PID.SetPatientName(nombre, apellido1, apellido2);
								}

								GIL::Hl7Parser::Segmentos::PID PID;
							};

							class PatientVisit {

							public:

								PatientVisit()
								{
								}

								void SetEpisodeNumber(const std::string& nep, const std::string& ambitoPeticion, const std::string& idCentro)
								{
									PV1.SetEpisodeNumber(nep, ambitoPeticion, idCentro);
								}

								void SetSetId(unsigned int id)
								{
									std::stringstream ss;
									ss << id;
									PV1.SetSetId(ss.str());
								}

								void SetPatientTypeExternal()
								{
									PV1.SetPatientClass("O");
								}

								void SetPatientTypeAdmitted()
								{
									PV1.SetPatientClass("I");
								}

							public:
								GIL::Hl7Parser::Segmentos::PV1 PV1;

							};

							Patient()
							{
							}

						public:

							void BuildSpecific(GIL::HL7::Message& msg)
							{
								msg << patientIdentification.PID;
								msg << patientVisit.PV1;
							}

							PatientIdentification patientIdentification;
							PatientVisit patientVisit;
						};

						class Order {

						public:
							class OrderCommon {

							public:
								OrderCommon()
								{
								}

								void SetStatusCompleted()
								{
									ORC.SetStatus("CM");
									ORC.SetControlOrder("SC");
								}

								void SetOrderingUID(const std::string& uid)
								{
									ORC.SetPlacerOrderNumber(uid);
								}

								void SetProducerUID(const std::string& producerUID)
								{
									ORC.SetFillerOrderNumber(producerUID);
								}

								void SetProducerUID(const std::string& accessionNumber,const std::string& idPACS)
								{
									std::stringstream ostr;
									ostr << accessionNumber << '^' << idPACS;
									ORC.SetFillerOrderNumber(ostr.str());
								}

								void SetOrderingProviderName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
								{
									ORC.SetOrderingProviderName(nombre, apellido1, apellido2);
								}

								void SetOrderingProviderNIF(const std::string& nif)
								{
									ORC.SetOrderingProviderNIF(nif);
								}

								void SetOrderingCenter(const std::string& id, const std::string& nombre)
								{
									ORC.SetOrderingFacility(id, nombre);
								}

								GIL::Hl7Parser::Segmentos::ORC ORC;
							};

							class TimingQuantity {
							public:
								TimingQuantity()
								{
								}

								void SetSetId(unsigned int id)
								{
									std::stringstream ss;
									ss << id;
									TQ1.SetSetId(ss.str());
								}

								void SetUrgencyNormal()
								{
									TQ1.SetUrgency("R");
								}

								void SetUrgencyUrgent()
								{
									TQ1.SetUrgency("S");
								}

								GIL::Hl7Parser::Segmentos::TQ1 TQ1;
							};

							class ObservationRequest {

							public:
								ObservationRequest()
								{
								}

								void SetSetId(unsigned int id)
								{
									std::stringstream ss;
									ss << id;
									OBR.SetSetId(ss.str());
								}

								void SetOrderingUID(const std::string& orderUId)
								{
									OBR.SetPlacerOrderNumber(orderUId);
								}

								void SetProducerUID(const std::string& producerUID)
								{
									OBR.SetFillerOrderNumber(producerUID);
								}

								void SetProducerUID(const std::string& accessionNumber,const std::string& idPACS)
								{
									std::stringstream ostr;
									ostr << accessionNumber << '^' << idPACS;
									OBR.SetFillerOrderNumber(ostr.str());
								}

								void SetIdentifier(const std::string& id)
								{
									OBR.SetIdentifier(id);
								}

								void SetIdentifier(const std::string& codPrueba, const std::string& tipoPrueba, const std::string& sistemaCodificacion)
								{
									OBR.SetIdentifier(codPrueba, tipoPrueba, sistemaCodificacion);
								}

								//  identificadordObservacionReferida = OBX.3, subIdentificadorObservacionReferida = OBX.4, valorEvidencia = OBX.4
								void SetParentResultIdentifier(const std::string& identificadordObservacionReferida, const std::string& subIdentificadorObservacionReferida, const std::string& valorEvidencia)
								{
									OBR.SetParentResultIdentifier(identificadordObservacionReferida, subIdentificadorObservacionReferida, valorEvidencia);
								}

								void SetParentID(const std::string& placerOrderUId, const std::string& fillerOderUId)
								{
									OBR.SetParentID(placerOrderUId, fillerOderUId);
								}

								void SetStudyUID(const std::string& StudyUID)
								{
									OBR.SetStudyUID(StudyUID);
								}

								void SetPlacerField1(const std::string& value)
								{
									OBR.SetPlacerField1(value);
								}

								void SetLateralityRight()
								{
									OBR.SetLaterality(GIL::Hl7Parser::Segmentos::OBR::LAT_R);
								}

								void SetLateralityLeft()
								{
									OBR.SetLaterality(GIL::Hl7Parser::Segmentos::OBR::LAT_L);
								}

								GIL::Hl7Parser::Segmentos::OBR OBR;
							};

							class ObservationGroup {

							public:

								class Observation {
								public:

									Observation()
									{
									}

									void Clear()
									{
										OBX = GIL::Hl7Parser::Segmentos::OBX();
									}

									void SetSetId(unsigned int id)
									{
										std::stringstream ss;
										ss << id;
										OBX.SetSetId(ss.str());
									}

									void SetDataType(const std::string& dataType)
									{
										OBX.SetValueType(dataType);
									}

									void SetSubIdentifier(unsigned int id)
									{
										std::stringstream ss;
										ss << id;
										OBX.SetSubIdentifier(ss.str());
									}

									void SetIdentifier(const std::string& codigoPrueba, const std::string& descPrueba, const std::string& sistemaCodificacion)
									{
										OBX.SetIdentifier(codigoPrueba, descPrueba, sistemaCodificacion);
									}

									void SetValue(const std::string& value, bool unscape = true)
									{
										OBX.SetValue(value, unscape);
									}

									void SetValueMetadata(const std::string& idAplicacion, const std::string& dataType, const std::string& dataSubType)
									{
										OBX.SetValueMetadata(idAplicacion, dataType, dataSubType);
									}

									void SetWado(const std::string& value)
									{
										OBX.SetWado(value);
									}

									void SetResponsiveObserverName(const std::string& nombre, const std::string& apellido1, const std::string& apellido2)
									{
										OBX.SetResponsiveObserverName(nombre, apellido1, apellido2);
									}

									void SetResponsiveObserverNIF(const std::string& nif)
									{
										OBX.SetResponsiveObserverNIF(nif);
									}

									void SetFinal()
									{
										OBX.SetObservationResultStatus("F");
									}

									void AddObservationMethod(const std::string& codigo, const std::string& desc, const std::string& namespaceid)
									{
										OBX.AddObservationMethod(codigo, desc, namespaceid);
									}

									GIL::Hl7Parser::Segmentos::OBX OBX;
								};

								ObservationGroup()
								{

								}

								void AddObservation(const Observation& ob)
								{
									Observations.push_back(ob);
								}

								std::list<Observation> Observations;
							};

							Order()
							{

							}

						public:
							void BuildSpecific(GIL::HL7::Message& msg)
							{
								msg << orderCommon.ORC;
								msg << observationRequest.OBR;
								msg << timingQuantity.TQ1;

								for (std::list<Order::ObservationGroup::Observation>::iterator it = observationGroup.Observations.begin(); it != observationGroup.Observations.end(); ++it)
								{
									msg << (*it).OBX;
								}

							}

							OrderCommon orderCommon;
							TimingQuantity timingQuantity;
							ObservationRequest observationRequest;
							ObservationGroup observationGroup;

						};

						PatientResult()
						{
						}

					public:

						void BuildSpecific(GIL::HL7::Message& msg)
						{
							patient.BuildSpecific(msg);

							for (std::list<Order>::iterator it = orders.begin(); it != orders.end(); ++it) {
								(*it).BuildSpecific(msg);
							}
						}

						void AddOrder(Order& order)
						{
							orders.push_back(order);
						}

						Patient patient;
						std::list<Order> orders;
					};

					ORU_R01Interpreter(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : MessageInterpreter("ORU", "R01", "ORU_R01", rs, fs, sfs)
					{
					}

				protected:

					virtual void BuildSpecific(GIL::HL7::Message& msg)
					{
						for (std::list<PatientResult>::iterator it = results.begin(); it != results.end(); ++it) {
							(*it).BuildSpecific(msg);
						}

					}

				public:

					void AddPatientResult(PatientResult& result)
					{
						results.push_back(result);
					}

				protected:

						std::list<PatientResult> results;
				};

				class ACKInterpreter : public MessageInterpreter {

				public:

					ACKInterpreter(const std::string& rs = RECORD_SEPARATOR, const std::string& fs = "|", const std::string& sfs = "^") : MessageInterpreter("ACK", "", "ACK", rs, fs,sfs)
					{
						MSH <<= GIL::HL7::Field(15, "MSH.15") << GIL::HL7::Component(1, "", "NE");
						MSH <<= GIL::HL7::Field(16, "MSH.16") << GIL::HL7::Component(1, "", "NE");
					}

					ACKInterpreter(const GIL::HL7::Message& msg) : MessageInterpreter(msg), MSA(), ERR()
					{
						MSA = msg.GetSegment("MSA", 1);
						ERR = msg.GetSegment("ERR", 1);
					}

					// Obtiene el Control ID del mensaje al que se refiere este ACK
					void SetReferralMessageControlId (MessageInterpreter& msg)
					{
						MSA.SetReferralMessageControlId(msg.MSH.GetMessageControlId());
					}

					void SetReferralMessageControlId (GIL::HL7::Message& msg)
					{
						MSA.SetReferralMessageControlId(msg["MSH"][10][1]);
					}

					void SetReferralMessageControlId (const std::string& msgControlId)
					{
						MSA.SetReferralMessageControlId(msgControlId);
					}

					const std::string& GetReferralMessageControlId ()
					{
						return MSA.GetReferralMessageControlId();
					}

					void SetAckCode(GIL::Hl7Parser::Segmentos::MSA::ACK_Code code)
					{
						return MSA.SetAckCode(code);
					}

					GIL::Hl7Parser::Segmentos::MSA::ACK_Code GetAckCode()
					{
						return MSA.GetAckCode();
					}

					bool IsSuccess ()
					{
						return MSA.IsSuccess();
					}

					bool IsRejected ()
					{
						return MSA.IsRejected();
					}

					bool IsErrored ()
					{
						return MSA.IsErrored();
					}

					bool IsUnknown ()
					{
						return MSA.IsUnknown();
					}

					void SetErrorCode(GIL::HL7::Message::ErrorCode errorCode)
					{
						ERR.SetErrorCode(errorCode);
					}

					GIL::HL7::Message::ErrorCode GetErrorCode()
					{
						return ERR.GetErrorCode();
					}

					void SetErrorSeverity(GIL::HL7::Message::ErrorSeverity errorSeverity)
					{
						return ERR.SetErrorSeverity(errorSeverity);
					}

					GIL::HL7::Message::ErrorSeverity GetErrorSeverity()
					{
						return ERR.GetErrorSeverity();
					}

				protected:

					virtual void BuildSpecific(GIL::HL7::Message& msg)
					{
						msg["MSA"] = MSA;
						msg["ERR"] = ERR;
					}

				public:

					GIL::Hl7Parser::Segmentos::MSA MSA;
					GIL::Hl7Parser::Segmentos::ERR ERR;
				};
		}
	}
}
