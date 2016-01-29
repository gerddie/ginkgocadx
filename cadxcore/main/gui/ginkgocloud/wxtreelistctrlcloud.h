/*
 *  
 *  $Id: wxtreelistctrlacquisition.h $
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

#include <wx/intl.h>

#include <wx/panel.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/srchctrl.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/treelist.h>
#include <wx/link_additions.h>
#include <wx/radiobut.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/statbox.h>
#include <wx/gbsizer.h>
#include <wx/button.h>
#include <wx/grid.h>
#include <wx/notebook.h>
#include <wx/dialog.h>
#include <wx/listbox.h>


#define DESCRIPTION_COLUMN 0
#define MODALITY_COLUMN 1
#define CREATOR_USER_NAME_COLUMN 2
#define LAST_MODIFICATOR_USER_NAME_COLUMN 3
#define DATE_COLUMN 4
#define URL_COLUMN 5
#define BUCKET_ID_COLUMN 6


class wxTreeListCtrlCloud : public wxTreeListCtrl
{
public:
	wxTreeListCtrlCloud(wxWindow *parent, wxWindowID id = -1,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxTR_DEFAULT_STYLE,
               const wxValidator &validator = wxDefaultValidator,
               const wxString& name = wxTreeListCtrlNameStr )
					: wxTreeListCtrl(parent, id, pos, size, style, validator, name)
    {
	}
	~wxTreeListCtrlCloud() {}

	virtual int OnCompareItems (const wxTreeItemId& item1, const wxTreeItemId& item2) 
	{
		wxTreeItemId root = GetRootItem();
		if (item1.IsOk() && item2.IsOk()) {
			//alphabetically no case sensitive
			int descCompare =  GetItemText(item1).CompareTo(GetItemText(item2), wxString::ignoreCase);
			if (descCompare !=0) {
				return descCompare;
			} else {
				//date compare...
				wxDateTime date1,date2;
				date1.ParseFormat(GetItemText(item1, DATE_COLUMN), _("%m/%d/%Y %H:%M:%S"));
				date2.ParseFormat(GetItemText(item2, DATE_COLUMN), _("%m/%d/%Y %H:%M:%S"));
				if (date1.IsValid() && date2.IsValid()) {
					wxTimeSpan diff = date1.Subtract(date2);
					if (diff.IsPositive()) {
						return 1;
					} else {
						return -1;
					}
				} else {
					return 0;
				}
			}
		}
		return wxTreeListCtrl::OnCompareItems(item1, item2);
	}

	virtual int OnCompareItems (const wxTreeItemId& item1, const wxTreeItemId& item2, int column) 
	{
		wxTreeItemId root = GetRootItem();
		if (item1.IsOk() && item2.IsOk() && column == DATE_COLUMN) {
			if (GetItemParent(item1) == root && GetItemParent(item2) == root) {
				//first case patient level:
				//alphabetically no case sensitive
				return wxTreeListCtrl::OnCompareItems(item1, item2, column);
			} else {
				//study and series level
				//date compare...
				wxDateTime date1,date2;
				date1.ParseFormat(GetItemText(item1, DATE_COLUMN), _("%m/%d/%Y %H:%M:%S"));
				date2.ParseFormat(GetItemText(item2, DATE_COLUMN), _("%m/%d/%Y %H:%M:%S"));
				if (date1.IsValid() && date2.IsValid()) {
					wxTimeSpan diff = date1.Subtract(date2);
					if (diff.IsPositive()) {
						return 1;
					} else {
						return -1;
					}
				} else {
					return 0;
				}
			} 
		}
		return wxTreeListCtrl::OnCompareItems(item1, item2, column);
	}
};
