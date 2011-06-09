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

#if !defined(AFX_CUSTOMLISTCTRL_H__336D6CC5_2B76_42DB_817D_6695AC2129AA__INCLUDED_)
#define AFX_CUSTOMLISTCTRL_H__336D6CC5_2B76_42DB_817D_6695AC2129AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CustomListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"
#include "Player.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomListCtrl window
/////////////////////////////////////////////////////////////////////////////
class CCustomListCtrl : public CSortListCtrl
{
// Construction
public:
	CCustomListCtrl();

	//return the current selection
	int getSelection(void);

	//add a player to the list
	void addPlayer(CPlayer *player, int iColor);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCustomListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCustomListCtrl();

	// Generated message map functions
protected:

	//{{AFX_MSG(CCustomListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CUSTOMLISTCTRL_H__336D6CC5_2B76_42DB_817D_6695AC2129AA__INCLUDED_)
