
/*
*
*  $Id: vtkopenglginkgotexture.cpp $
*  Ginkgo CADx Project
*
*  Code adapted from VTK
*
*
=========================================================================

Program:   Visualization Toolkit
Module:    $RCSfile: vtkGinkgoOpenGLTexture.cxx,v $

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#if defined max
#undef max
#endif
#if defined min
#undef min
#endif

#include <cmath>
#include <limits>

#if defined(__DEPRECATED)
#undef __DEPRECATED
#endif
#include "vtkopenglginkgotexture.h"

#include <vtkHomogeneousTransform.h>
#include <vtkImageData.h>
#include <vtkUnsignedCharArray.h>
#include <vtkUnsignedShortArray.h>
#include <vtkUnsignedIntArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>
#include <vtkOpenGLRenderer.h>
#include <vtkPointData.h>
#include <vtkRenderWindow.h>

#include <vtk/gl.h>

#include <vtkOpenGLRenderWindow.h>
#include <vtkTransform.h>
#include <vtkPixelBufferObject.h>
#include <vtkOpenGL.h>



#include <main/controllers/controladorlog.h>
#include <main/controllers/controladorpermisos.h>

#ifndef VTK_IMPLEMENT_MESA_CXX
vtkStandardNewMacro(vtkGinkgoOpenGLTexture);
#endif

#define ADAPTATIVE_BRIGHNESS_CONTRAST

inline char clampByte(int color)
{
        if( color & (~255) ) {
                color = (char)((-(color)) >> 31);
        } else {
                color = (char)(color);
        }
        return color;
}

// integer implementation (fast but incomplete)
inline char mixFixedPoint( char x, char y, char a)
{
        unsigned int alpha = a;
        unsigned int inv_alpha = 255 - a;
        return clampByte( (alpha * x + inv_alpha * y) >> 8);
}

/** Interpolate the normalized values between "x" and "y" with factor "1-a, a" (slow) **/
inline float mixNormalized(float x, float y, float a)
{
        return x * (1.0f - a) + (y * a);
}

inline float clamp(float min, float max, float val)
{
        if (val > max) {
                return max;
        } else {
                if (val < min) {
                        return min;
                } else {
                        return val;
                }
        }
}

// ----------------------------------------------------------------------------
// Initializes an instance, generates a unique index.
vtkGinkgoOpenGLTexture::vtkGinkgoOpenGLTexture()
{
        this->Index = 0;
        this->RenderWindow = 0;
        this->CheckedHardwareSupport=false;
        this->SupportsNonPowerOfTwoTextures=false;
        this->SupportsPBO=false;
        this->SupportsVertexShaders=false;
        this->SupportsFragmentShaders=false;
        this->SupportsMultiTexture=false;
        this->PBO=0;
        this->VertexProgram=0;
        this->FragmentProgram=0;
        this->ProgramObject=0;
        this->LUTIndex=0;
        this->UseShader=false;
        this->InternalEnableShaders = true;
        this->zsize=0;
        this->TIndex=0;
        this->TIndexChanged=false;
        this->LookupTableChanged = false;
        this->RGBImage = false;
        this->ResetBrightnessAndContrast();
}

void vtkGinkgoOpenGLTexture::SetBrightness(float brightness)
{
#if defined(ADAPTATIVE_BRIGHNESS_CONTRAST)
        this->Brightness = brightness;
#else
        this->Brightness = clamp(-1.0f, 1.0f, brightness);
#endif

        this->LookupTableChanged = true;
}

void vtkGinkgoOpenGLTexture::SetContrast(float contrast)
{
#if defined(ADAPTATIVE_BRIGHNESS_CONTRAST)
        this->Contrast = contrast;
#else
        this->Contrast = clamp(0.0f, 2.0f, contrast);
#endif

        this->LookupTableChanged = true;
}

float vtkGinkgoOpenGLTexture::GetBrightness()
{
        return this->Brightness;
}

float vtkGinkgoOpenGLTexture::GetContrast()
{
        return this->Contrast;
}

void vtkGinkgoOpenGLTexture::ResetBrightnessAndContrast()
{

#if defined(ADAPTATIVE_BRIGHNESS_CONTRAST)
        this->Brightness = 1.0f;
#else
        this->Brightness = 0.0f;
#endif
        this->Contrast = 1.0f;
        this->LookupTableChanged = true;
}

// ----------------------------------------------------------------------------
vtkGinkgoOpenGLTexture::~vtkGinkgoOpenGLTexture()
{
        if (this->RenderWindow) {
                this->ReleaseGraphicsResources(this->RenderWindow);
        }
        if(this->PBO!=0) {
                vtkErrorMacro(<< "PBO should have been deleted in ReleaseGraphicsResources()");
        }
        this->RenderWindow = NULL;
}

// ----------------------------------------------------------------------------
void vtkGinkgoOpenGLTexture::Initialize(vtkRenderer * vtkNotUsed(ren))
{
}

void vtkGinkgoOpenGLTexture::SetLookupTable(vtkScalarsToColors* table)
{
        vtkOpenGLTexture::SetLookupTable(table);
        this->LookupTableChanged = true;

}

// ----------------------------------------------------------------------------
// Release the graphics resources used by this texture.
void vtkGinkgoOpenGLTexture::ReleaseGraphicsResources(vtkWindow *renWin)
{
        if (this->Index && renWin && renWin->GetMapped()) {
                static_cast<vtkRenderWindow *>(renWin)->MakeCurrent();
#ifdef GL_VERSION_1_1
                // free any textures
                if (glIsTexture(static_cast<GLuint>(this->Index))) {
                        GLuint tempIndex;
                        tempIndex = this->Index;
                        // NOTE: Sun's OpenGL seems to require disabling of texture before delete
                        //glDisable(GL_TEXTURE_2D);
                        glDeleteTextures(1, &tempIndex);
                }
#else
                if (glIsList(this->Index)) {
                        glDeleteLists(this->Index,1);
                }
#endif
        }
        this->Index = 0;
        this->RenderWindow = NULL;
        this->CheckedHardwareSupport=false;
        this->SupportsNonPowerOfTwoTextures=false;
        this->SupportsPBO=false;
        if(this->PBO!=0) {
                this->PBO->Delete();
                this->PBO=0;
        }
        this->Modified();
}


#if defined(ADAPTATIVE_BRIGHNESS_CONTRAST) // "The OpenGL Shading Language" C.19.

const char *FragmenProgramCode_rgb[] =   {
        "uniform sampler2D imagetexture;",
        "uniform float brightness;",
        "uniform float contrast;",
        "vec4 averageLuminance = vec4(0.5, 0.5, 0.5, 1.0);",
        "void main(void)",
        "{",
        "vec4 color = texture2D(imagetexture, gl_TexCoord[0].xy);",
        "gl_FragColor = mix(color * brightness, mix(averageLuminance, color, contrast), 0.5);",
        "}"
};
#else
const char *FragmenProgramCode_rgb[] =   {
        "uniform sampler2D imagetexture;",
        "uniform float brightness;",
        "uniform float contrast;",
        "void main(void)",
        "{",
        "vec4 color = texture2D(imagetexture, gl_TexCoord[0].xy);",
        "float pa = color.a;",
        "color = ((color - 0.5) * max(contrast, 0.0)) + 0.5 + brightness;",
        "color.a = pa;",
        "gl_FragColor = color;", 
        "}"
};
#endif
const char *FragmenProgramCode_gray[] = {
        "uniform sampler2D imagetexture;",
        "uniform sampler1D lookuptable;",
        "uniform float lutShift;",
        "uniform float lutScale;",
        "void main(void)",
        "{",
        "vec4 color = texture2D(imagetexture, gl_TexCoord[0].xy);",
        "float lutColor = (color.r + lutShift) * lutScale;",
        "lutColor = clamp(lutColor, 0.0f, 1.0f);",
        "vec4 cr = texture1D(lookuptable, lutColor);",
        //"cr.a = 1.0;",
        "gl_FragColor = cr;",
        "}"
};


// ----------------------------------------------------------------------------
// Implement base class method.
void vtkGinkgoOpenGLTexture::Load(vtkRenderer *ren)
{
        vtkImageData *input = NULL;

        GNC::GCS::ILocker lock(this);
        {
                input = this->GetInput();

                GLenum TexFormat         = GL_LUMINANCE;
                GLenum TexInternalFormat = GL_LUMINANCE;
                GLenum TexType           = GL_UNSIGNED_BYTE;


                this->Initialize(ren);

                // Need to reload the texture.
                // There used to be a check on the render window's mtime, but
                // this is too broad of a check (e.g. it would cause all textures
                // to load when only the desired update rate changed).
                // If a better check is required, check something more specific,
                // like the graphics context.
                vtkOpenGLRenderWindow* renWin = static_cast<vtkOpenGLRenderWindow*>(ren->GetRenderWindow());

                if(this->BlendingMode != VTK_TEXTURE_BLENDING_MODE_NONE) {
                        glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);

                        switch(this->BlendingMode) {
                        case VTK_TEXTURE_BLENDING_MODE_REPLACE: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE);
                                break;
                        }
                        case VTK_TEXTURE_BLENDING_MODE_MODULATE: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_MODULATE);
                                break;
                        }
                        case VTK_TEXTURE_BLENDING_MODE_ADD: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);
                                break;
                        }
                        case VTK_TEXTURE_BLENDING_MODE_ADD_SIGNED: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD_SIGNED);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD_SIGNED);
                                break;
                        }
                        case VTK_TEXTURE_BLENDING_MODE_INTERPOLATE: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_INTERPOLATE);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_INTERPOLATE);
                                break;
                        }
                        case VTK_TEXTURE_BLENDING_MODE_SUBTRACT: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_SUBTRACT);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_SUBTRACT);
                                break;
                        }
                        default: {
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_ADD);
                                glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_ADD);
                        }
                        }
                }

                bool ImageDataChanged =
                        ( this->GetMTime() > this->LoadTime.GetMTime() )
                        || ( input->GetMTime() > this->LoadTime.GetMTime() )
                        || ( !UseShader && this->GetLookupTable() && (this->GetLookupTable()->GetMTime () > this->LoadTime.GetMTime()) )
                        || ( !UseShader && LookupTableChanged )
                        || ( renWin != this->RenderWindow.GetPointer() )
                        || ( renWin->GetContextCreationTime() > this->LoadTime );

                if (TIndexChanged || ImageDataChanged ) {
                        int bytesPerPixel;
                        int size[3];
                        vtkDataArray *scalars = NULL;
                        unsigned char* vptr = NULL;
                        unsigned char *resultData=NULL;
                        int xsize, ysize;
                        unsigned int xs,ys;
                        GLuint tempIndex=0;

                        unsigned long off = 0;
                        unsigned char* pNPOTPixelData = NULL;
                        unsigned char* pResampledPixelData = NULL;

                        // Get the scalars the user choose to color with.
                        scalars = this->GetInputArrayToProcess(0, input);

                        // make sure scalars are non null
                        if (!input || !scalars) {
                                LOG_ERROR("OpenGLComponent", "NULL INPUT");
                                return;
                        }

                        // get some info
                        input->GetDimensions(size);

                        int e[6] = {0, 0, 0, 0, 0, 0};
                        input->GetExtent(e);

                        //std::cout << "Update extent: [ " << e[0] << ", " << e[1] << ", " << e[2] << ", " << e[3] << ", " << e[4] << ", " << e[5] << " ] " << std::endl;

                        if (input->GetNumberOfCells() == scalars->GetNumberOfTuples()) {
                                // we are using cell scalars. Adjust image size for cells.
                                for (int kk=0; kk < 3; kk++) {
                                        if (size[kk]>1) {
                                                size[kk]--;
                                        }
                                }
                        }

                        bytesPerPixel = scalars->GetNumberOfComponents();

                        xsize = size[0];
                        ysize = size[1];
                        zsize = size[2];

                        if (xsize <= 0 || ysize <= 0 || zsize <= 0) {
                                LOG_ERROR("OpenGLComponent", "Wrong size: " << xsize << "x" << ysize << "x" << zsize);
                                return;
                        }
                        ///CHECK HARDWARE SUPPORT
                        if(!this->CheckedHardwareSupport) {
#ifdef VTK_RENDERING_OPENGL2
                                if (this->InternalEnableShaders && GNC::GCS::IControladorPermisos::Instance()->Get("core.opengl", "enable_shaders").Activo()) {                                        this->UseShader = true;
                                }
                                this->CheckedHardwareSupport = true;
                                this->SupportsNonPowerOfTwoTextures = true;
                                this->SupportsPBO=vtkPixelBufferObject::IsSupported(renWin);
#else                         
                                vtkOpenGLExtensionManager *m = renWin->GetExtensionManager();
                                this->CheckedHardwareSupport = true;
                                this->SupportsNonPowerOfTwoTextures = m->ExtensionSupported("GL_VERSION_2_0") ||
                                        m->ExtensionSupported("GL_ARB_texture_non_power_of_two");
                                this->SupportsPBO=vtkPixelBufferObject::IsSupported(renWin);
                                int supports_GL_VERSION_2_0=m->ExtensionSupported("GL_VERSION_2_0");
                                if(supports_GL_VERSION_2_0) {
                                        m->LoadExtension("GL_ARB_multitexture");
                                        m->LoadExtension("GL_VERSION_2_0");
                                        //ENABLE/DISABLE SHADER!
                                        if (this->InternalEnableShaders && GNC::GCS::IControladorPermisos::Instance()->Get("core.opengl", "enable_shaders").Activo()) {                                        this->UseShader = true;
                                        }
                                }
#endif                               
                        }

                        if (!UseShader) {
                                LookupTableChanged = false; // Reset LUT changed state
                        }

                        TIndex = std::max<int>(0, std::min<int>(TIndex, zsize - 1));
                        TIndexChanged = false;
                        off = size[0] * size[1] * TIndex * bytesPerPixel;
                        //LOG_DEBUG("OpenGLComponent", "Fijado indice " << TIndex);

                        // make sure using unsigned char data of color scalars type
                        // IF MAP SCALARS
                        if ( (this->MapColorScalarsThroughLookupTable) || (scalars->GetDataType() != VTK_UNSIGNED_CHAR) ) {
                                const unsigned int wdh = size[0] * size[1];
                                const unsigned int numComponents = scalars->GetNumberOfComponents();

                                this->RGBImage = false;

                                //IF THERE IS ONLY ONE COMPONENT COULD USE SHADERS...
                                if (numComponents == 1) {

                                        if (UseShader) {
                                                double range[2] = {0.0, 1.0};
                                                scalars->GetRange(range);

                                                TIndex = std::max<int>(0, std::min<int>(TIndex, zsize - 1));
                                                TIndexChanged = false;

                                                LOG_DEBUG("OpenGLComponent", "Tipo de componente: " << scalars->GetDataTypeAsString());
                                                if (scalars->GetDataType() == VTK_UNSIGNED_SHORT || scalars->GetDataType() == VTK_SHORT) {
                                                        //RESCALE TO UNSIGNED SHORT
                                                        bytesPerPixel         = 2;
                                                        TexInternalFormat = GL_LUMINANCE16;
                                                        TexType           = GL_UNSIGNED_SHORT;

                                                        unsigned short* resamplePixelData = new unsigned short[wdh];

                                                        const unsigned int psize = wdh;
                                                        double diff = range[1] - range[0];

                                                        if (diff < std::numeric_limits<double>::epsilon() ) {
                                                                range[0] = (scalars->GetDataType() == VTK_UNSIGNED_SHORT)?((double) std::numeric_limits<unsigned short>::min()):((double) std::numeric_limits<short>::min());
                                                                range[1] = (scalars->GetDataType() == VTK_UNSIGNED_SHORT)?((double) std::numeric_limits<unsigned short>::max()):((double) std::numeric_limits<short>::max());
                                                                diff = range[1] - range[0];
                                                        }

                                                        off = size[0] * size[1] * TIndex * bytesPerPixel;
                                                        unsigned char* dptr = (unsigned char*) scalars->GetVoidPointer(0);
                                                        if (dptr == NULL) {
                                                                delete[] resamplePixelData;
                                                                LOG_ERROR("OpenGLComponent", "Could not get pixel data pointer");
                                                                return;
                                                        }
                                                        vptr = (unsigned char*) (dptr + off);

                                                        double scale = 1 / diff;
                                                        unsigned short* dst = resamplePixelData;
                                                        short* src = (short*)vptr;

                                                        // Verificaciones extra para VIX. El flujo permanece igual.
                                                        unsigned short v;
                                                        long long maxVal1 = - ( (long long) std::numeric_limits<unsigned short>::min());
                                                        long long minVal1 = ( (long long) std::numeric_limits<unsigned short>::max());

                                                        long long maxVal2 = - ( (long long) std::numeric_limits<unsigned short>::min());
                                                        long long minVal2 = ( (long long) std::numeric_limits<unsigned short>::max());

                                                        for (unsigned int i = 0; i < psize; i++) {
                                                                if (scalars->GetDataType() == VTK_UNSIGNED_SHORT) {
                                                                        const unsigned short v1 = *((unsigned short*) src++);
                                                                        minVal1 = std::min(minVal1, (long long)v1);
                                                                        maxVal1 = std::max(maxVal1, (long long)v1);
                                                                        v = (unsigned short)(( scale * ((double)v1 - range[0]) ) * std::numeric_limits<unsigned short>::max());
                                                                        minVal2 = std::min(minVal2, (long long)v);
                                                                        maxVal2 = std::max(maxVal2, (long long)v);
                                                                } else {
                                                                        const signed short v1 = *((signed short*) src++);
                                                                        minVal1 = std::min(minVal1, (long long)v1);
                                                                        maxVal1 = std::max(maxVal1, (long long)v1);
                                                                        v = (unsigned short)(( scale * ((double)v1 - range[0]) ) * std::numeric_limits<unsigned short>::max());
                                                                        minVal2 = std::min(minVal2, (long long)v);
                                                                        maxVal2 = std::max(maxVal2, (long long)v);
                                                                }
                                                                *(dst++) =  v;
                                                        }
                                                        pResampledPixelData = (unsigned char*) resamplePixelData;
                                                        vptr = pResampledPixelData;
                                                } else if (scalars->GetDataType() == VTK_UNSIGNED_CHAR || scalars->GetDataType() == VTK_CHAR) {
                                                        //rescale to unsigned byte
                                                        bytesPerPixel         = 1;
                                                        TexInternalFormat = GL_LUMINANCE8;
                                                        TexType = GL_UNSIGNED_BYTE;

                                                        unsigned char* resamplePixelData = new unsigned char[wdh];

                                                        const unsigned int psize = wdh;
                                                        double diff = range[1] - range[0];

                                                        if (diff < std::numeric_limits<double>::epsilon() ) {
                                                                range[0] = (scalars->GetDataType() == VTK_UNSIGNED_CHAR)?((double) std::numeric_limits<unsigned char>::min()):((double) std::numeric_limits<char>::min());
                                                                range[1] = (scalars->GetDataType() == VTK_UNSIGNED_CHAR)?((double) std::numeric_limits<unsigned char>::max()):((double) std::numeric_limits<char>::max());
                                                                diff = range[1] - range[0];
                                                        }

                                                        off = size[0] * size[1] * TIndex * bytesPerPixel;
                                                        unsigned char* dptr = (unsigned char*) scalars->GetVoidPointer(0);
                                                        if (dptr == NULL) {
                                                                delete[] resamplePixelData;
                                                                LOG_ERROR("OpenGLComponent", "Could not get pixel data pointer");
                                                                return;
                                                        }
                                                        vptr = (unsigned char*) (dptr + off);

                                                        double scale = 1 / diff;
                                                        unsigned char* dst = resamplePixelData;
                                                        char* src = (char*)vptr;

                                                        unsigned char v;
                                                        for (unsigned int i = 0; i < psize; i++) {
                                                                if (scalars->GetDataType() == VTK_UNSIGNED_CHAR) {
                                                                        const unsigned char v1 = *((unsigned char*)src);
                                                                        v = (unsigned char)(( scale * ( v1 - range[0]) )*std::numeric_limits<unsigned char>::max());
                                                                        src++;
                                                                } else {
                                                                        v = (unsigned char)(( scale * ((char) *(src++) - range[0]) )*std::numeric_limits<unsigned char>::max());
                                                                }
                                                                *(dst++) =  v;
                                                        }
                                                        pResampledPixelData = (unsigned char*) resamplePixelData;
                                                        vptr = pResampledPixelData;
                                                } else {
                                                        LOG_DEBUG("OpenGLComponent", "Shaders don't support this component type: Wrong size: " << scalars->GetDataTypeAsString());
                                                        this->UseShader = false;
                                                }
                                        }
                                        //if support shaders but scalar type isn't short or char, vtk makes the mapping
                                        if (!this->UseShader) {
                                                //optimization, if we call mapscalars to colors with all scalars multiframe performance will be very bad
                                                if (zsize > 1) {
                                                        vtkDataArray* myScalars = vtkDataArray::CreateDataArray(scalars->GetDataType());
                                                        myScalars->SetNumberOfComponents(scalars->GetNumberOfComponents());
                                                        myScalars->SetNumberOfTuples(wdh);
                                                        myScalars->SetVoidArray((scalars->GetVoidPointer(wdh * TIndex)), wdh, 1);
                                                        myScalars->SetLookupTable(scalars->GetLookupTable());
                                                        vptr = this->MapScalarsToColors (myScalars);
                                                        myScalars->Delete();
                                                } else {
                                                        vptr = this->MapScalarsToColors (scalars);
                                                }
                                                if (vptr == NULL) {
                                                        LOG_DEBUG("OpenGLComponent", "Could not map pixel data");
                                                        return;
                                                }
                                                bytesPerPixel = 4;
                                        }
                                }//end 1 component
                                else {
                                        //lookup table with more than one component... extrange
                                        this->UseShader = false;
                                        pResampledPixelData = new unsigned char[wdh * 4];
                                        const unsigned int psize = wdh;
                                        double range[2] = {0.0, 1.0};
                                        scalars->GetRange(range);
                                        double diff = range[1] - range[0];

                                        vptr = (unsigned char*) scalars->GetVoidPointer(0);
                                        unsigned char *dst = pResampledPixelData;

                                        switch (scalars->GetDataType()) {
                                        case VTK_CHAR: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<char>::min();
                                                        range[1] = (double) std::numeric_limits<char>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                double scale = 255.0 / diff;

                                                char *src = ((char*) vptr) + (wdh * TIndex * numComponents);

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }

                                        }
                                        break;
                                        case VTK_UNSIGNED_CHAR: {
                                                unsigned char *src = ((unsigned char*) vptr) + (wdh * TIndex * numComponents);

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = *(src++);
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }

                                        }
                                        break;
                                        case VTK_SHORT: {

                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<short>::min();
                                                        range[1] = (double) std::numeric_limits<short>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                short *src = ((short*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                        case VTK_UNSIGNED_SHORT: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<unsigned short>::min();
                                                        range[1] = (double) std::numeric_limits<unsigned short>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                unsigned short *src = ((unsigned short*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;

                                        case VTK_INT: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<int>::min();
                                                        range[1] = (double) std::numeric_limits<int>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                int *src = ((int*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                        case VTK_UNSIGNED_INT: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<unsigned int>::min();
                                                        range[1] = (double) std::numeric_limits<unsigned int>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                unsigned int *src = ((unsigned int*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                        case VTK_FLOAT: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<float>::min();
                                                        range[1] = (double) std::numeric_limits<float>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                float *src = ((float*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                        case VTK_DOUBLE: {
                                                if (diff < std::numeric_limits<double>::epsilon() ) {
                                                        range[0] = (double) std::numeric_limits<double>::min();
                                                        range[1] = (double) std::numeric_limits<double>::max();
                                                        diff = range[1] - range[0];
                                                }

                                                double *src = ((double*) vptr) + (wdh * TIndex * numComponents);
                                                double scale = 255.0 / diff;

                                                for (unsigned int i = 0; i < psize; i++) {
                                                        for (unsigned int j = 0; j < 4; j++) {
                                                                if (j < numComponents) {
                                                                        const unsigned char v = (unsigned char) ( scale * ((double) *(src++)) );
                                                                        *(dst++) =  v;
                                                                } else {
                                                                        *(dst++) = 255;
                                                                }
                                                        }
                                                }
                                        }
                                        break;
                                        default:
                                                delete[] pResampledPixelData;
                                                LOG_ERROR("OpenGLComponent", "Pixel data not supported");
                                                return;
                                        }

                                        vptr = pResampledPixelData;
                                        bytesPerPixel = 4;
                                }//end more than one component...
                        } else {
                                if ( (scalars->GetDataType() == VTK_UNSIGNED_CHAR) ) {
                                        this->RGBImage = true;
                                } else {
                                        this->RGBImage = false;
                                        this->UseShader = false;
                                }

                                //RGB IMAGES
                                unsigned char *s_array = static_cast<vtkUnsignedCharArray *>(scalars)->GetPointer(0);
                                if (s_array != NULL) {
                                        int bsize = bytesPerPixel * size[0] * size[1];

                                        unsigned char *src = s_array + off;
                                        unsigned char *dst = vptr = pResampledPixelData = new unsigned char[bsize];

#if defined (ADAPTATIVE_BRIGHNESS_CONTRAST)
                                        for (int i = 0; i < bsize; i++) {
                                                const float colorIn = (float) ( *(src++) ) / 255.0f;
                                                const float clampedResult = clamp(0.0f, 1.0f, mixNormalized( colorIn * Brightness, mixNormalized(0.5f, colorIn, Contrast), 0.5f ));
                                                const char colorOut = (char) ( clamp(0.0f, 255.0f, 255.0f * clampedResult) );
                                                *(dst++) = colorOut;
                                        }
#else
                                        std::cout << "---\nBrightness = " << Brightness << std::endl;
                                        std::cout << "Contrast =  " << Contrast << std::endl;

                                        for (int i = 0; i < bsize; i++) {
                                                const float colorIn = (float) ( *(src++) ) / 255.0f;
                                                const float clampedResult = clamp(0.0f, 1.0f, ((colorIn - 0.5f) * std::max<float>(Contrast, 0.0f)) + 0.5f + Brightness );
                                                const char colorOut = (char) ( clamp(0.0f, 255.0f, 255.0f * clampedResult) );
                                                *(dst++) = colorOut;
                                        }
#endif
                                } else {
                                        LOG_DEBUG("OpenGLComponent", "Could not get pixel data pointer");
                                        return;
                                }
                        }

                        //if no use pixel shader...
                        if (!this->UseShader || RGBImage) {
                                TexInternalFormat = bytesPerPixel;
                                TexType = GL_UNSIGNED_BYTE;
                                switch (bytesPerPixel) {
                                case 1:
                                        TexFormat = GL_LUMINANCE;
                                        break;
                                case 2:
                                        TexFormat = GL_LUMINANCE_ALPHA;
                                        break;
                                case 3:
                                        TexFormat = GL_RGB;
                                        break;
                                case 4:
                                        TexFormat = GL_RGBA;
                                        break;
                                }
                                // if we are using OpenGL 1.1, you can force 32 or16 bit textures
#ifdef GL_VERSION_1_1
                                if (this->Quality == VTK_TEXTURE_QUALITY_32BIT) {
                                        switch (bytesPerPixel) {
                                        case 1:
                                                TexInternalFormat = GL_LUMINANCE8;
                                                break;
                                        case 2:
                                                TexInternalFormat = GL_LUMINANCE8_ALPHA8;
                                                break;
                                        case 3:
                                                TexInternalFormat = GL_RGB8;
                                                break;
                                        case 4:
                                                TexInternalFormat = GL_RGBA8;
                                                break;
                                        }
                                } else if (this->Quality == VTK_TEXTURE_QUALITY_16BIT) {
                                        switch (bytesPerPixel) {
                                        case 1:
                                                TexInternalFormat = GL_LUMINANCE4;
                                                break;
                                        case 2:
                                                TexInternalFormat = GL_LUMINANCE4_ALPHA4;
                                                break;
                                        case 3:
                                                TexInternalFormat = GL_RGB4;
                                                break;
                                        case 4:
                                                TexInternalFormat = GL_RGBA4;
                                                break;
                                        }
                                }
#endif
                        }

                        // -- decide whether the texture needs to be resampled --

                        GLint maxDimGL;
                        glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxDimGL);
                        // if larger than permitted by the graphics library then must resample
                        bool resampleNeeded = xsize > maxDimGL || ysize > maxDimGL;
                        if(resampleNeeded) {
                                vtkDebugMacro( "Texture too big for gl, maximum is " << maxDimGL);
                        }

                        if(!resampleNeeded && !this->SupportsNonPowerOfTwoTextures) {
                                // xsize and ysize must be a power of 2 in OpenGL
                                xs = static_cast<unsigned int>(xsize);
                                ys = static_cast<unsigned int>(ysize);
                                while (!(xs & 0x01)) {
                                        xs = xs >> 1;
                                }
                                while (!(ys & 0x01)) {
                                        ys = ys >> 1;
                                }
                                // if not a power of two then resampling is required
                                resampleNeeded= (xs>1) || (ys>1);
                        }

                        if(resampleNeeded) {
                                vtkDebugMacro(<< "Resampling texture to power of two for OpenGL");
                                resultData = this->ResampleToPowerOfTwo(xsize, ysize, vptr, bytesPerPixel);
                                pNPOTPixelData = resultData;

                        }

                        if ( resultData == NULL ) {
                                resultData = vptr;
                        }

                        // free any old display lists (from the old context)
                        if (this->RenderWindow) {
                                this->ReleaseGraphicsResources(this->RenderWindow);
                        }
                        this->RenderWindow = ren->GetRenderWindow();
                        if (this->RenderWindow == NULL) {
                                if (pResampledPixelData) {
                                        delete[] pResampledPixelData;
                                }
                                if (pNPOTPixelData) {
                                        delete[] pNPOTPixelData;
                                }
                                return;
                        }

                        // make the new context current before we mess with opengl
                        this->RenderWindow->MakeCurrent();

                        // define a display list for this texture
                        // get a unique display list id

#ifdef GL_VERSION_1_1
                        glGenTextures(1, &tempIndex);
                        this->Index = static_cast<long>(tempIndex);
                        glBindTexture(GL_TEXTURE_2D, this->Index);
#else
                        this->Index = glGenLists(1);
                        glDeleteLists (static_cast<GLuint>(this->Index), static_cast<GLsizei>(0));
                        glNewList (static_cast<GLuint>(this->Index), GL_COMPILE);
#endif
                        //seg fault protection for those wackos that don't use an
                        //opengl render window
                        if(this->RenderWindow->IsA("vtkOpenGLRenderWindow")) {
                                static_cast<vtkOpenGLRenderWindow *>(ren->GetRenderWindow())->
                                RegisterTextureResource( this->Index );
                        }

                        //pixel interpolate
                        if (this->Interpolate) {
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                                                 GL_LINEAR);
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                                                 GL_LINEAR );
                        } else {
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                        }
                        /////repeat texture??
                        if (this->Repeat) {
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT );
                                glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT );
                        } else {
                                if (this->EdgeClamp) {
                                        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                                                         GL_CLAMP_TO_EDGE );
                                        glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
                                                         GL_CLAMP_TO_EDGE );
                                }
                        }

                        // =======================================UPLOAD SHADER ========
                        if( this->UseShader) {
                                GLenum terror = 0;
                                GLint params = 0;

                                //code of the shader
                                if (this->ProgramObject == 0) {
                                        glEnable(GL_FRAGMENT_PROGRAM_ARB);
                                        //upload vertex program code...
                                        this->ProgramObject = glCreateProgram();
                                        terror = glGetError();

                                        this->VertexProgram = glCreateShader(GL_VERTEX_SHADER);
                                        terror = glGetError();

                                        std::string VertexProgramCode =
                                                "void main (void)\n"
                                                "{\n"
                                                "gl_TexCoord[0] = gl_MultiTexCoord0;\n"
                                                "gl_Position = ftransform();\n"
                                                "}\n";
                                        const char* ptr1 = (const char*)VertexProgramCode.c_str();
                                        glShaderSource(this->VertexProgram, 1, &ptr1, NULL);
                                        terror = glGetError();
                                        glCompileShader(this->VertexProgram);
                                        terror = glGetError();

                                        glGetShaderiv(this->VertexProgram, GL_COMPILE_STATUS, &params);

                                        if(params==GL_TRUE) {
                                                //std::cout <<"vertex shader source compiled successfully" << std::endl;
                                        } else {
                                                LOG_ERROR("Visualization", "vertex shader source compile error");
                                                // include null terminator
                                                glGetShaderiv(this->VertexProgram, GL_INFO_LOG_LENGTH, &params);
                                                if(params>0) {
                                                        char *buffer=new char[params];
                                                        glGetShaderInfoLog(this->VertexProgram, params, 0, buffer);
                                                        LOG_ERROR("Visualization", buffer);
                                                        delete[] buffer;
                                                } else {
                                                        LOG_ERROR("Visualization", "No error information available");
                                                }
                                        }

                                        this->FragmentProgram = glCreateShader(GL_FRAGMENT_SHADER);
                                        terror = glGetError();


                                        if (this->RGBImage) { // RGB Frament shader program.
                                                glShaderSource(this->FragmentProgram, sizeof(FragmenProgramCode_rgb) / sizeof (char*), FragmenProgramCode_rgb, 0);
                                        } else { // GrayLevel Frament shader program
                                                glShaderSource(this->FragmentProgram, sizeof(FragmenProgramCode_gray) / sizeof (char*), FragmenProgramCode_gray, 0);
                                        }
                                        
                                        terror = glGetError();
                                        glCompileShader(this->FragmentProgram);
                                        //terror = glGetError();

                                        glGetShaderiv(this->FragmentProgram, GL_COMPILE_STATUS, &params);

                                        if(params==GL_TRUE) {
                                                //std::cout <<"fragment shader source compiled successfully" << std::endl;
                                        } else {
                                                LOG_ERROR("Visualization", "Fragment shader source compile error");
                                                // include null terminator
                                                glGetShaderiv(this->FragmentProgram, GL_INFO_LOG_LENGTH, &params);
                                                if(params>0) {
                                                        char *buffer=new char[params];
                                                        glGetShaderInfoLog(this->FragmentProgram, params, 0, buffer);
                                                        LOG_ERROR("Visualization", buffer);
                                                        delete[] buffer;
                                                } else {
                                                        LOG_ERROR("Visualization", "No error information available");
                                                }
                                        }

                                        glAttachShader(this->ProgramObject, this->VertexProgram);
                                        terror = glGetError();
                                        glAttachShader(this->ProgramObject, this->FragmentProgram);
                                        terror = glGetError();

                                        glLinkProgram(this->ProgramObject);
                                        terror = glGetError();

                                        GLint linked;
                                        glGetProgramiv(this->ProgramObject, GL_LINK_STATUS, &linked);
                                        if (linked) {
                                                //std::cout << "GPU Program enlazado" << std::endl;
                                        }
                                        glDisable(GL_FRAGMENT_PROGRAM_ARB);
                                }

                        }//END PROGRAM SHADER

                        //UPLOAD TEXTURE...
                        if(this->SupportsPBO) {
                                if(this->PBO==0) {
                                        this->PBO=vtkPixelBufferObject::New();
                                        this->PBO->SetContext(renWin);
                                }
                                //std::cout << ">> UpdatePBO" << std::endl;
                                unsigned int dims[2];
                                vtkIdType increments[2];
                                dims[0] = static_cast<unsigned int>(xsize);
                                dims[1] = static_cast<unsigned int>(ysize);
                                increments[0] = 0;
                                increments[1] = 0;

                                if (RGBImage) {
                                        this->PBO->Upload2D(VTK_UNSIGNED_CHAR, resultData, dims, bytesPerPixel, increments);
                                } else {
                                        if (UseShader) {
                                                this->PBO->Upload2D(scalars->GetDataType(), resultData, dims, scalars->GetNumberOfComponents(), increments);
                                        } else {
                                                this->PBO->Upload2D(VTK_UNSIGNED_CHAR, resultData, dims, bytesPerPixel, increments);
                                        }
                                }
                                // non-blocking call
                                this->PBO->Bind(vtkPixelBufferObject::UNPACKED_BUFFER);
                                glTexImage2D( GL_TEXTURE_2D, 0 , TexInternalFormat, xsize, ysize, 0, TexFormat, TexType, 0);
                                this->PBO->UnBind();
                                //std::cout << "<< UpdatePBO" << std::endl;
                        } else {
                                //std::cout << ">> UpdateText(slow). ResultData = " << (void*)resultData << std::endl;
                                // blocking call
                                glTexImage2D( GL_TEXTURE_2D, 0 , TexInternalFormat, xsize, ysize, 0, TexFormat, TexType, resultData);
                                //std::cout << "<< UpdateText(slow)" << std::endl;

                        }
#ifndef GL_VERSION_1_1
                        glEndList ();
#endif
                        // modify the load time to the current time
                        this->LoadTime.Modified();

                        // free memory
                        if (pResampledPixelData) {
                                delete[] pResampledPixelData;
                        }
                        if (pNPOTPixelData) {
                                delete[] pNPOTPixelData;
                        }
                }// end if changes...

                //UPLOAD LOOKUP TABLE IF NEEDED
                bool LUT_NeedsToBeInitialized = !RGBImage && this->LUTIndex == 0;

                if (this->UseShader && ( LUT_NeedsToBeInitialized || LookupTableChanged)) {
                        if (RGBImage) {
                        } else if (LookupTable != NULL) {

                                GLsizei lutNvals = 1024;
                                GLvoid* lutData = NULL;

                                if (LookupTable != NULL) {
                                        vtkLookupTable* LUT = static_cast<vtkLookupTable*>(LookupTable);
                                        lutNvals = LUT->GetNumberOfTableValues();
                                        lutData = LUT->GetPointer(0);
                                } else {
                                        LOG_ERROR("OpenGLComponent", "LUT Not found. Bad initiallization");
                                }

                                //glEnable(GL_TEXTURE_1D);
#if defined(_WINDOWS) || defined(__WXGTK__)
                                glActiveTextureARB(GL_TEXTURE1_ARB);
#else
                                glActiveTexture(GL_TEXTURE1_ARB);
#endif
                                glGenTextures(1, &this->LUTIndex);
                                glBindTexture(GL_TEXTURE_1D, this->LUTIndex);
                                glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                                glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
                                glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S,     GL_CLAMP );
                                glTexParameterf( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T,     GL_CLAMP );

                                glTexImage1D( GL_TEXTURE_1D, 0 , GL_RGBA,
                                              lutNvals, 0, GL_RGBA,
                                              GL_UNSIGNED_BYTE, lutData);
#if defined(_WINDOWS) || defined(__WXGTK__)
                                glActiveTextureARB(GL_TEXTURE0_ARB);
#else
                                glActiveTexture(GL_TEXTURE0_ARB);
#endif
                        }
                }
                LookupTableChanged = false;
        }

        // execute the display list that uses creates the texture
#ifdef GL_VERSION_1_1
        glBindTexture(GL_TEXTURE_2D, this->Index);
#else
        glCallList(this->Index);
#endif

        // don't accept fragments if they have zero opacity. this will stop the
        // zbuffer from be blocked by totally transparent texture fragments.
        glAlphaFunc (GL_GREATER, static_cast<GLclampf>(0));
        glEnable (GL_ALPHA_TEST);

        if (this->PremultipliedAlpha) {
                // save the blend function.
                glPushAttrib(GL_COLOR_BUFFER_BIT);

                // make the blend function correct for textures premultiplied by alpha.
                glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        }

        // now bind it
        if (this->UseShader) {
                if (!this->RGBImage) {
                        glEnable(GL_TEXTURE_1D);
                }
        }
        glEnable(GL_TEXTURE_2D);
        // clear any texture transform
        glMatrixMode(GL_TEXTURE);
        glLoadIdentity();

        // build transformation
        if (this->Transform) {
                double *mat = this->Transform->GetMatrix()->Element[0];
                double mat2[16];
                mat2[0] = mat[0];
                mat2[1] = mat[4];
                mat2[2] = mat[8];
                mat2[3] = mat[12];
                mat2[4] = mat[1];
                mat2[5] = mat[5];
                mat2[6] = mat[9];
                mat2[7] = mat[13];
                mat2[8] = mat[2];
                mat2[9] = mat[6];
                mat2[10] = mat[10];
                mat2[11] = mat[14];
                mat2[12] = mat[3];
                mat2[13] = mat[7];
                mat2[14] = mat[11];
                mat2[15] = mat[15];

                // insert texture transformation
                glMultMatrixd(mat2);
        }
        glMatrixMode(GL_MODELVIEW);

        vtkOpenGLRenderer *oRenderer=static_cast<vtkOpenGLRenderer *>(ren);

        if(oRenderer->GetDepthPeelingHigherLayer()) {
                //std::cout << "DepthPeeling" << std::endl;
        }

        if (this->UseShader) {
                GLenum error = 0;
                GLint loc = 0;

                glEnable(GL_FRAGMENT_PROGRAM_ARB);
                error = glGetError();

                glUseProgram(this->ProgramObject);
                error = glGetError();

                // Common parameters for shaders

                // === imageTexture ===
                loc = glGetUniformLocation(this->ProgramObject,"imagetexture");
                if(loc != -1) {
                        glUniform1i(loc, 0);
                } else {
                        LOG_ERROR("OpenGLComponent", "error: imagetexture is not a uniform");
                }

                if (this->RGBImage) { // Parameters for RGB shader

                        // === brightness ===
                        loc = glGetUniformLocation(this->ProgramObject, "brightness");
                        if(loc != -1) {
                                glUniform1f(loc, Brightness);
                        } else {
                                LOG_ERROR("OpenGLComponent", "error: brightness is not a uniform");
                        }

                        // === contrast ===
                        loc = glGetUniformLocation(this->ProgramObject, "contrast");
                        if(loc != -1) {
                                glUniform1f(loc, Contrast);
                        } else {
                                LOG_ERROR("OpenGLComponent", "error: contrast is not a uniform");
                        }

                } else if (LookupTable != NULL) {
                        // Parameters for LUT shader

                        // === lookupTable ===
                        loc = glGetUniformLocation(this->ProgramObject,"lookuptable");
                        if(loc != -1) {
                                glUniform1i(loc, 1);
                        } else {
                                LOG_ERROR("OpenGLComponent", "error: lut is not a uniform");
                        }

                        double normRange[2] = {0.0, 1.0};
                        vtkDataArray *scalars = this->GetInputArrayToProcess(0, input);
                        scalars->GetRange(normRange);
                        double* drange = LookupTable->GetRange();

                        float range[2] = { static_cast<float>((drange[0]-normRange[0]) / (normRange[1]-normRange[0])),
                                           static_cast<float>((drange[1]-normRange[0]) / (normRange[1]-normRange[0]))
                                         };

                        // === shift ===
                        float shift = -range[0];
                        loc = glGetUniformLocation(this->ProgramObject,"lutShift");
                        if(loc != -1) {
                                glUniform1f(loc, shift);
                        } else {
                                LOG_ERROR("OpenGLComponent", "error: shift is not a uniform");
                        }

                        // === scale ===
                        float scale = (1.0f)/(range[1] - range[0]);
                        loc = glGetUniformLocation(this->ProgramObject,"lutScale");
                        if(loc != -1) {
                                glUniform1f(loc, scale);
                        } else {
                                LOG_ERROR("OpenGLComponent", "error: scale is not a uniform");
                        }
                }

                glDisable(GL_FRAGMENT_PROGRAM_ARB);
        }
}

// ----------------------------------------------------------------------------
void vtkGinkgoOpenGLTexture::PostRender(vtkRenderer *vtkNotUsed(ren))
{

        if (this->UseShader) {
                glUseProgram(0);
                /*

                #if defined(_WINDOWS) || defined(__WXGTK__)
                vtkgl::ActiveTextureARB(vtkgl::TEXTURE1_ARB);
                #else
                glActiveTexture(vtkgl::TEXTURE1_ARB);
                #endif

                glBindTexture(GL_TEXTURE_1D, 0);
                glDisable(GL_TEXTURE_1D);

                #if defined(_WINDOWS) || defined(__WXGTK__)
                vtkgl::ActiveTextureARB(vtkgl::TEXTURE0_ARB);
                #else
                glActiveTexture(vtkgl::TEXTURE0_ARB);
                #endif

                glBindTexture(GL_TEXTURE_2D, 0);
                */
        }
        if (this->GetInput() && this->PremultipliedAlpha) {
                // restore the blend function
                glPopAttrib();
        }
}

// ----------------------------------------------------------------------------
static int FindPowerOfTwo(int i)
{
        int size;

        for ( i--, size=1; i > 0; size*=2 ) {
                i /= 2;
        }

        // [these lines added by Tim Hutton (implementing Joris Vanden Wyngaerd's
        // suggestions)]
        // limit the size of the texture to the maximum allowed by OpenGL
        // (slightly more graceful than texture failing but not ideal)
        GLint maxDimGL;
        glGetIntegerv(GL_MAX_TEXTURE_SIZE,&maxDimGL);
        if ( size > maxDimGL ) {
                size = maxDimGL ;
        }
        // end of Tim's additions

        return size;
}

// ----------------------------------------------------------------------------
// Creates resampled unsigned char texture map that is a power of two in both
// x and y.
unsigned char *vtkGinkgoOpenGLTexture::ResampleToPowerOfTwo(int &xs,
                int &ys,
                unsigned char *dptr,
                int bpp)
{
        unsigned char *tptr, *p, *p1, *p2, *p3, *p4;
        int xsize, ysize, i, j, k, jOffset, iIdx, jIdx;
        double pcoords[3], hx, hy, rm, sm, w0, w1, w2, w3;

        xsize = FindPowerOfTwo(xs);
        ysize = FindPowerOfTwo(ys);
        if (this->RestrictPowerOf2ImageSmaller) {
                if (xsize > xs) {
                        xsize /= 2;
                }
                if (ysize > ys) {
                        ysize /= 2;
                }
        }
        hx = (xs - 1.0) / (xsize - 1.0);
        hy = (ys - 1.0) / (ysize - 1.0);

        tptr = p = new unsigned char[xsize*ysize*bpp];

        // Resample from the previous image. Compute parametric coordinates and
        // interpolate
        for (j=0; j < ysize; j++) {
                pcoords[1] = j*hy;

                jIdx = static_cast<int>(pcoords[1]);
                if ( jIdx >= (ys-1) ) { //make sure to interpolate correctly at edge
                        jIdx = ys - 2;
                        pcoords[1] = 1.0;
                } else {
                        pcoords[1] = pcoords[1] - jIdx;
                }
                jOffset = jIdx*xs;
                sm = 1.0 - pcoords[1];

                for (i=0; i < xsize; i++) {
                        pcoords[0] = i*hx;
                        iIdx = static_cast<int>(pcoords[0]);
                        if ( iIdx >= (xs-1) ) {
                                iIdx = xs - 2;
                                pcoords[0] = 1.0;
                        } else {
                                pcoords[0] = pcoords[0] - iIdx;
                        }
                        rm = 1.0 - pcoords[0];

                        // Get pointers to 4 surrounding pixels
                        p1 = dptr + bpp*(iIdx + jOffset);
                        p2 = p1 + bpp;
                        p3 = p1 + bpp*xs;
                        p4 = p3 + bpp;

                        // Compute interpolation weights interpolate components
                        w0 = rm*sm;
                        w1 = pcoords[0]*sm;
                        w2 = rm*pcoords[1];
                        w3 = pcoords[0]*pcoords[1];
                        for (k=0; k < bpp; k++) {
                                *p++ = static_cast<unsigned char>(p1[k]*w0 + p2[k]*w1 + p3[k]*w2
                                                                  + p4[k]*w3);
                        }
                }
        }

        xs = xsize;
        ys = ysize;

        return tptr;
}

// ----------------------------------------------------------------------------
void vtkGinkgoOpenGLTexture::PrintSelf(ostream& os, vtkIndent indent)
{
        this->Superclass::PrintSelf(os,indent);
        os << indent << "Index: " << this->Index << endl;
}


void vtkGinkgoOpenGLTexture::ForceEnableShaders(bool force)
{
        InternalEnableShaders = force;
}
