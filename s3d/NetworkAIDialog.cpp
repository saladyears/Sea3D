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
#include "NetworkAIDialog.h"
#include "SettlersView.h"
#include "AIDatabase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkAIDialog dialog


CNetworkAIDialog::CNetworkAIDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkAIDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkAIDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNetworkAIDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkAIDialog)
	DDX_Control(pDX, IDC_AI1_CHECK, m_AI1Check);
	DDX_Control(pDX, IDC_AI2_CHECK, m_AI2Check);
	DDX_Control(pDX, IDC_AI3_CHECK, m_AI3Check);
	DDX_Control(pDX, IDC_AI4_CHECK, m_AI4Check);
	DDX_Control(pDX, IDC_AI5_CHECK, m_AI5Check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkAIDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkAIDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkAIDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkAIDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_HOST;
	
	//init check marks
	initChecks();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// set up stuff, turn things on and off
/////////////////////////////////////////////////////////////////////////////
void CNetworkAIDialog::initChecks()
{
	int i, j;
	CButton *pCheck;
	CString str;

	//traverse controls
	for(i = 0; i < AI_DATABASE->numAI(); i++)
	{
		//set variables
		pCheck = &m_AI1Check + i;

		//set checkbox text
		str.Format("Use %s", AI_DATABASE->getAIName(i));
		pCheck->SetWindowText(str);

		//see if it's already being used
		for(j = 0; j < (int) GAME->m_players.size(); j++)
		{
			//find it
			if(GAME->m_players[j].m_player.getID() == AI_DATABASE->getAIID(i))
			{
				pCheck->SetCheck(TRUE);
			}
			else
			{
				pCheck->SetCheck(FALSE);
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle leaving
/////////////////////////////////////////////////////////////////////////////
void CNetworkAIDialog::OnOK() 
{
	int i;
	CButton *pCheck;
	CString str;

	//reset arrays
	m_uiIDs.RemoveAll();
	m_iColors.RemoveAll();

	//traverse controls
	for(i = 0; i < AI_DATABASE->numAI(); i++)
	{
		//set variables
		pCheck = &m_AI1Check + i;

		//get value
		if(FALSE == pCheck->GetCheck())
		{
			continue;
		}

		//otherwise, add it in
		m_uiIDs.Add(AI_DATABASE->getAIID(i));
		m_iColors.Add(0);
	}
	
	CHelpDialog::OnOK();
}