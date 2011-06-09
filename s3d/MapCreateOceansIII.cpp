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
// creates the Seafarers: Oceans (III) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createOceansIII(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_OCEANS_III;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "Oceans (III)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map uses hidden tiles
	pMap->m_bHasHiddenTiles = TRUE;

	//ports
	pMap->m_nPorts = 8;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (three player only)
	pMap->m_iMinPlayers = 3;
	pMap->m_iMaxPlayers = 3;

	//number of land/sea tiles
	pMap->m_iLandTiles = 18;
	pMap->m_iSeaTiles = 40;
	pMap->m_iHiddenTiles = 14;
	pMap->m_iHiddenChits = 6;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//no extra points

	//set the random resource amounts
	pMap->m_iRandomRes[RES_PORT3] = 3;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set the hidden resource amounts
	pMap->m_iHiddenRes[RES_TIMBER] = 1;
	pMap->m_iHiddenRes[RES_WHEAT] = 1;
	pMap->m_iHiddenRes[RES_ORE] = 2;
	pMap->m_iHiddenRes[RES_CLAY] = 1;
	pMap->m_iHiddenRes[RES_SHEEP] = 1;
	pMap->m_iHiddenRes[RES_GOLD] = 0;
	pMap->m_iHiddenRes[RES_DESERT] = 0;
	pMap->m_iHiddenRes[RES_OCEAN] = 8;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nHiddenChit[0] = 0;
	pMap->m_nHiddenChit[1] = 1;
	pMap->m_nHiddenChit[2] = 0;
	pMap->m_nHiddenChit[3] = 1;
	pMap->m_nHiddenChit[4] = 0;
	pMap->m_nHiddenChit[5] = 0;
	pMap->m_nHiddenChit[6] = 1;
	pMap->m_nHiddenChit[7] = 1;
	pMap->m_nHiddenChit[8] = 0;
	pMap->m_nHiddenChit[9] = 1;
	pMap->m_nHiddenChit[10] = 1;

	//create the map
	createMap(MAP_BORDERS_NO_X, pMap, coordsOceansIII);
	
	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 90.;
}

/////////////////////////////////////////////////////////////////////////////
// Four Islands (III) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsOceansIII(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 7: case 8: case 9: case 10: case 11: case 15: case 16: case 17:
	case 23: case 24: case 30: case 46:
		//set land type
		pPiece->m_iPieceType = MAP_LAND;

		//set the initial placement to TRUE for all land pieces (except the gold)
		if(46 == iIndex)
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
	case 1: case 3: case 5: case 13: case 18: case 21: case 29: case 37:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		//special port sides
		if(3 == iIndex) pPiece->m_iPermaPortSide = 2;
		if(13 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(18 == iIndex) pPiece->m_iPermaPortSide = 0;
		if(21 == iIndex) pPiece->m_iPermaPortSide = 5;
		if(29 == iIndex) pPiece->m_iPermaPortSide = 4;
		if(37 == iIndex) pPiece->m_iPermaPortSide = 0;
		
		break;
	//random tiles
	case 20: case 27: case 28: case 33: case 34: case 35: case 40: case 41:
	case 42: case 43: case 47: case 48: case 49: case 50:
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
	case 7:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 3;
		break;
	case 8:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 6;
		break;
	case 9:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 4;
		break;
	case 10:
		pPiece->m_iPermaType = RES_CLAY;
		pPiece->m_iPermaRoll = 10;
		break;
	case 11:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 9;
		break;
	case 15:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 5;
		break;
	case 16:
		pPiece->m_iPermaType = RES_SHEEP;
		pPiece->m_iPermaRoll = 6;
		break;
	case 17:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 2;
		break;
	case 23:
		pPiece->m_iPermaType = RES_ORE;
		pPiece->m_iPermaRoll = 10;
		break;
	case 24:
		pPiece->m_iPermaType = RES_TIMBER;
		pPiece->m_iPermaRoll = 8;
		break;
	case 30:
		pPiece->m_iPermaType = RES_WHEAT;
		pPiece->m_iPermaRoll = 11;
		break;
	case 46:
		pPiece->m_iPermaType = RES_GOLD;
		pPiece->m_iPermaRoll = 4;
		break;
	default:
		break;
	}
}

#endif		//DEBUG_BUILD_MAPS