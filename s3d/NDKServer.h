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
#pragma once
#include <afxsock.h>
#include "NDKDefines.h"
#include "NDKUserManager.h"

////////////////////////////////////////////////////////////////////////////////
// Forward declarations                                                       //
////////////////////////////////////////////////////////////////////////////////
class CNDKMessage;
class CNDKServerSocket;

////////////////////////////////////////////////////////////////////////////////
// Defines                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Enumeration of all type of disconnection
enum NDKServerDisconnection
{ 
	NDKServer_NormalDisconnection, 
	NDKServer_ClientCloseConnection,
	NDKServer_ErrorSendingMessage, 
	NDKServer_ErrorReceivingMessage,
	NDKServer_InvalidPlayer,
	NDKServer_VersionMismatch,
	NDKServer_Dropped,
	NDKServer_GameFull,
	NDKServer_GameOver,
	NDKServer_LadderOnly,
	NDKServer_LadderSameIP,
	NDKServer_MaxPlayers,
	NDKServer_QuitCutoff,
	NDKServer_DoubleJoin
};

typedef CArray<long, long> CLongArray;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// CNDKServer (abstract class)                                                //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// This class implements the server side of the client/server architecture.   //
// The derived class must provide implementation for four pure virtual.       //
// methods. These functions are CanConnect, OnConnect, OnMessage and          //
// OnDisconnect. The server sends a message when it disconnects a client or   //
// when the server stops.                                                     //
//                                                                            //
// Each user connected to the server is assigned a unique Id (long).          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class NDK_EXT_CLASS CNDKServer
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors / Destructor                                              //
	////////////////////////////////////////////////////////////////////////////

	// Constructor.
	CNDKServer();

	// Destructor.
	virtual ~CNDKServer();

	////////////////////////////////////////////////////////////////////////////
	// Attributes                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Returns if the server is started.
	BOOL IsStarted() const;

	// Returns the port.
	long GetPort() const;

	// Returns the number of connected users.
	long GetNbUsers() const;

	// Returns the Ids of all connected users.
	void GetUserIds(CLongArray& alIds) const;

	////////////////////////////////////////////////////////////////////////////
	// Operations                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Starts listening on the specified port.
	BOOL StartListening(long lPort);

	// Stops the server. A message is sent to all users to inform them.
	void Stop();

	// Sends a message to a specified user. If a problem occurs, OnDisconnect
	// callback will be called.
	BOOL SendMessageToUser(long lUserId, CNDKMessage& message);

	// Sends a message to all users. OnDisconnect callback will be called for 
	// each user that the message cannot be sent.
	BOOL SendMessageToAllUsers(CNDKMessage& message);

	// Sends a message to some user specified by the array of user Id. 
	// OnDisconnect callback will be called for each user that the message 
	// cannot be sent.
	BOOL SendMessageToSomeUsers(const CLongArray& alUserIds, CNDKMessage& message);

	// Sends a message to all users except for the specified user Id.
	// OnDisconnect callback will be called for each user that the message 
	// cannot be sent.
	BOOL SendMessageToAllUsersExceptFor(long lUserId, CNDKMessage& message);

	// Sends a message to all users that aren't in the specified array of user 
	// Id. OnDisconnect callback will be called for each user that the message 
	// cannot be sent.
	BOOL SendMessageToAllUsersExceptFor(const CLongArray& alUserIds, 
										CNDKMessage& message);

	// Disconnects a specified user. OnDisconnect callback will not be called 
	// for user disconnected that way.
	BOOL DisconnectUser(long lUserId);

	// Disconnect a user.
	BOOL DisconnectUser(long lUserId, NDKServerDisconnection disconnectionType);

	// Disconnects all users. OnDisconnection callback will not be call.
	void DisconnectAllUsers();

	// Pings a user. OnPing callback will be called when the server receives
	// the response from the user. OnDisconnect callback might be called if
	// a problem occurs.
	BOOL PingUser(long lUserId);

	// Pings all users. OnPing callback will be called when the server
	// receives the response from each user. OnDisconnect callback might be 
	// called if a problem occurs.
	void PingAllUsers();

protected:
	////////////////////////////////////////////////////////////////////////////
	// Callbacks                                                              //
	////////////////////////////////////////////////////////////////////////////

	// Called when a user tries to connect to the server. Return TRUE to accept
	// the connection or FALSE otherwise. The derived class must override this
	// method.
	virtual BOOL OnIsConnectionAccepted() = 0;

	// Called when a user is connected to the server. The derived class must 
	// override this method.
	virtual void OnConnect(long lUserId) = 0;

	// Called whenever a message is received from a user. The derived class must 
	// override this method.
	virtual void OnMessage(long lUserId, CNDKMessage& message) = 0;

	// Called whenever a user is disconnected (the user might have closed 
	// the connection or an error occurred when sending a message, for example). 
	// DisconnectUser don't need to be called when OnDisconnect callback is 
	// used. The derived class must override this method.
	virtual void OnDisconnect(long lUserId, NDKServerDisconnection disconnectionType) = 0;

	// Called when the ping from the user is received. The number of
	// milliseconds is returned since PingUser was called.
	virtual void OnPing(long lUserId, long lNbMilliseconds);

private:
	////////////////////////////////////////////////////////////////////////////
	// Private Operations                                                     //
	////////////////////////////////////////////////////////////////////////////
	
	// Processes pending accept.
	void ProcessPendingAccept(long lErrorCode);

	// Processes pending read.
	void ProcessPendingRead(CNDKServerSocket* pServerSocket, long lErrorCode);

	// Translates message and does the appropriate task for message handled by
	// the NDK.
	void TranslateMessage(long lUserId, CNDKMessage& message);

	////////////////////////////////////////////////////////////////////////////
	// Friendship                                                             //
	////////////////////////////////////////////////////////////////////////////

	friend class CNDKServerSocket;

	////////////////////////////////////////////////////////////////////////////
	// Disable Copy-Constructor and Assignment Operator                       //
	////////////////////////////////////////////////////////////////////////////
	
	CNDKServer(const CNDKServer&) {};
	void operator=(const CNDKServer &) {};
		
private:
	long			  m_lPort;
	CNDKUserManager   m_userMgr;
	CNDKServerSocket* m_pListeningSocket;
};
