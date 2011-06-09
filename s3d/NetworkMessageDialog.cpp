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
#include "NetworkMessageDialog.h"
#include "define.h"
#include "SettlersView.h"
#include "defineNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkMessageDialog dialog


CNetworkMessageDialog::CNetworkMessageDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkMessageDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkMessageDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNetworkMessageDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkMessageDialog)
	DDX_Control(pDX, IDC_CHECK1, m_Check1);
	DDX_Control(pDX, IDC_CHECK2, m_Check2);
	DDX_Control(pDX, IDC_CHECK3, m_Check3);
	DDX_Control(pDX, IDC_CHECK4, m_Check4);
	DDX_Control(pDX, IDC_CHECK5, m_Check5);
	DDX_Control(pDX, IDC_CHECK6, m_Check6);
	DDX_Control(pDX, IDC_CHECK7, m_Check7);
	DDX_Control(pDX, IDC_CHECK8, m_Check8);
	DDX_Control(pDX, IDC_CHECK9, m_Check9);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkMessageDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkMessageDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkMessageDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// set initial checkboxes
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkMessageDialog::OnInitDialog() 
{
	int i;
	CButton *pButton;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_SYSMESS;
	
	//run through the list and turn on checks
	for(i = 0; i < SYS_NOTIFY_SIZE; i++)
	{
		pButton = &m_Check1 + i;

		if(GAME->m_iSysNotify & (1 << i))
		{
			pButton->SetCheck(TRUE);
		}
		else
		{
			pButton->SetCheck(FALSE);
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// check for changes
/////////////////////////////////////////////////////////////////////////////
void CNetworkMessageDialog::OnOK() 
{
	int i;
	int iMessage;
	CButton *pButton;

	//reset the message
	iMessage = 0;

	//run through the list and turn on checks
	for(i = 0; i < SYS_NOTIFY_SIZE; i++)
	{
		pButton = &m_Check1 + i;

		if(TRUE == pButton->GetCheck())
		{
			iMessage |= (1 << i);
		}
	}
	

	//set it
	GAME->m_iSysNotify = iMessage;
	
	CHelpDialog::OnOK();
}
