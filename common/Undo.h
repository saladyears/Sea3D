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

#if !defined(AFX_UNDO_H__FBB3E737_CF89_4BC3_997D_635DD19F6A99__INCLUDED_)
#define AFX_UNDO_H__FBB3E737_CF89_4BC3_997D_635DD19F6A99__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// base class for necessary undo data
//////////////////////////////////////////////////////////////////////
class CUndo : public CObject  
{
public:
	DECLARE_SERIAL(CUndo)

	//{{AFX_VIRTUAL(CUndo)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CUndo();
	virtual ~CUndo();
	
private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};


#endif // !defined(AFX_UNDO_H__FBB3E737_CF89_4BC3_997D_635DD19F6A99__INCLUDED_)
