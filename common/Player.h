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

#if !defined(AFX_PLAYER_H__C847109F_8D8F_4E76_AC69_6AE3DF6C4F3A__INCLUDED_)
#define AFX_PLAYER_H__C847109F_8D8F_4E76_AC69_6AE3DF6C4F3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "define.h"

//conditional include
#ifndef AI_DLL
#include "NDKMessage.h"
#endif

//////////////////////////////////////////////////////////////////////
// class which holds information on one board piece
//////////////////////////////////////////////////////////////////////
class CPlayer : public CObject  
{
public:
	DECLARE_SERIAL(CPlayer)

	//{{AFX_VIRTUAL(CPlayer)
	public:
//conditional include
#ifndef AI_DLL
	virtual void Serialize(CArchive& ar);
#endif
	//}}AFX_VIRTUAL

	CPlayer();
	CPlayer(const CPlayer &data);
	virtual ~CPlayer();

	//assignment operator
	CPlayer &operator =(const CPlayer &data);

	//compare operator
	BOOL operator ==(const CPlayer &data);

	//streamline copy function
	void copy(const CPlayer &data);

public:
	//////////////////////////////////////////////////////////////////
	// public set functions
	//////////////////////////////////////////////////////////////////
	inline void setColor1(int i) {m_iColor1 = i;}
	inline void setColor2(int i) {m_iColor2 = i;}
	inline void setGameColor(int i) {m_iGameColor = i;}
	inline void setLanguage(int i) {m_iLanguage = i;}
	inline void setKnown(int i) {m_iKnown = i;}
	inline void setID(UINT ui) {m_uiId = ui;}
	inline void setAI(BOOL b) {m_bAI = b;}
	inline void setMadeHere(BOOL b) {m_bMadeHere = b;}
	inline void setHandle(BOOL b) {m_bHandle = b;}
	inline void setLadder(BOOL b) {m_bLadder = b;}
	inline void setReady(BOOL b) {m_bReady = b;}
	inline void setLadderID(CString s) {m_strLadderGUID = s;}
	inline void setFirstName(CString s) {m_strFirstName = s;}
	inline void setLastName(CString s) {m_strLastName = s;}
	inline void setIP(CString s) {if("" != s) m_strIP = s;}
	inline void setReport(CString s) {if("" != s) m_strPlayerReport = s;}

	//////////////////////////////////////////////////////////////////
	// public get functions
	//////////////////////////////////////////////////////////////////
	inline int getColor1(void) {return m_iColor1;}
	inline int getColor2(void) {return m_iColor2;}
	inline int getGameColor(void) {return m_iGameColor;}
	inline int getLanguage(void) {return m_iLanguage;}
	inline int getKnown(void) {return m_iKnown;}
	inline UINT getID(void) {return m_uiId;}
	inline BOOL getAI(void) {return m_bAI;}
	inline BOOL getHandle(void) {return m_bHandle;}
	inline BOOL getReady(void) {return m_bReady;}
	inline BOOL getMadeHere(void) {return m_bMadeHere;}
	inline BOOL getLadder(void) {return m_bLadder;}
	inline CString getLadderID(void) {return m_strLadderGUID;}
	inline CString getFirstName(void) {return m_strFirstName;}
	inline CString getLastName(void) {return m_strLastName;}
	inline CString getIP(void) {return m_strIP;}
	inline CString getReport(void) {return m_strPlayerReport;}
	
	CString getName(void);
	CString getCommaName(void);

//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////
	// network functions
	//////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);
#endif

private:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	//file version
	float m_fVer;

	//is this player an AI?
	BOOL m_bAI;

	//player name
	CString m_strFirstName;
	CString m_strLastName;

	//does this player go by a handle, not a full name?
	BOOL m_bHandle;

	//unique identifier
	UINT m_uiId;

	//is this player a ladder player?
	BOOL m_bLadder;

	//their ladder ID (a GUID on the web database)
	CString m_strLadderGUID;

	//their IP address (transitory, only used in connecting)
	CString m_strIP;

	//their ready state (transitory, only used in connecting)
	BOOL m_bReady;

	//created on this computer?
	BOOL m_bMadeHere;

	//preferred colors
	int m_iColor1;
	int m_iColor2;

	//game color
	int m_iGameColor;

	//preferred language
	int m_iLanguage;

	//known languages
	int m_iKnown;

	//this players report on rating, games played, drops, quits, etc.
	CString m_strPlayerReport;

private:
//conditional include
#ifndef AI_DLL
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);
#endif //AI_DLL
};

#endif // !defined(AFX_PLAYER_H__C847109F_8D8F_4E76_AC69_6AE3DF6C4F3A__INCLUDED_)
