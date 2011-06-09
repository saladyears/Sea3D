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

#if !defined(AFX_CHATCONTROL_H__7936417F_0A7C_48B3_80FA_389436381694__INCLUDED_)
#define AFX_CHATCONTROL_H__7936417F_0A7C_48B3_80FA_389436381694__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatControl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "NetChatMessage.h"
#include "NetSysMessage.h"

/////////////////////////////////////////////////////////////////////////////
// CChatControl window

class CChatControl : public CRichEditCtrl
{
// Construction
public:
	CChatControl();

// Attributes
public:
	void addMessage(CNetChatMessage *pMsg);
	void addSysMessage(CNetSysMessage *pMsg);
	void addString(CString str, BOOL bBold, COLORREF cr);

// Operations
public:
	void initControl(void);

	//make it a send box
	void setSend(void) {m_bSend = TRUE;}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatControl)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChatControl();

	// Generated message map functions
protected:
	CString getSysString(CNetSysMessage *pMsg);

	//is this a send box?
	BOOL m_bSend;

	//{{AFX_MSG(CChatControl)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCONTROL_H__7936417F_0A7C_48B3_80FA_389436381694__INCLUDED_)
