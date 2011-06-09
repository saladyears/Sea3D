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

#if !defined(AFX_GAMETYPEDIALOG_H__DAD6E30D_C1DC_4A46_8C50_A971CE4CD297__INCLUDED_)
#define AFX_GAMETYPEDIALOG_H__DAD6E30D_C1DC_4A46_8C50_A971CE4CD297__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GameTypeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BalloonHelp.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CGameTypeDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CGameTypeDialog : public CHelpDialog
{
// Construction
public:
	CGameTypeDialog(CWnd* pParent = NULL);   // standard constructor

	//get the selected mode
	int getMode(void) {return m_iMode;}

// Dialog Data
	//{{AFX_DATA(CGameTypeDialog)
	enum { IDD = IDD_GAME_TYPE_DLG };
	CButton	m_OKButton;
	CButton	m_HotseatRadio;
	CButton m_NetworkRadio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameTypeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//mode
	int m_iMode;

	//balloon help
	CBalloonHelp m_balloonHotseat;
	CBalloonHelp m_balloonOK;

	// Generated message map functions
	//{{AFX_MSG(CGameTypeDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnHotseatRadio();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GAMETYPEDIALOG_H__DAD6E30D_C1DC_4A46_8C50_A971CE4CD297__INCLUDED_)
