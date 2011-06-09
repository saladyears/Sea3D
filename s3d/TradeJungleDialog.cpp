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
#include "TradeJungleDialog.h"
#include "SettlersView.h"
#include "defineColors.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTradeJungleDialog dialog


CTradeJungleDialog::CTradeJungleDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CTradeJungleDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTradeJungleDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//reset
	m_nPlayer = 0;
	m_nJungles = 0;
	m_nNeeded = 0;

	m_strHelp = HELP_GL;
}


void CTradeJungleDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTradeJungleDialog)
	DDX_Control(pDX, IDC_W2_STATIC, m_W2Static);
	DDX_Control(pDX, IDC_W1_STATIC, m_W1Static);
	DDX_Control(pDX, IDC_W_STATIC, m_WStatic);
	DDX_Control(pDX, IDC_S3_STATIC, m_S3Static);
	DDX_Control(pDX, IDC_S2_STATIC, m_S2Static);
	DDX_Control(pDX, IDC_S_STATIC, m_SStatic);
	DDX_Control(pDX, IDC_O3_STATIC, m_O3Static);
	DDX_Control(pDX, IDC_O1_STATIC, m_O1Static);
	DDX_Control(pDX, IDC_O_STATIC, m_OStatic);
	DDX_Control(pDX, IDC_W2_BUTTON, m_W2Button);
	DDX_Control(pDX, IDC_W1_BUTTON, m_W1Button);
	DDX_Control(pDX, IDC_W_BUTTON, m_WButton);
	DDX_Control(pDX, IDC_S3_BUTTON, m_S3Button);
	DDX_Control(pDX, IDC_S2_BUTTON, m_S2Button);
	DDX_Control(pDX, IDC_S_BUTTON, m_SButton);
	DDX_Control(pDX, IDC_O3_BUTTON, m_O3Button);
	DDX_Control(pDX, IDC_O1_BUTTON, m_O1Button);
	DDX_Control(pDX, IDC_O_BUTTON, m_OButton);
	DDX_Control(pDX, IDC_3_RADIO, m_3Radio);
	DDX_Control(pDX, IDC_2_RADIO, m_2Radio);
	DDX_Control(pDX, IDC_1_RADIO, m_1Radio);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTradeJungleDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CTradeJungleDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTradeJungleDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// setup
/////////////////////////////////////////////////////////////////////////////
BOOL CTradeJungleDialog::OnInitDialog() 
{
	int i;
	int nRes = 0;
	int nChoice = 0;
	BOOL bSingles;
	CDC *dc;
	CString str;
	CHoverStatic *pStatic;
	CButtonST *pButton;
	HDC hDC;

	CHelpDialog::OnInitDialog();

	//get the device context
	dc = GetDC();
	hDC = dc->GetSafeHdc();

	//set up all the statics and bitmaps
	for(i = 0; i < 9; i++)
	{
		//get the res
		switch(i / 3)
		{
		case 0: nRes = RES_WHEAT; break;
		case 1: nRes = RES_SHEEP; break;
		case 2: nRes = RES_ORE; break;
		}

		//bitmaps
		pButton = &m_W2Button + i;

		//set the bitmaps
		pButton->SetBitmaps(VIEW->getResImage(nRes, hDC), COLOR_TRANSP, VIEW->getResImage(nRes, hDC), COLOR_TRANSP);
		pButton->EnableWindow(FALSE);

		//statics
		pStatic = &m_W2Static + i;

		//set the properties
		pStatic->SetFont(Tahoma12);
		pStatic->SetCenter();

		//set the text
		pStatic->SetText(VIEW->getResName(nRes));
	}
	
	//release the device context
	ReleaseDC(dc);

	//determine if we show single cards or not
	bSingles = (1 == (m_nJungles - m_nNeeded));

	//set top text
	str.Format("Which card%s would you like to keep?", bSingles ? "" : "s");
	SetWindowText(str);

	//show or hide the single options
	showSingles(bSingles);

	//if we've got singles and 2 jungles, it means we have to hide the one card
	//choice they don't have
	if((TRUE == bSingles) && (2 == m_nJungles))
	{
		//look at the player's res count for each
		if(0 == GAME->m_players[m_nPlayer].m_iRes[RES_WHEAT])
		{
			m_WButton.ShowWindow(SW_HIDE);
			m_WStatic.ShowWindow(SW_HIDE);
			m_1Radio.ShowWindow(SW_HIDE);
			nChoice = 1;
		}
		else if(0 == GAME->m_players[m_nPlayer].m_iRes[RES_ORE])
		{
			m_OButton.ShowWindow(SW_HIDE);
			m_OStatic.ShowWindow(SW_HIDE);
			m_2Radio.ShowWindow(SW_HIDE);
		}
		else
		{
			m_SButton.ShowWindow(SW_HIDE);
			m_SStatic.ShowWindow(SW_HIDE);
			m_3Radio.ShowWindow(SW_HIDE);
		}
	}

	//initial set radio selection
	switch(nChoice)
	{
	case 0:	m_1Radio.SetCheck(TRUE); break;
	case 1: m_2Radio.SetCheck(TRUE); break;
	case 2: m_3Radio.SetCheck(TRUE); break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// they cannot cancel out of this
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::initChoices(int nPlayer, int nJungles, int nNeeded, int *pFromBank)
{
	m_nPlayer = nPlayer;
	m_nJungles = nJungles;
	m_nNeeded = nNeeded;
	m_pFromBank = pFromBank;
}

/////////////////////////////////////////////////////////////////////////////
// show or hide the single options
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::showSingles(BOOL bShow)
{
	//single stuff
	m_WStatic.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_SStatic.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_OStatic.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_WButton.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_SButton.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
	m_OButton.ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	//two card stuff
	m_W2Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_W1Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_S3Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_S2Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_O3Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_O1Static.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_W2Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_W1Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_S3Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_S2Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_O3Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
	m_O1Button.ShowWindow(!bShow ? SW_SHOW : SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// they cannot cancel out of this
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::OnCancel() 
{
}

/////////////////////////////////////////////////////////////////////////////
// set their choices
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::OnOK() 
{
	int nSel;
	BOOL bSingles;

	//get selection
	if(TRUE == m_1Radio.GetCheck())
	{
		nSel = 0;
	}
	else if(TRUE == m_2Radio.GetCheck())
	{
		nSel = 1;
	}
	else
	{
		nSel = 2;
	}

	//determine if we're in singles mode or not
	bSingles = (1 == (m_nJungles - m_nNeeded));

	//run the selection
	if(TRUE == bSingles)
	{
		handleSingles(nSel);
	}
	else
	{
		handleDoubles(nSel);
	}

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// handle singles
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::handleSingles(int nSel)
{
	//trade for the card they want to keep
	switch(nSel)
	{
	case 0: m_pFromBank[RES_WHEAT] = 1; break;
	case 1: m_pFromBank[RES_ORE] = 1; break;
	case 2: m_pFromBank[RES_SHEEP] = 1; break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle doubles
/////////////////////////////////////////////////////////////////////////////
void CTradeJungleDialog::handleDoubles(int nSel)
{
	//trade for the cards they want
	switch(nSel)
	{
	case 0: 
		m_pFromBank[RES_WHEAT] = 1;
		m_pFromBank[RES_ORE] = 1;
		break;
	case 1:
		m_pFromBank[RES_WHEAT] = 1;
		m_pFromBank[RES_SHEEP] = 1;
		break;
	case 2:
		m_pFromBank[RES_ORE] = 1;
		m_pFromBank[RES_SHEEP] = 1;
		break;
	}
}