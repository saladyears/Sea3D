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
#include "GameInfo.h"
#include "Game.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameInfo::CGameInfo(CGame *pGame)
{
	int i;

	//sanity check
	if(NULL == pGame)
	{
		return;
	}

	//set mode
	m_iMode = pGame->m_iMode;

	//assign some stuff
	m_timeStart = pGame->m_timeStart;
	m_timeEnd = pGame->m_timeEnd;

	//add the player number
	m_iPlayers = pGame->m_players.size();

	//add the names
	for(i = 0; i < m_iPlayers; i++)
	{
		//add the name
		m_strNames.push_back(pGame->m_players[i].m_player.getName());
	}
}

CGameInfo::~CGameInfo()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CGameInfo &CGameInfo::operator =(const CGameInfo &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CGameInfo::copy(const CGameInfo &data)
{
	//mode
	m_iMode				= data.m_iMode;

	//the time/date stamp of this game, beginning and end
	m_timeStart			= data.m_timeStart;
	m_timeEnd			= data.m_timeEnd;

	//the number of players
	m_iPlayers			= data.m_iPlayers;

	//the player's names
	m_strNames			= data.m_strNames;
}