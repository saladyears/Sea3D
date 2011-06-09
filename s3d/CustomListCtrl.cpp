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
#include "CustomListCtrl.h"
#include "XLogFont.h"
#include "ColorDatabase.h"
#include "SettlersView.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl

CCustomListCtrl::CCustomListCtrl()
{
}

CCustomListCtrl::~CCustomListCtrl()
{
}


BEGIN_MESSAGE_MAP(CCustomListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CCustomListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// startup information
/////////////////////////////////////////////////////////////////////////////
void CCustomListCtrl::PreSubclassWindow() 
{
	//set the column information
	InsertColumn(0, "Color", LVCFMT_CENTER, 40);
	InsertColumn(1, "Player", LVCFMT_LEFT, 150, 2);
	
	//normal handling
	CSortListCtrl::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
// add a player to the list
///////////////////////////////////////////////////////////////////////////////
void CCustomListCtrl::addPlayer(CPlayer *player, int iColor)
{
	int nItems;
	CString str;

	//get the current number of items
	nItems = GetItemCount();

	//add the item
	str.Format("%d", iColor);
	InsertItem(nItems, str);

	//set item data to player ID
	SetItemData(nItems, (DWORD) player->getID());

	//now add sub information
	SetItemText(nItems, 1, player->getName());
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CCustomListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	BOOL bSelected;
	CDC *pDC;
	CRect rect, colorRect;
	CBrush *pBrush, brush;
	CPen *pPen, pen;
	COLORREF crColor;
	CString str;

	// Take the default processing unless we set this to something else below.
    *pResult = CDRF_DODEFAULT;

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		GetClientRect(&rect);

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
		//clear background
		pDC->FillSolidRect(&rect, COLOR_BLACK);

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
    else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		//set the font
		SelectObject(pLVCD->nmcd.hdc, m_font);

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT | CDRF_NOTIFYSUBITEMDRAW;
	}
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		//determine selection
		bSelected = ((int) pLVCD->nmcd.dwItemSpec == m_nSelection);

		//check for highlight
		if(TRUE == bSelected)
		{
			pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			pLVCD->clrText = COLOR_WHITE;
		}
		else
		{
			pLVCD->clrTextBk = COLOR_BLACK;
			pLVCD->clrText = COLOR_WHITE;
		}

		//draw normal
		if(0 != pLVCD->iSubItem)
		{
			*pResult = CDRF_DODEFAULT;
			return;
		}
		
		pDC = CDC::FromHandle (pLVCD->nmcd.hdc);

		//get item rect
		GetSubItemRect(pLVCD->nmcd.dwItemSpec, pLVCD->iSubItem, LVIR_LABEL, rect);

		//get correct color information here
		str = GetItemText(pLVCD->nmcd.dwItemSpec, 0);
		crColor = COLORS->getTextColor(atoi(str));

		//create the pen and brush
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		brush.CreateSolidBrush(crColor);

		//select them in
		pPen = pDC->SelectObject(&pen);
		pBrush = pDC->SelectObject(&brush);

		//underdraw
		if(TRUE == bSelected)
		{
			pDC->FillSolidRect(&rect, ::GetSysColor(COLOR_HIGHLIGHT));
		}
		else
		{
			pDC->FillSolidRect(&rect, COLOR_BLACK);
		}

		//set max rect size
		if(rect.Width() > 38)
		{
			rect.right = rect.left + 38;
		}
		else if(rect.Width() < 0)
		{
			rect.right = rect.left;
		}

		//create the color rect
		colorRect = rect;
		colorRect.DeflateRect(4, 2, 7, 2);
		pDC->Rectangle(colorRect);

		//select back old
		pDC->SelectObject(pPen);
		pDC->SelectObject(pBrush);

		*pResult = CDRF_SKIPDEFAULT;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the index of the selected player
/////////////////////////////////////////////////////////////////////////////
int CCustomListCtrl::getSelection()
{
	int i;
	UINT uiID;

	//sanity
	if(-1 == m_nSelection)
	{
		return -1;
	}

	//get the selected player's id
	uiID = GetItemData(m_nSelection);

	//search through the players
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		if(GAME->m_players[i].m_player.getID() == uiID)
		{
			return i;
		}
	}

	//did not find player!!
	return -1;
}