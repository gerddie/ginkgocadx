/*
 *
 *  $Id: visualizatorcontroller.cpp $
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
#include <wx/wx.h>
#include <wx/notebook.h>
#include <wx/dir.h>
#include <wx/file.h>
#include <set>
#include "visualizatorcontroller.h"

#include <visualizator/tools/overlaystool.h>
#include <visualizator/tools/ecgruletool.h>

#include <visualizator/dialogos/contenedorherramientas.h>
#include <main/gui/import/generaldata.h>
#include <main/gui/import/importationdata.h>
#include <main/gui/history3/synchronizedialog.h>
#include "../vistas/vista2d.h"
#include "../vistas/waveformview.h"
#include <visualizator/estudios/visualizatorstudy.h>
#include <visualizator/license.h>

#include <api/ientorno.h>
#include <api/ivista.h>
#include <api/dicom/imodelodicom.h>
#include <api/integration/dict.h>
#include <api/controllers/icontroladorcarga.h>
#include <visualizator/dialogos/importacion/selectimagesimportationvisualizator.h>

#include <api/controllers/icontroladorpermisos.h>

#include <api/toolsystem/itoolsregistry.h>

#include <api/controllers/ipacscontroller.h>
#include <api/controllers/icontroladorvistas.h>

#include <api/controllers/icontroladorcarga.h>
#include <main/controllers/controladorcarga.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/historycontroller.h>
#include <main/controllers/hangingprotocolcontroller.h>
#include <commands/pdfextractercommand.h>
#include <commands/comandopacs.h>

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

#include <api/controllers/icontroladorinternacionalizacion.h>
#include <resources/visualizatorresourcesmanager.h>
#include "../export/tagsprivados.h"
#include <api/dicom/dcmuiddictionary.h>
#include <main/controllers/anonusagestats.h>

namespace GNKVisualizator {
	class GNKVisualizatorControllerMode: public GNC::GCS::ModoControlador
	{
	public:
		//GNKVisualizatorControllerMode: defines ways of Dicom viewing of this extension
		//id: unique in this controller
		//listaModalidades: modality supporting list
		//abreSeriesSueltas: true if it opens series, false if it opens studies
		//bitmap: bitmap of this mode (for visualization purposes)
		GNKVisualizatorControllerMode(int id, const std::string& descripcion, TipoListaModalidades listaModalidades, TipoListaUIDsImportacion listaUIDsImportacion, wxBitmap * bitmap = NULL):GNC::GCS::ModoControlador(id, descripcion, listaModalidades, listaUIDsImportacion, bitmap, GNC::GCS::ModoControlador::THSL_ONLY_REGULAR)
		{
		}
		~GNKVisualizatorControllerMode() {}

		//returns true if this mode supports this pair of modality-transfer syntax
		virtual bool SupportsModalityFile(const std::string &modalidad, const std::string& transferSyntax) const{
			for(TipoListaModalidades::const_iterator it = m_listaModalidades.begin(); it!= m_listaModalidades.end(); ++it){
				if((*it) == modalidad && !(transferSyntax == "1.2.840.10008.1.2.4.100" || transferSyntax == "1.2.840.10008.1.2.4.101")){
					return true;
				}
			}
			return false;
		}
	};
}


/* Constructor */
GNKVisualizator::ControladorAtencionPrimaria::ControladorAtencionPrimaria(GNC::GCS::IEntorno* pEntorno) : GNC::GCS::IModuleController(pEntorno, EXT_SID, 0)
{
	//primero de todo registramos nuestro catalogo para la internacionalizacion de las cadenas
	{
		pEntorno->GetControladorInternacionalizacion()->AddCatalog("visualizator");
	}

	//registramos los permisos
	m_pEntorno->GetControladorPermisos()->AddDefinicionPermiso("atencionprimaria",_Std("Ginkgo CADx viewer"), "dicomizador",_Std("Simple importation of images"),false, "", true);
	m_pEntorno->GetControladorPermisos()->AddDefinicionPermiso("atencionprimaria",_Std("Ginkgo CADx viewer"),"vista2d",_Std("Medical viewer"), false, "", true);
	m_pEntorno->GetControladorPermisos()->AddDefinicionPermiso("atencionprimaria",_Std("Ginkgo CADx viewer"),"waveform",_Std("Waveform viewer"), false, "", true);

	m_pEntorno = pEntorno;	
	m_ImporterDescription = _Std("Default importer");
	
	// Codigos LOINC de los informes de A. Primaria.
	m_ListaInformes.push_back(GNC::GCS::IdHL7("TUID" + m_UID, "0", _Std("Ginkgo CADx Image Report"), "LN", "", "","v1.0"));

	//registrar herramientas
	GNC::GCS::IToolsRegistry::Instance()->RegisterTool(new GNKVisualizator::OverlaysTool());
	GNC::GCS::IToolsRegistry::Instance()->RegisterTool(new GNKVisualizator::ECGRuleTool());
}

GNKVisualizator::ControladorAtencionPrimaria::~ControladorAtencionPrimaria()
{
	for (ListaModos::iterator it = m_ListaModos.begin(); it != m_ListaModos.end(); ++it) {
		delete (*it);
	}
	m_ListaModos.clear();

	#if defined(_GINKGO_DEBUG) && defined(_WINDOWS)
	//cairo_debug_reset_static_data();
	//_CrtDumpMemoryLeaks();
	#endif
}

void GNKVisualizator::ControladorAtencionPrimaria::RegistrarVistas()
{
	int idModo = 0;
	GNC::GCS::ModoControlador::TipoListaModalidades listaModalidades;
	//TODO meter las modalidades que soporta...
	listaModalidades.push_back(std::string("MR")); //resonancia
	listaModalidades.push_back(std::string("CR")); //computed radiography
	listaModalidades.push_back(std::string("US")); //ultrasonido
	listaModalidades.push_back(std::string("BI")); //biomagnetic image
	listaModalidades.push_back(std::string("CT")); //computed tomography
	listaModalidades.push_back(std::string("XA")); //XRAY angiografia
	listaModalidades.push_back(std::string("SC")); //secondary capture
	listaModalidades.push_back(std::string("NM")); //nuclear medicine
	listaModalidades.push_back(std::string("CD")); //Color flow doppler
	listaModalidades.push_back(std::string("DD")); //Duplex Doppler
	listaModalidades.push_back(std::string("DG")); //Diaphanography
	listaModalidades.push_back(std::string("ES")); //Endoscopy
	listaModalidades.push_back(std::string("LS")); //Laser surface scan
	listaModalidades.push_back(std::string("PT")); //Positron emission tomography (PET)
	listaModalidades.push_back(std::string("RG")); //Radiographic imaging
	listaModalidades.push_back(std::string("ST")); //Single-photon emission computed tomography (SPECT)
	listaModalidades.push_back(std::string("MG")); //Mammography
	listaModalidades.push_back(std::string("IO")); //Intra-Oral
	listaModalidades.push_back(std::string("TG")); //Thermography
	listaModalidades.push_back(std::string("RF")); //Radio Fluoroscopy
	listaModalidades.push_back(std::string("RTIMAGE")); //Radiotherapy Image
	listaModalidades.push_back(std::string("RTDOSE")); //Radiotherapy Dose
	listaModalidades.push_back(std::string("RTSTRUCT")); //Radiotherapy Structure Set
	listaModalidades.push_back(std::string("RTPLAN")); //Radiotherapy Plan
	listaModalidades.push_back(std::string("RTRECORD")); //RT Treatment Record
	listaModalidades.push_back(std::string("HC")); //Hard Copy
	listaModalidades.push_back(std::string("DX")); //Digital Radiography
	listaModalidades.push_back(std::string("PX")); //Panoramic X-Ray
	listaModalidades.push_back(std::string("GM")); //General Microscopy
	listaModalidades.push_back(std::string("SM")); //Slide Microscopy
	listaModalidades.push_back(std::string("XC")); //External-camera Photography
	listaModalidades.push_back(std::string("DOC")); //DOCUMENT (PDF, WORD...)
	listaModalidades.push_back(std::string("PR")); //Presentation State
	//listaModalidades.push_back(std::string("SR")); //SR Document
	listaModalidades.push_back(std::string("IVUS")); //Intravascular Ultrasound
	listaModalidades.push_back(std::string("OP")); //Stereometric Relationship
	listaModalidades.push_back(std::string("OT")); //Other
	listaModalidades.push_back(std::string("SMR")); //Intra-oral Radiography
	listaModalidades.push_back(std::string("OCT")); //Optical Coherence Tomography
	listaModalidades.push_back(std::string("OT"));  //Other
	listaModalidades.push_back(std::string("OPR")); //Ophthalmic Refraction
	listaModalidades.push_back(std::string("OPV")); //Ophthalmic Visual Field
	listaModalidades.push_back(std::string("OPM")); //Ophthalmic Mapping
	listaModalidades.push_back(std::string("KO"));  //Key Object Selection
	listaModalidades.push_back(std::string("SEG")); //Segmentation
	listaModalidades.push_back(std::string("REG")); //Registration
	listaModalidades.push_back(std::string("OPT")); //Ophthalmic Tomography
	listaModalidades.push_back(std::string("BDUS"));//Bone Densitometry (ultrasound)
	listaModalidades.push_back(std::string("BMD")); //Bone Densitometry (X-Ray)

	GNC::GCS::ModoControlador::TipoListaUIDsImportacion listaUIDsImportacion;
	listaUIDsImportacion.push_back(EXT_IMPORTER_SID);

	if(m_pEntorno->GetControladorPermisos()->Get("atencionprimaria", "vista2d")) {
		m_ListaModos.push_back(new GNKVisualizatorControllerMode(idModo++, _Std("Medical Image Viewer"), listaModalidades, listaUIDsImportacion));
	}

	listaModalidades.clear();
	listaModalidades.push_back(std::string("ECG")); //Electrocardiography
	listaModalidades.push_back(std::string("EC")); //Electrocardiography
	listaModalidades.push_back(std::string("HD")); //Hemodynamic Waveform

	listaUIDsImportacion.clear();
	if(m_pEntorno->GetControladorPermisos()->Get("atencionprimaria", "waveform")) {
		m_ListaModos.push_back(new GNC::GCS::ModoControlador(idModo++, _Std("Waveform viewer"), listaModalidades, listaUIDsImportacion));
	}
}


bool GNKVisualizator::ControladorAtencionPrimaria::SupportsImportationUID(const std::string &importationUID)
{
	return (importationUID == EXT_IMPORTER_SID);
}

//helper opening...
GNC::GCS::IVista* OpenMedicalImageViewer(const std::set<long>& seriesPk, wxWindow* pParent, GNC::GCS::IEntorno* pEntorno, GNC::GCS::IModuleController* pModule, const GNC::GCS::Ptr<GNC::GCS::Layout>& hangingLayout, const GNC::GCS::Ptr<GNC::GCS::HangingProtocol>& hangingProtocol)
{
	LOG_STAT(ANON_STAT_VIEW_OS)
	wxWindowDisabler disabler;
	GNC::GCS::Ptr<GNKVisualizator::VisualizatorStudy> estudio = new GNKVisualizator::VisualizatorStudy();
	estudio->OpenedSeries.insert(estudio->OpenedSeries.begin(), seriesPk.begin(), seriesPk.end());
	estudio->Module = pModule;
	estudio->hangingLayout = hangingLayout;
	estudio->hangingProtocol = hangingProtocol;
	estudio->ParentWindow = pParent;
	estudio->InitializeContext(*seriesPk.begin());
	GNC::GCS::IVista* pVista = new GNKVisualizator::Vista2D(estudio);
	pEntorno->GetControladorVistas()->Registrar(pVista);
	pEntorno->GetControladorCarga()->CargaAsincrona(pVista);
	return pVista;
}

/* Abre un estudio de manera no interactiva desde un directorio y devuelve su controlador específico */
GNC::GCS::IVista* GNKVisualizator::ControladorAtencionPrimaria::OpenView(int modo, GNC::GCS::IHistoryController::SeriesModelList& series, GNC::GCS::Ptr<GNC::GCS::HangingProtocol> hangingProtocol)
{
	GNC::GCS::IVista* pVista = NULL;

	if (modo < 0 || modo >= (int) m_ListaModos.size()) {
		//TODO: Excepción
		return NULL;
	}

	//si no se soporta alguna modalidad pues se ignora
	std::set<long> pkSeries;
	bool wadoLinked = false;
	{
		for(GNC::GCS::IHistoryController::SeriesModelList::iterator it = series.begin(); it!= series.end(); ++it){
			for (GNC::GCS::IHistoryController::StringList::iterator itTsyntax = (*it).tsuids.begin(); itTsyntax != (*it).tsuids.end(); ++itTsyntax) {
				if(m_ListaModos[modo]->SupportsModalityFile((*it).series_modality, (*itTsyntax)))
				{
					pkSeries.insert((*it).pk);
					if ((*it).location == GNC::GCS::IHistoryController::TL_WadoLinked) {
						wadoLinked = true;
					}
				}
			}
		}
	}

	if (wadoLinked) {
		//show dialog then download and open again
		std::list<long> listOfPk;
		listOfPk.insert(listOfPk.begin(), pkSeries.begin(), pkSeries.end());
		GNC::GUI::SynchronizeDialog dlg(m_pEntorno->GetVentanaRaiz(), listOfPk, this, modo, hangingProtocol);
		dlg.ShowModal();
		return NULL;
	}
	
	//first of all open pdf...
	std::set<long> tmpPk = pkSeries;
	pkSeries.clear();
	for(std::set<long>::iterator it = tmpPk.begin(); it!= tmpPk.end(); ++it)
	{
		GNC::GCS::IHistoryController::LightFileModelList fileModels;
		GNC::GCS::HistoryController::Instance()->GetSeriesSortedFileModels((*it), fileModels);
		if (fileModels.size() == 0)
		{
			return NULL;
		}

		//if it's pdf...
		std::list<std::string> paths;
		bool isPdf = false;
		for (GNC::GCS::IHistoryController::LightFileModelList::iterator itFile = fileModels.begin(); itFile != fileModels.end(); ++itFile) 
		{
			paths.push_back((*itFile).real_path);
			if ((*itFile).sopcuid == GKUID_EncapsulatedPDFStorage) {
				isPdf = true;
			}
		}
		if (isPdf) {
			GADAPI::PDFExtracterCommandParameters* pParams = new GADAPI::PDFExtracterCommandParameters(paths);
			GADAPI::PDFExtracterCommand* pCmd = new GADAPI::PDFExtracterCommand(pParams);
			m_pEntorno->GetCommandController()->ProcessAsync("Extracting...", pCmd, NULL);
			continue;
		} else {
			pkSeries.insert((*it));
		}
	}

	GNC::GCS::IControladorVistas* pCtrlVistas = m_pEntorno->GetControladorVistas();
	wxWindow* pVentanaPadre = NULL;

	if (m_ListaModos[modo]->GetDescripcion() == _Std("Medical Image Viewer") && !pkSeries.empty()) 
	{
		{//comienza la deshabilitacion
			pVentanaPadre = pCtrlVistas->GetRootWindow();
			try{
				if (hangingProtocol.IsValid() && !hangingProtocol->getLayouts().empty())  {
					for (GNC::GCS::HangingProtocol::TListOfLayouts::const_iterator it = hangingProtocol->getLayouts().begin(); it != hangingProtocol->getLayouts().end(); ++it)
					{
						pVista = OpenMedicalImageViewer(pkSeries, pVentanaPadre, m_pEntorno, this, (*it), hangingProtocol);
					}
				} else {
					pVista = OpenMedicalImageViewer(pkSeries, pVentanaPadre, m_pEntorno, this, NULL, NULL);
				}
				
			}
			catch(GNC::GCS::VistaException &ex){
				if (pVista != NULL) {
					pCtrlVistas->Destruir(pVista);
				}
				wxString mensaje = wxString::FromUTF8(ex.GetCause().c_str());
				wxMessageDialog dialogo( m_pEntorno->GetVentanaRaiz(), mensaje, _("Error opening study"),  wxOK | wxICON_INFORMATION);
				dialogo.ShowModal();
				return NULL;
			}
			catch (...) {
				if (pVista != NULL) {
					pCtrlVistas->Destruir(pVista);
				}
				wxMessageDialog dialogo( m_pEntorno->GetVentanaRaiz(), _("Error opening study"), _("Error opening study"),  wxOK | wxICON_INFORMATION);
				dialogo.ShowModal();
				return NULL;
			}
		}//fin deshabilitacion
	}
	else if (m_ListaModos[modo]->GetDescripcion() == _Std("Waveform viewer") && !pkSeries.empty()) 
	{
		for(std::set<long>::iterator it = pkSeries.begin(); it!= pkSeries.end(); ++it)
		{
			{//comienza la deshabilitacion
				pVentanaPadre = pCtrlVistas->GetRootWindow();
				try{	
					LOG_STAT(ANON_STAT_VIEW_ECG)
					wxWindowDisabler disabler;
					GNC::GCS::Ptr<GNKVisualizator::ECGStudy> estudio = new GNKVisualizator::ECGStudy();
					estudio->OpenedSeries.push_back((*it));
					estudio->Module = this;
					estudio->ParentWindow = pVentanaPadre;
					estudio->InitializeContext(*pkSeries.begin());
					pVista = new GNKVisualizator::WaveFormView(estudio);
					pCtrlVistas->Registrar(pVista);				
				}
				catch(GNC::GCS::VistaException &ex){
					if (pVista != NULL) {
						pCtrlVistas->Destruir(pVista);
					}
					wxString mensaje = wxString::FromUTF8(ex.GetCause().c_str());
					wxMessageDialog dialogo( m_pEntorno->GetVentanaRaiz(), mensaje, _("Error opening study"),  wxOK | wxICON_INFORMATION);
					dialogo.ShowModal();
					return NULL;
				}
				catch (...) {
					if (pVista != NULL) {
						pCtrlVistas->Destruir(pVista);
					}
					wxMessageDialog dialogo( m_pEntorno->GetVentanaRaiz(), _("Error opening study"), _("Error opening study"),  wxOK | wxICON_INFORMATION);
					dialogo.ShowModal();
					return NULL;
				}
			}//fin deshabilitacion

			if (pVista == NULL ) {
				pCtrlVistas->Destruir(pVista);
				return NULL;
			}
			else {
				m_pEntorno->GetControladorCarga()->CargaAsincrona(pVista);
			}		
		}
	}
				

	return pVista;
}

bool GNKVisualizator::ControladorAtencionPrimaria::SoportaImportacion() const
{
	return m_pEntorno->GetControladorPermisos()->Get("atencionprimaria", "dicomizador");
}

void GNKVisualizator::ControladorAtencionPrimaria::GetPasosImportacion(IWizard* pWizard,std::list<IPasoWizard*> &listaPasos, wxWindow* pParent, std::string &dirTemporal, GNC::GCS::Ptr<GIL::IModeloIntegracion> pModeloIntegracion)
{
	GNC::GCS::Ptr<GNC::GUI::ImportationData> datosPersistentes(new GNC::GUI::ImportationData(pModeloIntegracion));

	IPasoWizard* paso=new GNC::GUI::GeneralData(pParent,pWizard,datosPersistentes, true);
	listaPasos.push_back(paso);

	paso = new GNKVisualizator::GUI::SelectImagesImportationVisualizator(pParent,pWizard,datosPersistentes,dirTemporal);
	listaPasos.push_back(paso);				
}
