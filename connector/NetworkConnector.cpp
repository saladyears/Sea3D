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
#include "NetworkConnector.h"
#include "SettlersView.h"
#include "MapDatabase.h"
#include "defineIni.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CNetworkConnector::CNetworkConnector()
{
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CNetworkConnector::~CNetworkConnector()
{
}

//////////////////////////////////////////////////////////////////////
// server initialization
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::initServer(CString, BOOL, CString, BOOL, BOOL )
{
}

//////////////////////////////////////////////////////////////////////
// post a host to the database
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::postServer(int)
{
}

//////////////////////////////////////////////////////////////////////
// post ladder stats to the database
//////////////////////////////////////////////////////////////////////
int CNetworkConnector::postLadder(CGame *)
{
	return -1;
}

//////////////////////////////////////////////////////////////////////
// repost a game to the ladder
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::repostGame(int)
{

}

//////////////////////////////////////////////////////////////////////
// ladder ASP
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::createPlayerASP(int)
{
	return "";
}

//////////////////////////////////////////////////////////////////////
// ladder ASP
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::createGameASP(BOOL)
{
	return "";
}

//////////////////////////////////////////////////////////////////////
// reset connector state
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::reset()
{
}

//////////////////////////////////////////////////////////////////////
// fill variables with server info
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::infoServer(CString &, BOOL &, CString &)
{
}

//////////////////////////////////////////////////////////////////////
// fill the string array with available game information
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::retrieveGames()
{
}

//////////////////////////////////////////////////////////////////////
// see if a game is a valid game
//////////////////////////////////////////////////////////////////////
BOOL CNetworkConnector::isGameValid(int , CString &, CArray <int, int> &)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// return connect information
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::connectToGame(int , CString &, UINT &)
{
}

//////////////////////////////////////////////////////////////////////
// check for a game
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::checkPrivate(int, BOOL &, CString &)
{
}

//////////////////////////////////////////////////////////////////////
// retreive the players in a game
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::retrievePlayers(int, CStringArray &)
{
}

//////////////////////////////////////////////////////////////////////
// create the host ASP connect string
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::createHostASP()
{
	return "";
}

//////////////////////////////////////////////////////////////////////
// handle the actual ASP page connection
//////////////////////////////////////////////////////////////////////
BOOL CNetworkConnector::connectASP(CString, CString &)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// validate the chosen name of a user for the ladder system
//////////////////////////////////////////////////////////////////////
int CNetworkConnector::validateName(CString)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
// register a ladder player
//////////////////////////////////////////////////////////////////////
BOOL CNetworkConnector::registerPlayer(CString, CString, CString &)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// download a ladder player
//////////////////////////////////////////////////////////////////////
int CNetworkConnector::downloadPlayer(CString, CString &, CString &)
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// get a report on a player
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::reportPlayer(CString)
{
	return "";
}

//////////////////////////////////////////////////////////////////////
// record a player quitting
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::quitPlayer(int, BOOL)
{
}

//////////////////////////////////////////////////////////////////////
// record a player returning
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::unquitPlayer(int)
{
}

//////////////////////////////////////////////////////////////////////
// clear quits at the end of the game
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::clearQuits()
{
}

//////////////////////////////////////////////////////////////////////
// return a player's quit score
//////////////////////////////////////////////////////////////////////
int CNetworkConnector::quitScore(CString)
{
	return 0;
}

//////////////////////////////////////////////////////////////////////
// encryption
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::encrypt(CString str)
{
	return str;
}

//////////////////////////////////////////////////////////////////////
// decryption
//////////////////////////////////////////////////////////////////////
CString CNetworkConnector::decrypt(CString str)
{
	return str;
}

//////////////////////////////////////////////////////////////////////
// create an internet session
//////////////////////////////////////////////////////////////////////
BOOL CNetworkConnector::createSession()
{
	return FALSE;
}

//////////////////////////////////////////////////////////////////////
// delete the current internet session
//////////////////////////////////////////////////////////////////////
void CNetworkConnector::deleteSession()
{
}
