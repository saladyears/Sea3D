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
#include "Game.h"
#include "GameDatabase.h"
#include "defineMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CGame, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CGame::CGame()
{
	//not a server by default
	m_bServer = FALSE;

	//not a ladder by default
	m_bLadder = FALSE;

	//not a team by default
	m_bTeam = FALSE;

	//default to a hotseat game
	m_iMode = GAME_MODE_HOTSEAT;

	//default to all system messages
	m_iSysNotify = 0xFFFFFFFF;

	//set the date/time
	m_timeStart = COleDateTime::GetCurrentTime();

	//give this game a new and unique ID
	m_uiID = (UINT) time(NULL);

	//default to no map
	m_uiMapID = 0;

	//default to false
	m_bInit = FALSE;
	m_bGenerate = FALSE;

	//not in game yet
	m_bInGame = FALSE;

	//default to the max allowed
	m_nMaxPlayers = 6;

	//default cutoff percent to 25%, but disabled
	m_bUseQuitCutoff = FALSE;
	m_nQuitCutoff = 25;

	//no network index yet
	m_iIndex = 0;
	m_uiPlayerID = 0;

	//no winner yet
	m_iWinner = -1;

	//the bank's resources
	::ZeroMemory(m_iBankRes, sizeof(m_iBankRes));

	//start off as a new game
	m_iState = GAME_NEW;

	//reset rolls to zero
	::ZeroMemory(m_iRolls, sizeof(m_iRolls));

	//no colors used yet
	::ZeroMemory(m_bColors, sizeof(m_bColors));
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CGame::CGame(const CGame &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CGame::~CGame()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CGame &CGame::operator =(const CGame &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CGame::copy(const CGame &data)
{
	//server infro
	m_bServer			= data.m_bServer;

	//ladder
	m_bLadder			= data.m_bLadder;

	//team
	m_bTeam				= data.m_bTeam;

	//game mode
	m_iMode				= data.m_iMode;

	//system message display settings
	m_iSysNotify		= data.m_iSysNotify;

	//the date and time of this game
	m_timeStart			= data.m_timeStart;
	m_timeEnd			= data.m_timeEnd;

	//the reference time for a player's turn
	m_timeRef			= data.m_timeRef;
	
	//the unique ID of this game
	m_uiID				= data.m_uiID;

	//map being used
	m_uiMapID			= data.m_uiMapID;

	//the current game state
	m_iState			= data.m_iState;

	//whether S3D generated the board
	m_bGenerate			= data.m_bGenerate;

	//are we currently in game?
	m_bInGame			= data.m_bInGame;

	//max players allowed
	m_nMaxPlayers		= data.m_nMaxPlayers;

	//cutoff percent for drops/quits
	m_bUseQuitCutoff	= data.m_bUseQuitCutoff;
	m_nQuitCutoff		= data.m_nQuitCutoff;

	//who this player is in network mode
	m_iIndex			= data.m_iIndex;
	m_uiPlayerID		= data.m_uiPlayerID;

	//game winner
	m_iWinner			= data.m_iWinner;

	//bank resources
	memcpy(m_iBankRes, data.m_iBankRes, sizeof(m_iBankRes));

	//the record of this game
	m_listUndo			= data.m_listUndo;

	//the array of player games
	m_players			= data.m_players;

	//the list of dice rolls
	m_iRollList			= data.m_iRollList;

	//the array of rolls
	memcpy(m_iRolls, data.m_iRolls, sizeof(m_iRolls));

	//variants
	m_variants			= data.m_variants;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CGame::Serialize(CArchive &ar)
{
	//save information
	if(ar.IsStoring())
	{
		save(ar);
	}
	//load information
	else
	{
		load(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// load data
//////////////////////////////////////////////////////////////////////
void CGame::load(CArchive &ar)
{
	int i;
	int nDummy;
	int iSize;
	BOOL bPreMap = FALSE;

	//check file version
	if(DATABASE->gameVer() >= VER_NETWORK)
	{
		//server
		ar >> m_bServer;

		//ladder
		if(DATABASE->gameVer() >= VER_LADDER)
		{
			ar >> m_bLadder;
		}

		//team
		if(DATABASE->gameVer() >= VER_TEAM)
		{
			ar >> m_bTeam;
		}

		//who this player is
		ar >> m_uiPlayerID;

		//network index
		if(DATABASE->gameVer() >= VER_NETWORKa)
		{
			ar >> m_iIndex;
		}
	}

	//check file version
	if(DATABASE->gameVer() >= VER_GAMEMODE)
	{
		//game mode
		ar >> m_iMode;

		//resource display settings
		//ar >> m_iResDisplay;	//RETIRED in 0.9.3
		if(DATABASE->gameVer() < VER_AUTOHIDE)
		{
			ar >> nDummy;
		}
	}

	//system display settings
	if(DATABASE->gameVer() >= VER_NETWORKb)
	{
		//system message settings
		ar >> m_iSysNotify;
	}

	//the date and time of this game
	ar >> m_timeStart;
	ar >> m_timeEnd;

	//the unique ID of this game
	ar >> m_uiID;

	//the map id of the map used
	if(DATABASE->gameVer() >= VER_NETWORKa)
	{
		ar >> m_uiMapID;
	}
	else
	{
		//otherwise, set it dependent on number of players coming up
		bPreMap = TRUE;
	}

	//the current game state
	if(DATABASE->gameVer() >= VER_NETWORKc)
	{
		ar >> m_iState;
	}
	//otherwise, read it in as a complete game
	else
	{
		m_iState = GAME_COMPLETE;
	}

	//who won this game?
	if(DATABASE->gameVer() >= VER_AI)
	{
		ar >> m_iWinner;
	}

	//whether trading has been kept track of
	//ar >> m_bTrades;		//RETIRED IN 1.0.1
	if(DATABASE->gameVer() < VER_SEAFARERS)
	{
		ar >> nDummy;
	}

	//whether S3D should generate the board
	ar >> m_bGenerate;

	if(DATABASE->gameVer() >= VER_AI)
	{
		//helper for undo
		ar >> m_bInGame;
	}

	//get the number of players
	ar >> iSize;

	//set the map used if it's an old game file
	if(TRUE == bPreMap)
	{
		m_uiMapID = (iSize < 5 ? MAP_SETTLERS_34 : MAP_SETTLERS_56);
	}

	//resize the array
	m_players.resize(iSize);
	
	//the array of player games
	for(i = 0; i < iSize; i++)
	{
		m_players[i].Serialize(ar);
	}

	//bank resources
	if(DATABASE->gameVer() >= VER_NETWORKc)
	{
		ar.Read(m_iBankRes, sizeof(m_iBankRes));
	}
	else
	{
		//calc bank res from players?
	}

	//get the size of the roll list
	ar >> iSize;

	//resize the array
	m_iRollList.resize(iSize);

	//read in the roll list
	for(i = 0; i < iSize; i++)
	{
		ar >> m_iRollList[i];
	}

	//read the array of rolls
	ar.Read(m_iRolls, sizeof(m_iRolls));

	//read in variants
	if(DATABASE->gameVer() >= VER_VARIANTS)
	{
		m_variants.Serialize(ar);
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CGame::save(CArchive &ar)
{
	int i;
	int iSize;

	//server
	ar << m_bServer;

	//ladder
	ar << m_bLadder;

	//team
	ar << m_bTeam;

	//who this player is
	ar << m_uiPlayerID;
	ar << m_iIndex;

	//game mode
	ar << m_iMode;

	//resource display settings
	//ar << m_iResDisplay;		//RETIRED IN VER_AUTOHIDE (0.9.3)

	//system message settings
	ar << m_iSysNotify;

	//the date and time of this game
	ar << m_timeStart;
	ar << m_timeEnd;

	//the unique ID of this game
	ar << m_uiID;

	//the map used
	ar << m_uiMapID;

	//the game state
	ar << m_iState;

	//who's the winner
	ar << m_iWinner;

	//whether trading has been kept track of
	//ar << m_bTrades;			//RETIRED IN VER_SEAFARERS (1.0.1)

	//whether S3D should generate the board
	ar << m_bGenerate;

	//helper for undo
	ar << m_bInGame;

	//get the number of players
	iSize = m_players.size();

	//the array of player games
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		m_players[i].Serialize(ar);
	}

	//bank resources
	ar.Write(m_iBankRes, sizeof(m_iBankRes));

	//get the size of the roll list
	iSize = m_iRollList.size();

	//output the dice roll array
	ar << iSize;
	for(i = 0; i < iSize; i++)
	{
		ar << m_iRollList[i];
	}

	//array of rolls
	ar.Write(m_iRolls, sizeof(m_iRolls));

	//variants
	m_variants.Serialize(ar);
}

//////////////////////////////////////////////////////////////////////
// search for a particular player and return their name
//////////////////////////////////////////////////////////////////////
CString CGame::getNameByID(UINT uiID)
{
	int i;

	//look through players
	for(i = 0; i < (int) m_players.size(); i++)
	{
		if(uiID == m_players[i].m_player.getID())
		{
			return m_players[i].m_player.getName();
		}
	}

	//not found
	return "";
}

//////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////
void CGame::SerializeToMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 0;

	//ladder
	msg.SetAt(iStart++, m_bLadder);

	//team
	msg.SetAt(iStart++, m_bTeam);

	//game ID
	msg.SetAt(iStart++, m_uiID);

	//helper for undo
	msg.SetAt(iStart++, m_bInGame);

	//bank resources
	msg.SetAt(iStart++, m_iBankRes, sizeof(m_iBankRes));

	//dice rolls
	iSize = m_iRollList.size();
	msg.SetAt(iStart++, iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.SetAt(iStart + i, m_iRollList[i]);
	}
	iStart += iSize;

	//rolls by number
	msg.SetAt(iStart++, m_iRolls, sizeof(m_iRolls));

	//variants handled in network code
}

//////////////////////////////////////////////////////////////////
// read from a network message
//////////////////////////////////////////////////////////////////
void CGame::SerializeFromMessage(CNDKMessage &msg)
{
	int i;
	int iSize;
	int iStart = 0;

	//ladder
	msg.GetAt(iStart++, m_bLadder);

	//team
	msg.GetAt(iStart++, m_bTeam);

	//game ID
	msg.GetAt(iStart++, m_uiID);

	//helper for undo
	msg.GetAt(iStart++, m_bInGame);

	//bank resources
	msg.GetAt(iStart++, m_iBankRes, sizeof(m_iBankRes));

	//dice rolls
	msg.GetAt(iStart++, iSize);
	m_iRollList.resize(iSize);
	for(i = 0; i < iSize; i++)
	{
		msg.GetAt(iStart + i, m_iRollList[i]);
	}
	iStart += iSize;

	//rolls by number
	msg.GetAt(iStart++, m_iRolls, sizeof(m_iRolls));

	//variants handled in network code
}