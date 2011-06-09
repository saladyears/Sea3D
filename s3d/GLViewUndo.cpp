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
#include "math.h"
#include "UndoOne.h"
#include "UndoShip.h"
#include "SettlersView.h"
#include "defineSounds.h"
#include "defineStat.h"
#include "defineCode.h"
#include "defineVariants.h"

/////////////////////////////////////////////////////////////////////////////
// add to the undo list
/////////////////////////////////////////////////////////////////////////////
void CGLView::addUndo(int iAction, CUndo *p)
{
	BOOL bEnable = FALSE;

	//add the new undo action, but not in network mode
	m_pGame->m_listUndo.push_back(CUndoNode(iAction, p));

	if(GAME_MODE_HOTSEAT == m_pGame->m_iMode || 
		((UNDO_ACTION == iAction || UNDO_CITY == iAction || UNDO_SETTLEMENT == iAction || UNDO_TRADE == iAction) && m_iPlayer == m_pGame->m_iIndex))
	{
		//set default
		bEnable = TRUE;

		//we've allowed trades, now verify that it's not with a partner
		if(UNDO_TRADE == iAction && 0 <= ((CUndoTrade *) p)->m_iPartner)
		{
			bEnable = FALSE;
		}
	}

	THISVIEW->getMessageButton()->enableUndo(bEnable);

	//add it to the undo tree
	UNDO->addItem(iAction, p);
}

/////////////////////////////////////////////////////////////////////////////
// undo the last action
/////////////////////////////////////////////////////////////////////////////
void CGLView::undo()
{
	int ID;
	int iPlayers[6];
	BOOL bSteal = FALSE;
	BOOL bAgain = FALSE;
	BOOL bPirate;

	list <CUndoNode>::iterator i;

	//play sound
	SOUND(SOUND_UNDO);

	//set it to the end
	i = m_pGame->m_listUndo.end();
	i--;

	//remove it from the undo tree
	UNDO->removeItem();

	//undo it
	switch((*i).m_iAction)
	{
	case UNDO_SETTLE_PLACE:
		undoSettlementPlace(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_ROAD_PLACE:
		undoRoadPlace(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_THIRD_ROAD_PLACE:
		undoThirdRoadPlace(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_SHIP_PLACE:
		undoShipPlace(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_FIRST_ROLL:
		m_pGame->m_bInGame = FALSE;
		undoRoll(((CUndoRoll *)(*i).m_pData));
		break;
	case UNDO_ROLL:
		m_pGame->m_bInGame = TRUE;
		undoRoll(((CUndoRoll *)(*i).m_pData));
		break;
	case UNDO_VOLCANO:
		undoVolcano(((CUndoVolcano *)(*i).m_pData));
		break;
	case UNDO_TRADE_JUNGLE:
		undoJungle(((CUndoTrade *)(*i).m_pData));
		break;
	case UNDO_ACTION_START:
		undoActionStart(((CUndoAction *)(*i).m_pData));
		break;
	case UNDO_ACTION_END:
		undoActionEnd(((CUndoEnd *)(*i).m_pData));
		break;
	case UNDO_ACTION:
		undoAction();
		break;
	case UNDO_ROAD:
		undoRoad(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_SHIP:
		undoShip(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_CITY:
		undoCity(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_SETTLEMENT:
		undoSettlement(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_MOVE_SHIP:
		undoMoveShip(((CUndoShip *)(*i).m_pData));
		break;
	case UNDO_ROBBER_PLACE:
		undoRobber(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_PIRATE_PLACE:
		undoPirate(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_BUY_DEVCARD:
		undoBuyCard(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_PLAY_VICTORY:
		undoPlayVictory(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_PLAY_MONOPOLY:
		undoPlayMonopoly(((CUndoMonopoly *)(*i).m_pData));
		break;
	case UNDO_PLAY_ROAD:
		undoPlayRoadBuild();
		break;
	case UNDO_PLAY_YEAR:
		undoPlayYearPlenty(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_PLAY_SOLDIER:
		undoPlaySoldier(((CUndoOne *)(*i).m_pData)->m_iOne);
		break;
	case UNDO_TRADE:
		undoTrade(((CUndoTrade *)(*i).m_pData));
		break;
	case UNDO_STEAL:
		//get the ID
		ID = ((CUndoOne *)(*i).m_pData)->m_iOne;

		//determine pirate or robber
		bPirate = ID / 100; ID -= (bPirate * 100);

		//determine
		undoSteal(ID);

		//we undid a steal
		bSteal = TRUE;

		//manually delete this undo to prevent adding another undo
		//from the steal dialog which would then be erased below
		delete (*i).m_pData;
		m_pGame->m_listUndo.erase(i, m_pGame->m_listUndo.end());

		//now handle restealing
		bAgain = handleSteal(bPirate);

		break;
	case UNDO_LOSECARDS:
		undoLoseCards(((CUndoTrade *)(*i).m_pData));

		//undo again
		bAgain = TRUE;
		break;
	case UNDO_GOLD:
		//undo the resources gained
		undoGold(((CUndoGold *)(*i).m_pData), iPlayers);

		//we undid the gold (count as a steal, since the desired effect is the
		//same)
		bSteal = TRUE;

		//manually delete this undo to prevent adding another undo
		//from the steal dialog which would then be erased below
		delete (*i).m_pData;
		m_pGame->m_listUndo.erase(i, m_pGame->m_listUndo.end());

		//now handle rerunning the gold dialog
		bAgain = handleGold(iPlayers);

		break;
	case UNDO_CHITLESS:
		undoChitless(((CUndoOne *)(*i).m_pData)->m_iOne);

		break;
	case UNDO_SPECIAL_BUILD:
		bAgain = undoSpecialBuild();
		break;
	case UNDO_SPECIAL_BUILD_END:
		undoSpecialBuildEnd();
		break;
	default:
		break;
	}

	//now remove it--undoing a steal manually deletes its own data because
	//undoing a steal brings up the steal from dialog, which could potentially
	//add another undo to the list... deleting that would have disastrous
	//consequences
	if(FALSE == bSteal)
	{
		delete (*i).m_pData;
		m_pGame->m_listUndo.erase(i, m_pGame->m_listUndo.end());
	}

	//undo again if needed
	if(TRUE == bAgain)
	{
		undo();
	}

	//if we're at the beginning of the list (restarting save games), disable undo
	if(0 == m_pGame->m_listUndo.size())
	{
		THISVIEW->getMessageButton()->enableUndo(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo an opening settlement placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoSettlementPlace(int ID)
{
	int iPlayer;
	int iTile1, iTile2, iTile3;
	int iCorner;

	//decode the ID
	DECODE_INIT_SETTLEMENT(ID, iPlayer, iTile1, iTile2, iTile3, iCorner);

	//remove it from the board
	handleCommonRemoveSettlement(iPlayer, iTile1, iCorner);

	//reset to city placement
	m_bCity = TRUE;

	//if this is the second city, decrease the requisite resources to the bank
	if(!m_bIncrease)
	{
		decResource(RES_STAT_BOARD, iPlayer, m_pBoard[iTile1].getType());
		decResource(RES_STAT_BOARD, iPlayer, m_pBoard[iTile2].getType());
		decResource(RES_STAT_BOARD, iPlayer, m_pBoard[iTile3].getType());

		//update jungles if needed
		if(RES_JUNGLE == m_pBoard[iTile1].getType())
		{
			m_pGame->m_players[iPlayer].m_nJungles--;
		}
		if(RES_JUNGLE == m_pBoard[iTile2].getType())
		{
			m_pGame->m_players[iPlayer].m_nJungles--;
		}
		if(RES_JUNGLE == m_pBoard[iTile3].getType())
		{
			m_pGame->m_players[iPlayer].m_nJungles--;
		}

		THISVIEW->updateCounts(iPlayer);

		//update the stats
		STAT->update(STAT_GOODS_RECV_BOARD);
	}

	//turn off undo possibility
	if((0 == m_iPlayer) && m_bIncrease)
	{
		THISVIEW->getMessageButton()->enableUndo(FALSE);
	}

	//dec one from the player's points
	decPoints(m_iPlayer);

	//update the control panel text
	THISVIEW->getControlPanel()->updateText();

	//clear the placement arrays
	m_iPossibleRoads.resize(0);
	m_iPossibleRoads.clear();
	m_iPossibleShips.resize(0);
	m_iPossibleShips.clear();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo moving a ship
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoMoveShip(CUndoShip *pShip)
{
	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientUndoBuild(GL_MOVE_SHIP, pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless, pShip->m_nTurn);

		//they can't undo anything past this
		THISVIEW->getMessageButton()->enableUndo(FALSE);
	}

	//do it
	handleCommonUndoShipMove(pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless, pShip->m_nTurn);

	//recalc the build options
	THISVIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo a tournament opening road
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoThirdRoadPlace(CUndoShip *pShip)
{
	int iTile1, iTile2;
	int iC1, iC2;

	//see if we're not in the right mode
	if(GL_PLACE_THIRD_ROAD != m_iMode)
	{
		//reset to the last player
		m_iPlayer = m_iNumPlayers - 1;

		//we are in tournament roads mode now
		m_bTournamentRoads = TRUE;

		//set the new mode
		m_iMode = GL_PLACE_THIRD_ROAD;
	}
	else
	{
		m_iPlayer--;
	}

	//decode the ID
	DECODE_INIT_ROAD(pShip->m_ID, iTile1, iC1, iTile2, iC2, m_iLastCity);

	//hide hidden tile
	undoHiddenTile(pShip->m_nHidden);

	//remove the last road placed from each of the board piece tiles
	m_pBoard[iTile1].removeRoad(iC1);
	m_pBoard[iTile2].removeRoad(iC2);

	//add one to the players stock
	m_pGame->m_players[m_iPlayer].m_nStockRoads++;

	//update the player display
	THISVIEW->updatePlayerDisplay();

	//update the panel text
	THISVIEW->getControlPanel()->updateText();

	//set the interaction mode to select
	m_bUseSelection = TRUE;

	//reset the selection
	m_iSelection = -1;

	//turn on hit tests
	setHitTest(TRUE);

	//update the display text
	THISVIEW->getControlPanel()->updateText();

	//determine possible roads
	prepareRoads();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo an opening road
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoRoadPlace(CUndoShip *pShip)
{
	int iTile1, iTile2;
	int iC1, iC2;

	//set the GL drawing mode if necessary
	if(m_iMode != GL_PLACE_PLAYERS)
	{
		setPreviousMode();

		//ensure that we don't have the flag on
		m_bTournamentRoads = FALSE;
	}

	//decode the ID
	DECODE_INIT_ROAD(pShip->m_ID, iTile1, iC1, iTile2, iC2, m_iLastCity);

	//hide hidden tile
	undoHiddenTile(pShip->m_nHidden);

	//remove the last road placed from each of the board piece tiles
	m_pBoard[iTile1].removeRoad(iC1);
	m_pBoard[iTile2].removeRoad(iC2);

	//set back the player
	m_iPlayer -= (m_bIncrease ? 1 : -1);
	if(m_iPlayer >= m_iNumPlayers)
	{
		m_iPlayer = m_iNumPlayers - 1; 
		m_bIncrease = TRUE;
	}

	//add one to the players stock
	m_pGame->m_players[m_iPlayer].m_nStockRoads++;

	//update the player display
	THISVIEW->updatePlayerDisplay();

	//make sure we don't go below zero
	if(m_iPlayer < 0) m_iPlayer = 0;

	//placing a road
	m_bCity = FALSE;

	//update the panel text
	THISVIEW->getControlPanel()->updateText();

	//hide the spotlight option
	m_iMode = GL_PLACE_PLAYERS;

	//reprepare
	prepareInitialRoads();

	//turn on hit tests
	setHitTest(TRUE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo an opening ship
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoShipPlace(CUndoShip *pShip)
{
	int iTile1, iTile2;
	int iC1, iC2;

	//set the GL drawing mode if necessary
	if(m_iMode != GL_PLACE_PLAYERS)
	{
		setPreviousMode();
	}

	//decode the ID (uses same ID type as roads)
	DECODE_INIT_ROAD(pShip->m_ID, iTile1, iC1, iTile2, iC2, m_iLastCity);

	//undo the hidden tile
	undoHiddenTile(pShip->m_nHidden);

	//remove the last road placed from each of the board piece tiles
	m_pBoard[iTile1].removeShip(iC1);
	m_pBoard[iTile2].removeShip(iC2);

	//set back the player
	m_iPlayer -= (m_bIncrease ? 1 : -1);
	if(m_iPlayer >= m_iNumPlayers)
	{
		m_iPlayer = m_iNumPlayers - 1; 
		m_bIncrease = TRUE;
	}

	//add one to the players stock
	m_pGame->m_players[m_iPlayer].m_nStockShips++;

	//update the player display
	THISVIEW->updatePlayerDisplay();

	//make sure we don't go below zero
	if(m_iPlayer < 0) m_iPlayer = 0;

	//placing a road
	m_bCity = FALSE;

	//update the panel text
	THISVIEW->getControlPanel()->updateText();

	//hide the spotlight option
	m_iMode = GL_PLACE_PLAYERS;

	//reprepare
	prepareInitialShips();

	//turn on hit tests
	setHitTest(TRUE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo the revealing of a hidden tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoHiddenTile(int nHidden)
{
	int nRes;
	BOOL bVolcanoes = FALSE;


	//sanity
	if((FALSE == MAP->m_bHasHiddenTiles) || (-1 == nHidden))
	{
		return;
	}

	//determine if we're using the jungles/volcanoes variants
	if(IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD) || IS_VARIANT(VARIANT_USE_JUNGLE))
	{
		bVolcanoes = TRUE;
	}
	
	//get the current res
	nRes = m_pBoard[nHidden].getType();

	//if it's a jungle or volcano and we have the variants on, roll back
	if((TRUE == bVolcanoes) && ((RES_JUNGLE == nRes) || (RES_VOLCANO == nRes)))
	{
		//set back to desert for anality sake
		nRes = RES_DESERT;

		//undo the roll
		m_pBoard[nHidden].setRoll(0);
		m_pBoard[nHidden].setRollTex(0);

		//decrement the index
		m_nHiddenVolcanoIndex--;
	}

	//reset it back to hidden
	m_pBoard[nHidden].setType(RES_HIDDEN);

	//reset the chits
	if(TRUE == VALID_RESOURCE(nRes))
	{
		//no roll
		m_pBoard[nHidden].setRoll(0);
		m_pBoard[nHidden].setRollTex(0);

		//remove the player's resource
		decResource(RES_STAT_DISCOVER, m_iPlayer, nRes);

		//decrement the index
		m_nHiddenChitIndex--;

		//update the player's counts
		THISVIEW->updateCounts(m_iPlayer);

		//update the stat
		STAT->update(STAT_GOODS_RECV_DISCOVER);
	}

	//decrement the index
	m_nHiddenIndex--;
}

/////////////////////////////////////////////////////////////////////////////
// undo the revealing of a chitless tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoChitlessTile(int nTile)
{
	//sanity
	if((FALSE == MAP->m_bHasChitlessTiles) || (-1 == nTile))
	{
		return;
	}

	//reset the roll on the tile
	m_pBoard[nTile].setRoll(0);
	m_pBoard[nTile].setRollTex(0);

	//chitless again
	m_pBoard[nTile].setChitless(TRUE);

	//subtract from the index
	m_nChitlessChitIndex--;
}

/////////////////////////////////////////////////////////////////////////////
// undo the special build phase start
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::undoSpecialBuild()
{
	BOOL bUndo;

	//return to the previous player
	bUndo = setPreviousPlayer();

	//see if we keep going
	return bUndo;
}

/////////////////////////////////////////////////////////////////////////////
// undo the end of the special build phase
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoSpecialBuildEnd()
{
	//hide the rolls and enable turn actions
	THISVIEW->getControlPanel()->returnToTurn();

	//return to the previous player
	setPreviousPlayer();
}

/////////////////////////////////////////////////////////////////////////////
// undo a roll of the dice
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoRoll(CUndoRoll *pRoll)
{
	int i, j;
	int iDie1, iDie2;
	int iRoll;

	//get dice
	iDie1 = pRoll->m_iRoll >> 16;
	iDie2 = pRoll->m_iRoll - iDie1;
	
	//get the roll number
	iRoll = iDie1 + iDie2;

	//skip sevens
	if(iRoll != 7)
	{
		//run through the list of goods and re-add them
		for(i = 0; i < m_iNumPlayers; i++)
		{
			//run through the resources
			for(j = 0; j < 5; j++)
			{
				//decrement resources obtained to the bank
				decResource(RES_STAT_BOARD, i, j, pRoll->m_iGoods[i][j]);
				
				//decrement resources lost
				decResource(RES_STAT_LOST_BOARD, i, j, pRoll->m_iLost[i][j], m_iRobberPlayer);
			}

			//run through jungles
			m_pGame->m_players[i].m_nJungles -= pRoll->m_nJungles[i];

			//update the player counts
			THISVIEW->updateCounts(i);
		}

		//update the stats
		STAT->update(STAT_GOODS_RECV_BOARD);
		STAT->update(STAT_GOODS_LOST_BOARD);
	}

	//remove it from the game's list
	m_pGame->m_iRollList.pop_back();
	m_pGame->m_iRolls[m_iPlayer][iRoll - 2]--;

	//update the stats
	STAT->update(STAT_ROLL_FREQ);

	//we need to update the control panel
	THISVIEW->getControlPanel()->showGoods(FALSE);
	THISVIEW->getControlPanel()->showRolls(TRUE);

	//set the previous draw mode
	setPreviousMode();

	//enable things properly
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//clear the last rolled tiles
	m_iLastTiles.clear();
}

/////////////////////////////////////////////////////////////////////////////
// undo a volcanic eruption
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoVolcano(CUndoVolcano *pVolcano)
{
	int i;
	int nCurLongest;
	int nPlayer, nTile, nCorner, nPrev, nExtra;
	int nCity;

	//create an undo action to take advantage of existing code
	CUndoAction action(pVolcano->m_iRoll, FALSE, pVolcano->m_strGoods);

	//ensure clickability
	THISVIEW->getControlPanel()->showVolcano(FALSE);

	//call it
	undoActionStart(&action);

	//now actually undo the destruction
	for(i = 0; i < (int) pVolcano->m_nDestruction.size(); i++)
	{
		//get the undo value
		DECODE_VOLCANO(pVolcano->m_nDestruction[i], nPlayer, nTile, nCorner, nPrev, nExtra);

		//get what's on the current location
		nCity = m_pBoard[nTile].getCity(nCorner);

		//replace what was lost if nothing exists
		if(0 == nCity)
		{
			//settlement
			if(1 == nPrev)
			{
				handleCommonAddSettlement(nPlayer, nTile, nCorner);
			}
			else
			{
				handleCommonAddCity(nPlayer, nTile, nCorner);
			}
		}
		else
		{
			handleCommonPromoteCity(nPlayer, nTile, nCorner);
		}
	}

	//redo longest road
	if(-1 != pVolcano->m_nLongestPlayer)
	{
		//get current longest player
		nCurLongest = longestPlayer();

		//take it away if necessary
		if(-1 != nCurLongest)
		{
			decLongestRoad(nCurLongest);
		}

		//regive it to this person
		incLongestRoad(pVolcano->m_nLongestPlayer);

		//create the longest road
		m_iLongestLength = 0;
		m_iLongestRoad.clear();

		//redo it
		longestRoad(pVolcano->m_nLongestPlayer);

		//recalculate road lengths and update everyone
		calculateRoadLengths();
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo a jungle trade
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoJungle(CUndoTrade *pJungle)
{
	//undo the jungle trade
	handleCommonUndoJungle(pJungle->m_iPartner, pJungle->m_iFrom);

	//update the build options
	THISVIEW->showBuildOptions(TRUE);

	//update trading
	THISVIEW->getControlPanel()->EnableTrade();
}

/////////////////////////////////////////////////////////////////////////////
// undo the beginning of waiting for an action
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoActionStart(CUndoAction *pAction)
{
	int i;
	int iDie1, iDie2;
	int nRoll;
	int nVolcano;

	//handle things differently if it's a volcano
	if(TRUE == pAction->m_bVolcano)
	{
		//get settings
		nRoll = pAction->m_iRoll >> 16;
		nVolcano = pAction->m_iRoll - (nRoll << 16);
		
		//set the volcano roll
		THISVIEW->getControlPanel()->setLastVolcano(nVolcano);

		//set the volcano roll
		m_nVolcanoRoll = nVolcano;

		//create volcano draw array
		m_nLastVolcano.resize(0);
		m_nLastVolcano.clear();

		//run through tiles
		for(i = 0; i < m_nTiles; i++)
		{
			if((m_pBoard[i].getRoll() == nRoll) && (RES_VOLCANO == m_pBoard[i].getType()))
			{
				m_nLastVolcano.push_back(i);
			}
		}

		//run through the list and update the volcano data
		for(i = 0; i < m_iNumPlayers; i++)
		{
			THISVIEW->getControlPanel()->updateVolcano(!pAction->m_strGoods[i].IsEmpty(), i, pAction->m_strGoods[i]);
		}

		//show the volcano
		THISVIEW->getControlPanel()->showVolcano(TRUE);

		//toggle actions on
		ACTIONS->ShowWindow(SW_SHOW);

		//set the draw mode
		m_iMode = GL_VOLCANO;
	}
	else
	{
		//get rolls
		iDie1 = pAction->m_iRoll >> 16;
		iDie2 = pAction->m_iRoll - (iDie1 << 16);

		//get roll
		nRoll = iDie1 + iDie2;

		//update the turn's last roll
		THISVIEW->getControlPanel()->setLastRoll(iDie1, iDie2);

		//run through the list and update the turn
		for(i = 0; i < m_iNumPlayers; i++)
		{
			THISVIEW->getControlPanel()->updateGoods(!pAction->m_strGoods[i].IsEmpty(), i, pAction->m_strGoods[i]);
		}

		//show the goods
		if(nRoll != 7)
		{
			//show goods
			THISVIEW->getControlPanel()->showGoods(TRUE);

			//toggle actions on
			ACTIONS->ShowWindow(SW_SHOW);
		}

		//clear the last tiles array
		m_iLastTiles.clear();

		//create the previous rolled tiles array
		for(i = 0; i < m_nTiles; i++)
		{
			//check for ocean tiles
			if(TRUE == VALID_OCEAN(m_pBoard[i].getType()))
			{
				continue;
			}

			if(m_pBoard[i].getRoll() == nRoll)
			{
				//add it
				m_iLastTiles.push_back(i);
			}
		}

		setPreviousMode();
	}

	//hide build options
	THISVIEW->showBuildOptions(FALSE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo the end of a turn
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoActionEnd(CUndoEnd *pEnd)
{
	int i;
	int iTime;
	CPlayerGame *pGame;

	//return to the previous player
	setPreviousPlayer(TRUE);

	//get the current player game
	pGame = &(m_pGame->m_players[m_iPlayer]);

	//rebuild the array of cards that became playable
	for(i = 0; i < 5; i++)
	{
		//now subtract them from the playable cards and add them to the held cards
		pGame->m_iCardHeld[i] -= pEnd->m_nCards[i];
		pGame->m_iCardBought[i] = pEnd->m_nCards[i];
	}

	//set the played value
	pGame->m_bPlayedCard = pEnd->m_bPlayed;

	//set the moved ship value
	pGame->m_bMovedShip = pEnd->m_bShipMoved;

	//get the last recorded turn time for this player
	iTime = pGame->m_iTurnTime[pGame->m_iTurnTime.size() - 1];

	//reset the ships build this turn array
	m_nShipsThisTurn.RemoveAll();
	for(i = 0; i < pEnd->m_nShipsPlaced.GetSize(); i++)
	{
		m_nShipsThisTurn.Add(pEnd->m_nShipsPlaced[i]);
	}

	//reset the ports built on this turn array
	m_nPortsThisTurn.RemoveAll();
	for(i = 0; i < pEnd->m_nPortsPlaced.GetSize(); i++)
	{
		m_nPortsThisTurn.Add(pEnd->m_nPortsPlaced[i]);
	}

	//pop it off the stack
	pGame->m_iTurnTime.pop_back();

	//reset the reference time
	m_pGame->m_timeRef = COleDateTime::GetCurrentTime() - COleDateTimeSpan(0, 0, 0, iTime);

	//hide the rolls and enable turn actions
	THISVIEW->getControlPanel()->returnToTurn();
}

/////////////////////////////////////////////////////////////////////////////
// undo the start of an action
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoAction()
{
	//set the new mode
	m_iMode = GL_WAIT;

	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//show all windows
	THISVIEW->showBuildOptions(TRUE);

	//reenable everything
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//if we're in network mode, they can only undo the last build placement
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->getMessageButton()->enableUndo(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle common road undo stuff
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoRoad(int ID, int nHidden, int nChitless)
{
	int iLongest;
	int iTile1, iTile2;
	int iC1, iC2;
	BOOL bFree;

	//decode the id
	DECODE_ROAD(ID, bFree, iTile1, iC1, iTile2, iC2, iLongest);

	//undo the hidden tile
	undoHiddenTile(nHidden);

	//undo chitless tile
	undoChitlessTile(nChitless);
	
	//remove the last road placed from each of the board piece tiles
	m_pBoard[iTile1].removeRoad(iC1);
	m_pBoard[iTile2].removeRoad(iC2);

	//reset the turn time placement
	m_pBoard[iTile1].setTurnPlacement(iC1, -1);
	m_pBoard[iTile2].setTurnPlacement(iC2, -1);

	//add one to the players stock
	m_pGame->m_players[m_iPlayer].m_nStockRoads++;

	//if it was free, add one to the number of free roads left
	if(TRUE == bFree)
	{
		m_iFreeRoads++;
	}
	else
	{
		//increment one clay and one timber from bank
		incResource(RES_STAT_BUILD, m_iPlayer, RES_TIMBER);
		incResource(RES_STAT_BUILD, m_iPlayer, RES_CLAY);
	}

	//check for longest road points
	if(iLongest >= -1)
	{
		//dec their points
		decLongestRoad(m_iPlayer);

		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		//they stole it
		if(iLongest >= 0)
		{
			//inc the points of the person they stole from
			incLongestRoad(iLongest);

			//create it
			longestRoad(iLongest);
		}
	}
	//they might have added to it
	else if(iLongest == -2)
	{
		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		longestRoad(m_iPlayer);
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//recalculate road lengths
	calculateRoadLengths();
}

/////////////////////////////////////////////////////////////////////////////
// handle common ship unto stuff
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoShip(int ID, int nHidden, int nChitless)
{
	int i;
	int iLongest;
	int iTile1, iTile2;
	int iC1, iC2;
	int ID1, ID2;
	BOOL bFree;

	//decode the id (uses the same format as roads)
	DECODE_ROAD(ID, bFree, iTile1, iC1, iTile2, iC2, iLongest);

	//undo the hidden tile
	undoHiddenTile(nHidden);

	//undo chitless tile
	undoChitlessTile(nChitless);

	//remove the last road placed from each of the board piece tiles
	m_pBoard[iTile1].removeShip(iC1);
	m_pBoard[iTile2].removeShip(iC2);

	//reset the turn time placement
	m_pBoard[iTile1].setTurnPlacement(iC1, -1);
	m_pBoard[iTile2].setTurnPlacement(iC2, -1);

	//create search ids
	ID1 = ENCODE_SEL(iTile1, iC1);
	ID2 = ENCODE_SEL(iTile2, iC2);

	//remove these tiles from the array of ships built this turn
	for(i = m_nShipsThisTurn.GetSize() - 1; i >= 0; i--)
	{
		if(m_nShipsThisTurn[i] == ID1)
		{
			m_nShipsThisTurn.RemoveAt(i);
			continue;
		}
		if(m_nShipsThisTurn[i] == ID2)
		{
			m_nShipsThisTurn.RemoveAt(i);
			continue;
		}
	}

	//add one to the players stock
	m_pGame->m_players[m_iPlayer].m_nStockShips++;

	//if it was free, add one to the number of free roads left
	if(TRUE == bFree)
	{
		m_iFreeRoads++;
	}
	else
	{
		//increment one sheep and one timber from bank
		incResource(RES_STAT_BUILD, m_iPlayer, RES_TIMBER);
		incResource(RES_STAT_BUILD, m_iPlayer, RES_SHEEP);
	}

	//check for longest road points
	if(iLongest >= -1)
	{
		//dec their points
		decLongestRoad(m_iPlayer);

		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		//they stole it
		if(iLongest >= 0)
		{
			//inc the points of the player they stole from
			incLongestRoad(iLongest);

			//create it
			longestRoad(iLongest);
		}
	}
	//they might have added to it
	else if(iLongest == -2)
	{
		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		longestRoad(m_iPlayer);
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//recalculate road lengths
	calculateRoadLengths();
}

/////////////////////////////////////////////////////////////////////////////
// undo a ship move
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoShipMove(int ID, int nHidden, int nChitless, int nTurn)
{
	int iOwner;
	int iLongest;
	int iOT1, iT1;
	int iOC1, iC1;
	int iT2, iC2;
	int nCurrent;

	//decode the ID
	DECODE_SHIP_MOVE(ID, iOwner, iOT1, iOC1, iT1, iC1, iLongest);

	//undo hidden tiles
	undoHiddenTile(nHidden);
	
	//undo chitless tile
	undoChitlessTile(nChitless);

	//find the current opposite side
	iT2 = m_pBoard[iT1].getSide(iC1);
	iC2 = (iC1 + 3) % 6;

	//remove this ship
	m_pBoard[iT1].removeShip(iC1);
	m_pBoard[iT2].removeShip(iC2);

	//reset the turn placements
	m_pBoard[iT1].setTurnPlacement(iC1, -1);
	m_pBoard[iT2].setTurnPlacement(iC2, -1);

	//find the old opposite side
	iT2 = m_pBoard[iOT1].getSide(iOC1);
	iC2 = (iOC1 + 3) % 6;

	//add the ship back to that side
	m_pBoard[iOT1].addShip(iOC1, m_iPlayer);
	m_pBoard[iT2].addShip(iC2, m_iPlayer);

	//reset the turn placements
	m_pBoard[iOT1].setTurnPlacement(iOC1, nTurn);
	m_pBoard[iT2].setTurnPlacement(iC2, nTurn);

	//reset the players ship moved this turn flag
	m_pGame->m_players[m_iPlayer].m_bMovedShip = FALSE;

	//get the current longest road player
	nCurrent = longestPlayer();

	//check for longest road points
	if(iLongest >= -1)
	{
		//dec their points
		decLongestRoad(m_iPlayer);

		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		//they stole it
		if(iLongest >= 0)
		{
			//inc the points of the person who had it
			incLongestRoad(iLongest);

			//create it
			longestRoad(iLongest);
		}
	}
	//they might have added to it
	else if(iLongest == -2)
	{
		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		longestRoad(m_iPlayer);
	}
	//or they might have removed it from themselves by moving the ship
	else if((-3 == iLongest) && ((iOwner - 1) == m_iPlayer))
	{
		//reset the counter
		m_iLongestLength = 0;

		//erase the array
		m_iLongestRoad.clear();

		//reset the player
		longestRoad(m_iPlayer);

		//inc their points
		handleCommonLongestPoints(m_iPlayer, nCurrent);
	}

	//run trade route checking if needed
	if(TRUE == MAP->m_bUsesTradeRoutes)
	{
		tradeRoutes();
	}

	//recalculate road lengths
	calculateRoadLengths();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of a road
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoRoad(CUndoShip *pShip)
{
	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientUndoBuild(GL_PLACE_ROAD, pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless);
	}

	//handle common undo
	handleCommonUndoRoad(pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless);

	//prepare roads
	prepareRoads();

	//set the new draw mode
	setDrawingMode(GL_ROAD);

	//turn on hit tests
	setHitTest(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of a road
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoShip(CUndoShip *pShip)
{
	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientUndoBuild(GL_PLACE_SHIP, pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless);
	}

	//handle common undo
	handleCommonUndoShip(pShip->m_ID, pShip->m_nHidden, pShip->m_nChitless);

	//prepare ships
	prepareShips();

	//set the new draw mode
	setDrawingMode(GL_SHIP);

	//turn on hit tests
	setHitTest(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// common undo the placement of a city
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoCity(int ID)
{
	int iSide1, iSide2, iSide3;
	int iCorner;

	//deconstruct the id
	DECODE_CITY(ID, iSide1, iSide2, iSide3, iCorner);

	//reduce the city
	handleCommonReduceCity(m_iPlayer, iSide1, iCorner);

	//increment three ore and two wheat from bank
	incResource(RES_STAT_BUILD, m_iPlayer, RES_ORE, 3);
	incResource(RES_STAT_BUILD, m_iPlayer, RES_WHEAT, 2);

	//update the counts
	THISVIEW->updateCounts(m_iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of a city
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoCity(int ID)
{
	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientUndoBuild(GL_PLACE_CITY, ID);
	}

	//handle common undo
	handleCommonUndoCity(ID);

	//prepare cities
	prepareCities();

	//set the new draw mode
	setDrawingMode(GL_CITY);

	//turn on hit tests
	setHitTest(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo common settlement place
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoSettlement(int ID)
{
	int i;
	int iLongest;
	int iTile1, iTile2, iTile3;
	int iCorner;
	int nCurLongest;
	int nPortTile = -1;

	//decode the ID
	DECODE_SETTLEMENT(ID, iLongest, iTile1, iTile2, iTile3, iCorner);

	//remove the settlement
	handleCommonRemoveSettlement(m_iPlayer, iTile1, iCorner);

	//increment one clay, timber, wheat, and sheep from bank
	incResource(RES_STAT_BUILD, m_iPlayer, RES_TIMBER);
	incResource(RES_STAT_BUILD, m_iPlayer, RES_CLAY);
	incResource(RES_STAT_BUILD, m_iPlayer, RES_SHEEP);
	incResource(RES_STAT_BUILD, m_iPlayer, RES_WHEAT);

	//update the counts
	THISVIEW->updateCounts(m_iPlayer);

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iTile1].getType()))
	{
		nPortTile = iTile1;
	}

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iTile2].getType())) 
	{
		nPortTile = iTile2;
	}

	//check for a port tile
	if(TRUE == VALID_PORT(m_pBoard[iTile3].getType())) 
	{
		nPortTile = iTile3;
	}

	//see if we should check to remove it from the ports placed this turn
	if(-1 != nPortTile)
	{
		//look for it
		for(i = 0; i < m_nPortsThisTurn.GetSize(); i++)
		{
			if(nPortTile == m_nPortsThisTurn[i])
			{
				m_nPortsThisTurn.RemoveAt(i);
				break;
			}
		}
	}

	//if iLongest is 9, it means that the person got longest road by building
	//this settlement between a road and a ship
	if(9 == iLongest)
	{
		//create the longest road
		m_iLongestLength = 0;
		m_iLongestRoad.clear();

		if(-3 == longestRoad(m_iPlayer))
		{
			//decrease their points
			decLongestRoad(m_iPlayer);
		}

		//reclaculate road lengths
		calculateRoadLengths();
	}
	//see if someone had longest road before this settlement was placed
	else if(iLongest > 0)
	{
		//get current longest
		nCurLongest = longestPlayer();

		//take it away
		if(-1 != nCurLongest)
		{
			decLongestRoad(nCurLongest);
		}

		//regive it to this person
		incLongestRoad(iLongest - 1);

		//create the longest road
		m_iLongestLength = 0;
		m_iLongestRoad.clear();

		//redo it
		longestRoad(iLongest - 1);

		//recalculate road lengths and update everyone
		calculateRoadLengths();
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of a settlement
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoSettlement(int ID)
{
	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientUndoBuild(GL_PLACE_SETTLEMENT, ID);
	}

	//handle common undo
	handleCommonUndoSettlement(ID);

	//prepare settlements
	prepareSettlements();

	//set the new draw mode
	setDrawingMode(GL_SETTLEMENT);

	//turn on hit tests
	setHitTest(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of the robber
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoRobber(int ID)
{
	int iDie1, iDie2;

	//decode the ID
	DECODE_ROBBER(ID, iDie1, iDie2, m_iRobber, m_iRobberPlayer, m_bSoldier);

	//set the interaction mode to select
	m_bUseSelection = TRUE;

	//reset the selection
	m_iSelection = -1;

	//set to the next mode
	m_iMode = GL_PLACE_ROBBER;

	//turn on hit test
	setHitTest(TRUE);

	//hide everything
	THISVIEW->showBuildOptions(FALSE);

	//if false was the soldier, set last dice
	if(FALSE == m_bSoldier)
	{
		//set dice
		THISVIEW->getControlPanel()->setLastRoll(iDie1, iDie2);
	}

	//update the player panel
	THISVIEW->getControlPanel()->updateText();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();
	
	//make sure rolls are hidden
	THISVIEW->getControlPanel()->showRolls(FALSE);

	//toggle actions off
	ACTIONS->ShowWindow(SW_HIDE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo the placement of the pirate
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPirate(int ID)
{
	int iDie1, iDie2;

	//undo the ID (use robber code since it is the same)
	DECODE_ROBBER(ID, iDie1, iDie2, m_iPirate, m_iPiratePlayer, m_bSoldier);
	
	//sanity on old pirate
	if(99 == m_iPirate)
	{
		m_iPirate = -1;
	}

	//set the interaction mode to select
	m_bUseSelection = TRUE;

	//reset the selection
	m_iSelection = -1;

	//set to the next mode
	m_iMode = GL_PLACE_ROBBER;

	//turn on hit test
	setHitTest(TRUE);

	//hide everything
	THISVIEW->showBuildOptions(FALSE);

	//if false was the soldier, set last dice
	if(FALSE == m_bSoldier)
	{
		//set dice
		THISVIEW->getControlPanel()->setLastRoll(iDie1, iDie2);
	}

	//update the player panel
	THISVIEW->getControlPanel()->updateText();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();
	
	//make sure rolls are hidden
	THISVIEW->getControlPanel()->showRolls(FALSE);

	//toggle actions off
	ACTIONS->ShowWindow(SW_HIDE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// undo buying a dev card
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoBuyCard(int ID)
{
	int iCard;
	int iPlayer;

	//decode the ID
	DECODE_SEL(ID, iCard, iPlayer);

	//decrease the cards held
	if(CARD_VICTORY == iCard)
	{
		m_pGame->m_players[iPlayer].m_iCardHeld[CARD_VICTORY]--;
	}
	else
	{
		m_pGame->m_players[iPlayer].m_iCardBought[iCard]--;
	}

	//set the index back one
	m_iDevCardIndex--;

	//increase ORE, SHEEP, and WHEAT from bank
	incResource(RES_STAT_BUILD, iPlayer, RES_ORE);
	incResource(RES_STAT_BUILD, iPlayer, RES_WHEAT);
	incResource(RES_STAT_BUILD, iPlayer, RES_SHEEP);

	//reshow build options
	THISVIEW->showBuildOptions(TRUE);

	//update the player panel
	THISVIEW->updateCounts(iPlayer);

	//redo enablement
	THISVIEW->getControlPanel()->EnableDC();
}

/////////////////////////////////////////////////////////////////////////////
// undo playing a victory point
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPlayVictory(int ID)
{
	int iPlayer;

	//figure out the ID
	iPlayer = ID;

	//subtract a point from the player
	decPoints(iPlayer);

	//add a victory card back
	m_pGame->m_players[iPlayer].m_iCardHeld[CARD_VICTORY]++;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_VICTORY]--;

	//update the panel
	THISVIEW->updateCounts(iPlayer);

	//update the turn dialog
	THISVIEW->getControlPanel()->EnableDC();
}

/////////////////////////////////////////////////////////////////////////////
// undo playing a monopoly card
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPlayMonopoly(CUndoMonopoly *pMonopoly)
{
	int i;
	int iType;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardHeld[CARD_MONOPOLY]++;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_MONOPOLY]--;

	//they can now replay a card
	m_pGame->m_players[m_iPlayer].m_bPlayedCard = FALSE;

	//get the type they stole
	iType = pMonopoly->m_iType;

	//add all the resources back to everyone else
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		//skip self
		if(i == m_iPlayer) continue;

		//see if this player has any of this card
		if(pMonopoly->m_iPlayers[i])
		{
			//set the number stolen from player
			incResource(RES_STAT_LOST_STOLEN, i, iType, pMonopoly->m_iPlayers[i], m_iPlayer);

			//subtract from the stealer
			decResource(RES_STAT_MONOPOLY, m_iPlayer, iType, pMonopoly->m_iPlayers[i], i);

			//update the counts
			THISVIEW->updateCounts(i);
		}
	}

	//update the stats
	STAT->update(STAT_GOODS_RECV_STEAL);
	STAT->update(STAT_GOODS_LOST_STEAL);

	//update the player panel
	THISVIEW->updateCounts(m_iPlayer);

	//enable dev cards
	THISVIEW->getControlPanel()->EnableDC();

	//show the build options
	THISVIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo playing a road building card
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPlayRoadBuild()
{
	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardHeld[CARD_ROAD_BUILD]++;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_ROAD_BUILD]--;

	//reset any free roads
	m_iFreeRoads = 0;

	//they can now replay a card
	m_pGame->m_players[m_iPlayer].m_bPlayedCard = FALSE;

	//update the player panel
	THISVIEW->updateCounts(m_iPlayer);

	//enable dev cards
	THISVIEW->getControlPanel()->EnableDC();

	//show the build options
	THISVIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// undo playing a year of plenty card
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPlayYearPlenty(int ID)
{
	int i;
	int iAmount;
	int iMult = 10000;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardHeld[CARD_YEAR_PLENTY]++;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_YEAR_PLENTY]--;

	//they can now replay a card
	m_pGame->m_players[m_iPlayer].m_bPlayedCard = FALSE;

	//fill in the amounts
	for(i = 0; i < 5; i++)
	{
		//get the amount
		iAmount = ID / iMult; ID -= (iAmount * iMult);

		//decrease these resources to the bank
		decResource(RES_STAT_DEV, m_iPlayer, i, iAmount);

		//decrase the multiplier
		iMult /= 10;
	}

	//update the stats
	STAT->update(STAT_GOODS_RECV_DEVCARD);

	//update the player panel
	THISVIEW->updateCounts(m_iPlayer);

	//update build options
	THISVIEW->showBuildOptions(TRUE);

	//enable dev cards
	THISVIEW->getControlPanel()->EnableDC();
}

/////////////////////////////////////////////////////////////////////////////
// undo playing a soldier card
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoPlaySoldier(int iStat)
{
	//a soldier card is no longer being played
	m_bSoldier = FALSE;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardHeld[CARD_SOLDIER]++;

	//update the player data
	m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_SOLDIER]--;

	//they can now replay a card
	m_pGame->m_players[m_iPlayer].m_bPlayedCard = FALSE;

	//if the stat is > 0, the player had stolen largest army with this card
	if(iStat >= 0)
	{
		//they'd stolen it
		m_pGame->m_players[m_iPlayer].m_bLargestArmy = FALSE;

		//decrement points
		decPoints(m_iPlayer, 2);

		//now the other playe has it back
		m_pGame->m_players[iStat].m_bLargestArmy = TRUE;

		//increment points
		incPoints(iStat, 2);
	}
	//if it's -1, then they just got it and have lost it again
	else if(iStat == -1)
	{
		//they'd stolen it
		m_pGame->m_players[m_iPlayer].m_bLargestArmy = FALSE;

		//decrement points
		decPoints(m_iPlayer, 2);
	}

	//update the player panel
	THISVIEW->updateCounts(m_iPlayer);

	//set the interaction mode to off
	m_bUseSelection = FALSE;

	//reset the selection
	m_iSelection = -1;

	//set to the next mode
	m_iMode = GL_WAIT;

	//enable dev cards
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//if we're in-turn, update the turn text, otherwise show the rolls
	if(THISVIEW->getControlPanel()->inTurn())
	{
		THISVIEW->getControlPanel()->updateTurn();
	}
	else
	{
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle actions on
		ACTIONS->ShowWindow(SW_SHOW);
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// common trade undo
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoTrade(int iPartner, int *iTo, int *iFrom, int *iBank)
{
	int i;

	//undo the trade
	for(i = 0; i < 5; i++)
	{
		//check if it's to a partner
		if(iPartner >= 0)
		{
			//decrement the number of trades between these two
			m_pGame->m_players[m_iPlayer].m_iOtherTradeTo[iPartner][i] -= iTo[i];
			m_pGame->m_players[iPartner].m_iOtherTradeTo[m_iPlayer][i] -= iFrom[i];

			//decrement the cards received from each other
			m_pGame->m_players[m_iPlayer].m_iOtherTradeFrom[iPartner][i] -= iFrom[i];
			m_pGame->m_players[iPartner].m_iOtherTradeFrom[m_iPlayer][i] -= iTo[i];

			//increment and decrement from players
			incResource(RES_STAT_TRADE, m_iPlayer, i, iTo[i]);
			decResource(RES_STAT_TRADE, m_iPlayer, i, iFrom[i]);

			incResource(RES_STAT_TRADE, iPartner, i, iFrom[i]);
			decResource(RES_STAT_TRADE, iPartner, i, iTo[i]);
		}
		//otherwise, it's to the bank
		else
		{
			incResource(RES_STAT_TRADE_BANK, m_iPlayer, i, iTo[i], -2);
			decResource(RES_STAT_TRADE_BANK, m_iPlayer, i, iFrom[i], -2);
		}
	}

	//undo stats
	if(iPartner >= 0)
	{
		//decrease the trade numbers for these players
		m_pGame->m_players[m_iPlayer].m_iTradesOthers[iPartner]--;
		m_pGame->m_players[iPartner].m_iTradesOthers[m_iPlayer]--;
	}
	else
	{
		//decrease trade totals
		m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_2FOR1] -= iBank[TRADE_2FOR1];
		m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_3FOR1] -= iBank[TRADE_3FOR1];
		m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_4FOR1] -= iBank[TRADE_4FOR1];
	}

	//update the stat window
	STAT->update(STAT_TRADE_NUMBER);

	//update this players counts
	THISVIEW->updateCounts(m_iPlayer);

	if(iPartner >= 0)
	{
		//update this players counts
		THISVIEW->updateCounts(iPartner);
	}
}

/////////////////////////////////////////////////////////////////////////////
// undo a jungle trade
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonUndoJungle(int nJungles, int *nFrom)
{
	int i;

	//add to the player's jungles
	m_pGame->m_players[m_iPlayer].m_nJungles += nJungles;

	//undo the trade
	for(i = 0; i < 5; i++)
	{
		//update the resources
		if(0 < nFrom[i])
		{
			decResource(RES_STAT_TRADE_JUNGLE, m_iPlayer, i, nFrom[i]);
		}
	}

	//update this players counts
	THISVIEW->updateCounts(m_iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// undo a trade
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoTrade(CUndoTrade *pTrade)
{
	int iPartner;

	//get the trading partner
	iPartner = pTrade->m_iPartner;

	//send the message
	if(GAME_MODE_NETWORK == m_pGame->m_iMode && 0 > iPartner)
	{
		THISVIEW->clientUndoAutoTrade(pTrade->m_iTo, pTrade->m_iFrom, pTrade->m_iBank);

		//disable undo if it was not an auto-trade
		if(-1 == iPartner)
		{
			THISVIEW->getMessageButton()->enableUndo(FALSE);
		}
	}

	//handle undo trade
	handleCommonUndoTrade(iPartner, pTrade->m_iTo, pTrade->m_iFrom, pTrade->m_iBank);

	//update the build options
	THISVIEW->showBuildOptions(TRUE);

	//update trading
	THISVIEW->getControlPanel()->EnableTrade();
}

/////////////////////////////////////////////////////////////////////////////
// undo a steal (the BOOL signifies if we need to undo again)
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoSteal(int ID)
{
	BOOL bPirate;
	int iPlayer;
	int iRes;

	//decipher the ID
	DECODE_STEAL(ID, bPirate, iPlayer, iRes);

	//if the res is a five, they chose not to steal anything
	if(iRes != 5)
	{
		decResource(RES_STAT_STOLEN, m_iPlayer, iRes, 1, iPlayer);
		incResource(RES_STAT_LOST_STOLEN, iPlayer, iRes, 1, m_iPlayer);

		//update the player panel
		THISVIEW->updateCounts(m_iPlayer);
		THISVIEW->updateCounts(iPlayer);

		//update the stats
		STAT->update(STAT_GOODS_RECV_STEAL);
		STAT->update(STAT_GOODS_LOST_STEAL);
	}

	//make sure the rolls are hidden
	THISVIEW->getControlPanel()->showRolls(FALSE);

	//toggle actions on
	ACTIONS->ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// keep things in order when redoing a steal
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::handleSteal(BOOL bPirate)
{
	int iRet;
	BOOL bSteal;
	CMultiUseDialog dlg;

	//return to the steal dialog
	if(TRUE == bPirate)
	{
		bSteal = initPirateSteal(&dlg);
	}
	else
	{
		bSteal = initSteal(&dlg);
	}

	//if we need to rerun it, rerun it
	if(TRUE == bSteal)
	{
		//run it
		iRet = dlg.DoModal();
		
		//if they cancelled, treat as an undo
		if(IDCANCEL == iRet)
		{
			return TRUE;
		}
	}

	//show everything again
	if(CONTROLPANEL->inTurn())
	{
		THISVIEW->getControlPanel()->updateTurn();
	}
	//we're before, so show the roll possibilities
	else
	{
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle actions on
		ACTIONS->ShowWindow(SW_SHOW);
	}

	//no need to undo
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// undo losing cards to a 7
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoLoseCards(CUndoTrade *pLose)
{
	int i;
	int iPlayer;

	//get the player
	iPlayer = pLose->m_iPartner;

	//run through the resources and add them back
	for(i = 0; i < 5; i++)
	{
		//add the resource back in from the bank
		incResource(RES_STAT_LOST_7, iPlayer, i, pLose->m_iTo[i]);
	}

	//update stats
	STAT->update(STAT_GOODS_LOST_7);

	//update the player panel
	THISVIEW->updateCounts(iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// undo players gaining cards on a gold roll
/////////////////////////////////////////////////////////////////////////////
int *CGLView::undoGold(CUndoGold *pGold, int *iPlayers)
{
	int i, j;
	int iCount;

	//clear the memory
	::ZeroMemory(iPlayers, sizeof(iPlayers));

	//run through the list of players, compiling counts for what they got
	for(i = 0; i < m_iNumPlayers; i++)	
	{
		//reset the count
		iCount = 0;

		//remove the resource
		for(j = 0; j < 5; j++)
		{
			decResource(RES_STAT_GOLD, i, j, pGold->m_iResources[i][j]);

			//add to the count
			iCount += pGold->m_iResources[i][j];
		}

		//update the player
		THISVIEW->updateCounts(i);

		//set the player total
		iPlayers[i] = iCount;
	}

	//update the stats
	STAT->update(STAT_GOODS_RECV_GOLD);

	//return the player list
	return iPlayers;
}

/////////////////////////////////////////////////////////////////////////////
// undo swapping a chitless tile
/////////////////////////////////////////////////////////////////////////////
void CGLView::undoChitless(int ID)
{
	int nOld;

	//decode the ID
	DECODE_CHITLESS(ID, m_nChitlessTile, nOld);

	//set the chitless tile's new roll and tex
	m_pBoard[nOld].setRoll(m_pBoard[m_nChitlessTile].getRoll());
	m_pBoard[nOld].setRollTex(m_pBoard[m_nChitlessTile].getRollTex());
	m_pBoard[nOld].setRollTexType(m_pBoard[m_nChitlessTile].getRollTexType());
	m_pBoard[nOld].setChitless(FALSE);

	//now set the chitless tile
	m_pBoard[m_nChitlessTile].setRoll(0);
	m_pBoard[m_nChitlessTile].setRollTex(0);
	m_pBoard[m_nChitlessTile].setChitless(TRUE);

	//now set things up again
	initChitless();
}

/////////////////////////////////////////////////////////////////////////////
// handle rerunning the gold dialog
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::handleGold(int *iPlayers)
{
	CMultiUseDialog dlg;

	//run the dialog again
	dlg.setMode(MULTIUSE_GOLD);
	dlg.setPlayers(iPlayers);

	//run it
	if(IDOK == dlg.DoModal())
	{
		//run it
		handleCommonGold();

		return FALSE;
	}

	//they cancelled, so we undo the last action too
	return TRUE;
}