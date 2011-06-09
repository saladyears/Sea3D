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

#if !defined(AFX_COLOR_H__4F111B5C_4880_43FA_8F29_42D67C057E30__INCLUDED_)
#define AFX_COLOR_H__4F111B5C_4880_43FA_8F29_42D67C057E30__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"

//////////////////////////////////////////////////////////////////////
// class which holds information about one color
//////////////////////////////////////////////////////////////////////
class CColor : public CObject
{
public:
	DECLARE_SERIAL(CColor)

	//{{AFX_VIRTUAL(CColor)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CColor();
	CColor(const CColor &data);
	virtual ~CColor();

	//assignment operator
	CColor &operator =(const CColor &data);

	//compare operator
	BOOL operator ==(const CColor &data);

	//streamline copy function
	void copy(const CColor &data);

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//color name
	CString m_strName;

	//text color
	COLORREF m_crText;

	//draw color
	COLORREF m_crDraw;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
};

#endif // !defined(AFX_COLOR_H__4F111B5C_4880_43FA_8F29_42D67C057E30__INCLUDED_)
