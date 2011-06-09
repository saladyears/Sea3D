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

#if !defined(AFX_CMapDatabaseDialog_H__EDA40688_1F30_481B_A7E2_A7B6D60A1F7D__INCLUDED_)
#define AFX_CMapDatabaseDialog_H__EDA40688_1F30_481B_A7E2_A7B6D60A1F7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CMapDatabaseDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "MapListCtrl.h"
#include "BalloonHelp.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// dual use - database management and map selection
/////////////////////////////////////////////////////////////////////////////
class CMapDatabaseDialog : public CHelpDialog
{
// Construction
public:
	CMapDatabaseDialog(CWnd* pParent = NULL);   // standard constructor

	int inline getMap(void) {return m_iMap;}
	void inline setSelection(BOOL b) {m_bSelect = b;}

	//handle selection
	void handleSelection(void);

// Dialog Data
	//{{AFX_DATA(CMapDatabaseDialog)
	enum { IDD = IDD_DATABASE_MAPS };
	CButton	m_OKButton;
	CButton	m_CancelButton;
	CButton	m_ImportButton;
	CButton	m_ExportButton;
	CMapListCtrl	m_MapList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapDatabaseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//the selected map
	int m_iMap;

	//selection mode on?
	BOOL m_bSelect;

	//balloon help
	CBalloonHelp m_balloonList;
	CBalloonHelp m_balloonOK;

	// Generated message map functions
	//{{AFX_MSG(CMapDatabaseDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnClickMapList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CMapDatabaseDialog_H__EDA40688_1F30_481B_A7E2_A7B6D60A1F7D__INCLUDED_)
