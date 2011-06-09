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

#if !defined(AFX_PLAYERDATABASE_H__F5AA9BBE_4710_4442_995D_2CD75E2896C6__INCLUDED_)
#define AFX_PLAYERDATABASE_H__F5AA9BBE_4710_4442_995D_2CD75E2896C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// instance definition
//////////////////////////////////////////////////////////////////////
#define PLAYERS		CPlayerDatabase::instance()

//////////////////////////////////////////////////////////////////////
// included files
//////////////////////////////////////////////////////////////////////
#include "Player.h"
#include "Game.h"

//////////////////////////////////////////////////////////////////////
// static class for writing to and from the player database
//////////////////////////////////////////////////////////////////////
class CPlayerDatabase  
{
public:
	//////////////////////////////////////////////////////////////////
	// constructor / destructor
	//////////////////////////////////////////////////////////////////
	CPlayerDatabase();
	virtual ~CPlayerDatabase();

protected:
	/////////////////////////////////////////////////////////////////////////
	// a pointer to a static instance of this class
	/////////////////////////////////////////////////////////////////////////
	static CPlayerDatabase *m_pPlayerDatabase;

	//opens the database and gets information
	static void open(void);

	//the array of players
	static CArray <CPlayer, CPlayer> m_players;

public:
	/////////////////////////////////////////////////////////////////////
	// instanstiation/destruction methods
	/////////////////////////////////////////////////////////////////////
	static CPlayerDatabase *instance(void);
	static void release(void);

	//////////////////////////////////////////////////////////////////
	// information methods
	//////////////////////////////////////////////////////////////////
	static int numPlayers(void);

	//////////////////////////////////////////////////////////////////
	// database methods
	//////////////////////////////////////////////////////////////////
	static CPlayer getPlayer(int iPlayer);
	static CPlayer getPlayerByID(UINT uiID);
	static int addPlayer(CPlayer *pPlayer);
	static void deletePlayer(int iPlayer);
	static void savePlayers(void);
	static void updateGames(int iPlayer);

	static void updatePlayer(int iPlayer, CPlayer &player);

};

#endif // !defined(AFX_PLAYERDATABASE_H__F5AA9BBE_4710_4442_995D_2CD75E2896C6__INCLUDED_)
