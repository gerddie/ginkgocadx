/*
 *  
 *  $Id: ginkgomatrix4x4.cpp $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#ifndef __DEPRECATED
#define __DEPRECATED
#endif
#include "ginkgomatrix4x4.h"

GNC::GinkgoMatrix4x4::GinkgoMatrix4x4(): rawMatrix(vtkSmartPointer<vtkMatrix4x4>::New())
{
	rawMatrix->Zero();
}
GNC::GinkgoMatrix4x4::GinkgoMatrix4x4(const vtkSmartPointer<vtkMatrix4x4>& raw): rawMatrix(raw)
{
}

GNC::GinkgoMatrix4x4::~GinkgoMatrix4x4()
{
}

void GNC::GinkgoMatrix4x4::Print(std::ostream& os)
{
	rawMatrix->Print(os);
}

void GNC::GinkgoMatrix4x4::DeepCopy(const GNC::GCS::IGinkgoMatrix4x4* other)
{
	const GinkgoMatrix4x4* pMatrix = dynamic_cast<const GinkgoMatrix4x4*>(other);
	if (pMatrix != NULL) {
		rawMatrix->DeepCopy(pMatrix->rawMatrix);
	}
}

void GNC::GinkgoMatrix4x4::Zero()
{
	rawMatrix->Zero();
}

void GNC::GinkgoMatrix4x4::Identity()
{
	rawMatrix->Identity();
}


GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GinkgoMatrix4x4::Invert()
{
	vtkSmartPointer<vtkMatrix4x4> other = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Invert(rawMatrix, other);
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> returnPtr((GNC::GCS::IGinkgoMatrix4x4*)new GinkgoMatrix4x4(other));
	return returnPtr;
}

GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GinkgoMatrix4x4::Transpose()
{
	vtkSmartPointer<vtkMatrix4x4> other = vtkSmartPointer<vtkMatrix4x4>::New();
	vtkMatrix4x4::Transpose(rawMatrix, other);
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> returnPtr((GNC::GCS::IGinkgoMatrix4x4*)new GinkgoMatrix4x4(other));
	return returnPtr;
}

double* GNC::GinkgoMatrix4x4::MultiplyPoint(const double in[4], double out[4])
{
	rawMatrix->MultiplyPoint(in, out);
	return out;
}

GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GinkgoMatrix4x4::Multiply4x4(const GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4>& other)
{
	const GinkgoMatrix4x4* pMatrix = dynamic_cast<const GinkgoMatrix4x4*>(other.GetRawPointer());
	if (pMatrix != NULL) {
		vtkSmartPointer<vtkMatrix4x4> result = vtkSmartPointer<vtkMatrix4x4>::New();
		vtkMatrix4x4::Multiply4x4(rawMatrix, pMatrix->rawMatrix, result);
		GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> returnPtr((GNC::GCS::IGinkgoMatrix4x4*)new GinkgoMatrix4x4(result));
		return returnPtr;
	}
	return NULL;
}

GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GinkgoMatrix4x4::Multiply4x4(const vtkMatrix4x4* other)
{
	vtkSmartPointer<vtkMatrix4x4> result = vtkSmartPointer<vtkMatrix4x4>::New();
	#if defined(_WINDOWS)
	vtkMatrix4x4::Multiply4x4(rawMatrix, other, result);
	#else // Hack: VTK version of linux version requires the second parameter to be const
	vtkSmartPointer<vtkMatrix4x4> copy = vtkSmartPointer<vtkMatrix4x4>::New();
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			copy->SetElement(i,j, other->GetElement(i,j));
		}
	}
	
	vtkMatrix4x4::Multiply4x4(rawMatrix, copy, result);
	#endif
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> returnPtr((GNC::GCS::IGinkgoMatrix4x4*)new GinkgoMatrix4x4(result));
	return returnPtr;
}

GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> GNC::GinkgoMatrix4x4::Adjoint()
{
	vtkSmartPointer<vtkMatrix4x4> other = vtkSmartPointer<vtkMatrix4x4>::New();
	rawMatrix->Adjoint(rawMatrix, other);
	GNC::GCS::Ptr<GNC::GCS::IGinkgoMatrix4x4> returnPtr((GNC::GCS::IGinkgoMatrix4x4*)new GinkgoMatrix4x4(other));
	return returnPtr;
}

double GNC::GinkgoMatrix4x4::Determinant()
{
	return rawMatrix->Determinant();
}

void GNC::GinkgoMatrix4x4::SetElement(int i, int j, double value)
{
	rawMatrix->SetElement(i,j,value);
}

double GNC::GinkgoMatrix4x4::GetElement(int i, int j) const 
{
	return rawMatrix->GetElement(i,j);
}

void GNC::GinkgoMatrix4x4::ToDoubleArrayByColumns(double matrix[16])
{
	for (int i = 0,k=0; i < 4; ++i) {
		for (int j = 0; j<4; ++j,++k) {
			matrix[k] = rawMatrix->GetElement(j,i);
		}
	}
}

