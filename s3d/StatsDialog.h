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

#if !defined(AFX_STATSDIALOG_H__F67030BC_2915_46D6_AFA4_2DB188B323B1__INCLUDED_)
#define AFX_STATSDIALOG_H__F67030BC_2915_46D6_AFA4_2DB188B323B1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatsDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "StatGrid.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CStatsDialog dialog

class CStatsDialog : public CHelpDialog
{
// Construction
public:
	CStatsDialog(CWnd* pParent = NULL);   // standard constructor
	~CStatsDialog();
	//initialize the stats block
	void init(void) {m_grid.init();}

	//update a stat block
	void update(int iBlock) {m_grid.update(iBlock);}

	//retrieves names
	CString getName(int iStat, BOOL &bHead);

// Dialog Data
	//{{AFX_DATA(CStatsDialog)
	enum { IDD = IDD_STATS_DLG };
	CTreeCtrl	m_StatTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//the stored width of the tree
	int m_iWidth;

	//the grid
	CStatGrid m_grid;

	//clear the menu check
	void hideMenu(void);

	// Generated message map functions
	//{{AFX_MSG(CStatsDialog)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangedStatTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATSDIALOG_H__F67030BC_2915_46D6_AFA4_2DB188B323B1__INCLUDED_)
