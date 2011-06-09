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

#if !defined(AFX_MULTIUSEDIALOG_H__572091C3_ED7C_4AF3_A7C5_2F1A56A8AF4A__INCLUDED_)
#define AFX_MULTIUSEDIALOG_H__572091C3_ED7C_4AF3_A7C5_2F1A56A8AF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultiUseDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "HoverStatic.h"
#include "BtnST.h"
#include "CustomListCtrl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CMultiUseDialog dialog

class CMultiUseDialog : public CHelpDialog
{
// Construction
public:
	CMultiUseDialog(CWnd* pParent = NULL);   // standard constructor

	//for modal dialogs
	void setMode(int iMode) {m_iMode = iMode;}
	void setAdjust(int iAdjust) {m_iAdjust = iAdjust;}

	//for year of plenty
	int *getRes(void) {return m_iRes;}

	//reinit everything
	void init();

	//set the pirate flag
	void setPirate(void) {m_bPirate = TRUE;}

	//set the players
	inline void setPlayers(int *i) {memcpy(m_iPlayers, i, sizeof(m_iPlayers));}

	//click handler in control
	void handleClick(void);

// Dialog Data
	//{{AFX_DATA(CMultiUseDialog)
	enum { IDD = IDD_MULTIUSE_DLG };
	CHoverStatic	m_CardsToLoseStatic;
	CHoverStatic	m_CardsLostStatic;
	CCustomListCtrl	m_PlayerList;
	CButton	m_CancelButton;
	CButton	m_ShowCardsCheck;
	CButton	m_DoneButton;
	CButton	m_WhoStatic;
	CButton	m_SelStatic;
	CButton	m_PlayerStatic;
	CHoverStatic	m_Sel1Text;
	CHoverStatic	m_Sel2Text;
	CHoverStatic	m_Sel3Text;
	CHoverStatic	m_Sel4Text;
	CHoverStatic	m_Sel5Text;
	CHoverStatic	m_Sel1Qty;
	CHoverStatic	m_Sel2Qty;
	CHoverStatic	m_Sel3Qty;
	CHoverStatic	m_Sel4Qty;
	CHoverStatic	m_Sel5Qty;
	CHoverStatic	m_InHand1Text;
	CHoverStatic	m_InHand2Text;
	CHoverStatic	m_InHand3Text;
	CHoverStatic	m_InHand4Text;
	CHoverStatic	m_InHand5Text;
	CHoverStatic	m_InHand1Qty;
	CHoverStatic	m_InHand2Qty;
	CHoverStatic	m_InHand3Qty;
	CHoverStatic	m_InHand4Qty;
	CHoverStatic	m_InHand5Qty;
	CButtonST	m_Sel1Button;
	CButtonST	m_Sel2Button;
	CButtonST	m_Sel3Button;
	CButtonST	m_Sel4Button;
	CButtonST	m_Sel5Button;
	CButtonST	m_InHand1Button;
	CButtonST	m_InHand2Button;
	CButtonST	m_InHand3Button;
	CButtonST	m_InHand4Button;
	CButtonST	m_InHand5Button;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiUseDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//the selected resources
	int m_iRes[5];

	//the selected players
	int m_iPlayers[6];

	//the arrays for losing cards
	int m_iCardsToLose[6];
	int m_iCardsLost[6][5];
	int m_iCardsSel[6];

	//what mode we're in
	int m_iMode;

	//the adjustment player
	int m_iAdjust;

	//the number of year of plenty cards selected
	int m_iYear;

	//the stolen card
	int m_iStolen;

	//is this card (soldier or victory) being played before the roll?
	BOOL m_bBefore;

	//whether the steal was played by a pirate or not
	BOOL m_bPirate;

	//show the actual player's cards in multiplayer hotseat
	BOOL m_bShowPlayer;

	//update the player cards
	void updatePlayer(void);

	//clear the steal area
	void clearSteal(void);

	//enables the ok button for losing cards
	void enableOK(void);

	//calculate cards lost for this player
	int calcCardsLost(int iPlayer);

	// Generated message map functions
	//{{AFX_MSG(CMultiUseDialog)
	virtual BOOL OnInitDialog();
	afx_msg void HandleInHandClick(UINT nID);
	afx_msg void HandleSelClick(UINT nID);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnShowcardsCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIUSEDIALOG_H__572091C3_ED7C_4AF3_A7C5_2F1A56A8AF4A__INCLUDED_)
