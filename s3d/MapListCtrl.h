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

#if !defined(AFX_MAPLISTCTRL_H__1C18A3CD_FF72_4630_9336_1F1671257C87__INCLUDED_)
#define AFX_MAPLISTCTRL_H__1C18A3CD_FF72_4630_9336_1F1671257C87__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "SortListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CMapListCtrl window

class CMapListCtrl : public CSortListCtrl
{
// Construction
public:
	CMapListCtrl();

// Attributes
public:

	//loads the maps and populates list control
	void loadMaps(void);

	//populate map data
	void populateRow(int iIndex);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMapListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMapListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPLISTCTRL_H__1C18A3CD_FF72_4630_9336_1F1671257C87__INCLUDED_)
