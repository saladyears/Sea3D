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

#if !defined(AFX_NETWORKPLAYCARDDIALOG_H__EC99BD05_9614_4166_A176_3D8321525FC4__INCLUDED_)
#define AFX_NETWORKPLAYCARDDIALOG_H__EC99BD05_9614_4166_A176_3D8321525FC4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkPlayCardDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BtnSt.h"
#include "ListBoxST.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayCardDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CNetworkPlayCardDialog : public CHelpDialog
{
// Construction
public:
	CNetworkPlayCardDialog(CWnd* pParent = NULL);   // standard constructor

	//called before the dice roll
	inline void setBefore(void) {m_bBefore = TRUE;}

// Dialog Data
	//{{AFX_DATA(CNetworkPlayCardDialog)
	enum { IDD = IDD_NETWORK_PLAY_CARD_DLG };
	CButton	m_OKButton;
	CListBoxST	m_CardsList;
	CButtonST	m_ImageButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkPlayCardDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//update the current players cards
	void updatePlayer(void);

	//the current player
	int m_iPlayer;

	//currently displayed card
	int m_iCurrent;

	//whether we're being called before the roll
	BOOL m_bBefore;

	//play options
	void playVictory(void);
	void playMonopoly(void);
	void playRoad(void);
	void playYear(void);

	// Generated message map functions
	//{{AFX_MSG(CNetworkPlayCardDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCardsList();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKPLAYCARDDIALOG_H__EC99BD05_9614_4166_A176_3D8321525FC4__INCLUDED_)
