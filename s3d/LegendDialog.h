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

#if !defined(AFX_LEGENDDIALOG_H__2E6C48D0_8BD5_43FC_AA9E_366B5B6E2B0B__INCLUDED_)
#define AFX_LEGENDDIALOG_H__2E6C48D0_8BD5_43FC_AA9E_366B5B6E2B0B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LegendDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "LegendButton.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CLegendDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CLegendDialog : public CHelpDialog
{
// Construction
public:
	CLegendDialog(CWnd* pParent = NULL);   // standard constructor

	//pass through
	void setIndex(int i) {m_legend.setIndex(i);}

// Dialog Data
	//{{AFX_DATA(CLegendDialog)
	enum { IDD = IDD_LEGEND_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLegendDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CLegendButton m_legend;

	//hide the menu check
	void hideMenu(void);

	// Generated message map functions
	//{{AFX_MSG(CLegendDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEGENDDIALOG_H__2E6C48D0_8BD5_43FC_AA9E_366B5B6E2B0B__INCLUDED_)
