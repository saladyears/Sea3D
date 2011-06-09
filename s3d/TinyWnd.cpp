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
#include "TinyWnd.h"
#include "define.h"
#include "SettlersView.h"
#include "defineDraw.h"
#include "defineColors.h"
#include ".\tinywnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTinyWnd

CTinyWnd::CTinyWnd()
{
	m_ToolTip.m_hWnd = NULL;
}

CTinyWnd::~CTinyWnd()
{
	//clean up remaining bitmap
	HBITMAP hBitmap = NULL;

	//check for button
	if(NULL == m_button.GetSafeHwnd())
	{
		return;
	}

	//get bitmap
	hBitmap = m_button.GetBitmap();

	//destroy
	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}
}


BEGIN_MESSAGE_MAP(CTinyWnd, CWnd)
	//{{AFX_MSG_MAP(CTinyWnd)
	ON_BN_CLICKED(ID_TINY_BUTTON, OnButton)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTinyWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// handle startup
/////////////////////////////////////////////////////////////////////////////
void CTinyWnd::init()
{
	int i;
	CRect rect;

	//get the client rectangle
	GetClientRect(&rect);

	//create the button
	m_button.Create("", WS_CHILD | WS_VISIBLE | BS_BITMAP | BS_FLAT, rect, this, ID_TINY_BUTTON);

	//create the tooltip
	m_ToolTip.Create(this);
	m_ToolTip.AddTool(&m_button, "", rect, 1);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetDelayTime(150);

	//reset rect
	rect.left = rect.right - PANEL_ICON;
	rect.top = rect.bottom - PANEL_ICON;
	rect.OffsetRect(-2, -2);

	//create trade buttons
	for(i = 0; i < 5; i++)
	{
		m_trades[i].Create("", CHILD, rect, &m_button, 0);
		m_trades[i].DrawTransparent(TRUE);
		m_trades[i].DrawFlatFocus(FALSE);
		m_trades[i].DrawBorder(FALSE);
		m_trades[i].EnableWindow(FALSE);

		//set rect
		if(i < 2)
		{
			rect.OffsetRect(0, -16);
		}
		else
		{
			rect.OffsetRect(-16, 0);
		}

		//reset to bottom if needed
		if(2 == i)
		{
			rect.OffsetRect(0, 32);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// load in the right bitmap
/////////////////////////////////////////////////////////////////////////////
void CTinyWnd::updateTiny(BOOL bInitial)
{
	int i;
	int iOffset = 0;
	HBITMAP hBitmap = NULL;
	HBITMAP hOld = NULL;
	CDC *dc;

	//set the initial flag
	m_bInitial = bInitial;

	//get the drawable dc
	dc = GetDC();

	//if we're the dev card set the bitmaps here
	if(GL_DEVCARD == m_iType)
	{
		hBitmap = VIEW->getTinyImage(TINY_DEVCARD, dc->GetSafeHdc());
	}
	else
	{
		//get the type offset
		switch(m_iType)
		{
		case GL_CITY: iOffset = TINY_CITYW; break;
		case GL_ROAD: iOffset = TINY_ROADW; break;
		case GL_SETTLEMENT: iOffset = TINY_SETTLEW; break;
		case GL_SHIP_MOVE:
		case GL_SHIP: iOffset = TINY_SHIPW; break;
		}

		//set the bitmap correctly
		switch(GAME->m_players[VIEW->getPlayer()].m_iColor)
		{
		case CR_WHITE: hBitmap = VIEW->getTinyImage(iOffset + 0, dc->GetSafeHdc()); break;
		case CR_BLUE: hBitmap = VIEW->getTinyImage(iOffset + 1, dc->GetSafeHdc()); break;
		case CR_GREEN: hBitmap = VIEW->getTinyImage(iOffset + 2, dc->GetSafeHdc()); break;
		case CR_ORANGE: hBitmap = VIEW->getTinyImage(iOffset + 3, dc->GetSafeHdc()); break;
		case CR_RED: hBitmap = VIEW->getTinyImage(iOffset + 4, dc->GetSafeHdc()); break;
		case CR_BROWN: hBitmap = VIEW->getTinyImage(iOffset + 5, dc->GetSafeHdc()); break;
		}
	}

	//set the bitmap
	hOld = m_button.SetBitmap(hBitmap);

	//delete the old one
	if(hOld) ::DeleteObject(hOld);

	//return the context
	ReleaseDC(dc);

	//get the drawable dc
	dc = m_button.GetDC();

	//update the bitmaps
	for(i = 0; i < 5; i++)
	{
		if(GL_SHIP_MOVE == m_iType)
		{
			m_trades[i].SetBitmaps(VIEW->getHistoryImage(HISTORY_MOVE, dc->GetSafeHdc()), COLOR_BLACK, VIEW->getHistoryImage(HISTORY_MOVE, dc->GetSafeHdc()), COLOR_BLACK);
		}
		else
		{
			m_trades[i].SetBitmaps(VIEW->getHistoryImage(HISTORY_TRADE, dc->GetSafeHdc()), COLOR_BLACK, VIEW->getHistoryImage(HISTORY_TRADE, dc->GetSafeHdc()), COLOR_BLACK);
		}
	}

	//return the context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// button handler
/////////////////////////////////////////////////////////////////////////////
void CTinyWnd::OnButton()
{
	//see if we need to trade to the bank
	if((TRUE == VALID_BUILD(m_iType)) && (FALSE == m_bInitial))
	{
		if(-1 == VIEW->tradeToBank(m_iType))
		{
			return;
		}
	}

	switch(m_iType)
	{
	case GL_ROAD:
	case GL_CITY:
	case GL_SETTLEMENT:
	case GL_SHIP:
		if(TRUE == m_bInitial)
		{
			VIEW->setInitialMode(m_iType);
		}
		else
		{
			VIEW->addUndo(UNDO_ACTION, NULL);
			VIEW->setDrawingMode(m_iType);
		}
		break;
	//if we're the dev card, we've got work to do
	case GL_DEVCARD:
		buyDevCard();
		break;
	case GL_SHIP_MOVE:
		VIEW->setMoveShipMode();
		break;
	}

	//return focus to view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// buy an actual dev card
/////////////////////////////////////////////////////////////////////////////
void CTinyWnd::buyDevCard()
{
	int iCard = 0;
	int iPlayer;
	CString str;
	
	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the specific card that they're buying
	iCard = VIEW->buyDevCard();

	//tell the user what kind of card they got
	if(GAME_MODE_NETWORK != GAME->m_iMode)
	{
		str.Format("Got a %s card.", VIEW->getDevName(iCard));
		MessageBox(str, "Development Card");
	}

	//handle common
	VIEW->handleCommonBuyDevCard(iPlayer, iCard);

	//handle network stuff
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientBuyDevCard(iCard);
	}

	//update the build options
	VIEW->showBuildOptions(FALSE);
	VIEW->showBuildOptions(TRUE);

	//recheck dc
	CONTROLPANEL->EnableDC();
}

/////////////////////////////////////////////////////////////////////////////
// handle showing trade options
/////////////////////////////////////////////////////////////////////////////
void CTinyWnd::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	int i;
	int iTrades;
	CString str, strTemp;

	CWnd::OnShowWindow(bShow, nStatus);
	
	//if we're not being seen, it doesn't matter
	if(FALSE == bShow || NULL == GAME || FALSE == GAME->m_bInit)
	{
		return;
	}

	//see how many trades we'd need to make to get this res
	iTrades = VIEW->tradeToBank(m_iType, TRUE);

	//set the ship move to show the arrow
	if(GL_SHIP_MOVE == m_iType)
	{
		iTrades = 1;
	}
	
	//run through and refresh the icons
	for(i = 0; i < 5; i++)
	{
		if(NULL != m_trades[i].GetSafeHwnd())
		{
			//turn it on or off
			if(i < iTrades)
			{
				m_trades[i].ShowWindow(SW_SHOW);
			}
			else
			{
				m_trades[i].ShowWindow(SW_HIDE);
			}
		}
	}

	switch(m_iType)
	{
	case GL_ROAD: str = "Build a road"; break;
	case GL_SHIP: str = "Build a ship"; break;
	case GL_SETTLEMENT: str = "Build a settlement"; break;
	case GL_CITY: str = "Build a city"; break;
	case GL_DEVCARD: str = "Buy a development card"; break;
	case GL_SHIP_MOVE: str = "Move a ship"; break;
	}

	//add trades to bank
	if((0 < iTrades) && (GL_SHIP_MOVE != m_iType))
	{
		strTemp.Format(" (%d trade%s to bank)", iTrades, iTrades > 1 ? "s" : "");
		str += strTemp;
	}

	//update tooltip
	m_ToolTip.UpdateTipText(str, &m_button, 1);
}

/////////////////////////////////////////////////////////////////////////////
// update tooltips
/////////////////////////////////////////////////////////////////////////////
BOOL CTinyWnd::PreTranslateMessage(MSG* pMsg) 
{	
	if(NULL != m_ToolTip.m_hWnd)
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	return CWnd::PreTranslateMessage(pMsg);
}