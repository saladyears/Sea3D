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

#if !defined(AFX_PLAYERDATABASEDIALOG_H__0E3A9D47_3027_4491_B116_1D8A0F504B82__INCLUDED_)
#define AFX_PLAYERDATABASEDIALOG_H__0E3A9D47_3027_4491_B116_1D8A0F504B82__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayerDatabaseDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "PlayerListCtrl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CPlayerDatabaseDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CPlayerDatabaseDialog : public CHelpDialog
{
// Construction
public:
	CPlayerDatabaseDialog(CWnd* pParent = NULL);   // standard constructor

	//handle selection
	void handleSelection(void);

// Dialog Data
	//{{AFX_DATA(CPlayerDatabaseDialog)
	enum { IDD = IDD_DATABASE_PLAYER };
	CButton	m_EditButton;
	CButton	m_DeleteButton;
	CPlayerListCtrl	m_PlayerList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDatabaseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayerDatabaseDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnAddButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnEditButton();
	afx_msg void OnDblclkPlayerList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDATABASEDIALOG_H__0E3A9D47_3027_4491_B116_1D8A0F504B82__INCLUDED_)
