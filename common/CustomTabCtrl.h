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

#if !defined(AFX_CUSTOMTABCTRL_H__46E3F301_4982_4491_93A8_BC1DEDEC2886__INCLUDED_)
#define AFX_CUSTOMTABCTRL_H__46E3F301_4982_4491_93A8_BC1DEDEC2886__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomTabCtrl window

class CCustomTabCtrl : public CTabCtrl
{
// Construction
public:
	CCustomTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomTabCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomTabCtrl();

	//draw a tab
	void DrawTab(CDC *pDC, int nTab);

// Data members
protected:
	//the drawing font
	CFont m_font;

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMTABCTRL_H__46E3F301_4982_4491_93A8_BC1DEDEC2886__INCLUDED_)
