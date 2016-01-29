/*
 *  
 *  $Id: initwx.h $
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
#include <wx/app.h>
#include <wx/bitmap.h>
#include <wx/snglinst.h>

namespace GNC {
	namespace GCS {
		namespace Events {
			class IEvento;
		}
	}
}

class wxSingleInstanceChecker;
class Entorno;
class stServer;

// Colores de la ventana del logo
#define SPLASH_COLOR_RELLENO "#4400FF" // Azul
#define SPLASH_COLOR_BORDE   "#d4d4d4" // Gris
#define SPLASH_COLOR_TEXTO   "#d4d4d4" // Gris

class wxSplashScreen;
class VentanaPrincipal;
class GinkgoEventsDelegate;

class EXTAPI Ginkgo : public wxApp {
public:
	wxSplashScreen* GetLogoWindow();

	virtual bool OnInit();
	virtual int OnExit();

#ifdef __WXMAC__
	virtual void MacOpenFile(const wxString &fileName);
	std::string m_OpenFileName;
	bool m_Inicializado;
#endif

	virtual void DoProgress(wxString str);


	void OnSplashClose(wxCloseEvent& event);

	void WriteHeaderLog();

//region "Carga de Extensiones"
	void LoadPlugins();
//endregion


private:


protected:
	wxSplashScreen*			m_pVentanaLogo;
	wxBitmap                m_pBitmapOriginal;
	VentanaPrincipal*       m_pVentanaPrincipal;
	wxSingleInstanceChecker m_singleInstanceChecker;
	stServer*               m_pServer;
	GinkgoEventsDelegate*	m_pEventObserverDelegate;

public:

	wxCriticalSection m_SplashCriticalSection;
};

#if defined(_WIN32)
int WINAPI InitWX(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
#else
int InitWX(int argc, char** argv);
#endif
