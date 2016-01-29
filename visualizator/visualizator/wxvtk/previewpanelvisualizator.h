/*
 *  
 *  $Id: previewpanelvisualizator.h $
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