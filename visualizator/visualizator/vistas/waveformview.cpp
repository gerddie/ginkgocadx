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

#include <list>

#include <wx/wx.h>
#include <wx/checkbox.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/busyinfo.h>
#include <wx/msgdlg.h>
#include <wx/xml/xml.h>
#include <wx/utils.h>

#include <api/globals.h>
#include <api/math/geometry3d.h>
#include <api/toolsystem/itoolsregistry.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/controllers/icontroladorcarga.h>
#include <api/dicom/imodelodicom.h>
#include <api/dicom/idicomizador.h>
#include <api/controllers/icontroladorpermisos.h>
#include <eventos/modificacionimagen.h>
#include <eventos/modificacionfichero.h>

#include <api/imodelointegracion.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorpermisos.h>
#include "../eventos/visualizatorevents.h"
#include "../estudios/visualizatorstudy.h"

#include <visualizator/widgets/wecg.h>

#include "waveformview.h"
#include "../wxvtk/gwaveformview.h"
#include "../dialogos/contenedorherramientas.h"
#include <main/entorno.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
#include <vtkDataSetAttributes.h>
#include <vtkAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageData.h>
#include <vtkAlgorithmOutput.h>
#include <vtkInformation.h>
#include <vtkCamera.h>

/* Constructor */

GNKVisualizator::WaveFormView::WaveFormView(const GNC::GCS::Ptr<GNKVisualizator::ECGStudy>& estudio) : GNC::GCS::IVista((GNC::GCS::Ptr<GNC::GCS::IStudyContext>)estudio), VisualizatorStudy(estudio)
{
	m_Cargada = false;
	
	VisualizatorStudy->Window = GVista = new GNKVisualizator::GUI::GWaveformView(this);
}


GNKVisualizator::WaveFormView::~WaveFormView()
{
	DetenerPipeline();
}

//----------------------------------------------------------------------------------------------------
//region Interfaz de Carga


// Comienzo de carga. Metodo sincrono con la interfaz.
void GNKVisualizator::WaveFormView::OnCargaIniciada()
{
}

// Paso 1: Cargar los ficheros en memoria. Metodo NO sincrono con la interfaz.
void GNKVisualizator::WaveFormView::CargarEstudio(GNC::GCS::IComando* /*pCmdInvocador*/)
{
}

// Finalizacion de la carga. Metodo sincrono con la interfaz.
void GNKVisualizator::WaveFormView::OnCargaFinalizada()
{
	GVista->OnFinishLoad();
}

// Paso 2: Inicializacion del pipeline. Metodo sincrono con la interfaz.
void GNKVisualizator::WaveFormView::IniciarPipeline()
{
	try {
		GVista->InitPipeline();
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

	//se conecta el evento Ginkgo
	GNC::GCS::Events::EventoModificacionFichero evtModificado;
	evtModificado.SetVista(this);

	VisualizatorStudy->ActiveFileIndex = 0;
	
	typedef GNC::GCS::Vector3D Vec;

	GVista->ViewImage2D->UpdateImage();
	
	vtkSmartPointer<vtkCamera> cam = GVista->ViewImage2D->GetRenderer()->GetActiveCamera();
	
	Vec focalPoint;
	Vec camPos;
	Vec spacing;
	Vec dims;
	Vec winsize;
	int idims[3];
	int iwinsize[2];

	GVista->ViewInteractor2D->Layout();
	GVista->ViewInteractor2D->GetParent()->Layout();

	GVista->ViewImage2D->GetSpacing(spacing.v);
	GVista->ViewImage2D->GetDimensions(idims);
	GVista->ViewInteractor2D->GetClientSize(&iwinsize[0], &iwinsize[1]);
	//((wxWindowBase* )GVista->ViewInteractor2D)->GetSize();

	dims.x = idims[0];
	dims.y = idims[1];
	winsize.x = iwinsize[0];
	winsize.y = iwinsize[1];
	
	Vec worldViewPort = (spacing * (dims + Vec(1.0, 1.0, 0.0))); // + 1.0 cause pixel centers to pixels edges conversion

	double parallelScale = worldViewPort.y / 2.0;

	GVista->ViewImage2D->SetInitialParallelScale( parallelScale );

	double rel = (winsize.x / winsize.y); // Relación ancho/alto del viewPort
	
	focalPoint = Vec( rel * (parallelScale), parallelScale, 0.0);
	camPos = focalPoint - Vec(0.0, 0.0, 1.0);

	cam->SetPosition( camPos.v );
	cam->SetFocalPoint( focalPoint.v );	
	cam->SetViewUp(0.0, -1.0, 0.0);
	cam->SetClippingRange(-2.0, 2.0);	

	//lanzo el evento de imagen cargada
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(this,GNC::GCS::Events::EventoModificacionImagen::ImagenCargada));
	/*
	GNC::GCS::IEntorno::Instance()->GetControladorHerramientas()->RefrescarHerramientas();
	*/

	GenerarTitulo();
}

// Parada del pipeline. Metodo sincrono con la interfaz Se invoca en el caso de que ocurra un error de carga.
void GNKVisualizator::WaveFormView::DetenerPipeline()
{
	GVista->StopPipeline();
}

//endregion


bool GNKVisualizator::WaveFormView::SoportaGuardar()
{
	return false;
}

bool GNKVisualizator::WaveFormView::SoportaExportar()
{
	return false;
}

void GNKVisualizator::WaveFormView::Activar()
{
	GNC::GCS::IVista::Activar();
	GVista->SetFocus();
}


wxWindow* GNKVisualizator::WaveFormView::GetWindow()
{
	return GVista;
}

void GNKVisualizator::WaveFormView::GenerarTitulo()
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

GNC::GCS::IVista* GNKVisualizator::WaveFormView::GetView()
{
	return this;
}

