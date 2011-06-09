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
#include "NetworkPlayCardDialog.h"
#include "SettlersView.h"
#include "MonopolyCardDialog.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayCardDialog dialog


CNetworkPlayCardDialog::CNetworkPlayCardDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkPlayCardDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkPlayCardDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iCurrent = -1;
	m_bBefore = FALSE;
}


void CNetworkPlayCardDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkPlayCardDialog)
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_CARDS_LIST, m_CardsList);
	DDX_Control(pDX, IDC_IMAGE_BUTTON, m_ImageButton);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkPlayCardDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkPlayCardDialog)
	ON_LBN_SELCHANGE(IDC_CARDS_LIST, OnSelchangeCardsList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkPlayCardDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkPlayCardDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_PLAYCARD;

	//get the current player
	m_iPlayer = VIEW->getPlayer();

	//disable image window
	m_ImageButton.EnableWindow(FALSE);

	//disable OK button until selection
	m_OKButton.EnableWindow(FALSE);

	//hide build options
	VIEW->showBuildOptions(FALSE);

	//update the player
	updatePlayer();

	//set focus to the cancel button
	GetDlgItem(IDCANCEL)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// update the card list of the current player
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::updatePlayer()
{
	int i, j;
	int iIndex;
	int iHeld, iBought;
	BOOL bPlayed;
	BOOL bEnable;

	//determine if they've already played a card
	bPlayed = GAME->m_players[m_iPlayer].m_bPlayedCard;

	//clear the list
	m_CardsList.ResetContent();

	//go through the list of players cards
	for(i = 0; i < CARD_SIZE; i++)
	{
		iHeld = GAME->m_players[m_iPlayer].m_iCardHeld[i];
		iBought = GAME->m_players[m_iPlayer].m_iCardBought[i];

		//set enable flag
		bEnable = TRUE;

		//add cards
		for(j = 0; j < iHeld; j++)
		{
			iIndex = m_CardsList.AddString(VIEW->getDevName(i));

			//set data pointer
			m_CardsList.SetItemData(iIndex, (DWORD) i);

			//enable/disable
			if(CARD_VICTORY != i)
			{
				//determine non-playability
				if(TRUE == bPlayed || (TRUE == m_bBefore && CARD_SOLDIER != i))
				{
					bEnable = FALSE;
				}
			}

			//enable/disable the item
			m_CardsList.EnableItem(iIndex, bEnable);
		}
		for(j = 0; j < iBought; j++)
		{
			iIndex = m_CardsList.AddString(VIEW->getDevName(i));
			m_CardsList.EnableItem(iIndex, FALSE);

			//set data pointer
			m_CardsList.SetItemData(iIndex, (DWORD) i);
		}
	}
	
	//find the first enabled selection
	for(i = 0; i < m_CardsList.GetCount(); i++)
	{
		if(TRUE == m_CardsList.IsItemEnabled(i))
		{
			m_CardsList.SetCurSel(i);
			OnSelchangeCardsList();
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the card bitmaps
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::OnSelchangeCardsList() 
{
	CDC *dc;
	HDC hDC;
	int iIndex;
	int iCard;
	BOOL bEnable;
	
	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//get card index
	iIndex = m_CardsList.GetCurSel();

	if(-1 == iIndex)
	{
		m_ImageButton.ShowWindow(SW_HIDE);
		m_iCurrent = -1;
		return;
	}
	
	//get the card
	iCard = (int) m_CardsList.GetItemData(iIndex);

	if(m_iCurrent != iCard)
	{
		//set bitmaps
		m_ImageButton.SetBitmaps(VIEW->getCardsImage(iCard, hDC), COLOR_TRANSP, VIEW->getCardsImage(iCard, hDC), COLOR_TRANSP);

		//make sure visible
		m_ImageButton.ShowWindow(SW_SHOW);

		//set card
		m_iCurrent = iCard;
	}

	//set OK button
	bEnable = m_CardsList.IsItemEnabled(iIndex);
	m_OKButton.EnableWindow(bEnable);

	//release the device context
	ReleaseDC(dc);
}

/////////////////////////////////////////////////////////////////////////////
// handle using a card
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::OnOK() 
{
	int iCard;
	int iIndex;

	//get the current card selection
	iIndex = m_CardsList.GetCurSel();

	//sanity check
	if(-1 == iIndex)
	{
		return;
	}

	//get card value
	iCard = m_CardsList.GetItemData(iIndex);

	switch(iCard)
	{
	//play a victory point
	case CARD_VICTORY: playVictory(); break;
	case CARD_MONOPOLY: playMonopoly(); break;
	case CARD_ROAD_BUILD: playRoad(); break;
	case CARD_YEAR_PLENTY: playYear(); break;
	case CARD_SOLDIER: 
		//if we're before, hide the roll possibilities
		if(TRUE == m_bBefore)
		{
			CONTROLPANEL->showRolls(FALSE);

			//toggle actions off
			ACTIONS->ShowWindow(SW_HIDE);
		}

		VIEW->playSoldier();
		break;
	default: break;
	}
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// play a victory point
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::playVictory()
{
	VIEW->handleCommonPlayVictory(m_iPlayer);

	//if we're in network mode, tell everyone
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientPlayDevCard(CARD_VICTORY);
	}
	
	//show the build options if it's not beforehand
	if(FALSE == m_bBefore)
	{
		VIEW->showBuildOptions(TRUE);
	}

	//check for the win
	VIEW->checkForWin();
}

/////////////////////////////////////////////////////////////////////////////
// play a monopoly card
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::playMonopoly()
{
	int iType;
	int iRet;
	CMonopolyCardDialog c;

	//make app aware
	VIEW->setDialog2(&c);

	//run it
	iRet = c.DoModal();

	//reset app
	VIEW->setDialog2(NULL);

	//see if we aborted or cancelled
	if(IDCANCEL == iRet || IDABORT == iRet) 
	{
		return;
	}

	//get the type
	iType = c.getType();

	//if we're in network mode, tell everyone
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientPlayDevCard(CARD_MONOPOLY, iType);
	}

	//handle it
	VIEW->handleCommonPlayMonopoly(m_iPlayer, iType);

	//reshow the build options
	VIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// play a road-building card
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::playRoad()
{
	//handle road building
	VIEW->handleCommonPlayRoad(m_iPlayer);

	//if we're in network mode, tell everyone
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientPlayDevCard(CARD_ROAD_BUILD);
	}
	
	//called from normal turn actions
	VIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// play a year of plenty card
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::playYear()
{
	int i;
	int iRet;
	int iRes[5];
	int iCard1 = -1, iCard2 = -1;
	CMultiUseDialog dlg;

	//see what they select from the bank
	dlg.setMode(MULTIUSE_YEAR);

	//make app aware
	VIEW->setDialog(&dlg, TRUE);

	//run it
	iRet = dlg.DoModal();

	//make app aware
	VIEW->setDialog(NULL);

	if(IDCANCEL == iRet || IDABORT == iRet) 
	{
		return;
	}

	//get the resources
	memcpy(iRes, dlg.getRes(), sizeof(iRes));

	//find the cards
	for(i = 0; i < 5; i++)
	{
		if(0 < iRes[i])
		{
			if(-1 == iCard1) iCard1 = i;
			else {iCard2 = i; break;}
			iRes[i]--;
		}

		//repeat, incase they use the same card twice
		if(0 < iRes[i])
		{
			if(-1 == iCard1) iCard1 = i;
			else {iCard2 = i; break;}
			iRes[i]--;
		}
	}

	//handle year of plenty
	VIEW->handleCommonPlayYear(m_iPlayer, iCard1, iCard2);

	//tell others in network mode
	if(GAME_MODE_NETWORK == GAME->m_iMode)
	{
		VIEW->clientPlayDevCard(CARD_YEAR_PLENTY, iCard1, iCard2);
	}

	//make sure everything is shown
	VIEW->showBuildOptions(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// make sure we reshow the build options
/////////////////////////////////////////////////////////////////////////////
void CNetworkPlayCardDialog::OnCancel() 
{
	if((FALSE == m_bBefore) && (FALSE == VIEW->isWaiting()))
	{
		VIEW->showBuildOptions(TRUE);
	}
	
	CHelpDialog::OnCancel();
}
