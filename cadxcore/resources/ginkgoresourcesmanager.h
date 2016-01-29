/*
 *  
 *  $Id: ginkgoresourcemanager.h $
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
#include <api/api.h>
#include <wx/bitmap.h>
#include <wx/animate.h>

 class EXTAPI GinkgoResourcesManager {

 	public:

 	// Adquisicion
 	class EXTAPI Acquisition {
 		public:
		static wxBitmap GetIcoAdquireFile();
		static wxBitmap GetIcoAdquirePACS();
		static wxBitmap GetIcoDownload();
		static wxBitmap GetIcoClear();
		static wxBitmap GetIcoRemoveDownload();
 	};

 	class EXTAPI AngleBar {
 		public:
		static wxBitmap GetIcoAnguloCobb();
		static wxBitmap GetIcoAnguloNormal();
 	};

 	class EXTAPI MagnifierBar {
 		public:
		static wxBitmap GetIcoZoomIn();
		static wxBitmap GetIcoZoomOut();
 	};

	class EXTAPI GridBar {
 		public:
		static wxBitmap GetIcoVerRejilla();
		static wxBitmap GetIcoOcultarRejilla();
 	};

 	class EXTAPI WindowLevelBar {
 		public:
		static wxBitmap GetIcoReset();
 	};

 	class EXTAPI MarkingBar {
 		public:
 		static wxBitmap GetIcoElipse();
 		static wxBitmap GetIcoFlecha();
 		static wxBitmap GetIcoPoligono();
		static wxBitmap GetIcoFreeForm();
 		static wxBitmap GetIcoPunto();
 		static wxBitmap GetIcoRectangulo();
 		static wxBitmap GetIcoTrapezoide();
 	};

 	class EXTAPI History {
 		public:
		static wxBitmap GetIcoEstudio();
		static wxBitmap GetIcoSerie();
		static wxBitmap GetIcoImagen();
		static wxBitmap GetIcoHistorialOpen();
		static wxBitmap GetIcoHombre();
		static wxBitmap GetIcoMujer();
		static wxBitmap GetIcoOtro();
		static wxBitmap GetIcoFicheroClave();
		static wxBitmap GetIcoPlegar();
		static wxBitmap GetIcoDesplegar();
		static wxBitmap GetMoviePreview();
		static wxBitmap GetPDFPreview();
		static wxBitmap GetIcoUnknownFile();
		static wxBitmap GetIcoCleanAll();
		static wxBitmap GetIcoAllPatients();
		static wxBitmap GetIcoLocalFile();
		static wxBitmap GetIcoLocalLinked();
		static wxBitmap GetIcoWadoLinked();
		static wxBitmap GetIcoLocalFileRecent();
		static wxBitmap GetIcoLocalLinkedRecent();
		static wxBitmap GetIcoWadoLinkedRecent();
		static wxBitmap GetIcoWadoLinkedPreview();
 	};

 	class EXTAPI ToolIcons {
 		public:
 		static wxBitmap GetIcoAngulo();
 		static wxBitmap GetIcoAnotacion();
 		static wxBitmap GetIcoCruz();
 		static wxBitmap GetIcoElevacion();
 		static wxBitmap GetIcoFlecha();
 		static wxBitmap GetIcoLongitud();
 		static wxBitmap GetIcoVarita();
		static wxBitmap GetIcoLazoChroma();
 		static wxBitmap GetIcoWindowLevel();
 		static wxBitmap GetIcoZoom();
		static wxBitmap GetIcoSinOpciones();
		static wxBitmap GetIcoDebug();
		static wxBitmap GetIcoMarcado();
		static wxBitmap GetIcoRejilla();
		static wxBitmap GetIcoMostrarOcultarTags();
		static wxBitmap GetIcoDesencajar();
		static wxBitmap GetIcoCornerAnnotations();
		static wxBitmap GetIcoInforme();
		static wxBitmap GetIcoSlider();
		static wxBitmap GetIcoLupa();
		static wxBitmap GetIcoPan();
		static wxBitmap GetIcoMouseR();
		static wxBitmap GetIcoMouseL();
		static wxBitmap GetIcoMouseRL();
		static wxBitmap GetIcoAnonymize();
		static wxBitmap GetIcoLayout();
		static wxBitmap GetIcoFlipVertical();
		static wxBitmap GetIcoFlipHorizontal();		
		static wxBitmap GetIcoRotateLeft();
		static wxBitmap GetIcoRotateRight();
		static wxBitmap GetIcoInvert();
		static wxBitmap GetIcoTrackball();
		static wxBitmap GetIcoDownloadCloud();
		static wxBitmap GetIcoSearchCloud();
		static wxBitmap GetIcoCopyToClipboard();
		static wxBitmap GetIcoBrightnessContrast();
	};

 	class EXTAPI MenuIcons {
 		public:
 		static wxBitmap GetIcoAbrir();
		static wxBitmap GetIcoOpenDir();
		static wxBitmap GetIcoRemovableUnit();
		static wxBitmap GetIcoOpenFile();
 		static wxBitmap GetIcoCerrarTab();
 		static wxBitmap GetIcoConfiguracion();
 		static wxBitmap GetIcoEditor();
 		static wxBitmap GetIcoExportar();
 		static wxBitmap GetIcoFiltrado();
 		static wxBitmap GetIcoGuardar();
 		static wxBitmap GetIcoImportar();
		static wxBitmap GetIcoDicomDir();
 		static wxBitmap GetIcoReset();
 		static wxBitmap GetIcoSalir();
		static wxBitmap GetIcoImpresora();
		static wxBitmap GetIcoVistaPreliminar();
		static wxBitmap GetIcoCalibrarImagen();
		static wxBitmap GetIcoEliminar();
		static wxBitmap GetIcoEliminarTodo();
		static wxBitmap GetIcoEditar();
		static wxBitmap GetIcoSendToPACS();
		static wxBitmap GetIcoEliminarWidget();
		static wxBitmap GetIcoPantallaCompleta();
		static wxBitmap GetIcoUser();
		static wxBitmap GetIcoHelp();
		static wxBitmap GetIcoSupport();
		static wxBitmap GetIcoObtainExtensions();
		static wxBitmap GetIcoTick();
		static wxBitmap GetIcoCross();
		static wxBitmap GetIcoPaste();
 	};

 	class EXTAPI Logos {
 		public:
 		static wxBitmap GetLogoGinkgo48x48();
		static wxBitmap GetLogoGinkgo16x16();
		static wxBitmap GetLogoGinkgo32x32();
		static wxBitmap GetLogoSplash();
		static wxBitmap GetExtensionsLogo();
		static wxBitmap GetExtensionsErrorLogo();
 	};

	class EXTAPI BigIcons {
 		public:
 		static wxBitmap GetIcoBigBook();
		static wxBitmap GetIcoBigDicomize();
		static wxBitmap GetIcoBigQR();
		static wxBitmap GetIcoBigAcquire();
		static wxBitmap GetIcoBigGinkgoCloudSearch();
		static wxBitmap GetIcoDefaultDicomizer();
	};

	class EXTAPI Cursors {
		public:
		static wxImage GetCursorTransparente();
		static wxImage GetCursorLupa();
		static wxImage GetCursorCrearLinea();
		static wxImage GetCursorManoAbierta();
		static wxImage GetCursorManoCerrada();
		static wxImage GetCursorSeleccionar();
		static wxImage GetCursorCrearAnguloCobb();
		static wxImage GetCursorCrearAnguloNormal();
		static wxImage GetCursorCrearAnotacion();
		static wxImage GetCursorCrearElipse();
		static wxImage GetCursorCrearPoligono();
		static wxImage GetCursorCrearPunto();
		static wxImage GetCursorCrearRectangulo();
		static wxImage GetCursorWindowLevel();
		static wxImage GetCursorAnclaSimple();
		static wxImage GetCursorAnclaCompuesta();
		static wxImage GetRotationCursor();
	};

	class EXTAPI ProgressBar {
 		public:
		static wxAnimation GetAniLoading();
		static wxBitmap GetIcoStopped();
 	};

	class EXTAPI Wizards {
	public:
		static wxBitmap GetIcoBarraSuperior();
	};

	class EXTAPI LayoutBar {
	public:
		static wxBitmap GetIcoAddColumn();
		static wxBitmap GetIcoAddRow();
		static wxBitmap GetIcoDelColumn();
		static wxBitmap GetIcoDelRow();
		static wxBitmap GetIcoLayout1x1();
		static wxBitmap GetIcoLayout1x2();
		static wxBitmap GetIcoLayout2x1();
		static wxBitmap GetIcoLayout2x2();
		static wxBitmap GetIcoLayout3x3();
		static wxBitmap GetIcoLayout4x4();
		static wxBitmap GetIcoLayout1x2Blue();
		static wxBitmap GetIcoLayout2x1Blue();
	};

	class EXTAPI MenuTabs {
	public:
		static wxBitmap GetIcoCerrarTab();
		static wxBitmap GetIcoPrimerPlano();
		static wxBitmap GetIcoGrid2Col();
		static wxBitmap GetIcoGrid3Col();
		static wxBitmap GetIcoReagrupar();
	};

	class EXTAPI CineBar {
	public:
		static wxBitmap GetIcoPlay();
		static wxBitmap GetIcoBucle();
		static wxBitmap GetIcoNoBucle();
		static wxBitmap GetIcoStop();		
		static wxBitmap GetIcoPause();	
		static wxBitmap GetIcoCine();		
	};

 };
