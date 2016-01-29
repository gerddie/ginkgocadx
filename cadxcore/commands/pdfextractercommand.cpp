/*
 *
 *  $Id: PDFExtracterCommandParameters.cpp $
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
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/mimetype.h>
#include <wx/msgdlg.h>

#include <main/controllers/dicommanager.h>
#include <main/entorno.h>

#include "pdfextractercommand.h"

#define IDC_EXTRACTER          5002

// Singleton de persistencia
namespace GADAPI {

	PDFExtracterCommandParameters::PDFExtracterCommandParameters(const std::list<std::string>& rutas, const std::list<std::string>& dests, bool forOpening) 
	{			
		m_pathsOriginal.insert(m_pathsOriginal.begin(), rutas.begin(), rutas.end());
		m_pathsExtracted.insert(m_pathsExtracted.begin(), dests.begin(), dests.end());
		m_error = false;
		m_ForOpening = forOpening;
	}

	PDFExtracterCommandParameters::~PDFExtracterCommandParameters() {
	}


	PDFExtracterCommand::PDFExtracterCommand(PDFExtracterCommandParameters* pParams) : IComando(pParams)
	{
		m_pExtracterParameters = pParams;
		SetId(IDC_EXTRACTER);
		EsperaA(IDC_EXTRACTER);
	}

	void PDFExtracterCommand::Execute()
	{
			
		wxString wxPathDirectorioTemporal;

		if (m_pExtracterParameters->m_pathsExtracted.empty()) {
			wxPathDirectorioTemporal = FROMPATH(GNC::GCS::IEntorno::Instance()->CreateGinkgoTempDir());
		}

		class ProxyNotificador: public GNC::IProxyNotificadorProgreso 
		{
		public:
			ProxyNotificador(PDFExtracterCommand* pCmd) {
				m_pCmd = pCmd;
			}

			virtual bool NotificarProgreso(float progresoNormalizado, const std::string& texto) 
			{
				return m_pCmd->NotificarProgreso(progresoNormalizado, texto);					
			}
			PDFExtracterCommand* m_pCmd;
		};

		ProxyNotificador proxy(this);		
		bool calculatePaths = m_pExtracterParameters->m_pathsExtracted.empty();

		int i = 0;
		for (std::vector<std::string>::iterator itRutas = m_pExtracterParameters->m_pathsOriginal.begin(); itRutas != m_pExtracterParameters->m_pathsOriginal.end(); ++itRutas) 
		{

			std::string stdFilePath;
			if (calculatePaths) {
				wxString fpath = wxPathDirectorioTemporal + wxFileName::GetPathSeparator() + wxString::Format(wxT("%d.pdf"), (int)(i) );
				stdFilePath = std::string( TOPATH(fpath) );
			}
			else {
				stdFilePath = m_pExtracterParameters->m_pathsExtracted.at(i);
			}
			//extract pdf data in temp directory...
			GIL::DICOM::DICOMManager manager;
			manager.CargarFichero((*itRutas));
			//encapsulated pdf...
			if (manager.ExtractTagToFile(0x0042, 0x0011, stdFilePath, &proxy)) {
				m_pExtracterParameters->m_pathsExtracted.push_back(stdFilePath);
			} else {
				m_pExtracterParameters->m_error = true;
			}
			i++;
		}
	}

	void PDFExtracterCommand::Update()
	{
		if (m_pExtracterParameters->m_error) {
			wxMessageBox(_("Error extracting pdf, Ginkgo CADx is unable to load this format"), _("Error"), wxOK);
		}
		if (m_pExtracterParameters->m_pathsExtracted.empty()) {
			return;
		}

		if (m_pExtracterParameters->m_ForOpening) {

			wxFileType* fileType = wxTheMimeTypesManager->GetFileTypeFromMimeType(wxT("application/pdf"));
			if (fileType == NULL) {
				wxMessageBox(_("There isn't any pdf reader registered in the system"), _("Info"), wxICON_WARNING|wxOK);
				return;
			}

			for (std::vector<std::string>::iterator itRutas = m_pExtracterParameters->m_pathsExtracted.begin(); itRutas != m_pExtracterParameters->m_pathsExtracted.end(); ++itRutas) {
				wxString openCmd = fileType->GetOpenCommand(FROMPATH((*itRutas)));
				if (!openCmd.IsEmpty()) {
					wxExecute(openCmd);
				} else {
					wxMessageBox(_("There isn't any pdf reader registered in the system"), _("Info"), wxICON_WARNING|wxOK);
					break;
				}
			}
			delete fileType;
	//		wxMessageBox(_("PDF files will be opened with system's PDF reader..."), _("Info"), wxICON_INFORMATION | wxOK);
		}
		else {
			wxMessageBox(_("PDF files extracted successfully..."), _("Info"), wxICON_INFORMATION | wxOK);
		}
	}

	void PDFExtracterCommand::OnAbort()
	{
	}

	void PDFExtracterCommand::LiberarRecursos()
	{
	}
}
