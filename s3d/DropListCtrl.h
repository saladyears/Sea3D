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

#if !defined(AFX_DROPLISTCTRL_H__40446D96_D70D_4977_A190_CFA9BED53577__INCLUDED_)
#define AFX_DROPLISTCTRL_H__40446D96_D70D_4977_A190_CFA9BED53577__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DropListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "TBTextTarget.h"
#include "PlayerListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CDropListCtrl window
/////////////////////////////////////////////////////////////////////////////
class CDropListCtrl : public CPlayerListCtrl, public TBTextTarget
{
// Construction
public:
	CDropListCtrl();

// Attributes
public:

// Operations
public:
	//data processing
	void ProcessData(CString strData);

	//drag/drop handlers
	HRESULT dragOver(void);
	HRESULT dragLeave(void);
	HRESULT dragEnter(void);
	void drop(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDropListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDropListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CDropListCtrl)
	afx_msg void OnCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DROPLISTCTRL_H__40446D96_D70D_4977_A190_CFA9BED53577__INCLUDED_)
