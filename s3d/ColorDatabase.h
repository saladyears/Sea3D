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

#if !defined(AFX_COLORDATABASE_H__61305F33_64A8_41FB_A52B_FB49D0EEE8EE__INCLUDED_)
#define AFX_COLORDATABASE_H__61305F33_64A8_41FB_A52B_FB49D0EEE8EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// instance definition
//////////////////////////////////////////////////////////////////////
#define COLORS		CColorDatabase::instance()

//////////////////////////////////////////////////////////////////////
// included files
//////////////////////////////////////////////////////////////////////
#include "Color.h"

//////////////////////////////////////////////////////////////////////
// static class for reading from the color database
//////////////////////////////////////////////////////////////////////
class CColorDatabase  
{
public:
	//////////////////////////////////////////////////////////////////
	// constructor / destructor
	//////////////////////////////////////////////////////////////////
	CColorDatabase();
	virtual ~CColorDatabase();

protected:
	/////////////////////////////////////////////////////////////////////////
	// a pointer to a static instance of this class
	/////////////////////////////////////////////////////////////////////////
	static CColorDatabase *m_pColorDatabase;

	//opens the database and gets information
	static void open(void);

	//the array of colors
	static CArray <CColor, CColor> m_colors;

public:
	/////////////////////////////////////////////////////////////////////
	// instanstiation/destruction methods
	/////////////////////////////////////////////////////////////////////
	static CColorDatabase *instance(void);
	static void release(void);

	//////////////////////////////////////////////////////////////////
	// information methods
	//////////////////////////////////////////////////////////////////
	static int numColors(void);

	//////////////////////////////////////////////////////////////////
	// database methods
	//////////////////////////////////////////////////////////////////
	static CString getColorName(int iColor);
	static COLORREF getTextColor(int iColor);
	static COLORREF getDrawColor(int iColor);

	static int getColorByCREF(COLORREF crColor);
};

#endif // !defined(AFX_COLORDATABASE_H__61305F33_64A8_41FB_A52B_FB49D0EEE8EE__INCLUDED_)
