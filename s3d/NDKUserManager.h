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
#include "NDKMessage.h"
#include "NDKUser.h"

////////////////////////////////////////////////////////////////////////////////
// Forward declarations                                                       //
////////////////////////////////////////////////////////////////////////////////
class CNDKServerSocket;

////////////////////////////////////////////////////////////////////////////////
// Defines                                                                    //
////////////////////////////////////////////////////////////////////////////////
typedef CArray<long, long>                CLongArray;
typedef CList<CNDKUser, CNDKUser &>       CUserList;
typedef CList<long, long>                 CLongList;
typedef CList<CNDKMessage, CNDKMessage &> CMessageList;


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// CNDKUserManager (concrete class)                                           //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// This class manages all connected user to the user. It can add and remove   //
// user and send/receive message.                                             //
//                                                                            //                                                                            //
////////////////////////////////////////////////////////////////////////////////

class CNDKUserManager  
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors / Destructor                                              //
	////////////////////////////////////////////////////////////////////////////

	// Constructor.
	CNDKUserManager();

	// Destructor.
	virtual ~CNDKUserManager();

	////////////////////////////////////////////////////////////////////////////
	// Attributes                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Gets the number of users.
	long GetNbUsers() const;

	// Returns the Ids of all users.
	void GetUserIds(CLongArray& alIds) const;

	////////////////////////////////////////////////////////////////////////////
	// Operations                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Adds a user then returns its new Id.
	long AddUser(CNDKServerSocket* pServerSocket);

	// Removes a user.
	BOOL RemoveUser(long lUserId);

	// Removes all users.
	void RemoveAllUsers();
	
	// Sends a message to a specified user.
	BOOL SendMessage(long lUserId, CNDKMessage& message);

	// Sends a message to all users.
	BOOL SendMessageToAllUsers(CNDKMessage& message, CLongList& listIds);

	// Sends a message to all users except for user specified in alUserIds.
	BOOL SendMessageToAllUsersExceptFor(const CLongArray& alUserIds, 
										CNDKMessage& message, CLongList& listIds);

	// Processes pending read.
	BOOL ProcessPendingRead(CNDKServerSocket* pSocket, long lErrorCode, 
							long& lUserId, CMessageList& messages);
	
private:
	////////////////////////////////////////////////////////////////////////////
	// Private Operations                                                     //
	////////////////////////////////////////////////////////////////////////////

	// Gets a user from an Id.
	CNDKUser GetUserFromId(long lUserId) const;

	// Gets a user from a socket.
	CNDKUser GetUserFromSocket(CNDKServerSocket*) const;

private:
	CUserList m_users;
	long	  m_lNextId;
};
