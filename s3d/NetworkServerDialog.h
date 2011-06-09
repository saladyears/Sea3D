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

#if !defined(AFX_NETWORKSERVERDIALOG_H__83849A91_3CD9_474B_B24B_CDB9B05A87CC__INCLUDED_)
#define AFX_NETWORKSERVERDIALOG_H__83849A91_3CD9_474B_B24B_CDB9B05A87CC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkServerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "NetworkListCtrl.h"
#include "ChatControl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkServerDialog dialog

class CNetworkServerDialog : public CHelpDialog
{
// Construction
public:
	CNetworkServerDialog(CWnd* pParent = NULL);   // standard constructor

	//return the selected map
	int getMap(void) {return m_iMap;}

	//set reload mode
	void setReload(BOOL b) {m_bReload = b;}

	//get reload mode
	BOOL getReload(void) {return m_bReload;}

	//see if we can continue a saved network game
	void checkForContinue(void);

	//start functions
	void startNormal(void);
	void startReload(void);

	//handles ladder restrictions
	void handleLadder(void);

// Dialog Data
	//{{AFX_DATA(CNetworkServerDialog)
	enum { IDD = IDD_NETWORK_SERVER };
	CButton	m_ConnectorButton;
	CEdit	m_TCPStatic;
	CButton	m_VariantsButton;
	CButton	m_BootButton;
	CButton	m_MapButton;
	CButton m_AIButton;
	CChatControl		m_InfoEdit;
	CButton				m_OKButton;
	CChatControl		m_SendEdit;
	CChatControl		m_ChatEdit;
	CNetworkListCtrl	m_PlayerList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkServerDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//accelerator resource
	HACCEL m_hAccelTable;

	//the map they want
	int m_iMap;

	//are in reload mode?
	BOOL m_bReload;

	//listening port
	UINT m_uiPort;

	//update the info string window
	void updateInfoString(void);

	// Generated message map functions
	//{{AFX_MSG(CNetworkServerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSendButton();
	virtual void OnCancel();
	afx_msg void OnAdvancedButton();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMapButton();
	virtual void OnOK();
	afx_msg void OnBootButton();
	afx_msg void OnVariantsButton();
	afx_msg void OnEditPaste();
	afx_msg void OnAiButton();
	afx_msg void OnConnectorButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKSERVERDIALOG_H__83849A91_3CD9_474B_B24B_CDB9B05A87CC__INCLUDED_)
