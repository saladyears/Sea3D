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
// creates the Seafarers: Into The Desert (IV) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createIntoDesertIV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_INTO_DESERT_IV;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Into The Desert (IV)";
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
	pMap->m_iMinPlayers = 4;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 30;
	pMap->m_iSeaTiles = 46;

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
	createMap(MAP_BORDERS_XX, pMap, coordsIntoDesertIV);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 90.;
}

/////////////////////////////////////////////////////////////////////////////
// Into The Desert (IV) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsIntoDesertIV(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 9: case 18: case 19: case 26: case 27: case 28: case 29: case 30: 
	case 35: case 36: case 37: case 38: case 44: case 45: case 46:
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
	case 10: case 20: case 21: case 22:
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces
		pPiece->m_bInitial = FALSE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
		break;
	//islands
	case 12: case 13: case 14: case 15: case 49: case 57: case 60: case 61: 
	case 63: case 65: case 66:
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
	case 1: case 11: case 17: case 31: case 34: case 43: case 47: case 53: case 54:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(1 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(11 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(17 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(31 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(34 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(53 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(54 == iIndex) pPiece->m_iPermaPortSide = 5;

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
	case 9:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 4;
		break;
	case 10:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 12:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 13:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 14:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 15:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 11;
		break;
	case 18:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 8;
		break;
	case 19:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 20:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 21:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 22:
		pPiece->m_iPermaType = RES_DESERT;
		break;
	case 26:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 5;
		break;
	case 27:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 28:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 4;
		break;
	case 29:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 30:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 6;
		break;
	case 35:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 9;
		break;
	case 36:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 6;
		break;
	case 37:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 38:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 44:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 45:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 46:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 49:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 57:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 60:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 6;
		break;
	case 61:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 63:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 10;
		break;
	case 65:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 3;
		break;
	case 66:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS