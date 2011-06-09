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

#if !defined(AFX_CONTROLPANELWND_H__63337AB6_61AE_42A8_9298_CF4413112BA3__INCLUDED_)
#define AFX_CONTROLPANELWND_H__63337AB6_61AE_42A8_9298_CF4413112BA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlPanelWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "TilesTabCtrl.h"
#include "SpecialTabCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CControlPanelWnd window
/////////////////////////////////////////////////////////////////////////////
class CControlPanelWnd : public CWnd
{
// Construction
public:
	CControlPanelWnd();

// Attributes
public:
	//set the currently selected resource
	void setResource(int nRes);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlPanelWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CControlPanelWnd();

// Data members
protected:
	CTilesTabCtrl *m_pTabTiles;
	CSpecialTabCtrl *m_pTabSpecial;

	// Generated message map functions
protected:
	//{{AFX_MSG(CControlPanelWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLPANELWND_H__63337AB6_61AE_42A8_9298_CF4413112BA3__INCLUDED_)
