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
#include "NetworkView.h"
#include "SettlersView.h"
#include "UndoOne.h"
#include "defineAI.h"
#include "defineNetwork.h"
#include "defineSounds.h"
#include "defineStat.h"
#include "defineVariants.h"
#include "defineIni.h"
#include "defineColors.h"
#include "defineCode.h"

/////////////////////////////////////////////////////////////////////////////
// client and server message - add a new player to the game
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnAddPlayer(CNDKMessage &message, long lUserID)
{
	int i;
	int iSize;
	int iColor;
	int nScore;
	BOOL bReload = FALSE;
	CString strIP, strCompareIP;
	CPlayer player;
	CNetSysMessage sys;
	CNDKMessage msg(S3D_SYS_MESSAGE);

	//determine if we are reloading
	if(NULL != m_pServerDialog)
	{
		bReload = m_pServerDialog->getReload();
	}

	//see if the host will even allow this many players to be added
	if( (TRUE == m_pGame->m_bServer) && (FALSE == m_saveState.isSaved()) && 
		(FALSE == bReload) && ((int) GAME->m_players.size() >= GAME->m_nMaxPlayers))
	{
		sys.m_uiMessage = SYS_MAX_PLAYERS;
		sys.m_strData.Format("%d", GAME->m_nMaxPlayers);
		sys.SerializeToMessage(msg);

		//send to this year
		SendMessageToUser(lUserID, msg);

		//disconnect the user
		DisconnectUser(lUserID, NDKServer_MaxPlayers);

		return;
	}

	//fill the player info
	player.SerializeFromMessage(message);

	//see if this player already exists in the game
	if((NULL != m_pServerDialog) && (FALSE == bReload))
	{
		for(i = 0; i < (int) m_pGame->m_players.size(); i++)
		{
			if(m_pGame->m_players[i].m_player == player)
			{
				//set the message
				sys.m_uiMessage = SYS_DOUBLE_JOIN;
				sys.m_strData = player.getName();
				sys.SerializeToMessage(msg);

				//send it to everyone
				SendMessageToAllUsers(msg);
				OnSysMessage(msg);

				//disconnect the user
				DisconnectUser(lUserID, NDKServer_DoubleJoin);

				return;
			}
		}
	}

	//if we are in saved state, waiting for players to rejoin, we must check this player
	if(TRUE == m_saveState.isSaved())
	{

		//see if we can rejoin the player
		handleInGameRejoin(lUserID, &player);

		return;
	}

	//see if this player makes the drops/quits cutoff
	if((NULL != m_pServerDialog) && (TRUE == CONNECTOR->getConnector()) && 
		(TRUE == m_pGame->m_bUseQuitCutoff) && (FALSE == bReload))
	{
		nScore = CONNECTOR->quitScore(player.getLadderID());

		//if they're over the cutoff, boot them
		if(nScore > m_pGame->m_nQuitCutoff)
		{
			//set the message
			sys.m_uiMessage = SYS_QUIT_CUTOFF;
			sys.m_strData.Format("%s tried to join with a drop/quit percent of %d.", player.getName(), nScore);
			sys.SerializeToMessage(msg);

			//send it to everyone
			SendMessageToAllUsers(msg);
			OnSysMessage(msg);

			//disconnect the user
			DisconnectUser(lUserID, NDKServer_QuitCutoff);

			return;
		}
	}

	//check to see if this is a ladder player (if we're in a ladder game)
	if((TRUE == m_pGame->m_bLadder) && (TRUE == m_pGame->m_bServer))
	{
//ignore in debug mode
#ifndef _DEBUG
		/*
		//get the new player's IP address
		strIP = player.getIP();

		//check IP addresses to see if we have any repeats
		for(i = 0; i < (int) m_pGame->m_players.size(); i++)
		{
			//get this player's IP
			strCompareIP = m_pGame->m_players[i].m_player.getIP();

			//check for exact same
			if(strIP == strCompareIP)
			{
				//set the ID
				sys.m_uiMessage = SYS_LADDER_SAMEIP;
				sys.SerializeToMessage(msg);

				//send to this year
				SendMessageToUser(lUserID, msg);

				//disconnect the user
				DisconnectUser(lUserID, NDKServer_LadderSameIP);

				//send warning to everyone
				sys.m_uiMessage = SYS_LADDER_WARNIP;
				sys.m_strData = player.getName();
				sys.m_strData2 = m_pGame->m_players[i].m_player.getName();
				sys.SerializeToMessage(msg);

				//send to all users
				OnSysMessage(msg);
				SendMessageToAllUsers(msg);

				return;
			}
		}
		*/
#endif	//_DEBUG

		//see if we're already in a game
		if(FALSE == player.getLadder())
		{
			//set the ID
			sys.m_uiMessage = SYS_LADDER_GAME;
			sys.SerializeToMessage(msg);

			//send to this year
			SendMessageToUser(lUserID, msg);

			//disconnect the user
			DisconnectUser(lUserID, NDKServer_LadderOnly);

			return;
		}
	}

	//if we are restarting a network game, check to see that this player is valid
	if(TRUE == bReload)
	{
		if(FALSE == handleReload(lUserID, &player))
		{
			return;
		}
	}

	//play sound
	SOUND(SOUND_PLAYER_JOIN);

	//handle colors
	iColor = handleColors(lUserID, &player, bReload);

	//reserialize to save new color info
	player.SerializeToMessage(message);

	//get size of players
	iSize = m_pGame->m_players.size();

	//add it to the list control if we're not reloading
	if(FALSE == bReload)
	{
		if(NULL != m_pList)
		{
			m_pList->addPlayer(&player, iColor);
		}

		//add to the player games
		m_pGame->m_players.push_back(CPlayerGame());
		m_pGame->m_players[iSize].m_player = player;
		m_pGame->m_players[iSize].m_iColor = iColor;
	}

	//if we are the server, set the map value and update all other players
	if(TRUE == m_pGame->m_bServer)
	{
		//set the map value
		if(FALSE == player.getAI())
		{
			m_mapPlayerID.SetAt(lUserID, player.getID());
			m_mapSocketID.SetAt(player.getID(), lUserID);
		}

		//inform all other players about this new player
		SendMessageToAllUsersExceptFor(lUserID, message);

		//create the sys message
		sys.m_uiMessage = SYS_JOINED_GAME;
		sys.m_strData = player.getName();

		//send it to all
		message.SetId(S3D_SYS_MESSAGE);
		sys.SerializeToMessage(message);

		//send it out
		SendMessageToAllUsers(message);
		OnSysMessage(message);

		if(FALSE == player.getAI())
		{
			message.SetId(S3D_ADD_PLAYER);

			//run through server list of players
			for(i = 0; i < iSize; i++)
			{
				//make sure we don't send ourselves
				if ((m_pGame->m_players[i].m_player.getID() == player.getID()) ||
					((TRUE == bReload) && (FALSE == m_pGame->m_players[i].m_bJoined)))
				{
					continue;
				}

				//send player
				m_pGame->m_players[i].m_player.SerializeToMessage(message);
				SendMessageToUser(lUserID, message);
			}

			//now send the current map info string to the new user
			message.SetId(S3D_MAP_INFO);

			//send it to all
			message.SetAt(0, m_strMapInfo);
			SendMessageToUser(lUserID, message);
		}

		if(NULL != m_pServerDialog)
		{
			//finally check to see if we can continue a saved game
			if(TRUE == bReload)
			{
				m_pServerDialog->checkForContinue();
			}
			//otherwise, update the ladder status
			else
			{
				m_pServerDialog->handleLadder();
			}
		}

		//update the connector if needed
		if(TRUE == CONNECTOR->getConnector())
		{
			CONNECTOR->postServer(CONNECTOR_UPDATE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - remove a player from the game
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnRemovePlayer(CNDKMessage &message)
{
	int i;
	BOOL bReload;
	BOOL bDelete = TRUE;
	UINT uiID;
	vector <CPlayerGame> players;

	//play sound
	SOUND(SOUND_PLAYER_LEAVE);

	//get the player ID
	message.GetAt(0, uiID);
	message.GetAt(1, bReload);

	//remove from list control
	if(NULL != m_pList)
	{
		if((TRUE == m_pGame->m_bServer) && (TRUE == bReload))
		{
			m_pList->updateJoin(uiID, FALSE);

			//mark this player as no longer joined
			for(i = 0; i < (int) m_pGame->m_players.size(); i++)
			{
				if(uiID == m_pGame->m_players[i].m_player.getID())
				{
					m_pGame->m_players[i].m_bJoined = FALSE;
					break;
				}
			}

			//don't delete the player game
			bDelete = FALSE;
		}
		else
		{
			m_pList->removePlayer(uiID);
		}
	}

	//if we're in the game, don't delete the player or we'll crash
	if(TRUE == m_pGame->m_bInit)
	{
		bDelete = FALSE;
	}

	//search through player games and remove this player
	if(TRUE == bDelete)
	{
		for(i = 0; i < (int) m_pGame->m_players.size(); i++)
		{
			if(uiID == m_pGame->m_players[i].m_player.getID())
			{
				//reset the color they were using
				m_pGame->m_bColors[m_pGame->m_players[i].m_player.getGameColor()] = FALSE;
				continue;
			}

			//copy
			players.push_back(m_pGame->m_players[i]);
		}

		//copy back
		m_pGame->m_players = players;
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - got a new color
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnColorAssign(CNDKMessage &message)
{
	int iColor;

	//get the new color
	message.GetAt(0, iColor);

	//set the new color in the game
	m_pGame->m_players[0].m_iColor = iColor;
	m_pGame->m_players[0].m_player.setGameColor(iColor);

	//update the list control
	if(NULL != m_pList)
	{
		m_pList->updatePlayerColor(m_pGame->m_uiPlayerID, iColor);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client and server message - handle a chat message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnChatMessage(CNDKMessage &message, long lUserID)
{
	CNetChatMessage chat;

	//play the sound
	SOUND(SOUND_CHAT);

	//fill it
	chat.SerializeFromMessage(message);

	//let the chat control handle
	if(NULL != m_pChat)
	{
		m_pChat->addMessage(&chat);
	}

	//if this is the server, send it out to everyone but the sender
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - handle a system message from server
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSysMessage(CNDKMessage &message)
{
	CNetSysMessage sys;

	//read it
	sys.SerializeFromMessage(message);

	//see if we even need to show this message
	switch(sys.m_uiMessage)
	{
	case SYS_PORT_PLACE:
	case SYS_SETTLE_PLACE:
	case SYS_ROAD_PLACE:
	case SYS_SHIP_PLACE:
	case SYS_SHIP_MOVE:
	case SYS_CITY_PLACE: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_BUILD)) return; break;
	case SYS_ROLL: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_ROLLS)) return; break;
	case SYS_PIRATE_PLACE:
	case SYS_ROBBER_PLACE: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_ROBBER)) return; break;
	case SYS_STEAL: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_STEAL)) return; break;
	case SYS_TRADE_JUNGLE:
	case SYS_TRADE_TO_BANK: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_TRADE_BANK)) return; break;
	case SYS_BUY_DEV_CARD: 
	case SYS_PLAY_DEV_CARD: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_STEAL)) return; break;
	case SYS_LOSE_CARDS: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_LOSE)) return; break;
	case SYS_END_TURN: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_END)) return; break;
	case SYS_CHITLESS:
	case SYS_GOLD:
	case SYS_TRADE_OFFER:
	case SYS_TRADE: if(FALSE == (m_pGame->m_iSysNotify & SYS_NOTIFY_TRADE_OTHERS)) return; break;
	}

	//let the message or chat control handle it
	if(NULL != m_pMessage)
	{
		m_pMessage->addSysMessage(&sys);
	}
	else if(NULL != m_pChat)
	{
		m_pChat->addSysMessage(&sys);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - update the ping status for a player
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPingUpdate(CNDKMessage &message)
{
	UINT uiID;
	long lSeconds;

	//get from message
	message.GetAt(0, uiID);
	message.GetAt(1, lSeconds);

	//add to our list
	if(NULL != m_pList)
	{
		m_pList->updatePing(uiID, lSeconds);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - prepare for data arrival
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnStartSend(CNDKMessage &message)
{
	int iStart = 1;

	//get variants
	m_pGame->m_variants.SerializeFromMessage(iStart, message);

	//create the status dialog
	createStatus();

	if(NULL != m_pStatusDialog)
	{
		m_pStatusDialog->m_StatusStatic.SetWindowText("Receiving map data...");
	}

	//close down the client dialog box
	if(NULL != m_pClientDialog)
	{
		m_pClientDialog->handleOK();
	}
}

/////////////////////////////////////////////////////////////////////////////
// receive the map data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnMap(CNDKMessage &message)
{
	//load the map to memory
	MAPS->SerializeMapFromMessage(message);
}

/////////////////////////////////////////////////////////////////////////////
// receive game data from server on reload
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnGame(CNDKMessage &message)
{
	m_pGame->SerializeFromMessage(message);
}

/////////////////////////////////////////////////////////////////////////////
// receive undo data from server on reload
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnUndo(CNDKMessage &message)
{
	THISVIEW->getUndoDialog()->SerializeFromMessage(message);
}

/////////////////////////////////////////////////////////////////////////////
// get ready for board data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnStartBoard(CNDKMessage &message)
{
	int i;
	int iStart = 0;
	int iSize;
	int iPlayers;

	message.GetAt(iStart++, iSize);
	message.GetAt(iStart++, iPlayers);

	//resize the player games
	m_pGame->m_players.resize(iPlayers);

	//resize the board
	m_pBoard.SetSize(iSize);

	//array of dev cards
	message.GetAt(iStart++, iSize);

	//resize arrays
	m_iDevCardArray.resize(iSize);
	m_iDevCardSet.resize(iSize);

	//read values
	for(i = 0; i < iSize; i++)
	{
		message.GetAt(iStart++, m_iDevCardSet[i]);
		message.GetAt(iStart++, m_iDevCardArray[i]);
	}

	//hidden tile array
	message.GetAt(iStart++, iSize);
	m_nHiddenArray.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		message.GetAt(iStart++, m_nHiddenArray[i]);
	}

	//hidden volcano array
	message.GetAt(iStart++, iSize);
	m_nHiddenVolcanoArray.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		message.GetAt(iStart++, m_nHiddenVolcanoArray[i]);
	}

	//hidden chit array
	message.GetAt(iStart++, iSize);
	m_nHiddenChitArray.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		message.GetAt(iStart++, m_nHiddenChitArray[i]);
	}

	//chitless chit array
	message.GetAt(iStart++, iSize);
	m_nChitlessChitArray.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		message.GetAt(iStart++, m_nChitlessChitArray[i]);
	}

	//map id
	message.GetAt(iStart++, m_pGame->m_uiMapID);
	message.GetAt(iStart++, m_pGame->m_bLadder);

	if(NULL != m_pStatusDialog)
	{
		m_pStatusDialog->m_StatusStatic.SetWindowText("Receiving board data...");
		m_pStatusDialog->m_Progress.SetRange32(0, iSize + iPlayers);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - received a board piece
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnBoardPiece(CNDKMessage &message)
{
	int iIndex;

	//get the index
	message.GetAt(0, iIndex);

	if(NULL != m_pStatusDialog)
	{
		//update the status dialog
		m_pStatusDialog->m_Progress.SetPos(iIndex);
	}

	//serialize
	m_pBoard[iIndex].SerializeFromMessage(message);
}

/////////////////////////////////////////////////////////////////////////////
// get ready for player data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnStartPlayer(CNDKMessage &/*message*/)
{
	if(NULL != m_pStatusDialog)
	{
		m_pStatusDialog->m_StatusStatic.SetWindowText("Receiving player data...");
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - received a player piece
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPlayerPiece(CNDKMessage &message)
{
	int iBoard;
	int iIndex;

	//get board size
	iBoard = m_pBoard.GetSize();

	//get the index
	message.GetAt(0, iIndex);

	if(NULL != m_pStatusDialog)
	{
		//update the status dialog
		m_pStatusDialog->m_Progress.SetPos(iBoard + iIndex);
	}

	//serialize
	m_pGame->m_players[iIndex].m_player.SerializeFromMessage(message);

	//check to see if our player is here
	if(m_pGame->m_players[iIndex].m_player.getID() == m_pGame->m_uiPlayerID)
	{
		m_pGame->m_iIndex = iIndex;
	}

	//set the color
	m_pGame->m_players[iIndex].m_iColor = m_pGame->m_players[iIndex].m_player.getGameColor();

	//initialize size
	m_pGame->m_players[iIndex].initPlayer(MAP->m_iLandTiles + MAP->m_iSeaTiles);
}

/////////////////////////////////////////////////////////////////////////////
// client message - received a player game
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPlayerGame(CNDKMessage &message)
{
	int iBoard;
	int iIndex;

	//get board size
	iBoard = m_pBoard.GetSize();

	//get the index
	message.GetAt(0, iIndex);

	if(NULL != m_pStatusDialog)
	{
		//update the status dialog
		m_pStatusDialog->m_Progress.SetPos(iBoard + iIndex);
	}

	//serialize
	m_pGame->m_players[iIndex].SerializeFromMessage(message);

	//check to see if our player is here
	if(m_pGame->m_players[iIndex].m_player.getID() == m_pGame->m_uiPlayerID)
	{
		m_pGame->m_iIndex = iIndex;
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - get saved data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSavedData(CNDKMessage &message)
{
	//get message information
	SerializeFromMessage(message);
}

/////////////////////////////////////////////////////////////////////////////
// client message - data has finished arriving
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnEndSend(CNDKMessage &message)
{
	//delete the status dialog
	deleteStatus();

	//load the map
	loadMap(TRUE);

	//get the robber position
	message.GetAt(0, m_iRobber);
	message.GetAt(1, m_iPirate);

	//set the allowed cards if varianted
	if(IS_VARIANT(VARIANT_ALTER_ALLOWED_CARDS))
	{
		setAllowedCards(VARIANTS->m_iAllowedCards);
	}

	//set up ports
	findPorts();

	THISVIEW->initNetwork();
	setNextMode();
	setNetDrawMode(GL_WAIT);

	//create the chat window
	createChatWindow();

	//set the game init
	m_pGame->m_bInit = TRUE;

	//show ourselves
	GetParent()->ShowWindow(SW_MAXIMIZE);
	GetParent()->ShowWindow(SW_SHOW);

	//initialize chat and admin windows
	THISVIEW->initChatAdmin();
}

/////////////////////////////////////////////////////////////////////////////
// client message - data has finished arriving
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnEndSave(CNDKMessage &message)
{
	int i;
	BOOL bRejoin;

	//delete the status dialog
	deleteStatus();

	//load the map
	loadMap(TRUE, TRUE);

	//get the robber position
	message.GetAt(0, m_iRobber);
	message.GetAt(1, m_iPirate);
	message.GetAt(2, bRejoin);

	//fire things up
	THISVIEW->initNetwork();
	setNextMode();
	setNetDrawMode(GL_WAIT);

	//create the chat window
	createChatWindow();

	//update stats
	for(i = 0; i < STAT_SIZE; i++)
	{
		STAT->update(i);
	}

	//set the game init
	m_pGame->m_bInit = TRUE;

	//show ourselves
	GetParent()->ShowWindow(SW_MAXIMIZE);
	GetParent()->ShowWindow(SW_SHOW);

	//reset road lengths
	calculateRoadLengths();

	//toggle actions off
	ACTIONS->ShowWindow(SW_HIDE);

	//see if it's our turn
	if((m_iPlayer == m_pGame->m_iIndex) && (FALSE == bRejoin))
	{
		//toggle controls
		CONTROLS->ShowWindow(SW_SHOW);

		THISVIEW->getControlPanel()->enterTurn();
	}
	else
	{
		//toggle controls
		CONTROLS->ShowWindow(SW_HIDE);
	}

	//initialize chat and admin windows
	THISVIEW->initChatAdmin();
}

/////////////////////////////////////////////////////////////////////////////
// client message - the server has selected a map
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnMapInfo(CNDKMessage &message)
{
	CString str;

	//get the message
	message.GetAt(0, str);

	//set the info text
	if(NULL != m_pInfo)
	{
		m_pInfo->SetWindowText("");
		m_pInfo->addString(str, FALSE, COLOR_LTGRAY);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - set a new net drawing mode
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnDrawMode(CNDKMessage &message)
{
	int iMode;

	//get the mode
	message.GetAt(0, iMode);

	//get whether selection is enabled
	message.GetAt(1, m_bUseSelection);

	//get incrase value
	message.GetAt(2, m_bIncrease);

	//reset selection
	if(TRUE == m_bUseSelection)
	{
		m_iSelection = -1;
	}

	//set the current player to this player
	m_iPlayer = m_pGame->m_iIndex;

	//set it
	setNetDrawMode(iMode);

	//flash the control bar
	SetForegroundWindow();
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - set panel text
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPanelText(CNDKMessage &message, long lUserID, BOOL bIgnore)
{
	int iPlayer;
	long lMessage;
	CString str;

	if(TRUE == m_pGame->m_bServer && FALSE == bIgnore)
	{
		message.GetAt(0, lMessage);
		serverPanelText(lMessage, m_iPlayer, lUserID);
		return;
	}

	//get the message
	message.GetAt(0, str);
	message.GetAt(1, iPlayer);

	//make sure the color is correct
	THISVIEW->getControlPanel()->updateColor(iPlayer);

	//set the text
	THISVIEW->getControlPanel()->setPanelText(str);
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - it's time to place a port
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPortPlace(CNDKMessage &message)
{
	//read the values
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, m_nPortRes);

	//set the drawing mode
	setNetDrawMode(GL_PLACE_PORT);
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone placed a port
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPort(CNDKMessage &message, long lUserID)
{
	int nTile;
	int nSide;
	int nRes;

	//get values
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, nTile);
	message.GetAt(2, nSide);
	message.GetAt(3, nRes);

	//set it
	handleCommonPortPlace(nTile, nSide, nRes);

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_PORT_PLACE, m_iPlayer);

		//continue placing
		serverPortPlace();
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - when someone has placed a settlement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSettlePlace(CNDKMessage &message, long lUserID)
{
	int iPlayer, ID;
	int iT1, iC1;
	
	//get values
	message.GetAt(0, ID);
	message.GetAt(1, m_bIncrease);
	message.GetAt(2, m_iPlayer);

	//decode the ID
	DECODE_NET(ID, iPlayer, iT1, iC1);

	//handle placement
	handleCommonSettlePlace(iPlayer, iT1, iC1);
	
	//if we're the server, send out to all who didn't get it
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_SETTLE_PLACE, iPlayer);

		//set the waiting for road text
		if(FALSE == m_pGame->m_bInGame)
		{
			serverPanelText(S3D_ROAD_PLACE, iPlayer, lUserID);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - when someone has placed a road
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnRoadPlace(CNDKMessage &message, long lUserID)
{
	int iPlayer, ID;
	int iT1, iC1;
	
	//get values
	message.GetAt(0, ID);
	message.GetAt(1, m_iPlayer);

	//decode the ID
	DECODE_NET(ID, iPlayer, iT1, iC1);

	//handle placement
	handleCommonRoadPlace(iPlayer, iT1, iC1);
	
	//if we're the server, send out to all who didn't get it
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_ROAD_PLACE, iPlayer);

		//go to next player
		if(TRUE == m_bTournamentRoads)
		{
			serverThirdRoadPlace();
		}
		else if(FALSE == m_pGame->m_bInGame)
		{
			serverPlayerPlace();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - when someone has placed a ship
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnShipPlace(CNDKMessage &message, long lUserID)
{
	int iPlayer, ID;
	int iT1, iC1;
	
	//get values
	message.GetAt(0, ID);
	message.GetAt(1, m_iPlayer);

	//decode the ID
	DECODE_NET(ID, iPlayer, iT1, iC1);

	//handle placement
	handleCommonShipPlace(iPlayer, iT1, iC1);
	
	//if we're the server, send out to all who didn't get it
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_SHIP_PLACE, iPlayer);

		//go to next player
		if(FALSE == m_pGame->m_bInGame)
		{
			serverPlayerPlace();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - when someone has moved a ship
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnShipMove(CNDKMessage &message, long lUserID)
{
	int iPlayer, ID;
	int iOldTile, iOldSide;
	int iNewTile, iNewSide;
	int nDummy;
	
	//get values
	message.GetAt(0, ID);
	message.GetAt(1, m_iPlayer);

	//decode the ID
	DECODE_SHIP_MOVE(ID, iPlayer, iOldTile, iOldSide, iNewTile, iNewSide, nDummy);

	//handle placement
	handleCommonMoveShip(iPlayer, iOldTile, iOldSide, iNewTile, iNewSide);
	
	//if we're the server, send out to all who didn't get it
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_SHIP_MOVE, iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - when someone has placed a road
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnCityPlace(CNDKMessage &message, long lUserID)
{
	int ID;

	//get values
	message.GetAt(0, ID);
	message.GetAt(1, m_iPlayer);

	//handle placement
	handleCommonCityPlace(m_iPlayer, ID);

	//if we're the server, send out to all who didn't get it
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_CITY_PLACE, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - start up the turn
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnStartTurn(CNDKMessage &message)
{
	BOOL bAuto;

	//set our player correctly
	message.GetAt(0, m_iPlayer);

	//make sure the color is correct
	THISVIEW->getControlPanel()->updateColor(m_iPlayer);

	//toggle actions on
	ACTIONS->ShowWindow(SW_SHOW);

	//set the text
	if(m_iPlayer == m_pGame->m_iIndex)
	{
		//play sound
		SOUND(SOUND_TURN);

		THISVIEW->getControlPanel()->setPanelText("It is your turn.  Click to roll.");

		//get ready to roll
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle controls
		CONTROLS->ShowWindow(SW_SHOW);

		//enable stuff
		THISVIEW->getControlPanel()->EnableTrade();
		THISVIEW->getControlPanel()->EnableDC();
		THISVIEW->getControlPanel()->EnableEndTurn();

		//flash the control bar
		SetForegroundWindow();

		//autoroll if needed
		bAuto = GetPrivateProfileInt(INI_GENERAL, INI_AUTO_ROLL, FALSE, INI_FILE);

		//roll if they want it and they have no soldier cards
		if(TRUE == bAuto && 0 >= m_pGame->m_players[m_iPlayer].m_iCardHeld[CARD_SOLDIER])
		{
			THISVIEW->getControlPanel()->OnRollButton();
		}
	}

	//update the player boxes
	THISVIEW->updateCounts(m_iPlayer);
	THISVIEW->updateCounts(m_iPlayer ? m_iPlayer - 1 : m_iNumPlayers - 1);
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - got a roll message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnRoll(CNDKMessage &message, long lUserID, BOOL bIgnore)
{
	int iRoll;
	int iDie1, iDie2;
	BOOL bFirst;
	CString str;
	CNDKMessage msg;

	//get the roll
	message.GetAt(0, iDie1);
	message.GetAt(1, iDie2);
	message.GetAt(2, m_iPlayer);
	message.GetAt(3, bFirst);

	//get roll
	iRoll = iDie1 + iDie2;

	//play sound
	if(TRUE == bFirst)
	{
		//play the sound
		SOUND(SOUND_ROLL_DICE);
	}

	//send out the system message
	if(TRUE == m_pGame->m_bServer && TRUE == bFirst)
	{
		if(TRUE == NEEDS_AN(iRoll))
		{
			str.Format("an %d.", iRoll);
		}
		else
		{
			str.Format("a %d.", iRoll);
		}
		serverSysMessage(SYS_ROLL, m_iPlayer, str);
	}

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer && FALSE == bIgnore)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);
	}

	//if it's a seven, and we haven't checked for lose cards yet, do so
	if(7 == iRoll && TRUE == bFirst)
	{
		if(TRUE == m_pGame->m_bServer)
		{
			serverWaitForLose(lUserID);
		}

		//handle a seven roll, checking for lose cards
		THISVIEW->getControlPanel()->handleCommonRoll(iDie1, iDie2, bFirst);
	}
	else if(7 != iRoll)
	{
		//wait for gold reports to come in
		if(TRUE == m_pGame->m_bServer)
		{
			serverWaitForGold(lUserID);
		}

		//do stuff
		THISVIEW->getControlPanel()->handleCommonRoll(iDie1, iDie2, bFirst);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - hide the goods data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnHideGoods(CNDKMessage &message, long lUserID)
{
	vector <CString> str;
	list <CUndoNode>::iterator iter;

	//hide the goods
	THISVIEW->getControlPanel()->showGoods(FALSE);

	//toggle actions window
	ACTIONS->ShowWindow(SW_HIDE);

	//set drawing mode
	setNetDrawMode(GL_WAIT);

	//we're in someone's turn
	THISVIEW->getControlPanel()->enterTurn();
	THISVIEW->getControlPanel()->OnGoodsButton();

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//also send a new message saying that we're waiting for this person to finish
		serverPanelText(S3D_START_TURN, m_iPlayer, lUserID);

		//set it to the end
		iter = m_pGame->m_listUndo.end();
		iter--;

		//only add the undo start action if the last undo is not a robber or pirate
		//placement
		if((UNDO_ROBBER_PLACE != (*iter).m_iAction) && (UNDO_PIRATE_PLACE != (*iter).m_iAction))
		{
			//add the undo start action
			addUndo(UNDO_ACTION_START, new CUndoAction(0, TRUE, str));
		}
		else
		{
			//otherwise, we need to add it after the steal message
			m_bActionSteal = TRUE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - hide the volcano data
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnHideVolcano(CNDKMessage &message, long lUserID)
{
	vector <CString> str;

	//add the undo start action
	addUndo(UNDO_ACTION_START, new CUndoAction(0, TRUE, str));

	//do stuff
	THISVIEW->getControlPanel()->showVolcano(FALSE);

	//toggle actions off
	ACTIONS->ShowWindow(SW_HIDE);

	//set drawing mode
	setNetDrawMode(GL_WAIT);

	//we're in someone's turn
	THISVIEW->getControlPanel()->enterTurn();
	THISVIEW->getControlPanel()->OnVolcanoButton();

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//also send a new message saying that we're waiting for this person to finish
		serverPanelText(S3D_START_TURN, m_iPlayer, lUserID);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - show the volcano roll!
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnVolcano(CNDKMessage &message, long lUserID)
{
	int nRoll;
	vector <int> nDestruction;
	vector <CString> strLines;

	//get the roll
	message.GetAt(0, nRoll);

	//prepare stuff
	checkForVolcano();

	//handle the volcanic eruption
	volcanicEruption(nRoll, nDestruction);

	//add the undo
	addUndo(UNDO_VOLCANO, new CUndoVolcano(0, 0, nDestruction, strLines));

	//show the data
	THISVIEW->getControlPanel()->setVolcanoRoll(nRoll);
	THISVIEW->getControlPanel()->showGoods(FALSE);
	THISVIEW->getControlPanel()->showVolcano(TRUE);

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server - someone ended their turn, so go to the next one
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnEndTurn(CNDKMessage &message, long lUserID)
{
	int i;
	int iPlayer = -1;
	UINT uiID;

	//do stuff
	THISVIEW->getControlPanel()->handleCommonEndTurn();

	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		//determine which player is trying to end the turn
		if(-1 == lUserID)
		{
			//the server
			iPlayer = m_pGame->m_iIndex;
		}
		else
		{
			//another player
			m_mapPlayerID.Lookup(lUserID, uiID);

			//search for the player
			for(i = 0; i < m_iNumPlayers; i++)
			{
				if(m_pGame->m_players[i].m_player.getID() == uiID)
				{
					iPlayer = i;
					break;
				}
			}
		}

		//sanity check to make sure this person can actually end the turn
		if(m_iPlayer != iPlayer)
		{
			return;
		}

		SendMessageToAllUsersExceptFor(lUserID, message);

		//send message
		serverSysMessage(SYS_END_TURN, m_iPlayer);

		//go on to the next person
		serverStartTurn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone just moved the robber
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPlaceRobber(CNDKMessage &message, long lUserID)
{
	//get the robber
	message.GetAt(0, m_iRobber);

	//add the undo
	addUndo(UNDO_ROBBER_PLACE, new CUndoOne(m_iPlayer * 10));

	//redraw
	Invalidate();

	//keep sending
	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_ROBBER_PLACE, m_iPlayer);
	}
	//otherwise, place the sound
	else
	{
		SOUND(SOUND_ROBBER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone just moved the pirate
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPlacePirate(CNDKMessage &message, long lUserID)
{
	//get the robber
	message.GetAt(0, m_iPirate);

	//add the undo
	addUndo(UNDO_PIRATE_PLACE, new CUndoOne(m_iPlayer * 10));

	//redraw
	Invalidate();

	//keep sending
	//if we're the server, keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_PIRATE_PLACE, m_iPlayer);
	}
	//otherwise, place the sound
	else
	{
		SOUND(SOUND_PIRATE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone stole a card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSteal(CNDKMessage &message, long lUserID)
{
	int iRes;
	int iPlayer;
	BOOL bPirate;
	CString str;
	vector <CString> strBlank;
	CNetSysMessage sys;
		
	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iPlayer);
	message.GetAt(2, iRes);
	message.GetAt(3, bPirate);

	//handle it
	handleCommonSteal(iPlayer, iRes, bPirate);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		if(iRes != 5)
		{
			str.Format("1 card from %s.", m_pGame->m_players[iPlayer].m_player.getName());
		}
		else
		{
			str = "nothing.  How refreshing!";
		}
		serverSysMessage(SYS_STEAL, m_iPlayer, str);

		//see if we need to add the action start message
		if(TRUE == m_bActionSteal)
		{
			//add the undo start action (necessary for rejoin resynchs)
			addUndo(UNDO_ACTION_START, new CUndoAction(0, TRUE, strBlank));
			
			//reset it
			m_bActionSteal = FALSE;
		}
	}

	//if this was the victim, tell them
	if((5 != iRes) && (iPlayer == m_pGame->m_iIndex))
	{
		str.Format("You lost 1 %s.", getResName(iRes, TRUE));

		//create the message
		sys.m_uiMessage = SYS_STEAL_REPORT;
		sys.m_strData = str;
		sys.m_iColor = m_pGame->m_players[m_iPlayer].m_iColor;

		//add it
		if(NULL != m_pMessage)
		{
			m_pMessage->addSysMessage(&sys);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone traded with the bank
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTradeToBank(CNDKMessage &message, long lUserID)
{
	int iBank[3];
	int iToBank[5];
	int iFromBank[5];
	CString str;

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iToBank, 5 * sizeof(int));
	message.GetAt(2, iFromBank, 5 * sizeof(int));
	message.GetAt(3, iBank, 3 * sizeof(int));

	//handle it
	handleCommonTradeToBank(m_iPlayer, iToBank, iFromBank, iBank, TRUE);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a notification message
		serverSysMessage(SYS_TRADE_TO_BANK, m_iPlayer, getTradeToBankString(iToBank, iFromBank));
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone traded jungles
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTradeJungle(CNDKMessage &message, long lUserID)
{
	int nJungles;
	int nFromBank[5];

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, nJungles);
	message.GetAt(2, nFromBank, 5 * sizeof(int));

	//handle it
	handleCommonTradeToJungle(m_iPlayer, nJungles, nFromBank);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a notification message
		serverSysMessage(SYS_TRADE_JUNGLE, m_iPlayer, getTradeJungleString(nJungles, nFromBank));
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone bought a dev card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnBuyDevCard(CNDKMessage &message, long lUserID)
{
	int iCard;

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iCard);

	//must increment the dev card index to keep up
	m_iDevCardIndex++;

	//handle it
	handleCommonBuyDevCard(m_iPlayer, iCard);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_BUY_DEV_CARD, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone played a dev card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPlayDevCard(CNDKMessage &message, long lUserID)
{
	int iType;
	int iCard1, iCard2;

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iType);
	message.GetAt(2, iCard1);
	message.GetAt(3, iCard2);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a system message
		serverSysMessage(SYS_PLAY_DEV_CARD, m_iPlayer, getDevName(iType));
	}

	//handle it
	switch(iType)
	{
	case CARD_VICTORY: handleCommonPlayVictory(m_iPlayer); break;
	case CARD_MONOPOLY: handleCommonPlayMonopoly(m_iPlayer, iCard1); break;
	case CARD_ROAD_BUILD: handleCommonPlayRoad(m_iPlayer); break;
	case CARD_YEAR_PLENTY: handleCommonPlayYear(m_iPlayer, iCard1, iCard2); break;
	case CARD_SOLDIER: handleCommonPlaySoldier(m_iPlayer); break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// server only message - handle someone telling us about losing cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnWaitForLose(CNDKMessage &message)
{
	int i;
	int iAI = 0;
	int iDie1, iDie2;
	int iPlayer;
	BOOL bWait;
	CNDKMessage msg;

	//get the data
	message.GetAt(0, iPlayer);
	message.GetAt(1, bWait);

	//make sure we have a valid array
	if((int) m_bWaitForLose.size() != m_iNumPlayers)
	{
		m_bWaitForLose.resize(0);
		m_bWaitForLose.resize(m_pGame->m_players.size(), FALSE);

		//count the number of AI players in the game and set their flag to done
		for(i = 0; i < m_iNumPlayers; i++)
		{
			if(TRUE == m_pGame->m_players[i].m_player.getAI())
			{
				iAI++;
				m_bWaitForLose[i] = TRUE;
			}
		}
	}

	//if we're waiting, tell everyone
	if(TRUE == bWait)
	{
		serverPanelText(S3D_WAIT_FOR_LOSE, 0, -2);

		//we're finished
		return;
	}
	//otherwise, set their finished flag
	else
	{
		m_bWaitForLose[iPlayer] = TRUE;
	}

	//now scan the players to see if everyone is finished
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//if we find someone who hasn't finished, keep waiting
		if(FALSE == m_bWaitForLose[i])
		{
			return;
		}
	}

	//reset the wait for lose cards array
	m_bWaitForLose.resize(0);

	//handle card loss for AI
	for(i = 0; i < m_iNumPlayers; i++)
	{
		if(TRUE == m_pGame->m_players[i].m_player.getAI())
		{
			//determine if they went over
			if(m_pGame->m_players[i].totalCards() > m_iAllowedCards)
			{
				if(AI_FINISHED > serverAI(AI_STATE_LOSE_CARDS, i))
				{
					return;
				}
			}
		}
	}

	//send the continue message
	msg.SetId(S3D_CONTINUE_7);
	if(m_lWaitID != -1)
	{
		SendMessageToUser(m_lWaitID, msg);
	}

	//we're done, tell everyone to handle the 7 roll normally
	THISVIEW->getControlPanel()->getLastRoll(iDie1, iDie2);
	msg = CNDKMessage(S3D_ROLL);
	msg.SetAt(0, iDie1);
	msg.SetAt(1, iDie2);
	msg.SetAt(2, m_iPlayer);
	msg.SetAt(3, FALSE);
	
	//send this message to everyone else
	SendMessageToAllUsersExceptFor(m_lWaitID, msg);

	//handle server stuff
	if(-1 == m_lWaitID)
	{
		OnContinue7(msg);
	}
	else
	{
		OnRoll(msg, -1, TRUE);
	}

	//handle waiting for rejoins
	if(TRUE == isWaiting())
	{
		m_saveState.resetState();
		m_saveState.saveState();
		THISVIEW->getControlPanel()->setPanelText("Waiting for players to rejoin.");
	}
}

/////////////////////////////////////////////////////////////////////////////
// server only message - handle someone telling us about getting gold
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnWaitForGold(CNDKMessage &message)
{
	int i;
	int iPlayer;
	int nGold;
	BOOL bSend = FALSE;
	CNDKMessage msg;

	//get the data
	message.GetAt(0, iPlayer);
	message.GetAt(1, nGold);

	//make sure we have a valid array
	if((int) m_nWaitForGold.size() != m_iNumPlayers)
	{
		m_nWaitForGold.resize(0);
		m_nWaitForGold.resize(m_pGame->m_players.size(), -1);

		//reset the gold index to the current player
		m_nGoldIndex = m_iPlayer;
		m_nGoldCount = 0;

		//HANDLE AI PLAYERS HERE
	}

	//set the flag
	m_nWaitForGold[iPlayer] = nGold;

	//now scan the players to see if everyone is finished
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//if we find someone who hasn't finished, keep waiting
		if(-1 == m_nWaitForGold[i])
		{
			return;
		}
		else if(0 < m_nWaitForGold[i])
		{
			bSend = TRUE;
		}
	}

	//if we made it here, it means we're done and need to send the gold request
	//to those that need it
	if(TRUE == bSend)
	{
		//run the gold message system
		incrementGold();

		//send out the server message
		serverPanelText(S3D_WAIT_FOR_GOLD, 0, -2);
	}
	else
	{
		//it's someone else's turn
		if(-1 != m_lWaitID)
		{
			//send the continue message
			msg.SetId(S3D_CONTINUE_ROLL);
			SendMessageToUser(m_lWaitID, msg);
		}
		//it's the servers turn
		else
		{
			OnContinueRoll(msg);
		}

		//reset the counter
		m_nWaitForGold.resize(0);
		m_nWaitForGold.clear();
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - continue with a 7 roll after waiting for lost cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnContinue7(CNDKMessage &/*message*/)
{
	THISVIEW->getControlPanel()->setTurn();

	//continue processing
	handleRoll(7);

	//if this is an AI player, handle the rest of the turn
	if(TRUE == m_pGame->m_players[m_iPlayer].m_player.getAI())
	{
		serverAITurn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - continue with a roll after waiting for gold cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnContinueRoll(CNDKMessage &/*message*/)
{
	//allow them to continue
	THISVIEW->getControlPanel()->continueTurn();
}

/////////////////////////////////////////////////////////////////////////////
// client/server message - someone lost cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnLoseCards(CNDKMessage &message, long lUserID)
{
	int iPlayer;
	int iRes[5];
	CNDKMessage msg(S3D_WAIT_FOR_LOSE);

	//get data
	message.GetAt(0, iPlayer);
	message.GetAt(1, iRes, sizeof(int) * 5);

	//handle it
	handleCommonLoseCards(iPlayer, iRes);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send a notification
		serverSysMessage(SYS_LOSE_CARDS, iPlayer, getLoseString(iRes));

		//report this person as ready to continue to ourselves
		msg.SetAt(0, iPlayer);
		msg.SetAt(1, FALSE);

		//handle it
		OnWaitForLose(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - this player gets gold!
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnInitGold(CNDKMessage &message)
{
	int iGold;
	int iIndex;

	//get the index and gold to receive
	message.GetAt(0, iIndex);
	message.GetAt(1, iGold);

	//reset the gold players array
	::ZeroMemory(m_nGoldPlayers, sizeof(m_nGoldPlayers));

	//set it in the array
	m_nGoldPlayers[iIndex] = iGold;

	//disable the goods button in the control panel so they can't click it
	//before the timer fires
	ACTIONS->Goods().EnableWindow(FALSE);

	//set the timer
	m_uiTimer = SetTimer(TIMER_GOLD, 1000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// server message - someone got gold, so tell everyone else and continue
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSelectGold(CNDKMessage &message, long lUserID)
{
	int i;
	int iIndex;
	int iCount = 0;
	CString str, strTemp;

	//get the player and what they got
	message.GetAt(0, iIndex);
	message.GetAt(1, m_iGainedCards, sizeof(m_iGainedCards));

	//if it's not from the server, handle it
	if((FALSE == m_pGame->m_bServer) || (-1 != lUserID))
	{
		handleCommonGold();
	}

	//send it to everyone else but the user
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//go through the resources
		for(i = 0; i < 5; i++)
		{
			if(0 < m_iGainedCards[iIndex][i])
			{
				strTemp.Format("%d %s, ", m_iGainedCards[iIndex][i], VIEW->getResName(i));
				iCount += m_iGainedCards[iIndex][i];
				str += strTemp;
			}
		}

		//remove the last comma
		if(0 < iCount)
		{
			//remove the last comma
			str = str.Left(str.GetLength() - 2);
		}

		//send the gold message
		serverSysMessage(SYS_GOLD, iIndex, str);

		//continue golding
		incrementGold();
	}
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - someone's offering a trade
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTradeOffer(CNDKMessage &message, long lUserID)
{
	int i, j;
	int iSize;
	int iTo[5];
	int iFor[5];
	int iCombined[11];
	int iCopy[11];
	long lUser;
	CString str;
	BOOL bAI;
	BOOL bSelf = FALSE;
	UINT uiPlayer;
	CLongArray lArray;
	CArray <int, int> iAIArray;

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iTo, sizeof(int) * 5);
	message.GetAt(2, iFor, sizeof(int) * 5);
	message.GetAt(3, m_uiTradeID);

	//create the combined array for AI players
	for(i = 0; i < 10; i++)
	{
		if(i < 5)
		{
			iCombined[i] = iTo[i];
		}
		else
		{
			iCombined[i] = iFor[i - 5];
		}
	}

	//set the player who wants the trade
	iCombined[10] = m_iPlayer;

	//if we're the server, we need to create the array of players getting this notification
	if(TRUE == m_pGame->m_bServer)
	{
		message.GetAt(4, iSize);

		//set the person that requested the trade
		m_lTradeID = lUserID;

		//read players
		for(i = 0; i < iSize; i++)
		{
			message.GetAt(5 + i, uiPlayer);

			//see if the server is one of the recipients of the trade offer
			if(m_pGame->m_uiPlayerID == uiPlayer)
			{
				bSelf = TRUE;
			}
			//lookup the user (if it's not ourselves)
			else
			{
				//first see if it's an AI
				for(j = 0, bAI = FALSE; j < m_iNumPlayers; j++)
				{
					if((m_pGame->m_players[j].m_player.getID() == uiPlayer) &&
						(TRUE == m_pGame->m_players[j].m_player.getAI()))
					{
						iAIArray.Add(j);
						bAI = TRUE;
						break;
					}
				}
				if(FALSE == bAI)
				{
					//find the socket
					m_mapSocketID.Lookup(uiPlayer, lUser);

					//add it to the array of those being sent the message
					lArray.Add(lUser);
				}
			}

			//add to the name str
			str += m_pGame->getNameByID(uiPlayer);
			str += ", ";
		}

		//send it out
		if(0 < lArray.GetSize())
		{
			SendMessageToSomeUsers(lArray, message);
		}

		//tell everyone about the trade offer
		if(iSize == (int) m_pGame->m_players.size() - 1)
		{
			str = "everyone";
		}
		else
		{
			str = str.Left(str.GetLength() - 2);
		}
		str += ".";
		serverSysMessage(SYS_TRADE_OFFER, m_iPlayer, str);

		//handle any AI players
		for(i = 0; i < iAIArray.GetSize(); i++)
		{
			//use a copy of the original to preserve it in case the AI counters
			memcpy(iCopy, iCombined, sizeof(iCopy));

			if(AI_FINISHED > serverAI(AI_STATE_TRADE_OFFER, iAIArray[i], iCopy))
			{
				return;
			}
		}
	}
	
	if(FALSE == m_pGame->m_bServer || TRUE == bSelf)
	{
		handleTradeOffer(iTo, iFor, m_uiTradeID);
	}
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - someone's replying to a trade
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTradeReply(CNDKMessage &message)
{
	int iPlayer;
	UINT uiID;
	short shStatus;

	//get data
	message.GetAt(0, iPlayer);
	message.GetAt(1, uiID);
	message.GetAt(2, shStatus);

	//send to the right person
	if(TRUE == m_pGame->m_bServer)
	{
		//send it to the right person
		if(-1 != m_lTradeID)
		{
			SendMessageToUser(m_lTradeID, message);
			return;
		}
	}

	//handle it
	handleTradeReply(iPlayer, uiID, shStatus);
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - someone's making a counteroffer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTradeCounter(CNDKMessage &message)
{
	int iPlayer;
	int iTo[5];
	int iFor[5];
	UINT uiID;

	//get data
	message.GetAt(0, iPlayer);
	message.GetAt(1, iTo, sizeof(int) * 5);
	message.GetAt(2, iFor, sizeof(int) * 5);
	message.GetAt(3, uiID);

	//send to the right person
	if(TRUE == m_pGame->m_bServer)
	{
		//send it to the right person
		if(-1 != m_lTradeID)
		{
			SendMessageToUser(m_lTradeID, message);
			return;
		}
	}

	//handle it
	handleTradeReply(iPlayer, uiID, TRADE_COFFER, iTo, iFor);
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - a trade has gone through
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTrade(CNDKMessage &message, long lUserID)
{
	int iPlayer;
	int iPartner;
	int iTo[5];
	int iFor[5];

	//get data
	message.GetAt(0, iPlayer);
	message.GetAt(1, iPartner);
	message.GetAt(2, iTo, sizeof(int) * 5);
	message.GetAt(3, iFor, sizeof(int) * 5);
	
	//handle it
	handleCommonTrade(iPlayer, iPartner, iTo, iFor);

	//continue telling others
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);
		
		//send a system message
		serverSysMessage(SYS_TRADE, m_iPlayer, getTradeString(iPartner, iTo, iFor));
	}
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - someone has moved a chitless tile
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnChitless(CNDKMessage &message, long lUserID)
{
	int nTile1, nTile2;

	//decode message
	message.GetAt(0, nTile1);
	message.GetAt(1, nTile2);

	//handle it
	handleCommonChitless(nTile1, nTile2);

	//continue telling others
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);
		
		//send a system message
		serverSysMessage(SYS_CHITLESS, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// server/client message - someone has won the game
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnWin(CNDKMessage &message, long lUserID)
{
	int iPlayer;

	//get data
	message.GetAt(0, iPlayer);

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//handle the last time update
		serverTimeUpdate();

		//remove the saved in progress game
		THISVIEW->removeSave();
	}

	//handle win
	handleCommonWin(iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// client message - the server is updating someone's turn time
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTimeUpdate(CNDKMessage &message)
{
	int iPlayer;
	int iSeconds;

	//get data
	message.GetAt(0, iPlayer);
	message.GetAt(1, iSeconds);

	//update time
	handleCommonTimeUpdate(iPlayer, iSeconds);
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - check for compatible versions
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnVersionCheck(CNDKMessage &message, long lUserId)
{
	CString str;
	CString strVer;
	CPlayer player;
	CNDKMessage msg(S3D_VERSION_CHECK);

	//get the version number
	message.GetAt(0, strVer);

	//if we're the server, it means someone has a wrong version
	if(TRUE == m_pGame->m_bServer)
	{
		//kill the user
		DisconnectUser(lUserId, NDKServer_VersionMismatch);

		//compare version numbers (only if we're not waiting for players to rejoin)
		if((strVer > (CString) GAME_VER) && (FALSE == m_saveState.isSaved()))
		{
			str.Format("Someone is trying to connect with Sea3D version %s, which is newer than your version %s.\nIt is recommended you visit http://www.settlers3d.net and download the latest version.", strVer, GAME_VER);
			MessageBox(str, "Incompatible Sea3D versions!");
		}
	}
	else
	{
		//compare versions
		if(strVer != (CString) GAME_VER)
		{
			//tell the server to kill us
			msg.SetAt(0, (CString) GAME_VER);

			//send the message telling the server
			SendMessageToServer(msg);

			str.Format("The server is using Sea3D version %s, and you are using version %s.\nOne or both of you need to download the latest version from http://www.settlers3d.net in order to play.", strVer, GAME_VER);
			MessageBox(str, "Incompatible Sea3D versions!");
		}
		else
		{
			//we're good and connected, so send player data
			player = m_pGame->m_players[0].m_player;

			//set the ID
			msg.SetId(S3D_ADD_PLAYER);

			//translate it
			player.SerializeToMessage(msg);

			//send it
			SendMessageToServer(msg);
		}

		//kill the timer so we don't auto-disconnect
		if(NULL != m_pClientDialog)
		{
			m_pClientDialog->killTimer();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - we've had to quit because someone dropped or quit
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTerminate()
{
	//set termination text
	THISVIEW->getControlPanel()->setPanelText("This game has been terminated.");

	//set draw mode
	m_iMode = GL_WAIT;

	//hide everything if we're in a game
	if(TRUE == m_pGame->m_bInit)
	{
		THISVIEW->showBuildOptions(FALSE);

		//toggle controls
		CONTROLS->setAnimate(FALSE);
		CONTROLS->ShowWindow(SW_HIDE);

		//toggle actions window
		ACTIONS->setAnimate(FALSE);
		ACTIONS->ShowWindow(SW_HIDE);
	}

	//close out any open dialog boxes
	if(NULL != m_pDialog2)
	{
		m_pDialog2->EndDialog(IDABORT);
	}

	if(NULL != m_pDialog)
	{
		m_pDialog->EndDialog(IDABORT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - undo a build placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnUndoBuild(CNDKMessage &message, long lUserID)
{
	int iOther, iOther2, iOther3;
	int iType;
	int ID;
	int iSys = 0;

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iType);
	message.GetAt(2, ID);
	message.GetAt(3, iOther);
	message.GetAt(4, iOther2);
	message.GetAt(5, iOther3);

	//handle it
	switch(iType)
	{
	case GL_MOVE_SHIP: handleCommonUndoShipMove(ID, iOther, iOther2, iOther3); iSys = SYS_UNDO_SHIP_MOVE; break;
	case GL_PLACE_ROAD: handleCommonUndoRoad(ID, iOther, iOther2); iSys = SYS_UNDO_ROAD; break;
	case GL_PLACE_SHIP: handleCommonUndoShip(ID, iOther, iOther2); iSys = SYS_UNDO_SHIP; break;
	case GL_PLACE_CITY: handleCommonUndoCity(ID); iSys = SYS_UNDO_CITY; break;
	case GL_PLACE_SETTLEMENT: handleCommonUndoSettlement(ID); iSys = SYS_UNDO_SETTLEMENT; break;
	}

	//clear it off the history
	THISVIEW->getUndoDialog()->removeItem();

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send system message
		serverSysMessage(iSys, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - undo an autotrade
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnUndoAutoTrade(CNDKMessage &message, long lUserID)
{
	int iTo[5];
	int iFrom[5];
	int iBank[3];

	//get data
	message.GetAt(0, m_iPlayer);
	message.GetAt(1, iTo, 5 * sizeof(int));
	message.GetAt(2, iFrom, 5 * sizeof(int));
	message.GetAt(3, iBank, 3 * sizeof(int));

	//handle it
	handleCommonUndoTrade(-2, iTo, iFrom, iBank);

	//clear it off the history
	THISVIEW->getUndoDialog()->removeItem();

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//send system message
		serverSysMessage(SYS_UNDO_AUTOTRADE, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client message - start a special build phase
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSpecialStart(CNDKMessage &message)
{
	CString str;

	//get the current player
	message.GetAt(0, m_iPlayer);

	//add the undo for the player's actions
	addUndo(UNDO_SPECIAL_BUILD, new CUndoOne(m_iPlayer));

	//play the special build transition sound
	SOUND(SOUND_SPECIAL_BUILD);

	//see if we are the actual player in question
	if(m_pGame->m_iIndex == m_iPlayer)
	{
		//turn on the special build phase
		m_bSpecialBuild = TRUE;

		//toggle controls
		CONTROLS->ShowWindow(SW_SHOW);

		//show build options
		THISVIEW->getControlPanel()->handleCommonSpecialBuild("Your Special Build Phase!");

		//flash the window
		SetForegroundWindow();
	}
	//otherwise, set the text that we are waiting for this person
	else
	{
		//format the text
		str.Format("Waiting for %s to finish Special Build Phase.", m_pGame->m_players[m_iPlayer].m_player.getFirstName());

		//set the text
		THISVIEW->getControlPanel()->setPanelText(str);
	}

	//update player displays
	VIEW->updatePlayerDisplay();
}

/////////////////////////////////////////////////////////////////////////////
// server message - continue a special build phase
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSpecialNext(CNDKMessage &/*message*/)
{
	serverSpecialBuild();
}

/////////////////////////////////////////////////////////////////////////////
// client message - the special build phase has ended
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSpecialEnd(CNDKMessage &/*message*/)
{
	//add it to the undo list
	addUndo(UNDO_SPECIAL_BUILD_END, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - save the current state
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnSaveState(CNDKMessage &/*message*/)
{
	//close out any open dialog boxes
	if(NULL != m_pDialog2)
	{
		m_pDialog2->EndDialog(IDABORT);
		m_pDialog2 = NULL;
	}

	if((NULL != m_pDialog) && (TRUE == m_bCanClose))
	{
		//copy the current saved state
		m_saveState = m_saveDialog;

		m_pDialog->SendMessage(WM_COMMAND, IDCANCEL, 0);
		m_pDialog = NULL;

		//hide all
		m_saveState.hideAll();
	}
	else
	{
		m_saveState.saveState();
	}

	//set the title text
	THISVIEW->getControlPanel()->setPanelText("Waiting for players to rejoin.");
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - restore the current state
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnRestoreState(CNDKMessage &/*message*/)
{
	m_saveState.restoreState();
}

/////////////////////////////////////////////////////////////////////////////
// client message - resynch after a rejoin
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnRejoinSynch(CNDKMessage &message)
{
	int nResynch;
	int nData1, nData2;

	//get data
	message.GetAt(0, nResynch);
	message.GetAt(1, nData1);
	message.GetAt(2, nData2);

	//hide all to start
	CONTROLS->ShowWindow(SW_HIDE);
	ACTIONS->ShowWindow(SW_HIDE); 
	VIEW->showBuildOptions(FALSE);

	//if we have to save the state, it gets ugly because someone else dropped before
	//we rejoined
	if(RESYNCH_SAVE_STATE & nResynch)
	{
		//set the messages text
		THISVIEW->getControlPanel()->setPanelText("Waiting for players to rejoin.");

		//set the resynch state for when we get the all clear
		m_saveState.saveState();
		m_saveState.saveResynch(nResynch, nData1, nData2);

		return;
	}

	//run the resynch
	clientResynch(nResynch, nData1, nData2);
}

/////////////////////////////////////////////////////////////////////////////
// client / server message - someone has altered their ready state
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnReadyState(CNDKMessage &message, long lUserID)
{
	int i;
	UINT uiID;
	BOOL bReady;

	//get variables
	message.GetAt(0, uiID);
	message.GetAt(1, bReady);

	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		if(uiID == m_pGame->m_players[i].m_player.getID())
		{
			m_pGame->m_players[i].m_player.setReady(bReady);
			break;
		}
	}

	//set the player's ready state
	GAME->m_players[GAME->m_iIndex].m_player.setReady(TRUE);

	//update the player list if we have it
	if(NULL != m_pList)
	{
		m_pList->updateReady(uiID, bReady);
	}

	//keep sending
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsersExceptFor(lUserID, message);

		//see if we can continue
		if((NULL != m_pServerDialog))
		{
			m_pServerDialog->handleLadder();
		}
	}
	else if((FALSE == bReady) && (uiID == m_pGame->m_players[m_pGame->m_iIndex].m_player.getID()))
	{
		//show the ready
		if(NULL != m_pClientDialog)
		{
			m_pClientDialog->m_ReadyButton.ShowWindow(SW_SHOW);
		}
	}
}