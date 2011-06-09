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

#if !defined(AFX_SETTINGSPLAYERDISPLAYDIALOG_H__933C6CDF_A620_4F22_883A_8A02ABF70996__INCLUDED_)
#define AFX_SETTINGSPLAYERDISPLAYDIALOG_H__933C6CDF_A620_4F22_883A_8A02ABF70996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SettingsPlayerDisplayDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CSettingsPlayerDisplayDialog dialog

class CSettingsPlayerDisplayDialog : public CHelpDialog
{
// Construction
public:
	CSettingsPlayerDisplayDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSettingsPlayerDisplayDialog)
	enum { IDD = IDD_SETTINGS_PLAYER_DISPLAY };
	CButton	m_BankStatic;
	CButton	m_BankCheck;
	CButton	m_NetworkStatic;
	CButton	m_HotseatStatic;
	CButton	m_NetworkCheck;
	CButton	m_HotseatCheck;
	CButton	m_AutohideCheck;
	CButton	m_AnimateCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSettingsPlayerDisplayDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableAll(BOOL bEnable);

	// Generated message map functions
	//{{AFX_MSG(CSettingsPlayerDisplayDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAutohideCheck();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTINGSPLAYERDISPLAYDIALOG_H__933C6CDF_A620_4F22_883A_8A02ABF70996__INCLUDED_)
