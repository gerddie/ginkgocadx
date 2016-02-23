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
#include <wx/msgdlg.h>
#include <wx/menu.h>
#include <wx/dnd.h>

#include "gvistacompleja.h"
#include "gvistasimple.h"
#include "../vistas/vista2d.h"
#include <api/internationalization/internationalization.h>
#include <visualizator/estudios/overlay.h>
#include <resources/ginkgoresourcesmanager.h>
#include <resources/visualizatorresourcesmanager.h>
#include <api/dicom/idicommanager.h>
#include <api/dicom/dcmdictionary.h>
#include <api/iannotator.h>

#include <api/autoptr.h>
#include <export/contracts/ipixelsbluringcontract.h>

#include "../eventos/visualizatorevents.h"
#include <api/controllers/icontroladorlog.h>
#include <api/controllers/icontroladorvistas.h>
#include <api/dicom/dcmdictionary.h>
#include <eventos/modificacionimagen.h>
#include <eventos/modificacionfichero.h>
#include <eventos/modificacionwidget.h>
#include <eventos/mensajes.h>
#include <eventos/render.h>
#include <api/iwidgetsmanager.h>
#include "../estudios/visualizatorstudy.h"

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkVISUManagement/vtkLookupTableManager.h>
#include <vtk/widgets/widgetsactor.h>
#include <vtkCamera.h>
#include <vtkPointData.h>
#include <vtkImageMathematics.h>
#include <api/westilo.h>

#include <api/imodelointegracion.h>
#include <api/controllers/imodulecontroller.h>
#include <api/controllers/icontroladorpermisos.h>
#include <main/controllers/commandcontroller.h>
#include <main/controllers/hangingprotocolcontroller.h>

#include <main/entorno.h>

#include <commands/comandodestruirvista.h>

#include <cairo/cairo.h>
#include <widgets/openglhelper.h>
#include <widgets/wanotacionesquina.h>


#include <export/tools/iherramientascore.h>
#include <visualizator/export/tools/ivisualizatortools.h>
#include <api/toolsystem/itoolcontroller.h>
#include <api/toolsystem/itool.h>
#include <visualizator/wxvtk/previewpanelvisualizator.h>

#include <wx/wxginkgotoolbar.h>

#define ID_BOTON_VER 1
#define ID_BOTON_LAYOUT 2
#define ID_BOTON_SINCRONIZAR 3
#define ID_BOTON_DESENCAJAR 4

//esta clase sirve para las barras de herramientas de abajo
class BarraHerramientasBotones : public wxGinkgoToolBar{
public:
	BarraHerramientasBotones(wxWindow* pParent, const wxColor& color = wxColor(0x40, 0x40, 0x40)): wxGinkgoToolBar(pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE)
	{
		SetToolBitmapSize(wxSize(16,16));
		wxGinkgoToolBarArt* pToolArt= new wxGinkgoToolBarArt();
		pToolArt->SetCustomBaseColor(color);
		SetArtProvider(pToolArt);
	}

	~BarraHerramientasBotones()
	{
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class GVistaComplejaOverlaysDelegate: public GNKVisualizator::IOverlaysContract
{
public:
	GVistaComplejaOverlaysDelegate(GVistaCompleja* gView):GView(gView)
	{
	}
	~GVistaComplejaOverlaysDelegate()
	{
	}
	
protected:
	GVistaCompleja* GView;
};


class GVistaComplejaWindowLayoutDelegate: public GNC::GCS::IWindowLayoutContract
{
public:
	GVistaComplejaWindowLayoutDelegate(GVistaCompleja* gView):GView(gView)
	{
	}
	~GVistaComplejaWindowLayoutDelegate()
	{
	}

	//contrato layout
	virtual void SetWindowLayout(int filas, int columnas)
	{
		if(filas <1 || columnas < 1)
		{
			//layout invalido
			return;
		}
		bool incrementado = false;

		const int maxSlices = m_MaxSlice - m_MinSlice +1;
		if(filas*columnas > maxSlices) {
			//layout invalido aunque podría ser valido...
			filas = (int)std::ceil((double)maxSlices/columnas);
		}

		int filasActuales = GView->m_pSizerSeries->GetRows();
		int columnasActuales = GView->m_pSizerSeries->GetCols();
		int tamAnterior = GView->m_VistasSimples.size();
		int nuevoTam = filas*columnas;

		GView->m_pSizerSeries->SetCols(columnas);
		GView->m_pSizerSeries->SetRows(filas);


		//FUNDAMENTAL PARA QUE los viewers se destruyan correctamente y sin leaks ya que Viewer es smart pointer
		GView->IVista->VisualizatorStudy->Viewer = NULL;
		if(tamAnterior > nuevoTam) {
			//hay que borrar
			for(int i = 0; i< tamAnterior-nuevoTam; i++) {
				GVistaCompleja::TListaGVistasSimples::iterator pVista = GView->m_VistasSimples.begin();
				GView->m_pManager->LiberarRecursos((*pVista)->ViewInteractor2D);
				GView->m_pSizerSeries->Detach((*pVista));
				(*pVista)->Destroy();

				GView->m_VistasSimples.erase(pVista);
			}
		} else {
			int interpolationMode = VTK_NEAREST_INTERPOLATION;
			for (unsigned int i = 0; i < GView->m_VistasSimples.size(); ++i) {
				if (i == 0) {				
					interpolationMode = GView->m_VistasSimples[i]->ViewImage2D->GetInterpolationMode();
				}
				GView->m_VistasSimples[i]->ViewImage2D->ResetZoom(false, 0);
			}

			//hay que crear
			for(unsigned int i = 0; i< (unsigned int) (nuevoTam - tamAnterior) && GView->m_VistasSimples.size() < (unsigned int) maxSlices; i++) {
				incrementado = true;
				GView->AddVistaSimple();
			}
			for (unsigned int i = 1; i < GView->m_VistasSimples.size(); ++i) {
				GView->m_VistasSimples[i]->ViewImage2D->SetInterpolationMode(interpolationMode);
			}
			//por si la posicion actual es invalida
			GView->m_posicion = 0;
		}
		//FUNDAMENTAL PARA QUE FUNCIONE LA HERRAMIENTA DE MAPAS DE COLOR
		GView->IVista->VisualizatorStudy->Viewer = GView->m_VistasSimples[0]->ViewImage2D;
		GView->ActualizarMaxMinSlider();
		GView->Layout();
		//nos quedamos en la posicion
		GView->GoToSlice(GView->m_posicion,false,false,false);
		if ( (filas > 1 || columnas > 1) && incrementado) { // condición para evitar doble render. Ya se comprueba por otro lado en la creación (con 1 vista).
			bool noerror = true;
			for(GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); noerror && it !=GView-> m_VistasSimples.end(); ++it) {
				GVistaSimple* vs = *it;
				vs->ViewInteractor2D->Render();
				long idTextura = vs->ViewImage2D->GetImageTexture();
				if (idTextura == 0 || !glIsTexture(idTextura)) {
					noerror = false;
				}
			}
			if (!noerror) {
				SetWindowLayout(filasActuales, columnasActuales);
				wxMessageBox(_("It has exhausted the system's video memory and has not been able to assign the grid configuration. Close some studies to free memory."), _("Error configuring the grid"), wxOK, NULL);
				return;
			}
		}
	}
	
	virtual int GetRows()
	{
		return GView->m_pSizerSeries->GetRows();
	}

	virtual int GetColumns()
	{
		return GView->m_pSizerSeries->GetCols();
	}
	
protected:
	GVistaCompleja* GView;
};

class GVistaComplejaDelegate: public GNC::GCS::ISliderContract,  public GNC::GCS::IWindowLevelContract, public GNC::GCS::IRuleContract
{
public:
	GVistaComplejaDelegate(GVistaCompleja* gView):GView(gView)
	{
	}
	~GVistaComplejaDelegate()
	{
	}

	//contrato widgets

	GNC::GCS::IWidgetsManager* GetManager()
	{ 
		return GView->m_pManager; 
	}

	void GetAllViewers(std::list<vtkGinkgoImageViewer*>& viewerList)
	{
		for (GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); it != GView->m_VistasSimples.end(); ++it) {
			viewerList.push_back((*it)->ViewImage2D);
		}
	}
	//

	void SetSlice(int slicenum)
	{
		int tsize = GView->IVista->VisualizatorStudy->GetTSizeActive();

		if (tsize == 1) {
			GView->GoToSlice(slicenum, false);
		}
		else {
			//bool ok = true;
			for(GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); it != GView-> m_VistasSimples.end(); ++it) {
				GVistaSimple* vs = *it;
				vs->GoToTSlice(slicenum, false);
			}
		}
	}

	int GetSlice() const
	{
		int tsize = GView->IVista->VisualizatorStudy->GetTSizeActive();

		if (tsize == 1) {
			return GView->m_pSliderSlice->GetValue();
		}
		else {
			GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin();
			if (it != GView-> m_VistasSimples.end()) {
				GVistaSimple* vs = *it;
				return vs->ViewImage2D->GetTindex();
			}
			else {
				return 0;
			}
		}
	}

	int GetNumberOfSlices() const
	{
		int tsize = GView->IVista->VisualizatorStudy->GetTSizeActive();

		if (tsize == 1) {
			return GView->IVista->VisualizatorStudy->GetSliceNumber();
		}
		else {
			return tsize;
		}
	}

	virtual ListaWindowLevels& GetWindowLevelList()
	{
		return m_listaWindowLevels;
	}
	virtual std::string GetLabelCurrent()
	{
		return m_labelCurrent;
	}
	virtual void SetLabelCurrent(const std::string& labelCurrent)
	{
		m_labelCurrent = labelCurrent;
	}

	virtual bool SupportsCallibration() {
		return true;
	}

	virtual void DoAutoCalculateWindowLevel()
	{
		for(GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); it != GView->m_VistasSimples.end(); ++it) {
			(*it)->ViewImage2D->SetAutoDefaultWindowLevel();
			(*it)->ViewImage2D->ResetToDefaultWindowLevel();
			SetUserDefinedWindowLevel((*it)->ViewImage2D->GetWindow(), (*it)->ViewImage2D->GetLevel());
		}
	}

	virtual void DoResetWindowLevel() 
	{
		if (HasDefaultWindowLevel()) {
			GNC::GCS::IWindowLevelContract::WindowLevel defWl = GetAndSetDefaultWindowLevel();
			for(GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); it != GView->m_VistasSimples.end(); ++it) {
				(*it)->ViewImage2D->SetDefaultWindowLevel(defWl.m_window, defWl.m_level);			
				(*it)->ViewImage2D->ResetToDefaultWindowLevel();
			}
		} else {
			DoAutoCalculateWindowLevel();
		}
	}

	virtual void DoSetWindowLevel(double window, double level)
	{
		for(GVistaCompleja::TListaGVistasSimples::iterator it = GView->m_VistasSimples.begin(); it != GView->m_VistasSimples.end(); ++it) {
			(*it)->ViewImage2D->SetWindow(window);
			(*it)->ViewImage2D->SetLevel(level);						
		}
	}

protected:
	GVistaCompleja* GView;
	ListaWindowLevels m_listaWindowLevels;
	std::string m_labelCurrent;
};

class GVistaComplejaPixelsBluringDelegate: public GNC::GCS::IPixelsBluringContract
{
	public:
		GVistaComplejaPixelsBluringDelegate(GVistaCompleja* gView):GView(gView)
		{
		}

		~GVistaComplejaPixelsBluringDelegate()
		{
		}
		virtual GNC::GCS::IWidgetsManager* GetManager()
		{ 
			return GView->m_pManager; 
		}
		virtual void GetAllViewers(std::list<vtkGinkgoImageViewer*>& viewerList)
		{
			return GView->Delegate->GetAllViewers(viewerList);
		} 

		GVistaCompleja* GView;
};

class GVistaComplejaAnnotatorDelegate: public GNC::GCS::IAnnotator
{
public:
	GVistaComplejaAnnotatorDelegate(GVistaCompleja* gView, GNKVisualizator::Vista2D* pView) : GNC::GCS::IAnnotator(pView), 
		GView(gView)
	{
	}
	~GVistaComplejaAnnotatorDelegate()
	{
	}


	virtual std::string GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key)
	{
		if (key == "${SLICE.IDX}") {
			if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
				std::ostringstream ostr;
				ostr << c->GetRenderer()->m_pImageViewer->GetTindex() + 1;
				return ostr.str();
			}
		} else if (key == "${SLICE.IDX.FULLTEXT}") {
			if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL && m_pView != NULL && m_pView->GetEstudio().IsValid()) {
				int dims[3] = {0, 0, 0};
				c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
				std::ostringstream ostr;
				if (c->offscreenCreado) {
					ostr << _Std("Slice: ") <<  c->OVID + 1;
					if (dims[2] > 1) {
						ostr << " T: 0";
					}
		
				}
				else {
					ostr << _Std("Slice: ") <<  c->GetRenderer()->GetVID() + 1;
					if (dims[2] > 1) {
						ostr << " T: " << c->GetRenderer()->m_pImageViewer->GetTindex();
					}
				}
				return ostr.str();
			}
		}
		return GNC::GCS::IAnnotator::GetAnnotationValue(c, key);
	}

protected:
	GVistaCompleja* GView;
};

//contracts dellegate

/////////////////////////////////////DRAG&DROP/////////////////////////////////////////
namespace GNKVisualizator {
	namespace GUI {
		class wxDropTargetPreviewPanelVisualizator: public wxDropTarget
		{
		public:
			wxDropTargetPreviewPanelVisualizator(GVistaCompleja* gView): GView(gView)
			{
				SetDataObject(new GNKVisualizator::GUI::wxDnDPreviewPanelVisualizator);
				SetDefaultAction(wxDragCopy);
			}

			~wxDropTargetPreviewPanelVisualizator()
			{
			}

			virtual wxDragResult OnDragOver(wxCoord x, wxCoord y, wxDragResult /*def*/)
			{ 
				wxPoint coords(x, y);
				coords = GView->ClientToScreen(coords);
				wxPoint clientPreview = GView->m_pPanelPreview->ScreenToClient(coords);
				wxPoint client = GView->ScreenToClient(coords);
				if (GView->m_pPanelPreview->HitTest(clientPreview) != wxHT_WINDOW_INSIDE && GView->HitTest(client) == wxHT_WINDOW_INSIDE) {
					return wxDragCopy;
				}

			  return wxDragNone; 
			}

			virtual wxDragResult OnData(wxCoord x, wxCoord y, wxDragResult /*def*/)
			{
				if ( !GetData() )
					  return wxDragNone;
				GNKVisualizator::GUI::wxDnDPreviewPanelVisualizator *dobj = (GNKVisualizator::GUI::wxDnDPreviewPanelVisualizator *)m_dataObject;
				
				wxPoint coords(x, y);
				coords = GView->ClientToScreen(coords);
				 
				wxPoint client = GView->ScreenToClient(coords);
				wxPoint clientPreview = GView->m_pPanelPreview->ScreenToClient(coords);
				if (GView->m_pPanelPreview->HitTest(clientPreview) != wxHT_WINDOW_INSIDE && GView->HitTest(client) == wxHT_WINDOW_INSIDE) {
					GView->LoadSeries(dobj->GetSeriesPk());
					return wxDragCopy;
				}
				return wxDragNone;
			}

		private:
			GVistaCompleja* GView;
		};
	}
}
//////////////////////////////////////////////END DRAG&DROP////////////////////////////////////////////


GVistaCompleja::GVistaCompleja(GNKVisualizator::Vista2D* pIVista) :
        VistaComplejaBase(pIVista->GetVisualizatorStudy()->ParentWindow ),
	IVista(pIVista),
        m_pManager (GNC::GCS::IEntorno::Instance()->NewWidgetsManager(IVista)),
	Delegate(new GVistaComplejaDelegate(this)),
	OverlaysDelegate(new GVistaComplejaOverlaysDelegate(this)),
	WindowLayoutDelegate(new GVistaComplejaWindowLayoutDelegate(this)),
	PixelsBluringDelegate(new GVistaComplejaPixelsBluringDelegate(this)),
        AnnotatorDelegate(new GVistaComplejaAnnotatorDelegate(this, pIVista)), 
	m_pPanelPreview(new GNKVisualizator::GUI::PreviewPanelVisualizator(pIVista, this))
{
	IVista->GetVisualizatorStudy()->Window = this;
	m_hasBeenRendered = false;
	m_posicion = m_pSliderSlice->GetValue();

	// Setup del manejador de widgets */

	//toolbar
	GNC::GCS::IToolController* pToolBar = IVista->GetToolController();
	//widgets...
	pToolBar->AddTool(HCore_Pointer, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Zoom, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Slider, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Pan, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Angulo, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Rule, (GNC::GCS::IRuleContract*) Delegate);
	pToolBar->AddTool(HCore_Nota, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Video, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_Marcado, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_WindowLevel, (GNC::GCS::IWindowLevelContract*) Delegate);
	//view menu...
	pToolBar->AddTool(HCore_CornerAnnotations, (GNC::GCS::ISliderContract*) Delegate);
	#if !defined(__WXGTK__)
	pToolBar->AddTool(HCore_DetachWindow, (GNC::GCS::IViewContract*) IVista);
	#endif
	pToolBar->AddTool(HCore_MetricGrid, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_PixelsBluring, PixelsBluringDelegate);
	pToolBar->AddTool(HCore_Reset, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HCore_WindowLayout, WindowLayoutDelegate);
	pToolBar->AddTool(HCore_RotateAndFlip, (GNC::GCS::ISliderContract*) Delegate);

	pToolBar->AddTool(HCore_SecondaryCapture, (GNC::GCS::ISliderContract*)Delegate);
	pToolBar->AddTool(HCore_CopyImageToClipboard, (GNC::GCS::ISliderContract*)Delegate);
	pToolBar->AddTool(HCore_SimplePrint, (GNC::GCS::ISliderContract*)Delegate);
	//tools
	pToolBar->AddTool(HCore_ShowDicomTags, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_ShowHistory, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_ColorMap, (GNC::GCS::ISliderContract*) Delegate);
	pToolBar->AddTool(HGNKVisualizator_Overlays, OverlaysDelegate);
	pToolBar->AddTool(HCore_ImageExport, (GNC::GCS::ISliderContract*)Delegate);
	pToolBar->AddTool(HCore_Deshacer, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_PACSUpload, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_DicomizeFromView, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_AddFileToHistory, (GNC::GCS::IViewContract*) IVista);
	pToolBar->AddTool(HCore_QueryRetrieve, (GNC::GCS::IViewContract*) IVista);

	m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaVisualizacion), 0, wxEXPAND);
	m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaMedicion), 0, wxEXPAND);
	m_pSizerTools->Add(pToolBar->CreateNewToolBar(this, GNC::GCS::ITool::TFamiliaAnotacion), 0, wxEXPAND);

	{
		GNC::GCS::IToolController::TIdList listids;
		listids.push_back(HCore_WindowLayout);
		listids.push_back(HCore_ShowHistory);
		wxWindow* pwxToolBar = pToolBar->CreateNewToolBar(this, listids, false);
		m_pSizerTools->Add(pwxToolBar, 1, wxEXPAND);
	}

	//connect key events..
	pToolBar->ConnectKeyEvents(this);

	//preview series and drag & drop	
	m_pSizerView->Insert(0, m_pPanelPreview, 0, wxEXPAND);
	SetDropTarget(new GNKVisualizator::GUI::wxDropTargetPreviewPanelVisualizator(this));		

	//se añaden los botones de ver/cine
	m_pBarraIzquierda = new BarraHerramientasBotones(m_pPanelManipulacion);
	m_pBarraIzquierda->AddTool(ID_BOTON_VER,_("Display Options"),GNKVisualizator::ResourcesManager::ToolIcons::GetIcoMenuImagen(),_("Display Options"));
	m_pBarraIzquierda->Connect(ID_BOTON_VER,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GVistaCompleja::OnMenuVerClick),NULL,this);

	m_pBarraIzquierda->Realize();
	m_pSizerIzquierda->Add(m_pBarraIzquierda,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);

	{
		GNC::GCS::IToolController::TIdList toolsLeft;
		#if !defined(__WXGTK__)
		toolsLeft.push_back(HCore_DetachWindow);
		#endif
		toolsLeft.push_back(HCore_WindowLayout);
		toolsLeft.push_back(HCore_Video);
		wxWindow* toolBarLeft = pToolBar->CreateNewToolBar(m_pPanelManipulacion, toolsLeft);
		m_pSizerIzquierda->Add(toolBarLeft,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);		
	}
	//se añaden los de sincronizar/dessincronizar
	m_pBarraDerecha = new BarraHerramientasBotones(m_pPanelManipulacion);
	m_pBarraDerecha->AddTool(ID_BOTON_SINCRONIZAR,_("Synchronize/Des synchronize"),GNKVisualizator::ResourcesManager::SynchronizationBar::GetIcoDessincronizar(),_("Synchronize/Des synchronize"),wxITEM_CHECK);
	m_pBarraDerecha->Connect(ID_BOTON_SINCRONIZAR,wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( GVistaCompleja::OnSincronizarClick),NULL,this);
	m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,false);

	m_pBarraDerecha->Realize();
	m_pSizerDerecho->Add(m_pBarraDerecha,0, wxALIGN_CENTER_VERTICAL|wxEXPAND,0);

	//Connect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GVistaCompleja::OnFocus),NULL,this);
	Connect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(GVistaCompleja::OnMouseWheel),NULL,this);

	GNC::GCS::Events::EventoModificacionImagen ev2(IVista);
	GNC::GCS::Events::EventoModificacionWidget ev3(IVista);
	GNC::GCS::Events::EventoRender ev4(IVista);
	GNKVisualizator::Events::EventoMostrarOverlay evt5(IVista);

	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev2);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev3);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, ev4);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this, evt5);

	AddVistaSimple();

	//FUNDAMENTAL PARA QUE FUNCIONE LA HERRAMIENTA DE MAPAS DE COLOR
	IVista->VisualizatorStudy->Viewer = m_VistasSimples[0]->ViewImage2D;

	//m_pManager->InsertarWidget(new GNC::GCS::Widgets::WProgreso(m_VistasSimples.front()->ViewInteractor2D, m_pManager, -1, "progreso", 666));
	m_pManager->EnableAnimation();
	m_pManager->SetAnnotator(AnnotatorDelegate);

	this->Layout();
}

GVistaCompleja::~GVistaCompleja()
{
	std::string modality("");
	IVista->GetEstudio()->GetTagActiveImage(GKDCM_Modality, modality);
	IVista->GetToolController()->FinalizeToolController();

	IVista->ComienzaDestruccion();
	//FUNDAMENTAL PARA QUE los viewers se destruyan correctamente y sin leaks ya que Viewer es smart pointer
	for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
		(*it)->UnRefIVista();
	}

	IVista->VisualizatorStudy->Viewer = NULL;

	if (IVista != NULL) {
		GNC::GCS::IEntorno::Instance()->GetCommandController()->AbortarComandosDeOwner(IVista);
	}

	//Disconnect(wxEVT_CHILD_FOCUS, wxChildFocusEventHandler(GVistaCompleja::OnFocus),NULL,this);
	this->Disconnect(wxEVT_MOUSEWHEEL,wxMouseEventHandler(GVistaCompleja::OnMouseWheel),NULL,this);

	if(m_ListaSincronizacion.size() >0){
		DesSincronizar();
	}

	m_pManager->EliminarTodosLosWidgets(false);

	m_VistasSimples.clear();

	GNC::GCS::IEntorno::Instance()->FreeWidgetsManager(m_pManager);
	m_pManager = NULL;
	
/*TODO	GNKVisualizator::IReconstructionTool* hReconstruction = GNC::GCS::IEntorno::Instance()->GetControladorHerramientas()->ObtenerHerramientaConcreta<GNKVisualizator::IReconstructionTool>(GNKVisualizator::IReconstructionTool::ID);
	if (hReconstruction != NULL) {
		((GNC::GCS::IContratable<GNKVisualizator::IReconstructionContract>*)hReconstruction)->DesSubscribirsLosDeLaVista(IVista);
	}*/

	IVista->Lock();
	delete IVista;
}

void GVistaCompleja::AddVistaSimple()
{
	GVistaSimple* pVistaSimple = new GVistaSimple(IVista,this);
	m_VistasSimples.push_back(pVistaSimple);
	m_pSizerSeries->Add(pVistaSimple,1,wxEXPAND);


	GNC::GCS::IWidgetsRenderer* pR = pVistaSimple->ViewInteractor2D;
	pR->SetManager(m_pManager);
	pR->SetIOverlay(pVistaSimple);
	//m_pManager->SetRendererActivo(pR);
	std::ostringstream os;
	os << "Renderer " << pVistaSimple << std::endl;
	pR->sid = os.str();
	////

	WidgetRepresentation* pWidgetsRepresentation = WidgetRepresentation::New();
	pWidgetsRepresentation->SetRenderer(pVistaSimple->ViewInteractor2D->FindPokedRenderer(0,0));
	pVistaSimple->ViewInteractor2D->FindPokedRenderer(0,0)->AddViewProp(pWidgetsRepresentation);
	pR->SetRepresentation(pWidgetsRepresentation);
	pWidgetsRepresentation->SetWidgetsManager(m_pManager);
	pWidgetsRepresentation->SetWidgetsRenderer(pR);
	pWidgetsRepresentation->Delete();

	pVistaSimple->ViewImage2D->SetWindowLevelFrom(m_VistasSimples[0]->ViewImage2D);
	pVistaSimple->ViewImage2D->SetLookupTable(m_VistasSimples[0]->ViewImage2D->GetLookupTable(), m_VistasSimples[0]->ViewImage2D->GetIdLookupTable());
	pVistaSimple->ViewImage2D->SetZoom(m_VistasSimples[0]->ViewImage2D->GetZoom());
	pVistaSimple->ViewImage2D->CopyCameraStatus(m_VistasSimples[0]->ViewImage2D);

	for (unsigned int i = 0; i < m_VistasSimples.size(); i++) {
		GNC::GCS::IWidgetsRenderer* pR = m_VistasSimples[i]->ViewInteractor2D;
		pR->SetVID(i, false);
		std::ostringstream os;
		os << "Renderer " << i << std::endl;
		pR->sid = os.str();
	}
}

void GVistaCompleja::LoadSeries(long seriesPk)
{
	IVista->VisualizatorStudy->InitializeContext(seriesPk);

	for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) 
	{
		(*it)->setEstudioReferido(IVista->GetEstudio());
	}
	IniciarPipeline();
}

void GVistaCompleja::IniciarPipeline(const GNC::GCS::Ptr<GNC::GCS::Layout>& layout)
{
	m_pManager->RemoveUserAnnotationWidgets(false);
	m_pPanelManipulacion->Enable(true);

	GNC::GCS::Ptr<std::list<GNKVisualizator::TOverlay> > overlays(IVista->VisualizatorStudy->GetOverlaysImagenActiva());
	OverlaysDelegate->Setup(overlays, IVista);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNKVisualizator::Events::EventoRefrescarListaOverlays(IVista,OverlaysDelegate->GetOverlayList()));

	//contratos que son 1:1 con gvistacompleja
	//esto es correcto porque en la creacion se mete siempre una vista simple
	GNC::GCS::Ptr<GIL::DICOM::DicomDataset> ds = IVista->GetEstudio()->GetTagsActiveImage();
	Delegate->Setup(*ds);
	PixelsBluringDelegate->Setup(ds->getTag(GKDCM_Modality));


	///////////recorrer las vistas simples y pasarles las imagenes	
	if(!m_VistasSimples.empty()) {
		int i = 0;
		for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
			GVistaSimple* pvs = *it;
			pvs->EstudioReferido->SetActiveIndex(i);
			pvs->SetOverlays(OverlaysDelegate->GetOverlayList(), OverlaysDelegate->m_EstadoOverlays);

			//despues se establece el WL
			if (Delegate->HasDefaultWindowLevel()) {
				GNC::GCS::IWindowLevelContract::WindowLevel wl = Delegate->GetAndSetDefaultWindowLevel();
				pvs->ViewImage2D->SetDefaultWindowLevel(wl.m_window, wl.m_level);
			}
			else {
				pvs->ViewImage2D->SetAutoDefaultWindowLevel();
			}
			i++;
		}
		m_pManager->SetRendererActivo( (* (m_VistasSimples.begin()) )->ViewInteractor2D );

		WindowLayoutDelegate->Setup(0, IVista->GetEstudio()->GetSliceNumber() - 1);

		ActualizarMaxMinSlider();
	}

	//contrato window level, hay que preguntarle al viewer de la primera vista porque el estudioatencionprimaria no tiene viewer
	IVista->GetToolController()->Enable(HCore_WindowLevel, m_VistasSimples[0]->ViewImage2D->GetNumberOfComponents() == 1);

	if (layout.IsValid()) {
		if (layout->getMatcher().IsValid()) {
			for (std::vector<long>::iterator it = IVista->VisualizatorStudy->OpenedSeries.begin(); it != IVista->VisualizatorStudy->OpenedSeries.end(); ++it)
			{
				if (layout->getMatcher()->MatchesSeries((*it))) 
				{
					LoadSeries(*it);
				}
			}
			WindowLayoutDelegate->SetWindowLayout(layout->getRows(), layout->getColumns());
		}
		layout->applyWindowProperties(IVista);
	} else {
		GoToSlice(0,false,false);
	}
}



void GVistaCompleja::DetenerPipeline()
{
	if (m_pManager != NULL) {
		m_pManager->OcultarTodosLosWidgets(true, 666);
		m_pManager->DisableAnimation();
		m_pPanelManipulacion->Enable(true);
		m_pManager->Render();
	}
}

void GVistaCompleja::OnCargaFinalizada()
{
	m_pPanelPreview->LoadModel();
	Layout();
	m_pManager->OcultarTodosLosWidgets(true, 666);
	m_pManager->DisableAnimation();
	m_pManager->Render();
}


void GVistaCompleja::OnPaint(wxPaintEvent& event)
{
	//std::cout << "GVistaCompleja::OnPaint()" << std::endl;
	VistaComplejaBase::OnPaint(event);
	if(GNC::GCS::IEntorno::Instance()->GetControladorVistas()->GetVistaActiva() == IVista)
	{
		wxPaintDC dc(this);
		wxColour colorLineaSeleccion(219, 219, 0, 255);
		dc.SetBrush(wxBrush(colorLineaSeleccion,wxTRANSPARENT));
		dc.SetPen(wxPen(colorLineaSeleccion, 3, wxSOLID));
		dc.DrawRectangle(wxRect(wxPoint(1, 1), wxPoint(dc.GetSize().x-1, dc.GetSize().y-1)));
	}
	//la comprobacion de la textura, se hace la primera vez que se pinta
	if(!m_hasBeenRendered ) {
		if(IVista->EstaCargada()) {
			for(GVistaCompleja::TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
				GVistaSimple* vs = *it;
				try {
					vs->ViewInteractor2D->Render();
					long idTextura = vs->ViewImage2D->GetImageTexture();
					if (idTextura != 0) {
						if ( !glIsTexture(idTextura) ) {
							LOG_ERROR("GVistaCompleja", "Textura no cargada en memoria de video")
							GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,_Std("Error: You have exhausted the system's video memory and has not been able to load the image. Close some studies to free memory."),GNC::GCS::Events::EventoMensajes::PopUpMessage,false));
							GADAPI::ComandoDestruirVistaParams* pParams = new GADAPI::ComandoDestruirVistaParams(IVista);
							GNC::GCS::IEntorno::Instance()->GetCommandController()->ProcessAsync(_Std("Destroy View"),new GADAPI::ComandoDestruirVista(pParams),NULL);
						}
					} else {
						//se comprobara mas tarde porque la textura todavia no tiene id
						return;
					}
				}catch(const std::bad_alloc&)
				{
					LOG_ERROR("GVistaCompleja","bad_alloc, error al reservar memoria física");
					GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,_Std("Error: System out of memory. Close some studies to free memory."),GNC::GCS::Events::EventoMensajes::PopUpMessage,false));
					GADAPI::ComandoDestruirVistaParams* pParams = new GADAPI::ComandoDestruirVistaParams(IVista);
					GNC::GCS::IEntorno::Instance()->GetCommandController()->ProcessAsync(_Std("Destroy View"),new GADAPI::ComandoDestruirVista(pParams),NULL);
				}
				catch(...)
				{
					LOG_ERROR("GVistaCompleja","excepción general al comprobar errores de memoria");
					GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoMensajes(NULL,_Std("Error: Internal error."),GNC::GCS::Events::EventoMensajes::PopUpMessage,false));
					GADAPI::ComandoDestruirVistaParams* pParams = new GADAPI::ComandoDestruirVistaParams(IVista);
					GNC::GCS::IEntorno::Instance()->GetCommandController()->ProcessAsync(_Std("Destroy View"),new GADAPI::ComandoDestruirVista(pParams),NULL);
				}
			}
			m_hasBeenRendered = true;
		}
	}
}

void GVistaCompleja::OnMenuVerClick(wxCommandEvent& )
{
	IVista->OnFocus();
	wxMenu ImageMenu ;
	IVista->GetToolController()->AppendsToolInMenu(this, &ImageMenu, GNC::GCS::ITool::TSeriesMenu);
	m_pBarraIzquierda->SetToolSticky(ID_BOTON_VER, true);
	wxRect rect = m_pBarraIzquierda->GetToolRect(ID_BOTON_VER);
	wxPoint pt = m_pBarraIzquierda->ClientToScreen(rect.GetBottomLeft());
	pt = ScreenToClient(pt);

	PopupMenu(&ImageMenu, pt);
	// make sure the button is "un-stuck"
	m_pBarraIzquierda->SetToolSticky(ID_BOTON_VER, false);
}

void GVistaCompleja::ActualizarMaxMinSlider()
{
	//depende de m_MinSlice y m_MaxSlice;
	//tb depende del numero de vistasimples
	int minSlider = WindowLayoutDelegate->m_MinSlice;
	int maxSlider = ((WindowLayoutDelegate->m_MaxSlice - WindowLayoutDelegate->m_MinSlice) +1)  - (m_pSizerSeries->GetRows() * m_pSizerSeries->GetCols());
	if (minSlider < maxSlider) {
		m_pSliderSlice->SetRange(minSlider, maxSlider);
		m_pSliderSlice->Enable(true);
		m_pSliderSlice->Show(true);
		m_pBarraDerecha->Show(true);
		m_pBarraDerecha->Refresh();
	} else {
		m_pSliderSlice->SetRange(0, 1);
		if(!m_ListaSincronizacion.empty())
		{
			Sincronizar(false);
		}
		m_pSliderSlice->Show(false);
		m_pBarraDerecha->Show(false);
	}
}


void GVistaCompleja::OnFocus(wxChildFocusEvent &)
{
	IVista->OnFocus();
}

void GVistaCompleja::OnSize(wxSizeEvent &)
{
	this->Layout();
}

void GVistaCompleja::OnZSliderScroll (wxScrollEvent& event) {
	int pos = event.GetPosition() - m_posicion;

	if(pos!=0){
		GoToSlice(event.GetPosition(), false);
	}
	event.Skip(false);
}

void GVistaCompleja::OnMouseWheel(wxMouseEvent& event)
{
	if (event.ControlDown())
	{
		if (event.ShiftDown()) {
			for (TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
				if(event.GetWheelRotation() > 0)
				{
					(*it)->GoToTSlice(-1);
				}
				else
				{
					(*it)->GoToTSlice(1);
				}
			}
		} else {
			if(event.GetWheelRotation() > 0)
			{
				GoToSlice(-1);
			}
			else
			{
				GoToSlice(1);
			}
		}
	} 
}

void GVistaCompleja::ActualizarImagen()
{
	//m_pImagenOriginal->Modified();
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::ImagenModificada, -1));
}

GNC::GCS::IVista* GVistaCompleja::GetView()
{
	return IVista;
}

//----------DIALOGO SINCRONIZACION...-------------
class DialogoSincronizacion: public DialogoSincronizacionBase {
public:
	typedef std::vector<GNKVisualizator::Vista2D*>	TipoListaVistas;
	DialogoSincronizacion(wxWindow* parent, TipoListaVistas listaVistas):
			DialogoSincronizacionBase(parent),
			m_lista(listaVistas)
	{
		for(TipoListaVistas::iterator it = listaVistas.begin(); it!=listaVistas.end(); ++it){
			m_pCheckListVistas->AppendString(wxString::FromUTF8((*it)->GetTitulo().c_str()));
		}
		this->Layout();
	}

	~DialogoSincronizacion(){

	}

	void OnAceptarClick(wxCommandEvent&){
		EndModal(wxID_OK);
	}

	TipoListaVistas GetVistasSeleccionadas(){
		TipoListaVistas resultado;
		wxArrayInt selecciones;
		for(unsigned int i=0; i<m_pCheckListVistas->GetCount();i++){
			//se busca la vista
			for(TipoListaVistas::iterator itVistas = m_lista.begin(); itVistas!= m_lista.end(); ++itVistas){
				if(m_pCheckListVistas->IsChecked(i)){
					resultado.push_back(m_lista[i]);
					break;
				}
			}
		}
		return resultado;
	}

protected:
	TipoListaVistas m_lista;
};

//---------- end DIALOGO SINCRONIZACION...-------------

void GVistaCompleja::OnSincronizarClick(wxCommandEvent&)
{
	bool enabled = m_pBarraDerecha->GetToolToggled(ID_BOTON_SINCRONIZAR);
	Sincronizar(enabled);
}

void GVistaCompleja::Sincronizar(bool sincronizar)
{
	//si esta toogled...
	if(!sincronizar) {
		//dessincronizar
		DesSincronizar();
	} else {
		//sincronizar
		typedef GNC::GCS::IControladorVistas::TipoListaVistas TipoListaIVistas;

		TipoListaVista2D lista;
		{
			TipoListaIVistas listaTmp = GNC::GCS::IEntorno::Instance()->GetControladorVistas()->GetVistas();

			//pillamos las vistas simples
			for(TipoListaIVistas::iterator it = listaTmp.begin(); it!= listaTmp.end(); ++it){
				GNKVisualizator::Vista2D* v = dynamic_cast<GNKVisualizator::Vista2D*> ((*it));
				if(v!=NULL && v!=IVista && v->GVista->EsSincronizable()){
					lista.push_back(v);
				}
			}
		}

		DialogoSincronizacion* dialogoSincronizacion = new DialogoSincronizacion(this, lista);
		int answer = dialogoSincronizacion->ShowModal();
		if (answer == wxID_OK) {
			//se pillan las vistas seleccionadas
			lista = dialogoSincronizacion->GetVistasSeleccionadas();
			if(!lista.empty()){
				//nos metemos en la lista
				lista.push_back(IVista);
				//y ahora sincronizamos a tol mundo
				for(DialogoSincronizacion::TipoListaVistas::iterator it = lista.begin(); it!= lista.end(); ++it){
					(*it)->GVista->Sincronizar(lista);
				}
				m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,true);
				m_pBarraDerecha->Refresh();
			} else {
				m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,false);
				m_pBarraDerecha->Refresh();
			}
		}
	}
}

void GVistaCompleja::Sincronizar(const TipoListaVista2D& lista)
{
	//se suscribe a los eventos de los brothers
	for(TipoListaVista2D::const_iterator it = lista.begin(); it!= lista.end(); ++it){
		if((*it) != IVista){
			AtencionPrimaria::Events::EventoSincronizacion evento((*it));
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->Registrar(this,evento);
			m_ListaSincronizacion.push_back((*it));
		}
	}
	m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,true);
	m_pBarraDerecha->Refresh();
}

bool GVistaCompleja::EsSincronizable(){
	return m_ListaSincronizacion.empty() && m_pSliderSlice->IsShown();
}

void GVistaCompleja::DesSincronizar(){
	for(TipoListaVista2D::iterator it = m_ListaSincronizacion.begin(); it!= m_ListaSincronizacion.end(); ++it){
		AtencionPrimaria::Events::EventoSincronizacion eventoOtro((*it));
		GNC::GCS::IEntorno::Instance()->GetControladorEventos()->DesRegistrar(this,eventoOtro);
	}
	m_ListaSincronizacion.clear();

	//dessincronizamos
	AtencionPrimaria::Events::EventoSincronizacion* evento = new AtencionPrimaria::Events::EventoSincronizacion(IVista,AtencionPrimaria::Events::EventoSincronizacion::Dessincronizar);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(evento);

	m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,false);
	m_pBarraDerecha->Refresh();
}

void GVistaCompleja::GoToSlice(int pos, bool relativa, bool propagar, bool render){
	if (WindowLayoutDelegate->m_MaxSlice == WindowLayoutDelegate->m_MinSlice) {
		if (render) {
			GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::SliceCambiado, -1, render);
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);
		}
		return;
	}
	int posicion;

	if(relativa){
		posicion = m_pSliderSlice->GetValue() + pos;
	}else{
		posicion = pos;
	}

	posicion = std::max(m_pSliderSlice->GetMin(), std::min(m_pSliderSlice->GetMax(), posicion));

	//si la posicion + las que se muestran en el layout actual es mayor al número de slices de la serie, no se avanza

	if(posicion + ( (int) m_VistasSimples.size() ) > IVista->GetEstudio()->GetSliceNumber()) {
		if (render) {
			GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::SliceCambiado, -1, render);
			GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);
		}
		return;
	}

	m_pSliderSlice->SetValue(posicion);
	m_posicion = posicion;

	//se reestablece el window/level antes del evento de modificación porque ese evento lo escucha la herramienta
	Delegate->Setup(*IVista->GetEstudio()->GetTagsImage(m_posicion));

	//se le indica al manager el vid
	IVista->GetEstudio()->ActiveFileIndex = posicion;
	for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
		(*it)->GoToSlice(posicion,false,false);

		//despues se establece el WL
		if (Delegate->HasCurrentWindowLevel()) {
			GNC::GCS::IWindowLevelContract::WindowLevel wl = Delegate->GetCurrentWindowLevel();
			(*it)->ViewImage2D->SetDefaultWindowLevel(wl.m_window, wl.m_level);
		} else if (Delegate->HasDefaultWindowLevel()) {
			GNC::GCS::IWindowLevelContract::WindowLevel wl = Delegate->GetAndSetDefaultWindowLevel();
			(*it)->ViewImage2D->SetDefaultWindowLevel(wl.m_window, wl.m_level);			
		} else {
			(*it)->ViewImage2D->SetAutoDefaultWindowLevel();
		}

		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays> pLista = IVista->VisualizatorStudy->GetOverlaysImagen(posicion);
		(*it)->SetOverlays(pLista,OverlaysDelegate->m_EstadoOverlays);

		posicion++;
	}
		
	
	GNC::GCS::Events::EventoModificacionImagen* pEvt = new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::SliceCambiado, -1, render);
	GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);

	if(propagar){
		GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new AtencionPrimaria::Events::EventoSincronizacion(IVista, AtencionPrimaria::Events::EventoSincronizacion::Scroll,pos,relativa));
	}
}

int GVistaCompleja::GetCurrentSlice() const
{
	return m_posicion;
}

void GVistaCompleja::PreProcesarEvento(GNC::GCS::Events::IEvent* evt, std::list<GNC::GCS::Events::IEvent*>& lista)
{
	if (evt == NULL) {
		std::cerr << "Error: Evento nulo" << std::endl;
		return;
	}
	switch (evt->GetCodigoEvento()) {
		case ginkgoEVT_GNKVisualizator_EventoSincronizacion:
			{
				AtencionPrimaria::Events::EventoSincronizacion* pEvt = dynamic_cast<AtencionPrimaria::Events::EventoSincronizacion*>(evt);
				if (pEvt == NULL) {
					std::cerr << "Error al interpretar evento como evento de sincronización: Evento = " << evt << std::endl;
					return;
				}
				if (pEvt->GetTipoEvento() != AtencionPrimaria::Events::EventoSincronizacion::Scroll) {
					return;
				}
				int posicion = pEvt->GetPosicion();
				if(pEvt->EsRelativa()){
					posicion += m_pSliderSlice->GetValue();
				}
				posicion = std::max(m_pSliderSlice->GetMin(), std::min(m_pSliderSlice->GetMax(), posicion));
			}
			break;
		case ginkgoEVT_Core_ModificacionImagen:
			{
				GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
				if (pEvt == NULL) {
					std::cerr << "Error al interpretar evento como evento de modificación de imagen: Evento = " << evt << std::endl;
					return;
				}
				switch (pEvt->GetTipo()) {
					case GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
						{
							lista.push_back(new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::ImagenCargada, -1));
							lista.push_back(new GNC::GCS::Events::EventoModificacionFichero(IVista,GNC::GCS::Events::EventoModificacionFichero::FicheroModificado));
						}
						break;
					default:
						break;
				}
				if (pEvt->GetApilarRender()) {
					lista.push_back(new GNC::GCS::Events::EventoRender(IVista,pEvt->GetSliceAfectado()));
				}
			}
			break;
		case ginkgoEVT_Core_ModificacionWidget:
			{
				GNC::GCS::Events::EventoModificacionWidget* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionWidget*>(evt);
				if (pEvt == NULL) {
					std::cerr << "Error al interpretar evento como evento de modificación de widget: Evento = " << evt << std::endl;
					return;
				}
				// TODO: Este flujo es muy mejorable
				lista.push_back(new GNC::GCS::Events::EventoModificacionFichero(IVista,GNC::GCS::Events::EventoModificacionFichero::FicheroModificado,IVista));
			}
			break;
		case ginkgoEVT_GNKVisualizator_EventoMostrarOverlay:
			{
				lista.push_back(new GNC::GCS::Events::EventoRender(IVista));
			}
			break;
		default:
			break;
	}
}

void GVistaCompleja::ProcesarEvento(GNC::GCS::Events::IEvent *evt)
{
	switch (evt->GetCodigoEvento()) {
		case ginkgoEVT_GNKVisualizator_EventoSincronizacion:
			{
				AtencionPrimaria::Events::EventoSincronizacion* pEvt = dynamic_cast<AtencionPrimaria::Events::EventoSincronizacion*>(evt);
				if(pEvt!=NULL){
					if(pEvt->GetTipoEvento() == AtencionPrimaria::Events::EventoSincronizacion::Scroll){
						GoToSlice(pEvt->GetPosicion(),pEvt->EsRelativa(),false);
					} else if (pEvt->GetTipoEvento() == AtencionPrimaria::Events::EventoSincronizacion::Dessincronizar) {
						for(TipoListaVista2D::iterator it = m_ListaSincronizacion.begin(); it!= m_ListaSincronizacion.end(); ++it){
							if((*it) == pEvt->GetVista()){
								GNC::GCS::IEntorno::Instance()->GetControladorEventos()->DesRegistrar(this,*pEvt);
								m_ListaSincronizacion.erase(it);
								if(m_ListaSincronizacion.empty()){
									m_pBarraDerecha->ForceToggleTool(ID_BOTON_SINCRONIZAR,false);
									m_pBarraDerecha->Refresh();
								}
								return;
							}
						}
					}
				}
			}
			break;

			case ginkgoEVT_Core_ModificacionImagen:
			{
				GNC::GCS::Events::EventoModificacionImagen* pEvt = dynamic_cast<GNC::GCS::Events::EventoModificacionImagen*>(evt);
				if (pEvt == NULL) {
					std::cerr << "Error al interpretar evento como evento de modificación de imagen: Evento = " << evt << std::endl;
					return;
				}
				switch (pEvt->GetTipo()) {
					case GNC::GCS::Events::EventoModificacionImagen::ImagenModificada:
					case GNC::GCS::Events::EventoModificacionImagen::ImagenCargada:
					case GNC::GCS::Events::EventoModificacionImagen::MapaModificado:
					case GNC::GCS::Events::EventoModificacionImagen::SliceCambiado:
							{
								for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
									(*it)->ViewImage2D->Modified();
								}
								GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
							}
							break;
					case GNC::GCS::Events::EventoModificacionImagen::ImagenRecalibrada:
						{
							
								int i = 0;
								for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it, i++) {
									m_VistasSimples[i]->GetEstudio()->RecalibrateActiveImage(pEvt->GetNewSpacing(), pEvt->GetNewOrigin());

									//esto es necesario para establecer bien las coordenadas cuando se cambia el origen
									(*it)->GoToSlice(m_posicion + i,false,false);
									pEvt->SetSlice(-1);
									(*it)->RefrescarOverlays(OverlaysDelegate->m_EstadoOverlays);
								}
								//se vuelve hacer setup del contrato mapa de color
							//	GNKVisualizator::IContratoMapaColor::Setup(m_pImagenOriginal);
								
							
						}
					break;
					case GNC::GCS::Events::EventoModificacionImagen::VisualizacionImagenModificada:
						//para que pille bien el window/level
						if (pEvt->AutoCalculeWindowLevel()) {
							Delegate->DoAutoCalculateWindowLevel();
						} else {
							if(pEvt->ResetWindowLevel()) {
								Delegate->DoResetWindowLevel();
							} else {
								Delegate->DoSetWindowLevel(pEvt->GetWindow(), pEvt->GetLevel());
							}
						}

						for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
							if(pEvt->ResetCurrentPoint())
							{
								/*
								double* bounds  = (*it)->ViewImage2D->GetInput()->GetBounds();
								const double* currentPoint = (*it)->ViewImage2D->GetCurrentPoint();
								double  pos[3] =
								{
									(bounds[0]+bounds[1])/2.0,
									(bounds[2]+bounds[3])/2.0,
									currentPoint[2]
								};

								(*it)->ViewImage2D->SetCurrentPoint(pos);
								*/
							}
							if(pEvt->ResetZoom())
							{
								(*it)->ViewImage2D->ResetZoom(false, pEvt->GetResetZoomMode());
							}
							//(*it)->ViewImage2D->Modified();
						}
						GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(new GNC::GCS::Events::EventoModificacionImagen(IVista,GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas));
					break;
					case GNC::GCS::Events::EventoModificacionImagen::ImagenDescargada:
					break;
					case GNC::GCS::Events::EventoModificacionImagen::AnotacionesEstaticasModificadas:

					break;
				}
			}//fin modificacion
			break;

		case ginkgoEVT_Core_Render:
			{
				for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
					(*it)->ViewInteractor2D->Refresh(false);
				}
			}
			break;
		case ginkgoEVT_Core_ModificacionFichero:
			{
				GNC::GCS::Events::EventoModificacionFichero* pEvt = new GNC::GCS::Events::EventoModificacionFichero();
				pEvt->SetVista(IVista);
				GNC::GCS::IEntorno::Instance()->GetControladorEventos()->ProcesarEvento(pEvt);
			}
			break;
		case ginkgoEVT_GNKVisualizator_EventoMostrarOverlay:
			{
				for(TListaGVistasSimples::iterator it = m_VistasSimples.begin(); it != m_VistasSimples.end(); ++it) {
					(*it)->RefrescarOverlays(OverlaysDelegate->m_EstadoOverlays);
					(*it)->ViewImage2D->Modified();
				}
			}
			break;
	}
}
