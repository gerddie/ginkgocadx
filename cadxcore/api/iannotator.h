/*
 *  
 *  $Id: ianotador.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 */
#pragma once
#include <string>
#include <api/math/geometry3d.h>

namespace GNC {
	namespace GCS {
		//region "Forward declarations"
		class Contexto3D;
		class IVista;
		//endregion

		//-----------------------------------------------------------------------------------------------

		class IAnnotator {
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