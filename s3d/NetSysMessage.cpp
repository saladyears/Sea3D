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
#include "Settlers.h"
#include "NetSysMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CNetSysMessage, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CNetSysMessage::CNetSysMessage()
{
	m_uiMessage = 0;
	m_iColor = -1;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CNetSysMessage::CNetSysMessage(const CNetSysMessage &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CNetSysMessage::~CNetSysMessage()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CNetSysMessage &CNetSysMessage::operator =(const CNetSysMessage &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CNetSysMessage::copy(const CNetSysMessage &data)
{
	//message
	m_uiMessage			= data.m_uiMessage;
	
	//data
	m_strData			= data.m_strData;
	m_strData2			= data.m_strData2;

	//color
	m_iColor				= data.m_iColor;
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CNetSysMessage::Serialize(CArchive &ar)
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
void CNetSysMessage::load(CArchive &ar)
{
	//message
	ar >> m_uiMessage;

	//data
	ar >> m_strData;
	ar >> m_strData2;

	//color
	ar >> m_iColor;
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CNetSysMessage::save(CArchive &ar)
{
	//message
	ar << m_uiMessage;

	//data
	ar << m_strData;
	ar << m_strData2;

	//color
	ar << m_iColor;
}

//////////////////////////////////////////////////////////////////////
// serialize to a message
//////////////////////////////////////////////////////////////////////
void CNetSysMessage::SerializeToMessage(CNDKMessage &msg)
{
	msg.SetAt(0, m_uiMessage);
	msg.SetAt(1, m_strData);
	msg.SetAt(2, m_strData2);
	msg.SetAt(3, m_iColor);
}

//////////////////////////////////////////////////////////////////////
// read data from a message
//////////////////////////////////////////////////////////////////////
void CNetSysMessage::SerializeFromMessage(CNDKMessage &msg)
{
	msg.GetAt(0, m_uiMessage);
	msg.GetAt(1, m_strData);
	msg.GetAt(2, m_strData2);
	msg.GetAt(3, m_iColor);
}

