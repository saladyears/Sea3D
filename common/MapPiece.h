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

#if !defined(AFX_MAPPIECE_H__6DEDE297_145D_4A5E_B194_613A5D72BD4A__INCLUDED_)
#define AFX_MAPPIECE_H__6DEDE297_145D_4A5E_B194_613A5D72BD4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#ifndef MAPS3D
#include "NDKMessage.h"
#endif

//////////////////////////////////////////////////////////////////////
// class which holds information about a piece in a map file
//////////////////////////////////////////////////////////////////////
class CMapPiece : public CObject  
{
public:
	DECLARE_SERIAL(CMapPiece)

	//{{AFX_VIRTUAL(CMapPiece)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CMapPiece(int iIndex = -1);
	CMapPiece(const CMapPiece &data);
	virtual ~CMapPiece();

	//assignment operator
	CMapPiece &operator =(const CMapPiece &data);

	//streamline copy function
	void copy(const CMapPiece &data);

#ifndef MAPS3D
	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(int &iStart, CNDKMessage &msg);
	void SerializeFromMessage(int &iStart, CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////
	
	//the index number of this piece
	int m_iIndex;

	//the island number of this piece (useful in Great Crossing scenarios)
	int m_nIsland;

	//what kind of piece is it (land, sea, or random)
	int m_iPieceType;

	//does it have a set type already (i.e. ore, timber, etc.)
	int m_iPermaType;

	//does it have a set roll value and texture
	int m_iPermaRoll;			//the roll value
	int m_iPermaRollTex;		//the texture used
	int m_iPermaRollTexType;	//settlers or seafarers chip types

	//does it have a permanent port side?
	int m_iPermaPortSide;

	//can this piece be built on in the initial placement?
	BOOL m_bInitial;

	//is this piece a secondary placement (i.e. worthy of extra points)?
	BOOL m_bSecondary;

	//can this piece be overwritten by a border?
	BOOL m_bBorder;
	
	//neighboring hexes
	int m_iSides[6];

	//openGL coordinates
	double m_glCoords[3];

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};

#endif // !defined(AFX_MAPPIECE_H__6DEDE297_145D_4A5E_B194_613A5D72BD4A__INCLUDED_)
