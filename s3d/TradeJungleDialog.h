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

#if !defined(AFX_TRADEJUNGLEDIALOG_H__BC3560CB_88BF_4E90_B2EA_6D89DCEFC676__INCLUDED_)
#define AFX_TRADEJUNGLEDIALOG_H__BC3560CB_88BF_4E90_B2EA_6D89DCEFC676__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TradeJungleDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "BtnST.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTradeJungleDialog dialog
/////////////////////////////////////////////////////////////////////////////
class CTradeJungleDialog : public CHelpDialog
{
// Construction
public:
	CTradeJungleDialog(CWnd* pParent = NULL);   // standard constructor

	//initialize what options they have available
	void initChoices(int nPlayer, int nJungles, int nNeeded, int *pFromBank);

	//show/hide stuff
	void showSingles(BOOL bShow);

// Dialog Data
	//{{AFX_DATA(CTradeJungleDialog)
	enum { IDD = IDD_TRADE_JUNGLE_DLG };
	CHoverStatic	m_W2Static;
	CHoverStatic	m_W1Static;
	CHoverStatic	m_WStatic;
	CHoverStatic	m_S3Static;
	CHoverStatic	m_S2Static;
	CHoverStatic	m_SStatic;
	CHoverStatic	m_O3Static;
	CHoverStatic	m_O1Static;
	CHoverStatic	m_OStatic;
	CButtonST		m_W2Button;
	CButtonST		m_W1Button;
	CButtonST		m_WButton;
	CButtonST		m_S3Button;
	CButtonST		m_S2Button;
	CButtonST		m_SButton;
	CButtonST		m_O3Button;
	CButtonST		m_O1Button;
	CButtonST		m_OButton;
	CButton			m_3Radio;
	CButton			m_2Radio;
	CButton			m_1Radio;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeJungleDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//singles selection
	void handleSingles(int nSel);

	//doubles selection
	void handleDoubles(int nSel);

	//the player
	int m_nPlayer;

	//their jungles
	int m_nJungles;

	//their cards needed
	int m_nNeeded;

	//what they're getting from the bank
	int *m_pFromBank;

	// Generated message map functions
	//{{AFX_MSG(CTradeJungleDialog)
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADEJUNGLEDIALOG_H__BC3560CB_88BF_4E90_B2EA_6D89DCEFC676__INCLUDED_)
