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

#if !defined(AFX_SETTINGSGENERALDIALOG_H__315D9001_A90D_4944_909F_922DCBE83D80__INCLUDED_)
#define AFX_SETTINGSGENERALDIALOG_H__315D9001_A90D_4944_909F_922DCBE83D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsGeneralDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneralDialog dialog

class CSettingsGeneralDialog : public CHelpDialog
{
// Construction
public:
	CSettingsGeneralDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingsGeneralDialog)
	enum { IDD = IDD_SETTINGS_GENERAL_DLG };
	CButton	m_AutorollHCheck;
	CButton	m_WaitCheck;
	CEdit	m_TimeoutEdit;
	CButton	m_AutorollCheck;
	CButton	m_EuroCheck;
	CButton	m_FontsCheck;
	CButton	m_AutoTradeCheck;
	CButton	m_SoundsCheck;
	CButton m_AnimateCheck;
	CButton m_EmbedCheck;
	CButton m_SplashCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsGeneralDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSettingsGeneralDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSGENERALDIALOG_H__315D9001_A90D_4944_909F_922DCBE83D80__INCLUDED_)
