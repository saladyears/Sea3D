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
#include "UndoAction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CUndoAction::CUndoAction(int iRoll, BOOL bVolcano, vector <CString> strGoods)
{
	//roll
	m_iRoll = iRoll;
	
	//volcano
	m_bVolcano = bVolcano;

	//strings
	m_strGoods = strGoods;
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CUndoAction::~CUndoAction()
{

}
