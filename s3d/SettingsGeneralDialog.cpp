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
#include "SettingsGeneralDialog.h"
#include "defineIni.h"
#include "SettlersView.h"
#include ".\settingsgeneraldialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneralDialog dialog


CSettingsGeneralDialog::CSettingsGeneralDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CSettingsGeneralDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsGeneralDialog)
	//}}AFX_DATA_INIT
}


void CSettingsGeneralDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsGeneralDialog)
	DDX_Control(pDX, IDC_AUTOROLL_H_CHECK, m_AutorollHCheck);
	DDX_Control(pDX, IDC_WAIT_CHECK, m_WaitCheck);
	DDX_Control(pDX, IDC_TIMEOUT_EDIT, m_TimeoutEdit);
	DDX_Control(pDX, IDC_AUTOROLL_CHECK, m_AutorollCheck);
	DDX_Control(pDX, IDC_EURO_CHECK, m_EuroCheck);
	DDX_Control(pDX, IDC_FONTS_CHECK, m_FontsCheck);
	DDX_Control(pDX, IDC_AUTOTRADE_CHECK, m_AutoTradeCheck);
	DDX_Control(pDX, IDC_SOUNDS_CHECK, m_SoundsCheck);
	DDX_Control(pDX, IDC_ANIMATE_CHECK, m_AnimateCheck);
	DDX_Control(pDX, IDC_EMBED_CHECK, m_EmbedCheck);
	DDX_Control(pDX, IDC_SPLASH_CHECK, m_SplashCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsGeneralDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CSettingsGeneralDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsGeneralDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CSettingsGeneralDialog::OnInitDialog() 
{
	int iValue;
	CString str;

	CHelpDialog::OnInitDialog();

	m_strHelp = HELP_GENSET;
	
	//lookup the .ini sections
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_SOUNDS, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 1;
		WritePrivateProfileString(INI_GENERAL, INI_SOUNDS, "1", INI_FILE);
	}
	
	//set sounds check
	m_SoundsCheck.SetCheck(iValue);

	//lookup auto-trade
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_AUTOTRADE, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 1;
		WritePrivateProfileString(INI_GENERAL, INI_AUTOTRADE, "1", INI_FILE);
	}

	//set legends check
	m_AutoTradeCheck.SetCheck(iValue);

	//lookup animate
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_ANIMATE, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 1;
		WritePrivateProfileString(INI_GENERAL, INI_ANIMATE, "1", INI_FILE);
	}

	//set animate check
	m_AnimateCheck.SetCheck(iValue);

	//lookup splash
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_SPLASH, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 1;
		WritePrivateProfileString(INI_GENERAL, INI_SPLASH, "1", INI_FILE);
	}

	//set animate check
	m_SplashCheck.SetCheck(iValue);

	//get embed check
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_EMBED_CHAT, 1, INI_FILE);

	//set it
	m_EmbedCheck.SetCheck(iValue);

	//disable embed if this is not a hotseat game
	if(VIEW && (GAME_MODE_HOTSEAT == GAME->m_iMode))
	{
		m_EmbedCheck.EnableWindow(FALSE);
	}

	//lookup hotseat auto roll
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_AUTO_ROLL_H, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 0;
		WritePrivateProfileString(INI_GENERAL, INI_AUTO_ROLL_H, "0", INI_FILE);
	}

	//set autoroll check
	m_AutorollHCheck.SetCheck(iValue);

	//lookup hotseat ask for next player
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_ASK_HOTSEAT, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 1;
		WritePrivateProfileString(INI_GENERAL, INI_ASK_HOTSEAT, "1", INI_FILE);
	}

	//set autoroll check
	m_WaitCheck.SetCheck(iValue);

	//lookup auto roll
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_AUTO_ROLL, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 0;
		WritePrivateProfileString(INI_GENERAL, INI_AUTO_ROLL, "0", INI_FILE);
	}

	//set autoroll check
	m_AutorollCheck.SetCheck(iValue);

	//lookup large fonts
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_LARGE_FONTS, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 0;
		WritePrivateProfileString(INI_GENERAL, INI_LARGE_FONTS, "0", INI_FILE);
	}

	//set legends check
	m_FontsCheck.SetCheck(iValue);
	
	//enable euro check
	m_EuroCheck.EnableWindow(VIEW->checkForEuro());

	//set euro value
	iValue = GetPrivateProfileInt(INI_OPENGL, INI_EURO, 0, INI_FILE);
	m_EuroCheck.SetCheck(iValue);

	//set timeout
	iValue = GetPrivateProfileInt(INI_GENERAL, INI_TRADE_TIMEOUT, -1, INI_FILE);

	//if it's not there, create it
	if(-1 == iValue)
	{
		//default to true
		iValue = 15;
		WritePrivateProfileString(INI_GENERAL, INI_TRADE_TIMEOUT, "15", INI_FILE);
	}

	//set the value
	str.Format("%d", iValue);
	m_TimeoutEdit.SetWindowText(str);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// shutdown
/////////////////////////////////////////////////////////////////////////////
void CSettingsGeneralDialog::OnOK() 
{
	int iCheck;
	int iTimeout;
	CString str;

	//validate timeout
	m_TimeoutEdit.GetWindowText(str);
	iTimeout = atoi(str);

	//validate
	if(iTimeout < 1 || iTimeout > 30)
	{
		MessageBox("Your network trade timeout must be between 1 and 30 seconds.", "Network Timeout");
		return;
	}

	//now write it
	WritePrivateProfileString(INI_GENERAL, INI_TRADE_TIMEOUT, str, INI_FILE);

	//get sound value
	iCheck = m_SoundsCheck.GetCheck();

	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_SOUNDS, str, INI_FILE);

	//get auto-trade value
	iCheck = m_AutoTradeCheck.GetCheck();

	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_AUTOTRADE, str, INI_FILE);

	//get large fonts value
	iCheck = m_FontsCheck.GetCheck();

	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_LARGE_FONTS, str, INI_FILE);

	//get animate value
	iCheck = m_AnimateCheck.GetCheck();

	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_ANIMATE, str, INI_FILE);

	//get splash value
	iCheck = m_SplashCheck.GetCheck();

	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_SPLASH, str, INI_FILE);

	//get the embed value
	iCheck = m_EmbedCheck.GetCheck();
	
	//write it out
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_EMBED_CHAT, str, INI_FILE);

	//get value
	iCheck = m_EuroCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_OPENGL, INI_EURO, str, INI_FILE);

	//get value
	iCheck = m_AutorollHCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_AUTO_ROLL_H, str, INI_FILE);

	//get value
	iCheck = m_WaitCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_ASK_HOTSEAT, str, INI_FILE);

	//get value
	iCheck = m_AutorollCheck.GetCheck();
	
	//write it out to the .ini file
	str.Format("%d", iCheck);
	WritePrivateProfileString(INI_GENERAL, INI_AUTO_ROLL, str, INI_FILE);

	CHelpDialog::OnOK();
}

