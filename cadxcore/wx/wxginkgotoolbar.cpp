
#include "wxginkgotoolbar.h"

#include <wx/dcbuffer.h>
#include <wx/menu.h>
#include <wx/aui/aui.h>

enum
{
	wxITEM_CONTROL = wxITEM_MAX,
	wxITEM_LABEL,
	wxITEM_SPACER
};

const int BUTTON_DROPDOWN_WIDTH = 10;

class ToolbarCommandCapture : public wxEvtHandler
{
public:

	ToolbarCommandCapture() { m_lastId = 0; }
	int GetCommandId() const { return m_lastId; }

	bool ProcessEvent(wxEvent& evt)
	{
		if (evt.GetEventType() == wxEVT_MENU)
		{
			m_lastId = evt.GetId();
			return true;
		}

		if (GetNextHandler())
			return GetNextHandler()->ProcessEvent(evt);

		return false;
	}

private:
	int m_lastId;
};

wxGinkgoToolBarArt::wxGinkgoToolBarArt()
{
}

wxGinkgoToolBarArt::wxGinkgoToolBarArt(const wxColor& color)
{
	m_baseColour = color;
}

wxGinkgoToolBarArt::~wxGinkgoToolBarArt()
{
}

void wxGinkgoToolBarArt::SetCustomBaseColor(const wxColor& color)
{
	m_baseColour = color;
}

wxColor wxGinkgoToolBarArt::GetCustomBaseColor()
{
	return m_baseColour;
}

int wxGinkgoToolBarArt::ShowDropDown(wxWindow* wnd, const wxAuiToolBarItemArray& items)
{
	wxMenu menuPopup;

	size_t items_added = 0;

	size_t i, count = items.GetCount();
	for (i = 0; i < count; ++i)
	{
		wxAuiToolBarItem& item = items.Item(i);

		if (item.GetKind() == wxITEM_NORMAL)
		{
			wxString text = item.GetShortHelp();
			if (text.empty())
				text = item.GetLabel();

			if (text.empty())
				text = wxT(" ");

			wxMenuItem* m =  new wxMenuItem(&menuPopup, item.GetId(), text, item.GetShortHelp());

			m->SetBitmap(item.GetBitmap());
			menuPopup.Append(m);
			items_added++;
		}
		else if (item.GetKind() == wxITEM_SEPARATOR)
		{
			if (items_added > 0)
				menuPopup.AppendSeparator();
		}
	}

	// find out where to put the popup menu of window items
	wxPoint pt = ::wxGetMousePosition();
	pt = wnd->ScreenToClient(pt);

	// find out the screen coordinate at the bottom of the tab ctrl
	wxRect cli_rect = wnd->GetClientRect();
	pt.y = cli_rect.y + cli_rect.height;

	ToolbarCommandCapture* cc = new ToolbarCommandCapture;
	wnd->PushEventHandler(cc);
	wnd->PopupMenu(&menuPopup, pt);
	int command = cc->GetCommandId();
	wnd->PopEventHandler(true);

	return command;
}

void wxGinkgoToolBarArt::DrawDropDownButton(
	wxDC& dc,
	wxWindow* /*wnd*/,
	const wxAuiToolBarItem& item,
	const wxRect& rect)
{
	const int BUTTON_DROPDOWN_WIDTH = 10;
	static const unsigned char DISABLED_TEXT_GREY_HUE = wxColour::AlphaBlend(0, 255, 0.4);
	const wxColour DISABLED_TEXT_COLOR(DISABLED_TEXT_GREY_HUE, DISABLED_TEXT_GREY_HUE, DISABLED_TEXT_GREY_HUE);

	int textWidth = 0, textHeight = 0, textX = 0, textY = 0;
	int bmpX = 0, bmpY = 0, dropBmpX = 0, dropBmpY = 0;

	wxRect buttonRect = wxRect(rect.x,
		rect.y,
		rect.width-BUTTON_DROPDOWN_WIDTH,
		rect.height);
	wxRect dropDownRect = wxRect(rect.x+rect.width-BUTTON_DROPDOWN_WIDTH-1,
		rect.y,
		BUTTON_DROPDOWN_WIDTH+1,
		rect.height);

	if (m_flags & wxAUI_TB_TEXT)
	{
		dc.SetFont(m_font);

		int tx, ty;
		if (m_flags & wxAUI_TB_TEXT)
		{
			dc.GetTextExtent(wxT("ABCDHgj"), &tx, &textHeight);
			textWidth = 0;
		}

		dc.GetTextExtent(item.GetLabel(), &textWidth, &ty);
	}

	dropBmpX = dropDownRect.x +
		(dropDownRect.width/2) -
		(m_buttonDropDownBmp.GetWidth()/2);
	dropBmpY = dropDownRect.y +
		(dropDownRect.height/2) -
		(m_buttonDropDownBmp.GetHeight()/2);

	if (m_textOrientation == wxAUI_TBTOOL_TEXT_BOTTOM)
	{
		bmpX = buttonRect.x +
			(buttonRect.width/2) -
			(item.GetBitmap().GetWidth()/2);
		bmpY = buttonRect.y +
			((buttonRect.height-textHeight)/2) -
			(item.GetBitmap().GetHeight()/2);

		textX = rect.x + (rect.width/2) - (textWidth/2) + 1;
		textY = rect.y + rect.height - textHeight - 1;
	}
	else if (m_textOrientation == wxAUI_TBTOOL_TEXT_RIGHT)
	{
		bmpX = rect.x + 3;

		bmpY = rect.y +
			(rect.height/2) -
			(item.GetBitmap().GetHeight()/2);

		textX = bmpX + 3 + item.GetBitmap().GetWidth();
		textY = rect.y +
			(rect.height/2) -
			(textHeight/2);
	}


	if (item.GetState() & wxAUI_BUTTON_STATE_PRESSED)
	{
		dc.SetPen(wxPen(m_highlightColour));
		dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(140)));
		dc.DrawRectangle(buttonRect);

		dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(170)));
		dc.DrawRectangle(dropDownRect);
	}
	else if (item.GetState() & wxAUI_BUTTON_STATE_HOVER ||
		item.IsSticky())
	{
		dc.SetPen(wxPen(m_highlightColour));
		dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(170)));
		dc.DrawRectangle(buttonRect);
		dc.DrawRectangle(dropDownRect);
	}
	else if (item.GetState() & wxAUI_BUTTON_STATE_CHECKED)
	{
		// Notice that this branch must come after the hover one to ensure the
		// correct appearance when the mouse hovers over a checked item.m_
		dc.SetPen(wxPen(m_highlightColour));
		dc.SetBrush(wxBrush(m_highlightColour.ChangeLightness(170)));
		dc.DrawRectangle(buttonRect);
		dc.DrawRectangle(dropDownRect);
	}

	wxBitmap bmp;
	wxBitmap dropbmp;
	if (item.GetState() & wxAUI_BUTTON_STATE_DISABLED)
	{
		bmp = item.GetDisabledBitmap();
		dropbmp = m_disabledButtonDropDownBmp;
	}
	else
	{
		bmp = item.GetBitmap();
		dropbmp = m_buttonDropDownBmp;
	}

	if (!bmp.IsOk())
		return;

	dc.DrawBitmap(bmp, bmpX, bmpY, true);
	dc.DrawBitmap(dropbmp, dropBmpX, dropBmpY, true);

	// set the item's text color based on if it is disabled
	dc.SetTextForeground(*wxBLACK);
	if (item.GetState() & wxAUI_BUTTON_STATE_DISABLED)
		dc.SetTextForeground(DISABLED_TEXT_COLOR);

	if ( (m_flags & wxAUI_TB_TEXT) && !item.GetLabel().empty() )
	{
		dc.DrawText(item.GetLabel(), textX, textY);
	}
}

void wxGinkgoToolBarArt::DrawButton(
	wxDC& dc,
	wxWindow* wnd,
	const wxAuiToolBarItem& item,
	const wxRect& rect)
{
	if (item.HasDropDown()) {
		//we have modified default draw function to draw checked + drop down (not supported in wxWidgets)
		DrawDropDownButton(dc, wnd, item, rect);
	} else {
		wxAuiDefaultToolBarArt::DrawButton(dc, wnd, item, rect);
	}
}

//==================================================================================

wxGinkgoToolBar::wxGinkgoToolBar(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) : wxAuiToolBar(parent, id, pos, size, style)
{
	SetToolBitmapSize(wxSize(16,16));
	wxAuiDefaultToolBarArt* pToolArt= new wxGinkgoToolBarArt();
	SetArtProvider(pToolArt);

	Connect(wxEVT_PAINT, wxPaintEventHandler(wxGinkgoToolBar::OnPaint), NULL, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxGinkgoToolBar::OnLeftUp), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(wxGinkgoToolBar::OnMotion), NULL, this);
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( wxGinkgoToolBar ::OnEraseBackground ) );
}

wxGinkgoToolBar::wxGinkgoToolBar(wxWindow* pParent, const wxColor& color, int style) : wxAuiToolBar(pParent, wxID_ANY, wxDefaultPosition, wxDefaultSize, style|wxAUI_TB_DEFAULT_STYLE)
{
	SetToolBitmapSize(wxSize(16,16));
	wxAuiDefaultToolBarArt* pToolArt= new wxGinkgoToolBarArt(color);
	SetArtProvider(pToolArt);
	Connect(wxEVT_PAINT, wxPaintEventHandler(wxGinkgoToolBar::OnPaint), NULL, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxGinkgoToolBar::OnLeftUp), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(wxGinkgoToolBar::OnMotion), NULL, this);
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( wxGinkgoToolBar ::OnEraseBackground ) );
}

wxGinkgoToolBar::wxGinkgoToolBar(wxWindow* pParent, const wxColor& color, wxFont& fontBar) : wxAuiToolBar(pParent,wxID_ANY, wxDefaultPosition, wxDefaultSize, wxAUI_TB_DEFAULT_STYLE)
{
	SetToolBitmapSize(wxSize(16,16));
	wxAuiDefaultToolBarArt* pToolArt= new wxGinkgoToolBarArt(color);
	pToolArt->SetFont(fontBar);
	SetArtProvider(pToolArt);
	Connect(wxEVT_PAINT, wxPaintEventHandler(wxGinkgoToolBar::OnPaint), NULL, this);
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Connect(wxEVT_LEFT_UP, wxMouseEventHandler(wxGinkgoToolBar::OnLeftUp), NULL, this);
	Connect(wxEVT_MOTION, wxMouseEventHandler(wxGinkgoToolBar::OnMotion), NULL, this);
	Connect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( wxGinkgoToolBar ::OnEraseBackground ) );
}

wxGinkgoToolBar::~wxGinkgoToolBar()
{
	Disconnect(wxEVT_MOTION, wxMouseEventHandler(wxGinkgoToolBar::OnMotion), NULL, this);
	Disconnect(wxEVT_LEFT_UP, wxMouseEventHandler(wxGinkgoToolBar::OnLeftUp), NULL, this);
	Disconnect(wxEVT_LEFT_DOWN, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Disconnect(wxEVT_LEFT_DCLICK, wxMouseEventHandler(wxGinkgoToolBar::OnLeftDown), NULL, this);
	Disconnect(wxEVT_PAINT, wxPaintEventHandler(wxGinkgoToolBar::OnPaint), NULL, this);
	Disconnect(wxEVT_ERASE_BACKGROUND, wxEraseEventHandler( wxGinkgoToolBar ::OnEraseBackground ) );
}

void wxGinkgoToolBar::ForceToggleTool(int tool_id, bool state)
{
	wxAuiToolBarItem* tool = FindTool(tool_id);

	if (tool)
	{
		if (tool->GetKind() == wxITEM_RADIO)
		{
			int i, idx, count;
			idx = GetToolIndex(tool_id);
			count = (int)m_items.GetCount();

			if (idx >= 0 && idx < count)
			{
				for (i = idx + 1; i < count; ++i)
				{
					if (m_items[i].GetKind() != wxITEM_RADIO)
						break;
					m_items[i].SetState(m_items[i].GetState() & ~wxAUI_BUTTON_STATE_CHECKED);
				}
				for (i = idx - 1; i >= 0; i--)
				{
					if (m_items[i].GetKind() != wxITEM_RADIO)
						break;
					m_items[i].SetState(m_items[i].GetState() & ~wxAUI_BUTTON_STATE_CHECKED);
				}
			}

			tool->SetState(tool->GetState() | wxAUI_BUTTON_STATE_CHECKED);
		}
		else
		{
			if (state == true) {
				tool->SetState(tool->GetState() | wxAUI_BUTTON_STATE_CHECKED);
			}
			else {
				tool->SetState(tool->GetState() & ~wxAUI_BUTTON_STATE_CHECKED);
			}
		}
	}
}

bool wxGinkgoToolBar::ForceGetToolToggled(int tool_id) const
{
	wxAuiToolBarItem* tool = FindTool(tool_id);

	if (tool)
	{
		return (tool->GetState() & wxAUI_BUTTON_STATE_CHECKED) ? true : false;
	}

	return false;
}

void wxGinkgoToolBar::ForceSetToolDropDown(int tool_id, bool dropdown)
{
	wxAuiToolBarItem* item = FindTool(tool_id);
	if (!item)
		return;

	if (item->GetKind() != wxITEM_NORMAL) {
		int kind = item->GetKind(); // Hack
		item->SetKind(wxITEM_NORMAL);
		item->SetHasDropDown(dropdown);
		item->SetKind(kind);
	}
	else {
		item->SetHasDropDown(dropdown);
	}
}

void wxGinkgoToolBar::ForceSetOrientation(int orientation)
{
	wxAuiToolBar::SetOrientation(orientation);
}

void wxGinkgoToolBar::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
	wxAutoBufferedPaintDC dc(this);
	wxRect cli_rect(wxPoint(0,0), GetClientSize());

	bool horizontal = m_orientation == wxHORIZONTAL;

	if (m_windowStyle & wxAUI_TB_PLAIN_BACKGROUND)
		m_art->DrawPlainBackground(dc, this, cli_rect);
	else
		m_art->DrawBackground(dc, this, cli_rect);

	int gripperSize = m_art->GetElementSize(wxAUI_TBART_GRIPPER_SIZE);
	int dropdown_size = m_art->GetElementSize(wxAUI_TBART_OVERFLOW_SIZE);

	// paint the gripper
	if (gripperSize > 0 && m_gripperSizerItem)
	{
		wxRect gripper_rect = m_gripperSizerItem->GetRect();
		if (horizontal)
			gripper_rect.width = gripperSize;
		else
			gripper_rect.height = gripperSize;
		m_art->DrawGripper(dc, this, gripper_rect);
	}

	// calculated how far we can draw items
	int last_extent;
	if (horizontal)
		last_extent = cli_rect.width;
	else
		last_extent = cli_rect.height;
	if (m_overflowVisible)
		last_extent -= dropdown_size;

	// paint each individual tool
	size_t i, count = m_items.GetCount();
	for (i = 0; i < count; ++i)
	{
		wxAuiToolBarItem& item = m_items.Item(i);

		if (!item.GetSizerItem())
			continue;

		wxRect item_rect = item.GetSizerItem()->GetRect();


		if ((horizontal  && item_rect.x + item_rect.width >= last_extent) ||
			(!horizontal && item_rect.y + item_rect.height >= last_extent))
		{
			break;
		}

		switch ( item.GetKind() )
		{
		case wxITEM_CHECK:
		case wxITEM_NORMAL:
			m_art->DrawButton(dc, this, item, item_rect);
			break;
		case wxITEM_RADIO:
			// draw a toggle button
			m_art->DrawButton(dc, this, item, item_rect);
			break;

		case wxITEM_SEPARATOR:
			// draw a separator
			m_art->DrawSeparator(dc, this, item_rect);
			break;
		case wxITEM_LABEL:
			// draw a text label only
			m_art->DrawLabel(dc, this, item, item_rect);
			break;

		case wxITEM_CONTROL:
			// draw the control's label
			m_art->DrawControlLabel(dc, this, item, item_rect);
			break;
		}

		// fire a signal to see if the item wants to be custom-rendered
		OnCustomRender(dc, item, item_rect);
	}

	// paint the overflow button
	if (dropdown_size > 0 && m_overflowSizerItem)
	{
		wxRect dropDownRect = GetOverflowRect();
		m_art->DrawOverflowButton(dc, this, dropDownRect, m_overflowState);
	}
}

void wxGinkgoToolBar::OnLeftDown(wxMouseEvent& evt)
{
	//very important to prevent trap because of capturemouse reentering
	if (HasCapture()) {
		ReleaseMouse();
		return;
	}
	wxRect cli_rect(wxPoint(0,0), GetClientSize());

	if (m_gripperSizerItem)
	{
		wxRect gripper_rect = m_gripperSizerItem->GetRect();
		if (gripper_rect.Contains(evt.GetX(), evt.GetY()))
		{
			// find aui manager
			wxAuiManager* manager = wxAuiManager::GetManager(this);
			if (!manager)
				return;

			int x_drag_offset = evt.GetX() - gripper_rect.GetX();
			int y_drag_offset = evt.GetY() - gripper_rect.GetY();

			// gripper was clicked
			manager->StartPaneDrag(this, wxPoint(x_drag_offset, y_drag_offset));
			return;
		}
	}

	if (m_overflowSizerItem)
	{
		wxRect overflow_rect = GetOverflowRect();

		if (m_art &&
			m_overflowVisible &&
			overflow_rect.Contains(evt.m_x, evt.m_y))
		{
			wxAuiToolBarEvent e(wxEVT_AUITOOLBAR_OVERFLOW_CLICK, -1);
			e.SetEventObject(this);
			e.SetToolId(-1);
			e.SetClickPoint(wxPoint(evt.GetX(), evt.GetY()));
			bool processed = GetEventHandler()->ProcessEvent(e);

			if (processed)
			{
				DoIdleUpdate();
			}
			else
			{
				size_t i, count;
				wxAuiToolBarItemArray overflow_items;


				// add custom overflow prepend items, if any
				count = m_customOverflowPrepend.GetCount();
				for (i = 0; i < count; ++i)
					overflow_items.Add(m_customOverflowPrepend[i]);

				// only show items that don't fit in the dropdown
				count = m_items.GetCount();
				for (i = 0; i < count; ++i)
				{
					if (!GetToolFitsByIndex(i))
						overflow_items.Add(m_items[i]);
				}

				// add custom overflow append items, if any
				count = m_customOverflowAppend.GetCount();
				for (i = 0; i < count; ++i)
					overflow_items.Add(m_customOverflowAppend[i]);

				int res = m_art->ShowDropDown(this, overflow_items);
				m_overflowState = 0;
				Refresh(false);
				if (res != -1)
				{
					wxCommandEvent event(wxEVT_MENU, res);
					event.SetEventObject(this);
					GetParent()->GetEventHandler()->ProcessEvent(event);
				}
			}

			return;
		}
	}

	m_dragging = false;
	m_actionPos = wxPoint(evt.GetX(), evt.GetY());
	m_actionItem = FindToolByPosition(evt.GetX(), evt.GetY());

	if (m_actionItem)
	{
		if (m_actionItem->GetState() & wxAUI_BUTTON_STATE_DISABLED)
		{
			m_actionPos = wxPoint(-1,-1);
			m_actionItem = NULL;
			return;
		}

		UnsetToolTip();

		// fire the tool dropdown event
		wxAuiToolBarEvent e(wxEVT_AUITOOLBAR_TOOL_DROPDOWN, m_actionItem->GetId());
		e.SetEventObject(this);
		e.SetToolId(m_actionItem->GetId());

		int mouse_x = evt.GetX();
		wxRect rect = m_actionItem->GetSizerItem()->GetRect();
		const bool dropDownHit = m_actionItem->HasDropDown() &&
			mouse_x >= (rect.x+rect.width-BUTTON_DROPDOWN_WIDTH-1) &&
			mouse_x < (rect.x+rect.width);
		e.SetDropDownClicked(dropDownHit);

		e.SetClickPoint(evt.GetPosition());
		e.SetItemRect(rect);

		// we only set the 'pressed button' state if we hit the actual button
		// and not just the drop-down
		SetPressedItem(dropDownHit ? 0 : m_actionItem);

		if(dropDownHit)
		{
			m_actionPos = wxPoint(-1,-1);
			m_actionItem = NULL;
		}

		if(!GetEventHandler()->ProcessEvent(e) || e.GetSkipped()) {
			CaptureMouse();
		}

		// Ensure hovered item is really ok, as mouse may have moved during
		//  event processing
		wxPoint cursor_pos_after_evt = ScreenToClient(wxGetMousePosition());
		SetHoverItem(FindToolByPosition(cursor_pos_after_evt.x, cursor_pos_after_evt.y));

		DoIdleUpdate();
	}
}

void wxGinkgoToolBar::OnLeftUp(wxMouseEvent& evt)
{
	//process only if this control has registered capture
	if (!HasCapture())
		return;

	SetPressedItem(NULL);

	wxAuiToolBarItem* hitItem;
	hitItem = FindToolByPosition(evt.GetX(), evt.GetY());
	SetHoverItem(hitItem);

	if (m_dragging)
	{
		ReleaseMouse();
	}
	else
	{
		if (m_actionItem && hitItem == m_actionItem)
		{
			UnsetToolTip();

			wxCommandEvent e(wxEVT_MENU, m_actionItem->GetId());
			e.SetEventObject(this);

			if (hitItem->GetKind() == wxITEM_CHECK || hitItem->GetKind() == wxITEM_RADIO)
			{
				const bool toggle = !(m_actionItem->GetState() & wxAUI_BUTTON_STATE_CHECKED);

				ForceToggleTool(m_actionItem->GetId(), toggle);

				// repaint immediately
				Refresh(false);
				Update();

				e.SetInt(toggle);
			}

			// we have to release the mouse *before* sending the event, because
			// we don't know what a handler might do. It could open up a popup
			// menu for example and that would make us lose our capture anyway.

			ReleaseMouse();

			GetEventHandler()->ProcessEvent(e);

			// Ensure hovered item is really ok, as mouse may have moved during
			// event processing
			wxPoint cursor_pos_after_evt = ScreenToClient(wxGetMousePosition());
			SetHoverItem(FindToolByPosition(cursor_pos_after_evt.x, cursor_pos_after_evt.y));

			DoIdleUpdate();
		}
		else
			ReleaseMouse();
	}
}
void wxGinkgoToolBar::OnMotion(wxMouseEvent& evt)
{
	const bool button_pressed = HasCapture();

	// start a drag event
	if (m_actionItem && !m_dragging && button_pressed &&
		abs(evt.GetX() - m_actionPos.x) + abs(evt.GetY() - m_actionPos.y) > 5)
	{
		// TODO: sending this event only makes sense if there is an 'END_DRAG'
		// event sent sometime in the future (see OnLeftUp())
		wxAuiToolBarEvent e(wxEVT_AUITOOLBAR_BEGIN_DRAG, GetId());
		e.SetEventObject(this);
		e.SetToolId(m_actionItem->GetId());

		m_dragging = GetEventHandler()->ProcessEvent(e) && !e.GetSkipped();

		DoIdleUpdate();
	}

	if(m_dragging)
		return;

	wxAuiToolBarItem* hitItem = FindToolByPosition(evt.GetX(), evt.GetY());
	if(button_pressed)
	{
		// if we have a button pressed we want it to be shown in 'depressed'
		// state unless we move the mouse outside the button, then we want it
		// to show as just 'highlighted'
		if (hitItem == m_actionItem)
			SetPressedItem(m_actionItem);
		else
		{
			SetPressedItem(NULL);
			SetHoverItem(m_actionItem);
		}
	}
	else
	{
		SetHoverItem(hitItem);

		// tooltips handling
		wxAuiToolBarItem* packingHitItem;
		packingHitItem = FindToolByPositionWithPacking(evt.GetX(), evt.GetY());
		if (packingHitItem)
		{
			if (packingHitItem != m_tipItem)
			{
				m_tipItem = packingHitItem;

				if ( !packingHitItem->GetShortHelp().empty() )
					SetToolTip(packingHitItem->GetShortHelp());
				else
					UnsetToolTip();
			}
		}
		else
		{
			UnsetToolTip();
			m_tipItem = NULL;
		}

		// figure out the dropdown button state (are we hovering or pressing it?)
		RefreshOverflowState();
	}
}

void wxGinkgoToolBar::OnEraseBackground( wxEraseEvent& event ) 
{
	event.Skip(false);
}
