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
#include "maps3d.h"
#else
#include "settlers.h"
#endif
#include "S3DMap.h"
#include "defineMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CS3DMap, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CS3DMap::CS3DMap()
{
	//set ID
	m_uiID = (UINT) time(NULL);

	//last edited is right now
	m_timeEdit = COleDateTime::GetCurrentTime();

	//settlers type
	m_iMode = MAP_SETTLERS;

	//default to 10 points needed
	m_iPoints = 10;

	//trade routes
	m_bUsesTradeRoutes = FALSE;

	//no chitless tiles
	m_bHasChitlessTiles = FALSE;

	//no port assignment
	m_bAssignsPorts = FALSE;

	//no desert buffer
	m_bDesertBuffer = FALSE;

	//no hidden tiles
	m_bHasHiddenTiles = FALSE;

	//no initial ship requirements
	m_bRequiresInitialShips = FALSE;

	//default to no chip laying method
	m_bUseChipMethod = FALSE;

	//no random amount of tiles
	m_nRandomTiles = 0;
	m_nRandomChits = 0;

	//no ports yet
	m_nPorts = 0;

	//not official
	m_bOfficial = FALSE;

	//no chips used yet
	m_iChipsUsed = 0;
	
	//default min of 1, max of 6
	m_iMinPlayers = 1;
	m_iMaxPlayers = 6;

	//no land or sea yet
	m_iLandTiles = 0;
	m_iSeaTiles = 0;
	m_iHiddenTiles = 0;
	m_iHiddenChits = 0;
	m_iChitlessTiles = 0;
	m_iChitlessChits = 0;

	//standard allowed cards
	m_iAllowedCards = 7;

	//standard bank allocation
	m_iBankRes = 19;

	//standard dev card total
	m_iDevTotal = 25;

	//standard dev card allotments
	m_iDevCards[CARD_VICTORY] = 5;
	m_iDevCards[CARD_MONOPOLY] = 2;
	m_iDevCards[CARD_ROAD_BUILD] = 2;
	m_iDevCards[CARD_YEAR_PLENTY] = 2;
	m_iDevCards[CARD_SOLDIER] = 14;

	//standard ships, roads, cities, settlements
	m_iStockShips = 15;
	m_iStockRoads = 15;
	m_iStockCities = 4;
	m_iStockSettlements = 5;

	//no bonuses
	m_nBonus1 = 0;
	m_nBonus2 = 0;

	//no default resource totals
	::ZeroMemory(m_iRandomRes, sizeof(m_iRandomRes));
	::ZeroMemory(m_iHiddenRes, sizeof(m_iHiddenRes));
	::ZeroMemory(m_iChitlessRes, sizeof(m_iChitlessRes));

	//no default chits
	::ZeroMemory(m_nRandomChit, sizeof(m_nRandomChit));
	::ZeroMemory(m_nHiddenChit, sizeof(m_nHiddenChit));
	::ZeroMemory(m_nChitlessChit, sizeof(m_nChitlessChit));

	//no rotation
	m_dInitialRot = 0.0;

	//standard depth is 80.
	m_dInitialDepth = 80.0;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CS3DMap::CS3DMap(const CS3DMap &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CS3DMap::~CS3DMap()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CS3DMap &CS3DMap::operator =(const CS3DMap &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CS3DMap::copy(const CS3DMap &data)
{
	//unique identifier
	m_uiID						= data.m_uiID;

	//last time edited
	m_timeEdit					= data.m_timeEdit;

	//type of game (settlers, seafarers, c&k)
	m_iMode						= data.m_iMode;

	//is it an official map (can't be deleted)
	m_bOfficial					= data.m_bOfficial;

	//map name
	m_strMapName				= data.m_strMapName;
	m_strMapCreator				= data.m_strMapCreator;

	//points needed to win
	m_iPoints					= data.m_iPoints;

	//trade routes
	m_bUsesTradeRoutes			= data.m_bUsesTradeRoutes;

	//chitless tiles
	m_bHasChitlessTiles			= data.m_bHasChitlessTiles;

	//port assignment
	m_bAssignsPorts				= data.m_bAssignsPorts;

	//desert buffer
	m_bDesertBuffer				= data.m_bDesertBuffer;

	//hidden tiles
	m_bHasHiddenTiles			= data.m_bHasHiddenTiles;

	//initial ship requirements
	m_bRequiresInitialShips		= data.m_bRequiresInitialShips;

	//chip laying method
	m_bUseChipMethod			= data.m_bUseChipMethod;

	//random amounts
	m_nRandomTiles				= data.m_nRandomTiles;
	m_nRandomChits				= data.m_nRandomChits;

	//ports
	m_nPorts					= data.m_nPorts;

	//chip texture files used
	m_iChipsUsed				= data.m_iChipsUsed;

	//min/max # players
	m_iMinPlayers				= data.m_iMinPlayers;
	m_iMaxPlayers				= data.m_iMaxPlayers;

	//number of land and sea tiles
	m_iLandTiles				= data.m_iLandTiles;
	m_iSeaTiles					= data.m_iSeaTiles;
	m_iHiddenTiles				= data.m_iHiddenTiles;
	m_iHiddenChits				= data.m_iHiddenChits;
	m_iChitlessTiles			= data.m_iChitlessTiles;
	m_iChitlessChits			= data.m_iChitlessChits;

	//maximum allowed cards in hand
	m_iAllowedCards				= data.m_iAllowedCards;

	//starting bank allocations
	m_iBankRes					= data.m_iBankRes;

	//numbers of dev cards for this map
	m_iDevTotal					= data.m_iDevTotal;
	memcpy(m_iDevCards, data.m_iDevCards, sizeof(m_iDevCards));

	//numbers of starting roads, cities, and settlements
	m_iStockRoads				= data.m_iStockRoads;
	m_iStockCities				= data.m_iStockCities;
	m_iStockSettlements			= data.m_iStockSettlements;

	//bonuses
	m_nBonus1					= data.m_nBonus1;
	m_nBonus2					= data.m_nBonus2;

	//number of places starting roll chits can be placed
	memcpy(m_iRollPlaces, data.m_iRollPlaces, sizeof(m_iRollPlaces));

	//numbers of randomly placeable resources in this map
	memcpy(m_iRandomRes, data.m_iRandomRes, sizeof(m_iRandomRes));
	memcpy(m_iHiddenRes, data.m_iHiddenRes, sizeof(m_iHiddenRes));
	memcpy(m_iChitlessRes, data.m_iChitlessRes, sizeof(m_iChitlessRes));

	//random chits
	memcpy(m_nRandomChit, data.m_nRandomChit, sizeof(m_nRandomChit));
	memcpy(m_nHiddenChit, data.m_nHiddenChit, sizeof(m_nHiddenChit));
	memcpy(m_nChitlessChit, data.m_nChitlessChit, sizeof(m_nChitlessChit));

	//the board pieces
	m_mapPieces					= data.m_mapPieces;

	//the borders
	m_mapBorders				= data.m_mapBorders;

	//border edges
	m_mapEdges					= data.m_mapEdges;

	//no rotation
	m_dInitialRot				= data.m_dInitialRot;

	//depth
	m_dInitialDepth				= data.m_dInitialDepth;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CS3DMap::Serialize(CArchive &ar)
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
void CS3DMap::load(CArchive &ar)
{
	int i;
	int iSize;

	//unique identifier
	ar >> m_uiID;

	//last time edited
	ar >> m_timeEdit;

	//type of game (settlers, seafarers, c&k)
	ar >> m_iMode;

	//is it an official map (can't be deleted)
	ar >> m_bOfficial;

	//map name
	ar >> m_strMapName;
	ar >> m_strMapCreator;

	//points needed
	ar >> m_iPoints;

	//trade routes
	ar >> m_bUsesTradeRoutes;

	//chitless tiles
	ar >> m_bHasChitlessTiles;

	//port assignment
	ar >> m_bAssignsPorts;

	//desert buffer
	ar >> m_bDesertBuffer;

	//hidden tiles
	ar >> m_bHasHiddenTiles;

	//initial ship reqs
	ar >> m_bRequiresInitialShips;

	//chip laying method
	ar >> m_bUseChipMethod;

	//random tiles used
	ar >> m_nRandomTiles;
	ar >> m_nRandomChits;

	//ports
	ar >> m_nPorts;

	//chip texture files used
	ar >> m_iChipsUsed;
	
	//min/max # players
	ar >> m_iMinPlayers;
	ar >> m_iMaxPlayers;

	//number of land and sea tiles
	ar >> m_iLandTiles;
	ar >> m_iSeaTiles;
	ar >> m_iHiddenTiles;
	ar >> m_iHiddenChits;
	ar >> m_iChitlessTiles;
	ar >> m_iChitlessChits;

	//maximum allowed cards in hand
	ar >> m_iAllowedCards;

	//starting bank allocations
	ar >> m_iBankRes;

	//numbers of dev cards for this map
	ar >> m_iDevTotal;
	ar.Read(m_iDevCards, sizeof(m_iDevCards));

	//numbers of starting roads, cities, and settlements
	ar >> m_iStockRoads;
	ar >> m_iStockShips;
	ar >> m_iStockCities;
	ar >> m_iStockSettlements;

	//bonuses
	ar >> m_nBonus1;
	ar >> m_nBonus2;

	//read in the roll places
	ar.Read(m_iRollPlaces, sizeof(m_iRollPlaces));

	//numbers of randomly placeable resources in this map
	ar.Read(m_iRandomRes, sizeof(m_iRandomRes));
	ar.Read(m_iHiddenRes, sizeof(m_iHiddenRes));
	ar.Read(m_iChitlessRes, sizeof(m_iChitlessRes));

	//random chits
	ar.Read(m_nRandomChit, sizeof(m_nRandomChit));
	ar.Read(m_nHiddenChit, sizeof(m_nHiddenChit));
	ar.Read(m_nChitlessChit, sizeof(m_nChitlessChit));

	//the board pieces
	ar >> iSize;
	m_mapPieces.resize(iSize);

	for(i = 0; i < iSize; i++)
	{
		m_mapPieces[i].Serialize(ar);
	}

	//the borders
	ar >> iSize;
	m_mapBorders.resize(iSize);

	for(i = 0; i < iSize; i++)
	{
		m_mapBorders[i].Serialize(ar);
	}

	//border edges
	ar >> iSize;
	m_mapEdges.reserve(iSize);

	for(i = 0; i < iSize; i++)
	{
		m_mapEdges[i].Serialize(ar);
	}

	//rotation
	ar >> m_dInitialRot;

	//depth
	ar >> m_dInitialDepth;
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CS3DMap::save(CArchive &ar)
{
	int i;

	//unique identifier
	ar << m_uiID;

	//last time edited
	ar << m_timeEdit;

	//type of game (settlers, seafarers, c&k)
	ar << m_iMode;

	//is it an official map (can't be deleted)
	ar << m_bOfficial;

	//map name
	ar << m_strMapName;
	ar << m_strMapCreator;

	//points needed
	ar << m_iPoints;

	//trade routes
	ar << m_bUsesTradeRoutes;

	//chitless tiles
	ar << m_bHasChitlessTiles;

	//port assignment
	ar << m_bAssignsPorts;

	//desert buffer
	ar << m_bDesertBuffer;

	//hidden tiles
	ar << m_bHasHiddenTiles;

	//initial ship reqs
	ar << m_bRequiresInitialShips;

	//chip laying method
	ar << m_bUseChipMethod;

	//random tiles used
	ar << m_nRandomTiles;
	ar << m_nRandomChits;

	//ports
	ar << m_nPorts;

	//chip texture files used
	ar << m_iChipsUsed;

	//min/max # players
	ar << m_iMinPlayers;
	ar << m_iMaxPlayers;

	//number of land and sea tiles
	ar << m_iLandTiles;
	ar << m_iSeaTiles;
	ar << m_iHiddenTiles;
	ar << m_iHiddenChits;
	ar << m_iChitlessTiles;
	ar << m_iChitlessChits;

	//maximum allowed cards in hand
	ar << m_iAllowedCards;

	//starting bank allocations
	ar << m_iBankRes;

	//numbers of dev cards for this map
	ar << m_iDevTotal;
	ar.Write(m_iDevCards, sizeof(m_iDevCards));

	//numbers of starting roads, cities, and settlements
	ar << m_iStockRoads;
	ar << m_iStockShips;
	ar << m_iStockCities;
	ar << m_iStockSettlements;

	//bonuses
	ar << m_nBonus1;
	ar << m_nBonus2;

	//write out the roll places
	ar.Write(m_iRollPlaces, sizeof(m_iRollPlaces));

	//numbers of randomly placeable resources in this map
	ar.Write(m_iRandomRes, sizeof(m_iRandomRes));
	ar.Write(m_iHiddenRes, sizeof(m_iHiddenRes));
	ar.Write(m_iChitlessRes, sizeof(m_iChitlessRes));

	//random chits
	ar.Write(m_nRandomChit, sizeof(m_nRandomChit));
	ar.Write(m_nHiddenChit, sizeof(m_nHiddenChit));
	ar.Write(m_nChitlessChit, sizeof(m_nChitlessChit));

	//the board pieces
	ar << m_mapPieces.size();

	for(i = 0; i < (int) m_mapPieces.size(); i++)
	{
		m_mapPieces[i].Serialize(ar);
	}

	//the borders
	ar << m_mapBorders.size();

	for(i = 0; i < (int) m_mapBorders.size(); i++)
	{
		m_mapBorders[i].Serialize(ar);
	}

	//border edges
	ar << m_mapEdges.size();

	for(i = 0; i < (int) m_mapEdges.size(); i++)
	{
		m_mapEdges[i].Serialize(ar);
	}

	//rotation
	ar << m_dInitialRot;

	//depth
	ar << m_dInitialDepth;
}

#ifndef MAPS3D
//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CS3DMap::SerializeToMessage(CNDKMessage &msg)
{
	int i;
	int iStart = 0;

	//unique identifier
	msg.SetAt(iStart++, m_uiID);

	//last time edited
	msg.SetAt(iStart++, (DATE) m_timeEdit);

	//type of game (settlers, seafarers, c&k)
	msg.SetAt(iStart++, m_iMode);

	//is it an official map (can't be deleted)
	msg.SetAt(iStart++, m_bOfficial);

	//map name
	msg.SetAt(iStart++, m_strMapName);
	msg.SetAt(iStart++, m_strMapCreator);

	//points needed
	msg.SetAt(iStart++, m_iPoints);

	//trade routes
	msg.SetAt(iStart++, m_bUsesTradeRoutes);

	//chitless tiles
	msg.SetAt(iStart++, m_bHasChitlessTiles);

	//port assignment
	msg.SetAt(iStart++, m_bAssignsPorts);

	//desert buffer
	msg.SetAt(iStart++, m_bDesertBuffer);

	//hidden tiles
	msg.SetAt(iStart++, m_bHasHiddenTiles);

	//initial ship reqs
	msg.SetAt(iStart++, m_bRequiresInitialShips);

	//chip laying method
	msg.SetAt(iStart++, m_bUseChipMethod);

	//chip texture files used
	msg.SetAt(iStart++, m_iChipsUsed);

	//min/max # players
	msg.SetAt(iStart++, m_iMinPlayers);
	msg.SetAt(iStart++, m_iMaxPlayers);

	//number of land and sea tiles
	msg.SetAt(iStart++, m_iLandTiles);
	msg.SetAt(iStart++, m_iSeaTiles);
	msg.SetAt(iStart++, m_iHiddenTiles);
	msg.SetAt(iStart++, m_iHiddenChits);
	msg.SetAt(iStart++, m_iChitlessTiles);
	msg.SetAt(iStart++, m_iChitlessChits);

	//maximum allowed cards in hand
	msg.SetAt(iStart++, m_iAllowedCards);

	//starting bank allocations
	msg.SetAt(iStart++, m_iBankRes);

	//numbers of dev cards for this map
	msg.SetAt(iStart++, m_iDevTotal);
	msg.SetAt(iStart++, (LPVOID) m_iDevCards, sizeof(m_iDevCards));

	//numbers of starting roads, cities, and settlements
	msg.SetAt(iStart++, m_iStockRoads);
	msg.SetAt(iStart++, m_iStockCities);
	msg.SetAt(iStart++, m_iStockSettlements);

	//bonuses
	msg.SetAt(iStart++, m_nBonus1);
	msg.SetAt(iStart++, m_nBonus2);

	//number of places starting roll chits can be placed
	msg.SetAt(iStart++, (LPVOID) m_iRollPlaces, sizeof(m_iRollPlaces));

	//numbers of randomly placeable resources in this map
	msg.SetAt(iStart++, (LPVOID) m_iRandomRes, sizeof(m_iRandomRes));
	msg.SetAt(iStart++, (LPVOID) m_iHiddenRes, sizeof(m_iHiddenRes));
	msg.SetAt(iStart++, (LPVOID) m_iChitlessRes, sizeof(m_iChitlessRes));

	//random chits
	msg.SetAt(iStart++, (LPVOID) m_nRandomChit, sizeof(m_nRandomChit));
	msg.SetAt(iStart++, (LPVOID) m_nHiddenChit, sizeof(m_nHiddenChit));
	msg.SetAt(iStart++, (LPVOID) m_nChitlessChit, sizeof(m_nChitlessChit));

	//the board pieces
	msg.SetAt(iStart++, (int) m_mapPieces.size());

	for(i = 0; i < (int) m_mapPieces.size(); i++)
	{
		m_mapPieces[i].SerializeToMessage(iStart, msg);
	}

	//the borders
	msg.SetAt(iStart++, (int) m_mapBorders.size());

	for(i = 0; i < (int) m_mapBorders.size(); i++)
	{
		m_mapBorders[i].SerializeToMessage(iStart, msg);
	}

	//the edges
	msg.SetAt(iStart++, (int) m_mapEdges.size());

	for(i = 0; i < (int) m_mapEdges.size(); i++)
	{
		m_mapEdges[i].SerializeToMessage(iStart, msg);
	}

	//rotation
	msg.SetAt(iStart++, m_dInitialRot);

	//depth
	msg.SetAt(iStart++, m_dInitialDepth);
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CS3DMap::SerializeFromMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 0;
	DATE time;

	//unique identifier
	msg.GetAt(iStart++, m_uiID);

	//last time edited
	msg.GetAt(iStart++, time);
	m_timeEdit = COleDateTime(time);

	//type of game (settlers, seafarers, c&k)
	msg.GetAt(iStart++, m_iMode);

	//is it an official map (can't be deleted)
	msg.GetAt(iStart++, m_bOfficial);

	//map name
	msg.GetAt(iStart++, m_strMapName);
	msg.GetAt(iStart++, m_strMapCreator);

	//points needed
	msg.GetAt(iStart++, m_iPoints);

	//trade routes
	msg.GetAt(iStart++, m_bUsesTradeRoutes);

	//chitless tiles
	msg.GetAt(iStart++, m_bHasChitlessTiles);

	//port assignment
	msg.GetAt(iStart++, m_bAssignsPorts);

	//desert buffer
	msg.GetAt(iStart++, m_bDesertBuffer);

	//hidden tiles
	msg.GetAt(iStart++, m_bHasHiddenTiles);

	//initial ship reqs
	msg.GetAt(iStart++, m_bRequiresInitialShips);

	//chip laying method
	msg.GetAt(iStart++, m_bUseChipMethod);

	//chip texture files used
	msg.GetAt(iStart++, m_iChipsUsed);

	//min/max # players
	msg.GetAt(iStart++, m_iMinPlayers);
	msg.GetAt(iStart++, m_iMaxPlayers);

	//number of land and sea tiles
	msg.GetAt(iStart++, m_iLandTiles);
	msg.GetAt(iStart++, m_iSeaTiles);
	msg.GetAt(iStart++, m_iHiddenTiles);
	msg.GetAt(iStart++, m_iHiddenChits);
	msg.GetAt(iStart++, m_iChitlessTiles);
	msg.GetAt(iStart++, m_iChitlessChits);

	//maximum allowed cards in hand
	msg.GetAt(iStart++, m_iAllowedCards);

	//starting bank allocations
	msg.GetAt(iStart++, m_iBankRes);

	//numbers of dev cards for this map
	msg.GetAt(iStart++, m_iDevTotal);
	msg.GetAt(iStart++, (LPVOID) m_iDevCards, sizeof(m_iDevCards));

	//numbers of starting roads, cities, and settlements
	msg.GetAt(iStart++, m_iStockRoads);
	msg.GetAt(iStart++, m_iStockCities);
	msg.GetAt(iStart++, m_iStockSettlements);

	//bonuses
	msg.GetAt(iStart++, m_nBonus1);
	msg.GetAt(iStart++, m_nBonus2);

	//number of places starting roll chits can be placed
	msg.GetAt(iStart++, (LPVOID) m_iRollPlaces, sizeof(m_iRollPlaces));

	//numbers of randomly placeable resources in this map
	msg.GetAt(iStart++, (LPVOID) m_iRandomRes, sizeof(m_iRandomRes));
	msg.GetAt(iStart++, (LPVOID) m_iHiddenRes, sizeof(m_iHiddenRes));
	msg.GetAt(iStart++, (LPVOID) m_iChitlessRes, sizeof(m_iChitlessRes));

	//random chits
	msg.GetAt(iStart++, (LPVOID) m_nRandomChit, sizeof(m_nRandomChit));
	msg.GetAt(iStart++, (LPVOID) m_nHiddenChit, sizeof(m_nHiddenChit));
	msg.GetAt(iStart++, (LPVOID) m_nChitlessChit, sizeof(m_nChitlessChit));

	//the board pieces
	msg.GetAt(iStart++, iSize);

	m_mapPieces.resize(iSize);

	for(i = 0; i < iSize; i++)
	{
		m_mapPieces[i].SerializeFromMessage(iStart, msg);
	}

	//get the size
	msg.GetAt(iStart++, iSize);

	m_mapBorders.resize(iSize);

	//borders
	for(i = 0; i < (int) m_mapBorders.size(); i++)
	{
		m_mapBorders[i].SerializeFromMessage(iStart, msg);
	}

	//get the size
	msg.GetAt(iStart++, iSize);

	m_mapEdges.resize(iSize);

	//border edges
	for(i = 0; i < (int) m_mapEdges.size(); i++)
	{
		m_mapEdges[i].SerializeFromMessage(iStart, msg);
	}

	//rotation
	msg.GetAt(iStart++, m_dInitialRot);
	
	//depth
	msg.GetAt(iStart++, m_dInitialDepth);
}
#endif