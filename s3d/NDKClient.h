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
//    notation is used. PingServer is added. Close connection sends a message //
//    to the server.                                                          //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes                                                                   //
////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <afxsock.h>
#include "NDKDefines.h"

////////////////////////////////////////////////////////////////////////////////
// Forward declarations                                                       //
////////////////////////////////////////////////////////////////////////////////
class CNDKMessage;

////////////////////////////////////////////////////////////////////////////////
// Defines                                                                    //
////////////////////////////////////////////////////////////////////////////////

// Enumeration of all type of disconnection of the client
enum NDKClientDisconnection
{ 
	NDKClient_NormalDisconnection, 
	NDKClient_ServerCloseConnection,
    NDKClient_ServerStop, 
	NDKClient_ErrorSendingMessage, 
	NDKClient_ErrorReceivingMessage
};


////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// CNDKClient (abstract class)                                                //
//                                                                            //
// -------------------------------------------------------------------------- //
//                                                                            //
// This class implements the client side of the client/server architecture.   //
// The derived class must provide implementation for two pure virtual         //
// methods. These functions are OnMessage and OnDisconnect.                   //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

class NDK_EXT_CLASS CNDKClient
{
public:
	////////////////////////////////////////////////////////////////////////////
	// Constructors / Destructor                                              //
	////////////////////////////////////////////////////////////////////////////

	// Constructor.
	CNDKClient();

	// Destructor.
	virtual ~CNDKClient();

	////////////////////////////////////////////////////////////////////////////
	// Attributes                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Returns if the client is connected to the server.
	BOOL IsConnected() const;

	// Gets the IP address and port number of the client.
	BOOL GetIpAndPort(CString& strIp, long& lPort) const;

	////////////////////////////////////////////////////////////////////////////
	// Operations                                                             //
	////////////////////////////////////////////////////////////////////////////

	// Opens a connection to the server, given its IP address and the port 
	// number. The IP can be a machine name as "ftp.ndk.com", or a dotted number 
	// as "123.456.78.9".
	BOOL OpenConnection(const CString& strServerIp, long lPort);

	// Closes an established connection with the server and a message is sent to
	// the server. OnDisconnection callback will be call with the value
	// NDKNormalDisconnection.
	void CloseConnection();
	
	// Sends a message to the server. If a problem occurs, the connection will
	// be closed and OnDisconnect callback will be called with the value 
	// NDKErrorSendingMessage.
	BOOL SendMessageToServer(CNDKMessage& message);

	// Pings the server. OnPing callback will be called when the client
	// receives the response from the server. If a problem occurs, the 
	// connection will be closed and OnDisconnect callback will be called with 
	// the value NDKErrorSendingMessage. Overrides OnPing in the derived class 
	// if PingServer is used.
	BOOL PingServer();

protected:
	////////////////////////////////////////////////////////////////////////////
	// Callbacks                                                              //
	////////////////////////////////////////////////////////////////////////////

	// Called when a message is received. The derived class must override this
	// method.
	virtual void OnMessage(CNDKMessage& message) = 0;

	// Called whenever a disconnection occurs. The NDKDisconnectionType specify
	// how the disconnection occurred. CloseConnection don't need to be called 
	// when OnDisconnect is used. The derived class must override this method.
	virtual void OnDisconnect(NDKClientDisconnection disconnectionType) = 0;

	// Called when the ping from the server is received. The number of
	// milliseconds is returned since PingServer was called.
	virtual void OnPing(long lNbMilliseconds);

private:
	////////////////////////////////////////////////////////////////////////////
	// Private Operations                                                     //
	////////////////////////////////////////////////////////////////////////////

	// Processes pending read. If a problem occurs, the connection will
	// be closed and OnDisconnect callback will be called with the value 
	// NDKErrorReceivingMessage.
	void ProcessPendingRead(long lErrorCode);

	// Translates message and does the appropriate task for message handled by
	// the NDK.
	void TranslateMessage(CNDKMessage& message);

	// Closes the connection.
	void CloseConnection(NDKClientDisconnection disconnectType);

	// Cleans up.
	void CleanUp();

	////////////////////////////////////////////////////////////////////////////
	// Friendship                                                             //
	////////////////////////////////////////////////////////////////////////////

	friend class CNDKClientSocket;

	////////////////////////////////////////////////////////////////////////////
	// Disable Copy-Constructor and Assignment Operator                       //
	////////////////////////////////////////////////////////////////////////////
	
	CNDKClient(const CNDKClient&) {};
	void operator=(const CNDKClient &) {};
	
private:
	CNDKClientSocket* m_pClientSocket;
	CSocketFile*	  m_pFile;
	CArchive*		  m_pArchiveIn;
	CArchive*		  m_pArchiveOut;
};
