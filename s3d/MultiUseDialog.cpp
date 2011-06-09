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
#include "MultiUseDialog.h"
#include "UndoOne.h"
#include "SettlersView.h"
#include "rand.h"
#include "ColorDatabase.h"
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
void OnClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CMultiUseDialog dialog


CMultiUseDialog::CMultiUseDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CMultiUseDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiUseDialog)
	//}}AFX_DATA_INIT

	//reset the mode
	m_iMode = -1;

	//not going to show player's cards in hotseat
	m_bShowPlayer = FALSE;

	//set the pirate flag
	m_bPirate = FALSE;
}


void CMultiUseDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiUseDialog)
	DDX_Control(pDX, IDC_INHAND1_QTY, m_InHand1Qty);
	DDX_Control(pDX, IDC_INHAND2_QTY, m_InHand2Qty);
	DDX_Control(pDX, IDC_INHAND3_QTY, m_InHand3Qty);
	DDX_Control(pDX, IDC_INHAND4_QTY, m_InHand4Qty);
	DDX_Control(pDX, IDC_INHAND5_QTY, m_InHand5Qty);
	DDX_Control(pDX, IDC_TOLOSE_STATIC, m_CardsToLoseStatic);
	DDX_Control(pDX, IDC_LOST_STATIC, m_CardsLostStatic);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	DDX_Control(pDX, IDCANCEL, m_CancelButton);
	DDX_Control(pDX, IDC_SHOWCARDS_CHECK, m_ShowCardsCheck);
	DDX_Control(pDX, IDOK, m_DoneButton);
	DDX_Control(pDX, IDC_WHO_STATIC, m_WhoStatic);
	DDX_Control(pDX, IDC_SEL_STATIC, m_SelStatic);
	DDX_Control(pDX, IDC_PLAYER_STATIC, m_PlayerStatic);
	DDX_Control(pDX, IDC_SEL1_TEXT, m_Sel1Text);
	DDX_Control(pDX, IDC_SEL2_TEXT, m_Sel2Text);
	DDX_Control(pDX, IDC_SEL3_TEXT, m_Sel3Text);
	DDX_Control(pDX, IDC_SEL4_TEXT, m_Sel4Text);
	DDX_Control(pDX, IDC_SEL5_TEXT, m_Sel5Text);
	DDX_Control(pDX, IDC_SEL1_QTY, m_Sel1Qty);
	DDX_Control(pDX, IDC_SEL2_QTY, m_Sel2Qty);
	DDX_Control(pDX, IDC_SEL3_QTY, m_Sel3Qty);
	DDX_Control(pDX, IDC_SEL4_QTY, m_Sel4Qty);
	DDX_Control(pDX, IDC_SEL5_QTY, m_Sel5Qty);
	DDX_Control(pDX, IDC_INHAND1_TEXT, m_InHand1Text);
	DDX_Control(pDX, IDC_INHAND2_TEXT, m_InHand2Text);
	DDX_Control(pDX, IDC_INHAND3_TEXT, m_InHand3Text);
	DDX_Control(pDX, IDC_INHAND4_TEXT, m_InHand4Text);
	DDX_Control(pDX, IDC_INHAND5_TEXT, m_InHand5Text);
	DDX_Control(pDX, IDC_SEL1_BUTTON, m_Sel1Button);
	DDX_Control(pDX, IDC_SEL2_BUTTON, m_Sel2Button);
	DDX_Control(pDX, IDC_SEL3_BUTTON, m_Sel3Button);
	DDX_Control(pDX, IDC_SEL4_BUTTON, m_Sel4Button);
	DDX_Control(pDX, IDC_SEL5_BUTTON, m_Sel5Button);
	DDX_Control(pDX, IDC_INHAND1_BUTTON, m_InHand1Button);
	DDX_Control(pDX, IDC_INHAND2_BUTTON, m_InHand2Button);
	DDX_Control(pDX, IDC_INHAND3_BUTTON, m_InHand3Button);
	DDX_Control(pDX, IDC_INHAND4_BUTTON, m_InHand4Button);
	DDX_Control(pDX, IDC_INHAND5_BUTTON, m_InHand5Button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiUseDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CMultiUseDialog)
	ON_BN_CLICKED(IDC_SHOWCARDS_CHECK, OnShowcardsCheck)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, IDC_INHAND1_BUTTON, IDC_INHAND5_BUTTON, HandleInHandClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_SEL1_BUTTON, IDC_SEL5_BUTTON, HandleSelClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiUseDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// set everything up
/////////////////////////////////////////////////////////////////////////////
BOOL CMultiUseDialog::OnInitDialog() 
{
	int i;
	BOOL bShowCards;
	CButtonST *pButton;
	CHoverStatic *pStatic;
	CDC *dc;
	HDC hDC;

	CHelpDialog::OnInitDialog();

	//set the help system value
	switch(m_iMode)
	{
	case MULTIUSE_YEAR: m_strHelp = HELP_GAMEPLAY; break;
	case MULTIUSE_STEAL: m_strHelp = HELP_STEAL; break;
	case MULTIUSE_LOSE: m_strHelp = HELP_LOSE7; break;
	case MULTIUSE_GOLD: m_strHelp = HELP_GOLD; break;
	}

	//set the player list callback
	m_PlayerList.setCallBack(OnClick, (DWORD) this);

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//if we're in network mode, don't allow cancel
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		m_CancelButton.EnableWindow(FALSE);
	}

	//set up cards to lose statics
	m_CardsToLoseStatic.SetFont(Tahoma12);
	m_CardsLostStatic.SetFont(Tahoma12);

	//show hide
	if(MULTIUSE_LOSE == m_iMode)
	{
		m_CardsToLoseStatic.ShowWindow(SW_SHOW);
		m_CardsLostStatic.ShowWindow(SW_SHOW);

		for(i = 0; i < 5; i++)
		{
			m_InHand1Qty.ShowWindow(SW_SHOW);
		}
	}
	else
	{
		if(MULTIUSE_GOLD == m_iMode)
		{
			m_CardsToLoseStatic.ShowWindow(SW_SHOW);
			m_CardsLostStatic.ShowWindow(SW_SHOW);
		}
		else
		{
			m_CardsToLoseStatic.ShowWindow(SW_HIDE);
			m_CardsLostStatic.ShowWindow(SW_HIDE);
		}

		for(i = 0; i < 5; i++)
		{
			m_InHand1Qty.ShowWindow(SW_HIDE);
		}
	}

	//set up all the hoverstatics
	for(i = 0; i < 20; i++)
	{
		//get the static
		pStatic = &m_Sel1Text + i;

		//set up values
		pStatic->SetFont(Tahoma12);
		pStatic->SetCenter();
	}
	
	//set up all the statics
	for(i = 0; i < 5; i++)
	{
		//get the button
		pStatic = &m_InHand1Text + i;

		//set it
		pStatic->SetText(VIEW->getResName(i));
		pStatic->ShowWindow(SW_HIDE);

		//get the button
		pStatic = &m_Sel1Text + i;

		//set it
		pStatic->SetText(VIEW->getResName(i));
		pStatic->ShowWindow(SW_HIDE);

		//update the buttons
		pButton = &m_InHand1Button + i;
		if(MULTIUSE_STEAL != m_iMode)
		{
			pButton->SetBitmaps(VIEW->getResImage(i, hDC), COLOR_TRANSP, VIEW->getResImage(i, hDC), COLOR_TRANSP);
		}

		pButton->ShowWindow(SW_HIDE);

		//update the buttons
		pButton = &m_Sel1Button + i;
		pButton->SetBitmaps(VIEW->getResImage(i, hDC), COLOR_TRANSP, VIEW->getResImage(i, hDC), COLOR_TRANSP);
		pButton->ShowWindow(SW_HIDE);
	}

	//set the before flag
	if(MULTIUSE_STEAL == m_iMode)
	{
		m_bBefore = !CONTROLPANEL->inTurn();
	}

	//only show the show cards button in hotseat stealing
	bShowCards = (MULTIUSE_STEAL == m_iMode && GAME_MODE_HOTSEAT == GAME->m_iMode);

	m_ShowCardsCheck.ShowWindow(bShowCards ? SW_SHOW : SW_HIDE);

	//init everything
	init();

	//release device context
	ReleaseDC(dc);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// reinitialize everything
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::init()
{
	int i;
	int iPlayer, iPlayers;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pStatic;

	//no cards selected yet
	m_iYear = 0;

	//no stolen card
	m_iStolen = 5;

	//reset the selected resources
	::ZeroMemory(m_iRes, sizeof(m_iRes));
	::ZeroMemory(m_iCardsToLose, sizeof(m_iCardsToLose));
	::ZeroMemory(m_iCardsLost, sizeof(m_iCardsLost));
	::ZeroMemory(m_iCardsSel, sizeof(m_iCardsSel));

	//if in yop mode, hide the list of players
	if(MULTIUSE_YEAR == m_iMode)
	{
		m_PlayerList.ShowWindow(SW_HIDE);
	}

	for(i = 0; i < 5; i++)
	{
		//clear the upper level
		pButton = &m_Sel1Button + i;
		pButton->ShowWindow(SW_HIDE);

		pStatic = &m_Sel1Text + i;
		pStatic->ShowWindow(SW_HIDE);

		pStatic = &m_Sel1Qty + i;
		pStatic->ShowWindow(SW_HIDE);

		//get the button and enable it
		pButton = &m_InHand1Button + i;
		pButton->EnableWindow();

		//get the static and enable it
		pStatic = &m_InHand1Text + i;
		pStatic->EnableWindow();

		//also show them if it's year of plenty
		if(m_iMode == MULTIUSE_YEAR)
		{
			//make sure we have some of this resource
			if(GAME->getBankRes(i))
			{
				pButton->ShowWindow(SW_SHOW);
				pStatic->ShowWindow(SW_SHOW);
			}
		}
	}

	//set up player text
	iPlayers = GAME->m_players.size();

	//get the current player
	iPlayer = VIEW->getPlayer();

	//add players if needed
	if(m_iMode == MULTIUSE_STEAL || m_iMode == MULTIUSE_LOSE || m_iMode == MULTIUSE_GOLD)
	{
		//set up the static buttons
		for(i = 0; i < iPlayers; i++)
		{
			//see if these people are losing/stealing/golding
			if(0 < m_iPlayers[i])
			{
				m_PlayerList.addPlayer(&(GAME->m_players[i].m_player), GAME->m_players[i].m_iColor);
			}
		}

		//if there's only one, set the selection
		if(1 == m_PlayerList.GetItemCount())
		{
			m_PlayerList.setSelection(0);
		}
	}

	//set things depending on mode
	switch(m_iMode)
	{
	case MULTIUSE_YEAR:
		SetWindowText("Select two cards from the bank");
		m_WhoStatic.SetWindowText("Available bank resources");
		m_SelStatic.SetWindowText("Selected resources");
		m_PlayerStatic.ShowWindow(SW_HIDE);
		m_DoneButton.EnableWindow(FALSE);
		break;
	case MULTIUSE_STEAL:
		//update the player
		updatePlayer();

		SetWindowText("Steal a card from a player");
		m_SelStatic.SetWindowText("Stolen card");
		m_PlayerStatic.SetWindowText("Players to steal from");
		m_DoneButton.SetWindowText("Steal Nothing");

		//can chose not to steal
		m_DoneButton.EnableWindow(TRUE);
		break;
	case MULTIUSE_LOSE:
		SetWindowText("Lose cards because of a 7");
		m_PlayerStatic.SetWindowText("Players losing cards");
		m_DoneButton.EnableWindow(FALSE);

		//set up the amounts of cards they need to lose
		for(i = 0; i < iPlayers; i++)
		{
			//use variant if needed
			if(IS_VARIANT(VARIANT_ROUND_LOST_UP))
			{
				m_iCardsToLose[i] = ((GAME->m_players[i].totalCards() + 1) >> 1);
			}
			else
			{
				m_iCardsToLose[i] = (GAME->m_players[i].totalCards() >> 1);
			}
		}

		//update the player
		updatePlayer();
		break;
	case MULTIUSE_GOLD:
		SetWindowText("Choose resources to acquire from gold");
		m_PlayerStatic.SetWindowText("Players getting resources");
		m_SelStatic.SetWindowText("Selected resources");
		m_DoneButton.EnableWindow(FALSE);

		//set up the amounts of cards they gain
		for(i = 0; i < iPlayers; i++)
		{
			m_iCardsToLose[i] = m_iPlayers[i];
		}

		//update the first player
		updatePlayer();
		break;
	default:
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle clicks
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::HandleInHandClick(UINT nID)
{
	int i;
	int iRes;
	int iSaveRes;
	int iSelection;
	int iResLost;
	int iCards;
	BOOL bDisable = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get the resource type
	iRes = (nID - IDC_INHAND1_BUTTON);
	iSaveRes = iRes;

	//get current selection
	iSelection = m_PlayerList.getSelection();

	//sanity
	if( (MULTIUSE_STEAL == m_iMode || MULTIUSE_LOSE == m_iMode || MULTIUSE_GOLD == m_iMode) &&
		(-1 == iSelection))
	{
		return;
	}

	//see if we need to use this game res or pick a random one
	if(MULTIUSE_STEAL == m_iMode && ((GAME_MODE_NETWORK == GAME->m_iMode) ||
		(GAME_MODE_HOTSEAT == GAME->m_iMode && FALSE == m_ShowCardsCheck.GetCheck())) )
	{
		iRes = VIEW->getRandomCard(iSelection);
		m_iRes[iRes]++;
	}
	else
	{
		m_iRes[iRes]++;
	}

	//get the buttons
	pButton = &m_Sel1Button + iRes;
	pText = &m_Sel1Text + iRes;
	pQty = &m_Sel1Qty + iRes;

	//calculate what they've got
	if(MULTIUSE_GOLD == m_iMode)
	{
		iCards = m_iRes[iRes] + GAME->m_players[iSelection].m_iRes[iRes];

		//it should be enabled since we're adding a card to it
		pButton->EnableWindow(TRUE);
		pText->EnableWindow(TRUE);
		pQty->EnableWindow(TRUE);
	}
	else
	{
		iCards = m_iRes[iRes];
	}

	//show windows if needed
	if(1 == iCards)
	{
		pButton->ShowWindow(SW_SHOW);
		pText->ShowWindow(SW_SHOW);
		pQty->ShowWindow(SW_SHOW);
	}

	//set the quantity
	str.Format("%d", iCards);
	pQty->SetText(str);

	//keep track
	switch(m_iMode)
	{
	case MULTIUSE_YEAR:
		//increment
		m_iYear++;

		//if it's two, disable everything
		if(m_iYear >= 2)
		{
			//disable controls
			bDisable = TRUE;

			//enable the done button
			m_DoneButton.EnableWindow();
		}

		//see if the card should disappear from below
		if(GAME->getBankRes(iRes) - m_iRes[iRes] == 0)
		{
			pButton = &m_InHand1Button + iRes;
			pText = &m_InHand1Text + iRes;

			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
		}
		break;
	case MULTIUSE_STEAL:
		//set the resource
		m_iStolen = iRes;

		//disable all
		bDisable = TRUE;

		//set the text
		m_DoneButton.SetWindowText("Steal Card");

		//if network mode and stealing, make it look like the other player's card
		//disappeared
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			pButton = &m_InHand1Button + iSaveRes;
			pText = &m_InHand1Text + iSaveRes;
			
			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
		}

		break;
	case MULTIUSE_LOSE:
		//increment the amount they're losing
		m_iCardsLost[iSelection][iRes]++;

		//increment the total they've lost
		m_iCardsSel[iSelection]++;

		//calculate amounts lost
		iResLost = GAME->m_players[iSelection].m_iRes[iRes] - m_iCardsLost[iSelection][iRes];

		//adjust amounts
		pQty = &m_InHand1Qty + iRes;
		str.Format("%d", iResLost);
		pQty->SetText(str);

		//see if the card should disappear from below
		if(0 == iResLost)
		{
			pButton = &m_InHand1Button + iRes;
			pText = &m_InHand1Text + iRes;

			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}

		//set text
		str.Format("Cards lost: %d", calcCardsLost(iSelection));
		m_CardsLostStatic.SetText(str);

		//see if they have reached their limit
		if(m_iCardsSel[iSelection] == m_iCardsToLose[iSelection])
		{
			bDisable = TRUE;
		}

		//check the ok button
		enableOK();
		break;
	case MULTIUSE_GOLD:
		//increment the amount they're gaining
		m_iCardsLost[iSelection][iRes]++;

		//increment the total they've chosen
		m_iCardsSel[iSelection]++;

		//calculate amounts gained
		iResLost = GAME->m_players[iSelection].m_iRes[iRes] - m_iCardsLost[iSelection][iRes];

		//see if the card should disappear from below
		if(GAME->getBankRes(iRes) - m_iCardsLost[iSelection][iRes] == 0)
		{
			pButton = &m_InHand1Button + iRes;
			pText = &m_InHand1Text + iRes;

			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
		}

		//set text
		str.Format("Cards gained: %d", calcCardsLost(iSelection));
		m_CardsLostStatic.SetText(str);

		//see if they have reached their limit
		if(m_iCardsSel[iSelection] == m_iCardsToLose[iSelection])
		{
			bDisable = TRUE;
		}

		//check the ok button
		enableOK();
		break;
	default:
		break;
	}

	if(TRUE == bDisable)
	{
		for(i = 0; i < 5; i++)
		{
			pButton = &m_InHand1Button + i;
			pButton->EnableWindow(FALSE);
			pText = &m_InHand1Text + i;
			pText->EnableWindow(FALSE);
			pQty = &m_InHand1Qty + i;
			pQty->EnableWindow(FALSE);
		}

		//if we're in multiplayer mode, disable the top selections, too
		//since they only get once chance to pick
		if(m_iMode == MULTIUSE_STEAL)
		{
			for(i = 0; i < 6; i++)
			{
				if(i < 5)
				{
					pButton = &m_Sel1Button + i;
					pButton->EnableWindow(FALSE);
					pText = &m_Sel1Qty + i;
					pText->EnableWindow(FALSE);
					pText = &m_Sel1Text + i;
					pText->EnableWindow(FALSE);
				}
			}

			//disable the list control so they can't pick again
			m_PlayerList.EnableWindow(FALSE);

			//disable the show goods check
			m_ShowCardsCheck.EnableWindow(FALSE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle clicks
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::HandleSelClick(UINT nID)
{
	int i;
	int iRes;
	int iCards;
	int iResLost;
	int iSelection;
	BOOL bEnable = FALSE;
	CString str;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//get current selection
	iSelection = m_PlayerList.getSelection();

	//sanity
	if( (MULTIUSE_STEAL == m_iMode || MULTIUSE_LOSE == m_iMode || MULTIUSE_GOLD == m_iMode) && 
		(-1 == iSelection))
	{
		return;
	}

	//get the resource type
	iRes = (nID - IDC_SEL1_BUTTON);

	//move it down
	m_iRes[iRes]--;

	//get the buttons
	pButton = &m_Sel1Button + iRes;
	pText = &m_Sel1Text + iRes;
	pQty = &m_Sel1Qty + iRes;

	//calculate what they've got
	if(MULTIUSE_GOLD == m_iMode)
	{
		iCards = m_iRes[iRes] + GAME->m_players[iSelection].m_iRes[iRes];

		//see if it should be disabled
		if(0 == m_iRes[iRes])
		{
			pButton->EnableWindow(FALSE);
			pText->EnableWindow(FALSE);
			pQty->EnableWindow(FALSE);
		}
	}
	else
	{
		iCards = m_iRes[iRes];
	}

	//hide the button if it's empty
	if(0 == iCards)
	{
		pButton->ShowWindow(SW_HIDE);
		pText->ShowWindow(SW_HIDE);
		pQty->ShowWindow(SW_HIDE);
	}

	//set the quantity
	str.Format("%d", iCards);
	pQty->SetText(str);

	//keep track
	switch(m_iMode)
	{
	case MULTIUSE_YEAR:
		//see if the card should disappear from above
		if(GAME->getBankRes(iRes) - m_iRes[iRes] == 1)
		{
			pButton = &m_Sel1Button + iRes;
			pText = &m_Sel1Text + iRes;
			pQty = &m_Sel1Qty + iRes;

			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}

		//increment
		m_iYear--;

		//if it's one, enable everything
		if(m_iYear == 1)
		{
			bEnable = TRUE;

			//disable the done button
			m_DoneButton.EnableWindow(FALSE);
		}
		break;
	case MULTIUSE_STEAL:
		//reset
		m_iStolen = 5;

		//set the text
		m_DoneButton.SetWindowText("Steal Nothing");

		//enable again
		bEnable = TRUE;
		break;
	case MULTIUSE_LOSE:
		//decrement the amount they're losing
		m_iCardsLost[iSelection][iRes]--;

		//decrement the total they've lost
		m_iCardsSel[iSelection]--;

		//calculate resources lost
		iResLost = GAME->m_players[iSelection].m_iRes[iRes] - m_iCardsLost[iSelection][iRes];

		//see if the card should disappear from above
		if(m_iCardsLost[iSelection][iRes] == 0)
		{
			pButton = &m_Sel1Button + iRes;
			pText = &m_Sel1Text + iRes;
			pQty = &m_Sel1Qty + iRes;

			//hide them
			pButton->ShowWindow(SW_HIDE);
			pText->ShowWindow(SW_HIDE);
			pQty->ShowWindow(SW_HIDE);
		}

		//set quantity
		str.Format("%d", iResLost);
		pQty = &m_InHand1Qty + iRes;
		pQty->SetText(str);

		//see if it needs to reappear below
		if(0 < iResLost)
		{
			pButton = &m_InHand1Button + iRes;
			pText = &m_InHand1Text + iRes;

			//hide them
			pButton->ShowWindow(SW_SHOW);
			pText->ShowWindow(SW_SHOW);
			pQty->ShowWindow(SW_SHOW);
		}

		//set text
		str.Format("Cards lost: %d", calcCardsLost(iSelection));
		m_CardsLostStatic.SetText(str);
		
		//they are under the limit
		bEnable = TRUE;

		//disable the ok button
		enableOK();
		break;
	case MULTIUSE_GOLD:
		//decrement the amount they're gaining
		m_iCardsLost[iSelection][iRes]--;

		//decrement the total they've gained
		m_iCardsSel[iSelection]--;

		//set text
		str.Format("Cards gained: %d", calcCardsLost(iSelection));
		m_CardsLostStatic.SetText(str);
		
		//they are under the limit
		bEnable = TRUE;

		//disable the ok button
		enableOK();
		break;
	default:
		break;
	}

	if(TRUE == bEnable)
	{
		for(i = 0; i < 5; i++)
		{
			pButton = &m_InHand1Button + i;
			pButton->EnableWindow(TRUE);
			pText = &m_InHand1Text + i;
			pText->EnableWindow(TRUE);
			pQty = &m_InHand1Qty + i;
			pQty->EnableWindow(TRUE);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the other players cards
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::updatePlayer()
{
	int i;
	int j;
	int iCurrent = 0;
	int iPlayer;
	int iCards;
	int iResLeft;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;
	CString str;
	CDC *dc;
	HDC hDC;

	//hide all buttons to start
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_InHand1Button + i;
		pText = &m_InHand1Text + i;
		pQty = &m_InHand1Qty + i;

		//start off hiding the buttons
		pButton->ShowWindow(SW_HIDE);
		pText->ShowWindow(SW_HIDE);
		pQty->ShowWindow(SW_HIDE);

		//redo for selections
		pButton = &m_Sel1Button + i;
		pText = &m_Sel1Text + i;
		pQty = &m_Sel1Qty + i;

		//hide all
		pButton->ShowWindow(SW_HIDE);
		pText->ShowWindow(SW_HIDE);
		pQty->ShowWindow(SW_HIDE);
	}

	//get the current selection
	iPlayer = m_PlayerList.getSelection();

	//reset text
	if(-1 == iPlayer)
	{
		m_WhoStatic.SetWindowText("");
		m_SelStatic.SetWindowText("");
		m_CardsLostStatic.SetText("");
		m_CardsToLoseStatic.SetText("");
		return;
	}

	//set the static text
	if(m_iMode == MULTIUSE_LOSE || m_iMode == MULTIUSE_STEAL)
	{
		str.Format("Cards for %s", GAME->m_players[iPlayer].m_player.getName());
		m_WhoStatic.SetWindowText(str);
	}

	//set the top text too for losing
	if(m_iMode == MULTIUSE_LOSE)
	{
		str.Format("Cards lost for %s", GAME->m_players[iPlayer].m_player.getName());
		m_SelStatic.SetWindowText(str);
	}
	else if(m_iMode == MULTIUSE_GOLD)
	{
		str.Format("Cards in hand for %s", GAME->m_players[iPlayer].m_player.getName());
		m_SelStatic.SetWindowText(str);
	}

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//go through and set up this players cards
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_InHand1Button + i;
		pText = &m_InHand1Text + i;
		pQty = &m_InHand1Qty + i;

		//get resources remaining
		if(MULTIUSE_GOLD != m_iMode)
		{
			iResLeft = GAME->m_players[iPlayer].m_iRes[i] - m_iCardsLost[iPlayer][i];
		}
		else
		{
			iResLeft = GAME->getBankRes(i);
		}

		//add cards if they have them
		//(works in steal mode because cards lost array is all zeros)
		if(0 < iResLeft)
		{
			if(!m_bShowPlayer && m_iMode == MULTIUSE_STEAL)
			{
				//get the number of cards
				iCards = GAME->m_players[iPlayer].m_iRes[i];

				for(j = 0; iCurrent < 5 && j < iCards; j++)
				{
					pButton = &m_InHand1Button + iCurrent;
					pButton->SetBitmaps(VIEW->getResImage(IMG_RESCARD, hDC), COLOR_TRANSP, VIEW->getResImage(IMG_RESCARD, hDC), COLOR_TRANSP);
					pButton->ShowWindow(SW_SHOW);
					pButton->EnableWindow();
					iCurrent++;
				}
			}
			else
			{
				//set the bitmaps properly
				pButton->SetBitmaps(VIEW->getResImage(i, hDC), COLOR_TRANSP, VIEW->getResImage(i, hDC), COLOR_TRANSP);

				//set the button
				pButton->ShowWindow(SW_SHOW);
				pButton->EnableWindow();

				//set the text
				pText->ShowWindow(SW_SHOW);
				pText->EnableWindow();

				//set text
				if(MULTIUSE_LOSE == m_iMode)
				{
					str.Format("%d", iResLeft);
					pQty->SetText(str);
					pQty->ShowWindow(SW_SHOW);
				}
			}
		}
		
		//set the cards above
		if(m_iMode == MULTIUSE_LOSE || m_iMode == MULTIUSE_GOLD)
		{
			//see if they've already chosen all the cards to lose
			pButton->EnableWindow(m_iCardsSel[iPlayer] < m_iCardsToLose[iPlayer]);
			pText->EnableWindow(m_iCardsSel[iPlayer] < m_iCardsToLose[iPlayer]);
			pQty->EnableWindow(m_iCardsSel[iPlayer] < m_iCardsToLose[iPlayer]);

			//also show any cards they've decided to lose so far
			pButton = &m_Sel1Button + i;
			pText = &m_Sel1Text + i;
			pQty = &m_Sel1Qty + i;

			if(MULTIUSE_LOSE == m_iMode)
			{
				//see if they've added cards to the top
				pButton->ShowWindow(m_iCardsLost[iPlayer][i] ? SW_SHOW : SW_HIDE);
				pText->ShowWindow(m_iCardsLost[iPlayer][i] ? SW_SHOW : SW_HIDE);
			}
			//for gold, we want to show what they currently have in hand, so we
			//show their hand + what they've gained
			else
			{
				iCards = m_iCardsLost[iPlayer][i] + GAME->m_players[iPlayer].m_iRes[i];

				pButton->ShowWindow(iCards ? SW_SHOW : SW_HIDE);
				pText->ShowWindow(iCards ? SW_SHOW : SW_HIDE);

				//they need to be disabled if they haven't actually added any
				//cards from gold yet
				pButton->EnableWindow(0 < m_iCardsLost[iPlayer][i]);
				pText->EnableWindow(0 < m_iCardsLost[iPlayer][i]);
			}

			//copy the array in to the current amount totals
			m_iRes[i] = m_iCardsLost[iPlayer][i];

			if(MULTIUSE_LOSE == m_iMode)
			{
				pQty->ShowWindow(m_iCardsLost[iPlayer][i] ? SW_SHOW : SW_HIDE);
				
				//set the qty text
				str.Format("%d", m_iCardsLost[iPlayer][i]);
				pQty->SetText(str);
			}
			else
			{
				//set the quantity
				iCards = m_iCardsLost[iPlayer][i] + GAME->m_players[iPlayer].m_iRes[i];

				str.Format("%d", iCards);
				pQty->SetText(str);

				pQty->ShowWindow(iCards ? SW_SHOW : SW_HIDE);

				//it needs to be disabled if they haven't actually added any
				//cards from gold yet
				pQty->EnableWindow(0 < m_iCardsLost[iPlayer][i]);
			}
		}
	}

	//set cards to lose quantity
	if(MULTIUSE_LOSE == m_iMode)
	{
		//set the cards to lose quantity
		str.Format("Cards to lose: %d", m_iCardsToLose[iPlayer]);
		m_CardsToLoseStatic.SetText(str);

		//set cards already lost quantity
		str.Format("Cards lost: %d", calcCardsLost(iPlayer));
		m_CardsLostStatic.SetText(str);
	}
	else if(MULTIUSE_GOLD == m_iMode)
	{
		//set the cards to lose quantity
		str.Format("Cards to gain: %d", m_iCardsToLose[iPlayer]);
		m_CardsToLoseStatic.SetText(str);

		//set cards already lost quantity
		str.Format("Cards gained: %d", calcCardsLost(iPlayer));
		m_CardsLostStatic.SetText(str);
	}

	//release device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// calculate cards lost for this player
/////////////////////////////////////////////////////////////////////////////
int CMultiUseDialog::calcCardsLost(int iPlayer)
{
	int i;
	int iTotal = 0;

	//go through resources
	for(i = 0; i < 5; i++)
	{
		iTotal += m_iCardsLost[iPlayer][i];
	}

	return iTotal;
}

/////////////////////////////////////////////////////////////////////////////
// clear the other player's trade data
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::clearSteal()
{
	int i;
	CButtonST *pButton;
	CHoverStatic *pText;
	CHoverStatic *pQty;

	//reset the steal card
	m_iStolen = 5;

	//clear the steal buttons
	for(i = 0; i < 5; i++)
	{
		//get the controls
		pButton = &m_Sel1Button + i;
		pText = &m_Sel1Text + i;
		pQty = &m_Sel1Qty + i;

		//clear
		pButton->ShowWindow(SW_HIDE);
		pText->ShowWindow(SW_HIDE);
		pQty->SetText("");

		//get the contorl
		pButton = &m_InHand1Button + i;

		//enable the window
		pButton->EnableWindow();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::OnOK() 
{
	int iPlayer;
	int iPlayers;
	int iAdjust[5];
	int iSelection;

	//get the current player
	iPlayer = VIEW->getPlayer();

	//get the number of players
	iPlayers = GAME->m_players.size();

	//get current selection
	iSelection = m_PlayerList.getSelection();

	//sanity
	if( (MULTIUSE_STEAL == m_iMode || MULTIUSE_LOSE == m_iMode || MULTIUSE_GOLD == m_iMode) && 
		(-1 == iSelection))
	{
		return;
	}

	//clear memory
	::ZeroMemory(iAdjust, sizeof(iAdjust));

	switch(m_iMode)
	{
	case MULTIUSE_YEAR:	
		//handled by PlayCardDialog.cpp
		break;
	case MULTIUSE_STEAL:

		//handle stealing
		VIEW->handleCommonSteal(iSelection, m_iStolen, m_bPirate);

		//if network, tell others
		if(GAME_MODE_NETWORK == GAME->m_iMode)
		{
			VIEW->clientSteal(iSelection, m_iStolen, m_bPirate);
		}

		//make sure the dev cards are updated
		CONTROLPANEL->EnableDC();
		break;
	case MULTIUSE_LOSE:
		//set the array of cards lost
		VIEW->setLostCards((int *) m_iCardsLost);
		break;
	case MULTIUSE_GOLD:
		//set the array of cards gained
		VIEW->setGainedCards((int *) m_iCardsLost);
		break;
	default:
		break;
	}

	if((m_iMode == MULTIUSE_YEAR) || ((m_iMode == MULTIUSE_STEAL) && !m_bBefore))
	{
		//we need to update what they can build
		VIEW->showBuildOptions(TRUE);
	}

	//enable the trade button
	CONTROLPANEL->EnableTrade();
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle cancelling
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::OnCancel() 
{
	BOOL bQuit;

	//see if we should be allowed to cancel
	bQuit = ((m_iMode == MULTIUSE_YEAR) && (TRUE == VIEW->isWaiting()));

	//not allowed in network mode
	if((GAME_MODE_NETWORK == GAME->m_iMode) && (FALSE == bQuit))
	{
		return;
	}

	//normal shutdown
	CHelpDialog::OnCancel();

	//if someone dropped, don't do anything else
	if(TRUE == VIEW->isWaiting())
	{
		return;
	}

	//switch depending on mode
	if(m_iMode == MULTIUSE_YEAR)
	{
		VIEW->showBuildOptions(TRUE);
	}
	else if(m_iMode == MULTIUSE_LOSE)
	{
		//need to show the rolls again
		CONTROLPANEL->showRolls(TRUE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle enableing the ok button
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::enableOK()
{
	int i;
	int iPlayers;
	BOOL bDone = TRUE;

	//get the number of players
	iPlayers = GAME->m_players.size();

	//search through the players to see if everyone's done
	for(i = 0; i < iPlayers; i++)
	{
		//see if this player needed to lose cards
		if(!m_iPlayers[i]) continue;

		//check for completeness
		if(m_iCardsSel[i] < m_iCardsToLose[i])
		{
			bDone = FALSE;
			break;
		}
	}
	
	//enable the OK button
	m_DoneButton.EnableWindow(bDone);
}

/////////////////////////////////////////////////////////////////////////////
// show/hide steal mode player
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::OnShowcardsCheck() 
{
	m_bShowPlayer = m_ShowCardsCheck.GetCheck();

	//update the player buttons
	updatePlayer();
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on a player
/////////////////////////////////////////////////////////////////////////////
void CMultiUseDialog::handleClick()
{
	//update the cards
	updatePlayer();

	//clear any current trade
	if(m_iMode == MULTIUSE_STEAL)
	{
		clearSteal();
	}
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CMultiUseDialog *) dwCookie)->handleClick();
	}
}