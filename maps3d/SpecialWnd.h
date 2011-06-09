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

#if !defined(AFX_SPECIALWND_H__F6A198C6_2364_42F6_8D71_589E6A52AC28__INCLUDED_)
#define AFX_SPECIALWND_H__F6A198C6_2364_42F6_8D71_589E6A52AC28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecialWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SpecialWnd.h"
#include "HoverStatic.h"
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CSpecialWnd window
/////////////////////////////////////////////////////////////////////////////
class CSpecialWnd : public CWnd
{
// Construction
public:
	CSpecialWnd();

// Attributes
public:

// Operations
public:
	void setMode(int nMode);
	void updateSpecial(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialWnd)
	//}}AFX_VIRTUAL
//Data members
protected:
	//the current display mode
	int m_nMode;

	//display statics
	CHoverStatic m_TilesStatic;
	CHoverStatic m_TilesSelectedStatic;
	CHoverStatic m_ChitsStatic;
	CHoverStatic m_ChitsSelectedStatic;

	//buttons
	CButtonST m_TilesButton;
	CButtonST m_ChitsButton;

	//the font
	CFont m_font;

// Implementation
public:
	virtual ~CSpecialWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpecialWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALWND_H__F6A198C6_2364_42F6_8D71_589E6A52AC28__INCLUDED_)
