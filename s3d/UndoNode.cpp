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
#include "settlers.h"
#include "UndoNode.h"
#include "Undo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CUndoNode, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CUndoNode::CUndoNode(int iAction, CUndo *pData)
{
	m_iAction = iAction;
	m_pData = pData;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CUndoNode::CUndoNode(const CUndoNode &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// denstructor
//////////////////////////////////////////////////////////////////////
CUndoNode::~CUndoNode()
{
}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CUndoNode &CUndoNode::operator =(const CUndoNode &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CUndoNode::copy(const CUndoNode &data)
{
	//the undo action
	m_iAction			= data.m_iAction;

	//pointer to the data for the undo action
	m_pData				= data.m_pData;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CUndoNode::Serialize(CArchive &ar)
{
	//save information
	if(ar.IsStoring()){
		save(ar);
	}
	//load information
	else{
		load(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CUndoNode::load(CArchive &/*ar*/)
{
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CUndoNode::save(CArchive &/*ar*/)
{
}
