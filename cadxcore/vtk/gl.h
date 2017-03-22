#ifndef ginkgo_vtkgl_h
#define ginkgo_vtkgl_h

#ifdef VTK_RENDERING_OPENGL2
#define GL_GLEXT_PROTOTYPES 1
#include <GL/gl.h>
#include <GL/glext.h>
#else 
#include <vtkOpenGLExtensionManager.h>
#include <vtkgl.h>
// this is really a bad hack
using namespace vtkgl;
#define glCreateProgram() vtkgl::CreateProgram()
#define glCreateShader(X) vtkgl::CreateShader(X)
#define glShaderSource(A,B,C,D) vtkgl::ShaderSource(A,B,C,D)
#define glCompileShader(X) vtkgl::CompileShader(X)
#define glAttachShader(X,Y) vtkgl::AttachShader(X,Y)
#define glGetShaderiv(A, B, C) vtkgl::GetShaderiv(A,B,C)
#define glGetShaderInfoLog(A,B,C,D) vtkgl::GetShaderInfoLog(A,B,C,D)
#define glLinkProgram(A) vtkgl::LinkProgram(A)
#define glUseProgram(A) vtkgl::UseProgram(A)
#define glGetProgramiv(A,B,C) vtkgl::GetProgramiv(A,B,C)
#define glGetUniformLocation vtkgl::GetUniformLocation
#define glUniform1i(A,B)  vtkgl::Uniform1i(A,B)
#define glUniform1f(A,B)  vtkgl::Uniform1f(A,B)

// for some reason these are not defined in older VTK or <GL/gl.h>
#ifndef GL_FRAGMENT_PROGRAM_ARB
#define GL_FRAGMENT_PROGRAM_ARB  vtkgl::FRAGMENT_PROGRAM_ARB
#endif

#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER vtkgl::VERTEX_SHADER
#endif

#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS vtkgl::COMPILE_STATUS
#endif

#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH vtkgl::INFO_LOG_LENGTH
#endif

#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER vtkgl::FRAGMENT_SHADER
#endif

#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS vtkgl::LINK_STATUS
#endif

#endif //VTK_RENDERING_OPENGL2

#endif 

