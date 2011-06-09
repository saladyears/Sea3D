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

#if !defined(AFX_BOARDPIECE_H__C8CA7C41_CF76_4BC9_A072_A8CF6051BCFA__INCLUDED_)
#define AFX_BOARDPIECE_H__C8CA7C41_CF76_4BC9_A072_A8CF6051BCFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"

//conditional includes
#ifndef AI_DLL
#include "NDKMessage.h"
#include "MapPiece.h"
#endif

//////////////////////////////////////////////////////////////////////
// class which holds information on one board piece
//////////////////////////////////////////////////////////////////////
class CBoardPiece : public CObject  
{
public:
	DECLARE_SERIAL(CBoardPiece)

	//{{AFX_VIRTUAL(CBoardPiece)
	public:
//conditional includes
#ifndef AI_DLL
	virtual void Serialize(CArchive& ar);
#endif
	//}}AFX_VIRTUAL

	CBoardPiece(int iIndex = -1);
	CBoardPiece(const CBoardPiece &data);
	virtual ~CBoardPiece();

	//assignment operator
	CBoardPiece &operator =(const CBoardPiece &data);

	//streamline copy function
	void copy(const CBoardPiece &data);

//conditional includes
#ifndef AI_DLL
	//set data from a map piece
	void copyFromMap(CMapPiece *pPiece);
#endif

public:
	//////////////////////////////////////////////////////////////////
	// public set methods
	//////////////////////////////////////////////////////////////////
	inline void setType(int i) {m_iType = i;}
	inline void setRoll(int i) {m_iRoll = i;}
	inline void setRollTex(int i) {m_iRollTex = i;}
	inline void setRollTexType(int i) {m_iRollTexType = i;}
	inline void setUsed(int i, BOOL b) {m_bRoadUsed[i] = b;}
	inline void setRotation(double d) {m_dRotate = d;}
	inline void setPort1(int i) {m_iPort1 = i;}
	inline void setPort2(int i) {m_iPort2 = i;}
	inline void setInitial(BOOL b) {m_bInitial = b;}
	inline void setChitless(BOOL b) {m_bChitless = b;}
	inline void setTurnPlacement(int i, int nTurn) {m_nTurnPlacements[i] = nTurn;}

	inline void addSettle(int iCorner, int iPlayer) {m_iCity[iCorner] = 10 + iPlayer;}
	inline void addRoad(int iSide, int iPlayer) {m_iRoads[iSide] = iPlayer;}
	inline void addShip(int iSide, int iPlayer) {m_iShips[iSide] = iPlayer;}
	inline void removeCity(int iCorner) {m_iCity[iCorner] = 0;}
	inline void removeRoad(int iSide) {m_iRoads[iSide] = -1;}
	inline void removeShip(int iSide) {m_iShips[iSide] = -1;}

	inline void makeCity(int iCorner) {m_iCity[iCorner] += 10;}
	inline void makeSettle(int iCorner) {m_iCity[iCorner] -= 10;}
	inline void setExtra(int iCorner, int i) {m_iExtra[iCorner] = i;}

	//////////////////////////////////////////////////////////////////
	// public get methods
	//////////////////////////////////////////////////////////////////
	inline int getType(void) {return m_iType;}
	inline int getSide(int i) {return m_iSides[i];}
	inline int getRoll(void) {return m_iRoll;}
	inline int getRollTex(void) {return m_iRollTex;}
	inline int getRollTexType(void) {return m_iRollTexType;}
	inline int getCity(int i) {return m_iCity[i];}
	inline int getRoad(int i) {return m_iRoads[i];}
	inline int getShip(int i) {return m_iShips[i];}
	inline int getPort1(void) {return m_iPort1;}
	inline int getPort2(void) {return m_iPort2;}
	inline int getExtra(int i) {return m_iExtra[i];}
	inline int getPortSide(void) {return m_nPortSide;}
	inline int getIsland(void) {return m_nIsland;}
	inline int getTurnPlacement(int i) {return m_nTurnPlacements[i];}
	inline BOOL getUsed(int i) {return m_bRoadUsed[i];}
	inline BOOL getInitial(void) {return m_bInitial;}
	inline BOOL getBorder(void) {return m_bBorder;}
	inline BOOL getSecondary(void) {return m_bSecondary;}
	inline BOOL getChitless(void) {return m_bChitless;}
	inline double getCoord(int i) {return m_coords[i];}
	inline double getRotation(void) {return m_dRotate;}

	int getLowestSide(void);
	int findSide(int iSide);

//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);
#endif

private:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////
	
	//the index number of this piece
	int m_iIndex;

	//the type of piece this is
	int m_iType;

	//the roll value on this piece
	int m_iRoll;
	int m_iRollTex;
	int m_iRollTexType;

	//the port corners on this tile
	int m_iPort1;
	int m_iPort2;

	//all the neighboring hexes
	int m_iSides[6];

	//whether the piece is blocked or not
	//BOOL m_bBlocked;		//RETIRED IN 0.8.6 -- WAS NEVER USED

	//the island identifier (for Greater Catan)
	int m_nIsland;			//ADDED in 1.0.4

	//is this tile chitless?
	BOOL m_bChitless;		//ADDED in 1.0.4

	//can this piece be built on initially? 
	BOOL m_bInitial;		//ADDED in 1.0.1

	//can this piece be overwritten by a border?
	BOOL m_bBorder;			//ADDED in 1.0.1

	//is this piece a secondary island?
	BOOL m_bSecondary;		//ADDED in 1.0.2

	//does this port have a permanent side?
	int m_nPortSide;		//ADDED in 1.0.2

	//GL coords
	double m_coords[3];

	//any amount of rotation needed
	double m_dRotate;

	//any roads built on this hex
	int m_iRoads[6];

	//any ships built on this hex
	int m_iShips[6];

	//the turn numbers of when the road or ship was placed there
	int m_nTurnPlacements[6];

	//roads used for longest road
	BOOL m_bRoadUsed[6];

	//any settlements/cities built on this hex
	int m_iCity[6];

	//any extra points for settlements/cities on this hex
	int m_iExtra[6];
	
private:
//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

	//////////////////////////////////////////////////////////////////////
	// random rotation value
	//////////////////////////////////////////////////////////////////////
	double randRot(void);
#endif
};

#endif // !defined(AFX_BOARDPIECE_H__C8CA7C41_CF76_4BC9_A072_A8CF6051BCFA__INCLUDED_)
