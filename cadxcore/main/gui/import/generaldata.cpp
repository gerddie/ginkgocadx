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


#include <string>
#include <sstream>
#include <cmath>

#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/valtext.h>
#include <wx/busyinfo.h>
#include <wx/image.h>
#include <wx/filename.h>
#include <wx/dir.h>
#include <wx/msgdlg.h>
#include <wx/tokenzr.h>

#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladorpermisos.h>

#include <api/ientorno.h>

#include <api/dicom/idicommanager.h>

#include <main/controllers/commandcontroller.h>

#include <commands/comandopacs.h>

#include <export/tagsprivadoscomunes.h>

#include "generaldata.h"

namespace GNC {
	namespace GUI {
		void SetStringField(GIL::DICOM::DicomDataset& tagsDicom, const std::string& tagDICOM, wxTextCtrl* field, bool autodisable = true) 
		{
			std::string value;
			if (tagsDicom.getTag(tagDICOM, value)) {
				field->SetValue(wxString::FromUTF8(value.c_str()));
				if (autodisable) {
					field->Enable(false);
				}
			}
		}

		void SetDateField(GIL::DICOM::DicomDataset& tagsDicom, const std::string& tagDICOM, wxDatePickerCtrl* field, bool autodisable = true) 
		{
			std::string value;
			if (tagsDicom.getTag(tagDICOM, value)) {
				wxDateTime fecha;
				fecha.ParseFormat(wxString::FromUTF8(value.c_str()),wxT("%Y%m%d"));
				if(fecha.IsValid()){
					field->SetValue(fecha);
				}
				if (autodisable) {
					field->Enable(false);
				}
			}
		}

		void SetTimeField(GIL::DICOM::DicomDataset& tagsDicom, const std::string& tagDICOM, wxMaskedTextCtrl* field, bool autodisable = true) 
		{
			std::string value;
			if (tagsDicom.getTag(tagDICOM, value)) {
				wxDateTime fecha;
				fecha.ParseFormat(wxString::FromUTF8(value.c_str()),wxT("%H%M%S"));
				if(fecha.IsValid()){
					field->SetDateTimeValue(fecha);
				}
				if (autodisable) {
					field->Enable(false);
				}
			}
		}

		GeneralData::GeneralData(wxWindow* pParent,IWizard* pWizard,const GNC::GCS::Ptr<ImportationData>& importationData, bool seriesDescription):PanelDatosGeneralesBase(pParent),IPasoWizard(pWizard)
		{
			Hide();
			m_pImportData=importationData;

			wxTextValidator OnlyNum = wxTextValidator(wxFILTER_NUMERIC);
			m_pPacienteAltura->SetValidator(OnlyNum);
			m_pPacientePeso->SetValidator(OnlyNum);

			wxDateTime date = wxDateTime::Now();
			m_pEstudioTime->SetDateTimeValue(date);
			m_pSerieTime->SetDateTimeValue(date);

			if(m_pImportData->m_pIntegrationModel.IsValid()) {
				GIL::DICOM::DicomDataset& tagsDicom = m_pImportData->m_pIntegrationModel->TagsDICOMOverwrite;
				SetStringField(tagsDicom, "0008|1030", m_pEstudioDescripcion, false);
				SetDateField(tagsDicom, "0008|0020", m_pEstudioFecha);
				SetTimeField(tagsDicom, "0008|0030", m_pEstudioTime);
				SetStringField(tagsDicom, "0008|103e", m_pSerieDescripcion);
				SetDateField(tagsDicom, "0008|0021", m_pSerieFecha);
				SetTimeField(tagsDicom, "0008|0031", m_pSerieTime);
				SetDateField(tagsDicom, "0010|0030", m_pPacienteFechaNacimiento);
				std::string tmp;
				if (tagsDicom.getTag("0010|1020", tmp)) { //height
					wxString value = wxString::FromUTF8(tmp.c_str());
					double height;
					if (value.ToDouble(&height)) {
						height *= 100.0;
						wxString wstraltura;
						wstraltura << ((int)height);
						m_pPacienteAltura->SetValue(wstraltura);
						m_pPacienteAltura->Enable(false);
					}
				}
				if (tagsDicom.getTag("0010|0040", tmp)) { //sex
					wxString value = wxString::FromUTF8(tmp.c_str());
					if(value.CompareTo(wxT("M"), wxString::ignoreCase) == 0){
						m_pPacienteSexo->SetSelection(1);
					}else if(value.CompareTo(wxT("F"), wxString::ignoreCase) == 0){
						m_pPacienteSexo->SetSelection(2);
					}else if(value.CompareTo(wxT("O"), wxString::ignoreCase) == 0){
						m_pPacienteSexo->SetSelection(3);
					}else{
						m_pPacienteSexo->SetSelection(0);
					}
					m_pPacienteSexo->Enable(false);
				}
				if (tagsDicom.getTag("0010|0020", tmp)) { // patient id
					SetStringField(tagsDicom, "0010|0020", m_pPacienteId);
				} else {
					m_pPacienteId->SetBackgroundColour(wxColor(255, 213, 213));
					m_pPacienteId->SetFocus();
				}
				//name and surname
				if (tagsDicom.getTag("0010|0010", tmp)) { //name of patient
					wxString value = wxString::FromUTF8(tmp.c_str());
					wxStringTokenizer tokenizer(value, wxT("^"));
					if(tokenizer.HasMoreTokens()) {
						m_pPatientSurname->SetValue(tokenizer.GetNextToken());
					} 
					if(tokenizer.HasMoreTokens()) {
						m_pPacienteNombre->SetValue(tokenizer.GetNextToken());
					} 
					m_pPatientSurname->Enable(false);
					m_pPacienteNombre->Enable(false);
				}

				// Populate Accno
				if (GNC::GCS::IControladorPermisos::Instance()->Get("core.importacion", "input_accno").Activo()) {
					SetStringField(tagsDicom, "0008|0050", m_pAccessionNumber);
					m_pAccessionNumber->Show();
					m_pLabelAccNumber->Show();
				} else {
					m_pAccessionNumber->Hide();
					m_pLabelAccNumber->Hide();
				}
			}
			m_pSeriesPanel->Show(seriesDescription);
			m_panelStudyDetails->Layout();
			Layout();
		}

		GeneralData::~GeneralData()
		{
		}


		//region "Metodos heredados de Ipasowizard"

		bool GeneralData::Attach()
		{
			return true;
		}

		std::string GeneralData::GetTitle(){
			return _Std("Overview data of DICOM files");
		}

		std::string GeneralData::GetSubTitle(){
			return _Std("DICOM data from the files to be generated can be populated");
		}

		bool GeneralData::Siguiente(){
			return true;
		}

		bool GeneralData::Anterior(){
			return false;
		}

		bool GeneralData::Cancelar(){
			return true;
		}

		bool GeneralData::Validar()
		{
			return Validar(false);
		}

		bool GeneralData::Validar(bool silent){
			wxString mensaje = wxEmptyString;

			if (m_pPacienteId->GetValue().IsEmpty()) {
				mensaje += _("\n* The patient ID is required.");
			}

			if(m_pPacienteNombre->GetValue().size() + m_pPatientSurname->GetValue().size() > 61) {
				mensaje += _("\n* Concatenation of patient name and surnames can't be higher than 61.");
			}

			if(!ValidarDouble(std::string(m_pPacienteAltura->GetValue().ToUTF8()),0.0f,400.0f, false, false)){
				mensaje += _("\n* The format of the height in centimeters is not correct. ");
			}

			if ( m_pPacienteFechaNacimiento->GetValue().IsLaterThan(m_pEstudioFecha->GetValue()) ) {
				mensaje += _("\n* The patient's birth date is after the date of study");
			}

			if(!ValidarDouble(std::string(m_pPacientePeso->GetValue().ToUTF8()),0.0f,400.0f, false, false)){
				mensaje += _("\n* The format of the weight is not correct. Valid range is between 0 and 400 Kg.");
			}

			if(mensaje != wxEmptyString){
				if (!silent) {
					wxMessageBox(_("The following errors have occurred when validating fields") + mensaje, _("Error validating fields"), wxICON_ERROR);
				}

				return false;
			} else {
				GetPacienteAltura();
				GetPacienteNombre();
				GetPacienteIdentificador();				
				GetPacienteEdad();
				GetPacienteFechaNacimiento();
				GetPacientePeso();
				GetPacienteSexo();
				GetEstudioDescripcion();
				GetEstudioFecha();
				GetEstudioTime();
				GetSerieDescripcion();
				GetSerieFecha();
				GetSerieTime();
				if (GNC::GCS::IControladorPermisos::Instance()->Get("core.importacion", "input_accno").Activo()) {
					GetAccno();
				}
				return true;
			}
		}

		bool GeneralData::IsSkipped()
		{
			return GNC::GCS::IControladorPermisos::Instance()->ObtenerPermiso("core.importacion", "skip_demographic") && Validar(true);
		}

		void GeneralData::GetPacienteNombre()
		{
			std::ostringstream ostr;
			ostr << m_pPatientSurname->GetValue().ToUTF8();
			ostr << "^" << m_pPacienteNombre->GetValue().ToUTF8();

			m_pImportData->baseImages.tags["0010|0010"] = ostr.str();
		}

		void GeneralData::GetPacienteIdentificador(){
			m_pImportData->baseImages.tags["0010|0020"] = std::string(m_pPacienteId->GetValue().ToUTF8());
		}

		void GeneralData::GetPacienteFechaNacimiento(){
			m_pImportData->baseImages.tags["0010|0030"] =  std::string(m_pPacienteFechaNacimiento->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
		}

		void GeneralData::GetPacienteEdad()
		{
			int edad = CalcularEdad();
			std::ostringstream os;
			os.fill('0');
			os.width(3);
			os << edad << "Y";
			m_pImportData->baseImages.tags["0010|1010"] =  os.str();
		}

		void GeneralData::GetPacientePeso(){
			std::string peso = std::string(m_pPacientePeso->GetValue().ToUTF8());
			if (peso.size() != 0) {
				m_pImportData->baseImages.tags["0010|1030"] =peso;
			}
		}

		void GeneralData::GetPacienteAltura(){
			if (!m_pPacienteAltura->GetValue().IsEmpty()) {
				std::istringstream is (std::string(m_pPacienteAltura->GetValue().ToUTF8()));
				float altura = 0.0f;
				is >> altura;
				altura = std::floor(altura);
				altura /= 100.0f;
				std::ostringstream os;
				os << altura;
				m_pImportData->baseImages.tags["0010|1020"] = os.str();
			}
		}

		void GeneralData::GetPacienteSexo(){
			switch(m_pPacienteSexo->GetSelection()){
				case 1:
					m_pImportData->baseImages.tags["0010|0040"] = "M";
					break;
				case 2:
					m_pImportData->baseImages.tags["0010|0040"] = "F";
					break;
				case 3:
					m_pImportData->baseImages.tags["0010|0040"] = "O";
					break;
				default:
					break;
			}
		}

		void GeneralData::OnKillFocusEstudioTimeControl(wxFocusEvent &){
			if(!m_pEstudioTime->GetDateTimeValue().IsValid()){
				wxDateTime ahora = wxDateTime::Now();
				m_pEstudioTime->SetDateTimeValue(ahora);
			}
		}

		void GeneralData::EnableComponents(bool enable){
			m_pEstudioDescripcion->Enable(enable);
			m_pEstudioFecha->Enable(enable);
			m_pEstudioTime->Enable(enable);
			m_pSerieDescripcion->Enable(enable);
			m_pSerieFecha->Enable(enable);
			m_pSerieTime->Enable(enable);
		}


		void GeneralData::GetEstudioFecha(){
			if(m_pEstudioFecha->GetValue().IsValid()){
				 m_pImportData->baseImages.tags["0008|0020"] = std::string(m_pEstudioFecha->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
			}
		}

		void GeneralData::GetEstudioTime(){
			if(m_pEstudioTime->GetDateTimeValue().IsValid()){
				m_pImportData->baseImages.tags["0008|0030"] = std::string(m_pEstudioTime->GetDateTimeValue().Format(wxT("%H%M%S")).ToUTF8());
			}
		}

		void GeneralData::GetEstudioDescripcion(){
			m_pImportData->baseImages.tags["0008|1030"] = std::string(m_pEstudioDescripcion->GetValue().ToUTF8());
		}

		void GeneralData::OnKillFocusSerieTimeControl(wxFocusEvent &){
			if(!m_pSerieTime->GetDateTimeValue().IsValid()){
				wxDateTime ahora = wxDateTime::Now();
				m_pSerieTime->SetDateTimeValue(ahora);
			}
		}

		void GeneralData::GetSerieFecha(){
			if (m_pSeriesPanel->IsShown()) {
				if(m_pSerieFecha->GetValue().IsValid()){
					m_pImportData->baseImages.tags["0008|0021"] = std::string(m_pSerieFecha->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
				}
			} else {
				if(m_pEstudioFecha->GetValue().IsValid()){
					 m_pImportData->baseImages.tags["0008|0021"] = std::string(m_pEstudioFecha->GetValue().Format(wxT("%Y%m%d")).ToUTF8());
				}
			}
		}

		void GeneralData::GetSerieTime(){
			if (m_pSeriesPanel->IsShown()) {
				if(m_pSerieTime->GetDateTimeValue().IsValid()){
					m_pImportData->baseImages.tags["0008|0031"] = std::string(m_pSerieTime->GetDateTimeValue().Format(wxT("%H%M%S")).ToUTF8());
				}
			} else {
				if(m_pEstudioTime->GetDateTimeValue().IsValid()){
					m_pImportData->baseImages.tags["0008|0031"] = std::string(m_pEstudioTime->GetDateTimeValue().Format(wxT("%H%M%S")).ToUTF8());
				}
			}
		}

		void GeneralData::GetAccno() {
			if (m_pAccessionNumber->IsShown()) {
				m_pImportData->baseImages.tags["0008|0050"] = std::string(m_pAccessionNumber->GetValue().ToUTF8());
			}
		}

		void GeneralData::GetSerieDescripcion(){
			if (m_pSeriesPanel->IsShown()) {
				m_pImportData->baseImages.tags["0008|103e"] =  std::string(m_pSerieDescripcion->GetValue().ToUTF8());
			} else {
				m_pImportData->baseImages.tags["0008|103e"] =  std::string(m_pEstudioDescripcion->GetValue().ToUTF8());
			}
		}

		int GeneralData::CalcularEdad()
		{
			wxDateTime fechaEstudio = m_pEstudioFecha->GetValue();
			wxDateTime fechaNacimiento = m_pPacienteFechaNacimiento->GetValue();
			int edad = 0;
			if( (fechaEstudio.GetMonth() > fechaNacimiento.GetMonth()) ||
				(fechaEstudio.GetMonth() == fechaNacimiento.GetMonth() && fechaEstudio.GetDay() >= fechaNacimiento.GetDay()) ){
					edad = fechaEstudio.GetYear() - fechaNacimiento.GetYear();
			} else {
				edad = fechaEstudio.GetYear() - fechaNacimiento.GetYear() - 1;
			}
			return edad;
		}
	}
}
