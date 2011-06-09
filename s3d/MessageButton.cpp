/*
 *  Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net)
 * 
 *  This program is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the Free 
 *  Software Foundation; either version 2 of the License, or (at your option) 
 *  any later version.
 * 
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 */

#include "stdafx.h"
#include "Settlers.h"
#include "MessageButton.h"
#include "SettlersView.h"
#include "defineColors.h"
#include "defineDraw.h"

// CMessageButton

IMPLEMENT_DYNAMIC(CMessageButton, CAnimateButton)
CMessageButton::CMessageButton()
{
	//not mouseable
	m_bMouseable = FALSE;
}

CMessageButton::~CMessageButton()
{
}


BEGIN_MESSAGE_MAP(CMessageButton, CAnimateButton)
	ON_WM_CREATE()
	ON_BN_CLICKED(ID_PANEL_UNDO, OnUndoButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// handle creation
/////////////////////////////////////////////////////////////////////////////
int CMessageButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	//set the rect
	CRect rect(0, 0, 0, 0);

	if (CAnimateButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//create the undo button
	m_UndoButton.Create("", CHILD | BS_ICON | BS_LEFT | BS_TOP, rect, this, ID_PANEL_UNDO);
	m_UndoButton.SetWindowPos(&wndTop, PANEL_WIDTH - PANEL_ICON - (PANEL_OFFSET * 2), PANEL_OFFSET, PANEL_ICON, PANEL_ICON, SWP_SHOWWINDOW);
	m_UndoButton.SetIcon(IDI_UNDO_ICON, IDI_UNDO_ICON);
	m_UndoButton.DrawTransparent(TRUE);
	m_UndoButton.SetTooltipText("Undo the last action");

	//create the title window
	m_Title.Create("", CHILD, rect, this, ID_PANEL_BUTTON);
	m_Title.SetWindowPos(&wndTop, PANEL_OFFSET, PANEL_OFFSET, PANEL_WIDTH - (PANEL_OFFSET * 4) - PANEL_ICON, 39, SWP_SHOWWINDOW);

	//set the font and border
	m_Title.SetFont(Tahoma12);
	m_Title.SetLeftBorder(PANEL_TITLE_LBORD);
	m_Title.SetTopBorder(PANEL_TITLE_TBORD);
	m_Title.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_Title.SetBackground(COLOR_BLACK);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// window setup
/////////////////////////////////////////////////////////////////////////////
BOOL CMessageButton::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= BS_OWNERDRAW;

	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle enabling/disabling the undo button
/////////////////////////////////////////////////////////////////////////////
void CMessageButton::enableUndo(BOOL b)
{
	//enable/disable the window
	m_UndoButton.EnableWindow(b);

	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle pressing of undo button
/////////////////////////////////////////////////////////////////////////////
void CMessageButton::OnUndoButton() 
{
	VIEW->undo();
	
	//set the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// paint background
/////////////////////////////////////////////////////////////////////////////
void CMessageButton::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting
	
	//get the client area
	GetClientRect(&rect);

	//fill the background
	dc.FillSolidRect(rect, COLOR_DKGRAY);
	rect.DeflateRect(1, 0, 0, 1);
	dc.FillSolidRect(rect, COLOR_BLACK);
}
