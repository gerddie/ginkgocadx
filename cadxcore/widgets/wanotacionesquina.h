/*
 *  
 *  $Id: wanotacionesquina.h $
 *  Ginkgo CADx Project
 *
 *  Copyright 2008-14 MetaEmotion S.L. All rights reserved.
 *  http://ginkgo-cadx.com
 *
 *  This file is licensed under LGPL v3 license.
 *  See License.txt for details
 *
 *
 */
#pragma once
#include <api/iwidgets.h>
#include <api/api.h>
#include <api/observers/ieventsobserver.h>
#include <vector>
#include <list>
#include "openglhelper.h"

#define TAMFUENTE_MIN 10.0f
#define PROPVIEWPORT 0.40f // Proporcion maxima del viewport, tanto de ancho como de alto. (25%)

namespace GNC {
	namespace GCS {

		namespace Widgets {

			class EstadoInterno;
			class OrientationMarks;

			//----------------------------------------------------------------------------------------------------
			//region Widget anotador
			class EXTAPI WAnotador : public GNC::GCS::Widgets::IWidget {
			public:

				WAnotador(IWidgetsManager* pManager, long vid, const char* nombre, long gid);

				~WAnotador();

				virtual void LiberarRecursos(GNC::GCS::IWidgetsRenderer* renderer);
			//endregion

			//----------------------------------------------------------------------------------------------------
			//region "Interfaz generica"
			public:
				virtual void OnMouseEvents(GNC::GCS::Events::EventoRaton&);

				virtual void OnKeyEvents(GNC::GCS::Events::EventoTeclado&);

				virtual bool HitTest(float x, float y, const GNC::GCS::Vector& umbral);

				virtual bool HitTest(GNC::GCS::Vector* vertices, int numVertices);

				virtual void Render(GNC::GCS::Contexto3D* c);

				virtual void OffscreenRender(Contexto3D* c);
				
				virtual void Seleccionar(bool seleccionado);

				virtual void Iluminar(bool iluminado);

				virtual void Ocultar(bool oculto);
				
				virtual void RecalcularPosicion(OrientationMarks* pAnotaciones, GNC::GCS::Contexto3D* c);

				virtual void ComputeOrientation(GNC::GCS::Contexto3D* c, std::ostringstream* dirs);

			//endregion

				//----------------------------------------------------------------------------------------------------
				//region "Interfaz especifica"
			public:
				virtual void SetTextColor(const GNC::GCS::GLHelper::TColor& color);				
				//endregion

			public:

				typedef enum TPosicionAnotacion {
					TP_TopLeft = 0,
					TP_TopRight,
					TP_BottomLeft,
					TP_BottomRight,
					TP_Top,
					TP_Right,
					TP_Bottom,
					TP_Left,
					TP_NumPosiciones
				} TPosicionAnotacion;

				//endregion


			

			//endregion

			//----------------------------------------------------------------------------------------------------
			//region "Estado interno"
			protected:

				EstadoInterno* m_Estado;				
				GNC::GCS::GLHelper::TColor m_color;
				
			//endregion

			};
			//endregion


			/**
			* Gets the orientation index given string. Default is zero.
			* strPos: The patient position string (HFS, HFP...)
			* defautlunknown: When orientation index is empty, set the default index to unknown ("?") or empty ("") values
			**/
			int EXTAPI GetPatientPositionOrientationIndex(const std::string& strPos, bool defaultunknown);

			/**
			* Gets the patient orientation char ("R", "L", "S"...) from:
			* patientPosIndex: The patient position index: get if from GetOrientationIndex() call.
			* axis: The axis: 0==x, 1==y, 2==z
			* dir: The direction: -1 == decreasing, +1 == decreasing
			**/
			const char* EXTAPI GetPatientPositionOrientationChar(int patientPosIndex, int axis, int dir);
		}
	}
}
