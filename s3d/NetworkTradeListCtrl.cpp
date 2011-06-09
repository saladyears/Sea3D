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
#include "NetworkTradeListCtrl.h"
#include "XLogFont.h"
#include "ColorDatabase.h"
#include "SettlersView.h"
#include "NetworkTradeDialog.h"
#include "defineColors.h"
#include "defineNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeListCtrl

CNetworkTradeListCtrl::CNetworkTradeListCtrl()
{
	//no status yet
	::ZeroMemory(m_shStatus, sizeof(m_shStatus));
}

CNetworkTradeListCtrl::~CNetworkTradeListCtrl()
{
	//delete font
	m_fontBold.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNetworkTradeListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CNetworkTradeListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnclick)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// startup information
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::PreSubclassWindow() 
{
	XLOGFONT font;

	//set the column information
	InsertColumn(0, "Color", LVCFMT_CENTER, 0);
	InsertColumn(1, "Trade", LVCFMT_CENTER, 42, 1);
	InsertColumn(2, "Player", LVCFMT_LEFT, 150, 2);
	InsertColumn(3, "Status", LVCFMT_LEFT, 150, 3);

	//create a font
	font = GetLogFont(13, "Tahoma", TRUE);
	
	//set the font to what we want
	m_fontBold.CreateFontIndirect((LOGFONT *) &font);
	
	//normal handling
	CSortListCtrl::PreSubclassWindow();
}

///////////////////////////////////////////////////////////////////////////////
// add a player to the list
///////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::addPlayer(CPlayer *player, int iColor)
{
	int nItems;
	CString str;

	//get the current number of items
	nItems = GetItemCount();

	//add the item
	str.Format("%d", iColor);
	InsertItem(nItems, str);

	//set check
	m_bChecks[nItems] = TRUE;

	//set item data to player ID
	SetItemData(nItems, (DWORD) player->getID());

	//now add sub information
	SetItemText(nItems, 2, player->getName());

	//status of no offer yet
	updateStatus(nItems, TRADE_NO_OFFER);
}

///////////////////////////////////////////////////////////////////////////////
// overridden drawing for the control
///////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nItem, nSubItem;
	CDC* pDC = NULL;
	CRect rect;
	CString str;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	//tell windows we want notifications for every item
	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		GetClientRect(&rect);

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
		//clear background
		pDC->FillSolidRect(&rect, COLOR_BLACK);

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	//tell windows we want notifications for every subitem
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	//actually draw subitem stuff
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		//get item and subitem
		nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		nSubItem = pLVCD->iSubItem;

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
		//get subitem rect
		GetSubItemRect(nItem, nSubItem, LVIR_LABEL, rect);

		//check for highlight
		pLVCD->clrTextBk = COLOR_BLACK;

		//set default background and text color
		pLVCD->clrText = COLOR_WHITE;

		//if it's the first column (the check box column) draw check box
		if(1 == nSubItem)
		{
			DrawCheckbox(nItem, pDC, rect);

			//we've drawn everything we need for this subitem
			*pResult = CDRF_SKIPDEFAULT;
		}
		//otherwise, it's only a matter of fonts
		else
		{
			//set different colors for status messages
			if(3 == nSubItem)
			{
				pLVCD->clrText = getStatusColor(nItem);

				//select the bold font
				pDC->SelectObject(&m_fontBold);
			}
			else
			{
				//get the color
				str = GetItemText(nItem, 0);

				//set color
				pLVCD->clrText = COLORS->getTextColor(atoi(str));

				//set non-bold font
				pDC->SelectObject(&m_font);
			}

			//tell windows we have a new font
			*pResult = CDRF_NEWFONT;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// set status text
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::updateStatus(int nItem, short shStatus)
{
	CString str;

	//set the status
	m_shStatus[nItem] = shStatus;

	//set the item text
	switch(shStatus)
	{
	case TRADE_NO_OFFER: str = "No offer"; break;
	case TRADE_OFFER: str = "Waiting..."; break;
	case TRADE_REJECT: str = "Rejected"; break;
	case TRADE_ACCEPT: str = "Accepted"; break;
	case TRADE_COUNTER: str = "Waiting for counteroffer..."; break;
	case TRADE_COFFER: str = "Made counteroffer"; break;
	}

	//set the item text
	SetItemText(nItem, 3, str);
}

/////////////////////////////////////////////////////////////////////////////
// reset statii
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::resetStatus(void) 
{
	int i;

	for(i = 0; i < GetItemCount(); i++)
	{
		updateStatus(i, TRADE_NO_OFFER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// returns a colorref for a status message
/////////////////////////////////////////////////////////////////////////////
COLORREF CNetworkTradeListCtrl::getStatusColor(int nItem)
{
	COLORREF cr = RGB(0, 0, 0);

	//get color
	switch(m_shStatus[nItem])
	{
	case TRADE_NO_OFFER:	cr = COLOR_WHITE; break;
	case TRADE_OFFER:		cr = COLOR_GRAY; break;
	case TRADE_REJECT:		cr = COLOR_RED; break;
	case TRADE_ACCEPT:		cr = COLOR_OUTLINEGREEN; break;
	case TRADE_COUNTER:		cr = COLOR_PURPLE; break;
	case TRADE_COFFER:		cr = COLOR_YELLOW; break;
	}

	//return the color
	return cr;
}

/////////////////////////////////////////////////////////////////////////////
// draws a checkbox for a row
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::DrawCheckbox(int nItem, CDC *pDC, CRect& rect)
{
	int i, x, y;
	CRect rectCheck;
	CPen *pOldPen = NULL;
	CPen pen;

	//start off the same
	rectCheck = rect;

	//check for highlight
	pDC->FillSolidRect(&rect, COLOR_BLACK);

	//center the checkbox
	rectCheck.left = rect.left + rect.Width() / 2 - rectCheck.Height()/ 2 - 1;
	rectCheck.right = rectCheck.left + rectCheck.Height() + 1;

	//add one to the left side
	rectCheck.left += 1;

	//fill rect around checkbox with white
	pDC->FillSolidRect(&rectCheck, COLOR_WHITE);

	//draw border
	pDC->DrawEdge(&rectCheck, EDGE_SUNKEN, BF_RECT);

	//set checked state
	if(TRUE == m_bChecks[nItem])
	{
		//create a black pen
		pen.CreatePen(PS_SOLID, 1, COLOR_BLACK);

		//select it into the device context
		pOldPen = pDC->SelectObject(&pen);

		// draw the checkmark
		x = rectCheck.left + 9;
		y = rectCheck.top + 3;
		
		//sanity
		ASSERT(x < rectCheck.right);
		
		//draw the lines
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y + 3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y + 3);
			x--;
			y--;
		}

		//select the old pen back
		if(NULL != pOldPen)
		{
			pDC->SelectObject(pOldPen);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a column click
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMView = (NM_LISTVIEW*)pNMHDR;
	
	//if it's sub item zero, we need to set toggle all check marks
	if(0 == pNMView->iSubItem)
	{
		//toggle checks here
		
		*pResult = 0;
	}
	//otherwise, submit to base class
	{
		CSortListCtrl::OnColumnclick(pNMHDR, pResult);
	}
}

/////////////////////////////////////////////////////////////////////////////
// try selection
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::OnClick(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/)
{
	int iSelection;
	CPoint point;
	LVHITTESTINFO lvHit;

	//get current mouse position
	GetCursorPos(&point);
	ScreenToClient(&point);

	//set it
	lvHit.pt = point;

	//perform hittest
	SubItemHitTest(&lvHit);

	//set the selection
	iSelection = lvHit.iItem;

	//if it's valid, toggle the check mark
	if(-1 != iSelection)
	{
		m_bChecks[iSelection] = !m_bChecks[iSelection];
	}

	//recheck for trading
	((CNetworkTradeDialog *) GetParent())->checkTrade();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handle a reply to a trade
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeListCtrl::handleReply(int iPlayer, short shStatus)
{
	int i;
	UINT uiID;

	//get the player's ID
	uiID = GAME->m_players[iPlayer].m_player.getID();

	//search for it
	for(i = 0; i < GetItemCount(); i++)
	{
		if(uiID == (UINT) GetItemData(i))
		{
			//update status
			updateStatus(i, shStatus);
			return;
		}
	}

	//did not find anyone!
}

/////////////////////////////////////////////////////////////////////////////
// find someone's status
/////////////////////////////////////////////////////////////////////////////
short CNetworkTradeListCtrl::getStatus(int iPlayer)
{
int i;
	UINT uiID;

	//get the player's ID
	uiID = GAME->m_players[iPlayer].m_player.getID();

	//search for it
	for(i = 0; i < GetItemCount(); i++)
	{
		if(uiID == (UINT) GetItemData(i))
		{
			//update status
			return m_shStatus[i];
		}
	}

	//did not find anyone!
	return TRADE_NO_OFFER;
}

/////////////////////////////////////////////////////////////////////////////
// determine if we're done waiting for a trade
/////////////////////////////////////////////////////////////////////////////
int CNetworkTradeListCtrl::isComplete()
{
	int i;
	BOOL bReject = TRUE;

	//see if there are any waiting statii
	for(i = 0; i < 5; i++)
	{
		if(TRADE_OFFER == m_shStatus[i])
		{
			//not done yet
			return TRADE_OFFER;
		}
		else if(TRADE_REJECT != m_shStatus[i] && TRADE_NO_OFFER != m_shStatus[i])
		{
			bReject = FALSE;
		}
	}

	//we're done
	return bReject ? TRADE_REJECT : TRADE_ACCEPT;
}

/////////////////////////////////////////////////////////////////////////////
// check to make sure a check mark has been checked
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkTradeListCtrl::isValid()
{
	int i;

	//look at checks
	for(i = 0; i < 5; i++)
	{
		if(TRUE == m_bChecks[i])
		{
			return TRUE;
		}
	}

	//no checks
	return FALSE;
}