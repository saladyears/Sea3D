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

#if !defined(AFX_NETSYSMESSAGE_H__513E584B_F049_4FE2_A4E0_D562C5D2F357__INCLUDED_)
#define AFX_NETSYSMESSAGE_H__513E584B_F049_4FE2_A4E0_D562C5D2F357__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////
// include files
//////////////////////////////////////////////////////////////////////
#include "NDKMessage.h"

//////////////////////////////////////////////////////////////////////
// class which handles a system message
//////////////////////////////////////////////////////////////////////
class CNetSysMessage : public CObject  
{
public:
	DECLARE_SERIAL(CNetSysMessage)

	//{{AFX_VIRTUAL(CNetSysMessage)
	public:
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

	CNetSysMessage();
	CNetSysMessage(const CNetSysMessage &data);
	virtual ~CNetSysMessage();

	//assignment operator
	CNetSysMessage &operator =(const CNetSysMessage &data);

	//compare operator
	BOOL operator ==(const CNetSysMessage &data);

	//streamline copy function
	void copy(const CNetSysMessage &data);

	//////////////////////////////////////////////////////////////////////
	// network serialize methods
	//////////////////////////////////////////////////////////////////////
	void SerializeToMessage(CNDKMessage &msg);
	void SerializeFromMessage(CNDKMessage &msg);

public:
	//////////////////////////////////////////////////////////////////
	// all the data
	//////////////////////////////////////////////////////////////////
	UINT m_uiMessage;
	CString m_strData;
	CString m_strData2;
	int m_iColor;
	
private:
	//////////////////////////////////////////////////////////////////////
	// serialize methods
	//////////////////////////////////////////////////////////////////////
	void save(CArchive &ar);
	void load(CArchive &ar);

};

#endif // !defined(AFX_NETSYSMESSAGE_H__513E584B_F049_4FE2_A4E0_D562C5D2F357__INCLUDED_)
