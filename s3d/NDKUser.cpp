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
#include "NDKUser.h"
#include "NDKServerSocket.h"


////////////////////////////////////////////////////////////////////////////////
// Constructors / Destructor                                                  //
////////////////////////////////////////////////////////////////////////////////

// Constructor.
CNDKUser::CNDKUser()
{
	m_lId = 0;
	m_pServerSocket = NULL;
}


// Constructor with initialization.
CNDKUser::CNDKUser(long lId, CNDKServerSocket* pServerSocket)
{
	ASSERT((lId > 0) && (pServerSocket != NULL));

	m_lId = lId;
	m_pServerSocket = pServerSocket;
}


// Destructor.
CNDKUser::~CNDKUser()
{
}


////////////////////////////////////////////////////////////////////////////////
// Attributes                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Gets the Id.
long CNDKUser::GetId() const
{
	return m_lId;
}


// Returns if the socket is the same as specified.
BOOL CNDKUser::IsSocketEqual(CNDKServerSocket* pServerSocket) const
{
	return m_pServerSocket == pServerSocket;
}


// Returns if the buffer of the socket is empty.
BOOL CNDKUser::IsSocketBufferEmpty() const
{
	ASSERT(m_pServerSocket != NULL);

	BOOL bResult = TRUE;

	if (m_pServerSocket != NULL)
		bResult = m_pServerSocket->IsBufferEmpty();

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
// Operations                                                                 //
////////////////////////////////////////////////////////////////////////////////

// Sends a message.
BOOL CNDKUser::SendMessage(CNDKMessage& message)
{
	ASSERT(m_pServerSocket != NULL);

	BOOL bResult = FALSE;

	if (m_pServerSocket != NULL)
	{
		TRY
		{
			m_pServerSocket->SendMessage(message);
			bResult = TRUE;
		}
		CATCH(CFileException, e)
		{
			m_pServerSocket->Close();
		}
		CATCH(CArchiveException, e)
		{
			m_pServerSocket->Close();
		}
		END_CATCH
	}

	return bResult;
}


// Reads a message.
BOOL CNDKUser::ReadMessage(CNDKMessage& message)
{
	ASSERT(m_pServerSocket != NULL);

	BOOL bResult = FALSE;

	if (m_pServerSocket != NULL)
	{
		TRY
		{
			m_pServerSocket->ReceiveMessage(message);
			bResult = TRUE;
		}
		CATCH(CFileException, e)
		{
			m_pServerSocket->Close();
		}
		END_CATCH
	}

	return bResult;
}


// Closes the socket.
void CNDKUser::CloseSocket()
{
	if (m_pServerSocket != NULL)
	{
		m_pServerSocket->ShutDown();
		m_pServerSocket->Close();

		delete m_pServerSocket;
		m_pServerSocket = NULL;
	}
}


////////////////////////////////////////////////////////////////////////////////
// Operators                                                                  //
////////////////////////////////////////////////////////////////////////////////

// Comparison operator.
BOOL CNDKUser::operator==(const CNDKUser& user) const
{
	return (m_lId == user.m_lId) && (m_pServerSocket == user.m_pServerSocket);
}
