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
// creates the Seafarers: New World (V) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createNewWorldV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_NEW_WORLD_V;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 12 points
	pMap->m_iPoints = 12;

	//map name
	pMap->m_strMapName = "A New World (V)";
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

	//this map lets players assign the ports
	pMap->m_bAssignsPorts = TRUE;

	//ports
	pMap->m_nPorts = 10;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_SEA_CHIPS;	

	//min/max players (five player only)
	pMap->m_iMinPlayers = 5;
	pMap->m_iMaxPlayers = 5;

	//number of land/sea tiles
	pMap->m_iLandTiles = 27;
	pMap->m_iSeaTiles = 58;

	//set random amounts
	pMap->m_nRandomTiles = 53;
	pMap->m_nRandomChits = 27;
	
	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//1 extra point
	pMap->m_nBonus1 = 1;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 6;
	pMap->m_iRandomRes[RES_WHEAT] = 5;
	pMap->m_iRandomRes[RES_ORE] = 5;
	pMap->m_iRandomRes[RES_CLAY] = 5;
	pMap->m_iRandomRes[RES_SHEEP] = 6;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 0;
	pMap->m_iRandomRes[RES_OCEAN] = 26;
	pMap->m_iRandomRes[RES_PORT3] = 5;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set random chit amounts (add 2 to get the actual value)
	pMap->m_nRandomChit[0] = 2;
	pMap->m_nRandomChit[1] = 3;
	pMap->m_nRandomChit[2] = 3;
	pMap->m_nRandomChit[3] = 3;
	pMap->m_nRandomChit[4] = 3;
	pMap->m_nRandomChit[5] = 0;
	pMap->m_nRandomChit[6] = 2;
	pMap->m_nRandomChit[7] = 4;
	pMap->m_nRandomChit[8] = 3;
	pMap->m_nRandomChit[9] = 3;
	pMap->m_nRandomChit[10] = 1;

	//create the map
	createMap(MAP_BORDERS_XX_X, pMap, coordsNewWorldV);
	
	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 100.;
}

/////////////////////////////////////////////////////////////////////////////
// New World (V) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsNewWorldV(int iIndex, CMapPiece *pPiece)
{
	switch(iIndex)
	{
	//skip borders
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8:
	case 18: case 27: case 37: case 46: case 56: case 65: case 75: case 84:
	case 83: case 82: case 81: case 80:	case 79: case 78: case 77: case 76:
	case 66: case 57: case 47: case 38: case 28: case 19: case 9:
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

#endif	//DEBUG_BUILD_MAPS
