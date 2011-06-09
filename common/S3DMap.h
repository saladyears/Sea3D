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

#if !defined(AFX_S3DMAP_H__DFBEA47B_30E2_4148_ADE6_F82A78DE47B5__INCLUDED_)
#define AFX_S3DMAP_H__DFBEA47B_30E2_4148_ADE6_F82A78DE47B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Border.h"
#include "MapPiece.h"
#ifndef MAPS3D
#include "NDKMessage.h"
#endif
#include "defineGame.h"

//////////////////////////////////////////////////////////////////////
// class which holds information about one map
//////////////////////////////////////////////////////////////////////
class CS3DMap : public CObject  
{
public:
	DECLARE_SERIAL(CS3DMap)

	//{{AFX_VIRTUAL(CS3DMap)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CS3DMap();
	CS3DMap(const CS3DMap &data);
	virtual ~CS3DMap();

	//assignment operator
	CS3DMap &operator =(const CS3DMap &data);

	//compare operator
	BOOL operator ==(const CS3DMap &data);

	//streamline copy function
	void copy(const CS3DMap &data);

#ifndef MAPS3D
	//write to/from message
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//unique identifier
	UINT m_uiID;

	//last time edited
	COleDateTime m_timeEdit;

	//type of game (settlers, seafarers, c&k)
	int m_iMode;

	//is it an official map (can't be deleted)
	BOOL m_bOfficial;

	//map name
	CString m_strMapName;
	CString m_strMapCreator;

	//points needed to win
	int m_iPoints;

	//does this map use trade route scoring (Great Crossing maps)
	BOOL m_bUsesTradeRoutes;

	//does this map have chitless tiles (Greater Catan maps)
	BOOL m_bHasChitlessTiles;

	//does this map allow players to assign ports at the beginning of the game?
	BOOL m_bAssignsPorts;

	//does this map use desert pieces as a buffer, allowing players to gain a
	//victory point for building past them?
	BOOL m_bDesertBuffer;

	//does this map have hidden tiles that get revealed as players discover them
	//via ships (this also gives them a free resource if the tile is a land tile)
	BOOL m_bHasHiddenTiles;

	//does it require that initial settlements have ships placed if they are
	//along the coast, unless the edge is touching the border?
	BOOL m_bRequiresInitialShips;

	//does it use the "round the board" chip laying method?
	BOOL m_bUseChipMethod;

	//how many random tiles are there in this map?
	int m_nRandomTiles;
	int m_nRandomChits;

	//the number of ports in this map
	int m_nPorts;

	//chip texture files used
	int m_iChipsUsed;
	
	//min/max # players
	int m_iMinPlayers;
	int m_iMaxPlayers;

	//number of land, sea, and hidden tiles
	int m_iLandTiles;
	int m_iSeaTiles;
	int m_iHiddenTiles;
	int m_iHiddenChits;
	int m_iChitlessTiles;
	int m_iChitlessChits;

	//maximum allowed cards in hand
	int m_iAllowedCards;

	//starting bank allocations
	int m_iBankRes;

	//numbers of dev cards for this map
	int m_iDevTotal;
	int m_iDevCards[CARD_SIZE];

	//numbers of starting roads, cities, and settlements
	int m_iStockShips;
	int m_iStockRoads;
	int m_iStockCities;
	int m_iStockSettlements;

	//bonuses for building on new islands
	int m_nBonus1;
	int m_nBonus2;

	//number of places starting roll chits can be placed
	int m_iRollPlaces[6];

	//numbers of randomly placeable resources in this map
	int m_iRandomRes[RES_SIZE];
	int m_iHiddenRes[RES_SIZE];
	int m_iChitlessRes[RES_SIZE];

	//numbers of randomly placeable chits in this map (2-11)
	int m_nRandomChit[11];
	int m_nHiddenChit[11];
	int m_nChitlessChit[11];

	//the board pieces
	vector <CMapPiece> m_mapPieces;

	//the borders, if any
	vector <CBorder> m_mapBorders;

	//the border edge points
	vector <CBorder> m_mapEdges;

	//any initial rotation on the map
	double m_dInitialRot;

	//inital depth to the map
	double m_dInitialDepth;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};

#endif // !defined(AFX_S3DMAP_H__DFBEA47B_30E2_4148_ADE6_F82A78DE47B5__INCLUDED_)
