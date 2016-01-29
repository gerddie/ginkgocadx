/*
 *  
 *  $Id: dicomizecommand.h $
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
#pragma once
#include <wx/string.h>
#include <api/globals.h>
#include <api/api.h>
#include <api/icommand.h>
#include <api/ientorno.h>
#include <api/observers/ieventsobserver.h>
#include <api/imodelointegracion.h>
#include <main/gui/import/importationdata.h>

class wxCriticalSectionLocker;
class wxSemaphore;

namespace GADAPI {
	class EXTAPI DicomizeCommandParams : public GNC::GCS::IComandoParams {
	public:
		DicomizeCommandParams(const GNC::GCS::Ptr<GNC::GUI::ImportationData>& pDatosPersistentes, const std::string& dirTemporal = "") ;

		DicomizeCommandParams( const DicomizeCommandParams& o);
		DicomizeCommandParams& operator = (const DicomizeCommandParams& o);

		virtual ~DicomizeCommandParams() ;

	public:
		GNC::GCS::Ptr<GNC::GUI::ImportationData> m_pImportData;
		std::string m_dirTemporal;
		std::list<std::string> m_listOfFiles;
		std::list<long> m_InsertedSeries;
		bool m_openAfterDicomize;

		std::string m_mensajeError;
		bool m_importacionCorrecta;
		bool m_copiadoCorrecto;
		wxSemaphore* m_pSemaforoEsperarComando;
		
		GNC::GCS::IComando*	m_pComandoEspera;
		GNC::GCS::IComandoParams*	m_pParamsEspera;
	};


	class EXTAPI DicomizeCommand : public GNC::GCS::IComando, public GNC::GCS::IEventsObserver {
	public:
		DicomizeCommand(DicomizeCommandParams* pParams);

		typedef struct {
					wxString campoLista;
					wxString loincCodigoCampo;
					wxString loincDescCampo;
					wxString valor;
		} tValoracion;

		typedef std::list<tValoracion> listaValoraciones;

	 protected:
		virtual void Execute();
		virtual void Update();

		  virtual void OnAbort();

		void LiberarRecursos();
	 public:
		virtual bool ImportarFicherosOriginales();
		
	 protected:
		virtual GIL::ISeriesModel ImportOriginalFiles(bool pdfFiles);

		virtual bool CopiarDicom();

		virtual bool SubirPACS();

		virtual void LanzarYEsperar(GNC::GCS::IComando* pCmd, GNC::GCS::IComandoParams* pCmdParams);

		virtual bool  BorrarArchivosTemporales(wxString dirPath);

		virtual void ProcesarEvento(GNC::GCS::Events::IEvent *evt) ;

		DicomizeCommandParams* m_pIntegracionParams;
	};
}
