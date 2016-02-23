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

#include <wx/wx.h>
#include <wx/checkbox.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/busyinfo.h>
#include <wx/msgdlg.h>
#include <wx/xml/xml.h>
#include <wx/utils.h>

#include <api/globals.h>
#include <api/toolsystem/itoolsregistry.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/dicom/imodelodicom.h>
#include <api/dicom/idicomizador.h>
#include <api/dicom/dcmdictionary.h>
#include <api/controllers/icontroladorpermisos.h>
#include <eventos/modificacionimagen.h>

#include <api/imodelointegracion.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorpermisos.h>
#include "../eventos/visualizatorevents.h"
#include "../estudios/visualizatorstudy.h"

#include <main/entorno.h>

#include "vista2d.h"
#include "../wxvtk/gvistacompleja.h"
#include "../wxvtk/gvistasimple.h"
#include "../dialogos/contenedorherramientas.h"


#define TAG_WIDGETS 0xb;

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkDataSetAttributes.h>
#include <vtkAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInformation.h>

#include <api/toolsystem/itoolcontroller.h>

/* Constructor */

GNKVisualizator::Vista2D::Vista2D(const GNC::GCS::Ptr<GNKVisualizator::VisualizatorStudy>& estudio) : GNC::GCS::IVista((GNC::GCS::Ptr<GNC::GCS::IStudyContext>)estudio), VisualizatorStudy(estudio)
{
	m_Cargada = false;
	m_IgnorarModificaciones = false;
	
	VisualizatorStudy->Window = GVista = new GVistaCompleja(this);
}



GNKVisualizator::Vista2D::~Vista2D()
{
//	GNC::GCS::IEntorno::Instance()->GetControladorCarga()->FreeLoader(&m_pLoader);
	m_IgnorarModificaciones = true;
	DetenerPipeline();
}

//----------------------------------------------------------------------------------------------------
//region Interfaz de Carga


// Comienzo de carga. Metodo sincrono con la interfaz.
void GNKVisualizator::Vista2D::OnCargaIniciada()
{
}

// Paso 1: Cargar los ficheros en memoria. Metodo NO sincrono con la interfaz.
void GNKVisualizator::Vista2D::CargarEstudio(GNC::GCS::IComando* /*pCmdInvocador*/)
{
	
}

// Finalizacion de la carga. Metodo sincrono con la interfaz.
void GNKVisualizator::Vista2D::OnCargaFinalizada()
{
	GVista->OnCargaFinalizada();
	GetToolController()->InitializeToolController();
	GVista->SetFocus();
}

// Paso 2: Inicializacion del pipeline. Metodo sincrono con la interfaz.
void GNKVisualizator::Vista2D::IniciarPipeline()
{
	try {
		GVista->IniciarPipeline(VisualizatorStudy->hangingLayout);

		std::string modality("");
		GetEstudio()->GetTagActiveImage(GKDCM_Modality, modality);
		GetToolController()->LoadStatus("viewer.view2d", modality); // XXX

	}
	catch (const std::bad_alloc&) {
		DetenerPipeline();
		m_Cargada = false;
		throw GNC::GCS::VistaException(_Std("Error: System out of memory. Close some studies to free memory."));
	}
	catch (GNC::GCS::VistaException&) {
		DetenerPipeline();
		m_Cargada = false;
		throw;
	}

	m_Cargada = true;
	GenerarTitulo();
}

// Parada del pipeline. Metodo sincrono con la interfaz Se invoca en el caso de que ocurra un error de carga.
void GNKVisualizator::Vista2D::DetenerPipeline()
{
	// FIXME: Not called ¿?
	std::string modality("");
	GetEstudio()->GetTagActiveImage(GKDCM_Modality, modality);
	GetToolController()->SaveStatus("viewer.view2d", modality); 
	GVista->DetenerPipeline();
}

//endregion


//region Interfaz IVista
bool GNKVisualizator::Vista2D::SoportaHerramienta(GNC::GCS::ITool* /*h*/)
{
	return GVista->GetManipulacionEnabled();
}
//endregion



bool GNKVisualizator::Vista2D::SoportaGuardar()
{
	std::string namespacePermisos("atencionprimaria.vista2d");
	if(GNC::GCS::IEntorno::Instance()->GetControladorPermisos()->Get(namespacePermisos,"guardar cambios")) {
		return true;
	} else {
		return false;
	}
}

bool GNKVisualizator::Vista2D::SoportaExportar()
{
	return true;
}

std::string GNKVisualizator::Vista2D::GetDICOMTagOriginal(std::string tagId)
{
	std::string str("");
	VisualizatorStudy->GetTagActiveImage(tagId, str);
	return str;
}

std::string GNKVisualizator::Vista2D::GetDICOMTagDiagnostico(std::string tagId)
{
	std::string str("");
	VisualizatorStudy->GetTagActiveImage(tagId, str);
	return str;
}

void GNKVisualizator::Vista2D::Activar()
{
	GNC::GCS::IVista::Activar();
//#if defined(_WINDOWS)
	GVista->SetFocus();
//#endif
}

void GNKVisualizator::Vista2D::Desactivar(){
	GNC::GCS::IVista::Desactivar();
}

wxWindow* GNKVisualizator::Vista2D::GetWindow()
{
	return GVista;
}


void GNKVisualizator::Vista2D::ActualizarImagen()
{
	GVista->ActualizarImagen();
}

/* Notificacion de cambio en la configuracion */
void GNKVisualizator::Vista2D::OnConfiguracionCambiada() {
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(this,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
}

void GNKVisualizator::Vista2D::ActivarRuta(long file_pk)
{
	GNC::GCS::IHistoryController::FileModel fileModel = GNC::GCS::IHistoryController::Instance()->GetFileModel(file_pk);
	int indice = VisualizatorStudy->GetPathIndex(fileModel.real_path);
	if (indice >= 0) {
		GVista->GoToSlice(indice, false, true, true);
	}
}

void GNKVisualizator::Vista2D::ComienzaDestruccion()
{	
	m_IgnorarModificaciones=true;
}

void ActivarRuta(const std::string & /*path*/)
{
}

//void GNKVisualizator::Vista2D::GetImageActual(ImageType::Pointer& img, MapaMapasValoracion& mapasValoracion, bool conWidgets, const GNC::GCS::Vector& size)
//{
//	GetImage(img, GVista->m_posicion, mapasValoracion, conWidgets, size);
//}
//
//void GNKVisualizator::Vista2D::GetImage(ImageType::Pointer& img, int index, MapaMapasValoracion& mapasValoracion, bool conWidgets, const GNC::GCS::Vector& size)
//{
//	//como no tiene mapas no se redirige
//	//TODO EXPORT IMAGES
//	/////////////GVista->GetImage(img, index, conWidgets, size);
//
//}
//
//GNKVisualizator::Vista2D::MapaMapasValoracion GNKVisualizator::Vista2D::GetMapasValoracion()
//{
//	MapaMapasValoracion mapas;
//	return mapas;
//}

void GNKVisualizator::Vista2D::GenerarTitulo()
{
	std::ostringstream ostr;
	std::string valor;

	VisualizatorStudy->GetTagActiveImage("0010|0010",valor);
	for (std::string::iterator it = valor.begin(); it != valor.end(); ++it) {
		if ( *it == '^') {
			*it = ' ';
		}
	}
	ostr << valor.c_str() << ", ";
	VisualizatorStudy->GetTagActiveImage("0008|1030",valor);
	ostr << valor.c_str();
	if (!valor.empty()) {
		ostr << "/";
	}
	VisualizatorStudy->GetTagActiveImage("0008|103e",valor);
	ostr << valor.c_str();
	m_Titulo = ostr.str();
}

GNC::GCS::IVista* GNKVisualizator::Vista2D::GetView()
{
	return this;
}

