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
#include "MapPiece.h"
#include "defineMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CMapPiece, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CMapPiece::CMapPiece(int iIndex)
{
	//index
	m_iIndex = iIndex;

	//default island
	m_nIsland = 0;

	//no piece type yet
	m_iPieceType = 0;

	//no permanent settings
	m_iPieceType = MAP_NOT_PERMANENT;
	m_iPermaType = MAP_NOT_PERMANENT;
	m_iPermaRoll = MAP_NOT_PERMANENT;
	m_iPermaRollTex = MAP_NOT_PERMANENT;
	m_iPermaRollTexType = MAP_NOT_PERMANENT;
	m_iPermaPortSide = MAP_NOT_PERMANENT;

	//allow initial placements by default
	m_bInitial = TRUE;

	//no secondary placements by default
	m_bSecondary = FALSE;

	//disallow border overwriting by default
	m_bBorder = FALSE;

	//no neighboring hexes
	memset(m_iSides, -1, sizeof(m_iSides));

	//no coordinates
	::ZeroMemory(m_glCoords, sizeof(m_glCoords));
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CMapPiece::CMapPiece(const CMapPiece &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CMapPiece::~CMapPiece()
{
}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CMapPiece &CMapPiece::operator =(const CMapPiece &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CMapPiece::copy(const CMapPiece &data)
{
	//the index number of this piece
	m_iIndex				= data.m_iIndex;

	//island number
	m_nIsland				= data.m_nIsland;

	//what kind of piece is it (land, sea, or random)
	m_iPieceType			= data.m_iPieceType;

	//does it have a set type already (i.e. ore, timber, etc.)
	m_iPermaType			= data.m_iPermaType;

	//does it have a set roll value and texture
	m_iPermaRoll			= data.m_iPermaRoll;
	m_iPermaRollTex			= data.m_iPermaRollTex;
	m_iPermaRollTexType		= data.m_iPermaRollTexType;

	//port side
	m_iPermaPortSide		= data.m_iPermaPortSide;

	//initial placement
	m_bInitial				= data.m_bInitial;

	//secondary placement
	m_bSecondary			= data.m_bSecondary;

	//border overwriting
	m_bBorder				= data.m_bBorder;
	
	//neighboring hexes
	memcpy(m_iSides, data.m_iSides, sizeof(m_iSides));

	//openGL coordinates
	memcpy(m_glCoords, data.m_glCoords, sizeof(m_glCoords));
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CMapPiece::Serialize(CArchive &ar)
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
void CMapPiece::load(CArchive &ar)
{
	//the index number of this piece
	ar >> m_iIndex;

	//island number
	ar >> m_nIsland;

	//what kind of piece is it (land, sea, or random)
	ar >> m_iPieceType;

	//does it have a set type already (i.e. ore, timber, etc.)
	ar >> m_iPermaType;

	//does it have a set roll value and texture
	ar >> m_iPermaRoll;
	ar >> m_iPermaRollTex;
	ar >> m_iPermaRollTexType;

	//port side
	ar >> m_iPermaPortSide;

	//initial placement
	ar >> m_bInitial;

	//secondary placement
	ar >> m_bSecondary;

	//border overwriting
	ar >> m_bBorder;
	
	//neighboring hexes
	ar.Read(m_iSides, sizeof(m_iSides));

	//openGL coordinates
	ar.Read(m_glCoords, sizeof(m_glCoords));
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CMapPiece::save(CArchive &ar)
{
	//the index number of this piece
	ar << m_iIndex;

	//island number
	ar << m_nIsland;

	//what kind of piece is it (land, sea, or random)
	ar << m_iPieceType;

	//does it have a set type already (i.e. ore, timber, etc.)
	ar << m_iPermaType;

	//does it have a set roll value and texture
	ar << m_iPermaRoll;
	ar << m_iPermaRollTex;
	ar << m_iPermaRollTexType;

	//port side
	ar << m_iPermaPortSide;

	//initial palcement
	ar << m_bInitial;

	//secondary placement
	ar << m_bSecondary;

	//border overwriting
	ar << m_bBorder;
	
	//neighboring hexes
	ar.Write(m_iSides, sizeof(m_iSides));

	//openGL coordinates
	ar.Write(m_glCoords, sizeof(m_glCoords));
}

#ifndef MAPS3D
//////////////////////////////////////////////////////////////////////
// write to an outgoing message
//////////////////////////////////////////////////////////////////////
void CMapPiece::SerializeToMessage(int &iStart, CNDKMessage &msg)
{
	//the index number of this piece
	msg.SetAt(iStart++,  m_iIndex);

	//island number
	msg.SetAt(iStart++, m_nIsland);

	//what kind of piece is it (land, sea, or random)
	msg.SetAt(iStart++,  m_iPieceType);

	//does it have a set type already (i.e. ore, timber, etc.)
	msg.SetAt(iStart++,  m_iPermaType);

	//does it have a set roll value and texture
	msg.SetAt(iStart++,  m_iPermaRoll);
	msg.SetAt(iStart++,  m_iPermaRollTex);
	msg.SetAt(iStart++,  m_iPermaRollTexType);

	//port side
	msg.SetAt(iStart++, m_iPermaPortSide);

	//initial placement
	msg.SetAt(iStart++,  m_bInitial);

	//secondary placement
	msg.SetAt(iStart++, m_bSecondary);

	//border overwriting
	msg.SetAt(iStart++,  m_bBorder);
	
	//neighboring hexes
	msg.SetAt(iStart++, m_iSides, sizeof(m_iSides));

	//openGL coordinates
	msg.SetAt(iStart++, m_glCoords, sizeof(m_glCoords));
}

//////////////////////////////////////////////////////////////////////
// read from an incoming message
//////////////////////////////////////////////////////////////////////
void CMapPiece::SerializeFromMessage(int &iStart, CNDKMessage &msg)
{
	//the index number of this piece
	msg.GetAt(iStart++,  m_iIndex);

	//island number
	msg.GetAt(iStart++, m_nIsland);

	//what kind of piece is it (land, sea, or random)
	msg.GetAt(iStart++,  m_iPieceType);

	//does it have a set type already (i.e. ore, timber, etc.)
	msg.GetAt(iStart++,  m_iPermaType);

	//does it have a set roll value and texture
	msg.GetAt(iStart++,  m_iPermaRoll);
	msg.GetAt(iStart++,  m_iPermaRollTex);
	msg.GetAt(iStart++,  m_iPermaRollTexType);

	//port side
	msg.GetAt(iStart++, m_iPermaPortSide);

	//initial placement
	msg.GetAt(iStart++,  m_bInitial);

	//secondary placement
	msg.GetAt(iStart++, m_bSecondary);

	//border overwriting
	msg.GetAt(iStart++,  m_bBorder);
	
	//neighboring hexes
	msg.GetAt(iStart++, m_iSides, sizeof(m_iSides));

	//openGL coordinates
	msg.GetAt(iStart++, m_glCoords, sizeof(m_glCoords));
}
#endif