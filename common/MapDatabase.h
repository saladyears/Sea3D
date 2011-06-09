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

#if !defined(AFX_MAPDATABASE_H__9C8E0AFC_AD1A_4C34_9E23_0FF1B4E13289__INCLUDED_)
#define AFX_MAPDATABASE_H__9C8E0AFC_AD1A_4C34_9E23_0FF1B4E13289__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"
#include "defineDebug.h"
#include "S3DMap.h"
#include "MapPiece.h"
#ifndef MAPS3D
#include "NDKMessage.h"
#endif

//////////////////////////////////////////////////////////////////////
// typedefs
//////////////////////////////////////////////////////////////////////
typedef void (*LPNMAPCOORDS)(int, CMapPiece *);

//////////////////////////////////////////////////////////////////////
// instance definition
//////////////////////////////////////////////////////////////////////
#define MAPS		CMapDatabase::instance()
#define MAP			CMapDatabase::instance()->getCurrent()

//////////////////////////////////////////////////////////////////////
// static class for writing to and from the map database
//////////////////////////////////////////////////////////////////////
class CMapDatabase  
{
public:
	//////////////////////////////////////////////////////////////////
	// constructor / destructor
	//////////////////////////////////////////////////////////////////
	CMapDatabase();
	~CMapDatabase();

protected:
	/////////////////////////////////////////////////////////////////////////
	// a pointer to a static instance of this class
	/////////////////////////////////////////////////////////////////////////
	static CMapDatabase *m_pMapDatabase;

	//an array of map IDs
	static CArray <UINT, UINT> m_uiMaps;

	//an array of offsets into the map file
	static CArray <DWORD, DWORD> m_dwOffsets;

	//array of map names
	static CStringArray m_strNames;

	//the current map version
	static float m_fVer;

	//the currently loaded map
	static CS3DMap m_Map;

	//opens the database and gets information
	static void open(void);

public:
	/////////////////////////////////////////////////////////////////////
	// instanstiation/destruction methods
	/////////////////////////////////////////////////////////////////////
	static CMapDatabase *instance(void);
	static void release(void);

	//////////////////////////////////////////////////////////////////
	// information methods
	//////////////////////////////////////////////////////////////////
	static int numMaps(void);
	static float mapVer(void);

	//////////////////////////////////////////////////////////////////
	// database methods
	//////////////////////////////////////////////////////////////////
	static CS3DMap *getCurrent(void) {return &m_Map;}
	static void setCurrent(CS3DMap *pMap) {m_Map = (*pMap);}
	static UINT getMapID(int iMap);
	static void addMap(CS3DMap *pMap);
	static void loadMap(int iMap);
	static BOOL loadMapByID(UINT uiID);
	static CString getMapInfo(int iMap);

#ifndef MAPS3D
	static void SerializeMapToMessage(CNDKMessage &msg);
	static void SerializeMapFromMessage(CNDKMessage &msg);
#endif

	static void integrity(CS3DMap *pMap);
	static void adjustMap(CS3DMap *pMap, double dX, double dZ);
	static void createMap(int nBorders, CS3DMap *pMap, LPNMAPCOORDS lpfnCoords);
	static void coords(int nIndex, CMapPiece *pPiece, int *nStart, int *nDZOff, int nLeft, int nWidth);
	static void sides(int nIndex, CMapPiece *pPiece, int nWidth, int nSize);

#ifdef DEBUG_BUILD_MAPS
	static void addMap(int iMap);

	//original map functions
	static void createOriginal34(CS3DMap *pMap);
	static void createOriginal56(CS3DMap *pMap);
	static void coordsOriginal(int iIndex, BOOL b56, CMapPiece *pPiece);

	//new shores functions
	static void createNewShores(CS3DMap *pMap);
	static void coordsNewShores(int iIndex, CMapPiece *pPiece);

	//four islands (III) functions
	static void createFourIslandsIII(CS3DMap *pMap);
	static void coordsFourIslandsIII(int iIndex, CMapPiece *pPiece);

	//four islands (IV) functions
	static void createFourIslandsIV(CS3DMap *pMap);
	static void coordsFourIslandsIV(int iIndex, CMapPiece *pPiece);

	//five islands (V) functions
	static void createFiveIslandsV(CS3DMap *pMap);
	static void coordsFiveIslandsV(int iIndex, CMapPiece *pPiece);

	//six islands (VI) functions
	static void createSixIslandsVI(CS3DMap *pMap);
	static void coordsSixIslandsVI(int iIndex, CMapPiece *pPiece);

	//oceans (III) functions
	static void createOceansIII(CS3DMap *pMap);
	static void coordsOceansIII(int iIndex, CMapPiece *pPiece);

	//oceans (IV) functions
	static void createOceansIV(CS3DMap *pMap);
	static void coordsOceansIV(int iIndex, CMapPiece *pPiece);

	//oceans (V) functions
	static void createOceansV(CS3DMap *pMap);
	static void coordsOceansV(int iIndex, CMapPiece *pPiece);

	//oceans (VI) functions
	static void createOceansVI(CS3DMap *pMap);
	static void coordsOceansVI(int iIndex, CMapPiece *pPiece);

	//into the desert (III) functions
	static void createIntoDesertIII(CS3DMap *pMap);
	static void coordsIntoDesertIII(int iIndex, CMapPiece *pPiece);

	//into the desert (IV) functions
	static void createIntoDesertIV(CS3DMap *pMap);
	static void coordsIntoDesertIV(int iIndex, CMapPiece *pPiece);

	//into the desert (V) functions
	static void createIntoDesertV(CS3DMap *pMap);
	static void coordsIntoDesertV(int iIndex, CMapPiece *pPiece);

	//into the desert (VI) functions
	static void createIntoDesertVI(CS3DMap *pMap);
	static void coordsIntoDesertVI(int iIndex, CMapPiece *pPiece);

	//a new world (III) functions
	static void createNewWorldIII(CS3DMap *pMap);
	static void coordsNewWorldIII(int iIndex, CMapPiece *pPiece);

	//a new world (IV) functions
	static void createNewWorldIV(CS3DMap *pMap);
	static void coordsNewWorldIV(int iIndex, CMapPiece *pPiece);

	//a new world (V) functions
	static void createNewWorldV(CS3DMap *pMap);
	static void coordsNewWorldV(int iIndex, CMapPiece *pPiece);

	//a new world (VI) functions
	static void createNewWorldVI(CS3DMap *pMap);
	static void coordsNewWorldVI(int iIndex, CMapPiece *pPiece);

	//great crossing (IV) functions
	static void createGreatCrossingIV(CS3DMap *pMap);
	static void coordsGreatCrossingIV(int iIndex, CMapPiece *pPiece);

	//great crossing (VI) functions
	static void createGreatCrossingVI(CS3DMap *pMap);
	static void coordsGreatCrossingVI(int iIndex, CMapPiece *pPiece);

	//greater catan (IV) functions
	static void createGreaterCatanIV(CS3DMap *pMap);
	static void coordsGreaterCatanIV(int iIndex, CMapPiece *pPiece);

	//greater catan (VI) functions
	static void createGreaterCatanVI(CS3DMap *pMap);
	static void coordsGreaterCatanVI(int iIndex, CMapPiece *pPiece);

#endif	//DEBUG_BUILD_MAPS
};

#endif // !defined(AFX_MAPDATABASE_H__9C8E0AFC_AD1A_4C34_9E23_0FF1B4E13289__INCLUDED_)
