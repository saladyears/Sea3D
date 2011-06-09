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
#include "Game.h"
#include "PlayerListCtrl.h"
#include "PlayerDatabase.h"
#include "GameDatabase.h"
#include "defineColors.h"
#include "defineDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerListCtrl

CPlayerListCtrl::CPlayerListCtrl()
{
	m_bStats = TRUE;
	m_bForeign = TRUE;
}

CPlayerListCtrl::~CPlayerListCtrl()
{
}


BEGIN_MESSAGE_MAP(CPlayerListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CPlayerListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerListCtrl message handlers


/////////////////////////////////////////////////////////////////////////////
// initialize things
/////////////////////////////////////////////////////////////////////////////
void CPlayerListCtrl::init(BOOL bStats, BOOL bForeign, BOOL bLadder)
{
	int nCol = 0;

	//set the flag
	m_bStats = bStats;
	m_bForeign = bForeign;
	m_bLadder = bLadder;

	//set up the player list
	if(TRUE == bLadder)
	{
		InsertColumn(nCol++, "", LVCFMT_LEFT, 15);
	}
	InsertColumn(nCol++, "Players", LVCFMT_LEFT, 100, 1);
	
	if(bStats)
	{
		InsertColumn(nCol++, "Games", LVCFMT_CENTER, 55, 1);
		InsertColumn(nCol++, "Wins", LVCFMT_CENTER, 55, 2);
		InsertColumn(nCol++, "Average", LVCFMT_CENTER, 65, 3);
	}

	//set the full row selection
	SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//load the players
	loadPlayers();
}

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CPlayerListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nItem;
	int iPlayer;
	int nX, nY;
	CDC* pDC = NULL;
	CRect rect;
	CImageList *pList;
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if(CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		GetClientRect(&rect);

		//get device context
		pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		
		//clear background
		pDC->FillSolidRect(&rect, COLOR_BLACK);

		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if(CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		//set the font
		SelectObject(pLVCD->nmcd.hdc, m_font);

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT | CDRF_NOTIFYSUBITEMDRAW;
	}
	else if((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		//get item
		nItem = (int) pLVCD->nmcd.dwItemSpec;
		
		//get player
		iPlayer = GetItemData(nItem);

		//if it's the first sub item, draw needed bitmaps
		if(0 == pLVCD->iSubItem && TRUE == m_bLadder)
		{
			//get our rectangle
			GetSubItemRect(nItem, 0, LVIR_BOUNDS, rect);

			//set the offsets
			nX = rect.left + 1;
			nY = rect.top + 1;

			//get the image list
			pList = APP->getImageConnector();

			//get device context
			pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

			//connector bitmap
			if(TRUE == PLAYERS->getPlayer(iPlayer).getLadder())
			{
				pList->DrawIndirect(pDC, CONNECT_LADDER, CPoint(nX + 1, nY), CSize(11, 11), CPoint(0, 0), ILD_NORMAL, SRCCOPY, CLR_NONE);
			}

			//done
			*pResult = CDRF_SKIPDEFAULT;
		}
		else
		{
			//check for highlight
			if(nItem == m_nSelection)
			{
				pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			}
			else
			{
				pLVCD->clrTextBk = COLOR_BLACK;
			}

			//set color
			if(TRUE == PLAYERS->getPlayer(iPlayer).getMadeHere())
			{
				pLVCD->clrText = COLOR_WHITE;
			}
			else
			{
				pLVCD->clrText = COLOR_GRAY;
			}

			//draw normal
			*pResult = CDRF_DODEFAULT;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// load the player database
/////////////////////////////////////////////////////////////////////////////
void CPlayerListCtrl::loadPlayers()
{
	int i, j, iSize;
	CPlayer player;
	
	//reset all content
	DeleteAllItems();

	//get the number of players
	iSize = PLAYERS->numPlayers();

	//populate the list
	for(i = 0, j = 0; i < iSize; i++)
	{
		//get the player
		player = PLAYERS->getPlayer(i);

		//see if we need to insert this item
		if(FALSE == player.getMadeHere() && FALSE == m_bForeign)
		{
			continue;
		}

		if(TRUE == m_bLadder)
		{
			InsertItem(j, player.getLadder() ? "1" : "0");
			SetItemText(j, 1, player.getCommaName());
		}
		else
		{
			InsertItem(j, player.getCommaName());
		}
		SetItemData(j, (LPARAM) i);

		//populate the other columns
		if(TRUE == m_bStats)
		{
			populateRow(i, j);		
		}

		j++;
	}

	//reset selection
	m_nSelection = -1;

	//call the sort function
	sort();
}

/////////////////////////////////////////////////////////////////////////////
// calculate some stats
/////////////////////////////////////////////////////////////////////////////
void CPlayerListCtrl::populateRow(int iPlayer, int iRow)
{
	int nCol = 1;
	int i, j, iSize;
	int iGames = 0, iWins = 0;
	float dGame;
	float dPoints = 0.0;
	float dHigh;
	CString str;
	CGame *pGame;
	CPlayer player;

	//create a new game file
	pGame = new CGame;

	//get the player in question
	player = PLAYERS->getPlayer(iPlayer);

	//get the number of games
	iSize = DATABASE->numGames();

	//run through each game
	for(i = 0; i < iSize; i++)
	{
		//reset high point total
		dHigh = 0.;

		//get the game
		DATABASE->getGame(i, pGame);

		//find the highest point total in this game
		for(j = 0; j < (int) pGame->m_players.size(); j++)
		{
			if(dHigh < (float) pGame->m_players[j].m_iPoints)
			{
				dHigh = (float) pGame->m_players[j].m_iPoints;
			}
		}

		//search for this player
		for(j = 0; j < (int) pGame->m_players.size(); j++)
		{
			if(pGame->m_players[j].m_player == player)
			{
				//add the games
				iGames++;

				//see if they won
				if(pGame->m_iWinner == j)
				{
					iWins++;
				}

				//get points for this player
				dGame = (float) pGame->m_players[j].m_iPoints;

				//total points
				dPoints += float((10. / dHigh) * dGame);

				//break from the loop
				break;
			}
		}
	}

	//set appropriate column
	if(TRUE == m_bLadder)
	{
		nCol = 2;
	}

	//set the columns
	str.Format("%d", iGames);
	SetItemText(iRow, nCol++, str);
	str.Format("%d", iWins);
	SetItemText(iRow, nCol++, str);
	str.Format("%.2f", iGames ? dPoints / (float) iGames : 0.0);
	SetItemText(iRow, nCol++, str);

	//delete the used game file
	delete pGame;
	pGame = NULL;
}
