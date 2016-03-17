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

#include <iostream>
#include <string>

template <class TPixel>
class ImgProxy
{
private:
        bool automem;

public:
        int anchura, altura, size;
        TPixel *data;

        ImgProxy()
        {
                this->anchura = 0;
                this->altura = 0;
                this->size = 0;
                this->data = NULL;
                this->automem = false;
                //std::cout << "ImgProxy[" << this << "] creado data = " << data << ", automem=" << automem << std::endl;
        }

        ImgProxy(const ImgProxy&o)
        {
                (*this) = o;
        }

        ImgProxy(int anchura, int altura, TPixel *data, bool automem = false)
        {
                this->anchura = anchura;
                this->altura = altura;
                this->size = anchura * altura;
                this->data = data;
                this->automem = automem;
                //std::cout << "ImgProxy[" << this << "] creado data = " << data << ", automem=" << automem << std::endl;
        }

        ImgProxy(int anchura, int altura)
        {
                this->anchura = anchura;
                this->altura = altura;
                this->size = anchura * altura;
                this->data = new TPixel[anchura*altura];
                this->automem = true;
                //std::cout << "ImgProxy[" << this << "] creado data = " << data << ", automem=" << automem << std::endl;
        }

        ~ImgProxy()
        {
                //std::cout << "ImgProxy[" << this << "] destruido data = " << data << ", automem=" << automem << std::endl;
                if (automem && this->data) {
                        //std::cout << "ImgProxy destruido" << std::endl;
                        delete[] this->data;
                        this->data = NULL;
                }

        }

        ImgProxy& operator=(const ImgProxy& o)
        {
                //std::cout << "PELIGRO!!!!! ImgProxy[" << this << "] asignado data = " << data << ", automem=" << automem << std::endl;
                anchura = o.anchura;
                altura = o.altura;
                size = o.size;
                if (o.automem) {

                        unsigned int size = anchura*altura;
                        data = new TPixel[size];
                        for (unsigned int i = 0; i < size; ++i) {
                                data[i] = o.data[i];
                        }
                        automem = true;
                } else {
                        data = o.data;
                        automem = false;
                }
                return *this;
        }

        inline void Fill(TPixel v)
        {
                for (int i = 0; i < this->size; ++i) {
                        this->data[i] = v;
                }
        }

        inline TPixel &Pixel(int x, int y)
        {
                return this->data[y * anchura + x];
        }

        inline const TPixel &Pixel(int x, int y) const
        {
                return this->data[y * anchura + x];
        }
};

#ifndef UCHAR
typedef unsigned char UCHAR;
#endif

#ifndef CHAR
typedef char CHAR;
#endif

typedef struct UCHAR3 {
        unsigned char v1, v2, v3;

        UCHAR3() : v1(0), v2(0), v3(0)
        { }

        UCHAR3(unsigned char _v1, unsigned char _v2, unsigned char _v3) : v1(_v1), v2(_v2), v3(_v3)
        { }
} UCHAR3;


typedef struct UCHAR4 {
        unsigned char v1, v2, v3, v4;

        UCHAR4() : v1(0), v2(0), v3(0), v4(0)
        { }

        UCHAR4(unsigned char _v1, unsigned char _v2, unsigned char _v3, unsigned char _v4) : v1(_v1), v2(_v2), v3(_v3), v4(_v4)
        { }
} UCHAR4;

typedef struct FLOAT3 {
        float v1, v2, v3;

        FLOAT3() : v1(0.0f), v2(0.0f), v3(0.0f)
        { }

        FLOAT3(float _v1, float _v2, float _v3) : v1(_v1), v2(_v2), v3(_v3)
        { }
} FLOAT3;


typedef struct FLOAT4 {
        float v1, v2, v3, v4;

        FLOAT4() : v1(0.0f), v2(0.0f), v3(0.0f), v4(0.0f)
        { }

        FLOAT4(float _v1, float _v2, float _v3, float _v4) : v1(_v1), v2(_v2), v3(_v3), v4(_v4)
        { }
} FLOAT4;
