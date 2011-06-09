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

#if !defined(AFX_TRADEBANKDIALOG_H__407718E1_0528_420C_ADCC_3DE20A74221A__INCLUDED_)
#define AFX_TRADEBANKDIALOG_H__407718E1_0528_420C_ADCC_3DE20A74221A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TradeBankDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTradeBankDialog dialog

class CTradeBankDialog : public CHelpDialog
{
// Construction
public:
	CTradeBankDialog(CWnd* pParent = NULL);   // standard constructor

	void inline setAvail(int *p) {memcpy(m_iRes, p, sizeof(m_iRes));}

	void inline setRes(CString str) {m_strRes = str;}

	int inline getType(void) {return m_iType;}

// Dialog Data
	//{{AFX_DATA(CTradeBankDialog)
	enum { IDD = IDD_TRADEFOR_DLG };
	CButton	m_QuestionStatic;
	CButton	m_TimberRadio;
	CButton	m_WheatRadio;
	CButton	m_OreRadio;
	CButton	m_ClayRadio;
	CButton	m_SheepRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeBankDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//which resources are available
	int m_iRes[5];

	//the resource being traded for
	CString m_strRes;

	//the type they'll trade
	int m_iType;

	// Generated message map functions
	//{{AFX_MSG(CTradeBankDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADEBANKDIALOG_H__407718E1_0528_420C_ADCC_3DE20A74221A__INCLUDED_)
