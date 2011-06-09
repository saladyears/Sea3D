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

#if !defined(AFX_NETWORKTRADEDIALOG_H__DB4FCF04_DD50_4998_A415_6DE0CF6674B5__INCLUDED_)
#define AFX_NETWORKTRADEDIALOG_H__DB4FCF04_DD50_4998_A415_6DE0CF6674B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NetworkTradeDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// include files
/////////////////////////////////////////////////////////////////////////////
#include "BtnST.h"
#include "HoverStatic.h"
#include "NetworkTradeListCtrl.h"
#include "HelpDialog.h"

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeDialog dialog

class CNetworkTradeDialog : public CHelpDialog
{
// Construction
public:
	CNetworkTradeDialog(CWnd* pParent = NULL);   // standard constructor

	//initialize
	void initPlayer(void);

	//trade reply handling
	void handleReply(int iPlayer, short shStatus, int *iTo, int *iFor);

	//make sure we have someone to trade with
	void checkTrade(void);

// Dialog Data
	//{{AFX_DATA(CNetworkTradeDialog)
	enum { IDD = IDD_NETWORK_TRADE_DLG };
	CHoverStatic	m_InfoButton;
	CButton	m_OPlayerStatic;
	CButton	m_DoneButton;
	CButton	m_TradeButton;
	CButton	m_PlayerStatic;
	CNetworkTradeListCtrl	m_PlayerList;
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
	CButton	m_Counter1Static;
	CButton	m_Counter2Static;
	CButton	m_Counter3Static;
	CButton	m_Counter4Static;
	CButton	m_Counter5Static;
	CButton	m_Accept1Button;
	CButton	m_Accept2Button;
	CButton	m_Accept3Button;
	CButton	m_Accept4Button;
	CButton	m_Accept5Button;
	CButtonST	m_C1T1Button;
	CButtonST	m_C1T2Button;
	CButtonST	m_C1T3Button;
	CButtonST	m_C1T4Button;
	CButtonST	m_C2T1Button;
	CButtonST	m_C2T2Button;
	CButtonST	m_C2T3Button;
	CButtonST	m_C2T4Button;
	CButtonST	m_C3T1Button;
	CButtonST	m_C3T2Button;
	CButtonST	m_C3T3Button;
	CButtonST	m_C3T4Button;
	CButtonST	m_C4T1Button;
	CButtonST	m_C4T2Button;
	CButtonST	m_C4T3Button;
	CButtonST	m_C4T4Button;
	CButtonST	m_C5T1Button;
	CButtonST	m_C5T2Button;
	CButtonST	m_C5T3Button;
	CButtonST	m_C5T4Button;
	CButtonST	m_C1O1Button;
	CButtonST	m_C1O2Button;
	CButtonST	m_C1O3Button;
	CButtonST	m_C1O4Button;
	CButtonST	m_C2O1Button;
	CButtonST	m_C2O2Button;
	CButtonST	m_C2O3Button;
	CButtonST	m_C2O4Button;
	CButtonST	m_C3O1Button;
	CButtonST	m_C3O2Button;
	CButtonST	m_C3O3Button;
	CButtonST	m_C3O4Button;
	CButtonST	m_C4O1Button;
	CButtonST	m_C4O2Button;
	CButtonST	m_C4O3Button;
	CButtonST	m_C4O4Button;
	CButtonST	m_C5O1Button;
	CButtonST	m_C5O2Button;
	CButtonST	m_C5O3Button;
	CButtonST	m_C5O4Button;
	CHoverStatic	m_C1T1Qty;
	CHoverStatic	m_C1T2Qty;
	CHoverStatic	m_C1T3Qty;
	CHoverStatic	m_C1T4Qty;
	CHoverStatic	m_C2T1Qty;
	CHoverStatic	m_C2T2Qty;
	CHoverStatic	m_C2T3Qty;
	CHoverStatic	m_C2T4Qty;
	CHoverStatic	m_C3T1Qty;
	CHoverStatic	m_C3T2Qty;
	CHoverStatic	m_C3T3Qty;
	CHoverStatic	m_C3T4Qty;
	CHoverStatic	m_C4T1Qty;
	CHoverStatic	m_C4T2Qty;
	CHoverStatic	m_C4T3Qty;
	CHoverStatic	m_C4T4Qty;
	CHoverStatic	m_C5T1Qty;
	CHoverStatic	m_C5T2Qty;
	CHoverStatic	m_C5T3Qty;
	CHoverStatic	m_C5T4Qty;
	CHoverStatic	m_C1O1Qty;
	CHoverStatic	m_C1O2Qty;
	CHoverStatic	m_C1O3Qty;
	CHoverStatic	m_C1O4Qty;
	CHoverStatic	m_C2O1Qty;
	CHoverStatic	m_C2O2Qty;
	CHoverStatic	m_C2O3Qty;
	CHoverStatic	m_C2O4Qty;
	CHoverStatic	m_C3O1Qty;
	CHoverStatic	m_C3O2Qty;
	CHoverStatic	m_C3O3Qty;
	CHoverStatic	m_C3O4Qty;
	CHoverStatic	m_C4O1Qty;
	CHoverStatic	m_C4O2Qty;
	CHoverStatic	m_C4O3Qty;
	CHoverStatic	m_C4O4Qty;
	CHoverStatic	m_C5O1Qty;
	CHoverStatic	m_C5O2Qty;
	CHoverStatic	m_C5O3Qty;
	CHoverStatic	m_C5O4Qty;
	CStatic	m_For1Static;
	CStatic	m_For2Static;
	CStatic	m_For3Static;
	CStatic	m_For4Static;
	CStatic	m_For5Static;
	CHoverStatic	m_TradeOfferStatic;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNetworkTradeDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	//update player data
	void updatePlayer(void);
	void updateOPlayer(void);

	//update trade data
	void updateTrade(BOOL bNew, int iPlace);
	void updateOTrade(BOOL bNew, int iPlace);

	//enable/disable stuff during network trading
	void enableTrade(BOOL bEnable);

	//clear trade data
	void clearTrade(BOOL bClearData = TRUE);
	void clearOTrade(BOOL bClearData = TRUE);

	//show the counteroffer section
	void showCounterSection(int iCount);

	//handle a rejection reply
	void handleReject(int iPlayer);
	void handleAccept(int iPlayer);
	void handleCounter(int iPlayer);
	void handleCounterOffer(int iPlayer, int *iTo, int *iFor);

	//trade data
	int m_iTrade[5];
	int m_iOTrade[5];
	int m_iTradePlace[4];
	int m_iOTradePlace[4];

	//counteroffer offsets
	int m_iCounterOffsets[6];

	//whether we can trade
	int m_iNumTrade;
	int m_iNumOTrade;
	BOOL m_bNetTrade;

	//the next spot open for replies
	int m_iCurrentReply;

	//who's in what reply spot
	int m_iPlayerReply[5];

	//the array of trade offers
	int m_iTradeArray[5][5];
	int m_iOTradeArray[5][5];

	// Generated message map functions
	//{{AFX_MSG(CNetworkTradeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnExecuteButton();
	virtual void OnOK();
	afx_msg void OnCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	void HandleAcceptClick(UINT nID);
	void HandleInHandClick(UINT nID);
	void HandleOHandClick(UINT nID);
	void HandleTradeClick(UINT nID);
	void HandleOTradeClick(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NETWORKTRADEDIALOG_H__DB4FCF04_DD50_4998_A415_6DE0CF6674B5__INCLUDED_)
