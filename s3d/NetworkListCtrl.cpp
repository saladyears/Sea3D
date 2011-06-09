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
#include "NetworkListCtrl.h"
#include "ColorDatabase.h"
#include "SettlersView.h"
#include "XLogfont.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkListCtrl

CNetworkListCtrl::CNetworkListCtrl()
{
}

CNetworkListCtrl::~CNetworkListCtrl()
{
	m_fontBold.DeleteObject();
}


BEGIN_MESSAGE_MAP(CNetworkListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CNetworkListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkListCtrl message handlers

/////////////////////////////////////////////////////////////////////////////
// set up some stuff
/////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::PreSubclassWindow() 
{
	XLOGFONT font;

	//create a font
	font = GetLogFont(13, "Tahoma", TRUE);
	
	//set the font to what we want
	m_fontBold.CreateFontIndirect((LOGFONT *) &font);

	//add columns
	InsertColumn(0, "Color", LVCFMT_LEFT, 40, 0);
	InsertColumn(1, "Player", LVCFMT_LEFT, 125, 1);
	InsertColumn(2, "Ping", LVCFMT_CENTER, 40, 2);
	InsertColumn(3, "Language", LVCFMT_CENTER, 70, 3);
	InsertColumn(4, "Other languages", LVCFMT_LEFT, 180, 4);
	InsertColumn(5, "", LVCFMT_LEFT, 0, 5);

	//turn on tooltips
	EnableToolTips();

	CSortListCtrl::PreSubclassWindow();
}

/////////////////////////////////////////////////////////////////////////////
// handle drawing
/////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	CDC *pDC;
	CRect rect, colorRect;
	CBrush *pBrush, brush;
	CPen *pPen, pen;
	COLORREF crColor;
	CString str;
	BOOL bReady;

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
		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT | CDRF_NOTIFYSUBITEMDRAW;
	}
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		str = GetItemText(pLVCD->nmcd.dwItemSpec, 5);

		//set the ready flag
		bReady = ("1" == str);

		//check for highlight
		if((int) pLVCD->nmcd.dwItemSpec == m_nSelection)
		{
			pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			pLVCD->clrText = COLOR_WHITE;

			if(1 == pLVCD->iSubItem)
			{
				//set the font
				SelectObject(pLVCD->nmcd.hdc, m_fontBold);
			}
			else
			{
				//set the font
				SelectObject(pLVCD->nmcd.hdc, m_font);
			}
		}
		else
		{
			pLVCD->clrTextBk = COLOR_BLACK;

			if(1 == pLVCD->iSubItem)
			{
				pLVCD->clrText = (bReady ? COLOR_OUTLINEGREEN : COLOR_RED);
				//set the font
				SelectObject(pLVCD->nmcd.hdc, m_fontBold);
			}
			else
			{
				//set the font
				SelectObject(pLVCD->nmcd.hdc, m_font);
				pLVCD->clrText = COLOR_WHITE;
			}
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
		pen.CreatePen(PS_SOLID, 1, COLOR_WHITE);
		brush.CreateSolidBrush(crColor);

		//select them in
		pPen = pDC->SelectObject(&pen);
		pBrush = pDC->SelectObject(&brush);

		//underdraw
		pDC->FillSolidRect(&rect, COLOR_BLACK);

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
// add a new player to the list
/////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::addPlayer(CPlayer *pPlayer, int iColor)
{
	int iItem;
	int iLang, iKnown;
	CString str;

	//get the current count
	iItem = GetItemCount();

	//set color
	str.Format("%d", iColor);
	InsertItem(iItem, str);
	SetItemData(iItem, (DWORD) pPlayer->getID());

	//set player name
	SetItemText(iItem, 1, pPlayer->getName());

	//ping determined by server

	//set primary language
	iLang = pPlayer->getLanguage();
	iKnown = pPlayer->getKnown();
	SetItemText(iItem, 3, VIEW->getLangName(iLang));

	//set known languages (minus primary)
	SetItemText(iItem, 4, VIEW->getLangString(iKnown & ~iLang));

	//default item text for ready state
	SetItemText(iItem, 5, (pPlayer->getReady() ? "1" : "0"));

	//reset selection
	m_nSelection = -1;

	sort();
}

/////////////////////////////////////////////////////////////////////////////
// remove a player by ID
////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::removePlayer(UINT uiID)
{
	int iPlayer;

	//find player
	iPlayer = findPlayer(uiID);

	if(-1 != iPlayer)
	{
		//remove
		DeleteItem(iPlayer);
	}

	//reset selection
	m_nSelection = -1;
}

/////////////////////////////////////////////////////////////////////////////
// see if we need a tooltip
////////////////////////////////////////////////////////////////////////////
int CNetworkListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int i, nSel;
	CString strText;
	UINT uiID;
	LVHITTESTINFO lvInfo;
	static HWND hWndTool = NULL;
	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
	CToolTipCtrl* pToolTip = pThreadState->m_pToolTip;
	
	//get current mouse position
	GetCursorPos(&point);
	ScreenToClient(&point);

	//set hit item stuff
	lvInfo.pt = point;

	//get the current row
	((CListCtrl &) *this).SubItemHitTest(&lvInfo);

	//get sel
	nSel = lvInfo.iItem;

	//see if valid
	if(-1 != nSel)
	{
		pTI->hwnd     = GetParent()->m_hWnd;
		pTI->uId      = (UINT)m_hWnd;
		pTI->uFlags  |= TTF_IDISHWND;

		//get the player's ID
		uiID = GetItemData(nSel);

		//find this player in the list
		for(i = 0; i < (int) GAME->m_players.size(); i++)
		{
			if(GAME->m_players[i].m_player.getID() == uiID)
			{
				strText = GAME->m_players[i].m_player.getReport();
				break;
			}
		}

		pTI->lpszText = (char *)malloc(strText.GetLength()+1);
		lstrcpy(pTI->lpszText, strText);
	}
	
	//set up the delays
	if((NULL != pToolTip) && (pToolTip->GetSafeHwnd() != NULL) && (hWndTool != pToolTip->GetSafeHwnd()))
	{
		hWndTool = pToolTip->GetSafeHwnd();
		pToolTip->SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, (LPARAM) MAKELONG(150, 0));
		pToolTip->SendMessage(TTM_SETDELAYTIME, TTDT_AUTOPOP,	MAKELONG(10000, 0));
	}

	//we found one
	return nSel;
}

////////////////////////////////////////////////////////////////////////////
// update a player's color
////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::updatePlayerColor(UINT uiID, int iColor)
{
	int iPlayer;
	CString str;

	//format the new color string
	str.Format("%d", iColor);

	//find player
	iPlayer = findPlayer(uiID);

	if(-1 != iPlayer)
	{
		SetItemText(iPlayer, 0, str);
	}
}

////////////////////////////////////////////////////////////////////////////
// update a player's ping information
////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::updatePing(UINT uiID, long lPing)
{
	int iPlayer;
	CString str;

	//find the player
	iPlayer = findPlayer(uiID);

	//update ping
	str.Format("%ld", lPing);

	if(-1 != iPlayer)
	{
		SetItemText(iPlayer, 2, str);
	}
}

////////////////////////////////////////////////////////////////////////////
// find a player in the list by their ID
////////////////////////////////////////////////////////////////////////////
int CNetworkListCtrl::findPlayer(UINT uiID)
{
	int i;

	//search items for this ID
	for(i = 0; i < GetItemCount(); i++)
	{
		if(GetItemData(i) == uiID)
		{
			return i;
		}
	}

	//not found
	return -1;
}

////////////////////////////////////////////////////////////////////////////
// update a player's name ready state
////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::updateReady(UINT uiID, BOOL bReady)
{
	int nPlayer;

	//find the player
	nPlayer = findPlayer(uiID);

	//ensure validity
	if(-1 == nPlayer)
	{
		return;
	}

	//set the item text for the index
	SetItemText(nPlayer, 5, bReady ? "1" : "0");

	//redraw
	Invalidate();
}

////////////////////////////////////////////////////////////////////////////
// update a player's name when rejoining a continuing network game
////////////////////////////////////////////////////////////////////////////
void CNetworkListCtrl::updateJoin(UINT uiID, BOOL bIn)
{
	int iIndex;
	int iPlayer;
	CString str;

	//get player
	iPlayer = findPlayer(uiID);

	//ensure validity
	if(-1 == iPlayer)
	{
		return;
	}

	//get item text
	str = GetItemText(iPlayer, 1);

	//search for the joined phrase
	iIndex = str.Find("joined");

	//strip it out
	if(-1 != iIndex)
	{
		//otherwise, strip it off for someone leaving
		str = str.Left(iIndex - 2);
	}

	//if they've just joined, add it on
	if(TRUE == bIn)
	{
		str += " (joined)";
	}

	//set the text
	SetItemText(iPlayer, 1, str);
}
