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
// creates the Seafarers: Greater Catan (VI) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createGreaterCatanVI(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_GREATER_CATAN_VI;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 18 points
	pMap->m_iPoints = 18;

	//map name
	pMap->m_strMapName = "Greater Catan (V/VI)";
	pMap->m_strMapCreator = "Jason Fugate";

	//9 allowed cards in hamd
	pMap->m_iAllowedCards = 9;

	//beefed up starting bank
	pMap->m_iBankRes = 25;

	//beefed up dev cards
	pMap->m_iDevTotal = 34;
	pMap->m_iDevCards[CARD_VICTORY] = 5;
	pMap->m_iDevCards[CARD_MONOPOLY] = 3;
	pMap->m_iDevCards[CARD_ROAD_BUILD] = 3;
	pMap->m_iDevCards[CARD_YEAR_PLENTY] = 3;
	pMap->m_iDevCards[CARD_SOLDIER] = 20;

	//uses chip laying algorithm
	pMap->m_bUseChipMethod = TRUE;

	//uses chitless tiles
	pMap->m_bHasChitlessTiles = TRUE;

	//there are 30 random tiles
	pMap->m_nRandomTiles = 30;

	//ports
	pMap->m_nPorts = 11;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_56_CHIPS | MAP_USES_SEA_CHIPS;	

	//min/max players
	pMap->m_iMinPlayers = 5;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 50;
	pMap->m_iSeaTiles = 53;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//extra cities in this scenario
	pMap->m_iStockCities = 8;

	//set the roll places
	pMap->m_iRollPlaces[0] = 15;
	pMap->m_iRollPlaces[1] = 48;
	pMap->m_iRollPlaces[2] = 84;
	pMap->m_iRollPlaces[3] = 86;
	pMap->m_iRollPlaces[4] = 53;
	pMap->m_iRollPlaces[5] = 17;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 6;
	pMap->m_iRandomRes[RES_WHEAT] = 6;
	pMap->m_iRandomRes[RES_ORE] = 5;
	pMap->m_iRandomRes[RES_CLAY] = 5;
	pMap->m_iRandomRes[RES_SHEEP] = 6;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 2;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 6;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set up chitless tiles
	pMap->m_iChitlessTiles = 20;

	//set the amounts
	pMap->m_iChitlessRes[RES_TIMBER] = 4;
	pMap->m_iChitlessRes[RES_WHEAT] = 4;
	pMap->m_iChitlessRes[RES_ORE] = 4;
	pMap->m_iChitlessRes[RES_CLAY] = 4;
	pMap->m_iChitlessRes[RES_SHEEP] = 4;
	pMap->m_iChitlessRes[RES_GOLD] = 0;
	pMap->m_iChitlessRes[RES_DESERT] = 0;

	//set the chitless chits
	pMap->m_iChitlessChits = 7;

	//set the amounts
	pMap->m_nChitlessChit[0] = 1;
	pMap->m_nChitlessChit[1] = 1;
	pMap->m_nChitlessChit[2] = 1;
	pMap->m_nChitlessChit[3] = 1;
	pMap->m_nChitlessChit[4] = 0;
	pMap->m_nChitlessChit[5] = 0;
	pMap->m_nChitlessChit[6] = 0;
	pMap->m_nChitlessChit[7] = 1;
	pMap->m_nChitlessChit[8] = 1;
	pMap->m_nChitlessChit[9] = 1;
	pMap->m_nChitlessChit[10] = 0;

	//create the map
	createMap(MAP_BORDERS_XX_X_XX, pMap, coordsGreaterCatanVI);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 115.;

	//set initial rotation
	pMap->m_dInitialRot = -30.;
}

/////////////////////////////////////////////////////////////////////////////
// Greater Catan (VI) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsGreaterCatanVI(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 15: case 16: case 17: case 26: case 27: case 28: case 29: case 37:
	case 38: case 39: case 40: case 41: case 48: case 49: case 50: case 51:
	case 52: case 53: case 60: case 61: case 62: case 63: case 64: case 72:
	case 73: case 74: case 75: case 84: case 85: case 86:
		//set land type
		pPiece->m_iPieceType = MAP_RANDOM;

		//set the initial placement to TRUE for starting land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_56_CHIPS;

		break;
	//ports
	case 3: case 6: case 25: case 30: case 36: case 54: case 59: case 76:
	case 87: case 95: case 97:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		if(25 == iIndex) pPiece->m_iPermaPortSide = 4;
		if(36 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(97 == iIndex) pPiece->m_iPermaPortSide = 5;
		
		break;
	//chitless islands
	case 12: case 13: case 19: case 20: case 21: case 24: case 31: case 32:
	case 35: case 44: case 55: case 58: case 66: case 67: case 70: case 78:
	case 81: case 82: case 89: case 90:
		//tile type
		pPiece->m_iPieceType = MAP_CHITLESS;

		//disallow initial build
		pPiece->m_bInitial = FALSE;

		//allow secondary build
		pPiece->m_bSecondary = TRUE;

		//set sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	default:
		//set ocean defaults
		pPiece->m_iPieceType = MAP_SEA;

		//set ocean
		pPiece->m_iPermaType = RES_OCEAN;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
