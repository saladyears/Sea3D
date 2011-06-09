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

#if !defined(AFX_NETWORKCONNECTORDIALOG_H__0025FF8E_A115_434D_AAA5_FB432B9993F0__INCLUDED_)
#define AFX_NETWORKCONNECTORDIALOG_H__0025FF8E_A115_434D_AAA5_FB432B9993F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkConnectorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "NetworkConnectorListCtrl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CNetworkConnectorDialog : public CHelpDialog
{
// Construction
public:
	CNetworkConnectorDialog(CWnd* pParent = NULL);   // standard constructor

	//handle selection
	void handleSelection(void);

	//return the game selection
	int getGameSel(void) {return m_nGameSel;}

// Dialog Data
	//{{AFX_DATA(CNetworkConnectorDialog)
	enum { IDD = IDD_NETWORK_CONNECTOR_DIALOG };
	CButton	m_90Radio;
	CButton	m_60Radio;
	CButton	m_30Radio;
	CButton	m_OffRadio;
	CButton	m_OKButton;
	CNetworkConnectorListCtrl	m_GameList;
	CStatic m_PlayersStatic;
	CStatic m_Player1Static;
	CStatic m_Player2Static;
	CStatic m_Player3Static;
	CStatic m_Player4Static;
	CStatic m_Player5Static;
	CStatic m_Player6Static;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkConnectorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void populateList(void);

	void parseString(int nIndex);

	//timer handling
	UINT m_uiTimer;

	//the final game selection
	int m_nGameSel;

	//tool tip controls
	CToolTipCtrl m_PlayerTips[6];

	// Generated message map functions
	//{{AFX_MSG(CNetworkConnectorDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void On30Radio();
	afx_msg void On60Radio();
	afx_msg void On90Radio();
	virtual void OnCancel();
	afx_msg void OnOffRadio();
	afx_msg void OnRefreshButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKCONNECTORDIALOG_H__0025FF8E_A115_434D_AAA5_FB432B9993F0__INCLUDED_)
