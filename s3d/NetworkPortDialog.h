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

#if !defined(AFX_NETWORKPORTDIALOG_H__4C0B2580_D914_4085_BC61_4FC57E8B1C16__INCLUDED_)
#define AFX_NETWORKPORTDIALOG_H__4C0B2580_D914_4085_BC61_4FC57E8B1C16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkPortDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkPortDialog dialog

class CNetworkPortDialog : public CHelpDialog
{
// Construction
public:
	CNetworkPortDialog(CWnd* pParent = NULL);   // standard constructor

	UINT getPort(void) {return m_uiPort;}
	void setPort(UINT i) {m_uiPort = i;}

// Dialog Data
	//{{AFX_DATA(CNetworkPortDialog)
	enum { IDD = IDD_NETWORK_PORT };
	CEdit	m_PortEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkPortDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_uiPort;

	// Generated message map functions
	//{{AFX_MSG(CNetworkPortDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKPORTDIALOG_H__4C0B2580_D914_4085_BC61_4FC57E8B1C16__INCLUDED_)
