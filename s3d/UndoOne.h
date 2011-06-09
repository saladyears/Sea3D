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

#if !defined(AFX_UNDOONE_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_)
#define AFX_UNDOONE_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// undo data for a roll placement
//////////////////////////////////////////////////////////////////////
class CUndoOne : public CUndo  
{
public:
	CUndoOne(int iOne = 0);
	virtual ~CUndoOne();

	//the selected tile
	int m_iOne;
};

#endif // !defined(AFX_UNDOONE_H__887602E2_5F90_442C_B6A2_761BEF8C5C81__INCLUDED_)
