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

#if !defined(AFX_PLAYERLISTCTRL_H__0B1C9807_D9D4_4BE8_8E23_C491C3474F03__INCLUDED_)
#define AFX_PLAYERLISTCTRL_H__0B1C9807_D9D4_4BE8_8E23_C491C3474F03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerListCtrl.h : header file
//

//////////////////////////////////////////////////////////////////////
// typedefs
//////////////////////////////////////////////////////////////////////
typedef void (*LPFNSELCHANGE)(int);

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerListCtrl window

class CPlayerListCtrl : public CSortListCtrl
{
// Construction
public:
	CPlayerListCtrl();

// Attributes
public:
	void init(BOOL bStats = TRUE, BOOL bForeign = TRUE, BOOL bLadder = TRUE);

	//loads and saves the player database
	void loadPlayers(void);

	//populate stat data
	void populateRow(int iPlayer, int iRow);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPlayerListCtrl();

	// Generated message map functions
protected:
	BOOL m_bStats;
	BOOL m_bForeign;
	BOOL m_bLadder;

	//{{AFX_MSG(CPlayerListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERLISTCTRL_H__0B1C9807_D9D4_4BE8_8E23_C491C3474F03__INCLUDED_)
