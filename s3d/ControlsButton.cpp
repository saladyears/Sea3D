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
#include "ControlsButton.h"
#include "SettlersView.h"
#include "defineColors.h"
#include "defineDraw.h"
#include "defineIni.h"

/////////////////////////////////////////////////////////////////////////////
// CControlsButton
/////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CControlsButton, CAnimateButton)

/////////////////////////////////////////////////////////////////////////////
// constructor
/////////////////////////////////////////////////////////////////////////////
CControlsButton::CControlsButton()
{
	//not mouseable
	m_bMouseable = FALSE;

	//update the animate flag
	updateAnimate();
}

/////////////////////////////////////////////////////////////////////////////
// destructor
/////////////////////////////////////////////////////////////////////////////
CControlsButton::~CControlsButton()
{
}


BEGIN_MESSAGE_MAP(CControlsButton, CAnimateButton)
	ON_BN_CLICKED(IDC_END_BUTTON, OnEndButton)
 	ON_BN_CLICKED(IDC_PLAY_BUTTON, OnPlayButton)
 	ON_BN_CLICKED(IDC_BANK_BUTTON, OnBankButton)
 	ON_BN_CLICKED(IDC_TRADE_BUTTON, OnTradeButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// create controls
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::createControls()
{
	int nX, nY;
	CDC *dc;
	HDC hDC;

	//get the dc
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the offsets
	nX = 5;
	nY = 7;

	//create the action buttons
	m_btnPlay.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_PLAY_BUTTON);
	m_btnPlay.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_ACTION_WIDTH, CONTROL_PANEL_ACTION_HEIGHT, SWP_SHOWWINDOW);
	m_btnPlay.DrawTransparent(TRUE);
	m_btnPlay.DrawFlatFocus(FALSE);
	m_btnPlay.DrawBorder(FALSE);
	m_btnPlay.EnableWindow(FALSE);

	//increment
	nX += CONTROL_PANEL_ACTION_WIDTH + 10;

	m_btnBank.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_BANK_BUTTON);
	m_btnBank.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_ACTION_WIDTH, CONTROL_PANEL_ACTION_HEIGHT, SWP_SHOWWINDOW);
	m_btnBank.DrawTransparent(TRUE);
	m_btnBank.DrawFlatFocus(FALSE);
	m_btnBank.DrawBorder(FALSE);
	m_btnBank.EnableWindow(FALSE);

	//increment
	nX += CONTROL_PANEL_ACTION_WIDTH + 10;

	m_btnTrade.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_TRADE_BUTTON);
	m_btnTrade.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_ACTION_WIDTH, CONTROL_PANEL_ACTION_HEIGHT, SWP_SHOWWINDOW);
	m_btnTrade.DrawTransparent(TRUE);
	m_btnTrade.DrawFlatFocus(FALSE);
	m_btnTrade.DrawBorder(FALSE);
	m_btnTrade.EnableWindow(FALSE);

	//increment
	nX += CONTROL_PANEL_ACTION_WIDTH + 10;

	m_btnEnd.Create("", CHILD, CRect(0, 0, 0, 0), this, IDC_END_BUTTON);
	m_btnEnd.SetWindowPos(&wndTop, nX, nY, CONTROL_PANEL_ACTION_WIDTH, CONTROL_PANEL_ACTION_HEIGHT, SWP_SHOWWINDOW);
	m_btnEnd.DrawTransparent(TRUE);
	m_btnEnd.DrawFlatFocus(FALSE);
	m_btnEnd.DrawBorder(FALSE);
	m_btnEnd.EnableWindow(FALSE);

	//set the bitmaps to disabled at first
	m_btnPlay.SetBitmaps(VIEW->getControlImage(BUTTON_PLAY_GRAY, hDC), COLOR_TRANSP, VIEW->getControlImage(BUTTON_PLAY_GRAY, hDC), COLOR_TRANSP);
	m_btnBank.SetBitmaps(VIEW->getControlImage(BUTTON_BANK_GRAY, hDC), COLOR_TRANSP, VIEW->getControlImage(BUTTON_BANK_GRAY, hDC), COLOR_TRANSP);
	m_btnTrade.SetBitmaps(VIEW->getControlImage(BUTTON_TRADE_GRAY, hDC), COLOR_TRANSP, VIEW->getControlImage(BUTTON_TRADE_GRAY, hDC), COLOR_TRANSP);
	m_btnEnd.SetBitmaps(VIEW->getControlImage(BUTTON_END_GRAY, hDC), COLOR_TRANSP, VIEW->getControlImage(BUTTON_END_GRAY, hDC), COLOR_TRANSP);

	//release the DC
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::OnEndButton()
{
	CONTROLPANEL->OnEndButton();
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::OnBankButton()
{
	CONTROLPANEL->OnBankButton();
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::OnPlayButton()
{
	CONTROLPANEL->OnPlayButton();
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::OnTradeButton()
{
	CONTROLPANEL->OnTradeButton();
}

/////////////////////////////////////////////////////////////////////////////
// window setup
/////////////////////////////////////////////////////////////////////////////
BOOL CControlsButton::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= BS_OWNERDRAW;

	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting
	
	//get the client area
	GetClientRect(&rect);

	//fill the background
	dc.FillSolidRect(rect, COLOR_DKGRAY);
	rect.DeflateRect(1, 0, 1, 1);
	dc.FillSolidRect(rect, COLOR_BLACK);
}

/////////////////////////////////////////////////////////////////////////////
// animate the show/hide
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::startShow(BOOL bShow)
{
	if(TRUE == bShow)
	{
		drawOffset(0);
	}
	else
	{
		drawOffset(43);
	}

	//ensure the view has the focus
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// actually move the thing
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::moveOffset(int nOffset)
{
	CRect rect;
	
	//get current window pos
	VIEW->GetClientRect(&rect);

	//resize
	SetWindowPos(NULL, rect.Width() - PANEL_WIDTH - 4 - CONTROLS_WIDTH, -nOffset, 0, 0, SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE);
}

/////////////////////////////////////////////////////////////////////////////
// update the animate value
/////////////////////////////////////////////////////////////////////////////
void CControlsButton::updateAnimate()
{
	m_bAnimate = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, 1, INI_FILE);

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