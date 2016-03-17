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

#pragma once

/*!
 * Includes
 */
#include <api/api.h>
////@begin includes
#include "wx/wx.h"
#include "wx/splitter.h"
#include "wx/dirctrl.h"
////@end includes

#include "thumbnailctrl.h"

/*!
 * Forward declarations
 */

////@begin forward declarations
class wxGenericDirCtrl;
class wxThumbnailCtrl;
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_IMAGE_BROWSER_DIALOG 21500
#define SYMBOL_wxThumbnailWidget_STYLE wxCAPTION|wxRESIZE_BORDER|wxSYSTEM_MENU|wxCLOSE_BOX
#define SYMBOL_wxThumbnailWidget_TITLE _("Image Browser Dialog")
#define SYMBOL_wxThumbnailWidget_IDNAME ID_IMAGE_BROWSER_DIALOG
#define SYMBOL_wxThumbnailWidget_SIZE wxSize(400, 300)
#define SYMBOL_wxThumbnailWidget_POSITION wxPoint(0, 0)
#define ID_IMAGE_BROWSER_SPLITTER 21501
#define ID_IMAGE_BROWSER_DIRCTRL 21502
#define ID_IMAGE_BROWSER_BROWSER 21503
////@end control identifiers

/*!
 * Compatibility
 */

#ifndef wxCLOSE_BOX
#define wxCLOSE_BOX 0x1000
#endif

/*!
 * wxThumbnailWidget class declaration
 */

class EXTAPI wxThumbnailWidget: public wxControl
{
        DECLARE_DYNAMIC_CLASS( wxThumbnailWidget )
        DECLARE_EVENT_TABLE()

public:
        /// Constructors
        wxThumbnailWidget( );
        wxThumbnailWidget( wxWindow* parent, wxWindowID id = SYMBOL_wxThumbnailWidget_IDNAME, const wxString& caption = SYMBOL_wxThumbnailWidget_TITLE, const wxPoint& pos = SYMBOL_wxThumbnailWidget_POSITION, const wxSize& size = SYMBOL_wxThumbnailWidget_SIZE, long style = SYMBOL_wxThumbnailWidget_STYLE );

        /// Creation
        bool Create( wxWindow* parent, wxWindowID id = SYMBOL_wxThumbnailWidget_IDNAME, const wxString& caption = SYMBOL_wxThumbnailWidget_TITLE, const wxPoint& pos = SYMBOL_wxThumbnailWidget_POSITION, const wxSize& size = SYMBOL_wxThumbnailWidget_SIZE, long style = SYMBOL_wxThumbnailWidget_STYLE );

        /// Creates the controls and sizers
        void CreateControls();

        /// Shows the images in the given dir
        void ShowFolder(const wxString& path);

        /// Accessors

        void SetSelection(const wxString& filename);
        wxString GetSelection() const ;
        wxArrayString GetSelections() const ;

        /// Custom event handlers
        void OnLeftClick(wxThumbnailEvent& event);
        void OnLeftDClick(wxThumbnailEvent& event);

////@begin wxThumbnailWidget event handler declarations

        /// wxEVT_COMMAND_TREELIST_SELECTION_CHANGED event handler for ID_IMAGE_BROWSER_DIRCTRL
        void OnImageBrowserDirctrlSelChanged( wxTreeEvent& event );

////@end wxThumbnailWidget event handler declarations

////@begin wxThumbnailWidget member function declarations

////@end wxThumbnailWidget member function declarations

        /// Should we show tooltips?
        static bool ShowToolTips();

////@begin wxThumbnailWidget member variables
        wxGenericDirCtrl* m_dirCtrl;
        wxThumbnailCtrl* m_imageBrowser;
////@end wxThumbnailWidget member variables

        // Work around calling selection callback twice
        int m_selectionCount;

        // Initial path or filename, if set
        wxString m_initialSelection;
};

