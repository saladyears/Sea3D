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
#include "defineMap.h"
#include "defineFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// static instantiations
/////////////////////////////////////////////////////////////////////////////
CMapDatabase *			CMapDatabase::m_pMapDatabase;
CArray <UINT, UINT>		CMapDatabase::m_uiMaps;
CArray <DWORD, DWORD>	CMapDatabase::m_dwOffsets;
float					CMapDatabase::m_fVer;
CS3DMap					CMapDatabase::m_Map;
CStringArray			CMapDatabase::m_strNames;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapDatabase::CMapDatabase()
{
	m_fVer = 0.0;
}

CMapDatabase::~CMapDatabase()
{

}

/////////////////////////////////////////////////////////////////////////////
// instance handling
/////////////////////////////////////////////////////////////////////////////
CMapDatabase *CMapDatabase::instance()
{
	//see if a game database already is open
	if(NULL == m_pMapDatabase)
	{
		//get a new audit handle
		m_pMapDatabase = new CMapDatabase();

		//get preliminary information
		open();
	}

	//return the instance
	return m_pMapDatabase;
}

/////////////////////////////////////////////////////////////////////////////
// release data
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::release()
{
	//delete the static instance if it exists
	if(NULL != m_pMapDatabase)
	{
		delete m_pMapDatabase;
		m_pMapDatabase = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the number of games in the database
/////////////////////////////////////////////////////////////////////////////
int CMapDatabase::numMaps()
{
	return m_uiMaps.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// return the current file version
/////////////////////////////////////////////////////////////////////////////
float CMapDatabase::mapVer()
{
	return m_fVer;
}

/////////////////////////////////////////////////////////////////////////////
// load a map from the database into the current map file
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::loadMap(int iMap)
{
	CFile file;

	//open the database file
	if(!file.Open(FILE_MAP, CFile::modeRead)) 
	{
		return;
	}

	//seek to this position in the game file
	file.Seek(m_dwOffsets[iMap], CFile::begin);
	CArchive ar(&file, CArchive::load);

	//load the file version
	ar >> m_fVer;
	
	//read the game data
	m_Map.Serialize(ar);

	//cleanup
	ar.Close();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////////
// load a map from the database based on ID
/////////////////////////////////////////////////////////////////////////////
BOOL CMapDatabase::loadMapByID(UINT uiID)
{
	int i;

	//search for this ID
	for(i = 0; i < m_uiMaps.GetSize(); i++)
	{
		if(uiID == m_uiMaps[i])
		{
			//we found it
			loadMap(i);
			
			return TRUE;
		}
	}
	
	//we did not found it
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::open(void)
{
	int i, iSize;
	DWORD dwLength;
	CFile file;
	CFile key;
	CS3DMap *pMap;

	//reset the array of game IDs
	m_uiMaps.RemoveAll();

	//if they don't exist, it's the first time they've done anything
	if(!file.Open(FILE_MAP, CFile::modeRead)) return;
	if(!key.Open(FILE_MAPKEY, CFile::modeRead)) return;
	
	//create the key archive
	CArchive arKey(&key, CArchive::load);

	//get the length of the file
	dwLength = (DWORD) key.GetLength();

	//calculate the number of games
	iSize = dwLength / sizeof(DWORD);

	//resize the offset array
	m_dwOffsets.SetSize(iSize);

	//keep going until we reach the end of the file
	for(i = 0; i < iSize; i++)
	{
		//read in the offset
		arKey >> m_dwOffsets[i];
	}

	//clean up
	arKey.Close();
	key.Close();

	//now read in all the game IDs
	for(i = 0; i < iSize; i++)
	{
		//create new data structure
		pMap = new CS3DMap;

		//seek to this position in the game file
		file.Seek(m_dwOffsets[i], CFile::begin);

		//read in the buffer
		CArchive ar(&file, CArchive::load);

		//read in the version
		ar >> m_fVer;

		//read in the data
		pMap->Serialize(ar);

		//get the game ID and name
		m_uiMaps.Add(pMap->m_uiID);
		m_strNames.Add(pMap->m_strMapName);

		//close the buffer
		ar.Close();

		//delete data structure
		delete pMap;
	}

	//close the files
	file.Close();

	//null the pointer
	pMap = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// return the requested map id
/////////////////////////////////////////////////////////////////////////////
UINT CMapDatabase::getMapID(int iMap)
{
	return m_uiMaps[iMap];
}

#ifndef MAPS3D
/////////////////////////////////////////////////////////////////////////////
// network message to write to an outgoing stream
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::SerializeMapToMessage(CNDKMessage &msg)
{
	//let the map do the work
	m_Map.SerializeToMessage(msg);
}

/////////////////////////////////////////////////////////////////////////////
// network message to read from an incoming stream
/////////////////////////////////////////////////////////////////////////////
void CMapDatabase::SerializeMapFromMessage(CNDKMessage &msg)
{
	//load the map
	m_Map.SerializeFromMessage(msg);

	//SAVE IT IF NEEDED HERE
}
#endif

/////////////////////////////////////////////////////////////////////////////
// create a string that holds all the map information
/////////////////////////////////////////////////////////////////////////////
CString CMapDatabase::getMapInfo(int iMap)
{
	CString str;
	CString strTemp;
	CString strType;

	//load up the map in question
	loadMap(iMap);

	//map name
	strTemp.Format("Map: %s", m_Map.m_strMapName);
	str += strTemp;

	if(m_Map.m_iMode & MAP_SEAFARERS)
	{
		strType += "Seafarers, ";
	}
	if(m_Map.m_iMode & MAP_CITIESKNIGHTS)
	{
		strType += "Cities & Knights, ";
	}

	//if it's still empty, it's just a settlers game
	if(TRUE == strType.IsEmpty())
	{
		strType = "Settlers, ";
	}

	//lose the comma
	strType = strType.Left(strType.GetLength() - 2);

	//type of game
	strTemp.Format("\nMap type: %s", strType);
	str += strTemp;

	//number of players
	if(m_Map.m_iMinPlayers == m_Map.m_iMaxPlayers)
	{
		strTemp.Format("\nPlayers: %d", m_Map.m_iMinPlayers);
	}
	else
	{
		strTemp.Format("\nPlayers: %d-%d", m_Map.m_iMinPlayers, m_Map.m_iMaxPlayers);
	}
	str += strTemp;

	//number of points
	strTemp.Format("\nPoints: %d", m_Map.m_iPoints);
	str += strTemp;

	//officialness
	strTemp.Format("\nOfficial: %s", m_Map.m_bOfficial ? "Yes" : "No");
	str += strTemp;

	//creator
	strTemp.Format("\nMap creator: %s", m_Map.m_strMapCreator);
	str += strTemp;

	return str;
}
