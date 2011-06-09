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

#if !defined(AFX_NEWPLAYERDIALOG_H__17B8A15E_C815_497B_9A06_B99CF9D2AF08__INCLUDED_)
#define AFX_NEWPLAYERDIALOG_H__17B8A15E_C815_497B_9A06_B99CF9D2AF08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewPlayerDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "ColorComboBox.h"
#include "Player.h"
#include "BalloonHelp.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNewPlayerDialog dialog

class CNewPlayerDialog : public CHelpDialog
{
// Construction
public:
	CNewPlayerDialog(CWnd* pParent = NULL);   // standard constructor

	//set the edit flag
	void inline setEdit(void) {m_bEdit = TRUE;}
	void inline setPlayer(CPlayer p) {m_Player = p;}
	CPlayer inline getPlayer(void) {return m_Player;}

	//set tour first value
	void setFirst(BOOL b) {m_bFirst = b;}

// Dialog Data
	//{{AFX_DATA(CNewPlayerDialog)
	enum { IDD = IDD_NEW_PLAYER_DLG };
	CButton	m_UpdateButton;
	CButton	m_DownloadButton;
	CButton	m_UnregisterButton;
	CButton	m_RegisterButton;
	CButton	m_LadderCheck;
	CStatic	m_LastNameStatic;
	CStatic	m_HandleStatic;
	CButton	m_OneNameCheck;
	CButton	m_OKButton;
	CComboBox	m_LangCombo;
	CButton	m_Lang1Check;
	CButton	m_Lang2Check;
	CButton	m_Lang3Check;
	CButton	m_Lang4Check;
	CButton	m_Lang5Check;
	CButton	m_Lang6Check;
	CButton	m_Lang7Check;
	CButton	m_Lang8Check;
	CButton	m_Lang9Check;
	CButton	m_Lang10Check;
	CButton	m_Lang11Check;
	CButton	m_Lang12Check;
	CButton	m_Lang13Check;
	CButton	m_Lang14Check;
	CButton	m_Lang15Check;
	CButton	m_Lang16Check;
	CColorComboBox	m_Color2Combo;
	CColorComboBox	m_Color1Combo;
	CEdit	m_LastNameCtrl;
	CEdit	m_FirstNameCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewPlayerDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//init everything
	void initDialog(void);

	//see if we've got a valid name
	BOOL validateName(BOOL bEmpty = TRUE);

	int m_iSel;
	BOOL m_bEdit;

	CPlayer m_Player;

	//tour ballooons
	BOOL m_bFirst;
	CBalloonHelp m_balloonOK;

	//did they just create a ladder player
	BOOL m_bLadderNow;

	// Generated message map functions
	//{{AFX_MSG(CNewPlayerDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLangCombo();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnOnenameCheck();
	afx_msg void OnLadderCheck();
	afx_msg void OnRegisterButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedDownloadButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWPLAYERDIALOG_H__17B8A15E_C815_497B_9A06_B99CF9D2AF08__INCLUDED_)
