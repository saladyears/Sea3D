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
#include "SettlersView.h"
#include "NetworkConnectorSetupDialog.h"
#include "NetworkConnector.h"
#include "define.h"
#include "defineIni.h"
#include "defineVariants.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorSetupDialog dialog


CNetworkConnectorSetupDialog::CNetworkConnectorSetupDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkConnectorSetupDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkConnectorSetupDialog)
	m_strPassword = _T("");
	m_nPrivate = FALSE;
	m_strName = _T("");
	m_strPort = _T("");
	//}}AFX_DATA_INIT

	m_strHelp = HELP_CONNECTOR_SETUP;
}


void CNetworkConnectorSetupDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkConnectorSetupDialog)
	DDX_Control(pDX, IDC_PORT_EDIT, m_PortEdit);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_NAME_EDIT, m_NameEdit);
	DDX_Control(pDX, IDC_PRIVATE_CHECK, m_PrivateCheck);
	DDX_Control(pDX, IDC_PASSWORD_EDIT, m_PasswordEdit);
	DDX_Text(pDX, IDC_PASSWORD_EDIT, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 32);
	DDX_Check(pDX, IDC_PRIVATE_CHECK, m_nPrivate);
	DDX_Text(pDX, IDC_NAME_EDIT, m_strName);
	DDX_Text(pDX, IDC_PORT_EDIT, m_strPort);
	DDV_MaxChars(pDX, m_strPort, 5);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LADDER_CHECK, m_LadderCheck);
	DDX_Control(pDX, IDC_MAX_SPIN, m_MaxSpin);
	DDX_Control(pDX, IDC_MAX_STATIC, m_MaxStatic);
	DDX_Control(pDX, IDC_CUTOFF_SPIN, m_CutoffSpin);
	DDX_Control(pDX, IDC_CUTOFF_STATIC, m_CutoffStatic);
	DDX_Control(pDX, IDC_CUTOFF_CHECK, m_CutoffCheck);
	DDX_Control(pDX, IDC_TEAM_CHECK, m_TeamCheck);
}


BEGIN_MESSAGE_MAP(CNetworkConnectorSetupDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkConnectorSetupDialog)
	ON_BN_CLICKED(IDC_PRIVATE_CHECK, OnPrivateCheck)
	ON_BN_CLICKED(IDC_ADVANCED_BUTTON, OnAdvancedButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CUTOFF_CHECK, OnBnClickedCutoffCheck)
	ON_BN_CLICKED(IDC_TEAM_CHECK, OnBnClickedTeamCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorSetupDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkConnectorSetupDialog::OnInitDialog() 
{
	int i;
	BOOL bLadder = TRUE;
	BOOL bPrivate;
	CString str, strName, strPassword;
	CString strIP;
	UDACCEL accel;

	CHelpDialog::OnInitDialog();

	//set up the spin control
	m_MaxSpin.SetRange(3, 6);

	//set the current value
	m_MaxSpin.SetPos(GAME->m_nMaxPlayers);

	//set the text
	str.Format("%d", GAME->m_nMaxPlayers);
	m_MaxStatic.SetWindowText(str);

	//set up the cutoff controls
	m_CutoffSpin.SetRange(0, 100);
	m_CutoffSpin.SetPos(GAME->m_nQuitCutoff);

	//make it so it only goes by 5's
	accel.nInc = 5;
	accel.nSec = 0;
	m_CutoffSpin.SetAccel(1, &accel);

	//set the text
	str.Format("%d", GAME->m_nQuitCutoff);
	m_CutoffStatic.SetWindowText(str);

	//set the check value and enable/disable
	m_CutoffCheck.SetCheck(GAME->m_bUseQuitCutoff);
	OnBnClickedCutoffCheck();

	//get the connector values
	CONNECTOR->infoServer(strName, bPrivate, strPassword);

	//get the full client rect before we show or hide anything
	GetWindowRect(&m_rect);

	//hide the advanced section if needed
	if(FALSE == CONNECTOR->isPortForwarding())
	{
		SetWindowPos(&wndTop, 0, 0, m_rect.Width(), m_rect.Height() - 107, SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		//set the values
		parseIPString(CONNECTOR->getIP());
		
		//port
		str.Format("%d", CONNECTOR->getPort());
		m_PortEdit.SetWindowText(str);
	}

	//see if any players are not ladder players
	for(i = 0; i < (int) GAME->m_players.size(); i++)
	{
		if(FALSE == GAME->m_players[i].m_player.getLadder())
		{
			bLadder = FALSE;
			break;
		}
	}

	//they can only play in ladder games if they are a ladder player and this is not a restart
	m_LadderCheck.EnableWindow((bLadder) && (FALSE == ((CNetworkServerDialog *) GetParent())->getReload()));

	//set the check
	m_LadderCheck.SetCheck(GAME->m_bLadder);

	//set the team check
	m_TeamCheck.SetCheck(GAME->m_bTeam);

	//set values
	m_NameEdit.SetWindowText(strName);
	
	//checkbox
	m_PrivateCheck.SetCheck(bPrivate);
	m_PasswordEdit.EnableWindow(bPrivate);

	//password
	m_PasswordEdit.SetWindowText(strPassword);
	
	//set the focus to the name control
	m_NameEdit.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::OnOK() 
{
	CString str;

	UpdateData();

	//update the max players allowed
	m_MaxStatic.GetWindowText(str);
	GAME->m_nMaxPlayers = atoi(str);

	//update quit cutoff percent
	m_CutoffStatic.GetWindowText(str);
	GAME->m_nQuitCutoff = atoi(str);

	//set the ladder value
	GAME->m_bLadder = m_LadderCheck.GetCheck();

	//set the team value
	GAME->m_bTeam = m_TeamCheck.GetCheck();

	//set some variants automatically if playing in team
	if(TRUE == GAME->m_bTeam)
	{
		VARIANTS->m_iVariants |= (VARIANT_TRADE_ZERO | VARIANT_TRADE_AFTER_BUILD);
	}

	//remove any |'s from the game name
	m_strName.Remove('|');

	if(TRUE == CONNECTOR->isPortForwarding())
	{
		//see if we need to get the IP
		if(FALSE == m_IPAddress.IsBlank())
		{
			//get the IP address
			str = getIPString();

			//now set it in the connector
			CONNECTOR->setIP(str);
		}

		//write it
		WritePrivateProfileString(INI_NETWORK, INI_PORTFOR_IP, str, INI_FILE);
		str.Empty();

		//see if we need to set the port
		if(FALSE == m_strPort.IsEmpty())
		{
			//set the value
			str = m_strPort;

			CONNECTOR->setPort(atoi(str));
		}
		else
		{
			str.Format("%d", PORT);
		}		

		//write it
		WritePrivateProfileString(INI_NETWORK, INI_PORTFOR_PORT, str, INI_FILE);
	}
	
	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// get the IP string from the control
/////////////////////////////////////////////////////////////////////////////
CString CNetworkConnectorSetupDialog::getIPString()
{
	BYTE w1, w2, w3, w4;
	CString str;

	//get the ip address
	m_IPAddress.GetAddress(w1, w2, w3, w4);

	//format the string
	str.Format("%d.%d.%d.%d", w1, w2, w3, w4);

	return str;
}

/////////////////////////////////////////////////////////////////////////////
// enable/disable the password control
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::OnPrivateCheck() 
{
	//set the value
	m_PasswordEdit.EnableWindow(m_PrivateCheck.GetCheck());
}

/////////////////////////////////////////////////////////////////////////////
// show/hide the advanced section
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::OnAdvancedButton() 
{
	BOOL bPort;

	//get current state
	bPort = CONNECTOR->isPortForwarding();
	bPort = !bPort;

	//set new state
	CONNECTOR->setPortForwarding(bPort);

	//write it out
	WritePrivateProfileString(INI_NETWORK, INI_PORTFOR, bPort ? "1" : "0", INI_FILE);

	//show it
	if(TRUE == bPort)
	{
		SetWindowPos(&wndTop, 0, 0, m_rect.Width(), m_rect.Height(), SWP_NOMOVE | SWP_NOZORDER);
	}
	//hide it
	else
	{
		SetWindowPos(&wndTop, 0, 0, m_rect.Width(), m_rect.Height() - 107, SWP_NOMOVE | SWP_NOZORDER);
	}
}

/////////////////////////////////////////////////////////////////////////////
// parse an IP string and put it in the control
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::parseIPString(CString str)
{
	int i, iStart, iDot;
	BYTE byte[4];

	//sanity checks
	if(str.GetLength() > 15)
	{
		return;
	}

	//count dots and look for non numbers
	for(i = 0, iDot = 0; i < str.GetLength(); i++)
	{
		if(str[i] == '.') 
		{
			iDot++;
			continue;
		}
		if(FALSE == IS_NUMBER(str[i]))
		{
			return;
		}
	}

	//count dots
	if(3 != iDot)
	{
		return;
	}

	//safety
	::ZeroMemory(byte, sizeof(byte));

	//set bytes
	for(i = 0, iStart = 0; i < 4; i++)
	{
		//find '.'
		iDot = str.Find('.', iStart);
		if(-1 == iDot) {iDot = iStart + 3;}

		byte[i] = (BYTE) atoi(str.Mid(iStart, iDot - iStart));
		iStart = iDot + 1;
	}
	
	//set the address
	m_IPAddress.SetAddress(byte[0], byte[1], byte[2], byte[3]);
}

/////////////////////////////////////////////////////////////////////////////
// they've checked or unchecked the cutoff value
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::OnBnClickedCutoffCheck()
{
	BOOL bCheck;
	
	//get their choice
	bCheck = m_CutoffCheck.GetCheck();

	//enable/disable based on it
	m_CutoffSpin.EnableWindow(bCheck);
	m_CutoffStatic.EnableWindow(bCheck);

	//set the game value
	GAME->m_bUseQuitCutoff = bCheck;
}

/////////////////////////////////////////////////////////////////////////////
// they've checked or unchecked the team check
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorSetupDialog::OnBnClickedTeamCheck()
{
	//enable/disable the # of players allowed
	if(TRUE == m_TeamCheck.GetCheck())
	{
		m_MaxSpin.EnableWindow(FALSE);
		m_MaxStatic.SetWindowText("4");
		m_MaxStatic.EnableWindow(FALSE);
	}
	else
	{
		m_MaxSpin.EnableWindow();
		m_MaxStatic.EnableWindow();
	}
}
