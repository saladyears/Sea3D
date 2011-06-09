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
// creates the Seafarers: Great Crossing (VI) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createGreatCrossingVI(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_GREAT_CROSSING_VI;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 13 points
	pMap->m_iPoints = 13;

	//map name
	pMap->m_strMapName = "The Great Crossing (VI)";
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

	//this map uses trade routes
	pMap->m_bUsesTradeRoutes = TRUE;

	//there are no random tiles
	pMap->m_nRandomTiles = 0;

	//ports
	pMap->m_nPorts = 12;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (six player only)
	pMap->m_iMinPlayers = 6;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 30;
	pMap->m_iSeaTiles = 55;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_PORT3] = 7;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_XX_X, pMap, coordsGreatCrossingVI);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 105.;
}

/////////////////////////////////////////////////////////////////////////////
// Great Crossing (VI) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsGreatCrossingVI(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	//left island
	case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: 
	case 20: case 21: case 22: case 23: case 24: case 25: case 26:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		//this is a different island
		pPiece->m_nIsland = 1;

		break;
	//right island
	case 58: case 59: case 60: case 61: case 62: case 63: case 64: case 67: 
	case 68: case 69: case 70: case 71: case 72: case 73: case 74:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		//this is a different island
		pPiece->m_nIsland = 2;

		break;
	//ports
	case 2: case 4: case 7: case 27: case 30: case 34: case 49: case 53: case 55:
	case 78: case 81: case 83:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(4 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(27 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(30 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(34 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(49 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(53 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(78 == iIndex) pPiece->m_iPermaPortSide = 5;

		break;
	default:
		pPiece->m_iPieceType = MAP_SEA;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		break;
	}

	//tile pieces
	switch(iIndex)
	{
	case 10:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 11:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 12:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 13:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 14:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 12;
		break;
	case 15:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 16:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 17:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 20:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 3;
		break;
	case 21:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 22:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 23:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 6;
		break;
	case 24:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 2;
		break;
	case 25:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 5;
		break;
	case 26:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 58:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 12;
		break;
	case 59:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 60:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 61:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 62:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 4;
		break;
	case 63:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 8;
		break;
	case 64:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 3;
		break;
	case 67:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 9;
		break;
	case 68:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 6;
		break;
	case 69:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 70:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 71:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 10;
		break;
	case 72:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 73:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 74:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 9;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
