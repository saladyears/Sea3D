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
#include "NetChatMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(CNetChatMessage, CObject, 1)

//////////////////////////////////////////////////////////////////////
// constructor
//////////////////////////////////////////////////////////////////////
CNetChatMessage::CNetChatMessage()
{
	m_iColor = 0;
}

//////////////////////////////////////////////////////////////////////
// copy constructor
//////////////////////////////////////////////////////////////////////
CNetChatMessage::CNetChatMessage(const CNetChatMessage &data)
{
	copy(data);
}

//////////////////////////////////////////////////////////////////////
// destructor
//////////////////////////////////////////////////////////////////////
CNetChatMessage::~CNetChatMessage()
{

}

//////////////////////////////////////////////////////////////////////
// assignment operator
//////////////////////////////////////////////////////////////////////
CNetChatMessage &CNetChatMessage::operator =(const CNetChatMessage &data)
{
	copy(data);

	return *this;
}

//////////////////////////////////////////////////////////////////////
// streamline copy function
//////////////////////////////////////////////////////////////////////
void CNetChatMessage::copy(const CNetChatMessage &data)
{
	//color
	m_iColor			= data.m_iColor;

	//player name
	m_strName			= data.m_strName;

	//message
	m_strMessage		= data.m_strMessage;
	
}

//////////////////////////////////////////////////////////////////////
// save or load this puppy
//////////////////////////////////////////////////////////////////////
void CNetChatMessage::Serialize(CArchive &ar)
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
void CNetChatMessage::load(CArchive &ar)
{
	//color
	ar >> m_iColor;

	//name
	ar >> m_strName;

	//message
	ar >> m_strMessage;
}

//////////////////////////////////////////////////////////////////////
// save data
//////////////////////////////////////////////////////////////////////
void CNetChatMessage::save(CArchive &ar)
{
	//color
	ar << m_iColor;

	//name
	ar << m_strName;

	//message
	ar << m_strMessage;
}

//////////////////////////////////////////////////////////////////////
// serialize to a message
//////////////////////////////////////////////////////////////////////
void CNetChatMessage::SerializeToMessage(CNDKMessage &msg)
{
	msg.SetAt(0, m_iColor);
	msg.SetAt(1, m_strName);
	msg.SetAt(2, m_strMessage);
}

//////////////////////////////////////////////////////////////////////
// read data from a message
//////////////////////////////////////////////////////////////////////
void CNetChatMessage::SerializeFromMessage(CNDKMessage &msg)
{
	msg.GetAt(0, m_iColor);
	msg.GetAt(1, m_strName);
	msg.GetAt(2, m_strMessage);
}
