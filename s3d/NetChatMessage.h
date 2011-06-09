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

#if !defined(AFX_NETCHATMESSAGE_H__6E7C2974_B00B_4904_9BA6_C2765E4FB634__INCLUDED_)
#define AFX_NETCHATMESSAGE_H__6E7C2974_B00B_4904_9BA6_C2765E4FB634__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "NDKMessage.h"

//////////////////////////////////////////////////////////////////////
// class which handles a chat message
//////////////////////////////////////////////////////////////////////
class CNetChatMessage : public CObject  
{
public:
	DECLARE_SERIAL(CNetChatMessage)

	//{{AFX_VIRTUAL(CNetChatMessage)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CNetChatMessage();
	CNetChatMessage(const CNetChatMessage &data);
	virtual ~CNetChatMessage();

	//assignment operator
	CNetChatMessage &operator =(const CNetChatMessage &data);

	//compare operator
	BOOL operator ==(const CNetChatMessage &data);

	//streamline copy function
	void copy(const CNetChatMessage &data);

	//////////////////////////////////////////////////////////////////////
	// network serialize methods
	//////////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////

	int m_iColor;
	CString m_strName;
	CString m_strMessage;

private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

};

#endif // !defined(AFX_NETCHATMESSAGE_H__6E7C2974_B00B_4904_9BA6_C2765E4FB634__INCLUDED_)
