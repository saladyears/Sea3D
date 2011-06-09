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

#if !defined(AFX_MONOPOLYCARDDIALOG_H__27B909A5_6F43_43A0_9B19_2BC9C6B01324__INCLUDED_)
#define AFX_MONOPOLYCARDDIALOG_H__27B909A5_6F43_43A0_9B19_2BC9C6B01324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MonopolyCardDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CMonopolyCardDialog dialog

class CMonopolyCardDialog : public CHelpDialog
{
// Construction
public:
	CMonopolyCardDialog(CWnd* pParent = NULL);   // standard constructor

	inline int getType(void) {return m_iType;}

// Dialog Data
	//{{AFX_DATA(CMonopolyCardDialog)
	enum { IDD = IDD_MONOPOLY_DLG };
	CButton		m_TimberRadio;
	CButton		m_WheatRadio;
	CButton		m_OreRadio;
	CButton		m_ClayRadio;
	CButton		m_SheepRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMonopolyCardDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_iType;

	// Generated message map functions
	//{{AFX_MSG(CMonopolyCardDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MONOPOLYCARDDIALOG_H__27B909A5_6F43_43A0_9B19_2BC9C6B01324__INCLUDED_)
