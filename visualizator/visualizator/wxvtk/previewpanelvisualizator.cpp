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


#include <wx/dnd.h>
#include <main/gui/previewpanel/previewpanelmodel.h>
#include <main/gui/previewpanel/previewpanel.h>
#include <api/controllers/ihistorycontroller.h>
#include <visualizator/wxvtk/gvistacompleja.h>
#include <api/dicom/dcmdictionary.h>
#include <api/internationalization/internationalization.h>
#include <eventos/scgenerated.h>
#include <api/controllers/ieventscontroller.h>
#include <api/ivista.h>
#include <resources/ginkgoresourcesmanager.h>
#include <main/gui/pacsupload/pacsuploaddialog.h>
#include <main/controllers/commandcontroller.h>
#include <commands/sendpacscommand.h>

#include "previewpanelvisualizator.h"

#define PACS_UPLOAD 2

namespace GNKVisualizator
{
namespace GUI
{
//////////////////////BEGIN D&D/////////////////////////
wxDnDPreviewPanelVisualizator::wxDnDPreviewPanelVisualizator(long seriesPk)
#if defined(_WINDOWS)
        : wxDataObjectSimple(wxDF_PRIVATE)
#else
        : wxDataObjectSimple(wxDF_UNICODETEXT)
#endif
{
        SeriesPk = seriesPk;
}
long wxDnDPreviewPanelVisualizator::GetSeriesPk()
{
        return SeriesPk;
}
// implement base class pure virtuals
// ----------------------------------
size_t wxDnDPreviewPanelVisualizator::GetDataSize() const
{
        return sizeof(SeriesPk);
}
bool wxDnDPreviewPanelVisualizator::GetDataHere(void *buf) const
{
        (*((long*)buf)) = SeriesPk;
        return true;
}
bool wxDnDPreviewPanelVisualizator::SetData(size_t /*len*/, const void *buf)
{
        SeriesPk = (*(const long*)buf);
        return true;
}
// Must provide overloads to avoid hiding them (and warnings about it)
size_t wxDnDPreviewPanelVisualizator::GetDataSize(const wxDataFormat&) const
{
        return GetDataSize();
}
bool wxDnDPreviewPanelVisualizator::GetDataHere(const wxDataFormat&, void *buf) const
{
        return GetDataHere(buf);
}
bool wxDnDPreviewPanelVisualizator::SetData(const wxDataFormat&, size_t len, const void *buf)
{
        return SetData(len, buf);
}
//////////////////////END D&D/////////////////////////


class IPreviewNodeVisualizator : public GNC::GUI::IPreviewNode
{
public:
        IPreviewNodeVisualizator(const std::string& label): GNC::GUI::IPreviewNode(label)
        {
        }
        void OnShowMenu(wxWindow* /*pParent*/)
        {
        }
        void OnPreviewDClick(wxWindow* /*pParent*/)
        {
        }
        virtual void OnPreviewLDown(wxWindow* /*pParent*/)
        {
        }
};

//////////////////////BEGIN MENU/////////////////////////
class EXTAPI wxLeafPanelDefaultHandler: public wxEvtHandler
{
public:
        wxLeafPanelDefaultHandler(wxEvtHandler* pParent, GVistaCompleja* view, long seriesPk):
                GView(view),
                SeriesPk(seriesPk),
                m_pParent(pParent)

        {
                // Connect Events
                m_pParent->Connect(PACS_UPLOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxLeafPanelDefaultHandler::OnPacsUpload),NULL,this);
        }
        virtual ~wxLeafPanelDefaultHandler()
        {
                m_pParent->Disconnect(PACS_UPLOAD, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( wxLeafPanelDefaultHandler::OnPacsUpload),NULL,this);
        }

        virtual void OnPacsUpload(wxCommandEvent& /*event*/)
        {
                std::list<long> viewSeries;
                viewSeries.insert(viewSeries.begin(), GView->GetView()->GetEstudio()->OpenedSeries.begin(), GView->GetView()->GetEstudio()->OpenedSeries.end());
                std::list<long> selectedSeries;
                selectedSeries.push_back(SeriesPk);

                GNC::GUI::PACSUploadDialog dlg(GView->GetView()->GetWindow(), viewSeries, selectedSeries);

                if (dlg.ShowModal() == wxID_OK) {
                        std::list<long> listOfSeries;
                        listOfSeries = dlg.GetSelectedSeries();
                        if (!listOfSeries.empty()) {
                                GADAPI::SendPACSCommand* pCmd = new GADAPI::SendPACSCommand(dlg.GetSelectedServer(), "", listOfSeries);
                                GNC::GCS::ICommandController::Instance()->ProcessAsync(_Std("Uploading to the PACS ..."),pCmd, NULL);
                        }
                }
        }

protected:
        GVistaCompleja* GView;
        long SeriesPk;
        wxEvtHandler* m_pParent;
};
//////////////////////END MENU/////////////////////////

class IPreviewLeafVisualizator : public GNC::GUI::IPreviewLeaf
{
public:
        IPreviewLeafVisualizator(GVistaCompleja* view, long seriesPk, const std::string& label, const std::string& label2): GNC::GUI::IPreviewLeaf(label, label2)
        {
                this->VisualizatorView = view;
                this->seriesPk = seriesPk;
        }
        virtual void OnShowMenu(wxWindow* pParent)
        {
                wxMenu popupMenu;
                wxMenuItem* pItem = popupMenu.Append(PACS_UPLOAD, _("Upload series to PACS..."));
#ifdef __WXMSW__
                pItem->SetBitmaps(GinkgoResourcesManager::MenuIcons::GetIcoSendToPACS());
#else
                pItem->SetBitmap(GinkgoResourcesManager::MenuIcons::GetIcoSendToPACS());
#endif
                wxLeafPanelDefaultHandler menuHandler(pParent, VisualizatorView, seriesPk);
                pParent->PopupMenu(&popupMenu);
        }
        virtual void OnPreviewDClick(wxWindow* /*pParent*/)
        {
                VisualizatorView->LoadSeries(seriesPk);
        }
        virtual void OnPreviewLDown(wxWindow* pParent)
        {
                wxDnDPreviewPanelVisualizator myData(seriesPk);
                wxDropSource dragSource(myData,pParent);
                wxDragResult result = dragSource.DoDragDrop(wxDrag_CopyOnly);
                if (result == wxDragCopy) {
                }
        }

        GVistaCompleja* VisualizatorView;
        long seriesPk;
};
}
}


GNKVisualizator::GUI::PreviewPanelVisualizator::PreviewPanelVisualizator(GNC::GCS::IVista* pView, GVistaCompleja* pParent) : GNC::GUI::PreviewPanel(pView, pParent)
{
        pRoot = NULL;
        GNC::GCS::IEventsController::Instance()->Registrar(this, GNC::GCS::Events::SCGeneratedEvent(m_pView));
}

GNKVisualizator::GUI::PreviewPanelVisualizator::~PreviewPanelVisualizator()
{
        if (pRoot != NULL) {
                delete pRoot;
        }
}


void GNKVisualizator::GUI::PreviewPanelVisualizator::LoadModel()
{
        if (pRoot != NULL) {
                delete pRoot;
        }

        GNC::GCS::Ptr<GNC::GCS::IStudyContext>& pStudy = m_pView->GetEstudio();
        GVistaCompleja* GView = dynamic_cast<GVistaCompleja*>(GetParent());

        pRoot = new GNKVisualizator::GUI::IPreviewNodeVisualizator(std::string(wxString::Format(_("%d series"), (int) (pStudy->OpenedSeries.size())).ToUTF8()));
        for (std::vector<long>::iterator it = pStudy->OpenedSeries.begin(); it != pStudy->OpenedSeries.end(); ++it) {
                GNC::GCS::IHistoryController::SeriesModel seriesModel = GNC::GCS::IHistoryController::Instance()->GetSeriesModel((*it));
                GNC::GCS::IHistoryController::FileModel fileModel = GNC::GCS::IHistoryController::Instance()->GetFrameOfReference((*it));
                std::ostringstream ostr;
                ostr << seriesModel.series_modality << " (" << seriesModel.num_instances << ")";
                GNC::GUI::IPreviewLeaf* pLeaf = new GNKVisualizator::GUI::IPreviewLeafVisualizator(GView, (*it), seriesModel.series_desc, ostr.str());
                pLeaf->SetFilePk(fileModel.pk);
                pRoot->AddChild(pLeaf);
        }
        GNC::GUI::PreviewPanel::LoadModel(pRoot);
}



void GNKVisualizator::GUI::PreviewPanelVisualizator::ProcesarEvento(GNC::GCS::Events::IEvent* evt)
{
        GNC::GUI::PreviewPanel::ProcesarEvento(evt);

        switch(evt->GetCodigoEvento()) {
        case ginkgoEVT_Core_SCGenerated: {
                GNC::GCS::Events::SCGeneratedEvent* pSCGenEvt = dynamic_cast<GNC::GCS::Events::SCGeneratedEvent*>(evt);
                if(pSCGenEvt != NULL) {
                        GNC::GCS::Ptr<GNC::GCS::IStudyContext>& pStudy = m_pView->GetEstudio();
                        for (std::list<long>::const_iterator itNew = pSCGenEvt->GetSeriesId().begin(); itNew != pSCGenEvt->GetSeriesId().end(); ++itNew) {
                                bool newSeries = true;
                                for (std::vector<long>::const_iterator itOpened = pStudy->OpenedSeries.begin();  itOpened != pStudy->OpenedSeries.end(); ++itOpened) {
                                        if ((*itOpened) == (*itNew)) {
                                                newSeries = false;
                                                break;
                                        }
                                }
                                if (newSeries) {
                                        pStudy->OpenedSeries.push_back((*itNew));
                                }
                        }
                        LoadModel();
                }
        }
        break;
        }
}

