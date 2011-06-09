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
// creates the Seafarers: Great Crossing (IV) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createGreatCrossingIV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_GREAT_CROSSING_IV;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 13 points
	pMap->m_iPoints = 13;

	//map name
	pMap->m_strMapName = "The Great Crossing (IV)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map uses trade routes
	pMap->m_bUsesTradeRoutes = TRUE;

	//there are no random tiles
	pMap->m_nRandomTiles = 0;

	//ports
	pMap->m_nPorts = 11;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (four player only)
	pMap->m_iMinPlayers = 4;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 22;
	pMap->m_iSeaTiles = 45;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_PORT3] = 6;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//create the map
	createMap(MAP_BORDERS_X, pMap, coordsGreatCrossingIV);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 90.;
}

/////////////////////////////////////////////////////////////////////////////
// Great Crossing (IV) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsGreatCrossingIV(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	//left island
	case 8: case 9: case 16: case 17: case 23: case 24: case 31: case 38: 
	case 39: case 46: case 53:
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
	case 13: case 20: case 27: case 28: case 35: case 42: case 43: case 49: 
	case 50: case 57: case 58:
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
	case 0: case 10: case 15: case 19: case 21: case 30: case 36: case 45: case 47:
	case 56: case 66:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(15 == iIndex) pPiece->m_iPermaPortSide = 4;
		if(21 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(30 == iIndex) pPiece->m_iPermaPortSide = 4;
		if(45 == iIndex) pPiece->m_iPermaPortSide = 4;

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
		pPiece->m_iPermaRoll = 2;
		break;
	case 9:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 4;
		break;
	case 13:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 16:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 6;
		break;
	case 17:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 5;
		break;
	case 20:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 6;
		break;
	case 23:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 24:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 10;
		break;
	case 27:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 2;
		break;
	case 28:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 4;
		break;
	case 31:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 35:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 5;
		break;
	case 38:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 3;
		break;
	case 39:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 11;
		break;
	case 42:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 3;
		break;
	case 43:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 9;
		break;
	case 46:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 9;
		break;
	case 49:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 10;
		break;
	case 50:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 53:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 12;
		break;
	case 57:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 11;
		break;
	case 58:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	default:
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
