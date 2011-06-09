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

#if !defined(AFX_UNDOVOLCANO_H__9168C9F7_1EA3_41D4_9DDB_A718A695AC2D__INCLUDED_)
#define AFX_UNDOVOLCANO_H__9168C9F7_1EA3_41D4_9DDB_A718A695AC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for a volcano eruption
//////////////////////////////////////////////////////////////////////
class CUndoVolcano : public CUndo  
{
public:
	CUndoVolcano(int iRoll, int nPlayer, vector <int> nDestruction, vector <CString> strGoods);
	virtual ~CUndoVolcano();

	//the dice roll
	int m_iRoll;

	//the previous longest road owner
	int m_nLongestPlayer;

	//the array of destroyed objects
	vector <int> m_nDestruction;

	//the array of strings
	vector <CString> m_strGoods;
};

#endif // !defined(AFX_UNDOVOLCANO_H__9168C9F7_1EA3_41D4_9DDB_A718A695AC2D__INCLUDED_)
