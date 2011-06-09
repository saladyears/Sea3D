/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

#if !defined(AFX_TRADEOFFERDIALOG_H__CD681A35_DD28_454D_A7B1_6F1B8607CEF3__INCLUDED_)
#define AFX_TRADEOFFERDIALOG_H__CD681A35_DD28_454D_A7B1_6F1B8607CEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TradeOfferDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BtnST.h"
#include "HoverStatic.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CTradeOfferDialog dialog

class CTradeOfferDialog : public CHelpDialog
{
// Construction
public:
	CTradeOfferDialog(CWnd* pParent = NULL);   // standard constructor

	//offer data
	void setOffer(int *iTo, int *iFor, UINT uiID);

// Dialog Data
	//{{AFX_DATA(CTradeOfferDialog)
	enum { IDD = IDD_TRADE_OFFER_DLG };
	CButton	m_OfferButton;
	CButton	m_PlayerStatic;
	CButton	m_OPlayerStatic;
	CButton	m_CounterButton;
	CButton	m_OKButton;
	CButtonST	m_Trade1Button;
	CButtonST	m_Trade2Button;
	CButtonST	m_Trade3Button;
	CButtonST	m_Trade4Button;
	CButtonST	m_OTrade1Button;
	CButtonST	m_OTrade2Button;
	CButtonST	m_OTrade3Button;
	CButtonST	m_OTrade4Button;
	CButtonST	m_OHand1Button;
	CButtonST	m_OHand2Button;
	CButtonST	m_OHand3Button;
	CButtonST	m_OHand4Button;
	CButtonST	m_OHand5Button;
	CButtonST	m_InHand1Button;
	CButtonST	m_InHand2Button;
	CButtonST	m_InHand3Button;
	CButtonST	m_InHand4Button;
	CButtonST	m_InHand5Button;
	CHoverStatic	m_Trade1Text;
	CHoverStatic	m_Trade2Text;
	CHoverStatic	m_Trade3Text;
	CHoverStatic	m_Trade4Text;
	CHoverStatic	m_Trade1Qty;
	CHoverStatic	m_Trade2Qty;
	CHoverStatic	m_Trade3Qty;
	CHoverStatic	m_Trade4Qty;
	CHoverStatic	m_OTrade1Text;
	CHoverStatic	m_OTrade2Text;
	CHoverStatic	m_OTrade3Text;
	CHoverStatic	m_OTrade4Text;
	CHoverStatic	m_OTrade1Qty;
	CHoverStatic	m_OTrade2Qty;
	CHoverStatic	m_OTrade3Qty;
	CHoverStatic	m_OTrade4Qty;
	CHoverStatic	m_OHand1Text;
	CHoverStatic	m_OHand2Text;
	CHoverStatic	m_OHand3Text;
	CHoverStatic	m_OHand4Text;
	CHoverStatic	m_OHand5Text;
	CHoverStatic	m_Inhand1Text;
	CHoverStatic	m_Inhand2Text;
	CHoverStatic	m_Inhand3Text;
	CHoverStatic	m_Inhand4Text;
	CHoverStatic	m_Inhand5Text;
	CHoverStatic	m_Inhand1Qty;
	CHoverStatic	m_Inhand2Qty;
	CHoverStatic	m_Inhand3Qty;
	CHoverStatic	m_Inhand4Qty;
	CHoverStatic	m_Inhand5Qty;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTradeOfferDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//current trade id
	UINT m_uiTradeID;

	//update player data
	void updatePlayer(void);
	void updateOPlayer(void);

	//update trade data
	void updateTrade(BOOL bNew, int iPlace);
	void updateOTrade(BOOL bNew, int iPlace);

	//enables the trade button
	void enableTrade(void);

	//handle undoing the timer
	void stopTimer(void);

	//trade data
	int m_iTrade[5];
	int m_iOTrade[5];
	int m_iTradePlace[4];
	int m_iOTradePlace[4];

	//whether we can trade
	int m_iNumTrade;
	int m_iNumOTrade;

	//reject timer
	int m_iTime;
	UINT m_uiReject;

	//the flag to see if they changed anything on the counter offer
	BOOL m_bChanged;

	// Generated message map functions
	//{{AFX_MSG(CTradeOfferDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCounterButton();
	afx_msg void OnOfferButton();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	void HandleInHandClick(UINT nID);
	void HandleOHandClick(UINT nID);
	void HandleTradeClick(UINT nID);
	void HandleOTradeClick(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRADEOFFERDIALOG_H__CD681A35_DD28_454D_A7B1_6F1B8607CEF3__INCLUDED_)
