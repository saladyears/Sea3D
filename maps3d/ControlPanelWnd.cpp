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
#include "ControlPanelWnd.h"
#include "defineColors.h"
#include "define.h"
#include "defineTab.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlPanelWnd

CControlPanelWnd::CControlPanelWnd()
{
}

CControlPanelWnd::~CControlPanelWnd()
{
	//cleanup
	if(NULL != m_pTabTiles)
	{
		delete m_pTabTiles;
		m_pTabTiles = NULL;
	}

	if(NULL != m_pTabSpecial)
	{
		delete m_pTabSpecial;
		m_pTabSpecial = NULL;
	}
}


BEGIN_MESSAGE_MAP(CControlPanelWnd, CWnd)
	//{{AFX_MSG_MAP(CControlPanelWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CControlPanelWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// draw
/////////////////////////////////////////////////////////////////////////////
void CControlPanelWnd::OnPaint() 
{
	CRect rect;
	CBrush brush, *pBrush;
	CPen pen, *pPen;

	CPaintDC dc(this); // device context for painting

	GetClientRect(&rect);

	dc.FillSolidRect(rect, COLOR_BLACK);

	brush.CreateSolidBrush(PANEL_BACKGROUND);
	pen.CreatePen(PS_SOLID, 1, COLOR_DKGRAY);

	pBrush = dc.SelectObject(&brush);
	pPen = dc.SelectObject(&pen);

	dc.RoundRect(&rect, CPoint(5, 5));

	dc.SelectObject(pPen);
	dc.SelectObject(pBrush);
}

/////////////////////////////////////////////////////////////////////////////
// setup
/////////////////////////////////////////////////////////////////////////////
int CControlPanelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int x;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pTabTiles = new CTilesTabCtrl;
	m_pTabTiles->Create(TAB_FLAGS | CHILD, CRect(TAB_OFFSET, TAB_OFFSET, TAB_OFFSET + TAB_TILES_WIDTH, TAB_OFFSET + TAB_HEIGHT), this, 0);

	//set starting pos
	x = TAB_OFFSET + TAB_TILES_WIDTH + TAB_SPACING;

	m_pTabSpecial = new CSpecialTabCtrl;
	m_pTabSpecial->Create(TAB_FLAGS| TCS_FIXEDWIDTH | CHILD, CRect(x, TAB_OFFSET, x + TAB_SPECIAL_WIDTH, TAB_OFFSET + TAB_HEIGHT), this, 0);

	//get next pos
	x += (TAB_SPECIAL_WIDTH + TAB_SPACING);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set the currently selected resource
/////////////////////////////////////////////////////////////////////////////
void CControlPanelWnd::setResource(int nRes)
{
}