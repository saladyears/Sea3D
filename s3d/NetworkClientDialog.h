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

#if !defined(AFX_NETWORKCLIENTDIALOG_H__20670ED1_C07F_4CC2_AD0F_8DA1ABB6A8E0__INCLUDED_)
#define AFX_NETWORKCLIENTDIALOG_H__20670ED1_C07F_4CC2_AD0F_8DA1ABB6A8E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkClientDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "NetworkListCtrl.h"
#include "ChatControl.h"
#include "HelpDialog.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkClientDialog dialog

class CNetworkClientDialog : public CHelpDialog
{
// Construction
public:
	CNetworkClientDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetworkClientDialog)
	enum { IDD = IDD_NETWORK_CLIENT };
	CButton	m_ConnectorButton;
	CButton	m_RecentIPButton;
	CChatControl	m_InfoEdit;
	CButton			m_AdvancedButton;
	CButton			m_ConnectButton;
	CStatic			m_TCPStatic;
	CChatControl	m_SendEdit;
	CChatControl	m_ChatEdit;
	CIPAddressCtrl	m_IPAddress;
	CNetworkListCtrl	m_PlayerList;
	//}}AFX_DATA

	BOOL initComm(CString strAddress, UINT uiPort);

	//kill the timer
	void killTimer(void) {KillTimer(m_uiTimeOut);}

	//connect or disconnect
	void disconnect(void);
	void connect(void);

	//all donw
	void handleOK(void) {CHelpDialog::OnOK();}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkClientDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//accelerator resource
	HACCEL m_hAccelTable;

	UINT m_uiPort;

	UINT m_uiTimeOut;

	//create a CStringArray of recent IPs
	void getRecentIP(CStringArray &strArray);

	void enableIP(void);

	void parseIPString(CString str);

	//run the connector
	void runConnector(int nIndex);

	// Generated message map functions
	//{{AFX_MSG(CNetworkClientDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnConnectButton();
	afx_msg void OnSendButton();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnAdvancedButton();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRecentipButton();
	afx_msg void OnEditPaste();
	afx_msg void OnConnectorButton();
	afx_msg void OnReadyButton();
	//}}AFX_MSG
	void HandlePopupClick(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
	CButton m_ReadyButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKCLIENTDIALOG_H__20670ED1_C07F_4CC2_AD0F_8DA1ABB6A8E0__INCLUDED_)
