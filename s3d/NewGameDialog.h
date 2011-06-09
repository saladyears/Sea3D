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

#if !defined(AFX_NEWGAMEDIALOG_H__4A45E54F_E4B2_46FE_90C5_C68C09839EAC__INCLUDED_)
#define AFX_NEWGAMEDIALOG_H__4A45E54F_E4B2_46FE_90C5_C68C09839EAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewGameDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "BtnSt.h"
#include "DropStatic.h"
#include "DropListCtrl.h"
#include "BalloonHelp.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNewGameDialog dialog

class CNewGameDialog : public CHelpDialog
{
// Construction
public:
	CNewGameDialog(CWnd* pParent = NULL);   // standard constructor

	int inline getNumPlayers(void) {return m_iPlayers;}

	void incPlayers(void);
	void decPlayers(void);

	//drop button functions
	void inline setDropButton(CDropStatic *p) {m_pDropButton = p;}
	CDropStatic inline *getDropButton(void) {return m_pDropButton;}

	//whether the list control is dragging
	BOOL inline getDrag(void) {return m_bDrag;}

	//resets all drag variables
	void resetAllDrags(void);

// Dialog Data
	//{{AFX_DATA(CNewGameDialog)
	enum { IDD = IDD_NEW_GAME_DLG };
	CButton	m_AddButton;
	CButton	m_VariantsButton;
	CButton			m_CancelButton;
	CDropListCtrl	m_PlayerList;
	CButton			m_OKButton;
	CDropStatic		m_WhiteText;
	CDropStatic		m_RedText;
	CDropStatic		m_OrangeText;
	CDropStatic		m_BlueText;
	CDropStatic		m_BrownText;
	CDropStatic		m_GreenText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewGameDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//number of players
	int m_iPlayers;

	//whether the list control is dragging
	BOOL m_bDrag;

	//the currently dragging drop window
	CDropStatic *m_pDropButton;

	//balloon help
	CBalloonHelp m_balloonAdd;
	CBalloonHelp m_balloonClick;
	CBalloonHelp m_balloonOK;
	BOOL m_bOnce;

	// Generated message map functions
	//{{AFX_MSG(CNewGameDialog)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBegindragPlayerList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnDblclkPlayerList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAddButton();
	afx_msg void OnVariantsButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWGAMEDIALOG_H__4A45E54F_E4B2_46FE_90C5_C68C09839EAC__INCLUDED_)
