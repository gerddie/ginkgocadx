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


#if defined(ENABLE_RECONSTRUCTIONTOOL)
#include <wx/menu.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include "reconstructiontool.h"
#include <eventos/eventosginkgo.h>


#ifdef __DEPRECATED
#undef __DEPRECATED
#endif

#include <vtkObject.h>

#include <vtkImageData.h>

#include <api/ientorno.h>
#include <api/controllers/icontroladorpermisos.h>
#include <vtkDataSetAttributes.h>
#include <vtkAlgorithm.h>
#include <vtkAlgorithmOutput.h>
#include <vtkExecutive.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtk/vtkginkgoimageviewer.h>

#define MENU_IDS_RECONSTRUCTION  4201

namespace GNKVisualizator {

	namespace GUI {

		class EventHandlerReconstructionTool: public wxEvtHandler {
		public:
			EventHandlerReconstructionTool(wxWindow* pParent, GNKVisualizator::ReconstructionTool* pTool, int modeId):wxEvtHandler()
			{
				m_pParent = pParent;
				m_pTool = pTool;

				m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerReconstructionTool::OnReconstructionModeSelected) ,NULL, this);
				m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(EventHandlerReconstructionTool::OnUpdateMenuReconstructionUI),NULL,this);
			}
			~EventHandlerReconstructionTool()
			{
				m_pParent->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( EventHandlerReconstructionTool::OnReconstructionModeSelected),NULL,this);
				m_pParent = NULL;
				m_pTool = NULL;
			}

			void OnReconstructionModeSelected( wxCommandEvent& event )
			{
				int modeId = event.GetId() - MENU_IDS_RECONSTRUCTION;
				m_pTool->SelectReconstructionMode(modeId);
				event.Skip(false);
			}

			void OnUpdateMenuReconstructionUI(wxUpdateUIEvent& event) {
				event.Enable(m_pTool->SupportsReconstructionMode(event.GetId() - MENU_IDS_RECONSTRUCTION));
			}

			GNKVisualizator::ReconstructionTool* m_pTool;
			wxWindow* m_pParent;
		};

		class MenuItemReconstruction : public wxMenuItem {
		public:
			MenuItemReconstruction(wxWindow* pParent,int idMode, const wxString& text,  wxMenu* pMenuPadre, GNKVisualizator::ReconstructionTool* pTool, GNC::GCS::IControladorPermisos* pCP):wxMenuItem(pMenuPadre,idMode,text,text,wxITEM_NORMAL) {
				pEventHandler = new EventHandlerReconstructionTool(pParent,pTool, idMode);
			}

			~MenuItemReconstruction()
			{
				if(pEventHandler != NULL) {
					delete pEventHandler;
					pEventHandler = NULL;
				}
			}

			EventHandlerReconstructionTool* pEventHandler;
		};




		/*
	class MenuRecontructionTool : public wxMenu {

			public:
				MenuRecontructionTool(wxWindow* pParent, GNKVisualizator::ReconstructionTool* pTool, GNC::GCS::IControladorPermisos* pCP) : wxMenu() {
				m_pParent = pParent;
				m_pTool = pTool;

				wxMenuItem* mode;
				int modeId = MENU_IDS_RECONSTRUCTION;

				//bool enabled = m_pTool->Habilitada();

				if (pCP != NULL && pCP->Get("atencionprimaria", "reconstruction")) {

					if ( pCP->Get("atencionprimaria.reconstruction","orthogonalmpr3d") ) {
						int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_OrthogonalMPR;
						mode = new wxMenuItem( this, modeId, _("3D MPR"));
						m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected) ,NULL, this);
						m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
						Append(mode);
					}

					if ( pCP->Get("atencionprimaria.reconstruction","mip3d") ) {
						int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_MIP3D;
						mode = new wxMenuItem( this, modeId, _("MIP 3D"));
						m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected) ,NULL, this);
						m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
						Append(mode);
					}

					if ( pCP->Get("atencionprimaria.reconstruction","volumerengering3d") ) {
						int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_VolumeRendering;
						mode = new wxMenuItem( this, modeId, _("3D volume rendering"));
						m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected) ,NULL, this);
						m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
						Append(mode);
					}

					if ( pCP->Get("atencionprimaria.reconstruction","surfacerendering3d") ) {
						int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_SurfaceRendering;
						mode = new wxMenuItem( this, modeId, _("3D surface rendering"));
						m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected) ,NULL, this);
						m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
						Append(mode);
					}


					if ( pCP->Get("atencionprimaria.reconstruction","endoscopy3d") ) {
						int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_3D_Endoscopy;
						mode = new wxMenuItem( this, modeId, _("3D endoscopy"));
						m_pParent->Connect(modeId, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected) ,NULL, this);
						m_pParent->Connect(modeId, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
						Append(mode);
					}

					m_pParent->Connect(pTool->ID, wxEVT_UPDATE_UI, wxUpdateUIEventHandler(MenuRecontructionTool::OnUpdateMenuReconstructionUI),NULL,this);
				}
			}

			~MenuRecontructionTool() {
				m_pParent->Disconnect(wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( MenuRecontructionTool::OnReconstructionModeSelected),NULL,this);
				m_pParent = NULL;
				m_pTool = NULL;
			}

			void OnReconstructionModeSelected( wxCommandEvent& event )
			{
				int modeId = event.GetId() - MENU_IDS_RECONSTRUCTION;
				m_pTool->SelectReconstructionMode(modeId);
				event.Skip(false);
			}

			void OnUpdateMenuReconstructionUI(wxUpdateUIEvent& event) {
				event.Enable(m_pTool->SupportsReconstructionMode(event.GetId() - MENU_IDS_RECONSTRUCTION));
			}

			GNKVisualizator::ReconstructionTool* m_pTool;
			wxWindow* m_pParent;
		};*/
	}
}

//----------------------------------------------------------------------

GNKVisualizator::ReconstructionTool::ReconstructionTool(GNC::GCS::IControladorPermisos* pControladorPermisos)
{
	m_Descripcion = _Std("3D/4D Reconstruction");
	m_IsMenu = true;
	m_pControladorPermisos = pControladorPermisos;
}

GNKVisualizator::ReconstructionTool::~ReconstructionTool()
{
}

bool GNKVisualizator::ReconstructionTool::AppendInMenu(wxWindow* pParent, wxMenu* pMenuParent)
{
	if (m_pControladorPermisos->Get("atencionprimaria", "reconstruction")) {
/*		wxMenu* pMenu = new GNKVisualizator::GUI::MenuRecontructionTool(pParent,this, m_pControladorPermisos);
		pMenuParent->Append(ID,wxString::FromUTF8(m_Descripcion.c_str()),pMenu);*/
		GNKVisualizator::GUI::MenuItemReconstruction* mode;
		//int modeId = MENU_IDS_RECONSTRUCTION;

		if ( m_pControladorPermisos->Get("atencionprimaria.reconstruction","orthogonalmpr3d") ) {
			int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_OrthogonalMPR;
			mode = new GNKVisualizator::GUI::MenuItemReconstruction(pParent, modeId, _("3D MPR"), pMenuParent,this, m_pControladorPermisos);
			pMenuParent->Append(mode);
		}

		if ( m_pControladorPermisos->Get("atencionprimaria.reconstruction","mip3d") ) {
			int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_MIP3D;
			mode = new GNKVisualizator::GUI::MenuItemReconstruction(pParent, modeId, _("MIP 3D"), pMenuParent,this, m_pControladorPermisos);
			pMenuParent->Append(mode);
		}

		if ( m_pControladorPermisos->Get("atencionprimaria.reconstruction","volumerengering3d") ) {
			int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_VolumeRendering;
			mode = new GNKVisualizator::GUI::MenuItemReconstruction(pParent, modeId, _("3D volume rendering"), pMenuParent,this, m_pControladorPermisos);
			pMenuParent->Append(mode);
		}

		if ( m_pControladorPermisos->Get("atencionprimaria.reconstruction","surfacerendering3d") ) {
			int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_SurfaceRendering;
			mode = new GNKVisualizator::GUI::MenuItemReconstruction(pParent, modeId, _("3D surface rendering"), pMenuParent,this, m_pControladorPermisos);
			pMenuParent->Append(mode);
		}


		if ( m_pControladorPermisos->Get("atencionprimaria.reconstruction","endoscopy3d") ) {
			int modeId = MENU_IDS_RECONSTRUCTION + (int)GNKVisualizator::Reconstruction::RM_3D_Endoscopy;
			mode = new GNKVisualizator::GUI::MenuItemReconstruction(pParent, modeId, _("3D endoscopy"), pMenuParent,this, m_pControladorPermisos);
			pMenuParent->Append(mode);
		}
	}
	return true;
}

//region "Realización de la interfaz IHerramienta"

void GNKVisualizator::ReconstructionTool::CrearPaneles( wxPanel* /*panel*/ )
{
	m_pAbstractPanelHerramientaOpciones =  NULL;
}

bool GNKVisualizator::ReconstructionTool::SupportsReconstructionMode(int mode)
{

	if(Habilitada())
	{
		// TODO: More and better checks required (Ex: modality, etc...)
		bool supported = true;
		for (TReconstructionContratable::IteradorListaContratos it1 = TReconstructionContratable::m_pListaActiva->begin(); it1 != TReconstructionContratable::m_pListaActiva->end(); it1++) {
			TReconstructionContract* pCR = (*it1);
			if(pCR->Estudio->Viewer != NULL) {

				int nc = pCR->Estudio->Viewer->GetNumberOfComponents();

				supported = ( supported && (nc == 1) );
			}
			else {
				supported = false;
			}
		}
		return supported;
	} else {
		return false;
	}
}

void GNKVisualizator::ReconstructionTool::SelectReconstructionMode(int mode)
{
	if(Habilitada())
	{
		if(mode >= 0 && mode < (int)GNKVisualizator::Reconstruction::RM_MaxMode) {
			for (TReconstructionContratable::IteradorListaContratos it1 = TReconstructionContratable::m_pListaActiva->begin(); it1 != TReconstructionContratable::m_pListaActiva->end(); it1++) {
				TReconstructionContract* pCR = (*it1);
				switch (mode) {
					case GNKVisualizator::Reconstruction::RM_OrthogonalMPR:
						pCR->SetReconstructionMode(GNKVisualizator::Reconstruction::RM_OrthogonalMPR);
						break;
					case GNKVisualizator::Reconstruction::RM_MIP3D:
						pCR->SetReconstructionMode(GNKVisualizator::Reconstruction::RM_MIP3D);
						break;
					case GNKVisualizator::Reconstruction::RM_VolumeRendering:
						pCR->SetReconstructionMode(GNKVisualizator::Reconstruction::RM_VolumeRendering);
						break;
					case GNKVisualizator::Reconstruction::RM_SurfaceRendering:
						pCR->SetReconstructionMode(GNKVisualizator::Reconstruction::RM_SurfaceRendering);
						break;
					case GNKVisualizator::Reconstruction::RM_3D_Endoscopy:
						pCR->SetReconstructionMode(GNKVisualizator::Reconstruction::RM_3D_Endoscopy);
						break;
				}
			}
		}
	}
}

void GNKVisualizator::ReconstructionTool::SolicitarActivacion()
{
}

void GNKVisualizator::ReconstructionTool::ConectarContratoFalso(bool ) {
	if (TReconstructionContratable::m_pListaActiva == NULL) {
		//std::cerr << "Error: Se trato de activar el modo deshabilitado de una vista sin haber asignado la vista activa. Error en la logica de activacion. Accion ignorada" << std::endl;
		return;
	}

}

void GNKVisualizator::ReconstructionTool::SetVistaActiva(GNC::GCS::IVista* pVista) {
	TReconstructionContratable::EstablecerVistaActiva(pVista);
}

// FIXME: Aviso!!!! para que funcione deben contratarse contratos pareados obligatorios y en el mismo orden
void GNKVisualizator::ReconstructionTool::ConectarContratos(bool )
{
	if (TReconstructionContratable::m_pListaActiva == NULL) {
		return;
	}

}

//endregion

#endif
