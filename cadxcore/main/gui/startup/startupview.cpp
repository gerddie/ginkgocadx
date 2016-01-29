/*
 *  
 *  $Id: startupview.cpp $
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
//#define _GINKGO_TRACE

#include <wx/wx.h>
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/file.h>
#include <wx/busyinfo.h>
#include <wx/msgdlg.h>
#include <wx/thread.h>

#include <wx/tarstrm.h>
#include <wx/zstream.h>
#include <wx/wfstream.h>

#include <api/globals.h>
#include <api/internationalization/internationalization.h>
#include <api/controllers/icontroladorvistas.h>
#include <main/entorno.h>
#include <main/controllers/controladorlog.h>
#include <main/controllers/configurationcontroller.h>
#include <main/controllers/controladoreventos.h>
#include <eventos/views.h>
#include <main/controllers/controladorinternacionalizacion.h>
#include <export/tools/iherramientascore.h>
#include <api/toolsystem/itoolcontroller.h>

#include "startupview.h"
#include "startupform.h"

#include <api/ientorno.h>

#undef __DEPRECATED
#include <ios>
#include <sstream>
#include <ostream>


#define LOGGER "STARTUP"
#define DEFAULT_RSS "http://twitter.com/statuses/user_timeline/247267254.rss"
#define DEFAULT_WELCOME_URL "http://ginkgo-cadx.com/[lan]/ginkgocadxwelcomescreen/"

namespace GNC {
	namespace GUI {
		StartUpView ::StartUpView (const GNC::GCS::Ptr<GNC::GUI::StartUpStudy>& study) : GNC::GCS::IVista((GNC::GCS::Ptr<GNC::GCS::IStudyContext>)study)
		{
			FooStudy = study;
			m_Cargada = false;

			FooStudy->Window = m_pWindow = new StartUpForm (this);
			
			GNC::GCS::IToolController* pToolBar = this->GetToolController();
			pToolBar->AddTool(HCore_ShowHistory, (GNC::GCS::IViewContract*) this);

			pToolBar->AddTool(HCore_DicomizeFromView, (GNC::GCS::IViewContract*) this);
			pToolBar->AddTool(HCore_AddFileToHistory, (GNC::GCS::IViewContract*) this);
			pToolBar->AddTool(HCore_QueryRetrieve, (GNC::GCS::IViewContract*) this);
			
			GNC::GCS::ControladorEventos::Instance()->ProcesarEvento(new GNC::GCS::Events::EventView(this, GNC::GCS::Events::EventView::Initialized));
		}

		StartUpView ::~StartUpView ()
		{
		}

		//----------------------------------------------------------------------------------------------------
	//region Loading interface

		// Starts loading. Synchronous with interface.
		void StartUpView::OnCargaIniciada()
		{
		}

		// Step1: LoadStudy not synchronized with interface thread
		void StartUpView::CargarEstudio(GNC::GCS::IComando* /*pCmdInvocador*/)
		{				
		}

		//Ends loading. Synchronous with interface
		void StartUpView::OnCargaFinalizada()
		{
		}

		// Step2: init pipeline synchronous with interface
		void StartUpView::IniciarPipeline()
		{
			//build view title
			BuildTitle();
		}


		// stops pipeline, it's called if there has been an error loading study
		void StartUpView::DetenerPipeline()
		{
		}

	//endregion

		bool StartUpView::SoportaGuardar()
		{
			return false;
		}

		bool StartUpView::SoportaExportar()
		{
			return false;
		}

		GNC::GCS::IVista* StartUpView::GetView()
		{
			return this;
		}

		void StartUpView::Activar() {
			GNC::GCS::IVista::Activar();
			m_pWindow->SetFocus();
		}

		wxWindow * StartUpView::GetWindow()
		{
			return m_pWindow;
		}

		wxSizer* StartUpView::GetBottomSizer()
		{
			return m_pWindow->m_pSizerCenter;
		}


		void StartUpView::BuildTitle()
		{
			m_Titulo = _Std("Start page");
		}
	}
}
