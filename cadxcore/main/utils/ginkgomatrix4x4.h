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

#ifndef GINKGOMATRIX4X4
#define GINKGOMATRIX4X4
#ifndef __DEPRECATED
#define __DEPRECATED
#endif
#include <api/utils/iginkgomatrix4x4.h>
#include <vtkSmartPointer.h>
#include <vtkMatrix4x4.h>

namespace GNC
{
class GinkgoMatrix4x4: public GNC::GCS::IGinkgoMatrix4x4
{
public:
        GinkgoMatrix4x4();
        GinkgoMatrix4x4(const vtkSmartPointer<vtkMatrix4x4>& raw);
        virtual ~GinkgoMatrix4x4();
        virtual void Print(std::ostream& os);
        virtual void DeepCopy(const GNC::GCS::IGinkgoMatrix4x4* other);
        virtual void Zero();
        virtual void Identity();
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Invert();
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Transpose();
        virtual double* MultiplyPoint(const double in[4], double out[4]);
        // Multiplies other to the right result = this * other
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Multiply4x4(const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& other);
        GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Multiply4x4(const vtkMatrix4x4* other);
        // Description:
        // Compute adjoint of the matrix and put it into out.
        virtual GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> Adjoint();
        // Description:
        // Compute the determinant of the matrix and return it.
        virtual double Determinant();
        // Description:
        // Sets the element i,j in the matrix.
        virtual void SetElement(int i, int j, double value);
        // Description:
        // Returns the element i,j from the matrix.
        virtual double GetElement(int i, int j) const;
        virtual void ToDoubleArrayByColumns(double matrix[16]);
protected:
        vtkSmartPointer<vtkMatrix4x4> rawMatrix;
};
}
#endif
