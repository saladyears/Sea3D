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
#include "defineNetwork.h"
#include "defineMap.h"
#include "defineCode.h"

/////////////////////////////////////////////////////////////////////////////
// reuse some code
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientDispatch(CNDKMessage &msg)
{
	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		SendMessageToAllUsers(msg);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle port placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientPortPlace(int nTile, int nSide, int nType)
{
	CNDKMessage msg(S3D_PORT);

	//set message
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, nTile);
	msg.SetAt(2, nSide);
	msg.SetAt(3, nType);

	//send it
	clientDispatch(msg);

	if(TRUE == m_pGame->m_bServer)
	{
		//send the message
		serverSysMessage(SYS_PORT_PLACE, m_iPlayer);

		//now continue the port placement
		serverPortPlace();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle player placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientSettlePlace(int ID)
{
	BOOL bAI;

	CNDKMessage msg(S3D_SETTLE_PLACE);

	//set current player
	msg.SetAt(0, ID);
	msg.SetAt(1, m_bIncrease);
	msg.SetAt(2, m_iPlayer);

	//dispatch
	clientDispatch(msg);

	//determine if the player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_SETTLE_PLACE, m_iPlayer);

		if(FALSE == m_pGame->m_bInGame)
		{
			//send road text message
			serverPanelText(S3D_ROAD_PLACE, m_iPlayer, bAI ? -10 : -1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle road placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientRoadPlace(int ID)
{
	CNDKMessage msg(S3D_ROAD_PLACE);

	//set current player
	msg.SetAt(0, ID);
	msg.SetAt(1, m_iPlayer);

	//dispatch
	clientDispatch(msg);

	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_ROAD_PLACE, m_iPlayer);

		if(TRUE == m_bTournamentRoads)
		{
			serverThirdRoadPlace();
		}
		else if(FALSE == m_pGame->m_bInGame)
		{
			//move to next player
			serverPlayerPlace();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a ship placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientShipPlace(int ID)
{
	CNDKMessage msg(S3D_SHIP_PLACE);

	//set current player
	msg.SetAt(0, ID);
	msg.SetAt(1, m_iPlayer);

	//dispatch
	clientDispatch(msg);

	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_SHIP_PLACE, m_iPlayer);

		if(FALSE == m_pGame->m_bInGame)
		{
			//move to next player
			serverPlayerPlace();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle ship movement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientShipMove(int ID)
{
	CNDKMessage msg(S3D_SHIP_MOVE);

	//set current player
	msg.SetAt(0, ID);
	msg.SetAt(1, m_iPlayer);

	//dispatch
	clientDispatch(msg);

	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_SHIP_MOVE, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a city placement
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientCityPlace(int ID)
{
	CNDKMessage msg(S3D_CITY_PLACE);

	//set current player
	msg.SetAt(0, ID);
	msg.SetAt(1, m_iPlayer);

	//dispatch
	clientDispatch(msg);

	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_CITY_PLACE, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a roll
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientRoll(int iDie1, int iDie2)
{
	int iRoll;
	CString str;
	CNDKMessage msg(S3D_ROLL);

	//set roll
	msg.SetAt(0, iDie1);
	msg.SetAt(1, iDie2);
	msg.SetAt(2, m_iPlayer);
	msg.SetAt(3, TRUE);

	//calculate roll
	iRoll = iDie1 + iDie2;

	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		if(TRUE == NEEDS_AN(iRoll))
		{
			str.Format("an %d.", iRoll);
		}
		else
		{
			str.Format("a %d.", iRoll);
		}
		serverSysMessage(SYS_ROLL, m_iPlayer, str);

		if(7 == iRoll)
		{
			serverWaitForLose(-1);
		}
		else
		{
			serverWaitForGold(-1);
		}
	}

	//dispatch
	clientDispatch(msg);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click off the goods
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientHideGoods()
{
	BOOL bAI;
	CNDKMessage msg(S3D_HIDE_GOODS);

	//determine AIness
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//dispatch
	clientDispatch(msg);

	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		//send wait message
		serverPanelText(S3D_START_TURN, m_iPlayer, bAI ? -10 : -1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a click off the volcano
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientHideVolcano()
{
	BOOL bAI;
	CNDKMessage msg(S3D_HIDE_VOLCANO);

	//determine AIness
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//dispatch
	clientDispatch(msg);

	//if we're the server, send this message to everyone
	if(TRUE == m_pGame->m_bServer)
	{
		//send wait message
		serverPanelText(S3D_START_TURN, m_iPlayer, bAI ? -10 : -1);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a volcanic eruption
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientVolcano(int nRoll)
{
	CNDKMessage msg(S3D_VOLCANO);

	//set the roll
	msg.SetAt(0, nRoll);

	//dispatch
	clientDispatch(msg);
}

/////////////////////////////////////////////////////////////////////////////
// handle someone ending their turn
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientEndTurn()
{
	CNDKMessage msg(S3D_END_TURN);

	//sanity check to make sure this person can actually end the turn
	if(m_pGame->m_iIndex != m_iPlayer && FALSE == m_pGame->m_players[m_iPlayer].m_player.getAI())
	{
		return;
	}

	//dispatch
	clientDispatch(msg);

	//if we're the server, just go to the next person
	if(TRUE == m_pGame->m_bServer)
	{
		//send message
		serverSysMessage(SYS_END_TURN, m_iPlayer);

		//go on to the next person
		serverStartTurn();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone placing the robber
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientPlaceRobber()
{
	CNDKMessage msg(S3D_PLACE_ROBBER);

	msg.SetAt(0, m_iRobber);

	//dispatch
	clientDispatch(msg);

	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_ROBBER_PLACE, m_iPlayer);
	}

	//send the goods
	msg = CNDKMessage(S3D_HIDE_GOODS);

	//dispatch
	clientDispatch(msg);
}

/////////////////////////////////////////////////////////////////////////////
// handle someone placing the pirate
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientPlacePirate()
{
	CNDKMessage msg(S3D_PLACE_PIRATE);

	msg.SetAt(0, m_iPirate);

	//dispatch
	clientDispatch(msg);

	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_PIRATE_PLACE, m_iPlayer);
	}

	//send the goods
	msg = CNDKMessage(S3D_HIDE_GOODS);

	//dispatch
	clientDispatch(msg);
}

/////////////////////////////////////////////////////////////////////////////
// send a message to the panel text
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientPanelText(long lMessage)
{
	CNDKMessage msg(S3D_PANEL_TEXT);
	
	//set the message
	msg.SetAt(0, lMessage);

	//if we're the server, just send the panel text
	if(TRUE == m_pGame->m_bServer)
	{
		serverPanelText(lMessage, m_iPlayer, -1);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone stealing a card with the robber
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientSteal(int iPlayer, int iRes, BOOL bPirate)
{
	CString str;
	CNDKMessage msg(S3D_STEAL);

	//set parameters
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iPlayer);
	msg.SetAt(2, iRes);
	msg.SetAt(3, bPirate);

	//dispatch
	clientDispatch(msg);

	//send a system message
	if(TRUE == m_pGame->m_bServer)
	{
		if(iRes != 5)
		{
			str.Format("1 card from %s.", m_pGame->m_players[iPlayer].m_player.getName());
		}
		else
		{
			str = "nothing.  How refreshing!";
		}
		serverSysMessage(SYS_STEAL, m_iPlayer, str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone trading with the bank
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientTradeToBank(int iToBank[5], int iFromBank[5], int iBank[3])
{
	CNDKMessage msg(S3D_TRADE_TO_BANK);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, (LPVOID) iToBank, 5 * sizeof(int));
	msg.SetAt(2, (LPVOID) iFromBank, 5 * sizeof(int));
	msg.SetAt(3, (LPVOID) iBank, 3 * sizeof(int));

	//dispatch
	clientDispatch(msg);

	//send the message
	if(TRUE == m_pGame->m_bServer)
	{
		//send a notification message
		serverSysMessage(SYS_TRADE_TO_BANK, m_iPlayer, getTradeToBankString(iToBank, iFromBank));
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone trading jungles
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientTradeJungle(int nJungles, int nFromBank[5])
{
	CNDKMessage msg(S3D_TRADE_JUNGLE);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, nJungles);
	msg.SetAt(2, (LPVOID) nFromBank, 5 * sizeof(int));

	//dispatch
	clientDispatch(msg);

	//send the message
	if(TRUE == m_pGame->m_bServer)
	{
		//send a notification message
		serverSysMessage(SYS_TRADE_JUNGLE, m_iPlayer, getTradeJungleString(nJungles, nFromBank));
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone buying a dev card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientBuyDevCard(int iCard)
{
	CNDKMessage msg(S3D_BUY_DEV_CARD);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iCard);

	//dispatch
	clientDispatch(msg);

	//send the notification message
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_BUY_DEV_CARD, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle someone playing a dev card
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientPlayDevCard(int iType, int iCard1, int iCard2)
{
	CNDKMessage msg(S3D_PLAY_DEV_CARD);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iType);
	msg.SetAt(2, iCard1);
	msg.SetAt(3, iCard2);

	//dispatch
	clientDispatch(msg);

	//send the notification message
	if(TRUE == m_pGame->m_bServer)
	{
		serverSysMessage(SYS_PLAY_DEV_CARD, m_iPlayer, getDevName(iType));
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle lose cards notifications
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientWaitForLose(BOOL bWait)
{
	CNDKMessage msg(S3D_WAIT_FOR_LOSE);

	//set data
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, bWait);

	//dispatch
	if(TRUE == m_pGame->m_bServer)
	{
		OnWaitForLose(msg);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle gold notifications
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientWaitForGold(int nGold)
{
	CNDKMessage msg(S3D_WAIT_FOR_GOLD);

	//set data
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, nGold);

	//dispatch
	if(TRUE == m_pGame->m_bServer)
	{
		OnWaitForGold(msg);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle selecting gold
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientSelectGold()
{
	CNDKMessage msg(S3D_SELECT_GOLD);

	//now send what they got
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, m_iGainedCards, sizeof(m_iGainedCards));

	//send it back to the server (or else just handle it)
	if(TRUE == m_pGame->m_bServer)
	{
		OnSelectGold(msg, -1);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle actual losing of cards
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientLoseCards(int *iRes)
{
	CNDKMessage msg(S3D_LOSE_CARDS);
	CNDKMessage msg2(S3D_WAIT_FOR_LOSE);

	//set data
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, iRes, sizeof(int) * 5);

	//dispatch
	clientDispatch(msg);

	//send a notification
	if(TRUE == m_pGame->m_bServer)
	{
		serverSysMessage(SYS_LOSE_CARDS, m_pGame->m_iIndex, getLoseString(iRes));

		//report ourselves as ready to continue
		msg2.SetAt(0, m_pGame->m_iIndex);
		msg2.SetAt(1, FALSE);

		//handle it
		OnWaitForLose(msg2);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a trade offer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientTradeOffer(int *iTo, int *iFor, CArray <UINT, UINT> &uiPlayers)
{
	int i;
	int iSize;
	CNDKMessage msg(S3D_TRADE_OFFER);

	//set current trade ID
	m_uiTradeID = (UINT) time(NULL);

	//set data
	iSize = uiPlayers.GetSize();
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iTo, sizeof(int) * 5);
	msg.SetAt(2, iFor, sizeof(int) * 5);
	msg.SetAt(3, m_uiTradeID);
	msg.SetAt(4, iSize);

	//add the players getting the message
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(5 + i, uiPlayers[i]);
	}

	//if we're the server, handle it ourselves
	if(TRUE == m_pGame->m_bServer)
	{
		OnTradeOffer(msg, -1);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// make a trade counteroffer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientTradeCounter(UINT uiID, int *iTo, int *iFor)
{
	CNDKMessage msg(S3D_TRADE_COUNTER);

	//set data
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, iTo, sizeof(int) * 5);
	msg.SetAt(2, iFor, sizeof(int) * 5);
	msg.SetAt(3, uiID);

	//if we're the server, handle it ourselves
	if(TRUE == m_pGame->m_bServer)
	{
		OnTradeCounter(msg);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// reply to a trade offer
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientReplyOffer(UINT uiID, short shStatus)
{
	CNDKMessage msg(S3D_TRADE_REPLY);

	//set data
	msg.SetAt(0, m_pGame->m_iIndex);
	msg.SetAt(1, uiID);
	msg.SetAt(2, shStatus);

	//dispatch
	if(TRUE == m_pGame->m_bServer)
	{
		OnTradeReply(msg);
	}
	else
	{
		SendMessageToServer(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// a trade has been finalized
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientTrade(int iPartner, int *iTo, int *iFor)
{
	CNDKMessage msg(S3D_TRADE);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iPartner);
	msg.SetAt(2, iTo, sizeof(int) * 5);
	msg.SetAt(3, iFor, sizeof(int) * 5);

	//dispatch
	clientDispatch(msg);

	//send system message
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_TRADE, m_iPlayer, getTradeString(iPartner, iTo, iFor));
	}
}

/////////////////////////////////////////////////////////////////////////////
// the player has swapped chitless tiles
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientChitless(int nTile1, int nTile2)
{
	CNDKMessage msg(S3D_CHITLESS);

	//set data
	msg.SetAt(0, nTile1);
	msg.SetAt(1, nTile2);

	//dispatch
	clientDispatch(msg);

	//send system message
	if(TRUE == m_pGame->m_bServer)
	{
		//send a system message
		serverSysMessage(SYS_CHITLESS, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// this player has won!
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientWin(int nPlayer)
{
	CNDKMessage msg(S3D_WIN);

	//set data
	msg.SetAt(0, nPlayer);

	//dispatch
	clientDispatch(msg);

	//if we're the server, handle the last time update
	if(TRUE == m_pGame->m_bServer)
	{
		serverTimeUpdate();

		//remove the saved in progress game
		THISVIEW->removeSave();
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo a build option
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientUndoBuild(int iType, int iUndo, int iOther, int iOther2, int iOther3)
{
	int iSys = 0;
	CNDKMessage msg(S3D_UNDO_BUILD);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iType);
	msg.SetAt(2, iUndo);
	msg.SetAt(3, iOther);
	msg.SetAt(4, iOther2);
	msg.SetAt(5, iOther3);

	//dispatch
	clientDispatch(msg);

	//send system message
	if(TRUE == m_pGame->m_bServer)
	{
		switch(iType)
		{
		case GL_MOVE_SHIP: iSys = SYS_UNDO_SHIP_MOVE; break;
		case GL_PLACE_ROAD: iSys = SYS_UNDO_ROAD; break;
		case GL_PLACE_SHIP: iSys = SYS_UNDO_SHIP; break;
		case GL_PLACE_CITY: iSys = SYS_UNDO_CITY; break;
		case GL_PLACE_SETTLEMENT: iSys = SYS_UNDO_SETTLEMENT; break;
		}

		//send sys message
		serverSysMessage(iSys, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo an auto-trade
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientUndoAutoTrade(int *iTo, int *iFrom, int *iBank)
{
	CNDKMessage msg(S3D_UNDO_AUTOTRADE);

	//set data
	msg.SetAt(0, m_iPlayer);
	msg.SetAt(1, iTo, 5 * sizeof(int));
	msg.SetAt(2, iFrom, 5 * sizeof(int));
	msg.SetAt(3, iBank, 3 * sizeof(int));

	//dispatch
	clientDispatch(msg);

	//send system message
	if(TRUE == m_pGame->m_bServer)
	{
		serverSysMessage(SYS_UNDO_AUTOTRADE, m_iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// we're done with the special build phase
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientSpecialBuildNext()
{
	CNDKMessage msg(S3D_SPECIAL_NEXT);

	//turn off the special build phase
	m_bSpecialBuild = FALSE;

	//toggle controls
	CONTROLS->ShowWindow(SW_HIDE);

	//if we're not the server, send it to the server
	if(FALSE == m_pGame->m_bServer)
	{
		SendMessageToServer(msg);
	}
	else
	{
		OnSpecialNext(msg);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle resynchs
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientResynch(int nResynch, int nData1, int nData2)
{
	//check for our turn
	if(!(RESYNCH_TURN & nResynch))
	{
		if(RESYNCH_LOSE_7 & nResynch)
		{
			//see if we need to lose cards
			handleLoseCards();
		}

		//set the message text
		MESSAGES->Title().SetText("Waiting for current player to continue.");

		return;
	}

	//otherwise, set everything the way it should be
	if(RESYNCH_PORT_PLACE & nResynch)
	{
		//set the res
		m_nPortRes = nData1;

		//set the drawing mode
		setNetDrawMode(GL_PLACE_PORT);
	}
	else if(RESYNCH_INITIAL_THIRD_ROAD & nResynch)
	{
		//not in game yet
		m_pGame->m_bInGame = FALSE;

		//turn on the flag
		m_bTournamentRoads = TRUE;

		//set the mode
		setNetDrawMode(GL_PLACE_THIRD_ROAD);
	}
	else if(RESYNCH_INITIAL_SETTLE & nResynch)
	{
		//set increase setting
		if(RESYNCH_INITIAL_FIRST & nResynch)
		{
			m_bIncrease = TRUE;
		}
		else
		{
			m_bIncrease = FALSE;
		}
		
		//see if it's a tournament city placement
		m_pGame->m_bInGame = FALSE;
		m_bCity = TRUE;
		setNetDrawMode(GL_PLACE_PLAYERS);
	}
	else if(RESYNCH_INITIAL_ROAD & nResynch)
	{
		m_bCity = FALSE;
		m_pGame->m_bInGame = FALSE;

		//set increase setting
		if(RESYNCH_INITIAL_FIRST & nResynch)
		{
			m_bIncrease = TRUE;
		}
		else
		{
			m_bIncrease = FALSE;
		}

		//encode the last city
		m_iLastCity = ENCODE_SEL(nData1, nData2);

		//check for settle place
		if(TRUE == handleCommonInitialShipCheck(nData1, nData2))
		{
			return;
		}
		else
		{
			prepareInitialRoads();
			setNetDrawMode(GL_PLACE_PLAYERS);
			MESSAGES->Title().SetText("Your turn to place a road.");
		}
	}
	else if(RESYNCH_PRE_ROLL & nResynch)
	{
		CONTROLS->ShowWindow(SW_SHOW);
		THISVIEW->getControlPanel()->postBuildPhase();
		THISVIEW->getControlPanel()->setPanelText("It is your turn.  Click to roll.");
	}
	else if(RESYNCH_SHOW_ROLLS & nResynch)
	{
		THISVIEW->getControlPanel()->setLastRoll(nData1, nData2);
		THISVIEW->getControlPanel()->showGoods(TRUE);
		ACTIONS->Goods().EnableWindow();
		ACTIONS->ShowWindow(SW_SHOW);
		CONTROLS->ShowWindow(SW_SHOW);
		THISVIEW->getControlPanel()->setPanelText("Click or press the spacebar to continue.");
	}
	else if(RESYNCH_SHOW_VOLCANO & nResynch)
	{
		THISVIEW->getControlPanel()->showVolcano(TRUE);
		ACTIONS->Goods().EnableWindow();
		ACTIONS->ShowWindow(SW_SHOW);
		CONTROLS->ShowWindow(SW_SHOW);
	}
	else if(RESYNCH_LOSE_7 & nResynch)
	{
		//see if we need to lose cards
		handleLoseCards();
	}
	else if(RESYNCH_ROBBER & nResynch)
	{
		setNetDrawMode(GL_PLACE_ROBBER);
		//if we're in a turn, set the turn
		if(1 == nData1)
		{
			THISVIEW->getControlPanel()->setTurn();
		}
		THISVIEW->getControlPanel()->updateText();
	}
	else if(RESYNCH_ROBBER_STEAL & nResynch)
	{
		//set the robber
		m_iRobber = nData1;

		//if we're in a turn, set the turn
		if(1 == nData2)
		{
			THISVIEW->getControlPanel()->setTurn();
		}
		
		//run the steal
		handleCommonInitSteal(FALSE);
	}
	else if(RESYNCH_PIRATE_STEAL & nResynch)
	{
		//set the pirate
		m_iPirate = nData1;

		//if we're in a turn, set the turn
		if(1 == nData2)
		{
			THISVIEW->getControlPanel()->setTurn();
		}

		//run the steal
		handleCommonInitSteal(FALSE, TRUE);
	}
	//otherwise, we're just in our turn and can act normally
	else
	{
		THISVIEW->getControlPanel()->enterTurn();
		CONTROLS->ShowWindow(SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle client readiness
/////////////////////////////////////////////////////////////////////////////
void CNetworkView::clientReady(UINT uiID, BOOL bReady)
{
	CNDKMessage msg(S3D_READY_STATE);

	//set data
	msg.SetAt(0, uiID);
	msg.SetAt(1, bReady);

	//handle it ourselves
	OnReadyState(msg);

	//dispatch it to everyone else
	clientDispatch(msg);
}