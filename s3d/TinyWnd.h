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

#if !defined(AFX_TINYWND_H__2FC8EDCB_8247_49C8_9B17_3325668DAC4E__INCLUDED_)
#define AFX_TINYWND_H__2FC8EDCB_8247_49C8_9B17_3325668DAC4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TinyWnd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BtnST.h"

/////////////////////////////////////////////////////////////////////////////
// CTinyWnd window

class CTinyWnd : public CWnd
{
// Construction
public:
	CTinyWnd();

// Attributes
public:
	void setType(int i) {m_iType = i;}
	void updateTiny(BOOL bInitial = FALSE);

// Operations
public:
	void init(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTinyWnd)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTinyWnd();

	// Generated message map functions
protected:
	//handle buying a dev card
	void buyDevCard(void);

	//{{AFX_MSG(CTinyWnd)
	afx_msg void OnButton();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//the button
	CButton m_button;

	//trade buttons
	CButtonST m_trades[5];

	//the tooltip
	CToolTipCtrl m_ToolTip;

	//is this an initial placement
	BOOL m_bInitial;

	//the window type
	int m_iType;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TINYWND_H__2FC8EDCB_8247_49C8_9B17_3325668DAC4E__INCLUDED_)
