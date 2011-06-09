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

#include "stdafx.h"
#include "settlers.h"
#include "TradeOfferDialog.h"
#include "SettlersView.h"
#include "defineNetwork.h"
#include "defineIni.h"
#include "defineVariants.h"
#include "defineColors.h"
#include ".\tradeofferdialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeOfferDialog dialog


CTradeOfferDialog::CTradeOfferDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CTradeOfferDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeOfferDialog)
	//}}AFX_DATA_INIT

	//zero out memory
	::ZeroMemory(m_iTrade, sizeof(m_iTrade));
	::ZeroMemory(m_iOTrade, sizeof(m_iOTrade));

	//no trade ID
	m_uiTradeID = 0;
}


void CTradeOfferDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeOfferDialog)
	DDX_Control(pDX, IDC_OFFER_BUTTON, m_OfferButton);
	DDX_Control(pDX, IDC_PLAYER_STATIC, m_PlayerStatic);
	DDX_Control(pDX, IDC_OPLAYER_STATIC, m_OPlayerStatic);
	DDX_Control(pDX, IDC_COUNTER_BUTTON, m_CounterButton);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_TRADE1_BUTTON, m_Trade1Button);
	DDX_Control(pDX, IDC_TRADE2_BUTTON, m_Trade2Button);
	DDX_Control(pDX, IDC_TRADE3_BUTTON, m_Trade3Button);
	DDX_Control(pDX, IDC_TRADE4_BUTTON, m_Trade4Button);
	DDX_Control(pDX, IDC_OTRADE1_BUTTON, m_OTrade1Button);
	DDX_Control(pDX, IDC_OTRADE2_BUTTON, m_OTrade2Button);
	DDX_Control(pDX, IDC_OTRADE3_BUTTON, m_OTrade3Button);
	DDX_Control(pDX, IDC_OTRADE4_BUTTON, m_OTrade4Button);
	DDX_Control(pDX, IDC_OHAND1_BUTTON, m_OHand1Button);
	DDX_Control(pDX, IDC_OHAND2_BUTTON, m_OHand2Button);
	DDX_Control(pDX, IDC_OHAND3_BUTTON, m_OHand3Button);
	DDX_Control(pDX, IDC_OHAND4_BUTTON, m_OHand4Button);
	DDX_Control(pDX, IDC_OHAND5_BUTTON, m_OHand5Button);
	DDX_Control(pDX, IDC_INHAND1_BUTTON, m_InHand1Button);
	DDX_Control(pDX, IDC_INHAND2_BUTTON, m_InHand2Button);
	DDX_Control(pDX, IDC_INHAND3_BUTTON, m_InHand3Button);
	DDX_Control(pDX, IDC_INHAND4_BUTTON, m_InHand4Button);
	DDX_Control(pDX, IDC_INHAND5_BUTTON, m_InHand5Button);
	DDX_Control(pDX, IDC_TRADE1_TEXT, m_Trade1Text);
	DDX_Control(pDX, IDC_TRADE2_TEXT, m_Trade2Text);
	DDX_Control(pDX, IDC_TRADE3_TEXT, m_Trade3Text);
	DDX_Control(pDX, IDC_TRADE4_TEXT, m_Trade4Text);
	DDX_Control(pDX, IDC_TRADE1_QTY, m_Trade1Qty);
	DDX_Control(pDX, IDC_TRADE2_QTY, m_Trade2Qty);
	DDX_Control(pDX, IDC_TRADE3_QTY, m_Trade3Qty);
	DDX_Control(pDX, IDC_TRADE4_QTY, m_Trade4Qty);
	DDX_Control(pDX, IDC_OTRADE1_TEXT, m_OTrade1Text);
	DDX_Control(pDX, IDC_OTRADE2_TEXT, m_OTrade2Text);
	DDX_Control(pDX, IDC_OTRADE3_TEXT, m_OTrade3Text);
	DDX_Control(pDX, IDC_OTRADE4_TEXT, m_OTrade4Text);
	DDX_Control(pDX, IDC_OTRADE1_QTY, m_OTrade1Qty);
	DDX_Control(pDX, IDC_OTRADE2_QTY, m_OTrade2Qty);
	DDX_Control(pDX, IDC_OTRADE3_QTY, m_OTrade3Qty);
	DDX_Control(pDX, IDC_OTRADE4_QTY, m_OTrade4Qty);
	DDX_Control(pDX, IDC_OHAND1_TEXT, m_OHand1Text);
	DDX_Control(pDX, IDC_OHAND2_TEXT, m_OHand2Text);
	DDX_Control(pDX, IDC_OHAND3_TEXT, m_OHand3Text);
	DDX_Control(pDX, IDC_OHAND4_TEXT, m_OHand4Text);
	DDX_Control(pDX, IDC_OHAND5_TEXT, m_OHand5Text);
	DDX_Control(pDX, IDC_INHAND1_TEXT, m_Inhand1Text);
	DDX_Control(pDX, IDC_INHAND2_TEXT, m_Inhand2Text);
	DDX_Control(pDX, IDC_INHAND3_TEXT, m_Inhand3Text);
	DDX_Control(pDX, IDC_INHAND4_TEXT, m_Inhand4Text);
	DDX_Control(pDX, IDC_INHAND5_TEXT, m_Inhand5Text);
	DDX_Control(pDX, IDC_INHAND1_QTY, m_Inhand1Qty);
	DDX_Control(pDX, IDC_INHAND2_QTY, m_Inhand2Qty);
	DDX_Control(pDX, IDC_INHAND3_QTY, m_Inhand3Qty);
	DDX_Control(pDX, IDC_INHAND4_QTY, m_Inhand4Qty);
	DDX_Control(pDX, IDC_INHAND5_QTY, m_Inhand5Qty);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeOfferDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CTradeOfferDialog)
	ON_BN_CLICKED(IDC_COUNTER_BUTTON, OnCounterButton)
	ON_BN_CLICKED(IDC_OFFER_BUTTON, OnOfferButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_INHAND1_BUTTON, IDC_INHAND5_BUTTON, HandleInHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OHAND1_BUTTON, IDC_OHAND5_BUTTON, HandleOHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TRADE1_BUTTON, IDC_TRADE4_BUTTON, HandleTradeClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OTRADE1_BUTTON, IDC_OTRADE4_BUTTON, HandleOTradeClick)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeOfferDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CTradeOfferDialog::OnInitDialog() 
{
	int i;
	CHoverStatic *pStatic;
	CButtonST *pButton;
	CDC *dc;
	HDC hDC;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_TRADEOFFER;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();
	
	//set up all the hoverstatics
	for(i = 0; i < 31; i++)
	{
		//get the static
		pStatic = &m_Trade1Text + i;

		//set up values
		pStatic->SetFont(Tahoma12);
		pStatic->SetCenter();
	}

	//set up all the inhand statics
	for(i = 0; i < 5; i++)
	{
		//get the button
		pStatic = &m_Inhand1Text + i;
		pStatic->SetText(VIEW->getResName(i));

		//get the button
		pStatic = &m_OHand1Text + i;
		pStatic->SetText(VIEW->getResName(i));

		//update the buttons
		pButton = &m_InHand1Button + i;
		pButton->SetBitmaps(VIEW->getResImage(i, hDC), COLOR_TRANSP, VIEW->getResImage(i, hDC), COLOR_TRANSP);

		//update the buttons
		pButton = &m_OHand1Button + i;
		pButton->SetBitmaps(VIEW->getResImage(i, hDC), COLOR_TRANSP, VIEW->getResImage(i, hDC), COLOR_TRANSP);
	}

	//release the device context
	ReleaseDC(dc);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// set offer data
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::setOffer(int *iTo, int *iFor, UINT uiID)
{
	int i, j, k;
	int iRes;
	int iPlayer;
	BOOL bCanTrade = TRUE;
	BOOL bAnyTrade = FALSE;
	CRect rect;
	CRect rectClient;
	CString str;
	CDC *dc;
	HDC hDC;
	CHoverStatic *pStatic;
	CHoverStatic *pText;
	CButtonST *pButton;

	//reset trade counter
	m_iNumTrade = 0;

	//reset the changed flag
	m_bChanged = FALSE;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();
	
	//get screen rect
	GetWindowRect(&rect);

	//get client rect
	GetClientRect(&rectClient);

	//enable counter button
	m_CounterButton.EnableWindow();

	//set data
	memcpy(m_iTrade, iTo, sizeof(m_iTrade));
	memcpy(m_iOTrade, iFor, sizeof(m_iOTrade));

	//reset trade places
	memset(m_iTradePlace, -1, sizeof(m_iTradePlace));
	memset(m_iOTradePlace, -1, sizeof(m_iOTradePlace));

	//set trade places
	for(i = 0, j = 0, k = 0; i < 5; i++)
	{
		if(0 < m_iTrade[i])
		{
			m_iTradePlace[j++] = i;		
		}
		if(0 < m_iOTrade[i])
		{
			m_iOTradePlace[k++] = i;
		}
	}

	//trade id
	m_uiTradeID = uiID;

	//set various text
	iPlayer = VIEW->getPlayer();

	//set title text
	str.Format("%s wants to trade!", GAME->m_players[iPlayer].m_player.getName());
	SetWindowText(str);

	//set static text
	str.Format("%s is offering these cards", GAME->m_players[iPlayer].m_player.getFirstName());
	m_PlayerStatic.SetWindowText(str);

	//set player to who we are
	iPlayer = GAME->m_iIndex;

	//update statics
	for(i = 0; i < 4; i++)
	{
		//get controls
		pText = &m_Trade1Text + i;
		pStatic = &m_Trade1Qty + i;
		pButton = &m_Trade1Button + i;

		//get the res
		iRes = m_iTradePlace[i];

		//show/hide from trader
		if(-1 != iRes)
		{
			m_iNumTrade++;
			pText->SetText(VIEW->getResName(iRes));
			pText->ShowWindow(SW_SHOW);
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			pButton->ShowWindow(SW_SHOW);
			pButton->EnableWindow(FALSE);
			str.Format("%d", m_iTrade[iRes]);
			pStatic->SetText(str);
			pStatic->ShowWindow(SW_SHOW);
		}
		else
		{
			pText->ShowWindow(SW_HIDE);
			pButton->ShowWindow(SW_HIDE);
			pStatic->ShowWindow(SW_HIDE);
		}

		//get controls
		pText = &m_OTrade1Text + i;
		pStatic = &m_OTrade1Qty + i;
		pButton = &m_OTrade1Button + i;

		//get the res
		iRes = m_iOTradePlace[i];

		//show/hide
		if(-1 != iRes)
		{
			pText->SetText(VIEW->getResName(iRes));
			pText->ShowWindow(SW_SHOW);
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			pButton->ShowWindow(SW_SHOW);
			pButton->EnableWindow(FALSE);
			str.Format("%d", m_iOTrade[iRes]);
			pStatic->SetText(str);
			pStatic->ShowWindow(SW_SHOW);
		}
		else
		{
			pText->ShowWindow(SW_HIDE);
			pButton->ShowWindow(SW_HIDE);
			pStatic->ShowWindow(SW_HIDE);
		}

		//see if they can even do this trade
		if(GAME->m_players[iPlayer].m_iRes[iRes] < m_iOTrade[iRes])
		{
			bCanTrade = FALSE;
		}
	}

	//reset the counteroffer buttons to be on (turned off in OnCounter)
	for(i = 0; i < 5; i++)
	{
		pButton = &m_OHand1Button + i;
		pButton->EnableWindow();

		pButton = &m_InHand1Button + i;
		pButton->EnableWindow();

		pText = &m_OHand1Text + i;
		pText->EnableWindow();

		pText = &m_Inhand1Text + i;
		pText->EnableWindow();

		pText = &m_Inhand1Qty + i;
		pText->EnableWindow();

		//see if we even have the ability to counteroffer
		if(0 < GAME->m_players[iPlayer].m_iRes[i])
		{
			bAnyTrade = TRUE;
		}
	}

	//enable/disable ability to trade
	m_OKButton.EnableWindow(bCanTrade);
	m_CounterButton.EnableWindow(bAnyTrade);

	//update the current player
	updatePlayer();	

	//update other player
	updateOPlayer();

	//set auto-reject timer
	m_iTime = GetPrivateProfileInt(INI_GENERAL, INI_TRADE_TIMEOUT, 15, INI_FILE);
	m_uiReject = SetTimer(TIMER_REJECT_TRADE, 1000, NULL);

	//set window pos sans counter	
	SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height() - rectClient.Height() + 105, SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

	//show ourselves
	ShowWindow(SW_SHOWNOACTIVATE);

	//release context
	ReleaseDC(dc);

	//set the focus to the view
	VIEW->SetFocus();
}
/////////////////////////////////////////////////////////////////////////////
// update the current player data
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::updatePlayer()
{
	int i;
	int iPlayer;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CString str;

	//get this player
	iPlayer = GAME->m_iIndex;

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_OHand1Button + i;
		pText = &m_OHand1Text + i;
		pQty = &m_Inhand1Qty + i;

		//add cards if they have them
		if(GAME->m_players[iPlayer].m_iRes[i])
		{
			//set the button
			pButton->ShowWindow(SW_SHOW);

			//set the text
			pText->ShowWindow(SW_SHOW);

			//enable windows
			pQty->EnableWindow();
			pQty->ShowWindow(SW_SHOW);

			//set the quantity
			str.Format("%d", GAME->m_players[iPlayer].m_iRes[i]);
			pQty->SetText(str);
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
			pQty->SetText("");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the incoming trader players cards
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::updateOPlayer()
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_InHand1Button + i;
		pText = &m_Inhand1Text + i;
	
		//set the button
		pButton->ShowWindow(SW_SHOW);

		//set the text
		pText->ShowWindow(SW_SHOW);
	}
}

/////////////////////////////////////////////////////////////////////////////
// they want to trade
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnOK() 
{
	//stop the timer
	stopTimer();

	//reply positiviely
	VIEW->clientReplyOffer(m_uiTradeID, TRADE_ACCEPT);

	//make app aware
	VIEW->setDialog(NULL);
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// they don't want to trade
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnCancel() 
{
	//kill the timer
	stopTimer();

	//reply negatively
	VIEW->clientReplyOffer(m_uiTradeID, TRADE_REJECT);
	
	//make app aware
	VIEW->setDialog(NULL);
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// show everything
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnCounterButton() 
{
	int i;
	int iRes;
	int iPlayer;
	int iCount = 4;
	int nQty;
	CRect rect;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//kill the timer
	stopTimer();

	//send the reply
	VIEW->clientReplyOffer(m_uiTradeID, TRADE_COUNTER);

	//set new texts
	str.Format("Ask %s for these cards...", GAME->m_players[VIEW->getPlayer()].m_player.getName());
	m_PlayerStatic.SetWindowText(str);

	//set our text
	m_OPlayerStatic.SetWindowText("in exchange for these cards");

	//get who we are
	iPlayer = GAME->m_iIndex;

	//go through what the trader wants from us and weed out anything that
	//we don't have
	for(i = 0; i < 4; i++)
	{	
		//enable all other player trade windows
		pButton = &m_Trade1Button + i;
		pButton->EnableWindow();

		//disable this side, if necessary
		if(-1 != m_iTradePlace[i])
		{
			iRes = m_iTradePlace[i];

			pButton = &m_OHand1Button + iRes;
			pButton->EnableWindow(FALSE);

			pText = &m_OHand1Text + iRes;
			pText->EnableWindow(FALSE);

			pQty = &m_Inhand1Qty + iRes;
			pQty->EnableWindow(FALSE);
		}

		//skip ones that aren't there
		if(-1 == m_iOTradePlace[i])
		{
			iCount--;
			continue;
		}

		//get the res
		iRes = m_iOTradePlace[i];

		//get button
		pButton = &m_OTrade1Button + i;

		//enable window for trading
		pButton->EnableWindow();

		//determine if we have enough to match what they want
		if(GAME->m_players[iPlayer].m_iRes[iRes] < m_iOTrade[iRes])
		{
			//reset counter spot
			m_iOTradePlace[i] = -1;
			m_iOTrade[iRes] = 0;

			pText = &m_OTrade1Text + i;
			pQty = &m_OTrade1Qty + i;

			//hide all
			pText->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
			pButton->ShowWindow(SW_HIDE);

			//we've changed the trade already
			m_bChanged = TRUE;

			//decrement counter
			iCount--;
		}
		else
		{
			//disable opposing side
			pButton = &m_InHand1Button + iRes;
			pButton->EnableWindow(FALSE);

			pText = &m_Inhand1Text + iRes;
			pText->EnableWindow(FALSE);

			//now correctly set how many cards should appear below

			//get the onhand quantity
			nQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iOTrade[iRes];

			//get quantity pointer
			pQty = &m_Inhand1Qty + iRes;

			//if it's zero, remove this card
			if(nQty == 0)
			{
				//get the other pointers
				pButton = &m_OHand1Button + iRes;
				pText = &m_OHand1Text + iRes;

				//remove the texts
				pText->ShowWindow(SW_HIDE);
				pButton->ShowWindow(SW_HIDE);
				pQty->ShowWindow(SW_HIDE);
			}
			else
			{
				//set the new text
				str.Format("%d", nQty);
				pQty->SetText(str);
			}
		}
	}

	//get screen rect
	GetWindowRect(&rect);

	//disable this button
	m_CounterButton.EnableWindow(FALSE);

	//disable the accept button so they dont' accidently press it
	m_OKButton.EnableWindow(FALSE);

	//show all
	SetWindowPos(&wndTop, 0, 0, rect.Width(), 260, SWP_NOMOVE | SWP_NOZORDER);

	//set the trade flag
	m_iNumOTrade = iCount;

	//enable/disable the offer button
	enableTrade();

	//set the focus to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on the offerer's inhand card
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::HandleInHandClick(UINT nID)
{
	int i;
	int iRes;
	int iQty;
	int iPlayer;
	BOOL bNew = FALSE;
	BOOL bInList = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get the resource type
	iRes = (nID - IDC_INHAND1_BUTTON);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//search for it in the list
	for(i = 0; i < 4; i++)
	{
		//if we find it, or a -1, break;
		if(m_iTradePlace[i] == iRes)
		{
			bInList = TRUE;
			break;
		}
	}

	if(FALSE == bInList)
	{
		for(i = 0; i < 4; i++)
		{
			//it's a new card
			if(m_iTradePlace[i] == -1)
			{
				//set the place
				bNew = TRUE;
				m_iTradePlace[i] = iRes;

				//disable this card on the other side
				pButton = &m_OHand1Button + iRes;
				pButton->EnableWindow(FALSE);

				pText = &m_OHand1Text + iRes;
				pText->EnableWindow(FALSE);

				pQty = &m_Inhand1Qty + iRes;
				pQty->EnableWindow(FALSE);
				break;
			}
		}
	}

	//increase the number of cards being traded
	m_iTrade[iRes]++;

	//decrease the onhand quantity
	iQty = MAP->m_iBankRes - m_iTrade[iRes];

	//if it's zero, remove this card
	if(iQty == 0)
	{
		//get the other pointers
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;

		//remove the texts
		pText->ShowWindow(SW_HIDE);

		//hide the window
		pButton->ShowWindow(SW_HIDE);
	}

	//update the trade data
	updateTrade(bNew, i);

	//set the trade bool
	m_iNumTrade++;

	//see if we can trade
	enableTrade();

	//set the changed flag
	m_bChanged = TRUE;

	//set the focus to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// update trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::updateTrade(BOOL bNew, int iPlace)
{
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CString str;
	CDC *dc;
	HDC hDC;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the quantity pointer
	pQty = &m_Trade1Qty + iPlace;

	//set the quantity
	str.Format("%d", m_iTrade[m_iTradePlace[iPlace]]);
	pQty->SetText(str);

	//see if we need to add a new card
	if(TRUE == bNew)
	{
		//set the bitmaps and unhide
		pButton = &m_Trade1Button + iPlace;
		pText = &m_Trade1Text + iPlace;

		//set the bitmaps
		pButton->SetBitmaps(VIEW->getResImage(m_iTradePlace[iPlace], hDC), COLOR_TRANSP, VIEW->getResImage(m_iTradePlace[iPlace], hDC), COLOR_TRANSP);
		pButton->ShowWindow(SW_SHOW);

		//set the text
		pText->SetText(VIEW->getResName(m_iTradePlace[iPlace]));
		pText->ShowWindow(SW_SHOW);

		//show the quantity
		pQty->ShowWindow(SW_SHOW);
	}

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on another player's card
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::HandleOHandClick(UINT nID)
{
	int i;
	int iRes;
	int iQty;
	int iPlayer;
	BOOL bNew = FALSE;
	BOOL bInList = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get the resource type
	iRes = (nID - IDC_OHAND1_BUTTON);

	//get who we are
	iPlayer = GAME->m_iIndex;

	//search for it in the list
	for(i = 0; i < 4; i++)
	{
		//if we find it, or a -1, break;
		if(m_iOTradePlace[i] == iRes)
		{
			bInList = TRUE;
			break;
		}
	}

	if(FALSE == bInList)
	{
		for(i = 0; i < 4; i++)
		{
			//it's a new card
			if(m_iOTradePlace[i] == -1)
			{
				//set the place
				bNew = TRUE;
				m_iOTradePlace[i] = iRes;

				//disable this card on the other side
				pButton = &m_InHand1Button + iRes;
				pButton->EnableWindow(FALSE);

				pText = &m_Inhand1Text + iRes;
				pText->EnableWindow(FALSE);
				break;
			}
		}
	}

	//increase the number of cards being traded
	m_iOTrade[iRes]++;

	//get the onhand quantity
	iQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iOTrade[iRes];

	//get quantity pointer
	pQty = &m_Inhand1Qty + iRes;

	//if it's zero, remove this card
	if(iQty == 0)
	{
		//get the other pointers
		pButton = &m_OHand1Button + iRes;
		pText = &m_OHand1Text + iRes;

		//remove the texts
		pText->ShowWindow(SW_HIDE);
		pButton->ShowWindow(SW_HIDE);
		pQty->ShowWindow(SW_HIDE);
	}
	else
	{
		//set the new text
		str.Format("%d", iQty);
		pQty->SetText(str);
	}

	//update the trade data
	updateOTrade(bNew, i);

	//set the trade bool
	m_iNumOTrade++;

	//see if we can trade
	enableTrade();

	//set the changed flag
	m_bChanged = TRUE;

	//set the focus to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// update other player trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::updateOTrade(BOOL bNew, int iPlace)
{
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CString str;
	CDC *dc;
	HDC hDC;

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set the quantity pointer
	pQty = &m_OTrade1Qty + iPlace;

	//set the quantity
	str.Format("%d", m_iOTrade[m_iOTradePlace[iPlace]]);
	pQty->SetText(str);

	//see if we need to add a new card
	if(TRUE == bNew)
	{
		//set the bitmaps and unhide
		pButton = &m_OTrade1Button + iPlace;
		pText = &m_OTrade1Text + iPlace;

		//set the bitmaps
		pButton->SetBitmaps(VIEW->getResImage(m_iOTradePlace[iPlace], hDC), COLOR_TRANSP, VIEW->getResImage(m_iOTradePlace[iPlace], hDC), COLOR_TRANSP);
		pButton->ShowWindow(SW_SHOW);

		//set the text
		pText->SetText(VIEW->getResName(m_iOTradePlace[iPlace]));
		pText->ShowWindow(SW_SHOW);

		//show quantity
		pQty->ShowWindow(SW_SHOW);
	}

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the trade queue
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::HandleTradeClick(UINT nID)
{
	int i;
	int iQty;
	int iRes;
	int iPlace;
	int iPlayer;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get the current place
	iPlace = (nID - IDC_TRADE1_BUTTON);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the resource in question
	iRes = m_iTradePlace[iPlace];

	//subtract one from the amounts being traded
	m_iTrade[iRes]--;

	//get the quantity button
	pQty = &m_Trade1Qty + iPlace;

	//if it's a zero, remove this card from the trade stack
	if(m_iTrade[iRes] == 0)
	{
		//get the buttons
		pButton = &m_Trade1Button + iPlace;
		pText = &m_Trade1Text + iPlace;

		//hide them
		pButton->ShowWindow(SW_HIDE);
		pText->SetText("");
		pQty->SetText("");

		//set this value to empty
		m_iTradePlace[iPlace] = -1;

		//enable the other side
		pButton = &m_OHand1Button + iRes;
		pButton->EnableWindow();

		pText = &m_OHand1Text + iRes;
		pText->EnableWindow();

		pQty = &m_Inhand1Qty + iRes;
		pQty->EnableWindow();
	}
	else
	{
		//set the new quantity
		str.Format("%d", m_iTrade[iRes]);
		pQty->SetText(str);
	}

	//get the new in hand quantity
	iQty = MAP->m_iBankRes - m_iTrade[iRes];

	//if it's 1, then we need to re show this window and text
	if(iQty == 1)
	{
		//get the buttons
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;

		//show them
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
	}

	//check to see if we're not all empty
	for(i = 0, m_iNumTrade = 0; i < 4; i++)
	{
		if(m_iTradePlace[i] != -1)
		{
			m_iNumTrade++;
			break;
		}
	}

	//see if we can trade
	enableTrade();

	//set the changed flag
	m_bChanged = TRUE;

	//set the focus to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// enable the trade button
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::enableTrade()
{
	BOOL bEnable;

	//see what we got
	if(IS_VARIANT(VARIANT_TRADE_ZERO))
	{
		bEnable = (m_iNumTrade + m_iNumOTrade > 0);
	}
	else
	{
		bEnable = ((m_iNumTrade > 0) && (m_iNumOTrade > 0));
	}

	//enable the window
	m_OfferButton.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the other players trade queue
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::HandleOTradeClick(UINT nID)
{
	int i;
	int iQty;
	int iRes;
	int iPlace;
	int iPlayer;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get the current place
	iPlace = (nID - IDC_OTRADE1_BUTTON);

	//get the current player
	iPlayer = GAME->m_iIndex;

	//get the resource in question
	iRes = m_iOTradePlace[iPlace];

	//subtract one from the amounts being traded
	m_iOTrade[iRes]--;

	//get the quantity button
	pQty = &m_OTrade1Qty + iPlace;

	//if it's a zero, remove this card from the trade stack
	if(m_iOTrade[iRes] == 0)
	{
		//get the buttons
		pButton = &m_OTrade1Button + iPlace;
		pText = &m_OTrade1Text + iPlace;

		//hide them
		pButton->ShowWindow(SW_HIDE);
		pText->SetText("");
		pQty->SetText("");

		//set this value to empty
		m_iOTradePlace[iPlace] = -1;

		//enable the other side
		pButton = &m_InHand1Button + iRes;
		pButton->EnableWindow();

		pText = &m_Inhand1Text + iRes;
		pText->EnableWindow();
	}
	else
	{
		//set the new quantity
		str.Format("%d", m_iOTrade[iRes]);
		pQty->SetText(str);
	}

	//get the new in hand quantity
	iQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iOTrade[iRes];

	//get quantity
	pQty = &m_Inhand1Qty + iRes;

	//if it's 1, then we need to re show this window and text
	if(iQty == 1)
	{
		//get the buttons
		pButton = &m_OHand1Button + iRes;
		pText = &m_OHand1Text + iRes;

		//show them
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
		pQty->ShowWindow(SW_SHOW);
	}
	
	//update the quantity
	str.Format("%d", iQty);
	pQty->SetText(str);

	//check to see if we're not all empty
	for(i = 0, m_iNumOTrade = 0; i < 4; i++)
	{
		if(m_iOTradePlace[i] != -1)
		{
			m_iNumOTrade++;
			break;
		}
	}

	//see if we can trade
	enableTrade();

	//set the changed flag
	m_bChanged = TRUE;

	//set the focus to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// make the counter offer
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnOfferButton() 
{
	VIEW->clientTradeCounter(m_uiTradeID, m_iTrade, m_iOTrade);

	//shut down
	EndDialog(-1);
}

/////////////////////////////////////////////////////////////////////////////
// turn off the timer
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::stopTimer()
{
	int iPlayer;
	CString str;

	//set various text
	iPlayer = VIEW->getPlayer();

	//set title text
	str.Format("%s wants to trade!", GAME->m_players[iPlayer].m_player.getName(), m_iTime, (m_iTime == 1 ? "" : "s"));
	SetWindowText(str);

	//kill the timer
	KillTimer(m_uiReject);
}

/////////////////////////////////////////////////////////////////////////////
// handle autoreject trade countdown
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnTimer(UINT nIDEvent) 
{
	int iPlayer;
	CString str;

	switch(nIDEvent)
	{
	case TIMER_REJECT_TRADE:
		m_iTime--;

		//set various text
		iPlayer = VIEW->getPlayer();

		//set title text
		str.Format("%s wants to trade! (%d second%s until auto-rejection)", GAME->m_players[iPlayer].m_player.getName(), m_iTime, (m_iTime == 1 ? "" : "s"));
		SetWindowText(str);

		//see if we're done
		if(0 >= m_iTime)
		{
			OnCancel();
		}
		break;
	default: break;
	}
	
	CHelpDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// stop pesky keyboard messages
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	VIEW->PostMessage(WM_CHAR, (WPARAM) nChar, MAKELPARAM(nRepCnt, nFlags));
}

/////////////////////////////////////////////////////////////////////////////
// stop pesky keyboard messages
/////////////////////////////////////////////////////////////////////////////
void CTradeOfferDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	VIEW->PostMessage(WM_KEYDOWN, (WPARAM) nChar, MAKELPARAM(nRepCnt, nFlags));
}
