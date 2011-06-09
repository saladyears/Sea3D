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
#include "CustomTabCtrl.h"
#include "defineColors.h"
#include "defineTab.h"
#include "XLogFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomTabCtrl

CCustomTabCtrl::CCustomTabCtrl()
{
}

CCustomTabCtrl::~CCustomTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CCustomTabCtrl)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomTabCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// initial stuff
/////////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::PreSubclassWindow() 
{
	XLOGFONT font;

	//create a font
	font = GetLogFont(12, "Tahoma");
	
	//set the font to what we want
	m_font.CreateFontIndirect((LOGFONT *) &font);
	
	CTabCtrl::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// tab creation
/////////////////////////////////////////////////////////////////////////////
int CCustomTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTabCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetItemSize(CSize(TAB_ITEM_WIDTH, TAB_ITEM_HEIGHT));
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// draw
/////////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::OnPaint() 
{
	int i;
	int nTabs;
	CRect rect;
	CPen pen, *pPen;
	CBrush brush, *pBrush;

	CPaintDC dc(this); // device context for painting

	//get draw area
	GetClientRect(&rect);

	//first, fill the background
	dc.FillSolidRect(rect, PANEL_BACKGROUND);

	//now create the border
	rect.DeflateRect(0, 0, 0, TAB_ITEM_HEIGHT);

	//create pen and brush
	pen.CreatePen(PS_SOLID, 1, TAB_COLOR_BORDER);
	brush.CreateSolidBrush(TAB_COLOR_BACKGROUND);

	//select into DC
	pPen = dc.SelectObject(&pen);
	pBrush = dc.SelectObject(&brush);

	//create the rect
	dc.RoundRect(rect, CPoint(TAB_CURVE, TAB_CURVE));

	//select out
	dc.SelectObject(pPen);
	dc.SelectObject(pBrush);

	//get number of tabs
	nTabs = GetItemCount();

	//draw each tab
	for(i = 0; i < nTabs; i++)
	{
		DrawTab(&dc, i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// draw a tab
/////////////////////////////////////////////////////////////////////////////
void CCustomTabCtrl::DrawTab(CDC *pDC, int nTab)
{
	int x, y;
	char szText[32];
	BOOL bSel;
	CRect rect, inner;
	CFont *pFont;
	CPen *pPen, pen;
	CBrush *pBrush, brush;
	TCITEM item;

	//see if we are the selected tab
	bSel = (GetCurSel() == nTab);

	//get the tabs draw area
	GetItemRect(nTab, &rect);

	//get item info
	item.mask = TCIF_TEXT;
	item.pszText = szText;
	item.cchTextMax = 32;
	GetItem(nTab, &item);

	//if we are the selected one, adjust the rect
	if(TRUE == bSel)
	{
		rect.DeflateRect(0, 0, 0, 1);
		pen.CreatePen(PS_SOLID, 1, TAB_COLOR_BORDER);
	}
	else
	{
		rect.DeflateRect(0, 2, 0, 2);
		pen.CreatePen(PS_SOLID, 1, TAB_COLOR_NOT_SELECTED);
	}

	//create the pen and brush
	brush.CreateSolidBrush(TAB_COLOR_BACKGROUND);

	//select
	pPen = pDC->SelectObject(&pen);
	pBrush = pDC->SelectObject(&brush);

	//create the tab
	pDC->RoundRect(&rect, CPoint(TAB_ITEM_CURVE, TAB_ITEM_CURVE));

	//now kill the part we didn't need
	inner = CRect(rect.left + 1, rect.top, rect.right - 1, rect.top + TAB_ITEM_CURVE);
	pDC->FillSolidRect(inner, TAB_COLOR_BACKGROUND);

	//finsh off the lines
	pDC->MoveTo(rect.left, rect.top + bSel - (!nTab));
	pDC->LineTo(rect.left, rect.top + bSel + TAB_ITEM_CURVE);
	pDC->MoveTo(rect.right - 1, rect.top + bSel);
	pDC->LineTo(rect.right - 1, rect.top + bSel + TAB_ITEM_CURVE);

	//unselect
	pDC->SelectObject(pBrush);
	pDC->SelectObject(pPen);

	//select the font and color
	pFont = pDC->SelectObject(&m_font);
	pDC->SetTextColor(COLOR_WHITE);

	//calc text position
	x = rect.left + 4;
	y = rect.top + 1;

	//adjust non selected
	if(FALSE == bSel)
	{
		y--;
	}
	else
	{
		y++;
	}

	//write the text
	pDC->TextOut(x, y, CString(item.pszText));

	//unselect font and pen
	pDC->SelectObject(pFont);
}
