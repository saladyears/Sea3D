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

#if !defined(AFX_GAMEINFO_H__207DB5AA_93E4_47F9_8D78_80564D74BF0A__INCLUDED_)
#define AFX_GAMEINFO_H__207DB5AA_93E4_47F9_8D78_80564D74BF0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// class declarations
//////////////////////////////////////////////////////////////////////
class CGame;

//////////////////////////////////////////////////////////////////////
// class which holds game information in the open game dialog
//////////////////////////////////////////////////////////////////////
class CGameInfo  
{
public:
	CGameInfo(CGame *pGame = NULL);
	virtual ~CGameInfo();

	//assignment operator
	CGameInfo &operator =(const CGameInfo &data);

	//streamline copy function
	void copy(const CGameInfo &data);

	//game mode
	int m_iMode;

	//the time/date stamp of this game, beginning and end
	COleDateTime m_timeStart;
	COleDateTime m_timeEnd;

	//the number of players
	int m_iPlayers;

	//the player's names
	vector <CString> m_strNames;
};

#endif // !defined(AFX_GAMEINFO_H__207DB5AA_93E4_47F9_8D78_80564D74BF0A__INCLUDED_)
