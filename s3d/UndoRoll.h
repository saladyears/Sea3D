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

#if !defined(AFX_UNDOROLL_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_)
#define AFX_UNDOROLL_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for a trade
//////////////////////////////////////////////////////////////////////
class CUndoRoll : public CUndo  
{
public:
	CUndoRoll(int iRoll, int *iGoods, int *iLost, int *nJungles);
	virtual ~CUndoRoll();

	//the number rolled
	int m_iRoll;

	//the number of jungles gained
	int m_nJungles[6];

	//the arrays of goods received and lost
	int m_iGoods[6][6];
	int m_iLost[6][6];
};

#endif // !defined(AFX_UNDOROLL_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_)
