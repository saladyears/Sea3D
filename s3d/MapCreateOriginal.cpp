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
// creates the basic Settlers of Catan 3/4 player map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createOriginal34(CS3DMap *pMap)
{
	int i, j;

	//identifier
	pMap->m_uiID = MAP_SETTLERS_34;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SETTLERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//map name
	pMap->m_strMapName = "Standard Settlers";
	pMap->m_strMapCreator = "Jason Fugate";

	//uses chip laying algorithm
	pMap->m_bUseChipMethod = TRUE;

	//random tiles used
	pMap->m_nRandomTiles = 19;

	//ports in this map
	pMap->m_nPorts = 9;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_34_CHIPS;

	//min/max players
	pMap->m_iMinPlayers = 3;
	pMap->m_iMaxPlayers = 4;

	//number of land/sea tiles
	pMap->m_iLandTiles = 19;
	pMap->m_iSeaTiles = 18;

	//default allowed cards in hand

	//default bank allocations

	//default dev cards

	//no ships in this scenario
	pMap->m_iStockShips = 0;

	//set the roll places
	pMap->m_iRollPlaces[0] = 8;
	pMap->m_iRollPlaces[1] = 10;
	pMap->m_iRollPlaces[2] = 12;
	pMap->m_iRollPlaces[3] = 14;
	pMap->m_iRollPlaces[4] = 16;
	pMap->m_iRollPlaces[5] = 18;

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

	//resize the map pieces
	pMap->m_mapPieces.resize(37);

	//set the map pieces
	for(i = 0; i < 37; i++)
	{
		//set the piece attributes
		pMap->m_mapPieces[i].m_iIndex = i;

		//what kind of piece
		if(i < 19)
		{
			pMap->m_mapPieces[i].m_iPieceType = MAP_RANDOM;
		}
		else
		{
			pMap->m_mapPieces[i].m_iPieceType = MAP_SEA;
			pMap->m_mapPieces[i].m_bBorder = TRUE;
		}

		//set the coords
		coordsOriginal(i, FALSE, &(pMap->m_mapPieces[i]));
	}

	//reset the -1 values that are for 5-6 player maps
	for(i = 19; i < 37; i++)
	{
		for(j = 0; j < 6; j++)
		{
			if(37 <= pMap->m_mapPieces[i].m_iSides[j])
			{
				pMap->m_mapPieces[i].m_iSides[j] = -1;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// create the 5/6 player map
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createOriginal56(CS3DMap *pMap)
{
	int i;

	//identifier
	pMap->m_uiID = MAP_SETTLERS_56;

	//last time edited
	pMap->m_timeEdit = COleDateTime::GetCurrentTime();

	//type of game
	pMap->m_iMode = MAP_SETTLERS;

	//official map
	pMap->m_bOfficial = TRUE;

	//map name
	pMap->m_strMapName = "Standard Settlers";
	pMap->m_strMapCreator = "Jason Fugate";

	//default number of points

	//start out zoomed out a little farther than normal
	pMap->m_dInitialDepth = 100.;

	//uses chip laying algorithm
	pMap->m_bUseChipMethod = TRUE;

	//random tiles used
	pMap->m_nRandomTiles = 30;

	//ports
	pMap->m_nPorts = 11;

	//chip texture file used
	pMap->m_iChipsUsed = MAP_USES_56_CHIPS;

	//min/max players
	pMap->m_iMinPlayers = 5;
	pMap->m_iMaxPlayers = 6;

	//number of land/sea tiles
	pMap->m_iLandTiles = 30;
	pMap->m_iSeaTiles = 22;

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

	//no ships in this scenario
	pMap->m_iStockShips = 0;

	//set the roll places
	pMap->m_iRollPlaces[0] = 21;
	pMap->m_iRollPlaces[1] = 24;
	pMap->m_iRollPlaces[2] = 27;
	pMap->m_iRollPlaces[3] = 29;
	pMap->m_iRollPlaces[4] = 16;
	pMap->m_iRollPlaces[5] = 19;

	//set the random resource amounts
	pMap->m_iRandomRes[RES_TIMBER] = 6;
	pMap->m_iRandomRes[RES_WHEAT] = 6;
	pMap->m_iRandomRes[RES_ORE] = 5;
	pMap->m_iRandomRes[RES_CLAY] = 5;
	pMap->m_iRandomRes[RES_SHEEP] = 6;
	pMap->m_iRandomRes[RES_GOLD] = 0;
	pMap->m_iRandomRes[RES_DESERT] = 2;
	pMap->m_iRandomRes[RES_OCEAN] = 0;
	pMap->m_iRandomRes[RES_PORT3] = 5;
	pMap->m_iRandomRes[RES_PORTTIMBER] = 1;
	pMap->m_iRandomRes[RES_PORTWHEAT] = 1;
	pMap->m_iRandomRes[RES_PORTORE] = 1;
	pMap->m_iRandomRes[RES_PORTCLAY] = 1;
	pMap->m_iRandomRes[RES_PORTSHEEP] = 2;

	//resize the map pieces
	pMap->m_mapPieces.resize(52);

	//set the map pieces
	for(i = 0; i < 52; i++)
	{
		//set the piece attributes
		pMap->m_mapPieces[i].m_iIndex = i;

		//what kind of piece
		if(i < 30)
		{
			pMap->m_mapPieces[i].m_iPieceType = MAP_RANDOM;
		}
		else
		{
			pMap->m_mapPieces[i].m_iPieceType = MAP_SEA;
			pMap->m_mapPieces[i].m_bBorder = TRUE;
		}

		//set the coords
		coordsOriginal(i, TRUE, &(pMap->m_mapPieces[i]));
	}
}

/////////////////////////////////////////////////////////////////////////////
// set openGL coordinates and sides
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coordsOriginal(int iIndex, BOOL b56, CMapPiece *pPiece)
{
	pPiece->m_glCoords[1] = 0.;

	switch(iIndex)
	{
	case 0: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = 0.00;		break;
	case 1: pPiece->m_glCoords[0] = -((X / 2) + X);			pPiece->m_glCoords[2] = -Z;			break;
	case 2: pPiece->m_glCoords[0] = -((X / 2) + X);			pPiece->m_glCoords[2] = Z;			break;
	case 3: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = Z * 2;		break;
	case 4: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = Z;			break;
	case 5: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = -Z;			break;
	case 6: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = -Z * 2;		break;
	case 7: pPiece->m_glCoords[0] = -((X / 2) + X);			pPiece->m_glCoords[2] = -Z * 3;		break;
	case 8: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;		pPiece->m_glCoords[2] = -Z * 2;		break;
	case 9: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;		pPiece->m_glCoords[2] = 0.00;		break;
	case 10: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;	pPiece->m_glCoords[2] = Z * 2;		break;
	case 11: pPiece->m_glCoords[0] = -((X / 2) + X);		pPiece->m_glCoords[2] = Z * 3;		break;
	case 12: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = Z * 4;		break;
	case 13: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = Z * 3;		break;
	case 14: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = Z * 2;		break;
	case 15: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = 0.00;		break;
	case 16: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = -Z * 2;		break;
	case 17: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = -Z * 3;		break;
	case 18: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = -Z * 4;		break;
	case 19: pPiece->m_glCoords[0] = -((X / 2) + X);		pPiece->m_glCoords[2] = -Z * 5;		break;
	case 20: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;	pPiece->m_glCoords[2] = -Z * 4;		break;
	case 21: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = -Z * 3;		break;
	case 22: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = -Z;			break;
	case 23: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = Z;			break;
	case 24: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = Z * 3;		break;
	case 25: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;	pPiece->m_glCoords[2] = Z * 4;		break;
	case 26: pPiece->m_glCoords[0] = -((X / 2) + X);		pPiece->m_glCoords[2] = Z * 5;		break;
	case 27: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = Z * 6;		break;
	case 28: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = Z * 5;		break;
	case 29: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = Z * 4;		break;
	case 30: pPiece->m_glCoords[0] = ((X / 2) + X) * 3;		pPiece->m_glCoords[2] = Z * 3;		break;
	case 31: pPiece->m_glCoords[0] = ((X / 2) + X) * 3;		pPiece->m_glCoords[2] = Z;			break;
	case 32: pPiece->m_glCoords[0] = ((X / 2) + X) * 3;		pPiece->m_glCoords[2] = -Z;			break;
	case 33: pPiece->m_glCoords[0] = ((X / 2) + X) * 3;		pPiece->m_glCoords[2] = -Z * 3;		break;
	case 34: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = -Z * 4;		break;
	case 35: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = -Z * 5;		break;
	case 36: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = -Z * 6;		break;

	//5-6 player
	case 37: pPiece->m_glCoords[0] = -((X / 2) + X);		pPiece->m_glCoords[2] = -Z * 7;		break;
	case 38: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;	pPiece->m_glCoords[2] = -Z * 6;		break;
	case 39: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = -Z * 5;		break;
	case 40: pPiece->m_glCoords[0] = -((X / 2) + X) * 4;	pPiece->m_glCoords[2] = -Z * 4;		break;
	case 41: pPiece->m_glCoords[0] = -((X / 2) + X) * 4;	pPiece->m_glCoords[2] = -Z * 2;		break;
	case 42: pPiece->m_glCoords[0] = -((X / 2) + X) * 4;	pPiece->m_glCoords[2] = 0.00;		break;
	case 43: pPiece->m_glCoords[0] = -((X / 2) + X) * 4;	pPiece->m_glCoords[2] = Z * 2;		break;
	case 44: pPiece->m_glCoords[0] = -((X / 2) + X) * 4;	pPiece->m_glCoords[2] = Z * 4;		break;
	case 45: pPiece->m_glCoords[0] = -((X / 2) + X) * 3;	pPiece->m_glCoords[2] = Z * 5;		break;
	case 46: pPiece->m_glCoords[0] = -((X / 2) + X) * 2;	pPiece->m_glCoords[2] = Z * 6;		break;
	case 47: pPiece->m_glCoords[0] = -((X / 2) + X);		pPiece->m_glCoords[2] = Z * 7;		break;
	case 48: pPiece->m_glCoords[0] = 0.00;					pPiece->m_glCoords[2] = Z * 8;		break;
	case 49: pPiece->m_glCoords[0] = ((X / 2) + X);			pPiece->m_glCoords[2] = Z * 7;		break;
	case 50: pPiece->m_glCoords[0] = ((X / 2) + X) * 2;		pPiece->m_glCoords[2] = Z * 6;		break;
	case 51: pPiece->m_glCoords[0] = ((X / 2) + X) * 3;		pPiece->m_glCoords[2] = Z * 5;		break;
	}

	//set sides
	switch(iIndex)
	{
	case 0: pPiece->m_iSides[0] = 1; pPiece->m_iSides[1] = 2; pPiece->m_iSides[2] = 3; pPiece->m_iSides[3] = 4;
			pPiece->m_iSides[4] = 5; pPiece->m_iSides[5] = 6; break;
	
	case 1: pPiece->m_iSides[0] = 8; pPiece->m_iSides[1] = 9; pPiece->m_iSides[2] = 2; pPiece->m_iSides[3] = 0;
			pPiece->m_iSides[4] = 6; pPiece->m_iSides[5] = 7; break;
	
	case 2: pPiece->m_iSides[0] = 9; pPiece->m_iSides[1] = 10; pPiece->m_iSides[2] = 11; pPiece->m_iSides[3] = 3;
			pPiece->m_iSides[4] = 0; pPiece->m_iSides[5] = 1; break;

	case 3: pPiece->m_iSides[0] = 2; pPiece->m_iSides[1] = 11; pPiece->m_iSides[2] = 12; pPiece->m_iSides[3] = 13;
			pPiece->m_iSides[4] = 4; pPiece->m_iSides[5] = 0; break;

	case 4: pPiece->m_iSides[0] = 0; pPiece->m_iSides[1] = 3; pPiece->m_iSides[2] = 13; pPiece->m_iSides[3] = 14;
			pPiece->m_iSides[4] = 15; pPiece->m_iSides[5] = 5; break;

	case 5: pPiece->m_iSides[0] = 6; pPiece->m_iSides[1] = 0; pPiece->m_iSides[2] = 4; pPiece->m_iSides[3] = 15;
			pPiece->m_iSides[4] = 16; pPiece->m_iSides[5] = 17; break;

	case 6: pPiece->m_iSides[0] = 7; pPiece->m_iSides[1] = 1; pPiece->m_iSides[2] = 0; pPiece->m_iSides[3] = 5;
			pPiece->m_iSides[4] = 17; pPiece->m_iSides[5] = 18; break;

	case 7: pPiece->m_iSides[0] = 20; pPiece->m_iSides[1] = 8; pPiece->m_iSides[2] = 1; pPiece->m_iSides[3] = 6;
			pPiece->m_iSides[4] = 18; pPiece->m_iSides[5] = 19; break;

	case 8: pPiece->m_iSides[0] = 21; pPiece->m_iSides[1] = 22; pPiece->m_iSides[2] = 9; pPiece->m_iSides[3] = 1;
			pPiece->m_iSides[4] = 7; pPiece->m_iSides[5] = 20; break;

	case 9: pPiece->m_iSides[0] = 22; pPiece->m_iSides[1] = 23; pPiece->m_iSides[2] = 10; pPiece->m_iSides[3] = 2;
			pPiece->m_iSides[4] = 1; pPiece->m_iSides[5] = 8; break;

	case 10: pPiece->m_iSides[0] = 23; pPiece->m_iSides[1] = 24; pPiece->m_iSides[2] = 25; pPiece->m_iSides[3] = 11;
			 pPiece->m_iSides[4] = 2; pPiece->m_iSides[5] = 9; break;

	case 11: pPiece->m_iSides[0] = 10; pPiece->m_iSides[1] = 25; pPiece->m_iSides[2] = 26; pPiece->m_iSides[3] = 12;
			 pPiece->m_iSides[4] = 3; pPiece->m_iSides[5] = 2; break;

	case 12: pPiece->m_iSides[0] = 11; pPiece->m_iSides[1] = 26; pPiece->m_iSides[2] = 27; pPiece->m_iSides[3] = 28;
			 pPiece->m_iSides[4] = 13; pPiece->m_iSides[5] = 3; break;

	case 13: pPiece->m_iSides[0] = 3; pPiece->m_iSides[1] = 12; pPiece->m_iSides[2] = 28; pPiece->m_iSides[3] = 29;
			 pPiece->m_iSides[4] = 14; pPiece->m_iSides[5] = 4; break;

	case 14: pPiece->m_iSides[0] = 4; pPiece->m_iSides[1] = 13; pPiece->m_iSides[2] = 29; pPiece->m_iSides[3] = 30;
			 pPiece->m_iSides[4] = 31; pPiece->m_iSides[5] = 15; break;

	case 15: pPiece->m_iSides[0] = 5; pPiece->m_iSides[1] = 4; pPiece->m_iSides[2] = 14; pPiece->m_iSides[3] = 31;
			 pPiece->m_iSides[4] = 32; pPiece->m_iSides[5] = 16; break;

	case 16: pPiece->m_iSides[0] = 17; pPiece->m_iSides[1] = 5; pPiece->m_iSides[2] = 15; pPiece->m_iSides[3] = 32;
			 pPiece->m_iSides[4] = 33; pPiece->m_iSides[5] = 34; break;

	case 17: pPiece->m_iSides[0] = 18; pPiece->m_iSides[1] = 6; pPiece->m_iSides[2] = 5; pPiece->m_iSides[3] = 16;
			 pPiece->m_iSides[4] = 34; pPiece->m_iSides[5] = 35; break;

	case 18: pPiece->m_iSides[0] = 19; pPiece->m_iSides[1] = 7; pPiece->m_iSides[2] = 6; pPiece->m_iSides[3] = 17;
			 pPiece->m_iSides[4] = 35; pPiece->m_iSides[5] = 36; break;

	case 19: pPiece->m_iSides[0] = 38; pPiece->m_iSides[1] = 20; pPiece->m_iSides[2] = 7; pPiece->m_iSides[3] = 18;
			 pPiece->m_iSides[4] = 36; pPiece->m_iSides[5] = 37; break;

	case 20: pPiece->m_iSides[0] = 39; pPiece->m_iSides[1] = 21; pPiece->m_iSides[2] = 8; pPiece->m_iSides[3] = 7;
			 pPiece->m_iSides[4] = 19; pPiece->m_iSides[5] = 38; break;

	case 21: pPiece->m_iSides[0] = 40; pPiece->m_iSides[1] = 41; pPiece->m_iSides[2] = 22; pPiece->m_iSides[3] = 8;
			 pPiece->m_iSides[4] = 20; pPiece->m_iSides[5] = 39; break;

	case 22: pPiece->m_iSides[0] = 41; pPiece->m_iSides[1] = 42; pPiece->m_iSides[2] = 23; pPiece->m_iSides[3] = 9;
			 pPiece->m_iSides[4] = 8; pPiece->m_iSides[5] = 21; break;

	case 23: pPiece->m_iSides[0] = 42; pPiece->m_iSides[1] = 43; pPiece->m_iSides[2] = 24; pPiece->m_iSides[3] = 10;
			 pPiece->m_iSides[4] = 9; pPiece->m_iSides[5] = 22; break;

	case 24: pPiece->m_iSides[0] = 43; pPiece->m_iSides[1] = 44; pPiece->m_iSides[2] = 45; pPiece->m_iSides[3] = 25;
			 pPiece->m_iSides[4] = 10; pPiece->m_iSides[5] = 23; break;

	case 25: pPiece->m_iSides[0] = 24; pPiece->m_iSides[1] = 45; pPiece->m_iSides[2] = 46; pPiece->m_iSides[3] = 26;
			 pPiece->m_iSides[4] = 11; pPiece->m_iSides[5] = 10; break;

	case 26: pPiece->m_iSides[0] = 25; pPiece->m_iSides[1] = 46; pPiece->m_iSides[2] = 47; pPiece->m_iSides[3] = 27;
			 pPiece->m_iSides[4] = 12; pPiece->m_iSides[5] = 11; break;

	case 27: pPiece->m_iSides[0] = 26; pPiece->m_iSides[1] = 47; pPiece->m_iSides[2] = 48; pPiece->m_iSides[3] = 49;
			 pPiece->m_iSides[4] = 28; pPiece->m_iSides[5] = 12; break;

	case 28: pPiece->m_iSides[0] = 12; pPiece->m_iSides[1] = 27; pPiece->m_iSides[2] = 49; pPiece->m_iSides[3] = 50;
			 pPiece->m_iSides[4] = 29; pPiece->m_iSides[5] = 13; break;

	case 29: pPiece->m_iSides[0] = 13; pPiece->m_iSides[1] = 28; pPiece->m_iSides[2] = 50; pPiece->m_iSides[3] = 51;
			 pPiece->m_iSides[4] = 30; pPiece->m_iSides[5] = 14; break;

	case 30: pPiece->m_iSides[0] = 14; pPiece->m_iSides[1] = 29; pPiece->m_iSides[2] = 51; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = 31; break;

	case 31: pPiece->m_iSides[0] = 15; pPiece->m_iSides[1] = 14; pPiece->m_iSides[2] = 30; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = 32; break;

	case 32: pPiece->m_iSides[0] = 16; pPiece->m_iSides[1] = 15; pPiece->m_iSides[2] = 31; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = 33; break;

	case 33: pPiece->m_iSides[0] = 34; pPiece->m_iSides[1] = 16; pPiece->m_iSides[2] = 32; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = -1; break;

	case 34: pPiece->m_iSides[0] = 35; pPiece->m_iSides[1] = 17; pPiece->m_iSides[2] = 16; pPiece->m_iSides[3] = 33;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = -1; break;

	case 35: pPiece->m_iSides[0] = 36; pPiece->m_iSides[1] = 18; pPiece->m_iSides[2] = 17; pPiece->m_iSides[3] = 34;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = -1; break;

	case 36: pPiece->m_iSides[0] = 37; pPiece->m_iSides[1] = 19; pPiece->m_iSides[2] = 18; pPiece->m_iSides[3] = 35;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = -1; break;


	//5-6 pl
	case 37: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = 38; pPiece->m_iSides[2] = 19; pPiece->m_iSides[3] = 36;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = -1; break;

	case 38: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = 39; pPiece->m_iSides[2] = 20; pPiece->m_iSides[3] = 19;
			 pPiece->m_iSides[4] = 37; pPiece->m_iSides[5] = -1; break;

	case 39: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = 40; pPiece->m_iSides[2] = 21; pPiece->m_iSides[3] = 20;
			 pPiece->m_iSides[4] = 38; pPiece->m_iSides[5] = -1; break;

	case 40: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = 41; pPiece->m_iSides[3] = 21;
			 pPiece->m_iSides[4] = 39; pPiece->m_iSides[5] = -1; break;

	case 41: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = 42; pPiece->m_iSides[3] = 22;
			 pPiece->m_iSides[4] = 21; pPiece->m_iSides[5] = 40; break;

	case 42: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = 43; pPiece->m_iSides[3] = 23;
			 pPiece->m_iSides[4] = 22; pPiece->m_iSides[5] = 41; break;

	case 43: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = 44; pPiece->m_iSides[3] = 24;
			 pPiece->m_iSides[4] = 23; pPiece->m_iSides[5] = 42; break;

	case 44: pPiece->m_iSides[0] = -1; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = 45;
			 pPiece->m_iSides[4] = 24; pPiece->m_iSides[5] = 43; break;

	case 45: pPiece->m_iSides[0] = 44; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = 46;
			 pPiece->m_iSides[4] = 25; pPiece->m_iSides[5] = 24; break;

	case 46: pPiece->m_iSides[0] = 45; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = 47;
			 pPiece->m_iSides[4] = 26; pPiece->m_iSides[5] = 25; break;

	case 47: pPiece->m_iSides[0] = 46; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = 48;
			 pPiece->m_iSides[4] = 27; pPiece->m_iSides[5] = 26; break;

	case 48: pPiece->m_iSides[0] = 47; pPiece->m_iSides[1] = -1; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = 49; pPiece->m_iSides[5] = 27; break;

	case 49: pPiece->m_iSides[0] = 27; pPiece->m_iSides[1] = 48; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = 50; pPiece->m_iSides[5] = 28; break;

	case 50: pPiece->m_iSides[0] = 28; pPiece->m_iSides[1] = 49; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = 51; pPiece->m_iSides[5] = 29; break;

	case 51: pPiece->m_iSides[0] = 29; pPiece->m_iSides[1] = 50; pPiece->m_iSides[2] = -1; pPiece->m_iSides[3] = -1;
			 pPiece->m_iSides[4] = -1; pPiece->m_iSides[5] = 30; break;

	default:
		break;
	}
}

#endif		//DEBUG_BUILD_MAPS