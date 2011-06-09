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

#if !defined(AFX_CONNECTORREGISTERDLG_H__1DF1422B_63C5_44EB_8107_E16064C8D440__INCLUDED_)
#define AFX_CONNECTORREGISTERDLG_H__1DF1422B_63C5_44EB_8107_E16064C8D440__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectorRegisterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CConnectorRegisterDlg dialog
/////////////////////////////////////////////////////////////////////////////
class CConnectorRegisterDlg : public CHelpDialog
{
// Construction
public:
	CConnectorRegisterDlg(CWnd* pParent = NULL);   // standard constructor

	//set the player we're working with
	void setPlayer(CPlayer player) {m_player = player;}

	//return the player we've worked with
	CPlayer getPlayer(void) {return m_player;}

// Dialog Data
	//{{AFX_DATA(CConnectorRegisterDlg)
	enum { IDD = IDD_CONNECTOR_REGISTER_DLG };
	CButton	m_ValidateButton;
	CButton	m_Step2Static;
	CButton	m_Step1Static;
	CStatic	m_RegisterStatusStatic;
	CButton	m_RegisterButton;
	CStatic	m_PasswordStatic;
	CEdit	m_PasswordEdit;
	CButton	m_OneNameCheck;
	CStatic	m_NameStatusStatic;
	CStatic	m_LastNameStatic;
	CEdit	m_LastNameEdit;
	CStatic	m_HandleStatic;
	CEdit	m_FirstNameEdit;
	CStatic	m_ConfirmStatic;
	CEdit	m_ConfirmEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectorRegisterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//check for a valid name
	void validateName(void);

	//enable/disable the sections
	void enableStep1(BOOL bEnable);
	void enableStep2(BOOL bEnable);

	//the current player
	CPlayer m_player;

	// Generated message map functions
	//{{AFX_MSG(CConnectorRegisterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOnenameCheck();
	afx_msg void OnValidateButton();
	afx_msg void OnRegisterButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTORREGISTERDLG_H__1DF1422B_63C5_44EB_8107_E16064C8D440__INCLUDED_)
