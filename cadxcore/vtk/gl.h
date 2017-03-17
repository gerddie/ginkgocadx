#ifndef vtkgl_h
#define vtkgl_h

#ifdef VTK_RENDERING_OPENGL2
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#else 
#include <vtkOpenGLExtensionManager.h>
#include <vtkgl.h>
#endif

#endif 

