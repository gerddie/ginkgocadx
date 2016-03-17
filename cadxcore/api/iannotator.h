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
#include <string>
#include <api/math/geometry3d.h>

namespace GNC
{
namespace GCS
{
//region "Forward declarations"
class Contexto3D;
class IVista;
//endregion

//-----------------------------------------------------------------------------------------------

class IAnnotator
{
public:
        IAnnotator(GNC::GCS::IVista *pView);
        virtual ~IAnnotator();

        /* special annotation values like ${NEWLINE}*/
        virtual std::string GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key);



        /* tag syntax is the same as defined in api/dicom/dcmdictionary.h   gggg|eeee*/
        virtual std::string GetTagValue(GNC::GCS::Contexto3D* c, const std::string& key, const std::string& defValue = "", const std::string& format ="");

        //el comportamiento por defecto de las anotaciones con posicion es el mismo que las de sin posicion
        //si se desean pueden sobreescribirse
        virtual std::string GetPositionAnnotation(GNC::GCS::Contexto3D* c, const GNC::GCS::Vector3D& wP);
protected:
        std::string FormatTag(const std::string& value, const std::string& format);
        std::string GetFloatingPointTagWithPrecision(const std::string& key, const std::string& defValue, int precision);
        bool ContainsTag(const std::string& key);

        GNC::GCS::IVista* m_pView;
};
}
}
