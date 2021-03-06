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

#include "previewpanelbase.h"
#include "previewpanelmodel.h"
#include "ipreviewpanel.h"
#include <main/controllers/thumbnailcontroller.h>

class wxColour;
namespace GNC
{
namespace GCS
{
class IVista;
class IModuleController;
}
namespace GUI
{
class NotificadorPreview;
}
}

namespace GNC
{
namespace GUI
{
class LeafTimer;

class LeafPanel: public LeafPanelBase, public ISeleccionablePreview, public INodoHistorial, GADAPI::ThumbnailsNotifier
{
public:
        LeafPanel(GNC::GCS::IVista* pView, INodoHistorial* pPadre,  GNC::GUI::IPreviewLeaf* pLeaf, bool rowEven);
        ~LeafPanel();

        void Seleccionar(bool seleccionar,bool force);
        wxWindow* GetWxWindow();
        void SetImage(long file_pk, wxImage* pImage);
        virtual void GetNodeSize(int &x, int &y);


protected:
        virtual void OnChar(wxKeyEvent &event);
        virtual void OnLeafClick(wxMouseEvent& event);
        virtual void OnLeafDClick(wxMouseEvent& event);
        virtual void OnMouseWheel(wxMouseEvent& event);
        virtual void OnMenuMouse(wxMouseEvent& event);
        void OnPaint(wxPaintEvent &event);
        virtual void OnEraseBackground( wxEraseEvent& event );

        bool                    m_modificado;
        GNC::GUI::IPreviewLeaf* m_pLeaf;
        bool					m_dibujada;
        GNC::GCS::IVista*		m_pView;
        bool m_rowEven;
};

//void SetModeloIntegracion (GNC::GCS::Ptr<GIL::IModeloIntegracion> pModeloIntegracion);

//	std::list<GNC::GCS::IVista*> m_pVistas;

//	void InitFields(const std::string& descripcion, const std::string& fecha,
//		const std::string& hora, const std::string& id_importador, const std::string& id_modalidad,
//		const std::string& uidEstudio, const std::string uidTransferSyntax);
//	void Seleccionar(bool seleccionar,bool force=false);
//	void SetAbierto(bool abierto);
//	void SetModificado(bool modificado);
//	void UpdateStatus();
//	void SetImage(wxImage* pImage);
//	void GenerarThumbnail();
//	void OnTimerParpadeo();

//	//eventos
//	virtual void OnMouseAbrir(wxMouseEvent& event);
//	virtual void OnMouseSeleccionar(wxMouseEvent &event);
//	virtual void OnMenuMouse(wxMouseEvent &event);
//	virtual void OnKeyDown(wxKeyEvent &event);
//	virtual void OnMouseWheel( wxMouseEvent& event );
//	//

//	int RefrescarNumeroSlices();
//	void IncrementarNumeroSlices();
////	bool InsertarDCM(const GNC::GCS::ControladorHistorial::ModeloDCM& modeloDCM);

//	//movidas de estudio para ver si esta abierto y tal
//	virtual void AddVista(GNC::GCS::IVista* pVista);
//	virtual void VistaModificada(GNC::GCS::IVista* pVista);
//	virtual void VistaGuardada(GNC::GCS::IVista* pVista);
//	virtual void RemoveVista(GNC::GCS::IVista* pVista);
//	//
//	void OnAbrir(wxCommandEvent& event);
//	void Abrir(bool forzarEstudio = false);
//	void OnAbrirCon(wxCommandEvent& event);
//	void AbrirDICOM(GNC::GCS::IModuleController* pModulo, int idModo, bool forzarEstudio);
//	void OnEliminar(wxCommandEvent &event);
//	bool DoDelete(bool ask);
//	void OnLimpiarHistorial(wxCommandEvent &event);
//	void OnUploadPACS(wxCommandEvent &event);
//	void OnReprocessIntegration(wxCommandEvent &event);

//	void Detach(wxWindow* pHijo);
//	void Eliminar();
//	void OnPaint(wxPaintEvent &event);
//
//	std::string GetModalidad();

//protected:

//	bool m_modificado;
//	//en una serie todo deberia importarse con el mismo importador, y todo deberia ser de la misma modalidad...
//	GNC::GCS::Ptr<GIL::IModeloIntegracion> m_pModeloIntegracion;
//	int					  m_numeroSlices;
//	std::string			  m_uidEstudio;
//	std::string         m_idImportador;
//	std::string         m_idModalidad;
//	std::string			  m_uidTransferSyntax;
//	NotificadorPreview* m_pNotificadorPreview;
//	TimerPanelSerie*    m_pTimer;
//	wxWindow*           m_pBWin;
//	bool					  m_dibujada;
}
}
