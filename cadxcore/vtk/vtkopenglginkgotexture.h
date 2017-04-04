/*
 *
 *  $Id: vtkopenglginkgotexture.h $
 *  Ginkgo CADx Project
 *
 *  Code adapted from VTK
 *
 *
=========================================================================

 Program:   Visualization Toolkit
 Module:    $RCSfile: vtkGinkgoOpenGLTexture.h,v $

 Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
 All rights reserved.
 See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notice for more information.

 =========================================================================*/
// .NAME vtkGinkgoOpenGLTexture - OpenGL texture map
// .SECTION Description
// vtkGinkgoOpenGLTexture is a concrete implementation of the abstract class
// vtkTexture. vtkGinkgoOpenGLTexture interfaces to the OpenGL rendering library.

#pragma once
#include <api/ilock.h>
#include <vtkOpenGLTexture.h>
//BTX
#include <vtkWeakPointer.h> // needed for vtkWeakPointer.
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
//ETX


#include <vtkCommand.h>

class vtkWindow;
class vtkOpenGLRenderer;
class vtkRenderWindow;
class vtkPixelBufferObject;
class vtkOpenGLRenderWindow;
class vtkActor;



class vtkGinkgoOpenGLTexture : public vtkOpenGLTexture, public GNC::GCS::ILockable
{
public:
        static vtkGinkgoOpenGLTexture *New();
        vtkTypeMacro(vtkGinkgoOpenGLTexture,vtkOpenGLTexture);
        virtual void PrintSelf(ostream& os, vtkIndent indent);

        // Description:
        // Implement base class method.
        void Load(vtkRenderer *ren);

        // Descsription:
        // Clean up after the rendering is complete.
        virtual void PostRender(vtkRenderer *ren);

        // Description:
        // Release any graphics resources that are being consumed by this texture.
        // The parameter window could be used to determine which graphic
        // resources to release. Using the same texture object in multiple
        // render windows is NOT currently supported.
        void ReleaseGraphicsResources(vtkWindow *);


        // Description:
        // Get the openGL texture name to which this texture is bound.
        // This is available only if GL version >= 1.1
        vtkGetMacro(Index, long);

        long GetTextureId() const
        {
                return Index;
        }

        //indica si la textura tiene id (se ha intentado cargar)
        bool TextureDefined() const
        {
                return Index != 0;
        }

        void SetTindex(int index)
        {
                GNC::GCS::ILocker lock(this);
                if (TIndex != index) {
                        this->TIndex = index;
                        this->TIndexChanged = true;
                }
        }

        int GetTindex()  const
        {
                return this->TIndex;
        }

        operator long () const
        {
                return Index;
        }

        /** Set Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        void SetBrightness(float brightness);

        /** Set Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        void SetContrast(float contrast);

        /** Get Image Brightness (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        float GetBrightness();

        /** Get Image Contrast (-1.0, 0.0, 1.0) (Only aplicable with RGB Images) **/
        float GetContrast();

        void ResetBrightnessAndContrast();

        void ForceEnableShaders(bool force);

#ifdef VTK_RENDERING_OPENGL2
        class vtkShaderCallback : public vtkCommand {
        public:
                vtkShaderCallback();

                static vtkShaderCallback *New() { return new vtkShaderCallback; }

                void SetGrayParameters(int tex_unit, int lut_unit, double shift, double scale);
                void SetRgbParameters(double brightness, double contrast);
                virtual void Execute(vtkObject *, unsigned long, void*cbo);
        private:
                enum EActiveShader {
                        as_gray,
                        as_rgb,
                        as_none
                };

                EActiveShader m_active_shader;
                int m_tex_unit;
                int m_lut_unit;
                double m_shift;
                double m_scale;

                double m_brightness;
                double m_contrast;

        };

        void SetMapper(vtkSmartPointer<vtkPolyDataMapper>& mapper);
        vtkSmartPointer<vtkShaderCallback> m_shader_callback;
#endif

        //BTX
protected:
        vtkGinkgoOpenGLTexture();
        ~vtkGinkgoOpenGLTexture();

        unsigned char *ResampleToPowerOfTwo(int &xsize, int &ysize,
                                            unsigned char *dptr, int bpp);

        vtkTimeStamp   LoadTime;
        unsigned int Index; // actually GLuint
        vtkWeakPointer<vtkRenderWindow> m_render_window;   // RenderWindow used for previous render
        bool CheckedHardwareSupport;
        bool SupportsNonPowerOfTwoTextures;
        bool SupportsPBO;
        bool SupportsVertexShaders;
        bool SupportsFragmentShaders;
        bool SupportsMultiTexture;
        unsigned int LUTIndex;
        bool m_use_shader;
        int zsize;
        int TIndex;
        bool TIndexChanged;
        bool m_lut_changed;
        bool InternalEnableShaders;
        bool RGBImage;

        float m_brightness; // Only for RGB Images
        float m_contrast;   // Only for RGB Images

#ifdef VTK_RENDERING_OPENGL2
        vtkSmartPointer<vtkTextureObject> m_lut;
        int m_this_texture_unit;
        int m_lut_texture_unit;
        int LoadLUT(vtkOpenGLRenderWindow *ren);
        int m_lut_size;
        vtkSmartPointer<vtkPolyDataMapper>  m_mapper;
#else
        vtkPixelBufferObject *PBO;
        long VertexProgram;
        long FragmentProgram;
        long ProgramObject;
#endif
public:
        void SetLookupTable(vtkScalarsToColors* table);

private:
        vtkGinkgoOpenGLTexture(const vtkGinkgoOpenGLTexture&);  // Not implemented.
        void operator=(const vtkGinkgoOpenGLTexture&);  // Not implemented.

        // Description:
        // Handle loading in extension support
        virtual void Initialize(vtkRenderer * ren);

        void SetupTexturing(vtkRenderer *ren);
        //ETX



};
