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

#if !defined(AFX_BANKBUTTON_H__4AC67161_8E5D_4603_8A56_0DDBBF3AD35C__INCLUDED_)
#define AFX_BANKBUTTON_H__4AC67161_8E5D_4603_8A56_0DDBBF3AD35C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BankButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "AnimateButton.h"

/////////////////////////////////////////////////////////////////////////////
// CBankButton window
/////////////////////////////////////////////////////////////////////////////
class CBankButton : public CAnimateButton
{
// Construction
public:
	CBankButton();

// Attributes
public:

// Operations
public:

	//update the resource counts
	void updateCounts(void);

	//overrides
	void updateAutohide(BOOL bFirst = FALSE);
	void handleAutohide(void);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBankButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBankButton();

	// Generated message map functions
protected:

	//tooltip setup
	void initToolTips(void);

	//overrides
	void showItems(BOOL bShow);
	void moveOffset(int nOffset);

	//bitmap buttons
	CStatic m_btnBank[6];

	//tooltip controls for the buttons
	CToolTipCtrl m_ToolTip[6];

	//title
	CHoverStatic m_stTitle;

	//large fonts?
	BOOL m_bLarge;

	//auto-hide offsets and variables
	BOOL m_bBank;

	//{{AFX_MSG(CBankButton)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANKBUTTON_H__4AC67161_8E5D_4603_8A56_0DDBBF3AD35C__INCLUDED_)
