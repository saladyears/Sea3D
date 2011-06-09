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

#if !defined(AFX_SPECIALTABCTRL_H__648274E6_3A3B_4B42_B804_288C78C04963__INCLUDED_)
#define AFX_SPECIALTABCTRL_H__648274E6_3A3B_4B42_B804_288C78C04963__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SpecialTabCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "CustomTabCtrl.h"
#include "SpecialWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CSpecialTabCtrl window
/////////////////////////////////////////////////////////////////////////////
class CSpecialTabCtrl : public CCustomTabCtrl
{
// Construction
public:
	CSpecialTabCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpecialTabCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpecialTabCtrl();

// Data members
protected:
	CSpecialWnd *m_pSpecialWnd;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSpecialTabCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPECIALTABCTRL_H__648274E6_3A3B_4B42_B804_288C78C04963__INCLUDED_)
