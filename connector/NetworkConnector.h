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


#if !defined(AFX_NETWORKCONNECTOR_H__BFA2C8F7_0428_48D6_B0DD_63457E7A23F4__INCLUDED_)
#define AFX_NETWORKCONNECTOR_H__BFA2C8F7_0428_48D6_B0DD_63457E7A23F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// include files
////////////////////////////////////////////////////////////////////////////
#include "defineConnector.h"

/////////////////////////////////////////////////////////////////////////////
// network connector class
/////////////////////////////////////////////////////////////////////////////
class CNetworkConnector  
{
public:
	CNetworkConnector();
	virtual ~CNetworkConnector();

	//server methods
	void initServer(CString, BOOL, CString, BOOL, BOOL);
	void postServer(int nGame = 0);
	int postLadder(CGame *);
	void infoServer(CString &, BOOL &, CString &);
	void reset(void);

	//client methods
	void retrieveGames(void);
	void connectToGame(int, CString &, UINT &);
	void checkPrivate(int, BOOL &, CString &);
	void retrievePlayers(int, CStringArray &);

	//ladder methods
	int validateName(CString );
	BOOL registerPlayer(CString, CString, CString &);
	int downloadPlayer(CString, CString &, CString &);
	CString reportPlayer(CString);
	void repostGame(int);
	void quitPlayer(int, BOOL);
	void unquitPlayer(int);
	void clearQuits(void);
	int quitScore(CString);
	CString playerIP(void) {return "";};

	//connection methods
	BOOL connectASP(CString, CString &);

	//connector use methods
	void setConnector(BOOL b) {m_bUseConnector = b;}
	BOOL getConnector(void) {return m_bUseConnector;}

	//query methods
	BOOL isServer(void) {return m_bServer;}
	BOOL inConnection(void) {return m_bInConnection;}
	BOOL isPortForwarding(void) {return m_bPortForwarding;}
	BOOL isGameValid(int, CString &, CArray <int, int> &);
	UINT getPort(void) {return m_uiPort;}
	CString getIP(void) {return m_strIP;}

	//set methods
	void setIP(CString str) {m_strIP = str;}
	void setPort(UINT ui) {m_uiPort = ui;}
	void setPortForwarding(BOOL b) {m_bPortForwarding = b;}
	void setName(CString) {}
	void setRestart(BOOL) {}

	//the game list
	CStringArray *gameList(void) {return &m_strGames;}

	//encryption
	CString encrypt(CString str);
	CString decrypt(CString str);

// Implementation
protected:
	CString createHostASP(void);

	//ladder ASP
	CString createPlayerASP(int nPlayer);
	CString createGameASP(BOOL bFinal);

	//internet sessions
	BOOL createSession(void);
	void deleteSession(void);

// Data members
protected:
	//whether we are actually using the connector
	BOOL m_bUseConnector;

	//are we the server?
	BOOL m_bServer;

	//are we in a connection
	BOOL m_bInConnection;

	//are we using port forwarding?
	BOOL m_bPortForwarding;

	//ip address
	CString m_strIP;

	//port number
	UINT m_uiPort;

	//the retrieved games
	CStringArray m_strGames;
};

#endif // !defined(AFX_NETWORKCONNECTOR_H__BFA2C8F7_0428_48D6_B0DD_63457E7A23F4__INCLUDED_)
