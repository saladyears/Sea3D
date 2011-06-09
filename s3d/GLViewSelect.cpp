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
#include "UndoOne.h"
#include "UndoShip.h"
#include "SettlersView.h"
#include "MultiUseDialog.h"
#include "defineNetwork.h"
#include "defineSounds.h"
#include "defineStat.h"
#include "defineMap.h"
#include "defineCode.h"
#include "defineVariants.h"

/////////////////////////////////////////////////////////////////////////////
// catch a selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::SelectObject(int id)
{
	//set the selection id
	m_iSelection = id - 1;

	//check for a good selection
	if(m_iSelection < 0) return;

	//call the selection
	if(FALSE == m_bHitTest)
	{
		//handle it
		handleSelect();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelect()
{
	switch(m_iMode)
	{
	case GL_PLACE_PLAYERS:
		handleSelectPlace();
		break;
	case GL_PLACE_THIRD_ROAD:
		handleSelectThirdRoad();
		break;
	case GL_PLACE_ROAD:
		handleSelectRoad();
		break;
	case GL_PLACE_SHIP:
		handleSelectShip();
		break;
	case GL_PLACE_CITY:
		handleSelectCity();
		break;
	case GL_PLACE_SETTLEMENT:
		handleSelectSettle();
		break;
	case GL_PLACE_ROBBER:
		handleSelectRobber();
		break;
	case GL_CHOOSE_SHIP:
		handleSelectChooseShip();
		break;
	case GL_MOVE_SHIP:
		handleSelectMoveShip();
		break;
	case GL_PLACE_PORT:
		handleSelectPort();
		break;
	case GL_CHITLESS_TILE:
		handleSelectChitless();
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// see if a chitless tile is revealed
/////////////////////////////////////////////////////////////////////////////
void CGLView::checkForChitlessTile(int nTile, int nCorner, int &nChitless)
{
	int nRoll;
	int nTile2;

	//sanity
	if(FALSE == MAP->m_bHasChitlessTiles)
	{
		return;
	}

	//get the other tile
	nTile2 = m_pBoard[nTile].getSide(nCorner);

	//check for chitless on tile 1
	if( (TRUE == m_pBoard[nTile].getChitless()) && 
		(FALSE == m_pBoard[nTile].getInitial()))
	{
		//see if we're over the limit
		if(m_nChitlessChitIndex >= (int) m_nChitlessChitArray.size())
		{
			//use special placement rules
			m_nChitlessTile = nTile;
			
			return;
		}
		else
		{
			//get the roll value
			nRoll = m_nChitlessChitArray[m_nChitlessChitIndex];

			//set the chit
			m_pBoard[nTile].setRoll(nRoll + 2);
			m_pBoard[nTile].setRollTex(nRoll - (nRoll > 5));

			//no longer chitless
			m_pBoard[nTile].setChitless(FALSE);

			//set the chitless value
			nChitless = nTile;

			//increment the index
			m_nChitlessChitIndex++;
		}
	}

	//repeat for tile 2
	if( (TRUE == m_pBoard[nTile2].getChitless()) && 
		(FALSE == m_pBoard[nTile].getInitial()))
	{
		//see if we're over the limit
		if(m_nChitlessChitIndex >= (int) m_nChitlessChitArray.size())
		{
			//use special placement rules
			m_nChitlessTile = nTile2;
			
			return;
		}
		else
		{
			//get the roll value
			nRoll = m_nChitlessChitArray[m_nChitlessChitIndex];

			//set the chit
			m_pBoard[nTile2].setRoll(nRoll + 2);
			m_pBoard[nTile2].setRollTex(nRoll - (nRoll > 5));

			//no longer chitless
			m_pBoard[nTile2].setChitless(FALSE);

			//set the chitless value
			nChitless = nTile2;

			//increment the index
			m_nChitlessChitIndex++;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// see if a hidden tile is revealed
/////////////////////////////////////////////////////////////////////////////
int CGLView::checkForHiddenTile(int nTile, int nCorner)
{
	int i;
	int nRes;
	int nRoll;
	int nHiddenTile = -1;
	BOOL bVolcanoes = FALSE;
	BOOL bSkip = FALSE;

	//see if this map even supports them
	if(FALSE == MAP->m_bHasHiddenTiles)
	{
		return -1;
	}

	//determine if we're using the jungles/volcanoes variants
	if(IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD) || IS_VARIANT(VARIANT_USE_JUNGLE))
	{
		bVolcanoes = TRUE;
	}

	//get the possibly hidden tiles
	for(i = 0; i < 2; i++)
	{
		//check both sides
		if(0 == i)
		{
			nHiddenTile = m_pBoard[nTile].getSide((nCorner + 5) % 6);
		}
		else
		{
			nHiddenTile = m_pBoard[nTile].getSide((nCorner + 1) % 6);
		}

		//check to see if the next tile is hidden
		if(RES_HIDDEN == m_pBoard[nHiddenTile].getType())
		{
			//get the next resource
			nRes = m_nHiddenArray[m_nHiddenIndex];

			//see if it's a desert and needs to be replaced
			if((RES_DESERT == nRes) && (TRUE == bVolcanoes))
			{
				//set the type
				if(IS_VARIANT(VARIANT_USE_JUNGLE))
				{
					nRes = RES_JUNGLE;
				}
				else
				{
					nRes = RES_VOLCANO;
				}

				//get the next roll
				nRoll = m_nHiddenVolcanoArray[m_nHiddenVolcanoIndex];

				//set the roll
				m_pBoard[nHiddenTile].setRoll(nRoll);

				//set the roll tex
				if(m_pBoard[nHiddenTile].getRollTexType() & MAP_USES_SEA_CHIPS)
				{
					m_pBoard[nHiddenTile].setRollTex((nRoll > 5) ? (nRoll - 3) : (nRoll - 2));
				}
				else
				{
					//NOT NEEDED YET
				}

				//increment index
				m_nHiddenVolcanoIndex++;

				//set the skip flag
				bSkip = TRUE;
			}

			//reveal it
			m_pBoard[nHiddenTile].setType(nRes);

			//see if it needs a chit
			if((TRUE == VALID_RESOURCE(nRes)) && (FALSE == bSkip))
			{
				//get the next roll
				nRoll = m_nHiddenChitArray[m_nHiddenChitIndex];

				//set the roll
				m_pBoard[nHiddenTile].setRoll(nRoll + 2);

				//set the roll tex
				if(m_pBoard[nHiddenTile].getRollTexType() & MAP_USES_SEA_CHIPS)
				{
					m_pBoard[nHiddenTile].setRollTex((nRoll > 5) ? (nRoll - 1) : nRoll);
				}
				else
				{
					//NOT NEEDED YET
				}

				//give the player a resource
				incResource(RES_STAT_DISCOVER, m_iPlayer, nRes);

				//increment the index
				m_nHiddenChitIndex++;

				//update the stat
				STAT->update(STAT_GOODS_RECV_DISCOVER);
			}

			//increment the index
			m_nHiddenIndex++;

			//we found one so break out of the loop
			break;
		}
		else
		{
			nHiddenTile = -1;
		}
	}
	
	return nHiddenTile;
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a city or road
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectPlace()
{
	int ID;
	int iTile1;
	int iC1;
	BOOL bAI;
	BOOL bShip;

	//determine if the current player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	if(TRUE == m_bCity)
	{
		//deconstruct the selection
		DECODE_SEL(m_iSelection, iTile1, iC1);

		//do common stuff
		handleCommonSettlePlace(m_iPlayer, iTile1, iC1);

		//if we are in network mode, others need to learn about this
		if(GAME_MODE_NETWORK == m_pGame->m_iMode)
		{
			//set next text
			if(FALSE == bAI)
			{
				THISVIEW->getControlPanel()->setPanelText("Your turn to place a road.");
			}
			ID = ENCODE_NET(m_iPlayer, iTile1, iC1);
			THISVIEW->clientSettlePlace(ID);
		}

		//check to see if a ship can be built
		if(TRUE == handleCommonInitialShipCheck(iTile1, iC1))
		{
			return;
		}

		//prepare the initial road locations
		prepareInitialRoads();

		//update the panel text
		THISVIEW->getControlPanel()->updateText();
		
		//continue road placement
		m_iMode = GL_PLACE_PLAYERS;

		//turn on hit tests for roads
		setHitTest(TRUE);

		//if we're in a tour, keep going
		if(TRUE == APP->inTour())
		{
			THISVIEW->closeBalloon();
		}
	}
	else
	{
		//determine if we are using ships or roads
		bShip = (0 < m_iPossibleShips.size());

		//use common code
		if((FALSE == bShip) && (FALSE == handleSelectRoad()))
		{
			return;
		}
		else if((TRUE == bShip) && (FALSE == handleSelectShip()))
		{
			return;
		}

		//clear the arrays
		if(FALSE == m_bTournamentRoads)
		{
			m_iPossibleRoads.resize(0);
			m_iPossibleRoads.clear();
			m_iPossibleShips.resize(0);
			m_iPossibleShips.clear();
		}

		//if we're in network mode, we're done
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			return;
		}

		//go up a player
		m_iPlayer += (m_bIncrease ? 1 : -1);
		if(m_iPlayer >= m_iNumPlayers)
		{
			m_iPlayer = m_iNumPlayers - 1; 
			m_bIncrease = FALSE;
		}

		//see if we're done
		if(m_iPlayer < 0) 
		{
			//start back at first player
			m_iPlayer = 0;

			//move on to the start of the game
			setNextMode();

			//set the reference time for the first player's turn
			m_pGame->m_timeRef = COleDateTime::GetCurrentTime();
		}
		else
		{
			//turn on hit tests
			setHitTest(TRUE);
		}

		//reneable the undo if it was disabled
		if(GAME_MODE_HOTSEAT == GAME->m_iMode)
		{
			//enable the undo
			THISVIEW->getMessageButton()->enableUndo(TRUE);
		}

		//update the player information display
		THISVIEW->updatePlayerDisplay();

		//update the panel text
		THISVIEW->getControlPanel()->updateText();

		//if we're in a tour, keep going
		if(TRUE == APP->inTour())
		{
			THISVIEW->closeBalloon();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// derotate a road placement
/////////////////////////////////////////////////////////////////////////////
int CGLView::deRotate(int &iTile, int &iCorner)
{
	double dRot, dAdj;

	if(FALSE == m_pGame->m_players[m_iPlayer].m_player.getAI())
	{
		//set the adjustment
		dAdj = 60. - ((iCorner % 3) * 120.);

		//derotate
		dRot = m_pBoard[iTile].getRotation();
		iCorner += int((dRot + dAdj) / 60.);
		iCorner = (iCorner + 6) % 6;
	}

	return ENCODE_SEL(iTile, iCorner);
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a third road in tournament starting
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectThirdRoad()
{
	int ID;
	int i, iCorner, iC;
	int iTile1;
	BOOL bGood = FALSE;

	//deconstruct the selection
	DECODE_SEL(m_iSelection, iTile1, iCorner);

	//derotate
	iC = deRotate(iTile1, iCorner);	

	//search through the list and see if we have a good selection
	for(i = 0; i < (int) m_iPossibleRoads.size(); i++)
	{
		if(iC == m_iPossibleRoads[i]) {bGood = TRUE; break;}
	}

	//check
	if(FALSE == bGood) 
	{
		return;
	}

	//do it
	handleCommonRoadPlace(m_iPlayer, iTile1, iCorner);

	//keep going in hotseat mode
	if(GAME_MODE_HOTSEAT == m_pGame->m_iMode)
	{
		//increment the player
		m_iPlayer++;

		//if over, move to the next phase
		if(m_iPlayer >= m_iNumPlayers)
		{
			//no longer in tournament road mode
			m_bTournamentRoads = FALSE;

			//reset to start of list
			m_iPlayer = 0;

			//continue on
			setNextMode();
		}
		else
		{
			m_bUseSelection = TRUE;

			//reset the selection
			m_iSelection = -1;

			//turn on hit tests
			setHitTest(TRUE);

			//update the display text
			THISVIEW->getControlPanel()->updateText();

			//determine possible roads
			prepareRoads();
		}
	}
	//send it along in network mode
	else
	{
		//return to waiting
		m_iMode = GL_WAIT;

		//tell others
		ID = ENCODE_NET(m_iPlayer, iTile1, iCorner);
		THISVIEW->clientRoadPlace(ID);
	}

	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a road once the game has started
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::handleSelectRoad()
{
	int ID;
	int i, iCorner, iC;
	int iTile1;
	BOOL bGood = FALSE;

	//deconstruct the selection
	DECODE_SEL(m_iSelection, iTile1, iCorner);

	//derotate
	iC = deRotate(iTile1, iCorner);	

	//search through the list and see if we have a good selection
	for(i = 0; i < (int) m_iPossibleRoads.size(); i++)
	{
		if(iC == m_iPossibleRoads[i]) {bGood = TRUE; break;}
	}

	//check
	if(FALSE == bGood) 
	{
		return FALSE;
	}

	//do it
	handleCommonRoadPlace(m_iPlayer, iTile1, iCorner);

	//set board entry
	if( (-1 == m_nChitlessTile) &&
		((GAME_MODE_NETWORK == m_pGame->m_iMode) || 
		(TRUE == m_pGame->m_bInGame)))
	{
		m_iMode = GL_WAIT;
		
		//turn off selections
		m_bUseSelection = FALSE;
			
		//reset the selection
		m_iSelection = -1;
	}

	//set the next mode
	if(TRUE == m_pGame->m_bInGame)
	{
		//the have now build on this turn, so trading should be disabled
		m_pGame->m_players[m_iPlayer].m_bPlacedBuild = TRUE;

		//only enable stuff if we don't have to set a chitless tile
		if(-1 == m_nChitlessTile)
		{
			//enable/disable trading and dc buying
			THISVIEW->getControlPanel()->EnableTrade();
			THISVIEW->getControlPanel()->EnableDC();
			THISVIEW->getControlPanel()->EnableEndTurn();

			//show everything again
			THISVIEW->showBuildOptions(TRUE);
		}
	}

	//if we are in network mode, others need to learn about this
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		//tell others
		ID = ENCODE_NET(m_iPlayer, iTile1, iCorner);
		THISVIEW->clientRoadPlace(ID);
	}

	//check for the win
	checkForWin();

	//see if we need to set the chitless tile
	if((GAME_COMPLETE != m_pGame->m_iState) && (-1 != m_nChitlessTile))
	{
		initChitless();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a ship
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::handleSelectShip()
{
	int ID;
	int i, iCorner, iC;
	int iTile1;
	BOOL bGood = FALSE;

	//deconstruct the selection
	DECODE_SEL(m_iSelection, iTile1, iCorner);

	//derotate
	iC = deRotate(iTile1, iCorner);	

	//search through the list and see if we have a good selection
	for(i = 0; i < (int) m_iPossibleShips.size(); i++)
	{
		if(iC == m_iPossibleShips[i]) {bGood = TRUE; break;}
	}

	//check
	if(FALSE == bGood) 
	{
		return FALSE;
	}

	//do it
	handleCommonShipPlace(m_iPlayer, iTile1, iCorner);

	//set board entry
	if( (-1 == m_nChitlessTile) &&
		((GAME_MODE_NETWORK == m_pGame->m_iMode) || 
		(TRUE == m_pGame->m_bInGame)))
	{
		m_iMode = GL_WAIT;
		
		//turn off selections
		m_bUseSelection = FALSE;
			
		//reset the selection
		m_iSelection = -1;
	}

	//set the next mode
	if(TRUE == m_pGame->m_bInGame)
	{
		//the have now build on this turn, so trading should be disabled
		m_pGame->m_players[m_iPlayer].m_bPlacedBuild = TRUE;

		//only enable stuff if we don't have to set a chitless tile
		if(-1 == m_nChitlessTile)
		{
			//enable/disable trading and dc buying
			THISVIEW->getControlPanel()->EnableTrade();
			THISVIEW->getControlPanel()->EnableDC();
			THISVIEW->getControlPanel()->EnableEndTurn();

			//show everything again
			THISVIEW->showBuildOptions(TRUE);
		}
	}

	//if we are in network mode, others need to learn about this
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		//tell others
		ID = ENCODE_NET(m_iPlayer, iTile1, iCorner);
		THISVIEW->clientShipPlace(ID);
	}

	//check for the win
	checkForWin();

	//see if we need to set the chitless tile
	if((GAME_COMPLETE != m_pGame->m_iState) && (-1 != m_nChitlessTile))
	{
		initChitless();
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a city once the game has started
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectCity()
{
	//check for a good selection
	if(m_iSelection <= 0) return;

	//run it
	handleCommonCityPlace(m_iPlayer, m_iSelection);

	//the have now build on this turn, so trading should be disabled
	m_pGame->m_players[m_iPlayer].m_bPlacedBuild = TRUE;

	//set the next mode
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		//tell others
		THISVIEW->clientCityPlace(m_iSelection);
	}
	
	//set wait mode
	m_iMode = GL_WAIT;

	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//enable/disable trading and dc buying
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//show everything again
	THISVIEW->showBuildOptions(TRUE);

	//check for the win
	checkForWin();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles placement of a settlement once the game has started
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectSettle()
{
	int ID;
	int iTile1;
	int iC1;
	
	//check for a good selection
	if(m_iSelection < 0) return;

	//deconstruct the selection
	DECODE_SEL(m_iSelection, iTile1, iC1);

	//handle common code
	handleCommonSettlePlace(m_iPlayer, iTile1, iC1);

	if(TRUE == m_pGame->m_bInGame)
	{
		//the have now build on this turn, so trading should be disabled
		m_pGame->m_players[m_iPlayer].m_bPlacedBuild = TRUE;
	}

	//tell the network if needed
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		//tell others
		ID = ENCODE_NET(m_iPlayer, iTile1, iC1);
		THISVIEW->clientSettlePlace(ID);
	}

	//set the next mode
	m_iMode = GL_WAIT;

	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//enable/disable trading and dc buying
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//show everything again
	THISVIEW->showBuildOptions(TRUE);

	//check for the win
	checkForWin();

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles selection of the robber placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectRobber()
{
	int ID;
	int iOld;
	int iDie1 = 0, iDie2 = 0;
	BOOL bAI;

	//see if this is even valid
	if(m_iRobber == m_iSelection || m_iPirate == m_iSelection)
	{
		return;
	}

	//first see if this is a pirate placement
	if(TRUE == VALID_OCEAN(m_pBoard[m_iSelection].getType()))
	{
		m_iPiratePlayer = m_iPlayer;
		handleSelectPirate();
		return;
	}

	//set the robber player
	m_iRobberPlayer = m_iPlayer;

	//play the sound
	SOUND(SOUND_ROBBER);

	//see if this player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//save the old robber location
	iOld = m_iRobber;

	//set the new robber tile
	m_iRobber = m_iSelection;

	//reenable the undo if in hotseat
	if(GAME_MODE_HOTSEAT == GAME->m_iMode)
	{
		THISVIEW->getMessageButton()->enableUndo(TRUE);
	}

	//set the next mode
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		//tell others
		THISVIEW->clientPlaceRobber();
	}
	
	//set wait mode
	m_iMode = GL_WAIT;

	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//get die information if needed
	if(FALSE == m_bSoldier)
	{
		THISVIEW->getControlPanel()->getLastRoll(iDie1, iDie2);
	}

	//encode the ID
	ID = ENCODE_ROBBER(iDie1, iDie2, iOld, m_iRobberPlayer, m_bSoldier);

	//add the undo
	addUndo(UNDO_ROBBER_PLACE, new CUndoOne(ID));

	//set to the next turn
	if(TRUE == m_bSoldier)
	{
		//playing soldier is now done
		m_bSoldier = FALSE;
	}

	//handle stealing
	handleCommonInitSteal(bAI);
}

/////////////////////////////////////////////////////////////////////////////
// handles stealing
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleCommonInitSteal(BOOL bAI, BOOL bPirate)
{
	int iRet;
	BOOL bSteal;
	CMultiUseDialog dlg;

	//see if they need to steal from a player
	if(FALSE == bAI)
	{
		if(TRUE == bPirate)
		{
			//set the pirate flag
			dlg.setPirate();
			bSteal = (initPirateSteal(&dlg) > 0);
		}
		else
		{
			bSteal = (initSteal(&dlg) > 0);
		}

		//see if we even need to run the dialog
		if(TRUE == bSteal)
		{
			//warn others what's happening
			if(GAME_MODE_NETWORK == m_pGame->m_iMode)
			{
				THISVIEW->clientPanelText(S3D_WAIT_FOR_STEAL);
			}

			//make app aware
			THISVIEW->setDialog(&dlg);

			//run it
			iRet = dlg.DoModal();

			//make app aware
			THISVIEW->setDialog(NULL);
			
			//if they cancelled, treat as an undo
			if(IDCANCEL == iRet)
			{
				undo();
				return;
			}
			else if(IDABORT == iRet)
			{
				//dumped out in network mode
				return;
			}
		}
	}

	//show everything again
	if(CONTROLPANEL->inTurn())
	{
		//toggle actions off
		THISVIEW->getControlPanel()->showGoods(FALSE);
		ACTIONS->ShowWindow(SW_HIDE);

		THISVIEW->getControlPanel()->updateTurn();

		//tell others what we're doing
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			//toggle controls
			CONTROLS->ShowWindow(SW_SHOW);

			THISVIEW->clientPanelText(S3D_START_TURN);

			//if we're waiting for a rejoin, hide everything again
			if(TRUE == THISVIEW->isWaiting())
			{
				THISVIEW->getSaveState().resetState();
				THISVIEW->getSaveState().saveState();
				THISVIEW->getControlPanel()->setPanelText("Waiting for players to rejoin.");
			}
		}
	}
	//we're before, so show the roll possibilities
	else
	{
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle actions on
		CONTROLS->ShowWindow(SW_SHOW);
		ACTIONS->ShowWindow(SW_SHOW);

		//tell others what we're doing
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			THISVIEW->clientPanelText(S3D_ROLL);
			THISVIEW->getControlPanel()->setPanelText("It is your turn.  Click to roll.");
		}

		//if we're waiting for a rejoin, hide everything again
		if(TRUE == THISVIEW->isWaiting())
		{
			THISVIEW->getSaveState().resetState();
			THISVIEW->getSaveState().saveState();
			THISVIEW->getControlPanel()->setPanelText("Waiting for players to rejoin.");
		}
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles selection of the pirate placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectPirate()
{
	int ID;
	int iOld;
	int iDie1 = 0, iDie2 = 0;
	BOOL bAI;

	//play the sound
	SOUND(SOUND_PIRATE);

	//see if this player is an AI
	bAI = m_pGame->m_players[m_iPlayer].m_player.getAI();

	//save the old robber location
	iOld = m_iPirate;

	//set the new robber tile
	m_iPirate = m_iSelection;

	//reenable the undo if in hotseat
	if(GAME_MODE_HOTSEAT == GAME->m_iMode)
	{
		THISVIEW->getMessageButton()->enableUndo(TRUE);
	}

	//set the next mode
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		//tell others
		THISVIEW->clientPlacePirate();
	}
	
	//set wait mode
	m_iMode = GL_WAIT;

	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//get die information if needed
	if(FALSE == m_bSoldier)
	{
		THISVIEW->getControlPanel()->getLastRoll(iDie1, iDie2);
	}

	//sanity -- this kludge requires that no map ever have more than 99 tiles, which
	//shouldn't be a problem (I hope)
	if(-1 == iOld)
	{
		iOld = 99;
	}

	//create the ID (use the robber code which is the same)
	ID = ENCODE_ROBBER(iDie1, iDie2, iOld, m_iPiratePlayer, m_bSoldier);

	//add the undo
	addUndo(UNDO_PIRATE_PLACE, new CUndoOne(ID));

	//set to the next turn
	if(TRUE == m_bSoldier)
	{
		//playing soldier is now done
		m_bSoldier = FALSE;
	}

	handleCommonInitSteal(bAI, TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// handles selecting a ship to move
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectChooseShip()
{
	int iT1, iT2;
	int iSide1, iSide2;

	//decode the selection
	DECODE_SEL(m_iSelection, iT1, iSide1);

	//get the opposing side
	iT2 = m_pBoard[iT1].getSide(iSide1);
	iSide2 = (iSide1 + 3) % 6;

	//set the values
	m_iMovingShip[0] = m_iSelection;
	m_iMovingShip[1] = ENCODE_SEL(iT2, iSide2);

	//remove these ships from the current tiles
	m_pBoard[iT1].removeShip(iSide1);
	m_pBoard[iT2].removeShip(iSide2);

	//get the current turn placement to save it
	m_iLastMoveTurn = m_pBoard[iT1].getTurnPlacement(iSide1);

	//clear the turn placement time
	m_pBoard[iT1].setTurnPlacement(iSide1, -1);
	m_pBoard[iT2].setTurnPlacement(iSide2, -1);

	//prepare to move the ship
	prepareShips();

	//turn on hit test
	setHitTest(TRUE);

	//turn on selections
	m_bUseSelection = TRUE;
	
	//reset the selection
	m_iSelection = -1;

	//set the draw mode
	m_iMode = GL_MOVE_SHIP;

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles moving a ship
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectMoveShip()
{
	int ID;
	int i, iSide;
	int iTile1;
	int iOT, iOSide;
	BOOL bGood = FALSE;

	//deconstruct the selection
	DECODE_SEL(m_iSelection, iTile1, iSide);

	//derotate
	ID = deRotate(iTile1, iSide);	

	//search through the list and see if we have a good selection
	for(i = 0; i < (int) m_iPossibleShips.size(); i++)
	{
		if(ID == m_iPossibleShips[i]) 
		{
			bGood = TRUE; 
			break;
		}
	}

	//check
	if(FALSE == bGood) 
	{
		return;
	}

	//get the old tile and corner
	DECODE_SEL(m_iMovingShip[0], iOT, iOSide);

	//do it
	handleCommonMoveShip(m_iPlayer, iOT, iOSide, iTile1, iSide);

	//clear the move ship array
	::ZeroMemory(m_iMovingShip, sizeof(m_iMovingShip));

	//if we don't have to set a chitless tile, reset the mode
	if(-1 == m_nChitlessTile)
	{
		//set board entry
		m_iMode = GL_WAIT;
		
		//turn off selections
		m_bUseSelection = FALSE;
		m_iSelection = -1;

		//enable/disable trading and dc buying
		THISVIEW->getControlPanel()->EnableTrade();
		THISVIEW->getControlPanel()->EnableDC();
		THISVIEW->getControlPanel()->EnableEndTurn();
		
		//show everything again
		THISVIEW->showBuildOptions(TRUE);
	}

	//if we are in network mode, others need to learn about this
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		ENCODE_SHIP_MOVE(ID, m_iPlayer, iOT, iOSide, iTile1, iSide, 0);
		
		//tell others
		THISVIEW->clientShipMove(ID);
	}

	//check for the win
	checkForWin();

	//see if we need to set the chitless tile
	if((GAME_COMPLETE != m_pGame->m_iState) && (-1 != m_nChitlessTile))
	{
		initChitless();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handles selecting a port placement
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectPort()
{
	int i;
	int nRes;
	int nSide = 0;
	int nLand = 0;
	int nLandTile = 0;
	BOOL bSetPort = FALSE;

	//do different things based on whether this is the ocean tile or the 
	//land tile
	if(-1 == m_nPortTile)
	{
		//set the port tile
		m_nPortTile = m_iSelection;

		//now check to see if there are multiple land tiles facing this one...
		//if there are, we'll need to select which one to point towards, otherwise
		//we can just assign it now
		for(i = 0; i < 6; i++)
		{
			nSide = m_pBoard[m_nPortTile].getSide(i);

			if((-1 != nSide) && (TRUE == VALID_LAND(m_pBoard[nSide].getType())))
			{
				nLand++;
				nLandTile = i;
			}
		}

		//if there's only one land tile, use it
		if(1 == nLand)
		{
			nSide = nLandTile;
			bSetPort = TRUE;
		}
		//otherwise, prepare the port sides for selection
		else
		{
			preparePortSides();
		}
	}
	else
	{
		//figure out which side the selected tile belongs to
		for(i = 0; i < 6; i++)
		{
			if(m_iSelection == m_pBoard[m_nPortTile].getSide(i))
			{
				nSide = i;
				break;
			}
		}
		
		//set the port
		bSetPort = TRUE;
	}

	//set the port if we can
	if(TRUE == bSetPort)
	{
		//get the res type in hotseat
		if(GAME_MODE_HOTSEAT == m_pGame->m_iMode)
		{
			nRes = m_nPortArray[m_nPortIndex];
		}
		//network
		else
		{
			nRes = m_nPortRes;
		}

		//set it
		handleCommonPortPlace(m_nPortTile, nSide, nRes);

		//if hotseat, continue
		if(GAME_MODE_HOTSEAT == m_pGame->m_iMode)
		{
			//increment the index
			m_nPortIndex++;

			//see if we should continue
			if(m_nPortIndex < (int) m_nPortArray.size())
			{
				//increment the player
				m_iPlayer++;

				//wrap around
				if(m_iPlayer >= (int) m_pGame->m_players.size())
				{
					m_iPlayer = 0;
				}

				//update the player display
				THISVIEW->updatePlayerDisplay();

				//update control panel
				THISVIEW->getControlPanel()->updateText();

				//reset the placement places
				preparePorts();

				//reset the port tile
				m_nPortTile = -1;
			}
			else
			{
				//reset to the starting player
				m_iPlayer = 0;

				//update the player display
				THISVIEW->updatePlayerDisplay();

				//continue on
				setNextMode();

				//update control panel
				THISVIEW->getControlPanel()->updateText();
			}
		}
		else
		{
			//set mode to wait
			m_iMode = GL_WAIT;
			
			//no selection
			m_bUseSelection = FALSE;

			//send it on
			THISVIEW->clientPortPlace(m_nPortTile, nSide, nRes);
		}
	}

	if((GAME_MODE_HOTSEAT == m_pGame->m_iMode) || (-1 != m_nPortTile))
	{
		//reset the selection
		m_iSelection = -1;

		//turn hit tests on again
		setHitTest(TRUE);
	}

	//redraw
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// handles selecting a chitless swap
/////////////////////////////////////////////////////////////////////////////
void CGLView::handleSelectChitless()
{
	//handle it
	handleCommonChitless(m_nChitlessTile, m_iSelection);

	//report it
	if(GAME_MODE_NETWORK == m_pGame->m_iMode)
	{
		THISVIEW->clientChitless(m_nChitlessTile, m_iSelection);
	}

	//reset the chitless tile
	m_nChitlessTile = -1;

	//continue on
	m_iMode = GL_WAIT;
	
	//turn off selections
	m_bUseSelection = FALSE;
		
	//reset the selection
	m_iSelection = -1;

	//enable/disable trading and dc buying
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//show everything again
	THISVIEW->showBuildOptions(TRUE);

	//reset the control panel text
	THISVIEW->getControlPanel()->updateTurn();

	//redraw
	Invalidate();
}