/*=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: wxVTKRenderWindowInteractor.cxx,v $
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

#include <assert.h>

#include "wxVTKRenderWindowInteractor.h"

#include <api/math/geometry3d.h>
#include <api/icontexto.h>
#include <api/ientorno.h>

#include <api/controllers/icontroladorlog.h>

//This is needed for vtk 3.1 :
#ifndef VTK_MAJOR_VERSION
#  include "vtkVersion.h"
#endif

#include "vtkDebugLeaks.h"
#include "vtkCommand.h"

#ifdef __WXMAC__
#ifdef __WXCOCOA__
#include "vtkCocoaRenderWindow.h"
#else
#include "vtkCarbonRenderWindow.h"
#endif
#endif

#include <vtk/vtkginkgoimageviewer.h>
#include <vtkImageActor.h>
#include <vtkWorldPointPicker.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkInteractorObserver.h>


// function to get VTK keysyms from ascii characters
static const char* ascii_to_key_sym(int);
// function to get VTK keysyms from Qt keys
static const char* wx_key_to_key_sym(int);

class Timer
{
private:
#if defined(_WINDOWS)
        LARGE_INTEGER mInicio;
        LARGE_INTEGER mFin;
        LARGE_INTEGER mFrecuencia;
#else

#endif
public:
        Timer()
        {
        }

        void start()
        {
#if defined(_WINDOWS)
                QueryPerformanceCounter( &mInicio );
#else

#endif
        }

        void restart()
        {
#if defined(_WINDOWS)
                QueryPerformanceCounter( &mInicio );
#else

#endif
        }

        void stop()
        {
#if defined(_WINDOWS)
                QueryPerformanceCounter(&mFin);
                QueryPerformanceFrequency(&mFrecuencia);
#else

#endif
        }
#ifdef _WINDOWS
#define UNUSED
#else
#define UNUSED __attribute__((unused))
#endif

        friend std::ostream& operator<<(std::ostream& out, const Timer& t UNUSED )
        {
#if defined(_WINDOWS)
                const double tiempo = (double)(t.mFin.QuadPart - t.mInicio.QuadPart ) / (double) t.mFrecuencia.QuadPart;
                return out << tiempo << " seg. " << 1.0 / tiempo << " Op./seg.";
#else
                return out << "No implementado";

#endif
        }

        friend std::ostream& operator<<(std::ostream& out, const Timer* t)
        {
                if (t == NULL) {
                        return out << "NULL";
                } else {
                        return out << *t;
                }
        }
};

// To access objc calls on cocoa
#ifdef __WXCOCOA__
#ifdef VTK_USE_COCOA
#import <Cocoa/Cocoa.h>
// This trick is no longer need in VTK CVS, should get rid of that:
#define id Id
#else
#error Build mismatch you need both wxWidgets and VTK to be configure against Cocoa to work
#endif //VTK_USE_COCOA
#endif //__WXCOCOA__

#ifdef __WXGTK__
#include <gdk/gdkx.h>
#include "gdk/gdkprivate.h"
#include <gtk/gtk.h>
#endif

#ifdef __WXX11__
#include "wx/x11/privx.h"
#define GetXWindow(wxwin)   ((Window)(wxwin)->GetHandle())
#endif


//For more info on this class please go to:
//http://wxvtk.sf.net
//This hack is for some buggy wxGTK version:
#define WX_USE_X_CAPTURE 1

#define ID_wxVTKRenderWindowInteractor_TIMER 1001

#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
IMPLEMENT_DYNAMIC_CLASS(wxVTKRenderWindowInteractor, wxGLCanvas)
#else
IMPLEMENT_DYNAMIC_CLASS(wxVTKRenderWindowInteractor, wxWindow)
#endif  //__WXGTK__

//---------------------------------------------------------------------------
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
BEGIN_EVENT_TABLE(wxVTKRenderWindowInteractor, wxGLCanvas)
#else
        BEGIN_EVENT_TABLE(wxVTKRenderWindowInteractor, wxWindow)
#endif //__WXGTK__
        //refresh window by doing a Render
        EVT_PAINT       (wxVTKRenderWindowInteractor::OnPaint)
        EVT_ERASE_BACKGROUND(wxVTKRenderWindowInteractor::OnEraseBackground)
        EVT_MOTION      (wxVTKRenderWindowInteractor::OnMotion)

        //Bind the events to the event converters
        EVT_LEFT_DCLICK (wxVTKRenderWindowInteractor::OnMouseDoubleClick)
        EVT_MIDDLE_DCLICK (wxVTKRenderWindowInteractor::OnMouseDoubleClick)
        EVT_RIGHT_DCLICK (wxVTKRenderWindowInteractor::OnMouseDoubleClick)
        EVT_LEFT_DOWN   (wxVTKRenderWindowInteractor::OnButtonDown)
        EVT_MIDDLE_DOWN (wxVTKRenderWindowInteractor::OnButtonDown)
        EVT_RIGHT_DOWN  (wxVTKRenderWindowInteractor::OnButtonDown)
        EVT_LEFT_UP     (wxVTKRenderWindowInteractor::OnButtonUp)
        EVT_MIDDLE_UP   (wxVTKRenderWindowInteractor::OnButtonUp)
        EVT_RIGHT_UP    (wxVTKRenderWindowInteractor::OnButtonUp)
        EVT_ENTER_WINDOW(wxVTKRenderWindowInteractor::OnEnter)
        EVT_LEAVE_WINDOW(wxVTKRenderWindowInteractor::OnLeave)
        EVT_MOUSEWHEEL  (wxVTKRenderWindowInteractor::OnMouseWheel)
        EVT_MOUSE_CAPTURE_LOST(wxVTKRenderWindowInteractor::OnMouseCaptureLost)
        EVT_KEY_DOWN    (wxVTKRenderWindowInteractor::OnKeyDown)
        EVT_KEY_UP      (wxVTKRenderWindowInteractor::OnKeyUp)
        EVT_CHAR        (wxVTKRenderWindowInteractor::OnChar)
        EVT_TIMER       (ID_wxVTKRenderWindowInteractor_TIMER, wxVTKRenderWindowInteractor::OnTimer)
        EVT_SIZE        (wxVTKRenderWindowInteractor::OnSize)
END_EVENT_TABLE()

//vtkCxxMacro(wxVTKRenderWindowInteractor, "$Revision: 1276 $")
vtkInstantiatorNewMacro(wxVTKRenderWindowInteractor)

#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
static int wxvtk_attributes[]= {
        WX_GL_DOUBLEBUFFER,
        WX_GL_RGBA,
        WX_GL_DEPTH_SIZE,
        16,
        0
};
#endif

//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor::wxVTKRenderWindowInteractor()
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
        : wxGLCanvas(0, -1, wxvtk_attributes, wxDefaultPosition, wxDefaultSize, 0, wxT("wxVTKRenderWindowInteractor")),
          vtkRenderWindowInteractor()
#else
        : wxWindow(), vtkRenderWindowInteractor()
#endif //__WXGTK__
          , timer(this, ID_wxVTKRenderWindowInteractor_TIMER)
          , ActiveButton(wxEVT_NULL)
          , Stereo(0)
          , Handle(0)
          , Created(true)
          , RenderWhenDisabled(1)
          , UseCaptureMouse(1)
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
          , GLContext(new wxGLContext (this))
#endif
{
        vtkDebugLeaks::ConstructClass("wxVTKRenderWindowInteractor");
#if defined(__WXMSW__) || defined(__WXMAC__)
        this->SetUseCaptureMouse(0);
#endif
        this->RenderWindow = NULL;
        this->SetRenderWindow(vtkRenderWindow::New());
        this->RenderWindow->Delete();
        this->m_pPicker = vtkWorldPointPicker::New();
        this->m_c = new TContexto();
        this->m_c->Reset();
        this->m_c->pWindow = this;
        this->m_c->pRenderer = this;
}

//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor::wxVTKRenderWindowInteractor(wxWindow *parent, wxWindowID id,
                const wxPoint &pos,
                const wxSize &size,
                long style, const wxString &name)
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
        : wxGLCanvas(parent, id, wxvtk_attributes, pos, size, style, name),
          vtkRenderWindowInteractor(),
#else
        :
        wxWindow(parent, id, pos, size, style, name), vtkRenderWindowInteractor(),
#endif //__WXGTK__
          timer(this, ID_wxVTKRenderWindowInteractor_TIMER),
          ActiveButton(wxEVT_NULL),
          Stereo(0),
          Handle(0),
          Created(true),
          RenderWhenDisabled(1),
          UseCaptureMouse(0)
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
          , GLContext(new wxGLContext (this))
#endif
{
        vtkDebugLeaks::ConstructClass("wxVTKRenderWindowInteractor");
#if defined(__WXMSW__) || defined(__WXMAC__)
        this->SetUseCaptureMouse(1);
#endif
        this->RenderWindow = NULL;
        this->SetRenderWindow(vtkRenderWindow::New());
        this->RenderWindow->Delete();
        this->m_c = new TContexto();
        this->m_c->Reset();
#ifdef __WXMAC__
        // On Mac (Carbon) we don't get notified of the initial window size with an EVT_SIZE event,
        // so we update the size information of the interactor/renderwindow here
        this->UpdateSize(size.x, size.y);
#endif
        this->m_pPicker = vtkWorldPointPicker::New();

        this->m_c->pWindow = this;
        this->m_c->pRenderer = this;
}
//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor::~wxVTKRenderWindowInteractor()
{
        DestruirContexto3D();
        if (this->m_pPicker != NULL) {
                this->m_pPicker->Delete();
                this->m_pPicker = NULL;
        }

        delete m_c;

        SetRenderWindow(NULL);
        SetInteractorStyle(NULL);

        // this is a leak, but for some reason, deleting the context here
        // results in a segfault. My bet is that vtkRenderWindowInteractor
        // does still hold a resource that makes X11[xcb] fail on an unknown
        // sequence number, but since wxVTKRenderWindowInteractor is derived
        // from vtkRenderWindowInteractor the tear-down can not be done in
        // the right order.
        // delete GLContext;
}
//---------------------------------------------------------------------------
wxVTKRenderWindowInteractor * wxVTKRenderWindowInteractor::New()
{
        // we don't make use of the objectfactory, because we're not registered
        return new wxVTKRenderWindowInteractor;
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Initialize()
{
        int *size = RenderWindow->GetSize();
        // enable everything and start rendering
        Enable();
        //RenderWindow->Start();

        // set the size in the render window interactor
        Size[0] = size[0];
        Size[1] = size[1];

        // this is initialized
        Initialized = 1;

        if (Size[0] > 0 && Size[1] > 0) {
                CrearContexto3D(Size[0],Size[1]);
        }
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Enable()
{
        // if already enabled then done
        if (Enabled)
                return;

        // that's it
        Enabled = 1;
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
        SetCurrent( *GLContext );
#endif
        Modified();
}
//---------------------------------------------------------------------------
bool wxVTKRenderWindowInteractor::Enable(bool enable)
{
#if defined(__WXGTK__) && defined(wxUSE_GLCANVAS)
        return wxGLCanvas::Enable(enable);
#else
        return wxWindow::Enable(enable);
#endif
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Disable()
{
        // if already disabled then done
        if (!Enabled)
                return;

        // that's it (we can't remove the event handler like it should be...)
        Enabled = 0;
        Modified();
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Start()
{
        // the interactor cannot control the event loop
        vtkErrorMacro( << "wxVTKRenderWindowInteractor::Start() "
                       "interactor cannot control event loop.");
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::UpdateSize(int x, int y)
{
        if( RenderWindow ) {
                // if the size changed tell render window
                if ( x != Size[0] || y != Size[1] ) {
                        // adjust our (vtkRenderWindowInteractor size)
                        Size[0] = x;
                        Size[1] = y;
                        // and our RenderWindow's size
                        RenderWindow->SetSize(x, y);
                }
                if (x > 0 && y > 0) {
                        CrearContexto3D(x,y);
                } else {
                        DestruirContexto3D();
                }
                if (x > 0 && y > 0) {
                        CalcularRelacionPixelsPantalla(FindPokedRenderer(0,0));
                }
        }
}
//---------------------------------------------------------------------------
int wxVTKRenderWindowInteractor::CreateTimer(int WXUNUSED(timertype))
{
        // it's a one shot timer
        if (!timer.Start(10, TRUE))
                return 0;

        return 1;

}
//------------------------------------------------------------------
int wxVTKRenderWindowInteractor::InternalCreateTimer(int /*timerId*/, int timerType,
                unsigned long duration)
{
        if (!timer.Start(duration, timerType == OneShotTimer))
                return 0;

        return ID_wxVTKRenderWindowInteractor_TIMER;
}
//------------------------------------------------------------------
int wxVTKRenderWindowInteractor::InternalDestroyTimer(int /*platformTimerId*/)
{
        timer.Stop();
        return 1;
}
//---------------------------------------------------------------------------
int wxVTKRenderWindowInteractor::DestroyTimer()
{
        // do nothing
        return 1;
}

bool wxVTKRenderWindowInteractor::Reparent( wxWindowBase *newParent )
{
        // should not reparent to NULL , because then it is turned into a toplevel windows
        // which it is not
        assert(newParent);


        LOG_DEBUG("Core/GL", "Reparenting to " << (void*) newParent << std::endl);

        return wxWindowBase::Reparent(newParent);
}

//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnTimer(wxTimerEvent& WXUNUSED(event))
{
        if (!Enabled)
                return;
        // pass the right timer id
        int timerId = this->GetCurrentTimerId();
        this->InvokeEvent(vtkCommand::TimerEvent, &timerId);
}

//---------------------------------------------------------------------------
// NOTE on implementation:
// Bad luck you ended up in the only tricky place of this code.
// A few note, wxWidgets still refuse to provide such convenient method
// so I have to maintain it myself, eventhough this is completely integrated
// in wxPython...
// Anyway if this happen to break for you then compare to a recent version of wxPython
// and look for the function long wxPyGetWinHandle(wxWindow* win)
// in wxPython/src/helpers.cpp
long wxVTKRenderWindowInteractor::GetHandleHack()
{
        // __WXMSW__ is for Win32
        //__WXMAC__ stands for using Carbon C-headers, using either the CarbonLib/CFM or the native Mach-O builds (which then also use the latest features available)
        // __WXGTK__ is for both gtk 1.2.x and gtk 2.x
#if defined(__WXMSW__) || defined(__WXMAC__)
        long handle_tmp = 0;
        handle_tmp = (long)this->GetHandle();
#endif //__WXMSW__

        //__WXCOCOA__ stands for using the objective-c Cocoa API
#ifdef __WXCOCOA__
        // Here is how to find the NSWindow
        wxTopLevelWindow* toplevel = dynamic_cast<wxTopLevelWindow*>(
                                             wxGetTopLevelParent( this ) );
        if (toplevel != NULL ) {
                handle_tmp = (long)toplevel->GetNSWindow();
        }
        // The NSView will be deducted from
        // [(NSWindow*)Handle contentView]
        // if only I knew how to write that in c++
#endif //__WXCOCOA__

        // Find and return the actual X-Window.
#if defined(__WXGTK__) || defined(__WXX11__)
        return this->GetXWindow();
#else
        return handle_tmp;
#endif

}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnPaint(wxPaintEvent& WXUNUSED(event))
{
        //must always be here
        wxPaintDC pDC(this);
        //do it here rather than in the cstor: this is safer.
        if(!Handle) {
                Handle = GetHandleHack();
                RenderWindow->SetWindowId(reinterpret_cast<void *>(Handle));
                // Cocoa
                // this->GetNSView() <-> DisplayId
                // this->GetTopLevel()->GetNSWindow() <-> WindowId
#ifdef __WXMSW__
                RenderWindow->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHWND()));
#endif //__WXMSW__

                // This is another hack to prevent the VTK Render Window from closing the display.
                // If VTK closes the display, ~wxContext chashes while trying to destroy its
                // glContext (because the display is closed). The Get -> Set makes this VTK
                // object think someone else is responsible for the display.
                this->RenderWindow->SetDisplayId(this->RenderWindow->GetGenericDisplayId());
        }
        // get vtk to render to the wxWindows
        WSetCurrent();
        Render();
#ifdef __WXMAC__
        // This solves a problem with repainting after a window resize
        // See also: http://sourceforge.net/mailarchive/forum.php?thread_id=31690967&forum_id=41789
#ifdef __WXCOCOA__
        vtkCocoaRenderWindow * rwin = vtkCocoaRenderWindow::SafeDownCast(RenderWindow);
        if( rwin ) {
                rwin->UpdateContext();
        }
#else
        vtkCarbonRenderWindow* rwin = vtkCarbonRenderWindow::SafeDownCast(RenderWindow);
        if( rwin ) {
                rwin->UpdateGLRegion();
        }
#endif
#endif
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnEraseBackground(wxEraseEvent &event)
{
#if defined (_WINDOWS)
        //turn off background erase to reduce flickering on MSW
        //event.Skip(true);
        //std::cout << "wxVTKRenderWindowInteractor::OnEraseBackground()" << std::endl;
        event.Skip(false);
        /*
        event.GetDC()->SetBrush(wxBrush(wxColor(0,0,0)));
        event.GetDC()->DrawRectangle(this->GetClientRect());
        */
#else
        event.Skip(false);
#endif
}
//---------------------------------------------------------------------------

void wxVTKRenderWindowInteractor::OnSize(wxSizeEvent& /*event*/)
{

        int w, h;
        GetClientSize(&w, &h);

#if defined(__WXGTK__)
        GtkWidget *widget = GetHandle();
        if (widget != NULL) {
                GtkWidget* parent_widget = GetParent()->GetHandle();
                if (parent_widget != NULL) {
                        gtk_widget_realize(parent_widget);
                        Window parent_wid = GDK_WINDOW_XWINDOW( parent_widget->window );
                        RenderWindow->SetParentId(reinterpret_cast<void *>(parent_wid));
                } else {
                        RenderWindow->SetParentId(NULL);
                }
                gtk_widget_realize(widget);
                Window wid = GDK_WINDOW_XWINDOW( widget->window );
                RenderWindow->SetWindowId(reinterpret_cast<void *>(wid));

                SetCurrent(*GLContext);
                UpdateSize(w, h);
                return;
        } else {
                return;
        }
#else
        UpdateSize(w, h);
        if (!Enabled) {
                return;
        }
        InvokeEvent(vtkCommand::ConfigureEvent, NULL);
#endif
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnMotion(wxMouseEvent &event)
{
        if (!Enabled) {
                return;
        }
        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

        InvokeEvent(vtkCommand::MouseMoveEvent, NULL);
        TransformarEvento(event);
}
//---------------------------------------------------------------------------

void wxVTKRenderWindowInteractor::OnEnter(wxMouseEvent &event)
{
        if (!Enabled) {
                return;
        }

        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

        InvokeEvent(vtkCommand::EnterEvent, NULL);
        TransformarEvento(event);
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnLeave(wxMouseEvent &event)
{
        if (!Enabled) {
                return;
        }

        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

        InvokeEvent(vtkCommand::LeaveEvent, NULL);
        TransformarEvento(event);
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnKeyDown(wxKeyEvent &event)
{
        if (!Enabled) {
                return;
        }
        Activar();

        // new style
        int keycode = event.GetKeyCode();
        char key = '\0';
        if (((unsigned int)keycode) < 256) {
                // TODO: Unicode in non-Unicode mode ??
                key = (char)keycode;
        }

        const char* keysym = ascii_to_key_sym( keycode );
        if(!keysym) {
                // get virtual keys
                keysym = wx_key_to_key_sym( keycode );
        }

        if(!keysym) {
                keysym = "None";
        }

        // we don't get a valid mouse position inside the key event on every platform
        // so we retrieve the mouse position explicitly and pass it along
        wxPoint mousePos = ScreenToClient(wxGetMousePosition());
        SetEventInformationFlipY(mousePos.x, mousePos.y,
                                 event.ControlDown(), event.ShiftDown(), key, 0, keysym);
        InvokeEvent(vtkCommand::KeyPressEvent, NULL);
        TransformarEvento(event);
        if (event.GetSkipped()) {
                event.ResumePropagation(20);
        }
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnKeyUp(wxKeyEvent &event)
{
        if (!Enabled) {
                return;
        }
        Activar();

        // new style
        int keycode = event.GetKeyCode();
        char key = '\0';
        if (((unsigned int)keycode) < 256) {
                // TODO: Unicode in non-Unicode mode ??
                key = (char)keycode;
        }

        const char* keysym = ascii_to_key_sym( keycode );
        if(!keysym) {
                // get virtual keys
                keysym = wx_key_to_key_sym( keycode );
        }

        if(!keysym) {
                keysym = "None";
        }


        // we don't get a valid mouse position inside the key event on every platform
        // so we retrieve the mouse position explicitly and pass it along
        wxPoint mousePos = ScreenToClient(wxGetMousePosition());
        SetEventInformationFlipY(mousePos.x, mousePos.y,
                                 event.ControlDown(), event.ShiftDown(), key, 0, keysym);
        InvokeEvent(vtkCommand::KeyReleaseEvent, NULL);
        TransformarEvento(event);
        if (event.GetSkipped()) {
                event.ResumePropagation(20);
        }
        event.Skip();

}

//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnChar(wxKeyEvent &event)
{
        if (!Enabled) {
                return;
        }
        TransformarEvento(event);

        // new style
        int keycode = event.GetKeyCode();
        char key = '\0';
        if (((unsigned int)keycode) < 256) {
                // TODO: Unicode in non-Unicode mode ??
                key = (char)keycode;
        }

        const char* keysym = ascii_to_key_sym( keycode );
        if(!keysym) {
                // get virtual keys
                keysym = wx_key_to_key_sym( keycode );
        }

        if(!keysym) {
                keysym = "None";
        }

        // we don't get a valid mouse position inside the key event on every platform
        // so we retrieve the mouse position explicitly and pass it along
        wxPoint mousePos = ScreenToClient(wxGetMousePosition());
        SetEventInformationFlipY(mousePos.x, mousePos.y,
                                 event.ControlDown(), event.ShiftDown(), key, 0, keysym);
        InvokeEvent(vtkCommand::CharEvent, NULL);

        TransformarEvento(event);
        if (event.GetSkipped()) {
                event.ResumePropagation(20);
        }
        event.Skip();
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnButtonDown(wxMouseEvent &event)
{
        if (!Enabled || (ActiveButton != wxEVT_NULL)) {
                return;
        }
        Activar();
        ActiveButton = event.GetEventType();

        // On Mac (Carbon) and Windows we don't automatically get the focus when
        // you click inside the window
        // we therefore set the focus explicitly
        // Apparently we need that on linux (GTK) too:
        this->SetFocus();

        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

        if(event.RightDown()) {
                InvokeEvent(vtkCommand::RightButtonPressEvent, NULL);
        } else if(event.LeftDown()) {

                InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
        } else if (event.LeftDClick()) {

                InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
        }

        else if(event.MiddleDown()) {
                InvokeEvent(vtkCommand::MiddleButtonPressEvent, NULL);
        }
        //save the button and capture mouse until the button is released
        //Only if :
        //1. it is possible (WX_USE_X_CAPTURE)
        //2. user decided to.
        if ((ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && UseCaptureMouse) {
                CaptureMouse();
        }
        TransformarEvento(event);
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnButtonUp(wxMouseEvent &event)
{
        //EVT_xxx_DOWN == EVT_xxx_UP - 1
        //This is only needed if two mouse buttons are pressed at the same time.
        //In wxWindows 2.4 and later: better use of wxMOUSE_BTN_RIGHT or
        //wxEVT_COMMAND_RIGHT_CLICK
        if (!Enabled || (ActiveButton != (event.GetEventType()-1))) {
                return;
        }
        Activar();

        // See report by Shang Mu / Kerry Loux on wxVTK mailing list
        this->SetFocus();

        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);

        if(ActiveButton == wxEVT_RIGHT_DOWN) {
                InvokeEvent(vtkCommand::RightButtonReleaseEvent, NULL);
        } else if(ActiveButton == wxEVT_LEFT_DOWN) {
                InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
        } else if(ActiveButton == wxEVT_MIDDLE_DOWN) {
                InvokeEvent(vtkCommand::MiddleButtonReleaseEvent, NULL);
        }
        //if the ActiveButton is realeased, then release mouse capture
        if ((ActiveButton != wxEVT_NULL) && WX_USE_X_CAPTURE && UseCaptureMouse) {
                ReleaseMouse();
        }
        ActiveButton = wxEVT_NULL;
        TransformarEvento(event);
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnMouseWheel(wxMouseEvent& event)
{
        Activar();
        // Mouse wheel was only added after VTK 4.4 (I think...)
        // new style
        //Set vtk event information ... The numebr of wheel rotations is stored in
        //the x varible.  y varible is zero
        SetEventInformationFlipY(event.GetX() , event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 0, NULL);
        //si esta el control pulsado no se procesa, lo procesaran mas adelante
        if(!event.ControlDown()) {
                if(event.GetWheelRotation() > 0) {
                        //Send event to VTK
                        InvokeEvent(vtkCommand::MouseWheelForwardEvent, NULL);
                } else {
                        //Send event to VTK
                        InvokeEvent(vtkCommand::MouseWheelBackwardEvent, NULL);
                }
        }

        TransformarEvento(event);
}

void wxVTKRenderWindowInteractor::OnMouseCaptureLost(wxMouseCaptureLostEvent& /*event*/)
{
        if (ActiveButton != wxEVT_NULL) {
                //ReleaseMouse();
        }
        // Reset ActiveButton so that
        // 1. we do not process mouse button up events any more,
        // 2. the next button down event will be processed and call CaptureMouse().
        // Otherwise ReleaseMouse() will be called
        // without a previous CaptureMouse().
        ActiveButton = wxEVT_NULL;
}

//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::Render()
{
        //Timer t;
        //t.start();

        int renderAllowed = !IsFrozen();

        if (renderAllowed && !RenderWhenDisabled) {
                //the user doesn't want us to render when the toplevel frame
                //is disabled - first find the top level parent
                wxWindow *topParent = wxGetTopLevelParent(this);
                if (topParent) {
                        //if it exists, check whether it's enabled
                        //if it's not enabeld, renderAllowed will be false
                        renderAllowed = topParent->IsEnabled();
                }
        }

        if (renderAllowed) {
                if(Handle && (Handle == GetHandleHack()) ) {
                        CalcularRelacionPixelsPantalla(FindPokedRenderer(0,0));
                        RenderWindow->Render();
                        //t.stop();
                        //std::cout << "wxVTKRenderWindowInteractor::Render(1) " << t << std::endl;
                } else if(GetHandleHack()) {
                        //this means the user has reparented us; let's adapt to the
                        //new situation by doing the WindowRemap dance
                        //store the new situation

                        Handle = GetHandleHack();

#if defined(__WXOSX__)
                        RenderWindow->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHandle()));
                        RenderWindow->SetWindowId(reinterpret_cast<void *>(Handle));
#elif defined(__WXMSW__)
                        RenderWindow->SetParentId(reinterpret_cast<void *>(this->GetParent()->GetHWND()));
                        RenderWindow->SetWindowId(reinterpret_cast<void *>(Handle));
#endif

                        CalcularRelacionPixelsPantalla(FindPokedRenderer(0,0));
                        bool doPlot = !Initialized;

                        RenderWindow->Render();

                        if (doPlot) {
                                GNC::GCS::IControladorLog* cLog = GNC::GCS::IControladorLog::Instance();
                                if (cLog != NULL && cLog->IsEnabledFor(GNC::GCS::IControladorLog::DebugLog) ) {
                                        const char* str;
                                        LOG_DEBUG("Core/GL", "OpenGL Subsytem initiallized." << std::endl);
                                        str = (const char*) glGetString(GL_VENDOR);
                                        if (str != NULL) {
                                                LOG_DEBUG("Core/GL", "GL_VENDOR:" << str << std::endl);
                                        }
                                        str = (const char*) glGetString(GL_RENDERER);
                                        if (str != NULL) {
                                                LOG_DEBUG("Core/GL", "GL_RENDERER:" << str << std::endl);
                                        }
                                        str = (const char*) glGetString(GL_VERSION);
                                        if (str != NULL) {
                                                LOG_DEBUG("Core/GL", "GL_VERSION:" << str << std::endl);
                                        }
                                        if (cLog->IsEnabledFor(GNC::GCS::IControladorLog::TraceLog)) {
                                                str = (const char*) glGetString(GL_EXTENSIONS);
                                                if (str != NULL) {
                                                        LOG_TRACE("Core/GL", "GL_EXTENSIONS:" << str << std::endl);
                                                }
                                        }
                                }
                        }

                        //t.stop();
                        //std::cerr << "wxVTKRenderWindowInteractor::Render(2) " << t << std::endl;
                }
        }
}
//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::SetRenderWhenDisabled(int newValue)
{
        //Change value of __RenderWhenDisabled ivar.
        //If __RenderWhenDisabled is false (the default), this widget will not
        //call Render() on the RenderWindow if the top level frame (i.e. the
        //containing frame) has been disabled.

        //This prevents recursive rendering during wxSafeYield() calls.
        //wxSafeYield() can be called during the ProgressMethod() callback of
        //a VTK object to have progress bars and other GUI elements updated -
        //it does this by disabling all windows (disallowing user-input to
        //prevent re-entrancy of code) and then handling all outstanding
        //GUI events.

        //However, this often triggers an OnPaint() method for wxVTKRWIs,
        //resulting in a Render(), resulting in Update() being called whilst
        //still in progress.

        RenderWhenDisabled = (newValue != 0);
}
//---------------------------------------------------------------------------
//
// Set the variable that indicates that we want a stereo capable window
// be created. This method can only be called before a window is realized.
//
void wxVTKRenderWindowInteractor::SetStereo(int capable)
{
        if (Stereo != capable) {
                Stereo = capable;
                RenderWindow->StereoCapableWindowOn();
                RenderWindow->SetStereoTypeToCrystalEyes();
                Modified();
        }
}

//---------------------------------------------------------------------------
//
//
void wxVTKRenderWindowInteractor::PrintSelf(ostream& os, vtkIndent indent)
{
        this->Superclass::PrintSelf(os, indent);
}



// ***** keysym stuff below  *****

static const char *AsciiToKeySymTable[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, "Tab", 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        "space", "exclam", "quotedbl", "numbersign",
        "dollar", "percent", "ampersand", "quoteright",
        "parenleft", "parenright", "asterisk", "plus",
        "comma", "minus", "period", "slash",
        "0", "1", "2", "3", "4", "5", "6", "7",
        "8", "9", "colon", "semicolon", "less", "equal", "greater", "question",
        "at", "A", "B", "C", "D", "E", "F", "G",
        "H", "I", "J", "K", "L", "M", "N", "O",
        "P", "Q", "R", "S", "T", "U", "V", "W",
        "X", "Y", "Z", "bracketleft",
        "backslash", "bracketright", "asciicircum", "underscore",
        "quoteleft", "a", "b", "c", "d", "e", "f", "g",
        "h", "i", "j", "k", "l", "m", "n", "o",
        "p", "q", "r", "s", "t", "u", "v", "w",
        "x", "y", "z", "braceleft", "bar", "braceright", "asciitilde", "Delete",
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char* ascii_to_key_sym(int i)
{
        if(i >= 0 && i<256 ) {
                return AsciiToKeySymTable[i];
        }
        return 0;
}


#define WXVTK_HANDLE(x,y) \
  case x : \
  ret = y; \
  break;

const char* wx_key_to_key_sym(int i)
{
        const char* ret = 0;
        switch(i) {
                WXVTK_HANDLE(WXK_CLEAR, "Clear")
                WXVTK_HANDLE(WXK_BACK, "BackSpace")
                WXVTK_HANDLE(WXK_TAB, "Tab")
                WXVTK_HANDLE(WXK_RETURN, "Return")
                WXVTK_HANDLE(WXK_SHIFT, "Shift_L")
                WXVTK_HANDLE(WXK_CONTROL, "Control_L")
                WXVTK_HANDLE(WXK_ALT, "Alt_L")
                WXVTK_HANDLE(WXK_PAUSE, "Pause")
                WXVTK_HANDLE(WXK_CAPITAL, "Caps_Lock")
                WXVTK_HANDLE(WXK_ESCAPE, "Escape")
                WXVTK_HANDLE(WXK_SPACE, "space")
                WXVTK_HANDLE(WXK_END, "End")
                WXVTK_HANDLE(WXK_HOME, "Home")
                WXVTK_HANDLE(WXK_LEFT, "Left")
                WXVTK_HANDLE(WXK_UP, "Up")
                WXVTK_HANDLE(WXK_RIGHT, "Right")
                WXVTK_HANDLE(WXK_DOWN, "Down")
                WXVTK_HANDLE(WXK_SNAPSHOT, "Snapshot")
                WXVTK_HANDLE(WXK_INSERT, "Insert")
                WXVTK_HANDLE(WXK_DELETE, "Delete")
                WXVTK_HANDLE(WXK_HELP, "Help")
                WXVTK_HANDLE(WXK_NUMPAD0, "0")
                WXVTK_HANDLE(WXK_NUMPAD1, "1")
                WXVTK_HANDLE(WXK_NUMPAD2, "2")
                WXVTK_HANDLE(WXK_NUMPAD3, "3")
                WXVTK_HANDLE(WXK_NUMPAD4, "4")
                WXVTK_HANDLE(WXK_NUMPAD5, "5")
                WXVTK_HANDLE(WXK_NUMPAD6, "6")
                WXVTK_HANDLE(WXK_NUMPAD7, "7")
                WXVTK_HANDLE(WXK_NUMPAD8, "8")
                WXVTK_HANDLE(WXK_NUMPAD9, "9")
                // KP_0 - KP_9
                WXVTK_HANDLE(WXK_MULTIPLY, "asterisk")
                WXVTK_HANDLE(WXK_ADD, "plus")
                // bar
                WXVTK_HANDLE(WXK_SUBTRACT, "minus")
                WXVTK_HANDLE(WXK_DECIMAL, "period")
                WXVTK_HANDLE(WXK_SEPARATOR, "slash")
                WXVTK_HANDLE(WXK_F1, "F1")
                WXVTK_HANDLE(WXK_F2, "F2")
                WXVTK_HANDLE(WXK_F3, "F3")
                WXVTK_HANDLE(WXK_F4, "F4")
                WXVTK_HANDLE(WXK_F5, "F5")
                WXVTK_HANDLE(WXK_F6, "F6")
                WXVTK_HANDLE(WXK_F7, "F7")
                WXVTK_HANDLE(WXK_F8, "F8")
                WXVTK_HANDLE(WXK_F9, "F9")
                WXVTK_HANDLE(WXK_F10, "F10")
                WXVTK_HANDLE(WXK_F11, "F11")
                WXVTK_HANDLE(WXK_F12, "F12")
                WXVTK_HANDLE(WXK_F13, "F13")
                WXVTK_HANDLE(WXK_F14, "F14")
                WXVTK_HANDLE(WXK_F15, "F15")
                WXVTK_HANDLE(WXK_F16, "F16")
                WXVTK_HANDLE(WXK_F17, "F17")
                WXVTK_HANDLE(WXK_F18, "F18")
                WXVTK_HANDLE(WXK_F19, "F19")
                WXVTK_HANDLE(WXK_F20, "F20")
                WXVTK_HANDLE(WXK_F21, "F21")
                WXVTK_HANDLE(WXK_F22, "F22")
                WXVTK_HANDLE(WXK_F23, "F23")
                WXVTK_HANDLE(WXK_F24, "F24")
                WXVTK_HANDLE(WXK_NUMLOCK, "Num_Lock")
                WXVTK_HANDLE(WXK_SCROLL, "Scroll_Lock")
                WXVTK_HANDLE(WXK_PAGEUP, "Page_Up")
                WXVTK_HANDLE(WXK_PAGEDOWN, "Page_Down")
#if WXWIN_COMPATIBILITY_2_6
                WXVTK_HANDLE(WXK_NUMPAD_PRIOR, "Page_Up")
                WXVTK_HANDLE(WXK_NUMPAD_NEXT, "Page_Down")
#endif
                WXVTK_HANDLE(WXK_NUMPAD_END, "End")
                WXVTK_HANDLE(WXK_NUMPAD_BEGIN, "Begin")
                WXVTK_HANDLE(WXK_NUMPAD_INSERT, "Insert")
                WXVTK_HANDLE(WXK_NUMPAD_DELETE, "Delete")
                WXVTK_HANDLE(WXK_NUMPAD_EQUAL, "Equal")
                WXVTK_HANDLE(WXK_NUMPAD_MULTIPLY, "asterix")
                WXVTK_HANDLE(WXK_NUMPAD_ADD, "plus")
                WXVTK_HANDLE(WXK_NUMPAD_SEPARATOR, "slash")
                WXVTK_HANDLE(WXK_NUMPAD_SUBTRACT, "minus")
                WXVTK_HANDLE(WXK_NUMPAD_DECIMAL, "period")
                WXVTK_HANDLE(WXK_NUMPAD_DIVIDE, "slash")

                WXVTK_HANDLE(WXK_WINDOWS_LEFT, "Win_L")
                WXVTK_HANDLE(WXK_WINDOWS_RIGHT, "Win_R")
                WXVTK_HANDLE(WXK_WINDOWS_MENU, "Menu")
//NOTHING TO DO..WXK_COMMAND IS "Control_L"			WXVTK_HANDLE(WXK_COMMAND, "Command")


        default:
                break;
        }
        return ret;
}


//---------------------------------------------------------------------------
void wxVTKRenderWindowInteractor::OnMouseDoubleClick(wxMouseEvent &event)
{
        if (!Enabled || (ActiveButton != wxEVT_NULL)) {
                return;
        }
        Activar();
        ActiveButton = event.GetEventType();

        // On Mac (Carbon) and Windows we don't automatically get the focus when
        // you click inside the window
        // we therefore set the focus explicitly
        // Apparently we need that on linux (GTK) too:
        this->SetFocus();

        SetEventInformationFlipY(event.GetX(), event.GetY(),
                                 event.ControlDown(), event.ShiftDown(), '\0', 1, NULL);

        if (event.LeftDClick()) {
                InvokeEvent(vtkCommand::LeftButtonPressEvent, NULL);
                InvokeEvent(vtkCommand::LeftButtonReleaseEvent, NULL);
        }

        ActiveButton = wxEVT_NULL;
        TransformarEvento(event);
}

//----------------------------------------------------------------------------------------------------
//region Interfaz IWidgetsRenderer

void wxVTKRenderWindowInteractor::PostRender(bool inmediato)
{
        if(inmediato) {
                //this->Refresh(false);
                //this->Update();
                this->Render();
        } else {
                this->Refresh(false);
        }

}

void wxVTKRenderWindowInteractor::WSetCurrent()
{
        this->RenderWindow->MakeCurrent();
}

void wxVTKRenderWindowInteractor::OnActivar()
{
        this->SetFocus();
}

//----------------------------------------------------------------------------------------------------
//region "Interfaz de transformacion de coordenadas"

void wxVTKRenderWindowInteractor::DisplayToWorld(double displayPos[3], double worldPos[4])
{
        worldPos[0] = worldPos[1] = worldPos[2] = worldPos[3] = 0.0;
        if (m_pPicker != NULL && m_pImageViewer != NULL) {
                int x = displayPos[0];
                int y = displayPos[1];
                vtkRenderer* ren = FindPokedRenderer(x, y);

                if (ren != NULL) {
                        vtkCamera* cam = ren->GetActiveCamera();
                        if (cam != NULL) {
                                double pt[3]  = { static_cast<double>(x), static_cast<double>(y), 0.0};
                                double world[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                                m_pPicker->Pick(pt, ren);
                                m_pPicker->GetPickPosition(world);

                                double cpos[3];
                                double cdir[3];
                                cam->GetPosition(cpos);
                                cam->GetDirectionOfProjection(cdir);
                                if (cam->GetParallelProjection()) {
                                        this->m_pImageViewer->IntersectarRayo(world, cdir, worldPos);
                                        worldPos[3] = 1.0;
                                }
                        }
                }
        }

}

void wxVTKRenderWindowInteractor::WorldToDisplay(double worldPos[4], double displayPos[3])
{
        if (!Enabled || !Initialized) {
                return;
        }
        vtkRenderer* pRenderer = FindPokedRenderer(0,0);

        if (pRenderer == NULL || !pRenderer->IsActiveCameraCreated()) {
                return;
        }

        vtkInteractorObserver::ComputeWorldToDisplay(pRenderer, worldPos[0], worldPos[1], worldPos[2], displayPos);

}

void wxVTKRenderWindowInteractor::GetCamVectors(double camPos[4], double camDir[4], double camUp[4])
{
        bool available = false;

        if (m_pPicker != NULL && m_pImageViewer != NULL) {
                int x = Size[0] >> 1;
                int y = Size[1] >> 1;
                vtkRenderer* ren = FindPokedRenderer(x, y);

                if (ren != NULL) {
                        vtkCamera* cam = ren->GetActiveCamera();
                        cam->GetPosition(camPos);
                        cam->GetDirectionOfProjection(camDir);
                        cam->GetViewUp(camUp);
                        camPos[3] = camDir[3] = camUp[3] = 1.0;
                        available = true;
                }
        }
        if (!available) {
                for (int i = 0; i < 4; i++) {
                        camPos[i] = camDir[i] = camUp[i] = std::numeric_limits<double>::quiet_NaN();
                }
        }
}

void wxVTKRenderWindowInteractor::RayPick(double displayPos[2], double worldPos[4])
{

        bool error = true;

        if (m_pPicker != NULL && m_pImageViewer != NULL) {
                int x = (int)(displayPos[0]);
                int y = Size[1] - (int)(displayPos[1]) - 1;
                vtkRenderer* ren = FindPokedRenderer(x, y);

                if (ren != NULL) {
                        vtkCamera* cam = ren->GetActiveCamera();
                        double pt[3]  = { static_cast<double>(x), static_cast<double>(y), 0.0 };
                        double world[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                        m_pPicker->Pick(pt, ren);
                        m_pPicker->GetPickPosition(world);

                        double cpos[3];
                        double cdir[3];
                        cam->GetPosition(cpos);
                        cam->GetDirectionOfProjection(cdir);
                        if (cam->GetParallelProjection()) {
                                this->m_pImageViewer->IntersectarRayo(world, cdir, worldPos);
                                worldPos[3] = 1.0;
                                error = false;
                        }
                }
        }
        if (error) {
                worldPos[0] = worldPos[1] = worldPos[2] = worldPos[3] = std::numeric_limits<double>::quiet_NaN();
        }

}

void wxVTKRenderWindowInteractor::RayPick(double worldPos[4])
{

        bool error = true;

        if (m_pPicker != NULL && m_pImageViewer != NULL) {
                int x = Size[0] >> 1;
                int y = Size[1] >> 1;
                vtkRenderer* ren = FindPokedRenderer(x, y);

                if (ren != NULL) {
                        vtkCamera* cam = ren->GetActiveCamera();
                        double cpos[3];
                        double cdir[3];
                        cam->GetPosition(cpos);
                        cam->GetDirectionOfProjection(cdir);
                        this->m_pImageViewer->IntersectarRayo(cpos, cdir, worldPos);
                        worldPos[3] = 1.0;
                        error = false;
                }
        }
        if (error) {
                worldPos[0] = worldPos[1] = worldPos[2] = worldPos[3] = std::numeric_limits<double>::quiet_NaN();
        }

}

//endregion

//----------------------------------------------------------------------------------------------------
//region "Helpers de contexto de dibujado"


void wxVTKRenderWindowInteractor::TransformarEvento(wxMouseEvent &event)
{
        if (m_pPicker != NULL && m_pImageViewer != NULL) {
                GNC::GCS::Events::EventoRaton wrappedEvent(&event, m_c);
                int x = event.GetX();
                int y = Size[1] - event.GetY() - 1;
                vtkRenderer* ren = FindPokedRenderer(x, y);

                if (ren != NULL) {
                        vtkCamera* cam = ren->GetActiveCamera();
                        if (cam != NULL) {


                                double pt[3]  = { static_cast<double>(x), static_cast<double>(y), 0.0f };
                                double world[4] = {0.0f, 0.0f, 0.0f, 0.0f};
                                m_pPicker->Pick(pt, ren);
                                m_pPicker->GetPickPosition(world);

                                double cpos[3];
                                double cdir[3];
                                cam->GetPosition(cpos);
                                cam->GetDirectionOfProjection(cdir);
                                if (cam->GetParallelProjection()) {
                                        double ipt[3];

                                        this->m_pImageViewer->IntersectarRayo(world, cdir, wrappedEvent.wP.v);
                                        wrappedEvent.wP.v[3] = 1.0;
                                        this->m_pImageViewer->Proyect2D(wrappedEvent.wP, ipt);
                                        wrappedEvent.iP.Asignar(ipt[0], ipt[1]);

                                } else {
                                        //std::cout << "Error: Reproyeccion en proyeccion perspectiva no soportado" << std::endl;
                                        event.Skip(wrappedEvent.GetSkipped());
                                }
                                CalcularRelacionPixelsPantalla(ren);
                                PropagarEvento(wrappedEvent);
                                event.Skip(wrappedEvent.GetSkipped());
                        }
                }
        }
}

void wxVTKRenderWindowInteractor::TransformarEvento(wxKeyEvent &event)
{
        GNC::GCS::Events::EventoTeclado wrappedEvent(&event, m_c);
        PropagarEvento(wrappedEvent);
        event.Skip(wrappedEvent.GetSkipped());
}

/** Proyecta worlPos sobre el viewport, crea un rectangulo de lado 1, lo reproyecta al mundo y devuelve las dimensiones del cubo envolvente de esta ultima reproyección.
* El resultado obtenido, sirve para calcular la proporcion pixels pantalla <-> coordenadas mundo.
* Si pos es NULL, se utiliza el centro del viewport como punto generador del rectángulo.
* El resultado queda almacenado en el contexto de dibujado (miembro relacionMundoPantalla_{x,y,z})
*/
void wxVTKRenderWindowInteractor::CalcularRelacionPixelsPantalla(vtkRenderer* pRenderer)
{
        if (pRenderer == NULL || !pRenderer->IsActiveCameraCreated() || this->m_pImageViewer == NULL || m_c == NULL) {
                return;
        }

        GNC::GCS::Vector screenCenter(Size[0] >> 1, Size[1] >> 1);

        GNC::GCS::Vector   screenPoint[4]; // Screen Points: {center.top_left, center.top_right, center.bottom_right, center.bottom_left}: OpenGL Covention, 0,0 = lower,left
        GNC::GCS::Vector3D  worldPoint[4]; // World points
        GNC::GCS::Vector3D  planePoint[4]; // World points proyected in plane point (in world coords)
        GNC::GCS::Vector3D  imagePoint[4]; // World points proyected in plane point (in image coords)

        /*
        screenPoint[0].Asignar(screenCenter.x - 0.5, screenCenter.y - 0.5); // s[0] = bottom,left  [0,0]
        screenPoint[1].Asignar(screenCenter.x + 0.5, screenCenter.y - 0.5); // s[1] = top,left     [x,0]
        screenPoint[2].Asignar(screenCenter.x + 0.5, screenCenter.y + 0.5); // s[2] = top,right    [x,y]
        screenPoint[3].Asignar(screenCenter.x - 0.5, screenCenter.y + 0.5); // s[3] = bottom,right [0,y]
        */

        screenPoint[0].Asignar(screenCenter.x - 0.5, screenCenter.y + 0.5); // s[0] = top,left     [0,0]
        screenPoint[1].Asignar(screenCenter.x + 0.5, screenCenter.y + 0.5); // s[1] = top,right    [x,0]
        screenPoint[2].Asignar(screenCenter.x + 0.5, screenCenter.y - 0.5); // s[2] = bottom,right [x,y]
        screenPoint[3].Asignar(screenCenter.x - 0.5, screenCenter.y - 0.5); // s[3] = bottom,left  [y,0]

        // Transform the screen coords => plane points
        GNC::GCS::Vector3D camDir = pRenderer->GetActiveCamera()->GetDirectionOfProjection();
        for (int i = 0; i < 4; ++i) {
                vtkInteractorObserver::ComputeDisplayToWorld(pRenderer, screenPoint[i].x, screenPoint[i].y, 0, worldPoint[i].v);
                this->m_pImageViewer->IntersectarRayo(worldPoint[i].v, camDir.v, planePoint[i].v);
                this->m_pImageViewer->Proyect2D(planePoint[i].v, imagePoint[i].v);
        }

        // Calculate ScreenDirectionCosines
        this->m_c->ScreenDirectionCosines[0] = GNC::GCS::Vector(imagePoint[1].x - imagePoint[0].x , imagePoint[1].y - imagePoint[0].y).Normalizar(); // xdir [x, 0]
        this->m_c->ScreenDirectionCosines[1] = GNC::GCS::Vector(imagePoint[3].x - imagePoint[0].x , imagePoint[3].y - imagePoint[0].y).Normalizar(); // ydir [0, y]
        GNC::GCS::Vector3D rel((imagePoint[1] - imagePoint[0]).Norma2(), (imagePoint[3] - imagePoint[0]).Norma2(),1);

        //std::cout << "Dir : { " << this->m_c->ScreenDirectionCosines[0] << " }, { " << this->m_c->ScreenDirectionCosines[1] << " }" << std::endl;
        //std::cout << "Rel : { " << rel << " }" << std::endl;

        if (m_pImageViewer) {
                this->m_c->rotacion       = this->m_pImageViewer->GetCameraRotation();
                this->m_c->flipHorizontal = this->m_pImageViewer->GetCameraFlipHorizontal();
                this->m_c->flipVertical   = this->m_pImageViewer->GetCameraFlipVertical();
        }
        this->m_c->SetupImageScreenRel(rel.x, rel.y, rel.z);
}

void wxVTKRenderWindowInteractor::CrearContexto3D(int ancho, int alto)
{
        DestruirContexto3D();

        m_c->ancho = ancho;
        m_c->alto = alto;

}

void wxVTKRenderWindowInteractor::DestruirContexto3D()
{
        m_c->Reset();
}
//endregion
