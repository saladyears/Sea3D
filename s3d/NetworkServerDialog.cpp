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
#include "NetworkServerDialog.h"
#include "SettlersView.h"
#include "PlayerDatabase.h"
#include "NetChatMessage.h"
#include "NetworkPortDialog.h"
#include "MapDatabaseDialog.h"
#include "VariantsDialog.h"
#include "NetworkAIDialog.h"
#include "AIDatabase.h"
#include "NetworkConnectorSetupDialog.h"
#include "NetworkFirewallDialog.h"
#include "defineNetwork.h"
#include "defineVariants.h"
#include "defineIni.h"
#include "defineMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkServerDialog dialog


CNetworkServerDialog::CNetworkServerDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkServerDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkServerDialog)
	//}}AFX_DATA_INIT

	//default port
	m_uiPort = PORT;

	//no map
	m_iMap = -1;

	//no reload
	m_bReload = FALSE;
}


void CNetworkServerDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkServerDialog)
	DDX_Control(pDX, IDC_CONNECTOR_BUTTON, m_ConnectorButton);
	DDX_Control(pDX, IDC_TCPIP_STATIC, m_TCPStatic);
	DDX_Control(pDX, IDC_VARIANTS_BUTTON, m_VariantsButton);
	DDX_Control(pDX, IDC_BOOT_BUTTON, m_BootButton);
	DDX_Control(pDX, IDC_MAP_BUTTON, m_MapButton);
	DDX_Control(pDX, IDC_AI_BUTTON, m_AIButton);
	DDX_Control(pDX, IDC_INFO_EDIT, m_InfoEdit);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_SEND_EDIT, m_SendEdit);
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkServerDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkServerDialog)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_BN_CLICKED(IDC_ADVANCED_BUTTON, OnAdvancedButton)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MAP_BUTTON, OnMapButton)
	ON_BN_CLICKED(IDC_BOOT_BUTTON, OnBootButton)
	ON_BN_CLICKED(IDC_VARIANTS_BUTTON, OnVariantsButton)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_BN_CLICKED(IDC_AI_BUTTON, OnAiButton)
	ON_BN_CLICKED(IDC_CONNECTOR_BUTTON, OnConnectorButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkServerDialog message handlers

BOOL CNetworkServerDialog::OnInitDialog() 
{
	CString strIP;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_HOST;

	//tell the server about our list control
	VIEW->setListCtrl(&m_PlayerList);
	VIEW->setChatCtrl(&m_ChatEdit);
	VIEW->setInfoCtrl(&m_InfoEdit);
	VIEW->setServerDialog(this);

	//get the IP address
	strIP = APP->getIPAddress();

	//clear the send edit
	m_SendEdit.SetWindowText("");
	m_TCPStatic.SetWindowText(strIP);

	//create the listening port only if this is a fresh game
	VIEW->StartListening(m_uiPort);

	//create the ping timer
	VIEW->serverStartPing();

	//disable start until map is selected
	m_OKButton.EnableWindow(FALSE);

	//variants button disabled until map is selected
	m_VariantsButton.EnableWindow(FALSE);

	//disalbe connector window until they choose a map
	m_ConnectorButton.EnableWindow(FALSE);

	//enable/disable AI button
	m_AIButton.EnableWindow(APP->hasAI() && !m_bReload);

	//set the original info string
	if(TRUE == m_bReload)
	{
		startReload();
	}
	else
	{
		startNormal();
	}

	//load the accelerator resource
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// initial start settings
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::startNormal()
{
	CPlayer player;

	//get the player
	player = PLAYERS->getPlayerByID(GAME->m_uiPlayerID);
	player.setReady(TRUE);

	//set info string
	VIEW->setInfoString("Map: no map selected");

	//set button text
	m_OKButton.SetWindowText("Start game!");

	//add ourselves to the list control
	m_PlayerList.addPlayer(&player, GAME->m_players[GAME->m_iIndex].m_iColor);

	//set title text
	SetWindowText("Host a network game");
}

/////////////////////////////////////////////////////////////////////////////
// initial start settings
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::startReload()
{
	int i;
	CString str;

	//set map
	m_iMap = GAME->m_uiMapID;

	//set the team text
	if(TRUE == GAME->m_bTeam)
	{
		str = "This is a team game.\n\n";
	}
	//set the ladder text
	if(TRUE == GAME->m_bLadder)
	{
		//get rid of the extra newline
		str = str.Left(str.GetLength() - 1);

		//add the ladder text
		str += "This is a ladder game.\n\n";
	}

	//create info string
	str += MAPS->getMapInfo(m_iMap);

	//add variant string
	str += VARIANTS->getVariantString();

	//set map text
	VIEW->setInfoString(str);

	//disable map button
	m_MapButton.EnableWindow(FALSE);

	//boot player button is disabled
	m_BootButton.EnableWindow(FALSE);

	//enable the connector button for restarts
	m_ConnectorButton.EnableWindow();

	//set button text
	m_OKButton.SetWindowText("Continue!");

	//add players to the control
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		m_PlayerList.addPlayer(&(GAME->m_players[i].m_player), GAME->m_players[i].m_iColor);

		//add AI's as joined already
		if(TRUE == GAME->m_players[i].m_player.getAI())
		{
			//tell the list
			m_PlayerList.updateJoin(GAME->m_players[i].m_player.getID(), TRUE);

			//set their flag as joined
			GAME->m_players[i].m_bJoined = TRUE;
		}
	}

	//set the server player as joined
	m_PlayerList.updateJoin(GAME->m_uiPlayerID, TRUE);

	//set their flag as joined
	GAME->m_players[GAME->m_iIndex].m_bJoined = TRUE;

	//set title text
	SetWindowText("Restart a network game");

	//with AI, see if we can continue yet
	checkForContinue();
}

/////////////////////////////////////////////////////////////////////////////
// send stuff
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnSendButton() 
{
	CString str;
	CNetChatMessage chat;
	CNDKMessage msg(S3D_CHAT_MESSAGE);

	//get the data from the edit
	m_SendEdit.GetWindowText(str);

	//set the fields
	chat.m_iColor = GAME->m_players[GAME->m_iIndex].m_iColor;
	chat.m_strName = GAME->m_players[GAME->m_iIndex].m_player.getFirstName();
	chat.m_strMessage = str;
	
	//clear it
	m_SendEdit.SetWindowText("");

	//add it ourselves
	m_ChatEdit.addMessage(&chat);

	//translate it
	chat.SerializeToMessage(msg);

	//transmit it
	VIEW->SendMessageToAllUsers(msg);

	//set the focus back to the edit
	m_SendEdit.SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// they're quitting!
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnCancel() 
{
	//see if anyone is connected
	if(VIEW->GetNbUsers() > 0)
	{
		if(IDNO == MessageBox("There are people connected to this server!  Are you sure you want to quit?", "Network Connection", MB_YESNO))
		{
			return;
		}
	}

	//stop the server
	VIEW->Stop();

	//see if the connector needs to be updated
	if(TRUE == CONNECTOR->getConnector())
	{
		CONNECTOR->postServer(CONNECTOR_DELETE);
	}
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// change listening port
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnAdvancedButton() 
{
	CNetworkPortDialog dlg;

	//set it
	dlg.setPort(m_uiPort);

	if(IDOK == dlg.DoModal())
	{
		//if there's anyone connected, see if they really want to do this
		if(VIEW->GetNbUsers() > 0)
		{
			if(IDNO == MessageBox("There are people connected to this server!  Are you sure you want to use a different port?", "Network Connection", MB_YESNO))
			{
				return;
			}
		}

		//set the new port
		m_uiPort = dlg.getPort();

		//stop the server
		VIEW->Stop();

		//recreate the listening port
		VIEW->StartListening(m_uiPort);

		//clear everything (only if it's not a reload)
		if(FALSE == m_bReload)
		{
			VIEW->resetGame();
		}

		//if using the connector, update
		if(TRUE == CONNECTOR->getConnector())
		{
			CONNECTOR->setPort(m_uiPort);
			CONNECTOR->postServer(CONNECTOR_UPDATE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle shutdown
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnDestroy() 
{
	CHelpDialog::OnDestroy();
	
	//references are no longer valid
	VIEW->setListCtrl(NULL);
	VIEW->setChatCtrl(NULL);
	VIEW->setInfoCtrl(NULL);
	VIEW->setServerDialog(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// handle timer
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_PING: VIEW->PingAllUsers(); break;
	default: break;
	}
	
	CHelpDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// get which map they want
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnMapButton() 
{
	int i;
	CString str;
	CMapDatabaseDialog dlg;

	if(IDOK == dlg.DoModal())
	{
		m_iMap = dlg.getMap();

		//update all of the players readiness
		for(i = 0; i < (int) GAME->m_players.size(); i++)
		{
			//skip self
			if(i == GAME->m_iIndex)
			{
				continue;
			}

			VIEW->clientReady(GAME->m_players[i].m_player.getID(), FALSE);
		}

		//set the team text
		if(TRUE == GAME->m_bTeam)
		{
			str = "This is a team game.\n\n";
		}
		//set the ladder text
		if(TRUE == GAME->m_bLadder)
		{
			//get rid of the extra newline
			str = str.Left(str.GetLength() - 1);

			//add the ladder text
			str += "This is a ladder game.\n\n";
		}

		//create info string (which loads the map)
		str += MAPS->getMapInfo(m_iMap);

		//set variant qualities
		VARIANTS->m_iStockRoads = MAP->m_iStockRoads;
		VARIANTS->m_iStockShips = MAP->m_iStockShips;
		VARIANTS->m_iStockCities = MAP->m_iStockCities;
		VARIANTS->m_iStockSettlements = MAP->m_iStockSettlements;
		VARIANTS->m_iAllowedCards = MAP->m_iAllowedCards;
		VARIANTS->m_iPoints = MAP->m_iPoints;

		//reset bit string
		VARIANTS->m_iVariants &= ~(VARIANT_ALTER_ROADS | VARIANT_ALTER_CITIES | VARIANT_ALTER_SETTLEMENTS | VARIANT_ALTER_ALLOWED_CARDS);

		//set settings
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_TRADE_ZERO, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_TRADE_ZERO;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_EQUAL_ODDS, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_EQUAL_ODDS;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_TRADE_BUILD, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_TRADE_AFTER_BUILD;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_NO_7, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_NO_7_FIRST_ROUND;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_SHOW_CARDS, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_SHOW_ALL_CARDS;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_ROUND_UP, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_ROUND_LOST_UP;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_HIDE_CHIPS, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_HIDE_CHIPS;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_SPECIAL, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_SPECIAL_BUILD;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_TOUR_START, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_TOURNAMENT_START;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_NO_TRADE, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_NO_TRADING;
		if(TRUE == GetPrivateProfileInt(INI_VARIANT, INI_VAR_LIMIT_TRADE, 0, INI_FILE)) VARIANTS->m_iVariants |= VARIANT_LIMIT_TRADING;

		//no 7's rounds
		VARIANTS->m_nNoSevens = GetPrivateProfileInt(INI_VARIANT, INI_VAR_NO_SEVENS, 1, INI_FILE);

		//trade offers limit
		VARIANTS->m_nTradeOffers = GetPrivateProfileInt(INI_VARIANT, INI_VAR_LIMIT_NUM, 3, INI_FILE);

		//set the allowed cards if the special build phase is turned on
		if(IS_VARIANT(VARIANT_SPECIAL_BUILD) && (9 == MAP->m_iAllowedCards))
		{
			//set it
			VARIANTS->m_iAllowedCards = 7;
			VARIANTS->m_iVariants |= VARIANT_ALTER_ALLOWED_CARDS;
		}

		//desert/volcanoes/jungles
		VARIANTS->m_iVariants |=  GetPrivateProfileInt(INI_VARIANT, INI_VAR_DESERTS, 0, INI_FILE);

		//must load sea chips as well if using jungles or deserts
		if(IS_VARIANT(VARIANT_USE_JUNGLE) || IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
		{
			MAP->m_iChipsUsed |= MAP_USES_SEA_CHIPS;
		}

		//add variant string
		str += VARIANTS->getVariantString();

		//tell all
		VIEW->setInfoString(str);

		//handle ladder restrictions
		handleLadder();

		//now we can choose variants
		m_VariantsButton.EnableWindow();

		//enable the connector window
		if(FALSE == CONNECTOR->getConnector())
		{
			m_ConnectorButton.EnableWindow();
		}
		//otherwise, update the data
		else
		{
			CONNECTOR->postServer(CONNECTOR_UPDATE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// make sure they've selected a map
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnOK() 
{
	if(-1 == m_iMap)
	{
		AfxMessageBox("You must select a map to play before starting.");
		return;
	}

	//if this is a restart, and a ladder game, everyone has joined, so clear
	//out the quits/drops for this game
	if((TRUE == m_bReload) && (TRUE == GAME->m_bLadder))
	{
		//clear any quits from the game
		CONNECTOR->clearQuits();
	}

	//if using the connector, set the new state
	if(TRUE == CONNECTOR->getConnector())
	{
		CONNECTOR->postServer(CONNECTOR_INGAME);
	}
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// remove a player from the game
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnBootButton() 
{
	int iSelection;
	UINT uiID;

	//get the current selection
	iSelection = m_PlayerList.querySelection();

	//make sure we have one
	if(-1 == iSelection)
	{
		AfxMessageBox("You must select a player before you can boot them.");
		return;
	}

	//get their ID
	uiID = (UINT) m_PlayerList.GetItemData(iSelection);

	//if it's ours, we can't boot ourselves
	if(uiID == GAME->m_uiPlayerID)
	{
		AfxMessageBox("You can't boot yourself from the game, you're the server!");
		return;
	}

	//boot this player
	VIEW->bootPlayer(uiID);
}

/////////////////////////////////////////////////////////////////////////////
// see if we can continue a network save game
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::checkForContinue()
{
	int i;
	BOOL bContinue = TRUE;

	//search players to see if all have joined
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		if(FALSE == GAME->m_players[i].m_bJoined)
		{
			bContinue = FALSE;
			break;
		}
	}

	//enable the continue button
	m_OKButton.EnableWindow(bContinue);
}

/////////////////////////////////////////////////////////////////////////////
// run the variants dialog
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnVariantsButton() 
{
	int i;

	CVariantsDialog dlg;

	//run it
	if(IDOK == dlg.DoModal())
	{
		//update all of the players readiness
		for(i = 0; i < (int) GAME->m_players.size(); i++)
		{
			//skip self
			if(i == GAME->m_iIndex)
			{
				continue;
			}

			VIEW->clientReady(GAME->m_players[i].m_player.getID(), FALSE);
		}

		updateInfoString();
	}	
}

/////////////////////////////////////////////////////////////////////////////
// update the info string window
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::updateInfoString()
{
	CString str;

	//set the team text
	if(TRUE == GAME->m_bTeam)
	{
		str = "This is a team game.\n\n";
	}
	//set the ladder text
	if(TRUE == GAME->m_bLadder)
	{
		//get rid of the extra newline
		str = str.Left(str.GetLength() - 1);

		//add the ladder text
		str += "This is a ladder game.\n\n";
	}

	if(-1 != m_iMap)
	{
		//create info string
		str += MAPS->getMapInfo(m_iMap);
	}

	//add variant string
	str += VARIANTS->getVariantString();

	//tell all
	VIEW->setInfoString(str);
}

/////////////////////////////////////////////////////////////////////////////
// handles message forwarding
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkServerDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(NULL != m_hAccelTable)
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) 
		{
			return TRUE;
		}
	}
	
	return CHelpDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// handles pasting
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnEditPaste() 
{
	CWnd *pFocus;

	//get what has current focus
	pFocus = GetFocus();

	//compare
	if(&m_SendEdit == pFocus)
	{
		m_SendEdit.PasteSpecial(CF_TEXT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handles AI
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnAiButton() 
{
	int i;
	UINT uiID;
	CPlayer player;
	CNDKMessage msg;
	CNetworkAIDialog dlg;
	CArray <UINT, UINT> *pIDs = NULL;
	CArray <int, int> *pColors = NULL;

	//run dialog
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	//first delete all current AI members
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		//get ID
		uiID = GAME->m_players[i].m_player.getID();
		
		//see if it is an AI
		if(uiID < (UINT) AI_DATABASE->numAI())
		{
			//remove it
			msg = CNDKMessage(S3D_REMOVE_PLAYER);
			msg.SetAt(0, uiID);
			msg.SetAt(1, FALSE);

			//send it
			VIEW->OnMessage(-1, msg);

			//send to all
			VIEW->SendMessageToAllUsers(msg);
		}
	}

	//get pointers
	pIDs = dlg.getIDs();
	pColors = dlg.getColors();

	//add them all
	for(i = 0; i < pIDs->GetSize(); i++)
	{
		//create the player
		player.setAI(TRUE);
		player.setID(pIDs->GetAt(i));
		player.setColor1(pColors->GetAt(i));
		player.setColor2(pColors->GetAt(i));

		//set name
		player.setFirstName(AI_DATABASE->getAIName(pIDs->GetAt(i)));
		player.setLastName("(AI)");

		msg = CNDKMessage(S3D_ADD_PLAYER);
		player.SerializeToMessage(msg);

		//send it
		VIEW->OnMessage(-1, msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handles Connector
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::OnConnectorButton() 
{
	BOOL bFirewall;
	CString strIP;
	CNetworkFirewallDialog fire;
	CNetworkConnectorSetupDialog dlg;

	//get the IP
	strIP = APP->getIPAddress();

	//run the check
	if((0 == strIP.Find("192.168")) || (0 == strIP.Find("10.0")))
	{
		//look in the .ini file
		bFirewall = GetPrivateProfileInt(INI_NETWORK, INI_FIREWALL, FALSE, INI_FILE);

		if((FALSE == bFirewall) && (IDCANCEL == fire.DoModal()))
		{
			return;
		}
	}

	//if we don't have port forwarding on, set the IP and port
	if(FALSE == CONNECTOR->isPortForwarding())
	{
		CONNECTOR->setIP(strIP);
		CONNECTOR->setPort(m_uiPort);
	}

	//run it
	if(IDOK == dlg.DoModal())
	{
		//set the data
		CONNECTOR->initServer(dlg.m_strName, dlg.m_nPrivate, dlg.m_strPassword, m_bReload, GAME->m_bLadder);

		//if we don't have port forwarding on, set the IP and port
		if(FALSE == CONNECTOR->isPortForwarding())
		{
			CONNECTOR->setIP(strIP);
			CONNECTOR->setPort(m_uiPort);
		}

		//handle ladder restrictions
		handleLadder();

		if(FALSE == CONNECTOR->getConnector())
		{
			//we're using the connector
			CONNECTOR->setConnector(TRUE);

			//post it
			CONNECTOR->postServer();
		}
		//already started one so update it
		else
		{
			CONNECTOR->postServer(CONNECTOR_UPDATE);
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////
// handle ladder restrictions
/////////////////////////////////////////////////////////////////////////////
void CNetworkServerDialog::handleLadder()
{
	int i;
	BOOL bTeam = TRUE;
	BOOL bReady = TRUE;

	//determine if everyone is ready
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		if(FALSE == GAME->m_players[i].m_player.getReady())
		{
			bReady = FALSE;
			break;
		}
	}

	//if this is a team game, see if we've got 4 players
	if(TRUE == GAME->m_bTeam)
	{
		bTeam = (4 == GAME->m_players.size());
	}

	//if we have a ladder game, certain rectrictions are in place
	if(TRUE == GAME->m_bLadder)
	{
		//must have at least 3 players
		m_OKButton.EnableWindow(bTeam && bReady && (3 <= GAME->m_players.size()));

		//check variants to see if we have any illegal ones
		if(VARIANTS->m_iVariants & (~VARIANT_ACCEPTABLE_LADDER))
		{
			//reset
			VARIANTS->m_iVariants &= VARIANT_ACCEPTABLE_LADDER;
		}

		//though, if this is a team game, always turn on trades for zero
		if(TRUE == GAME->m_bTeam)
		{
			VARIANTS->m_iVariants |= VARIANT_TRADE_ZERO;
		}

		//ensure that no sevens is only one round
		VARIANTS->m_nNoSevens = 1;
	}
	else
	{
		//enable start button
		m_OKButton.EnableWindow(bReady && bTeam);
	}

	//reload the window
	updateInfoString();
}