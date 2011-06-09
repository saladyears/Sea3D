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

#if !defined(AFX_GAMELISTCTRL_H__9EBBF294_699D_42E1_8852_09ACCCBA1DDE__INCLUDED_)
#define AFX_GAMELISTCTRL_H__9EBBF294_699D_42E1_8852_09ACCCBA1DDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"
#include "GameInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CGameListCtrl window

class CGameListCtrl : public CSortListCtrl
{
// Construction
public:
	CGameListCtrl();

// Attributes
public:
	//function reads in the game info
	void readGames(void);

	//function that populates the list control
	void populateList(void);

	//set incomplete mode
	void setIncomplete(BOOL b) {m_bIncomplete = b;}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGameListCtrl();

	// Generated message map functions
protected:
	//array of game information headers
	vector <CGameInfo> m_gameInfo;

	//are we reading incomplete games?
	BOOL m_bIncomplete;

	//{{AFX_MSG(CGameListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMELISTCTRL_H__9EBBF294_699D_42E1_8852_09ACCCBA1DDE__INCLUDED_)
