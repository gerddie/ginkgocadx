/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: wxVTKRenderWindowInteractor.h,v $
  Language:  C++
  Date:      $Date: 2009-09-26 18:46:34 +0200 (Sat, 26 Sep 2009) $
  Version:   $Revision: 1276 $

  Copyright (c) 1993-2002 Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

// .NAME  wxVTKRenderWindowInteractor - class to enable VTK to render to
// and interact with wxWindow.
// .SECTION Description
//  wxVTKRenderWindowInteractor provide a VTK widget for wxWindow. This class
// was completely rewrote to have the 'Look & Feel' of the python version:
// wxVTKRenderWindowInteractor.py
// .SECTION Caveats
//  - There is a know bug that prevent this class to works for more info see
// WX_USE_X_CAPTURE. This bug only affect wxGTK from 2.3.2 to wxGTK 2.4.0.
//  - Furthermore this class is tempated over either wxWindows or wxGLCanvas,
// in wxWindows 2.3.1 and earlier, the wxGLCanvas had scroll bars, you can avoid
// this effect by playing with WX_BASE_CLASS at your risk (you might end up with
// lot of flickering.)
//  - This class might not be easily readable as it tried to work with VTK 3.2
//  and 4.x. This class doesn't support reparenting with VTK 4.2 and earlier.
// .SECTION see also
// wxVTKRenderWindowInteractor.py wxVTKRenderWindow.py

#ifndef _wxVTKRenderWindowInteractor_h_
#define _wxVTKRenderWindowInteractor_h_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#include <api/api.h>
#include <memory>

#ifdef __BORLANDC__
#  pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/timer.h>
#include <wx/dcclient.h>

#ifdef __DEPRECATED
#undef __DEPRECATED
#endif
// vtk includes
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderWindow.h"

#include "vtkINRIA3DConfigure.h"

#if defined(__WXGTK__)
#if defined(wxUSE_GLCANVAS)
#    include <wx/window.h>
#    include <wx/glcanvas.h>
#  else
#    error "problem of wxGLCanvas, you need to build wxWidgets with opengl"
#endif
#endif //__WXGTK__

// Motif version (renamed into wxX11 for wxWindow 2.4 and newer)
#if defined(__WXMOTIF__)
# error This GUI is not supported by wxVTKRenderWindowInteractor for now
#endif

// wx forward declarations
class wxPaintEvent;
class wxMouseEvent;
class wxTimerEvent;
class wxKeyEvent;
class wxSizeEvent;
class wxGLContext;

#include <api/ievento.h>
#include <api/ioverlay.h>
#include <api/iwidgetsrenderer.h>

#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
class wxVTKRenderWindowInteractor : public wxGLCanvas, public vtkRenderWindowInteractor, public GNC::GCS::IWidgetsRenderer
#else
class EXTAPI wxVTKRenderWindowInteractor : public wxWindow, public vtkRenderWindowInteractor, public GNC::GCS::IWidgetsRenderer
#endif //__WXGTK__
{
        DECLARE_DYNAMIC_CLASS(wxVTKRenderWindowInteractor)

public:
        //constructors
        wxVTKRenderWindowInteractor();

        wxVTKRenderWindowInteractor(wxWindow *parent,
                                    wxWindowID id,
                                    const wxPoint &pos = wxDefaultPosition,
                                    const wxSize &size = wxDefaultSize,
                                    long style = wxWANTS_CHARS | wxNO_FULL_REPAINT_ON_RESIZE | wxCLIP_CHILDREN,
                                    const wxString &name = wxPanelNameStr);
        vtkTypeMacro(wxVTKRenderWindowInteractor,vtkRenderWindowInteractor);
        static wxVTKRenderWindowInteractor * New();
        void PrintSelf(ostream& os, vtkIndent indent);

        //destructor
        ~wxVTKRenderWindowInteractor();

        // vtkRenderWindowInteractor overrides
        void Initialize();
        void Enable();
        bool Enable(bool enable);
        void Disable();
        void Start();
        void UpdateSize(int x, int y);
        int CreateTimer(int timertype);
        int DestroyTimer();
        void TerminateApp() {};

        virtual bool Reparent( wxWindowBase *newParent );

        // event handlers
        virtual void OnPaint(wxPaintEvent &event);
        virtual void OnEraseBackground (wxEraseEvent& event);
        virtual void OnMotion(wxMouseEvent &event);

        virtual void OnButtonDown(wxMouseEvent &event);
        virtual void OnButtonUp(wxMouseEvent &event);
        virtual void OnMouseDoubleClick(wxMouseEvent &event);

#if !(VTK_MAJOR_VERSION == 3 && VTK_MINOR_VERSION == 1)
        virtual void OnEnter(wxMouseEvent &event);
        virtual void OnLeave(wxMouseEvent &event);
        virtual void OnMouseWheel(wxMouseEvent& event);
        virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& event);
        virtual void OnKeyDown(wxKeyEvent &event);
        virtual void OnKeyUp(wxKeyEvent &event);
        virtual void OnChar(wxKeyEvent &event);
#endif
        virtual void OnTimer(wxTimerEvent &event);
        virtual void OnSize(wxSizeEvent &event);

        void Render();
        void SetRenderWhenDisabled(int newValue);

        // Description:
        // Prescribe that the window be created in a stereo-capable mode. This
        // method must be called before the window is realized. Default if off.
        vtkGetMacro(Stereo,int);
        vtkBooleanMacro(Stereo,int);
        virtual void SetStereo(int capable);

        // Description:
        // As CaptureMouse could be a problem sometimes on a window box
        // This method allow to set or not the CaptureMouse.
        // This method actually will works only if WX_USE_X_CAPTURE was set to 1
        vtkSetMacro(UseCaptureMouse,int);
        vtkBooleanMacro(UseCaptureMouse,int);

#if VTK_MAJOR_VERSION > 5 || (VTK_MAJOR_VERSION == 5 && VTK_MINOR_VERSION >= 2)
protected:
        virtual int InternalCreateTimer(int timerId, int timerType, unsigned long duration);
        virtual int InternalDestroyTimer(int platformTimerId);
#endif

protected:
        wxTimer timer;
        int ActiveButton;
        long GetHandleHack();
        int Stereo;

private:
        long Handle;
        bool Created;
        int RenderWhenDisabled;
        int UseCaptureMouse;

#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
        wxGLContext *GLContext;
#endif

        //----------------------------------------------------------------------------------------------------
        //region Interfaz IWidgetsRenderer
public:
        virtual void PostRender(bool inmediato = false);

        virtual void WSetCurrent();

        virtual void OnActivar();
        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Interfaz de transformacion de coordenadas"
public:
        virtual void DisplayToWorld(double displayPos[3], double worldPos[4]);

        virtual void WorldToDisplay(double worldPos[4], double displayPos[3]);

        /** Gets camera vectors (camPos, camDir, camUp) **/
        void GetCamVectors(double camPos[4], double camDir[4], double camUp[4]);

        /** shots a ray from unproyected displayPos with camera direction and intersects with image plane **/
        void RayPick(double displayPos[3], double worldPos[4]);

        /** shots a ray from campPos with camera direction and intersects with image plane **/
        void RayPick(double worldPos[4]);

        //endregion

        //----------------------------------------------------------------------------------------------------
        //region "Helpers de contexto de dibujado"
protected:
        virtual void TransformarEvento(wxMouseEvent &event);
        virtual void TransformarEvento(wxKeyEvent &event);
        virtual void CrearContexto3D(int ancho, int alto);
        virtual void DestruirContexto3D();

        /* Proyecta worlPos sobre el viewport, crea un rectángulo de lado 1, lo reproyecta al mundo y devuelve las dimensiones del cubo envolvente de esta ultima reproyección.
        * El resultado obtenido, sirve para calcular la proporción pixels pantalla <-> coordenadas mundo.
        * Si pos es NULL, se utiliza el centro del viewport como punto generador del rectángulo.
        * El resultado queda almacenado en el contexto de dibujado (miembro relacionMundoPantalla_{x,y,z})
        */
        virtual void  CalcularRelacionPixelsPantalla(vtkRenderer* pRenderer);
        //endregion

        DECLARE_EVENT_TABLE()
};

#endif //_wxVTKRenderWindowInteractor_h_
