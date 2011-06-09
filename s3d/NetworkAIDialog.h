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

#if !defined(AFX_NETWORKAIDIALOG_H__1F293EBD_47FD_4703_9B60_4DF93FA2AAB9__INCLUDED_)
#define AFX_NETWORKAIDIALOG_H__1F293EBD_47FD_4703_9B60_4DF93FA2AAB9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkAIDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "ColorComboBox.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkAIDialog dialog

class CNetworkAIDialog : public CHelpDialog
{
// Construction
public:
	CNetworkAIDialog(CWnd* pParent = NULL);   // standard constructor

	//return used IDs
	CArray <UINT, UINT> *getIDs(void) {return &m_uiIDs;}
	CArray <int, int> *getColors(void) {return &m_iColors;}

// Dialog Data
	//{{AFX_DATA(CNetworkAIDialog)
	enum { IDD = IDD_NETWORK_AI };
	CButton	m_AI1Check;
	CButton	m_AI2Check;
	CButton	m_AI3Check;
	CButton	m_AI4Check;
	CButton	m_AI5Check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkAIDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//set check boxes
	void initChecks(void);

	//array of IDs being used
	CArray <UINT, UINT> m_uiIDs;
	CArray <int, int> m_iColors;

	// Generated message map functions
	//{{AFX_MSG(CNetworkAIDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKAIDIALOG_H__1F293EBD_47FD_4703_9B60_4DF93FA2AAB9__INCLUDED_)
