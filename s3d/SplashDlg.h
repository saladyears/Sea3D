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

#if !defined(AFX_SPLASHDLG_H__5C9A7465_96E1_499E_B70F_F011D4B4D947__INCLUDED_)
#define AFX_SPLASHDLG_H__5C9A7465_96E1_499E_B70F_F011D4B4D947__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplashDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BtnST.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSplashDlg dialog

class CSplashDlg : public CHelpDialog
{
// Construction
public:
	CSplashDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSplashDlg)
	enum { IDD = IDD_SPLASH_DLG };
	CButton	m_SplashButton;
	CButtonST	m_NetworkButton;
	CButtonST	m_HotseatButton;
	CButtonST	m_DisclaimerButton;
	CButtonST	m_SkipButton;
	CButtonST	m_TourButton;
	CButtonST	m_OpenButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont m_font;
	// Generated message map functions
	//{{AFX_MSG(CSplashDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnOpenButton();
	afx_msg void OnTourButton();
	afx_msg void OnSkipButton();
	afx_msg void OnDisclaimerButton();
	afx_msg void OnHotseatButton();
	afx_msg void OnNetworkButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASHDLG_H__5C9A7465_96E1_499E_B70F_F011D4B4D947__INCLUDED_)
