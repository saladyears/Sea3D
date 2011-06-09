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
// creates the Seafarers: Six Islands (VI) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createSixIslandsVI(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_SIX_ISLANDS_VI;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Six Islands (VI)";
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

	//this map uses the initial ship requirements
	pMap->m_bRequiresInitialShips = TRUE;

	//there are no random tiles
	pMap->m_nRandomTiles = 0;

	//ports
	pMap->m_nPorts = 11;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (six player only)
	pMap->m_iMinPlayers = 6;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 32;
	pMap->m_iSeaTiles = 62;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//set the extra points
	pMap->m_nBonus1 = 1;
	pMap->m_nBonus2 = 2;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 0;
	pMap->m_iRandomRes[RES_WHEAT] = 0;
	pMap->m_iRandomRes[RES_ORE] = 0;
	pMap->m_iRandomRes[RES_CLAY] = 0;
	pMap->m_iRandomRes[RES_SHEEP] = 0;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 0;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 6;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_XX_XX, pMap, coordsSixIslandsVI);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 105.;
}

/////////////////////////////////////////////////////////////////////////////
// Six Islands (VI) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsSixIslandsVI(int iIndex, CMapPiece *pPiece)
{
		//set the tile type
	switch(iIndex)
	{
	case 11: case 12: case 13: case 15: case 18: case 19: case 22: case 23:
	case 25: case 26: case 28: case 29: case 32: case 33: case 36: case 50:
	case 54: case 57: case 58: case 60: case 61: case 64: case 65: case 67:
	case 68: case 70: case 71: case 74: case 75: case 78: case 81: case 82:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = TRUE;

		//allow secondary placement for all islands
		pPiece->m_bSecondary = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	//ports
	case 1: case 5: case 8: case 34: case 38: case 43: case 63: case 72:
	case 76: case 88: case 91:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(8 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(72 == iIndex) pPiece->m_iPermaPortSide = 1;
		
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
		pPiece->m_iPermaRoll = 2;
		break;
	case 12:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 4;
		break;
	case 13:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 15:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 6;
		break;
	case 18:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 19:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 6;
		break;
	case 22:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 23:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 10;
		break;
	case 25:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 11;
		break;
	case 26:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 28:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 3;
		break;
	case 29:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 32:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 3;
		break;
	case 33:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 36:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 50:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 9;
		break;
	case 54:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 4;
		break;
	case 57:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 58:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 60:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 61:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 4;
		break;
	case 64:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 65:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 12;
		break;
	case 67:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 68:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 3;
		break;
	case 70:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 71:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 11;
		break;
	case 74:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 5;
		break;
	case 75:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 78:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 2;
		break;
	case 81:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 9;
		break;
	case 82:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 12;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
