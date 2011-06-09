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
#include "UndoEnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CUndoEnd::CUndoEnd(int *nCards, BOOL bPlayed, BOOL bMoved, CArray <int, int> *pShips, CArray <int, int> *pPorts)
{
	int i;

	//cards bought
	memcpy(m_nCards, nCards, sizeof(m_nCards));

	//card played?
	m_bPlayed = bPlayed;

	//ship moved?
	m_bShipMoved = bMoved;

	//ships built
	for(i = 0; i < pShips->GetSize(); i++)
	{
		m_nShipsPlaced.Add(pShips->GetAt(i));
	}

	//ports built
	for(i = 0; i < pPorts->GetSize(); i++)
	{
		m_nPortsPlaced.Add(pPorts->GetAt(i));
	}
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CUndoEnd::~CUndoEnd()
{
}
