#pragma once
/*
*  
*  $Id: ginkgomatrix4x4.h $
*  Ginkgo CADx Project
*
*  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
*  http://ginkgo-cadx.com
*
*  This file is licensed under LGPL v3 license.
*  See License.txt for details
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

namespace GNC {
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
