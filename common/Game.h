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

#if !defined(AFX_GAME_H__298A4146_3706_4FDD_965B_D94C760E2219__INCLUDED_)
#define AFX_GAME_H__298A4146_3706_4FDD_965B_D94C760E2219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"
#include "Player.h"
#include "PlayerGame.h"
#include "UndoNode.h"
#include "Variants.h"

//////////////////////////////////////////////////////////////////////
// class which holds information about one game
//////////////////////////////////////////////////////////////////////
class CGame : public CObject  
{
public:
	DECLARE_SERIAL(CGame)

	//{{AFX_VIRTUAL(CGame)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CGame();
	CGame(const CGame &data);
	virtual ~CGame();

	//assignment operator
	CGame &operator =(const CGame &data);

	//streamline copy function
	void copy(const CGame &data);

	//returns a player name based on ID
	CString getNameByID(UINT uiID);

	//returns the bank res the user requested
	int getBankRes(int i) {return m_iBankRes[i];}

//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);
#endif

public:
	//////////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////////

	//server or not?
	BOOL m_bServer;

	//ladder game or not?
	BOOL m_bLadder;

	//team game or not
	BOOL m_bTeam;

	//game mode
	int m_iMode;

	//resource display settings
	//int m_iResDisplay;		//RETIRED IN VER_AUTOHIDE (0.9.3)

	//system message settings
	int m_iSysNotify;

	//the date and time of this game
	COleDateTime m_timeStart;
	COleDateTime m_timeEnd;

	//the reference time for a player's turn
	COleDateTime m_timeRef;

	//the unique ID of this game
	UINT m_uiID;

	//the unique ID of the map used in this game
	UINT m_uiMapID;

	//is this a new game or not? (not saved to file)
	//BOOL m_bNewGame;			//RETIRED IN VER_NETWORKc

	//the current game state
	int m_iState;

	//have we been initialized?
	BOOL m_bInit;

	//whether trading has been kept track of
	BOOL m_bTrades;

	//whether S3D should generate the board
	BOOL m_bGenerate;

	//are we currently in the game?
	BOOL m_bInGame;

	//the maximum amount of players allowed in this game
	int m_nMaxPlayers;

	//the cutoff percent for drops/quits
	BOOL m_bUseQuitCutoff;
	int m_nQuitCutoff;

	//in network mode, who this player is
	int m_iIndex;
	UINT m_uiPlayerID;

	//explicit winner variable for recording who wins the game (VER_AI)
	int m_iWinner;

	//the bank's resources
	int m_iBankRes[5];

	//the record of this game
	list <CUndoNode> m_listUndo;

	//the array of player games
	vector <CPlayerGame> m_players;

	//the list of dice rolls
	vector <int> m_iRollList;

	//rolls by number
	int m_iRolls[6][11];

	//colors used by players in this game
	BOOL m_bColors[6];

	//variant information
	CVariants m_variants;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

};

#endif // !defined(AFX_GAME_H__298A4146_3706_4FDD_965B_D94C760E2219__INCLUDED_)
