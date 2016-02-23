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

#include "iannotator.h"
#include <api/westilo.h>
#include <api/icontexto.h>
#include <api/iwidgetsrenderer.h>
#include <api/iwidgetsmanager.h>
#include <api/ivista.h>
#include <api/istudycontext.h>
#include <main/entorno.h>
#include <vtkginkgoimageviewer.h>
#include <api/dicom/dcmdictionary.h>
#include <api/internationalization/internationalization.h>

#include <wx/string.h>
#include <wx/translation.h>
#include <wx/datetime.h>

#include <vtkRenderWindow.h>
#include <vtkImageData.h>

GNC::GCS::IAnnotator::IAnnotator(GNC::GCS::IVista *pView): m_pView(pView)
{
}

GNC::GCS::IAnnotator::~IAnnotator()
{
}

std::string GNC::GCS::IAnnotator::GetAnnotationValue(GNC::GCS::Contexto3D* c, const std::string& key)
{
	if (key == "${NEWLINE}") {
		return "\n";
	} else if (key =="${GINKGO.VERSION.FULLTEXT}") {
		return GNC::Entorno::Instance()->GetGinkgoVersionString();
	} else if (key =="${GINKGO.ARCH}") {
		if (sizeof(void*) == 4) {
			return "32 bits";
		}
		else {
			return "64 bits";
		}
	} else if (key == "${VOLUME.IDX}") {
		if (m_pView != NULL && m_pView != NULL && m_pView->GetEstudio().IsValid()) {
			std::ostringstream ostr;
			ostr << m_pView->GetEstudio()->ActiveFileIndex + 1;
			return ostr.str();
		}
	} else if (key == "${SLICE.WIDTH}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			int dims[3];
			c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
			std::ostringstream ostr;
			ostr << dims[0];
			return ostr.str();
		}
	} else if (key == "${SLICE.HEIGHT}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			int dims[3];
			c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
			std::ostringstream ostr;
			ostr << dims[1];
			return ostr.str();
		}
	} else if (key == "${SLICE.WL.FULLTEXT}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			std::ostringstream ostr;
			ostr << "WL: ";
			ostr << std::setiosflags(ios::fixed) << std::setprecision(ANNOTATION_FP_PRECISION);
			ostr << c->GetRenderer()->m_pImageViewer->GetWindow() << "/" << c->GetRenderer()->m_pImageViewer->GetLevel();
			return ostr.str();
		}
	} else if (key == "${SLICE.IDX}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			std::ostringstream ostr;
			ostr << c->GetRenderer()->m_pImageViewer->GetTindex() + 1;
			return ostr.str();
		}
	} else if (key == "${SLICE.COUNT}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			int dims[3];
			c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
			std::ostringstream ostr;
			ostr << dims[2];
			return ostr.str();
		}
	} else if (key == "${SLICE.IDX.FULLTEXT}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL && m_pView != NULL && m_pView->GetEstudio().IsValid()) {
			int dims[3];
			c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
			std::ostringstream ostr;
			ostr << _Std("Slice") << ": " <<  c->GetRenderer()->m_pImageViewer->GetTindex() + 1 << " " << _Std("of") << " " << dims[2] << std::endl << _Std("Volume") << ": " << m_pView->GetEstudio()->ActiveFileIndex + 1;
			return ostr.str();
		}
	} else if (key == "${SLICE.VOXEL.SIZE}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL && m_pView != NULL && m_pView->GetEstudio().IsValid()) {
			double spacing[3];
			bool hasSpacing = m_pView->GetEstudio()->GetSpacingActive(spacing[0], spacing[1], spacing[2]);
			std::ostringstream ostr;
			if (hasSpacing) {
				ostr << std::setiosflags(ios::fixed) << std::setprecision(ANNOTATION_FP_PRECISION);
				ostr << _Std("Voxel:") << spacing[0] << "x" << spacing[1] << " mm.";
			}
			else {
				ostr << _Std("Image without metric information");
			}
			return ostr.str();
		}
	} else if (key == "${SLICE.THICKNESS}") {
		return GetFloatingPointTagWithPrecision(GKDCM_SliceThickness, "N/A", ANNOTATION_FP_PRECISION);		
	}  else if (key == "${SLICE.THICKNESS.FULLTEXT}") {
		if(ContainsTag(GKDCM_SliceThickness)) {
			std::ostringstream ostr;
			ostr << _Std("Slice thickness") << ": " << GetFloatingPointTagWithPrecision(GKDCM_SliceThickness, "N/A", ANNOTATION_FP_PRECISION) << " mm.";
			return ostr.str();
		}
	} else if (key == "${SLICE.LOCATION}") {
		return GetFloatingPointTagWithPrecision(GKDCM_SliceLocation, "N/A", ANNOTATION_FP_PRECISION);
	}  else if (key == "${SLICE.LOCATION.FULLTEXT}") {
		if(ContainsTag(GKDCM_SliceLocation)) {
			std::ostringstream ostr;
			ostr << _Std("Slice location") << ": " << GetFloatingPointTagWithPrecision(GKDCM_SliceLocation, "N/A", ANNOTATION_FP_PRECISION);
			return ostr.str();
		}
	} else if (key == "${VIEW.SCALE}") {
		if (c->GetRenderer() != NULL && c->GetRenderer()->m_pImageViewer != NULL) {
			double ppmm =  (c->GetRenderer()->m_pImageViewer->GetRenderWindow()->GetDPI() / 25.4); // pixels per mm
			double mmpp =  1.0 / ppmm; // mm per pixel
			double spacing[3];
			c->GetRenderer()->m_pImageViewer->GetSpacing(spacing);
			double onePixelHeight = c->GetRenderer()->m_pImageViewer->GetZoomFactor();
			std::ostringstream ostr;
			ostr << std::setiosflags(ios::fixed) << std::setprecision(ANNOTATION_FP_PRECISION);
			ostr << _Std("Scale") << " 1:" << ( (spacing[1] * mmpp) / onePixelHeight);
			return ostr.str();
		}
	} else if (key == "${SLICE.LATERALITY}") {
		std::string lat = GetTagValue(c, GKDCM_ImageLaterality);
		if(lat != "") {
			std::ostringstream ostr;
			if(lat=="R"){
				ostr << _Std("Right") << ";" ;
			}else if(lat=="L"){
				ostr << _Std("Left") << ";";
			}
			return ostr.str();
		}
	}
	
	return "";
}

std::string GNC::GCS::IAnnotator::FormatTag(const std::string& value, const std::string& format)
{
	if (format.empty())
	{
		return value;
	} 
	else if (format == "date")
	{
		wxDateTime date;
		date.ParseFormat(wxString::FromUTF8(value.c_str()),wxT("%Y%m%d"));
		if (date.IsValid()) {
			return std::string(date.Format(_("%Y-%m-%d")).ToUTF8());
		}
	}
	else if (format == "time")
	{
		wxDateTime time;
		time.ParseFormat(wxString::FromUTF8(value.c_str()),wxT("%H%M%S"));
		if(time.IsValid()){
			return std::string(time.Format(_("%H:%M:%S")).ToUTF8());
		}
	}
	//unknown format, or value hasn't right format
	return value;
}

std::string GNC::GCS::IAnnotator::GetFloatingPointTagWithPrecision(const std::string& key, const std::string& defValue, int precision)
{
	if (m_pView != NULL && m_pView->GetEstudio().IsValid()) {
		std::string value;
		if (m_pView->GetEstudio()->GetTagActiveImage(key, value) && !value.empty()) {

			double fpVal = 0.0f;

			std::istringstream is(value);
			is >> fpVal;

			std::ostringstream ostr;
			ostr << std::setiosflags(ios::fixed) << std::setprecision(precision);
			ostr << fpVal;
			return ostr.str();
		} 
	} 
	return defValue;
}

bool GNC::GCS::IAnnotator::ContainsTag(const std::string& key)
{
	if (m_pView != NULL && m_pView->GetEstudio().IsValid()) {
		std::string value;
		if (m_pView->GetEstudio()->GetTagActiveImage(key, value) && !value.empty()) {
			return true;
		} 
	} 
	return false;
}

std::string GNC::GCS::IAnnotator::GetTagValue(GNC::GCS::Contexto3D* /*c*/, const std::string& key, const std::string& defValue, const std::string& format)
{
	if (m_pView != NULL && m_pView->GetEstudio().IsValid()) {
		std::string value;
		if (m_pView->GetEstudio()->GetTagActiveImage(key, value) && !value.empty()) {
			return FormatTag(value, format);
		} 
	} 
	return defValue;
}

std::string GNC::GCS::IAnnotator::GetPositionAnnotation(GNC::GCS::Contexto3D* c, const GNC::GCS::Vector3D& wP)
{
	if (c->pRenderer == NULL || c->pRenderer->m_pImageViewer == NULL) {
		return "";
	}
	std::ostringstream os;
	os.setf(ios::floatfield, ios::fixed );
	os.precision(2);
	os.fill('0');

	//double zpos = SeriesViewer->GetPipeline()->GetCurrentSliceIndex();

	double ip[3] = {0,0,0};
	c->pRenderer->m_pImageViewer->Proyect2D(wP.v, ip);
	int pp[3] = {0, 0, 0};
	int dims[3];

	c->GetRenderer()->m_pImageViewer->GetDimensions(dims);
				
	os << _Std("Position:") << " W:(" << wP.x << ", " << wP.y << "," << wP.z << "); I:(" << ip[0] << ", " << ip[1] << ", " << ip[2] << ")";

	//corregir de convencion DICOM a convenciotk

	c->GetRenderer()->m_pImageViewer->CoordenadasImagenACoordenadasPixel(ip, pp);
	if (pp[0]>=0 && pp[0] < dims[0] && pp[1]>=0 && pp[1] < dims[1] && pp[2] >=0 && pp[2] < dims[2]) {
		const vtkSmartPointer<vtkImageData>& pImg = c->GetRenderer()->m_pImageViewer->GetDataObject();
		//image is not updated...
		if (pImg == NULL || pImg->GetNumberOfPoints() == 0 || pImg->GetDimensions()[0] != dims[0] || pImg->GetDimensions()[1] != dims[1] )
		{
			return "";
		}
		void *scalar = pImg->GetScalarPointer( (int)pp[0], (int)pp[1], (int)pp[2] );		

		if( scalar ) {
			
			bool primero = true;
			double value = 0.0f;

			for(int i = 0; i< c->GetRenderer()->m_pImageViewer->GetNumberOfComponents(); i++) {				
				switch (c->GetRenderer()->m_pImageViewer->GetInput()->GetScalarType()) {
					case VTK_DOUBLE:
						value = (((double*)scalar)[i]);
						break;
					case VTK_FLOAT:
						value = (double) (((float*)scalar)[i]);
						break;
					case VTK_UNSIGNED_LONG:
						value = (double) (((unsigned long*)scalar)[i]);
						break;
					case VTK_LONG:
						value = (double) (((long*)scalar)[i]);
						break;
					case VTK_UNSIGNED_INT:
						value = (double) (((unsigned int*)scalar)[i]);
						break;
					case VTK_INT:
						value = (double) (((int*)scalar)[i]);
						break;
					case VTK_UNSIGNED_SHORT:
						value = (double) (((unsigned short*)scalar)[i]);
						break;
					case VTK_SHORT:
						value = (double) (((short*)scalar)[i]);
						break;
					case VTK_UNSIGNED_CHAR:
						value = (double) (((unsigned char*)scalar)[i]);
						break;
					case VTK_CHAR:
						value = (double) (((char*)scalar)[i]);
						break;
				}

				if(primero) {
					os << " " << _Std("Value:") << value;
				} else {
					os << "," << value;
				}
				primero = false;
			}
		}
	}

	return os.str();
}
