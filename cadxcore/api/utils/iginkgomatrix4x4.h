#pragma once
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

#ifndef IGINKGOMATRIX4X4
#define IGINKGOMATRIX4X4
#include <api/autoptr.h>

namespace GNC
{
namespace GCS
{
class IGinkgoMatrix4x4
{
protected:
        IGinkgoMatrix4x4();
public:
        virtual ~IGinkgoMatrix4x4();

        static GNC::GCS::Ptr<IGinkgoMatrix4x4> New();

        virtual void Print(std::ostream& os) = 0;
        // Description:
        // Non-static member function. Assigns *from* elements array
        virtual void DeepCopy(const IGinkgoMatrix4x4* other) = 0;
        // Description:
        // Set all of the elements to zero.
        virtual void Zero() = 0;
        // Description:
        // Set equal to Identity matrix
        virtual void Identity() = 0;
        // Description:
        // Matrix Inversion (adapted from Richard Carling in "Graphics Gems,"
        // Academic Press, 1990).
        virtual GNC::GCS::Ptr<IGinkgoMatrix4x4> Invert() = 0;
        // Description:
        // Transpose the matrix .
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Transpose() = 0;
        // Description:
        // Multiply a homogeneous coordinate by this matrix, i.e. out = A*in.
        // The in[4] and out[4] can be the same array. Returns a pointer to out
        virtual double* MultiplyPoint(const double in[4], double out[4]) = 0;
        // Description:
        // Multiplies other to the right result = this * other
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Multiply4x4(const GNC::GCS::Ptr<IGinkgoMatrix4x4>& other) = 0;
        // Description:
        // Compute adjoint of the matrix and put it into out.
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>  Adjoint() = 0;
        // Description:
        // Compute the determinant of the matrix and return it.
        virtual double Determinant() = 0;
        // Description:
        // Sets the element i,j in the matrix.
        virtual void SetElement(int i, int j, double value) = 0;
        // Description:
        // Returns the element i,j from the matrix.
        virtual double GetElement(int i, int j) const = 0;

        virtual void ToDoubleArrayByColumns(double matrix[16]) = 0;
};
}
}
#endif
