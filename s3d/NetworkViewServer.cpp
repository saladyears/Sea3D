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
#include "rand.h"
#include "UndoOne.h"
#include "defineCode.h"
#include "defineAI.h"
#include "defineNetwork.h"
#include "defineVariants.h"

/////////////////////////////////////////////////////////////////////////////
// handle port placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverPortPlace(BOOL bStart)
{
	long lUserID = -1;
	UINT uiID;
	CNDKMessage msg(S3D_PORT_PLACE);
	
	//if this is the first time through, we need to set a random starting player
	if(TRUE == bStart)
	{
		//in port placement
		m_bPortPlacement = TRUE;

		//generate the port placements
		generatePortPlace();

		//start with a random player
		m_iPlayer = int(RAND * m_iNumPlayers);

		//cap it (it's a one in 4 billion chance, but hey)
		if(m_iPlayer >= m_iNumPlayers) m_iPlayer = m_iNumPlayers - 1;

		//now subtract one since it's going to be added to immediately
		m_iPlayer--;
	}
	else
	{
		//increment the port index
		m_nPortIndex++;
	}

	//see if we are done
	if(m_nPortIndex >= (int) m_nPortArray.size())
	{
		//no longer in port placement
		m_bPortPlacement = FALSE;

		serverPlayerPlace(TRUE);
		return;
	}

	//increment the player
	m_iPlayer++;

	//see if we've gone over
	if(m_iPlayer >= m_iNumPlayers) m_iPlayer = 0;

	//find this player and send them a message
	uiID = m_pGame->m_players[m_iPlayer].m_player.getID();

	//look it up
	m_mapSocketID.Lookup(uiID, lUserID);

	//send a message to everyone telling them what we're waiting for
	serverPanelText(S3D_PORT_PLACE, m_iPlayer, lUserID);

	//create the message and send it
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, m_nPortArray[m_nPortIndex]);

	//send it
	if(-1 != lUserID)
	{
		SendMessageToUser(lUserID, msg);
	}
	else
	{
		OnPortPlace(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle player placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverPlayerPlace(BOOL bStart)
{
	BOOL bAI;
	long lUserID = -1;
	UINT uiID;
	CString str;
	CString strName;

	//if this is the first placement, do stuff
	if(TRUE == bStart)
	{
		m_iPlayer = -1;
	}

	//increment the player and send the message
	m_iPlayer += (m_bIncrease ? 1 : -1);
	if(m_iPlayer >= m_iNumPlayers)
	{
		m_iPlayer = m_iNumPlayers - 1; 
		m_bIncrease = FALSE;
	}

	//see if we're done
	if(-1 == m_iPlayer)
	{
		if(IS_VARIANT(VARIANT_TOURNAMENT_START))
		{
			serverThirdRoadPlace(TRUE);
		}
		else
		{
			//set the start time reference
			m_pGame->m_timeRef = COleDateTime::GetCurrentTime();

			//start turns
			serverStartTurn(TRUE);
		}

		return;
	}

	//find this player and send them a message
	uiID = m_pGame->m_players[m_iPlayer].m_player.getID();
	strName = m_pGame->m_players[m_iPlayer].m_player.getName();

	//determine if this player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//look it up
	if(FALSE == bAI)
	{
		m_mapSocketID.Lookup(uiID, lUserID);
	}
	else
	{
		lUserID = -10;
	}

	//send a message to everyone telling them what we're waiting for
	serverPanelText(S3D_SETTLE_PLACE, m_iPlayer, lUserID);

	//send the draw message
	if(FALSE == bAI)
	{
		serverDrawMode(lUserID, GL_PLACE_PLAYERS, TRUE);
	}
	else
	{
		//set wait mode
		setNetDrawMode(GL_WAIT);

		//once for settlement
		if(AI_FINISHED > serverAI(AI_STATE_INITIAL))
		{
			return;
		}

		//set wait mode
		setNetDrawMode(GL_WAIT);

		//prepare road placements
		prepareInitialRoads();

		//once for road
		if(AI_FINISHED > serverAI(AI_STATE_INITIAL))
		{
			return;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle third road placements
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverThirdRoadPlace(BOOL bStart)
{
	BOOL bAI;
	long lUserID = -1;
	UINT uiID;
	CString str;
	CString strName;

	//if this is the first placement, do stuff
	if(TRUE == bStart)
	{
		m_bTournamentRoads = TRUE;
		m_iPlayer = -1;
	}

	//increase the player
	m_iPlayer++;

	//see if we're done
	if(m_iPlayer >= m_iNumPlayers)
	{
		//no longer in roads mode
		m_bTournamentRoads = FALSE;

		//set the start time reference
		m_pGame->m_timeRef = COleDateTime::GetCurrentTime();

		//start turns
		serverStartTurn(TRUE);

		return;
	}

	//find this player and send them a message
	uiID = m_pGame->m_players[m_iPlayer].m_player.getID();
	strName = m_pGame->m_players[m_iPlayer].m_player.getName();

	//determine if this player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//look it up
	if(FALSE == bAI)
	{
		m_mapSocketID.Lookup(uiID, lUserID);
	}
	else
	{
		lUserID = -10;
	}

	//send a message to everyone telling them what we're waiting for
	serverPanelText(S3D_ROAD_PLACE, m_iPlayer, lUserID);

	//send the draw message
	if(FALSE == bAI)
	{
		serverDrawMode(lUserID, GL_PLACE_THIRD_ROAD, TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// send a start turn message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverStartTurn(BOOL bStart)
{
	//make sure we start correctly
	if(TRUE == bStart)
	{
		m_iPlayer = -1;

		//set the in game flag
		m_pGame->m_bInGame = TRUE;
	}
	//otherwise, we just ended someone's turn, so update their time stamp
	else
	{
		serverTimeUpdate();
	}

	//update the count of this player
	if(-1 != m_iPlayer)
	{
		THISVIEW->updateCounts(m_iPlayer);
	}

	//see if we need to go to the special build phase
	if((FALSE == bStart) && (IS_VARIANT(VARIANT_SPECIAL_BUILD)))
	{
		//set the index player
		m_nSpecialStart = m_iPlayer;

		//run the special build phase
		serverSpecialBuild();
	}
	else
	{
		//continue the turn
		serverContinueTurn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// continue the start of the turn
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverContinueTurn()
{
	int iRoll;
	long lUserID = -1;
	BOOL bAI;
	UINT uiID;
	CString str;
	CString strName;
	CNDKMessage msg(S3D_START_TURN);

	//increment the player and see if we need to wrap
	m_iPlayer++;

	//go back to the first player if we've wrapped
	if(m_iPlayer >= m_iNumPlayers)
	{
		m_iPlayer = 0;
	}

	//update the count of this player
	THISVIEW->updateCounts(m_iPlayer);

	//find this player and send them a message
	uiID = m_pGame->m_players[m_iPlayer].m_player.getID();
	strName = m_pGame->m_players[m_iPlayer].m_player.getName();

	//determine if this player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//look it up
	if(FALSE == bAI)
	{
		m_mapSocketID.Lookup(uiID, lUserID);
	}
	else
	{
		lUserID = -10;
	}

	//set the new player
	msg.SetAt(0, m_iPlayer);

	//send it to everyone
	SendMessageToAllUsers(msg);

	//handle it ourselves
	OnStartTurn(msg);
	
	//tell everyone else they're waiting
	serverPanelText(S3D_ROLL, m_iPlayer, lUserID);

	//if it's the AI, roll for it
	if(TRUE == bAI)
	{
		//handle a roll
		THISVIEW->getControlPanel()->OnRollButton();

		//see if it rolled a 7
		iRoll = m_pGame->m_iRollList[m_pGame->m_iRollList.size() - 1];

		//let other processing be handled it it's a 7
		if(7 == iRoll)
		{
			return;
		}

		//handle the AI turn
		serverAITurn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle the special build phase
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverSpecialBuild()
{
	CNDKMessage msg(S3D_SPECIAL_START);

	//increment the player and see if we need to wrap
	m_iPlayer++;

	//go back to the first player if we've wrapped
	if(m_iPlayer >= m_iNumPlayers)
	{
		m_iPlayer = 0;
	}

	//see if we've wrapped around
	if(m_nSpecialStart == m_iPlayer)
	{
		//send out the end message
		msg = CNDKMessage(S3D_SPECIAL_END);

		//send it to everyone
		SendMessageToAllUsers(msg);

		//handle it ourselves
		OnSpecialEnd(msg);

		//actually continue the turn
		serverContinueTurn();
	}
	else
	{
		//send out the build phase start message and let the clients
		//handle it
		msg.SetAt(0, m_iPlayer);

		//send it to everyone
		SendMessageToAllUsers(msg);

		//handle it ourselves
		OnSpecialStart(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// update a player's turn time
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverTimeUpdate()
{
	int iSeconds;
	COleDateTime time;
	COleDateTimeSpan timespan;
	CNDKMessage msg(S3D_TIME_UPDATE);

	//get the current time
	time = COleDateTime::GetCurrentTime();
	
	//calculate the difference in time for this turn
	timespan = time - m_pGame->m_timeRef;

	//calcuate turn length
	iSeconds = (int) timespan.GetTotalSeconds();

	//set new time
	m_pGame->m_timeRef = COleDateTime::GetCurrentTime();

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iSeconds);

	//handle update and then send to everyone else
	handleCommonTimeUpdate(m_iPlayer, iSeconds);
	SendMessageToAllUsers(msg);
}

/////////////////////////////////////////////////////////////////////////////
// send a draw mode message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverDrawMode(long lUserID, int iMode, BOOL bUseSelection)
{
	CNDKMessage msg(S3D_DRAW_MODE);

	//set the mode
	msg.SetAt(0, iMode);
	msg.SetAt(1, bUseSelection);
	msg.SetAt(2, m_bIncrease);

	//send it (to server if -1, or client if not)
	if(-1 == lUserID)
	{
		OnDrawMode(msg);
	}
	else
	{
		SendMessageToUser(lUserID, msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// send a panel message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverPanelText(long lMessage, int iPlayer, long lUserID)
{
	CString str;
	CString strFormat;
	CString strName;
	CNetSysMessage sys;
	CNDKMessage msg(S3D_SYS_MESSAGE);
	CNDKMessage message;

	//set the message id
	message.SetId(S3D_PANEL_TEXT);

	//get the name of the player
	strName = m_pGame->m_players[iPlayer].m_player.getName();

	//get the format
	switch(lMessage)
	{
	case S3D_PORT_PLACE: strFormat = "Waiting for %s to place a port."; break;
	case S3D_SETTLE_PLACE:
		if(IS_VARIANT(VARIANT_TOURNAMENT_START) && (FALSE == m_bIncrease))
		{
			strFormat = "Waiting for %s to place a city."; 
		}
		else
		{
			strFormat = "Waiting for %s to place a settlement."; 
		}
		break;
	case S3D_ROAD_PLACE: strFormat = "Waiting for %s to place a road."; break;
	case S3D_ROLL: strFormat = "Waiting for %s to roll."; break;
	case S3D_RESTART_GAME:
	case S3D_START_TURN: strFormat = "Waiting for %s to finish turn actions."; break;
	case S3D_WAIT_FOR_STEAL: strFormat = "Waiting for %s to steal a card."; break;
	case S3D_WAIT_FOR_LOSE: strFormat = "Waiting for players to lose cards."; iPlayer = m_iPlayer; break;
	case S3D_WAIT_FOR_GOLD: strFormat = "Waiting for players to use gold."; break;
	}

	//format the string
	str.Format(strFormat, strName);

	//set message data
	message.SetAt(0, str);
	message.SetAt(1, iPlayer);

	//send it out
	SendMessageToAllUsersExceptFor(lUserID, message);

	//make sure we get it too
	if(-1 != lUserID)
	{
		OnPanelText(message, -1, TRUE);
	}

	//if we get an S3D_START_TURN message, autosave
	if(S3D_START_TURN == lMessage)
	{
		THISVIEW->autoSave();

		//send autosave message
		sys.m_uiMessage = SYS_AUTO_SAVE;
		sys.SerializeToMessage(msg);
		OnSysMessage(msg);		
	}
}

/////////////////////////////////////////////////////////////////////////////
// we got a 7 rolled some place, so we have to wait for players to determine
// if they lose cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverWaitForLose(long lUserID)
{
	//set the wait ID to the person who just rolled
	m_lWaitID = lUserID;
}

/////////////////////////////////////////////////////////////////////////////
// we're in seafarers and a 7 was not rolled so we have to wait and see if
// players rolled a gold tile
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverWaitForGold(long lUserID)
{
	m_lWaitID = lUserID;
}

/////////////////////////////////////////////////////////////////////////////
// handle sending a system message
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverSysMessage(UINT uiMessage, int iPlayer, CString str)
{
	CNetSysMessage sys;
	CNDKMessage msg(S3D_SYS_MESSAGE);

	//set some stuff
	sys.m_uiMessage = uiMessage;
	sys.m_strData = m_pGame->m_players[iPlayer].m_player.getName();
	sys.m_strData2 = str;
	sys.m_iColor = m_pGame->m_players[iPlayer].m_iColor;

	//serialize
	sys.SerializeToMessage(msg);

	//send
	SendMessageToAllUsers(msg);
	OnSysMessage(msg);
}

/////////////////////////////////////////////////////////////////////////////
// start the internal ping timer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverStartPing()
{
	//create the ping timer
	m_uiPing = SetTimer(TIMER_PING, 5000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// handle an AI's turn
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAITurn(BOOL bRoll)
{
	//skip the roll on restarts if it's our turn
	if(TRUE == bRoll && AI_FINISHED > serverAI(AI_STATE_ROLL))
	{
		return;
	}

	//now enter the main turn loop
	if(AI_FINISHED > serverAIloop())
	{
		return;
	}

	//now end the turn
	THISVIEW->getControlPanel()->OnEndButton();
}

/////////////////////////////////////////////////////////////////////////////
// main turn loop for the AI
/////////////////////////////////////////////////////////////////////////////
int CNetworkView::serverAIloop()
{
	int i = 0;
	int iMax = 20;
	int iRet;

	//get the first value
	iRet = serverAI(AI_STATE_TURN);

	//main loop
	while((iRet > AI_FINISHED) && (i < iMax))
	{
		//do turn action
		iRet = serverAI(AI_STATE_TURN);

		//increment
		i++;
	}

	//return last value
	return iRet;
}

/////////////////////////////////////////////////////////////////////////////
// run the AI and get results
/////////////////////////////////////////////////////////////////////////////
int CNetworkView::serverAI(int iState, int iPlayer, int *iTrade)
{
	int iResult;
	int ID;
	int *iData = NULL;
	BOOL bClean = FALSE;

	//if we're not in lose cards mode, simply use the declared ID
	switch(iState)
	{
	case AI_STATE_LOSE_CARDS:
		iData = new int[5];
		bClean = TRUE;
		break;
	case AI_STATE_TRADE_OFFER:
		iData = iTrade;
		break;
	case AI_STATE_ROLL:
		ID = m_iRobber;
	default:
		iData = &ID;
		iPlayer = m_iPlayer;
		break;
	}

	//sanity
	if(-1 == iPlayer)
	{
		return AI_ERR_INVALID_PARAMS;
	}

	//if turn actions, find possible roads, cities and settlements
	if(AI_STATE_TURN == iState)
	{
		prepareRoads();
		prepareSettlements();
		prepareCities();
	}

	//run it
	iResult = APP->QueryAction(iState, iPlayer, m_pGame, &m_pBoard, iData);

	//do stuff with the outcome
	switch(iResult)
	{
	case AI_INITIAL_SETTLEMENT:
		//let base handling take care of it
		m_iSelection = *iData;
		handleSelectPlace();
		break;
	case AI_INITIAL_ROAD:
	case AI_BUILD_ROAD:
		//let base handling take care of it
		m_iSelection = *iData;
		handleSelectRoad();
		break;
	case AI_NORMAL_ROLL:
		//handle a goods click
		THISVIEW->getControlPanel()->OnAIGoods();
		break;
	case AI_SEVEN_ROLL:
		ID = *iData / 100; *iData -= (ID * 100);
		serverAISteal(*iData / 10, *iData % 10, ID);
		break;
	case AI_LOST_CARDS:
		serverAILoseCards(iResult, iPlayer, iData);
		break;
	case AI_TRADE_REJECT:
	case AI_TRADE_ACCEPT:
	case AI_TRADE_COUNTER:
		serverAITradeOffer(iPlayer, iResult, iData);
		break;
	case AI_BUILD_SETTLEMENT:
		//base handling
		m_iSelection = *iData;
		handleSelectSettle();
		break;
	case AI_BUILD_CITY:
		//base handling
		m_iSelection = *iData;
		handleSelectCity();
		break;
	case AI_BUY_DEVCARD:
		serverAIBuyCard(iPlayer);
		break;
	case AI_PLAY_SOLDIER:
		serverAIPlayCard(iPlayer, CARD_SOLDIER, *iData / 10, *iData % 10);
		break;
	case AI_PLAY_VICTORY:
		serverAIPlayCard(iPlayer, CARD_VICTORY);
		break;
	case AI_PLAY_YEAR:
		serverAIPlayCard(iPlayer, CARD_YEAR_PLENTY, *iData / 10, *iData % 10);
		break;
	case AI_PLAY_MONOPOLY:
		serverAIPlayCard(iPlayer, CARD_MONOPOLY, *iData);
		break;
	case AI_PLAY_ROAD:
		serverAIPlayCard(iPlayer, CARD_ROAD_BUILD, *iData);
		break;

	//error messages
	case AI_ERR_NO_DLL:
	case AI_ERR_NO_FUNC:
	case AI_ERR_INVALID_PARAMS:
	case AI_ERR_SHUTDOWN:
	default: break;
	}

	//clean up data pointer
	if(TRUE == bClean && NULL != iData)
	{
		delete iData;
		iData = NULL;
	}

	//return the result
	return iResult;
}

/////////////////////////////////////////////////////////////////////////////
// handle an AI placing the robber and stealing
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAISteal(int iTile, int iPlayer, BOOL bPirate)
{
	int iCard;

	//base handling
	m_iSelection = iTile;

	//set the person who played last
	if(TRUE == bPirate)
	{
		m_iPiratePlayer = m_iPlayer;
		handleSelectPirate();
	}
	else
	{
		m_iRobberPlayer = m_iPlayer;
		handleSelectRobber();
	}

	//now handle stealing
	iCard = getRandomCard(iPlayer);

	//handle stealing
	handleCommonSteal(iPlayer, iCard, bPirate);

	//tell others
	clientSteal(iPlayer, iCard, bPirate);
}

/////////////////////////////////////////////////////////////////////////////
// handle an AI losing cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAILoseCards(int &iResult, int iPlayer, int *iCards)
{
	CNDKMessage msg(S3D_LOSE_CARDS);

	//sanity
	if(NULL == iCards)
	{
		iResult = AI_ERR_INVALID_PARAMS;
		return;
	}

	//set data
	msg.SetAt(0, iPlayer);
	msg.SetAt(1, iCards, sizeof(int) * 5);

	//since this happens on the server, call the function directly
	OnLoseCards(msg, -1);
}

/////////////////////////////////////////////////////////////////////////////
// handle AI buying a dev card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAIBuyCard(int iPlayer)
{
	int iCard;

	//get the specific card that they're buying
	iCard = buyDevCard();

	//handle common
	handleCommonBuyDevCard(iPlayer, iCard);

	//handle network stuff
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		clientBuyDevCard(iCard);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle AI response to a trade offer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAITradeOffer(int iPlayer, int iReply, int *iOffer)
{
	int i;
	int iTo[5];
	int iFor[5];
	short shStatus;
	CNDKMessage msg(S3D_TRADE_REPLY);

	//decompile offer
	for(i = 0; i < 10; i++)
	{
		if(i < 5)
		{
			iTo[i] = iOffer[i];
		}
		else
		{
			iFor[i - 5] = iOffer[i];
		}
	}

	//set up parameters
	msg.SetAt(0, iPlayer);
	msg.SetAt(1, m_uiTradeID);

	//send the reponse
	switch(iReply)
	{
	case AI_TRADE_ACCEPT:
		shStatus = TRADE_ACCEPT;
		break;
	case AI_TRADE_COUNTER:
		shStatus = TRADE_COUNTER;
		break;
	case AI_TRADE_REJECT:
	default:
		shStatus = TRADE_REJECT;
		break;
	}

	//set the status
	msg.SetAt(2, shStatus);

	//handle the message
	OnTradeReply(msg);
}

/////////////////////////////////////////////////////////////////////////////
// handle AI playing a card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverAIPlayCard(int iPlayer, int iCard, int iData1, int iData2)
{
	CNDKMessage msg(S3D_PLAY_DEV_CARD);
	
	//set some stuff
	msg.SetAt(0, iPlayer);
	msg.SetAt(1, iCard);
	msg.SetAt(2, 0);
	msg.SetAt(3, 0);

	switch(iCard)
	{
	case CARD_MONOPOLY: 
		msg.SetAt(2, iData1); 
		break;
	case CARD_YEAR_PLENTY: 
		msg.SetAt(3, iData1); 
		msg.SetAt(4, iData2);
		break;
	default:
		break;
	}

	//send the dev card info
	OnPlayDevCard(msg, -10);

	//if it's a soldier, handle the steal
	if(CARD_SOLDIER == iCard)
	{
		serverAISteal(iData1, iData2, FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// someone dropped so we must save the state
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverSaveState()
{
	CNDKMessage msg(S3D_SAVE_STATE);

	//send it to everyone
	SendMessageToAllUsers(msg);

	//handle it ourselves
	OnSaveState(msg);
}

/////////////////////////////////////////////////////////////////////////////
// everyone has rejoined so we restore the state
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverRestoreState()
{
	CNDKMessage msg(S3D_RESTORE_STATE);

	//send it to everyone
	SendMessageToAllUsers(msg);

	//handle it ourselves
	OnRestoreState(msg);

	//update the connector if needed
	if(TRUE == CONNECTOR->getConnector())
	{
		CONNECTOR->setName("Now in game...");
		CONNECTOR->setRestart(FALSE);
		CONNECTOR->postServer(CONNECTOR_INGAME);
	}
}

/////////////////////////////////////////////////////////////////////////////
// resynch a rejoined player
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::serverResynchRejoin(long lUserID, int nPlayer)
{
	int nAction;
	int nData1 = -1, nData2 = -1, nTemp;
	int nResynch = 0;
	BOOL bInTurn = TRUE;
	list <CUndoNode>::iterator iter;
	CNDKMessage msg(S3D_REJOIN_SYNCH);

	//create the resynch string
	if(TRUE == m_saveState.isSaved())
	{
		nResynch |= RESYNCH_SAVE_STATE;
	}

	//set it to the end
	iter = m_pGame->m_listUndo.end();
	iter--;

	//get the action
	nAction = (*iter).m_iAction;

	//current turn or not
	if(m_iPlayer == nPlayer)
	{
		nResynch |= RESYNCH_TURN;

		//first or second place
		if(TRUE == m_bIncrease)
		{
			nResynch |= RESYNCH_INITIAL_FIRST;
		}

		//port placements
		if(TRUE == m_bPortPlacement)
		{
			nResynch |= RESYNCH_PORT_PLACE;

			//send the current port to place
			nData1 = m_nPortArray[m_nPortIndex];
		}
		//initial placements
		else if(FALSE == m_pGame->m_bInGame)
		{
			//determine third roadness
			if(TRUE == m_bTournamentRoads)
			{
				nResynch |= RESYNCH_INITIAL_THIRD_ROAD;
			}
			//determine if they have placed a city/settlement yet
			else if((UNDO_SETTLE_PLACE == nAction) || (UNDO_CITY == nAction))
			{
				nResynch |= RESYNCH_INITIAL_ROAD;

				if(UNDO_SETTLE_PLACE == nAction)
				{
					DECODE_INIT_SETTLEMENT(((CUndoOne *)(*iter).m_pData)->m_iOne, nTemp, nData1, nTemp, nTemp, nData2);
				}
				else
				{
					DECODE_CITY(((CUndoOne *)(*iter).m_pData)->m_iOne, nData1, nTemp, nTemp, nData2);
				}
			}
			else
			{
				nResynch |= RESYNCH_INITIAL_SETTLE;
			}
		}
		else
		{
			//determine if we are in a turn or preturn
			do
			{
				if(UNDO_ROLL == (*iter).m_iAction)
				{
					break;
				}
				else if((UNDO_ACTION_END == (*iter).m_iAction) || (UNDO_ROAD_PLACE == (*iter).m_iAction) || (UNDO_SHIP_PLACE == (*iter).m_iAction) || (UNDO_THIRD_ROAD_PLACE == (*iter).m_iAction))
				{
					bInTurn = FALSE;
					break;
				}

				iter--;
			}while(m_pGame->m_listUndo.begin() != iter);

			//check for losing cards on a 7
			if((0 != m_bWaitForLose.size()) && (FALSE == m_bWaitForLose[nPlayer]))
			{
				nResynch |= RESYNCH_LOSE_7;
			}
			else if((UNDO_ACTION_END == nAction) || (UNDO_ROAD_PLACE == nAction) || (UNDO_SHIP_PLACE == nAction) || (UNDO_THIRD_ROAD_PLACE == nAction))
			{
				nResynch |= RESYNCH_PRE_ROLL;
			}
			//see if we're in volcano state
			else if(UNDO_VOLCANO == nAction)
			{
				nResynch |= RESYNCH_SHOW_VOLCANO;
			}
			else if((UNDO_LOSECARDS == nAction) || (UNDO_PLAY_SOLDIER == nAction) || ((UNDO_ROLL == nAction) && (7 == ((CUndoRoll *)(*iter).m_pData)->m_iRoll)))
			{
				nResynch |= RESYNCH_ROBBER;

				//if we're in a turn, set it
				if(TRUE == bInTurn)
				{
					nData1 = 1;
				}
			}
			else if(UNDO_ROLL == nAction)
			{
				nResynch |= RESYNCH_SHOW_ROLLS;

				//set the data
				THISVIEW->getControlPanel()->getLastRoll(nData1, nData2);
			}
			else if(UNDO_ROBBER_PLACE == nAction)
			{
				nResynch |= RESYNCH_ROBBER_STEAL;
				nData1 = m_iRobber;

				//set turn
				if(TRUE == bInTurn)
				{
					nData2 = 1;
				}
			}
			else if(UNDO_PIRATE_PLACE == nAction)
			{
				nResynch |= RESYNCH_PIRATE_STEAL;
				nData1 = m_iPirate;

				//set turn
				if(TRUE == bInTurn)
				{
					nData2 = 1;
				}
			}
		}
	}
	else
	{
		//check for losing cards on a 7
		if((0 != m_bWaitForLose.size()) && (FALSE == m_bWaitForLose[nPlayer]))
		{
			nResynch |= RESYNCH_LOSE_7;
		}
	}

	//resynch data
	msg.SetAt(0, nResynch);
	msg.SetAt(1, nData1);
	msg.SetAt(2, nData2);

	//send it
	SendMessageToUser(lUserID, msg);
}