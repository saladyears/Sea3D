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
// creates the Seafarers: A New World (III) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createNewWorldIII(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_NEW_WORLD_III;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "A New World (III)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map lets players assign the ports
	pMap->m_bAssignsPorts = TRUE;

	//ports
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (three player only)
	pMap->m_iMinPlayers = 3;
	pMap->m_iMaxPlayers = 3;

	//number of land/sea tiles
	pMap->m_iLandTiles = 20;
	pMap->m_iSeaTiles = 47;

	//set random amounts
	pMap->m_nRandomTiles = 39;
	pMap->m_nRandomChits = 20;
	
	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//1 extra point
	pMap->m_nBonus1 = 1;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 4;
	pMap->m_iRandomRes[RES_WHEAT] = 4;
	pMap->m_iRandomRes[RES_ORE] = 4;
	pMap->m_iRandomRes[RES_CLAY] = 4;
	pMap->m_iRandomRes[RES_SHEEP] = 4;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 0;
	pMap->m_iRandomRes[RES_OCEAN] = 19;
	pMap->m_iRandomRes[RES_PORT3] = 4;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nRandomChit[0] = 1;
	pMap->m_nRandomChit[1] = 2;
	pMap->m_nRandomChit[2] = 2;
	pMap->m_nRandomChit[3] = 2;
	pMap->m_nRandomChit[4] = 2;
	pMap->m_nRandomChit[5] = 0;
	pMap->m_nRandomChit[6] = 2;
	pMap->m_nRandomChit[7] = 3;
	pMap->m_nRandomChit[8] = 3;
	pMap->m_nRandomChit[9] = 2;
	pMap->m_nRandomChit[10] = 1;

	//create the map
	createMap(MAP_BORDERS_X, pMap, coordsNewWorldIII);
	
	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 90.;
}

/////////////////////////////////////////////////////////////////////////////
// A New World (III) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsNewWorldIII(int iIndex, CMapPiece *pPiece)
{
	switch(iIndex)
	{
	//skip borders
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 14: 
	case 21: case 29: case 36: case 44: case 51: case 59: case 66: case 65: 
	case 64: case 63: case 62: case 61:	case 60: case 52: case 45: case 37: 
	case 30: case 22: case 15: case 7: 
		pPiece->m_iPieceType = MAP_SEA;

		//set the initial placement to FALSE for border
		pPiece->m_bInitial = FALSE;

		//set type
		pPiece->m_iPermaType = RES_OCEAN;
		break;
	default:
		//all tiles are random
		pPiece->m_iPieceType = MAP_RANDOM;

		//all tiles can be placed on initially
		pPiece->m_bInitial = TRUE;

		//all tiles can be islands
		pPiece->m_bSecondary = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;
		break;
	}
}


#endif		//DEBUG_BUILD_MAPS