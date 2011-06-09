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
#include "Game.h"
#include "GameListCtrl.h"
#include "GameDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameListCtrl

CGameListCtrl::CGameListCtrl()
{
	//default to normal games
	m_bIncomplete = FALSE;
}

CGameListCtrl::~CGameListCtrl()
{
}


BEGIN_MESSAGE_MAP(CGameListCtrl, CSortListCtrl)
	//{{AFX_MSG_MAP(CGameListCtrl)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomdraw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGameListCtrl message handlers

void CGameListCtrl::PreSubclassWindow() 
{
	CSortListCtrl::PreSubclassWindow();

	InsertColumn(0, "Date", LVCFMT_LEFT, 80);
	InsertColumn(1, "Time", LVCFMT_CENTER, 80, 1);
	InsertColumn(2, "Game type", LVCFMT_LEFT, 80, 2);
	InsertColumn(3, "# Players", LVCFMT_CENTER, 75, 3);
	InsertColumn(4, "Length", LVCFMT_CENTER, 80, 4);
	InsertColumn(5, "Player 1", LVCFMT_LEFT, 100, 5);
	InsertColumn(6, "Player 2", LVCFMT_LEFT, 100, 6);
	InsertColumn(7, "Player 3", LVCFMT_LEFT, 100, 7);
	InsertColumn(8, "Player 4", LVCFMT_LEFT, 100, 8);
	InsertColumn(9, "Player 5", LVCFMT_LEFT, 100, 9);
	InsertColumn(10, "Player 6", LVCFMT_LEFT, 100, 10);

	//set the full row selection
	SetExtendedStyle(LVS_EX_FULLROWSELECT);

	//load the games
	readGames();
}

/////////////////////////////////////////////////////////////////////////////
// read in the games from the database file
/////////////////////////////////////////////////////////////////////////////
void CGameListCtrl::readGames()
{
	int i, iSize;
	CGame *pGame;

	//clear the array
	m_gameInfo.clear();

	//create a new game file
	pGame = new CGame;

	//find out how many games there are
	if(TRUE == m_bIncomplete)
	{
		iSize = DATABASE->numIncompleteGames();
	}
	else
	{
		iSize = DATABASE->numGames();
	}

	//keep going until we reach the end of the file
	for(i = 0; i < iSize; i++)
	{
		//get this game
		if(TRUE == m_bIncomplete)
		{
			DATABASE->getIncompleteGame(i, pGame);
		}
		else
		{
			DATABASE->getGame(i, pGame);
		}

		//add a game info
		m_gameInfo.push_back(CGameInfo(pGame));
	}

	//delete the used game file
	delete pGame;
	pGame = NULL;

	//populate the list control
	populateList();
}

/////////////////////////////////////////////////////////////////////////////
// function that populates the list control
/////////////////////////////////////////////////////////////////////////////
void CGameListCtrl::populateList()
{
	int i, j;
	CString str;
	COleDateTimeSpan span;

	//clear the list
	DeleteAllItems();

	//scan through the number of games
	for(i = 0; i < (int) m_gameInfo.size(); i++)
	{
		//insert the starting date
		InsertItem(i, m_gameInfo[i].m_timeStart.Format("%#m.%d.%Y"), 0);

		//insert the time
		SetItemText(i, 1, m_gameInfo[i].m_timeStart.Format("%#I:%M %p"));

		//set type
		switch(m_gameInfo[i].m_iMode)
		{
		case GAME_MODE_HOTSEAT: str = "Hotseat"; break;
		case GAME_MODE_NETWORK: str = "Network"; break;
		}

		//insert game type
		SetItemText(i, 2, str);

		//insert the number of players
		str.Format("%d", m_gameInfo[i].m_iPlayers);
		SetItemText(i, 3, str);

		//insert the length of the game played
		if(TRUE == m_bIncomplete)
		{
			//not complete yet, so mark it so
			SetItemText(i, 4, "Incomplete");
		}
		else
		{
			span = m_gameInfo[i].m_timeEnd - m_gameInfo[i].m_timeStart;
			SetItemText(i, 4, span.Format("%Hh, %Mm, %Ss"));
		}

		//create the player strings
		for(j = 0; j < (int) m_gameInfo[i].m_strNames.size(); j++)
		{
			SetItemText(i, 5 + j, m_gameInfo[i].m_strNames[j]);
		}		

		//set the item data
		SetItemData(i, (LPARAM) i);
	}

	//reset the selection
	m_nSelection = -1;
}

/////////////////////////////////////////////////////////////////////////////
// handle custom drawing
/////////////////////////////////////////////////////////////////////////////
void CGameListCtrl::OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	int nItem;
	CDC* pDC = NULL;
	CRect rect;
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

		//get item
		nItem = (int) pLVCD->nmcd.dwItemSpec;

		//set text color and highlight
		if(nItem == m_nSelection)
		{
			pLVCD->clrTextBk = ::GetSysColor(COLOR_HIGHLIGHT);
			pLVCD->clrText = COLOR_WHITE;
		}
		else
		{
			pLVCD->clrTextBk = COLOR_BLACK;
			pLVCD->clrText = COLOR_WHITE;
		}

		// Tell Windows to paint the control itself.
		*pResult = CDRF_NEWFONT;
	}
}