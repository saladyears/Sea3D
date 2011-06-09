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

#if !defined(AFX_CUSTOMLISTCTRL_H__06C9ED95_92D3_4698_81A0_62A859DF7007__INCLUDED_)
#define AFX_CUSTOMLISTCTRL_H__06C9ED95_92D3_4698_81A0_62A859DF7007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl window

class CCustomListCtrl : public CListCtrl
{
// Construction
public:
	CCustomListCtrl();

// Attributes
public:
	//clear the current selection
	void resetSel(void) {m_nSelection = -1;}

	//set the icon size
	void setIconSize(int i) {m_nIconSize = i;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCustomListCtrl)
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

//Data members
protected:
	int m_nSelection;

	//icon size
	int m_nIconSize;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMLISTCTRL_H__06C9ED95_92D3_4698_81A0_62A859DF7007__INCLUDED_)
