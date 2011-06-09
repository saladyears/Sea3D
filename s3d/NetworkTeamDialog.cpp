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
#include "Settlers.h"
#include "NetworkTeamDialog.h"
#include "SettlersView.h"


// CNetworkTeamDialog dialog

IMPLEMENT_DYNAMIC(CNetworkTeamDialog, CDialog)
CNetworkTeamDialog::CNetworkTeamDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CNetworkTeamDialog::IDD, pParent)
	, m_nTeamCombo(0)
{
}

CNetworkTeamDialog::~CNetworkTeamDialog()
{
}

void CNetworkTeamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEAM_COMBO, m_TeamCombo);
	DDX_CBIndex(pDX, IDC_TEAM_COMBO, m_nTeamCombo);
}


BEGIN_MESSAGE_MAP(CNetworkTeamDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// initialization
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkTeamDialog::OnInitDialog()
{
	int i;

	CDialog::OnInitDialog();

	//add player names to the combo box
	for(i = 1; i < (int) GAME->m_players.size(); i++)
	{
		m_TeamCombo.AddString(GAME->m_players[i].m_player.getName());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// cancel is not allowed
/////////////////////////////////////////////////////////////////////////////
void CNetworkTeamDialog::OnCancel()
{
}
