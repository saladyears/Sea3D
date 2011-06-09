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

#if !defined(AFX_SORTLISTCTRL_H__FF78FC7B_D046_4D9F_9CDD_812938C07304__INCLUDED_)
#define AFX_SORTLISTCTRL_H__FF78FC7B_D046_4D9F_9CDD_812938C07304__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();

	//return the selection
	int querySelection(void) {return m_nSelection;}

	//set the selection
	void setSelection(int i = -1) {m_nSelection = i; Invalidate();}

	//set the callback function
	void setCallBack(LPNCALLBACK lpfnClick, DWORD dwCookie) {m_lpfnClick = lpfnClick; m_dwCookie = dwCookie;}

// Attributes
public:
	void sort(void);

	//sorting variables
	int m_iSort[3];
	BOOL m_bReverse[3];

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	//the font of the list control
	CFont m_font;

	//current selection
	int m_nSelection;

	//callback stuff
	LPFNCALLBACK m_lpfnClick;
	DWORD m_dwCookie;

	//{{AFX_MSG(CSortListCtrl)
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTLISTCTRL_H__FF78FC7B_D046_4D9F_9CDD_812938C07304__INCLUDED_)
