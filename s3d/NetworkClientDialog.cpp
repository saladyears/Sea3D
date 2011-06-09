/* 
 * Settlers3D - Copyright (C) 2001-2003 Jason Fugate (jfugate@settlers3d.net) 
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the Free 
 * Software Foundation; either version 2 of the License, or (at your option) 
 * any later version. 
 * 
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 * for more details. 
 */

#include "stdafx.h"
#include "settlers.h"
#include "NetworkClientDialog.h"
#include "SettlersView.h"
#include "NetworkPortDialog.h"
#include "NetworkConnectorDialog.h"
#include "defineIni.h"
#include "defineNetwork.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkClientDialog dialog


CNetworkClientDialog::CNetworkClientDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkClientDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkClientDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	//assign default port
	m_uiPort = PORT;

	m_uiTimeOut = 0;
}


void CNetworkClientDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkClientDialog)
	DDX_Control(pDX, IDC_CONNECTOR_BUTTON, m_ConnectorButton);
	DDX_Control(pDX, IDC_RECENTIP_BUTTON, m_RecentIPButton);
	DDX_Control(pDX, IDC_INFO_EDIT, m_InfoEdit);
	DDX_Control(pDX, IDC_ADVANCED_BUTTON, m_AdvancedButton);
	DDX_Control(pDX, IDC_CONNECT_BUTTON, m_ConnectButton);
	DDX_Control(pDX, IDC_TCP_STATIC, m_TCPStatic);
	DDX_Control(pDX, IDC_SEND_EDIT, m_SendEdit);
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_PLAYER_LIST, m_PlayerList);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_READY_BUTTON, m_ReadyButton);
}


BEGIN_MESSAGE_MAP(CNetworkClientDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkClientDialog)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, OnConnectButton)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_ADVANCED_BUTTON, OnAdvancedButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_RECENTIP_BUTTON, OnRecentipButton)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_BN_CLICKED(IDC_CONNECTOR_BUTTON, OnConnectorButton)
	ON_BN_CLICKED(IDC_READY_BUTTON, OnReadyButton)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED, ID_POPUP_RECENT1, ID_POPUP_RECENT1 + INI_NUM_IPS, HandlePopupClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkClientDialog message handlers

BOOL CNetworkClientDialog::OnInitDialog() 
{
	CPlayer player;

	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_CONNECT;

	//tell the view about our list control
	VIEW->setListCtrl(&m_PlayerList);
	VIEW->setChatCtrl(&m_ChatEdit);
	VIEW->setInfoCtrl(&m_InfoEdit);
	VIEW->setClientDialog(this);

	//hide the ready button until they connect
	m_ReadyButton.ShowWindow(SW_HIDE);

	//clear the send edit
	m_SendEdit.SetWindowText("");
	m_InfoEdit.SetWindowText("");

	//load the accelerator resource
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));

	//get the player
	player = GAME->m_players[0].m_player;

	//add ourselves to the list control
	m_PlayerList.addPlayer(&player, GAME->m_players[0].m_iColor);

	//enable IP
	enableIP();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// try connecting to the IP address the user has
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnConnectButton() 
{
	int i;
	BYTE w1, w2, w3, w4;
	CString str, strErr, strIP;
	CWaitCursor wait;
	CStringArray strArray;

	//see if we're already connected
	if(TRUE == VIEW->IsConnected())
	{
		if(IDNO == MessageBox("You are currently connected to a game.  Are you sure you want to quit?", "Network Connection", MB_YESNO))
		{
			return;
		}

		//close the connection
		VIEW->CloseConnection();

		return;
	}

	//see if we have a valid IP string
	if(TRUE == m_IPAddress.IsBlank())
	{
		MessageBox("You must specify an IP address.", "Network Connection");
		return;
	}

	//get the ip address
	m_IPAddress.GetAddress(w1, w2, w3, w4);

	//format the string
	str.Format("%d.%d.%d.%d", w1, w2, w3, w4);

	//initialize comm
	if(FALSE == initComm(str, m_uiPort))
	{
		return;
	}

	//we are connected
	connect();

	//add it to the array if it's not in there
	getRecentIP(strArray);

	//find it
	for(i = 0; i < strArray.GetSize(); i++)
	{
		if(strArray[i] == str)
		{
			//remove it if it's there
			strArray.RemoveAt(i);
		}
	}

	//now add it in
	strArray.InsertAt(0, str);

	//write it out
	for(i = 0; i < INI_NUM_IPS; i++)
	{
		//format the output string
		str.Format("%s%d", INI_IP_STRING, i);

		//get the ip
		if(strArray.GetSize() > i)
		{
			strIP = strArray[i];
		}
		else
		{
			strIP = "";
		}

		//write it out
		WritePrivateProfileString(INI_NETWORK, str, strIP, INI_FILE);
	}
}

/////////////////////////////////////////////////////////////////////////////
// we have a connection
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::connect()
{
	m_TCPStatic.SetWindowText("Connected to:");
	m_ConnectButton.SetWindowText("Disconnect");
	m_AdvancedButton.EnableWindow(FALSE);
	m_IPAddress.EnableWindow(FALSE);
	m_RecentIPButton.EnableWindow(FALSE);
	m_ConnectorButton.EnableWindow(FALSE);

	//show the ready button
	m_ReadyButton.ShowWindow(SW_SHOW);
}

/////////////////////////////////////////////////////////////////////////////
// connection has ended
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::disconnect()
{
	CPlayer player;

	//set UI elements
	m_TCPStatic.SetWindowText("TCP/IP Address:");
	m_ConnectButton.SetWindowText("Connect");
	m_AdvancedButton.EnableWindow();
	m_IPAddress.EnableWindow();
	m_RecentIPButton.EnableWindow();
	m_ConnectorButton.EnableWindow();

	//hide the ready button until they connect
	m_ReadyButton.ShowWindow(SW_HIDE);

	//set the player's ready state
	GAME->m_players[GAME->m_iIndex].m_player.setReady(FALSE);

	//enable IP
	enableIP();
}

/////////////////////////////////////////////////////////////////////////////
// initialize communications
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkClientDialog::initComm(CString strAddress, UINT uiPort)
{
	BOOL bOpen;

	//try opening the connection
	bOpen = VIEW->OpenConnection(strAddress, uiPort);

	//if it's false, give a reason
	if(FALSE == bOpen)
	{
		MessageBox("No response from server.", "Network Connection");
		return FALSE;
	}

	//start the timeout timer with a 15 second wait
	m_uiTimeOut = SetTimer(TIMER_TIMEOUT, 15000, NULL);

	//we're good
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// send a chat message
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnSendButton() 
{
	CString str;
	CNetChatMessage chat;
	CNDKMessage msg(S3D_CHAT_MESSAGE);

	//get the data from the edit
	m_SendEdit.GetWindowText(str);

	//set the fields
	chat.m_iColor = GAME->m_players[GAME->m_iIndex].m_iColor;
	chat.m_strName = GAME->m_players[GAME->m_iIndex].m_player.getFirstName();
	chat.m_strMessage = str;
	
	//clear it
	m_SendEdit.SetWindowText("");

	//add it ourselves
	m_ChatEdit.addMessage(&chat);

	//translate it
	chat.SerializeToMessage(msg);

	//transmit it
	VIEW->SendMessageToServer(msg);

	//set the focus back to the edit
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// send a chat message
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnCancel() 
{
	//see if we're connected
	if(TRUE == VIEW->IsConnected())
	{
		if(IDNO == MessageBox("You are currently connected to a game.  Are you sure you want to quit?", "Network Connection", MB_YESNO))
		{
			return;
		}

		//close the connection
		VIEW->CloseConnection();
	}
	
	CHelpDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////
// cleanup
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnDestroy() 
{
	CHelpDialog::OnDestroy();
	
	//references are no longer valid
	VIEW->setListCtrl(NULL);
	VIEW->setChatCtrl(NULL);
	VIEW->setInfoCtrl(NULL);
	VIEW->setClientDialog(NULL);
}

/////////////////////////////////////////////////////////////////////////////
// opportunity to change the port
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnAdvancedButton() 
{
	CNetworkPortDialog dlg;

	//set the current port
	dlg.setPort(m_uiPort);

	//run it
	if(IDOK == dlg.DoModal())
	{
		m_uiPort = dlg.getPort();
	}
}

/////////////////////////////////////////////////////////////////////////////
// handle a time out
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMER_TIMEOUT:
		//we got a timeout, meaning no return signal from the server
		
		//disconnect
		VIEW->CloseConnection();
		disconnect();

		//kill the timer
		KillTimer(m_uiTimeOut);

		//display message
		MessageBox("Timeout waiting for response from the server.  Sea3D connected to the specified address, but did not receive acknowledgement.\nThere might not be an Sea3D server on the other end.", "Network Timeout");

		break;
	default: break;
	}
	
	CHelpDialog::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// handle a click on the Recent IP button
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnRecentipButton() 
{
	int i;
	CMenu menu;
	CMenu *submenu;
	CPoint point;
	CStringArray strArray;

	//get where we are
	GetCursorPos(&point);

	//load the menu
	menu.LoadMenu(IDR_RECENTIP);

	//get the submenu
	submenu = menu.GetSubMenu(0);

	//sanity
	if(NULL == submenu)
	{
		return;
	}

	//get string array
	getRecentIP(strArray);

	//sanity
	if(0 == strArray.GetSize())
	{
		return;
	}

	//clear the old menu items
	for(i = 0; i < INI_NUM_IPS; i++)
	{
		submenu->RemoveMenu(0, MF_BYPOSITION);
	}

	//add in the new ones
	for(i = 0; i < strArray.GetSize(); i++)
	{
		submenu->AppendMenu(MF_ENABLED | MF_STRING, ID_POPUP_RECENT1 + i, strArray[i]);
	}
	
	//set focus to the dialog so it doesn't look bad
	SetFocus();

	//track the popup
	submenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this, NULL);
}

/////////////////////////////////////////////////////////////////////////////
// see if the recent IP button should be enabled
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::enableIP()
{
	CStringArray strArray;

	//figure out if the recent IP button should be on
	getRecentIP(strArray);
	m_RecentIPButton.EnableWindow(strArray.GetSize() > 0);
}

/////////////////////////////////////////////////////////////////////////////
// return a CStringArray filled with recent IPs
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::getRecentIP(CStringArray &strArray)
{
	int i;
	CString str;
	CString strIP;

	//read through the sections
	for(i = 0; i < INI_NUM_IPS; i++)
	{
		//format the string
		str.Format("%s%d", INI_IP_STRING, i);

		//load the string
		GetPrivateProfileString(INI_NETWORK, str, "", strIP.GetBuffer(16), 16, INI_FILE);
		strIP.ReleaseBuffer();

		//add it to the array if it's non empty
		if(FALSE == strIP.IsEmpty())
		{
			strArray.Add(strIP);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// set the ip address on a popup click
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::HandlePopupClick(UINT nID)
{
	int iIP;
	CString strIP;
	CStringArray strArray;

	//get IP
	iIP = nID - ID_POPUP_RECENT1;

	//retrieve
	getRecentIP(strArray);

	//clamp
	if(iIP < 0) iIP = 0;
	if(iIP >= strArray.GetSize()) iIP = strArray.GetSize() - 1;

	//set
	strIP = strArray[iIP];

	//parse the string
	parseIPString(strIP);
}

/////////////////////////////////////////////////////////////////////////////
// handles message forwarding
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkClientDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(NULL != m_hAccelTable)
	{
		if (::TranslateAccelerator(m_hWnd, m_hAccelTable, pMsg)) 
		{
			return TRUE;
		}
	}
	
	return CHelpDialog::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////
// handles pasting
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnEditPaste() 
{
	CWnd *pFocus;
	CEdit *pEdit;
	CRect rect;
	CString str;

	//get what has current focus
	pFocus = GetFocus();

	//compare
	if(&m_SendEdit == pFocus)
	{
		m_SendEdit.PasteSpecial(CF_TEXT);
	}
	else if(m_TCPStatic.GetFocus() == pFocus)
	{
		rect = CRect(0, 0, 500, 20);

		//create the dummy edit
		pEdit = new CEdit();
		pEdit->Create(CHILD, rect, this, 0);

		//paste the text
		pEdit->Paste();

		//get it out
		pEdit->GetWindowText(str);
		delete pEdit;
		pEdit = NULL;

		//parse the string
		parseIPString(str);
	}
}

/////////////////////////////////////////////////////////////////////////////
// parase an IP string and put it in the control
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::parseIPString(CString str)
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
// run the connector dialog
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnConnectorButton() 
{
	int nSel;

	CNetworkConnectorDialog dlg;

	//run it
	if(IDOK == dlg.DoModal())
	{
		//get the selection
		nSel = dlg.getGameSel();

		//sanity
		if(-1 == nSel)
		{
			return;
		}

		//connect
		runConnector(nSel);
	}
}

/////////////////////////////////////////////////////////////////////////////
// update the player's ready state
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::OnReadyButton()
{
	VIEW->clientReady(GAME->m_players[GAME->m_iIndex].m_player.getID(), TRUE);

	//hide the ready button until something changes
	m_ReadyButton.ShowWindow(SW_HIDE);
}

/////////////////////////////////////////////////////////////////////////////
// run the connector
/////////////////////////////////////////////////////////////////////////////
void CNetworkClientDialog::runConnector(int nIndex)
{
	CString strIP;

	//get the IP and port data from the connector database
	CONNECTOR->connectToGame(nIndex, strIP, m_uiPort);

	//sanity
	if(TRUE == strIP.IsEmpty())
	{
		MessageBox("The IP address of the game was invalid.", "Connector");
		return;
	}

	//set the IP address
	parseIPString(strIP);

	//connect
	OnConnectButton();
}