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

#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_NETWORKCONNECTORSETUPDIALOG_H__80028A28_F7BD_45C8_BCBA_8D123DFA059E__INCLUDED_)
#define AFX_NETWORKCONNECTORSETUPDIALOG_H__80028A28_F7BD_45C8_BCBA_8D123DFA059E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkConnectorSetupDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorSetupDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CNetworkConnectorSetupDialog : public CHelpDialog
{
// Construction
public:
	CNetworkConnectorSetupDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetworkConnectorSetupDialog)
	enum { IDD = IDD_NETWORK_CONNECTOR_SETUP_DLG };
	CEdit	m_PortEdit;
	CIPAddressCtrl	m_IPAddress;
	CEdit	m_NameEdit;
	CButton	m_PrivateCheck;
	CEdit	m_PasswordEdit;
	CString	m_strPassword;
	BOOL	m_nPrivate;
	CString	m_strName;
	CString	m_strPort;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkConnectorSetupDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect m_rect;

	void parseIPString(CString str);
	CString getIPString(void);

	// Generated message map functions
	//{{AFX_MSG(CNetworkConnectorSetupDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnPrivateCheck();
	afx_msg void OnAdvancedButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_LadderCheck;
	CSpinButtonCtrl m_MaxSpin;
	CStatic m_MaxStatic;
	CSpinButtonCtrl m_CutoffSpin;
	CStatic m_CutoffStatic;
	CButton m_CutoffCheck;
	afx_msg void OnBnClickedCutoffCheck();
	CButton m_TeamCheck;
	afx_msg void OnBnClickedTeamCheck();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKCONNECTORSETUPDIALOG_H__80028A28_F7BD_45C8_BCBA_8D123DFA059E__INCLUDED_)
