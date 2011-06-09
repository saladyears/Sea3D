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
// creates the Seafarers: Into The Desert (III) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createIntoDesertIII(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_INTO_DESERT_III;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Into The Desert (III)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map uses desert buffers
	pMap->m_bDesertBuffer = TRUE;

	//there are no random tiles
	pMap->m_nRandomTiles = 0;

	//ports
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (three player only)
	pMap->m_iMinPlayers = 3;
	pMap->m_iMaxPlayers = 3;

	//number of land/sea tiles
	pMap->m_iLandTiles = 27;
	pMap->m_iSeaTiles = 40;

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
	pMap->m_iRandomRes[RES_PORT3] = 4;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_X, pMap, coordsIntoDesertIII);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 90.;
}

/////////////////////////////////////////////////////////////////////////////
// Into The Desert (III) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsIntoDesertIII(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 8: case 16: case 17: case 23: case 24: case 25: case 26: case 31: 
	case 32: case 33: case 34: case 39: case 40: case 41:
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
	case 9: case 18: case 19: case 20:
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
		break;
	//islands
	case 11: case 12: case 13: case 43: case 50: case 53: case 54: case 56: 
	case 58:
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
	case 1: case 10: case 15: case 28: case 30: case 38: case 42: case 47: case 48:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(1 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(15 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(30 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(47 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(48 == iIndex) pPiece->m_iPermaPortSide = 5;
		
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
	case 8:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 4;
		break;
	case 9:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 11:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 12:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 13:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 16:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 8;
		break;
	case 17:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 18:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 19:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 20:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 23:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 5;
		break;
	case 24:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 25:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 26:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 31:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 32:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 6;
		break;
	case 33:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 34:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 10;
		break;
	case 39:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 40:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 41:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 43:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 50:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 53:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 54:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 56:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 10;
		break;
	case 58:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif		//DEBUG_BUILD_MAPS