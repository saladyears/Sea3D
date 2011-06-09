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
#include "NetworkConnectorDialog.h"
#include "NetworkPasswordDialog.h"
#include "SettlersView.h"
#include "defineSounds.h"
#include ".\networkconnectordialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// global function declaration
/////////////////////////////////////////////////////////////////////////////
void OnConnectorListClick(DWORD dwCookie);

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorDialog dialog


CNetworkConnectorDialog::CNetworkConnectorDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkConnectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkConnectorDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_uiTimer = (UINT) -1;
	m_nGameSel = -1;

	m_strHelp = HELP_CONNECTOR;
}


void CNetworkConnectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkConnectorDialog)
	DDX_Control(pDX, IDC_90_RADIO, m_90Radio);
	DDX_Control(pDX, IDC_60_RADIO, m_60Radio);
	DDX_Control(pDX, IDC_30_RADIO, m_30Radio);
	DDX_Control(pDX, IDC_OFF_RADIO, m_OffRadio);
	DDX_Control(pDX, IDOK, m_OKButton);
	DDX_Control(pDX, IDC_GAME_LIST, m_GameList);
	DDX_Control(pDX, IDC_PLAYERS_STATIC, m_PlayersStatic);
	DDX_Control(pDX, IDC_PLAYER1_STATIC, m_Player1Static);
	DDX_Control(pDX, IDC_PLAYER2_STATIC, m_Player2Static);
	DDX_Control(pDX, IDC_PLAYER3_STATIC, m_Player3Static);
	DDX_Control(pDX, IDC_PLAYER4_STATIC, m_Player4Static);
	DDX_Control(pDX, IDC_PLAYER5_STATIC, m_Player5Static);
	DDX_Control(pDX, IDC_PLAYER6_STATIC, m_Player6Static);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkConnectorDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkConnectorDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_30_RADIO, On30Radio)
	ON_BN_CLICKED(IDC_60_RADIO, On60Radio)
	ON_BN_CLICKED(IDC_90_RADIO, On90Radio)
	ON_BN_CLICKED(IDC_OFF_RADIO, OnOffRadio)
	ON_BN_CLICKED(IDC_REFRESH_BUTTON, OnRefreshButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkConnectorDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkConnectorDialog::OnInitDialog() 
{
	int i;
	CStatic *pStatic;
	CRect rect;

	CHelpDialog::OnInitDialog();

	//set the callback
	m_GameList.setCallBack(OnConnectorListClick, (DWORD) this);

	//set the image list
	m_GameList.SetImageList(APP->getImageConnector(), LVSIL_NORMAL);
	
	//ok button is disabled until selection
	m_OKButton.EnableWindow(FALSE);

	//set default
	m_OffRadio.SetCheck(TRUE);

	//disable players static
	m_PlayersStatic.EnableWindow(FALSE);

	//empty all the statics
	for(i = 0; i < 6; i++)
	{
		pStatic = &m_Player1Static + i;

		//empty
		pStatic->SetWindowText("");
			
		//tooltip controls
		m_PlayerTips[i].Create(pStatic);
		pStatic->GetWindowRect(rect);
		ScreenToClient(rect);
		m_PlayerTips[i].AddTool(this, "", rect, 1);
		m_PlayerTips[i].Activate(TRUE);
		m_PlayerTips[i].SetDelayTime(150);
		m_PlayerTips[i].SetDelayTime(TTDT_AUTOPOP, 10000);
	}

	//populate the list
	populateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// populate the list
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::populateList()
{
	int i, j;
	int nIndex;
	BOOL bFound;
	CStringArray *pGames;
	CStringArray *pCompare;
	CStringArray strCompare;
	CWaitCursor wait;

	//get the current list of games
	pCompare = CONNECTOR->gameList();

	for(i = 0; i < pCompare->GetSize(); i++)
	{
		strCompare.Add((*pCompare)[i]);
	}

	//clear the current list
	m_GameList.DeleteAllItems();

	//get the list of games
	CONNECTOR->retrieveGames();

	//get the games
	pGames = CONNECTOR->gameList();

	//run through the list and add them
	for(i = 0; i < pGames->GetSize(); i++)
	{
		parseString(i);
	}

	//now traverse the added games and see if any of them are new
	for(i = 0; i < m_GameList.GetItemCount(); i++)
	{
		//get the game index
		nIndex = (m_GameList.GetItemData(i) >> 16);

		//reset flag
		bFound = FALSE;

		//compare to the old list
		for(j = 0; j < strCompare.GetSize(); j++)
		{
			if(strCompare[j].Left(10) == pGames->GetAt(nIndex).Left(10))
			{
				bFound = TRUE;
				break;
			}
		}

		//if it's not there, play the sound
		if(FALSE == bFound)
		{
			SOUND(SOUND_CONNECTOR_GAME);

			//only do it once
			break;
		}
	}

	//reset the selection
	m_GameList.setSelection();
	handleSelection();
}

/////////////////////////////////////////////////////////////////////////////
// handle map selection
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::handleSelection()
{
	int i;
	int nSel;
	CString strReport;
	CStatic *pStatic;
	CStringArray strPlayers;

	//get the selection
	nSel = m_GameList.querySelection();

	//enable/disable the windows
	m_OKButton.EnableWindow(-1 != nSel);
	m_PlayersStatic.EnableWindow(-1 != nSel);

	//clear the statics
	for(i = 0; i < 6; i++)
	{
		pStatic = &m_Player1Static + i;

		//empty
		pStatic->SetWindowText("");

		//update text
		m_PlayerTips[i].UpdateTipText("", this, 1);
	}

	//get the players
	if(-1 != nSel)
	{
		//get the real list
		nSel = (m_GameList.GetItemData(nSel) >> 16);

		//get the players
		CONNECTOR->retrievePlayers(nSel, strPlayers);

		//set the players
		for(i = 0; i < strPlayers.GetSize(); i++)
		{
			pStatic = &m_Player1Static + i;

			//empty
			pStatic->SetWindowText(strPlayers[i]);

			//get player reports
			strReport = CONNECTOR->reportPlayer(strPlayers[i]);

			//update text
			m_PlayerTips[i].UpdateTipText(strReport, this, 1);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// parse a game string and add it to the list
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::parseString(int nIndex)
{
	int i;
	int nSave;
	int nStatus;
	int nState = 0;
	int nMax = 4;
	CArray <int, int> nPipes;
	CString str, strTemp, strParse;

	//check it
	if(FALSE == CONNECTOR->isGameValid(nIndex, strParse, nPipes))
	{
		return;
	}

	//save the index
	nSave = nIndex;

	//now parse the string
	for(i = 0; i < CONNECTOR_FIELDS; i++)
	{
		//get the text
		str = strParse.Mid(nPipes[i], nPipes[i + 1] - nPipes[i]);

		//remove any pipes
		str.Remove(CONNECTOR_SEPARATOR);

		//add if necessary
		switch(i)
		{
		case CONNECTOR_ID:
			//insert the item
			nIndex = m_GameList.InsertItem(nIndex, "");
			break;
		case CONNECTOR_GAME:
			//set the game name
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_GAME, str);
			break;
		case CONNECTOR_HOST:
			//set the host name
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_HOST, str);
			break;
		case CONNECTOR_IP:
			//set the IP address
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_IP, str);
			break;
		case CONNECTOR_PORT:
			//set the port
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_PORT, str);
			break;
		case CONNECTOR_MAP:
			//set the map name
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_MAP, str);
			break;
		case CONNECTOR_MAX:
			nMax = atoi(str);
			break;
		case CONNECTOR_CURRENT:
			//format the string
			strTemp.Format("%s/%d", str, nMax);

			//set the current players
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_MAX, strTemp);
			break;
		case CONNECTOR_PRIVATE:
			//set the private value
			if("yes" == str)
			{
				nState |= CONNECTOR_LIST_LOCK;
			}
			break;
		case CONNECTOR_VER:
			//set the version value
			m_GameList.SetItemText(nIndex, CONNECTOR_LIST_VER, str);
			break;
		case CONNECTOR_STATUS:
			//the game must be new or updated to display in this list
			nStatus = atoi(str);

			//compare
			if(CONNECTOR_INGAME == nStatus)
			{
				m_GameList.DeleteItem(nIndex);
				return;
			}
			break;
		case CONNECTOR_RESTART:
			//set the restart value
			if("yes" == str)
			{
				nState |= CONNECTOR_LIST_RESTART;
			}
			break;
		case CONNECTOR_LADDER:
			//set the ladder value
			if("yes" == str)
			{
				nState |= CONNECTOR_LIST_LADDER;
			}
			break;
		}
	}

	//set the item data
	m_GameList.SetItemData(nIndex, (nSave << 16) + nState);
}

/////////////////////////////////////////////////////////////////////////////
// manual refresh
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::OnRefreshButton() 
{
	populateList();	
}

/////////////////////////////////////////////////////////////////////////////
// set the current selection
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::OnOK() 
{
	int nSel;
	BOOL bPrivate;
	CString strPassword;
	CNetworkPasswordDialog dlg;

	//get the list index
	nSel = m_GameList.querySelection();

	//set it
	if(-1 != nSel)
	{
		m_nGameSel = (m_GameList.GetItemData(nSel) >> 16);
	}

	//if this is the client, and it's a password protected game, they
	//have to enter the password
	if(FALSE == CONNECTOR->isServer())
	{
		//get the information
		CONNECTOR->checkPrivate(m_nGameSel, bPrivate, strPassword);

		//if private, run the dialog
		if(TRUE == bPrivate)
		{
			//decrypt the password
			strPassword = CONNECTOR->decrypt(strPassword);

			//run it
			if(IDOK == dlg.DoModal())
			{
				//compare
				if(dlg.m_strPassword != strPassword)
				{
					MessageBox("Invalid Password", "Connector");
					return;
				}
			}
			else
			{
				return;
			}
		}
	}

	//kill any current timer
	KillTimer(m_uiTimer);

	CHelpDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::OnCancel() 
{
	//kill any current timer
	KillTimer(m_uiTimer);
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// refresh
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_CONNECTOR:
		populateList();
		break;
	}
	
	CHelpDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::OnOffRadio() 
{
	//kill any current timer
	KillTimer(m_uiTimer);
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::On30Radio() 
{
	//kill any current timer
	KillTimer(m_uiTimer);

	//set the new timer
	m_uiTimer = SetTimer(TIMER_CONNECTOR, 30000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::On60Radio() 
{
	//kill any current timer
	KillTimer(m_uiTimer);

	//set the new timer
	m_uiTimer = SetTimer(TIMER_CONNECTOR, 60000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// radio handling
/////////////////////////////////////////////////////////////////////////////
void CNetworkConnectorDialog::On90Radio() 
{
	//kill any current timer
	KillTimer(m_uiTimer);

	//set the new timer
	m_uiTimer = SetTimer(TIMER_CONNECTOR, 90000, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// callback handler
/////////////////////////////////////////////////////////////////////////////
void OnConnectorListClick(DWORD dwCookie)
{
	if(NULL != dwCookie)
	{
		((CNetworkConnectorDialog *) dwCookie)->handleSelection();
	}
}

/////////////////////////////////////////////////////////////////////////////
// tooltip parsing
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkConnectorDialog::PreTranslateMessage(MSG* pMsg)
{
	int i;

	//relay others
	for(i = 0; i < 5; i++)
	{
		if(NULL != m_PlayerTips[i].m_hWnd)
		{
			m_PlayerTips[i].RelayEvent(pMsg);
		}
	}

	return CHelpDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// cleanup tooltips
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkConnectorDialog::DestroyWindow()
{
	int i;

	for(i = 0; i < 6; i++)
	{
		//tooltips
		m_PlayerTips[i].DestroyWindow();
	}

	return CHelpDialog::DestroyWindow();
}
