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
// creates the Seafarers: Into The Desert (V) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createIntoDesertV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_INTO_DESERT_V;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Into The Desert (V)";
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

	//this map uses desert buffers
	pMap->m_bDesertBuffer = TRUE;

	//there are no random tiles
	pMap->m_nRandomTiles = 0;

	//ports
	pMap->m_nPorts = 10;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (five player only)
	pMap->m_iMinPlayers = 5;
	pMap->m_iMaxPlayers = 5;

	//number of land/sea tiles
	pMap->m_iLandTiles = 38;
	pMap->m_iSeaTiles = 56;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//set the extra points
	pMap->m_nBonus1 = 1;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 0;
	pMap->m_iRandomRes[RES_WHEAT] = 0;
	pMap->m_iRandomRes[RES_ORE] = 0;
	pMap->m_iRandomRes[RES_CLAY] = 0;
	pMap->m_iRandomRes[RES_SHEEP] = 0;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 0;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 5;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_XX_XX, pMap, coordsIntoDesertV);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 105.;
}

/////////////////////////////////////////////////////////////////////////////
// Into The Desert (V) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsIntoDesertV(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 35: case 36: case 37: case 38: case 43: case 44: case 45: case 46: 
	case 47: case 48: case 53: case 54: case 55: case 56: case 57: case 58:
	case 64: case 74:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		//allow secondary placement for all islands
		pPiece->m_bSecondary = FALSE;
		break;
	//deserts
	case 24: case 25: case 26: case 27: case 28:
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
		break;
	//islands
	case 11: case 12: case 13: case 14: case 16: case 17: case 18: case 19: 
	case 50: case 71: case 76: case 77: case 79: case 81: case 82:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		//allow secondary placement for all islands
		pPiece->m_bSecondary = TRUE;
		break;

	//ports
	case 33: case 34: case 39: case 42: case 59: case 63: case 65: case 67: case 68:
	case 75:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(34 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(39 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(59 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(63 == iIndex) pPiece->m_iPermaPortSide = 4;
		if(67 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(68 == iIndex) pPiece->m_iPermaPortSide = 5;

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
	case 11:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 12;
		break;
	case 12:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 13:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 14:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 16:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 17:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 18:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 19:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 11;
		break;
	case 24:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 25:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 26:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 27:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 28:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 35:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 36:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 37:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 38:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 43:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 10;
		break;
	case 44:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 45:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 46:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 6;
		break;
	case 47:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 48:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 50:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 53:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 9;
		break;
	case 54:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 3;
		break;
	case 55:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 56:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 57:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 58:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 64:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 4;
		break;
	case 71:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 74:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 76:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 8;
		break;
	case 77:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 79:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 10;
		break;
	case 81:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 82:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
