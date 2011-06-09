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
#include "ConnectorDownloadDlg.h"
#include "NetworkConnector.h"
#include "ConnectorPasswordDlg.h"
#include "PlayerDatabase.h"

// CConnectorDownloadDlg dialog

IMPLEMENT_DYNAMIC(CConnectorDownloadDlg, CDialog)

CConnectorDownloadDlg::CConnectorDownloadDlg(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CConnectorDownloadDlg::IDD, pParent)
{
	m_strHelp = HELP_CONNECTOR_DOWNLOAD;
}

CConnectorDownloadDlg::~CConnectorDownloadDlg()
{
}

void CConnectorDownloadDlg::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HANDLE_EDIT, m_HandleEdit);
}


BEGIN_MESSAGE_MAP(CConnectorDownloadDlg, CHelpDialog)
	ON_BN_CLICKED(IDC_DOWNLOAD_BUTTON, OnBnClickedDownloadButton)
END_MESSAGE_MAP()


// CConnectorDownloadDlg message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CConnectorDownloadDlg::OnInitDialog() 
{
	CString strName;

	//start up
	CHelpDialog::OnInitDialog();

	//set the player information
	strName = m_player.getName();
	strName.Remove(' ');

	//set it in the handle box
	if(FALSE == strName.IsEmpty())
	{
		m_HandleEdit.SetWindowText(strName);
	}

	//set the focus
	m_HandleEdit.SetFocus();

	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// handle a click of the download button
/////////////////////////////////////////////////////////////////////////////
void CConnectorDownloadDlg::OnBnClickedDownloadButton()
{
	int i;
	int nResult;
	CString strPlayer;
	CString strPassword, strID;
	CConnectorPasswordDlg dlg;

	//get the player to look for
	m_HandleEdit.GetWindowText(strPlayer);

	//sanity
	if(TRUE == strPlayer.IsEmpty())
	{
		MessageBox("You must enter a handle to download.", "Sea3D Connector");
		return;
	}

	//get the player info from s3dconnector
	nResult = CONNECTOR->downloadPlayer(strPlayer, strPassword, strID);

	//check for ban
	if(-1 == nResult)
	{
		MessageBox("This player has been banned from ladder games.", "Sea3D Connector");
		EndDialog(IDCANCEL);
		return;
	}
	else if(FALSE == nResult)
	{
		MessageBox("Sea3D could not locate this player in the s3dconnector database!\nCheck your spelling and try again.", "Sea3D Connector");
		return;
	}

	//see if this ID already exists on this computer
	for(i = 0; i < PLAYERS->numPlayers(); i++)
	{
		//check ladder players only
		if(FALSE == PLAYERS->getPlayer(i).getLadder())
		{
			continue;
		}

		//see if it's a dupe
		if(PLAYERS->getPlayer(i).getLadderID() == strID)
		{
			MessageBox("You already have this player on this computer!", "Sea3D Connector");
			return;
		}
	}

	//they've made it here, so ask for the password
	if(IDOK != dlg.DoModal())
	{
		return;
	}

	//compare strings
	if(strPassword != dlg.m_strPassword)
	{
		MessageBox("Invalid password.", "Sea3D Connector");
		return;
	}

	//we made it, so set the value
	m_player.setFirstName(strPlayer);
	m_player.setHandle(TRUE);
	m_player.setLadderID(strID);
	m_player.setLadder(TRUE);

	//let them know they made it
	MessageBox("You have successfully downloaded " + strPlayer + " to this computer!", "Sea3D Connector");

	//leave
	EndDialog(IDOK);
}
