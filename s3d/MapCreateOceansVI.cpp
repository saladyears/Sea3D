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
// creates the Seafarers: Oceans (VI) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createOceansVI(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_OCEANS_VI;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Oceans (VI)";
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

	//this map uses hidden tiles
	pMap->m_bHasHiddenTiles = TRUE;

	//ports
	pMap->m_nPorts = 10;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (six player only)
	pMap->m_iMinPlayers = 6;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 34;
	pMap->m_iSeaTiles = 60;
	pMap->m_iHiddenTiles = 25;
	pMap->m_iHiddenChits = 11;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//no extra points

	//set the random resource amounts
	pMap->m_iRandomRes[RES_PORT3] = 5;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set the hidden resource amounts
	pMap->m_iHiddenRes[RES_TIMBER] = 3;
	pMap->m_iHiddenRes[RES_WHEAT] = 2;
	pMap->m_iHiddenRes[RES_ORE] = 2;
	pMap->m_iHiddenRes[RES_CLAY] = 2;
	pMap->m_iHiddenRes[RES_SHEEP] = 2;
	pMap->m_iHiddenRes[RES_GOLD] = 0;
	pMap->m_iHiddenRes[RES_DESERT] = 0;
	pMap->m_iHiddenRes[RES_OCEAN] = 14;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nHiddenChit[0] = 0;
	pMap->m_nHiddenChit[1] = 1;
	pMap->m_nHiddenChit[2] = 1;
	pMap->m_nHiddenChit[3] = 2;
	pMap->m_nHiddenChit[4] = 1;
	pMap->m_nHiddenChit[5] = 0;
	pMap->m_nHiddenChit[6] = 0;
	pMap->m_nHiddenChit[7] = 2;
	pMap->m_nHiddenChit[8] = 2;
	pMap->m_nHiddenChit[9] = 1;
	pMap->m_nHiddenChit[10] = 1;

	//create the map
	createMap(MAP_BORDERS_XX_XX, pMap, coordsOceansVI);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 105.;
}

/////////////////////////////////////////////////////////////////////////////
// Oceans (VI) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsOceansVI(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18:
	case 19: case 23: case 24: case 25: case 26: case 27: case 28: case 35:
	case 36: case 37: case 46: case 47: case 57: case 74: case 82:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces (except the gold)
		if(74 == iIndex || 82 == iIndex)
		{
			pPiece->m_bInitial = FALSE;
		}
		else
		{
			pPiece->m_bInitial = TRUE;
		}

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	//ports
	case 1: case 3: case 5: case 7: case 22: case 29: case 34: case 38:
	case 56: case 58:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(1 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(5 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(22 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(34 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(38 == iIndex) pPiece->m_iPermaPortSide = 1;
		
		break;
	//random tiles
	case 32: case 40: case 43: case 44: case 49: case 50: case 53: case 54:
	case 55: case 59: case 60: case 61: case 64: case 65: case 66: case 69:
	case 70: case 71: case 75: case 76: case 77: case 78: case 79: case 80:
	case 81:
		//set type
		pPiece->m_iPieceType = MAP_HIDDEN;

		//set piece type
		pPiece->m_iPermaType = RES_HIDDEN;

		//initial placement is false
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	//standard tiles
	default:
		pPiece->m_iPieceType = MAP_SEA;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//set type
		pPiece->m_iPermaType = RES_OCEAN;

		break;
	}

	//land tiles
	switch(iIndex)
	{
	case 11:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 5;
		break;
	case 12:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 6;
		break;
	case 13:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 14:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 3;
		break;
	case 15:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 16:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 17:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 18:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 6;
		break;
	case 19:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 11;
		break;
	case 23:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 10;
		break;
	case 24:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 25:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 26:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 9;
		break;
	case 27:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 28:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 35:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 36:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 37:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 3;
		break;
	case 46:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 47:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 10;
		break;
	case 57:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 74:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 4;
		break;
	case 82:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 8;
		break;
	default:
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
