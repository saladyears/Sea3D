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
#include "ChatWnd.h"
#include "defineColors.h"
#include "SettlersView.h"
#include "NDKMessage.h"
#include "XLogFont.h"
#include "defineNetwork.h"
#include ".\chatwnd.h"

// CChatWnd

IMPLEMENT_DYNAMIC(CChatWnd, CWnd)
CChatWnd::CChatWnd()
{
}

CChatWnd::~CChatWnd()
{
	m_font.DeleteObject();
}


BEGIN_MESSAGE_MAP(CChatWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChatWnd message handlers

/////////////////////////////////////////////////////////////////////////////
// startup
/////////////////////////////////////////////////////////////////////////////
int CChatWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	int nH, nW;
	CRect rect;
	XLOGFONT xfont;

	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//get the client rect
	GetClientRect(&rect);
	nW = rect.Width(); nH = rect.Height();

	//message control
	m_MessageEdit.Create(CHILD | TCS_HOTTRACK | TCS_BUTTONS | TCS_RAGGEDRIGHT | TCS_MULTISELECT | WS_VSCROLL,
		CRect(1, 1, nW - 1, ((nH - 20) >> 1) - 1), this, 0);
	m_MessageEdit.initControl();
	m_MessageEdit.SetWindowText("");

	//chat control
	m_ChatEdit.Create(CHILD | CHILD | TCS_HOTTRACK | TCS_BUTTONS | TCS_RAGGEDRIGHT | TCS_MULTISELECT | WS_VSCROLL,
		CRect(1, ((nH - 20) >> 1), nW - 1, nH - 21), this, 0);
	m_ChatEdit.initControl();
	m_ChatEdit.SetWindowText("");

	//create the chat controls
	m_SendEdit.Create(CHILD | TCS_VERTICAL | TCS_BUTTONS, CRect(1, nH - 20, nW - 40, nH - 1), this, 0);
	m_SendEdit.initControl();
	m_SendEdit.SetWindowText("");
	m_SendEdit.setSend();

	//create the font
	xfont = GetLogFont(12, "Tahoma");
	m_font.CreateFontIndirect((LOGFONT *) &xfont);

	//create the send button
	m_SendButton.Create("Send", CHILD, CRect(nW - 39, nH - 20, nW - 1, nH - 1), this, IDC_SEND_BUTTON);
	m_SendButton.SetButtonStyle(BS_DEFPUSHBUTTON);
	m_SendButton.SetColor(CButtonST::BTNST_COLOR_BK_IN, COLOR_BLACK);
	m_SendButton.SetColor(CButtonST::BTNST_COLOR_BK_OUT, COLOR_BLACK);
	m_SendButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, COLOR_WHITE);
	m_SendButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, COLOR_WHITE);
	m_SendButton.SetFont(&m_font);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// pass the controls to the view
/////////////////////////////////////////////////////////////////////////////
void CChatWnd::setControls(void)
{
	//set the view pointers
	VIEW->setSendCtrl(&m_SendEdit);
	VIEW->setChatCtrl(&m_ChatEdit);
	VIEW->setMessageCtrl(&m_MessageEdit);
}

/////////////////////////////////////////////////////////////////////////////
// draw background
/////////////////////////////////////////////////////////////////////////////
void CChatWnd::OnPaint()
{
	CRect rect;
	CPaintDC dc(this); // device context for painting

	//get draw area
	GetClientRect(&rect);

	//fill it
	dc.FillSolidRect(&rect, COLOR_BUILDGRAY);
}

/////////////////////////////////////////////////////////////////////////////
// handle sending a message
/////////////////////////////////////////////////////////////////////////////
void CChatWnd::OnSendButton() 
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
// resize controls
/////////////////////////////////////////////////////////////////////////////
void CChatWnd::OnSize(UINT nType, int cx, int cy)
{
	int nChatW, nChatH;
	CRect rect;

	CWnd::OnSize(nType, cx, cy);

	//calcuate chat widths and heights
	nChatW = cx - 2;
	nChatH = ((cy - 20) >> 1) - 1;

	//resize controls
	m_MessageEdit.SetWindowPos(&wndTop, 1, 1, nChatW, nChatH, SWP_SHOWWINDOW);
	m_ChatEdit.SetWindowPos(&wndTop, 1, ((cy - 20) >> 1) + 1, nChatW, nChatH - 1, SWP_SHOWWINDOW);
	m_SendEdit.SetWindowPos(&wndTop, 1, cy - 20, cx - 42, 19, SWP_SHOWWINDOW);
	m_SendButton.SetWindowPos(&wndTop, cx - 40, cy - 20, 39, 19, SWP_SHOWWINDOW);
}
