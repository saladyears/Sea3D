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
// creates the Seafarers: Into The Desert (VI) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createIntoDesertVI(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_INTO_DESERT_VI;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Into The Desert (VI)";
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

	//min/max players (six player only)
	pMap->m_iMinPlayers = 6;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 43;
	pMap->m_iSeaTiles = 60;

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
	createMap(MAP_BORDERS_XX_X_XX, pMap, coordsIntoDesertVI);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 110.;
}

/////////////////////////////////////////////////////////////////////////////
// Into The Desert (VI) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsIntoDesertVI(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 39: case 40: case 41: case 42: case 47: case 48: case 49: case 50: 
	case 51: case 52: case 53: case 58: case 59: case 60: case 61: case 62: 
	case 63: case 64: case 70: case 71: case 81: case 82:
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
	case 27: case 28: case 29: case 30: case 31:
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
		break;
	//islands
	case 12: case 13: case 14: case 15: case 16: case 18: case 19: case 20: 
	case 21: case 55: case 78: case 84: case 85: case 87: case 89: case 90:
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
	case 37: case 38: case 43: case 46: case 65: case 69: case 72: case 74: case 75:
	case 83:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(38 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(43 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(65 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(74 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(75 == iIndex) pPiece->m_iPermaPortSide = 5;

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

	case 12:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 4;
		break;
	case 13:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 12;
		break;
	case 14:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 15:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 16:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 18:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 19:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 20:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 21:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 11;
		break;
	case 27:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 28:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 29:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 30:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 31:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 39:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 40:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 41:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 42:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 47:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 2;
		break;
	case 48:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 10;
		break;
	case 49:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 50:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 51:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 6;
		break;
	case 52:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 53:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 55:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 58:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 12;
		break;
	case 59:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 9;
		break;
	case 60:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 3;
		break;
	case 61:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 62:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 63:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 64:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 70:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 3;
		break;
	case 71:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 4;
		break;
	case 78:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 81:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 6;
		break;
	case 82:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 84:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 8;
		break;
	case 85:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 87:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 10;
		break;
	case 89:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 90:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
