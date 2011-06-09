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

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// NDK 2.0 - Network Development Kit                                          //
//                                                                            //
// Authors: Sebastien Lachance                                                //
//                                                                            //
// E-mail:  netblitz@rocler.qc.ca                                             //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Permission to use, copy, modify, and distribute this software for any      //
// purpose and without fee is hereby granted. This is no guarantee about the  //
// use of this software. For any comments, bugs or thanks, please email us.   //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// Targeted Platform: Any Windows version                                     //
//                                                                            //
// Compiled with:     Visual C++ 6.0                                          //
//                                                                            //
// Last modification: January 2002                                            //
//                                                                            //
// History:                                                                   //
//                                                                            //
// 1- First release of this file.                                             //
// 2- The class is renamed and some optimizations are applied. Hungarian      //
//    notation is used.                                                       //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes                                                                   //
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "NDKUserManager.h"
#include "NDKServer.h"


////////////////////////////////////////////////////////////////////////////////
// Constructors / Destructor                                                  //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKUserManager::CNDKUserManager()
{
	m_lNextId = 0;
}


// Destructor.
CNDKUserManager::~CNDKUserManager()
{
	RemoveAllUsers();
}


////////////////////////////////////////////////////////////////////////////////
// Attributes                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Gets the number of users.
long CNDKUserManager::GetNbUsers() const
{
	return m_users.GetCount();
}


// Returns the Ids of all users.
void CNDKUserManager::GetUserIds(CLongArray& alIds) const
{
	alIds.RemoveAll();

	POSITION pos = m_users.GetHeadPosition();

	while (pos != NULL)
	{
    #if _MSC_VER >= 1300
		CNDKUser user = m_users.GetNext(pos);
    #else
		CNDKUser& user = m_users.GetNext(pos);
    #endif // _MSC_VER > 1300
		alIds.Add(user.GetId());
	}
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Adds a user then returns its new Id.
long CNDKUserManager::AddUser(CNDKServerSocket* pServerSocket)
{
	CNDKUser user(++m_lNextId, pServerSocket);

	m_users.AddTail(user);
	
	return m_lNextId;
}


// Removes a user.
BOOL CNDKUserManager::RemoveUser(long lUserId)
{
	BOOL bResult = FALSE;

	CNDKUser user = GetUserFromId(lUserId);

	if (user.GetId() > 0)
	{
		POSITION pos = m_users.Find(user);

		if (pos != NULL)
		{
			user.CloseSocket();
			m_users.RemoveAt(pos);
			bResult = TRUE;
		}
	}

	return bResult;
}


// Removes all users.
void CNDKUserManager::RemoveAllUsers()
{
	POSITION pos = m_users.GetHeadPosition();

	while (pos != NULL)
	{
		CNDKUser& user = m_users.GetNext(pos);
		user.CloseSocket();
	}

	m_users.RemoveAll();
}


// Sends a message to a specified user.
BOOL CNDKUserManager::SendMessage(long lUserId, CNDKMessage &msg)
{
	BOOL      bResult = FALSE;
	CNDKUser user = GetUserFromId(lUserId);

	if (user.GetId() > 0)
	{	
		bResult = user.SendMessage(msg);

		if (!bResult)
			RemoveUser(user.GetId());
	}

	return bResult;
}


// Sends a message to all users.
BOOL CNDKUserManager::SendMessageToAllUsers(CNDKMessage& message, CLongList& listIds)
{
	POSITION pos = m_users.GetHeadPosition();

	while (pos != NULL)
	{
		CNDKUser& user = m_users.GetNext(pos);
		
		if (!user.SendMessage(message))
			listIds.AddTail(user.GetId());
	}

	BOOL bResult = listIds.IsEmpty();

	pos = listIds.GetHeadPosition();
	
	while (pos != NULL)
		RemoveUser(listIds.GetNext(pos));

	return bResult;
}


// Sends a message to all users except for user specified in alUserIds.
BOOL CNDKUserManager::SendMessageToAllUsersExceptFor(const CLongArray& alUserIds, 
													 CNDKMessage& message, 
													 CLongList& listIds)
{
	POSITION pos = m_users.GetHeadPosition();

	while (pos != NULL)
	{
		CNDKUser& user = m_users.GetNext(pos);

		BOOL bFound = FALSE;

		for (long lUserIndex = 0; lUserIndex < alUserIds.GetSize(); lUserIndex++)
		{
			if (alUserIds[lUserIndex] == user.GetId())
			{
				bFound = TRUE;
				break;
			}
		}
		
		// Send the message only if the user isn't in the array of exception
		if (!bFound && !user.SendMessage(message))
			listIds.AddTail(user.GetId());
	}

	BOOL bResult = listIds.IsEmpty();

	pos = listIds.GetHeadPosition();
	
	while (pos != NULL)
		RemoveUser(listIds.GetNext(pos));

	return bResult;

}


// Processes pending read.
BOOL CNDKUserManager::ProcessPendingRead(CNDKServerSocket* pSocket, long lErrorCode, 
										  long& lUserId, CMessageList& messages)
{
	BOOL      bResult = FALSE;
	CNDKUser user = GetUserFromSocket(pSocket);

	lUserId = user.GetId();

	if ((lErrorCode == 0) && (lUserId > 0))
	{
		do
		{
			CNDKMessage message;
			bResult = user.ReadMessage(message);

			messages.AddTail(message);

			// m_users.Find(user) is needed to check if the user is still 
			// connected because if the OnMessage causes a disconnection of 
			// the user, we must stop the reading.
		} while (bResult && m_users.Find(user) && !user.IsSocketBufferEmpty());
	}

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
// Private Operations                                                         //
////////////////////////////////////////////////////////////////////////////////

// Gets a user from an Id.
CNDKUser CNDKUserManager::GetUserFromId(long lUserId) const
{
	CNDKUser user;
	POSITION pos = m_users.GetHeadPosition();
	
	while (pos != NULL)
	{
		user = m_users.GetNext(pos);

		if (user.GetId() == lUserId)
			pos = NULL;
	}

	return user;
}


// Gets a user from a socket.
CNDKUser CNDKUserManager::GetUserFromSocket(CNDKServerSocket* pServerSocket) const
{
	CNDKUser user;
	POSITION pos = m_users.GetHeadPosition();

	while (pos != NULL)
	{
		user = m_users.GetNext(pos);

		if (user.IsSocketEqual(pServerSocket))
			pos = NULL;
	}

	return user;
}
