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
#ifdef MAPS3D
#include "Maps3D.h"
#else
#include "settlers.h"
#endif
#include "MapDatabase.h"
#include "defineGL.h"
#include "defineMap.h"
#include "defineFile.h"
#include "defineBorders.h"

#ifdef DEBUG_BUILD_MAPS
/////////////////////////////////////////////////////////////////////////////
// adds an official map to the database
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::addMap(int iMap)
{
	CFile file;
	DWORD dwStart;
	CS3DMap *pMap;

	//create the map
	pMap = new CS3DMap;

	//fill the information
	switch(iMap)
	{
	case MAP_SETTLERS_34: createOriginal34(pMap); break;
	case MAP_SETTLERS_56: createOriginal56(pMap); break;
	case MAP_SEA_NEW_SHORES: createNewShores(pMap); break;
	case MAP_SEA_FOUR_ISLANDS_III: createFourIslandsIII(pMap); break;
	case MAP_SEA_FOUR_ISLANDS_IV: createFourIslandsIV(pMap); break;
	case MAP_SEA_FIVE_ISLANDS_V: createFiveIslandsV(pMap); break;
	case MAP_SEA_SIX_ISLANDS_VI: createSixIslandsVI(pMap); break;
	case MAP_SEA_OCEANS_III: createOceansIII(pMap); break;
	case MAP_SEA_OCEANS_IV: createOceansIV(pMap); break;
	case MAP_SEA_OCEANS_V: createOceansV(pMap); break;
	case MAP_SEA_OCEANS_VI: createOceansVI(pMap); break;
	case MAP_SEA_INTO_DESERT_III: createIntoDesertIII(pMap); break;
	case MAP_SEA_INTO_DESERT_IV: createIntoDesertIV(pMap); break;
	case MAP_SEA_INTO_DESERT_V: createIntoDesertV(pMap); break;
	case MAP_SEA_INTO_DESERT_VI: createIntoDesertVI(pMap); break;
	case MAP_SEA_NEW_WORLD_III: createNewWorldIII(pMap); break;
	case MAP_SEA_NEW_WORLD_IV: createNewWorldIV(pMap); break;
	case MAP_SEA_NEW_WORLD_V: createNewWorldV(pMap); break;
	case MAP_SEA_NEW_WORLD_VI: createNewWorldVI(pMap); break;
	case MAP_SEA_GREAT_CROSSING_IV: createGreatCrossingIV(pMap); break;
	case MAP_SEA_GREAT_CROSSING_VI: createGreatCrossingVI(pMap); break;
	case MAP_SEA_GREATER_CATAN_IV: createGreaterCatanIV(pMap); break;
	case MAP_SEA_GREATER_CATAN_VI: createGreaterCatanVI(pMap); break;
	}

	//open the database file
	file.Open(FILE_MAP, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

	//seek to the end (append)
	file.SeekToEnd();

	//get the starting position
	dwStart = file.GetPosition();

	//create the archive
	CArchive ar(&file, CArchive::store);

	//output the file version
	ar << (float) FILE_VER;

	//serialize the game
	pMap->Serialize(ar);

	//close the file
	ar.Close();
	file.Close();

	//now open the offset file
	file.Open(FILE_MAPKEY, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite);

	//get to the end
	file.SeekToEnd();

	//open the archive
	CArchive arKey(&file, CArchive::store);

	//dump the offset for this file
	arKey << dwStart;

	//flush it
	arKey.Close();
	file.Close();

	//recalculate
	open();

	//cleanup
	delete pMap;
	pMap = NULL;
}
#endif		//DEBUG_BUILD_MAPS

/////////////////////////////////////////////////////////////////////////////
// generalized map creation function
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::createMap(int nBorders, CS3DMap *pMap, LPNMAPCOORDS lpfnCoords)
{
	int iIndex;
	int nSize = 0;
	CMapPiece *pPiece;

	//calculate map size
	switch(nBorders)
	{
	case MAP_BORDERS_NO_X:		nSize = MAP_SIZE_NO_X;		break;
	case MAP_BORDERS_X:			nSize = MAP_SIZE_X;			break;
	case MAP_BORDERS_XX:		nSize = MAP_SIZE_XX;		break;
	case MAP_BORDERS_XX_X:		nSize = MAP_SIZE_XX_X;		break;
	case MAP_BORDERS_XX_XX:		nSize = MAP_SIZE_XX_XX;		break;
	case MAP_BORDERS_XX_X_XX:	nSize = MAP_SIZE_XX_X_XX;	break;
	}

	//resize the map pieces
	pMap->m_mapPieces.resize(nSize);

	//set the map piece types
	for(iIndex = 0; iIndex < nSize; iIndex++)
	{
		//set the piece pointer
		pPiece = &(pMap->m_mapPieces[iIndex]);

		//set the piece attributes
		pPiece->m_iIndex = iIndex;

		//create the coords
		if(NULL != lpfnCoords)
		{
			lpfnCoords(iIndex, pPiece);
		}

		//set the roll tex
		if(0 < pPiece->m_iPermaRoll)
		{
			if(7 > pPiece->m_iPermaRoll)
			{
				pPiece->m_iPermaRollTex = pPiece->m_iPermaRoll - 2;
			}
			else
			{
				pPiece->m_iPermaRollTex = pPiece->m_iPermaRoll - 3;
			}
		}

		//set map coordinates
		switch(nBorders)
		{
		case MAP_BORDERS_NO_X:		ASSIGN_MAP_COORDS_NO_X;		break;
		case MAP_BORDERS_X:			ASSIGN_MAP_COORDS_X;		break;
		case MAP_BORDERS_XX:		ASSIGN_MAP_COORDS_XX;		break;
		case MAP_BORDERS_XX_X:		ASSIGN_MAP_COORDS_XX_X;		break;
		case MAP_BORDERS_XX_XX:		ASSIGN_MAP_COORDS_XX_XX;	break;
		case MAP_BORDERS_XX_X_XX:	ASSIGN_MAP_COORDS_XX_X_XX;	break;
		}
	}

	//assign borders and adjustment
	switch(nBorders)
	{
	case MAP_BORDERS_NO_X: 
		ASSIGN_MAP_BORDERS_NO_X; 
		ASSIGN_MAP_ADJUST_NO_X;
		break;
	case MAP_BORDERS_X:
		ASSIGN_MAP_BORDERS_X;
		ASSIGN_MAP_ADJUST_X;
		break;
	case MAP_BORDERS_XX:
		ASSIGN_MAP_BORDERS_XX;
		ASSIGN_MAP_ADJUST_XX;
		break;
	case MAP_BORDERS_XX_X:
		ASSIGN_MAP_BORDERS_XX_X;
		ASSIGN_MAP_ADJUST_XX_X;
		break;
	case MAP_BORDERS_XX_XX:
		ASSIGN_MAP_BORDERS_XX_XX;
		ASSIGN_MAP_ADJUST_XX_XX;
		break;
	case MAP_BORDERS_XX_X_XX:
		ASSIGN_MAP_BORDERS_XX_X_XX;
		ASSIGN_MAP_ADJUST_XX_X_XX;
		break;
	}
	
	//set initial rotation
	if(MAP_BORDERS_XX >= nBorders)
	{
		pMap->m_dInitialRot = -30.;
	}
	else
	{
		pMap->m_dInitialRot = 60.;
	}

	//run the integrity check
	integrity(pMap);
}

/////////////////////////////////////////////////////////////////////////////
// map coord adjustment
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::adjustMap(CS3DMap *pMap, double dX, double dZ)
{
	int i;

	//pieces
	for(i = 0; i < (int) pMap->m_mapPieces.size(); i++)
	{
		pMap->m_mapPieces[i].m_glCoords[0] += dX;
		pMap->m_mapPieces[i].m_glCoords[2] += dZ;
	}

	//borders
	for(i = 0; i < (int) pMap->m_mapBorders.size(); i++)
	{
		pMap->m_mapBorders[i].m_glCoords[0] += dX;
		pMap->m_mapBorders[i].m_glCoords[2] += dZ;
	}
}

/////////////////////////////////////////////////////////////////////////////
// piece integrity check
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::integrity(CS3DMap *pMap)
{
	int i, j, k;
	BOOL bFound = FALSE;
	CString str;

	//run through each piece looking at sides
	for(i = 0; i < (int) pMap->m_mapPieces.size(); i++)
	{
		//look at each side
		for(j = 0; j < 6; j++)
		{
			//skip -1's
			if(-1 == pMap->m_mapPieces[i].m_iSides[j])
			{
				continue;
			}

			//reset the flag
			bFound = FALSE;

			//search through the sides for this piece to find the current link
			for(k = 0; k < 6; k++)
			{
				if(pMap->m_mapPieces[pMap->m_mapPieces[i].m_iSides[j]].m_iSides[k] == i)
				{
					//there is a match
					bFound = TRUE;
					break;
				}
			}

			//make sure there's a match
			if(FALSE == bFound)
			{
				str.Format("Piece %d did not have %d.", pMap->m_mapPieces[i].m_iSides[j], i);
				MessageBox(NULL, str, NULL, MB_OK);
				break;
			}
		}

		if(FALSE == bFound)
		{
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// coordinate locating based on standardized map calculations
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::coords(int nIndex, CMapPiece *pPiece, int *nStart, int *nDZOff, int nLeft, int nWidth)
{
	int i, j;
	int nDepth = 0, nShift = 0;
	BOOL bDone = FALSE;

	//run through each start place and see if this tile is in it
	for(i = 0; FALSE == bDone; i++)
	{
		//reset j
		j = nStart[i];

		//reset the depth
		nDepth = 0;

		while(j >= 0)
		{
			if(j == nIndex)
			{
				nShift = nLeft + i;
				bDone = TRUE;
				break;
			}
			j -= nWidth;

			//increment the depth
			nDepth += 2;
		}
	}

	//determine depth
	nDepth += nDZOff[i - 1];

	//determine X
	pPiece->m_glCoords[0] = nShift * ((X / 2) + X);	
	pPiece->m_glCoords[1] = 0.;
	pPiece->m_glCoords[2] = nDepth * -Z;
}

/////////////////////////////////////////////////////////////////////////////
// sides based on standardized map calculations
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::sides(int nIndex, CMapPiece *pPiece, int nWidth, int nSize)
{
	int i;

	//sides
	pPiece->m_iSides[0] = nIndex - (nWidth + 1);
	pPiece->m_iSides[1] = nIndex - 1;
	pPiece->m_iSides[2] = nIndex + nWidth;
	pPiece->m_iSides[3] = nIndex + (nWidth + 1);
	pPiece->m_iSides[4] = nIndex + 1;
	pPiece->m_iSides[5] = nIndex - nWidth;

	//now constrain edges
	for(i = 0; i < 6; i++)
	{
		if((0 > pPiece->m_iSides[i]) || (nSize <= pPiece->m_iSides[i]))
		{
			pPiece->m_iSides[i] = -1;
		}
	}
}
