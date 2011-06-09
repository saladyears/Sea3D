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
#include "PlayerDatabase.h"
#include "GameDatabase.h"
#include "UndoDialog.h"
#include "GLView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// static instantiations
/////////////////////////////////////////////////////////////////////////////
CPlayerDatabase *			CPlayerDatabase::m_pPlayerDatabase;
CArray <CPlayer, CPlayer>	CPlayerDatabase::m_players;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlayerDatabase::CPlayerDatabase()
{

}

CPlayerDatabase::~CPlayerDatabase()
{

}

/////////////////////////////////////////////////////////////////////////////
// instance handling
/////////////////////////////////////////////////////////////////////////////
CPlayerDatabase *CPlayerDatabase::instance()
{
	//see if a game database already is open
	if(NULL == m_pPlayerDatabase)
	{
		//get a new audit handle
		m_pPlayerDatabase = new CPlayerDatabase();

		//get preliminary information
		open();
	}

	//return the instance
	return m_pPlayerDatabase;
}

/////////////////////////////////////////////////////////////////////////////
// release data
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::release()
{
	//delete the static instance if it exists
	if(NULL != m_pPlayerDatabase)
	{
		delete m_pPlayerDatabase;
		m_pPlayerDatabase = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////
// return the number of players in the database
/////////////////////////////////////////////////////////////////////////////
int CPlayerDatabase::numPlayers()
{
	return m_players.GetSize();
}

/////////////////////////////////////////////////////////////////////////////
// opens the database and gets information
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::open(void)
{
	int i, iSize;
	CFile file;
	BOOL bResave = FALSE;

	//reset the player array
	m_players.RemoveAll();

	//open the file
	if(!file.Open(FILE_PLAYER, CFile::modeRead)) return;

	//create the archive
	CArchive ar(&file, CArchive::load);

	//read the size
	ar >> iSize;

	//resize the array
	m_players.SetSize(iSize);

	//read the players
	for(i = 0; i < iSize; i++)
	{
		//read in the file
		m_players[i].Serialize(ar);
	}

	//close down everything
	ar.Close();
	file.Close();

	//CLEANUP IN 1.1.8

	//scan the list and remove any players that were not created here
	for(i = iSize - 1; i >= 0; i--)
	{
		if((FALSE == m_players[i].getMadeHere()) || (1 < m_players[i].getMadeHere()))
		{
			m_players.RemoveAt(i);
			bResave = TRUE;
		}
	}

	//if we have to resave, do so
	if(TRUE == bResave)
	{
		savePlayers();
	}
}

/////////////////////////////////////////////////////////////////////////////
// assign a pointer to a player
/////////////////////////////////////////////////////////////////////////////
CPlayer CPlayerDatabase::getPlayer(int iPlayer)
{
	//sanity check
	if(0 == m_players.GetSize())
	{
		//create an empty player to return so we don't crash
		return CPlayer();
	}

	//bounds check
	if(iPlayer < 0 || iPlayer >= m_players.GetSize())
	{
		//return the first player so things don't crash
		iPlayer = 0;
	}

	//set the pointer
	return m_players[iPlayer];
}

/////////////////////////////////////////////////////////////////////////////
// get a player by their unique ID number
/////////////////////////////////////////////////////////////////////////////
CPlayer CPlayerDatabase::getPlayerByID(UINT uiID)
{
	int i;
	int iSize;

	//get num players
	iSize = m_players.GetSize();

	//sanity check
	if(0 == iSize)
	{
		//create an empty player to return so we don't crash
		return CPlayer();
	}

	//search
	for(i = 0; i < iSize; i++)
	{
		if(uiID == m_players[i].getID())
		{
			return m_players[i];
		}
	}

	//not found!
	return CPlayer();
}

/////////////////////////////////////////////////////////////////////////////
// add a player to the database
/////////////////////////////////////////////////////////////////////////////
int CPlayerDatabase::addPlayer(CPlayer *pPlayer)
{
	int iIndex;

	//add this player to the array
	iIndex = m_players.Add(*pPlayer);

	//save the database
	savePlayers();

	//reload
	open();

	//return added index
	return iIndex;
}

/////////////////////////////////////////////////////////////////////////////
// delete a player from the database
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::deletePlayer(int iPlayer)
{
	//delete this player from the array
	m_players.RemoveAt(iPlayer);

	//save the file
	savePlayers();

	//reload
	open();
}

/////////////////////////////////////////////////////////////////////////////
// update a players name
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::updatePlayer(int iPlayer, CPlayer &player)
{
	m_players[iPlayer] = player;
}

/////////////////////////////////////////////////////////////////////////////
// save the players to the output file
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::savePlayers()
{
	int i;
	int iSize;
	CFile file;

	//open the file
	if(!file.Open(FILE_PLAYER, CFile::modeWrite | CFile::modeCreate)) return;

	//create the archive
	CArchive ar(&file, CArchive::store);

	//determine the size
	iSize = (int) m_players.GetSize();
	
	//write the size
	ar << iSize;

	//serialize the objects
	for(i = 0; i < iSize; i++)
	{
		m_players[i].Serialize(ar);
	}

	//close down
	ar.Close();
	file.Close();
}

/////////////////////////////////////////////////////////////////////////////
// update games in the game database with a new name
/////////////////////////////////////////////////////////////////////////////
void CPlayerDatabase::updateGames(int iPlayer)
{
	int i, j, iSize;
	int iGames = 0;
	CString str;
	CGame *pGame;
	CGLView *pGL;
	CUndoDialog *pUndo;
	CWaitCursor wc;

	//make sure the temp files are clear
	DATABASE->clearTemp();

	//find out the number of games
	iSize = DATABASE->numGames();

	//run through the games
	for(i = 0; i < iSize; i++)
	{
		//create a new data structures
		pGame = new CGame;
		pUndo = new CUndoDialog;
		pGL = new CGLView;

		//instantiate
		pUndo->Create(IDD_UNDO_DLG, NULL);
		pUndo->ShowWindow(SW_HIDE);

		//load the game uninitialzied
		DATABASE->loadGame(i, pGame, pUndo, pGL);

		//search for this player
		for(j = 0; j < (int) pGame->m_players.size(); j++)
		{
			if(pGame->m_players[j].m_player == m_players[iPlayer])
			{
				//set it
				pGame->m_players[j].m_player = m_players[iPlayer];

				//increment the number of games
				iGames++;

				//break from the loop
				break;
			}
		}

		//write to the temp file
		DATABASE->addGame(pGame, pUndo, pGL, FILE_TEMP, FILE_TEMPKEY);

		//delete the used data structures
		delete pGame;
		delete pGL;
		delete pUndo;
	}

	//copy the temp files to the regular files
	DATABASE->copyTemp();

	//give a message to the user
	if(iGames)
	{
		//create the string
		str.Format("%d %s %s updated to %s.", iGames, (iGames > 1) ? "games" : "game", (iGames > 1) ? "were" : "was", m_players[iPlayer].getName());
	}
	else
	{
		//create the string
		str.Format("%s was not in any games in the database.", m_players[iPlayer].getName());
	}

	//let the user know what happened
	MessageBox(NULL, str, "Update games", MB_OK);

	//null the pointers
	pGame = NULL;
	pGL = NULL;
	pUndo = NULL;
}