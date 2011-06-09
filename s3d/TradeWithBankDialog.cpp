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

#include "stdafx.h"
#include "settlers.h"
#include "TradeWithBankDialog.h"
#include "SettlersView.h"
#include "defineStat.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeWithBankDialog dialog


CTradeWithBankDialog::CTradeWithBankDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CTradeWithBankDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeWithBankDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTradeWithBankDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeWithBankDialog)
	DDX_Control(pDX, IDC_PLAYER_STATIC, m_PlayerStatic);
	DDX_Control(pDX, IDC_EXECUTE_BUTTON, m_TradeButton);
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
	DDX_Control(pDX, IDC_INHAND1_TRADE, m_Inhand1Trade);
	DDX_Control(pDX, IDC_INHAND2_TRADE, m_Inhand2Trade);
	DDX_Control(pDX, IDC_INHAND3_TRADE, m_Inhand3Trade);
	DDX_Control(pDX, IDC_INHAND4_TRADE, m_Inhand4Trade);
	DDX_Control(pDX, IDC_INHAND5_TRADE, m_Inhand5Trade);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeWithBankDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CTradeWithBankDialog)
	ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_INHAND1_BUTTON, IDC_INHAND5_BUTTON, HandleInHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OHAND1_BUTTON, IDC_OHAND5_BUTTON, HandleOHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TRADE1_BUTTON, IDC_TRADE4_BUTTON, HandleTradeClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OTRADE1_BUTTON, IDC_OTRADE4_BUTTON, HandleOTradeClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeWithBankDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CTradeWithBankDialog::OnInitDialog() 
{
	int i;
	CHoverStatic *pStatic;
	CButtonST *pButton;
	CDC *dc;
	HDC hDC;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_TRADEBANK;
	
	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();
	
	//set up all the hoverstatics
	for(i = 0; i < 36; i++)
	{
		//get the static
		pStatic = &m_Trade1Text + i;

		//set up values
		pStatic->SetCenter();

		//set color on trade buttons
		if(i >= 31)
		{
			pStatic->SetFont(Tahoma12B);
			pStatic->SetColors(COLOR_LTGREEN, COLOR_LTGREEN);
		}
		else
		{
			pStatic->SetFont(Tahoma12);
		}
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

	//setup stuff
	initDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// other initialization
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::initDialog()
{
	//clear trading arrays
	clearTrade();
	clearOTrade();

	//update the current player
	updatePlayer();	

	//update other player
	updateOPlayer();

	//save current and hide all
	VIEW->showBuildOptions(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// update the current player data
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::updatePlayer()
{
	int i;
	int iRes;
	int iPlayer;
	BOOL bThree = FALSE;
	BOOL bPort = FALSE;
	BOOL bEnable;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CHoverStatic *pTrade;
	CString str;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the static text
	m_PlayerStatic.SetWindowText(GAME->m_players[iPlayer].m_player.getName());

	//determine if they have 3-1 port
	bThree = VIEW->havePort(iPlayer, RES_PORT3);

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//determine if they have this port
		bPort = VIEW->havePort(iPlayer, RES_PORTTIMBER + i);

		//get the controls
		pButton = &m_InHand1Button + i;
		pText = &m_Inhand1Text + i;
		pQty = &m_Inhand1Qty + i;
		pTrade = &m_Inhand1Trade + i;

		//get resource quantity
		iRes = GAME->m_players[iPlayer].m_iRes[i];
		
		//add cards if they have them
		if(0 < iRes)
		{
			//set the button
			pButton->ShowWindow(SW_SHOW);

			//set the text
			pText->ShowWindow(SW_SHOW);

			//set the quantity
			str.Format("%d", GAME->m_players[iPlayer].m_iRes[i]);
			pQty->SetText(str);
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
			pQty->SetText("");
			pTrade->ShowWindow(SW_HIDE);
		}

		//set trade text
		if(TRUE == bPort) str = "2-1";
		else if(TRUE == bThree) str = "3-1";
		else str = "4-1";

		//set text
		pTrade->SetText(str);

		//enable/disable windows if they can trade them or not
		bEnable = ((iRes >= 4) || (bThree && iRes >= 3) || (bPort && iRes >= 2));
		pButton->EnableWindow(bEnable);
		pText->EnableWindow(bEnable);
		pQty->EnableWindow(bEnable);
		pTrade->EnableWindow(bEnable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the banks cards
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::updateOPlayer()
{
	int i;
	int iRes;
	CButtonST *pButton;
	CHoverStatic *pText;

	//go through and set up the banks cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_OHand1Button + i;
		pText = &m_OHand1Text + i;

		//see if this card is available
		iRes = GAME->getBankRes(i);

		//add cards if they have them
		if(0 < iRes)
		{
			//set the button
			pButton->ShowWindow(SW_SHOW);

			//set the text
			pText->ShowWindow(SW_SHOW);
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// clear the trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::clearTrade()
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//clear the memory array
	::ZeroMemory(m_iTrade, sizeof(m_iTrade));
	::ZeroMemory(m_iBank, sizeof(m_iBank));

	//clear the trade screens
	for(i = 0; i < 4; i++)
	{
		//clear the place memory
		m_iTradePlace[i] = -1;

		//get the controls
		pButton = &m_Trade1Button + i;
		pText = &m_Trade1Text + i;
		pQty = &m_Trade1Qty + i;

		//clear
		pButton->ShowWindow(SW_HIDE);
		pText->SetText("");
		pQty->SetText("");
	}

	//enable other side trade buttons
	for(i = 0; i < 5; i++)
	{
		//get the contorl
		pButton = &m_OHand1Button + i;

		//enable the window
		pButton->EnableWindow();

		//enable all text windows
		pText = &m_OHand1Text + i;
		pText->EnableWindow();
	}

	//trade counter
	m_iTradeCounter = 0;

	//disable the trade button
	m_TradeButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// clear the other player's trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::clearOTrade()
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//clear the memory array
	::ZeroMemory(m_iOTrade, sizeof(m_iOTrade));

	//clear the trade screens
	for(i = 0; i < 4; i++)
	{
		//clear the place memory
		m_iOTradePlace[i] = -1;

		//get the controls
		pButton = &m_OTrade1Button + i;
		pText = &m_OTrade1Text + i;
		pQty = &m_OTrade1Qty + i;

		//clear
		pButton->ShowWindow(SW_HIDE);
		pText->SetText("");
		pQty->SetText("");
	}

	//enable other side trade buttons
	for(i = 0; i < 5; i++)
	{
		//get the contorl
		pButton = &m_InHand1Button + i;
		pButton->EnableWindow();

		//enable all text windows
		pText = &m_Inhand1Text + i;
		pText->EnableWindow();
	}

	//trade counter
	m_iOTradeCounter = 0;

	//disable the trade button
	m_TradeButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on an inhand card
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::HandleInHandClick(UINT nID)
{
	int i;
	int iRes;
	int iQty;
	int iTrade = 0;
	int iPlayer;
	BOOL bThree;
	BOOL bPort;
	BOOL bInList = FALSE;
	BOOL bNew = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CHoverStatic *pTrade;

	//get the resource type
	iRes = (nID - IDC_INHAND1_BUTTON);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//determine port status
	bThree = VIEW->havePort(iPlayer, RES_PORT3);
	bPort = VIEW->havePort(iPlayer, RES_PORTTIMBER + iRes);

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
				break;
			}
		}
	}

	//get the quantity pointer
	pQty = &m_Inhand1Qty + iRes;
	pTrade = &m_Inhand1Trade + iRes;

	//determine the number of cards being traded
	if(TRUE == bPort)
	{
		iTrade = 2;
		m_iBank[TRADE_2FOR1]++;
	}
	else if(TRUE == bThree)
	{
		iTrade = 3;
		m_iBank[TRADE_3FOR1]++;
	}
	else
	{
		iTrade = 4;
		m_iBank[TRADE_4FOR1]++;
	}

	//update the trade quantity
	m_iTrade[iRes] += iTrade;

	//decrease the onhand quantity
	iQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iTrade[iRes];

	//if it's zero, remove this card
	if(iQty == 0)
	{
		//get the other pointers
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;

		//remove the texts
		pText->ShowWindow(SW_HIDE);
		pQty->ShowWindow(SW_HIDE);

		//hide the window
		pButton->ShowWindow(SW_HIDE);
		pTrade->ShowWindow(SW_HIDE);
	}
	//or, if it's less then we can continue to trade, disable it
	else if(iQty < iTrade)
	{
		//get the other pointers
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;

		pButton->EnableWindow(FALSE);
		pText->EnableWindow(FALSE);
		pQty->EnableWindow(FALSE);
		pTrade->EnableWindow(FALSE);
	}
		
	//set the new text
	str.Format("%d", iQty);
	pQty->SetText(str);

	//update the trade data
	updateTrade(bNew, i);

	//increment trade counter
	m_iTradeCounter++;

	//see if we can trade
	m_TradeButton.EnableWindow(m_iTradeCounter && (m_iTradeCounter == m_iOTradeCounter));
}

/////////////////////////////////////////////////////////////////////////////
// update trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::updateTrade(BOOL bNew, int iPlace)
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
	}

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a bank card
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::HandleOHandClick(UINT nID)
{
	int i;
	int iRes;
	int iQty;
	BOOL bInList = FALSE;
	BOOL bNew = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CHoverStatic *pTrade;

	//get the resource type
	iRes = (nID - IDC_OHAND1_BUTTON);

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

				pQty = &m_Inhand1Qty + iRes;
				pQty->EnableWindow(FALSE);

				pTrade = &m_Inhand1Trade + iRes;
				pTrade->EnableWindow(FALSE);

				break;
			}
		}
	}

	//increase the number of cards being traded
	m_iOTrade[iRes]++;

	//get the onhand quantity
	iQty = GAME->getBankRes(iRes);

	//if it's zero, remove this card
	if(0 == (m_iOTrade[iRes] - iQty))
	{
		//get the other pointers
		pButton = &m_OHand1Button + iRes;
		pText = &m_OHand1Text + iRes;

		//remove the texts
		pText->ShowWindow(SW_HIDE);

		//hide the window
		pButton->ShowWindow(SW_HIDE);
	}
	else
	{
		//set the new text
		str.Format("%d", iQty);
	}

	//update the trade data
	updateOTrade(bNew, i);

	//set the trade bool
	m_iOTradeCounter++;

	//see if we can trade
	m_TradeButton.EnableWindow(m_iOTradeCounter && (m_iOTradeCounter == m_iTradeCounter));
}

/////////////////////////////////////////////////////////////////////////////
// update other player trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::updateOTrade(BOOL bNew, int iPlace)
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
	}

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the trade queue
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::HandleTradeClick(UINT nID)
{
	int iQty;
	int iRes;
	int iPlace;
	int iPlayer;
	int iTrade;
	BOOL bThree;
	BOOL bPort;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CHoverStatic *pTrade;

	//get the current place
	iPlace = (nID - IDC_TRADE1_BUTTON);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the resource in question
	iRes = m_iTradePlace[iPlace];

	//determine port status
	bThree = VIEW->havePort(iPlayer, RES_PORT3);
	bPort = VIEW->havePort(iPlayer, RES_PORTTIMBER + iRes);

	//determine the number of cards being traded
	if(TRUE == bPort)
	{
		iTrade = 2;
		m_iBank[TRADE_2FOR1]--;
	}
	else if(TRUE == bThree)
	{
		iTrade = 3;
		m_iBank[TRADE_3FOR1]--;
	}
	else
	{
		iTrade = 4;
		m_iBank[TRADE_4FOR1]--;
	}

	//subtract from the amounts being traded
	m_iTrade[iRes] -= iTrade;

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
	}
	else
	{
		//set the new quantity
		str.Format("%d", m_iTrade[iRes]);
		pQty->SetText(str);
	}

	//get the new in hand quantity
	iQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iTrade[iRes];

	//get the quantity button
	pQty = &m_Inhand1Qty + iRes;

	//if it's 1, then we need to re show this window and text
	if(iQty >= 1)
	{
		//get the buttons
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;
		pTrade = &m_Inhand1Trade + iRes;

		//show them
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
		pTrade->ShowWindow(SW_SHOW);

		//enable windows if it's tradeable
		if(iQty >= iTrade)
		{
			pButton->EnableWindow();
			pText->EnableWindow();
			pQty->EnableWindow();
			pTrade->EnableWindow();
		}
	}

	//update the quantity
	str.Format("%d", iQty);
	pQty->SetText(str);

	//subtract one from the amount trading
	m_iTradeCounter--;

	//see if we can trade
	m_TradeButton.EnableWindow(m_iTradeCounter && (m_iTradeCounter == m_iOTradeCounter));
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the other players trade queue
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::HandleOTradeClick(UINT nID)
{
	int iQty;
	int iRes;
	int iPlace;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CHoverStatic *pTrade;

	//get the current place
	iPlace = (nID - IDC_OTRADE1_BUTTON);

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

		pQty = &m_Inhand1Qty + iRes;
		pQty->EnableWindow();

		pTrade = &m_Inhand1Trade + iRes;
		pTrade->EnableWindow();
	}
	else
	{
		//set the new quantity
		str.Format("%d", m_iOTrade[iRes]);
		pQty->SetText(str);
	}

	//get the new in hand quantity
	iQty = GAME->getBankRes(iRes);

	//if it's 1, then we need to re show this window and text
	if(iQty == 1)
	{
		//get the buttons
		pButton = &m_OHand1Button + iRes;
		pText = &m_OHand1Text + iRes;

		//show them
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
	}

	//decrease counter
	m_iOTradeCounter--;

	//see if we can trade
	m_TradeButton.EnableWindow(m_iOTradeCounter && (m_iOTradeCounter == m_iTradeCounter));
}

/////////////////////////////////////////////////////////////////////////////
// handle trading to the bank
/////////////////////////////////////////////////////////////////////////////
void CTradeWithBankDialog::OnExecuteButton() 
{
	int iPlayer;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//send it on in network mode
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientTradeToBank(m_iTrade, m_iOTrade, m_iBank);
	}

	//handle common trade
	VIEW->handleCommonTradeToBank(iPlayer, m_iTrade, m_iOTrade, m_iBank);

	//clear the trade data
	clearTrade();
	clearOTrade();

	//update the players
	updatePlayer();
	updateOPlayer();
}
