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
#include "GLView.h"
#include "SettlersView.h"
#include "PlayerDatabase.h"
#include "WinDialog.h"
#include "VictoryPointDialog.h"
#include "UndoOne.h"
#include "defineStat.h"
#include "defineSounds.h"
#include "defineCode.h"
#include "defineNetwork.h"
#include "defineConnector.h"
#include "defineVariants.h"
#include "defineMap.h"

/////////////////////////////////////////////////////////////////////////////
// common code for gaining resources from gold
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonGold()
{
	int i, j;

	//increment the players resources
	for(i = 0; i < m_iNumPlayers; i++)
	{
		for(j = 0; j < 5; j++)
		{
			incResource(RES_STAT_GOLD, i, j, m_iGainedCards[i][j]);
		}

		//update the player counts
		THISVIEW->updateCounts(i);
	}

	//update the amounts players have
	addUndo(UNDO_GOLD, new CUndoGold((int *) m_iGainedCards));

	//update the stats
	STAT->update(STAT_GOODS_RECV_GOLD);
}

/////////////////////////////////////////////////////////////////////////////
// common code for setting a port
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPortPlace(int nTile, int nSide, int nType)
{
	//set the port
	m_pBoard[nTile].setType(nType);

	//set the port values
	m_pBoard[nTile].setPort1(nSide);
	m_pBoard[nTile].setPort2((nSide + 1) % 6);

	//set the rotation
	m_pBoard[nTile].setRotation(((nSide + 4) % 6) * 60.);

	//add it to the port list
	m_iPorts.Add((nType << 16) + nTile);
}

/////////////////////////////////////////////////////////////////////////////
// common code for a tile being revealed
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonChitless(int nTile1, int nTile2)
{
	int ID;

	//set the chitless tile's new roll and tex
	m_pBoard[nTile1].setRoll(m_pBoard[nTile2].getRoll());
	m_pBoard[nTile1].setRollTex(m_pBoard[nTile2].getRollTex());
	m_pBoard[nTile1].setRollTexType(m_pBoard[nTile2].getRollTexType());
	m_pBoard[nTile1].setChitless(FALSE);

	//now set the old tile
	m_pBoard[nTile2].setRoll(0);
	m_pBoard[nTile2].setRollTex(0);
	m_pBoard[nTile2].setChitless(TRUE);

	//create the undo id
	ID = ENCODE_CHITLESS(nTile1, nTile2);

	//add the undo
	addUndo(UNDO_CHITLESS, new CUndoOne(ID));

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// common code for losing cards
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonLoseCards(int iPlayer, int *iRes)
{
	int i;
	int iTotal = 0;

	//add to the total
	for(i = 0; i < 5; i++)
	{
		iTotal += iRes[i];

		//lose resources
		decResource(RES_STAT_LOST_7, iPlayer, i, iRes[i]);
	}

	//see if we can add this
	if(0 < iTotal)
	{
		//use the trade undo structure
		addUndo(UNDO_LOSECARDS, new CUndoTrade(iPlayer, iRes, iRes));
	}

	//update the player
	THISVIEW->updateCounts(iPlayer);

	//update stats
	STAT->update(STAT_GOODS_LOST_7);
}

/////////////////////////////////////////////////////////////////////////////
// common win code
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonWin(int iPlayer)
{
	int i;
	int iCards;
	int iPoints;
	CString str;
	CWinDialog dlg;
	CVictoryPointDialog vic;

	//show that they won
	if(GAME_MODE_NETWORK != m_pGame->m_iMode)
	{
		//play sound
		SOUND(SOUND_WIN);

		//set the name of the winner
		dlg.setWinner(m_pGame->m_players[iPlayer].m_player.getName());

		//if they cancelled, undo the last action
		if(dlg.DoModal() == IDCANCEL)
		{
			undo();
			return;
		}
	}
	else
	{
		//play sound
		if(iPlayer == m_pGame->m_iIndex)
		{
			SOUND(SOUND_WIN);
		}
		else
		{
			SOUND(SOUND_LOSE);
		}

		str.Format("%s won the game!", m_pGame->m_players[iPlayer].m_player.getName());
		MessageBox(str, "Game over");
	}

	//set the winning player
	m_pGame->m_iWinner = iPlayer;

	//hide all windows
	THISVIEW->showBuildOptions(FALSE);

	//otherwise, the game is done
	addUndo(UNDO_WIN, new CUndoOne(iPlayer));

	//now see if any players have remaining unplayed victory points
	for(i = 0; i < m_iNumPlayers; i++)
	{
		iCards = m_pGame->m_players[i].m_iCardHeld[CARD_VICTORY];

		//check this users cards
		if(0 == iCards)
		{
			continue;
		}

		//the number of points they got was just the number of cards they had left
		iPoints = iCards;

		//set to zero
		m_pGame->m_players[i].m_iCardHeld[CARD_VICTORY] = 0;

		//increment this person's total points
		incPoints(i, iPoints);

		//add the undo
		if(iPlayer != i) 
		{
			addUndo(UNDO_WASTE, new CUndoOne(ENCODE_SEL(i, iPoints)));
		}

		//update the players counts
		THISVIEW->updateCounts(i);
	}

	//set game state
	m_pGame->m_iState = GAME_COMPLETE;

	//save the game
	saveGame();

	//no longer add players from games (removed in 1.1.8)

	//set the drawing mode
	m_iMode = GL_GAME_OVER;

	//disable all options
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//hide build options
	THISVIEW->showBuildOptions(FALSE);

	//update the player panel
	THISVIEW->getControlPanel()->updateText();

	//if we're in network mode and we're the server, show the drop to dialogs button
	if(GAME_MODE_NETWORK == m_pGame->m_iMode && TRUE == m_pGame->m_bLadder)
	{
		if(TRUE == m_pGame->m_bServer)
		{
			//DISABLED UNTIL I CAN GET IT TO WORK
			//THISVIEW->getControlPanel()->setGameOver();

			//see if the connector needs to be updated
			if(TRUE == CONNECTOR->getConnector())
			{
				CONNECTOR->postServer(CONNECTOR_COMPLETE);
			}

			//clear any quits from the game
			CONNECTOR->clearQuits();
		}

		//post the stats!
		CONNECTOR->postLadder(GAME);
		MessageBox("This game has been successfully posted to the ladder ranking.", "Sea3D Connector");
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a turn time update
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonTimeUpdate(int iPlayer, int iSeconds)
{
	//convert to seconds and add to the player's stack
	m_pGame->m_players[iPlayer].m_iTurnTime.push_back(iSeconds);

	//update the stats
	STAT->update(STAT_TIME_TURN);

	//if we're in network mode, keep setting the ending time to the current time
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		m_pGame->m_timeEnd = COleDateTime::GetCurrentTime();
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code to handle playing a soldier card
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPlaySoldier(int iPlayer)
{
	int nArmy;
	CString str;
	CString strName;

	//subtract one from the players playable dev cards
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_SOLDIER]--;

	//update the player data
	m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_SOLDIER]++;

	//get largest army info
	nArmy = largestArmy();

	//play the sound if they didn't get largeset army
	if((-1 != nArmy) && (0 > nArmy))
	{
		SOUND(SOUND_PLAY_SOLDIER);
	}

	//add the undo information
	addUndo(UNDO_PLAY_SOLDIER, new CUndoOne(nArmy));

	//set the person who played the robber last
	m_iRobberPlayer = iPlayer;

	//update player button
	THISVIEW->updateCounts(iPlayer);

	//they can't play another one until next turn
	m_pGame->m_players[iPlayer].m_bPlayedCard = TRUE;

	//if we're in network mode and it's not our turn, then do nothing
	if(GAME_MODE_NETWORK == GAME->m_iMode && iPlayer != GAME->m_iIndex)
	{
		//get the name of the punk who's laying down the robber
		strName = GAME->m_players[iPlayer].m_player.getName();

		//set the text
		str.Format("Waiting for %s to place the robber.", strName);
		THISVIEW->getControlPanel()->setPanelText(str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle common trading to bank
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonTradeToBank(int iPlayer, int iTradedTo[5], int iTradedFor[5], int iBank[3], BOOL bAuto)
{
	int i;
	CString strTrade;
	CString strTo, strFor;
	CString strTemp;

	//play sound
	SOUND(SOUND_TRADE_BANK);

	//now inc/dec resources
	for(i = 0; i < 5; i++)
	{
		//handle resources they traded in to the bank
		if(iTradedTo[i])
		{
			decResource(RES_STAT_TRADE_BANK, iPlayer, i, iTradedTo[i]);
		}

		//handle resources they traded for from the bank
		if(iTradedFor[i])
		{
			incResource(RES_STAT_TRADE_BANK, iPlayer, i, iTradedFor[i]);
		}
	}

	//increase trade totals
	m_pGame->m_players[iPlayer].m_iTradesBank[TRADE_2FOR1] += iBank[TRADE_2FOR1];
	m_pGame->m_players[iPlayer].m_iTradesBank[TRADE_3FOR1] += iBank[TRADE_3FOR1];
	m_pGame->m_players[iPlayer].m_iTradesBank[TRADE_4FOR1] += iBank[TRADE_4FOR1];
	
	//update stats
	STAT->update(STAT_TRADE_NUMBER);

	//update the players ocunts
	THISVIEW->updateCounts(iPlayer);

	//add the trade to the list
	if(TRUE == bAuto)
	{
		addUndo(UNDO_TRADE, new CUndoTrade(-2, iTradedTo, iTradedFor, iBank));
	}
	else
	{
		addUndo(UNDO_TRADE, new CUndoTrade(-1, iTradedTo, iTradedFor, iBank));
	}

	//start of the trade string
	strTrade = "Traded";

	//now create the string and handle resource trading
	for(i = 0; i < 5; i++)
	{
		//look at traded to
		if(iTradedTo[i])
		{
			strTemp.Format(" %d %s,", iTradedTo[i], getResName(i, TRUE));
			strTo += strTemp;
		}

		//look at the traded for
		if(iTradedFor[i])
		{
			strTemp.Format(" %d %s,", iTradedFor[i], getResName(i, TRUE));
			strFor += strTemp;
		}
	}

	//now trim the strings and create the large one
	strTo = strTo.Left(strTo.GetLength() - 1);
	strFor = strFor.Left(strFor.GetLength() - 1);

	//add them on
	strTrade += strTo;
	strTrade += " to the bank for";
	strTrade += strFor;
	strTrade += ".";

	//let the user know what happened
	if(GAME_MODE_NETWORK != m_pGame->m_iMode && TRUE == bAuto)
	{
		MessageBox(strTrade, "Trade to the bank");
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle jungle trading
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonTradeToJungle(int nPlayer, int nJungles, int nFromBank[5])
{
	int i;

	//play the sound
	SOUND(SOUND_JUNGLE);

	//reduce the player's jungles
	m_pGame->m_players[nPlayer].m_nJungles -= nJungles;

	//do the resources
	for(i = 0; i < 5; i++)
	{
		//update the resources
		if(0 < nFromBank[i])
		{
			incResource(RES_STAT_TRADE_JUNGLE, nPlayer, i, nFromBank[i]);
		}
	}

	//add the undo
	addUndo(UNDO_TRADE_JUNGLE, new CUndoTrade(nJungles, NULL, nFromBank, NULL));
}

/////////////////////////////////////////////////////////////////////////////
// handle trading
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonTrade(int iPlayer, int iPartner, int *iTradeTo, int *iTradeFor)
{
	int i;

	//play sound
	SOUND(SOUND_TRADE);

	//basics
	for(i = 0; i < 5; i++)
	{
		//increment the number of trades between these two
		GAME->m_players[iPlayer].m_iOtherTradeTo[iPartner][i] += iTradeTo[i];
		GAME->m_players[iPartner].m_iOtherTradeTo[iPlayer][i] += iTradeFor[i];

		//increment the number of goods received from each other
		GAME->m_players[iPlayer].m_iOtherTradeFrom[iPartner][i] += iTradeFor[i];
		GAME->m_players[iPartner].m_iOtherTradeFrom[iPlayer][i] += iTradeTo[i];

		//increment and decrement from players
		VIEW->incResource(RES_STAT_TRADE, iPlayer, i, iTradeFor[i], iPartner);
		VIEW->incResource(RES_STAT_TRADE, iPartner, i, iTradeTo[i], iPlayer);
		VIEW->decResource(RES_STAT_TRADE, iPlayer, i, iTradeTo[i], iPartner);
		VIEW->decResource(RES_STAT_TRADE, iPartner, i, iTradeFor[i], iPlayer);
	}

	//increase the trade numbers for these players
	GAME->m_players[iPlayer].m_iTradesOthers[iPartner]++;
	GAME->m_players[iPartner].m_iTradesOthers[iPlayer]++;

	//update the player panel
	VIEW->updateCounts(iPartner);
	VIEW->updateCounts(iPlayer);

	//update trade data
	STAT->update(STAT_TRADE_NUMBER);

	//add this trade to the undo pile
	VIEW->addUndo(UNDO_TRADE, new CUndoTrade(iPartner, iTradeTo, iTradeFor));
}

/////////////////////////////////////////////////////////////////////////////
// common steal code
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonSteal(int iPlayer, int iRes, BOOL bPirate)
{
	int ID;

	//play sound
	SOUND(SOUND_STEAL);

	//create the ID
	ID = ENCODE_STEAL(bPirate, iPlayer, iRes);

	//add it
	VIEW->addUndo(UNDO_STEAL, new CUndoOne(ID));

	//change resources from the players
	if(iRes != 5)
	{
		VIEW->incResource(RES_STAT_STOLEN, m_iPlayer, iRes, 1, iPlayer);
		VIEW->decResource(RES_STAT_LOST_STOLEN, iPlayer, iRes, 1, m_iPlayer);

		//update the player panel
		VIEW->updateCounts(m_iPlayer);
		VIEW->updateCounts(iPlayer);

		//update the stats
		STAT->update(STAT_GOODS_RECV_STEAL);
		STAT->update(STAT_GOODS_LOST_STEAL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle losing cards on a 7 roll
/////////////////////////////////////////////////////////////////////////////
int CGLView::handleLoseCards()
{
	int i;

	//reset lost total
	m_iLostTotal = 0;

	//run through and see if anyone loses cards
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//if we're in network mode, only check ourselves
		if(GAME_MODE_NETWORK == m_pGame->m_iMode && i != m_pGame->m_iIndex)
		{
			continue;
		}

		//see if they're over
		if(m_pGame->m_players[i].totalCards() > m_iAllowedCards)
		{
			m_iLostTotal++;
		}
	}

	//see if we need to run anything
	if(0 < m_iLostTotal)
	{
		//if we're hotseat or the server, just run normally
		if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) || (TRUE == m_pGame->m_bServer))
		{
			runLoseCards();
		}
		//otherwise, kick off the timer
		else
		{
			//start the timer
			m_uiTimer = SetTimer(TIMER_LOSE_CARDS, 1000, NULL);
		}
	}
	//if we're in network mode, let the server know we can continue
	else if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientWaitForLose(FALSE);
	}

	//return how many players lost cards
	return m_iLostTotal;
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a victory card
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPlayVictory(int iPlayer)
{
	CString str;

	//play the sound
	SOUND(SOUND_PLAY_VICTORY);
	
	//add one to the player's victory points
	incPoints(iPlayer);

	//decrement cards on hand
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_VICTORY]--;

	//update the player data
	m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_VICTORY]++;

	//add the undo
	addUndo(UNDO_PLAY_VICTORY, new CUndoOne(iPlayer));

	//update the panel
	THISVIEW->updateCounts(iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a monopoly card
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPlayMonopoly(int iPlayer, int iType)
{
	int i;
	int iRes;
	int iPlayers[6];
	CString str, strTemp;

	//play sound
	SOUND(SOUND_PLAY_MONOPOLY);

	//reset the trade array
	::ZeroMemory(iPlayers, sizeof(iPlayers));

	//suck all the resources from everyone else
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		//skip self
		if(i == iPlayer) continue;

		//get res of this player
		iRes = m_pGame->m_players[i].m_iRes[iType];

		//see if this player has any of this card
		if(0 < iRes)
		{
			//add them to the list
			strTemp.Format("%d %s from %s, ", iRes, getResName(iType, TRUE), m_pGame->m_players[i].m_player.getFirstName());

			//add to the list
			str += strTemp;

			//set the number stolen
			iPlayers[i] = m_pGame->m_players[i].m_iRes[iType];

			//steal it from players
			incResource(RES_STAT_MONOPOLY, iPlayer, iType, m_pGame->m_players[i].m_iRes[iType], i);
			decResource(RES_STAT_LOST_STOLEN, i, iType, m_pGame->m_players[i].m_iRes[iType], iPlayer);

			//update the counts of the player stolen from
			THISVIEW->updateCounts(i);
		}
	}

	//update the stats
	STAT->update(STAT_GOODS_RECV_STEAL);
	STAT->update(STAT_GOODS_LOST_STEAL);

	//remove the final comma
	if(0 < str.GetLength())
	{
		str = str.Left(str.GetLength() - 2);

		//now send it
		if((GAME_MODE_NETWORK == m_pGame->m_iMode) && (TRUE == m_pGame->m_bServer))
		{
			THISVIEW->serverSysMessage(SYS_MONOPOLY, iPlayer, str);
		}
	}

	//add the undo
	addUndo(UNDO_PLAY_MONOPOLY, new CUndoMonopoly(iType, iPlayers));

	//subtract the playable card count
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_MONOPOLY]--;

	//update the player data
	m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_MONOPOLY]++;

	//update the panel
	THISVIEW->updateCounts(iPlayer);

	//they can't play another one until next turn
	m_pGame->m_players[iPlayer].m_bPlayedCard = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a road building card
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPlayRoad(int iPlayer)
{
	//play the sound
	SOUND(SOUND_PLAY_ROAD_BUILD);

	//add the undo
	addUndo(UNDO_PLAY_ROAD, NULL);

	//subtract the playable card count
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_ROAD_BUILD]--;

	//update the player data
	m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_ROAD_BUILD]++;

	//let the user build two free roads
	playRoadBuild();

	//update the panel
	THISVIEW->updateCounts(iPlayer);

	//they can't play another one until next turn
	m_pGame->m_players[iPlayer].m_bPlayedCard = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a year of plenty card
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPlayYear(int iPlayer, int iCard1, int iCard2)
{
	int ID = 0;

	//play the sound
	SOUND(SOUND_PLAY_YEAR);

	//create the undo ID
	ID += (int) pow(10., (4 - iCard1));
	ID += (int) pow(10., (4 - iCard2));

	//increase resources
	incResource(RES_STAT_DEV, iPlayer, iCard1);
	incResource(RES_STAT_DEV, iPlayer, iCard2);

	//update the stats
	STAT->update(STAT_GOODS_RECV_DEVCARD);

	//update the player panel
	THISVIEW->updateCounts(iPlayer);

	//add the undo
	addUndo(UNDO_PLAY_YEAR, new CUndoOne(ID));

	//subtract the playable card count
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_YEAR_PLENTY]--;

	//update the player data
	m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_YEAR_PLENTY]++;

	//update the panel
	THISVIEW->updateCounts(iPlayer);

	//they can't play another one until next turn
	m_pGame->m_players[iPlayer].m_bPlayedCard = TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handle common buy cards code
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonBuyDevCard(int iPlayer, int iCard)
{
	//dec the resources to the bank
	VIEW->decResource(RES_STAT_BUILD, iPlayer, RES_ORE);
	VIEW->decResource(RES_STAT_BUILD, iPlayer, RES_WHEAT);
	VIEW->decResource(RES_STAT_BUILD, iPlayer, RES_SHEEP);

	//recheck trading
	THISVIEW->getControlPanel()->EnableTrade();

	//increment it in the right array
	if(CARD_VICTORY == iCard)
	{
		GAME->m_players[iPlayer].m_iCardHeld[iCard]++;
	}
	//can't play it on this turn
	else
	{
		GAME->m_players[iPlayer].m_iCardBought[iCard]++;
	}

	//update the player panel
	THISVIEW->updateCounts(iPlayer);

	//add the undo
	addUndo(UNDO_BUY_DEVCARD, new CUndoOne(ENCODE_SEL(iCard, iPlayer)));
}

/////////////////////////////////////////////////////////////////////////////
// handles common parts of placing a first settlement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonSettlePlace(int iPlayer, int iT1, int iC1)
{
	int ID;
	int iT2, iT3;
	int iC2, iC3;
	int nLongest;
	int iPortTile = -1;
	int iPort = -1;
	BOOL bPort = FALSE;

	//get the other two tiles
	iT2 = m_pBoard[iT1].getSide(iC1);
	iT3 = m_pBoard[iT1].getSide((iC1 + 5) % 6);

	//set opposite corners
	iC2 = (iC1 + 4) % 6;
	iC3 = (iC1 + 2) % 6;

	//see if we're using tournament start variant
	if((FALSE == m_pGame->m_bInGame) && (FALSE == m_bIncrease) && (IS_VARIANT(VARIANT_TOURNAMENT_START)))
	{
		//add it as a city
		handleCommonAddCity(m_iPlayer, iT1, iC1);
	}
	else
	{
		//add the settlements to the board
		handleCommonAddSettlement(iPlayer, iT1, iC1);
	}

	//create the settlement ids
	m_iLastCity  = ENCODE_SEL(iT1, iC1);

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iT1].getType()))
	{
		iPortTile = iT1; iPort = iC1;
	}

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iT2].getType())) 
	{
		iPortTile = iT2; iPort = iC2;
	}

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iT3].getType())) 
	{
		iPortTile = iT3; iPort = iC3;
	}

	//play the sound
	if(iPortTile > -1 && ((iPort == m_pBoard[iPortTile].getPort1()) ||
		(iPort == m_pBoard[iPortTile].getPort2())))
	{
		//only add to the list if the trade and build order is turned off
		if(!IS_VARIANT(VARIANT_TRADE_AFTER_BUILD))
		{
			m_nPortsThisTurn.Add(iPortTile);
		}
		bPort = TRUE;
	}

	if(FALSE == bPort)
	{
		if((FALSE == m_pGame->m_bInGame) && (FALSE == m_bIncrease) && (IS_VARIANT(VARIANT_TOURNAMENT_START)))
		{
			SOUND(SOUND_BUILD_CITY);
		}
		else
		{
			SOUND(SOUND_BUILD_SETTLEMENT);
		}
	}
	else
	{
		SOUND(SOUND_BUILD_PORT);
	}

	//act differently if it's initial placement
	if(FALSE == m_pGame->m_bInGame)
	{
		//create the undo ID
		ID = ENCODE_INIT_SETTLEMENT(iPlayer, iT1, iT2, iT3, iC1);

		//move to road mode
		m_bCity = FALSE;

		//add the undo
		addUndo(UNDO_SETTLE_PLACE, new CUndoOne(ID));

		//if this is the second city, increase the requisite resources from bank
		if(FALSE == m_bIncrease)
		{
			incResource(RES_STAT_BOARD, iPlayer, m_pBoard[iT1].getType());
			incResource(RES_STAT_BOARD, iPlayer, m_pBoard[iT2].getType());
			incResource(RES_STAT_BOARD, iPlayer, m_pBoard[iT3].getType());

			//update jungles if needed
			if(RES_JUNGLE == m_pBoard[iT1].getType())
			{
				m_pGame->m_players[iPlayer].m_nJungles++;
			}
			if(RES_JUNGLE == m_pBoard[iT2].getType())
			{
				m_pGame->m_players[iPlayer].m_nJungles++;
			}
			if(RES_JUNGLE == m_pBoard[iT3].getType())
			{
				m_pGame->m_players[iPlayer].m_nJungles++;
			}

			THISVIEW->updateCounts(iPlayer);

			//update the stats
			STAT->update(STAT_GOODS_RECV_BOARD);
		}
		//if we're using trade routes, set the player's home island
		else if(TRUE == MAP->m_bUsesTradeRoutes)
		{
			m_pGame->m_players[iPlayer].m_nHomeIsland = m_pBoard[iT1].getIsland();
		}

		//turn on undo possibility
		if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) && (0 == iPlayer) && m_bIncrease)
		{
			THISVIEW->getMessageButton()->enableUndo(TRUE);
		}

		//redraw
		Invalidate();

		return;
	}

	//in game handling

	//decrement one clay, timber, wheat, and sheep to the bank
	decResource(RES_STAT_BUILD, iPlayer, RES_TIMBER);
	decResource(RES_STAT_BUILD, iPlayer, RES_CLAY);
	decResource(RES_STAT_BUILD, iPlayer, RES_SHEEP);
	decResource(RES_STAT_BUILD, iPlayer, RES_WHEAT);

	//update the player panel
	THISVIEW->updateCounts(iPlayer);

	//get longest road value
	nLongest = handleCommonLongestCheck(iPlayer);

	//create the undo ID
	ID = ENCODE_SETTLEMENT(nLongest, iT1, iT2, iT3, iC1);

	//add the undo
	addUndo(UNDO_SETTLEMENT, new CUndoOne(ID));

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles common parts of placing a first road
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonRoadPlace(int iPlayer, int iT1, int iC1)
{
	int i;
	int ID;
	int iC2, iT2;
	int nTurn;
	int iLongest;
	int nHiddenTile;
	int nChitless = -1;
	BOOL bFree;

	//calculate turn
	nTurn = m_pGame->m_iRollList.size();

	//play the sound
	SOUND(SOUND_BUILD_ROAD);

	//find other side
	iT2 = m_pBoard[iT1].getSide(iC1);

	//add the roads
	m_pBoard[iT1].addRoad(iC1, iPlayer);
	m_pBoard[iT1].setTurnPlacement(iC1, nTurn);

	//second tile
	iC2 = (iC1 + 3) % 6;
	m_pBoard[iT2].addRoad(iC2, iPlayer);
	m_pBoard[iT2].setTurnPlacement(iC2, nTurn);

	//subtract one from the players stock
	m_pGame->m_players[iPlayer].m_nStockRoads--;

	//check for hidden tile
	nHiddenTile = checkForHiddenTile(iT1, iC1);

	//check for chitless tile if it's this player's turn
	if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) || (m_pGame->m_iIndex == m_iPlayer) || (m_nChitlessChitIndex < (int) m_nChitlessChitArray.size()))
	{
		checkForChitlessTile(iT1, iC1, nChitless);
	}

	//go back to city mode
	if(FALSE == m_pGame->m_bInGame)
	{
		//create the undo ID
		ID = ENCODE_INIT_ROAD(iT1, iC1, iT2, iC2, m_iLastCity);

		//add the undo
		if(TRUE == m_bTournamentRoads)
		{
			addUndo(UNDO_THIRD_ROAD_PLACE, new CUndoShip(ID, nHiddenTile, nChitless));
		}
		else
		{
			addUndo(UNDO_ROAD_PLACE, new CUndoShip(ID, nHiddenTile, nChitless));
		}

		m_bCity = TRUE;

		//update all player counts
		for(i = 0; i < m_iNumPlayers; i++)
		{
			THISVIEW->updateCounts(i);
		}

		//redraw
		Invalidate();

		//done
		return;
	}

	//see if it's free
	bFree = m_iFreeRoads ? TRUE : FALSE;

	//if they played road building, subtract one from the number played
	if(TRUE == bFree)
	{
		m_iFreeRoads--;
	}
	else
	{
		//decrement one clay and one timber to the bank
		decResource(RES_STAT_BUILD, iPlayer, RES_TIMBER);
		decResource(RES_STAT_BUILD, iPlayer, RES_CLAY);
	}

	//check for longest road
	iLongest = longestRoad(iPlayer);

	//create the undo ID
	ENCODE_ROAD(ID, bFree, iT1, iC1, iT2, iC2, iLongest);

	//see if they've just got it
	handleCommonLongestPoints(iPlayer, iLongest);

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//update all player counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->updateCounts(i);
	}

	//create the undo
	addUndo(UNDO_ROAD, new CUndoShip(ID, nHiddenTile, nChitless));

	//reenable undo if possible
	if(GAME_MODE_HOTSEAT == GAME->m_iMode || ((-1 == nChitless) && (-1 == nHiddenTile) && (m_iPlayer == m_pGame->m_iIndex)))
	{
		THISVIEW->getMessageButton()->enableUndo(TRUE);
	}
	
	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles common parts of placing a ship
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonShipPlace(int iPlayer, int iT1, int iC1)
{
	int i;
	int ID;
	int iC2, iT2;
	int iLongest;
	int nHiddenTile;
	int nTurn;
	BOOL bFree;
	int nChitless = -1;

	//calculate turn
	nTurn = m_pGame->m_iRollList.size();

	//play the sound
	SOUND(SOUND_BUILD_SHIP);

	//find other side
	iT2 = m_pBoard[iT1].getSide(iC1);

	//add the roads
	m_pBoard[iT1].addShip(iC1, iPlayer);
	m_pBoard[iT1].setTurnPlacement(iC1, nTurn);

	//second tile
	iC2 = (iC1 + 3) % 6;
	m_pBoard[iT2].addShip(iC2, iPlayer);
	m_pBoard[iT2].setTurnPlacement(iC2, nTurn);

	//add these tiles to the array of ships built this turn
	if(TRUE == m_pGame->m_bInGame)
	{
		m_nShipsThisTurn.Add(ENCODE_SEL(iT1, iC1));
		m_nShipsThisTurn.Add(ENCODE_SEL(iT2, iC2));
	}

	//subtract one from the players stock
	m_pGame->m_players[iPlayer].m_nStockShips--;

	//check for hidden tile
	nHiddenTile = checkForHiddenTile(iT1, iC1);

	//check for chitless tile if it's this player's turn
	if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) || (m_pGame->m_iIndex == m_iPlayer) || (m_nChitlessChitIndex < (int) m_nChitlessChitArray.size()))
	{
		checkForChitlessTile(iT1, iC1, nChitless);
	}

	//go back to city mode
	if(FALSE == m_pGame->m_bInGame)
	{
		//create the undo ID (uses same ID type as roads)
		ID = ENCODE_INIT_ROAD(iT1, iC1, iT2, iC2, m_iLastCity);

		//add the undo
		addUndo(UNDO_SHIP_PLACE, new CUndoShip(ID, nHiddenTile, nChitless));

		m_bCity = TRUE;

		//redraw
		Invalidate();

		//update all player counts
		for(i = 0; i < m_iNumPlayers; i++)
		{
			THISVIEW->updateCounts(i);
		}

		//done
		return;
	}

	//see if it's free
	bFree = m_iFreeRoads ? TRUE : FALSE;

	//if they played road building, subtract one from the number played
	if(TRUE == bFree)
	{
		m_iFreeRoads--;
	}
	else
	{
		//decrement one clay and one timber to the bank
		decResource(RES_STAT_BUILD, iPlayer, RES_TIMBER);
		decResource(RES_STAT_BUILD, iPlayer, RES_SHEEP);
	}

	//check for longest road
	iLongest = longestRoad(iPlayer);

	//create the undo ID (uses the same format as roads)
	ENCODE_ROAD(ID, bFree, iT1, iC1, iT2, iC2, iLongest);

	//see if they've just got it
	handleCommonLongestPoints(iPlayer, iLongest);
	
	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//update all player counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->updateCounts(i);
	}

	//create the undo
	addUndo(UNDO_SHIP, new CUndoShip(ID, nHiddenTile, nChitless));

	//reenable undo if possible
	if(GAME_MODE_HOTSEAT == GAME->m_iMode || ((-1 == nChitless) && (-1 == nHiddenTile) && (m_iPlayer == m_pGame->m_iIndex)))
	{
		THISVIEW->getMessageButton()->enableUndo(TRUE);
	}
	
	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// common move ship code
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonMoveShip(int iPlayer, int iOT1, int iOC1, int iT1, int iC1)
{
	int i;
	int ID;
	int iC2, iT2;
	int iOwner;
	int iLongest;
	int nHidden;
	int nChitless = -1;
	int nTurn;

	//calculate turn
	nTurn = m_pGame->m_iRollList.size();

	//play the sound
	SOUND(SOUND_MOVE_SHIP);

	//this player has now moved a ship this turn
	m_pGame->m_players[iPlayer].m_bMovedShip = TRUE;

	//determine who current has the longest road
	iOwner = longestPlayer();

	//find other side
	iT2 = m_pBoard[iT1].getSide(iC1);

	//add the roads
	m_pBoard[iT1].addShip(iC1, iPlayer);
	m_pBoard[iT1].setTurnPlacement(iC1, nTurn);

	//second tile
	iC2 = (iC1 + 3) % 6;
	m_pBoard[iT2].addShip(iC2, iPlayer);
	m_pBoard[iT2].setTurnPlacement(iC2, nTurn);

	//if this is network mode, we also need to remove the original ship since
	//they didn't get to see it happen when the player was picking which ship
	//to move
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		//find the other side
		iT2 = m_pBoard[iOT1].getSide(iOC1);

		//remove
		m_pBoard[iOT1].removeShip(iOC1);
		m_pBoard[iOT1].setTurnPlacement(iOC1, -1);

		//second side
		iC2 = (iOC1 + 3) % 6;
		m_pBoard[iT2].removeShip(iC2);
		m_pBoard[iT2].setTurnPlacement(iC2, -1);
	}

	//if this player is the owner of longest road, see if they lost it
	if(iOwner == iPlayer)
	{
		//get everyone's road lengths
		calculateRoadLengths();

		//start off as if they're keeping it
		iLongest = -2;

		//traverse each player, comparing their road length to this players
		for(i = 0; i < m_iNumPlayers; i++)
		{
			//skip self
			if(i == iPlayer)
			{
				continue;
			}

			//if any player's road length is greater than this player, this player
			//lost longest road by moving a ship
			if(m_pGame->m_players[i].m_iRoadLength > m_pGame->m_players[iPlayer].m_iRoadLength)
			{
				//we know they lost it
				iLongest = longestRoad(iPlayer);
			}
		}

		//if no one else beat them, see if they lost it because they went below 5
		if((-2 == iLongest) && (m_pGame->m_players[iPlayer].m_iRoadLength < 5))
		{
			iLongest = -3;
		}
	}
	//otherwise, see if they stole it
	else
	{
		//check for longest road
		iLongest = longestRoad(iPlayer);
	}

	//create the undo ID
	ENCODE_SHIP_MOVE(ID, (iOwner + 1), iOT1, iOC1, iT1, iC1, iLongest);

	//adjust points
	handleCommonLongestPoints(iPlayer, iLongest, iOwner, TRUE);

	//see if a tile was shown
	nHidden = checkForHiddenTile(iT1, iC1);

	//check for chitless tile if it's this player's turn
	if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) || (m_pGame->m_iIndex == m_iPlayer) || (m_nChitlessChitIndex < (int) m_nChitlessChitArray.size()))
	{
		checkForChitlessTile(iT1, iC1, nChitless);
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//update all player counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->updateCounts(i);
	}

	//create the undo
	addUndo(UNDO_MOVE_SHIP, new CUndoShip(ID, nHidden, nChitless, m_iLastMoveTurn));

	//reenable undo if possible
	if(GAME_MODE_HOTSEAT == GAME->m_iMode || ((-1 == nChitless) && (-1 == nHidden) && (m_iPlayer == m_pGame->m_iIndex)))
	{
		THISVIEW->getMessageButton()->enableUndo(TRUE);
	}
	
	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles common city placement code
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonCityPlace(int iPlayer, int ID)
{
	int iSave;
	int iCorner;
	int iSide1, iSide2, iSide3;

	//play sound
	SOUND(SOUND_BUILD_CITY);

	//save the ID
	iSave = ID;

	//deconstruct (OpenGL creates the actual ID as the selection name)
	DECODE_CITY(ID, iSide1, iSide2, iSide3, iCorner);

	//promote the city
	handleCommonPromoteCity(iPlayer, iSide1, iCorner);

	//decrement three ore and two wheat to the bank
	decResource(RES_STAT_BUILD, iPlayer, RES_ORE, 3);
	decResource(RES_STAT_BUILD, iPlayer, RES_WHEAT, 2);

	//update the player panel
	THISVIEW->updateCounts(iPlayer);

	//add the undo
	addUndo(UNDO_CITY, new CUndoOne(iSave));

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// common code when a settlement is added to the board
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonAddSettlement(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;
	int nExtra;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//add the settlements to the board
	m_pBoard[nTile].addSettle(nSide, nPlayer);
	m_pBoard[nTile2].addSettle(nC2, nPlayer);
	m_pBoard[nTile3].addSettle(nC3, nPlayer);

	//subtract one from the players stock
	m_pGame->m_players[nPlayer].m_nStockSettle--;

	//increment the player counter
	m_pGame->m_players[nPlayer].m_iSettlements[nTile]++;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile2]++;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile3]++;

	//add one to the players points
	incPoints(nPlayer);

	//see if we need to continue
	if(FALSE == m_pGame->m_bInGame)
	{
		return;
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//calculate extra points for this settlement placement
	nExtra = calculateExtraPoints(nTile, nSide, nPlayer);

	//increment the players points
	incPoints(nPlayer, nExtra);

	//set the points in the board pieces
	m_pBoard[nTile].setExtra(nSide, nExtra);
	m_pBoard[nTile2].setExtra(nC2, nExtra);
	m_pBoard[nTile3].setExtra(nC3, nExtra);

	//set the bonus used if they got extra
	if(0 < nExtra)
	{
		if(FALSE == m_pGame->m_players[nPlayer].m_bBonus1)
		{
			m_pGame->m_players[nPlayer].m_bBonus1 = TRUE;
		}
		else if(FALSE == m_pGame->m_players[nPlayer].m_bBonus2)
		{
			m_pGame->m_players[nPlayer].m_bBonus2 = TRUE;
		}

		//play the sound
		SOUND(SOUND_EXTRA_POINT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code when a settlement is removed from the board
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonRemoveSettlement(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;
	int nExtra;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//remove the last settlement placed from each of the board piece tiles
	m_pBoard[nTile].removeCity(nSide);
	m_pBoard[nTile2].removeCity(nC2);
	m_pBoard[nTile3].removeCity(nC3);

	//add one from the players stock
	m_pGame->m_players[nPlayer].m_nStockSettle++;

	//decrement the count references from this player
	m_pGame->m_players[nPlayer].m_iSettlements[nTile]--;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile2]--;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile3]--;

	//see if we need to continue
	if(FALSE == m_pGame->m_bInGame)
	{
		return;
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//get extra amount
	nExtra = m_pBoard[nTile].getExtra(nSide);

	//clear any extra points
	m_pBoard[nTile].setExtra(nSide, 0);
	m_pBoard[nTile2].setExtra(nC2, 0);
	m_pBoard[nTile3].setExtra(nC3, 0);

	//dec one from the player's points for settlement
	decPoints(nPlayer);

	//decrement any extra points
	decPoints(nPlayer, nExtra);

	//reset the bonus used if they got extra
	if(0 < nExtra)
	{
		if(TRUE == m_pGame->m_players[nPlayer].m_bBonus2)
		{
			m_pGame->m_players[nPlayer].m_bBonus2 = FALSE;
		}
		else if(TRUE == m_pGame->m_players[nPlayer].m_bBonus1)
		{
			m_pGame->m_players[nPlayer].m_bBonus1 = FALSE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code to add a city directly to the board (usually from undoing a volcano)
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonAddCity(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;
	int nExtra;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//add the settlements to the board
	m_pBoard[nTile].addSettle(nSide, nPlayer);
	m_pBoard[nTile2].addSettle(nC2, nPlayer);
	m_pBoard[nTile3].addSettle(nC3, nPlayer);

	//now make them into cities
	m_pBoard[nTile].makeCity(nSide);
	m_pBoard[nTile2].makeCity(nC2);
	m_pBoard[nTile3].makeCity(nC3);

	//subtract one from the players stock
	m_pGame->m_players[nPlayer].m_nStockCities--;

	//increment the player counter
	m_pGame->m_players[nPlayer].m_iCities[nTile]++;
	m_pGame->m_players[nPlayer].m_iCities[nTile2]++;
	m_pGame->m_players[nPlayer].m_iCities[nTile3]++;

	//add two to the players points
	incPoints(nPlayer, 2);

	//see if we need to continue
	if(FALSE == m_pGame->m_bInGame)
	{
		return;
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//calculate extra points for this settlement placement
	nExtra = calculateExtraPoints(nTile, nSide, nPlayer);

	//increment the players points
	incPoints(nPlayer, nExtra);

	//set the points in the board pieces
	m_pBoard[nTile].setExtra(nSide, nExtra);
	m_pBoard[nTile2].setExtra(nC2, nExtra);
	m_pBoard[nTile3].setExtra(nC3, nExtra);

	//set the bonus used if they got extra
	if(0 < nExtra)
	{
		if(FALSE == m_pGame->m_players[nPlayer].m_bBonus1)
		{
			m_pGame->m_players[nPlayer].m_bBonus1 = TRUE;
		}
		else if(FALSE == m_pGame->m_players[nPlayer].m_bBonus2)
		{
			m_pGame->m_players[nPlayer].m_bBonus2 = TRUE;
		}

		//play the sound
		SOUND(SOUND_EXTRA_POINT);
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code to promote a city from a settlement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonPromoteCity(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//update tiles
	m_pBoard[nTile].makeCity(nSide);
	m_pBoard[nTile2].makeCity(nC2);
	m_pBoard[nTile3].makeCity(nC3);

	//subtract one from the players city stock
	m_pGame->m_players[nPlayer].m_nStockCities--;

	//add one to the players settlement stocks
	m_pGame->m_players[nPlayer].m_nStockSettle++;

	//update player data
	m_pGame->m_players[nPlayer].m_iSettlements[nTile]--;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile2]--;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile3]--;

	//increment cities
	m_pGame->m_players[nPlayer].m_iCities[nTile]++;
	m_pGame->m_players[nPlayer].m_iCities[nTile2]++;
	m_pGame->m_players[nPlayer].m_iCities[nTile3]++;

	//add one to the players points
	incPoints(nPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// common code when a city is reduced to a settlement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonReduceCity(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//update tiles
	m_pBoard[nTile].makeSettle(nSide);
	m_pBoard[nTile2].makeSettle(nC2);
	m_pBoard[nTile3].makeSettle(nC3);

	//subtract one from the players settlement stock
	m_pGame->m_players[nPlayer].m_nStockSettle--;

	//add one to the players city stocks
	m_pGame->m_players[nPlayer].m_nStockCities++;

	//update player data
	m_pGame->m_players[nPlayer].m_iSettlements[nTile]++;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile2]++;
	m_pGame->m_players[nPlayer].m_iSettlements[nTile3]++;

	//decrement cities
	m_pGame->m_players[nPlayer].m_iCities[nTile]--;
	m_pGame->m_players[nPlayer].m_iCities[nTile2]--;
	m_pGame->m_players[nPlayer].m_iCities[nTile3]--;

	//dec one from the player's points for reducing a city
	decPoints(nPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// common code when a city is destroyed (usually by volcanoes)
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonRemoveCity(int nPlayer, int nTile, int nSide)
{
	int nC2, nC3;
	int nTile2, nTile3;
	int nExtra;

	//get alternate sides
	nTile2 = m_pBoard[nTile].getSide(nSide);
	nTile3 = m_pBoard[nTile].getSide((nSide + 5) % 6);

	//get extra amount
	nExtra = m_pBoard[nTile].getExtra(nSide);

	//set opposite corners
	nC2 = (nSide + 4) % 6;
	nC3 = (nSide + 2) % 6;

	//update tiles
	m_pBoard[nTile].removeCity(nSide);
	m_pBoard[nTile2].removeCity(nC2);
	m_pBoard[nTile3].removeCity(nC3);

	//add one to the players city stocks
	m_pGame->m_players[nPlayer].m_nStockCities++;

	//decrement cities
	m_pGame->m_players[nPlayer].m_iCities[nTile]--;
	m_pGame->m_players[nPlayer].m_iCities[nTile2]--;
	m_pGame->m_players[nPlayer].m_iCities[nTile3]--;

	//clear any extra points
	m_pBoard[nTile].setExtra(nSide, 0);
	m_pBoard[nTile2].setExtra(nC2, 0);
	m_pBoard[nTile3].setExtra(nC3, 0);

	//dec two from the player's points for destroying a city
	decPoints(nPlayer, 2);

	//decrement any extra points
	decPoints(nPlayer, nExtra);

	//reset the bonus used if they got extra
	if(0 < nExtra)
	{
		if(TRUE == m_pGame->m_players[nPlayer].m_bBonus2)
		{
			m_pGame->m_players[nPlayer].m_bBonus2 = FALSE;
		}
		else if(TRUE == m_pGame->m_players[nPlayer].m_bBonus1)
		{
			m_pGame->m_players[nPlayer].m_bBonus1 = FALSE;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code for checking longest road lengths
/////////////////////////////////////////////////////////////////////////////
int CGLView::handleCommonLongestCheck(int nPlayer)
{
	int nCurPlayer = -1;
	int nCurLength = 0;
	int nLongest;

	//get the current longest road player
	nCurPlayer = longestPlayer();

	//if someone has it, see if they lost it
	if((nCurPlayer != -1) && (nCurPlayer != nPlayer))
	{
		//get the current length
		nCurLength = m_iLongestLength;

		//reset the longest length
		m_iLongestLength = 0;
		m_iLongestRoad.clear();

		//now get the longest length for this player
		longestRoad(nCurPlayer);

		//if it's now less, someone might have longer
		if(nCurLength > m_iLongestLength)
		{
			//dec their points
			decLongestRoad(nCurPlayer);

			//find the new longest road player
			handleCommonLongestFindPlayer();

			//add one to the player for the undo
			nCurPlayer++;
		}
		//its not less, so check to see if the new player has built a longer road
		else
		{
			//get the value for this player
			nLongest = longestRoad(nPlayer);

			//if they stole it, do stuff
			if(0 <= nLongest)
			{
				//dec their points
				decLongestRoad(nCurPlayer);

				//add one to the player for the undo
				nCurPlayer++;

				//inc their points
				incLongestRoad(nPlayer);
			}
			else
			{
				nCurPlayer = 0;
			}
		}
	}
	//with the addition of Seafarers, it's possible to connect a road and a ship
	//by building a settlement in between them, which *could* give a player the
	//longest road
	else
	{
		//see if they get it or have it already
		nLongest = longestRoad(nPlayer);

		//the only two results from this are either they just got, or they already
		//had it...
		if(-1 == nLongest || -2 == nLongest)
		{
			//use 9 as a flag since we can't have that many players
			nCurPlayer = 9;

			//see if they just got it
			if(-1 == nLongest)
			{
				//inc their points
				incLongestRoad(nPlayer);
			}
		}
		else
		{
			nCurPlayer = 0;
		}
	}
	
	//recalculate road lengths and update everyone
	calculateRoadLengths();

	//return who has the longest road
	return nCurPlayer;
}

/////////////////////////////////////////////////////////////////////////////
// common code for finding the player with the longest road
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonLongestFindPlayer()
{
	int i;
	int nLongLength = 0;
	int nLongPlayer = -1;
	BOOL bRepeat = FALSE;

	//traverse players, looking for the longest road
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//reset the longest length
		m_iLongestLength = 0;
		m_iLongestRoad.clear();

		//run it for this player
		longestRoad(i);

		//compare against the current longest length
		if(nLongLength < m_iLongestLength)
		{
			nLongLength = m_iLongestLength;
			nLongPlayer = i;
			bRepeat = FALSE;
		}
		//if it's the same, multiple people have the same length road,
		//and so it reverts to no one having the longest road
		else if(nLongLength == m_iLongestLength)
		{
			bRepeat = TRUE;
		}
	}

	//if there's a repeat, or no longest player, no one has longest road
	if((TRUE == bRepeat) || (nLongPlayer == -1))
	{
		//reset the longest length
		m_iLongestLength = 0;
		m_iLongestRoad.clear();
	}
	else
	{
		//inc their points
		incLongestRoad(nLongPlayer);

		//create the longest road
		longestRoad(nLongPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code for adjusting longest road points based on the return value
// of longestRoad()
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonLongestPoints(int nPlayer, int nResult, int nOwner, BOOL bMove)
{
	if(nResult >= -1)
	{
		//inc their points
		incLongestRoad(nPlayer);

		//see if they stole it
		if(nResult >= 0)
		{
			//dec their points
			decLongestRoad(nResult);
		}
	}
	//see if they lost it by moving the ship
	else if((TRUE == bMove) && (-3 == nResult) && (nPlayer == nOwner))
	{
		//dec their points
		decLongestRoad(nPlayer);

		//find whoever has the longest road
		handleCommonLongestFindPlayer();

		//recalc road lengths
		calculateRoadLengths();
	}
}

/////////////////////////////////////////////////////////////////////////////
// common code for determining if a ship can be built from an initial placement
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::handleCommonInitialShipCheck(int nTile, int nC1)
{
	int nMinus;
	int nType1, nType2, nType3;
	BOOL bEdge = FALSE;

	//if this is a seafarers enabled map, we check to see if they can build
	//a ship from this settlement... if so, we need to go to a new mode where they
	//can choose to built a ship or a road
	if(MAP->m_iMode & MAP_SEAFARERS)
	{
		//check to see if we can build a ship from this tile, which means that
		//one of the neighboring tiles must be a sea tile

		//get the trailing side
		nMinus = (nC1 + 5) % 6;

		//get the tile types
		nType1 = m_pBoard[nTile].getType();
		nType2 = m_pBoard[m_pBoard[nTile].getSide(nC1)].getType();
		nType3 = m_pBoard[m_pBoard[nTile].getSide(nMinus)].getType();

		//check for sea tiles
		if( (TRUE == VALID_OCEAN(nType1)) ||
			(TRUE == VALID_OCEAN(nType2)) ||
			(TRUE == VALID_OCEAN(nType3)))
		{
			//first we determine if they have the rule on about building ships
			//instead of roads if the tile is not on the edge
			if(TRUE == MAP->m_bRequiresInitialShips)
			{
				//check each of the tiles to see if it is an ocean AND a border
				//if that is the case, then they have the option to build a road
				//instead of a ship... otherwise, if they're all NOT borders, then
				//they have to build a ship
				if((TRUE == VALID_OCEAN(nType1)) && (TRUE == m_pBoard[nTile].getBorder()))
				{
					bEdge = TRUE;
				}

				//second tile
				if((TRUE == VALID_OCEAN(nType2)) && (TRUE == m_pBoard[m_pBoard[nTile].getSide(nC1)].getBorder()))
				{
					bEdge = TRUE;
				}

				//third tile
				if((TRUE == VALID_OCEAN(nType3)) && (TRUE == m_pBoard[m_pBoard[nTile].getSide(nMinus)].getBorder()))
				{
					bEdge = TRUE;
				}

				//if we have no edge pieces, than force a ship building
				if(FALSE == bEdge)
				{
					//prepare the ship locations
					prepareInitialShips();

					//set the new mode
					m_iMode = GL_PLACE_PLAYERS;

					//update the panel text
					THISVIEW->getControlPanel()->updateText();

					//turn on hit tests for ships
					setHitTest(TRUE);

					return TRUE;
				}
			}

			//set the mode
			m_iMode = GL_WAIT;

			//show the build options
			THISVIEW->showInitialOptions(TRUE);

			return TRUE;
		}
	}

	return FALSE;
}