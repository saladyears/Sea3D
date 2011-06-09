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
#include "VariantsDialog.h"
#include "SettlersView.h"
#include "defineVariants.h"
#include "defineIni.h"
#include "defineColors.h"
#include "defineMap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVariantsDialog dialog


CVariantsDialog::CVariantsDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CVariantsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVariantsDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVariantsDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVariantsDialog)
	DDX_Control(pDX, IDC_SPECIAL_CHECK, m_SpecialCheck);
	DDX_Control(pDX, IDC_PAREN_STATIC, m_ParenStatic);
	DDX_Control(pDX, IDC_DESERT_RADIO, m_DesertRadio);
	DDX_Control(pDX, IDC_VOLCANO_RADIO, m_VolcanoRadio);
	DDX_Control(pDX, IDC_JUNGLES_RADIO, m_JungleRadio);
	DDX_Control(pDX, IDC_GOLD_CHECK, m_GoldCheck);
	DDX_Control(pDX, IDC_SHIPS_STATIC, m_ShipsStatic);
	DDX_Control(pDX, IDC_SHIPS_EDIT, m_ShipsEdit);
	DDX_Control(pDX, IDC_HIDECHIPS_CHECK, m_HideChipsCheck);
	DDX_Control(pDX, IDC_ROUNDUP_CHECK, m_RoundUpCheck);
	DDX_Control(pDX, IDC_SHOWCARDS_CHECK, m_ShowCardsCheck);
	DDX_Control(pDX, IDC_TRADES_AFTER_CHECK, m_TradeAfterCheck);
	DDX_Control(pDX, IDC_SEVENS_CHECK, m_SevensCheck);
	DDX_Control(pDX, IDC_POINTS_EDIT, m_PointsEdit);
	DDX_Control(pDX, IDC_ALLOWED_EDIT, m_AllowedEdit);
	DDX_Control(pDX, IDC_CITIES_EDIT, m_CitiesEdit);
	DDX_Control(pDX, IDC_SETTLE_EDIT, m_SettleEdit);
	DDX_Control(pDX, IDC_ROADS_EDIT, m_RoadsEdit);
	DDX_Control(pDX, IDC_NOSEVENS_EDIT, m_NoSevensEdit);
	DDX_Control(pDX, IDC_EQUAL_ODDS_CHECK, m_EqualOddsCheck);
	DDX_Control(pDX, IDC_ALLOW_ZERO_CHECK, m_AllowZeroCheck);
	DDX_Control(pDX, IDC_ROLL_INFO_STATIC, m_RollStatic);
	DDX_Control(pDX, IDC_ALLOWED_INFO_STATIC, m_AllowedStatic);
	DDX_Control(pDX, IDC_TOURNAMENT_CHECK, m_TournamentCheck);
	DDX_Control(pDX, IDC_NOTRADE_CHECK, m_NoTradeCheck);
	DDX_Control(pDX, IDC_TRADELIMIT_CHECK, m_TradeLimitCheck);
	DDX_Control(pDX, IDC_TRADELIMIT_EDIT, m_TradeLimitEdit);
	DDX_Control(pDX, IDC_TRADELIMIT_STATIC, m_TradeLimitStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVariantsDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CVariantsDialog)
	ON_BN_CLICKED(IDC_JUNGLES_RADIO, OnJungleRadio)
	ON_BN_CLICKED(IDC_DESERT_RADIO, OnDesertRadio)
	ON_BN_CLICKED(IDC_VOLCANO_RADIO, OnVolcanoRadio)
	ON_BN_CLICKED(IDC_SPECIAL_CHECK, OnSpecialCheck)
	ON_BN_CLICKED(IDC_NOTRADE_CHECK, OnNoTradeCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVariantsDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// start up
/////////////////////////////////////////////////////////////////////////////
BOOL CVariantsDialog::OnInitDialog() 
{
	CString str;

	CHelpDialog::OnInitDialog();
	
	//help system
	m_strHelp = HELP_VARIANTS;

	//set up info text
	m_AllowedStatic.SetFont(Tahoma12);
	m_AllowedStatic.SetColors(COLOR_GRAY, COLOR_GRAY);
	m_AllowedStatic.SetText("This allows players to give cards to each other.");

	//set up info text
	m_RollStatic.SetFont(Tahoma12);
	m_RollStatic.SetColors(COLOR_GRAY, COLOR_GRAY);
	m_RollStatic.SetText("This makes all dice roll probabilities the same.");

	//set check marks
	m_AllowZeroCheck.SetCheck(IS_VARIANT(VARIANT_TRADE_ZERO));
	m_EqualOddsCheck.SetCheck(IS_VARIANT(VARIANT_EQUAL_ODDS));
	m_TradeAfterCheck.SetCheck(IS_VARIANT(VARIANT_TRADE_AFTER_BUILD));
	m_SevensCheck.SetCheck(IS_VARIANT(VARIANT_NO_7_FIRST_ROUND));
	m_ShowCardsCheck.SetCheck(IS_VARIANT(VARIANT_SHOW_ALL_CARDS));
	m_RoundUpCheck.SetCheck(IS_VARIANT(VARIANT_ROUND_LOST_UP));
	m_HideChipsCheck.SetCheck(IS_VARIANT(VARIANT_HIDE_CHIPS));
	m_SpecialCheck.SetCheck(IS_VARIANT(VARIANT_SPECIAL_BUILD));
	m_TournamentCheck.SetCheck(IS_VARIANT(VARIANT_TOURNAMENT_START));
	m_NoTradeCheck.SetCheck(IS_VARIANT(VARIANT_NO_TRADING));
	m_TradeLimitCheck.SetCheck(IS_VARIANT(VARIANT_LIMIT_TRADING));

	//enable/disable other trading
	OnNoTradeCheck();

	//set radios
	if(IS_VARIANT(VARIANT_USE_JUNGLE))
	{
		m_JungleRadio.SetCheck(TRUE);
		m_ParenStatic.EnableWindow(FALSE);
		m_GoldCheck.EnableWindow(FALSE);
	}
	else if(IS_VARIANT(VARIANT_USE_VOLCANO))
	{
		m_VolcanoRadio.SetCheck(TRUE);
	}
	else if(IS_VARIANT(VARIANT_USE_VOLCANO_GOLD))
	{
		m_VolcanoRadio.SetCheck(TRUE);
		m_GoldCheck.SetCheck(TRUE);
	}
	else
	{
		m_DesertRadio.SetCheck(TRUE);
		m_ParenStatic.EnableWindow(FALSE);
		m_GoldCheck.EnableWindow(FALSE);
	}

	//trade offer limit
	str.Format("%d", VARIANTS->m_nTradeOffers);
	m_TradeLimitEdit.SetWindowText(str);

	//no sevens box
	str.Format("%d", VARIANTS->m_nNoSevens);
	m_NoSevensEdit.SetWindowText(str);

	//set edit amounts
	str.Format("%d", VARIANTS->m_iStockRoads);
	m_RoadsEdit.SetWindowText(str);

	str.Format("%d", VARIANTS->m_iStockShips);
	m_ShipsEdit.SetWindowText(str);

	//enable/disable ships
	if(FALSE == (MAP->m_iMode & MAP_SEAFARERS))
	{
		m_ShipsEdit.EnableWindow(FALSE);
		m_ShipsStatic.EnableWindow(FALSE);
	}

	str.Format("%d", VARIANTS->m_iStockSettlements);
	m_SettleEdit.SetWindowText(str);

	str.Format("%d", VARIANTS->m_iStockCities);
	m_CitiesEdit.SetWindowText(str);

	str.Format("%d", VARIANTS->m_iAllowedCards);
	m_AllowedEdit.SetWindowText(str);

	str.Format("%d", VARIANTS->m_iPoints);
	m_PointsEdit.SetWindowText(str);

	//disable things if this is a ladder game
	if(TRUE == GAME->m_bLadder)
	{
		disableLadder();
	}

	//if hotseat, disable the trade limit offer, since it's irrelevant
	if(GAME_MODE_HOTSEAT == GAME->m_iMode)
	{
		m_TradeLimitCheck.EnableWindow(FALSE);
		m_TradeLimitEdit.EnableWindow(FALSE);
		m_TradeLimitStatic.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// handle setting variants
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnOK() 
{
	int iRoads, iShips, iSettle, iCities, iAllowed, iPoints, nSevens, nLimit;
	int nDeserts = 0;
	BOOL bCheck;
	CString str;

	//trade offer limit
	m_TradeLimitEdit.GetWindowText(str);
	nLimit = atoi(str);

	//no sevens
	m_NoSevensEdit.GetWindowText(str);
	nSevens = atoi(str);

	//validation of amounts here
	m_RoadsEdit.GetWindowText(str);
	iRoads = atoi(str);

	//ships
	m_ShipsEdit.GetWindowText(str);
	iShips = atoi(str);

	//cities
	m_CitiesEdit.GetWindowText(str);
	iCities = atoi(str);

	//settlements
	m_SettleEdit.GetWindowText(str);
	iSettle = atoi(str);

	//allowed cards
	m_AllowedEdit.GetWindowText(str);
	iAllowed = atoi(str);

	//points
	m_PointsEdit.GetWindowText(str);
	iPoints = atoi(str);

	//validate
	if((TRUE == m_TradeLimitCheck.GetCheck()) && ((1 > nLimit) || (20 < nLimit)))
	{
		MessageBox("You must limit trade offers to between 1 and 20 per round.", "Variant Error");
		return;
	}
	if((TRUE == m_SevensCheck.GetCheck()) && ((1 > nSevens) || (99 < nSevens)))
	{
		MessageBox("You must have no 7's between 1 and 99 rounds.", "Variant Error");
		return;
	}
	if(4 >= iRoads)
	{
		MessageBox("You must have at least five roads in stock.", "Variant Error");
		return;
	}
	if((MAP->m_iMode & MAP_SEAFARERS) && 4 >= iShips)
	{
		MessageBox("You must have at least five ships in stock.", "Variant Error");
		return;
	}
	if(1 >= iSettle)
	{
		MessageBox("You must have at least two settlements in stock.", "Variant Error");
		return;
	}
	if(0 >= iCities)
	{
		MessageBox("You must have at least one city in stock.", "Variant Error");
		return;
	}
	if(4 >= iAllowed)
	{
		MessageBox("You must allow players to have at least five cards.", "Variant Error");
		return;
	}
	if(2 >= iPoints)
	{
		MessageBox("You must play to at least three points.", "Variant Error");
		return;
	}

	//zero trades allowed
	bCheck = m_AllowZeroCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_TRADE_ZERO;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_TRADE_ZERO;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_TRADE_ZERO, str, INI_FILE);

	//equal odds rolls
	bCheck = m_EqualOddsCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_EQUAL_ODDS;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_EQUAL_ODDS;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_EQUAL_ODDS, str, INI_FILE);

	//trade after build
	bCheck = m_TradeAfterCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_TRADE_AFTER_BUILD;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_TRADE_AFTER_BUILD;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_TRADE_BUILD, str, INI_FILE);

	//no 7's first time
	bCheck = m_SevensCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_NO_7_FIRST_ROUND;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_NO_7_FIRST_ROUND;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_NO_7, str, INI_FILE);

	//trade limits
	bCheck = m_TradeLimitCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_LIMIT_TRADING;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_LIMIT_TRADING;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_LIMIT_TRADE, str, INI_FILE);

	//show all cards
	bCheck = m_ShowCardsCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_SHOW_ALL_CARDS;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_SHOW_ALL_CARDS;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_SHOW_CARDS, str, INI_FILE);

	//round up on a 7
	bCheck = m_RoundUpCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_ROUND_LOST_UP;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ROUND_LOST_UP;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_ROUND_UP, str, INI_FILE);

	//tournament start rules
	bCheck = m_TournamentCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_TOURNAMENT_START;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_TOURNAMENT_START;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_TOUR_START, str, INI_FILE);

	//no trading
	bCheck = m_NoTradeCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_NO_TRADING;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_NO_TRADING;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_NO_TRADE, str, INI_FILE);

	//radio buttons
	if(TRUE == m_DesertRadio.GetCheck())
	{
		VARIANTS->m_iVariants &= (~VARIANT_USE_JUNGLE & ~VARIANT_USE_VOLCANO & ~VARIANT_USE_VOLCANO_GOLD);

		//reset sea chips if needed
		if(FALSE == (MAP->m_iMode & MAP_SEAFARERS))
		{
			MAP->m_iChipsUsed &= ~MAP_USES_SEA_CHIPS;
		}
	}
	//volcanoes
	else if(TRUE == m_VolcanoRadio.GetCheck())
	{
		if(FALSE == m_GoldCheck.GetCheck())
		{
			VARIANTS->m_iVariants |= VARIANT_USE_VOLCANO;
			VARIANTS->m_iVariants &= (~VARIANT_USE_JUNGLE & ~VARIANT_USE_VOLCANO_GOLD);
			nDeserts = VARIANT_USE_VOLCANO;
		}
		else
		{
			VARIANTS->m_iVariants |= VARIANT_USE_VOLCANO_GOLD;
			VARIANTS->m_iVariants &= (~VARIANT_USE_JUNGLE & ~VARIANT_USE_VOLCANO);
			nDeserts = VARIANT_USE_VOLCANO_GOLD;
		}

		//must load sea chips as well
		MAP->m_iChipsUsed |= MAP_USES_SEA_CHIPS;
	}
	//jungles
	else if(TRUE == m_JungleRadio.GetCheck())
	{
		VARIANTS->m_iVariants |= VARIANT_USE_JUNGLE;
		VARIANTS->m_iVariants &= (~VARIANT_USE_VOLCANO & ~VARIANT_USE_VOLCANO_GOLD);
		nDeserts = VARIANT_USE_JUNGLE;

		//must load sea chips as well
		MAP->m_iChipsUsed |= MAP_USES_SEA_CHIPS;
	}
	
	//write out the value
	str.Format("%d", nDeserts);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_DESERTS, str, INI_FILE);

	//hide chips until after building
	bCheck = m_HideChipsCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_HIDE_CHIPS;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_HIDE_CHIPS;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_HIDE_CHIPS, str, INI_FILE);

	//special build phase
	bCheck = m_SpecialCheck.GetCheck();

	//set flag
	if(TRUE == bCheck)
	{
		VARIANTS->m_iVariants |= VARIANT_SPECIAL_BUILD;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_SPECIAL_BUILD;
	}

	//set the .ini file flag
	str.Format("%d", bCheck ? 1 : 0);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_SPECIAL, str, INI_FILE);

	//no sevens
	VARIANTS->m_nNoSevens = nSevens;
	str.Format("%d", nSevens);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_NO_SEVENS, str, INI_FILE);

	//trade offer limits
	VARIANTS->m_nTradeOffers = nLimit;
	str.Format("%d", nLimit);
	WritePrivateProfileString(INI_VARIANT, INI_VAR_LIMIT_NUM, str, INI_FILE);

	//roads
	if(iRoads != MAP->m_iStockRoads)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_ROADS;
		VARIANTS->m_iStockRoads = iRoads;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_ROADS;
	}

	//ships
	if(iShips != MAP->m_iStockShips)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_SHIPS;
		VARIANTS->m_iStockShips = iShips;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_SHIPS;
	}

	//settlements
	if(iSettle != MAP->m_iStockSettlements)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_SETTLEMENTS;
		VARIANTS->m_iStockSettlements = iSettle;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_SETTLEMENTS;
	}

	//cities
	if(iCities != MAP->m_iStockCities)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_CITIES;
		VARIANTS->m_iStockCities = iCities;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_CITIES;
	}

	//allowed cards
	if(iAllowed != MAP->m_iAllowedCards)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_ALLOWED_CARDS;
		VARIANTS->m_iAllowedCards = iAllowed;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_ALLOWED_CARDS;
	}
	
	//points
	if(iPoints != MAP->m_iPoints)
	{
		VARIANTS->m_iVariants |= VARIANT_ALTER_POINTS;
		VARIANTS->m_iPoints = iPoints;
	}
	else
	{
		VARIANTS->m_iVariants &= ~VARIANT_ALTER_POINTS;
	}

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnDesertRadio() 
{
	m_ParenStatic.EnableWindow(FALSE);
	m_GoldCheck.EnableWindow(FALSE);
	m_GoldCheck.SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnVolcanoRadio() 
{
	m_ParenStatic.EnableWindow();
	m_GoldCheck.EnableWindow();
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnJungleRadio() 
{
	m_ParenStatic.EnableWindow(FALSE);
	m_GoldCheck.EnableWindow(FALSE);
	m_GoldCheck.SetCheck(FALSE);
}

/////////////////////////////////////////////////////////////////////////////
// special build phase
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnSpecialCheck() 
{
	//see if it needs to go to change
	if(9 == MAP->m_iAllowedCards)
	{
		//set the allowed cards to 7 if they are normally 9 (or vice versa)
		if(TRUE == m_SpecialCheck.GetCheck())
		{
			m_AllowedEdit.SetWindowText("7");
		}
		else
		{
			m_AllowedEdit.SetWindowText("9");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// special build phase
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::OnNoTradeCheck()
{
	BOOL bEnable;

	bEnable = (FALSE == m_NoTradeCheck.GetCheck());

	//turn on/off other trade possibilities
	m_AllowZeroCheck.EnableWindow(bEnable);
	m_TradeAfterCheck.EnableWindow(bEnable);

	//if false, remove checks
	if(FALSE == bEnable)
	{
		m_AllowZeroCheck.SetCheck(FALSE);
		m_TradeAfterCheck.SetCheck(FALSE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// disallow ladder options if in a ladder game
/////////////////////////////////////////////////////////////////////////////
void CVariantsDialog::disableLadder()
{
	//the only things allowed are trade after build, no 7 first round, round lost cards up, 
	//use jungles & volcanoes, and special build phases

	//disable disallowed trading
	m_NoTradeCheck.EnableWindow(FALSE);
	m_TradeAfterCheck.EnableWindow();

	//disable trades for zero
	m_AllowZeroCheck.EnableWindow(FALSE);

	//disable show cards at all times
	m_ShowCardsCheck.EnableWindow(FALSE);

	//disable equal odds
	m_EqualOddsCheck.EnableWindow(FALSE);

	//disable stock amounts
	m_RoadsEdit.EnableWindow(FALSE);
	m_ShipsEdit.EnableWindow(FALSE);
	m_SettleEdit.EnableWindow(FALSE);
	m_CitiesEdit.EnableWindow(FALSE);

	//disable points to win
	m_PointsEdit.EnableWindow(FALSE);

	//disable allowed cards
	m_AllowedEdit.EnableWindow(FALSE);

	//disable show chips
	m_HideChipsCheck.EnableWindow(FALSE);

	//no sevens amount must be one
	m_NoSevensEdit.SetWindowText("1");
	m_NoSevensEdit.EnableWindow(FALSE);
}
