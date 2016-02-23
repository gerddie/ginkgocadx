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
