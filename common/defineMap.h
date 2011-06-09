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

/////////////////////////////////////////////////////////////////////////////
// map defines
/////////////////////////////////////////////////////////////////////////////

#if !defined(DEFINE_MAP_H)
#define DEFINE_MAP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// map defines
/////////////////////////////////////////////////////////////////////////////
#define MAP_LAND			(1)
#define MAP_SEA				(2)
#define MAP_RANDOM			(4)
#define MAP_PORT			(8)
#define MAP_HIDDEN			(16)
#define MAP_CHITLESS		(32)

//flag to signify what parts are not permanenty set in a map
#define MAP_NOT_PERMANENT	(-1)

//game types
#define MAP_SETTLERS		(0)
#define MAP_SEAFARERS		(1)
#define MAP_CITIESKNIGHTS	(2)

//chip texture flags
#define MAP_USES_34_CHIPS	(1)
#define MAP_USES_56_CHIPS	(2)
#define MAP_USES_SEA_CHIPS	(4)

//official maps
#define MAP_SIZE					23
#define MAP_SETTLERS_34				0
#define MAP_SETTLERS_56				1
#define MAP_SEA_NEW_SHORES			2
#define MAP_SEA_FOUR_ISLANDS_III	3
#define MAP_SEA_FOUR_ISLANDS_IV		4
#define MAP_SEA_FIVE_ISLANDS_V		5
#define MAP_SEA_SIX_ISLANDS_VI		6
#define MAP_SEA_OCEANS_III			7
#define MAP_SEA_OCEANS_IV			8
#define MAP_SEA_OCEANS_V			9
#define MAP_SEA_OCEANS_VI			10
#define MAP_SEA_INTO_DESERT_III		11
#define MAP_SEA_INTO_DESERT_IV		12
#define MAP_SEA_INTO_DESERT_V		13
#define MAP_SEA_INTO_DESERT_VI		14
#define MAP_SEA_NEW_WORLD_III		15
#define MAP_SEA_NEW_WORLD_IV		16
#define MAP_SEA_NEW_WORLD_V			17
#define MAP_SEA_NEW_WORLD_VI		18
#define MAP_SEA_GREAT_CROSSING_IV	19
#define MAP_SEA_GREAT_CROSSING_VI	20
#define MAP_SEA_GREATER_CATAN_IV	21
#define MAP_SEA_GREATER_CATAN_VI	22

/////////////////////////////////////////////////////////////////////////////
// map border types
/////////////////////////////////////////////////////////////////////////////
#define MAP_BORDERS_NONE			0
#define MAP_BORDERS_NO_X			1
#define MAP_BORDERS_X				2
#define MAP_BORDERS_XX				3
#define MAP_BORDERS_XX_X			4
#define MAP_BORDERS_XX_XX			5
#define MAP_BORDERS_XX_X_XX			6

/////////////////////////////////////////////////////////////////////////////
// map coordinate systems
/////////////////////////////////////////////////////////////////////////////
#define MAP_SIZE_NO_X				58
#define MAP_SIZE_X					67
#define MAP_SIZE_XX					76
#define MAP_SIZE_XX_X				85
#define MAP_SIZE_XX_XX				94
#define MAP_SIZE_XX_X_XX			103

/////////////////////////////////////////////////////////////////////////////
// map adjustments
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_ADJUST_NO_X		adjustMap(pMap, (X * -0.75), (Z * 8.5))
#define ASSIGN_MAP_ADJUST_X			adjustMap(pMap, (X * -1.50), (Z * 9.0))
#define ASSIGN_MAP_ADJUST_XX		adjustMap(pMap, (X * -2.25), (Z * 9.5))
#define ASSIGN_MAP_ADJUST_XX_X		adjustMap(pMap, (X * -3.00), (Z * 10.0))
#define ASSIGN_MAP_ADJUST_XX_XX		adjustMap(pMap, (X * -3.75), (Z * 10.5))
#define ASSIGN_MAP_ADJUST_XX_X_XX	adjustMap(pMap, (X * -4.50), (Z * 11.0))

/////////////////////////////////////////////////////////////////////////////
// borders with no X tiles - the smallest possible borders
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_NO_X \
	{ \
	int nLeft = -4; \
	int nStart[] = {6, 19, 32, 45, 52, 53, 54, 55, 56, 57}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 12: case 18: \
	case 25: case 31: case 38: case 44: case 51: case 57: case 56: case 55: \
	case 54: case 53: case 52: case 45:	case 39: case 32: case 26: case 19: \
	case 13: case 6: \
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 6); \
	sides(iIndex, pPiece, 6, MAP_SIZE_NO_X); \
	switch(iIndex) \
	{ \
	case 6: case 19: case 32: case 45: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 13: case 26: case 39: case 52: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 12: case 25: case 38: case 51: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 5: case 18: case 31: case 44: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_X \
	{ \
	int nLeft = -4; \
	int nStart[] = {7, 22, 37, 52, 60, 61, 62, 63, 64, 65, 66}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5, 6}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 14: \
	case 21: case 29: case 36: case 44: case 51: case 59: case 66: case 65: \
	case 64: case 63: case 62: case 61:	case 60: case 52: case 45: case 37: \
	case 30: case 22: case 15: case 7: \
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 7); \
	sides(iIndex, pPiece, 7, MAP_SIZE_X); \
	switch(iIndex) \
	{ \
	case 7: case 22: case 37: case 52: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 15: case 30: case 45: case 60: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 14: case 29: case 44: case 59: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 6: case 21: case 36: case 51: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one XX tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_XX \
	{ \
	int nLeft = -4; \
	int nStart[] = {8, 25, 42, 59, 68, 69, 70, 71, 72, 73, 74, 75}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5, 6, 7}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: \
	case 16: case 24: case 33: case 41: case 50: case 58: case 67: case 75: \
	case 74: case 73: case 72: case 71:	case 70: case 69: case 68: case 59: \
	case 51: case 42: case 34: case 25: case 17: case 8: \
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 8); \
	sides(iIndex, pPiece, 8, MAP_SIZE_XX); \
	switch(iIndex) \
	{ \
	case 8: case 25: case 42: case 59: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 17: case 34: case 51: case 68: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 16: case 33: case 50: case 67: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 7: case 24: case 41: case 58: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with one XX and on X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_XX_X \
	{ \
	int nLeft = -4; \
	int nStart[] = {9, 28, 47, 66, 76, 77, 78, 79, 80, 81, 82, 83, 84}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: \
	case 18: case 27: case 37: case 46: case 56: case 65: case 75: case 84: \
	case 83: case 82: case 81: case 80:	case 79: case 78: case 77: case 76: \
	case 66: case 57: case 47: case 38: case 28: case 19: case 9:\
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 9); \
	sides(iIndex, pPiece, 9, MAP_SIZE_XX_X); \
	switch(iIndex) \
	{ \
	case 9: case 28: case 47: case 66: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 19: case 38: case 57: case 76: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 18: case 37: case 56: case 75: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 8: case 27: case 46: case 65: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with two XX tiles
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_XX_XX \
	{ \
	int nLeft = -4; \
	int nStart[] = {10, 31, 52, 73, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:\
	case 10: case 21: case 31: case 42: case 52: case 63: case 73: case 84: \
	case 85: case 86: case 87: case 88:	case 89: case 90: case 91: case 92: \
	case 93: case 83: case 72: case 62: case 51: case 41: case 30: case 20:\
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 10); \
	sides(iIndex, pPiece, 10, MAP_SIZE_XX_XX); \
	switch(iIndex) \
	{ \
	case 10: case 31: case 52: case 73: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 21: case 42: case 63: case 84: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 20: case 41: case 62: case 83: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 9: case 30: case 51: case 72: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}

/////////////////////////////////////////////////////////////////////////////
// borders with two XX tiles and one X tile
/////////////////////////////////////////////////////////////////////////////
#define ASSIGN_MAP_COORDS_XX_X_XX \
	{ \
	int nLeft = -4; \
	int nStart[] = {11, 34, 57, 80, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102}; \
	int nDZOff[] = {10, 7, 4, 1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10}; \
	switch(iIndex) \
	{ \
	case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: case 8: case 9:\
	case 10: case 11: case 23: case 34: case 46: case 57: case 69: case 80: case 92: \
	case 93: case 94: case 95: case 96:	case 97: case 98: case 99: case 100: \
	case 101: case 102: case 91: case 79: case 68: case 56: case 45: case 33: case 22:\
		pPiece->m_bBorder = TRUE; \
		break; \
	default: break; \
	} \
	coords(iIndex, pPiece, nStart, nDZOff, nLeft, 11); \
	sides(iIndex, pPiece, 11, MAP_SIZE_XX_X_XX); \
	switch(iIndex) \
	{ \
	case 11: case 34: case 57: case 80: \
		pPiece->m_iSides[0] = -1; \
		pPiece->m_iSides[1] = -1; \
		pPiece->m_iSides[2] = -1;\
		break; \
	case 23: case 46: case 69: case 92: \
		pPiece->m_iSides[1] = -1; \
		break; \
	case 22: case 45: case 68: case 91: \
		pPiece->m_iSides[3] = -1; \
		pPiece->m_iSides[4] = -1; \
		pPiece->m_iSides[5] = -1;\
	case 10: case 33: case 56: case 79: \
		pPiece->m_iSides[4] = -1; \
		break; \
	} \
	}
#endif	//DEFINE_MAP_H