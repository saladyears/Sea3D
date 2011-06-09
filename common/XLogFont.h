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

#if !defined(AFX_XLOGFONT_H_)
#define AFX_XLOGFONT_H_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// font defines
/////////////////////////////////////////////////////////////////////////////
#define Arial13				GetLogFont(13, "Arial")
#define Arial13B			GetLogFont(13, "Arial", true)
#define Arial14				GetLogFont(14, "Arial")
#define Arial14B			GetLogFont(14, "Arial", true)
#define Arial14I			GetLogFont(14, "Arial", false, true)
#define Arial14BI			GetLogFont(14, "Arial", true, true)
#define Arial15				GetLogFont(15, "Arial")
#define Arial15B			GetLogFont(15, "Arial", true)
#define Arial15I			GetLogFont(15, "Arial", false, true)
#define Arial15BI			GetLogFont(15, "Arial", true, true)
#define Arial18BI			GetLogFont(18, "Arial", true, true)
#define Tahoma11			GetLogFont(11, "Tahoma")
#define Tahoma12			GetLogFont(12, "Tahoma")
#define Tahoma12B			GetLogFont(12, "Tahoma", true)
#define Tahoma13			GetLogFont(13, "Tahoma")
#define Tahoma14			GetLogFont(14, "Tahoma")
#define Tahoma18B			GetLogFont(18, "Tahoma", true)
#define Verdana12			GetLogFont(12, "Verdana")
#define Verdana13B			GetLogFont(13, "Verdana", true)
#define Verdana14B			GetLogFont(14, "Verdana", true)

/////////////////////////////////////////////////////////////////////////////
// font functions
/////////////////////////////////////////////////////////////////////////////
const LOGFONT GetLogFont(int iHeight = 14, CString szName = "Arial", bool bBold = false, bool bItalic = false);

//////////////////////////////////////////////////////////////////
//class for manipulating fonts
//////////////////////////////////////////////////////////////////
class XLOGFONT{
	public:
		LOGFONT font;
		operator LOGFONT() {return font;}
		XLOGFONT &operator = (const XLOGFONT &lf);
		XLOGFONT &operator = (const LOGFONT &lf);
		BOOL operator ==(const XLOGFONT &lf);

		friend CArchive &operator >> (CArchive &in, XLOGFONT &x);
		friend CArchive &operator << (CArchive &out, XLOGFONT &x);
};

#endif // !defined(AFX_XLOGFONT_H_)