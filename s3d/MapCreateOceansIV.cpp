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
// creates the Seafarers: Oceans (IV) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createOceansIV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_OCEANS_IV;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Oceans (IV)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map uses hidden tiles
	pMap->m_bHasHiddenTiles = TRUE;

	//ports
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (four player only)
	pMap->m_iMinPlayers = 4;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 25;
	pMap->m_iSeaTiles = 51;
	pMap->m_iHiddenTiles = 22;
	pMap->m_iHiddenChits = 7;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//no extra points

	//set the random resource amounts
	pMap->m_iRandomRes[RES_PORT3] = 4;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set the hidden resource amounts
	pMap->m_iHiddenRes[RES_TIMBER] = 1;
	pMap->m_iHiddenRes[RES_WHEAT] = 2;
	pMap->m_iHiddenRes[RES_ORE] = 1;
	pMap->m_iHiddenRes[RES_CLAY] = 1;
	pMap->m_iHiddenRes[RES_SHEEP] = 2;
	pMap->m_iHiddenRes[RES_GOLD] = 0;
	pMap->m_iHiddenRes[RES_DESERT] = 1;
	pMap->m_iHiddenRes[RES_OCEAN] = 14;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nHiddenChit[0] = 0;
	pMap->m_nHiddenChit[1] = 1;
	pMap->m_nHiddenChit[2] = 1;
	pMap->m_nHiddenChit[3] = 2;
	pMap->m_nHiddenChit[4] = 1;
	pMap->m_nHiddenChit[5] = 0;
	pMap->m_nHiddenChit[6] = 0;
	pMap->m_nHiddenChit[7] = 1;
	pMap->m_nHiddenChit[8] = 1;
	pMap->m_nHiddenChit[9] = 0;
	pMap->m_nHiddenChit[10] = 0;

	//create the map
	createMap(MAP_BORDERS_XX, pMap, coordsOceansIV);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 95.;
}

/////////////////////////////////////////////////////////////////////////////
// Four Islands (IV) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsOceansIV(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 18:
	case 19: case 20: case 21: case 22: case 23: case 26: case 32: case 60:
	case 66:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces (except the gold)
		if(60 == iIndex || 66 == iIndex)
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
	case 1: case 4: case 6: case 17: case 24: case 27: case 30: case 34:
	case 41:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(4 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(6 == iIndex) pPiece->m_iPermaPortSide = 3;
		if(27 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(30 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(34 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(41 == iIndex) pPiece->m_iPermaPortSide = 0;
		
		break;
	//random tiles
	case 36: case 37: case 38: case 39: case 43: case 44: case 45: case 46:
	case 47: case 48: case 49: case 52: case 53: case 54: case 55: case 56:
	case 57: case 61: case 62: case 63: case 64: case 65:
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
	case 9:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 4;
		break;
	case 10:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 3;
		break;
	case 11:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 8;
		break;
	case 12:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 11;
		break;
	case 13:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 12;
		break;
	case 14:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 4;
		break;
	case 15:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 3;
		break;
	case 18:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 6;
		break;
	case 19:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 9;
		break;
	case 20:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 10;
		break;
	case 21:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 6;
		break;
	case 22:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 9;
		break;
	case 23:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 8;
		break;
	case 26:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 2;
		break;
	case 32:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 5;
		break;
	case 60:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 10;
		break;
	case 66:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 11;
		break;
	default:
		break;
	}
}


#endif		//DEBUG_BUILD_MAPS