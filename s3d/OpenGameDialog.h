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

#if !defined(AFX_OPENGAMEDIALOG_H__A4CCE22C_184F_41E2_B107_1D68E182429C__INCLUDED_)
#define AFX_OPENGAMEDIALOG_H__A4CCE22C_184F_41E2_B107_1D68E182429C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OpenGameDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "GameListCtrl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// COpenGameDialog dialog

class COpenGameDialog : public CHelpDialog
{
// Construction
public:
	COpenGameDialog(CWnd* pParent = NULL);   // standard constructor

	//get the selection
	int inline getFile(void) {return m_iFile;}

	//set the incomplete flag
	void setIncomplete(BOOL b) {m_bIncomplete = b;}

	//handle selection
	void handleSelection(void);

// Dialog Data
	//{{AFX_DATA(COpenGameDialog)
	enum { IDD = IDD_DATABASE_GAME };
	CButton	m_ImportButton;
	CButton	m_ExportButton;
	CButton	m_OpenButton;
	CButton	m_DeleteButton;
	CButton m_RepostButton;
	CGameListCtrl	m_GameList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenGameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//export games from the database
	void exportGames(CString strFile, vector <int> iGames);

	//are we the incomplete games dialog?
	BOOL m_bIncomplete;

	//the file to open
	int m_iFile;

	// Generated message map functions
	//{{AFX_MSG(COpenGameDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnOpenButton();
	afx_msg void OnDeleteButton();
	afx_msg void OnExportButton();
	afx_msg void OnImportButton();
	afx_msg void OnRepostButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGAMEDIALOG_H__A4CCE22C_184F_41E2_B107_1D68E182429C__INCLUDED_)
