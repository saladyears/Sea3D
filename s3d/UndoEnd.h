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

#if !defined(AFX_UNDOEND_H__02CCD433_03F7_4963_AC2B_DAAC03F0DF15__INCLUDED_)
#define AFX_UNDOEND_H__02CCD433_03F7_4963_AC2B_DAAC03F0DF15__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for the end of a turn
//////////////////////////////////////////////////////////////////////
class CUndoEnd : public CUndo  
{
public:
	CUndoEnd(int *nCards, BOOL bPlayed, BOOL bMoved, CArray <int, int> *pShips, CArray <int, int> *pPorts);
	virtual ~CUndoEnd();

	//dev cards bought on this turn
	int m_nCards[5];

	//whether a card was played on this turn
	BOOL m_bPlayed;

	//whether a ship was moved on this turn
	BOOL m_bShipMoved;

	//array of ships placed on this turn
	CArray <int, int> m_nShipsPlaced;

	//array of ports built on this turn
	CArray <int, int> m_nPortsPlaced;
};

#endif // !defined(AFX_UNDOEND_H__02CCD433_03F7_4963_AC2B_DAAC03F0DF15__INCLUDED_)
