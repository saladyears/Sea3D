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

#if !defined(AFX_NETWORKPLAYERDIALOG_H__852BD6C0_67B0_4951_8288_CC646298BA0B__INCLUDED_)
#define AFX_NETWORKPLAYERDIALOG_H__852BD6C0_67B0_4951_8288_CC646298BA0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkPlayerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "PlayerListCtrl.h"
#include "ColorComboBox.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayerDialog dialog

class CNetworkPlayerDialog : public CHelpDialog
{
// Construction
public:
	CNetworkPlayerDialog(CWnd* pParent = NULL);   // standard constructor

	//host or connect
	BOOL hostGame(void) {return m_bHost;}

	//player selected
	int playerSel(void) {return m_iPlayer;}
	void getColors(int &i1, int &i2) {i1 = m_iColor1; i2 = m_iColor2;}

	void handleSelection(void);

// Dialog Data
	//{{AFX_DATA(CNetworkPlayerDialog)
	enum { IDD = IDD_NETWORK_PLAYERS };
	CColorComboBox	m_Color2Combo;
	CColorComboBox	m_Color1Combo;
	CButton	m_OKButton;
	CButton m_EditButton;
	CPlayerListCtrl	m_PlayerList;
	CButton	m_HostRadio;
	CButton	m_ConnectRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkPlayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	int m_iPlayer;
	int m_iColor1;
	int m_iColor2;
	BOOL m_bHost;

	// Generated message map functions
	//{{AFX_MSG(CNetworkPlayerDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnAddButton();
	afx_msg void OnEditButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKPLAYERDIALOG_H__852BD6C0_67B0_4951_8288_CC646298BA0B__INCLUDED_)
