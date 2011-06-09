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
#include "NetworkChatDialog.h"
#include "NetworkView.h"
#include "NDKMessage.h"
#include "SettlersView.h"
#include "defineNetwork.h"
#include "defineIni.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNetworkChatDialog dialog


CNetworkChatDialog::CNetworkChatDialog(CWnd* pParent /*=NULL*/)
	: CHelpDialog(CNetworkChatDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNetworkChatDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNetworkChatDialog::DoDataExchange(CDataExchange* pDX)
{
	CHelpDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNetworkChatDialog)
	DDX_Control(pDX, IDC_SEND_BUTTON, m_SendButton);
	DDX_Control(pDX, IDC_SEND_EDIT, m_SendEdit);
	DDX_Control(pDX, IDC_CHAT_EDIT, m_ChatEdit);
	DDX_Control(pDX, IDC_MESSAGE_EDIT, m_MessageEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNetworkChatDialog, CHelpDialog)
	//{{AFX_MSG_MAP(CNetworkChatDialog)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_MOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNetworkChatDialog message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkChatDialog::OnInitDialog() 
{
	CHelpDialog::OnInitDialog();

	//help system
	m_strHelp = HELP_CHAT;
	
	//clear the send edit
	m_SendEdit.setSend();
	m_SendEdit.SetWindowText("");
	m_ChatEdit.SetWindowText("");
	m_MessageEdit.SetWindowText("");

	//load the accelerator resource
	m_hAccelTable = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/////////////////////////////////////////////////////////////////////////////
// set the controls
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::setControls()
{
	//set the chat control
	VIEW->setChatCtrl(&m_ChatEdit);
	VIEW->setSendCtrl(&m_SendEdit);
	VIEW->setMessageCtrl(&m_MessageEdit);
}

/////////////////////////////////////////////////////////////////////////////
// handle sending a message
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnSendButton() 
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
	if(TRUE == GAME->m_bServer)
	{
		VIEW->SendMessageToAllUsers(msg);
	}
	else
	{
		VIEW->SendMessageToServer(msg);
	}

	//set the focus back to the view
	VIEW->SetFocus();
}

/////////////////////////////////////////////////////////////////////////////
// move everything around
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnSize(UINT nType, int cx, int cy) 
{
	int iH;
	int iW1 = 0, iW2;
	CRect rect;
	CString str;

	CHelpDialog::OnSize(nType, cx, cy);

	//sanity
	if(NULL == m_SendButton.GetSafeHwnd())
	{
		return;
	}

	//move the send button
	m_SendButton.GetClientRect(&rect);
	iW2 = rect.Width();
	iH = rect.Height();
	m_SendButton.MoveWindow(cx - iW2 - 1, cy - iH - 1, iW2, iH);

	//move the send edit box
	m_SendEdit.GetClientRect(&rect);
	m_SendEdit.MoveWindow(0, cy - iH - 1, cx - iW1 - iW2 - 3, iH);

	//move the chat control
	m_ChatEdit.MoveWindow(0, 0, (cx >> 1) - 1, cy - iH - 2);
	m_MessageEdit.MoveWindow((cx >> 1) + 1, 0, (cx >> 1) - 1, cy - iH - 2);

	//update ini file
	str.Format("%d", cx);
	WritePrivateProfileString(INI_DISPLAY, INI_CHAT_SIZE_X, str, INI_FILE);

	str.Format("%d", cy);
	WritePrivateProfileString(INI_DISPLAY, INI_CHAT_SIZE_Y, str, INI_FILE);
}

/////////////////////////////////////////////////////////////////////////////
// don't allow resizes to miniscule dimensions
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	CRect rect;

	CWnd::OnGetMinMaxInfo(lpMMI);

	//set the min track point
	lpMMI->ptMinTrackSize = CPoint(150, 80);
}

/////////////////////////////////////////////////////////////////////////////
// safely shut down
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnDestroy() 
{
	if(NULL != VIEW)
	{
		//set the chat control
		VIEW->setChatCtrl(NULL);
	}

	CHelpDialog::OnDestroy();	
}

/////////////////////////////////////////////////////////////////////////////
// handle accelerator messages
/////////////////////////////////////////////////////////////////////////////
BOOL CNetworkChatDialog::PreTranslateMessage(MSG* pMsg) 
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
// handle pasting
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnEditPaste() 
{
	m_SendEdit.PasteSpecial(CF_TEXT);
}

/////////////////////////////////////////////////////////////////////////////
// update .ini file on movement
/////////////////////////////////////////////////////////////////////////////
void CNetworkChatDialog::OnMove(int x, int y) 
{
	CString str;

	CHelpDialog::OnMove(x, y);
	
	//update ini file
	str.Format("%d", x);
	WritePrivateProfileString(INI_DISPLAY, INI_CHAT_OFFSET_X, str, INI_FILE);

	str.Format("%d", y);
	WritePrivateProfileString(INI_DISPLAY, INI_CHAT_OFFSET_Y, str, INI_FILE);
}
