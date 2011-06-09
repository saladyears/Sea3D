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
#include "LegendButton.h"
#include "SettlersView.h"
#include "ColorDatabase.h"
#include "defineDraw.h"
#include "defineIni.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLegendButton

CLegendButton::CLegendButton()
{
	//no index yet
	m_iIndex = -1;

	//get large fonts value
	m_bLarge = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);
}

CLegendButton::~CLegendButton()
{
}


BEGIN_MESSAGE_MAP(CLegendButton, CButton)
	//{{AFX_MSG_MAP(CLegendButton)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLegendButton message handlers

/////////////////////////////////////////////////////////////////////////////
// set drawing style
/////////////////////////////////////////////////////////////////////////////
BOOL CLegendButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	
	return CButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// handle creation
/////////////////////////////////////////////////////////////////////////////
int CLegendButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int i;
	int dx, dy;
	int iImage = 0;
	CRect rect;
	CRect statrect;
	CRect costrect;
	CString str;
	CDC *dc;
	HDC hDC;
	int iResWidth;
	int iResHeight;
	int iCardOffset;
	int iYOffset;
	int iXOffset;

	if (CButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//get some values
	if(FALSE == m_bLarge)
	{
		iResWidth = LEGEND_RES_WIDTH;
		iResHeight = LEGEND_RES_HEIGHT;
		iCardOffset = 5;

		//set the jump amount
		iXOffset = iResWidth + LEGEND_RES_SPACER_X;
		iYOffset = iResHeight + LEGEND_RES_SPACER_Y;
	}
	else
	{
		iResWidth = LARGE_LEGEND_RES_WIDTH;
		iResHeight = LARGE_LEGEND_RES_HEIGHT;
		iCardOffset = 0;

		//set the jump amount
		iXOffset = iResWidth + LARGE_LEGEND_RES_SPACER_X;
		iYOffset = iResHeight + LARGE_LEGEND_RES_SPACER_Y;
	}

	//set dev card image
	if(FALSE == m_bLarge)
	{
		rect = CRect(LEGEND_IMAGE_START_X, LEGEND_RES_START_Y, LEGEND_IMAGE_START_X + iResWidth, LEGEND_RES_START_Y + iResHeight);
	}
	else
	{
		rect = CRect(LARGE_LEGEND_IMAGE_START_X, LEGEND_RES_START_Y, LARGE_LEGEND_IMAGE_START_X + iResWidth, LEGEND_RES_START_Y + iResHeight);
	}

	m_stDev.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
	m_stDev.SetBitmap(VIEW->getResImage(m_bLarge ? IMG_DEVCARD : IMG_DEVCARD_S, dc->GetSafeHdc()));

	//set road image
	if(FALSE == m_bLarge)
	{
		rect = CRect(LEGEND_IMAGE_START_X, LEGEND_IMAGE_START_Y, LEGEND_IMAGE_START_X + LEGEND_IMAGE_WIDTH, LEGEND_IMAGE_START_Y + LEGEND_IMAGE_HEIGHT);
	}
	else
	{
		rect = CRect(LARGE_LEGEND_IMAGE_START_X, LARGE_LEGEND_IMAGE_START_Y, LARGE_LEGEND_IMAGE_START_X + LARGE_LEGEND_IMAGE_WIDTH, LARGE_LEGEND_IMAGE_START_Y + LARGE_LEGEND_IMAGE_HEIGHT);
	}
	m_stRoad.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//set ship image
	rect.OffsetRect(0, iYOffset);
	m_stShip.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
	
	//set settlement image
	rect.OffsetRect(0, iYOffset);
	m_stSettle.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//set city image
	rect.OffsetRect(0, iYOffset);
	m_stCity.Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);

	//create equals buttons
	if(FALSE == m_bLarge)
	{
		rect = CRect(LEGEND_EQUALS_START_X, LEGEND_EQUALS_START_Y, LEGEND_EQUALS_START_X + LEGEND_EQUALS_WIDTH, LEGEND_EQUALS_START_Y + LEGEND_EQUALS_HEIGHT);
	}
	else
	{
		rect = CRect(LARGE_LEGEND_EQUALS_START_X, LARGE_LEGEND_EQUALS_START_Y, LARGE_LEGEND_EQUALS_START_X + LARGE_LEGEND_EQUALS_WIDTH, LARGE_LEGEND_EQUALS_START_Y + LARGE_LEGEND_EQUALS_HEIGHT);
	}

	//create all the equals
	for(i = 0; i < 5; i++)
	{
		m_hEquals[i].Create("", CHILD, rect, this, 0);
		if(FALSE == m_bLarge)
		{
			m_hEquals[i].SetFont(Tahoma12);
		}
		else
		{
			m_hEquals[i].SetFont(Tahoma14);
		}
		m_hEquals[i].SetTopBorder(0);
		m_hEquals[i].SetCenter();
		m_hEquals[i].SetBackground(COLOR_BLACK);
		m_hEquals[i].SetText("=");

		//offset
		rect.OffsetRect(0, iYOffset);
	}

	//set rect
	statrect = CRect(LEGEND_RES_START_X, LEGEND_RES_START_Y, LEGEND_RES_START_X + iResWidth, LEGEND_RES_START_Y + iResHeight);
	rect = statrect;

	//set up dev card bitmaps
	for(i = 0; i < 3; i++)
	{
		//get the image
		switch(i)
		{
		case 0: iImage = IMG_WHEAT; rect.OffsetRect(iXOffset, 0); break;
		case 1: iImage = IMG_ORE; break;
		case 2: iImage = IMG_SHEEP; rect.OffsetRect(iXOffset, 0); break;
		}
		m_btnDev[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		m_btnDev[i].SetBitmap(VIEW->getResImage(iImage + iCardOffset, dc->GetSafeHdc()));

		//offset the rect
		rect.OffsetRect(iXOffset, 0);
	}

	//offset
	rect = statrect;
	rect.OffsetRect(0, iYOffset);

	//set up road bitmaps
	for(i = 0; i < 2; i++)
	{
		//get the image
		switch(i)
		{
		case 0: iImage = IMG_TIMBER; break;
		case 1: iImage = IMG_CLAY; rect.OffsetRect(iXOffset << 1, 0); break;
		}
		m_btnRoad[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		m_btnRoad[i].SetBitmap(VIEW->getResImage(iImage + iCardOffset, dc->GetSafeHdc()));

		//offset the rect
		rect.OffsetRect(iXOffset, 0);
	}

	//set rect
	rect = statrect;
	rect.OffsetRect(0, iYOffset << 1);

	//set up ship bitmaps
	for(i = 0; i < 2; i++)
	{
		//get the image
		switch(i)
		{
		case 0: iImage = IMG_TIMBER; break;
		case 1: iImage = IMG_SHEEP; rect.OffsetRect(iXOffset * 3, 0); break;
		}
		m_btnShip[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		m_btnShip[i].SetBitmap(VIEW->getResImage(iImage + iCardOffset, dc->GetSafeHdc()));

		//offset the rect
		rect.OffsetRect(iXOffset, 0);
	}

	//set rect
	rect = statrect;
	rect.OffsetRect(0, iYOffset * 3);

	//set up settlement bitmaps
	for(i = 0; i < 4; i++)
	{
		//get the image
		switch(i)
		{
		case 0: iImage = IMG_TIMBER; break;
		case 1: iImage = IMG_WHEAT; break;
		case 2: iImage = IMG_CLAY; rect.OffsetRect(iXOffset, 0); break;
		case 3: iImage = IMG_SHEEP; break;
		}
		m_btnSettle[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		m_btnSettle[i].SetBitmap(VIEW->getResImage(iImage + iCardOffset, dc->GetSafeHdc()));

		//offset the rect
		rect.OffsetRect(iXOffset, 0);
	}

	//set rect
	rect = statrect;
	rect.OffsetRect(0, iYOffset << 2);

	//set up city bitmaps
	for(i = 0; i < 2; i++)
	{
		//get the image
		switch(i)
		{
		case 0: iImage = IMG_WHEAT; rect.OffsetRect(iXOffset, 0); break;
		case 1: iImage = IMG_ORE; break;
		}

		m_btnCity[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this, 0);
		m_btnCity[i].SetBitmap(VIEW->getResImage(iImage + iCardOffset, dc->GetSafeHdc()));

		//create the cost static
		costrect = rect;
		if(FALSE == m_bLarge)
		{
			dx = (rect.Width() - LEGEND_COST_WIDTH) >> 1;
			dy = (rect.Height() - LEGEND_COST_HEIGHT) >> 1;
		}
		else
		{
			dx = (rect.Width() - LARGE_LEGEND_COST_WIDTH) >> 1;
			dy = (rect.Height() - LARGE_LEGEND_COST_HEIGHT) >> 1;
		}

		//deflat rect
		costrect.DeflateRect(dx, dy, dx, dy);

		//create hoverstatic
		str.Format("%d", i + 2);
		m_hCityCost[i].Create("", CHILD, costrect, this, 0);
		if(FALSE == m_bLarge)
		{
			m_hCityCost[i].SetFont(Tahoma12);
		}
		else
		{
			m_hCityCost[i].SetFont(Tahoma14);
		}
		m_hCityCost[i].SetTopBorder(0 + m_bLarge);
		m_hCityCost[i].SetLeftBorder(3 + m_bLarge);
		m_hCityCost[i].SetBackground(COLOR_BLACK);
		m_hCityCost[i].SetColors(COLOR_WHITE, COLOR_WHITE);
		m_hCityCost[i].SetText(str);

		//offset the rect
		rect.OffsetRect(iXOffset, 0);
	}

	//release dc
	ReleaseDC(dc);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set the index properly
/////////////////////////////////////////////////////////////////////////////
void CLegendButton::setIndex(int iIndex)
{
	int i;
	int iImage = 0;
	CDC *dc;
	HDC hDC;
	
	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the index
	m_iIndex = iIndex;

	//set bitmaps
	if(-1 != m_iIndex)
	{
		switch(GAME->m_players[m_iIndex].m_iColor)
		{
		case CR_WHITE:	iImage = m_bLarge ? IMG_ROADW : HISTORY_ROADW; break;
		case CR_RED:	iImage = m_bLarge ? IMG_ROADR : HISTORY_ROADR; break;
		case CR_GREEN:	iImage = m_bLarge ? IMG_ROADG : HISTORY_ROADG; break;
		case CR_BLUE:	iImage = m_bLarge ? IMG_ROADB : HISTORY_ROADB; break;
		case CR_ORANGE: iImage = m_bLarge ? IMG_ROADO : HISTORY_ROADO; break;
		case CR_BROWN:	iImage = m_bLarge ? IMG_ROADBN : HISTORY_ROADBN; break;
		}

		//get color
		m_color = COLORS->getTextColor(GAME->m_players[m_iIndex].m_iColor);
	}
	else
	{
		iImage = m_bLarge ? IMG_ROADW : HISTORY_ROADW;
		m_color = COLORS->getTextColor(CR_WHITE);
	}

	if(FALSE == m_bLarge)
	{
		m_stShip.SetBitmap(VIEW->getHistoryImage(iImage + (HISTORY_SHIPW - HISTORY_ROADW), dc->GetSafeHdc()));
		m_stRoad.SetBitmap(VIEW->getHistoryImage(iImage, dc->GetSafeHdc()));
		m_stSettle.SetBitmap(VIEW->getHistoryImage(iImage - 12, dc->GetSafeHdc()));
		m_stCity.SetBitmap(VIEW->getHistoryImage(iImage - 6, dc->GetSafeHdc()));
	}
	else
	{
		m_stShip.SetBitmap(VIEW->getResImage(iImage + 6, dc->GetSafeHdc()));
		m_stRoad.SetBitmap(VIEW->getResImage(iImage, dc->GetSafeHdc()));
		m_stSettle.SetBitmap(VIEW->getResImage(iImage - 12, dc->GetSafeHdc()));
		m_stCity.SetBitmap(VIEW->getResImage(iImage - 6, dc->GetSafeHdc()));
	}

	//set equals colors
	for(i = 0; i < 5; i++)
	{
		m_hEquals[i].SetColors(m_color, m_color);
	}

	//release context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CLegendButton::DrawItem(LPDRAWITEMSTRUCT /*lpDIS*/) 
{	
}

/////////////////////////////////////////////////////////////////////////////
// clean up bitmaps
/////////////////////////////////////////////////////////////////////////////
void CLegendButton::OnDestroy() 
{
	int i;
	CStatic *pStatic;
	HBITMAP hBitmap;

	CButton::OnDestroy();
	
	//roads
	for(i = 0; i < 2; i++)
	{
		hBitmap = m_btnRoad[i].GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//ships
	for(i = 0; i < 2; i++)
	{
		hBitmap = m_btnShip[i].GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//settlements
	for(i = 0; i < 4; i++)
	{
		hBitmap = m_btnSettle[i].GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//cities
	for(i = 0; i < 2; i++)
	{
		hBitmap = m_btnCity[i].GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//dev cards
	for(i = 0; i < 3; i++)
	{
		hBitmap = m_btnDev[i].GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}

	//other images
	for(i = 0; i < 4; i++)
	{
		pStatic = &m_stRoad + i;

		//get bitmap handle
		hBitmap = pStatic->GetBitmap();

		//destroy
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// stop flicker
/////////////////////////////////////////////////////////////////////////////
void CLegendButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
}

/////////////////////////////////////////////////////////////////////////////
// stop flicker
/////////////////////////////////////////////////////////////////////////////
BOOL CLegendButton::OnEraseBkgnd(CDC* pDC) 
{
	CRect rect;

	//get the client area
	GetClientRect(&rect);

	//erase the background
	pDC->FillSolidRect(&rect, COLOR_BLACK);

	return TRUE;
}