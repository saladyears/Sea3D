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
#include "ColorDatabase.h"
#include "defineColors.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// static instantiations
/////////////////////////////////////////////////////////////////////////////
CColorDatabase *			CColorDatabase::m_pColorDatabase;
CArray <CColor, CColor>		CColorDatabase::m_colors;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CColorDatabase::CColorDatabase()
{

}

CColorDatabase::~CColorDatabase()
{

}

/////////////////////////////////////////////////////////////////////////////
// instance handling
/////////////////////////////////////////////////////////////////////////////
CColorDatabase *CColorDatabase::instance()
{
	//see if a game database already is open
	if(NULL == m_pColorDatabase)
	{
		//get a new audit handle
		m_pColorDatabase = new CColorDatabase();

		//get preliminary information
		open();
	}

	//return the instance
	return m_pColorDatabase;
}

/////////////////////////////////////////////////////////////////////////////
// release data
/////////////////////////////////////////////////////////////////////////////
void CColorDatabase::release()
{
	//delete the static instance if it exists
	if(NULL != m_pColorDatabase)
	{
		delete m_pColorDatabase;
		m_pColorDatabase = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the number of players in the database
/////////////////////////////////////////////////////////////////////////////
int CColorDatabase::numColors()
{
	return m_colors.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
void CColorDatabase::open(void)
{
	int i;

	//reset the player array
	m_colors.RemoveAll();

	//resize
	m_colors.SetSize(6);

	//cycle through
	for(i = 0; i < 6; i++)
	{
		switch(i)
		{
		case CR_WHITE:
			m_colors[i].m_strName = "White";
			m_colors[i].m_crText = RGB(235, 235, 235);
			m_colors[i].m_crDraw = RGB(235, 235, 235);
			break;
		case CR_RED:
			m_colors[i].m_strName = "Red";
			m_colors[i].m_crText = RGB(255, 188, 188);
			m_colors[i].m_crDraw = RGB(223, 32, 32);
			break;
		case CR_GREEN:
			m_colors[i].m_strName = "Green";
			m_colors[i].m_crText = RGB(170, 255, 170);
			m_colors[i].m_crDraw = RGB(0, 128, 0);
			break;
		case CR_BLUE:
			m_colors[i].m_strName = "Blue";
			m_colors[i].m_crText = RGB(179, 213, 255);
			m_colors[i].m_crDraw = RGB(32, 32, 223);
			break;
		case CR_BROWN:
			m_colors[i].m_strName = "Brown";
			m_colors[i].m_crText = RGB(209, 182, 156);
			m_colors[i].m_crDraw = RGB(100, 50, 0);
			break;
		case CR_ORANGE:
			m_colors[i].m_strName = "Orange";
			m_colors[i].m_crText = RGB(255, 210, 151);
			m_colors[i].m_crDraw = RGB(255, 131, 6);
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// retrieve a color text name
/////////////////////////////////////////////////////////////////////////////
CString CColorDatabase::getColorName(int iColor)
{
	return m_colors[iColor].m_strName;
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
COLORREF CColorDatabase::getTextColor(int iColor)
{
	return m_colors[iColor].m_crText;
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
COLORREF CColorDatabase::getDrawColor(int iColor)
{
	return m_colors[iColor].m_crDraw;
}

/////////////////////////////////////////////////////////////////////////////
// search for a color by COLORREF
/////////////////////////////////////////////////////////////////////////////
int CColorDatabase::getColorByCREF(COLORREF crColor)
{
	int i;

	//run through and find
	for(i = 0; i < m_colors.GetSize(); i++)
	{
		if ((crColor == m_colors[i].m_crDraw) ||
			(crColor == m_colors[i].m_crText))
		{
			return i;
		}
	}

	//could not find it, but don't want to die
	return 0;
}