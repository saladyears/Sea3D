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
#include "ActionsButton.h"
#include "SettlersView.h"
#include "defineColors.h"
#include "defineDraw.h"
#include "defineIni.h"

/////////////////////////////////////////////////////////////////////////////
// CActionsButton
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CActionsButton, CAnimateButton)
CActionsButton::CActionsButton()
{
	//update the animate flag
	updateAnimate();
}

CActionsButton::~CActionsButton()
{
}


BEGIN_MESSAGE_MAP(CActionsButton, CAnimateButton)
	ON_BN_CLICKED(IDC_GOODS_BUTTON, OnGoodsButton)
 	ON_BN_CLICKED(IDC_VOLCANO_BUTTON, OnVolcanoButton)
 	ON_BN_CLICKED(IDC_ROLL_BUTTON, OnRollButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// create controls
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::createControls()
{
	int nX, nY;

	//set offsets
	nX = 2;
	nY = PANEL_OFFSET;

	//create the multiplayer roll button
	m_btnRoll.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_ROLL_BUTTON);
	m_btnRoll.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_GOODS_WIDTH + 2, CONTROL_PANEL_ROLL_HEIGHT * 4, SWP_HIDEWINDOW);
	m_btnRoll.SetFont(Tahoma12);
	m_btnRoll.SetCenterClick();
	m_btnRoll.SetTopBorder((CONTROL_PANEL_ROLL_HEIGHT * 2) - 5);
	m_btnRoll.SetText("Click to roll (or press Ctrl+R)");
	m_btnRoll.SetBackground(COLOR_BLACK);
	m_btnRoll.SetColors(COLOR_WHITE, COLOR_BLUE);
	m_btnRoll.TurnOnBorder(COLOR_WHITE);

	//create the goods button
	m_btnGoods.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_GOODS_BUTTON);
	m_btnGoods.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_GOODS_WIDTH + 2, CONTROL_PANEL_GOODS_BUTTON, SWP_HIDEWINDOW);
	m_btnGoods.createControls();

	//create the volcano button
	m_btnVolcano.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_VOLCANO_BUTTON);
	m_btnVolcano.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_GOODS_WIDTH + 2, CONTROL_PANEL_GOODS_BUTTON, SWP_HIDEWINDOW);
	m_btnVolcano.createControls(TRUE);
	m_btnVolcano.ShowWindow(SW_HIDE);

}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::OnRollButton()
{
	CONTROLPANEL->OnRollButton();
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::OnGoodsButton()
{
	CONTROLPANEL->OnGoodsButton();
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::OnVolcanoButton()
{
	CONTROLPANEL->OnVolcanoButton();
}

/////////////////////////////////////////////////////////////////////////////
// window setup
/////////////////////////////////////////////////////////////////////////////
BOOL CActionsButton::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= BS_OWNERDRAW;

	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting

	//get the client area
	GetClientRect(&rect);

	//fill the background
	dc.FillSolidRect(rect, COLOR_DKGRAY);
	rect.DeflateRect(1, 1, 0, 1);
	dc.FillSolidRect(rect, COLOR_BLACK);
}

/////////////////////////////////////////////////////////////////////////////
// animate the show/hide
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::startShow(BOOL bShow)
{
	if(TRUE == bShow)
	{
		drawOffset(0);
	}
	else
	{
		drawOffset(PANEL_WIDTH + 1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// actually move the thing
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::moveOffset(int nOffset)
{
	CRect rect;
	
	//get view dimensions
	VIEW->GetClientRect(&rect);

	//resize
	SetWindowPos(NULL, rect.Width() - PANEL_WIDTH - 1 + nOffset, CONTROLS_HEIGHT + 4, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

/////////////////////////////////////////////////////////////////////////////
// update the animate value
/////////////////////////////////////////////////////////////////////////////
void CActionsButton::updateAnimate()
{
	CRect rect;

	m_bAnimate = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, 1, INI_FILE);

	//reset offset if needed
	if(FALSE == m_bAnimate)
	{
		//hide the window if needed
		if(0 != m_nOffset)
		{
			CWnd::ShowWindow(SW_HIDE);
		}

		//reset offset
		drawOffset(0);
	}
	//see if it needs to be moved
	else if(NULL != m_hWnd)
	{
		//get client area
		GetClientRect(&rect);

		if(FALSE == IsWindowVisible())
		{
			drawOffset(rect.Width() + 1);
		}
	}
}
