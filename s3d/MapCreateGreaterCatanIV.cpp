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
// creates the Seafarers: Greater Catan (IV) map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createGreaterCatanIV(CS3DMap *pMap)
{
	//identifier
	pMap->m_uiID = MAP_SEA_GREATER_CATAN_IV;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SEAFARERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//play to 18 points
	pMap->m_iPoints = 18;

	//map name
	pMap->m_strMapName = "Greater Catan (IV)";
	pMap->m_strMapCreator = "Jason Fugate";

	//uses chip laying algorithm
	pMap->m_bUseChipMethod = TRUE;

	//uses chitless tiles
	pMap->m_bHasChitlessTiles = TRUE;

	//there are 19 random tiles
	pMap->m_nRandomTiles = 19;

	//ports
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_34_CHIPS | MAP_USES_SEA_CHIPS;	

	//min/max players (four player only)
	pMap->m_iMinPlayers = 4;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 37;
	pMap->m_iSeaTiles = 57;

	//15 ships in this scenario
	pMap->m_iStockShips = 15;

	//extra cities in this scenario
	pMap->m_iStockCities = 8;

	//set the roll places
	pMap->m_iRollPlaces[0] = 35;
	pMap->m_iRollPlaces[1] = 55;
	pMap->m_iRollPlaces[2] = 77;
	pMap->m_iRollPlaces[3] = 79;
	pMap->m_iRollPlaces[4] = 59;
	pMap->m_iRollPlaces[5] = 37;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 4;
	pMap->m_iRandomRes[RES_WHEAT] = 4;
	pMap->m_iRandomRes[RES_ORE] = 3;
	pMap->m_iRandomRes[RES_CLAY] = 3;
	pMap->m_iRandomRes[RES_SHEEP] = 4;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 1;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 4;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 1;

	//set up chitless tiles
	pMap->m_iChitlessTiles = 18;

	//set the amounts
	pMap->m_iChitlessRes[RES_TIMBER] = 4;
	pMap->m_iChitlessRes[RES_WHEAT] = 4;
	pMap->m_iChitlessRes[RES_ORE] = 3;
	pMap->m_iChitlessRes[RES_CLAY] = 3;
	pMap->m_iChitlessRes[RES_SHEEP] = 4;
	pMap->m_iChitlessRes[RES_GOLD] = 0;
	pMap->m_iChitlessRes[RES_DESERT] = 0;

	//set the chitless chits
	pMap->m_iChitlessChits = 7;

	//set the amounts
	pMap->m_nChitlessChit[0] = 1;
	pMap->m_nChitlessChit[1] = 1;
	pMap->m_nChitlessChit[2] = 1;
	pMap->m_nChitlessChit[3] = 1;
	pMap->m_nChitlessChit[4] = 0;
	pMap->m_nChitlessChit[5] = 0;
	pMap->m_nChitlessChit[6] = 0;
	pMap->m_nChitlessChit[7] = 1;
	pMap->m_nChitlessChit[8] = 1;
	pMap->m_nChitlessChit[9] = 1;
	pMap->m_nChitlessChit[10] = 0;

	//create the map
	createMap(MAP_BORDERS_XX_XX, pMap, coordsGreaterCatanIV);

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 105.;

	//set initial rotation
	pMap->m_dInitialRot = -30.;
}

/////////////////////////////////////////////////////////////////////////////
// Greater Catan (IV) coordinates
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsGreaterCatanIV(int iIndex, CMapPiece *pPiece)
{
	//set the tile type
	switch(iIndex)
	{
	case 35: case 36: case 37: case 45: case 46: case 47: case 48: case 55:
	case 56: case 57: case 58: case 59: case 66: case 67: case 68: case 69:
	case 77: case 78: case 79:
		//set land type
		pPiece->m_iPieceType = MAP_RANDOM;

		//set the initial placement to TRUE for starting land pieces
		pPiece->m_bInitial = TRUE;

		//tell it to use sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_34_CHIPS;

		break;
	//ports
	case 25: case 27: case 34: case 49: case 54: case 70: case 76: case 88:
	case 90:
		//set port
		pPiece->m_iPieceType = MAP_PORT;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;
		
		break;
	//chitless islands
	case 11: case 12: case 14: case 15: case 17: case 18: case 19: case 22:
	case 28: case 29: case 32: case 40: case 53: case 61: case 64: case 74:
	case 75: case 82:
		//tile type
		pPiece->m_iPieceType = MAP_CHITLESS;

		//disallow initial build
		pPiece->m_bInitial = FALSE;

		//allow secondary build
		pPiece->m_bSecondary = TRUE;

		//set sea chips
		pPiece->m_iPermaRollTexType = MAP_USES_SEA_CHIPS;

		break;
	default:
		//set ocean defaults
		pPiece->m_iPieceType = MAP_SEA;

		//set ocean
		pPiece->m_iPermaType = RES_OCEAN;

		//set the initial placement to FALSE for all other pieces
		pPiece->m_bInitial = FALSE;

		break;
	}
}

#endif	//DEBUG_BUILD_MAPS
