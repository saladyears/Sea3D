/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

#include "stdafx.h"
#include "settlers.h"
#include "PlayerButton.h"
#include "SettlersView.h"
#include "ColorDatabase.h"
#include "memdc.h"
#include "defineDraw.h"
#include "defineVariants.h"
#include "defineIni.h"
#include "defineColors.h"
#include "defineMap.h"
#include ".\playerbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CPlayerButton

CPlayerButton::CPlayerButton()
{
	//reset variables
	resetAll();
}

CPlayerButton::~CPlayerButton()
{
}


BEGIN_MESSAGE_MAP(CPlayerButton, CAnimateButton)
	//{{AFX_MSG_MAP(CPlayerButton)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerButton message handlers

/////////////////////////////////////////////////////////////////////////////
// set to owner draw
/////////////////////////////////////////////////////////////////////////////
BOOL CPlayerButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	
	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// create text statics
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::createText()
{
	CRect rect;

	//set up the rectangle
	if(TRUE == m_bLarge)
	{
		rect = CRect(PLAYER_NAME_OFFSET, PANEL_OFFSET + 1, PLAYER_NAME_OFFSET + LARGE_PLAYER_NAME_WIDTH, LARGE_STATIC_HEIGHT + PANEL_OFFSET + 1);
	}
	else
	{
		rect = CRect(PLAYER_NAME_OFFSET, PANEL_OFFSET + 1, PLAYER_NAME_OFFSET + PLAYER_NAME_WIDTH, STATIC_HEIGHT + PANEL_OFFSET + 1);
	}

	//create the buttons
	m_PlayerName.Create("", CHILD, rect, this, 0);
	m_PlayerName.SetTopBorder(0);
	m_PlayerName.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_PlayerName.SetBackground(COLOR_BLACK);

	//create the point button
	if(TRUE == m_bLarge)
	{
		rect.OffsetRect(0, LARGE_STATIC_HEIGHT);
		rect.right = LARGE_PLAYER_POINTS_WIDTH - PLAYER_NAME_OFFSET;
	}
	else
	{
		rect.OffsetRect(0, STATIC_HEIGHT);
		rect.right = PLAYER_POINTS_WIDTH - PLAYER_NAME_OFFSET;
	}

	//create the total point button
	if(TRUE == m_bLarge)
	{
		rect.OffsetRect(LARGE_PLAYER_POINTS_WIDTH - PLAYER_NAME_OFFSET, 0);
	}
	else
	{
		rect.OffsetRect(PLAYER_POINTS_WIDTH - PLAYER_NAME_OFFSET, 0);
	}

	//set fonts
	if(TRUE == m_bLarge)
	{
		m_PlayerName.SetFont(Tahoma14);
	}
	else
	{
		m_PlayerName.SetFont(Tahoma12);
	}
}

/////////////////////////////////////////////////////////////////////////////
// calculate button width
/////////////////////////////////////////////////////////////////////////////
int CPlayerButton::buttonWidth()
{
	//calculate button width
	if(TRUE == m_bLarge)
	{
		if(MAP->m_iMode & MAP_SEAFARERS)	return LARGE_PLAYER_BUTTON_WIDTH_SEA;
		else return LARGE_PLAYER_BUTTON_WIDTH;
	}
	else
	{
		if(MAP->m_iMode & MAP_SEAFARERS) return PLAYER_BUTTON_WIDTH_SEA;
		else return PLAYER_BUTTON_WIDTH;
	}
}

/////////////////////////////////////////////////////////////////////////////
// create icon bitmaps
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::createIcons()
{
	int iWidth;
	int iStart;
	int iOffset;
	CRect rect;
	CDC *dc;
	HDC hDC;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//get button width
	iWidth = buttonWidth();

	//create rect
	if(TRUE == m_bLarge)
	{
		iStart = LARGE_PANEL_ICON + LARGE_PLAYER_STOCK_TEXT_WIDTH + 2;
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 1, iWidth - iStart + LARGE_PANEL_ICON, PANEL_OFFSET + 1 + LARGE_PANEL_ICON);
		iOffset = LARGE_PANEL_ICON + 2;
	}
	else
	{
		iStart = PANEL_ICON + PLAYER_STOCK_TEXT_WIDTH + 2;
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 1, iWidth - iStart + PANEL_ICON, PANEL_OFFSET + 1 + PANEL_ICON);
		iOffset = PANEL_ICON + 2;
	}	

	//create points button
	m_Points.Create("", CHILD, rect, this, 0);
	m_Points.SetTooltipText("Points total");

	//offset the longest road button
	rect.OffsetRect(0, iOffset);

	//create the largest army button
	m_Army.Create("", CHILD, rect, this, 0);
	m_Army.SetTooltipText("Army size");
	m_Army.ShowWindow(SW_HIDE);

	//offset the longest road button
	rect.OffsetRect(0, iOffset);

	//create longest road button
	m_Road.Create("", CHILD, rect, this, 0);
	m_Road.SetTooltipText("Road Length");
	m_Road.ShowWindow(SW_HIDE);

	if(TRUE == m_bLarge)
	{
		m_Army.SetBitmaps(VIEW->getResImage(IMG_SOLDIER, hDC), COLOR_TRANSP, VIEW->getResImage(IMG_SOLDIER, hDC), COLOR_TRANSP);
		m_Points.SetBitmaps(VIEW->getResImage(IMG_SMILEY, hDC), COLOR_TRANSP, VIEW->getResImage(IMG_SMILEY, hDC), COLOR_TRANSP);
	}
	else
	{
		m_Army.SetBitmaps(VIEW->getHistoryImage(HISTORY_SOLDIER, hDC), COLOR_TRANSP, VIEW->getHistoryImage(HISTORY_SOLDIER, hDC), COLOR_TRANSP);
		m_Points.SetBitmaps(VIEW->getHistoryImage(HISTORY_WIN, hDC), COLOR_TRANSP, VIEW->getHistoryImage(HISTORY_WIN, hDC), COLOR_TRANSP);
	}
	
	//create stock hoverstatics
	if(TRUE == m_bLarge)
	{
		iStart = LARGE_PLAYER_STOCK_TEXT_WIDTH + 1;
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 9, iWidth - 1, PANEL_OFFSET + 9 + LARGE_STATIC_HEIGHT);
	}
	else
	{
		iStart = PLAYER_STOCK_TEXT_WIDTH + 1;
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 2, iWidth - 1, PANEL_OFFSET + 2 + STATIC_HEIGHT);
	}

	//create the point button
	m_P.Create("", CHILD, rect, this, 0);
	m_P.SetBackground(COLOR_BLACK);
	m_P.SetCenter();
	m_P.SetColors(COLOR_WHITE, COLOR_WHITE);

	//offset the longest road button
	rect.OffsetRect(0, iOffset);

	m_A.Create("", CHILD, rect, this, 0);
	m_A.SetBackground(COLOR_BLACK);
	m_A.SetCenter();

	//offset the longest road button
	rect.OffsetRect(0, iOffset);

	m_R.Create("", CHILD, rect, this, 0);
	m_R.SetBackground(COLOR_BLACK);
	m_R.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_R.SetCenter();

	//set fonts
	if(TRUE == m_bLarge)
	{
		m_A.SetFont(Tahoma13);
		m_R.SetFont(Tahoma13);
		m_P.SetFont(Tahoma13);
	}
	else
	{
		m_A.SetFont(Tahoma11);
		m_R.SetFont(Tahoma11);
		m_P.SetFont(Tahoma11);
	}

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// create stock images
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::createStock()
{
	int iIcon;
	int iStart;
	int iOffset;
	int iSeaOffset;
	int iWidth;
	int iTextWidth;
	CRect rect;
	BOOL bSea;

	//set sea flag
	bSea = (MAP->m_iMode & MAP_SEAFARERS);

	//calculate button width
	iWidth = buttonWidth();
	iTextWidth = m_bLarge ? LARGE_PLAYER_STOCK_TEXT_WIDTH : PLAYER_STOCK_TEXT_WIDTH;
	iIcon = m_bLarge ? LARGE_PANEL_ICON : PANEL_ICON;
	iSeaOffset = bSea ? (iIcon + (iIcon / 4) + iTextWidth) : 0;

	if(TRUE == m_bLarge)
	{
		iStart = ((iIcon + iTextWidth + 2) << 1) + 1 + iSeaOffset;
	}
	else
	{
		iStart = ((iIcon + iTextWidth + 2) << 1) + 5 + iSeaOffset;
	}

	//set the creation rect
	rect = CRect(iWidth - iStart, PANEL_OFFSET + 1, iWidth - iStart + iIcon, PANEL_OFFSET + 1 + iIcon);

	//roads
	m_stRoads.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//create sea if needed
	if(TRUE == bSea)
	{
		rect.OffsetRect(iSeaOffset, 0);
		m_stShips.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		rect.OffsetRect(-iSeaOffset, 0);
	}

	//set offset
	iOffset = iIcon + 2;

	//settlement
	rect.OffsetRect(0, iOffset);
	m_stSettle.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//city
	rect.OffsetRect(0, iOffset);
	m_stCity.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//offset the text
	iStart -= iIcon;

	//create static info texts
	if(TRUE == m_bLarge)
	{	
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 9, iWidth - iStart + LARGE_PLAYER_STOCK_TEXT_WIDTH, PANEL_OFFSET + 9 + LARGE_STATIC_HEIGHT);
	}
	else
	{
		rect = CRect(iWidth - iStart, PANEL_OFFSET + 2, iWidth - iStart + PLAYER_STOCK_TEXT_WIDTH, PANEL_OFFSET + 2 + STATIC_HEIGHT);
	}

	if(TRUE == bSea)
	{
		rect.OffsetRect(iSeaOffset, 0);
		m_hShips.Create("", CHILD, rect, this, 0);
		m_hShips.SetBackground(COLOR_BLACK);
		m_hShips.ShowWindow(SW_HIDE);
		m_hShips.SetCenter();
		rect.OffsetRect(-iSeaOffset, 0);
	}

	m_hRoads.Create("", CHILD, rect, this, 0);
	m_hRoads.SetBackground(COLOR_BLACK);
	m_hRoads.ShowWindow(SW_HIDE);
	m_hRoads.SetCenter();

	rect.OffsetRect(0, iOffset);
	m_hSettle.Create("", CHILD, rect, this, 0);
	m_hSettle.SetBackground(COLOR_BLACK);
	m_hSettle.ShowWindow(SW_HIDE);
	m_hSettle.SetCenter();

	rect.OffsetRect(0, iOffset);
	m_hCity.Create("", CHILD, rect, this, 0);
	m_hCity.SetBackground(COLOR_BLACK);
	m_hCity.ShowWindow(SW_HIDE);
	m_hCity.SetCenter();

	//set fonts
	if(TRUE == m_bLarge)
	{
		m_hShips.SetFont(Tahoma13);
		m_hRoads.SetFont(Tahoma13);
		m_hSettle.SetFont(Tahoma13);
		m_hCity.SetFont(Tahoma13);
	}
	else
	{
		m_hShips.SetFont(Tahoma11);
		m_hRoads.SetFont(Tahoma11);
		m_hSettle.SetFont(Tahoma11);
		m_hCity.SetFont(Tahoma11);
	}
}

/////////////////////////////////////////////////////////////////////////////
// set up all the hoverstatic buttons
/////////////////////////////////////////////////////////////////////////////
int CPlayerButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CAnimateButton::OnCreate(lpCreateStruct) == -1)
		return -1;

	//get large fonts value
	m_bLarge = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);

	//create the text
	createText();

	//create icons
	createIcons();

	//create stock images
	createStock();

	//set startup point value
	m_P.SetText("0");

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// create resource images
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::createRes()
{
	int i;
	int iOffset;
	int iSpacer;
	CRect rect, rectB;
	CHoverStatic *pStatic;
	CStatic *pBitmap;
	CDC *dc;
	HDC hDC;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//reset the rect
	if(TRUE == m_bLarge)
	{
		rect  = CRect(PLAYER_NAME_OFFSET, LARGE_PLAYER_RES_OFFSET + LARGE_PLAYER_RES_HEIGHT, PLAYER_NAME_OFFSET + LARGE_PLAYER_RES_TEXT_WIDTH, LARGE_PLAYER_RES_OFFSET + LARGE_PLAYER_RES_HEIGHT + STATIC_HEIGHT);
		rectB = CRect(PLAYER_NAME_OFFSET, LARGE_PLAYER_RES_OFFSET, PLAYER_NAME_OFFSET + LARGE_PLAYER_RES_WIDTH, LARGE_PLAYER_RES_OFFSET + LARGE_PLAYER_RES_HEIGHT);
		iSpacer = LARGE_PLAYER_RES_WIDTH + LARGE_PLAYER_RES_SPACER;
	}
	else
	{
		rect  = CRect(PLAYER_NAME_OFFSET, PLAYER_RES_OFFSET + PLAYER_RES_HEIGHT, PLAYER_NAME_OFFSET + PLAYER_RES_TEXT_WIDTH, PLAYER_RES_OFFSET + PLAYER_RES_HEIGHT + STATIC_HEIGHT);
		rectB = CRect(PLAYER_NAME_OFFSET, PLAYER_RES_OFFSET, PLAYER_NAME_OFFSET + PLAYER_RES_WIDTH, PLAYER_RES_OFFSET + PLAYER_RES_HEIGHT);
		iSpacer = PLAYER_RES_WIDTH + PLAYER_RES_SPACER;
	}

	//add the resource buttons
	for(i = 0; i < 6; i++)
	{
		pBitmap = &(m_stGoods[i]);
		pStatic = &(m_hGoods[i]);

		pBitmap->Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rectB, this);
		if((GAME_MODE_NETWORK == GAME->m_iMode) && (m_iIndex != GAME->m_iIndex) && (FALSE == IS_VARIANT(VARIANT_SHOW_ALL_CARDS)))
		{
			if(i < 5)
			{
				if(TRUE == m_bLarge)
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_RESCARD, hDC));
				}
				else
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_RESCARD_S, hDC));
				}
			}
			else
			{
				if(TRUE == m_bLarge)
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_DEVCARD, hDC));
				}
				else
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_DEVCARD_S, hDC));
				}
			}
		}
		else
		{
			if(i < 5)
			{
				if(TRUE == m_bLarge)
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_TIMBER + i, hDC));
				}
				else
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_TIMBER_S + i, hDC));
				}
			}
			else
			{
				if(TRUE == m_bLarge)
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_DEVCARD, hDC));
				}
				else
				{
					pBitmap->SetBitmap(VIEW->getResImage(IMG_DEVCARD_S, hDC));
				}
			}
		}
		pBitmap->ShowWindow(SW_HIDE);

		pStatic->Create("", CHILD, rect, this, 0);
		pStatic->SetTopBorder(0);
		pStatic->SetCenter();
		pStatic->SetColors(COLOR_WHITE, COLOR_WHITE);
		pStatic->SetBackground(COLOR_BLACK);
		pStatic->ShowWindow(SW_HIDE);

		//set font
		if(TRUE == m_bLarge)
		{
			pStatic->SetFont(Tahoma14);
		}
		else
		{
			pStatic->SetFont(Tahoma12);
		}

		//create the jungle bitmap if needed
		if(IS_VARIANT(VARIANT_USE_JUNGLE) && (5 == i))
		{
			CRect rectJ = rectB;
			
			if(TRUE == m_bLarge)
			{
				rectJ.OffsetRect(-10, -25);
				rectJ.right = rectJ.left + 24;
				rectJ.bottom = rectJ.top + 24;
			}
			else
			{
				rectJ.OffsetRect(-8, -17);
				rectJ.right = rectJ.left + 16;
				rectJ.bottom = rectJ.top + 16;
			}

			m_stJungle.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rectJ, this);
			m_stJungle.ShowWindow(SW_HIDE);

			//text
			if(TRUE == m_bLarge)
			{
				m_stJungle.SetBitmap(VIEW->getResImage(IMG_JUNGLE, hDC));
				rectJ.OffsetRect(27, 8);
				rectJ.right = rectJ.left + 13;
				rectJ.bottom = rectJ.top + 13;
				m_hJungle.SetFont(Tahoma13);
			}
			else
			{
				m_stJungle.SetBitmap(VIEW->getResImage(IMG_JUNGLE_S, hDC));
				rectJ.OffsetRect(17, 5);
				rectJ.right = rectJ.left + 9;
				rectJ.bottom = rectJ.top + 11;
				m_hJungle.SetFont(Tahoma11);
			}
			
			m_hJungle.Create("", CHILD, rectJ, this, 0);
			m_hJungle.SetTopBorder(0);
			m_hJungle.SetCenter();
			m_hJungle.SetColors(COLOR_WHITE, COLOR_WHITE);
			m_hJungle.SetBackground(COLOR_BLACK);
			m_hJungle.SetText("1");
			m_hJungle.ShowWindow(SW_HIDE);

			//set up tooltip for jungle
			m_JungleTip.Create(&m_stJungle);
			m_stJungle.GetWindowRect(rectJ);
			ScreenToClient(rectJ);
			m_JungleTip.AddTool(this, "Jungle Discoveries", rectJ, 1);
			m_JungleTip.Activate(TRUE);
			m_JungleTip.SetDelayTime(150);
		}

		if(i < 5)
		{
			iOffset = iSpacer;
		}
		else
		{
			iOffset = iSpacer + 5;
		}

		rect.OffsetRect(iOffset, 0);
		rectB.OffsetRect(iOffset, 0);

		//create tooltips
		if(i < 5)
		{
			CRect rect;

			//set up tooltip
			m_ResTips[i].Create(pBitmap);
			pBitmap->GetWindowRect(rect);
			ScreenToClient(rect);
			m_ResTips[i].AddTool(this, "", rect, 1);
			m_ResTips[i].Activate(TRUE);
			m_ResTips[i].SetDelayTime(150);
		}
	}

	//set up tooltip for dev card
	m_ToolTip.Create(&(m_stGoods[5]));
	m_stGoods[5].GetWindowRect(rect);
	ScreenToClient(rect);
	m_ToolTip.AddTool(this, "", rect, 1);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetDelayTime(150);

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// set the index properly
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::setIndex(int iIndex)
{
	int iWidth;
	int iImage = 0;
	CDC *dc;
	HDC hDC;

	//calculate button width
	iWidth = buttonWidth();

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the index
	m_iIndex = iIndex;

	//clean out bitmaps
	cleanup();

	//create res buttons
	createRes();

	//get the color
	m_color = COLORS->getTextColor(GAME->m_players[m_iIndex].m_iColor);
	m_colorOutline = COLORS->getDrawColor(GAME->m_players[m_iIndex].m_iColor);

	//if we're white, then darken it
	if(0 == GAME->m_players[m_iIndex].m_iColor)
	{
		m_colorOutline = COLOR_GRAY;
	}

	//get the longest road image
	switch(GAME->m_players[m_iIndex].m_iColor)
	{
	case CR_WHITE:	iImage = (m_bLarge ? IMG_ROADW : HISTORY_ROADW); break;
	case CR_RED:	iImage = (m_bLarge ? IMG_ROADR : HISTORY_ROADR); break;
	case CR_GREEN:	iImage = (m_bLarge ? IMG_ROADG : HISTORY_ROADG); break;
	case CR_BLUE:	iImage = (m_bLarge ? IMG_ROADB : HISTORY_ROADB); break;
	case CR_ORANGE: iImage = (m_bLarge ? IMG_ROADO : HISTORY_ROADO); break;
	case CR_BROWN:	iImage = (m_bLarge ? IMG_ROADBN : HISTORY_ROADBN); break;
	}

	//set the name
	m_PlayerName.SetText(GAME->m_players[m_iIndex].m_player.getName());
	m_PlayerName.SetColors(m_color, m_color);

	//set the longest road image
	if(TRUE == m_bLarge)
	{
		m_Road.SetBitmaps(VIEW->getResImage(iImage, hDC), COLOR_TRANSP, VIEW->getResImage(iImage, hDC), COLOR_TRANSP);
	}
	else
	{
		m_Road.SetBitmaps(VIEW->getHistoryImage(iImage, hDC), COLOR_TRANSP, VIEW->getHistoryImage(iImage, hDC), COLOR_TRANSP);	
	}

	//set static bitmaps
	if(TRUE == m_bLarge)
	{
		if(NULL != m_stShips.m_hWnd)
		{
			m_stShips.SetBitmap(VIEW->getResImage(iImage + 6, dc->GetSafeHdc()));
		}
		m_stRoads.SetBitmap(VIEW->getResImage(iImage, dc->GetSafeHdc()));
		m_stSettle.SetBitmap(VIEW->getResImage(iImage - 12, dc->GetSafeHdc()));
		m_stCity.SetBitmap(VIEW->getResImage(iImage - 6, dc->GetSafeHdc()));
	}
	else
	{
		if(NULL != m_stShips.m_hWnd)
		{
			m_stShips.SetBitmap(VIEW->getHistoryImage(iImage + (HISTORY_SHIPW - HISTORY_ROADW), dc->GetSafeHdc()));
		}
		m_stRoads.SetBitmap(VIEW->getHistoryImage(iImage, dc->GetSafeHdc()));
		m_stSettle.SetBitmap(VIEW->getHistoryImage(iImage - 12, dc->GetSafeHdc()));
		m_stCity.SetBitmap(VIEW->getHistoryImage(iImage - 6, dc->GetSafeHdc()));
	}

	//set colors
	m_hShips.SetColors(m_color, m_color);
	if(NULL != m_hShips.m_hWnd)
	{
		m_hShips.ShowWindow(SW_SHOW);
	}

	//set colors
	m_hRoads.SetColors(m_color, m_color);
	m_hRoads.ShowWindow(SW_SHOW);

	//set colors
	m_hSettle.SetColors(m_color, m_color);
	m_hSettle.ShowWindow(SW_SHOW);

	//set colors
	m_hCity.SetColors(m_color, m_color);
	m_hCity.ShowWindow(SW_SHOW);
	
	//determine offset
	if(TRUE == m_bLarge)
	{
		m_nOffsetAmount = iWidth - 60;
	}
	else
	{
		m_nOffsetAmount = iWidth - 40;
	}

	//update auto-hide
	updateAutohide(TRUE);

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// update current resource totals
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::updateCounts()
{
	int i;
	int iRes;
	int iCards;
	int iCount;
	int iTotal = 0;
	BOOL bOver;
	BOOL bWinning = TRUE;
	CString str;
	CString strTemp;

	//first get the total to see if they're over the limit
	for(i = 0; i < 5; i++)
	{
		iTotal += GAME->m_players[m_iIndex].m_iRes[i];
	}

	//now see if we're over
	bOver = iTotal > VIEW->getAllowedCards();

	//set colors
	for(i = 0; i < 5; i++)
	{
		if(TRUE == bOver)
		{
			m_hGoods[i].SetColors(COLOR_RED, COLOR_RED);
		}
		else
		{
			m_hGoods[i].SetColors(COLOR_WHITE, COLOR_WHITE);
		}
	}

	//run through and compare
	for(i = 0; i < 5; i++)
	{
		//get res
		iRes = GAME->m_players[m_iIndex].m_iRes[i];

		//update tooltip text
		if(0 < iRes)
		{
			if(	(GAME_MODE_HOTSEAT == GAME->m_iMode && VIEW->getPlayer() == m_iIndex) ||
				(GAME_MODE_NETWORK == GAME->m_iMode && GAME->m_iIndex == m_iIndex) ||
				(IS_VARIANT(VARIANT_SHOW_ALL_CARDS) ) )
			{
				setResTipText(i, VIEW->getResName(i));
			}
			else
			{
				str.Format("Cheaters never prosper.", iRes);
				setResTipText(i, str);
			}
		}
		else
		{
			if(0 == i && 
				((GAME_MODE_HOTSEAT == GAME->m_iMode && VIEW->getPlayer() != m_iIndex) ||
				(GAME_MODE_NETWORK == GAME->m_iMode && GAME->m_iIndex != m_iIndex) 
				&& (FALSE == IS_VARIANT(VARIANT_SHOW_ALL_CARDS) ) ) )
			{
				str.Format("Cheaters never prosper.", iRes);
				setResTipText(i, str);
			}
			else
			{
				str = "";
			}

			setResTipText(i, str);
		}

		//skip cards that don't need updating
		if(m_iRes[i] == iRes)
		{
			continue;
		}

		//set it
		m_iRes[i] = iRes;

		//format the text
		str.Format("%d", iRes);

		//set the text
		m_hGoods[i].SetText(str);
	}

	//jungles
	if(m_nJungles != GAME->m_players[m_iIndex].m_nJungles)
	{
		m_nJungles = GAME->m_players[m_iIndex].m_nJungles;

		//set it
		str.Format("%d", m_nJungles);
		m_hJungle.SetText(str);

		//show/hide
		m_hJungle.ShowWindow((0 < m_nJungles));
		m_stJungle.ShowWindow((0 < m_nJungles));
	}

	//show the goods
	updateDisplay();
	
	//points
	if(m_iPoints != GAME->m_players[m_iIndex].m_iPoints)
	{
		str.Format("%d", GAME->m_players[m_iIndex].m_iPoints);
		m_P.SetText(str);
		m_iPoints = GAME->m_players[m_iIndex].m_iPoints;
	}

	//compare against everyone else's points
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		//compare
		if(m_iPoints < GAME->m_players[i].m_iPoints)
		{
			bWinning = FALSE;
			break;
		}
	}

	//set color
	if(TRUE == bWinning)
	{
		m_P.SetColors(COLOR_RED, COLOR_RED);
	}
	else
	{
		m_P.SetColors(COLOR_WHITE, COLOR_WHITE);
	}
	
	//dev cards
	iCards = GAME->m_players[m_iIndex].totalDevCards();

	if((m_iCards != iCards) || (GAME_MODE_HOTSEAT == GAME->m_iMode))
	{
		str.Format("%d", iCards);
		m_hGoods[5].SetText(str);
		m_iCards = iCards;

		//show/hide the windows
		m_hGoods[5].ShowWindow(iCards ? SW_SHOW : SW_HIDE);
		m_stGoods[5].ShowWindow(iCards ? SW_SHOW : SW_HIDE);

		//only make the tooltip text if we're not in tracking mode
		if(0 < iCards)
		{
			//clear out the output string
			str.Empty();

			//prepare the tooltip, too
			for(i = 0; i < 5; i++)
			{
				//get the number of cards of each type
				iCount = GAME->m_players[m_iIndex].m_iCardHeld[i] + GAME->m_players[m_iIndex].m_iCardBought[i];

				//add it to the string
				if(0 < iCount)
				{
					strTemp.Format("%d %s card%s, ", iCount, VIEW->getDevName(i), (iCount > 1 ? "s" : ""));
					str += strTemp;
				}
			}

			//get rid of the last comma and space
			str = str.Left(str.GetLength() - 2);

			//can only show this player's in network mode
			if( ((GAME_MODE_NETWORK == GAME->m_iMode) && (m_iIndex != GAME->m_iIndex)) ||
				((GAME_MODE_HOTSEAT == GAME->m_iMode) && (m_iIndex != VIEW->getPlayer())))
			{
				str = "No cheating!";
			}

			//set the tooltip
			setToolTipText(str);
		}
	}

	//soldiers played
	if(m_iSoldiers != GAME->m_players[m_iIndex].m_iCardPlayed[CARD_SOLDIER])
	{
		m_iSoldiers = GAME->m_players[m_iIndex].m_iCardPlayed[CARD_SOLDIER];

		//format the text
		str.Format("%d", m_iSoldiers);

		//set the hover text
		m_A.SetText(str);

		//show/hide army windows
		m_Army.ShowWindow((m_iSoldiers > 0) ? SW_SHOW : SW_HIDE);
		m_A.ShowWindow((m_iSoldiers > 0) ? SW_SHOW : SW_HIDE);
	}

	//if they have largest army, set the color to red
	if(TRUE == GAME->m_players[m_iIndex].m_bLargestArmy)
	{
		m_A.SetColors(COLOR_RED, COLOR_RED);
	}
	//otherwise, white
	else
	{
		m_A.SetColors(COLOR_WHITE, COLOR_WHITE);
	}

	//longest road
	if(m_bLongestRoad != GAME->m_players[m_iIndex].m_bLongestRoad)
	{
		m_bLongestRoad = GAME->m_players[m_iIndex].m_bLongestRoad;

		//set color
		if(TRUE == m_bLongestRoad)
		{
			m_R.SetColors(COLOR_RED, COLOR_RED);
		}
		else
		{
			m_R.SetColors(COLOR_WHITE, COLOR_WHITE);
		}
	}

	//get road length
	if(m_iRoadLength != GAME->m_players[m_iIndex].m_iRoadLength)
	{
		m_iRoadLength = GAME->m_players[m_iIndex].m_iRoadLength;

		//format the text
		str.Format("%d", m_iRoadLength);

		//set the text
		m_R.SetText(str);

		if(0 < m_iRoadLength)
		{
			m_Road.ShowWindow(SW_SHOW);
			m_R.ShowWindow(SW_SHOW);
		}
		else
		{
			m_Road.ShowWindow(SW_HIDE);
			m_R.ShowWindow(SW_HIDE);
		}
	}

	//stock
	if(MAP->m_iMode & MAP_SEAFARERS && m_iShip != GAME->m_players[m_iIndex].m_nStockShips)
	{
		m_iShip = GAME->m_players[m_iIndex].m_nStockShips;
		str.Format("%d", m_iShip);
		m_hShips.SetText(str);
	}

	if(m_iRoad != GAME->m_players[m_iIndex].m_nStockRoads)
	{
		m_iRoad = GAME->m_players[m_iIndex].m_nStockRoads;
		str.Format("%d", m_iRoad);
		m_hRoads.SetText(str);
	}

	if(m_iSettle != GAME->m_players[m_iIndex].m_nStockSettle)
	{
		m_iSettle = GAME->m_players[m_iIndex].m_nStockSettle;
		str.Format("%d", m_iSettle);
		m_hSettle.SetText(str);
	}

	if(m_iCity != GAME->m_players[m_iIndex].m_nStockCities)
	{
		m_iCity = GAME->m_players[m_iIndex].m_nStockCities;
		str.Format("%d", m_iCity);
		m_hCity.SetText(str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// update auto-hide notification
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::updateAutohide(BOOL bFirst)
{
	//call base
	CAnimateButton::updateAutohide();

	//read all the auto-hide values
	m_bAutohideNet = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_NET, FALSE, INI_FILE);
	m_bAutohideOther = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_OTHER, FALSE, INI_FILE);
	m_bAnimate = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_ANIMATE, FALSE, INI_FILE);

	if(TRUE == m_bAnimate && TRUE == bFirst)
	{
		RedrawWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle auto-hiding in various modes
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::handleAutohide()
{
	BOOL bHide = TRUE;

	//make sure we even can hide
	if(FALSE == m_bAutohide)
	{
		bHide = FALSE;
	}

	//see if we SHOULD not autohide
	if((GAME_MODE_NETWORK == GAME->m_iMode && TRUE == m_bAutohideNet && m_iIndex == GAME->m_iIndex) ||
		(GAME_MODE_NETWORK != GAME->m_iMode && TRUE == m_bAutohideOther && m_iIndex == VIEW->getPlayer()))
	{
		bHide = FALSE;
	}

	//auto-hide
	if(TRUE == bHide)
	{
		drawOffset(m_nOffsetAmount);
	}
	else
	{
		drawOffset();
	}
}

/////////////////////////////////////////////////////////////////////////////
// actually move the thing
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::moveOffset(int nOffset)
{
	int iY;
	int iHeight;
	int iDif;
	CRect rect;
	CRgn region;
	CRgn nopaint;
	CRgn combine;

	if(TRUE == m_bLarge)
	{
		iHeight = LARGE_PLAYER_BUTTON_HEIGHT;
		iDif = 3;
	}
	else
	{
		iHeight = PLAYER_BUTTON_HEIGHT;
		iDif = 6;
	}

	//calculate Y
	iY = m_iIndex * (iHeight + iDif);

	//resize
	SetWindowPos(NULL, -nOffset, iY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);

	//set redraw region
	GetClientRect(&rect);
	region.CreateRectRgnIndirect(&rect);
	InvalidateRgn(&region);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the goods
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::showItems(BOOL bShow)
{
	int iShow;
	
	//set show value
	iShow = (bShow ? SW_SHOW : SW_HIDE);

	//hide redraws
	LockWindowUpdate();

	//show/hide player name
	m_PlayerName.ShowWindow(iShow);

	//hide other text
	if(NULL != m_hShips.m_hWnd)
	{
		m_hShips.ShowWindow(iShow);
	}
	m_hRoads.ShowWindow(iShow);
	m_hSettle.ShowWindow(iShow);
	m_hCity.ShowWindow(iShow);

	//hide redraws
	UnlockWindowUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// update the display of goods
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::updateDisplay()
{
	int i;
	int iGoods;
	int iPlayer;
	CString str;
	CStatic *pBitmap;
	CHoverStatic *pStatic;
	CDC *dc;
	HDC hDC;
	HBITMAP hBitmap;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//get current player
	iPlayer = VIEW->getPlayer();

	if (((GAME_MODE_NETWORK == GAME->m_iMode) && (m_iIndex == GAME->m_iIndex)) ||
		((GAME_MODE_HOTSEAT == GAME->m_iMode) && (m_iIndex == iPlayer)) ||
		(IS_VARIANT(VARIANT_SHOW_ALL_CARDS))
		)
	{
		//add the resource buttons
		for(i = 0; i < 5; i++)
		{
			pBitmap = &m_stGoods[i];
			pStatic = &m_hGoods[i];

			//if we're in hotseat mode, we need to set the bitmap back to timber
			if(GAME_MODE_HOTSEAT == GAME->m_iMode && 0 == i)
			{
				if(TRUE == m_bLarge)
				{
					hBitmap = pBitmap->SetBitmap(VIEW->getResImage(IMG_TIMBER, dc->GetSafeHdc()));
				}
				else
				{
					hBitmap = pBitmap->SetBitmap(VIEW->getResImage(IMG_TIMBER_S, dc->GetSafeHdc()));
				}

				//delete old
				if(NULL != hBitmap)
				{
					DeleteObject(hBitmap);
				}

				//set the quantity properly
				str.Format("%d", m_iRes[i]);
				pStatic->SetText(str);
			}

			//show/hide
			pBitmap->ShowWindow(m_iRes[i] ? SW_SHOW : SW_HIDE);
			pStatic->ShowWindow(m_iRes[i] ? SW_SHOW : SW_HIDE);
		}
	}
	else
	{
		//show hide stuff
		pBitmap = &m_stGoods[0];
		pStatic = &m_hGoods[0];

		//if in hotseat mode, must set to back of card
		if(GAME_MODE_HOTSEAT == GAME->m_iMode)
		{
			if(TRUE == m_bLarge)
			{
				hBitmap = pBitmap->SetBitmap(VIEW->getResImage(IMG_RESCARD, dc->GetSafeHdc()));
			}
			else
			{
				hBitmap = pBitmap->SetBitmap(VIEW->getResImage(IMG_RESCARD_S, dc->GetSafeHdc()));
			}

			//delete old
			if(NULL != hBitmap)
			{
				DeleteObject(hBitmap);
			}
		}

		//calculate total goods
		iGoods = GAME->m_players[m_iIndex].totalCards();
		str.Format("%d", iGoods);

		//set the text
		pStatic->SetText(str);
		pBitmap->ShowWindow(iGoods ? SW_SHOW : SW_HIDE);
		pStatic->ShowWindow(iGoods ? SW_SHOW : SW_HIDE);

		//hide all others
		if(GAME_MODE_HOTSEAT == GAME->m_iMode)
		{
			for(i = 1; i < 5; i++)
			{
				//get controls
				pBitmap = &m_stGoods[i];
				pStatic = &m_hGoods[i];

				//hide
				pBitmap->ShowWindow(SW_HIDE);
				pStatic->ShowWindow(SW_HIDE);
			}
		}
	}

	//release the DC
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// set the tooltip text
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::setToolTipText(CString str)
{
	if (TRUE == str.IsEmpty()) 
	{
		return;
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText(str, this, 1);
}

/////////////////////////////////////////////////////////////////////////////
// set res tooltips
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::setResTipText(int iRes, CString str)
{
	if (TRUE == str.IsEmpty()) 
	{
		return;
	}

	//update text
	m_ResTips[iRes].UpdateTipText(str, this, 1);
}

/////////////////////////////////////////////////////////////////////////////
// send knowledge to the tolltip
/////////////////////////////////////////////////////////////////////////////
BOOL CPlayerButton::PreTranslateMessage(MSG* pMsg) 
{
	int i;

	if(NULL != m_ToolTip.m_hWnd)
	{
		m_ToolTip.RelayEvent(pMsg);
	}

	if(NULL != m_JungleTip.m_hWnd)
	{
		m_JungleTip.RelayEvent(pMsg);
	}

	//relay others
	for(i = 0; i < 5; i++)
	{
		if(NULL != m_ResTips[i].m_hWnd)
		{
			m_ResTips[i].RelayEvent(pMsg);
		}
	}

	return CAnimateButton::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// reset variables
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::resetAll()
{
	memset(m_iRes, 0, sizeof(m_iRes));

	m_iPoints = -1;
	m_iCards = -1;

	m_iSoldiers = -1;
	m_bLongestRoad = FALSE;
	m_bTracking = FALSE;
	m_bLarge = FALSE;

	//no road length yet
	m_iRoadLength = -1;

	//no index yet
	m_iIndex = -1;

	//no jungles yet
	m_nJungles = 0;

	// No tooltip created
	m_ToolTip.m_hWnd = NULL;
	m_JungleTip.m_hWnd = NULL;

	//reset totals
	m_iShip = -1;
	m_iRoad = -1;
	m_iSettle = -1;
	m_iCity = -1;

	//reset offset
	m_nOffset = 0;
	m_nOffsetDraw = 0;
	m_nOffsetAmount = 0;
}

/////////////////////////////////////////////////////////////////////////////
// bitmap cleanup
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::cleanup()
{
	int i;
	CStatic *pBitmap;
	HBITMAP hBitmap = NULL;

	//run through list
	for(i = 0; i < 6; i++)
	{
		pBitmap = &m_stGoods[i];
		
		//check for validity
		if(NULL == pBitmap->GetSafeHwnd())
		{
			continue;
		}

		//get bitmap handle
		hBitmap = pBitmap->GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//jungle
	hBitmap = NULL;
	if(NULL != m_stJungle.m_hWnd)
	{
		hBitmap = m_stJungle.GetBitmap();
	}
	
	//destroy
	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}

	//get static buttons
	hBitmap = m_stCity.GetBitmap();

	//destroy
	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}

	//get static buttons
	hBitmap = m_stRoads.GetBitmap();

	//destroy
	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}

	//get static buttons
	if(NULL != m_stShips.m_hWnd)
	{
		hBitmap = m_stShips.GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//get static buttons
	hBitmap = m_stSettle.GetBitmap();

	//destroy
	if(NULL != hBitmap)
	{
		DeleteObject(hBitmap);
	}
}

/////////////////////////////////////////////////////////////////////////////
// stop flicker
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
}

/////////////////////////////////////////////////////////////////////////////
// destroy all windows
/////////////////////////////////////////////////////////////////////////////
BOOL CPlayerButton::DestroyWindow() 
{
	int i;

	//remove bitmaps
	cleanup();

	//largest army
	m_Army.DestroyWindow();
	m_A.DestroyWindow();

	//longest road
	m_Road.DestroyWindow();
	m_R.DestroyWindow();

	//number of points
	m_Points.DestroyWindow();
	m_P.DestroyWindow();

	for(i = 0; i < 6; i++)
	{
		//bitmap pictures
		m_stGoods[i].DestroyWindow();
		m_hGoods[i].DestroyWindow();

		if(i < 5) m_ResTips[i].DestroyWindow();
	}
	//stock pictures
	m_stShips.DestroyWindow();
	m_stRoads.DestroyWindow();
	m_stSettle.DestroyWindow();
	m_stCity.DestroyWindow();
	m_stJungle.DestroyWindow();

	//tooltip for the dev card button
	m_ToolTip.DestroyWindow();
	m_JungleTip.DestroyWindow();

	//hoverstatic information
	m_PlayerName.DestroyWindow();

	//hoverstatic stock info
	m_hShips.DestroyWindow();
	m_hRoads.DestroyWindow();
	m_hSettle.DestroyWindow();
	m_hCity.DestroyWindow();
	m_hJungle.DestroyWindow();

	//reset variables
	resetAll();
	
	return CAnimateButton::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// paint
/////////////////////////////////////////////////////////////////////////////
void CPlayerButton::OnPaint()
{
	int iWidth;
	int iHeight;
	int iLines;
	int iSea;
	BOOL bTurn;
	CPen pen, *pPen;
	CRect rect;
	CBrush brush, *pBrush;
	CPaintDC dc(this);

	//get client rect
	GetClientRect(&rect);

	//get the width
	iWidth = rect.Width();
	iHeight = rect.Height();

	//set the background mode to transparent
	dc.SetBkMode(TRANSPARENT);

	//fill the rectangle
	dc.FillSolidRect(&rect, COLOR_BLACK);

	//determine turn state
	bTurn = (VIEW->getPlayer() == m_iIndex);

	//create the color bar
	pen.CreatePen(PS_SOLID, 1, m_color);
	brush.CreateSolidBrush(m_color);

	//select the old pen
	pPen = dc.SelectObject(&pen);
	pBrush = dc.SelectObject(&brush);

	//deflat top and bottom by one
	rect.DeflateRect(0, 1, 0, 0);
	rect.right = 8;

	//handle offset
	if(m_nOffset == m_nOffsetDraw)
	{
		rect.OffsetRect(m_nOffsetDraw, 0);
	}

	//draw the rectangle
	dc.Rectangle(rect);

	//if turn, draw black rectangle inside
	if(TRUE == bTurn)
	{
		rect.DeflateRect(1, 0, 1, 1);
		
		//draw it
		dc.FillSolidRect(rect, m_colorOutline);
	}

	//set line offset
	iLines = m_bLarge ? 55 : 35;

	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		iSea = m_bLarge ? 115 : 65;
	}
	else
	{
		iSea = iLines;
	}

	//draw the lines
	dc.MoveTo(0, 0);
	dc.LineTo(iWidth, 0);
	dc.MoveTo(0, iHeight - 1);
	dc.LineTo(iWidth, iHeight - 1);

	dc.MoveTo(iWidth - 1, 0);
	dc.LineTo(iWidth - 1, iHeight - 1);

	if(0 == m_nOffset)
	{
		dc.MoveTo(iWidth - iLines, 0);
		dc.LineTo(iWidth - iLines, iHeight - 1);
		dc.MoveTo(iWidth - (iLines + iSea), 0);
		dc.LineTo(iWidth - (iLines + iSea), iHeight - 1);
	}

	//restore the old
	dc.SelectObject(pPen);
	dc.SelectObject(pBrush);

	//delete the objects
	pen.DeleteObject();
	brush.DeleteObject();
}
