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

#if !defined(AFX_NETWORKCHATDIALOG_H__454352B6_1E8D_4E0E_929F_C8ADCD46F4CE__INCLUDED_)
#define AFX_NETWORKCHATDIALOG_H__454352B6_1E8D_4E0E_929F_C8ADCD46F4CE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkChatDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "ChatControl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkChatDialog dialog

class CNetworkChatDialog : public CHelpDialog
{
// Construction
public:
	CNetworkChatDialog(CWnd* pParent = NULL);   // standard constructor

	void setControls(void);

// Dialog Data
	//{{AFX_DATA(CNetworkChatDialog)
	enum { IDD = IDD_NETWORK_CHAT };
	CButton			m_SendButton;
	CChatControl	m_SendEdit;
	CChatControl	m_ChatEdit;
	CChatControl	m_MessageEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkChatDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//accelerator resource
	HACCEL m_hAccelTable;

	// Generated message map functions
	//{{AFX_MSG(CNetworkChatDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSendButton();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnEditPaste();
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKCHATDIALOG_H__454352B6_1E8D_4E0E_929F_C8ADCD46F4CE__INCLUDED_)
