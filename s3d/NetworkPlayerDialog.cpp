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
#include "NetworkPlayerDialog.h"
#include "PlayerDatabase.h"
#include "NewPlayerDialog.h"
#include "NetworkConnector.h"
#include "defineIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declaration
/////////////////////////////////////////////////////////////////////////////
void OnNetworkPlayerClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayerDialog dialog


CNetworkPlayerDialog::CNetworkPlayerDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkPlayerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkPlayerDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//default to hosting
	m_bHost = FALSE;

	//default to no player
	m_iPlayer = -1;
}


void CNetworkPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPlayerDialog)
	DDX_Control(pDX, IDC_COLOR2_COMBO, m_Color2Combo);
	DDX_Control(pDX, IDC_COLOR1_COMBO, m_Color1Combo);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_EditButton);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	DDX_Control(pDX, IDC_HOST_RADIO, m_HostRadio);
	DDX_Control(pDX, IDC_CONNECT_RADIO, m_ConnectRadio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPlayerDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkPlayerDialog)
	ON_BN_CLICKED(IDC_ADD_BUTTON, OnAddButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayerDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// start things off
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkPlayerDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_NETWORK;
	
	//read value from ini file
	m_bHost = GetPrivateProfileInt(INI_NETWORK, INI_LAST_HOST, FALSE, INI_FILE);

	//set last value
	if(TRUE == m_bHost)
	{
		m_HostRadio.SetCheck(TRUE);
	}
	else
	{
		m_ConnectRadio.SetCheck(TRUE);
	}

	//init the player choices
	m_PlayerList.init(TRUE, FALSE);

	//set the callback function
	m_PlayerList.setCallBack((LPFNCALLBACK) OnNetworkPlayerClick, (DWORD) this);

	//disable the ok button until we get a good choice
	m_OKButton.EnableWindow(FALSE);
	m_EditButton.EnableWindow(FALSE);

	//if there's only one player in the list, select it
	if(1 == m_PlayerList.GetItemCount())
	{
		m_PlayerList.setSelection(0);
		handleSelection();
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// handle a click in the player list
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayerDialog::handleSelection(void)
{
	int nSel;

	//retrieve the selection from the list
	nSel = m_PlayerList.querySelection();
	
	//do stuff if we have a player
	if(-1 == nSel)
	{
		m_iPlayer = -1;
	}
	else
	{
		//get the selected player
		m_iPlayer = m_PlayerList.GetItemData(nSel);

		//set the color combos
		m_Color1Combo.SetCurSel(PLAYERS->getPlayer(m_iPlayer).getColor1());
		m_Color2Combo.SetCurSel(PLAYERS->getPlayer(m_iPlayer).getColor2());
	}

	//enable/disable the OK button and edit buttons
	m_OKButton.EnableWindow(m_iPlayer != -1);
	m_EditButton.EnableWindow(m_iPlayer != -1);
}

/////////////////////////////////////////////////////////////////////////////
// set host variable
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayerDialog::OnOK() 
{
	int nCheck;
	CString str;
	CPlayer player;

	m_bHost = m_HostRadio.GetCheck();

	//get the color selections
	m_iColor1 = m_Color1Combo.GetCurSel();
	m_iColor2 = m_Color2Combo.GetCurSel();

	//see if they're valid
	if(-1 == m_iColor1 || -1 == m_iColor2)
	{
		MessageBox("You must select a preferred color and second choice.", "Color Preferences");
		return;
	}

	//compare against current selection
	if((m_iColor1 != PLAYERS->getPlayer(m_iPlayer).getColor1()) ||
		m_iColor2 != PLAYERS->getPlayer(m_iPlayer).getColor2())
	{
		if(IDYES == MessageBox("Would you like these colors to become your default colors?", "Color Preferences", MB_YESNO))
		{
			player = PLAYERS->getPlayer(m_iPlayer);
			player.setColor1(m_iColor1);
			player.setColor2(m_iColor2);
			PLAYERS->updatePlayer(m_iPlayer, player);
			PLAYERS->savePlayers();
			
		}
	}

	//write to .ini file
	str.Format("%d", m_bHost ? 1 : 0);
	WritePrivateProfileString(INI_NETWORK, INI_LAST_HOST, str, INI_FILE);

	//if the player is a ladder player, check to see that they still exist and
	//are not banned
	if(TRUE == PLAYERS->getPlayer(m_iPlayer).getLadder())
	{
		//check the name
		nCheck = CONNECTOR->validateName(PLAYERS->getPlayer(m_iPlayer).getName());

		//parse
		if(-1 == nCheck)
		{
			//they've been banned
			MessageBox("You have been banned from playing on the ladder.  You may no longer play ladder games with this player.", "Sea3D Connector");
		}
		//check for non existence
		else if(TRUE == nCheck)
		{
			MessageBox("Sea3D was not able to find your player on the ladder!  You must redownload\r\nyour player (or reregister) before playing in ladder games.", "Sea3D Connector");
		}

		//cleanup
		if(0 != nCheck)
		{
			player = PLAYERS->getPlayer(m_iPlayer);
			player.setLadder(FALSE);
			player.setLadderID("");
			PLAYERS->updatePlayer(m_iPlayer, player);

			//save the database
			PLAYERS->savePlayers();

			//reset list
			m_PlayerList.loadPlayers();

			return;
		}
	}

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// add a new player to the database
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayerDialog::OnAddButton() 
{
	CPlayer player;
	CNewPlayerDialog d;

	//run the new player dialog
	if(d.DoModal() == IDOK)
	{
		player = d.getPlayer();

		//add it to the player database
		PLAYERS->addPlayer(&player);

		//reset list
		m_PlayerList.loadPlayers();

		//reset buttons
		m_OKButton.EnableWindow(FALSE);
		m_EditButton.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// add a new player to the database
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayerDialog::OnEditButton() 
{
	BOOL bUpdate = FALSE;
	CPlayer player, playerNew;
	CNewPlayerDialog dlg;

	//get existing player
	player = PLAYERS->getPlayer(m_iPlayer);

	//set player info
	dlg.setEdit();
	dlg.setPlayer(player);

	//run the new player dialog
	if(dlg.DoModal() == IDOK)
	{
		//get the updated player
		playerNew = dlg.getPlayer();

		//see if we need to update the game database
		if ((playerNew.getFirstName() != player.getFirstName()) ||
			(playerNew.getLastName() != player.getLastName()))
		{
			bUpdate = TRUE;
		}

		//update the player
		PLAYERS->updatePlayer(m_iPlayer, playerNew);

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
				PLAYERS->updateGames(m_iPlayer);
			}
		}

		//reset buttons
		m_OKButton.EnableWindow(FALSE);
		m_EditButton.EnableWindow(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnNetworkPlayerClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CNetworkPlayerDialog *) dwCookie)->handleSelection();
	}
}