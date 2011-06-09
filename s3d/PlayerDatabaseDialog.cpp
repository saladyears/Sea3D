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
#include "PlayerDatabaseDialog.h"
#include "NewPlayerDialog.h"
#include "GLView.h"
#include "UndoDialog.h"

#include "PlayerDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declaration
/////////////////////////////////////////////////////////////////////////////
void OnPlayerDatabaseClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CPlayerDatabaseDialog dialog


CPlayerDatabaseDialog::CPlayerDatabaseDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CPlayerDatabaseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPlayerDatabaseDialog)
	//}}AFX_DATA_INIT
}


void CPlayerDatabaseDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDatabaseDialog)
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_DeleteButton);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerDatabaseDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CPlayerDatabaseDialog)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, OnDeleteButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_NOTIFY(NM_DBLCLK, IDC_PLAYER_LIST, OnDblclkPlayerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDatabaseDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// handle initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CPlayerDatabaseDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	m_PlayerList.init();

	//help system
	m_strHelp = HELP_DBPLAYERS;

	//set the callback
	m_PlayerList.setCallBack(OnPlayerDatabaseClick, (DWORD) this);

	//delete and edit buttons disabled until selection
	m_DeleteButton.EnableWindow(FALSE);
	m_EditButton.EnableWindow(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// add a new player to the database
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabaseDialog::OnAddButton() 
{
	CPlayer player;
	CNewPlayerDialog d;

	//run the new player dialog
	if(d.DoModal() == IDOK)
	{
		player = d.getPlayer();

		//add it to the player database
		PLAYERS->addPlayer(&player);

		//reset everything
		m_PlayerList.loadPlayers();
	}	
}

/////////////////////////////////////////////////////////////////////////////
// delete a new player to the database
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabaseDialog::OnDeleteButton() 
{
	int nSel;
	int iPlayer;

	//get the selection
	nSel = m_PlayerList.querySelection();
	
	//sanity check
	if(nSel == -1) 
	{
		return;
	}

	//see if they really want to
	if(IDNO == MessageBox("Are you sure you want to delete this player?", "Delete player?", MB_YESNO)) 
	{
		return;
	}

	//get the player number
	iPlayer = m_PlayerList.GetItemData(nSel);

	//delete the player
	PLAYERS->deletePlayer(iPlayer);

	//reset the contents
	m_PlayerList.loadPlayers();

	//redisable the controls until they select someone
	m_DeleteButton.EnableWindow(FALSE);
	m_EditButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// edit a player's name
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabaseDialog::OnEditButton() 
{
	int iSel;
	int nSelection;
	BOOL bUpdate = FALSE;
	CPlayer player;
	CPlayer newPlayer;
	CNewPlayerDialog dlg;

	//get selection
	nSelection = m_PlayerList.querySelection();

	//sanity check
	if(nSelection == -1) 
	{
		return;
	}

	//get the actual vector array number
	iSel = (int) m_PlayerList.GetItemData(nSelection);

	//get the player information
	player = PLAYERS->getPlayer(iSel);

	//set the edit flag
	dlg.setEdit();
	dlg.setPlayer(player);

	//run it
	if(dlg.DoModal() == IDOK)
	{
		//get the updated player
		newPlayer = dlg.getPlayer();

		//see if we need to update the game database
		if ((newPlayer.getFirstName() != player.getFirstName()) ||
			(newPlayer.getLastName() != player.getLastName()))
		{
			bUpdate = TRUE;
		}

		//update the player
		PLAYERS->updatePlayer(iSel, newPlayer);

		//save it and repopulate
		PLAYERS->savePlayers();

		//reload
		m_PlayerList.loadPlayers();

		//ask if they want to update the game database
		if(TRUE == bUpdate)
		{
			if(IDYES == MessageBox("Do you want to update the game database with the new name?", "Update database?", MB_YESNO))
			{
				//update the game database
				PLAYERS->updateGames(iSel);
			}
		}

		//redisable the controls until they select someone
		m_DeleteButton.EnableWindow(FALSE);
		m_EditButton.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle clicking in the list
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabaseDialog::handleSelection()
{
	int nSel;
	int iPlayer;
	
	//get the selected item(s)
	nSel = m_PlayerList.querySelection();

	//enable the windows
	m_DeleteButton.EnableWindow(nSel != -1);
	m_EditButton.EnableWindow(nSel != -1);
	
	//check to make sure player was created here
	if(-1 != nSel)
	{
		iPlayer = m_PlayerList.GetItemData(nSel);

		//determine editability
		m_EditButton.EnableWindow(PLAYERS->getPlayer(iPlayer).getMadeHere());
	}
}

/////////////////////////////////////////////////////////////////////////////
// search through the game database and update names
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabaseDialog::OnDblclkPlayerList(NMHDR* /*pNMHDR*/, LRESULT* /*pResult*/) 
{
	//normal click
	handleSelection();
	
	//edit
	if(TRUE == m_EditButton.IsWindowEnabled())
	{
		OnEditButton();
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnPlayerDatabaseClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CPlayerDatabaseDialog *) dwCookie)->handleSelection();
	}
}