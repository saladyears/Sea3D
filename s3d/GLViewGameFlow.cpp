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
#include "SettlersView.h"
#include "TradeBankDialog.h"
#include "TradeJungleDialog.h"
#include "MultiUseDialog.h"
#include "rand.h"
#include "UndoOne.h"
#include "defineNetwork.h"
#include "defineStat.h"
#include "defineSounds.h"
#include "defineVariants.h"
#include "defineMap.h"
#include "defineCode.h"

/////////////////////////////////////////////////////////////////////////////
// setup the next mode based on what we're leaving
/////////////////////////////////////////////////////////////////////////////
void CGLView::setNextMode(void)
{
	switch(m_iMode)
	{
	//switch from waiting for player selection to player placement
	case GL_NORMAL:

		//set the interaction mode to select
		m_bUseSelection = TRUE;

		//reset the selection
		m_iSelection = -1;
		
		//if this map has special port placement by players, we have to
		//initialize the mechanism and set the drawing mode, as long as we are
		//the server, or it's a hotseat game
		if((TRUE == MAP->m_bAssignsPorts) && (GAME_MODE_HOTSEAT == m_pGame->m_iMode))
		{
			//init function here
			initPortPlacement();

			//set the mode
			m_iMode = GL_PLACE_PORT;
		}
		else
		{
			//set to the next mode
			m_iMode = GL_PLACE_PLAYERS;
		}

		//turn on hit tests
		setHitTest(TRUE);

		//set to the panel
		THISVIEW->initPanel();

		//set the correct text in the control panel if we're in port placement
		if((TRUE == MAP->m_bAssignsPorts) && (GAME_MODE_HOTSEAT == m_pGame->m_iMode))
		{
			//update control panel
			THISVIEW->getControlPanel()->updateText();
		}

		break;
	//switch from placing ports to player placement
	case GL_PLACE_PORT:
		//set the interaction mode to select
		m_bUseSelection = TRUE;

		//reset the selection
		m_iSelection = -1;
		
		//set to the next mode
		m_iMode = GL_PLACE_PLAYERS;

		//turn on hit tests
		setHitTest(TRUE);

		break;
	//switch from player placement to waiting mode
	case GL_PLACE_PLAYERS:
		//if we're in tournament road, we need to go to second road placement
		if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) && (IS_VARIANT(VARIANT_TOURNAMENT_START)))
		{
			//we are in tournament roads mode now
			m_bTournamentRoads = TRUE;

			//set the interaction mode to select
			m_bUseSelection = TRUE;

			//reset the selection
			m_iSelection = -1;
			
			//set the new mode
			m_iMode = GL_PLACE_THIRD_ROAD;

			//turn on hit tests
			setHitTest(TRUE);

			//update the display text
			THISVIEW->getControlPanel()->updateText();

			//determine possible roads
			prepareRoads();
		}
		//otherwise, move on to the normal game
		else
		{
			//disable select
			m_bUseSelection = FALSE;

			//reset the selection
			m_iSelection = -1;

			//set the new mode
			m_iMode = GL_WAIT;

			//show the buttons and actions on the control panel
			THISVIEW->getControlPanel()->showRolls(TRUE);

			//toggle actions on
			ACTIONS->ShowWindow(SW_SHOW);

			//toggle controls
			CONTROLS->ShowWindow(SW_SHOW);
		}

		break;
	//start the game
	case GL_PLACE_THIRD_ROAD:

		//disable select
		m_bUseSelection = FALSE;

		//reset the selection
		m_iSelection = -1;

		//set the new mode
		m_iMode = GL_WAIT;

		//show the buttons and actions on the control panel
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle actions on
		ACTIONS->ShowWindow(SW_SHOW);
		
		//toggle controls
		CONTROLS->ShowWindow(SW_SHOW);

		break;
	case GL_WAIT:
		//set the new mode
		m_iMode = GL_SHOW_ROLLED;

		break;
	case GL_VOLCANO:
	case GL_SHOW_ROLLED:
	case GL_PLACE_ROAD:
	case GL_PLACE_SHIP:
		//set the new mode
		m_iMode = GL_WAIT;

		break;
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// setup the previous mode based on what we're leaving
/////////////////////////////////////////////////////////////////////////////
void CGLView::setPreviousMode()
{
	switch(m_iMode)
	{
	//wait mode
	case GL_VOLCANO:
	case GL_WAIT:
	case GL_PLACE_THIRD_ROAD:
		if(TRUE == m_pGame->m_bInGame)
		{
			m_iMode = GL_SHOW_ROLLED;
		}
		//return to player placing
		else
		{
			//set the interaction mode to select
			m_bUseSelection = TRUE;

			//set to the right mode
			m_iMode = GL_PLACE_PLAYERS;

			//turn on hit tests
			setHitTest(TRUE);
			
			//reset the player to the right spot
			m_iPlayer = -1;

			//toggle actions off
			ACTIONS->ShowWindow(SW_HIDE);

			//toggle controls
			CONTROLS->ShowWindow(SW_HIDE);
		}

		break;
	//returning from showing the rolled values
	case GL_PLACE_ROBBER:
	case GL_SHOW_ROLLED:
		//set the previous drawing mode
		m_iMode = GL_WAIT;

		break;

	//returning from player placement to player selection
	case GL_PLACE_PLAYERS:

		//set the mode
		m_iMode = GL_NORMAL;

		break;
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// set the chips based on the selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::setChips()
{
	int i;
	int nCorner;
	int nTile;
	int nNext = 0;
	int nChip = 0;
	BOOL b34;
	BOOL bValid;
	BOOL bClockwise;
	BOOL bContinue = TRUE;
	vector <BOOL> bTiles;
	const int *pRoll;

	//determine which corner to start on
	nCorner = int(RAND * 6.0);

	//get the initial tile
	nTile = MAP->m_iRollPlaces[nCorner];

	//determine clockwise or non clockwise
	bClockwise = int(RAND * 2.0) ? TRUE : FALSE;

	//create the array of tiles used by the algorithm
	bTiles.resize(m_nTiles, FALSE);

	//see which type of chipset we need to use
	b34 = MAP->m_iChipsUsed & MAP_USES_34_CHIPS;

	//get the list of lookup values
	pRoll = b34 ? &u_34Chips[0] : &u_56Chips[0];

	//continue looping through the tiles while we can
	while(TRUE == bContinue)
	{
		//set the current tile
		if(m_pBoard[nTile].getType() != RES_DESERT)
		{
			m_pBoard[nTile].setRoll(pRoll[nChip]);
			m_pBoard[nTile].setRollTex(nChip);

			//increment the chip counter
			nChip++;
		}
		//otherwise, set the robber on the desert
		else
		{
			m_iRobber = nTile;
		}

		//set this tile as used
		bTiles[nTile] = TRUE;

		//if we have reached a tile that is an ocean tile or already used,
		//we must adjust the corner
		for(i = 0, bValid = FALSE; i < 6; i++)
		{
			//get the next tile
			if(TRUE == bClockwise)
			{
				nNext = m_pBoard[nTile].getSide((nCorner + 4) % 6);
			}
			else
			{
				nNext = m_pBoard[nTile].getSide((nCorner + 2) % 6);
			}

			if( (TRUE == VALID_OCEAN(m_pBoard[nNext].getType())) ||
				(TRUE == bTiles[nNext]))
			{
				//set the new corner and try it again
				if(TRUE == bClockwise)
				{
					//subtract one
					nCorner = (nCorner + 5) % 6;
				}
				else
				{
					//add one
					nCorner = (nCorner + 1) % 6;
				}
			}
			else
			{
				bValid = TRUE;
				break;
			}
		}

		//see if we're done
		if(FALSE == bValid)
		{
			bContinue = FALSE;
			continue;
		}

		//set the new tile
		nTile = nNext;
	}
}

/////////////////////////////////////////////////////////////////////////////
// checks to see if a road belonging to the player is next to this corner
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::roadCheck(int iTile, int iCorner)
{
	int iMinus;
	
	//set the minus value
	iMinus = (iCorner + 5) % 6;

	//check this side
	if(m_pBoard[iTile].getRoad(iCorner) == m_iPlayer) return TRUE;
	if(m_pBoard[iTile].getShip(iCorner) == m_iPlayer) return TRUE;

	//check minus side
	if(m_pBoard[iTile].getRoad(iMinus) == m_iPlayer) return TRUE;
	if(m_pBoard[iTile].getShip(iMinus) == m_iPlayer) return TRUE;

	//check straight out side
	iTile = m_pBoard[iTile].getSide(iCorner);
	iCorner = (iCorner + 4) % 6;
	if(m_pBoard[iTile].getRoad(iCorner) == m_iPlayer) return TRUE;
	if(m_pBoard[iTile].getShip(iCorner) == m_iPlayer) return TRUE;

	//no road
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// checks to see if a city is on a certain part of the tile
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::cityCheck(int iTile, int iCorner)
{
	int i;
	int iPlus, iMinus;

	//run two checks
	for(i = 0; i < 2; i++)
	{
		//sanity
		if(-1 == iTile)
		{
			continue;
		}

		//set the plus and minus for this tile
		iMinus = (iCorner + 5) % 6;
		iPlus = (iCorner + 1) % 6;
	
		//check this tile
		if(m_pBoard[iTile].getCity(iMinus) || m_pBoard[iTile].getCity(iPlus) || m_pBoard[iTile].getCity(iCorner))
		{
			return TRUE;
		}

		//check neighboring tile
		iTile = m_pBoard[iTile].getSide((iCorner + 5) % 6);
		iCorner = (iCorner + 2) % 6;
	}
	
	//no cities found
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// determine whether a player other than the current player owns a city
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::cityBlock(int iTile, int iCorner)
{
	int ID;
	
	//get the city ID on this corner
	ID = m_pBoard[iTile].getCity(iCorner);

	//see if it's empty or owned by this player (not blocked)
	if(!ID || (ID % 10) == m_iPlayer) return FALSE;

	//blocked
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// makes sure no roads are going to built on the sea
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::seaBlock(int iTile, int iSide)
{
	int iCompare;
	BOOL bSea;

	//get the comparison tile
	iCompare = m_pBoard[iTile].getSide(iSide);

	//see if both of these tiles are ocean tiles
	bSea = (TRUE == VALID_OCEAN(m_pBoard[iTile].getType())) && 
			(TRUE == VALID_OCEAN(m_pBoard[iCompare].getType()));

	return bSea;
}

/////////////////////////////////////////////////////////////////////////////
// makes sure no ships are going to built on the land
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::landBlock(int iTile, int iSide)
{
	int iCompare;
	BOOL bLand;

	//get the comparison tile
	iCompare = m_pBoard[iTile].getSide(iSide);

	//see if both of these tiles are ocean tiles
	bLand = (TRUE == VALID_LAND(m_pBoard[iTile].getType())) && 
			(TRUE == VALID_LAND(m_pBoard[iCompare].getType()));

	return bLand;
}

/////////////////////////////////////////////////////////////////////////////
// increments a given resource
/////////////////////////////////////////////////////////////////////////////
int CGLView::incResource(int iType, int iPlayer, int iResource, int iAmount, int iOther)
{
	CPlayerGame *pGame;

	//first see if it's a valid resource
	if(!VALID_RESOURCE(iResource)) return -1;

	//set the pointer
	pGame = &(m_pGame->m_players[iPlayer]);

	//remove from the bank
	if((TRUE == IS_BANK(iType)) && (RES_GOLD != iResource))
	{
		//if they want more than the bank has, adjust the amount
		if((iAmount > m_pGame->m_iBankRes[iResource]) && (RES_STAT_TRADE_JUNGLE != iType))
		{
			iAmount += (m_pGame->m_iBankRes[iResource] - iAmount);
		}

		//subtract from the bank
		m_pGame->m_iBankRes[iResource] -= iAmount;
	}

	//increase current (unless it's a loss to the board)
	if((iType != RES_STAT_LOST_BOARD) && (RES_GOLD != iResource))
	{
		pGame->m_iRes[iResource] += iAmount;
	}

	//now handle stats
	switch(iType)
	{
	case RES_STAT_DISCOVER:		pGame->m_iResDiscover[iResource] += iAmount; break;
	case RES_STAT_GOLD:			pGame->m_iResGold[iResource] += iAmount; break;
	case RES_STAT_BOARD:		pGame->m_iResBoard[iResource] += iAmount; break;
	case RES_STAT_STOLEN:		pGame->m_iResStolen[iOther][iResource] += iAmount; break;
	case RES_STAT_DEV:			pGame->m_iResDev[iResource] += iAmount; break;
	case RES_STAT_MONOPOLY:		pGame->m_iResStolen[iOther][iResource] += iAmount; break;
	case RES_STAT_LOST_7:		pGame->m_iLostRoll[iResource] -= iAmount; break;
	case RES_STAT_LOST_STOLEN:	pGame->m_iLostStolen[iOther][iResource] -= iAmount; break;
	case RES_STAT_LOST_BOARD:	pGame->m_iLostBoard[iOther][iResource] += iAmount; break;
	case RES_STAT_TRADE_BANK: 
		if(iOther == -1)		pGame->m_iBankTradeFrom[iResource] += iAmount;
		else					pGame->m_iBankTradeTo[iResource] -= iAmount;
		break;
	case RES_STAT_BUILD:
	case RES_STAT_ADJUST:
	case RES_STAT_TRADE:	break;
	}

	//return the actual amount added
	return iAmount;
}

/////////////////////////////////////////////////////////////////////////////
// decrements a given resource
/////////////////////////////////////////////////////////////////////////////
void CGLView::decResource(int iType, int iPlayer, int iResource, int iAmount, int iOther)
{
	CPlayerGame *pGame;

	//first see if it's a valid resource
	if(!VALID_RESOURCE(iResource)) return;

	//set the pointer
	pGame = &(m_pGame->m_players[iPlayer]);

	//if it's from the bank, increase from the bank
	if((TRUE == IS_BANK(iType)) && (RES_GOLD != iResource))
	{
		//increment the amount in the bank
		m_pGame->m_iBankRes[iResource] += iAmount;
	}

	//decrease current (unless it's a loss to the board)
	if((iType != RES_STAT_LOST_BOARD) && (RES_GOLD != iResource))
	{
		pGame->m_iRes[iResource] -= iAmount;
	}

	//now handle stats
	switch(iType)
	{
	case RES_STAT_DISCOVER:		pGame->m_iResDiscover[iResource] -= iAmount; break;
	case RES_STAT_GOLD:			pGame->m_iResGold[iResource] -= iAmount; break;
	case RES_STAT_BOARD:		pGame->m_iResBoard[iResource] -= iAmount; break;
	case RES_STAT_STOLEN:		pGame->m_iResStolen[iOther][iResource] -= iAmount; break;
	case RES_STAT_DEV:			pGame->m_iResDev[iResource] -= iAmount; break;
	case RES_STAT_MONOPOLY:		pGame->m_iResStolen[iOther][iResource] -= iAmount; break;
	case RES_STAT_LOST_7:		pGame->m_iLostRoll[iResource] += iAmount; break;
	case RES_STAT_LOST_STOLEN:	pGame->m_iLostStolen[iOther][iResource] += iAmount; break;
	case RES_STAT_LOST_BOARD:	pGame->m_iLostBoard[iOther][iResource] -= iAmount; break;
	case RES_STAT_TRADE_BANK: 
		if(iOther == -1)		pGame->m_iBankTradeTo[iResource] += iAmount;
		else					pGame->m_iBankTradeFrom[iResource] -= iAmount;
		break;
	case RES_STAT_BUILD:
	case RES_STAT_TRADE:	break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// do special things on a seven roll
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSeven(int iDie1, int iDie2)
{
	//set dice totals
	if(GAME_MODE_NETWORK == m_pGame->m_iMode && m_pGame->m_iIndex != m_iPlayer)
	{
		//set dice totals
		THISVIEW->getControlPanel()->setLastRoll(iDie1, iDie2);
		THISVIEW->getControlPanel()->showGoods(TRUE);
	}

	//once we've done it once, we've done it
	m_pGame->m_bInGame = TRUE;

	//see if anyone loses cards
	if(-1 == handleLoseCards()) 
	{
		//if they cancel, act as if nothing happened
		return;
	}

	//add it to the list
	addUndo(m_pGame->m_bInGame ? UNDO_ROLL : UNDO_FIRST_ROLL, new CUndoRoll(7, NULL, NULL, NULL));

	//if this is not network mode, continue on
	if(GAME_MODE_NETWORK != m_pGame->m_iMode)
	{
		handleRoll(7);
	}
}

/////////////////////////////////////////////////////////////////////////////
// we've got a dice roll, increment everyone's resources
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleRoll(int iRoll)
{
	int i, j, k, iIndex;
	int iTile;
	int iType;
	int iSettle, iCity;
	int iCount = 0;
	int iTotal;
	int iGoods[6][6];
	int iGoodsLost[6][6];
	int nJungles[6];
	CString strList, strTemp;
	CString str, strName;
	vector <CString> strArray;

	//resize string array
	strArray.resize(m_iNumPlayers);

	//see if we even need to to do this
	if(iRoll == 7)
	{
		//get things read for the seven roll
		initSeven();

		goto end;
	}

	//reset the tiles array
	m_iLastTiles.clear();

	//run through the list of tiles and find this roll
	for(i = 0; i < m_nTiles; i++)
	{
		//check for ocean tiles
		if(TRUE == VALID_OCEAN(m_pBoard[i].getType()))
		{
			continue;
		}

		//compare roll values
		if(m_pBoard[i].getRoll() == iRoll)
		{
			//add it
			m_iLastTiles.push_back(i);
			iCount++;
		}
	}

	//reset the goods and goods lost arrays
	::ZeroMemory(iGoods, sizeof(iGoods));
	::ZeroMemory(iGoodsLost, sizeof(iGoodsLost));
	::ZeroMemory(nJungles, sizeof(nJungles));

	//check player settlements and cities
	//start with current player and wrap around so that if the bank runs out, the
	//correct people get resources
	for(i = 0, iIndex = m_iPlayer; i < m_iNumPlayers; i++, iIndex++)
	{
		//see if we're over
		if(iIndex >= m_iNumPlayers) iIndex = 0;

		//reset total count
		iTotal = 0;

		//set the player name
		strList = m_pGame->m_players[iIndex].m_player.getFirstName();
		strList += ":";

		//settlements and cities
		for(j = 0; j < iCount; j++)
		{
			//get the selected tile
			iTile = m_iLastTiles[j];

			//see how many settlements on this tile
			iSettle = m_pGame->m_players[iIndex].m_iSettlements[iTile];
			iCity = m_pGame->m_players[iIndex].m_iCities[iTile];

			//get the type of resource this tile is
			iType = m_pBoard[iTile].getType();

			//check for volcanoes
			if((RES_VOLCANO == iType) && IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
			{
				iType = RES_GOLD;
			}
			
			//increment settlements
			for(k = 0; k < iSettle; k++)
			{
				//handle jungles
				if(RES_JUNGLE == iType)
				{
					if(iTile != m_iRobber)
					{
						m_pGame->m_players[iIndex].m_nJungles++;
						nJungles[iIndex]++;
					}
					continue;
				}

				if(iTile != m_iRobber)
				{
					//increment resources gained from bank
					iGoods[iIndex][iType] += incResource(RES_STAT_BOARD, iIndex, iType);
				}
				else
				{
					//increment resources lost
					iGoodsLost[iIndex][iType] += incResource(RES_STAT_LOST_BOARD, iIndex, iType, 1, m_iRobberPlayer);
				}
			}

			//increment cities
			for(k = 0; k < iCity; k++)
			{
				//handle jungles
				if(RES_JUNGLE == iType)
				{
					if(iTile != m_iRobber)
					{
						m_pGame->m_players[iIndex].m_nJungles += 2;
						nJungles[iIndex] += 2;
					}
					continue;
				}

				//make sure we have it
				if(iTile != m_iRobber)
				{
					//increment resources gained from bank
					iGoods[iIndex][iType] += incResource(RES_STAT_BOARD, iIndex, iType, 2);
				}
				else
				{
					//increment resources lost
					iGoodsLost[iIndex][iType] += incResource(RES_STAT_LOST_BOARD, iIndex, iType, 2, m_iRobberPlayer);
				}
			}
		}

		//create the list
		for(j = RES_TIMBER; j < RES_DESERT; j++)
		{
			if(iGoods[iIndex][j])
			{
				strTemp.Format(" %d %s,", iGoods[iIndex][j], getResName(j));
				strList += strTemp;
				iTotal += iGoods[iIndex][j];
			}
		}

		//handle jungles
		if(nJungles[iIndex])
		{
			iTotal += nJungles[iIndex];
			strTemp.Format(" %d %s%s,", nJungles[iIndex], getResName(RES_JUNGLE), (nJungles[iIndex] == 1) ? "y" : "ies");
			strList += strTemp;
		}

		//remove the last comma
		strList = strList.Left(strList.GetLength() - 1);

		//update player counts
		THISVIEW->updateCounts(iIndex);

		//update goods information
		if(0 < iTotal)
		{
			strArray[iIndex] = strList;
		}
	}

	//update the stats
	STAT->update(STAT_GOODS_RECV_BOARD);
	STAT->update(STAT_GOODS_LOST_BOARD);

	//set the next mode (show what settlements/cities just got rolled)
	setNextMode();

end:

	//now run through and update the counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->getControlPanel()->updateGoods(!strArray[i].IsEmpty(), i, strArray[i]);
	}

	if(7 != iRoll)
	{
		//add this roll to the list
		addUndo(m_pGame->m_bInGame ? UNDO_ROLL : UNDO_FIRST_ROLL, new CUndoRoll(iRoll, (int *) iGoods, (int *) iGoodsLost, (int *) nJungles));
	}

	//once we've done it once, we've done it
	m_pGame->m_bInGame = TRUE;
	
	//now handle gold
	if(7 != iRoll)
	{
		initGold(iGoods);
	}
}

/////////////////////////////////////////////////////////////////////////////
// initialize everything for handling gold roll
/////////////////////////////////////////////////////////////////////////////
void CGLView::initGold(int iGoods[6][6])
{
	int i;
	int iGold[6];
	BOOL bGold = FALSE;

	//run through the players
	for(i = 0; i < 6; i++)
	{
		//if we're in network mode, we only care about ourselves
		if( (GAME_MODE_NETWORK == m_pGame->m_iMode) &&
			(i != m_pGame->m_iIndex))
		{
			continue;
		}

		//copy the gold value
		iGold[i] = iGoods[i][RES_GOLD];

		//see if we got any
		if(0 < iGold[i])
		{
			bGold = TRUE;
		}
	}

	//if in hotseat mode, we need to run the selection dialog for all
	if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) && (TRUE == bGold))
	{
		runGold(iGold);
	}
	//send the message if we're not the person whose turn it is
	else if(GAME_MODE_NETWORK == m_pGame->m_iMode) 		
	{
		if(m_iPlayer != m_pGame->m_iIndex)
		{
			//send the message
			THISVIEW->clientWaitForGold(iGold[m_pGame->m_iIndex]);
		}
		else
		{
			//set the gold value for the player whose turn it is
			m_nGetsGold = iGold[m_pGame->m_iIndex];
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// run the gold dialog
/////////////////////////////////////////////////////////////////////////////
void CGLView::runGold(int *iPlayers)
{
	CMultiUseDialog dlg;

	//set up the dialog
	dlg.setMode(MULTIUSE_GOLD);
	dlg.setPlayers(iPlayers);

	//set the dialog
	THISVIEW->setDialog(&dlg);
	
	//run it
	if(IDOK == dlg.DoModal())
	{
		//run it
		handleCommonGold();
	}
	else
	{
		undo();
	}

	//reset the dialog
	THISVIEW->setDialog(NULL);

	//send it to the server if needed
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientSelectGold();
	}
}

/////////////////////////////////////////////////////////////////////////////
// initialize everything for handling a seven roll
/////////////////////////////////////////////////////////////////////////////
void CGLView::initSeven()
{
	CString str;

	if(GAME_MODE_NETWORK != m_pGame->m_iMode || GAME->m_iIndex == m_iPlayer)
	{
		//set the interaction mode to select
		m_bUseSelection = TRUE;

		//reset the selection
		m_iSelection = -1;

		//set to the next mode
		m_iMode = GL_PLACE_ROBBER;

		//start hit tests
		setHitTest(TRUE);

		//set the person who played the robber last
		m_iRobberPlayer = m_iPlayer;

		//update the player panel
		THISVIEW->getControlPanel()->updateText();
		THISVIEW->getControlPanel()->EnableDC();
		THISVIEW->getControlPanel()->EnableTrade();
		THISVIEW->getControlPanel()->EnableEndTurn();

		//redraw the robber place
		Invalidate();
	}

	//if we're in network mode and it's not our turn, then do nothing
	if(GAME_MODE_NETWORK == GAME->m_iMode && m_iPlayer != GAME->m_iIndex)
	{
		if(MAP->m_iMode & MAP_SEAFARERS)
		{
			//set the name of the punk who's laying down the robber
			str.Format("Waiting for %s to place the robber or pirate.", GAME->m_players[m_iPlayer].m_player.getName());
		}
		else
		{
			//set the name of the punk who's laying down the robber
			str.Format("Waiting for %s to place the robber.", GAME->m_players[m_iPlayer].m_player.getName());
		}

		CONTROLPANEL->setPanelText(str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// initialize the port placement mechanism
/////////////////////////////////////////////////////////////////////////////
void CGLView::initPortPlacement()
{
	//generate the list
	generatePortPlace();

	//now that we've got the array set up, we need to start letting people
	//place them

	//start with a random player
	m_iPlayer = int(RAND * m_iNumPlayers);

	//cap it (it's a one in 4 billion chance, but hey)
	if(m_iPlayer >= m_iNumPlayers) m_iPlayer = m_iNumPlayers - 1;

	//prepare the port placements
	preparePorts();
}

/////////////////////////////////////////////////////////////////////////////
// return a resource name
/////////////////////////////////////////////////////////////////////////////
CString CGLView::getResName(int iResource, BOOL bLower)
{
	CString str;
	switch(iResource)
	{
	case RES_TIMBER:		str = "Timber"; break;
	case RES_SHEEP:			str = "Sheep"; break;
	case RES_WHEAT:			str = "Wheat"; break;
	case RES_ORE:			str = "Ore"; break;
	case RES_CLAY:			str = "Clay"; break;
	case RES_DESERT:		str = "Desert"; break;
	case RES_OCEAN:			str = "Ocean"; break;
	case RES_PORT3:			str = "3 for 1 Port"; break;
	case RES_PORTTIMBER:	str = "Timber Port"; break;
	case RES_PORTWHEAT:		str = "Wheat Port"; break;
	case RES_PORTORE:		str = "Ore Port"; break;
	case RES_PORTCLAY:		str = "Clay Port"; break;
	case RES_PORTSHEEP:		str = "Sheep Port"; break;
	case RES_GOLD:			str = "Gold"; break;
	case RES_JUNGLE:		str = "Discover"; break;
	}

	//see if we want lower case
	if(bLower) str.MakeLower();

	//should never get here
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// return a development card name
/////////////////////////////////////////////////////////////////////////////
CString CGLView::getDevName(int iCard)
{
	CString str;
	switch(iCard)
	{
	case CARD_VICTORY: str = "Victory Point"; break;
	case CARD_MONOPOLY:	str = "Monopoly"; break;
	case CARD_ROAD_BUILD: str = "Road Building"; break;
	case CARD_YEAR_PLENTY: str = "Year of Plenty"; break;
	case CARD_SOLDIER:	str = "Soldier"; break;
	}

	//should never get here
	return str;
}

/////////////////////////////////////////////////////////////////////////////
// return the string name of a language
/////////////////////////////////////////////////////////////////////////////
CString CGLView::getLangName(int iLang)
{
	switch(iLang)
	{
	case LANGUAGE_ENGLISH:		return "English";
	case LANGUAGE_DUTCH:		return "Dutch";
	case LANGUAGE_GERMAN:		return "German";
	case LANGUAGE_SPANISH:		return "Spanish";
	case LANGUAGE_ITALIAN:		return "Italian";
	case LANGUAGE_FRENCH:		return "French";
	case LANGUAGE_PORTUGUESE:	return "Portuguese";
	case LANGUAGE_ROMANIAN:		return "Romanian";
	case LANGUAGE_ARABIC:		return "Arabic";
	case LANGUAGE_SWEDISH:		return "Swedish";
	case LANGUAGE_NORWIEGAN:	return "Norwiegan";
	case LANGUAGE_KOREAN:		return "Korean";
	case LANGUAGE_CHINESE:		return "Chinese";
	case LANGUAGE_JAPANESE:		return "Japanese";
	case LANGUAGE_RUSSIAN:		return "Russian";
	case LANGUAGE_OTHER:		return "Other";
	default:
		break;
	}

	//should never get here
	return "Unknown";
}

/////////////////////////////////////////////////////////////////////////////
// return a compiled string of all the known languages of someone
/////////////////////////////////////////////////////////////////////////////
CString CGLView::getLangString(int iKnown)
{
	int i;
	CString str;

	for(i = 0; i < LANGUAGES_SIZE; i++)
	{
		if(iKnown & (1 << i))
		{
			str += getLangName(1 << i);
			str += ", ";
		}
	}

	//remove the last comma
	str = str.Left(str.GetLength() - 2);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// move onto the next player's turn
/////////////////////////////////////////////////////////////////////////////
void CGLView::setNextPlayer()
{
	int nCurPlayer;

	//check to see if we need to start the special build phase
	if((IS_VARIANT(VARIANT_SPECIAL_BUILD)) && (FALSE == m_bSpecialBuild) &&
		(m_nSpecialStart != m_iPlayer))
	{
		//turn on the build phase
		m_bSpecialBuild = TRUE;

		//set the starting player that we wrap to
		m_nSpecialStart = m_iPlayer;
	}

	//set current player
	nCurPlayer = m_iPlayer;

	//increment
	m_iPlayer++;

	//check for end
	if(m_iPlayer == m_iNumPlayers) m_iPlayer = 0;

	//if we're in the special build phase, keep moving through the build phase
	//until we're done
	if(TRUE == m_bSpecialBuild)
	{
		if(m_iPlayer == m_nSpecialStart)
		{
			//we're done, so we want to move to the next player's turn
			m_bSpecialBuild = FALSE;

			//increment
			m_iPlayer++;

			//check for end
			if(m_iPlayer == m_iNumPlayers) m_iPlayer = 0;

			//add the undo icon
			addUndo(UNDO_SPECIAL_BUILD_END, NULL);

			//reset the start player
			m_nSpecialStart = -1;

			//do post build phase stuff
			THISVIEW->getControlPanel()->postBuildPhase();
		}
		else
		{
			//add the undo icon
			addUndo(UNDO_SPECIAL_BUILD, new CUndoOne(m_iPlayer));

			//run the special build phase
			THISVIEW->getControlPanel()->specialBuildPhase();
			return;
		}
	}

	//tell the panel
	THISVIEW->getControlPanel()->updateText();

	//update player displays
	THISVIEW->updatePlayerDisplay();

	//update the count of this player
	THISVIEW->updateCounts(nCurPlayer);

	//update the count of this player
	THISVIEW->updateCounts(m_iPlayer);
}

/////////////////////////////////////////////////////////////////////////////
// move to the previous player's turn
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::setPreviousPlayer(BOOL bIgnoreSpecial)
{
	BOOL bUndo = FALSE;

	//check to see if we need to return to the special build phase
	if((IS_VARIANT(VARIANT_SPECIAL_BUILD)) && (FALSE == m_bSpecialBuild) &&
		(m_nSpecialStart != m_iPlayer - 1) && (FALSE == bIgnoreSpecial))
	{
		//turn on the build phase
		m_bSpecialBuild = TRUE;

		//set the starting player that we wrap to
		m_nSpecialStart = m_iPlayer - 1;

		//decrement the player to get past the end player
		m_iPlayer--;

		//check for end
		if(m_iPlayer == -1) m_iPlayer = m_iNumPlayers - 1;
	}
	else if(TRUE == bIgnoreSpecial)
	{
		//decrement the player to get past the end player
		m_iPlayer--;

		//check for end
		if(m_iPlayer == -1) m_iPlayer = m_iNumPlayers - 1;

		//reset special start
		m_nSpecialStart = -1;
	}

	//increment
	m_iPlayer--;

	//check for end
	if(m_iPlayer == -1) m_iPlayer = m_iNumPlayers - 1;

	//if we're in the special build phase, keep moving through the build phase
	//until we're done
	if(TRUE == m_bSpecialBuild)
	{
		if(m_iPlayer == m_nSpecialStart)
		{
			//we're done, so we want to move to the next player's turn
			m_bSpecialBuild = FALSE;

			//increment
			m_iPlayer--;

			//check for end
			if(m_iPlayer == -1) m_iPlayer = m_iNumPlayers - 1;

			//undo the end of the turn as well
			bUndo = TRUE;
		}
		else
		{
			//hide current windows
			THISVIEW->showBuildOptions(FALSE);

			//run the special build phase
			THISVIEW->getControlPanel()->specialBuildPhase();
			
			//skip any further stuff
			return FALSE;
		}
	}

	//tell the panel
	THISVIEW->getControlPanel()->updateText();

	//update player displays
	THISVIEW->updatePlayerDisplay();

	return bUndo;
}

/////////////////////////////////////////////////////////////////////////////
// switch to new modes
/////////////////////////////////////////////////////////////////////////////
void CGLView::setDrawingMode(int iMode)
{
	switch(iMode)
	{
	case GL_ROAD:		
		//set the new mode
		m_iMode = GL_PLACE_ROAD;
		break;
	case GL_CITY:
		//set the new mode
		m_iMode = GL_PLACE_CITY;
		break;
	case GL_SETTLEMENT:
		//set the new mode
		m_iMode = GL_PLACE_SETTLEMENT;
		break;
	case GL_SHIP:
		//set the new mode
		m_iMode = GL_PLACE_SHIP;
		break;
	default:
		m_iMode = iMode;
		Invalidate();
		return;
		break;
	}

	//turn on hit test
	setHitTest(TRUE);

	//turn on selections
	m_bUseSelection = TRUE;
	
	//reset the selection
	m_iSelection = -1;

	//hide build options
	THISVIEW->showBuildOptions(FALSE);

	//disable all options
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();
	
	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// set the initial placement roads or ships
/////////////////////////////////////////////////////////////////////////////
void CGLView::setInitialMode(int iMode)
{
	if(GL_SHIP == iMode)
	{
		prepareInitialShips();
	}
	else
	{
		prepareInitialRoads();
	}

	//disable the undo to prevent them from backing out... they can still
	//undo (in hotseat) once they've placed the ship or road
	THISVIEW->getMessageButton()->enableUndo(FALSE);

	//hid the options
	THISVIEW->showInitialOptions(FALSE);

	//continue road placement
	m_iMode = GL_PLACE_PLAYERS;

	//update the panel text
	THISVIEW->getControlPanel()->updateText();

	//turn on hit tests for roads
	setHitTest(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// prepare to move a ship
/////////////////////////////////////////////////////////////////////////////
void CGLView::setMoveShipMode()
{
	//turn on hit test
	setHitTest(TRUE);

	//turn on selections
	m_bUseSelection = TRUE;
	
	//reset the selection
	m_iSelection = -1;

	//set the ship choosing mode
	m_iMode = GL_CHOOSE_SHIP;

	//hide build options
	THISVIEW->showBuildOptions(FALSE);

	//disable actions
	CONTROLPANEL->EnableDC();
	CONTROLPANEL->EnableTrade();
	CONTROLPANEL->EnableEndTurn();

	//disable undo until done
	THISVIEW->getMessageButton()->enableUndo(FALSE);

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// determine which combinations are invalid road/ship placements
/////////////////////////////////////////////////////////////////////////////
void CGLView::invalidPlaces(BOOL bShip, int iTile1, int iCorner, BOOL &b12, BOOL &b13, BOOL &b23)
{
	int iMinus;
	int iTile2, iTile3;

	//set the trailing value
	iMinus = (iCorner + 5) % 6;

	//get the nearest tiles
	iTile2 = m_pBoard[iTile1].getSide(iCorner);
	iTile3 = m_pBoard[iTile1].getSide(iMinus);

	//check to see if neighboring tiles are land
	if(((bShip == VALID_LAND(m_pBoard[iTile1].getType())) &&
		(bShip == VALID_LAND(m_pBoard[iTile2].getType()))) ||
	   ((TRUE == m_pBoard[iTile1].getBorder()) &&
	    (TRUE == m_pBoard[iTile2].getBorder())))
	{
		b12 = FALSE;
	}
	else
	{
		b12 = TRUE;
	}

	if(((bShip == VALID_LAND(m_pBoard[iTile1].getType())) &&
		(bShip == VALID_LAND(m_pBoard[iTile3].getType()))) ||
	   ((TRUE == m_pBoard[iTile1].getBorder()) &&
	    (TRUE == m_pBoard[iTile3].getBorder())))
	{
		b13 = FALSE;
	}
	else
	{
		b13 = TRUE;
	}

	if(((bShip == VALID_LAND(m_pBoard[iTile2].getType())) &&
		(bShip == VALID_LAND(m_pBoard[iTile3].getType()))) ||
	   ((TRUE == m_pBoard[iTile2].getBorder()) &&
	    (TRUE == m_pBoard[iTile3].getBorder())))
	{
		b23 = FALSE;
	}
	else
	{
		b23 = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// see if a ship can be moved from a certain tile and side
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::moveShip(int iTile, int iSide)
{
	int iTile2;
	int iCity;
	int iPlus, iMinus;
	int iBranch1, iBranch2;
	int iTouch = 0;
	int nCompare;
	BOOL bCity;
	BOOL bClosedLoop = FALSE;

	//this algorithm works in the following manner: For a ship to be movable, it
	//can only be touching another ship or settlement on just one side of itself.
	//If it is touching a ship or settlement on both sides, it is part of a chain
	//and cannot be moved.  This will automatically lock down ships once they
	//become part of a chain connecting settlements and cities.

	//set the plus and minus side for the current tile
	iPlus = ((iSide + 1) % 6);
	iMinus = ((iSide + 5) % 6);

	//get the branched sides
	iBranch1 = ((iSide + 2) % 6);
	iBranch2 = ((iSide + 4) % 6);
	
	//get the connecting side
	iTile2 = m_pBoard[iTile].getSide(iSide);

	//get the plus side city
	iCity = m_pBoard[iTile].getCity(iPlus);

	//determine if this city belongs to this player
	bCity = (0 != iCity) && ((iCity % 10) == m_iPlayer);

	//set the comparison value
	nCompare = ENCODE_SEL(iTile, iPlus);

	//determine whether this city is part of a closed loop back to itself of ships,
	//in which case they should be allowed to move the ship
	if(TRUE == bCity)
	{
		if(TRUE == shipIterate(nCompare, iTile, iPlus, iTile2))
		{
			bClosedLoop = TRUE;
		}
		else if(TRUE == shipIterate(nCompare, iTile2, iBranch1, iTile))
		{
			bClosedLoop = TRUE;
		}
	}

	//check the plus side
	if(((m_pBoard[iTile].getShip(iPlus) == m_iPlayer) ||
		(m_pBoard[iTile2].getShip(iBranch1) == m_iPlayer) ||
		(TRUE == bCity)) && ((FALSE == bClosedLoop) &&
		(iTile != m_iPirate) && (iTile2 != m_iPirate)))
	{
		iTouch++;
	}

	//reset the closed loop variable
	bClosedLoop = FALSE;

	//get the minus side city
	iCity = m_pBoard[iTile].getCity(iSide);

	//determine if this city belongs to this player
	bCity = (0 != iCity) && ((iCity % 10) == m_iPlayer);

	//set the comparison value
	nCompare = ENCODE_SEL(iTile, iSide);

	//determine whether this city is part of a closed loop back to itself of ships,
	//in which case they should be allowed to move the ship
	if(TRUE == bCity)
	{
		if(TRUE == shipIterate(nCompare, iTile, iMinus, iTile2))
		{
			bClosedLoop = TRUE;
		}
		else if(TRUE == shipIterate(nCompare, iTile2, iBranch2, iTile))
		{
			bClosedLoop = TRUE;
		}
	}

	//check the minus side
	if(((m_pBoard[iTile].getShip(iMinus) == m_iPlayer) ||
		(m_pBoard[iTile2].getShip(iBranch2) == m_iPlayer) ||
		(TRUE == bCity)) && ((FALSE == bClosedLoop) &&
		(iTile != m_iPirate) && (iTile2 != m_iPirate)))
	{
		iTouch++;
	}

	return (iTouch == 1);
}

/////////////////////////////////////////////////////////////////////////////
// handle point incrementation
/////////////////////////////////////////////////////////////////////////////
void CGLView::incPoints(int iPlayer, int iPoints, BOOL bNoUpdate)
{
	int i;

	//add one to the players points
	m_pGame->m_players[iPlayer].m_iPoints += iPoints;

	//update the player counts
	if(FALSE == bNoUpdate)
	{
		for(i = 0; i < m_iNumPlayers; i++)
		{
			THISVIEW->updateCounts(i);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle point decrementation
/////////////////////////////////////////////////////////////////////////////
void CGLView::decPoints(int iPlayer, int iPoints, BOOL bNoUpdate)
{
	//subtract one from the players points
	m_pGame->m_players[iPlayer].m_iPoints -= iPoints;

	//update the players counts
	if(FALSE == bNoUpdate)
	{
		THISVIEW->updateCounts(iPlayer);
	}
}

/////////////////////////////////////////////////////////////////////////////
// longest road points
/////////////////////////////////////////////////////////////////////////////
void CGLView::incLongestRoad(int nPlayer)
{
	//set the flag
	m_pGame->m_players[nPlayer].m_bLongestRoad = TRUE;

	//increase the points
	incPoints(nPlayer, 2);
}

/////////////////////////////////////////////////////////////////////////////
// longest road points
/////////////////////////////////////////////////////////////////////////////
void CGLView::decLongestRoad(int nPlayer)
{
	//set the flag
	m_pGame->m_players[nPlayer].m_bLongestRoad = FALSE;

	//decrease the points
	decPoints(nPlayer, 2);
}

/////////////////////////////////////////////////////////////////////////////
// determine if more dev cards can be bought
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::checkForDevCards(void)
{
	//compare index
	if(m_iDevCardIndex < (int) m_iDevCardArray.size())
	{
		return TRUE;
	}
	
	//we're at the end
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// see if anyone might get wrecked by a volcano
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::checkForVolcano()
{
	int i, j;
	int nTile;

	//reset volcano array
	m_nLastVolcano.resize(0);
	m_nLastVolcano.clear();

	//search last rolled tiles
	for(i = 0; i < (int) m_iLastTiles.size(); i++)
	{
		//get the tile
		nTile = m_iLastTiles[i];

		//see if it's a volcano or not
		if(RES_VOLCANO != m_pBoard[nTile].getType())
		{
			continue;
		}

		//see if anyone has built on it
		for(j = 0; j < 6; j++)
		{
			if(0 < m_pBoard[nTile].getCity(j))
			{
				m_nLastVolcano.push_back(nTile);
				break;
			}
		}
	}

	//did not find any
	return (m_nLastVolcano.size() != 0);
}

/////////////////////////////////////////////////////////////////////////////
// handle volcanic eruption
/////////////////////////////////////////////////////////////////////////////
int CGLView::volcanicEruption(int nRoll, vector <int> &nDestruction)
{
	int i, j;
	int nC2, nC3;
	int nTile, nTile2, nTile3;
	int nCity, nType;
	int nPlayer, nExtra, nDestroy;
	int nRot;
	int nOwner = -1;
	double dRot;
	int nTemp, nLoss[6], nLosses;
	BOOL bLongest = FALSE;
	BOOL bLost = FALSE;
	CString str;
	CStringArray strArray;

	//resize the string array
	strArray.SetSize(m_iNumPlayers);

	//clear the loss array
	::ZeroMemory(nLoss, sizeof(nLoss));

	//set volcano roll
	m_nVolcanoRoll = nRoll;

	//play the sound
	SOUND(SOUND_VOLCANIC_ERUPTION);

	//search last rolled tiles
	for(i = 0; i < (int) m_nLastVolcano.size(); i++)
	{
		//get the tile
		nTile = m_nLastVolcano[i];

		//get tile rotation
		dRot = m_pBoard[nTile].getRotation();

		//calculate rotation
		nRot = int(dRot / 60.);

		//determine the actual corner
		nRoll = (nRoll + nRot) % 6;

		//see if anyone has built on it
		for(j = 0; j < 6; j++)
		{
			//get the city and any extra points
			nCity = m_pBoard[nTile].getCity(j);
			nExtra = m_pBoard[nTile].getExtra(j);

			//check for city
			if(0 == nCity)
			{
				continue;
			}

			//get the player and settlement/city type
			DECODE_SEL(nCity, nType, nPlayer);

			//having a city/settlement on a volcano counts as 1 point
			nLoss[nPlayer] += 1;

			//see if we can continue (no destruction)
			if(j != nRoll)
			{
				continue;
			}

			//get alternate sides
			nTile2 = m_pBoard[nTile].getSide(j);
			nTile3 = m_pBoard[nTile].getSide((j + 5) % 6);

			//set opposite corners
			nC2 = (j + 4) % 6;
			nC3 = (j + 2) % 6;

			//destruction!!

			//check for city
			if(2 == nType)
			{
				//see if the player has any settlements to replace it with
				if(0 < m_pGame->m_players[nPlayer].m_nStockSettle)
				{
					//the city is reduced to a settlement (no loss of extra points)
					handleCommonReduceCity(nPlayer, nTile, j);
					
					//city reductions count as 1000 points
					nLoss[nPlayer] += 1000;

					//create the desctruction code
					nDestroy = ENCODE_VOLCANO(nPlayer, nTile, j, 2, 0);

					//add it
					nDestruction.push_back(nDestroy);
				}
				else
				{
					//the city is destroyed because the player has no settlements
					//to replace it with (and loses extra points below it)
					handleCommonRemoveCity(nPlayer, nTile, j);
					
					//city destruction counts as 100 points
					nLoss[nPlayer] += 100;

					//longest road must now be checked
					bLongest = TRUE;

					//create the desctruction code
					nDestroy = ENCODE_VOLCANO(nPlayer, nTile, j, 2, nExtra);

					//add it
					nDestruction.push_back(nDestroy);
				}
			}
			//settlement destruction
			else
			{
				//the settlement is destroyed (and the player loses any extra points)
				handleCommonRemoveSettlement(nPlayer, nTile, j);

				//settlement destruction counts as 10 points
				nLoss[nPlayer] += 10;

				//longest road must now be checked
				bLongest = TRUE;

				//create the desctruction code
				nDestroy = ENCODE_VOLCANO(nPlayer, nTile, j, 1, nExtra);

				//add it
				nDestruction.push_back(nDestroy);
			}
		}
	}

	//now create the loss strings
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//check for loss
		if(0 == nLoss[i])
		{
			continue;
		}

		//reset losses
		nLosses = 0;

		//start things off
		strArray[i].Format("%s ", m_pGame->m_players[i].m_player.getFirstName());

		//get city reduction
		nTemp = nLoss[i] / 1000;
		nLoss[i] -= (nTemp * 1000);

		//set it
		if(0 < nTemp)
		{
			str.Format("had %d %s reduced, ", nTemp, (nTemp == 1) ? "city" : "cities");
			strArray[i] += str;
			nLosses += nTemp;
		}

		//get city destruction
		nTemp = nLoss[i] / 100;
		nLoss[i] -= (nTemp * 100);

		//set it
		if(0 < nTemp)
		{
			str.Format("had %d %s destroyed, ", nTemp, (nTemp == 1) ? "city" : "cities");
			strArray[i] += str;
			nLosses += nTemp;
		}

		//get settlement destruction
		nTemp = nLoss[i] / 10;

		//set it
		if(0 < nTemp)
		{
			str.Format("had %d settlement%s destroyed, ", nTemp, (nTemp == 1) ? "" : "s");
			strArray[i] += str;
			nLosses += nTemp;
		}

		//clean up
		if(0 == nLosses)
		{
			strArray[i] += "escaped unscathed!";
		}
		else
		{
			strArray[i] = strArray[i].Left(strArray[i].GetLength() - 2);
			strArray[i] += ".";

			//play the sound
			SOUND(SOUND_VOLCANIC_DESTRUCT);
		}
	}

	//now run through and update the counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->getControlPanel()->updateVolcano(!strArray[i].IsEmpty(), i, strArray[i]);
	}

	//check to see if we need to recalc longest road
	if(TRUE == bLongest)
	{
		//get current longest owner
		nOwner = longestPlayer();

		//see if someone now has a longer road--has to be the case where the 
		//city/settlement destroyed was blocking someone else's longer road
		if(-1 != nOwner)
		{
			//get everyone's road lengths
			calculateRoadLengths();

			//traverse each player, comparing their road length to this players
			for(i = 0; i < m_iNumPlayers; i++)
			{
				//skip self
				if(i == nOwner)
				{
					continue;
				}

				//if any player's road length is greater than this player, this player
				//lost longest road by having someone else's city/settlement destroyed
				if(m_pGame->m_players[i].m_iRoadLength > m_pGame->m_players[nOwner].m_iRoadLength)
				{
					//they've lost it
					bLost = TRUE;
				}
			}

			//if no one else beat them, see if they lost it because they went below 5,
			//though not sure how this could happen with a volcano
			if((FALSE == bLost) && (m_pGame->m_players[nOwner].m_iRoadLength < 5))
			{
				bLost = TRUE;
			}
			
			//handle the loss
			if(TRUE == bLost)
			{
				//dec their points
				decLongestRoad(nOwner);

				//find whoever has the longest road
				handleCommonLongestFindPlayer();

				//recalc road lengths
				calculateRoadLengths();

				//the "previous" owner is nOwner, so keep it as is
			}
			//they still have it so return that there is no "previous" owner
			else
			{
				nOwner = -1;
			}
		}
		//if no current owner, see if someone got it by having this settlement/city
		//destroyed--has to be the case where the city/settlement was cutting their
		//roads off and no one else had over 5 roads together
		else
		{
			//find the longest road player
			handleCommonLongestFindPlayer();

			//the previous owner stays at -1 since no one had it before this
		}
	}

	//set volcano mode
	m_iMode = GL_VOLCANO;

	//reset selection
	m_bUseSelection = FALSE;
	m_iSelection = -1;

	//redraw
	Invalidate();

	//return any previous owner of longest road
	return nOwner;
}

/////////////////////////////////////////////////////////////////////////////
// see if the game has been won
/////////////////////////////////////////////////////////////////////////////
void CGLView::checkForWin()
{
	int i;
	int iPoints = 0;
	COleDateTime time;
	COleDateTimeSpan timespan;

	//get the points we're playing to
	if(IS_VARIANT(VARIANT_ALTER_POINTS))
	{
		iPoints = VARIANTS->m_iPoints;
	}
	else
	{
		iPoints = MAP->m_iPoints;
	}

	//run through the player list
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//check for win
		if(m_pGame->m_players[i].m_iPoints >= iPoints)
		{
			//if in network mode, tell others
			if(GAME_MODE_NETWORK == m_pGame->m_iMode)
			{
				THISVIEW->clientWin(i);
			}
			else
			{
				//disable the undo button
				THISVIEW->getMessageButton()->enableUndo(FALSE);

				//get the current time
				time = COleDateTime::GetCurrentTime();

				//set the ending time of the game
				m_pGame->m_timeEnd = time;
		
				//calculate the difference in time for this turn
				timespan = time - m_pGame->m_timeRef;

				//set the new reference time
				m_pGame->m_timeRef = time;

				//update time counter
				handleCommonTimeUpdate(m_iPlayer, (int) timespan.GetTotalSeconds());
			}

			//we won!
			handleCommonWin(i);

			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle playing a soldier
/////////////////////////////////////////////////////////////////////////////
void CGLView::playSoldier()
{
	//handle common soldier
	handleCommonPlaySoldier(m_iPlayer);

	//set to the next mode
	m_iMode = GL_PLACE_ROBBER;

	//set hit tests
	setHitTest(TRUE);

	// a soldier card is being played
	m_bSoldier = TRUE;

	//set the interaction mode to select
	m_bUseSelection = TRUE;

	//reset the selection
	m_iSelection = -1;

	//if it's network, tell others
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientPlayDevCard(CARD_SOLDIER);
	}

	//update the player panel
	THISVIEW->getControlPanel()->updateText();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//check for the win
	checkForWin();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// look for largest army
/////////////////////////////////////////////////////////////////////////////
int CGLView::largestArmy()
{
	int i;
	int iPlayer = -1;

	//see if this player has played three soldier cards
	if(m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_SOLDIER] < 3)
	{
		//does not have largest army
		return -3;
	}

	//if they have three or more, check to see if anyone else has largest army
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//check player
		if(m_pGame->m_players[i].m_bLargestArmy) 
		{
			iPlayer = i;
			break;
		}
	}

	//if the same person already has largest army return 
	if(iPlayer == m_iPlayer)
	{
		//already has largest army
		return -2;
	}

	//if no one has largest army, this person gets it
	if(iPlayer == -1)
	{
		//set the flag
		m_pGame->m_players[m_iPlayer].m_bLargestArmy = TRUE;

		//increment the number of points
		incPoints(m_iPlayer, 2);

		//play sound
		SOUND(SOUND_LARGEST_ARMY);

		//first one to largest army
		return -1;
	}

	//otherwise, compare to see if they've stolen it from this person
	if(m_pGame->m_players[m_iPlayer].m_iCardPlayed[CARD_SOLDIER] >
		m_pGame->m_players[iPlayer].m_iCardPlayed[CARD_SOLDIER])
	{
		//they've stolen it
		m_pGame->m_players[iPlayer].m_bLargestArmy = FALSE;

		//now they have it
		m_pGame->m_players[m_iPlayer].m_bLargestArmy = TRUE;

		//decrement points
		decPoints(iPlayer, 2);

		//increment points
		incPoints(m_iPlayer, 2);

		//play sound
		SOUND(SOUND_LARGEST_ARMY);

		//return the person they stole it from
		return iPlayer;
	}

	//if they make it here, it means they've tied someone for largest army
	//but haven't stolen it yet (do not have largest army)
	return -3;
}

/////////////////////////////////////////////////////////////////////////////
// return the current player with longest road
/////////////////////////////////////////////////////////////////////////////
int CGLView::longestPlayer()
{
	int i;
	int nOwner = -1;

	//scan players
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//set the owner
		if(TRUE == m_pGame->m_players[i].m_bLongestRoad)
		{
			nOwner = i;
			break;
		}
	}

	return nOwner;
}

/////////////////////////////////////////////////////////////////////////////
// reset road lengths to correct values
/////////////////////////////////////////////////////////////////////////////
void CGLView::calculateRoadLengths()
{
	int i;
	int nPlayer;

	//get the longest road player
	nPlayer = longestPlayer();

	//reset road lengths
	for(i = 0; i < m_iNumPlayers; i++)
	{
		//reset all road lengths
		m_pGame->m_players[i].m_iRoadLength = 0;
	}

	//now get road lengths
	if(-1 != nPlayer)
	{
		longestRoad(nPlayer);
	}

	//get everyone else
	for(i = 0; i < m_iNumPlayers; i++)
	{
		if(nPlayer == i) continue;

		longestRoad(i);
	}

	//update all counts
	for(i = 0; i < m_iNumPlayers; i++)
	{
		THISVIEW->updateCounts(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// does a certain player have this port?
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::havePort(int iPlayer, int iPort)
{
	int i, j;
	int iType, iTile;
	int iSize;

	//get list of ports size
	iSize = m_iPorts.GetSize();

	//run through the port list
	for(i = 0; i < iSize; i++)
	{
		//break down the port type
		iType = m_iPorts[i] >> 16;
		iTile = m_iPorts[i] - (iType << 16);
		
		bool thisTurn = false;

		//see if they built it on this turn
		for(j = 0; j < m_nPortsThisTurn.GetSize(); j++)
		{
			if(iTile == m_nPortsThisTurn[j])
			{
				thisTurn = true;
			}
		}

		if(true == thisTurn)
		{
			continue;
		}

		//see if it's what they want
		if(iType == iPort && TRUE == checkForCity(iPlayer, iTile))
		{
			return TRUE;
		}
	}
	
	//no port
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// see if a player has a city on this tile for a port
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::checkForCity(int iPlayer, int iTile)
{
	int i;
	int iPort;
	int iCity;

	for(i = 0; i < 2; i++)
	{
		if(i == 0)
		{
			//try the first port number
			iPort = m_pBoard[iTile].getPort1();
		}
		else
		{
			//check with the second port possibility
			iPort = m_pBoard[iTile].getPort2();
		}

		//check for a city
		if(-1 != iPort)
		{
			iCity = m_pBoard[iTile].getCity(iPort);

			if(iCity > 0 && iCity % 10 == iPlayer)
			{
				//found one
				return TRUE;
			}
		}
	}

	//no cities here
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// trade resources to the bank to get the desired goods
// returns -1 if not possible to trade, otherwise, the number of missing
// goods
/////////////////////////////////////////////////////////////////////////////
int CGLView::tradeToBank(int iType, BOOL bCheck)
{
	int i, j;
	int iSel;
	int iRet;
	int iRes[5];
	int iBank[3];
	int iNeeded[5];
	int iTradeable[5];
	int iTradedTo[5];
	int iTradedFor[5];
	int iMissing = 0;
	int iTypesTrade = 0;
	int iNumTradeable = 0;
	int nJungles = 0;
	int nJungleTrades = 0;
	BOOL bThree;
	BOOL bOnce;

	//copy what the player has
	memcpy(iRes, m_pGame->m_players[m_iPlayer].m_iRes, sizeof(iRes));

	//zero out the needed goods array and tradable array
	::ZeroMemory(iBank, sizeof(iBank));
	::ZeroMemory(iNeeded, sizeof(iNeeded));
	::ZeroMemory(iTradeable, sizeof(iTradeable));
	::ZeroMemory(iTradedTo, sizeof(iTradedTo));
	::ZeroMemory(iTradedFor, sizeof(iTradedFor));

	//set up what the player needs
	switch(iType)
	{
	case GL_ROAD: iNeeded[RES_TIMBER] = 1; iNeeded[RES_CLAY] = 1; break;
	case GL_SHIP: iNeeded[RES_TIMBER] = 1; iNeeded[RES_SHEEP] = 1; break;
	case GL_SETTLEMENT: iNeeded[RES_TIMBER] = 1; iNeeded[RES_CLAY] = 1; iNeeded[RES_WHEAT] = 1;
						iNeeded[RES_SHEEP] = 1; break;
	case GL_CITY: iNeeded[RES_ORE] = 3; iNeeded[RES_WHEAT] = 2; break;
	case GL_DEVCARD: iNeeded[RES_ORE] = 1; iNeeded[RES_WHEAT] = 1; iNeeded[RES_SHEEP] = 1; 
					 nJungles = m_pGame->m_players[m_iPlayer].m_nJungles; break;
	default: return -1;
	}

	//check to see if they've played the road-building card
	if((iType == GL_ROAD || iType == GL_SHIP) && m_iFreeRoads)
	{
		//no goods needed to trade in since they're free
		return 0;
	}

	//see if this player has a 3 port
	bThree = havePort(m_iPlayer, RES_PORT3);

	//now run through and see what the player has and what they're missing
	for(i = 0; i < 5; i++)
	{
		//set the once flag
		bOnce = TRUE;

		//skip resources that aren't needed
		if(0 < iNeeded[i])
		{
			//subtract what the user has
			if(iRes[i] >= iNeeded[i])
			{
				iRes[i] -= iNeeded[i];
				iNeeded[i] = 0;
			}
			else
			{
				iNeeded[i] -= iRes[i];
				iRes[i] = 0;

				//add to the number of cards missing list
				iMissing += iNeeded[i];
			}
		}

		//see if the user has tradable resources here
		while(iRes[i] > 0)
		{
			//see if they can trade this resource port
			if(havePort(m_iPlayer, RES_PORTTIMBER + i)) 
			{
				iRes[i] -= 2;
			}
			//see if they have a three port
			else if(bThree)
			{
				iRes[i] -= 3;
			}
			//try for a four trade in
			else
			{
				iRes[i] -= 4;
			}

			//see if they still have some
			if(iRes[i] >= 0) 
			{
				iTradeable[i]++;

				//keep track of the total
				iNumTradeable++;

				if(TRUE == bOnce)
				{
					//increment the number of different resources tradeable
					iTypesTrade++;

					//turn it off
					bOnce = FALSE;
				}
			}
		}
	}

	//see if we have enough jungles to cover it
	if(nJungles >= (iMissing - iNumTradeable))
	{
		//this figures out how many cards we don't have covered by
		//jungles and what we have in hand
		nJungleTrades = (3 - nJungles) - (3 - iMissing);

		//sanity
		if(0 > nJungleTrades)
		{
			nJungleTrades = 0;
		}

		//see if we can just return
		if(TRUE == bCheck)
		{
			return nJungleTrades;
		}
	}

	//now check to make sure the bank has all the cards we still need
	for(i = 0; i < 5; i++)
	{
		//check the bank to make sure they can even get this resource!
		if(0 < iNeeded[i] && 0 >= m_pGame->m_iBankRes[i])
		{
			return -1;
		}
	}

	//now see if we even need to trade
	if((0 == iMissing) && ((TRUE == bCheck) || ((FALSE == bCheck) && (0 == nJungles)))) 
	{
		//there are none missing, so we're golden
		return 0;
	}

	//sanity check
	if(iNumTradeable + nJungles < iMissing)
	{
		//can't get what we want
		return -1;
	}

	//if they get here and check is on, they can do it
	if(TRUE == bCheck) 
	{
		//return the number of missing that we have
		return iMissing;
	}

	//if we're using jungles, then we only need to ask about what's not 
	//covered by what we have and what jungles we have
	if(0 < nJungles)
	{
		//handle the jungle issue
		tradeToJungle(nJungles, iNeeded);

		//set what we need to trade to the bank for and handle the rest normally
		iMissing = nJungleTrades;

		//see if we need to continue
		if(0 == iMissing)
		{
			return 0;
		}
	}

	//see if we're going to have to ask what to trade for
	if((iTypesTrade > iMissing) || ((1 != iTypesTrade) && (iNumTradeable > iMissing) && (1 != iMissing)))
	{
		while(((iTypesTrade > iMissing) || ((1 != iTypesTrade) && (iNumTradeable > iMissing) && (1 != iMissing))) && iMissing)
		{
			CTradeBankDialog dlg;

			//set what's avaialable
			dlg.setAvail(iTradeable);

			//get the requested resource type
			for(j = 0; j < 5; j++)
			{
				if(iNeeded[j]) break;
			}

			//set the resource name
			dlg.setRes(getResName(j, TRUE));

			//make app aware
			THISVIEW->setDialog(&dlg, TRUE);

			//run it
			iRet = dlg.DoModal();

			//make app aware
			THISVIEW->setDialog(NULL);

			//check for abort
			if((IDABORT == iRet) || (THISVIEW->isWaiting()))
			{
				return -1;
			}

			//get the type they selected
			iSel = dlg.getType();

			//now, exchange it
			if(havePort(m_iPlayer, iSel + RES_PORTTIMBER)) 
			{
				iBank[TRADE_2FOR1]++;
				m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_2FOR1]++;
				iTradedTo[iSel] += 2;
			}
			//see if they have a three port
			else if(bThree)
			{
				iBank[TRADE_3FOR1]++;
				m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_3FOR1]++;
				iTradedTo[iSel] += 3;
			}
			//try for a four trade in
			else 
			{
				iBank[TRADE_4FOR1]++;
				m_pGame->m_players[m_iPlayer].m_iTradesBank[TRADE_4FOR1]++;
				iTradedTo[iSel] += 4;
			}

			//subtract from the needed
			iTradeable[iSel]--;
			iNeeded[j]--;
			iTradedFor[j]++;
			iMissing--;
			iNumTradeable--;

			//see if we can lose a traded type
			if(0 == iTradeable[iSel])
			{
				iTypesTrade--;
			}
		}
	}

	//otherwise, start trading in
	for(i = 0; i < 5; i++)
	{
		//skip the resource if we don't need any
		if(0 == iNeeded[i]) 
		{
			continue;
		}

		//do it while this resource is still needed
		while(0 < iNeeded[i])
		{
			//increment the traded for array
			iTradedFor[i]++;

			//travel through the tradeable resources
			for(j = 0; j < 5; j++)
			{
				//skip resources we can't trade
				if(0 == iTradeable[j]) 
				{
					continue;
				}

				//trade in this resource
				if(havePort(m_iPlayer, j + RES_PORTTIMBER))
				{
					iBank[TRADE_2FOR1]++;
					iTradedTo[j] += 2;
				}
				//see if they have a three port
				else if(bThree) 
				{
					iBank[TRADE_3FOR1]++;
					iTradedTo[j] += 3;
				}
				//try for a four trade in
				else 
				{
					iBank[TRADE_4FOR1]++;
					iTradedTo[j] += 4;
				}

				//remove one from the tradeable resources
				iTradeable[j]--;
				break;
			}

			//decrement
			iNeeded[i]--;
		}
	}

	//now do stuff
	handleCommonTradeToBank(m_iPlayer, iTradedTo, iTradedFor, iBank, (iType != GL_DEVCARD));

	//if we're in network mode, everyone else needs to know what happened
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientTradeToBank(iTradedTo, iTradedFor, iBank);
	}

	//handled successfully
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// handle turning in jungles for needed resources
/////////////////////////////////////////////////////////////////////////////
void CGLView::tradeToJungle(int nJungles, int *nNeeded)
{
	int i, j;
	int nJungleFor[5];
	int nJunglesUsed;
	int nTotalNeed = 0;
	BOOL bSpecial;
	CTradeJungleDialog dlg;

	//reset memory
	::ZeroMemory(nJungleFor, sizeof(nJungleFor));

	//calculate the total need
	for(i = 0; i < 5; i++)
	{
		nTotalNeed += nNeeded[i];
	}

	//calculate the jungles we'll use
	nJunglesUsed = min(nJungles, 3);

	//determine if we have the special case
	bSpecial = ((2 == nJunglesUsed) && (1 == nTotalNeed));

	//see if we have the special case
	if(3 == nJunglesUsed)
	{
		//in this case, they just get one of every card needed
		nJungleFor[RES_WHEAT] = 1;
		nJungleFor[RES_ORE] = 1;
		nJungleFor[RES_SHEEP] = 1;
	}
	//if we have more jungles than we have need, they're going to have to choose
	//what they want to keep
	else if(nJunglesUsed > nTotalNeed)
	{
		//set up the dialog
		dlg.initChoices(m_iPlayer, nJunglesUsed, nTotalNeed, nJungleFor);

		//run it
		dlg.DoModal();

		//if we're in the special case, we still have to fill in the one missing card
		if(TRUE == bSpecial)
		{
			for(i = 0; i < 5; i++)
			{
				if(0 != nNeeded[i])
				{
					nJungleFor[i] = nNeeded[i];
					nNeeded[i] = 0;
					break;
				}
			}
		}
	}
	//if the need and the jungles are equal, it's just a simple matter of 
	//giving them what they need
	else if(nJunglesUsed == nTotalNeed)
	{
		//what we trade for is exactly what's needed
		memcpy(nJungleFor, nNeeded, sizeof(nJungleFor));

		//we don't need anything anymore
		::ZeroMemory(nNeeded, sizeof(nNeeded));
	}
	//if we have fewer jungles than need, they're going to have to do some other
	//trading to get what they want... in the mean time, we'll just fill in
	//what we can
	else if(nJunglesUsed < nTotalNeed)
	{
		for(i = 0, j = 0; (i < 5) && (j < nJunglesUsed); i++)
		{
			//skip unneeded resources
			if(0 == nNeeded[i])
			{
				continue;
			}

			//otherwise, fill the need
			nJungleFor[i] = nNeeded[i];
			nNeeded[i] = 0;
			j++;
		}
	}

	//tell others if we need to
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientTradeJungle(nJunglesUsed, nJungleFor);
	}

	//execute the trade
	handleCommonTradeToJungle(m_iPlayer, nJunglesUsed, nJungleFor);
}

/////////////////////////////////////////////////////////////////////////////
// set up the steal dialog
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::initSteal(CMultiUseDialog *pDlg)
{
	int i;
	int iPlayer;
	int iPlayers[6];
	int iSteal = 0;

	//reset the player array
	::ZeroMemory(iPlayers, sizeof(iPlayers));

	//see which players can be stolen from
	for(i = 0; i < 6; i++)
	{
		//see if there's a city here
		if(m_pBoard[m_iRobber].getCity(i) < 10) continue;

		//get the player
		iPlayer = m_pBoard[m_iRobber].getCity(i) % 10;

		//skip current player (why they would do this, who knows?)
		if(iPlayer == m_iPlayer) continue;

		//see if this player can be stolen from
		if(!m_pGame->m_players[iPlayer].canTrade()) continue;

		//increment this player
		iPlayers[iPlayer]++;

		//see if it's new
		if(iPlayers[iPlayer] == 1)
		{
			iSteal++;
		}
	}

	if(0 < iSteal)
	{
		//set the players
		pDlg->setPlayers(iPlayers);

		//set the mode
		pDlg->setMode(MULTIUSE_STEAL);
	}

	//return the possibility of stealing
	return (iSteal);
}

/////////////////////////////////////////////////////////////////////////////
// set up the steal dialog
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::initPirateSteal(CMultiUseDialog *pDlg)
{
	int i;
	int iPlayer;
	int iPlayers[6];
	int iSteal = 0;

	//reset the player array
	::ZeroMemory(iPlayers, sizeof(iPlayers));

	//see which players can be stolen from
	for(i = 0; i < 6; i++)
	{
		//see if there's a city here
		if(-1 == m_pBoard[m_iPirate].getShip(i)) 
		{
			continue;
		}

		//get the player
		iPlayer = m_pBoard[m_iPirate].getShip(i);

		//skip current player (why they would do this, who knows?)
		if(iPlayer == m_iPlayer) continue;

		//see if this player can be stolen from
		if(FALSE == m_pGame->m_players[iPlayer].canTrade()) 
		{
			continue;
		}

		//increment this player
		iPlayers[iPlayer]++;

		//see if it's new
		if(iPlayers[iPlayer] == 1)
		{
			iSteal++;
		}
	}

	if(0 < iSteal)
	{
		//set the players
		pDlg->setPlayers(iPlayers);

		//set the mode
		pDlg->setMode(MULTIUSE_STEAL);
	}

	//return the possibility of stealing
	return (iSteal);
}

/////////////////////////////////////////////////////////////////////////////
// initialize chitless tile selection mode
/////////////////////////////////////////////////////////////////////////////
void CGLView::initChitless()
{
	int nPass = 0;
	int nTiles = 0;

	//set the bit
	m_nGLDraw = GL_BIT_TILE_SELECT;

	//reset the selectable tile array
	resetTileSelect();

	//filter by the third rule
	filterRule3();

	//filter by the second rule
Rule2:
	filterRule2();

	//filter by the first rule
Rule1:
	nTiles = filterRule1();

	//if we got no tiles, go start over and go back to the next most
	//important rule
	if(0 == nTiles)
	{
		//set the bit
		m_nGLDraw = GL_BIT_TILE_SELECT;

		//reset the selection
		resetTileSelect();

		//up the pass
		nPass++;

		//goto
		switch(nPass)
		{
		case 1: goto Rule2; break;
		case 2: goto Rule1; break;
		case 3:
			//in this case, all selectable tiles are valid
			filterRule0();
			break;
		}
	}

	//set the drawing mode
	m_iMode = GL_CHITLESS_TILE;

	//set the interaction mode to select
	m_bUseSelection = TRUE;

	//reset the selection
	m_iSelection = -1;

	//set hit testing
	setHitTest(TRUE);

	//update the control panel text
	THISVIEW->getControlPanel()->updateText();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// chitless filter rule 3
/////////////////////////////////////////////////////////////////////////////
void CGLView::filterRule3()
{
	int i, j;
	int nCity;
	vector <int> nValid;

	//Rule 3: You may only remove a token from a hex that is adjacent to
	//at least one of your cities.

	//search the array of tiles
	for(i = 0; i < m_nTiles; i++)
	{
		//if it's an ocean tile or not an initial placement tile, it cannot
		//be selected... it must also have a valid chit
		if( (TRUE == VALID_OCEAN(m_pBoard[i].getType())) ||
			(FALSE == m_pBoard[i].getInitial()) || 
			(TRUE == m_pBoard[i].getChitless()) ||
			(FALSE == VALID_RESOURCE(m_pBoard[i].getType())))
		{
			continue;
		}

		//now search the tile to see if this player has a settlement or city
		//on it
		for(j = 0; j < 6; j++)
		{
			//get the city ID
			nCity = m_pBoard[i].getCity(j);

			//see if we own the city
			if((0 != nCity) && ((nCity % 10) == m_iPlayer))
			{
				nValid.push_back(i);
			}
		}
	}

	//now reset the array and set the new values
	m_nGLDraw = 0;
	resetTileSelect();

	//set the selections
	for(i = 0; i < (int) nValid.size(); i++)
	{
		m_bTileSelect[nValid[i]] = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// chitless filter rule 2
/////////////////////////////////////////////////////////////////////////////
void CGLView::filterRule2()
{
	int i, j;
	int nCity;
	int nTile2, nTile3;
	BOOL bValid;
	vector <int> nSelectable;
	vector <int> nValid;

	//Rule 2: All settlements and cities on the Mainland must be adjacent to
	//at least one hex with a number token.  You cannot remove a token if it
	//would cause any settlement or city to have no resource production.

	//first, compile the list of currently selectable tiles
	for(i = 0; i < m_nTiles; i++)
	{
		if(TRUE == m_bTileSelect[i])
		{
			nSelectable.push_back(i);
		}
	}

	//now go through the list of selectable tiles and filter out those that
	//do not meet the criteria
	for(i = 0; i < (int) nSelectable.size(); i++)
	{
		//if it's an ocean tile or not an initial placement tile, it cannot
		//be selected
		if( (TRUE == VALID_OCEAN(m_pBoard[nSelectable[i]].getType())) ||
			(FALSE == m_pBoard[nSelectable[i]].getInitial()) || 
			(TRUE == m_pBoard[nSelectable[i]].getChitless()) ||
			(FALSE == VALID_RESOURCE(m_pBoard[nSelectable[i]].getType())))
		{
			continue;
		}

		//scan each corner for a city
		for(j = 0, bValid = TRUE; j < 6; j++)
		{
			nCity = m_pBoard[nSelectable[i]].getCity(j);

			//if it's equal to zero (no city or settlement), continue
			if(0 == nCity)
			{
				continue;
			}

			//otherwise, look at the two adjoining tiles... if they are both
			//chitless, this tile is invalid
			nTile2 = m_pBoard[nSelectable[i]].getSide(j);
			nTile3 = m_pBoard[nSelectable[i]].getSide((j + 5) % 6);

			//check
			if( (TRUE == m_pBoard[nTile2].getChitless()) &&
				(TRUE == m_pBoard[nTile3].getChitless()))
			{
				bValid = FALSE;
				break;
			}
		}

		//see if we can add it
		if(TRUE == bValid)
		{
			nValid.push_back(nSelectable[i]);
		}
	}

	//now reset the array and set the new values
	m_nGLDraw = 0;
	resetTileSelect();

	//set the selections
	for(i = 0; i < (int) nValid.size(); i++)
	{
		m_bTileSelect[nValid[i]] = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// chitless filter rule 1
/////////////////////////////////////////////////////////////////////////////
int CGLView::filterRule1()
{
	int i;
	int nTile2;
	BOOL bIslandRed = FALSE;
	BOOL bTileRed;
	vector <int> nSelectable;
	vector <int> nValid;

	//Rule 3: The red tokens (numbered 6 & 8) ma not be placed next to one
	//another on the small islands.

	//determine whether the island is next to red tile or not
	for(i = 0; i < 6; i++)
	{
		//get the tile
		nTile2 = m_pBoard[m_nChitlessTile].getSide(i);

		//check it
		if( (6 == m_pBoard[nTile2].getRoll()) ||
			(8 == m_pBoard[nTile2].getRoll()))
		{
			bIslandRed = TRUE;
			break;
		}
	}

	//first, compile the list of currently selectable tiles
	for(i = 0; i < m_nTiles; i++)
	{
		if(TRUE == m_bTileSelect[i])
		{
			nSelectable.push_back(i);
		}
	}

	//now go through the list of selectable tiles and filter out those that
	//do not meet the criteria
	for(i = 0; i < (int) nSelectable.size(); i++)
	{
		//if it's an ocean tile or not an initial placement tile, it cannot
		//be selected
		if( (TRUE == VALID_OCEAN(m_pBoard[nSelectable[i]].getType())) ||
			(FALSE == m_pBoard[nSelectable[i]].getInitial()) || 
			(TRUE == m_pBoard[nSelectable[i]].getChitless()) ||
			(FALSE == VALID_RESOURCE(m_pBoard[nSelectable[i]].getType())))
		{
			continue;
		}

		//determine if the tile is a red chit
		if( (6 == m_pBoard[nSelectable[i]].getRoll()) ||
			(8 == m_pBoard[nSelectable[i]].getRoll()))
		{
			bTileRed = TRUE;
		}
		else
		{
			bTileRed = FALSE;
		}

		//see if it can be added
		if((FALSE == bIslandRed) || (FALSE == bTileRed))
		{
			nValid.push_back(nSelectable[i]);
		}
	}

	//now reset the array and set the new values
	m_nGLDraw = 0;
	resetTileSelect();

	//set the selections
	for(i = 0; i < (int) nValid.size(); i++)
	{
		m_bTileSelect[nValid[i]] = TRUE;
	}

	return nValid.size();
}

/////////////////////////////////////////////////////////////////////////////
// chitless filter rule 0
/////////////////////////////////////////////////////////////////////////////
void CGLView::filterRule0()
{
	int i;
	vector <int> nValid;

	//if we get here, which I don't think is even possible, it means that there
	//are no restrictions on what tile they can select, which means that the only
	//filters are that it must be on the main island

	//run through the list
	for(i = 0; i < m_nTiles; i++)
	{
		//if it's an ocean tile or not an initial placement tile, it cannot
		//be selected
		if( (TRUE == VALID_OCEAN(m_pBoard[i].getType())) ||
			(FALSE == m_pBoard[i].getInitial()) || 
			(TRUE == m_pBoard[i].getChitless()) ||
			(FALSE == VALID_RESOURCE(m_pBoard[i].getType())))
		{
			continue;
		}

		//otherwise, it's valid
		nValid.push_back(i);
	}

	//now reset the array and set the new values
	m_nGLDraw = 0;
	resetTileSelect();

	//set the selections
	for(i = 0; i < (int) nValid.size(); i++)
	{
		m_bTileSelect[nValid[i]] = TRUE;
	}
}

/////////////////////////////////////////////////////////////////////////////
// someone buys a dev card in network/hotseat mode
/////////////////////////////////////////////////////////////////////////////
int CGLView::buyDevCard(void)
{
	int iCard;

	//set the card
	iCard = m_iDevCardSet[m_iDevCardArray[m_iDevCardIndex]];

	//increment the index
	m_iDevCardIndex++;

	//return the card
	return iCard;
}

/////////////////////////////////////////////////////////////////////////////
// handle the timer
/////////////////////////////////////////////////////////////////////////////
void CGLView::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_LOSE_CARDS:
		//kill the timer
		KillTimer(m_uiTimer);

		//run the lose cards
		runLoseCards();
		
		break;
	case TIMER_GOLD:
		//kill the timer
		KillTimer(m_uiTimer);

		//run gold
		runGold(m_nGoldPlayers);
		break;
	default:
		break;
	}
	
	CGLBaseView::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// run the lose cards dialog
/////////////////////////////////////////////////////////////////////////////
void CGLView::runLoseCards()
{
	int i, j;
	int iRet;
	int iTotal;
	int iLost[5];
	int iPlayers[6];
	CMultiUseDialog dlg;

	//zero out the players array
	::ZeroMemory(iPlayers, sizeof(iPlayers));
	::ZeroMemory(iLost, sizeof(iLost));
	::ZeroMemory(m_iLostCards, sizeof(m_iLostCards));

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
			iPlayers[i]++;
		}
	}
	
	//play sound
	SOUND(SOUND_LOSE_ON_7);

	//if we're in network mode, let others know that they're waiting
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientWaitForLose(TRUE);
	}

	//set the losing players array
	dlg.setPlayers(iPlayers);

	//set the mode
	dlg.setMode(MULTIUSE_LOSE);

	//make app aware
	THISVIEW->setDialog(&dlg);

	//run it
	iRet = dlg.DoModal();

	//make app aware
	THISVIEW->setDialog(NULL);

	//parse results
	if(IDCANCEL == iRet || IDABORT == iRet)
	{	
		//they cancelled
		m_iLostTotal = -1;
	}

	//if players lost cards, report that
	if(0 < m_iLostTotal)
	{
		//add an undo for each player that lost cards
		for(i = 0; i < m_iNumPlayers; i++)
		{
			//if in network mode, only check self
			if(GAME_MODE_NETWORK == m_pGame->m_iMode && m_pGame->m_iIndex != i)
			{
				continue;
			}

			//reset total
			iTotal = 0;

			//keep track if they lost anything
			for(j = 0; j < 5; j++)
			{
				//set the array
				iLost[j] = m_iLostCards[i][j];
				iTotal += iLost[j];
			}

			//see if they lost any
			if(0 == iTotal)
			{
				continue;
			}

			//handle it
			handleCommonLoseCards(i, iLost);

			//see if we need to tell others
			if(GAME_MODE_NETWORK == m_pGame->m_iMode)
			{
				THISVIEW->clientLoseCards(iLost);
			}
		}

		//no longer have cards to lose
		m_iLostTotal = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// pick a random card from a player
/////////////////////////////////////////////////////////////////////////////
int CGLView::getRandomCard(int iPlayer)
{
	int i, j;
	int iRes;
	int iRandom;
	vector <int> iCards;

	for(i = 0; i < 5; i++)
	{
		iRes = GAME->m_players[iPlayer].m_iRes[i];

		for(j = 0; j < iRes; j++)
		{
			iCards.push_back(i);
		}
	}

	//get a random array value
	iRandom = int(RAND * iCards.size());

	//return the random card
	return iCards[iRandom];
}
