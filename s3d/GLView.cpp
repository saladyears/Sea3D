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
#include "NetworkTeamDialog.h"
#include "rand.h"
#include "math.h"
#include "defineIni.h"
#include "defineColors.h"
#include "defineMap.h"
#include "defineVariants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGLView

CGLView::CGLView()
{
	//no game yet
	m_pGame = NULL;
}

CGLView::~CGLView()
{
	//clear all
	resetAll();

	//delete the game
	deleteGame();
}

BEGIN_MESSAGE_MAP(CGLView, CGLBaseView)
	//{{AFX_MSG_MAP(CGLView)
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGLView message handlers

/////////////////////////////////////////////////////////////////////////////
// handle window creation
/////////////////////////////////////////////////////////////////////////////
int CGLView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGLBaseView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//create the new game
	m_pGame = new CGame();

	//set game mode
	m_pGame->m_iMode = APP->getMode();

	//start off in normal mode
	m_iMode = GL_NORMAL;

	//current setup piece is first
	m_iSetup = 0;

	//reset all variables
	resetAll();

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// reset all variables
/////////////////////////////////////////////////////////////////////////////
void CGLView::resetAll()
{
	list <CUndoNode>::iterator i;

	//current player
	m_iPlayer = 0;

	//start off increasing, with settlement
	m_bIncrease = TRUE;
	m_bCity = TRUE;

	//robber is not there
	m_iRobber = 0;

	//no robber player yet
	m_iRobberPlayer = 0;

	//pirate settings
	m_iPirate = -1;
	m_iPiratePlayer = -1;

	//no longest road yet
	m_iLongestLength = 0;

	//no players yet
	m_iNumPlayers = 0;

	//start at first dev card
	m_iDevCardIndex = 0;

	//no volcano roll yet
	m_nVolcanoRoll = -1;

	//not in special build phase
	m_nSpecialStart = -1;
	m_bSpecialBuild = FALSE;

	//tournament road placements
	m_bTournamentRoads = FALSE;

	//start at the first hidden tile
	m_nHiddenIndex = 0;
	m_nHiddenVolcanoIndex = 0;
	m_nHiddenChitIndex = 0;
	m_nChitlessTile = -1;
	m_nChitlessChitIndex = 0;

	//no losers yet
	m_iLostTotal = 0;

	//no move turn yet
	m_iLastMoveTurn = -1;

	//no soldier card
	m_bSoldier = FALSE;

	//no selection
	m_bUseSelection = FALSE;

	//no free roads
	m_iFreeRoads = 0;

	//no moving ships
	m_iMovingShip[0] = -1;
	m_iMovingShip[1] = -1;

	//no gold
	m_nGetsGold = 0;

	//no port index
	m_nPortTile= -1;
	m_nPortIndex = 0;

	//no screen shot
	m_bScreen = FALSE;

	//not in port placement
	m_bPortPlacement = FALSE;
	
	if(NULL == m_pGame)
	{
		return;
	}

	//iterate the undo list and delete pointers
	for(i = m_pGame->m_listUndo.begin(); i != m_pGame->m_listUndo.end(); i++)
	{
		if(NULL != (*i).m_pData)
		{
			delete ((*i).m_pData);
		}
	}

}

/////////////////////////////////////////////////////////////////////////////
// load board data from the selected map
/////////////////////////////////////////////////////////////////////////////
void CGLView::loadMap(BOOL bClient, BOOL bSkip)
{
	int i;

	//calculate players
	m_iNumPlayers = m_pGame->m_players.size();

	//resets the eye
	resetEye();

	//tile amounts
	if(FALSE == bSkip)
	{
		//set the allowed cards if varianted
		if(IS_VARIANT(VARIANT_ALTER_ALLOWED_CARDS))
		{
			m_iAllowedCards = VARIANTS->m_iAllowedCards;
		}
		else
		{
			//set allowed cards
			m_iAllowedCards = MAP->m_iAllowedCards;
		}

		//set bank resources
		for(i = 0; i < 5; m_pGame->m_iBankRes[i++] = MAP->m_iBankRes);
	}

	//get total size
	m_nTiles = MAP->m_iLandTiles + MAP->m_iSeaTiles;

	//set the bit so that the light array is turned on
	m_nGLDraw = GL_BIT_LIGHT;

	//reset light array
	resetLight();
	
	//build the board pieces
	if(FALSE == bClient)
	{
		buildBoard();
	}

	//generate the board if desired
	if(m_pGame->m_bGenerate) 
	{
		generateBoard();
	}

	//build the texture maps
	if(IS_VARIANT(VARIANT_USE_JUNGLE) || IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
	{
		MAP->m_iChipsUsed |= MAP_USES_SEA_CHIPS;
	}

	buildOtherTextures();

	//set initial rotation
	m_dyRot = MAP->m_dInitialRot;

	//make sure textures are enabled
	glEnable(GL_TEXTURE_2D);
}

/////////////////////////////////////////////////////////////////////////////
// delete the current game
/////////////////////////////////////////////////////////////////////////////
void CGLView::deleteGame(void)
{
	if(m_pGame)
	{
		delete m_pGame;
		m_pGame = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// randomize an array of integers
/////////////////////////////////////////////////////////////////////////////
void CGLView::randomizeArray(vector <int> *pArray)
{
	int i;
	int iSize;
	int iTemp;
	int iSwitch;

	//get the size of the array
	iSize = pArray->size();

	//make a pass through and randomize
	for(i = 0; i < iSize; i++)
	{
		//get the random value
		iSwitch = int(RAND * iSize);

		//don't replace this index with itself!
		if(iSwitch == i) 
		{
			continue;
		}

		//switch the values
		iTemp = (*pArray)[i];
		(*pArray)[i] = (*pArray)[iSwitch];
		(*pArray)[iSwitch] = iTemp;
	}
}

/////////////////////////////////////////////////////////////////////////////
// create the board pieces
/////////////////////////////////////////////////////////////////////////////
void CGLView::buildBoard()
{
	int i;

	//set to the correct number of tiles
	m_pBoard.RemoveAll();
	m_pBoard.SetSize(m_nTiles);

	//run through and create each piece
	for(i = 0; i < m_nTiles; i++)
	{
		m_pBoard[i].copyFromMap(&(MAP->m_mapPieces[i]));
	}
}

/////////////////////////////////////////////////////////////////////////////
// generate a random board, if desired
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateBoard()
{
	//generate the random tiles
	generateRandomTiles();

	//create hidden tiles
	generateHiddenTiles();

	//generate chitless tiles
	generateChitlessTiles();

	//generate the ports
	generatePorts();

	//set the chips
	if(TRUE == MAP->m_bUseChipMethod)
	{
		setChips();
	}

	//generate volcanoes if we have them
	if(IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
	{
		generateVolcanoes();
	}

	//generate jungles if we have them
	if(IS_VARIANT(VARIANT_USE_JUNGLE))
	{
		generateJungles();
	}

	//set all the ports and rotations
	findPorts();

	//generate the dev cards array
	generateDevCards();

	//now set the mode
	m_iMode = GL_NORMAL;
}

/////////////////////////////////////////////////////////////////////////////
// generate random tiles on the board
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateRandomTiles()
{
	int i, j, k;
	int nRoll;
	int nRandomTiles;
	int nRandomChits;
	BOOL bValid;
	vector <int> nTileIndex;
	vector <int> nTiles;
	vector <int> nChitIndex;
	vector <int> nChits;

	//get the number of random tiles in this map
	nRandomTiles = MAP->m_nRandomTiles;

	//see if we need to continue
	if(0 == nRandomTiles)
	{
		return;
	}

	//resize the index array and the random tile array
	nTileIndex.resize(nRandomTiles);
	nTiles.resize(nRandomTiles);

	//fill it in consecutive order to start
	for(i = 0; i < nRandomTiles; i++) nTileIndex[i] = i;

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nTileIndex);

	//set up the array of random land tiles to iterate through
	for(i = RES_TIMBER, k = 0; i <= RES_OCEAN; i++)
	{
		for(j = 0; j < MAP->m_iRandomRes[i]; j++)
		{
			nTiles[k++] = i;
		}
	}

	//now set the tiles on the map
	for(i = 0, j = 0; i < m_nTiles; i++)
	{
		//only assign random tiles if they are set to the MAP_RANDOM type
		if(MAP_RANDOM & MAP->m_mapPieces[i].m_iPieceType)
		{
			m_pBoard[i].setType(nTiles[nTileIndex[j++]]);

			//all sea tiles cannot be initial placements
			if(TRUE == VALID_OCEAN(m_pBoard[i].getType()))
			{
				m_pBoard[i].setInitial(FALSE);
			}
		}
	}

	//get the number of random chits in the map
	nRandomChits = MAP->m_nRandomChits;

	//see if we need to continue
	if(0 == nRandomChits)
	{
		return;
	}

	//resize the index array and the hidden chit array
	nChitIndex.resize(nRandomChits);
	nChits.resize(nRandomChits);

	//fill the index array
	for(i = 0; i < nRandomChits; nChitIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nChitIndex);

	//fill up the array of random chits to iterate through
	for(i = 0, k = 0; i < 11; i++)
	{
		for(j = 0; j < MAP->m_nRandomChit[i]; j++)
		{
			nChits[k++] = i;
		}
	}

	//now set the tiles on the map
	for(i = 0, j = 0; i < m_nTiles; i++)
	{
		//set the chits if we can
		if((MAP_RANDOM & MAP->m_mapPieces[i].m_iPieceType) &&
			(TRUE == VALID_RESOURCE(m_pBoard[i].getType())))
		{
			nRoll = nChits[nChitIndex[j]] + 2;

			//set the roll
			m_pBoard[i].setRoll(nRoll);

			//set the roll texture (assumed to be Seafarers right now)
			if(7 > nRoll)
			{
				m_pBoard[i].setRollTex(nRoll - 2);
			}
			else
			{
				m_pBoard[i].setRollTex(nRoll - 3);
			}

			j++;
		}
	}

	do
	{
		//validate the chits to ensure no 6's and 8's are near each other
		bValid = validateChits();
	}
	while(FALSE == bValid);
}

/////////////////////////////////////////////////////////////////////////////
// ensure that no 6's and 8's are next to each other
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::validateChits()
{
	int i, j, k;
	int nRoll;
	int nR1, nR2;
	int nSwitch;
	int nTiles;
	BOOL bValid = TRUE;
	BOOL bSwitch;
	vector <int> nChitTiles;
	vector <int> nChitIndex;

	//first compile the list of other chit tiles to use
	for(i = 0; i < m_nTiles; i++)
	{
		if((MAP_RANDOM & MAP->m_mapPieces[i].m_iPieceType) &&
			(TRUE == VALID_RESOURCE(m_pBoard[i].getType())))
		{
			nChitTiles.push_back(i);
		}
	}

	//get the number of tiles
	nTiles = nChitTiles.size();

	//now create a random index into the array for switching purposes
	nChitIndex.resize(nTiles);

	//fill the index array
	for(i = 0; i < nTiles; nChitIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nChitIndex);

	//now we traverse the list, stopping at each valid chit tile to see if
	//it is a 6 or an 8 and if it is, then reassigning it
	for(i = 0; i < m_nTiles; i++)
	{
		//get the roll value
		nRoll = m_pBoard[i].getRoll();

		//check for 8 or 6
		if((6 != nRoll) && (8 != nRoll))
		{
			continue;
		}

		//now verify that it is indeed a random chit
		if(FALSE == (MAP_RANDOM & MAP->m_mapPieces[i].m_iPieceType))
		{
			continue;
		}

		//now we check each side to ensure that it is not surrounded by another
		//6 or 8
		for(j = 0; j < 6; j++)
		{
			//get the roll of the neighboring side
			nRoll = m_pBoard[m_pBoard[i].getSide(j)].getRoll();

			//if it's a six or an eight, we've got a problem
			if((6 == nRoll) || (8 == nRoll))
			{
				bValid = FALSE;
				break;
			}
		}

		//if valid, continue
		if(TRUE == bValid)
		{
			continue;
		}

		//traverse the chit index looking for a swap
		for(j = 0; j < nTiles; j++)
		{
			//get the switch value
			nSwitch = nChitTiles[nChitIndex[j]];
			nRoll = m_pBoard[nSwitch].getRoll();

			//if it's the same as the tile we're trying to switch, continue
			if(nSwitch == i)
			{
				continue;
			}

			//if the switch tile is also a 6 or 8 continue, since that would
			//accomplish nothing
			if((6 == nRoll) || (8 == nRoll))
			{
				continue;
			}

			//now check to see if it is valid
			for(k = 0, bSwitch = TRUE; k < 6; k++)
			{
				//get the roll of the neighboring side
				nRoll = m_pBoard[m_pBoard[nSwitch].getSide(k)].getRoll();

				//if it's a six or an eight, we can't use this tile
				if((6 == nRoll) || (8 == nRoll))
				{
					bSwitch = FALSE;
					break;
				}
			}

			//if we can't use the tile, continue
			if(FALSE == bSwitch)
			{
				continue;
			}

			//since we've made it here, we can switch the tiles

			//get the roll values
			nR1 = m_pBoard[i].getRoll();
			nR2 = m_pBoard[nSwitch].getRoll();

			//swap
			m_pBoard[i].setRoll(nR2);
			m_pBoard[nSwitch].setRoll(nR1);

			//swap textures values
			nR1 = m_pBoard[i].getRollTex();
			nR2 = m_pBoard[nSwitch].getRollTex();

			//swap
			m_pBoard[i].setRollTex(nR2);
			m_pBoard[nSwitch].setRollTex(nR1);

			//since we had to switch, continue again
			return FALSE;
		}
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// create any needed hidden tiles arrays
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateHiddenTiles()
{
	int i, j, k;
	int nHiddenTiles;
	int nHiddenChits;
	int nHiddenVolcanoes = 0, nRoll;
	BOOL bVolcanoes = FALSE;
	vector <int> nTileIndex;
	vector <int> nTiles;
	vector <int> nChitIndex;
	vector <int> nChits;

	//see if we even need to do this
	if(FALSE == MAP->m_bHasHiddenTiles)
	{
		return;
	}

	//determine if we're using the jungles/volcanoes variants
	if(IS_VARIANT(VARIANT_USE_VOLCANO) || IS_VARIANT(VARIANT_USE_VOLCANO_GOLD) || IS_VARIANT(VARIANT_USE_JUNGLE))
	{
		bVolcanoes = TRUE;
	}

	//get the number of hidden tiles in this map
	nHiddenTiles = MAP->m_iHiddenTiles;

	//resize the index array and the hidden tile array
	nTileIndex.resize(nHiddenTiles);
	nTiles.resize(nHiddenTiles);
	m_nHiddenArray.resize(nHiddenTiles);

	//fill the index array
	for(i = 0; i < nHiddenTiles; nTileIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nTileIndex);

	//fill up the array of random land tiles to iterate through
	for(i = RES_TIMBER, k = 0; i <= RES_OCEAN; i++)
	{
		for(j = 0; j < MAP->m_iHiddenRes[i]; j++)
		{
			nTiles[k++] = i;
		}
	}

	//if we have deserts and we have jungles/volcanoes on, it will have to 
	//be replaced
	if(TRUE == bVolcanoes)
	{
		nHiddenVolcanoes = MAP->m_iHiddenRes[RES_DESERT];

		//resize the array
		m_nHiddenVolcanoArray.resize(nHiddenVolcanoes);

		//fill it
		for(i = 0; i < nHiddenVolcanoes; i++)
		{
			//create a random roll value
			nRoll = int(RAND * 6);

			//map to actual roll
			if(nRoll < 3)
			{
				nRoll += 3;
			}
			else
			{
				nRoll += 6;
			}

			//sanity
			if(nRoll > 11) nRoll = 11;

			//set it
			m_nHiddenVolcanoArray[i] = nRoll;
		}
	}

	//now fill the actual array
	for(i = 0; i < nHiddenTiles; i++)
	{
		m_nHiddenArray[i] = nTiles[nTileIndex[i]];
	}

	//get the number of hidden chits in the map
	nHiddenChits = MAP->m_iHiddenChits;

	//resize the index array and the hidden chit array
	nChitIndex.resize(nHiddenChits);
	nChits.resize(nHiddenChits);
	m_nHiddenChitArray.resize(nHiddenChits);

	//fill the index array
	for(i = 0; i < nHiddenChits; nChitIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nChitIndex);

	//fill up the array of random land tiles to iterate through
	for(i = 0, k = 0; i < 11; i++)
	{
		for(j = 0; j < MAP->m_nHiddenChit[i]; j++)
		{
			nChits[k++] = i;
		}
	}

	//now fill the actual array
	for(i = 0; i < nHiddenChits; i++)
	{
		m_nHiddenChitArray[i] = nChits[nChitIndex[i]];
	}
}

/////////////////////////////////////////////////////////////////////////////
// generate chitless tiles on a board
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateChitlessTiles()
{
	int i, j, k;
	int nChitlessTiles;
	int nChitlessChits;
	vector <int> nTiles;
	vector <int> nChits;
	vector <int> nTileIndex;
	vector <int> nChitIndex;

	//see if we even need to do this
	if(FALSE == MAP->m_bHasChitlessTiles)
	{
		return;
	}

	//get the number of random tiles in this map
	nChitlessTiles = MAP->m_iChitlessTiles;

	//see if we need to continue
	if(0 == nChitlessTiles)
	{
		return;
	}

	//resize the index array and the random tile array
	nTileIndex.resize(nChitlessTiles);
	nTiles.resize(nChitlessTiles);

	//fill it in consecutive order to start
	for(i = 0; i < nChitlessTiles; nTileIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nTileIndex);

	//set up the array of random land tiles to iterate through
	for(i = RES_TIMBER, k = 0; i <= RES_OCEAN; i++)
	{
		for(j = 0; j < MAP->m_iChitlessRes[i]; j++)
		{
			nTiles[k++] = i;
		}
	}

	//now set the tiles on the map
	for(i = 0, j = 0; i < m_nTiles; i++)
	{
		//only assign random tiles if they are set to the MAP_RANDOM type
		if(MAP_CHITLESS & MAP->m_mapPieces[i].m_iPieceType)
		{
			m_pBoard[i].setType(nTiles[nTileIndex[j++]]);
		}
	}

	//now create the chitless chit index
	nChitlessChits = MAP->m_iChitlessChits;

	//resize the index array and the hidden chit array
	nChitIndex.resize(nChitlessChits);
	nChits.resize(nChitlessChits);
	m_nChitlessChitArray.resize(nChitlessChits);

	//fill the index array
	for(i = 0; i < nChitlessChits; nChitIndex[i] = i++);

	//randomize it, so that if the array is traversed from first to last, it
	//presents a random order of indexes
	randomizeArray(&nChitIndex);

	//fill up the array of random land tiles to iterate through
	for(i = 0, k = 0; i < 11; i++)
	{
		for(j = 0; j < MAP->m_nChitlessChit[i]; j++)
		{
			nChits[k++] = i;
		}
	}

	//now fill the actual array
	for(i = 0; i < nChitlessChits; i++)
	{
		m_nChitlessChitArray[i] = nChits[nChitIndex[i]];
	}
}

/////////////////////////////////////////////////////////////////////////////
// generate ports on the board
/////////////////////////////////////////////////////////////////////////////
void CGLView::generatePorts()
{
	int i, j, k;
	int nOcean;
	int nPorts;
	vector <int> nTileIndex;
	vector <int> nTiles;

	//sanity check to see if the map lets players assign ports
	if(TRUE == MAP->m_bAssignsPorts)
	{
		return;
	}

	//get the number of ports in this map
	nPorts = MAP->m_nPorts;

	//resize the array to use ports
	nTileIndex.resize(nPorts);
	nTiles.resize(nPorts);

	//fill it in consecutive order to start
	for(i = 0; i < nPorts; nTileIndex[i] = i++);

	//randomize it again
	randomizeArray(&nTileIndex);

	//now create the port type index
	for(i = RES_PORT3, k = 0; i <= RES_PORTSHEEP; i++)
	{
		for(j = 0; j < MAP->m_iRandomRes[i]; j++)
		{
			nTiles[k++] = i;
		}
	}

	//if this is the original settlers maps, the ports and sea tiles need
	//to alternate around the edge of the board
	if((MAP_SETTLERS_34 == MAP->m_uiID) || (MAP_SETTLERS_56 == MAP->m_uiID))
	{		
		//set the random ocean number
		nOcean = int(RAND * 1.0);

		//run through the sea array
		for(i = 0; i < nPorts; i++)
		{
			//set the non ocean types
			m_pBoard[MAP->m_iLandTiles + (i << 1) + nOcean].setType(nTiles[nTileIndex[i]]);

			//set the ocean types
			m_pBoard[MAP->m_iLandTiles +(i << 1) + !nOcean].setType(RES_OCEAN);
		}
	}
	//otherwise, we search for port squares to set
	else
	{
		for(i = 0, j = 0; i < m_nTiles; i++)
		{
			//skip non-port tiles
			if(FALSE == (MAP_PORT & MAP->m_mapPieces[i].m_iPieceType))
			{
				continue;
			}

			//set the port
			m_pBoard[i].setType(nTiles[nTileIndex[j++]]);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// generate the array of ports to be placed
/////////////////////////////////////////////////////////////////////////////
void CGLView::generatePortPlace()
{
	int i, j, k;
	int nPorts;
	vector <int> nPortIndex;

	//create the random array of ports to be placed
	
	//get the number of ports in this map
	nPorts = MAP->m_nPorts;

	//resize the array to use ports
	nPortIndex.resize(nPorts);
	m_nPortArray.resize(nPorts);

	//fill it in consecutive order to start
	for(i = 0; i < nPorts; nPortIndex[i] = i++);

	//randomize it again
	randomizeArray(&nPortIndex);

	//now create the port type index
	for(i = RES_PORT3, k = 0; i <= RES_PORTSHEEP; i++)
	{
		for(j = 0; j < MAP->m_iRandomRes[i]; j++)
		{
			m_nPortArray[nPortIndex[k++]] = i;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// replace deserts with volcanoes
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateVolcanoes()
{
	int i, j;
	int nSideRoll;
	int nRoll;
	BOOL bRed;

	//replace all deserts with volcanoes
	for(i = 0; i < m_nTiles; i++)
	{
		if(RES_DESERT != m_pBoard[i].getType())
		{
			continue;
		}

		//set new type
		m_pBoard[i].setType(RES_VOLCANO);

		//scan around this tile to see if there are any red chip rolls nearby
		for(j = 0, bRed = FALSE; j < 6; j++)
		{
			//get the roll of the neigboring tile
			nSideRoll = m_pBoard[m_pBoard[i].getSide(j)].getRoll();

			//see if it's a red roll (6 or 8)
			if((6 == nSideRoll) || (8 == nSideRoll))
			{
				bRed = TRUE;
			}
		}

		//get the initial roll
		nRoll = int(RAND * 10);

		//create a random roll value
		while(TRUE == bRed)
		{
			//see if we can stop if we've got a non 6 or 8 roll
			if((4 != nRoll) && (5 != nRoll))
			{
				bRed = FALSE;
			}
			else
			{
				nRoll = int(RAND * 10);
			}
		}

		//map to actual roll
		if(nRoll < 5)
		{
			nRoll += 2;
		}
		else
		{
			nRoll += 3;
		}

		//sanity
		if(nRoll > 12) nRoll = 12;

		//set roll values on board
		m_pBoard[i].setRoll(nRoll);
		m_pBoard[i].setRollTex(nRoll - 2 - ((nRoll > 7) ? 1 : 0));
		m_pBoard[i].setRollTexType(MAP_USES_SEA_CHIPS);

		//no initial placement
		m_pBoard[i].setInitial(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// replace deserts with jungles
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateJungles()
{
	int i;
	int nRoll;

	//replace all deserts with volcanoes
	for(i = 0; i < m_nTiles; i++)
	{
		if(RES_DESERT != m_pBoard[i].getType())
		{
			continue;
		}

		//set new type
		m_pBoard[i].setType(RES_JUNGLE);

		//create a random roll value
		nRoll = int(RAND * 6);

		//map to actual roll
		if(nRoll < 3)
		{
			nRoll += 3;
		}
		else
		{
			nRoll += 6;
		}

		//sanity
		if(nRoll > 11) nRoll = 11;

		//set roll values on board
		m_pBoard[i].setRoll(nRoll);
		m_pBoard[i].setRollTex(nRoll - 2 - ((nRoll > 7) ? 1 : 0));
		m_pBoard[i].setRollTexType(MAP_USES_SEA_CHIPS);
	}
}

/////////////////////////////////////////////////////////////////////////////
// create the dev card arrays at the beginning of a game
/////////////////////////////////////////////////////////////////////////////
void CGLView::generateDevCards()
{
	int i, j, k;
	int nDevCards;

	//dev cards
	nDevCards = MAP->m_iDevTotal;

	//resize the dev card array
	m_iDevCardArray.resize(nDevCards);
	m_iDevCardSet.resize(nDevCards);

	//fill the dev card array index with random entries
	for(i = 0; i < nDevCards; m_iDevCardArray[i] = i++);

	//randomize the dev card array, which points into the set array during
	//the game
	randomizeArray(&m_iDevCardArray);

	//set up what cards are in the actual set
	for(i = 0, k= 0; i < CARD_SIZE; i++)
	{
		for(j = 0; j < MAP->m_iDevCards[i]; j++)
		{
			m_iDevCardSet[k++] = i;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// resets the eye placement to its starting location
/////////////////////////////////////////////////////////////////////////////
void CGLView::resetEye(void)
{
	int i;
	int iComma;
	CString str;

	//reset eyepoint
	m_dCamera[0] = 0.;
	m_dCamera[1] = (float) MAP->m_dInitialDepth;
	m_dCamera[2] = 1.0;	

	//reset rotation
	m_dxRot = 0.;
	m_dyRot = 0.;

	//reset look point
	m_dLook[0] = 0.;
	m_dLook[1] = 0.;
	m_dLook[2] = 0.;

	//camera pos
	GetPrivateProfileString(INI_OPENGL, INI_CAMERA_POS, "", str.GetBuffer(32), 32, INI_FILE);
	str.ReleaseBuffer();

	if(FALSE == str.IsEmpty())
	{
		//deconstruct
		for(i = 0; i < 3; i++)
		{
			iComma = str.Find(',', 0);
			if(-1 == iComma)
			{
				iComma = str.GetLength();
			}
			//break it down
			m_dCamera[i] = (float) atof(str.Left(iComma));
			str = str.Right(str.GetLength() - iComma - 1);
		}
	}

	//rotation
	GetPrivateProfileString(INI_OPENGL, INI_ROTATION, "", str.GetBuffer(32), 32, INI_FILE);
	str.ReleaseBuffer();

	if(FALSE == str.IsEmpty())
	{
		//deconstruct
		for(i = 0; i < 2; i++)
		{
			iComma = str.Find(',', 0);
			if(-1 == iComma)
			{
				iComma = str.GetLength();
			}
			//break it down
			if(i) m_dyRot = atof(str.Left(iComma));
			else m_dxRot = atof(str.Left(iComma));
			str = str.Right(str.GetLength() - iComma - 1);
		}
	}

	//look point
	GetPrivateProfileString(INI_OPENGL, INI_LOOK_POS, "", str.GetBuffer(32), 32, INI_FILE);
	str.ReleaseBuffer();

	if(FALSE == str.IsEmpty())
	{
		//deconstruct
		for(i = 0; i < 3; i++)
		{
			iComma = str.Find(',', 0);
			if(-1 == iComma)
			{
				iComma = str.GetLength();
			}
			//break it down
			m_dLook[i] = (float) atof(str.Left(iComma));
			str = str.Right(str.GetLength() - iComma - 1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// reset lighting
/////////////////////////////////////////////////////////////////////////////
void CGLView::resetLight()
{
	BOOL bOn;

	//determine light
	bOn = m_nGLDraw & GL_BIT_LIGHT;

	//clear the array
	m_bLight.clear();
	m_bLight.resize(m_nTiles, bOn ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// reset tile selection
/////////////////////////////////////////////////////////////////////////////
void CGLView::resetTileSelect()
{
	BOOL bSelect;

	bSelect = m_nGLDraw & GL_BIT_TILE_SELECT;

	m_bTileSelect.clear();
	m_bTileSelect.resize(m_nTiles, bSelect ? TRUE : FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// randomize the player order for network or hotseat games
/////////////////////////////////////////////////////////////////////////////
void CGLView::randomizePlayers()
{
	int i;
	int iSwitch;
	int iPlayers;
	int iColor;
	int nTeamIndex, nTeamSpot;
	CPlayer player;
	CNetworkTeamDialog team;

	//get the number of players
	iPlayers = m_pGame->m_players.size();

	if(TRUE == m_pGame->m_bTeam)
	{
		//ask the host who their partner is
		team.DoModal();

		//get the index of their teammate
		nTeamIndex = team.m_nTeamCombo + 1;

		//pick a random player to switch the host with
		iSwitch = int(RAND * iPlayers);

		//if it's not zero, switch them
		if(0 != iSwitch)
		{
			//if it's with the teammate, set the new teammate index
			if(iSwitch == nTeamIndex)
			{
				nTeamIndex = 0;
			}

			//switch the values
			player = m_pGame->m_players[0].m_player;
			m_pGame->m_players[0].m_player = m_pGame->m_players[iSwitch].m_player;
			m_pGame->m_players[iSwitch].m_player = player;

			//switch colors
			iColor = m_pGame->m_players[0].m_iColor;
			m_pGame->m_players[0].m_iColor = m_pGame->m_players[iSwitch].m_iColor;
			m_pGame->m_players[iSwitch].m_iColor = iColor;
		}

		//set the teamspot
		nTeamSpot = ((iSwitch + 2) % 4);

		//now switch the teammate with whoever is in their spot (if we have to)
		if(nTeamSpot != nTeamIndex)
		{
			//switch the values
			player = m_pGame->m_players[nTeamSpot].m_player;
			m_pGame->m_players[nTeamSpot].m_player = m_pGame->m_players[nTeamIndex].m_player;
			m_pGame->m_players[nTeamIndex].m_player = player;

			//switch colors
			iColor = m_pGame->m_players[nTeamSpot].m_iColor;
			m_pGame->m_players[nTeamSpot].m_iColor = m_pGame->m_players[nTeamIndex].m_iColor;
			m_pGame->m_players[nTeamIndex].m_iColor = iColor;
		}

		//now, for the heck of it, switch the remaining two if desired
		if(1 == int(RAND * 2))
		{
			nTeamSpot = ((iSwitch + 1) % 4);
			nTeamIndex = ((nTeamSpot + 2) % 4);

			//switch the values
			player = m_pGame->m_players[nTeamSpot].m_player;
			m_pGame->m_players[nTeamSpot].m_player = m_pGame->m_players[nTeamIndex].m_player;
			m_pGame->m_players[nTeamIndex].m_player = player;

			//switch colors
			iColor = m_pGame->m_players[nTeamSpot].m_iColor;
			m_pGame->m_players[nTeamSpot].m_iColor = m_pGame->m_players[nTeamIndex].m_iColor;
			m_pGame->m_players[nTeamIndex].m_iColor = iColor;
		}
	}
	else
	{
		//make a pass through and randomize
		for(i = 0; i < iPlayers; i++)
		{
			//get the random value
			iSwitch = int(RAND * iPlayers);

			//sanity check
			if(iSwitch == i) continue;

			//switch the values
			player = m_pGame->m_players[i].m_player;
			m_pGame->m_players[i].m_player = m_pGame->m_players[iSwitch].m_player;
			m_pGame->m_players[iSwitch].m_player = player;

			//switch colors
			iColor = m_pGame->m_players[i].m_iColor;
			m_pGame->m_players[i].m_iColor = m_pGame->m_players[iSwitch].m_iColor;
			m_pGame->m_players[iSwitch].m_iColor = iColor;
		}
	}

	//if we're in network mode and we're the server, make sure our index is correct
	if ((GAME_MODE_NETWORK == m_pGame->m_iMode) && 
		(TRUE == m_pGame->m_bServer))
	{
		for(i = 0; i < iPlayers; i++)
		{
			//find it
			if(m_pGame->m_uiPlayerID == m_pGame->m_players[i].m_player.getID())
			{
				m_pGame->m_iIndex = i;
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// take a screen shot of the OpenGL window
/////////////////////////////////////////////////////////////////////////////
BOOL CGLView::screenShot(CString strFile)
{
	m_bScreen = TRUE;
	m_strScreenPath = strFile;

	//draw it
	Invalidate();

	return TRUE;
}
