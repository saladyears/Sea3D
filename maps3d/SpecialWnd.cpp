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
#include "maps3d.h"
#include "SpecialWnd.h"
#include "define.h"
#include "defineColors.h"
#include "defineMaps3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpecialWnd

CSpecialWnd::CSpecialWnd()
{
	XLOGFONT xfont;

	//create the needed font
	xfont = GetLogFont(12, "Tahoma");
	m_font.CreateFontIndirect(&(LOGFONT)xfont);

	//default mode is ports
	m_nMode = SPECIAL_PORTS;
}

CSpecialWnd::~CSpecialWnd()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CSpecialWnd, CWnd)
	//{{AFX_MSG_MAP(CSpecialWnd)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSpecialWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// initial setup
/////////////////////////////////////////////////////////////////////////////
int CSpecialWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	CRect rect;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//set the rect
	rect = CRect(1, 1, 56, 13);

	//create the hover statics
	m_TilesStatic.Create("", CHILD, rect, this, 0);
	m_TilesStatic.SetFont(Tahoma12);
	m_TilesStatic.SetBackground(COLOR_BLACK);
	m_TilesStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_TilesStatic.SetText("Tiles: 0");

	//board selected
	rect.OffsetRect(55, 0);
	m_TilesSelectedStatic.Create("", CHILD, rect, this, 0);
	m_TilesSelectedStatic.SetFont(Tahoma12);
	m_TilesSelectedStatic.SetBackground(COLOR_BLACK);
	m_TilesSelectedStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_TilesSelectedStatic.SetText("Selected: 0");

	//chits
	rect.OffsetRect(-55, 17);
	m_ChitsStatic.Create("", CHILD, rect, this, 0);
	m_ChitsStatic.SetFont(Tahoma12);
	m_ChitsStatic.SetBackground(COLOR_BLACK);
	m_ChitsStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_ChitsStatic.SetText("Chits: 0");

	//chits selected
	rect.OffsetRect(55, 0);
	m_ChitsSelectedStatic.Create("", CHILD, rect, this, 0);
	m_ChitsSelectedStatic.SetFont(Tahoma12);
	m_ChitsSelectedStatic.SetBackground(COLOR_BLACK);
	m_ChitsSelectedStatic.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_ChitsSelectedStatic.SetText("Selected: 0");

	//buttons
	rect.right += 25;
	rect.OffsetRect(65, -16);
	m_TilesButton.Create("", CHILD, rect, this, 0);
	m_TilesButton.DrawTransparent();
	m_TilesButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_TilesButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_TilesButton.SetWindowText("Select Tiles");
	m_TilesButton.SetFont(&m_font);

	rect.OffsetRect(0, 17);
	m_ChitsButton.Create("", CHILD, rect, this, 0);
	m_ChitsButton.DrawTransparent();
	m_ChitsButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_ChitsButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_ChitsButton.SetWindowText("Select Chits");
	m_ChitsButton.SetFont(&m_font);

	//set initial mode
	setMode(SPECIAL_PORTS);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set a new mode
/////////////////////////////////////////////////////////////////////////////
void CSpecialWnd::setMode(int nMode)
{
	m_nMode = nMode;

	//update display
	updateSpecial();
}

/////////////////////////////////////////////////////////////////////////////
// update the mode
/////////////////////////////////////////////////////////////////////////////
void CSpecialWnd::updateSpecial()
{
	int nTiles, nTileSelect;
	int nChits, nChitSelect;

	//show the chits by default
	m_ChitsButton.ShowWindow(SW_SHOW);
	m_ChitsStatic.ShowWindow(SW_SHOW);
	m_ChitsSelectedStatic.ShowWindow(SW_SHOW);

	//set things
	switch(m_nMode)
	{
	case SPECIAL_PORTS:
		//hide chits
		m_ChitsButton.ShowWindow(SW_HIDE);
		m_ChitsStatic.ShowWindow(SW_HIDE);
		m_ChitsSelectedStatic.ShowWindow(SW_HIDE);

		break;

	default: break;
	}

	Invalidate();
}
