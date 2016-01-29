/*
 *  
 *  $Id: visualizatorstudy.h $
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
#include "overlay.h"
#include <api/istudycontext.h>

class wxWindow;
class wxSizer;
class wxXmlNode;
class wxXmlDocument;

namespace GNKVisualizator {
	class VisualizatorStudy : public GNC::GCS::IStudyContext
	{
	public:
		
		VisualizatorStudy();
		~VisualizatorStudy();

		void InitializeContext(long seriesPk);

		//overlays...
		bool TieneOverlaysImagenActiva();
		bool TieneOverlaysImagen(const int indiceImagen);
		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays> GetOverlaysImagenActiva();
		GNC::GCS::Ptr<GNKVisualizator::TListaOverlays> GetOverlaysImagen(const int indice);

		virtual void CallbackCargarTagsImagen(const int /*indice*/, GIL::DICOM::IDICOMManager* /*pDicomManager*/) {}

	protected:

		std::vector<GNC::GCS::Ptr<TListaOverlays> >                ListaOverlays;
	};
}
