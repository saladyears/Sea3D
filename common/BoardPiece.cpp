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
#include "BoardPiece.h"

//conditinal includes
#ifndef AI_DLL
#include "settlers.h"
#include "rand.h"
#include "GameDatabase.h"
#include "SettlersView.h"
#include "defineMap.h"
#endif

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CBoardPiece, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CBoardPiece::CBoardPiece(int iIndex)
{
	//set the index
	m_iIndex = iIndex;

	//no type yet
	m_iType = -1;

	//default roll settings
	m_iRoll = 0;
	m_iRollTex = 0;
	m_iRollTexType = 0;

	//the port corners on this tile
	m_iPort1 = -1;
	m_iPort2 = -1;

	//defaults
	m_nIsland = 0;
	m_bChitless = FALSE;
	m_bInitial = TRUE;
	m_bBorder = FALSE;
	m_bSecondary = FALSE;

	//no port side
	m_nPortSide = -1;

	//initialize road settings
	memset(m_iRoads, -1, sizeof(m_iRoads));

	//initialize ship settings
	memset(m_iShips, -1, sizeof(m_iShips));

	//turn placements
	memset(m_nTurnPlacements, -1, sizeof(m_nTurnPlacements));

	//initialize longest road settings
	memset(m_bRoadUsed, 0, sizeof(m_bRoadUsed));

	//initialize city settings
	memset(m_iCity, 0, sizeof(m_iCity));

	//init extra
	ZeroMemory(m_iExtra, sizeof(m_iExtra));

	//no rotation
	m_dRotate = 0.;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CBoardPiece::CBoardPiece(const CBoardPiece &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CBoardPiece::~CBoardPiece()
{
}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CBoardPiece &CBoardPiece::operator =(const CBoardPiece &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CBoardPiece::copy(const CBoardPiece &data)
{
	//the index number of this piece
	m_iIndex			= data.m_iIndex;

	//the type of piece this is
	m_iType				= data.m_iType;

	//the roll value on this piece
	m_iRoll				= data.m_iRoll;
	m_iRollTex			= data.m_iRollTex;
	m_iRollTexType		= data.m_iRollTexType;

	//the port corners on this tile
	m_iPort1			= data.m_iPort1;
	m_iPort2			= data.m_iPort2;

	//GL coords
	memcpy(m_coords, data.m_coords, sizeof(m_coords));

	//piece rotation
	m_dRotate			= data.m_dRotate;

	//neighbors
	memcpy(m_iSides, data.m_iSides, sizeof(m_iSides));

	//island number
	m_nIsland			= data.m_nIsland;

	//chitless
	m_bChitless			= data.m_bChitless;

	//initial
	m_bInitial			= data.m_bInitial;

	//border
	m_bBorder			= data.m_bBorder;

	//secondary
	m_bSecondary		= data.m_bSecondary;

	//port side
	m_nPortSide			= data.m_nPortSide;

	//roads
	memcpy(m_iRoads, data.m_iRoads, sizeof(m_iRoads));

	//ships
	memcpy(m_iShips, data.m_iShips, sizeof(m_iShips));

	//turn placements
	memcpy(m_nTurnPlacements, data.m_nTurnPlacements, sizeof(m_nTurnPlacements));

	//longest road settings
	memcpy(m_bRoadUsed, data.m_bRoadUsed, sizeof(m_bRoadUsed));

	//cities/settlements
	memcpy(m_iCity, data.m_iCity, sizeof(m_iCity));	

	//extra points
	memcpy(m_iExtra, data.m_iExtra, sizeof(m_iExtra));
}

//conditional include
#ifndef AI_DLL

//////////////////////////////////////////////////////////////////////
// copy map information to this board piece
//////////////////////////////////////////////////////////////////////
void CBoardPiece::copyFromMap(CMapPiece *pPiece)
{
	//start a copying
	m_iIndex = pPiece->m_iIndex;

	//piece type
	if(MAP_NOT_PERMANENT != pPiece->m_iPermaType)
	{
		m_iType = pPiece->m_iPermaType;
	}

	if(MAP_NOT_PERMANENT != pPiece->m_iPermaRoll)
	{
		m_iRoll = pPiece->m_iPermaRoll;
		m_iRollTex = pPiece->m_iPermaRollTex;
	}

	//see if we have a roll tex type
	if(MAP_NOT_PERMANENT != pPiece->m_iPermaRollTexType)
	{
		m_iRollTexType = pPiece->m_iPermaRollTexType;
	}
	else
	{
		//set it equal to the chip settings
		m_iRollTexType = MAP->m_iChipsUsed & (~MAP_USES_SEA_CHIPS);
	}

	//island number
	m_nIsland = pPiece->m_nIsland;

	//chitless
	m_bChitless = (MAP_CHITLESS == pPiece->m_iPieceType);

	//initial allowance
	m_bInitial = pPiece->m_bInitial;

	//border
	m_bBorder = pPiece->m_bBorder;

	//secondary placement
	m_bSecondary = pPiece->m_bSecondary;

	//port side
	m_nPortSide = pPiece->m_iPermaPortSide;

	//all the neighboring hexes
	memcpy(m_iSides, pPiece->m_iSides, sizeof(m_iSides));

	//GL coords
	memcpy(m_coords, pPiece->m_glCoords, sizeof(m_coords));

	//set random rotation
	if(RES_HIDDEN != m_iType)
	{
		m_dRotate = randRot();
	}
}

//////////////////////////////////////////////////////////////////////
// generate a random rotation value
//////////////////////////////////////////////////////////////////////
double CBoardPiece::randRot()
{
	static int iRand = 0;
	double dRand;

	//get a number between 0 and 5
	dRand = 5.0 * RAND;

	//clamp to an integer
	iRand = (int) dRand;

	//multiply by 60 to get the rotation
	return double(iRand * 60.0);
}

#endif //AI_DLL

//////////////////////////////////////////////////////////////////////
// returns the lowest side neighbor
//////////////////////////////////////////////////////////////////////
int CBoardPiece::getLowestSide(void)
{
	int i;
	int iLow = 100;

	//run through the list
	for(i = 0; i < 6; i++)
	{
		if(m_iSides[i] < iLow) iLow = m_iSides[i];
	}

	return iLow;
}

//////////////////////////////////////////////////////////////////////
// finds which edge a certain side is on
//////////////////////////////////////////////////////////////////////
int CBoardPiece::findSide(int iSide)
{
	int i;

	for(i = 0; i < 6; i++)
	{
		if(m_iSides[i] == iSide) return i;
	}

	//should never happen
	return -1;
}

//conditional include
#ifndef AI_DLL
//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CBoardPiece::Serialize(CArchive &ar)
{
	//save information
	if(ar.IsStoring())
	{
		save(ar);
	}
	//load information
	else
	{
		load(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CBoardPiece::load(CArchive &ar)
{
	BOOL bBlocked;
	CGame *pGame;

	//the index number of this piece
	ar >> m_iIndex;

	//the type of piece this is
	ar >> m_iType;

	//the roll value on this piece
	ar >> m_iRoll;
	ar >> m_iRollTex;

	if(DATABASE->gameVer() >= VER_NETWORKa)
	{
		ar >> m_iRollTexType;
	}
	else
	{
		//get the GL view
		pGame = DATABASE->getGame();
		if(NULL != pGame)
		{
			m_iRollTexType = (pGame->m_players.size() < 5) ? MAP_USES_34_CHIPS : MAP_USES_56_CHIPS;
		}
		else
		{
			m_iRollTexType = MAP_USES_34_CHIPS;
		}
	}

	//read port information
	if(DATABASE->gameVer() >= VER_NETWORKd)
	{
		ar >> m_iPort1;
		ar >> m_iPort2;
	}

	//all the neighboring hexes
	ar.Read(m_iSides, sizeof(m_iSides));

	//whether the piece is blocked or not - RETIRED IN 0.8.6
	if(DATABASE->gameVer() < VER_NETWORKd)
	{
		ar >> bBlocked;
	}

	//chitless
	if(DATABASE->gameVer() >= VER_GREATER_CATAN)
	{
		ar >> m_nIsland;
		ar >> m_bChitless;
	}

	//initial placement and border
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar >> m_bInitial;
		ar >> m_bBorder;
	}

	//secondary placement
	if(DATABASE->gameVer() >= VER_FOUR_ISLANDS)
	{
		ar >> m_bSecondary;
		ar >> m_nPortSide;
	}

	//GL coords
	ar.Read(m_coords, sizeof(m_coords));

	//any amount of rotation needed
	ar >> m_dRotate;

	//any roads built on this hex
	ar.Read(m_iRoads, sizeof(m_iRoads));

	//any ships built on this hex
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar.Read(m_iShips, sizeof(m_iShips));
	}

	//turn placements
	if(DATABASE->gameVer() >= VER_GREATER_CATAN)
	{
		ar.Read(m_nTurnPlacements, sizeof(m_nTurnPlacements));
	}

	//roads used for longest road
	ar.Read(m_bRoadUsed, sizeof(m_bRoadUsed));

	//any settlements/cities built on this hex
	ar.Read(m_iCity, sizeof(m_iCity));

	//read extra points
	if(DATABASE->gameVer() >= VER_SEAFARERS)
	{
		ar.Read(m_iExtra, sizeof(m_iExtra));
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CBoardPiece::save(CArchive &ar)
{
	//the index number of this piece
	ar << m_iIndex;

	//the type of piece this is
	ar << m_iType;

	//the roll value on this piece
	ar << m_iRoll;
	ar << m_iRollTex;
	ar << m_iRollTexType;

	//write out port information
	ar << m_iPort1;
	ar << m_iPort2;

	//all the neighboring hexes
	ar.Write(m_iSides, sizeof(m_iSides));

	//whether the piece is blocked or not - RETIRED IN 0.8.6
	//ar << m_bBlocked;

	//island number
	ar << m_nIsland;

	//chitless
	ar << m_bChitless;

	//initial placement and border
	ar << m_bInitial;
	ar << m_bBorder;

	//secondary placement
	ar << m_bSecondary;
	
	//port side
	ar << m_nPortSide;

	//GL coords
	ar.Write(m_coords, sizeof(m_coords));

	//any amount of rotation needed
	ar << m_dRotate;

	//any roads built on this hex
	ar.Write(m_iRoads, sizeof(m_iRoads));

	//any ships built on this hex
	ar.Write(m_iShips, sizeof(m_iShips));

	//turn placements
	ar.Write(m_nTurnPlacements, sizeof(m_nTurnPlacements));

	//roads used for longest road
	ar.Write(m_bRoadUsed, sizeof(m_bRoadUsed));

	//any settlements/cities built on this hex
	ar.Write(m_iCity, sizeof(m_iCity));

	//extra points
	ar.Write(m_iExtra, sizeof(m_iExtra));
}

//////////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////////
void CBoardPiece::SerializeToMessage(CNDKMessage &msg)
{
	int iStart = 1;
	
	//index
	msg.SetAt(iStart++, m_iIndex);

	//type of piece
	msg.SetAt(iStart++, m_iType);

	//roll value
	msg.SetAt(iStart++, m_iRoll);
	msg.SetAt(iStart++, m_iRollTex);
	msg.SetAt(iStart++, m_iRollTexType);

	//port corners
	msg.SetAt(iStart++, m_iPort1);
	msg.SetAt(iStart++, m_iPort2);

	//neighboring sides
	msg.SetAt(iStart++, m_iSides, sizeof(m_iSides));
	
	//initial placment and border
	msg.SetAt(iStart++, m_nIsland);
	msg.SetAt(iStart++, m_bChitless);
	msg.SetAt(iStart++, m_bInitial);
	msg.SetAt(iStart++, m_bBorder);
	msg.SetAt(iStart++, m_bSecondary);

	//port side
	msg.SetAt(iStart++, m_nPortSide);

	//openGL coords
	msg.SetAt(iStart++, m_coords, sizeof(m_coords));
	
	//rotation
	msg.SetAt(iStart++, m_dRotate);

	//any roads built on this hex
	msg.SetAt(iStart++, m_iRoads, sizeof(m_iRoads));

	//any ships built on this hex
	msg.SetAt(iStart++, m_iShips, sizeof(m_iShips));

	//turn placements
	msg.SetAt(iStart++, m_nTurnPlacements, sizeof(m_nTurnPlacements));

	//roads used for longest road
	msg.SetAt(iStart++, m_bRoadUsed, sizeof(m_bRoadUsed));

	//any settlements/cities built on this hex
	msg.SetAt(iStart++, m_iCity, sizeof(m_iCity));

	//extra points
	msg.SetAt(iStart++, m_iExtra, sizeof(m_iExtra));
}

//////////////////////////////////////////////////////////////////////
// read from a network message
//////////////////////////////////////////////////////////////////////
void CBoardPiece::SerializeFromMessage(CNDKMessage &msg)
{
	int iStart = 1;
	
	//index
	msg.GetAt(iStart++, m_iIndex);

	//type of piece
	msg.GetAt(iStart++, m_iType);

	//roll value
	msg.GetAt(iStart++, m_iRoll);
	msg.GetAt(iStart++, m_iRollTex);
	msg.GetAt(iStart++, m_iRollTexType);

	//port corners
	msg.GetAt(iStart++, m_iPort1);
	msg.GetAt(iStart++, m_iPort2);

	//neighboring sides
	msg.GetAt(iStart++, m_iSides, sizeof(m_iSides));

	//initial placment and border
	msg.GetAt(iStart++, m_nIsland);
	msg.GetAt(iStart++, m_bChitless);
	msg.GetAt(iStart++, m_bInitial);
	msg.GetAt(iStart++, m_bBorder);
	msg.GetAt(iStart++, m_bSecondary);

	//port side
	msg.GetAt(iStart++, m_nPortSide);
	
	//openGL coords
	msg.GetAt(iStart++, m_coords, sizeof(m_coords));
	
	//rotation
	msg.GetAt(iStart++, m_dRotate);

	//any roads built on this hex
	msg.GetAt(iStart++, m_iRoads, sizeof(m_iRoads));

	//any ships built on this hex
	msg.GetAt(iStart++, m_iShips, sizeof(m_iShips));

	//turn placements
	msg.GetAt(iStart++, m_nTurnPlacements, sizeof(m_nTurnPlacements));

	//roads used for longest road
	msg.GetAt(iStart++, m_bRoadUsed, sizeof(m_bRoadUsed));

	//any settlements/cities built on this hex
	msg.GetAt(iStart++, m_iCity, sizeof(m_iCity));

	//extra points
	msg.GetAt(iStart++, m_iExtra, sizeof(m_iExtra));
}
#endif