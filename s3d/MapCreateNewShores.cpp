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
#include "MapDatabase.h"
#include "defineGL.h"
#include "defineMap.h"

#ifdef DEBUG_BUILD_MAPS
/////////////////////////////////////////////////////////////////////////////
// create the seafarers new shores map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createNewShores(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_NEW_SHORES;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 13 points
	pMap->m_iPoints = 13;

	//map name
	pMap->m_strMapName = "New Shores";
	pMap->m_strMapCreator = "Jason Fugate";

	//uses chip laying algorithm
	pMap->m_bUseChipMethod = TRUE;

	//random tiles used
	pMap->m_nRandomTiles = 19;

	//ports
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_34_CHIPS | MAP_USES_SEA_CHIPS;	

	//min/max players
	pMap->m_iMinPlayers = 3;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 27;
	pMap->m_iSeaTiles = 49;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//set the extra points
	pMap->m_nBonus1 = 1;

	//set the roll places
	pMap->m_iRollPlaces[0] = 27;
	pMap->m_iRollPlaces[1] = 43;
	pMap->m_iRollPlaces[2] = 61;
	pMap->m_iRollPlaces[3] = 63;
	pMap->m_iRollPlaces[4] = 47;
	pMap->m_iRollPlaces[5] = 29;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 4;
	pMap->m_iRandomRes[RES_WHEAT] = 4;
	pMap->m_iRandomRes[RES_ORE] = 3;
	pMap->m_iRandomRes[RES_CLAY] = 3;
	pMap->m_iRandomRes[RES_SHEEP] = 4;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 1;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 4;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_XX, pMap, coordsNewShores);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 95.;
}


/////////////////////////////////////////////////////////////////////////////
// New Shores extra coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsNewShores(int iIndex, CMapPiece *pPiece)
{
	//set tile types
	switch(iIndex)
	{
	case 27: case 28: case 29: case 35: case 36: case 37: case 38: case 43:
	case 44: case 45: case 46: case 47: case 52: case 53: case 54: case 55:
	case 61: case 62: case 63:
		//set land type
		pPiece->m_iPieceType = MAP_RANDOM;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use 34 chips
		pPiece->m_iPermaRollTexType = MAP_USES_34_CHIPS;

		break;
	//islands
	case 9: case 10: case 13: case 14: case 15: case 40: case 49: case 66:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//no initial
		pPiece->m_bInitial = FALSE;

		//secondary
		pPiece->m_bSecondary = TRUE;

		//sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	//ports
	case 18: case 20: case 30: case 34: case 48: case 51: case 64: case 69:
	case 71:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		break;
	default:
		//ocean tiles
		pPiece->m_iPieceType = MAP_SEA;
		pPiece->m_iPermaType = RES_OCEAN;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		break;
	}

	//tile pieces
	switch(iIndex)
	{
	case 9:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 10:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 13:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 4;
		break;
	case 14:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 3;
		break;
	case 15:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 8;
		break;
	case 40:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 10;
		break;
	case 49:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 2;
		break;
	case 66:
		pPiece->m_iPermaType = RES_GOLD;		
		pPiece->m_iPermaRoll = 9;
		break;
	}
}

#endif		//DEBUG_BUILD_MAPS