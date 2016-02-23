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

#include "thumbnailwidget.h"

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>

#ifdef __BORLANDC__
#pragma hdrstop
#endif

////@begin includes
////@end includes

#include <wx/dir.h>
#include <wx/filename.h>
#include "thumbnailwidget.h"
// #include "thumbnailitem.h"
// #include "utils.h"

////@begin XPM images
////@end XPM images

/// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename);


/*!
 * wxThumbnailWidget type definition
 */

IMPLEMENT_DYNAMIC_CLASS( wxThumbnailWidget, wxControl )

/*!
 * wxThumbnailWidget event table definition
 */

BEGIN_EVENT_TABLE( wxThumbnailWidget, wxControl )
    // The correct control is actually the tree control, not the dir control.
    EVT_TREE_SEL_CHANGED( wxID_TREECTRL, wxThumbnailWidget::OnImageBrowserDirctrlSelChanged )

    EVT_THUMBNAIL_LEFT_CLICK(ID_IMAGE_BROWSER_BROWSER, wxThumbnailWidget::OnLeftClick)
    EVT_THUMBNAIL_LEFT_DCLICK(ID_IMAGE_BROWSER_BROWSER, wxThumbnailWidget::OnLeftDClick)

////@begin wxThumbnailWidget event table entries
    EVT_TREE_SEL_CHANGED( ID_IMAGE_BROWSER_DIRCTRL, wxThumbnailWidget::OnImageBrowserDirctrlSelChanged )

////@end wxThumbnailWidget event table entries

END_EVENT_TABLE()

/*!
 * wxThumbnailWidget constructors
 */

wxThumbnailWidget::wxThumbnailWidget( )
{
    m_selectionCount = -1;
}

wxThumbnailWidget::wxThumbnailWidget( wxWindow* parent, wxWindowID id, const wxString& caption, const wxPoint& pos, const wxSize& size, long style )
{
    m_selectionCount = -1;
    Create(parent, id, caption, pos, size, style);
}

/*!
 * wxThumbnailWidget creator
 */

bool wxThumbnailWidget::Create( wxWindow* parent, wxWindowID id, const wxString& /*caption*/, const wxPoint& pos, const wxSize& size, long style )
{
////@begin wxThumbnailWidget member initialisation
    m_dirCtrl = NULL;
    m_imageBrowser = NULL;
////@end wxThumbnailWidget member initialisation

////@begin wxThumbnailWidget creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
	 wxControl::Create(parent, id, pos, size, style);

    CreateControls();
    GetSizer()->Fit(this);
    GetSizer()->SetSizeHints(this);
    Centre();
////@end wxThumbnailWidget creation

    if (!m_initialSelection.IsEmpty())
        SetSelection(m_initialSelection);

    wxSplitterWindow* splitter = (wxSplitterWindow*) FindWindow(ID_IMAGE_BROWSER_SPLITTER);
    // Make the splitter update its sizes now instead of in idle time
    // to reduce on-screen changes
    splitter->UpdateSize();

    return TRUE;
}

/*!
 * Control creation for wxThumbnailWidget
 */

void wxThumbnailWidget::CreateControls()
{
#define EXTRA_STYLES wxTH_TEXT_LABEL|wxTH_IMAGE_LABEL|wxTH_EXTENSION_LABEL|wxTH_MULTIPLE_SELECT

////@begin wxThumbnailWidget content construction

    wxThumbnailWidget* item1 = this;

    wxBoxSizer* item2 = new wxBoxSizer(wxVERTICAL);
    item1->SetSizer(item2);
    item1->SetAutoLayout(TRUE);

    wxBoxSizer* item3 = new wxBoxSizer(wxVERTICAL);
    item2->Add(item3, 1, wxGROW|wxALL, 5);

    wxSplitterWindow* item4 = new wxSplitterWindow( item1, ID_IMAGE_BROWSER_SPLITTER, wxDefaultPosition, wxSize(500, 400), wxSP_3DBORDER|wxSP_3DSASH|wxNO_BORDER|wxCLIP_CHILDREN|wxSP_LIVE_UPDATE  );
    wxGenericDirCtrl* item5 = new wxGenericDirCtrl( item4, ID_IMAGE_BROWSER_DIRCTRL, _T(""), wxDefaultPosition, wxDefaultSize, wxDIRCTRL_DIR_ONLY, _T("All files (*.*)|*.*"), 0 );
    m_dirCtrl = item5;
    wxThumbnailCtrl* item6 = new wxThumbnailCtrl( item4, ID_IMAGE_BROWSER_BROWSER, wxDefaultPosition, wxDefaultSize, wxSUNKEN_BORDER|wxHSCROLL|wxVSCROLL|wxTH_TEXT_LABEL|wxTH_IMAGE_LABEL|wxTH_EXTENSION_LABEL|wxTH_MULTIPLE_SELECT );
    m_imageBrowser = item6;
    item4->SplitVertically(item5, item6, 180);
    item3->Add(item4, 1, wxGROW|wxALL|wxFIXED_MINSIZE, 5);

    wxBoxSizer* item7 = new wxBoxSizer(wxHORIZONTAL);
    item3->Add(item7, 0, wxGROW, 5);

    item7->Add(5, 5, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* item9 = new wxButton( item1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    item9->SetDefault();
    item7->Add(item9, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    wxButton* item11 = new wxButton( item1, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add(item11, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

    wxButton* item10 = new wxButton( item1, wxID_CANCEL, _("&Help"), wxDefaultPosition, wxDefaultSize, 0 );
    item7->Add(item10, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

////@end wxThumbnailWidget content construction
}

/*!
 * Should we show tooltips?
 */

bool wxThumbnailWidget::ShowToolTips()
{
    return TRUE;
}

/*!
 * wxEVT_COMMAND_TREELIST_SELECTION_CHANGED event handler for ID_IMAGE_BROWSER_DIRCTRL
 */

void wxThumbnailWidget::OnImageBrowserDirctrlSelChanged( wxTreeEvent& event )
{
    if (!m_dirCtrl || !m_imageBrowser)
        return;

    m_selectionCount ++;

    if (m_selectionCount > 1)
    {
        m_selectionCount --;
        return;
    }

	wxTreeItemId id = event.GetItem();
    if (id)
    {
        wxDirItemData* data = (wxDirItemData*) m_dirCtrl->GetTreeCtrl()->GetItemData(id);
        if (data && data->m_isDir)
        {
            ShowFolder(data->m_path);
        }
    }

    m_selectionCount --;
}

/// Shows the images in the given dir
void wxThumbnailWidget::ShowFolder(const wxString& path)
{
    wxBusyCursor busy;
    m_imageBrowser->Clear();

    m_imageBrowser->Freeze();
    wxDir dir;
    if (dir.Open(path))
    {
        wxString filename;

        bool cont = dir.GetFirst(&filename, wxT("*.*"), wxDIR_FILES);
        while ( cont )
        {
            wxString file = path + wxFILE_SEP_PATH + filename;
            if (wxFileExists(file) && DetermineImageType(file) != -1)
            {
                m_imageBrowser->Append(new wxImageThumbnailItem(file));
            }

            cont = dir.GetNext(&filename);
        }
    }
    m_imageBrowser->Thaw();
}

/// Custom event handlers
void wxThumbnailWidget::OnLeftClick(wxThumbnailEvent& event)
{
    if (event.GetFlags() & wxTHUMBNAIL_ALT_DOWN)
    {
        int index = event.GetIndex();
        m_imageBrowser->Tag(index, !m_imageBrowser->IsTagged(index));
    }
}

void wxThumbnailWidget::OnLeftDClick(wxThumbnailEvent& WXUNUSED(event))
{
    //EndModal(wxID_OK);
}

void wxThumbnailWidget::SetSelection(const wxString& filename)
{
    if (!m_imageBrowser)
    {
        m_initialSelection = filename;
        return;
    }

    m_selectionCount = 0;

    if (wxDirExists(filename))
    {
        m_dirCtrl->SetPath(filename);
        return;
    }

    wxString path = wxPathOnly(filename);
    wxString file = wxFileNameFromPath(filename);
    m_dirCtrl->SetPath(path);

    m_selectionCount = -1;

    int index = m_imageBrowser->FindItemForFilename(filename);
    if (index > -1)
    {
        m_imageBrowser->Select(index);
        m_imageBrowser->EnsureVisible(index);
    }
}

wxString wxThumbnailWidget::GetSelection() const
{
    int sel = m_imageBrowser->GetSelection();
    if (sel > -1)
    {
        wxThumbnailItem* item = m_imageBrowser->GetItem(sel);
        if (item)
        {
            return item->GetFilename();
        }
    }
    return wxEmptyString;
}

wxArrayString wxThumbnailWidget::GetSelections() const
{
    wxArrayString arr;
    wxArrayInt selections = m_imageBrowser->GetSelections();
    size_t i;
    for (i = 0; i < selections.GetCount(); i++)
    {
        wxThumbnailItem* item = m_imageBrowser->GetItem(selections[i]);
        if (item)
            arr.Add(item->GetFilename());
    }
    return arr;
}

// Returns the image type, or -1, determined from the extension.
static int DetermineImageType(const wxString& filename)
{
    wxString path, name, ext;

    wxFileName::SplitPath(filename, &path, &name, &ext);

    ext.MakeLower();
    if (ext == wxT("jpg") || ext == wxT("jpeg"))
        return wxBITMAP_TYPE_JPEG;
    else if (ext == wxT("gif"))
        return wxBITMAP_TYPE_GIF;
    else if (ext == wxT("bmp"))
        return wxBITMAP_TYPE_BMP;
    else if (ext == wxT("png"))
        return wxBITMAP_TYPE_PNG;
    else if (ext == wxT("pcx"))
        return wxBITMAP_TYPE_PCX;
    else if (ext == wxT("tif") || ext == wxT("tiff"))
        return wxBITMAP_TYPE_TIF;
    else
        return -1;
}


