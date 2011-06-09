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
#include "ColorDatabase.h"
#include "SettlersView.h"
#include "defineNetwork.h"
#include "defineSounds.h"
#include "defineVariants.h"
#include "defineIni.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkView

IMPLEMENT_DYNCREATE(CNetworkView, CView)

CNetworkView::CNetworkView()
{
	m_lWaitID = -1;
	m_lTradeID = -1;

	//no ping
	m_uiPing = (UINT) -1;

	//current trade ID
	m_uiTradeID = 0;

	//no gold player yet
	m_nGoldIndex = -1;
	m_nGoldCount = 0;

	m_bCanClose = FALSE;
	m_bActionSteal = FALSE;

	m_pList = NULL;
	m_pChat = NULL;
	m_pSend = NULL;
	m_pInfo = NULL;
	m_pTrade = NULL;
	m_pDialog = NULL;
	m_pDialog2 = NULL;
	m_pMessage = NULL;
	m_pChatWindow = NULL;
	m_pClientDialog = NULL;
	m_pServerDialog = NULL;
	m_pStatusDialog = NULL;
	m_pTradeOfferDialog = NULL;
}

CNetworkView::~CNetworkView()
{
	//clean up status
	deleteStatus();
	deleteChatWindow();
}


BEGIN_MESSAGE_MAP(CNetworkView, CGLView)
	//{{AFX_MSG_MAP(CNetworkView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkView drawing

void CNetworkView::OnDraw(CDC* /*pDC*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CNetworkView diagnostics

#ifdef _DEBUG
void CNetworkView::AssertValid() const
{
	CGLView::AssertValid();
}

void CNetworkView::Dump(CDumpContext& dc) const
{
	CGLView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNetworkView message handlers

/////////////////////////////////////////////////////////////////////////////
// server override - should we accept the connection?
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkView::OnIsConnectionAccepted()
{
	long lUsers;
	
	//see how many users there are
	lUsers = GetNbUsers();

	//we are still OK if there are 5 or less
	return (lUsers <= 5);
}

/////////////////////////////////////////////////////////////////////////////
// server override - handle a new connection
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnConnect(long lUserId)
{
	CNetSysMessage sys;
	CNDKMessage msg(S3D_SYS_MESSAGE);

	//see if the game is over
	if(GAME_COMPLETE == m_pGame->m_iState)
	{
		//set the id
		sys.m_uiMessage = SYS_GAME_OVER;
		sys.SerializeToMessage(msg);

		//send it
		SendMessageToUser(lUserId, msg);

		//disconnect the user
		DisconnectUser(lUserId, NDKServer_GameOver);

		return;
	}

	//see if we're already in a game
	if(TRUE == m_pGame->m_bInit)
	{
		//if we are not saving the state, it means that the game is full
		if(FALSE == m_saveState.isSaved())
		{
			//set the ID
			sys.m_uiMessage = SYS_GAME_FULL;
			sys.SerializeToMessage(msg);

			//send to this year
			SendMessageToUser(lUserId, msg);

			//disconnect the user
			DisconnectUser(lUserId, NDKServer_GameFull);

			return;
		}
	}

	//put in a value into the ID map, which will be updated later as soon
	//as the player data arrives.
	m_mapPlayerID.SetAt(lUserId, (UINT) 0);

	//send a version check message to the client
	msg = CNDKMessage(S3D_VERSION_CHECK);
	msg.SetAt(0, (CString) GAME_VER);
	
	//send to client
	SendMessageToUser(lUserId, msg);
}

/////////////////////////////////////////////////////////////////////////////
// server override - handle a disconnection
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnDisconnect(long lUserId, NDKServerDisconnection disconnectionType)
{
	int i, j;
	int nPlayer = 0;
	int iQuit = 0;
	UINT uiID;
	BOOL bReload = FALSE;
	CString str;
	CNDKMessage msg;
	CNetSysMessage sys;

	//see if they're just getting dumped
	if( (NDKServer_GameFull == disconnectionType) || 
		(NDKServer_GameOver == disconnectionType) || 
		(NDKServer_LadderOnly == disconnectionType) ||
		(NDKServer_LadderSameIP == disconnectionType) ||
		(NDKServer_MaxPlayers == disconnectionType) ||
		(NDKServer_QuitCutoff == disconnectionType) ||
		(NDKServer_DoubleJoin == disconnectionType))
	{
		return;
	}

	//lookup quitting player
	m_mapPlayerID.Lookup(lUserId, uiID);

	//post system message based on what happened to this user
	msg.SetId(S3D_SYS_MESSAGE);

	//get the specific quit string
	switch (disconnectionType)
	{
	case NDKServer_NormalDisconnection:	iQuit = SYS_BOOTED_FROM_GAME; break;
	case NDKServer_ClientCloseConnection: iQuit = SYS_LEFT_GAME; break;
    case NDKServer_ErrorSendingMessage:	iQuit = SYS_SEND_ERROR; break;
	case NDKServer_ErrorReceivingMessage: iQuit = SYS_RECV_ERROR; break;
	case NDKServer_InvalidPlayer: iQuit = SYS_INVALID_PLAYER; break;
	case NDKServer_VersionMismatch: iQuit = SYS_VERSION_MISMATCH; break;
	case NDKServer_Dropped: iQuit = SYS_DROPPED; break;
	default: break;
	}

	//add the quit id
	sys.m_uiMessage = iQuit;
	sys.m_strData = m_pGame->getNameByID(uiID);
	sys.SerializeToMessage(msg);

	//send to all users
	OnSysMessage(msg);
	SendMessageToAllUsers(msg);

	//remove the player from the ID map
	m_mapPlayerID.RemoveKey(lUserId);
	m_mapSocketID.RemoveKey(uiID);

	//if we have already started the game, we must pause and wait for rejoins
	if((TRUE == m_pGame->m_bInit))
	{
		if(GAME_COMPLETE != m_pGame->m_iState)
		{
			//determine which player dropped
			for(i = 0; i < m_iNumPlayers; i++)
			{
				if(m_pGame->m_players[i].m_player.getID() == uiID)
				{
					nPlayer = i;
					break;
				}
			}

			//if this is a ladder game, and this is the first person to drop, they
			//get a drop/quit mark in their record for this game until they rejoin
			if((TRUE == m_pGame->m_bLadder) && (FALSE == m_saveState.isSaved()))
			{
				CONNECTOR->quitPlayer(nPlayer, (SYS_LEFT_GAME == iQuit));
			}

			//set that players joined flag
			m_pGame->m_players[nPlayer].m_bJoined = FALSE;

			//save state
			serverSaveState();

			//see if the connector needs to be updated
			if(TRUE == CONNECTOR->getConnector())
			{
				//set the starting text
				str = "Waiting for ";

				//create the new name string
				for(i = 0, j = 0; i < m_iNumPlayers; i++)
				{
					if(FALSE == m_pGame->m_players[i].m_bJoined)
					{
						//add initial comma if needed
						if(0 != j)
						{
							str += ", ";
						}

						//add player's name
						str += m_pGame->m_players[i].m_player.getName();

						//increment j
						j++;
					}
				}

				//set the final bit
				str += " to rejoin";

				//repost to the connector with the update message
				CONNECTOR->setName(str);
				CONNECTOR->setRestart(TRUE);
				CONNECTOR->postServer(CONNECTOR_UPDATE);
			}
		}
		//the game has ended, so it doesn't matter
		else
		{
			OnTerminate();
		}
	}
	//otherwise, we need to update the amount of people in the game
	else
	{
		//determine if we are reloading
		if(NULL != m_pServerDialog)
		{
			bReload = m_pServerDialog->getReload();
		}

		//now send the remove message
		msg = CNDKMessage(S3D_REMOVE_PLAYER);
		msg.SetAt(0, uiID);
		msg.SetAt(1, bReload);

		//remove the player from the server game
		OnRemovePlayer(msg);

		//send to all
		SendMessageToAllUsers(msg);

		if(TRUE == CONNECTOR->getConnector())
		{
			//update the connector
			CONNECTOR->postServer(CONNECTOR_UPDATE);
		}

		//update the ladder status
		if((NULL != m_pServerDialog))
		{
			m_pServerDialog->handleLadder();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// server override - handle a message arrival
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnMessage(long lUserId, CNDKMessage& message)
{
	//handle message
	switch(message.GetId())
	{
	case S3D_ADD_PLAYER: OnAddPlayer(message, lUserId); break;
	case S3D_REMOVE_PLAYER: OnRemovePlayer(message); break;
	case S3D_CHAT_MESSAGE: OnChatMessage(message, lUserId); break;
	case S3D_PORT_PLACE: OnPortPlace(message); break;
	case S3D_PORT: OnPort(message, lUserId); break;
	case S3D_SETTLE_PLACE: OnSettlePlace(message, lUserId); break;
	case S3D_ROAD_PLACE: OnRoadPlace(message, lUserId); break;
	case S3D_SHIP_PLACE: OnShipPlace(message, lUserId); break;
	case S3D_CITY_PLACE: OnCityPlace(message, lUserId); break;
	case S3D_SHIP_MOVE: OnShipMove(message, lUserId); break;
	case S3D_ROLL: OnRoll(message, lUserId, FALSE); break;
	case S3D_HIDE_GOODS: OnHideGoods(message, lUserId); break;
	case S3D_HIDE_VOLCANO: OnHideVolcano(message, lUserId); break;
	case S3D_END_TURN: OnEndTurn(message, lUserId); break;
	case S3D_PLACE_ROBBER: OnPlaceRobber(message, lUserId); break;
	case S3D_PLACE_PIRATE: OnPlacePirate(message, lUserId); break;
	case S3D_STEAL: OnSteal(message, lUserId); break;
	case S3D_PANEL_TEXT: OnPanelText(message, lUserId); break;
	case S3D_TRADE_TO_BANK: OnTradeToBank(message, lUserId); break;
	case S3D_TRADE_JUNGLE: OnTradeJungle(message, lUserId); break;
	case S3D_BUY_DEV_CARD: OnBuyDevCard(message, lUserId); break;
	case S3D_PLAY_DEV_CARD: OnPlayDevCard(message, lUserId); break;
	case S3D_WAIT_FOR_LOSE: OnWaitForLose(message); break;
	case S3D_WAIT_FOR_GOLD: OnWaitForGold(message); break;
	case S3D_LOSE_CARDS: OnLoseCards(message, lUserId); break;
	case S3D_SELECT_GOLD: OnSelectGold(message, lUserId); break;
	case S3D_TRADE_OFFER: OnTradeOffer(message, lUserId); break;
	case S3D_TRADE_REPLY: OnTradeReply(message); break;
	case S3D_TRADE_COUNTER: OnTradeCounter(message); break;
	case S3D_TRADE: OnTrade(message, lUserId); break;
	case S3D_CHITLESS: OnChitless(message, lUserId); break;
	case S3D_WIN: OnWin(message, lUserId); break;
	case S3D_VERSION_CHECK: OnVersionCheck(message, lUserId); break;
	case S3D_UNDO_BUILD: OnUndoBuild(message, lUserId); break;
	case S3D_UNDO_AUTOTRADE: OnUndoAutoTrade(message, lUserId); break;
	case S3D_SPECIAL_NEXT: OnSpecialNext(message); break;
	case S3D_VOLCANO: OnVolcano(message, lUserId); break;
	case S3D_READY_STATE: OnReadyState(message, lUserId); break;
	default: break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// server override - handle a ping message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnPing(long lUserId, long lNbMilliseconds)
{
	UINT uiID = 0;
	CNDKMessage message;

	//get user ID
	m_mapPlayerID.Lookup(lUserId, uiID);
		
	//only update if we're not in a game yet
	if(FALSE == m_pGame->m_bInit)
	{
		//update everyone with the ping information
		message.SetId(S3D_PING_UPDATE);

		//add it to the message
		message.SetAt(0, uiID);
		message.SetAt(1, lNbMilliseconds);

		//send it to all
		OnPingUpdate(message);
		SendMessageToAllUsers(message);
	}
}

/////////////////////////////////////////////////////////////////////////////
// client override - handle a message arrival
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnMessage(CNDKMessage& message)
{
	//handle message
	switch(message.GetId())
	{
	case S3D_ADD_PLAYER: OnAddPlayer(message); break;
	case S3D_COLOR_ASSIGN: OnColorAssign(message); break;
	case S3D_REMOVE_PLAYER: OnRemovePlayer(message); break;
	case S3D_CHAT_MESSAGE: OnChatMessage(message); break;
	case S3D_SYS_MESSAGE: OnSysMessage(message); break;
	case S3D_PING_UPDATE: OnPingUpdate(message); break;
	case S3D_START_SEND: OnStartSend(message); break;
	case S3D_MAP: OnMap(message); break;
	case S3D_GAME: OnGame(message); break;
	case S3D_UNDO: OnUndo(message); break;
	case S3D_START_BOARD: OnStartBoard(message); break;
	case S3D_BOARD_PIECE: OnBoardPiece(message); break;
	case S3D_START_PLAYER: OnStartPlayer(message); break;
	case S3D_PLAYER_PIECE: OnPlayerPiece(message); break;
	case S3D_PLAYER_GAME: OnPlayerGame(message); break;
	case S3D_SAVED_DATA: OnSavedData(message); break;
	case S3D_END_SEND: OnEndSend(message); break;
	case S3D_END_SAVE: OnEndSave(message); break;
	case S3D_MAP_INFO: OnMapInfo(message); break;
	case S3D_DRAW_MODE: OnDrawMode(message); break;
	case S3D_PANEL_TEXT: OnPanelText(message); break;
	case S3D_PORT_PLACE: OnPortPlace(message); break;
	case S3D_PORT: OnPort(message); break;
	case S3D_SETTLE_PLACE: OnSettlePlace(message); break;
	case S3D_ROAD_PLACE: OnRoadPlace(message); break;
	case S3D_SHIP_PLACE: OnShipPlace(message); break;
	case S3D_CITY_PLACE: OnCityPlace(message); break;
	case S3D_SHIP_MOVE: OnShipMove(message); break;
	case S3D_START_TURN: OnStartTurn(message); break;
	case S3D_ROLL: OnRoll(message); break;
	case S3D_HIDE_GOODS: OnHideGoods(message); break;
	case S3D_HIDE_VOLCANO: OnHideVolcano(message); break;
	case S3D_END_TURN: OnEndTurn(message); break;
	case S3D_PLACE_ROBBER: OnPlaceRobber(message); break;
	case S3D_PLACE_PIRATE: OnPlacePirate(message); break;
	case S3D_STEAL: OnSteal(message); break;
	case S3D_TRADE_TO_BANK: OnTradeToBank(message); break;
	case S3D_TRADE_JUNGLE: OnTradeJungle(message); break;
	case S3D_BUY_DEV_CARD: OnBuyDevCard(message); break;
	case S3D_PLAY_DEV_CARD: OnPlayDevCard(message); break;
	case S3D_CONTINUE_7: OnContinue7(message); break;
	case S3D_CONTINUE_ROLL: OnContinueRoll(message); break;
	case S3D_INIT_GOLD: OnInitGold(message); break;
	case S3D_SELECT_GOLD: OnSelectGold(message); break;
	case S3D_LOSE_CARDS: OnLoseCards(message); break;
	case S3D_TRADE_OFFER: OnTradeOffer(message); break;
	case S3D_TRADE_REPLY: OnTradeReply(message); break;
	case S3D_TRADE_COUNTER: OnTradeCounter(message); break;
	case S3D_TRADE: OnTrade(message); break;
	case S3D_CHITLESS: OnChitless(message); break;
	case S3D_WIN: OnWin(message); break;
	case S3D_TIME_UPDATE: OnTimeUpdate(message); break;
	case S3D_VERSION_CHECK: OnVersionCheck(message); break;
	case S3D_UNDO_BUILD: OnUndoBuild(message); break;
	case S3D_UNDO_AUTOTRADE: OnUndoAutoTrade(message); break;
	case S3D_VOLCANO: OnVolcano(message); break;
	case S3D_SPECIAL_START: OnSpecialStart(message); break;
	case S3D_SPECIAL_END: OnSpecialEnd(message); break;
	case S3D_SAVE_STATE: OnSaveState(message); break;
	case S3D_RESTORE_STATE: OnRestoreState(message); break;
	case S3D_REJOIN_SYNCH: OnRejoinSynch(message); break;
	case S3D_READY_STATE: OnReadyState(message); break;
	default: break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// client override - handle a disconnect
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnDisconnect(NDKClientDisconnection disconnectionType)
{
	int iQuit = 0;
	CNDKMessage msg;
	CNetSysMessage sys;

	//kill the timer so we don't auto-disconnect
	if(NULL != m_pClientDialog)
	{
		m_pClientDialog->killTimer();
	}

	//look at connection type
	switch (disconnectionType)
	{
	case NDKClient_NormalDisconnection:	iQuit = SYS_LEFT_GAME; break;
	case NDKClient_ServerCloseConnection: iQuit = SYS_BOOTED_FROM_GAME; break;
    case NDKClient_ServerStop: iQuit = SYS_SERVER_QUIT; break;
	case NDKClient_ErrorSendingMessage:	SYS_SEND_ERROR; break;
	case NDKClient_ErrorReceivingMessage: SYS_RECV_ERROR; break;
	default: break;
	}

	//set data
	sys.m_uiMessage = iQuit;
	sys.m_strData = m_pGame->getNameByID(m_pGame->m_uiPlayerID);

	//translate
	sys.SerializeToMessage(msg);

	//send to self
	OnSysMessage(msg);

	//reset the game
	resetGame();

	//handle termination
	OnTerminate();
}

/////////////////////////////////////////////////////////////////////////////
// server-only handler for a new player and color resolution
/////////////////////////////////////////////////////////////////////////////
int CNetworkView::handleColors(long lUserID, CPlayer *pPlayer, BOOL bReload)
{
	int i;
	int iColor = -1;
	BOOL bColorOK = FALSE;
	CNDKMessage msg;
	CNetSysMessage sys;

	//make sure we're the server
	if(FALSE == m_pGame->m_bServer && FALSE == pPlayer->getAI())
	{
		return pPlayer->getGameColor();
	}

	if(bReload == FALSE)
	{
		if(FALSE == m_pGame->m_bColors[pPlayer->getColor1()])
		{
			//they got their first choice
			iColor = pPlayer->getColor1();
			bColorOK = TRUE;
		}

		if(-1 == iColor && FALSE == m_pGame->m_bColors[pPlayer->getColor2()])
		{
			//they got their second choice
			iColor = pPlayer->getColor2();
		}

		//still no color assigned
		if(-1 == iColor)
		{
			//they got the shaft, find the first open color
			for(i = 0; i < 6; i++)
			{
				if(FALSE == m_pGame->m_bColors[i])
				{
					iColor = i;
					break;
				}
			}
		}

		//make that color unavailable
		m_pGame->m_bColors[iColor] = TRUE;

		//give this player that color
		pPlayer->setGameColor(iColor);
	}
	else
	{
		//see what they're already assigned color
		for(i = 0; i < (int) m_pGame->m_players.size(); i++)
		{
			if(m_pGame->m_players[i].m_player.getID() == pPlayer->getID())
			{
				iColor = m_pGame->m_players[i].m_iColor;
				break;
			}
		}

		//now see if it's the same
		if(iColor != -1 && iColor != pPlayer->getColor1())
		{
			bColorOK = FALSE;
		}
		else
		{
			bColorOK = TRUE;
		}
	}

	//tell them about their new color if needed
	if(FALSE == bColorOK && FALSE == pPlayer->getAI())
	{
		msg.SetId(S3D_COLOR_ASSIGN);
		msg.SetAt(0, iColor);
		
		//tell the user about they're new color
		SendMessageToUser(lUserID, msg);

		//add a system message
		msg.SetId(S3D_SYS_MESSAGE);
		sys.m_uiMessage = SYS_COLOR_ASSIGN;
		sys.m_strData = pPlayer->getName();
		sys.m_strData2 = COLORS->getColorName(iColor);

		//translate
		sys.SerializeToMessage(msg);
		
		//send it
		SendMessageToAllUsers(msg);
		OnSysMessage(msg);
	}

	//return the color gained
	return iColor;
}

/////////////////////////////////////////////////////////////////////////////
// handle a player rejoining in game
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkView::handleInGameRejoin(long lUserID, CPlayer *pPlayer)
{
	int i;
	int nCount = 0;
	int nPlayer = -1;
	CNetSysMessage sys;
	CNDKMessage msg(S3D_SYS_MESSAGE);

	//search through the list of players to see if this player needs to rejoin
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//skip players already joined
		if(TRUE == m_pGame->m_players[i].m_bJoined)
		{
			continue;
		}

		//see if this player is the same
		if(m_pGame->m_players[i].m_player == (*pPlayer))
		{
			nPlayer = i;
		}
		else
		{
			//increment counter of missing players
			nCount++;
		}
	}

	//if the player is not a part of the game, remove them
	if(-1 == nPlayer)
	{
		//set the ID
		sys.m_uiMessage = SYS_GAME_FULL;
		sys.SerializeToMessage(msg);

		//send to this year
		SendMessageToUser(lUserID, msg);

		//disconnect the user
		DisconnectUser(lUserID, NDKServer_GameFull);

		//remove the player link
		m_mapPlayerID.RemoveKey(lUserID);

		return FALSE;
	}
	else
	{
		//add the player back in
		m_mapPlayerID.SetAt(lUserID, pPlayer->getID());
		m_mapSocketID.SetAt(pPlayer->getID(), lUserID);

		//set the players flag back to joined
		m_pGame->m_players[nPlayer].m_bJoined = TRUE;

		//create the sys message
		sys.m_uiMessage = SYS_REJOIN_GAME;
		sys.m_strData = pPlayer->getName();

		//send it to all
		sys.SerializeToMessage(msg);

		//send it out
		SendMessageToAllUsers(msg);
		OnSysMessage(msg);

		//if this is the last player to rejoin, we can continue
		if(0 == nCount)
		{
			//if this is a ladder game, the player has rejoined so remove their quit
			if(TRUE == m_pGame->m_bLadder)
			{
				CONNECTOR->unquitPlayer(nPlayer);
			}

			serverRestoreState();
		}

		//send the player's data to rejoin the game
		sendRejoinData(lUserID, nPlayer);

		return TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a player connecting in a reload of a network game
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkView::handleReload(long lUserID, CPlayer *pPlayer)
{
	int i;
	BOOL bJoin = FALSE;

	//sanity check
	if(NULL == m_pList)
	{
		return FALSE;
	}

	//search player games to see if this player has joined yet
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		//find the player
		if(pPlayer->getID() == m_pGame->m_players[i].m_player.getID())
		{
			//see if they've joined already
			if(FALSE == m_pGame->m_players[i].m_bJoined)
			{
				//allow them to join
				m_pGame->m_players[i].m_bJoined = TRUE;
				bJoin = TRUE;
			}
		}
	}

	//if we have no join, disconnect the user
	if(FALSE == bJoin)
	{
		//we must disconnect the user
		DisconnectUser(lUserID, NDKServer_InvalidPlayer);

		return FALSE;
	}

	//otherwise, update the list
	m_pList->updateJoin(pPlayer->getID(), TRUE);
	
	//valid update
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// reset the game after a disconnect
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::resetGame()
{
	int iColor;
	CPlayer player;

	//save this player
	player = m_pGame->m_players[0].m_player;

	//reset the ready flag
	player.setReady(m_pGame->m_bServer);

	//get their original, desired color
	iColor = player.getColor1();

	//clear player games
	if(FALSE == m_pGame->m_bInit)
	{
		m_pGame->m_players.clear();
		m_pGame->m_players.resize(1);
		m_pGame->m_players[0].m_player = player;
		m_pGame->m_players[0].m_iColor = iColor;
		m_pGame->m_players[0].m_player.setGameColor(iColor);
	}

	if(TRUE == m_pGame->m_bServer)
	{
		//clear out the used colors
		::ZeroMemory(m_pGame->m_bColors, sizeof(m_pGame->m_bColors));

		//set this color as used
		m_pGame->m_bColors[iColor] = TRUE;
	}

	//reset dialog
	if(NULL != m_pClientDialog)
	{
		m_pClientDialog->disconnect();
	}

	//reset player list
	if(NULL != m_pList)
	{
		//clear out the list control
		m_pList->DeleteAllItems();

		//readd this player
		m_pList->addPlayer(&player, iColor);
	}
}

/////////////////////////////////////////////////////////////////////////////
// send board data to clients
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::sendBoardData(long lUserID)
{
	int i;
	int iStart = 0;
	int iSize;
	int iBoard;
	int iPlayers;
	CNDKMessage msg;

	//get the size of the board
	iBoard = m_pBoard.GetSize();

	//get the size of the board and players
	iPlayers = (int) m_pGame->m_players.size();

	if(NULL != m_pStatusDialog)
	{
		m_pStatusDialog->m_StatusStatic.SetWindowText("Sending board data...");
		m_pStatusDialog->m_Progress.SetRange32(0, iBoard + iPlayers);
		m_pStatusDialog->m_Progress.SetPos(0);
	}

	//notify
	msg.SetId(S3D_START_SEND);
	
	//send variant info
	i = 1;
	m_pGame->m_variants.SerializeToMessage(i, msg);

	//send properly
	if(-1 == lUserID)
	{
		SendMessageToAllUsers(msg);
	}
	else
	{
		SendMessageToUser(lUserID, msg);
	}

	//send the map information
	msg = CNDKMessage(S3D_MAP);
	MAPS->SerializeMapToMessage(msg);

	//send properly
	if(-1 == lUserID)
	{
		SendMessageToAllUsers(msg);
	}
	else
	{
		SendMessageToUser(lUserID, msg);
	}

	//send board info
	msg = CNDKMessage(S3D_START_BOARD);
	msg.SetAt(iStart++, iBoard);
	msg.SetAt(iStart++, iPlayers);

	//array of dev cards
	iSize = (int) m_iDevCardSet.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart++, m_iDevCardSet[i]);
		msg.SetAt(iStart++, m_iDevCardArray[i]);
	}

	//hidden tile array
	iSize = m_nHiddenArray.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart++, m_nHiddenArray[i]);
	}

	//hidden volcano array
	iSize = m_nHiddenVolcanoArray.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart++, m_nHiddenVolcanoArray[i]);
	}

	//hidden chit array
	iSize = m_nHiddenChitArray.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart++, m_nHiddenChitArray[i]);
	}

	//chitless chit array
	iSize = m_nChitlessChitArray.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart++, m_nChitlessChitArray[i]);
	}

	//map id and ladder setting
	msg.SetAt(iStart++, m_pGame->m_uiMapID);
	msg.SetAt(iStart++, m_pGame->m_bLadder);

	//send properly
	if(-1 == lUserID)
	{
		SendMessageToAllUsers(msg);
	}
	else
	{
		SendMessageToUser(lUserID, msg);
	}

	//write board data
	for(i = 0; i < iBoard; i++)
	{
		msg = CNDKMessage(S3D_BOARD_PIECE);

		if(NULL != m_pStatusDialog)
		{
			m_pStatusDialog->m_Progress.SetPos(i);
		}
		
		msg.SetAt(0, i);
		m_pBoard[i].SerializeToMessage(msg);

		//send it
		if(-1 == lUserID)
		{
			SendMessageToAllUsers(msg);
		}
		else
		{
			SendMessageToUser(lUserID, msg);
		}
	}

	//finish board data
	msg = CNDKMessage(S3D_START_PLAYER);
	
	//send properly
	if(-1 == lUserID)
	{
		SendMessageToAllUsers(msg);
	}
	else
	{
		SendMessageToUser(lUserID, msg);
	}

	if(NULL != m_pStatusDialog)
	{
		m_pStatusDialog->m_StatusStatic.SetWindowText("Sending player data...");
	}
}

/////////////////////////////////////////////////////////////////////////////
// send data to a rejoin player
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::sendRejoinData(long lUserID, int nPlayer)
{
	int i;
	int nBoard;
	CNDKMessage msg;
	CWaitCursor wait;
	
	//send game data
	msg.SetId(S3D_GAME);
	m_pGame->SerializeToMessage(msg);
	SendMessageToUser(lUserID, msg);

	//send undo data
	msg = CNDKMessage(S3D_UNDO);
	THISVIEW->getUndoDialog()->SerializeToMessage(msg);
	SendMessageToUser(lUserID, msg);

	//send board and map data
	sendBoardData(lUserID);
	
	//get board size
	nBoard = m_pBoard.GetSize();

	//write player data
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//now send the players, in order
		msg = CNDKMessage(S3D_PLAYER_GAME);
		msg.SetAt(0, i);
		m_pGame->m_players[i].SerializeToMessage(msg);

		//send it
		SendMessageToUser(lUserID, msg);
	}

	//write miscellaneous data
	msg = CNDKMessage(S3D_SAVED_DATA);
	SerializeToMessage(msg);
	SendMessageToUser(lUserID, msg);

	//notify
	msg = CNDKMessage(S3D_END_SAVE);
	msg.SetAt(0, m_iRobber);
	msg.SetAt(1, m_iPirate);
	msg.SetAt(2, TRUE);
	SendMessageToUser(lUserID, msg);

	//resynch the player
	serverResynchRejoin(lUserID, nPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// monster send function to write player/board data to all connected
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::sendGameData()
{
	int i;
	int iBoard;
	CNDKMessage msg;
	
	//create the status dialog
	createStatus();

	//send game data
	msg.SetId(S3D_GAME);
	m_pGame->SerializeToMessage(msg);
	SendMessageToAllUsers(msg);

	//send board and map data
	sendBoardData();
	
	//now send the players, in order
	msg.SetId(S3D_PLAYER_PIECE);

	//get board size
	iBoard = m_pBoard.GetSize();

	//write player data
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		if(NULL != m_pStatusDialog)
		{
			m_pStatusDialog->m_Progress.SetPos(iBoard + i);
		}

		msg.SetAt(0, i);
		m_pGame->m_players[i].m_player.SerializeToMessage(msg);

		//send it
		SendMessageToAllUsers(msg);

		//initialize size
		m_pGame->m_players[i].initPlayer(MAP->m_iLandTiles + MAP->m_iSeaTiles);
	}

	//notify
	msg = CNDKMessage(S3D_END_SEND);
	msg.SetAt(0, m_iRobber);
	msg.SetAt(1, m_iPirate);
	SendMessageToAllUsers(msg);

	//delete the status dialog
	deleteStatus();

	//create the chat window
	createChatWindow();

	//set the allowed cards if varianted
	if(IS_VARIANT(VARIANT_ALTER_ALLOWED_CARDS))
	{
		VIEW->setAllowedCards(VARIANTS->m_iAllowedCards);
	}

	//set everyone's joined flags
	for(i = 0; i < m_iNumPlayers; i++)
	{
		m_pGame->m_players[i].m_bJoined = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// resend game data across the network
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::sendSaveData()
{
	int i;
	int iBoard;
	CNDKMessage msg;
	
	//set the game init, which disallows people to join
	m_pGame->m_bInit = TRUE;

	//create the status dialog
	createStatus();

	//send game data
	msg.SetId(S3D_GAME);
	m_pGame->SerializeToMessage(msg);
	SendMessageToAllUsers(msg);

	//send undo data
	msg = CNDKMessage(S3D_UNDO);
	THISVIEW->getUndoDialog()->SerializeToMessage(msg);
	SendMessageToAllUsers(msg);

	//send board and map data
	sendBoardData();
	
	//get board size
	iBoard = m_pBoard.GetSize();

	//write player data
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		//now send the players, in order
		msg = CNDKMessage(S3D_PLAYER_GAME);

		if(NULL != m_pStatusDialog)
		{
			m_pStatusDialog->m_Progress.SetPos(iBoard + i);
		}

		msg.SetAt(0, i);
		m_pGame->m_players[i].SerializeToMessage(msg);

		//send it
		SendMessageToAllUsers(msg);
	}

	//write miscellaneous data
	msg = CNDKMessage(S3D_SAVED_DATA);
	SerializeToMessage(msg);
	SendMessageToAllUsers(msg);

	//notify
	msg = CNDKMessage(S3D_END_SAVE);
	msg.SetAt(0, m_iRobber);
	msg.SetAt(1, m_iPirate);
	msg.SetAt(2, FALSE);
	SendMessageToAllUsers(msg);

	//delete the status dialog
	deleteStatus();

	//create the chat window
	createChatWindow();
}

/////////////////////////////////////////////////////////////////////////////
// create the status dialog
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::createStatus()
{
	if(NULL == m_pStatusDialog)
	{
		m_pStatusDialog = new CNetworkStatusDialog(this);
		m_pStatusDialog->Create(IDD_NETWORK_STATUS, this);
		m_pStatusDialog->CenterWindow();
		m_pStatusDialog->ShowWindow(SW_SHOW);
	}

	if(NULL == m_pTradeOfferDialog)
	{
		//create the trade offer dialog
		m_pTradeOfferDialog = new CTradeOfferDialog(this);	
		m_pTradeOfferDialog->Create(IDD_TRADE_OFFER_DLG);
		m_pTradeOfferDialog->ShowWindow(SW_HIDE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// delete the status dialog
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::deleteStatus()
{
	if(NULL != m_pStatusDialog)
	{
		delete m_pStatusDialog;
		m_pStatusDialog = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// create the chat window
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::createChatWindow()
{
	int x, y;
	int cx, cy;
	int sx, sy;
	CRect rect;
	CDC *pDC;

	if(NULL == m_pChatWindow)
	{
		//get the device context
		pDC = GetDC();

		//read .ini file
		x = GetPrivateProfileInt(INI_DISPLAY, INI_CHAT_OFFSET_X, -1, INI_FILE);
		y = GetPrivateProfileInt(INI_DISPLAY, INI_CHAT_OFFSET_Y, -1, INI_FILE);

		cx = GetPrivateProfileInt(INI_DISPLAY, INI_CHAT_SIZE_X, -1, INI_FILE);
		cy = GetPrivateProfileInt(INI_DISPLAY, INI_CHAT_SIZE_Y, -1, INI_FILE);

		//get device caps
		sx = pDC->GetDeviceCaps(HORZRES);
		sy = pDC->GetDeviceCaps(VERTRES);

		//release the dc
		ReleaseDC(pDC);

		//make sure it will stay on screen
		if(cx >= sx) cx = sx - 1;
		if(cy >= sy) cy = sy - 1;

		//create dialog
		m_pChatWindow = new CNetworkChatDialog(this);
		m_pChatWindow->Create(IDD_NETWORK_CHAT, this);

		//get rect for measurement
		m_pChatWindow->GetWindowRect(&rect);

		//evaluate .ini settings
		if(-1 == x) x = rect.left;
		if(-1 == y) y = rect.top;
		if(-1 == cx) cx = rect.Width();
		if(-1 == cy) cy = rect.Height();

		//make sure that it doesn't go over the screen
		if(x + cx >= sx) x = sx - cx - 1;
		if(y + cy >= sy) y = sy - cy - 1;

		//size window
		m_pChatWindow->SetWindowPos(&wndTop, x, y, cx, cy, SWP_NOZORDER | SWP_HIDEWINDOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// delete the chat window
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::deleteChatWindow()
{
	if(NULL != m_pChatWindow)
	{
		delete m_pChatWindow;
		m_pChatWindow = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// set a new map information string
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::setInfoString(CString str)
{
	CNDKMessage msg(S3D_MAP_INFO);

	//set the new string
	m_strMapInfo = str;

	//send it to all
	msg.SetAt(0, m_strMapInfo);
	SendMessageToAllUsers(msg);

	//set the info text
	if(NULL != m_pInfo)
	{
		m_pInfo->SetWindowText("");
		m_pInfo->addString(m_strMapInfo, FALSE, COLOR_LTGRAY);
	}
}

/////////////////////////////////////////////////////////////////////////////
// boot a player based on ID
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::bootPlayer(UINT uiID)
{
	long lPlayer;

	//lookup the player
	if(TRUE == m_mapSocketID.Lookup(uiID, lPlayer))
	{
		//destroy!
		DisconnectUser(lPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// return a string that has the information on a jungle trade
/////////////////////////////////////////////////////////////////////////////
CString CNetworkView::getTradeJungleString(int nJungles, int *nFromBank)
{
	int i;
	CString str;
	CString strTemp;

	//create the formatted string
	str.Format(" %d Discover%s for", nJungles, (nJungles == 1) ? "y" : "ies");

	//run through the list of goods they traded
	for(i = 0; i < 5; i++)
	{
		//see if they got this item
		if(0 < nFromBank[i])
		{
			//add it
			strTemp.Format(" %d %s,", nFromBank[i], VIEW->getResName(i, TRUE));
			str += strTemp;
		}
	}

	//get rid of the last comma
	str = str.Left(str.GetLength() - 1);
	str += ".";

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// return a string that has the information on a trade
/////////////////////////////////////////////////////////////////////////////
CString CNetworkView::getTradeToBankString(int *iToBank, int *iFromBank)
{
	int i;
	CString strTrade;
	CString strTo, strFor;
	CString strTemp;

	//now create the string and handle resource trading
	for(i = 0; i < 5; i++)
	{
		//look at traded to
		if(iToBank[i])
		{
			strTemp.Format(" %d %s,", iToBank[i], getResName(i, TRUE));
			strTo += strTemp;
		}

		//look at the traded for
		if(iFromBank[i])
		{
			strTemp.Format(" %d %s,", iFromBank[i], getResName(i, TRUE));
			strFor += strTemp;
		}
	}

	//now trim the strings and create the large one
	strTo = strTo.Left(strTo.GetLength() - 1);
	strFor = strFor.Left(strFor.GetLength() - 1);

	//add them on
	strTrade = strTo;
	strTrade += " to the bank for";
	strTrade += strFor;
	strTrade += ".";

	//return the formatted string
	return strTrade;
}

/////////////////////////////////////////////////////////////////////////////
// return a string that has the information on losing cards
/////////////////////////////////////////////////////////////////////////////
CString CNetworkView::getLoseString(int *iLose)
{
	int i;
	CString str;
	CString strTemp;

	//add the resources
	for(i = 0; i < 5; i++)
	{
		//see if they lost this card
		if(0 == iLose[i])
		{
			continue;
		}

		//format
		strTemp.Format(" %d %s,", iLose[i], VIEW->getResName(i, TRUE));

		//add it
		str += strTemp;
	}

	//get rid of the last comma
	str = str.Left(str.GetLength() - 1);
	str += ".";

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// get the trade string
/////////////////////////////////////////////////////////////////////////////
CString CNetworkView::getTradeString(int iPartner, int *iTo, int *iFor)
{
	int i;
	int iToCount = 0;
	int iForCount = 0;
	CString str;
	CString strTemp;
	CString strTo, strFor;

	//run through the list of goods they traded
	for(i = 0; i < 5; i++)
	{
		//see if they traded this item
		if(0 < iTo[i])
		{
			//increment counter
			iToCount++;

			//add it
			strTemp.Format(" %d %s,", iTo[i], getResName(i, TRUE));
			strTo += strTemp;
		}

		//see if they traded for this item
		if(0 < iFor[i])
		{
			//increment counter
			iForCount++;

			//add it
			strTemp.Format(" %d %s,", iFor[i], getResName(i, TRUE));
			strFor += strTemp;
		}
	}

	//get rid of the last commas
	strTo = strTo.Left(strTo.GetLength() - 1);
	strFor = strFor.Left(strFor.GetLength() - 1);

	//add the to clause
	if(0 < iToCount)
	{
		str += strTo;
	}
	else
	{
		str += " nothing";
	}

	//add the other player
	str += " to ";
	str += m_pGame->m_players[iPartner].m_player.getName();
	str += " for";

	//add the from clause
	if(0 < iForCount)
	{
		str += strFor;
	}
	else
	{
		str += " nothing";
	}

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// run a trade offer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::handleTradeOffer(int *iTo, int *iFor, UINT uiID)
{
	//sanity check
	if(NULL == m_pTradeOfferDialog)
	{
		return;
	}

	//play sound
	SOUND(SOUND_TRADE_OFFER);

	//make app aware
	setDialog(m_pTradeOfferDialog, TRUE);

	//set params
	m_pTradeOfferDialog->setOffer(iTo, iFor, uiID);
}

/////////////////////////////////////////////////////////////////////////////
// handle a trade offer update
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::handleTradeReply(int iPlayer, UINT uiID, short shStatus, int *iTo, int *iFor)
{
	//see if this trade ID is even legitimate
	if(uiID != m_uiTradeID)
	{
		return;
	}

	//handle it
	if(NULL != m_pTrade)
	{
		m_pTrade->handleReply(iPlayer, shStatus, iTo, iFor);
	}
}

/////////////////////////////////////////////////////////////////////////////
// get a socket ID
/////////////////////////////////////////////////////////////////////////////
long CNetworkView::getSocketID(UINT uiID)
{
	long lSocket = -1;

	//look it up
	m_mapSocketID.Lookup(uiID, lSocket);

	return lSocket;
}

/////////////////////////////////////////////////////////////////////////////
// find the next person in the list to send a gold message to
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::incrementGold()
{
	int i;
	int nCurIndex;
	long lUserID = -1;
	UINT uiID;
	BOOL bDone = FALSE;
	CNDKMessage msg;

	//start with the current gold index player and find the next person
	//in the list that is getting gold from this roll
	for(i = 0; i < m_iNumPlayers; i++, m_nGoldIndex++, m_nGoldCount++)
	{
		//wrap around
		if(m_nGoldIndex >= m_iNumPlayers)
		{
			m_nGoldIndex = 0;
		}

		//see if we're done (by getting all the way back to the current
		//player)
		if(m_nGoldCount >= m_iNumPlayers)
		{
			bDone = TRUE;
			break;
		}

		//see if this player gets gold
		if(0 < m_nWaitForGold[m_nGoldIndex])
		{
			break;
		}
	}

	//if done, send the continue message on to the waiting player
	if(TRUE == bDone)
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

		//reset the array
		m_nWaitForGold.resize(0);
		m_nWaitForGold.clear();
	}
	//if we're not done, it means we found a player to send the gold
	//message to, so send it
	else
	{
		msg.SetId(S3D_INIT_GOLD);

		//set the current index for the gold
		nCurIndex = m_nGoldIndex;

		//increment for the next time around
		m_nGoldCount++;
		m_nGoldIndex++;

		//set the player and the amount they get
		msg.SetAt(0, nCurIndex);
		msg.SetAt(1, m_nWaitForGold[nCurIndex]);

		//send it
		if(nCurIndex == m_pGame->m_iIndex)
		{
			//to the server
			OnInitGold(msg);
		}
		else
		{
			uiID = m_pGame->m_players[nCurIndex].m_player.getID();

			//look up the ID
			m_mapSocketID.Lookup(uiID, lUserID);

			//send the message
			SendMessageToUser(lUserID, msg);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// a dialog is being run, save the state in case someone drops
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::setDialog(CDialog *p, BOOL bCanClose)
{
	m_pDialog = p;

	if(NULL != m_pDialog)
	{
		m_bCanClose = bCanClose; 
		m_saveDialog.saveState(TRUE);
	}
	else
	{
		m_bCanClose = TRUE;
		m_saveDialog.resetState();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle timer events
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_PING: 
		//ping all
		PingAllUsers();
		break;
	default: break;
	}
	
	CGLView::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// handle cleanup
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::OnDestroy() 
{
	//shutdown the AI
	APP->ShutdownAI();

	//kill the timer
	if(-1 != m_uiPing)
	{
		KillTimer(m_uiPing);
	}

	//kill the trade dialog
	if(NULL != m_pTradeOfferDialog)
	{
		delete m_pTradeOfferDialog;
		m_pTradeOfferDialog = NULL;
	}

	CGLView::OnDestroy();
}