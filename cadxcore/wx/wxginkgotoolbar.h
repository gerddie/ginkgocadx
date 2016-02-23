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

#include <wx/bitmap.h>
#include <wx/aui/auibar.h>
#include <wx/colour.h>

class wxGinkgoToolBarArt: public wxAuiDefaultToolBarArt
{
public:
	wxGinkgoToolBarArt();
	wxGinkgoToolBarArt(const wxColor& color);
	~wxGinkgoToolBarArt();

	virtual void SetCustomBaseColor(const wxColor& color);

	virtual int ShowDropDown(wxWindow* wnd, const wxAuiToolBarItemArray& items);

	wxColor GetCustomBaseColor();

	virtual void DrawDropDownButton(
		wxDC& dc,
		wxWindow* wnd,
		const wxAuiToolBarItem& item,
		const wxRect& rect);

	virtual void DrawButton(
		wxDC& dc,
		wxWindow* wnd,
		const wxAuiToolBarItem& item,
		const wxRect& rect);
};

class wxGinkgoToolBar : public wxAuiToolBar
{
public:
	wxGinkgoToolBar(wxWindow* parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxAUI_TB_DEFAULT_STYLE);
	wxGinkgoToolBar(wxWindow* pParent, const wxColor& color = wxColor(0x40, 0x40, 0x40), int style = 0);
	wxGinkgoToolBar(wxWindow* pParent, const wxColor& color, wxFont& fontBar);

	~wxGinkgoToolBar();

	/**
	* Use this method instead of ToggleTool
	**/
	virtual void ForceToggleTool(int tool_id, bool state);
	/**
	* Use this method instead of GetToolToggled
	**/
	virtual bool ForceGetToolToggled(int tool_id) const;
	/**
	* Use this method instead of SetToolDropDown
	**/
	virtual void ForceSetToolDropDown(int tool_id, bool dropdown);

	/**change visibility of setOrientation...*/
	virtual void ForceSetOrientation(int orientation);

	virtual void OnPaint(wxPaintEvent& evt);
	virtual void OnLeftDown(wxMouseEvent& evt);
	virtual void OnLeftUp(wxMouseEvent& evt);
	virtual void OnMotion(wxMouseEvent& evt);
	virtual void OnEraseBackground( wxEraseEvent& event ) ;
};


