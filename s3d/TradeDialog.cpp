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
#include "TradeDialog.h"
#include "SettlersView.h"
#include "UndoTrade.h"
#include "ColorDatabase.h"
#include "MapDatabase.h"
#include "defineVariants.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declaration
/////////////////////////////////////////////////////////////////////////////
void OnPlayerClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CTradeDialog dialog


CTradeDialog::CTradeDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CTradeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeDialog)
	//}}AFX_DATA_INIT
}


void CTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeDialog)
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	DDX_Control(pDX, IDOK, m_DoneButton);
	DDX_Control(pDX, IDC_EXECUTE_BUTTON, m_TradeButton);
	DDX_Control(pDX, IDC_OPLAYER_STATIC, m_OPlayerStatic);
	DDX_Control(pDX, IDC_PLAYER_STATIC, m_PlayerStatic);
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


BEGIN_MESSAGE_MAP(CTradeDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CTradeDialog)
	ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_INHAND1_BUTTON, IDC_INHAND5_BUTTON, HandleInHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OHAND1_BUTTON, IDC_OHAND5_BUTTON, HandleOHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TRADE1_BUTTON, IDC_TRADE4_BUTTON, HandleTradeClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OTRADE1_BUTTON, IDC_OTRADE4_BUTTON, HandleOTradeClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// initialize
/////////////////////////////////////////////////////////////////////////////
BOOL CTradeDialog::OnInitDialog() 
{
	int i;
	CHoverStatic *pStatic;
	CButtonST *pButton;
	CDC *dc;
	HDC hDC;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_TRADE;

	//set the player list callback
	m_PlayerList.setCallBack(OnPlayerClick, (DWORD) this);

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

	//set trade button text
	m_TradeButton.SetWindowText("&Trade");

	//release the device context
	ReleaseDC(dc);

	//intialize
	init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// initialize with a new player
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::init(void)
{
	int i;
	int iPlayer, iPlayers;
	CString str;

	//get the number of players
	iPlayers = GAME->m_players.size();

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set up the static buttons
	for(i = 0; i < iPlayers; i++)
	{
		//see if we can fill it
		if(i != iPlayer && GAME->m_players[i].canTrade())
		{
			//add the player
			m_PlayerList.addPlayer(&(GAME->m_players[i].m_player), GAME->m_players[i].m_iColor);
		}
	}

	//update the current player
	updatePlayer();	

	//update other player
	updateOPlayer();

	//clear trading arrays
	clearTrade();
	clearOTrade();

	//save current and hide all
	VIEW->showBuildOptions(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// update the current player data
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::updatePlayer()
{
	int i;
	int iPlayer;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CString str;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//set the static text
	m_PlayerStatic.SetWindowText(GAME->m_players[iPlayer].m_player.getName());

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_InHand1Button + i;
		pText = &m_Inhand1Text + i;
		pQty = &m_Inhand1Qty + i;

		//add cards if they have them
		if(GAME->m_players[iPlayer].m_iRes[i])
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
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the other players cards
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::updateOPlayer()
{
	int i;
	int iRes;
	int iPlayer;
	CButtonST *pButton;
	CHoverStatic *pText;

	//get the current selection
	iPlayer = m_PlayerList.getSelection();

	if(-1 == iPlayer)
	{
		//no text
		m_OPlayerStatic.SetWindowText("");
	}
	else
	{
		//set the static text
		m_OPlayerStatic.SetWindowText(GAME->m_players[iPlayer].m_player.getName());
	}

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_OHand1Button + i;
		pText = &m_OHand1Text + i;

		//see if this card is available
		if(-1 == iPlayer)
		{
			iRes = 0;
		}
		else
		{
			iRes = GAME->m_players[iPlayer].m_iRes[i];
		}

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
void CTradeDialog::clearTrade()
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//clear the memory array
	::ZeroMemory(m_iTrade, sizeof(m_iTrade));

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

	//set the number of cards being traded
	m_iNumTrade = 0;

	//disable the trade button
	m_TradeButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// clear the other player's trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::clearOTrade()
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

	//set the trade bool
	m_iNumOTrade = 0;

	//disable the trade button
	m_TradeButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a player
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::handleClick()
{
	//update the cards
	updateOPlayer();

	//clear any current trade
	clearOTrade();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on an inhand card
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::HandleInHandClick(UINT nID)
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
				break;
			}
		}
	}

	//get the quantity pointer
	pQty = &m_Inhand1Qty + iRes;

	//increase the number of cards being traded
	m_iTrade[iRes]++;

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
		pQty->SetText("");

		//hide the window
		pButton->ShowWindow(SW_HIDE);
	}
	else
	{
		//set the new text
		str.Format("%d", iQty);
		pQty->SetText(str);
	}

	//update the trade data
	updateTrade(bNew, i);

	//set the trade bool
	m_iNumTrade++;

	//see if we can trade
	enableTrade();
}

/////////////////////////////////////////////////////////////////////////////
// enable the trade button
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::enableTrade()
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
	m_TradeButton.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// update trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::updateTrade(BOOL bNew, int iPlace)
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
	if(bNew)
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
// handle a click on another player's card
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::HandleOHandClick(UINT nID)
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

	//get selected player
	iPlayer = m_PlayerList.getSelection();

	//sanity
	if(-1 == iPlayer)
	{
		return;
	}

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
				break;
			}
		}
	}

	//increase the number of cards being traded
	m_iOTrade[iRes]++;

	//get the onhand quantity
	iQty = GAME->m_players[iPlayer].m_iRes[iRes] - m_iOTrade[iRes];

	//if it's zero, remove this card
	if(iQty == 0)
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
	m_iNumOTrade++;

	//see if we can trade
	enableTrade();
}

/////////////////////////////////////////////////////////////////////////////
// update other player trade data
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::updateOTrade(BOOL bNew, int iPlace)
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
	if(bNew)
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
void CTradeDialog::HandleTradeClick(UINT nID)
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
	if(iQty == 1)
	{
		//get the buttons
		pButton = &m_InHand1Button + iRes;
		pText = &m_Inhand1Text + iRes;

		//show them
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
	}

	//update the quantity
	str.Format("%d", iQty);
	pQty->SetText(str);

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
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the other players trade queue
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::HandleOTradeClick(UINT nID)
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
	iPlayer = m_PlayerList.getSelection();

	//sanity
	if(-1 == iPlayer)
	{
		return;
	}

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
}

/////////////////////////////////////////////////////////////////////////////
// handle a trade
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::OnExecuteButton() 
{
	int iPlayer;
	int iPartner;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get trading partner
	iPartner = m_PlayerList.getSelection();

	//sanity
	if(-1 == iPartner)
	{
		return;
	}

	//handle common trade
	VIEW->handleCommonTrade(iPlayer, iPartner, m_iTrade, m_iOTrade);

	//clear the trade data
	clearTrade();
	clearOTrade();

	//update the players
	updatePlayer();
	updateOPlayer();
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::OnOK() 
{
	//make sure the turn window is showing
	VIEW->showBuildOptions(TRUE);
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle cancelling
/////////////////////////////////////////////////////////////////////////////
void CTradeDialog::OnCancel() 
{
	//make sure the build options are on
	VIEW->showBuildOptions(TRUE);
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnPlayerClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CTradeDialog *) dwCookie)->handleClick();
	}
}