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
#include <main/gui/previewpanel/previewpanel.h>
#include <visualizator/estudios/visualizatorstudy.h>

class GVistaCompleja;
namespace GNKVisualizator {
	namespace GUI {
		class wxDnDPreviewPanelVisualizator : public wxDataObjectSimple
		{
		public:
			 wxDnDPreviewPanelVisualizator(long seriesPk = -1);
			 long GetSeriesPk();
			 // implement base class pure virtuals
			 // ----------------------------------
			 virtual size_t GetDataSize() const;
			 virtual bool GetDataHere(void *buf) const;
			 virtual bool SetData(size_t /*len*/, const void *buf);
			 // Must provide overloads to avoid hiding them (and warnings about it)
			 virtual size_t GetDataSize(const wxDataFormat&) const;
			 virtual bool GetDataHere(const wxDataFormat&, void *buf) const;
			 virtual bool SetData(const wxDataFormat&, size_t len, const void *buf);

		private:
			 long SeriesPk;
			 DECLARE_NO_COPY_CLASS(wxDnDPreviewPanelVisualizator)
		};

		class PreviewPanelVisualizator: public GNC::GUI::PreviewPanel
		{
		public:
			PreviewPanelVisualizator(GNC::GCS::IVista* pView, GVistaCompleja* pParent);
			~PreviewPanelVisualizator();

			void LoadModel();
			
			virtual void ProcesarEvento(GNC::GCS::Events::IEvent* evt) ;

			GNC::GUI::IPreviewNode* pRoot;
		};
	}
}
