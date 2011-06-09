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
#include "BankButton.h"
#include "SettlersView.h"
#include "MapDatabase.h"
#include "memdc.h"
#include "defineDraw.h"
#include "defineIni.h"
#include "defineColors.h"
#include ".\bankbutton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBankButton

CBankButton::CBankButton()
{
	int i;

	//no tooltips yet
	for(i = 0; i < 6; i++)
	{
		m_ToolTip[i].m_hWnd = NULL;
	}
}

CBankButton::~CBankButton()
{
}


BEGIN_MESSAGE_MAP(CBankButton, CAnimateButton)
	//{{AFX_MSG_MAP(CBankButton)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBankButton message handlers

/////////////////////////////////////////////////////////////////////////////
// set to owner draw
/////////////////////////////////////////////////////////////////////////////
BOOL CBankButton::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style |= BS_OWNERDRAW;
	
	return CAnimateButton::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// set up buttons
/////////////////////////////////////////////////////////////////////////////
int CBankButton::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	int i;
	int iImage;
	CDC *dc;
	HDC hDC;
	CRect rect;

	if (CAnimateButton::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	//get large fonts value
	m_bLarge = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, 0, INI_FILE);

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set up the rectangle
	rect = CRect(4, PANEL_OFFSET + 1, 30, PANEL_OFFSET + 1 + STATIC_HEIGHT);
	
	//create the title
	m_stTitle.Create("", CHILD, rect, this, 0);
	m_stTitle.SetFont(Tahoma12);
	m_stTitle.SetTopBorder(0);
	m_stTitle.SetColors(COLOR_WHITE, COLOR_WHITE);
	m_stTitle.SetBackground(COLOR_BLACK);
	m_stTitle.SetText("Bank");

	//set starting rect
	if(FALSE == m_bLarge)
	{
		rect = CRect(4, PANEL_OFFSET + STATIC_HEIGHT + 10, 4 + BANK_RES_WIDTH, PANEL_OFFSET + STATIC_HEIGHT + 10 + BANK_RES_HEIGHT);
	}
	else
	{
		rect = CRect(4, PANEL_OFFSET + STATIC_HEIGHT + 10, 4 + LARGE_BANK_RES_WIDTH, PANEL_OFFSET + STATIC_HEIGHT + 10 + LARGE_BANK_RES_HEIGHT);
	}

	//set up bitmaps
	for(i = 0; i < 6; i++)
	{
		//get image
		iImage = ((i + 1) * DECK_LEVELS) - 1;

		m_btnBank[i].Create("", CHILD | SS_BITMAP | SS_CENTERIMAGE, rect, this);
		if(FALSE == m_bLarge)
		{
			m_btnBank[i].SetBitmap(VIEW->getDeckImage(iImage, dc->GetSafeHdc()));
		}
		else
		{
			m_btnBank[i].SetBitmap(VIEW->getDeckImage(DECK_SIZE + iImage, dc->GetSafeHdc()));
		}

		//offset and continue
		if(FALSE == m_bLarge)
		{
			rect.OffsetRect(BANK_RES_WIDTH + 3, 0);
		}
		else
		{
			rect.OffsetRect(LARGE_BANK_RES_WIDTH + 4, 0);
		}
	}

	//determine offset
	if(TRUE == m_bLarge)
	{
		m_nOffsetAmount = LARGE_BANK_BUTTON_WIDTH - 60;
	}
	else
	{
		m_nOffsetAmount = BANK_BUTTON_WIDTH - 40;
	}

	//handle auto-hiding
	updateAutohide();

	//release the dc
	ReleaseDC(dc);
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// update the resource counts
/////////////////////////////////////////////////////////////////////////////
void CBankButton::updateCounts()
{
	int i;
	int iImage;
	int iBank;
	int iTotal;
	double dPercent;
	BOOL bShow;
	HBITMAP hBitmap;
	CDC *dc;
	HDC hDC;
	CRect rect;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//initialize tooltips
	initToolTips();

	//run through list
	for(i = 0; i < 6; i++)
	{
		if(i < 5)
		{
			//get bank total from map database
			iTotal = MAP->m_iBankRes;

			//get current bank res
			iBank = GAME->getBankRes(i);
		}
		else
		{
			//get dev card total
			iTotal = MAP->m_iDevTotal;

			//get current amount
			iBank = iTotal - VIEW->getDevCardIndex();
		}

		//get percent
		dPercent = (double) iBank / (double) iTotal;

		//get base image
		iImage = (int) (dPercent / 0.20);

		//set to correct image
		iImage += (i * DECK_LEVELS);

		//set picture
		if(FALSE == m_bLarge)
		{
			hBitmap = m_btnBank[i].SetBitmap(VIEW->getDeckImage(iImage, dc->GetSafeHdc()));
		}
		else
		{
			hBitmap = m_btnBank[i].SetBitmap(VIEW->getDeckImage(DECK_SIZE + iImage, dc->GetSafeHdc()));
		}

		//delete old
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}

		//get current show value
		bShow = m_btnBank[i].IsWindowVisible();

		//show hide for percentage 0.
		if(0.0 == dPercent)
		{
			m_btnBank[i].ShowWindow(SW_HIDE);
		}
		else
		{
			m_btnBank[i].ShowWindow((bShow || (0 == m_nOffset)) ? SW_SHOW : SW_HIDE);
		}

		//create tooltips
		m_btnBank[i].GetWindowRect(rect);
		ScreenToClient(&rect);

		if(i < 5)
		{
			m_ToolTip[i].AddTool(this, VIEW->getResName(i), rect, 1);
		}
		else
		{
			m_ToolTip[i].AddTool(this, "Dev Cards", rect, 1);
		}
	}

	//release the dc
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle tooltips
/////////////////////////////////////////////////////////////////////////////
BOOL CBankButton::PreTranslateMessage(MSG* pMsg) 
{
	int i;

	//relay the message
	for(i = 0; i < 6; i++)
	{
		if(NULL != m_ToolTip[i].m_hWnd)
		{
			m_ToolTip[i].RelayEvent(pMsg);
		}
	}
	
	return CAnimateButton::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// initialize tool tip stuff
/////////////////////////////////////////////////////////////////////////////
void CBankButton::initToolTips()
{
	int i;

	//run through the list
	for(i = 0; i < 6; i++)
	{
		//create the tool tip for the dev card window
		if(NULL == m_ToolTip[i].m_hWnd)
		{
			// Create ToolTip control
			m_ToolTip[i].Create(&(m_btnBank[i]));
			
			// Create inactive
			m_ToolTip[i].Activate(TRUE);

			//set delay to minimal
			m_ToolTip[i].SetDelayTime(150);
		} // if
	}
}

/////////////////////////////////////////////////////////////////////////////
// stop flicker
/////////////////////////////////////////////////////////////////////////////
void CBankButton::OnLButtonDown(UINT /*nFlags*/, CPoint /*point*/) 
{
}



/////////////////////////////////////////////////////////////////////////////
// handle autohide
/////////////////////////////////////////////////////////////////////////////
void CBankButton::updateAutohide(BOOL bFirst)
{
	//base class
	CAnimateButton::updateAutohide();

	//read all the auto-hide values
	m_bBank = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_BANK, FALSE, INI_FILE);
	m_bAnimate = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_ANIMATE, FALSE, INI_FILE);

	//handle the first time through
	if(TRUE == m_bAnimate && TRUE == bFirst)
	{
		showItems(FALSE);
		RedrawWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle auto-hiding in various modes
/////////////////////////////////////////////////////////////////////////////
void CBankButton::handleAutohide()
{
	BOOL bHide = TRUE;

	//make sure we even can hide
	if(FALSE == m_bAutohide)
	{
		bHide = FALSE;
	}

	//see if we SHOULD not autohide
	if(TRUE == m_bBank)
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
void CBankButton::moveOffset(int nOffset)
{
	int iHeight;
	int iWidth;
	CRect rect;
	CRgn region;
	CRgn nopaint;
	CRgn combine;

	if(TRUE == m_bLarge)
	{
		iHeight = LARGE_BANK_BUTTON_HEIGHT;
		iWidth = LARGE_BANK_BUTTON_WIDTH - nOffset;
	}
	else
	{
		iHeight = BANK_BUTTON_HEIGHT;
		iWidth = BANK_BUTTON_WIDTH - nOffset;
	}

	//resize
	SetWindowPos(NULL, 0, 0, iWidth, iHeight, SWP_NOZORDER | SWP_NOMOVE);

	//set redraw region
	GetClientRect(&rect);
	region.CreateRectRgnIndirect(&rect);

	//now get the text area
	m_stTitle.GetWindowRect(&rect);
	ScreenToClient(&rect);

	//subtract this region from the update region
	nopaint.CreateRectRgnIndirect(&rect);
	combine.CreateRectRgn(0, 0, 0, 0);
	combine.CombineRgn(&region, &nopaint, RGN_DIFF); 

	//redraw
	InvalidateRgn(&combine);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the goods
/////////////////////////////////////////////////////////////////////////////
void CBankButton::showItems(BOOL bShow)
{
	int i;

	//hide redraws
	LockWindowUpdate();

	//show the buttons
	for(i = 0; i < 6; i++)
	{
		if(i < 5)
		{
			m_btnBank[i].ShowWindow(bShow && (0 < GAME->getBankRes(i)) ? SW_SHOW : SW_HIDE);
		}
		else
		{
			m_btnBank[i].ShowWindow(bShow && (VIEW->getDevCardIndex() < MAP->m_iDevTotal) ? SW_SHOW : SW_HIDE);
		}
	}

	//hide redraws
	UnlockWindowUpdate();
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
BOOL CBankButton::DestroyWindow() 
{
	int i;
	HBITMAP hBitmap;

	//clean up bitmaps
	for(i = 0; i < 6; i++)
	{
		hBitmap = m_btnBank[i].GetBitmap();

		//delete old
		if(NULL != hBitmap)
		{
			DeleteObject(hBitmap);
		}

		//destroy
		m_btnBank[i].DestroyWindow();
		m_ToolTip[i].DestroyWindow();
	}

	//title
	m_stTitle.DestroyWindow();
	
	return CAnimateButton::DestroyWindow();
}

/////////////////////////////////////////////////////////////////////////////
// paint
/////////////////////////////////////////////////////////////////////////////
void CBankButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CPen pen, *pPen;
	CRect rect;
	CBrush brush, *pBrush;

	//get the rect
	GetClientRect(&rect);

	//set the background mode to transparent
	dc.SetBkMode(TRANSPARENT);

	//create the color bar
	pen.CreatePen(PS_SOLID, 1, COLOR_WHITE);
	brush.CreateSolidBrush(COLOR_BLACK);

	//select the old pen
	pPen = dc.SelectObject(&pen);
	pBrush = dc.SelectObject(&brush);

	//draw the rectangle
	dc.Rectangle(rect);

	//restore the old
	dc.SelectObject(pPen);
	dc.SelectObject(pBrush);

	//delete the objects
	pen.DeleteObject();
	brush.DeleteObject();
}
