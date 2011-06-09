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
#include "SettingsPlayerDisplayDialog.h"
#include "SettlersView.h"
#include "defineIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSettingsPlayerDisplayDialog dialog


CSettingsPlayerDisplayDialog::CSettingsPlayerDisplayDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CSettingsPlayerDisplayDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSettingsPlayerDisplayDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSettingsPlayerDisplayDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSettingsPlayerDisplayDialog)
	DDX_Control(pDX, IDC_BANK_STATIC, m_BankStatic);
	DDX_Control(pDX, IDC_BANK_CHECK, m_BankCheck);
	DDX_Control(pDX, IDC_NETWORK_STATIC, m_NetworkStatic);
	DDX_Control(pDX, IDC_HOTSEAT_STATIC, m_HotseatStatic);
	DDX_Control(pDX, IDC_NETWORK_CHECK, m_NetworkCheck);
	DDX_Control(pDX, IDC_HOTSEAT_CHECK, m_HotseatCheck);
	DDX_Control(pDX, IDC_AUTOHIDE_CHECK, m_AutohideCheck);
	DDX_Control(pDX, IDC_ANIMATE_CHECK, m_AnimateCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSettingsPlayerDisplayDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CSettingsPlayerDisplayDialog)
	ON_BN_CLICKED(IDC_AUTOHIDE_CHECK, OnAutohideCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSettingsPlayerDisplayDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CSettingsPlayerDisplayDialog::OnInitDialog() 
{
	BOOL bValue;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_PLAYDISP;
	
	//read values from .ini file
	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE, FALSE, INI_FILE);

	//set autohide check
	m_AutohideCheck.SetCheck(bValue);

	//enable all
	EnableAll(bValue);

	//set other checks
	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_ANIMATE, FALSE, INI_FILE);
	m_AnimateCheck.SetCheck(bValue);

	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_NET, FALSE, INI_FILE);
	m_NetworkCheck.SetCheck(bValue);

	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_OTHER, FALSE, INI_FILE);
	m_HotseatCheck.SetCheck(bValue);

	bValue = GetPrivateProfileInt(INI_DISPLAY, INI_AUTOHIDE_BANK, FALSE, INI_FILE);
	m_BankCheck.SetCheck(bValue);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable controls
/////////////////////////////////////////////////////////////////////////////
void CSettingsPlayerDisplayDialog::EnableAll(BOOL bEnable)
{
	m_AnimateCheck.EnableWindow(bEnable);
	m_NetworkStatic.EnableWindow(bEnable);
	m_NetworkCheck.EnableWindow(bEnable);
	m_HotseatStatic.EnableWindow(bEnable);
	m_HotseatCheck.EnableWindow(bEnable);
	m_BankStatic.EnableWindow(bEnable);
	m_BankCheck.EnableWindow(bEnable);
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable on a check
/////////////////////////////////////////////////////////////////////////////
void CSettingsPlayerDisplayDialog::OnAutohideCheck() 
{
	EnableAll(m_AutohideCheck.GetCheck());
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CSettingsPlayerDisplayDialog::OnOK() 
{
	int i;
	BOOL bCheck;

	//auto-hide value
	bCheck = m_AutohideCheck.GetCheck();
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE, bCheck ? "1" : "0", INI_FILE);

	//animate value
	bCheck = m_AnimateCheck.GetCheck();
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_ANIMATE, bCheck ? "1" : "0", INI_FILE);

	//network value
	bCheck = m_NetworkCheck.GetCheck();
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_NET, bCheck ? "1" : "0", INI_FILE);

	//others value
	bCheck = m_HotseatCheck.GetCheck();
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_OTHER, bCheck ? "1" : "0", INI_FILE);

	//bank value
	bCheck = m_BankCheck.GetCheck();
	WritePrivateProfileString(INI_DISPLAY, INI_AUTOHIDE_BANK, bCheck ? "1" : "0", INI_FILE);

	//update buttons
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		VIEW->updateAutohide();
	}
	
	CHelpDialog::OnOK();
}