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
#include "Player.h"
#include "defineNetwork.h"

//conditional include
#ifndef AI_DLL
#include "settlers.h"
#include "GameDatabase.h"
#endif //AI_DLL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CPlayer, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CPlayer::CPlayer()
{
	//file version
	m_fVer = FILE_VER;

	//is this player an AI?
	m_bAI = FALSE;

	//no handle yet
	m_bHandle = FALSE;

	//set the unique identifier based on time
	m_uiId = (UINT) time(NULL);

	//not ladder player yet
	m_bLadder = FALSE;

	//default to false
	m_bMadeHere = FALSE;

	//default ready state
	m_bReady = FALSE;

	//no preferred colors
	m_iColor1 = -1;
	m_iColor2 = -1;
	m_iGameColor = 0;

	//default language is english
	m_iLanguage = LANGUAGE_ENGLISH;
	m_iKnown = LANGUAGE_ENGLISH;

	//report information
	m_strPlayerReport = "No information available";
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CPlayer::CPlayer(const CPlayer &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CPlayer::~CPlayer()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CPlayer &CPlayer::operator =(const CPlayer &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// comparison operator
//////////////////////////////////////////////////////////////////////
BOOL CPlayer::operator ==(const CPlayer &data)
{
	return m_uiId == data.m_uiId;
}

//////////////////////////////////////////////////////////////////////
// return the player's name
//////////////////////////////////////////////////////////////////////
CString CPlayer::getName()
{
	CString str;

	//see if it's just a handle
	if(TRUE == m_bHandle)
	{
		str = m_strFirstName;
	}
	else
	{
		str = m_strFirstName + " " + m_strLastName;
	}

	//return the name
	return str;
}

//////////////////////////////////////////////////////////////////////
// return the player's name in comma form
//////////////////////////////////////////////////////////////////////
CString CPlayer::getCommaName() 
{
	CString str;

	if(TRUE == m_bHandle)
	{
		str = m_strFirstName;
	}
	else
	{
		str = m_strLastName + ", " + m_strFirstName;
	}

	//return the name
	return str;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CPlayer::copy(const CPlayer &data)
{
	//file version
	m_fVer						= data.m_fVer;

	//is this player an AI?
	m_bAI						= data.m_bAI;

	//player name
	m_strFirstName				= data.m_strFirstName;
	m_strLastName				= data.m_strLastName;

	//using a handle?
	m_bHandle					= data.m_bHandle;

	//identifier
	m_uiId						= data.m_uiId;

	//ladder player?
	m_bLadder					= data.m_bLadder;
	m_strLadderGUID				= data.m_strLadderGUID;

	//ip address
	m_strIP						= data.m_strIP;

	//ready state
	m_bReady					= data.m_bReady;

	//created on this computer?
	m_bMadeHere					= data.m_bMadeHere;

	//preferred colors
	m_iColor1					= data.m_iColor1;
	m_iColor2					= data.m_iColor2;
	m_iGameColor				= data.m_iGameColor;

	//preferred language
	m_iLanguage					= data.m_iLanguage;

	//known languages
	m_iKnown					= data.m_iKnown;

	//player report
	m_strPlayerReport			= data.m_strPlayerReport;
}

//conditional include
#ifndef AI_DLL
//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CPlayer::Serialize(CArchive &ar)
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
void CPlayer::load(CArchive &ar)
{
	BOOL bSkip = FALSE;;

	//do not read a file version if we're reading from a game file and
	//its version is older than VER_NETWORK
	if(DATABASE->isReading() && DATABASE->gameVer() < VER_NETWORK)
	{
		bSkip = TRUE;
	}
	else
	{
		ar >> m_fVer;
	}

	//AI
	if(m_fVer >= VER_AI)
	{
		ar >> m_bAI;
	}

	//player name
	ar >> m_strFirstName;
	ar >> m_strLastName;

	//using a handle?
	if(m_fVer >= VER_LADDER)
	{
		ar >> m_bHandle;
	}
	//make an intelligent guess
	else
	{
		m_bHandle = m_strLastName.IsEmpty();
	}

	//identifier
	ar >> m_uiId;

	//ladder player info
	if(m_fVer >= VER_LADDER)
	{
		//ladder status
		ar >> m_bLadder;

		//ladder ID
		ar >> m_strLadderGUID;
	}

	//read these in, only if we have a good version
	if(!bSkip && m_fVer >= VER_NETWORK)
	{
		//created on this computer?
		ar >> m_bMadeHere;

		//sanity check
		if(1 < m_bMadeHere) 
		{
			m_bMadeHere = 0;
		}

		//preferred colors
		ar >> m_iColor1;
		ar >> m_iColor2;

		//read game color
		if(m_fVer >= VER_NETWORKa)
		{
			ar >> m_iGameColor;
		}

		//preferred language
		ar >> m_iLanguage;

		//known languages
		ar >> m_iKnown;
	}
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CPlayer::save(CArchive &ar)
{
	//file version
	ar << FILE_VER;

	//AI
	ar << m_bAI;

	//player name
	ar << m_strFirstName;
	ar << m_strLastName;

	//handle
	ar << m_bHandle;

	//identifier
	ar << m_uiId;

	//ladder info
	ar << m_bLadder;

	//ladder id
	ar << m_strLadderGUID;

	//created on this computer?
	ar << m_bMadeHere;

	//preferred colors
	ar << m_iColor1;
	ar << m_iColor2;

	//game color
	ar << m_iGameColor;

	//preferred language
	ar << m_iLanguage;

	//known languages
	ar << m_iKnown;
}

//////////////////////////////////////////////////////////////////////
// write to a network message
//////////////////////////////////////////////////////////////////////
void CPlayer::SerializeToMessage(CNDKMessage &msg)
{
	int iStart = 1;

	//file version
	msg.SetAt(iStart++, m_fVer);

	//AI
	msg.SetAt(iStart++, m_bAI);

	//player name
	msg.SetAt(iStart++, m_strFirstName);
	msg.SetAt(iStart++, m_strLastName);

	//handle
	msg.SetAt(iStart++, m_bHandle);

	//identifier
	msg.SetAt(iStart++, m_uiId);

	//ladder status
	msg.SetAt(iStart++, m_bLadder);

	//ladder ID
	msg.SetAt(iStart++, m_strLadderGUID);

	//ip address
	msg.SetAt(iStart++, m_strIP);

	//ready state
	msg.SetAt(iStart++, m_bReady);

	//created on this computer -- always transmit as FALSE
	msg.SetAt(iStart++, FALSE);

	//preferred colors
	msg.SetAt(iStart++, m_iColor1);
	msg.SetAt(iStart++, m_iColor2);

	//game color
	msg.SetAt(iStart++, m_iGameColor);

	//preferred language
	msg.SetAt(iStart++, m_iLanguage);

	//known languages
	msg.SetAt(iStart++, m_iKnown);

	//player report
	msg.SetAt(iStart++, m_strPlayerReport);
}

//////////////////////////////////////////////////////////////////////
// read from a network message
//////////////////////////////////////////////////////////////////////
void CPlayer::SerializeFromMessage(CNDKMessage &msg)
{
	BOOL bDummy;
	int iStart = 1;

	//file version
	msg.GetAt(iStart++, m_fVer);

	//AI
	msg.GetAt(iStart++, m_bAI);

	//player name
	msg.GetAt(iStart++, m_strFirstName);
	msg.GetAt(iStart++, m_strLastName);

	//handle
	msg.GetAt(iStart++, m_bHandle);

	//identifier
	msg.GetAt(iStart++, m_uiId);

	//ladder status
	msg.GetAt(iStart++, m_bLadder);

	//ladder ID
	msg.GetAt(iStart++, m_strLadderGUID);

	//ip address
	msg.GetAt(iStart++, m_strIP);

	//ready state
	msg.GetAt(iStart++, m_bReady);

	//created on this computer?
	msg.GetAt(iStart++, bDummy);
	m_bMadeHere = FALSE;

	//preferred colors
	msg.GetAt(iStart++, m_iColor1);
	msg.GetAt(iStart++, m_iColor2);

	//game color
	msg.GetAt(iStart++, m_iGameColor);

	//preferred language
	msg.GetAt(iStart++, m_iLanguage);

	//known languages
	msg.GetAt(iStart++, m_iKnown);

	//player report
	msg.GetAt(iStart++, m_strPlayerReport);
}

#endif //AI_DLL