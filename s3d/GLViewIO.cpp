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
#include "UndoDialog.h"
#include "StatsDialog.h"
#include "GameDatabase.h"
#include "SettlersView.h"
#include "defineStat.h"
#include "defineVariants.h"
#include "defineMap.h"

/////////////////////////////////////////////////////////////////////////////
// save the game to the game database
/////////////////////////////////////////////////////////////////////////////
void CGLView::saveGame()
{
	//save the game to the database
	DATABASE->addGame(m_pGame, UNDO, this);
}

/////////////////////////////////////////////////////////////////////////////
// load a game from the database
/////////////////////////////////////////////////////////////////////////////
void CGLView::loadGame(int iGame, BOOL bIncomplete)
{
	int i;

	//load the game data
	if(TRUE == bIncomplete)
	{
		DATABASE->loadIncompleteGame(iGame, m_pGame, UNDO, this);

		//set the save game flag
		THISVIEW->setGameIndex(iGame);
	}
	else
	{
		DATABASE->loadGame(iGame, m_pGame, UNDO, this);
	}

	//set some size stuff
	m_nTiles = m_pBoard.GetSize();
	m_iNumPlayers = m_pGame->m_players.size();

	//reset road lengths
	calculateRoadLengths();

	//load up the map they used
	MAPS->loadMapByID(m_pGame->m_uiMapID);

	//reset look position
	resetEye();

	//copy the initial rotation
	m_dyRot = MAP->m_dInitialRot;

	//reset the jungle/volcano variants
	if(IS_VARIANT(VARIANT_USE_JUNGLE) || IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
	{
		MAP->m_iChipsUsed |= MAP_USES_SEA_CHIPS;
	}

	//build the texture maps
	buildOtherTextures();

	//make sure textures are enabled
	glEnable(GL_TEXTURE_2D);

	//initialize stats
	STAT->init();

	//update stats
	for(i = 0; i < STAT_SIZE; i++)
	{
		STAT->update(i);
	}

	//the game has been inited
	m_pGame->m_bInit = TRUE;

	//prepare build options if we're in game
	if(GAME_COMPLETE != m_pGame->m_iState)
	{
		m_iMode = GL_WAIT;

		//reset the time ref
		m_pGame->m_timeRef = COleDateTime::GetCurrentTime();
	}
	else
	{
		m_iMode = GL_GAME_OVER;

		//switch to the player panel
		THISVIEW->initPanel();

		//update counts
		for(i = 0; i < (int) m_pGame->m_players.size(); i++)
		{
			THISVIEW->updateCounts(i);
		}

		//toggle actions off
		ACTIONS->ShowWindow(SW_HIDE);

		//toggle controls
		CONTROLS->ShowWindow(SW_HIDE);
	}

	//refresh
	Invalidate();
}

/////////////////////////////////////////////////////////////////////////////
// called when loading an incomplete game
/////////////////////////////////////////////////////////////////////////////
void CGLView::updateGame()
{
	int i;

	prepareRoads();
	prepareCities();
	prepareSettlements();

	//switch to the player panel
	THISVIEW->initPanel();

	//see if we're in a turn
	if(GAME_IN_TURN == m_pGame->m_iState)
	{
		THISVIEW->getControlPanel()->enterTurn();

		//show/hide things
		THISVIEW->getControlPanel()->showRolls(FALSE);

		//toggle actions off
		ACTIONS->ShowWindow(SW_HIDE);
	}
	else
	{
		THISVIEW->getControlPanel()->showRolls(TRUE);

		//toggle actions on
		ACTIONS->ShowWindow(SW_SHOW);
	}

	//set correct color
	THISVIEW->getControlPanel()->updateColor(m_iPlayer);

	//toggle controls
	CONTROLS->ShowWindow(SW_SHOW);

	//disable all options
	THISVIEW->getControlPanel()->EnableDC();
	THISVIEW->getControlPanel()->EnableTrade();
	THISVIEW->getControlPanel()->EnableEndTurn();

	//update counts
	for(i = 0; i < (int) m_pGame->m_players.size(); i++)
	{
		THISVIEW->updateCounts(i);
	}
}

/////////////////////////////////////////////////////////////////////////////
// store and load GL drawing data
/////////////////////////////////////////////////////////////////////////////
void CGLView::Serialize(CArchive& ar) 
{
	//save information
	if(ar.IsStoring())
	{
		save(ar);
	}
	//load information
	else
	{
		load(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// load GL drawing data
/////////////////////////////////////////////////////////////////////////////
void CGLView::load(CArchive &ar)
{
	int i;
	int iSize;
	BOOL bInGame;
	int iOld[6];
	BOOL bNothing;
	int nDummy;

	//read in the board pieces
	ar >> iSize;
	
	//resize the array
	m_pBoard.SetSize(iSize);

	//board pieces
	for(i = 0; i < iSize; i++)
	{
		m_pBoard[i].Serialize(ar);
	}

	//the number of land tiles
	//ar >> m_iLandTiles;			//RETIRED IN 1.0.1
	if(DATABASE->gameVer() < VER_SEAFARERS)
	{
		ar >> nDummy;
	}

	//the longest road length
	ar >> m_iLongestLength;

	//read in longest road segments
	ar >> iSize;

	//resize the array
	m_iLongestRoad.resize(iSize);

	//longest road
	for(i = 0; i < iSize; i++)
	{
		ar >> m_iLongestRoad[i];
	}

	//the current robber location
	ar >> m_iRobber;

	//the current pirate location
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar >> m_iPirate;
	}

	//current drawing mode
	if(DATABASE->gameVer() >= VER_NETWORKc)
	{
		ar >> m_iMode;
	}
	else
	{
		//must be a done game if it doesn't have state
		m_iMode = GL_GAME_OVER;
	}

	//if it's not game over mode, read in all the background variables
	if(GL_GAME_OVER != m_iMode)
	{
		//current player
		ar >> m_iPlayer;

		//selection?
		ar >> m_bUseSelection;

		//selection
		ar >> m_iSelection;

		//light array size
		ar >> iSize;

		//light array
		m_bLight.resize(iSize);
		for(i = 0; i < iSize; i++)
		{
			ar >> m_bLight[i];
		}

		//number of sea tiles
		//ar >> m_iSeaTiles;			//RETIRED IN 1.0.1
		if(DATABASE->gameVer() < VER_SEAFARERS)
		{
			ar >> nDummy;
		}

		//who played the robber last
		ar >> m_iRobberPlayer;

		//who played the pirate last
		if(DATABASE->gameVer() >= VER_SEAFARERS)
		{
			ar >> m_iPiratePlayer;

			//the ships built on this turn
			ar >> iSize;

			//resize
			m_nShipsThisTurn.SetSize(iSize);

			//read in
			for(i = 0; i < iSize; i++)
			{
				ar >> m_nShipsThisTurn[i];
			}
		}

		//the number of allowed cards in hand
		ar >> m_iAllowedCards;

		if(DATABASE->gameVer() < VER_AI)
		{
			ar >> bInGame;
		}

		//whether a soldier is being played
		ar >> m_bSoldier;

		//for road building, how many free road placements are left
		ar >> m_iFreeRoads;

		//dev cards
		ar >> m_iDevCardIndex;

		//dev card arrays
		ar >> iSize;

		//resize arrays
		m_iDevCardArray.resize(iSize);
		m_iDevCardSet.resize(iSize);

		//dev card info
		for(i = 0; i < iSize; i++)
		{
			ar >> m_iDevCardArray[i];
			ar >> m_iDevCardSet[i];
		}

		//hidden cards
		if(DATABASE->gameVer() >= VER_FOUR_ISLANDS)
		{
			//index
			ar >> m_nHiddenIndex;

			//array
			ar >> iSize;
			m_nHiddenArray.resize(iSize);
			for(i = 0; i < iSize; i++)
			{
				ar >> m_nHiddenArray[i];
			}

			//hidden volcanoes/jungles
			if(DATABASE->gameVer() >= VER_OCEANSIV_FIX)
			{
				//index
				ar >> m_nHiddenVolcanoIndex;

				//array
				ar >> iSize;
				m_nHiddenVolcanoArray.resize(iSize);
				for(i = 0; i < iSize; i++)
				{
					ar >> m_nHiddenVolcanoArray[i];
				}
			}

			//chit index
			ar >> m_nHiddenChitIndex;

			//chit array
			ar >> iSize;
			m_nHiddenChitArray.resize(iSize);
			for(i = 0; i < iSize; i++)
			{
				ar >> m_nHiddenChitArray[i];
			}
		}

		//chitless chits
		if(DATABASE->gameVer() >= VER_GREATER_CATAN)
		{
			//index
			ar >> m_nChitlessChitIndex;

			//array
			ar >> iSize;
			m_nChitlessChitArray.resize(iSize);
			for(i = 0; i < iSize; i++)
			{
				ar >> m_nChitlessChitArray[i];
			}
		}

		//ports size
		ar >> iSize;

		//ports data
		m_iPorts.SetSize(iSize);
		for(i = 0; i < iSize; i++)
		{
			ar >> m_iPorts[i];
		}

		//last rolled tiles size
		ar >> iSize;

		//last rolled tiles
		m_iLastTiles.resize(iSize);
		for(i = 0; i < iSize; i++)
		{
			ar >> m_iLastTiles[i];
		}

		//other stuff
		ar >> m_iLastCity;

		if(DATABASE->gameVer() < VER_094)
		{
			ar.Read(iOld, sizeof(iOld));
			ar >> bNothing;
			ar >> bNothing;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// store GL drawing data
/////////////////////////////////////////////////////////////////////////////
void CGLView::save(CArchive &ar)
{
	int i;
	int iSize;

	//write out the board pieces
	iSize = m_pBoard.GetSize();

	//board pieces
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		m_pBoard[i].Serialize(ar);
	}

	//the number of land tiles
	//ar << m_iLandTiles;			//RETIRED IN 1.0.1

	//the longest road length
	ar << m_iLongestLength;

	//the longest road segments
	iSize = m_iLongestRoad.size();

	//longest road
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		ar << m_iLongestRoad[i];
	}

	//the current robber location
	ar << m_iRobber;

	//the current pirate location
	ar << m_iPirate;

	//current drawing mode
	ar << m_iMode;

	//if we're not in game over mode, write out all background variables
	if(GL_GAME_OVER != m_iMode)
	{
		//current player
		ar << m_iPlayer;

		//selection?
		ar << m_bUseSelection;

		//selection
		ar << m_iSelection;

		//light array size
		iSize = m_bLight.size();

		//light array
		ar << iSize;
		for(i = 0; i < iSize; i++)
		{
			ar << m_bLight[i];
		}

		//number of sea tiles
		//ar << m_iSeaTiles;			//RETIRED IN 1.0.1

		//who played the robber last
		ar << m_iRobberPlayer;

		//who played the pirate last
		ar << m_iPiratePlayer;

		//the ships built on this turn
		ar << m_nShipsThisTurn.GetSize();

		//write out
		for(i = 0; i < m_nShipsThisTurn.GetSize(); i++)
		{
			ar << m_nShipsThisTurn[i];
		}

		//the number of allowed cards in hand
		ar << m_iAllowedCards;

		//helper for undo
		//ar << m_bInGame;			//MOVED TO GAME.CPP IN 0.9.2

		//whether a soldier is being played
		ar << m_bSoldier;

		//for road building, how many free road placements are left
		ar << m_iFreeRoads;

		//dev cards
		ar << m_iDevCardIndex;

		//dev card arrays
		iSize = m_iDevCardArray.size();

		//dev card info
		ar << iSize;
		for(i = 0; i < iSize; i++)
		{
			ar << m_iDevCardArray[i];
			ar << m_iDevCardSet[i];
		}

		//hidden tiles
		ar << m_nHiddenIndex;

		//hidden array
		iSize = m_nHiddenArray.size();
		ar << iSize;
		
		for(i = 0; i < iSize; i++)
		{
			ar << m_nHiddenArray[i];
		}

		//hidden volcano info
		ar << m_nHiddenVolcanoIndex;

		//hidden array
		iSize = m_nHiddenVolcanoArray.size();
		ar << iSize;
		
		for(i = 0; i < iSize; i++)
		{
			ar << m_nHiddenVolcanoArray[i];
		}

		//hidden chits
		ar << m_nHiddenChitIndex;

		//hidden chit array
		iSize = m_nHiddenChitArray.size();
		ar << iSize;
		
		for(i = 0; i < iSize; i++)
		{
			ar << m_nHiddenChitArray[i];
		}

		//chitless chits
		ar << m_nChitlessChitIndex;

		//chitless chit array
		iSize = m_nChitlessChitArray.size();
		ar << iSize;

		for(i = 0; i < iSize; i++)
		{
			ar << m_nChitlessChitArray[i];
		}

		//ports size
		iSize = m_iPorts.GetSize();

		//ports data
		ar << iSize;
		for(i = 0; i < iSize; i++)
		{
			ar << m_iPorts[i];
		}

		//last rolled tiles size
		iSize = m_iLastTiles.size();

		//last rolled tiles
		ar << iSize;
		for(i = 0; i < iSize; i++)
		{
			ar << m_iLastTiles[i];
		}

		//other stuff
		ar << m_iLastCity;

		//RETIRED IN 0.9.4
		//ar.Write(m_iPossibleRoads, sizeof(m_iPossibleRoads));
		//ar << m_bIncrease;
		//ar << m_bCity;
	}
}

