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
#include "defineNetwork.h"
#include "ChatControl.h"
#include "ColorDatabase.h"
#include "defineColors.h"
#include ".\chatcontrol.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static DWORD CALLBACK streamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);

/////////////////////////////////////////////////////////////////////////////
// CChatControl

CChatControl::CChatControl()
{
	m_bSend = FALSE;
}

CChatControl::~CChatControl()
{
}


BEGIN_MESSAGE_MAP(CChatControl, CRichEditCtrl)
	//{{AFX_MSG_MAP(CChatControl)
	//}}AFX_MSG_MAP
	ON_WM_CHAR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatControl message handlers

/////////////////////////////////////////////////////////////////////////////
// add a chat message to the control
/////////////////////////////////////////////////////////////////////////////
void CChatControl::addMessage(CNetChatMessage *pMsg)
{
	CString str;
	COLORREF cr;

	//get the color
	cr = COLORS->getTextColor(pMsg->m_iColor);

	//get the player
	str = '\n';
	str += pMsg->m_strName;

	//write it bold
	addString(str, TRUE, cr);

	//get the message
	str = ": ";
	str += pMsg->m_strMessage;
	
	//write it normal
	addString(str, FALSE, cr);

	//scroll it
	LineScroll(1);
}

/////////////////////////////////////////////////////////////////////////////
// add a system message
/////////////////////////////////////////////////////////////////////////////
void CChatControl::addSysMessage(CNetSysMessage *pMsg)
{
	int iColor;
	CString str;
	COLORREF cr;

	//get the formatted string
	str = getSysString(pMsg);

	//get the color
	iColor = pMsg->m_iColor;

	if(-1 == iColor)
	{
		cr = COLOR_WHITE;
	}
	else
	{
		cr = COLORS->getTextColor(iColor);
	}

	//write it bold
	addString(str, (-1 == iColor), cr);

	//scroll it
	LineScroll(1);
}

/////////////////////////////////////////////////////////////////////////////
// underlying implementation
/////////////////////////////////////////////////////////////////////////////
void CChatControl::addString(CString str, BOOL bBold, COLORREF cr)
{
	int iLength;
	EDITSTREAM es;
	CHARFORMAT cf;

	//avoid replacing selected text
	iLength = GetTextLength();
	SetSel(iLength, iLength);

	//set the "cookie"
	es.dwCookie = (DWORD) &str;
	es.pfnCallback = streamIn;

	//prepare formatting
	cf.dwMask = CFM_COLOR | CFM_BOLD;
	cf.dwEffects = bBold ? CFE_BOLD : 0;
	cf.crTextColor = cr;
	
	SetWordCharFormat(cf);

	//stream in the text
	StreamIn(SF_TEXT | SFF_SELECTION, es);
}

/////////////////////////////////////////////////////////////////////////////
// callback message
/////////////////////////////////////////////////////////////////////////////
DWORD CALLBACK streamIn(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	static BOOL bDone = FALSE;
	CString *pstrText;

	//set the pointer
	pstrText = (CString *) dwCookie;

	//set the buffer
	strncpy((char *) pbBuff, (LPCTSTR) *pstrText, cb);

	//set the length
	if(FALSE == bDone)
	{
		*pcb = strlen((char *) pbBuff);
		bDone = TRUE;
	}
	else
	{
		*pcb = 0;
		bDone = FALSE;
	}

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// set up some stuff
/////////////////////////////////////////////////////////////////////////////
void CChatControl::PreSubclassWindow() 
{
	CRichEditCtrl::PreSubclassWindow();

	//initialize
	initControl();	
}

/////////////////////////////////////////////////////////////////////////////
// initialize
/////////////////////////////////////////////////////////////////////////////
void CChatControl::initControl()
{
	CRect rect;
	CHARFORMAT cf;
	CNetSysMessage msg;

	//go black
	SetBackgroundColor(FALSE, RGB(0, 0, 0));

	//set default font stuff
	cf.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD;
	cf.dwEffects = 0;
	cf.yHeight = 162;
	cf.crTextColor = RGB(255, 255, 255);
	strncpy(cf.szFaceName, "Tahoma", sizeof(cf.szFaceName));
	SetDefaultCharFormat(cf);

	//give a little border
	GetRect(&rect);
	rect.DeflateRect(5, 3, 0, 0);
	SetRect(&rect);

	//add the starting message
	addString("***Chat services have been initialized.", FALSE, COLOR_WHITE);
}

/////////////////////////////////////////////////////////////////////////////
// return a formatted system message
/////////////////////////////////////////////////////////////////////////////
CString CChatControl::getSysString(CNetSysMessage *pMsg)
{
	CString str;
	CString strTemp;

	//start it off
	str = "\n***";

	//format the string
	switch(pMsg->m_uiMessage)
	{
	case SYS_JOINED_GAME: strTemp = pMsg->m_strData + " joined the game."; break;
	case SYS_REJOIN_GAME: strTemp = pMsg->m_strData + " rejoined the game."; break;
	case SYS_LEFT_GAME: strTemp = pMsg->m_strData + " left the game."; break;
	case SYS_BOOTED_FROM_GAME: strTemp = pMsg->m_strData + " was booted from the game!"; break;
	case SYS_SEND_ERROR: strTemp = pMsg->m_strData + " lost the connection while sending data!"; break;
	case SYS_RECV_ERROR: strTemp = pMsg->m_strData + " lost the connection while receiving data!"; break;
	case SYS_SERVER_QUIT: strTemp = "The server was shut down and the connection lost!"; break;
	case SYS_INVALID_PLAYER: strTemp = "A player tried to connect who was not in the saved game!  They were terminated."; break;
	case SYS_VERSION_MISMATCH: strTemp = "A player tried to connect with a different version of Sea3D! They were terminated."; break;
	case SYS_DROPPED: strTemp = pMsg->m_strData + " dropped from the game!"; break;
	case SYS_COLOR_ASSIGN: strTemp = pMsg->m_strData + " was assigned the color " + pMsg->m_strData2 + " by the server."; break;
	case SYS_PORT_PLACE: strTemp = pMsg->m_strData + " placed a port."; break;
	case SYS_SETTLE_PLACE: strTemp = pMsg->m_strData + " built a settlement."; break;
	case SYS_ROAD_PLACE: strTemp = pMsg->m_strData + " built a road."; break;
	case SYS_SHIP_PLACE: strTemp = pMsg->m_strData + " built a ship."; break;
	case SYS_CITY_PLACE: strTemp = pMsg->m_strData + " built a city."; break;
	case SYS_SHIP_MOVE: strTemp = pMsg->m_strData + " moved a ship."; break;
	case SYS_ROLL: strTemp = pMsg->m_strData + " rolled " + pMsg->m_strData2; break;
	case SYS_ROBBER_PLACE: strTemp = pMsg->m_strData + " placed the robber."; break;
	case SYS_PIRATE_PLACE: strTemp = pMsg->m_strData + " placed the pirate."; break;
	case SYS_STEAL: strTemp = pMsg->m_strData + " stole " + pMsg->m_strData2; break;
	case SYS_STEAL_REPORT: strTemp = pMsg->m_strData; break;
	case SYS_TRADE_TO_BANK: strTemp = pMsg->m_strData + " traded" + pMsg->m_strData2; break;
	case SYS_TRADE_JUNGLE: strTemp = pMsg->m_strData + " traded" + pMsg->m_strData2; break;
	case SYS_BUY_DEV_CARD: strTemp = pMsg->m_strData + " bought a development card."; break;
	case SYS_PLAY_DEV_CARD: strTemp = pMsg->m_strData + " played a " + pMsg->m_strData2 + " card."; break;
	case SYS_LOSE_CARDS: strTemp = pMsg->m_strData + " lost" + pMsg->m_strData2; break;
	case SYS_GOLD: strTemp = pMsg->m_strData + " gained " + pMsg->m_strData2 + " from gold."; break;
	case SYS_CHITLESS: strTemp = pMsg->m_strData + " swapped a roll chit."; break;
	case SYS_END_TURN: strTemp = pMsg->m_strData + " ended turn."; break;
	case SYS_TRADE_OFFER: strTemp = pMsg->m_strData + " offered a trade to " + pMsg->m_strData2; break;
	case SYS_TRADE: strTemp = pMsg->m_strData + " traded" + pMsg->m_strData2 + "."; break;
	case SYS_AUTO_SAVE: strTemp = "Game auto-saved."; break;
	case SYS_GAME_FULL: strTemp = "The game had already started."; break;
	case SYS_GAME_OVER: strTemp = "The game has already ended."; break;
	case SYS_LADDER_GAME: strTemp = "That game is only open to ladder players."; break;
	case SYS_LADDER_SAMEIP: strTemp = "You tried to connect to a ladder game with the same IP address as another player.  This is not allowed!"; break;
	case SYS_LADDER_WARNIP: strTemp = pMsg->m_strData + " tried to connect but had the same IP address as " + pMsg->m_strData2 + ". In ladder games, this is not allowed."; break;
	case SYS_MAX_PLAYERS: strTemp = "The host player had set a limit of " + pMsg->m_strData + " players for the game and the game was full."; break;
	case SYS_QUIT_CUTOFF: strTemp = pMsg->m_strData; break;
	case SYS_DOUBLE_JOIN: strTemp = pMsg->m_strData + " tried to join the game a second time!"; break;
	case SYS_UNDO_ROAD: strTemp = pMsg->m_strData + " undid a road placement."; break;
	case SYS_UNDO_SHIP: strTemp = pMsg->m_strData + " undid a ship placement."; break;
	case SYS_UNDO_SHIP_MOVE: strTemp = pMsg->m_strData + " undid a ship move."; break;
	case SYS_UNDO_CITY: strTemp = pMsg->m_strData + " undid a city placement."; break;
	case SYS_UNDO_SETTLEMENT: strTemp = pMsg->m_strData + " undid a settlement placement."; break;
	case SYS_UNDO_AUTOTRADE: strTemp = pMsg->m_strData + " undid a trade with the bank."; break;
	case SYS_MONOPOLY: strTemp = pMsg->m_strData + " stole " + pMsg->m_strData2 + "."; break;
	}

	//add it
	str += strTemp;

	return str;
}

void CChatControl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if((VK_RETURN == nChar) && (TRUE == m_bSend))
	{
		GetParent()->PostMessage(WM_COMMAND, IDC_SEND_BUTTON, 0);
	}
	else
	{
		CRichEditCtrl::OnChar(nChar, nRepCnt, nFlags);
	}
}
