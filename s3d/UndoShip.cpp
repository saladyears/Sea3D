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
#include "UndoShip.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CUndoShip::CUndoShip(int ID, int nHidden, int nChitless, int nTurn)
{
	//the standard ID
	m_ID = ID;

	//any hidden tile that was revealed
	m_nHidden = nHidden;

	//chitless tiles revealed
	m_nChitless = nChitless;

	//the turn this moved ship was place don
	m_nTurn = nTurn;
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CUndoShip::~CUndoShip()
{

}
