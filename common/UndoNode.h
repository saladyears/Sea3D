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

#if !defined(AFX_UNDONODE_H__3761CDBD_8B66_4309_8860_B5389398F4C2__INCLUDED_)
#define AFX_UNDONODE_H__3761CDBD_8B66_4309_8860_B5389398F4C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "Undo.h"

//////////////////////////////////////////////////////////////////////
// one node in the linked list of undo actions
//////////////////////////////////////////////////////////////////////
class CUndoNode : public CObject  
{
public:
	DECLARE_SERIAL(CUndoNode)

	//{{AFX_VIRTUAL(CUndoNode)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CUndoNode(int i = -1, CUndo *p = NULL);
	CUndoNode(const CUndoNode &data);
	virtual ~CUndoNode();

	//assignment operator
	CUndoNode &operator =(const CUndoNode &data);

	//streamline copy function
	void copy(const CUndoNode &data);

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////
	
	//the undo action
	int m_iAction;

	//pointer to the data for the undo action
	CUndo *m_pData;
	
private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};


#endif // !defined(AFX_UNDONODE_H__3761CDBD_8B66_4309_8860_B5389398F4C2__INCLUDED_)
