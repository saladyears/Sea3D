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
#include "NewGameDialog.h"
#include "SettlersView.h"
#include "PlayerDatabase.h"
#include "ColorDatabase.h"
#include "MapDatabase.h"
#include "NewPlayerDialog.h"
#include "VariantsDialog.h"
#include "defineVariants.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewGameDialog dialog


CNewGameDialog::CNewGameDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNewGameDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewGameDialog)
	//}}AFX_DATA_INIT

	m_iPlayers = 0;
	m_pDropButton = NULL;

	//haven't created someone yet
	m_bOnce = FALSE;
}


void CNewGameDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewGameDialog)
	DDX_Control(pDX, IDC_ADD_BUTTON, m_AddButton);
	DDX_Control(pDX, IDC_VARIANTS_BUTTON, m_VariantsButton);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_WHITE_TEXT, m_WhiteText);
	DDX_Control(pDX, IDC_RED_TEXT, m_RedText);
	DDX_Control(pDX, IDC_ORANGE_TEXT, m_OrangeText);
	DDX_Control(pDX, IDC_BLUE_TEXT, m_BlueText);
	DDX_Control(pDX, IDC_BROWN_TEXT, m_BrownText);
	DDX_Control(pDX, IDC_GREEN_TEXT, m_GreenText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewGameDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNewGameDialog)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_PLAYER_LIST, OnBegindragPlayerList)
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYER_LIST, OnDblclkPlayerList)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_VARIANTS_BUTTON, OnVariantsButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewGameDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// start with default
/////////////////////////////////////////////////////////////////////////////
BOOL CNewGameDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_PLAYSEL;

	m_PlayerList.init(FALSE, FALSE, FALSE);

	//ok is disabled until three players are in
	m_OKButton.EnableWindow(FALSE);

	//turn on borders
	m_WhiteText.TurnOnBorder();
	m_RedText.TurnOnBorder();
	m_OrangeText.TurnOnBorder();
	m_BlueText.TurnOnBorder();
	m_BrownText.TurnOnBorder();
	m_GreenText.TurnOnBorder();

	//register drag and drop
	::RegisterDragDrop(m_WhiteText.GetSafeHwnd(), &m_WhiteText);
	::RegisterDragDrop(m_RedText.GetSafeHwnd(), &m_RedText);
	::RegisterDragDrop(m_GreenText.GetSafeHwnd(), &m_GreenText);
	::RegisterDragDrop(m_BlueText.GetSafeHwnd(), &m_BlueText);
	::RegisterDragDrop(m_BrownText.GetSafeHwnd(), &m_BrownText);
	::RegisterDragDrop(m_OrangeText.GetSafeHwnd(), &m_OrangeText);
	::RegisterDragDrop(m_PlayerList.GetSafeHwnd(), &m_PlayerList);

	//set list control pointers
	m_WhiteText.setListCtrl(&m_PlayerList);
	m_RedText.setListCtrl(&m_PlayerList);
	m_OrangeText.setListCtrl(&m_PlayerList);
	m_BlueText.setListCtrl(&m_PlayerList);
	m_BrownText.setListCtrl(&m_PlayerList);
	m_GreenText.setListCtrl(&m_PlayerList);

	//setup all the color buttons
	m_WhiteText.SetBackground(COLORS->getTextColor(CR_WHITE));
	m_RedText.SetBackground(COLORS->getTextColor(CR_RED));
	m_OrangeText.SetBackground(COLORS->getTextColor(CR_ORANGE));
	m_BlueText.SetBackground(COLORS->getTextColor(CR_BLUE));
	m_BrownText.SetBackground(COLORS->getTextColor(CR_BROWN));
	m_GreenText.SetBackground(COLORS->getTextColor(CR_GREEN));
	
	//set up fonts
	m_WhiteText.SetFont(Tahoma18B);
	m_RedText.SetFont(Tahoma18B);
	m_OrangeText.SetFont(Tahoma18B);
	m_GreenText.SetFont(Tahoma18B);
	m_BrownText.SetFont(Tahoma18B);
	m_BlueText.SetFont(Tahoma18B);

	//set up center
	m_WhiteText.SetLeftBorder(10); m_WhiteText.SetTopBorder(7);
	m_RedText.SetLeftBorder(10); m_RedText.SetTopBorder(7);
	m_OrangeText.SetLeftBorder(10); m_OrangeText.SetTopBorder(7);
	m_GreenText.SetLeftBorder(10); m_GreenText.SetTopBorder(7);
	m_BrownText.SetLeftBorder(10); m_BrownText.SetTopBorder(7);
	m_BlueText.SetLeftBorder(10); m_BlueText.SetTopBorder(7);

	//set text colors
	m_WhiteText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);
	m_RedText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);
	m_OrangeText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);
	m_GreenText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);
	m_BrownText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);
	m_BlueText.SetColors(COLOR_DKGRAY, COLOR_DKGRAY);

	//set title text
	if(GAME_MODE_HOTSEAT == GAME->m_iMode)
	{
		SetWindowText("Select the players in your Sea3D game");
	}

	//make sure we're centered
	CenterWindow();

	//if we're in tour mode, start up the balloon stuff
	if(TRUE == APP->inTour())
	{
		//see if they already have players
		if(1 < m_PlayerList.GetItemCount())
		{
			m_balloonClick.Create("Sea3D Tour",
				"Well, it seems like you've already got some players in the database!  Now you need to add\nthem to the game.  You can do this by double-clicking a name in the list.  This will assign the\nname to the next available color.  You can give a player a different color by clicking and\ndragging them to the color you want them to have.\n\nDouble-click the player's names in the list to add them to the game.\n\nPress Esc to cancel the tour.", 
				CPoint(5, 25),
				CBalloonHelp::unSHOW_INNER_SHADOW,
				&m_PlayerList);
		}
		else
		{
			//set up the hotseat balloon
			m_balloonAdd.Create("Sea3D Tour",
			"Now we have a map selected for a hotseat game.  There's only one problem... we don't\nhave any players to play with!  Fortunately, Sea3D allows you to create players right when\nyou need them.\n\nClick the Add Player button.\n\nPress Esc to cancel the tour.", 
			CPoint(5, 3),
			CBalloonHelp::unSHOW_INNER_SHADOW,
			&m_AddButton);
		}

		//add the variant to the game to prevent 7's rolled first round
		VARIANTS->m_iVariants |= VARIANT_NO_7_FIRST_ROUND;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// start a new game
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnOK() 
{
	int i, j;
	int iIndex[6];
	CPlayer player;

	//create the id list
	iIndex[0] = m_WhiteText.getIndex();
	iIndex[1] = m_RedText.getIndex();
	iIndex[2] = m_OrangeText.getIndex();
	iIndex[3] = m_BlueText.getIndex();
	iIndex[4] = m_BrownText.getIndex();
	iIndex[5] = m_GreenText.getIndex();

	//create the basic game structure
	GAME->m_players.resize(m_iPlayers);
	VIEW->setNumPlayers();

	//set the IDs and colors
	for(i = 0, j = 0; i < 6; i++)
	{
		if(iIndex[i] != -1)
		{
			//get the player
			player = PLAYERS->getPlayer(iIndex[i]);
			
			//set the player
			GAME->m_players[j].m_player = player;

			//set the color
			GAME->m_players[j].m_iColor = i;

			//initialize size
			GAME->m_players[j++].initPlayer(MAP->m_iLandTiles + MAP->m_iSeaTiles);
		}
	}

	//set the game init
	GAME->m_bInit = TRUE;

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle drag and drop
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnBegindragPlayerList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int idx, iSel;
	CRect rect;
	CString strItem;

	//normal stuff
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	//default return value
	*pResult = 0;

	//create the drag&drop source and data objects
	COleDropSource *pDropSource = new COleDropSource;
	COleDataSource *pDataSource = new COleDataSource;

	//see if there's a selection
	if((idx = pNMListView->iItem) == -1) return;

	//turn on dragging
	resetAllDrags();
	m_bDrag = TRUE;

	//get the right index
	iSel = m_PlayerList.GetItemData(idx);

	//get the item text
	strItem.Format("%d\t%d", iSel, idx);

	//get the bounding rectangle
	m_PlayerList.GetItemRect(0, &rect, LVIR_BOUNDS);

	TRY
	{
		//create the shared file
		CSharedFile file(GMEM_ZEROINIT|GMEM_DDESHARE|GMEM_MOVEABLE);
		TRY
		{
			//create the archive
			CArchive ar(&file, CArchive::store);
			TRY
			{
				// for CF_TEXT use a flat ASCII-stream
				ar.WriteString(strItem);    
				ar.Close();
			}
			CATCH_ALL(eInner)
			{
				// exception while writing into or closing the archive
				ASSERT(FALSE);
			}
			END_CATCH_ALL;
		}
		CATCH_ALL(eMiddle)
		{
			// exception in the destructor of ar
			ASSERT(FALSE);
		}
		END_CATCH_ALL;

		// put the file object into the data object
		pDataSource->CacheGlobalData(CF_TEXT, file.Detach());
		pDataSource->DoDragDrop(DROPEFFECT_MOVE|DROPEFFECT_COPY, &rect, pDropSource);
	}
	CATCH_ALL(eOuter)
	{
		// exception while destructing the file
		ASSERT(FALSE);
	}
	END_CATCH_ALL;

	delete pDropSource;
	delete pDataSource;
}

/////////////////////////////////////////////////////////////////////////////
// resets all drag variables
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::resetAllDrags()
{
	//list control
	m_bDrag = FALSE;

	//buttons
	m_WhiteText.resetDrag();
	m_RedText.resetDrag();
	m_OrangeText.resetDrag();
	m_GreenText.resetDrag();
	m_BrownText.resetDrag();
	m_BlueText.resetDrag();
}

/////////////////////////////////////////////////////////////////////////////
// handle losing and gaining players
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::incPlayers() 
{
	//increment
	m_iPlayers++;

	//compare
	m_OKButton.EnableWindow(1 <= m_iPlayers && m_iPlayers <= 6);

	//see if we're done with the tour
	if(TRUE == APP->inTour() && 2 == m_iPlayers)
	{
		//close the old
		if(NULL != m_balloonClick.m_hWnd)
		{
			m_balloonClick.SendMessage(WM_CLOSE);
		}

		if(NULL == m_balloonOK.m_hWnd)
		{
			m_balloonOK.Create("Sea3D Tour",
			"Now click OK to continue on.\n\nPress Esc to cancel the tour.", 
			CPoint(25, 5),
			CBalloonHelp::unSHOW_INNER_SHADOW,
			&m_OKButton);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle losing and gaining players
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::decPlayers() 
{
	//decrement
	m_iPlayers--; 

	//compare
	m_OKButton.EnableWindow(1 <= m_iPlayers && m_iPlayers <= 6);
}

/////////////////////////////////////////////////////////////////////////////
// treat like an undo
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnCancel() 
{
	//no longer in a tour
	APP->setTour(FALSE);
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// handle a double-click in the list, to add a player
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnDblclkPlayerList(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	int i;
	int nSelection;
	int iSel;
	int iStatic = -1;
	CString strItem;
	CDropStatic *pStatic;

	//get the selection
	nSelection = m_PlayerList.querySelection();

	//sanity check
	if(-1 == nSelection) 
	{
		return;
	}

	//run through and find the first open spot
	for(i = 0; i < 6; i++)
	{
		//set the static pointer
		pStatic = &m_WhiteText + i;

		//see if it's empty
		if(pStatic->GetText().IsEmpty())
		{
			iStatic = i;
			break;
		}
	}

	//if we didn't find a free one, return
	if(iStatic == -1) return;

	//reset to the free one
	pStatic = &m_WhiteText + iStatic;

	//add it
	iSel = m_PlayerList.GetItemData(nSelection);

	//get the item text
	strItem.Format("%d\t%d", iSel, nSelection);

	//add it
	pStatic->ProcessData(strItem);
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// add a new player to the database
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnAddButton() 
{
	int iSize, iIndex;
	CPlayer player;
	CNewPlayerDialog d;

	//close down balloon
	if(NULL != m_balloonAdd.m_hWnd)
	{
		m_balloonAdd.SendMessage(WM_CLOSE);
	}

	//set the value
	if(TRUE == APP->inTour())
	{
		d.setFirst(!m_bOnce);
	}

	//run the new player dialog
	if(d.DoModal() == IDOK)
	{
		player = d.getPlayer();

		//add it to the player database
		iIndex = PLAYERS->addPlayer(&player);

		//add them to the control
		iSize = m_PlayerList.GetItemCount();

		//insert the player
		m_PlayerList.InsertItem(iSize, player.getCommaName());
		m_PlayerList.SetItemData(iSize, (LPARAM) iIndex);

		//reset selection
		m_PlayerList.setSelection();

		//resort
		m_PlayerList.sort();

		//now reuse the add balloon if needed
		if(TRUE == APP->inTour())
		{
			if(FALSE == m_bOnce)
			{
				//set up the balloon
				m_balloonAdd.Create("Sea3D Tour",
				"One player down, one to go. Sea3D allows you to play with anywhere from 1 to 6\nplayers.  (Why you would want to play with just one, I don't know, but you can...)\nNow create a second player to join your hotseat game.\n\nClick the Add Player button.\n\nPress Esc to cancel the tour.", 
				CPoint(5, 3),
				CBalloonHelp::unSHOW_INNER_SHADOW,
				&m_AddButton);
				m_bOnce = TRUE;
			}
			else
			{
				m_balloonClick.Create("Sea3D Tour",
				"Now that you've added players to the database, you need to add them to the game.\nYou can do this by double-clicking a name in the list.  This will assign the name\nto the next available color.  You can give a player a different color by clicking\nand dragging them to the color you want them to have.\n\nDouble-click the player's names in the list to add them to the game.\n\nPress Esc to cancel the tour.", 
				CPoint(5, 25),
				CBalloonHelp::unSHOW_INNER_SHADOW,
				&m_PlayerList);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle variants
/////////////////////////////////////////////////////////////////////////////
void CNewGameDialog::OnVariantsButton() 
{
	CVariantsDialog dlg;
	
	dlg.DoModal();
}