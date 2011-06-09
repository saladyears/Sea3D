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

#if !defined(AFX_WINDIALOG_H__EEDDD1E9_6C54_4F70_B212_F65E1A1322C5__INCLUDED_)
#define AFX_WINDIALOG_H__EEDDD1E9_6C54_4F70_B212_F65E1A1322C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WinDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CWinDialog dialog

class CWinDialog : public CHelpDialog
{
// Construction
public:
	CWinDialog(CWnd* pParent = NULL);   // standard constructor

	//set the winner
	void inline setWinner(CString str) {m_strWinner = str;}

// Dialog Data
	//{{AFX_DATA(CWinDialog)
	enum { IDD = IDD_WIN_DLG };
	CStatic	m_WinStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//the name of the winner
	CString m_strWinner;

	// Generated message map functions
	//{{AFX_MSG(CWinDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDIALOG_H__EEDDD1E9_6C54_4F70_B212_F65E1A1322C5__INCLUDED_)
