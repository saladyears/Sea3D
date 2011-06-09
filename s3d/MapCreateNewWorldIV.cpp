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
// creates the Seafarers: A New World (IV) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createNewWorldIV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_NEW_WORLD_IV;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "A New World (IV)";
	pMap->m_strMapCreator = "Jason Fugate";

	//this map lets players assign the ports
	pMap->m_bAssignsPorts = TRUE;

	//ports
	pMap->m_nPorts = 10;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (four player only)
	pMap->m_iMinPlayers = 4;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 23;
	pMap->m_iSeaTiles = 53;

	//set random amounts
	pMap->m_nRandomTiles = 46;
	pMap->m_nRandomChits = 23;
	
	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//1 extra point
	pMap->m_nBonus1 = 1;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 5;
	pMap->m_iRandomRes[RES_WHEAT] = 5;
	pMap->m_iRandomRes[RES_ORE] = 4;
	pMap->m_iRandomRes[RES_CLAY] = 4;
	pMap->m_iRandomRes[RES_SHEEP] = 5;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 0;
	pMap->m_iRandomRes[RES_OCEAN] = 23;
	pMap->m_iRandomRes[RES_PORT3] = 5;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nRandomChit[0] = 1;
	pMap->m_nRandomChit[1] = 3;
	pMap->m_nRandomChit[2] = 3;
	pMap->m_nRandomChit[3] = 3;
	pMap->m_nRandomChit[4] = 2;
	pMap->m_nRandomChit[5] = 0;
	pMap->m_nRandomChit[6] = 2;
	pMap->m_nRandomChit[7] = 3;
	pMap->m_nRandomChit[8] = 3;
	pMap->m_nRandomChit[9] = 2;
	pMap->m_nRandomChit[10] = 1;

	//create the map
	createMap(MAP_BORDERS_XX, pMap, coordsNewWorldIV);
	
	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 95.;
}

/////////////////////////////////////////////////////////////////////////////
// A New World (IV) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsNewWorldIV(int iIndex, CMapPiece *pPiece)
{
	switch(iIndex)
	{
	//skip borders
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: 
	case 16: case 24: case 33: case 41: case 50: case 58: case 67: case 75: 
	case 74: case 73: case 72: case 71:	case 70: case 69: case 68: case 59: 
	case 51: case 42: case 34: case 25: case 17: case 8: 
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