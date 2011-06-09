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

#if !defined(AFX_UNDOGOLD_H__D846643F_AD42_405B_B4C4_665521A6F16B__INCLUDED_)
#define AFX_UNDOGOLD_H__D846643F_AD42_405B_B4C4_665521A6F16B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for trading in gold
//////////////////////////////////////////////////////////////////////
class CUndoGold : public CUndo  
{
public:
	CUndoGold(int *pRes);
	virtual ~CUndoGold();

	//the amount of resources each person got from the gold trade
	int m_iResources[6][5];
};

#endif // !defined(AFX_UNDOGOLD_H__D846643F_AD42_405B_B4C4_665521A6F16B__INCLUDED_)
