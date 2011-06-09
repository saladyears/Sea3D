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

#if !defined(AFX_UNDOSHIP_H__FDA7E0E2_A3FF_41B0_BAEE_AB74709E6627__INCLUDED_)
#define AFX_UNDOSHIP_H__FDA7E0E2_A3FF_41B0_BAEE_AB74709E6627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for a ship
//////////////////////////////////////////////////////////////////////
class CUndoShip : public CUndo  
{
public:
	CUndoShip(int ID, int nHidden, int nChitless, int nTurn = -1);
	virtual ~CUndoShip();

	//the standard ID
	int m_ID;

	//any hidden tile that was revealed
	int m_nHidden;

	//any chitless tiles that were revealed
	int m_nChitless;

	//the turn this moved ship was placed on
	int m_nTurn;
};

#endif // !defined(AFX_UNDOSHIP_H__FDA7E0E2_A3FF_41B0_BAEE_AB74709E6627__INCLUDED_)
