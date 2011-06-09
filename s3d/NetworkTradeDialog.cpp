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
#include "NetworkTradeDialog.h"
#include "SettlersView.h"
#include "defineNetwork.h"
#include "defineSounds.h"
#include "defineVariants.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeDialog dialog

CNetworkTradeDialog::CNetworkTradeDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkTradeDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkTradeDialog)
	//}}AFX_DATA_INIT

	m_bNetTrade = FALSE;
}


void CNetworkTradeDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkTradeDialog)
	DDX_Control(pDX, IDC_INFO_BUTTON, m_InfoButton);
	DDX_Control(pDX, IDC_OPLAYER_STATIC, m_OPlayerStatic);
	DDX_Control(pDX, IDOK, m_DoneButton);
	DDX_Control(pDX, IDC_EXECUTE_BUTTON, m_TradeButton);
	DDX_Control(pDX, IDC_PLAYER_STATIC, m_PlayerStatic);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
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
	DDX_Control(pDX, IDC_COUNTER1_STATIC, m_Counter1Static);
	DDX_Control(pDX, IDC_COUNTER2_STATIC, m_Counter2Static);
	DDX_Control(pDX, IDC_COUNTER3_STATIC, m_Counter3Static);
	DDX_Control(pDX, IDC_COUNTER4_STATIC, m_Counter4Static);
	DDX_Control(pDX, IDC_COUNTER5_STATIC, m_Counter5Static);
	DDX_Control(pDX, IDC_ACCEPT1_BUTTON, m_Accept1Button);
	DDX_Control(pDX, IDC_ACCEPT2_BUTTON, m_Accept2Button);
	DDX_Control(pDX, IDC_ACCEPT3_BUTTON, m_Accept3Button);
	DDX_Control(pDX, IDC_ACCEPT4_BUTTON, m_Accept4Button);
	DDX_Control(pDX, IDC_ACCEPT5_BUTTON, m_Accept5Button);
	DDX_Control(pDX, IDC_C1T1_BUTTON, m_C1T1Button);
	DDX_Control(pDX, IDC_C1T2_BUTTON, m_C1T2Button);
	DDX_Control(pDX, IDC_C1T3_BUTTON, m_C1T3Button);
	DDX_Control(pDX, IDC_C1T4_BUTTON, m_C1T4Button);
	DDX_Control(pDX, IDC_C2T1_BUTTON, m_C2T1Button);
	DDX_Control(pDX, IDC_C2T2_BUTTON, m_C2T2Button);
	DDX_Control(pDX, IDC_C2T3_BUTTON, m_C2T3Button);
	DDX_Control(pDX, IDC_C2T4_BUTTON, m_C2T4Button);
	DDX_Control(pDX, IDC_C3T1_BUTTON, m_C3T1Button);
	DDX_Control(pDX, IDC_C3T2_BUTTON, m_C3T2Button);
	DDX_Control(pDX, IDC_C3T3_BUTTON, m_C3T3Button);
	DDX_Control(pDX, IDC_C3T4_BUTTON, m_C3T4Button);
	DDX_Control(pDX, IDC_C4T1_BUTTON, m_C4T1Button);
	DDX_Control(pDX, IDC_C4T2_BUTTON, m_C4T2Button);
	DDX_Control(pDX, IDC_C4T3_BUTTON, m_C4T3Button);
	DDX_Control(pDX, IDC_C4T4_BUTTON, m_C4T4Button);
	DDX_Control(pDX, IDC_C5T1_BUTTON, m_C5T1Button);
	DDX_Control(pDX, IDC_C5T2_BUTTON, m_C5T2Button);
	DDX_Control(pDX, IDC_C5T3_BUTTON, m_C5T3Button);
	DDX_Control(pDX, IDC_C5T4_BUTTON, m_C5T4Button);
	DDX_Control(pDX, IDC_C1O1_BUTTON, m_C1O1Button);
	DDX_Control(pDX, IDC_C1O2_BUTTON, m_C1O2Button);
	DDX_Control(pDX, IDC_C1O3_BUTTON, m_C1O3Button);
	DDX_Control(pDX, IDC_C1O4_BUTTON, m_C1O4Button);
	DDX_Control(pDX, IDC_C2O1_BUTTON, m_C2O1Button);
	DDX_Control(pDX, IDC_C2O2_BUTTON, m_C2O2Button);
	DDX_Control(pDX, IDC_C2O3_BUTTON, m_C2O3Button);
	DDX_Control(pDX, IDC_C2O4_BUTTON, m_C2O4Button);
	DDX_Control(pDX, IDC_C3O1_BUTTON, m_C3O1Button);
	DDX_Control(pDX, IDC_C3O2_BUTTON, m_C3O2Button);
	DDX_Control(pDX, IDC_C3O3_BUTTON, m_C3O3Button);
	DDX_Control(pDX, IDC_C3O4_BUTTON, m_C3O4Button);
	DDX_Control(pDX, IDC_C4O1_BUTTON, m_C4O1Button);
	DDX_Control(pDX, IDC_C4O2_BUTTON, m_C4O2Button);
	DDX_Control(pDX, IDC_C4O3_BUTTON, m_C4O3Button);
	DDX_Control(pDX, IDC_C4O4_BUTTON, m_C4O4Button);
	DDX_Control(pDX, IDC_C5O1_BUTTON, m_C5O1Button);
	DDX_Control(pDX, IDC_C5O2_BUTTON, m_C5O2Button);
	DDX_Control(pDX, IDC_C5O3_BUTTON, m_C5O3Button);
	DDX_Control(pDX, IDC_C5O4_BUTTON, m_C5O4Button);
	DDX_Control(pDX, IDC_C1T1_QTY, m_C1T1Qty);
	DDX_Control(pDX, IDC_C1T2_QTY, m_C1T2Qty);
	DDX_Control(pDX, IDC_C1T3_QTY, m_C1T3Qty);
	DDX_Control(pDX, IDC_C1T4_QTY, m_C1T4Qty);
	DDX_Control(pDX, IDC_C2T1_QTY, m_C2T1Qty);
	DDX_Control(pDX, IDC_C2T2_QTY, m_C2T2Qty);
	DDX_Control(pDX, IDC_C2T3_QTY, m_C2T3Qty);
	DDX_Control(pDX, IDC_C2T4_QTY, m_C2T4Qty);
	DDX_Control(pDX, IDC_C3T1_QTY, m_C3T1Qty);
	DDX_Control(pDX, IDC_C3T2_QTY, m_C3T2Qty);
	DDX_Control(pDX, IDC_C3T3_QTY, m_C3T3Qty);
	DDX_Control(pDX, IDC_C3T4_QTY, m_C3T4Qty);
	DDX_Control(pDX, IDC_C4T1_QTY, m_C4T1Qty);
	DDX_Control(pDX, IDC_C4T2_QTY, m_C4T2Qty);
	DDX_Control(pDX, IDC_C4T3_QTY, m_C4T3Qty);
	DDX_Control(pDX, IDC_C4T4_QTY, m_C4T4Qty);
	DDX_Control(pDX, IDC_C5T1_QTY, m_C5T1Qty);
	DDX_Control(pDX, IDC_C5T2_QTY, m_C5T2Qty);
	DDX_Control(pDX, IDC_C5T3_QTY, m_C5T3Qty);
	DDX_Control(pDX, IDC_C5T4_QTY, m_C5T4Qty);
	DDX_Control(pDX, IDC_C1O1_QTY, m_C1O1Qty);
	DDX_Control(pDX, IDC_C1O2_QTY, m_C1O2Qty);
	DDX_Control(pDX, IDC_C1O3_QTY, m_C1O3Qty);
	DDX_Control(pDX, IDC_C1O4_QTY, m_C1O4Qty);
	DDX_Control(pDX, IDC_C2O1_QTY, m_C2O1Qty);
	DDX_Control(pDX, IDC_C2O2_QTY, m_C2O2Qty);
	DDX_Control(pDX, IDC_C2O3_QTY, m_C2O3Qty);
	DDX_Control(pDX, IDC_C2O4_QTY, m_C2O4Qty);
	DDX_Control(pDX, IDC_C3O1_QTY, m_C3O1Qty);
	DDX_Control(pDX, IDC_C3O2_QTY, m_C3O2Qty);
	DDX_Control(pDX, IDC_C3O3_QTY, m_C3O3Qty);
	DDX_Control(pDX, IDC_C3O4_QTY, m_C3O4Qty);
	DDX_Control(pDX, IDC_C4O1_QTY, m_C4O1Qty);
	DDX_Control(pDX, IDC_C4O2_QTY, m_C4O2Qty);
	DDX_Control(pDX, IDC_C4O3_QTY, m_C4O3Qty);
	DDX_Control(pDX, IDC_C4O4_QTY, m_C4O4Qty);
	DDX_Control(pDX, IDC_C5O1_QTY, m_C5O1Qty);
	DDX_Control(pDX, IDC_C5O2_QTY, m_C5O2Qty);
	DDX_Control(pDX, IDC_C5O3_QTY, m_C5O3Qty);
	DDX_Control(pDX, IDC_C5O4_QTY, m_C5O4Qty);
	DDX_Control(pDX, IDC_FOR1_STATIC, m_For1Static);
	DDX_Control(pDX, IDC_FOR2_STATIC, m_For2Static);
	DDX_Control(pDX, IDC_FOR3_STATIC, m_For3Static);
	DDX_Control(pDX, IDC_FOR4_STATIC, m_For4Static);
	DDX_Control(pDX, IDC_FOR5_STATIC, m_For5Static);
	DDX_Control(pDX, IDC_TRADE_OFFER_STATIC, m_TradeOfferStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkTradeDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkTradeDialog)
	ON_BN_CLICKED(IDC_EXECUTE_BUTTON, OnExecuteButton)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_ACCEPT1_BUTTON, IDC_ACCEPT5_BUTTON, HandleAcceptClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_INHAND1_BUTTON, IDC_INHAND5_BUTTON, HandleInHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OHAND1_BUTTON, IDC_OHAND5_BUTTON, HandleOHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_TRADE1_BUTTON, IDC_TRADE4_BUTTON, HandleTradeClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_OTRADE1_BUTTON, IDC_OTRADE4_BUTTON, HandleOTradeClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkTradeDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// get everything ready
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkTradeDialog::OnInitDialog() 
{
	int i;
	int iDif;
	CHoverStatic *pStatic;
	CButtonST *pButton;
	CDC *dc;
	HDC hDC;
	CRect rect;
	CRect rectClient;
	CString str;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_NETTRADE;

	//set info text
	m_InfoButton.SetFont(Verdana12);
	m_InfoButton.SetColors(COLOR_GRAY, COLOR_GRAY);
	m_InfoButton.SetText("Left side: what they want from you.  Right side: what they'll give to you.");

	//set trade offer text
	str.Format("Offers left: %d", (VARIANTS->m_nTradeOffers - GAME->m_players[VIEW->getPlayer()].m_nTradeOffers));
	m_TradeOfferStatic.SetFont(Verdana12);
	m_TradeOfferStatic.SetColors(COLOR_GRAY, COLOR_GRAY);
	m_TradeOfferStatic.SetText(str);

	//show hide it
	if(IS_VARIANT(VARIANT_LIMIT_TRADING))
	{
		m_TradeOfferStatic.ShowWindow(SW_SHOW);
	}
	else
	{
		m_TradeOfferStatic.ShowWindow(SW_HIDE);
	}

	//get screen size
	GetWindowRect(&rect);

	//get client rect
	GetClientRect(&rectClient);

	//calculate height dif
	iDif = rect.Height() - rectClient.Height();

	//counteroffer offsets
	m_iCounterOffsets[0] = iDif + 299;
	m_iCounterOffsets[1] = iDif + 392;
	m_iCounterOffsets[2] = iDif + 470;
	m_iCounterOffsets[3] = iDif + 548;
	m_iCounterOffsets[4] = iDif + 627;
	m_iCounterOffsets[5] = rect.Height();

	//hide counteroffer section
	showCounterSection(0);
	
	//set the pointer
	VIEW->setTradeDialog(this);

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

	for(i = 0; i < 40; i++)
	{
		//get the static
		pStatic = &m_C1T1Qty + i;

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

	//keep going
	initPlayer();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// initialize with a new player
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::initPlayer(void)
{
	int i;
	int iPlayer, iPlayers;
	int nTeam;
	CString str;

	//get the number of players
	iPlayers = GAME->m_players.size();

	//get the current player
	iPlayer = VIEW->getPlayer();

	//in team games, only the team member gets added
	if(TRUE == GAME->m_bTeam)
	{
		//get their teammate
		nTeam = ((iPlayer + 2) % 4);

		m_PlayerList.addPlayer(&(GAME->m_players[nTeam].m_player), GAME->m_players[nTeam].m_iColor);
	}
	else
	{
		//set up the static buttons
		for(i = 0; i < iPlayers; i++)
		{
			if(iPlayer != i)
			{
				//add all players
				m_PlayerList.addPlayer(&(GAME->m_players[i].m_player), GAME->m_players[i].m_iColor);
			}
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
void CNetworkTradeDialog::updatePlayer()
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
void CNetworkTradeDialog::updateOPlayer()
{
	int i;
	int iRes;
	int iPlayer;
	CButtonST *pButton;
	CHoverStatic *pText;

	//see who's turn it is
	iPlayer = VIEW->getPlayer();

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_OHand1Button + i;
		pText = &m_OHand1Text + i;

		//see if this card is available - show all available cards, not just what
		//others have in their hands
		iRes = MAP->m_iBankRes;

		//add cards if they have them
		if(0 < iRes)
		{
			//set the button
			pButton->ShowWindow(SW_SHOW);
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
void CNetworkTradeDialog::clearTrade(BOOL bClearData)
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//clear the memory array
	if(TRUE == bClearData)
	{
		::ZeroMemory(m_iTrade, sizeof(m_iTrade));
	}

	//clear the trade screens
	for(i = 0; i < 4; i++)
	{
		if(TRUE == bClearData)
		{
			//clear the place memory
			m_iTradePlace[i] = -1;
		}

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

	//set the trade bool
	m_iNumTrade = 0;

	//disable the trade button
	m_TradeButton.EnableWindow(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// clear the other player's trade data
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::clearOTrade(BOOL bClearData)
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//clear the memory array
	if(TRUE == bClearData)
	{
		::ZeroMemory(m_iOTrade, sizeof(m_iOTrade));
	}

	//clear the trade screens
	for(i = 0; i < 4; i++)
	{
		if(TRUE == bClearData)
		{
			//clear the place memory
			m_iOTradePlace[i] = -1;
		}

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
// handle a trade!
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::HandleAcceptClick(UINT nID)
{
	int i;
	int iPlayer;
	int iPartner;
	int iButton;
	int iTrade[5];
	int iOTrade[5];

	//get the button clicked
	iButton = (nID - IDC_ACCEPT1_BUTTON);

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get partner
	iPartner = m_iPlayerReply[iButton];

	//create arrays
	for(i = 0; i < 5; i++)
	{
		iTrade[i] = m_iTradeArray[iButton][i];
		iOTrade[i] = m_iOTradeArray[iButton][i];
	}

	//handle common trade
	VIEW->handleCommonTrade(iPlayer, iPartner, iTrade, iOTrade);

	//tell network
	VIEW->clientTrade(iPartner, iTrade, iOTrade);

	//reset stuff
	clearTrade();
	clearOTrade();

	//update the players
	updatePlayer();
	updateOPlayer();

	//reset stuff
	showCounterSection(0);

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on an inhand card
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::HandleInHandClick(UINT nID)
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
	checkTrade();

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// update trade data
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::updateTrade(BOOL bNew, int iPlace)
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
// handle a click on another player's card
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::HandleOHandClick(UINT nID)
{
	int i;
	int iRes;
	int iQty;
	BOOL bNew = FALSE;
	BOOL bInList = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;

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
	iQty = MAP->m_iBankRes - m_iOTrade[iRes];

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
	checkTrade();

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// update other player trade data
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::updateOTrade(BOOL bNew, int iPlace)
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
void CNetworkTradeDialog::HandleTradeClick(UINT nID)
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
	checkTrade();

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a card currently in the other players trade queue
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::HandleOTradeClick(UINT nID)
{
	int i;
	int iQty;
	int iRes;
	int iPlace;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

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
	}
	else
	{
		//set the new quantity
		str.Format("%d", m_iOTrade[iRes]);
		pQty->SetText(str);
	}

	//get the new in hand quantity
	iQty = MAP->m_iBankRes - m_iOTrade[iRes];

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
	checkTrade();

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the trade execute button
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::checkTrade()
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
	
	//check remaining trade offers
	if(IS_VARIANT(VARIANT_LIMIT_TRADING) && (0 >= (VARIANTS->m_nTradeOffers - GAME->m_players[VIEW->getPlayer()].m_nTradeOffers)))
	{
		bEnable = FALSE;
	}

	//see if there are players to trade to
	m_bNetTrade = m_PlayerList.isValid();

	//enable the button
	m_TradeButton.EnableWindow(bEnable && m_bNetTrade);
}

/////////////////////////////////////////////////////////////////////////////
// send the trade offer
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::OnExecuteButton() 
{
	int i;
	int iCount = 0;
	CString str;
	CArray <UINT, UINT> uiPlayers;

	//clear out everything
	updatePlayer();
	updateOPlayer();
	clearTrade(FALSE);
	clearOTrade(FALSE);

	//reset all extra statii
	m_PlayerList.resetStatus();

	//fill the player array with those getting the trade offer
	for(i = 0; i < m_PlayerList.GetItemCount(); i++)
	{
		if(TRUE == m_PlayerList.getCheck(i))
		{
			//increment the count
			iCount++;

			//add the player to the offer list
			uiPlayers.Add((UINT) m_PlayerList.GetItemData(i));

			//update the status to trade offered while we're at it
			m_PlayerList.updateStatus(i, TRADE_OFFER);
		}
	}

	//reset current spot
	m_iCurrentReply = 0;

	//clear trade arrays
	::ZeroMemory(m_iTradeArray, sizeof(m_iTradeArray));
	::ZeroMemory(m_iOTradeArray, sizeof(m_iOTradeArray));

	//clear reply spots
	memset(m_iPlayerReply, -1, sizeof(m_iPlayerReply));

	//disable everything
	enableTrade(FALSE);

	//show counter offer section
	showCounterSection(iCount);

	//now offer the trade
	VIEW->clientTradeOffer(m_iTrade, m_iOTrade, uiPlayers);

	//update their trade offers used
	if(IS_VARIANT(VARIANT_LIMIT_TRADING))
	{
		GAME->m_players[VIEW->getPlayer()].m_nTradeOffers++;

		//format the string and set it
		str.Format("Offers left: %d", (VARIANTS->m_nTradeOffers - GAME->m_players[VIEW->getPlayer()].m_nTradeOffers));
		m_TradeOfferStatic.SetText(str);
	}

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable stuff during a trade
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::enableTrade(BOOL bEnable)
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pStatic;

	//enable/disable all stuff

	//they can't leave until everyone responds
	m_DoneButton.EnableWindow(bEnable);

	//player list
	m_PlayerList.EnableWindow(bEnable);

	//miscellaneous buttons
	m_TradeButton.EnableWindow(bEnable);
	m_OPlayerStatic.EnableWindow(bEnable);
	m_PlayerStatic.EnableWindow(bEnable);

	//trade buttons
	for(i = 0; i < 18; i++)
	{
		pButton = &m_Trade1Button + i;
		pButton->EnableWindow(bEnable);
	}

	//hover statics
	for(i = 0; i < 31; i++)
	{
		pStatic = &m_Trade1Text + i;
		pStatic->EnableWindow(bEnable);
	}
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the counteroffer section
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::showCounterSection(int iCount)
{
	int i;
	CRect rect;
	CButton *pButton;
	CStatic *pFor;
	CButtonST *pReply;
	CHoverStatic *pQty;

	//get our place in the window
	GetWindowRect(&rect);
	
	//hide lower part of screen until ready
	SetWindowPos(&wndTop, 0, 0, rect.Width(), m_iCounterOffsets[iCount], SWP_NOMOVE | SWP_NOZORDER);

	//set counter text
	for(i = 0; i < iCount; i++)
	{
		//get button
		pButton = &m_Counter1Static + i;
		pButton->SetWindowText("Waiting for reply...");
		
		//hide accept button
		pButton = &m_Accept1Button + i;
		pButton->ShowWindow(SW_HIDE);

		//hide for button
		pFor = &m_For1Static + i;
		pFor->ShowWindow(SW_HIDE);
	}

	//hide reply offer buttons
	for(i = 0; i < 4 * iCount; i++)
	{
		pReply = &m_C1T1Button + i;
		pReply->ShowWindow(SW_HIDE);

		pQty = &m_C1T1Qty + i;
		pQty->ShowWindow(SW_HIDE);	

		pReply = &m_C1O1Button + i;
		pReply->ShowWindow(SW_HIDE);

		pQty = &m_C1O1Qty + i;
		pQty->ShowWindow(SW_HIDE);	
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a trade reply
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::handleReply(int iPlayer, short shStatus, int *iTo, int *iFor)
{
	int i, j;
	BOOL bCanTrade;
	CString str;
	CButton *pButton;

	//sanity check to see if this player can even accept this trade
	if(TRADE_ACCEPT == shStatus)
	{
		for(i = 0; i < 5; i++)
		{
			if(GAME->m_players[iPlayer].m_iRes[i] < m_iOTrade[i])
			{
				shStatus = TRADE_REJECT;
				break;
			}
		}
	}

	//update status for this player
	m_PlayerList.handleReply(iPlayer, shStatus);

	//show what they said
	switch(shStatus)
	{
	case TRADE_REJECT: handleReject(iPlayer); break;
	case TRADE_ACCEPT: handleAccept(iPlayer); break;
	case TRADE_COUNTER: handleCounter(iPlayer); break;
	case TRADE_COFFER: handleCounterOffer(iPlayer, iTo, iFor); break;
	default: break;
	}

	//see if we're done
	shStatus = (short) m_PlayerList.isComplete();

	if(TRADE_OFFER != shStatus)
	{
		//reset all
		updatePlayer();
		updateOPlayer();
		clearTrade();
		clearOTrade();

		//if everyone rejected, close up shop
		if(TRADE_REJECT == shStatus)
		{
			//reset stuff
			showCounterSection(0);
		}
		else
		{
			//go through the list and see who we can actually trade with
			for(i = 0; i < m_iCurrentReply; i++)
			{
				//get player
				iPlayer = m_iPlayerReply[i];

				//see if they didn't reject us
				if(TRADE_REJECT != m_PlayerList.getStatus(iPlayer))
				{
					bCanTrade = TRUE;

					//determine if we can trade with this person
					for(j = 0; j < 5; j++)
					{
						if(GAME->m_players[GAME->m_iIndex].m_iRes[j] < m_iTradeArray[i][j])
						{
							bCanTrade = FALSE;
							break;
						}
					}

					//get the button
					pButton = &m_Accept1Button + i;

					//enable their trade button
					pButton->EnableWindow(bCanTrade);
				}
			}
		}

		//reenable everything
		enableTrade(TRUE);
		checkTrade();
	}

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a player saying no thanks
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::handleReject(int iPlayer)
{
	int i;
	int iPlace = -1;
	CString str;
	CButton *pButton;

	//play sound
	SOUND(SOUND_TRADE_REJECT);

	//look for the player in the current list
	for(i = 0; i < 5; i++)
	{
		if(iPlayer == m_iPlayerReply[i])
		{
			iPlace = i;
			break;
		}
	}

	//set the place
	if(-1 == iPlace)
	{
		iPlace = m_iCurrentReply;
		
		//set the player
		m_iPlayerReply[iPlace] = iPlayer;

		//increment the next position
		m_iCurrentReply++;
	}

	//get the right frame
	pButton = &m_Counter1Static + iPlace;

	//format string
	str.Format("%s rejected the offer", GAME->m_players[iPlayer].m_player.getName());

	//set the text
	pButton->SetWindowText(str);

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a player starting a counteroffer
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::handleCounter(int iPlayer)
{
	CString str;
	CButton *pButton;

	//get the right frame
	pButton = &m_Counter1Static + m_iCurrentReply;

	//set the player
	m_iPlayerReply[m_iCurrentReply] = iPlayer;

	//increment the next position
	m_iCurrentReply++;

	//format string
	str.Format("%s is going to make a counteroffer", GAME->m_players[iPlayer].m_player.getName());

	//set the text
	pButton->SetWindowText(str);

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a player making a counteroffer
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::handleCounterOffer(int iPlayer, int *iTo, int *iFor)
{
	int i, j, k;
	int iRes;
	int iPlace = -1;
	CDC *dc;
	HDC hDC;
	CString str;
	CStatic *pFor;
	CButton *pBorder;
	CButtonST *pButton = NULL;
	CHoverStatic *pQty = NULL;

	//play sound
	SOUND(SOUND_TRADE_OFFER);

	//look for the player in the current list
	for(i = 0; i < 5; i++)
	{
		if(iPlayer == m_iPlayerReply[i])
		{
			iPlace = i;
			break;
		}
	}

	//sanity check
	if(-1 == iPlace)
	{
		return;
	}

	//set text
	pBorder = &m_Counter1Static + iPlace;
	str.Format("%s makes a counteroffer", GAME->m_players[iPlayer].m_player.getName());
	pBorder->SetWindowText(str);

	//show for text
	pFor = &m_For1Static + iPlace;
	pFor->ShowWindow(SW_SHOW);

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//run through the incoming offer and make it the same
	for(i = 0, j = 0, k = 0; i < 5; i++)
	{
		//set data
		m_iTradeArray[iPlace][i] = iTo[i];

		switch(iPlace)
		{
		case 0: pButton = &m_C1T1Button + j; pQty = &m_C1T1Qty + j; break;
		case 1: pButton = &m_C2T1Button + j; pQty = &m_C2T1Qty + j; break;
		case 2: pButton = &m_C3T1Button + j; pQty = &m_C3T1Qty + j; break;
		case 3: pButton = &m_C4T1Button + j; pQty = &m_C4T1Qty + j; break;
		case 4: pButton = &m_C5T1Button + j; pQty = &m_C5T1Qty + j; break;
		}

		//set values
		if(0 != iTo[i])
		{
			iRes = i;
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			str.Format("%d", iTo[iRes]);
			pQty->SetText(str);

			//show windows
			pButton->EnableWindow(FALSE);
			pButton->ShowWindow(SW_SHOW);
			pQty->ShowWindow(SW_SHOW);

			//increment placement
			j++;
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}

		//set data
		m_iOTradeArray[iPlace][i] = iFor[i];

		//repeat for other side
		switch(iPlace)
		{
		case 0: pButton = &m_C1O1Button + k; pQty = &m_C1O1Qty + k; break;
		case 1: pButton = &m_C2O1Button + k; pQty = &m_C2O1Qty + k; break;
		case 2: pButton = &m_C3O1Button + k; pQty = &m_C3O1Qty + k; break;
		case 3: pButton = &m_C4O1Button + k; pQty = &m_C4O1Qty + k; break;
		case 4: pButton = &m_C5O1Button + k; pQty = &m_C5O1Qty + k; break;
		}

		//set values
		if(0 != iFor[i])
		{
			iRes = i;
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			str.Format("%d", iFor[iRes]);
			pQty->SetText(str);

			//show windows
			pButton->EnableWindow(FALSE);
			pButton->ShowWindow(SW_SHOW);
			pQty->ShowWindow(SW_SHOW);

			//increment placement
			k++;
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}
	}

	//release context
	ReleaseDC(dc);

	//show the trade button but don't enable it yet
	pBorder = &m_Accept1Button + iPlace;
	pBorder->EnableWindow(FALSE);
	pBorder->ShowWindow(SW_SHOW);

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle a player saying yes, I will
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::handleAccept(int iPlayer)
{
	int i;
	int iRes;
	int iPlace = -1;
	CDC *dc;
	HDC hDC;
	CString str;
	CStatic *pFor;
	CButton *pBorder;
	CButtonST *pButton = NULL;
	CHoverStatic *pQty = NULL;

	//play sound
	SOUND(SOUND_TRADE_ACCEPT);

	//look for the player in the current list
	for(i = 0; i < 5; i++)
	{
		if(iPlayer == m_iPlayerReply[i])
		{
			iPlace = i;
			break;
		}
	}

	//set the place
	if(-1 == iPlace)
	{
		iPlace = m_iCurrentReply;
		
		//set the player
		m_iPlayerReply[iPlace] = iPlayer;

		//increment the next position
		m_iCurrentReply++;
	}

	//set text
	pBorder = &m_Counter1Static + iPlace;
	str.Format("%s accepts the offer", GAME->m_players[iPlayer].m_player.getName());
	pBorder->SetWindowText(str);

	//show for text
	pFor = &m_For1Static + iPlace;
	pFor->ShowWindow(SW_SHOW);

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//run through the established offer and make it the same
	for(i = 0; i < 4; i++)
	{
		switch(iPlace)
		{
		case 0: pButton = &m_C1T1Button + i; pQty = &m_C1T1Qty + i; break;
		case 1: pButton = &m_C2T1Button + i; pQty = &m_C2T1Qty + i; break;
		case 2: pButton = &m_C3T1Button + i; pQty = &m_C3T1Qty + i; break;
		case 3: pButton = &m_C4T1Button + i; pQty = &m_C4T1Qty + i; break;
		case 4: pButton = &m_C5T1Button + i; pQty = &m_C5T1Qty + i; break;
		}

		//set values
		if(-1 != m_iTradePlace[i])
		{
			iRes = m_iTradePlace[i];
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			str.Format("%d", m_iTrade[iRes]);
			pQty->SetText(str);

			//show windows
			pButton->EnableWindow(FALSE);
			pButton->ShowWindow(SW_SHOW);
			pQty->ShowWindow(SW_SHOW);

			//set trade array spot
			m_iTradeArray[iPlace][iRes] = m_iTrade[iRes];
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}

		//repeat for other side
		switch(iPlace)
		{
		case 0: pButton = &m_C1O1Button + i; pQty = &m_C1O1Qty + i; break;
		case 1: pButton = &m_C2O1Button + i; pQty = &m_C2O1Qty + i; break;
		case 2: pButton = &m_C3O1Button + i; pQty = &m_C3O1Qty + i; break;
		case 3: pButton = &m_C4O1Button + i; pQty = &m_C4O1Qty + i; break;
		case 4: pButton = &m_C5O1Button + i; pQty = &m_C5O1Qty + i; break;
		}

		//set values
		if(-1 != m_iOTradePlace[i])
		{
			iRes = m_iOTradePlace[i];
			pButton->SetBitmaps(VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP, VIEW->getResImage(iRes, dc->GetSafeHdc()), COLOR_TRANSP);
			str.Format("%d", m_iOTrade[iRes]);
			pQty->SetText(str);

			//show windows
			pButton->EnableWindow(FALSE);
			pButton->ShowWindow(SW_SHOW);
			pQty->ShowWindow(SW_SHOW);

			//set other trade array spot
			m_iOTradeArray[iPlace][iRes] = m_iOTrade[iRes];
		}
		else
		{
			pButton->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}
	}

	//release context
	ReleaseDC(dc);

	//show the trade button but don't enable it yet
	pBorder = &m_Accept1Button + iPlace;
	pBorder->EnableWindow(FALSE);
	pBorder->ShowWindow(SW_SHOW);

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::OnOK() 
{
	//make sure the turn window is showing
	VIEW->showBuildOptions(TRUE);

	//make app aware
	VIEW->setDialog(NULL);

	//no more trade dialog
	VIEW->setTradeDialog(NULL);
	
	//run standard
	CHelpDialog::OnOK();

	//reneable
	CONTROLPANEL->InTrade(FALSE);

	//clean up
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// handle cancelling
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::OnCancel() 
{
	//make sure the build options are on
	VIEW->showBuildOptions(TRUE);

	//make app aware
	VIEW->setDialog(NULL);

	//no more trade dialog
	VIEW->setTradeDialog(NULL);
	
	//run standard
	CHelpDialog::OnCancel();

	//reneable
	CONTROLPANEL->InTrade(FALSE);

	//clean up
	delete this;
}

/////////////////////////////////////////////////////////////////////////////
// clean up
/////////////////////////////////////////////////////////////////////////////
void CNetworkTradeDialog::OnDestroy() 
{
	CHelpDialog::OnDestroy();
	
	delete this;
}
