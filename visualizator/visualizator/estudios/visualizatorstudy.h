/*
 *  Ginkgo CADx Project
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
