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
#include "settlers.h"
#include "GoodsButton.h"
#include "define.h"
#include "SettlersView.h"
#include "ControlPanel.h"
#include "defineDraw.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGoodsButton

CGoodsButton::CGoodsButton()
{
	//load cursor
	m_hCursor = AfxGetApp()->LoadCursor(IDC_HAND_CURSOR);
}

CGoodsButton::~CGoodsButton()
{
}


BEGIN_MESSAGE_MAP(CGoodsButton, CButton)
	//{{AFX_MSG_MAP(CGoodsButton)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_GOODS_STATIC, OnGoodsStatic)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGoodsButton message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CGoodsButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	
	return CButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// create startup stuff
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::createControls(BOOL bVolcano)
{
	int i;
	int iX, iY;
	CRect rect;

	//set volcano flag
	m_bVolcano = bVolcano;

	//get size
	GetClientRect(&rect);

	iY = CONTROL_PANEL_SPACER;
	iX = (CONTROL_PANEL_GOODS_WIDTH - int(2.5 * CONTROL_PANEL_DICE_WIDTH)) / 2;

	//create dice buttons
	m_btnRoll1.Create("", CHILD, CRect(0, 0, 0, 0), this, 0);
	m_btnRoll1.SetWindowPos(&wndTop, iX, iY, CONTROL_PANEL_DICE_WIDTH, CONTROL_PANEL_DICE_WIDTH, SWP_SHOWWINDOW);
	m_btnRoll1.EnableWindow(FALSE);

	//increment offset
	iX += int(1.5 * CONTROL_PANEL_DICE_WIDTH);
	m_btnRoll2.Create("", CHILD, CRect(0, 0, 0, 0), this, 0);
	m_btnRoll2.SetWindowPos(&wndTop, iX, iY, CONTROL_PANEL_DICE_WIDTH, CONTROL_PANEL_DICE_WIDTH, SWP_SHOWWINDOW);
	m_btnRoll2.EnableWindow(FALSE);

	//reset y
	iY = rect.bottom;

	//create the goods statics
	for(i = 5; i >= 0; i--)
	{
		m_btnGoods[i].Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_GOODS_STATIC);
		m_btnGoods[i].SetWindowPos(&wndTop, 1, iY - CONTROL_PANEL_GOODS_HEIGHT, CONTROL_PANEL_GOODS_WIDTH, CONTROL_PANEL_GOODS_HEIGHT, SWP_HIDEWINDOW);

		m_btnGoods[i].SetLeftBorder(PANEL_TITLE_LBORD);
		m_btnGoods[i].SetTopBorder(PANEL_TITLE_TBORD);
		m_btnGoods[i].SetFont(Tahoma11);
		m_btnGoods[i].TurnOnBorder();

		iY -= CONTROL_PANEL_GOODS_HEIGHT - 1;
	}
}

/////////////////////////////////////////////////////////////////////////////
// set the color background for a control
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::setBackground(int iPlayer, COLORREF color)
{
	m_btnGoods[iPlayer].SetBackground(color);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide a goods window
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::showWindow(int iPlayer, int iShow)
{
	m_btnGoods[iPlayer].ShowWindow(iShow);
}

/////////////////////////////////////////////////////////////////////////////
// set goods text
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::setText(int iPlayer, CString strText)
{
	m_btnGoods[iPlayer].SetText(strText);
}

/////////////////////////////////////////////////////////////////////////////
// draw handling
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CRect rect;
	CDC *pDC;
	
	//get the dc and item rectangle
	pDC = CDC::FromHandle(lpDIS->hDC);
	rect = lpDIS->rcItem;

	//fill the background
	pDC->FillSolidRect(rect, COLOR_WHITE);
	rect.DeflateRect(1, 1, 1, 1);
	pDC->FillSolidRect(rect, COLOR_BLACK);
}

/////////////////////////////////////////////////////////////////////////////
// set dice bitmaps
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::setDice(int iDie1, int iDie2)
{
	CDC *dc;
	HDC hDC;

	//get the dc
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the bitmaps
	if(-1 == iDie1)
	{
		m_btnRoll1.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnRoll1.SetBitmaps(VIEW->getDiceImage(iDie1, hDC), COLOR_TRANSP);
		m_btnRoll1.ShowWindow(SW_SHOW);
	}

	//bitmap 2
	if(-1 == iDie2)
	{
		m_btnRoll2.ShowWindow(SW_HIDE);
	}
	else
	{
		m_btnRoll2.SetBitmaps(VIEW->getDiceImage(iDie2, hDC), COLOR_TRANSP);
		m_btnRoll2.ShowWindow(SW_SHOW);
	}

	//release the dc
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// set to the hand when over
/////////////////////////////////////////////////////////////////////////////
BOOL CGoodsButton::OnSetCursor(CWnd* /*pWnd*/, UINT /*nHitTest*/, UINT /*message*/) 
{
	::SetCursor(m_hCursor);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle button clicks
/////////////////////////////////////////////////////////////////////////////
void CGoodsButton::OnGoodsStatic()
{
	if(FALSE == m_bVolcano)
	{
		CONTROLPANEL->OnGoodsButton();
	}
	else
	{
		CONTROLPANEL->OnVolcanoButton();
	}
}