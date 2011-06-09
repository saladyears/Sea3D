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
#include "NetworkFirewallDialog.h"
#include "defineIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkFirewallDialog dialog


CNetworkFirewallDialog::CNetworkFirewallDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkFirewallDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkFirewallDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strHelp = HELP_GL;
}


void CNetworkFirewallDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkFirewallDialog)
	DDX_Control(pDX, IDC_SHOWNEXT_CHECK, m_HideCheck);
	DDX_Control(pDX, IDC_FIREWALL_STATIC, m_FirewallStatic);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkFirewallDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkFirewallDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkFirewallDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// start up
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkFirewallDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();
	
	//set the firewall text
	m_FirewallStatic.SetWindowText("Sea3D has determined that your IP address begins with either 192.168 or 10.0.  This indicates that you are behind a firewall, which will prevent clients from connecting to your game.  Unless you have port forwarding set up on your firewall, it is recommended that you do not continue or clients will be confused by their inability to connect to you.\r\n\r\nIf you do have port forwarding set up, you can specify an alternate IP and port for clients to connect to with the Advanced button in the next dialog box.");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// shut down
/////////////////////////////////////////////////////////////////////////////
void CNetworkFirewallDialog::OnOK() 
{
	BOOL bHide;

	//get the value
	bHide = m_HideCheck.GetCheck();

	//set it
	WritePrivateProfileString(INI_NETWORK, INI_FIREWALL, bHide ? "1" : "0", INI_FILE);
	
	CHelpDialog::OnOK();
}
