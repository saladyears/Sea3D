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

#if !defined(AFX_NETWORKMESSAGEDIALOG_H__555D289A_0FD5_4EE9_B7E2_1169BFEC3D6D__INCLUDED_)
#define AFX_NETWORKMESSAGEDIALOG_H__555D289A_0FD5_4EE9_B7E2_1169BFEC3D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkMessageDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkMessageDialog dialog

class CNetworkMessageDialog : public CHelpDialog
{
// Construction
public:
	CNetworkMessageDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNetworkMessageDialog)
	enum { IDD = IDD_NETWORK_MESSAGES };
	CButton	m_Check1;
	CButton	m_Check2;
	CButton	m_Check3;
	CButton	m_Check4;
	CButton	m_Check5;
	CButton	m_Check6;
	CButton	m_Check7;
	CButton	m_Check8;
	CButton	m_Check9;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkMessageDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNetworkMessageDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKMESSAGEDIALOG_H__555D289A_0FD5_4EE9_B7E2_1169BFEC3D6D__INCLUDED_)
