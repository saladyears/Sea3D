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
#include "NDKServer.h"
#include "NDKDefines.h"
#include "NDKMessage.h"
#include "NDKServerSocket.h"


////////////////////////////////////////////////////////////////////////////////
// Constructors / Destructor                                                  //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKServer::CNDKServer()
{
	m_lPort = -1;
	m_pListeningSocket = NULL;
}


// Destructor.
CNDKServer::~CNDKServer()
{
	Stop();
}


////////////////////////////////////////////////////////////////////////////////
// Attributes                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Returns if the server is started.
BOOL CNDKServer::IsStarted() const
{
	return m_pListeningSocket != NULL;
}


// Returns the port.
long CNDKServer::GetPort() const
{
	return m_lPort;
}


// Returns the number of connected users.
long CNDKServer::GetNbUsers() const
{
	return m_userMgr.GetNbUsers();
}


// Returns the Ids of all connected users.
void CNDKServer::GetUserIds(CLongArray& alIds) const
{
	m_userMgr.GetUserIds(alIds);
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Starts listening on the specified port.
BOOL CNDKServer::StartListening(long lPort)
{
	// Don't call this method two times, call Stop before another
	// call to StartListening
	ASSERT(m_pListeningSocket == NULL); 

	BOOL bResult = FALSE;

	if ((m_pListeningSocket == NULL) && AfxSocketInit())
	{
		m_lPort = lPort;

		m_pListeningSocket = new CNDKServerSocket(this);

		if (m_pListeningSocket != NULL)
		{
			// Create the socket
			bResult = m_pListeningSocket->Create(m_lPort);

			// Start listening
			if (bResult)
				bResult = m_pListeningSocket->Listen();		
		}
	}

	// If an error occured destroy the socket
	if (!bResult && (m_pListeningSocket != NULL))
	{
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
	}

	return bResult;
}


// Stops the server. A message is sent to all users to inform them.
void CNDKServer::Stop()
{
	CNDKMessage msg(NDKServerClose);

	if (m_pListeningSocket)
		SendMessageToAllUsers(msg);

	DisconnectAllUsers();

	if (m_pListeningSocket != NULL)
	{
		delete m_pListeningSocket;
		m_pListeningSocket = NULL;
	}

	m_lPort = -1;
}


// Sends a message to a specified user. If a problem occurs, OnDisconnect
// callback will be called.
BOOL CNDKServer::SendMessageToUser(long lUserId, CNDKMessage &msg)
{
	BOOL bResult = m_userMgr.SendMessage(lUserId, msg);

	if (!bResult)
		OnDisconnect(lUserId, NDKServer_ErrorSendingMessage);
	
	return bResult;
}


// Sends a message to all users. OnDisconnect callback will be called for 
// each user that the message cannot be sent.
BOOL CNDKServer::SendMessageToAllUsers(CNDKMessage& message)
{
	CLongList listIds;

	BOOL bResult = m_userMgr.SendMessageToAllUsers(message, listIds);

	while (!listIds.IsEmpty())
		OnDisconnect(listIds.RemoveHead(), NDKServer_ErrorSendingMessage);

	return bResult;
}


// Sends a message to some user specified by the array of user Id. 
// OnDisconnect callback will be called for each user that the message 
// cannot be sent.
BOOL CNDKServer::SendMessageToSomeUsers(const CLongArray& alUserIds, 
										CNDKMessage& message)
{
	BOOL bResult = TRUE;

	for (long lUserIndex = 0; lUserIndex < alUserIds.GetSize(); lUserIndex++)
	{
		if (!SendMessageToUser(alUserIds[lUserIndex], message))
		{
			OnDisconnect(alUserIds[lUserIndex], NDKServer_ErrorSendingMessage);
			bResult = FALSE;
		}
	}

	return TRUE;
}


// Sends a message to all users except for the specified user Id.
// OnDisconnect callback will be called for each user that the message 
// cannot be sent.
BOOL CNDKServer::SendMessageToAllUsersExceptFor(long lUserId, CNDKMessage& message)
{
	CLongArray alUserIds;
	alUserIds.Add(lUserId);

	return SendMessageToAllUsersExceptFor(alUserIds, message);
}


// Sends a message to all users that aren't in the specified array of user 
// Id. OnDisconnect callback will be called for each user that the message 
// cannot be sent.
BOOL CNDKServer::SendMessageToAllUsersExceptFor(const CLongArray& alUserIds, 
												CNDKMessage& message)
{
	CLongList listIds;

	BOOL bResult = m_userMgr.SendMessageToAllUsersExceptFor(alUserIds, 
															message, 
															listIds);

	while (!listIds.IsEmpty())
		OnDisconnect(listIds.RemoveHead(), NDKServer_ErrorSendingMessage);

	return bResult;

}


// Disconnects a specified user. OnDisconnection callback will be call with the value
// NDKNormalDisconnection.
BOOL CNDKServer::DisconnectUser(long lUserId)
{
	return DisconnectUser(lUserId, NDKServer_NormalDisconnection);
}


// Disconnects all users. OnDisconnect callback will not be called for 
// users disconnected that way. 
void CNDKServer::DisconnectAllUsers()
{
	CNDKMessage msg(NDKServerDisconnectClient);

	SendMessageToAllUsers(msg);

	m_userMgr.RemoveAllUsers();
}


// Pings a user. OnPing callback will be called when the server receives
// the response from the user. OnDisconnect callback might be called if
// a problem occurs.
BOOL CNDKServer::PingUser(long lUserId)
{
	CNDKMessage message(NDKPingClient);
	message.Add((long)::GetTickCount());

	return SendMessageToUser(lUserId, message);
}


// Pings all users. OnPing callback will be called when the server
// receives the response from each user. OnDisconnect callback might be 
// called if a problem occurs.
void CNDKServer::PingAllUsers()
{
	CNDKMessage message(NDKPingClient);
	message.Add((long)::GetTickCount());

	SendMessageToAllUsers(message);
}


////////////////////////////////////////////////////////////////////////////////
// Protected Operations                                                       //
////////////////////////////////////////////////////////////////////////////////

// Called when the ping from the user is received. The number of
// milliseconds is returned since PingUser was called.
void CNDKServer::OnPing(long /*lUserId*/, long /*lNbMilliseconds*/)
{
}


////////////////////////////////////////////////////////////////////////////////
// Private Operations                                                         //
////////////////////////////////////////////////////////////////////////////////

// Processes pending accept.
void CNDKServer::ProcessPendingAccept(long lErrorCode)
{
	if (lErrorCode == 0)
	{
		CNDKServerSocket* pSocket = new CNDKServerSocket(this);

		if (pSocket != NULL)
		{
			BOOL bResult = FALSE;

			if (m_pListeningSocket->Accept(*pSocket))
			{
				// Ask to accept the new connection
				if (pSocket->Initialize() && OnIsConnectionAccepted())
				{
					OnConnect(m_userMgr.AddUser(pSocket));
					bResult = TRUE;
				}
			}

			if (!bResult)
				delete pSocket;
		}
	}
}


// Processes pending read.
void CNDKServer::ProcessPendingRead(CNDKServerSocket* pSocket, long lErrorCode)
{
	long         lUserId = 0;
	CMessageList messages;

	BOOL bResult = m_userMgr.ProcessPendingRead(pSocket, lErrorCode, lUserId, messages);

	if (bResult)
	{
		POSITION pos = messages.GetHeadPosition();

		while (pos != NULL)
			TranslateMessage(lUserId, messages.GetNext(pos));
		
		messages.RemoveAll();
	}
	else
	{
		DisconnectUser(lUserId, NDKServer_ErrorReceivingMessage);
	}
}


// Translates message and does the appropriate task for message handled by
// the NDK.
void CNDKServer::TranslateMessage(long lUserId, CNDKMessage& message)
{
	switch (message.GetId())
	{
	case NDKPingServer:
		SendMessageToUser(lUserId, message);
		break;

	case NDKPingClient:
		{
			long lNbMilliseconds = 0;
			message.GetAt(0, lNbMilliseconds);

			OnPing(lUserId, ::GetTickCount() - lNbMilliseconds);
		}
		break;

	case NDKClientDisconnect:
		DisconnectUser(lUserId, NDKServer_ClientCloseConnection);
		break;

	default:
		OnMessage(lUserId, message);
		break;
	}
}

// Disconnect a user.
BOOL CNDKServer::DisconnectUser(long lUserId, NDKServerDisconnection disconnectionType)
{
	CNDKMessage msg(NDKServerDisconnectClient);

	BOOL bResult = SendMessageToUser(lUserId, msg);
	
	if (bResult)
	{
		bResult = m_userMgr.RemoveUser(lUserId);

		OnDisconnect(lUserId, disconnectionType);
	}
	
	return bResult;
}
